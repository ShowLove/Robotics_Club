#include "cvision/colorspace.h"
#include "gvision/segment.h"
#include <cvcapture/cvimagecapture.h>
#include <vector>

using namespace std;
using namespace Zebulon;
using namespace Vision;

#define HEIGHT 800
#define WIDTH 600
#define size (HEIGHT*WIDTH)

int main(int argc, char *argv[])
{
    string rawFolder = string(argv[1]);
    
    Video::CvImageCapture rawCapture;
    Video::CvImageCapture::Image rawSrc;
    rawCapture.Start(rawFolder.c_str(), 0, 0, false, false);
    
    IplImage *rawImg = NULL;
    IplImage *cannyImg = NULL;
    IplImage *segImg = NULL;
    IplImage *aveImg = NULL;
    IplImage *finalImg = NULL;
    //IplImage *Faw = NULL;
    ColorSpace colorSpace;
    colorSpace.Setup(HEIGHT, WIDTH, 3);
    
    Vision::Segment *seg = NULL;
    seg=new Vision::Segment();
    
    Vision::Return ret;
    
    unsigned short *segmentMap;
    
    while(1)
    {
        rawCapture.GetFrame(&rawSrc);
        CvConvertToIplImage(rawSrc, &rawImg);
        
        cannyImg = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 3);
        aveImg = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 3);
        finalImg = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 3);
        //segImg = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 3);
        
        IplImage *imgCannyEdgeDetectGray = cvCreateImage(cvSize(rawImg->width,rawImg->height), IPL_DEPTH_8U, 1);
        cvCvtColor(rawImg, imgCannyEdgeDetectGray, CV_BGR2GRAY);
        
        //IplImage *out = cvCreateImage(cvGetSize(imgEqualizeGray), IPL_DEPTH_8U, 1);
        cvSmooth(imgCannyEdgeDetectGray, imgCannyEdgeDetectGray, CV_MEDIAN, 19); // 15 was interesting
        
        cvShowImage("Canny", imgCannyEdgeDetectGray);
        
        cvCanny(imgCannyEdgeDetectGray,imgCannyEdgeDetectGray,25,300,3); // 75, 300, 3
        cvDilate( imgCannyEdgeDetectGray, imgCannyEdgeDetectGray, 0, 3 ); // 3, 2
        cvErode( imgCannyEdgeDetectGray, imgCannyEdgeDetectGray, 0, 2 );
        
        cvCvtColor(imgCannyEdgeDetectGray, cannyImg, CV_GRAY2BGR);
        cvNot(cannyImg, cannyImg);
        
        
        seg->Setup(255, 0, 0, 50, 0);
        seg->Process(cannyImg,segImg);
        
        segmentMap = seg->GetSegMap();
        int segAmount = seg->GetNumSeg();
        cout << "SegAmount: " << segAmount << endl;
        
        //if(segAmount > 0)
        int bestSeg = 0;
        int currBestSegColorDiff = 1e6;
        int bestSegColorDiff = 1e6;
        
        unsigned char *rawImageData=(unsigned char *)rawImg->imageData;
        unsigned char *aveImageData=(unsigned char *)aveImg->imageData;
        memset(aveImageData,0,aveImg->imageSize);
        
        for(int s = 1; s <= segAmount; s++)
        {
            double aveColorRed = 0;
            double aveColorGreen = 0;
            double aveColorBlue = 0;
            double count = 0;
            vector< int > redColor;
            vector< int > greenColor;
            vector< int > blueColor;
            for(int n = 0; n < segImg->width*segImg->height; n++)
            {
                if(segmentMap[n] == s)
                {
                    redColor.push_back(rawImageData[3*n+2]);
                    greenColor.push_back(rawImageData[3*n+1]);
                    blueColor.push_back(rawImageData[3*n+0]);
                    
                    if(!(rawImageData[3*n+2] > 230 && rawImageData[3*n+1] > 230 && rawImageData[3*n+0] > 230) && 
                        !(rawImageData[3*n+2] < 30 && rawImageData[3*n+1] < 30 && rawImageData[3*n+0] < 30))
                    {
                        aveColorRed += rawImageData[3*n+2];
                        aveColorGreen += rawImageData[3*n+1];
                        aveColorBlue += rawImageData[3*n+0];
                        count++;
                    }
                }
            }
            
            if(count == 0)
            {
                aveColorRed = 0;
                aveColorGreen = 0;
                aveColorBlue = 0;
            }
            else
            {
                aveColorRed /= count;
                aveColorGreen /= count;
                aveColorBlue /= count;
            }
            
            // sort for median
            sort(redColor.begin(), redColor.end());
            sort(greenColor.begin(), greenColor.end());
            sort(blueColor.begin(), blueColor.end());
            //greenColor.push_back(rawImageData[3*n+1]);
            //blueColor.push_back(rawImageData[3*n+0]);
            
            for(int n = 0; n < segImg->width*segImg->height; n++)
            {
                if(segmentMap[n] == s)
                {
                    // median
                    int index;
                    index = count/2;
                    aveImageData[3*n+2] = redColor[index];
                    aveImageData[3*n+1] = greenColor[index];
                    aveImageData[3*n+0] = blueColor[index];
                    
                    // ave
                    /*aveImageData[3*n+2] = aveColorRed;
                    aveImageData[3*n+1] = aveColorGreen;
                    aveImageData[3*n+0] = aveColorBlue;*/
                }
            }
            
            currBestSegColorDiff = sqrt(pow(255 - aveColorRed, 2) + pow(0 - aveColorGreen, 2) + pow(0 - aveColorBlue, 2)); 
            if(currBestSegColorDiff < bestSegColorDiff)
            {
                bestSegColorDiff = currBestSegColorDiff;
                bestSeg = s;
            }
            
        }
        
        seg->GetMaxPosition(ret, bestSeg-1);
        
        cvCircle(aveImg, cvPoint(ret.mCenterI, ret.mCenterJ), 5, cvScalar(255, 0, 255), 5, 8);
        
        
        /*for(int s = 1; s <= segAmount; s++)
        {
            
        }*/
        
        
        
        
        cvShowImage("Raw", rawImg);
        //cvShowImage("Canny", cannyImg);
        cvShowImage("Seg", segImg);
        cvShowImage("Ave", aveImg);
        
        //colorSpace.SetIndividualChannels(true, true, true);
        
        
        /*unsigned char *imageDataRaw=(unsigned char *)rawImg->imageData;
        unsigned char *imageDataTru=(unsigned char *)truImg->imageData;
        int pos = 0;
        int posSingle = 0;
        int gray;

        for(int j=0;j<rawImg->height;j++)
        {
            for(int i=0;i<rawImg->width;i++)
            {
                pos=j*rawImg->widthStep+3*i;
                
                if(imageDataTru[pos] == 255)
                {
                    //cout << "255" << endl;
                    aveOneWhite += imageDataOne[posSingle];
                    //cout << "imageDataOne[posSingle]" << (int)imageDataOne[posSingle] << endl;
                    storageOneWhite[whiteIndex] = imageDataOne[posSingle];
                    
                    aveTwoWhite += imageDataTwo[posSingle];
                    storageTwoWhite[whiteIndex] = imageDataTwo[posSingle];
                    
                    aveThreeWhite += imageDataThree[posSingle];
                    storageThreeWhite[whiteIndex] = imageDataThree[posSingle];
                    
                    whiteCount++;
                    whiteIndex++;
                }
                else if(imageDataTru[pos] == 0)
                {
                    aveOneBlack += imageDataOne[posSingle];
                    storageOneBlack[blackIndex] = imageDataOne[posSingle];
                    
                    aveTwoBlack += imageDataTwo[posSingle];
                    storageTwoBlack[blackIndex] = imageDataTwo[posSingle];
                    
                    aveThreeBlack += imageDataThree[posSingle];
                    storageThreeBlack[blackIndex] = imageDataThree[posSingle];
                    
                    blackCount++;
                    blackIndex++;
                }
                
                posSingle++;
            }
        }*/
        
        /*cvShowImage("Full", aveImg);
        cvShowImage("One", finalImg);*/
        
    
        
        //cvRelease(&rawImg);
        //cvRelease(truImg);
        cvReleaseImage(&cannyImg);
        cvReleaseImage(&imgCannyEdgeDetectGray);
        cvReleaseImage(&aveImg);
        //cvReleaseImage(&segImg);
        cvReleaseImage(&finalImg);
        
        cvWaitKey(100);
    }
    
    return 0;
}