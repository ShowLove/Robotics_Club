////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturev4l.cpp
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
#include "cvcapture/imagecapturev4l.h"

using namespace std;
using namespace Zebulon;
using namespace Video;

/* convert from 4:2:2 YUYV interlaced to RGB24 */
/* based on ccvt_yuyv_bgr32() from camstream */
#define SAT(c) \
        if (c & (~255)) { if (c < 0) c = 0; else c = 255; }
void yuyv_to_rgb24 (int width, int height, unsigned char *src, unsigned char *dst);


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureV4L::ImageCaptureV4L()
{
    mInterlacedFlag = false;
    mCaptureDevice = 0;
    mVideoDevice = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureV4L::~ImageCaptureV4L()
{
    Stop();
    mCaptureDevice = 0;
    mVideoDevice = 0;
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
int ImageCaptureV4L::Start(const std::string& videoSource,
                          const int width,
                          const int height,
                          const bool interlace)
{
    int result = 0;
    
    // Check to see if the videoSource is a filename for a video.    
    
    int videoStandard = WEBCAM;   // Check libv4l.h file for #define.

    mVideoDevice = open_device((char *)videoSource.c_str());
    mCaptureDevice = init_capture_device(mVideoDevice, 
                                         width, 
                                         height, 
                                         0, 
                                         videoStandard, 
                                         3);
    if(mCaptureDevice == NULL)
    {
        // Error initializing device.
        return result;
    }
    // Using function pointer (no parameters for now)
    if( (*mCaptureDevice->actions->set_cap_param)(mVideoDevice, NULL, 0))
    {
        // Could not set the capture parameters.
        free_capture_device(mVideoDevice);
        close_device(mVideoDevice);
        return result;
    }
    if ((*mCaptureDevice->actions->init_capture)(mVideoDevice) < 0)
    {
        // Could not initialize capture.
        free_capture_device(mVideoDevice);
        close_device(mVideoDevice);
        return result; 
    }
    
    // Start capture.
    if ((*mCaptureDevice->actions->start_capture)(mVideoDevice) < 0)
    {
        // Could not start capture.
        (*mCaptureDevice->actions->free_capture)(mVideoDevice);
        free_capture_device(mVideoDevice);
        close_device(mVideoDevice);
        mVideoDevice = 0;
        return result; 
    }
    
    mImage.Create(mCaptureDevice->width, mCaptureDevice->height, 3);
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
int ImageCaptureV4L::Stop()
{
    if(mCaptureDevice)
    {
        if(mVideoDevice)
        {
            (*mCaptureDevice->actions->stop_capture)(mVideoDevice);
            (*mCaptureDevice->actions->free_capture)(mVideoDevice);
            free_capture_device(mVideoDevice);
            close_device(mVideoDevice);
        }
    }
    mCaptureDevice = 0;
    mVideoDevice = 0;

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
int ImageCaptureV4L::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    int size = 0;
    unsigned char* d;
    int result = 0;
    if( (d = (unsigned char*)(*mCaptureDevice->actions->dequeue_buffer)(mVideoDevice, &size)) != NULL)
    {
        // Convert to RGB24 data
        yuyv_to_rgb24((int)mImage.mWidth, 
                      (int)mImage.mHeight,
                      d,
                      mImage.mpData);
        //color flipped, faster if modift rgb24?
        //CvImageCapture::FlipChannels(mImage);
        // Put frame
        (*mCaptureDevice->actions->enqueue_buffer)(mVideoDevice);
        if(dest)
        {
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
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get the current frame from the camera/video
///          source but in the native format from the device (RAW).
///
///   \param[out] Pointer to image structure to fill with current frame.
///   \param[in] block If true, this method will block until a new frame arrives
///              otherwise it will fall through if a new frame isn't available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCaptureV4L::GetRawFrame(CvImageCapture::Image* dest, const bool block)
{
    int size = 0;
    unsigned char* d;
    int result = 0;
    if( (d = (unsigned char*)(*mCaptureDevice->actions->dequeue_buffer)(mVideoDevice, &size)) != NULL)
    {
        // Just copy raw image data.
        memcpy(mImage.mpData, d, size);
        // Put frame
        (*mCaptureDevice->actions->enqueue_buffer)(mVideoDevice);
        if(dest)
        {
            *dest = mImage;
        }
        result = 1;
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCaptureV4L::IsReady() const
{
    return mImage.mpData != NULL ? true : false;
}


/* convert from 4:2:2 YUYV interlaced to RGB24 */
/* based on ccvt_yuyv_bgr32() from camstream */
void yuyv_to_rgb24 (int width, int height, unsigned char *src, unsigned char *dst)
{
   unsigned char *s;
   unsigned char *d;
   int l, c;
   int r, g, b, cr, cg, cb, y1, y2;
   
   l = height;
   s = src;
   d = dst;
   while (l--) {
      c = width >> 1;
      while (c--) {
         y1 = *s++;
         cb = ((*s - 128) * 454) >> 8;
         cg = (*s++ - 128) * 88;
         y2 = *s++;
         cr = ((*s - 128) * 359) >> 8;
         cg = (cg + (*s++ - 128) * 183) >> 8;

         r = y1 + cr;
         b = y1 + cb;
         g = y1 - cg;
         SAT(r);
         SAT(g);
         SAT(b);

         *d++ = b;
         *d++ = g;
         *d++ = r;

         r = y2 + cr;
         b = y2 + cb;
         g = y2 - cg;
         SAT(r);
         SAT(g);
         SAT(b);

         *d++ = b;
         *d++ = g;
         *d++ = r;
      }
   }
}


/*  End of File */
