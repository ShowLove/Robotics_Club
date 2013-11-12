#include "cvision/colorspace.h"
#include <cvcapture/cvimagecapture.h>

using namespace std;
using namespace Zebulon;
using namespace Vision;

#define HEIGHT 800
#define WIDTH 600
#define size (HEIGHT*WIDTH)

int main(int argc, char *argv[])
{
    string rawFolder = string(argv[1])+"/raw";
    string truFolder = string(argv[1])+"/tru";
    string colorspace = string(argv[2]);
    
    Video::CvImageCapture rawCapture;
    Video::CvImageCapture::Image rawSrc;
    rawCapture.Start(rawFolder.c_str(), 0, 0, false, false);
    
    Video::CvImageCapture truCapture;
    Video::CvImageCapture::Image truSrc;
    truCapture.Start(truFolder.c_str(), 0, 0, false, false);
    
    IplImage *rawImg = NULL;
    IplImage *truImg = NULL;
    IplImage *rawCSFull = NULL;
    IplImage *rawCSOne = NULL;
    IplImage *rawCSTwo = NULL;
    IplImage *rawCSThree = NULL;
    //IplImage *Faw = NULL;
    ColorSpace colorSpace;
    
    if(colorspace == "GRAY")
    {
        colorSpace.Setup(HEIGHT, WIDTH, 1);
    }
    else
    {
        colorSpace.Setup(HEIGHT, WIDTH, 3);
    }
    
    double* storageOneWhite = new double[size];
    double* storageTwoWhite = new double[size];
    double* storageThreeWhite = new double[size];
    double* storageOneBlack = new double[size];
    double* storageTwoBlack = new double[size];
    double* storageThreeBlack = new double[size];
    
    while(1)
    {
        rawCapture.GetFrame(&rawSrc);
        CvConvertToIplImage(rawSrc, &rawImg);
        
        truCapture.GetFrame(&truSrc);
        CvConvertToIplImage(truSrc, &truImg);
        
        rawCSFull = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 3);
        rawCSOne = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 1);
        rawCSTwo = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 1);
        rawCSThree = cvCreateImage(cvSize(HEIGHT, WIDTH), IPL_DEPTH_8U, 1);
        
        colorSpace.SetIndividualChannels(true, true, true);
        
        if(colorspace == "GRAY")
        {
            cout << "GRAY" << endl;
            rawCSFull = cvCloneImage(rawImg);
            colorSpace.Gray(rawImg);
            colorSpace.GetImage(rawCSOne, colorSpace.CHANNEL_ONE);
            colorSpace.GetImage(rawCSTwo, colorSpace.CHANNEL_TWO);
            colorSpace.GetImage(rawCSThree, colorSpace.CHANNEL_THREE);
        }
        else if(colorspace == "HSV")
        {
            cout << "HSV" << endl;
            colorSpace.HSV(rawImg);
        }
        else if(colorspace == "HLS")
        {
            cout << "HLS" << endl;
            colorSpace.HLS(rawImg);
        }
        else if(colorspace == "YCrCb")
        {
            cout << "YCrCb" << endl;
            colorSpace.YCrCb(rawImg);
        }
        else if(colorspace == "CIEXYZ")
        {
            cout << "CIEXYZ" << endl;
            colorSpace.CIEXYZ(rawImg);
        }
        else if(colorspace == "CIELuv")
        {
            cout << "CIELuv" << endl;
            colorSpace.CIELuv(rawImg);
        }
        else if(colorspace == "CIELab")
        {
            cout << "CIELab" << endl;
            colorSpace.CIELab(rawImg);
        }
        else if(colorspace == "RATIO")
        {
            cout << "RATIO" << endl;
            colorSpace.Ratio(rawImg);
        }
        
        if(colorspace != "GRAY")
        {
            colorSpace.GetImage(rawCSFull, colorSpace.CHANNEL_FULL);
            colorSpace.GetImage(rawCSOne, colorSpace.CHANNEL_ONE);
            colorSpace.GetImage(rawCSTwo, colorSpace.CHANNEL_TWO);
            colorSpace.GetImage(rawCSThree, colorSpace.CHANNEL_THREE);
        }
        
        unsigned char *imageDataRaw=(unsigned char *)rawImg->imageData;
        unsigned char *imageDataTru=(unsigned char *)truImg->imageData;
        unsigned char *imageDataFull=(unsigned char *)rawCSFull->imageData;
        unsigned char *imageDataOne=(unsigned char *)rawCSOne->imageData;
        unsigned char *imageDataTwo=(unsigned char *)rawCSTwo->imageData;
        unsigned char *imageDataThree=(unsigned char *)rawCSThree->imageData;
        int pos = 0;
        int posSingle = 0;
        int gray;
        
        // For finding average in white areas
        //int size = rawImg->height*rawImg->width;
        double whiteCount = 1e-6;
        int whiteIndex = 0;
        double aveOneWhite = 0;
        double aveTwoWhite = 0;
        double aveThreeWhite = 0;
        
        
        // For finding average in black areas
        double blackCount = 1e-6;
        int blackIndex = 0;
        double aveOneBlack = 0;
        double aveTwoBlack = 0;
        double aveThreeBlack = 0;
        
        

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
        }
        
        //cout << "whiteCount: " << whiteCount << endl;
        //cout << "blackCount: " << blackCount << endl;
        //cout << "posSingle: " << posSingle << endl;
        //cout << "aveOneWhite: " << aveOneWhite << endl;
        aveOneWhite /= whiteCount;
        //cout << "aveOneWhite: " << aveOneWhite << endl;
        aveTwoWhite /= whiteCount;
        aveThreeWhite /= whiteCount;
        
        double tempSDOneWhite = 0;
        double tempSDTwoWhite = 0;
        double tempSDThreeWhite = 0;
        
        for(int i = 0; i < whiteIndex; i++)
        {
            tempSDOneWhite += pow(storageOneWhite[i]-aveOneWhite,2);
            tempSDTwoWhite += pow(storageTwoWhite[i]-aveTwoWhite,2);
            tempSDThreeWhite += pow(storageThreeWhite[i]-aveThreeWhite,2);
        }
        
        tempSDOneWhite = sqrt(tempSDOneWhite/whiteCount);
        tempSDTwoWhite = sqrt(tempSDTwoWhite/whiteCount);
        tempSDThreeWhite = sqrt(tempSDThreeWhite/whiteCount);
        
        cout << "aveOneWhite: " << aveOneWhite << endl;
        cout << "tempSDOneWhite: " << tempSDOneWhite << endl;
        cout << endl;
        
        cout << "aveTwoWhite: " << aveTwoWhite << endl;
        cout << "tempSDTwoWhite: " << tempSDTwoWhite << endl;
        cout << endl;
        
        cout << "aveThreeWhite: " << aveThreeWhite << endl;
        cout << "tempSDThreeWhite: " << tempSDThreeWhite << endl;
        cout << endl;
        //cout << "totalTwo: " << totalTwo << endl;
        //cout << "totalThree: " << totalThree << endl;
        
        aveOneBlack /= blackCount;
        aveTwoBlack /= blackCount;
        aveThreeBlack /= blackCount;
        
        double tempSDOneBlack = 0;
        double tempSDTwoBlack = 0;
        double tempSDThreeBlack = 0;
        
        for(int i = 0; i < blackIndex; i++)
        {
            tempSDOneBlack += pow(storageOneBlack[i]-aveOneBlack,2);
            tempSDTwoBlack += pow(storageTwoBlack[i]-aveTwoBlack,2);
            tempSDThreeBlack += pow(storageThreeBlack[i]-aveThreeBlack,2);
        }
        
        tempSDOneBlack = sqrt(tempSDOneBlack/blackCount);
        tempSDTwoBlack = sqrt(tempSDTwoBlack/blackCount);
        tempSDThreeBlack = sqrt(tempSDThreeBlack/blackCount);
        
        cout << "aveOneBlack: " << aveOneBlack << endl;
        cout << "tempSDOneBlack: " << tempSDOneBlack << endl;
        cout << endl;
        
        cout << "aveTwoBlack: " << aveTwoBlack << endl;
        cout << "tempSDTwoBlack: " << tempSDTwoBlack << endl;
        cout << endl;
        
        cout << "aveThreeBlack: " << aveThreeBlack << endl;
        cout << "tempSDThreeBlack: " << tempSDThreeBlack << endl;
        cout << endl;
        
        
        cvShowImage("Raw", rawImg);
        cvShowImage("Tru", truImg);
        
        cvShowImage("Full", rawCSFull);
        cvShowImage("One", rawCSOne);
        cvShowImage("Two", rawCSTwo);
        cvShowImage("Three", rawCSThree);
        
    
        
        //cvRelease(&rawImg);
        //cvRelease(truImg);
        cvReleaseImage(&rawCSFull);
        cvReleaseImage(&rawCSOne);
        cvReleaseImage(&rawCSTwo);
        cvReleaseImage(&rawCSThree);
        
        cvWaitKey(50);
    }
    
    return 0;
}