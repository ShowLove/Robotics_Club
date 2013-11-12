////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  cvimagecaptureimagelinux.cpp
///
///  Copyright 2009 Daniel Barber
///                 Jonathan Mohlenhoff
///                 Gary Stein
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains the the implementation of CvImageCapture for Linux.
///  -------------------------------------------------------------------------------
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "cvcapture/cvimagecapture.h"
#include "cvcapture/imagecapturev4l.h"
#include "cvcapture/imagecapturev4l2.h"
#include "cvcapture/imagecapturedc1394.h"
#include "cvcapture/imagecapturedv1394.h"
#ifdef USE_PYLON
#include "cvcapture/imagecapturepylon.h"
#endif // USE_PYLON
#include "cvcapture/imageloader.h"
#include <stdio.h>
#include <string.h>
#include <iostream>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <opencv/cxcore.h>

// For file lookup
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>    // NON-STANDARD ANSI C library, but common
#include <fnmatch.h>

using namespace std;
using namespace Zebulon;
using namespace Video;

// Simple class to store libv4l data used by CvImageCapture for Linux
class CapData
{
public:
    CapData()
    {
        mpCvCapture = 0;
        mpCapture  = NULL;
    }
    ~CapData()
    {
        if(mpCapture)
        {
            delete mpCapture;
            mpCapture = NULL;
        }
    }
    CvCapture* mpCvCapture;
    CvCaptureBase * mpCapture;
};


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::CvImageCapture()
{
    mInterlacedFlag = false;
    mpCapture = new CapData();
    mPauseFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::~CvImageCapture()
{
    Stop();
    if(mpCapture)
    {
        delete ((CapData *)(mpCapture));
    }
    mpCapture = NULL;
}


/** Load settings data. */
void CvImageCapture::SetSettingsFile(const std::string& filename)
{
    mSettingsFilename = filename;
    if(mpCapture)
    {
        CapData * capData = (CapData *)mpCapture;
        if(capData->mpCapture)
        {
            capData->mpCapture->LoadSettings(filename);
        }
    }
}

///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to start capture from a video source.
///
///   \param[in] videoSource Camera/Video source name or file path.  Examples:
///              "/dev/video0" or "Logitech Pro 9000" or a number "0", "1" for
///              the camera number. To connect to a Basler Pylon camera use
///              the video source format "pylon-#" where the # is the device
///              or camera enumeration.
///   \param[in] width Desired image capture width.  If set to 0, then the
///              default settings for the camera are used (default value).
///   \param[in] height Desired image capture height. If set to 0 then the
///              default settings for the camera are used (default value).
///   \param[in] interlace If the image data is interlaced, this flag will enable
///              automatic de-interlacing.
///   \param[in] useDefaultCaptureLib If true, the default capture library
///                                   wrapped by this class is used, otherwise
///                                   an alternate.  (e.g. libv4l in Linux).
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int CvImageCapture::Start(const std::string& videoSource,
                          const int width,
                          const int height,
                          const bool interlace,
                          const bool useDefaultCaptureLib)
{
    int result = 0;
    CapData * capData = (CapData *)mpCapture;
    Stop();

    // Try capture from video device.
    int deviceNum = -1;
    if(strstr(videoSource.c_str(), "/dev/") == NULL)
    {
        capData->mpCapture = new ImageLoader();
        if(capData->mpCapture->Start(videoSource) > 0)
        {
            result = 1;
        }
        else
        {
            delete capData->mpCapture;
            capData->mpCapture = NULL;
        }
    }

    //  Basically check in a specific order
    //  start with files, only checking "." for file extension
    if(result != 1 && strstr(videoSource.c_str(), ".") != NULL)
    {
        // Capture from file.
        if( (capData->mpCvCapture = cvCaptureFromFile(videoSource.c_str())) > 0)
        {
            result = 1;
        }
        else
        {

        }
    }
    //  Next check for dc1394, opencv doesn't want to function, using fw1394
    else if(sscanf(videoSource.c_str(), "/dev/video1394/%d", &deviceNum) == 1)
    {
        //use opencv
        if(useDefaultCaptureLib)
        {
            capData->mpCapture = new ImageCaptureDC1394();
            if(capData->mpCapture->Start(videoSource, width, height, interlace))
            {
                result = 1;
            }
        }
        //opencv supports firewire now
        else
        {
            printf("OpenCv\n");
            capData->mpCvCapture = cvCreateCameraCapture(deviceNum);
            if(capData->mpCvCapture)
            {
                result = 1;
                if(width > 0 && height > 0)
                {
                    cvSetCaptureProperty(capData->mpCvCapture, CV_CAP_PROP_FRAME_WIDTH, width);
                    cvSetCaptureProperty(capData->mpCvCapture, CV_CAP_PROP_FRAME_HEIGHT, height);
                }
                //cvSetCaptureProperty(capData->mpCvCapture, CV_CAP_PROP_WHITE_BALANCE, 1);

            }
        }
    }
    //  Check for V4L cameras, but different methods
    else if(sscanf(videoSource.c_str(), "/dev/video%d", &deviceNum) == 1)
    {
        //use opencv
        if(useDefaultCaptureLib)
        {
            //slighly undocumented feature of opencv -gary
            //if a v4l device, then in the 200's
            deviceNum+=200;
            //valid device, non-negative
            if(deviceNum>=0)
            {
                capData->mpCvCapture = cvCreateCameraCapture(deviceNum);
                if(capData->mpCvCapture)
                {
                    result = 1;
                    if(width > 0 && height > 0)
                    {
                        cvSetCaptureProperty(capData->mpCvCapture, CV_CAP_PROP_FRAME_WIDTH, width);
                        cvSetCaptureProperty(capData->mpCvCapture, CV_CAP_PROP_FRAME_HEIGHT, height);
                    }
                }
            }
        }
        else
        {
            /*capData->mpCapture = new ImageCaptureV4L();
            if(capData->mpCapture->Start(videoSource, width, height, interlace))
            {
                result = 1;
            }*/
            capData->mpCapture = new ImageCaptureV4L2();
            capData->mpCapture->LoadSettings(mSettingsFilename);
            if(capData->mpCapture->Start(videoSource, width, height, interlace))
            {
                result = 1;
            }
        }
    }
    else if(sscanf(videoSource.c_str(), "/dev/dv1394/%d", &deviceNum) == 1)
    {
        std::cout << "DV Camera\n";
        capData->mpCapture = new ImageCaptureDV1394();
        capData->mpCapture->LoadSettings(mSettingsFilename);
        if(capData->mpCapture->Start(videoSource, width, height, interlace))
        {
            result = 1;
        }
    }
    else if(sscanf(videoSource.c_str(), "/dev/dv1394-%d", &deviceNum) == 1)
    {
        std::cout << "RAW DV Camera\n";
        capData->mpCapture = new ImageCaptureDV1394();
        capData->mpCapture->LoadSettings(mSettingsFilename);
        if(capData->mpCapture->Start(videoSource, width, height, interlace))
        {
            result = 1;
        }
    }
    else if(sscanf(videoSource.c_str(), "/dev/fw%d", &deviceNum) == 1)
    {
        std::cout << "RAW DV Camera\n";
        capData->mpCapture = new ImageCaptureDV1394();
        capData->mpCapture->LoadSettings(mSettingsFilename);
        if(capData->mpCapture->Start(videoSource, width, height, interlace))
        {
            result = 1;
        }
    }
#ifdef USE_PYLON
    // Check for Basler Pylon Device
    else if(strstr(videoSource.c_str(), "pylon") != NULL)
    {
        capData->mpCapture = new ImageCapturePylon();
        capData->mpCapture->LoadSettings(mSettingsFilename);
        if(capData->mpCapture->Start(videoSource,width,height,interlace) > 0)
        {
            result = 1;
        }
        else
        {
            delete capData->mpCapture;
            capData->mpCapture = NULL;
        }
    }
#endif // USE_PYLON
    else if(result == 0)
    {
        std::cout << "Camera went through the list\n";
    }

    if(result == 0)
    {
        Stop();
    }
    else
    {
        if(mCallbacks.size() > 0 || mRawCallbacks.size() > 0)
        {
            if(!mCaptureThread.CreateThread(&CaptureThreadFunction, this))
            {
                std::cout << "Capture Thread could not start\n";
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops video capture.
///
///   \return 1 on succes, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int CvImageCapture::Stop()
{
    CapData * capData = (CapData *)mpCapture;

    if(capData->mpCapture)
    {
        capData->mpCapture->mStopCaptureFlag = true;
        CxUtils::SleepMs(100);
    }

    mCaptureThread.StopThread();

    if(capData->mpCvCapture)
    {
        cvReleaseCapture(&capData->mpCvCapture);
        capData->mpCvCapture = 0;
    }
    else if(capData->mpCapture)
    {
        capData->mpCapture->Stop();
        delete capData->mpCapture;
        capData->mpCapture = NULL;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get the current frame from the camera/video
///          source.  Normal frame data is in BGR format with top left
///          corner starting at beginning of array.
///
///   \param[out] Pointer to image structure to fill with current frame.
///   \param[in] block If true, this method will block until a new frame arrives
///              otherwise it will fall through if a new frame isn't available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int CvImageCapture::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    CapData * capData = (CapData *)mpCapture;
    if (dest != NULL)
    {
        dest->mID = mID;
    }

    // OpenCV
    if(capData->mpCvCapture)
    {
        IplImage* captured = cvQueryFrame(capData->mpCvCapture);
        if(captured)
        {
            if(captured->origin != IPL_ORIGIN_TL)
            {
                cvFlip(captured, captured, 0);
            }
            dest->Create(captured->width, captured->height, captured->nChannels);

            memcpy(dest->mpData, captured->imageData, captured->imageSize);
            return 1;
        }
    }
    // Other interface libraries.
    else if(capData->mpCapture)
    {
        return capData->mpCapture->GetFrame(dest, block);
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get the current frame from the camera/video
///          source.  This will try get RAW frame data if supported by
///                   device interface.
///
///   \param[out] Pointer to image structure to fill with current frame.
///   \param[in] block If true, this method will block until a new frame arrives
///              otherwise it will fall through if a new frame isn't available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int CvImageCapture::GetRawFrame(CvImageCapture::Image* dest, const bool block)
{
    CapData * capData = (CapData *)mpCapture;
    if (dest != NULL)
    {
        dest->mID = mID;
    }

    if(capData->mpCapture)
    {
        return capData->mpCapture->GetRawFrame(dest, block);
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::IsReady() const
{
    CapData * capData = (CapData *)mpCapture;
    if(capData->mpCvCapture)
    {
        return true;
    }
    else if(capData->mpCapture)
    {
        return capData->mpCapture->IsReady();
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if raw capture data can be retrieved.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::IsRawDataSupported() const
{
    CapData * capData = (CapData *)mpCapture;
    if(capData->mpCapture)
    {
        return capData->mpCapture->SupportsRawFrame();
    }

    return false;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to scan for video devices on the system.
///
///   \param[out] sources Video sources on the computer.
///
///   \return Number of sources detected.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int CvImageCapture::ScanForSources(std::vector<std::string>& sources)
{
    sources.clear();
    // Look for /dev/video0, /dev/video1, etc.
    DIR *directory = opendir("/dev/");
    if (directory)
    {
        dirent *d = readdir(directory);
        while (d)
        {
            int deviceNum = 0;
            if(sscanf(d->d_name, "video%d", &deviceNum) == 1)
            {
                std::string src = "/dev/";
                src += std::string(d->d_name);
                sources.push_back(src);
            }
            else if(sscanf(d->d_name, "dv1394/%d", &deviceNum) == 1)
            {
                std::string src = "/dev/";
                src += std::string(d->d_name);
                sources.push_back(src);
            }
            else if(sscanf(d->d_name, "dv1394-%d", &deviceNum) == 1)
            {
                std::string src = "/dev/";
                src += std::string(d->d_name);
                sources.push_back(src);
            }
            else if(sscanf(d->d_name, "video1394/%d", &deviceNum) == 1)
            {
                std::string src = "/dev/";
                src += std::string(d->d_name);
                sources.push_back(src);
            }
            else if(sscanf(d->d_name, "fw%d", &deviceNum) == 1)
            {
                std::string src = "/dev/";
                src += std::string(d->d_name);
                sources.push_back(src);
            }
            d = readdir(directory);
        }
        closedir(directory);
    }
    return (unsigned int)sources.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive image data continuously.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RegisterCallback(Callback* cb)
{
    if(cb != NULL)
    {
        mMutex.Lock();
        mCallbacks.insert(cb);
        mMutex.Unlock();
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive image data continuously.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RegisterRawCallback(RawCallback* cb)
{
    if(cb != NULL)
    {
        mMutex.Lock();
        mRawCallbacks.insert(cb);
        mMutex.Unlock();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a callback to receive image data continuously.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RemoveCallback(Callback* cb)
{
    mMutex.Lock();
    std::set<Callback*>::iterator cb2;
    cb2 = mCallbacks.find(cb);
    if(cb2 != mCallbacks.end())
    {
        mCallbacks.erase(cb2);
    }
    mMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a callback to receive image data continuously.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RemoveRawCallback(RawCallback* cb)
{
    mMutex.Lock();
    std::set<RawCallback*>::iterator cb2;
    cb2 = mRawCallbacks.find(cb);
    if(cb2 != mRawCallbacks.end())
    {
        mRawCallbacks.erase(cb2);
    }
    mMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RunCallback()
{
    mMutex.Lock();
    std::set<Callback*>::iterator cb;
    for(cb = mCallbacks.begin();
        cb != mCallbacks.end();
        cb++)
    {
        (*cb)->ProcessCameraData(&mImage);
    }
    std::set<RawCallback*>::iterator rcb;
    for(rcb = mRawCallbacks.begin();
        rcb != mRawCallbacks.end();
        rcb++)
    {
        (*rcb)->ProcessRawCameraData(&mRawImage);
    }
    mMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread method for continuous capture.
///
////////////////////////////////////////////////////////////////////////////////////
void CvImageCapture::CaptureThreadFunction(void *args)
{
    //Pass in as a void *, then cast, then virtuals should work
    CvImageCapture* camera = (CvImageCapture*)args;
    //call the appropriate real processing function per camera
    camera->CaptureFunction();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread method for continuous capture.
///
////////////////////////////////////////////////////////////////////////////////////
void CvImageCapture::CaptureFunction()
{

    //Tight Loop Capturing
    while(mCaptureThread.QuitThreadFlag() == false)
    {
        if(IsReady())
        {
            if(mPauseFlag == false)
            {
                mMutex.Lock();
                if(mCallbacks.size() > 0)
                {
                    // Too Many Copies? copy to local first?
                    GetFrame(&mImage);
                }
                if(mRawCallbacks.size() > 0)
                {
                    GetRawFrame(&mRawImage);
                }
                mMutex.Unlock();
                RunCallback();
            }
        }
        CxUtils::SleepMs(1);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \breturn If supported, pointer to raw capture device interface
///            is returned.
///
////////////////////////////////////////////////////////////////////////////////////
void *CvImageCapture::GetRawCapture()
{
    CapData * capData = (CapData *)mpCapture;
    if(capData->mpCapture)
    {
        ImageCaptureDC1394* dc = dynamic_cast<ImageCaptureDC1394*>(capData->mpCapture);
        if(dc)
        {
            return dc->GetCamera();
        }
#ifdef USE_PYLON
        ImageCapturePylon* pylon = dynamic_cast<ImageCapturePylon*>(capData->mpCapture);
        if(pylon)
        {
            return pylon;
        }
#endif
    }

    std::cout << "Raw Not Supported\n";
    return NULL;
}

/*  End of File */

