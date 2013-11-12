////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturev4l.cpp
///
///  Copyright 2012 David Adams
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
#include "cvcapture/imagecapturepylon.h"

#ifndef WIN32
#include <cxutils/fileio.h>
#include <pylon/PylonIncludes.h>
#include <pylon/gige/BaslerGigECamera.h>
#include <opencv/highgui.h>
#include "cvcapture/imagecaptureutils.h"

using namespace std;
using namespace Zebulon;
using namespace Video;
using namespace Pylon;
using namespace Basler_GigECameraParams;
using namespace Basler_GigEStreamParams;

typedef Pylon::CBaslerGigECamera Camera_t;
typedef Camera_t::StreamGrabber_t StreamGrabber_t;

namespace Zebulon
{
    namespace Video
    {
        /** Internal class for buffer data. */
        class PylonGrabBuffer
        {
        public:
            PylonGrabBuffer(const size_t ImageSize);
            ~PylonGrabBuffer();
            uint8_t* GetBufferPointer(void) { return m_pBuffer; }
            StreamBufferHandle GetBufferHandle(void) { return m_hBuffer; }
            void SetBufferHandle(StreamBufferHandle hBuffer) { m_hBuffer = hBuffer; }
        protected:
            uint8_t *m_pBuffer;
            StreamBufferHandle m_hBuffer;
        };
    }
}

/** Constructor allocates the image buffer. */
Zebulon::Video::PylonGrabBuffer::PylonGrabBuffer(const size_t ImageSize):
    m_pBuffer(NULL)
{
    m_pBuffer = new uint8_t[ ImageSize ];
    if (NULL == m_pBuffer)
    {
        GenICam::GenericException e("Not enough memory to allocate image buffer", __FILE__, __LINE__);
        throw e;
    }
}

/** Freeing the memory */
Zebulon::Video::PylonGrabBuffer::~PylonGrabBuffer()
{
    if (NULL != m_pBuffer)
        delete[] m_pBuffer;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCapturePylon::ImageCapturePylon()
{
    //    mpCamera = NULL;
    //    mBuffers = 10;
    //    mImagesToGrab = 100;
    mFlipY = true;
    mFlipX = false;
    mState = 0;
    mDefaultWidth = 1280;
    mDefaultHeight = 960;
    mpCamera = NULL;
    mpStream = NULL;
    mExposureTime = 5000;
    mPacketSize = 8192;
    mWhiteBalanceMode = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCapturePylon::~ImageCapturePylon()
{

}


/** Loads settings from a config file. */
bool ImageCapturePylon::LoadSettings(const string &filename)
{
    bool result = false;

    std::fstream f;

    f.open(filename.c_str(), std::ios_base::in);

    if(f.is_open())
    {
        while(f.eof() == false)
        {
            std::string line;
            CxUtils::FileIO::ReadLine(f, line);
            std::vector<std::string> tokens = CxUtils::FileIO::Tokenize(line, ",");

            if(tokens.size() >= 2)
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);

                if(tokens[0].c_str()[0] == '#')
                {
                    continue;
                }
                if(tokens[0].find("Exposure Time") != std::string::npos)
                {
                    result = true;
                    SetExposureTime(atoi(tokens[1].c_str()));
                }
                if(tokens[0].find("Packet Size") != std::string::npos)
                {
                    result = true;
                    mPacketSize = atoi(tokens[1].c_str());
                }
                if(tokens[0].find("White Balance Mode") != std::string::npos)
                {
                    result = true;
                    SetWhiteBalanceMode(atoi(tokens[1].c_str()) );
                }
                if(tokens[0].find("Flip Y") != std::string::npos)
                {
                    result = true;
                    mFlipY = atoi(tokens[1].c_str()) > 0 ? true : false;
                }
                if(tokens[0].find("Flip X") != std::string::npos)
                {
                    result = true;
                    mFlipX = atoi(tokens[1].c_str()) > 0 ? true : false;
                }
            }
        }
    }
    //CxUtils::FileIO::ReadLine

    return result;
}


///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to start capture from a video source.
///
///   \param[in] videoSource Camera/Video source name. Must be in format of
///              "pylon-#" Where # is the device number, (e.g. 0, 1, 2).
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
int ImageCapturePylon::Start(const std::string& videoSource,
                             const int width,
                             const int height,
                             const bool interlace)
{

    // Lookup device num from videoSource
    mDeviceNum = 0;
    if(sscanf(videoSource.c_str(), "pylon-%d", &mDeviceNum) < 1)
    {
        // Failed to parse string
        return 0;
    }
    mWidth = width;
    mHeight = height;

    // Buffers for grabbing
    uint32_t c_nBuffers = 10;
    // Init images grabbed to date
    mFrameNumber = 0;
    mState = 0;

    // Get the transport layer factory
    CTlFactory& TlFactory = CTlFactory::GetInstance();

    // Create the transport layer object needed to enumerate or
    //  create a camera object of type Camera_t::DeviceClass()
    ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());

    // Exit application if the specific transport layer is not available
    if (! pTl)
    {
        cerr << "Failed to create transport layer!" << endl;
        mState = -1;
        return 0;
    }

    // Get all attached cameras and exit application if no camera is found
    DeviceInfoList_t devices;

    // Enumerate devices.
    if (0 == pTl->EnumerateDevices(devices))
    {
        cerr << "No camera present!" << endl;
        mState = -1;
        return false;
    }

    // Try connect to the specific device defined, if in range.
    if(devices.size() > 0 && mDeviceNum > devices.size())
    {
        // Invalid device number.
        return false;
    }
    mpCamera = new Camera_t(TlFactory.CreateDevice(devices[ mDeviceNum ]));

    // Open the camera
    ( (Camera_t*)(mpCamera))->Open();

    // Get the first stream grabber object of the selected camera
    mpStream = new StreamGrabber_t(( (Camera_t*)(mpCamera))->GetStreamGrabber(0));

    // Open the stream grabber
    ( (StreamGrabber_t*)(mpStream))->Open();

    // Set the image format and AOI
    // Options =============================
    //( (Camera_t*)(mpCamera))->PixelFormat.SetValue(PixelFormat_YUV422Packed);
    //( (Camera_t*)(mpCamera))->PixelFormat.SetValue(PixelFormat_BayerBG12);
    //( (Camera_t*)(mpCamera))->OffsetX.SetValue(0);
    //( (Camera_t*)(mpCamera))->OffsetY.SetValue(0);

    ( (Camera_t*)(mpCamera))->PixelFormat.SetValue(PixelFormat_BayerBG8);

    // Not sure why this is not used.
    //int _CamWidth = ( (Camera_t*)(mpCamera))->Width.GetMax();
    //int _CamHeight = ( (Camera_t*)(mpCamera))->Height.GetMax();
    // If desired values are not good, use pre-defined value.
    int _CamWidth = mDefaultWidth;
    int _CamHeight = mDefaultHeight;
    if(mWidth == 0 )
    {
        mWidth = _CamWidth;
    }
    if(mHeight == 0)
    {
        mHeight = _CamHeight;
    }

    // Set capture values
    ( (Camera_t*)(mpCamera))->Width.SetValue(_CamWidth);
    ( (Camera_t*)(mpCamera))->Height.SetValue(_CamHeight);

    // TODO
    // Need to check this value for flipping...
    //( (Camera_t*)(mpCamera))->ReverseY(true);
    //( (Camera_t*)(mpCamera))->ReverseY.SetValue(true,false);

    mImage.Create(mWidth,mHeight,3);
    mRawImage.Create(_CamWidth,_CamHeight,3);
    //std::cout << "Height " << _CamHeight << "Width " << _CamWidth << std::endl;
    //  Disable acquisition start trigger if available
    {
        GenApi::IEnumEntry* acquisitionStart = ( (Camera_t*)(mpCamera))->TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
        if ( acquisitionStart && GenApi::IsAvailable( acquisitionStart))
        {
            ( (Camera_t*)(mpCamera))->TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
            ( (Camera_t*)(mpCamera))->TriggerMode.SetValue( TriggerMode_Off);
        }
    }

    //  Disable frame start trigger if available
    {
        GenApi::IEnumEntry* frameStart = ( (Camera_t*)(mpCamera))->TriggerSelector.GetEntry( TriggerSelector_FrameStart);
        if ( frameStart && GenApi::IsAvailable( frameStart))
        {
            ( (Camera_t*)(mpCamera))->TriggerSelector.SetValue( TriggerSelector_FrameStart);
            ( (Camera_t*)(mpCamera))->TriggerMode.SetValue( TriggerMode_Off);
        }
    }

    //Set exposure settings
    //( (Camera_t*)(mpCamera))->ExposureAuto.SetValue(ExposureAuto_Off);
    //( (Camera_t*)(mpCamera))->ExposureMode.SetValue(ExposureMode_Timed);
    //set cam exposure
    //( (Camera_t*)(mpCamera))->ExposureTimeRaw.SetValue(18000);

    //  This enables jumbo frames
    ( (Camera_t*)(mpCamera))->GevSCPSPacketSize.SetValue(mPacketSize);

    // Set timed exposure.
    ( (Camera_t*)(mpCamera))->ExposureMode.SetValue(ExposureMode_Timed);
    ( (Camera_t*)(mpCamera))->ExposureTimeRaw.SetValue(mExposureTime);


    // Set white balance mode
    switch(mWhiteBalanceMode)
    {
    case 1:
        ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);
        break;
    case 2:
        ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Continuous);
        break;
    default:
        ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Off);
        break;
    }

    // Get the image buffer size
    const size_t ImageSize = (size_t)(( (Camera_t*)(mpCamera))->PayloadSize.GetValue());

    // We won't use image buffers greater than ImageSize
    ( (StreamGrabber_t*)(mpStream))->MaxBufferSize.SetValue(ImageSize);

    // We won't queue more than c_nBuffers image buffers at a time
    ( (StreamGrabber_t*)(mpStream))->MaxNumBuffer.SetValue(c_nBuffers);

    // Allocate all resources for grabbing. Critical parameters like image
    // size now must not be changed until FinishGrab() is called.
    ( (StreamGrabber_t*)(mpStream))->PrepareGrab();


    // Buffers used for grabbing must be registered at the stream grabber.
    // The registration returns a handle to be used for queuing the buffer.
    for (uint32_t i = 0; i < c_nBuffers; ++i)
    {
        PylonGrabBuffer *pGrabBuffer = new PylonGrabBuffer(ImageSize);
        pGrabBuffer->SetBufferHandle(( (StreamGrabber_t*)(mpStream))->RegisterBuffer(
                                         pGrabBuffer->GetBufferPointer(), ImageSize));

        // Put the grab buffer object into the buffer list
        mBufferList.push_back(pGrabBuffer);
    }

    for (std::vector<PylonGrabBuffer*>::const_iterator x = mBufferList.begin(); x != mBufferList.end(); ++x)
    {
        // Put buffer into the grab queue for grabbing
        ( (StreamGrabber_t*)(mpStream))->QueueBuffer((*x)->GetBufferHandle(), NULL);
    }

    // Let the camera acquire images continuously ( Acquisiton mode equals
    // Continuous! )
    ( (Camera_t*)(mpCamera))->AcquisitionStart.Execute();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops video capture.
///
///   \return 1 on succes, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCapturePylon::Stop()
{
    if(mpCamera && mpStream)
    {
        // Stop acquisition
        ( (Camera_t*)(mpCamera))->AcquisitionStop.Execute();

        // Clean up
        mState = 0;
        // You must deregister the buffers before freeing the memory
        for (std::vector<PylonGrabBuffer*>::iterator it = mBufferList.begin(); it != mBufferList.end(); it++)
        {
            ( (StreamGrabber_t*)(mpStream))->DeregisterBuffer((*it)->GetBufferHandle());
            delete *it;
            *it = NULL;
        }
        mBufferList.clear();
        // Free all resources used for grabbing
        ( (StreamGrabber_t*)(mpStream))->FinishGrab();

        // Close stream grabber
        ( (StreamGrabber_t*)(mpStream))->Close();

        // Close camera
        ( (Camera_t*)(mpCamera))->Close();

        delete ( (Camera_t*)(mpCamera));
        delete ( (StreamGrabber_t*)(mpStream));
        mpCamera = mpStream = NULL;
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get the current frame from the camera/video
///          source.
///
///   \param[out] Pointer to image structure to fill with current frame.
///               If frame size is non-zero, those scale values are used,
///               otherwise the size from capture start is used.
///   \param[in] block If true, this method will block until a new frame arrives
///              otherwise it will fall through if a new frame isn't available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ImageCapturePylon::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    if(mpCamera == NULL || mpStream == NULL)
    {
        return 0;
    }

    CxUtils::Mutex::ScopedLock lock(&mMutex);
    // Wait for the grabbed image with timeout of 3 seconds
    if (( (StreamGrabber_t*)(mpStream))->GetWaitObject().Wait(3000))
    {
        // Get the grab result from the grabber's result queue
        GrabResult Result;
        ( (StreamGrabber_t*)(mpStream))->RetrieveResult(Result);

        if (Grabbed == Result.Status())
        {
            mFrameNumber++;
            // Grabbing was successful, process image

            // Get the pointer to the image buffer
            const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();

            // Should not be making copy of data...
            if(dest->mWidth == 0 || dest->mHeight == 0)
            {
                dest->Create(mImage.mWidth, mImage.mHeight, 3);
            }
            const cv::Mat original(Result.GetSizeY(),Result.GetSizeX(), CV_8UC1, (uint8_t*)pImageBuffer);
            cv::Mat scaledImage(dest->mHeight, dest->mWidth, CV_8UC3, dest->mpData);
            cv::Mat colorConvert(mRawImage.mHeight, mRawImage.mWidth, CV_8UC3, mRawImage.mpData);


            cv::cvtColor(original, colorConvert, CV_BayerBG2RGB);
            cv::resize(colorConvert, scaledImage, scaledImage.size());

            if(mFlipY && mFlipX)
            {
                cv::flip(scaledImage, scaledImage, -1);
            }
            else if(mFlipY)
            {
                cv::flip(scaledImage, scaledImage, 0);
            }
            else if(mFlipX)
            {
                cv::flip(scaledImage, scaledImage, 1);
            }

            // Reuse the buffer for grabbing the next image
            ( (StreamGrabber_t*)(mpStream))->QueueBuffer(Result.Handle(), NULL);
            mState = 1;

            return 1;
        }
        else if (Failed == Result.Status())
        {
            /*
            mState = -1;
            // Error handling
            cerr << "No image acquired!" << endl;
            cerr << "Error code : 0x" << hex
                 << Result.GetErrorCode() << endl;
            cerr << "Error description : "
                 << Result.GetErrorDescription() << endl;
            */
            // Reuse the buffer for grabbing the next image
            ( (StreamGrabber_t*)(mpStream))->QueueBuffer(Result.Handle(), NULL);
        }
    }

    return 0;
    /*
    int result = 0;
    mGrabbedNumber = mFrameNumber;//
    if(mFrameNumber > 0 && mState > 0)
    {
        mMutex.Lock();
        *dest = mImage;
        mMutex.Unlock();
        result = 1;
    }
    return result;
    */
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
int ImageCapturePylon::GetRawFrame(CvImageCapture::Image* dest, const bool block)
{
    if(mpCamera == NULL || mpStream == NULL)
    {
        return 0;
    }

    CxUtils::Mutex::ScopedLock lock(&mMutex);
    // Wait for the grabbed image with timeout of 3 seconds
    if (( (StreamGrabber_t*)(mpStream))->GetWaitObject().Wait(3000))
    {
        // Get the grab result from the grabber's result queue
        GrabResult Result;
        ( (StreamGrabber_t*)(mpStream))->RetrieveResult(Result);

        if (Grabbed == Result.Status())
        {
            mFrameNumber++;
            // Grabbing was successful, process image

            // Get the pointer to the image buffer
            const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();

            // Should not be making copy of data...
            if(dest->mWidth != mRawImage.mWidth || dest->mHeight != mRawImage.mHeight)
            {
                dest->Create(mRawImage.mWidth, mRawImage.mHeight, 3);
            }
            const cv::Mat original(Result.GetSizeY(),Result.GetSizeX(),CV_8UC1,(uint8_t*)pImageBuffer);
            cv::Mat colorConvert(dest->mHeight,dest->mWidth,CV_8UC3,dest->mpData);


            cv::cvtColor(original, colorConvert, CV_BayerBG2RGB);

            if(mFlipY && mFlipX)
            {
                cv::flip(colorConvert, colorConvert, -1);
            }
            else if(mFlipY)
            {
                cv::flip(colorConvert, colorConvert, 0);
            }
            else if(mFlipX)
            {
                cv::flip(colorConvert, colorConvert, 1);
            }

            // Reuse the buffer for grabbing the next image
            ( (StreamGrabber_t*)(mpStream))->QueueBuffer(Result.Handle(), NULL);
            mState = 1;

            return 1;
        }
        else if (Failed == Result.Status())
        {
            /*
            mState = -1;
            // Error handling
            cerr << "No image acquired!" << endl;
            cerr << "Error code : 0x" << hex
                 << Result.GetErrorCode() << endl;
            cerr << "Error description : "
                 << Result.GetErrorDescription() << endl;
            */
            // Reuse the buffer for grabbing the next image
            ( (StreamGrabber_t*)(mpStream))->QueueBuffer(Result.Handle(), NULL);
        }
    }

    return 0;
    /*
    int result = 0;
    mGrabbedNumber = mFrameNumber;//
    if(mFrameNumber > 0 && mState > 0)
    {
        mMutex.Lock();
        *dest = mRawImage;
        mMutex.Unlock();
        result = 1;
    }
    return result;
    */
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCapturePylon::IsReady() const
{
    return mState != -1;
}


///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the exposure time for the camera.
///
///   \param[in] exposureTime Exposure time [16, 100000000].
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCapturePylon::SetExposureTime(const int exposureTime)
{
    bool result = false;

    if(exposureTime >= 16 && exposureTime <= 100000000)
    {
        CxUtils::Mutex::ScopedLock lock(&mMutex);
        mExposureTime = exposureTime;
        // Set timed exposure.
        if(mpCamera && mpStream)
        {
            //( (Camera_t*)(mpCamera))->ExposureMode.SetValue(ExposureMode_Timed);
            ( (Camera_t*)(mpCamera))->ExposureTimeRaw.SetValue(mExposureTime);
        }
    }

    return result;
}


///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the auto white balance mode.
///
///   \param[in] mode White balance mode (see enum), 0 = Off, 1 = Once,
///                   2 = Continuous.  Not all values may be supported by device.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ImageCapturePylon::SetWhiteBalanceMode(const int mode)
{
    bool result = false;

    CxUtils::Mutex::ScopedLock lock(&mMutex);

    mWhiteBalanceMode = mode;
    if(mpCamera)
    {
        // Set white balance mode
        switch((ImageCapturePylon::WhiteBalanceMode)mWhiteBalanceMode)
        {
        case Once:
            ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Once);
            break;
        case Continuous:
            ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Continuous);
            break;
        default:
            ( (Camera_t*)(mpCamera))->BalanceWhiteAuto.SetValue(BalanceWhiteAuto_Off);
            break;
        }

        result = true;
    }

    return result;
}
#if 0

void ImageCapturePylon::CaptureThread(void * arg)
{
    ImageCapturePylon* ic = (ImageCapturePylon*)arg;

    // Buffers for grabbing
    uint32_t c_nBuffers = 10;
    // Number of images to be grabbed

    ic->mFrameNumber = 0;
    Pylon::PylonAutoInitTerm autoInitTerm;

    // Get the transport layer factory
    CTlFactory& TlFactory = CTlFactory::GetInstance();


    // Create the transport layer object needed to enumerate or
    // create a camera object of type Camera_t::DeviceClass()
    /*Pylon::String_t class_ = Camera_t::DeviceClass();
    std::cout << "Class: \'" << class_ << "\'" << std::endl;//*/
    /*ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());
    // Exit application if the specific transport layer is not available
    if (! pTl)
    {
        cerr << "Failed to create transport layer!" << endl;
        //pressEnterToExit();
        ic->mState = -1;
        return;
    }//*/

    // Create the transport layer object needed to enumerate or
    //  create a camera object of type Camera_t::DeviceClass()
    ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());

    // Exit application if the specific transport layer is not available
    if (! pTl)
    {
        cerr << "Failed to create transport layer!" << endl;
        ic->mState = -1;
        return;
    }

    // Get all attached cameras and exit application if no camera is found
    DeviceInfoList_t devices;


    /*TlInfoList_t infolist;
    TlFactory.CreateTl(Camera_t::DeviceClass());
    TlFactory.EnumerateTls(infolist);
    TlInfoList_t::iterator iter;
    for (iter=infolist.begin(); iter!=infolist.end(); iter++)
    {
        //TlFactory.ReleaseTl(*iter);
        std::cout << iter->GetFullName() << std::endl;
    }//*/

    /*TlFactory.EnumerateDevices(devices);
    if (devices.empty())//*/
    if (0 == pTl->EnumerateDevices(devices))
    {
        cerr << "No camera present!" << endl;
        ic->mState = -1;
        return;
    }

    // Create the camera object of the first available camera.
    // The camera object is used to set and get all available
    // camera features.
    //Camera_t Camera(pTl->CreateDevice(devices[ 0 ]));
    Camera_t Camera(TlFactory.CreateDevice(devices[ 0 ]));

    // Open the camera
    Camera.Open();

    // Get the first stream grabber object of the selected camera
    Camera_t::StreamGrabber_t StreamGrabber(Camera.GetStreamGrabber(0));

    // Open the stream grabber
    StreamGrabber.Open();

    // Set the image format and AOI
    //Camera.PixelFormat.SetValue(PixelFormat_YUV422Packed);
    Camera.PixelFormat.SetValue(PixelFormat_BayerBG8);
    //Camera.PixelFormat.SetValue(PixelFormat_BayerBG12);
    //Camera.OffsetX.SetValue(0);
    //Camera.OffsetY.SetValue(0);
//    int _CamWidth = Camera.Width.GetMax();
//    int _CamHeight = Camera.Height.GetMax();
    int _CamWidth = 1280;
    int _CamHeight = 960;
    if(ic->mWidth == 0 ) ic->mWidth = _CamWidth;
    if(ic->mHeight == 0) ic->mHeight = _CamHeight;


    Camera.Width.SetValue(_CamWidth);
    Camera.Height.SetValue(_CamHeight);
    //Camera.ReverseY(true);
    //Camera.ReverseY.SetValue(true,false);

    ic->mImage.Create(ic->mWidth,ic->mHeight,3);
    ic->mRawImage.Create(_CamWidth,_CamHeight,3);
    //std::cout << "Height " << _CamHeight << "Width " << _CamWidth << std::endl;
    //Disable acquisition start trigger if available
    {
        GenApi::IEnumEntry* acquisitionStart = Camera.TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
        if ( acquisitionStart && GenApi::IsAvailable( acquisitionStart))
        {
            Camera.TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
            Camera.TriggerMode.SetValue( TriggerMode_Off);
        }
    }

    //Disable frame start trigger if available
    {
        GenApi::IEnumEntry* frameStart = Camera.TriggerSelector.GetEntry( TriggerSelector_FrameStart);
        if ( frameStart && GenApi::IsAvailable( frameStart))
        {
            Camera.TriggerSelector.SetValue( TriggerSelector_FrameStart);
            Camera.TriggerMode.SetValue( TriggerMode_Off);
        }
    }
    //Set acquisition mode
    Camera.AcquisitionMode.SetValue(AcquisitionMode_Continuous);

    //Set exposure settings
    //Camera.ExposureAuto.SetValue(ExposureAuto_Off);
    //Camera.ExposureMode.SetValue(ExposureMode_Timed);
    //set cam exposure
    //Camera.ExposureTimeRaw.SetValue(18000);
    //this enables jumbo frames
    Camera.GevSCPSPacketSize.SetValue(8192);
    if(ic->mFlipX)
    {
       // cv::flip(scaledest,scaledest,0);
    }

    // Get the image buffer size
    const size_t ImageSize = (size_t)(Camera.PayloadSize.GetValue());

    // We won't use image buffers greater than ImageSize
    StreamGrabber.MaxBufferSize.SetValue(ImageSize);

    // We won't queue more than c_nBuffers image buffers at a time
    StreamGrabber.MaxNumBuffer.SetValue(c_nBuffers);

    // Allocate all resources for grabbing. Critical parameters like image
    // size now must not be changed until FinishGrab() is called.
    StreamGrabber.PrepareGrab();


    // Buffers used for grabbing must be registered at the stream grabber.
    // The registration returns a handle to be used for queuing the buffer.
    std::vector<PylonGrabBuffer*> BufferList;
    for (uint32_t i = 0; i < c_nBuffers; ++i)
    {
        PylonGrabBuffer *pGrabBuffer = new PylonGrabBuffer(ImageSize);
        pGrabBuffer->SetBufferHandle(StreamGrabber.RegisterBuffer(
                                         pGrabBuffer->GetBufferPointer(), ImageSize));

        // Put the grab buffer object into the buffer list
        BufferList.push_back(pGrabBuffer);
    }

    for (std::vector<PylonGrabBuffer*>::const_iterator x = BufferList.begin(); x != BufferList.end(); ++x)
    {
        // Put buffer into the grab queue for grabbing
        StreamGrabber.QueueBuffer((*x)->GetBufferHandle(), NULL);
    }

    // Let the camera acquire images continuously ( Acquisiton mode equals
    // Continuous! )
    Camera.AcquisitionStart.Execute();

    // Grab c_ImagesToGrab times
    //    for (int n = 0; n < c_ImagesToGrab; n++)
    //    {
    unsigned int gStartTime = time(NULL);

    while(true)
    //while(!ic->mCaptureThread.QuitThreadFlag())
    {
        //CxUtils::SleepMs(10);

        //        try
        //        {

        // Wait for the grabbed image with timeout of 3 seconds
        if (StreamGrabber.GetWaitObject().Wait(3000))
        {
            // Get the grab result from the grabber's result queue
            GrabResult Result;
            StreamGrabber.RetrieveResult(Result);

            if (Grabbed == Result.Status())
            {
                ic->mFrameNumber++;
                // Grabbing was successful, process image
                //cout << "Image #" << ic->mFrameNumber << " acquired!" << endl;
                //cout << "Size: " << Result.GetSizeX() << " x "
               //      << Result.GetSizeY() << endl;

                // Get the pointer to the image buffer
                const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();
                //cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0]
                //     << endl << endl;
                //ic->mImage.mWidth =  Result.GetSizeX();
                //ic->mImage.mHeight =  Result.GetSizeY();

                ic->mMutex.Lock();
                {
//                    ic->mFrameNumber++;
//                    ic->mRawImage.mWidth = 1280;
//                    ic->mRawImage.mHeight = 960;
//                    ic->mRawImage.mChannels = 1;
//                    ic->mRawImage.mpData = (uint8_t*)pImageBuffer;
//                    HalfBG2RGB(&ic->mRawImage,&ic->mImage,false,true);
//                    std::cout << "Captured frame "
//                              << ic->mFrameNumber << " at ["
//                              << Result.GetSizeX() << " x "
//                              << Result.GetSizeY() << "] - "
//                              << ( (( time(NULL) - gStartTime ) > 0 ) ? (double)ic->mFrameNumber/(time(NULL) - gStartTime) : 0 )
//                              << std::endl;
                    //ic->mRawImage.data = (uint8_t*)pImageBuffer;

                    const cv::Mat original(Result.GetSizeY(),Result.GetSizeX(),CV_8UC1,(uint8_t*)pImageBuffer);

                    cv::Mat scaledest(ic->mImage.mHeight,ic->mImage.mWidth,CV_8UC3,ic->mImage.mpData);
                    cv::Mat dest(ic->mRawImage.mHeight,ic->mRawImage.mWidth,CV_8UC3,ic->mRawImage.mpData);


                    cv::cvtColor(original, dest, CV_BayerBG2RGB);
                    cv::resize(dest,scaledest,scaledest.size());

                    if(ic->mFlipY)
                    {
                        cv::flip(scaledest,scaledest,0);
                    }
                    //cv::imshow("cvcapture: ",scaledest);
                    //cv::waitKey(1);



                }
                ic->mMutex.Unlock();

                // Reuse the buffer for grabbing the next image
                //if (ic->mFrameNumber < c_ImagesToGrab - c_nBuffers)
                StreamGrabber.QueueBuffer(Result.Handle(), NULL);
                ic->mState = 1;
            }
            else if (Failed == Result.Status())
            {
                ic->mState = -1;
                // Error handling
                cerr << "No image acquired!" << endl;
                cerr << "Error code : 0x" << hex
                     << Result.GetErrorCode() << endl;
                cerr << "Error description : "
                     << Result.GetErrorDescription() << endl;

                // Reuse the buffer for grabbing the next image
                //if (ic->mFrameNumber < c_ImagesToGrab - c_nBuffers)
                    StreamGrabber.QueueBuffer(Result.Handle(), NULL);
            }
        }
        else
        {
            ic->mState = -1;
            // Timeout
            cerr << "Timeout occurred!" << endl;

            // Get the pending buffer back (You are not allowed to deregister
            // buffers when they are still queued)
            StreamGrabber.CancelGrab();

            // Get all buffers back
            for (GrabResult r; StreamGrabber.RetrieveResult(r););

            // Cancel loop
            break;
        }
    }
    //        catch (GenICam::GenericException &e)
    //        {
    //            // Error handling
    //            cerr << "An exception occurred!" << endl

    //                 << e.GetDescription() << endl;
    //            //pressEnterToExit();
    //            return 1;
    //        }
    // Stop acquisition
    Camera.AcquisitionStop.Execute();

    // Clean up
    ic->mState = 0;
    // You must deregister the buffers before freeing the memory
    for (std::vector<PylonGrabBuffer*>::iterator it = BufferList.begin(); it != BufferList.end(); it++)
    {
        StreamGrabber.DeregisterBuffer((*it)->GetBufferHandle());
        delete *it;
        *it = NULL;
    }

    // Free all resources used for grabbing
    StreamGrabber.FinishGrab();

    // Close stream grabber
    StreamGrabber.Close();

    // Close camera
    Camera.Close();
}
//void ImageCapturePylon::CaptureThread(void * arg)
//{
//    ImageCapturePylon* ic = (ImageCapturePylon*)arg;

//    // Buffers for grabbing
//    uint32_t c_nBuffers = 10;
//    // Number of images to be grabbed

//    ic->mFrameNumber = 0;
//    Pylon::PylonAutoInitTerm autoInitTerm;

//    // Get the transport layer factory
//    CTlFactory& TlFactory = CTlFactory::GetInstance();


//    // Create the transport layer object needed to enumerate or
//    // create a camera object of type Camera_t::DeviceClass()
//    /*Pylon::String_t class_ = Camera_t::DeviceClass();
//    std::cout << "Class: \'" << class_ << "\'" << std::endl;//*/
//    /*ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());
//    // Exit application if the specific transport layer is not available
//    if (! pTl)
//    {
//        cerr << "Failed to create transport layer!" << endl;
//        //pressEnterToExit();
//        ic->mState = -1;
//        return;
//    }//*/

//    // Create the transport layer object needed to enumerate or
//    //  create a camera object of type Camera_t::DeviceClass()
//    ITransportLayer *pTl = TlFactory.CreateTl(Camera_t::DeviceClass());

//    // Exit application if the specific transport layer is not available
//    if (! pTl)
//    {
//        cerr << "Failed to create transport layer!" << endl;
//        ic->mState = -1;
//        return;
//    }

//    // Get all attached cameras and exit application if no camera is found
//    DeviceInfoList_t devices;


//    /*TlInfoList_t infolist;
//    TlFactory.CreateTl(Camera_t::DeviceClass());
//    TlFactory.EnumerateTls(infolist);
//    TlInfoList_t::iterator iter;
//    for (iter=infolist.begin(); iter!=infolist.end(); iter++)
//    {
//        //TlFactory.ReleaseTl(*iter);
//        std::cout << iter->GetFullName() << std::endl;
//    }//*/

//    /*TlFactory.EnumerateDevices(devices);
//    if (devices.empty())//*/
//    if (0 == pTl->EnumerateDevices(devices))
//    {
//        cerr << "No camera present!" << endl;
//        ic->mState = -1;
//        return;
//    }

//    // Create the camera object of the first available camera.
//    // The camera object is used to set and get all available
//    // camera features.
//    //Camera_t Camera(pTl->CreateDevice(devices[ 0 ]));
//    Camera_t Camera(TlFactory.CreateDevice(devices[ 0 ]));

//    // Open the camera
//    Camera.Open();

//    // Get the first stream grabber object of the selected camera
//    Camera_t::StreamGrabber_t StreamGrabber(Camera.GetStreamGrabber(0));

//    // Open the stream grabber
//    StreamGrabber.Open();

//    // Set the image format and AOI
//    //Camera.PixelFormat.SetValue(PixelFormat_YUV422Packed);
//    Camera.PixelFormat.SetValue(PixelFormat_BayerBG8);
//    //Camera.PixelFormat.SetValue(PixelFormat_BayerBG12);
//    //Camera.OffsetX.SetValue(0);
//    //Camera.OffsetY.SetValue(0);
////    int _CamWidth = Camera.Width.GetMax();
////    int _CamHeight = Camera.Height.GetMax();
//    int _CamWidth = 1280;
//    int _CamHeight = 960;
//    if(ic->mWidth == 0 ) ic->mWidth = _CamWidth;
//    if(ic->mHeight == 0) ic->mHeight = _CamHeight;


//    Camera.Width.SetValue(_CamWidth);
//    Camera.Height.SetValue(_CamHeight);
//    //Camera.ReverseY(true);
//    //Camera.ReverseY.SetValue(true,false);

//    ic->mImage.Create(ic->mWidth, ic->mHeight, 3);
//    ic->mRawImage.Create(_CamWidth,_CamHeight,3);
//    //std::cout << "Height " << _CamHeight << "Width " << _CamWidth << std::endl;
//    //Disable acquisition start trigger if available
//    {
//        GenApi::IEnumEntry* acquisitionStart = Camera.TriggerSelector.GetEntry( TriggerSelector_AcquisitionStart);
//        if ( acquisitionStart && GenApi::IsAvailable( acquisitionStart))
//        {
//            Camera.TriggerSelector.SetValue( TriggerSelector_AcquisitionStart);
//            Camera.TriggerMode.SetValue( TriggerMode_Off);
//        }
//    }

//    //Disable frame start trigger if available
//    {
//        GenApi::IEnumEntry* frameStart = Camera.TriggerSelector.GetEntry( TriggerSelector_FrameStart);
//        if ( frameStart && GenApi::IsAvailable( frameStart))
//        {
//            Camera.TriggerSelector.SetValue( TriggerSelector_FrameStart);
//            Camera.TriggerMode.SetValue( TriggerMode_Off);
//        }
//    }
//    //Set acquisition mode
//    Camera.AcquisitionMode.SetValue(AcquisitionMode_Continuous);

//    //Set exposure settings
//    //Camera.ExposureAuto.SetValue(ExposureAuto_Off);
//    //Camera.ExposureMode.SetValue(ExposureMode_Timed);
//    //set cam exposure
//    //Camera.ExposureTimeRaw.SetValue(18000);
//    //this enables jumbo frames
//    Camera.GevSCPSPacketSize.SetValue(8192);

//    // Get the image buffer size
//    const size_t ImageSize = (size_t)(Camera.PayloadSize.GetValue());

//    // We won't use image buffers greater than ImageSize
//    StreamGrabber.MaxBufferSize.SetValue(ImageSize);

//    // We won't queue more than c_nBuffers image buffers at a time
//    StreamGrabber.MaxNumBuffer.SetValue(c_nBuffers);

//    // Allocate all resources for grabbing. Critical parameters like image
//    // size now must not be changed until FinishGrab() is called.
//    StreamGrabber.PrepareGrab();


//    // Buffers used for grabbing must be registered at the stream grabber.
//    // The registration returns a handle to be used for queuing the buffer.
//    std::vector<PylonGrabBuffer*> BufferList;
//    for (uint32_t i = 0; i < c_nBuffers; ++i)
//    {
//        PylonGrabBuffer *pGrabBuffer = new PylonGrabBuffer(ImageSize);
//        pGrabBuffer->SetBufferHandle(StreamGrabber.RegisterBuffer(
//                                         pGrabBuffer->GetBufferPointer(), ImageSize));

//        // Put the grab buffer object into the buffer list
//        BufferList.push_back(pGrabBuffer);
//    }

//    for (std::vector<PylonGrabBuffer*>::const_iterator x = BufferList.begin(); x != BufferList.end(); ++x)
//    {
//        // Put buffer into the grab queue for grabbing
//        StreamGrabber.QueueBuffer((*x)->GetBufferHandle(), NULL);
//    }

//    // Let the camera acquire images continuously ( Acquisiton mode equals
//    // Continuous! )
//    Camera.AcquisitionStart.Execute();

//    // Grab c_ImagesToGrab times
//    //    for (int n = 0; n < c_ImagesToGrab; n++)
//    //    {
//    unsigned int gStartTime = time(NULL);

//    while(!ic->mCaptureThread.QuitThreadFlag())
//    {
//        //CxUtils::SleepMs(10);

//        //        try
//        //        {

//        // Wait for the grabbed image with timeout of 3 seconds
//        if (StreamGrabber.GetWaitObject().Wait(3000))
//        {
//            // Get the grab result from the grabber's result queue
//            GrabResult Result;
//            StreamGrabber.RetrieveResult(Result);

//            if (Grabbed == Result.Status())
//            {
//                //ic->mFrameNumber++;
//                // Grabbing was successful, process image
//                //cout << "Image #" << ic->mFrameNumber << " acquired!" << endl;
//                //cout << "Size: " << Result.GetSizeX() << " x "
//               //      << Result.GetSizeY() << endl;

//                // Get the pointer to the image buffer
//                const uint8_t *pImageBuffer = (uint8_t *) Result.Buffer();
//                //cout << "Gray value of first pixel: " << (uint32_t) pImageBuffer[0]
//                //     << endl << endl;
//                //ic->mImage.mWidth =  Result.GetSizeX();
//                //ic->mImage.mHeight =  Result.GetSizeY();

//                ic->mMutex.Lock();
//                {
//                    ic->mFrameNumber++;
//                    ic->mRawImage.mWidth = 1280;
//                    ic->mRawImage.mHeight = 960;
//                    ic->mRawImage.mChannels = 1;
//                    ic->mRawImage.mpData = (uint8_t*)pImageBuffer;
//                    HalfBG2RGB(&ic->mRawImage,&ic->mImage,false,true);
////                    std::cout << "Captured frame "
////                              << ic->mFrameNumber << " at ["
////                              //<< src.mWidth << " x "
////                              //<< src.mHeight << "] - "
////                              << ( (( time(NULL) - gStartTime ) > 0 ) ? (double)ic->mFrameNumber/(time(NULL) - gStartTime) : 0 )
////                              << std::endl;
////                    const cv::Mat original(ic->mRawImage.mHeight,ic->mRawImage.mWidth,CV_8UC1,(uint8_t*)pImageBuffer);

//                    //std::cout << "Raw size: " << ic->mRawImage.mWidth << "x" << ic->mRawImage.mHeight << std::endl;
//                    //std::cout << "Scaled size: " << ic->mImage.mWidth << "x" << ic->mImage.mHeight << std::endl;



////                    cv::Mat scaledest(ic->mImage.mHeight,ic->mImage.mWidth,CV_8UC3,ic->mImage.mpData);
////                    cv::Mat dest(ic->mRawImage.mHeight,ic->mRawImage.mWidth,CV_8UC3,ic->mRawImage.mpData);


////                    cv::cvtColor(original, dest, CV_BayerBG2RGB);
////                    cv::resize(dest,scaledest,scaledest.size());
////                    cv::flip(scaledest,scaledest,0);

//                }
//                ic->mMutex.Unlock();

//                // Reuse the buffer for grabbing the next image
//                //if (ic->mFrameNumber < c_ImagesToGrab - c_nBuffers)
//                    StreamGrabber.QueueBuffer(Result.Handle(), NULL);
//            }
//            else if (Failed == Result.Status())
//            {
//                // Error handling
//                cerr << "No image acquired!" << endl;
//                cerr << "Error code : 0x" << hex
//                     << Result.GetErrorCode() << endl;
//                cerr << "Error description : "
//                     << Result.GetErrorDescription() << endl;

//                // Reuse the buffer for grabbing the next image
//                //if (ic->mFrameNumber < c_ImagesToGrab - c_nBuffers)
//                    StreamGrabber.QueueBuffer(Result.Handle(), NULL);
//            }
//        }
//        else
//        {
//            // Timeout
//            cerr << "Timeout occurred!" << endl;

//            // Get the pending buffer back (You are not allowed to deregister
//            // buffers when they are still queued)
//            StreamGrabber.CancelGrab();

//            // Get all buffers back
//            for (GrabResult r; StreamGrabber.RetrieveResult(r););

//            // Cancel loop
//            break;
//        }
//    }
//    //        catch (GenICam::GenericException &e)
//    //        {
//    //            // Error handling
//    //            cerr << "An exception occurred!" << endl

//    //                 << e.GetDescription() << endl;
//    //            //pressEnterToExit();
//    //            return 1;
//    //        }
//    // Stop acquisition
//    Camera.AcquisitionStop.Execute();

//    // Clean up

//    // You must deregister the buffers before freeing the memory
//    for (std::vector<PylonGrabBuffer*>::iterator it = BufferList.begin(); it != BufferList.end(); it++)
//    {
//        StreamGrabber.DeregisterBuffer((*it)->GetBufferHandle());
//        delete *it;
//        *it = NULL;
//    }

//    // Free all resources used for grabbing
//    StreamGrabber.FinishGrab();

//    // Close stream grabber
//    StreamGrabber.Close();

//    // Close camera
//    Camera.Close();
//}

#endif

#endif

/*  End of File */
