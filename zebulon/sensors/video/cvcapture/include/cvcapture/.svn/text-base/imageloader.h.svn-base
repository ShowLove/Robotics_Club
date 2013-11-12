////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imageloader.h
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
#ifndef _CV_IMAGE_CAPTURE_IMAGE_LOADER__H
#define _CV_IMAGE_CAPTURE_IMAGE_LOADER__H

#include <vector>
#include <algorithm>
#include <string>
#include "cvcapture/cvimagecapture.h"
#include <cxutils/images/image.h>
#include <cxutils/mutex.h>

namespace Zebulon
{
    namespace Video
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ImageLoader
        ///   \brief Simulates video by reading a directory of enumerated images.  
        ///
        ///   Example of image name formats:  00001.jpg, 00002.jpg, etc.
        ///
        ///   Required Libraries:
        ///   CxUtils
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ImageLoader : public CvCaptureBase
        {
        public:
            ImageLoader();
            ~ImageLoader();
            virtual int Start(const std::string& videoImagesDirectory,
                              const int width = 0,
                              const int height = 0,
                              const bool interlace = false);
            virtual int Stop();
            virtual int GetFrame(CvImageCapture::Image* dest, const bool block = false);
            virtual int GetRawFrame(CvImageCapture::Image* dest, const bool block = false) { return 0; }
            virtual bool IsReady() const { return mImageFiles.size() > 0 ? true: false; }
        private:
            CvImageCapture::Image mImage;           ///<  Current image data loaded.
            unsigned int mFrameNumber;              ///<  Current frame number/count.
            unsigned int mIndex;                    ///<  Index in vector.
            CxUtils::Mutex mImageMutex;             ///<  Mutex for thread protection of data.
            std::string mDirectory;                 ///<  Directory containing image data.
            std::vector<std::string> mImageFiles;   ///<  Image filenames.
        };
    }
}

#endif

/*  End of File */
