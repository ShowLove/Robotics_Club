#include <cvcapture/cvimagecapture.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include <cxutils/mutex.h>

#ifdef WIN32
//#include "vld.h"
#endif

#include <stdio.h>
#include <vector>
#include <string>
#include <time.h>

using namespace std;
using namespace Zebulon;

#include "gvision/target.h"
#include "gvision/return.h"

#include "gvision/threshratio.h"
#include "gvision/segment.h"

#include "cvision/colorspace.h"

#include <fw/fw1394Camera.h>

//#define GRAY_CS
//#define HSV_CS
//#define HLS_CS
//#define YCrCb_CS
//#define CIEXYZ_CS
//#define CIELuv_CS
//#define CIELab_CS
//#define RATIO_CS

#define HISTO_EQUAL

//#define BUOYS
#define PATH
//#define NEW_BUOYS

#define WIDTH 800
#define HEIGHT 600
#define WAIT_TIME 2


int main(int argc, char *argv[])
{
    IplImage *img = NULL;
    img = cvCreateImage(cvSize(WIDTH,HEIGHT),IPL_DEPTH_8U,3); //cvLoadImage("realtestimg.png", CV_LOAD_IMAGE_COLOR);
    IplImage *outputFull = NULL;
    IplImage *outputIndividual = NULL;
    int fileNum = 0;
    char fileTrainingRaw[256];
    
    
    //CvSeq* contours;
    //CvMemStorage* storage = cvCreateMemStorage(0);
    //IplImage *dst = NULL;
    //IplImage *contourImage = NULL;
    
    //string imageFolder = string(argv[1]);
    
    //cout << "imageFolder: " << imageFolder << endl;
    //string rawFolder = string(argv[1]);
    Video::CvImageCapture capture;
    Video::CvImageCapture::Image captureSrc;
    //cout << "argv[1]: " << argv[1] << endl;
    capture.Start(argv[1], WIDTH, HEIGHT, false, false);
    
    Vision::ColorSpace colorSpace;
    
    //#ifdef GRAY_CS
        /*cout << "GRAY" << endl;
        colorSpace.Setup(WIDTH, HEIGHT, 1);
        outputFull = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
        outputIndividual = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
        dst = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
        contourImage = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);*/
        cout << "NOT GRAY" << endl;
        colorSpace.Setup(WIDTH, HEIGHT, 3);
        outputFull = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 3);
        outputIndividual = cvCreateImage(cvSize(WIDTH, HEIGHT), IPL_DEPTH_8U, 1);
    
    
    cvNamedWindow("Original", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Full", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("CH 1", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("CH 2", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("CH 3", CV_WINDOW_AUTOSIZE);
    
    //cvNamedWindow("Canny", CV_WINDOW_AUTOSIZE);
    vector< vector<int> > mixChannels;
    
    for(int i = 0; i < 8; i++)
    {
        vector<int> tempVector;
        for(int j = 0; j < 3; j++)
        {
            tempVector.push_back(0);
        }
        
        mixChannels.push_back(tempVector);
    }
    
    //colorSpace.SetMix(3);
    
    while(1)
    {
        
        capture.GetFrame(&captureSrc);
        CvConvertToIplImage(captureSrc, &img);
        
    /*#ifdef STATIC
        img = cvLoadImage("path.png", CV_LOAD_IMAGE_COLOR);
    #endif
        
    #ifdef BUOYS
        sprintf(fileTrainingRaw, "buoys/FWCamera/%06d.png", fileNum);
        fileNum++;
        if(fileNum > 546)
        {
            fileNum = 0;
        }
    #endif
    #ifdef PATH
        sprintf(fileTrainingRaw, "pathlong/%06d.png", fileNum);
        fileNum++;
        if(fileNum >= 304)
        {
            fileNum = 0;
        }
    #endif
    #ifdef NEW_BUOYS
        sprintf(fileTrainingRaw, "buoys/new_cam_buoys/%06d.png", fileNum);
        fileNum++;
        if(fileNum > 225)
        {
            fileNum = 0;
        }
        
    #endif
        //cout << fileNum << endl;
        cout << fileTrainingRaw << endl;
        //img = cvLoadImage("realtestimg.png", CV_LOAD_IMAGE_COLOR);
        img = cvLoadImage(fileTrainingRaw, CV_LOAD_IMAGE_COLOR);
        if(!img)
        {
            cout << "Error Loading Image: Raw Image" << endl;
            return 0;
        }*/
        
        //cout << "height: " << img->height << endl;
        //cout << "width: " << img->width << endl;

    
    
#if 1
    /*mixChannels[colorSpace.HSV_CS][0] = 1;
    mixChannels[colorSpace.HSV_CS][1] = 1;
    mixChannels[colorSpace.HSV_CS][1] = 1;*/
    // seems good for buoys, red and yellow
    /*mixChannels[colorSpace.YCrCb_CS][1] = 1;
    mixChannels[colorSpace.YCrCb_CS][2] = 1;
    mixChannels[colorSpace.Ratio_CS][1] = 1;*/
    // seems good for red, close to far, same with Lab
    /*mixChannels[colorSpace.CIELuv_CS][1] = 1;
    mixChannels[colorSpace.CIELuv_CS][2] = 1;
    mixChannels[colorSpace.Ratio_CS][1] = 1;*/
    // good for all buoys
    /*mixChannels[colorSpace.CIELab_CS][2] = 1;
    mixChannels[colorSpace.CIELuv_CS][1] = 1;
    mixChannels[colorSpace.CIELab_CS][1] = 1;*/
    // Green Buoys
    //mixChannels[colorSpace.HSV_CS][0] = 1;
    // Like for buoys
    /*mixChannels[colorSpace.CIELab_CS][2] = 1;
    mixChannels[colorSpace.YCrCb_CS][2] = 1;
    mixChannels[colorSpace.CIELab_CS][1] = 1;*/
    // Good Red
    mixChannels[colorSpace.Ratio_CS][0] = 1;
    mixChannels[colorSpace.Ratio_CS][1] = 1;
    mixChannels[colorSpace.Ratio_CS][2] = 1;
    
    colorSpace.Mix(img, mixChannels);
    
    cvShowImage("Original", img);
        
    colorSpace.GetImage(outputFull, colorSpace.CHANNEL_FULL);
    //cvSmooth(outputFull, outputFull, CV_MEDIAN, 9);
    /*cvSmooth( const CvArr* src, CvArr* dst,
               int smoothtype=CV_GAUSSIAN,
               int param1=3, int param2=0, double param3=0, double param4=0 )*/
    cvShowImage("Full", outputFull);
    
    //vector<IplImage*> combineImages;
    
    colorSpace.GetImage(outputIndividual, colorSpace.MIX_CHANNEL_THREE);
    cvShowImage("CH 3", outputIndividual);
    
    //combineImages.push_back(outputIndividual);
    
    colorSpace.GetImage(outputIndividual, colorSpace.MIX_CHANNEL_TWO);
    cvShowImage("CH 2", outputIndividual);
    
    //combineImages.push_back(outputIndividual);
    
    colorSpace.GetImage(outputIndividual, colorSpace.MIX_CHANNEL_ONE);
    cvShowImage("CH 1", outputIndividual);
    
    
#endif
    
    
    
    
#if 0

        // want to calculate channel one, two, three, invert one, invert two, invert three, invert full
        colorSpace.SetIndividualChannels(true, true, true, false, false, false, false);
        // full, one, two, three
        colorSpace.SetIndividualChannelHisto(false, false, false, false);
        //Vision::ColorSpace colorSpace;
        //cout << "SIZE OF COLORSPACE: " << colorSpace.mImageVector.size() << endl;
        
    #ifdef GRAY_CS
        colorSpace.Gray(img);
    #endif

    #ifdef HSV_CS
        colorSpace.HSV(img);
    #endif

    #ifdef HLS_CS
        colorSpace.HLS(img);
    #endif

    #ifdef YCrCb_CS
        colorSpace.YCrCb(img);
    #endif

    #ifdef RATIO_CS
        colorSpace.Ratio(img);
    #endif

    #ifdef CIEXYZ_CS
        colorSpace.CIEXYZ(img);
    #endif

    #ifdef CIELuv_CS
        colorSpace.CIELuv(img);
    #endif

    #ifdef CIELab_CS
        colorSpace.CIELab(img);
    #endif
    
    #ifdef HISTO_EQUAL
        
        colorSpace.HistogramEqualization();
    #endif
    

    
        cvShowImage("Original", img);
        
        colorSpace.GetImage(outputFull, colorSpace.CHANNEL_FULL);
        cvShowImage("Full", outputFull);
        
        //vector<IplImage*> combineImages;
        
        colorSpace.GetImage(outputIndividual, colorSpace.CHANNEL_THREE);
        cvShowImage("CH 3", outputIndividual);
        
        //combineImages.push_back(outputIndividual);
        
        colorSpace.GetImage(outputIndividual, colorSpace.CHANNEL_TWO);
        cvShowImage("CH 2", outputIndividual);
        
        //combineImages.push_back(outputIndividual);
        
        colorSpace.GetImage(outputIndividual, colorSpace.CHANNEL_ONE);
        cvShowImage("CH 1", outputIndividual);
        
#endif
        
        //combineImages.push_back(outputIndividual);
        
        //colorSpace.Combine(combineImages, outputIndividual);
        
        //cvShowImage("Combine", outputIndividual);
        
        /*colorSpace.GetImage(outputIndividual, colorSpace.CHANNEL_FULL);
        cvSmooth(outputIndividual, outputIndividual, CV_GAUSSIAN, 9, 9);
        cvShowImage("Smooth", outputIndividual);
        cvCanny(outputIndividual, outputIndividual, 50, 100, 3);
        //cvSobel(outGreen,imgSobelGray16, 1, 0, 3);
        cvShowImage("Canny", outputIndividual);*/
        
        /*cvFindContours( outputIndividual, storage, &contours, sizeof(CvContour), CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0) );
        cvZero( dst );
        cvZero( contourImage );
        
        for( ; contours != 0; contours = contours->h_next )
        {
            CvScalar color = CV_RGB( rand()&255, rand()&255, rand()&255 );
            // replace CV_FILLED with 1 to see the outlines 
            cvDrawContours( contourImage, contours, color, color, -1, CV_FILLED, 8 );
        }
        cvShowImage("Contours", contourImage);*/
                     
        
        /*void cvDrawContours( CvArr *image, CvSeq* contour,
                     double external_color, double hole_color,
                     int max_level, int thickness=1,
                     int connectivity=8 );*/
        /*int cvFindContours( CvArr* img, CvMemStorage* storage, CvSeq** firstContour,
                    int headerSize=sizeof(CvContour), CvContourRetrievalMode mode=CV_RETR_LIST,
                    CvChainApproxMethod method=CV_CHAIN_APPROX_SIMPLE );*/
        
        
        
        //cvWaitKey(10000);
        
    
        
        /*cvShowImage("Original", img);
        cvShowImage("Full", colorSpace.mImageVector[0]);
        cvShowImage("Red", colorSpace.mImageVector[3]);
        cvShowImage("Green", colorSpace.mImageVector[2]);
        cvShowImage("Blue", colorSpace.mImageVector[1]);*/
        
        //cvWaitKey(10000);
        

    
        cvWaitKey(WAIT_TIME);
        
        cvReleaseImage(&img);
        //cvReleaseImage(&outputFull);
        //cvReleaseImage(&outputIndividual);
    
    }

}

