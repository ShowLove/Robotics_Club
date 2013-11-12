////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturepylon.h
///
///  Copyright 2012 David Adams, Daniel Barber
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains a wrapper class for the Baslery Pylon image capture library.
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
#ifndef _CV_IMAGE_CAPTURE_PYLON__H
#define _CV_IMAGE_CAPTURE_PYLON__H


#include "cvcapture/cvimagecapture.h"

#ifndef WIN32

namespace Zebulon
{
    namespace Video
    {
        class PylonGrabBuffer;
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ImageCapturePylon
        ///   \brief Video Capture interface that uses the Baslyer Pylon libraries to
        ///   capture from a GIGE Camera device.
        ///
        ///   Required Libraries:
        ///   Windows: Not currently supported.
        ///   Linux: Pylon libraries.  See docs folder for details.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ImageCapturePylon : public CvCaptureBase
        {
        public:
            /** White balance modes. */
            enum WhiteBalanceMode
            {
                Off = 0,
                Once,
                Continuous // Not supported on all devices.
            };

            ImageCapturePylon();
            ~ImageCapturePylon();
            virtual bool LoadSettings(const std::string& filename);
            virtual int Start(const std::string& videoSource,
                              const int width = 0,
                              const int height = 0,
                              const bool interlace = false);
            virtual int Stop();
            virtual int GetFrame(CvImageCapture::Image* dest, const bool block = false);
            virtual int GetRawFrame(CvImageCapture::Image* dest, const bool block = false);
            virtual bool SupportsRawFrame() const { return true; }
            virtual bool IsReady() const;
            static unsigned int ScanForSources(std::vector<std::string>& sources);
            bool SetExposureTime(const int exposureTime);
            bool SetWhiteBalanceMode(const int mode);
        private:
            CxUtils::Mutex mMutex;              ///<  For thread protection of data
            int mState;                         ///<  Capture state of SDK interface.
            int mDeviceNum;                     ///<  Device number (which camera to connect to).
            unsigned int mFrameNumber;          ///<  Current frame number (so we know when new frames arrived).
            unsigned int mGrabbedNumber;        ///<  Frame number grabbed from pylon SDK.
            CvImageCapture::Image mImage;       ///<  Converted BGR image ready to go.
            CvImageCapture::Image mRawImage;    ///<  RAW unconverted image from camera itself.
            bool mFlipX;                        ///<  Flag to flip image on horizontal axis.
            bool mFlipY;                        ///<  Flag to flip on vertical axis.
            int mExposureTime;                  ///<  Exposure time DEFAULT = 5000.
            int mPacketSize;                    ///<  Packet size for streaming.
            int mWhiteBalanceMode;              ///<  White Balance Mode off by default (i.e. 0). [16, 100000000].
            int mWidth;                         ///<  Capture width resolution.
            int mHeight;                        ///<  Capture height resolution.
            int mDefaultWidth;                  ///<  Default camera max width.
            int mDefaultHeight;                 ///<  Default camera max height.
            void* mpCamera;                     ///<  Pointer to camera interface.
            void* mpStream;                     ///<  Pointer to stream interface.
            std::vector<PylonGrabBuffer*> mBufferList;  ///<  Buffered images for capture.
        };
    }
}

#endif

#endif

/*  End of File */
