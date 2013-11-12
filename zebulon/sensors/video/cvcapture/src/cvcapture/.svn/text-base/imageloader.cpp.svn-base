////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imageloader.cpp
///
///  Copyright 2010 Daniel Barber
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
#include "cvcapture/imageloader.h"
#include <cxutils/fileio.h>

using namespace Zebulon;
using namespace Video;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageLoader::ImageLoader()
{
    mIndex = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageLoader::~ImageLoader()
{
    Stop();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads the filenames of a directory containing images for loading.
///
///   Currently only jpg or png images supported.
///
///   \param[in] videoImagesDirectory Directory with image data.
///   \param[in] width Not used.
///   \param[in] height Not used.
///   \param[in] interlace Not used.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageLoader::Start(const std::string& videoImagesDirectory,
                       const int width,
                       const int height,
                       const bool interlace)
{
    Stop();
    CxUtils::Mutex::ScopedLock lock(&mImageMutex);
    if(CxUtils::FileIO::GetFiles(mImageFiles, "*.jpg", videoImagesDirectory, false, true) ||
       CxUtils::FileIO::GetFiles(mImageFiles, "*.png", videoImagesDirectory, false, true) ||
       CxUtils::FileIO::GetFiles(mImageFiles, "*.bmp", videoImagesDirectory, false, true))
    {
        std::sort(mImageFiles.begin(), mImageFiles.end());
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes all open files.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageLoader::Stop()
{
    mFrameNumber = 0;
    mIndex = 0;
    mImageFiles.clear();
    mImage.Destroy();
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the next image from the directory.
///
///   \param[out] dest Pointer to copy data to.
///   \param[in] block Not used.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageLoader::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    if(mImageFiles.size() > 0)
    {
        if(mIndex >= mImageFiles.size())
        {
            mIndex = 0; // Start over again.
        }
        if(mIndex < mImageFiles.size())
        {
            CxUtils::Image image;
            //std::cout << mImageFiles[mIndex] << std::endl;
            if(image.Load(mImageFiles[mIndex++]))
            {
                //image.Save("test.jpg");
                mFrameNumber++;
                if(dest)
                {
                    dest->Create(image.mWidth, image.mHeight, image.mChannels);
                    memcpy(dest->mpData, image.mpImage, image.mDataSize*sizeof(unsigned char));
                }
                //locked to ~15 fps? -gary
                CxUtils::SleepMs(1000/15);
                return 1;
            }
        }
    }

    return 0;
}


/*  End of File */

