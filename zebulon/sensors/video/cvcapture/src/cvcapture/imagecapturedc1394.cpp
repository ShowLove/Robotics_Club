////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturedc1394.cpp
///
///  Copyright 2009 Gary Stein
///                 Daniel Barber
///                 David Adams
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains a wrapper class for the libv4l image capture library.
///  -------------------------------------------------------------------------------
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Club at UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
#include "cvcapture/imagecapturedc1394.h"

using namespace std;
using namespace Zebulon;
using namespace Video;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureDC1394::ImageCaptureDC1394()
{
    mInterlacedFlag = false;
    mCamera = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureDC1394::~ImageCaptureDC1394()
{
    Stop();
    mCamera = 0;
}


///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to start capture from a video source.
///
///   \param[in] videoSource Camera/Video source name or file path.  Examples:
///              "/dev/video0" or "Logitech Pro 9000".
///   \param[in] width Desired image capture width.  If set to 0, then the
///              default settings for the camera are used (default value).
///   \param[in] height Desired image capture height. If set to 0 then the
///              default settings for the camera are used (default value).
///   \param[in] interlace If the image data is interlaced, this flag will enable
///              automatic de-interlacing.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCaptureDC1394::Start(const std::string& videoSource,
                          const int width,
                          const int height,
                          const bool interlace)
{
    int result = 0;
    
    // Check to see if the videoSource is a filename for a video.    

    int device;
    if(sscanf(videoSource.c_str(),"/dev/video1394/%d",&device)==0)
    {
        return 0;
    }
    //good so far
    //Passing 0 would be camera 1, 1 would be camera 2
    mCamera=new fw1394Camera(device);
    if(!mCamera)
    {
        return 0;
    }
    
    mFirst=0;
    //mCamera->printCameraInfo();
    //assume 640x480 for now
#define USINGYUV
    if(width == 640 && height == 480)
    {
#ifdef USINGYUV        
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_YUV422);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_YUV422);
        mCamera->doingYUV(true);
#else
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_RGB8);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_RGB8);
        mCamera->doingYUV(false);
#endif
        mCamera->setFramerate(fw1394Camera::fwFRAMERATE_15);
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
    }
    else if(width == 800 && height == 600)
    {
#ifdef USINGYUV        
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_800x600_YUV422);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_YUV422);
        mCamera->doingYUV(true);
#else
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_800x600_RGB8);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_RGB8);
        mCamera->doingYUV(false);
#endif
        
        mCamera->setFramerate(fw1394Camera::fwFRAMERATE_30);
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
    }
    else if(width == 1024 && height == 768)
    {
#ifdef USINGYUV        
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_1024x768_YUV422);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_YUV422);
        mCamera->doingYUV(true);
#else
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_1024x768_RGB8);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_RGB8);
        mCamera->doingYUV(false);
#endif
        
        mCamera->setFramerate(fw1394Camera::fwFRAMERATE_15);
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
    }
    else if(width == 1280 && height == 960)
    {
#ifdef USINGYUV        
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_1280x960_YUV422);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_YUV422);
        mCamera->doingYUV(true);
#else
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_1280x960_RGB8);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_RGB8);
        mCamera->doingYUV(false);
#endif
        
        mCamera->setFramerate(fw1394Camera::fwFRAMERATE_15);
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
    }
    /*if(width==0 && height==0)
    {
        //mCamera->printCameraInfo();
        /*mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_640x480_RGB8);
        mCamera->setFramerate(fw1394Camera::fwFRAMERATE_15);
        mCamera->setColorCoding(fw1394Camera::fwCOLOR_CODING_RGB8);
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
        
        mCamera->setVideoMode(fw1394Camera::fwVIDEO_MODE_FORMAT7_3);
        mCamera->setFormat7ROI();
        mCamera->getWidth(mWidth);
        mCamera->getHeight(mHeight);
    }*/
    //doesn't support other than 640x480 currently
    else
    {
        std::cout << "Camera Settting not Supported\n";
        return 0;
    }
    
    //size from above
    mImage.Create(mWidth, mHeight, 3);
    mInterlacedFlag = interlace;
    result = 1; // Success.
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops video capture.
///
///   \return 1 on succes, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCaptureDC1394::Stop()
{
    if(mCamera)
    {
        delete mCamera;
    }
    mCamera = 0;
    mFirst = 0;

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get the current frame from the camera/video
///          source.
///
///   \param[out] Pointer to image structure to fill with current frame.
///   \param[in] block If true, this method will block until a new frame arrives
///              otherwise it will fall through if a new frame isn't available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCaptureDC1394::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    int result = 0;
    //get data from the camera
    mCamera->acquire(mImage.mpData);
    //SwapRGBImage(mImage);
    mFirst = 1;
    
    if(dest)
    {
        //CvImageCapture::FlipChannels(mImage);
        //this doesn't look right, make memory? -gary
        if(mInterlacedFlag)
        {
            dest->Create(mImage.mWidth, mImage.mHeight, mImage.mChannels);
            CvImageCapture::DeInterlaceData(mImage.mpData, dest->mpData, mImage.mWidth, mImage.mHeight);
        }
        else
        {
            *dest = mImage;
        }
    }
    result = 1;

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCaptureDC1394::IsReady() const
{
    return mImage.mpData != NULL ? true : false;
}


/*  End of File */
