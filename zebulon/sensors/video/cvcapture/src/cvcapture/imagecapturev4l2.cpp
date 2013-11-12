////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturev4l2.cpp
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
///  Contains a wrapper class for the libv4l2 image capture library.
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
#include "cvcapture/imagecapturev4l2.h"

using namespace std;
using namespace Zebulon;
using namespace Video;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureV4L2::ImageCaptureV4L2()
{
    mFD=0;
    mInterlacedFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureV4L2::~ImageCaptureV4L2()
{
    Stop();
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
int ImageCaptureV4L2::Start(const std::string& videoSource,
                            const int width,
                            const int height,
                            const bool interlace)
{
    int result = 0;
    mFD=v4l1_open(videoSource.c_str(), O_RDWR);
    if(mFD<=0)
    {
        return result;
    }

    mDestFormat.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    mDestFormat.fmt.pix.pixelformat = V4L2_PIX_FMT_BGR24;
    mDestFormat.fmt.pix.field = V4L2_FIELD_INTERLACED;
    //open at fixed resolution
    if(width==0 || height==0)
    {
        //could have it auto find?
        mDestFormat.fmt.pix.width=320;
        mDestFormat.fmt.pix.height=240;
    }
    else
    {
        mDestFormat.fmt.pix.width=width;
        mDestFormat.fmt.pix.height=height;
    }
    
    //Create a converter (find formats)
    mpV4LConvertData = v4lconvert_create(mFD);
    if (mpV4LConvertData == NULL)
    {
        printf("v4lconvert_create error\n");
        return result;
    }
    //Loop through looking for a input to output format
    if (v4lconvert_try_format(mpV4LConvertData, &mDestFormat, &mSrcFormat) != 0)
    {
        printf("v4lconvert_try_format error\n");
        return result;
    }
    
    //set camera for known working src
    int ret = v4l1_ioctl(mFD, VIDIOC_S_FMT, &mSrcFormat);
    printf("Input Format: %dx%d ",mSrcFormat.fmt.pix.width,mSrcFormat.fmt.pix.height);
    GetFourCC(mSrcFormat.fmt.pix.pixelformat);
    printf("Output Format: %dx%d ",mDestFormat.fmt.pix.width,mDestFormat.fmt.pix.height);
    GetFourCC(mDestFormat.fmt.pix.pixelformat);
    mSrcImage.Create(mSrcFormat.fmt.pix.width, mSrcFormat.fmt.pix.height, 3);
    mDstImage.Create(mDestFormat.fmt.pix.width, mDestFormat.fmt.pix.height, 3);
    
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
int ImageCaptureV4L2::Stop()
{
    if(mFD>0)
    {
        v4l1_close(mFD);
        mFD=0;
    }
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
int ImageCaptureV4L2::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    int size = 0;
    int result = 0;

    //Read from camera the raw format
    size = v4l1_read(mFD,mSrcImage.mpData,mSrcFormat.fmt.pix.sizeimage);
    if(size <= 0)
    {
        //printf("Size: %d vs %d\n",size,mSrcFormat.fmt.pix.sizeimage);
        return result;
    }
    //So capture was good (convert to right format)
    v4lconvert_convert(mpV4LConvertData,
        &mSrcFormat,
        &mDestFormat,
        mSrcImage.mpData, mSrcFormat.fmt.pix.sizeimage,
        mDstImage.mpData, mDestFormat.fmt.pix.sizeimage);    
    
    
    if(dest)
    {
        if(mInterlacedFlag)
        {
            dest->Create(mDstImage.mWidth, mDstImage.mHeight, mDstImage.mChannels);
            CvImageCapture::DeInterlaceData(mDstImage.mpData, dest->mpData, mDstImage.mWidth, mDstImage.mHeight);
        }
        else
        {
            *dest = mDstImage;
        }
    }
    
    result = 1;
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
int ImageCaptureV4L2::GetRawFrame(CvImageCapture::Image* dest, const bool block)
{
    int size = 0;
    unsigned char* d;
    int result = 0;
    
    if(dest == NULL)
    {
        return 0;
    }
    
    dest->Create(mSrcImage.mWidth,
                 mSrcImage.mHeight,
                 mSrcImage.mChannels);
    dest->mStoredBytes = mSrcFormat.fmt.pix.sizeimage;
    // Copy directly to dest buffer to save # of copies.
    size = v4l1_read(mFD, dest->mpData, mSrcFormat.fmt.pix.sizeimage);
    if(size <= 0)
    {
        //printf("Size: %d vs %d\n", size, mSrcFormat.fmt.pix.sizeimage);
        return result;
    }

    result = 1;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCaptureV4L2::IsReady() const
{
    return mDstImage.mpData != NULL ? true : false;
}

void ImageCaptureV4L2::GetFourCC(int num)
{
    char A,B,C,D;
    A=num&0xFF;
    num>>=8;
    B=num&0xFF;
    num>>=8;
    C=num&0xFF;
    num>>=8;
    D=num&0xFF;
    printf("FourCC: %c%c%c%c\n",A,B,C,D);
}

/*  End of File */
