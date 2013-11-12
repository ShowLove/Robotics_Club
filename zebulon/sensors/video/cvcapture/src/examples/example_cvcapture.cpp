#include "cvcapture/cvimagecapture.h"
#include <cxutils/keyboard.h>
#include <cxutils/images/jpeg/jpeg.h>

#include <vector>
#include <string>
#include <time.h>
#include <stdio.h>
#include <sstream>
#include <iostream>

#define TEST_CALLBACK
//#define CV_USE_OPEN_CV

#ifdef CV_USE_OPEN_CV
#include <opencv/cv.h>
#include <opencv/highgui.h>
#endif

//using namespace std;
using namespace Zebulon;
using namespace Video;

unsigned int gCount = 0;
unsigned int gStartTime = 0;

#ifdef CV_USE_OPEN_CV
IplImage *gDisplayImage=NULL;
#endif

bool SaveToJPEG(const char *fileName, const unsigned char *buffer, const int size);

class TestCallback : public CvImageCapture::Callback
{
    virtual void ProcessCameraData(CvImageCapture::Image* dest)
    {
        std::cout << "Callback\n";
        if( gCount == 0 )
        {
            gStartTime = (unsigned int)(time(NULL));
        }
        std::cout << "Captured frame "
                  << ++gCount << " at ["
                  << dest->mWidth << " x "
                  << dest->mHeight << "] - "
                  << ( (( time(NULL) - gStartTime ) > 0 ) ? (double)gCount/(time(NULL) - gStartTime) : 0 )
                  << std::endl;
        std::stringstream str;
        str << "CapturedFrame" << gCount << ".jpg";

#ifdef CV_USE_OPEN_CV
        CvConvertToIplImage(*dest, &gDisplayImage);
        cvShowImage("Display",gDisplayImage);
        cvWaitKey(10);
#endif
        //CxUtils::Image copy;
        //copy.Create(dest->mHeight, dest->mWidth, 3, dest->mpData);
        //copy.Save(str.str());
    }
};

int main(int argc, char *argv[])
{
    CvImageCapture capture;
    CvImageCapture::Image src, rawSrc;
    TestCallback testCallback;
    std::vector<std::string> sources;

#ifdef TEST_CALLBACK
    // Callbacks must be registered before capture gStartTime.
    capture.RegisterCallback(&testCallback);
#endif

    capture.SetID(0);
    std::cout << "Press <enter> key to exit. Press the <c> key to save picture\n";
    std::cout << "Initializing Image Capture...\n";

    capture.SetSettingsFile("pylon_settings.txt");

    if(argc > 1)
    {
        if(argc > 2)
        {
            capture.Start(argv[1], 640, 480, false, false);
        }
        else
        {
            capture.Start(argv[1], 640, 480, false, false);
        }
    }
    else
    {
        sources.clear();
        if(capture.ScanForSources(sources))
        {
            std::cout << "The following video devices were found on the system\n";
            for(unsigned int i = 0; i < (unsigned int)sources.size(); i++)
            {
                std::cout << sources[i] << std::endl;
            }
            if(sources.size() && capture.Start(sources[0], 0, 0, false, false) == 0)
            {
#ifdef WIN32
                capture.Start("C:\\Users\\Public\\Public Videos\\Sample Videos\\Wildlife.wmv");
#endif
            }
        }
        else
        {
#ifdef WIN32
            capture.Start("C:\\Users\\Public\\Videos\\Sample Videos\\Wildlife.wmv");
#endif
        }
    }

    if(!capture.IsReady())
    {
        std::cout << "Unable to start capture\n";
        return 0;
    }
    else
    {
        std::cout << "Success!\n";
    }

    gCount = 0;
    std::cout << "Press <enter> or <esc> key to exit. Press the <c> key to save picture\n";

    //  Test stopping of capture
    //capture.Stop();
    //capture.Start("");

    bool loop = true;

    while(loop)
    {
        capture.SetSettingsFile("pylon_settings.txt");
#ifdef TEST_CALLBACK
        CxUtils::SleepMs(10);
#endif

#ifndef TEST_CALLBACK
        if(capture.GetFrame(&src, true))
        {
            if( gCount == 0 ) {
                gStartTime = (unsigned int)(time(NULL));
            }
            //SwapRGBImage(src);
#ifdef CV_USE_OPEN_CV
            CvConvertToIplImage(src,&gDisplayImage);
            cvShowImage("Display",gDisplayImage);
            char filename[1024];
            sprintf(filename,"/media/external1/stop/stop%06d.jpg",gCount);
            //cvSaveImage(filename,gDisplayImage);
           
#endif
            std::cout << "Captured frame "
                      << ++gCount << " at ["
                      << src.mWidth << " x "
                      << src.mHeight << "] - "
                      << ( (( time(NULL) - gStartTime ) > 0 ) ? (double)gCount/(time(NULL) - gStartTime) : 0 )
                      << std::endl;
                      
                      
                      
        }
        /*  For testing purposes.*/
        if(capture.IsRawDataSupported())
        {
            static bool first = true;
            if(first == true && capture.GetRawFrame(&rawSrc, true))
            {
                //first = false;
                std::stringstream str;
                str << "test_raw_" << gCount << ".jpg";

                // Assuming data arrived as MJPEG...
                //SaveToJPEG(str.str().c_str(), rawSrc.mpData, rawSrc.mStoredBytes);
            }
        }
        #ifdef CV_USE_OPEN_CV
        cvWaitKey(10);
        #endif
#endif
    }

    return 0;
}

bool SaveToJPEG(const char *fileName, const unsigned char *buffer, const int size)
{
#ifndef WIN32
    unsigned char* jpeg = NULL;
    unsigned int jpegSize = 0;
    //CxUtils::JPEG::ConvertMJPEG2JPEG(buffer, size, &jpeg, &jpegSize);
    FILE* fp = fopen(fileName, "wb");
    if(fp == NULL)
    {
        return false;
    }
    fwrite(jpeg, jpegSize, 1, fp);
    fclose(fp);
#endif

    return false;
}

/*  End of File */

