////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  cvimagecapture.h
///
///  Copyright 2012 Brian C. Becker
///                 Daniel Barber
///                 Gary Stein
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains the CvImageCapture wrapper class for cross platform video capture.
///  If you get a compile error in windows, make sure you are including this
///  header file before ALL other files.
///  -------------------------------------------------------------------------------
///
///  Modifications:
///  -------------------------------------------------------------------------------
///  8/10/2007 - Made imnprovements and code cleanup to interface.  This involved
///  more tightly integrating with the OpenCV library, merging the
///  DirectShowCapture class with the CvImageCapture class.  Also switched
///  to using a Callback in DirectShow so that we do not sample the same
///  frame multiple times (also increases performance).
///
///  9/18/2007 - Make sure to use CV_CAPTURE_IMPORTS to use the DLL version of
///  the library built using this file.
///
///  6/15/2007 - Added ability to disable use of OpenCV in Windows with the
///  CV_NO_OPENCV preprocessor definition. (Only works in Windows).
///
///  7/1/2008 - Made fixes which caused incompatibilities with other libraries
///  and removed need to include DirectShow/Windows files in the header file.
///  Also changed pre-processor defintions.  You must now disable OpenCV
///  requirement with CV_IMAGE_CAPTURE_NO_OPEN_CV.
///
///  2/21/2009 - Updated class interface to better wrap different image capture
///  libraries.  However, conversion functions still exist to convert
///  from CvImageCapture::Image to an IplImage pointer if you use the
///  CV_USE_OPEN_CV Preprocessor definition in windows.
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
#ifndef _CV_IMAGE_CAPTURE_H
#define _CV_IMAGE_CAPTURE_H

#ifdef WIN32

#pragma warning( disable : 4251)

#ifdef CV_IMAGE_CAPTURE_EXPORT
    #define CV_IMAGE_CAPTURE_DLL __declspec(dllexport)
#else
    #define CV_IMAGE_CAPTURE_DLL __declspec(dllimport)
   
    #if _MSC_VER < 1600
        #ifdef _DEBUG
        #pragma comment( lib, "libcvcapture_d.lib" )
        #else
        #pragma comment( lib, "libcvcapture.lib" )
        #endif
    #endif
#endif

#else
    //  For Linux
    #define CV_IMAGE_CAPTURE_DLL
#endif

#include <string>
#include <vector>
#include <set>

#include <string.h>

#ifdef WIN32
    #ifdef CV_USE_OPEN_CV
    #include <opencv/cv.h>
    #include <cxutils/images/image.h>
    #endif
#else
    #include <cxutils/thread.h>
    #include <cxutils/mutex.h>
    #include <cxutils/images/image.h>
    #include <opencv/cv.h>
#endif

namespace Zebulon
{
    namespace Video
    {

        class CvCaptureBase;

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class CvImageCapture
        ///   \brief Video Capture interface that supports USB/Firewire DV cameras and
        ///   video files.
        ///
        ///   Required Libraries:
        ///   Windows: DirectX 9 SDK, Windows Platform SDK (DirectShow) if compiling,
        ///            otherwise just use the produced DLL as is.
        ///   Linux: LibV4L, libdv, libdc
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CV_IMAGE_CAPTURE_DLL CvImageCapture
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Image
            ///   \brief Simple image data structure.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CV_IMAGE_CAPTURE_DLL Image
            {

            public:
                Image();
                Image(const Image& img);
                ~Image();
                Image& operator=(const Image& img);
                void Destroy();
                void Create(const unsigned short width,
                            const unsigned short height,
                            const unsigned short channels);
                inline unsigned char Get(unsigned short row, unsigned short col, unsigned short chan) const
                {
                    //return mpData[row*mChannels*mWidth+col*mChannels];
                    //return ((unsigned char*)(mpData + width*row))[col];
                    return mpData[row*mWidth*mChannels+col*mChannels+chan];
                }

                inline void Set(unsigned short row, unsigned short col, unsigned short chan, unsigned char val)
                {
                    mpData[row*mWidth*mChannels+col*mChannels+chan] = val;
                }

                unsigned short mWidth;    ///<  Image width.
                unsigned short mHeight;   ///<  Image height.
                unsigned short mChannels; ///<  Number of channels.
                unsigned char* mpData;    ///<  Image data [size is mWidth*mHeight*mChannels].
                unsigned int mStoredBytes;///<  Number of bytes of data stored at mpData.
                unsigned char mID;        ///<  ID's image for Visual Sensor
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Overload this callback and functions to recieve events from
            ///          generic camera
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CV_IMAGE_CAPTURE_DLL Callback
            {
                public:
                    Callback() {}
                    virtual ~Callback() {}
                    ////////////////////////////////////////////////////////////////////////////////////
                    ///
                    ///   \brief Event called when new images are produced
                    ///
                    ///   \params Memory for image data in internal format
                    ///
                    ////////////////////////////////////////////////////////////////////////////////////
                    virtual void ProcessCameraData(Zebulon::Video::CvImageCapture::Image* dest) {}
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Overload this callback and functions to recieve events from
            ///          generic camera
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CV_IMAGE_CAPTURE_DLL RawCallback
            {
                public:
                    RawCallback() {}
                    virtual ~RawCallback() {}
                    ////////////////////////////////////////////////////////////////////////////////////
                    ///
                    ///   \brief Event called when new images are produced
                    ///
                    ///   \params Memory for image data in internal format
                    ///
                    ////////////////////////////////////////////////////////////////////////////////////
                    virtual void ProcessRawCameraData(Zebulon::Video::CvImageCapture::Image* dest) {}
            };
            CvImageCapture();
            ~CvImageCapture();
#ifndef WIN32
            /** Method to set the name of a config file for specific camera
                devices/interfaces to use if they require it. */
            void SetSettingsFile(const std::string& filename);
#endif
            ////////////////////////////////////////////////////////////////////////////////////
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
            ///   \param[in] useDefaultCaptureLib If true, the default capture library
            ///                                   wrapped by this class is used, otherwise
            ///                                   an alternate.  (e.g. libv4l in Linux).
            ///
            ///   \return 1 on success, 0 on failure.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            int Start(const std::string& videoSource,
                      const int width = 0,
                      const int height = 0,
                      const bool interlace = false,
                      const bool useDefaultCaptureLib = true);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Stops video capture.
            ///
            ///   \return 1 on succes, 0 on failure.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            int Stop();
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
            int GetFrame(CvImageCapture::Image* dest, const bool block = false);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Use this method to get the current RAW frame from the camera/video
            ///          source.  RAW data may be compressed in whatever format the
            ///          hardware returns.
            ///
            ///   \param[out] Pointer to image structure to fill with current frame.
            ///   \param[in] block If true, this method will block until a new frame arrives
            ///              otherwise it will fall through if a new frame isn't available.
            ///
            ///   \return 1 on success, 0 on failure.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            int GetRawFrame(CvImageCapture::Image* dest, const bool block = false);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \return True if ready to capture image data, false otherwise.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            bool IsReady() const;
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \return True if raw capture data can be retrieved.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            bool IsRawDataSupported() const;
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \return True if able to pause capture, false otherwise.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual bool Pause() { mPauseFlag = true; return true; }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \return True if able to resume capture, false otherwise.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual bool Resume() { mPauseFlag = false; return true; }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief This method is used to scan for video devices on the system.
            ///
            ///   \param[out] sources Video sources on the computer.
            ///
            ///   \return Number of sources detected.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static unsigned int ScanForSources(std::vector<std::string>& sources);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief This method is used to de-interlace an interlaced image using
            ///   interpolation.
            ///
            ///   \param[in] src Source image data [3 channels RGB].
            ///   \param[out] dest The resulting de-interlaced image (must be pre-allocated).
            ///   \param[in] width Image width in pixels.
            ///   \param[in] height Image height in pixels.
            ///
            ///   \return True on success, false on failure.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            static inline bool DeInterlaceData(unsigned char* src,
                                               unsigned char* dest,
                                               unsigned short width,
                                               unsigned short height)
            {
                unsigned char* sPtr;
                unsigned char* dPtr;
                unsigned char* dRowUp, * dRowDown;
                unsigned int widthStep = width*3;
                if(src == NULL || dest == NULL || width == 0 || height == 0)
                {
                    return false;
                }
                for (int i = (int)(height - 1); i > 0; i--)
                {
                    sPtr = (unsigned char *)(src + (height - i)*widthStep);
                    //image was flipped -gary
                    //dPtr = (unsigned char *)(dest + i*width*3);
                    dPtr = (unsigned char *)(dest + (height - i)*widthStep);

                    //  Perform interlacing so that the video appears clear.  This is
                    //  only needed if the video is DV in most cases, which arrives
                    //  interlaced.
                    //For lines that are very top or very bottom we just copy or it is a scanline to use
                    if ((i - 1 < 0) || (i + 1 >= height) || ((unsigned int)i % 2 == 0) || i==1)
                    {
                        //Just copy
                        //memcpy(dPtr, sPtr, widthStep);
                        for (int j = 0; j < width; j++)
                        {
                            *dPtr++ = *sPtr++; // B
                            *dPtr++ = *sPtr++; // G
                            *dPtr++ = *sPtr++; // R
                        }
                    }
                    else
                    {
                        dRowUp = sPtr - widthStep;
                        dRowDown = sPtr + widthStep;

                        //Interpolate!!
                        for (int j = 0; j < width; j++)
                        {
                            *dPtr++ = (*dRowUp++ + *dRowDown++) / 2;  //  B
                            *dPtr++ = (*dRowUp++ + *dRowDown++) / 2;  //  G
                            *dPtr++ = (*dRowUp++ + *dRowDown++) / 2;  //  R
                        }
                    }
                }
                return true;
            }
            static inline void FlipChannels(Image& image)
            {
                if(image.mChannels == 3)
                {
                    unsigned char *ptr = image.mpData;
                    unsigned char temp;
                    for(unsigned int i = 0; i < ((unsigned int)(image.mWidth*image.mHeight*image.mChannels)); i+= 3)
                    {
                        temp = *ptr;
                        *ptr = *(ptr + 2);
                        *(ptr + 2) = temp;
                        ptr += 3;
                    }
                }
            }
            virtual bool RegisterCallback(Callback* cb);
            virtual bool RegisterRawCallback(RawCallback* cb);
#ifdef WIN32
            void RegisterCallback(void (*func)(Zebulon::Video::CvImageCapture::Image*, void*), void *);
#endif
            virtual bool RemoveCallback(Callback* cb);
            virtual bool RemoveRawCallback(RawCallback* cb);
            virtual bool SetID(unsigned char id);
            // Very ugly way to access internal video camera if needed
            // not not use unless you know what you are doing
            void *GetRawCapture();
        private:
            volatile bool mPauseFlag;   ///<  If true, pause capture/callbacks.
            Image mImage;               ///<  Image received.
            Image mRawImage;            ///<  Raw image data received.
            unsigned char mID;          ///< Camera ID.
            void* mpCapture;            ///<  Pointer to capture interface.
            volatile bool mInterlacedFlag; ///< Used to determine if image should be de-interlaced.
            std::string mSettingsFilename; ///<  Name of settings file to pass to interface on start.
            // Callback system
            bool RunCallback();
            std::set<Callback*> mCallbacks;
            std::set<RawCallback*> mRawCallbacks;
#ifndef WIN32
            //wow these names could be better -gary
            static void CaptureThreadFunction(void *args);
            void CaptureFunction();
            CxUtils::Thread mCaptureThread;     ///<  Threat for continuous capture from the camera/device.
            CxUtils::Mutex mMutex;              ///<  For thread protection during capture for callbacks.
#endif
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class CvCaptureBase
        ///   \brief Base class for creating video device interfaces.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CV_IMAGE_CAPTURE_DLL CvCaptureBase
        {
        public:
            friend class CvImageCapture;

            CvCaptureBase() { mStopCaptureFlag = false; }
            virtual ~CvCaptureBase() {}
            virtual bool SupportsRawFrame() const { return false; }
            virtual bool LoadSettings(const std::string& filename) { return true; }
            virtual int Start(const std::string& videoSource,   // Name of device.
                              const int captureWidth = 0,       // Desired capture resolution (0 is default)
                              const int captureHeight = 0,      // Desired capture resolution (0 is default)
                              const bool interlace = false) = 0;// Is the image data interlaced?
            virtual int Stop() = 0;
            virtual bool IsReady() const = 0;
            virtual int GetFrame(CvImageCapture::Image* dest, const bool block = false) = 0;
            virtual int GetRawFrame(CvImageCapture::Image* dest, const bool block = false) = 0;
        protected:
            volatile bool mStopCaptureFlag;  ///<  If true, stop capture.
        };


        #if !defined(WIN32) && !defined(WIN64)
        #ifndef CV_USE_OPEN_CV
            #define CV_USE_OPEN_CV
        #endif
        #endif

        // Someone added duplicate function (it helps to look at the whole file)!
        inline bool SwapRGBImage(CvImageCapture::Image& image)
        {
            // Flip the color channels to get RGB, RGB, ... instead of BGR, BGR...
            CvImageCapture::FlipChannels(image);
            return true;
        }

        #ifdef CV_USE_OPEN_CV
        // Method to convert from CvImageCapture::Image to IplImage
        inline bool CvConvertToIplImage(const CvImageCapture::Image& in, IplImage** out)
        {
            if(in.mpData != NULL)
            {
                if(*out == NULL ||
                   (*out)->width != (int)in.mWidth ||
                   (*out)->height != (int)in.mHeight ||
                   (*out)->nChannels != (int)in.mChannels)
                {
                    if(*out)
                    {
                        cvReleaseImage(out);
                    }
                    *out = cvCreateImage(cvSize((int)in.mWidth, (int)in.mHeight), IPL_DEPTH_8U, 3);
                }

                // Copy the data.
                memcpy((*out)->imageData, in.mpData, (*out)->imageSize);
                return true;
            }

            return false;
        }
        inline bool CvConvertToIplImage(const CxUtils::Image& in, IplImage** out)
        {
            if(in.mpImage != NULL)
            {
                if(*out == NULL ||
                   (*out)->width != (int)in.mWidth ||
                   (*out)->height != (int)in.mHeight ||
                   (*out)->nChannels != (int)in.mChannels)
                {
                    if(*out)
                    {
                        cvReleaseImage(out);
                    }
                    *out = cvCreateImage(cvSize((int)in.mWidth, (int)in.mHeight), IPL_DEPTH_8U, 3);
                }

                // Copy the data.
                memcpy((*out)->imageData, in.mpImage, (*out)->imageSize);
                return true;
            }

            return false;
        }

        #endif
    }
}
#endif // _CV_IMAGE_CAPTURE_H

/*  End of File */
