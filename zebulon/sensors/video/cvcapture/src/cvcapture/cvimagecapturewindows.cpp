////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  cvimagecapturewindows.cpp
///
///  Copyright 2009 Daniel Barber
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains the CvImageCapture implementation for Windows which uses DirectShow.
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
#include "cvcapture/cvimagecapture.h"
#include <iostream>

#define _WIN32_WINNT 0x500

#include <windows.h>

#include <commctrl.h>
#include <commdlg.h>
#include <tchar.h>
#include <dbt.h>

#define __EDEVDEFS__    // Don't include edevdefs.h

#include <mmreg.h>
#include <mmstream.h>
#include <xprtdefs.h>   // Include this instead of edevdefs, per MS docs
#include <dshow.h>
#include <dshowasf.h>   // Using ASF streaming AVI files
#include <qedit.h>
#include <atlbase.h>
#include <streams.h>
#include <objbase.h>
#include <initguid.h>

#include <assert.h>

using namespace std;
using namespace Zebulon;
using namespace Video;

class DirectShowCapture : protected ISampleGrabberCB
{
public:
    DirectShowCapture();
    ~DirectShowCapture();
    // Starts capturing from video source.
    int Start(const std::string& videoSource,
              const int width = 0,
              const int height = 0,
              const bool interlace = false);
    // Stops image capture.
    int Stop();
    int GetFrame(CvImageCapture::Image* dest, const bool block = false);
    // Check if image capture is ready (incomming data).
    bool IsReady() const;
    static unsigned int ScanForSources(std::vector<std::string>& sources);
    

    int InitializeVideo();
    int InitializeFile();
    static HRESULT GetVideoInputFilter( IBaseFilter** gottaFilter, wchar_t* matchName);
    static HRESULT GetVideoInputFilter( IBaseFilter** gottaFilter, const int num);
    static HRESULT GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirrequired, int iNum, IPin** ppPin);
    STDMETHODIMP_(ULONG) AddRef();
    STDMETHODIMP_(ULONG) Release();
    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv);
    virtual HRESULT STDMETHODCALLTYPE SampleCB(double sampleTime,
                                               IMediaSample *pSample);
    virtual HRESULT STDMETHODCALLTYPE BufferCB(double sampleTime,
                                               BYTE *pBuffer,
                                               long bufferLen);
    IMediaControl*            pControl;   ///<  Used by direct show for media control.
    ISampleGrabber*            pGrabber;   ///<  Used to grab frames.
    HANDLE mImageMutex;                 ///<  Mutex for thread protection.
    volatile bool mStopFlag;            ///<  Flag to stop video capture.
    volatile bool mReadyFlag;           ///<  Flag to signal capture is ready.
    volatile bool mInterlacedFlag;      ///<  Should we de-interlace the frame?
    volatile unsigned int mFrameNumber; ///<  Frame number.
    long mDataSize;                     ///<  Size of image buffer in bytes.
    int mWidth;                         ///<  Width of the image in pixels.
    int mHeight;                        ///<  Height of the image in pixels.
    int mDesWidth;                      ///<  Desired width.
    int mDesHeight;                     ///<  Desired height.
    unsigned char mID;                  ///<  Camera ID.
    unsigned char *mpImageBuffer;       ///<  Actual image data.
    unsigned int mGetFrameNumber;       ///<  The last frame number retrieved.
    double mFrameTimeMs;                ///<  Time of frame capture.
    std::string mSourceName;            ///<  Name of image source.
    CvImageCapture::Image mTempImage;   ///<  Temp image for callbacks.
    std::set<CvImageCapture::Callback*> mCallbacks;
    void (*mpFunctionCallback)(Zebulon::Video::CvImageCapture::Image*, void*);
    void* mpFunctionCallbackData;
    CvImageCapture* mpParent;           ///<  Pointer to parent object.
};


////////////////////////////////////////////////////////////////////////////////////
///
///   \macro SAFE_RELEASE
///   \brief Safely releases COM related pointers for DirectShow.
///
////////////////////////////////////////////////////////////////////////////////////
#define SAFE_RELEASE(pUnk)      \
        if (pUnk)               \
        {                       \
            pUnk->Release();    \
            pUnk = NULL;        \
        }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DirectShowCapture::DirectShowCapture()
{
    // Image data values.
    mWidth = mHeight = 0;
    mDataSize = 0;
    mpImageBuffer = NULL;
    mFrameNumber = 0;
    mFrameTimeMs = 0;

    mDesHeight = mDesWidth = 0;

    mImageMutex = NULL;
    mImageMutex = CreateMutex(NULL, FALSE, NULL);
    assert(mImageMutex);

    //  Flags.
    mStopFlag = true;
    mReadyFlag = false;
    mInterlacedFlag = false;

    pControl = NULL;
    pGrabber = NULL;
    mpFunctionCallbackData = NULL;
    mpFunctionCallback = NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
DirectShowCapture::~DirectShowCapture()
{
    Stop();

    if (mImageMutex)
    {
        ReleaseMutex(mImageMutex);
        CloseHandle(mImageMutex);
        mImageMutex = 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Start capture from a video source.
///
///   If width or height <= 0 the largest formats upported from the video
///   source is used.  Otherwise an attempt to select the size will be
///   tried, and if the desired resolution fails, a default resolution will be
///   used.  This feature may not be supported outside of Windows and does
///   not work with video files (only cameras).
///
///   \param videoSource The String/Friendly name of the video source.
///                      If left blank ( "" ), then the first device on the
///                      system will be used.  This value is the name of a
///                      camera or a video file.  You can also use an ascii
///                      number representing the device.  For example "0" for
///                      camera 0, "1" for camera 1, etc.
///   \param width Desired image width resolution if supported.  If <= 0 then
///                first working resolution is chosen from camera.
///   \param height Desired image height resolutoin if supported. If <= 0 then
///                first working resolution is chosen from camera.
///   \param iterlace If the video is interlaced, set to true and it will be
///                   automatically de-interlaced.
///
///   \return 1 if started, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DirectShowCapture::Start(const std::string& videoSource,
                             const int width,
                             const int height,
                             const bool interlace)
{
    int result = 0;

    Stop();

    USES_CONVERSION;

    CoInitializeEx( NULL, COINIT_MULTITHREADED);
    mSourceName = videoSource;
    mInterlacedFlag = interlace;
    if( mSourceName.empty() == false && GetFileAttributes( A2W(mSourceName.c_str()) ) != INVALID_FILE_ATTRIBUTES ) {
        result = InitializeFile();
    }
    //  Camera
    else {
        if( width > 0 && height > 0 ) {
            this->mDesHeight = height;
            this->mDesWidth = width;
        }
        else {
            mDesHeight = mDesWidth = 0;
        }
        result = InitializeVideo();
    }
    if( result == 0 ) {
        Stop();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops capture from video source.
///
///   \return 1 if stopped, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DirectShowCapture::Stop()
{
    mStopFlag = true;

    if(pControl) {
        pControl->Stop();
        pControl = NULL;
    }
    if(pGrabber) {
        pGrabber = NULL;
    }

    //  Now release everything and cleanup.
    CoUninitialize();

    mSourceName.clear();
    mHeight = mWidth = 0;
    mDataSize = 0;
    mFrameNumber = 0;
    mGetFrameNumber = 0;
    mFrameTimeMs = 0;

    WaitForSingleObject( mImageMutex, INFINITE );
    if( mpImageBuffer ) {
        delete[] mpImageBuffer;
        mpImageBuffer = NULL;
    }
    ReleaseMutex( mImageMutex );

    return 1;
}

bool DirectShowCapture::IsReady() const { return mReadyFlag; }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get the next frame from the image source.
///
///   An internal counter is kept that keeps track of what frame number was
///   last retrieved from the image source.  This counter prevents you from
///   get the same frame multiple times (over sampling).  If you set the
///   blocking flag to false, then if the next frame hasn't arrived yet, the
///   function will return 0.
///
///   \param dest The destination (where to save image to).
///   \param block If true, then this function will block until the next
///                image arrives, otherwise not.
///
///   \return 1 if image retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DirectShowCapture::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    int result = 0;
    unsigned char * ptr;
    unsigned char *iplPtr = NULL;
    CvImageCapture::Image *image;

    //  Check to see if this frame has already
    //  been captured.
    if( mGetFrameNumber >= mFrameNumber ) {
        //  If block is true, wait for the next frame
        //  to arrive, otherwise let user get the current frame.
        if( block ) 
        {
            unsigned int count = 0;
            while( mGetFrameNumber >= mFrameNumber && !mStopFlag ) {
                Sleep(1);
            }
        }
        return 0;
    }

    // Can't copy if input is null
    if( dest->mHeight != mHeight ||
        dest->mWidth != mWidth ||
        dest->mChannels != 3)
    {
        dest->Create((unsigned short)mWidth, (unsigned short)mHeight, 3);
    }

    image = dest;

    // pGrabber is our handle to the DirectShow "controller" if you will
    if (pGrabber && mImageMutex)
    {
        WaitForSingleObject(mImageMutex, INFINITE);

        // OK, so get the image

        if (mpImageBuffer && mDataSize)
        {
            ptr = mpImageBuffer;
            iplPtr = (unsigned char *)(image->mpData);

            if( !mInterlacedFlag )
            {
                // Copy the data so that the top of the image is at
                // row 0.
                for(unsigned short i = 0, r = (unsigned short)(mHeight - 1); i < (unsigned short)(mHeight) && r >= 0; i++, r--)
                {
                    memcpy(&image->mpData[i*mWidth*3],
                           &mpImageBuffer[r*mWidth*3],
                           mWidth*3);
                }
            }
            else 
            {
                CvImageCapture::DeInterlaceData(mpImageBuffer, image->mpData, image->mWidth, image->mHeight);
            }
            result = true;
            mGetFrameNumber = mFrameNumber;
        }

        ReleaseMutex(mImageMutex);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes capture from a video source, (USB or Firewire DV Camera).
///
///   \return 1 if success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DirectShowCapture::InitializeVideo()
{
    int result = 0;

    USES_CONVERSION;

    // COM variables to capture live video
    CComPtr <ICaptureGraphBuilder2>        pCaptureGraph;
    CComPtr <IGraphBuilder>                pGraph;
    CComPtr <IBaseFilter>                pVideoInputFilter;
    CComPtr <ISampleGrabber>            pGrabber;
    CComPtr <IAMStreamConfig>            pConfig;
    CComPtr <IAMVideoProcAmp>            pProcAmp;
    HRESULT hr; //  Used to store results of different operations.

    //  Create the filter graph manager and query for video interfaces
    hr = CoCreateInstance( CLSID_CaptureGraphBuilder2,
                           NULL,
                           CLSCTX_INPROC_SERVER,
                           IID_ICaptureGraphBuilder2,
                           (void **)&pCaptureGraph );
    if( FAILED(hr) ) {
        return result;
    }

    //  Now create a filter graph (this will do all encoding/decoding of data from camera.
    CoCreateInstance( CLSID_FilterGraph,
                      NULL,
                      CLSCTX_INPROC_SERVER,
                      IID_IFilterGraph,
                      (void **)&pGraph );
    if( !pGraph ) {
        return result;
    }

    //  Now get the fitler graph manager, and give the filter graph.
    hr = pCaptureGraph->SetFiltergraph( pGraph );
    if( FAILED(hr) ) {
        return result;
    }

    //  Using QueryInterface on the graph builder, get the Media Control object.
    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    if( FAILED(hr) ) {
        return result;
    }

    //  If the string is a number, then use it instead.
    bool isNumber = true;
    for(unsigned int i = 0; i < mSourceName.size(); i++) {
        if( isdigit( mSourceName.c_str()[i] ) == 0 ) {
            isNumber = false;
            break;
        }
    }
    if( isNumber )  {
        hr = GetVideoInputFilter( &pVideoInputFilter, atoi( mSourceName.c_str() ) );
    }
    else  {
        // Now create the video input filter from the camera name.
        hr = GetVideoInputFilter( &pVideoInputFilter, A2W( mSourceName.c_str() ) );
    }
    if( FAILED( hr ) ) {
        return result;
    }

    //  If we succeeded, add filter to our input filter graph.
    hr = pGraph->AddFilter(pVideoInputFilter, A2W( mSourceName.c_str() ) );

    //  Create the frame grabber.
    hr = pGrabber.CoCreateInstance( CLSID_SampleGrabber );
    if( !pGrabber ) {
        return result;
    }

    //  Get the frame grabber and add to the grabber base.
    CComQIPtr <IBaseFilter, &IID_IBaseFilter> pGrabberBase(pGrabber);
    hr = pGraph->AddFilter( pGrabberBase, L"Grabber" );

    //  Tell the grabber to grab 24-bit video.  Must do this before connecting.
    CMediaType GrabType;
    GrabType.SetType( &MEDIATYPE_Video );
    GrabType.SetSubtype( &MEDIASUBTYPE_RGB24 );
    hr = pGrabber->SetMediaType( &GrabType );
    if( FAILED(hr) ) {
        return result;
    }
    //  Tell the grabber to grab certain resolution.  Must do this before connecting.
    hr = pCaptureGraph->FindInterface(&PIN_CATEGORY_CAPTURE,
                                      &MEDIATYPE_Video,
                                      pVideoInputFilter,
                                      IID_IAMStreamConfig,
                                      (void **)&pConfig);
    if( FAILED( hr ) ) {
        return result;
    }

    AM_MEDIA_TYPE *theFormat = NULL;
    pConfig->GetFormat( &theFormat );

    VIDEOINFOHEADER *pvi = NULL;
    //  Create the format type and resolution
    if( theFormat->formattype == FORMAT_VideoInfo ) {

        pvi = (VIDEOINFOHEADER*)theFormat->pbFormat;
        hr = !S_OK;

        if( this->mDesWidth > 0 && this->mDesHeight > 0 ) {
            pvi->bmiHeader.biWidth = mDesWidth;
            pvi->bmiHeader.biHeight = mDesHeight;
            hr = pConfig->SetFormat(theFormat);
        }

        if( hr != S_OK ) {
            //  Figure out the resolution of the video data.
            int widthArray[] = {1920, 1680, 1600, 1440, 1280, 1024, 800, 720, 640, 320, 160 };
            int heightArray[] = {1200, 1080, 1050, 1200, 1024, 900, 800, 720, 768, 600, 480, 480, 240, 120 };
            for(int i = 0; i < 11 && hr != S_OK; i++) {
                for(int j = 0; j < 14 && hr != S_OK; j++) {
                    pvi->bmiHeader.biWidth = widthArray[i];
                    pvi->bmiHeader.biHeight = heightArray[j];
                    hr = pConfig->SetFormat(theFormat);
                    if(hr == S_OK)
                        break;
                }
            }
        }
        if (hr != S_OK) {
            mReadyFlag = false;
            Stop();
            return result;
        }
    }

    // Save height/width data.
    mHeight = pvi->bmiHeader.biHeight;
    mWidth = pvi->bmiHeader.biWidth;

    //  Allocate memory, but do it within critical section.
    WaitForSingleObject( mImageMutex, INFINITE );
    if( mpImageBuffer ) {
        delete[] mpImageBuffer;
        mpImageBuffer = NULL;
    }
    mDataSize = mHeight*mWidth*3;
    mpImageBuffer = new unsigned char[ mDataSize ];
    //  Leave critical section of memory.
    ReleaseMutex( mImageMutex );

    hr = pCaptureGraph->FindInterface(&PIN_CATEGORY_CAPTURE,
                                      &MEDIATYPE_Video,
                                      pVideoInputFilter,
                                      IID_IAMVideoProcAmp,
                                      (void **)&pProcAmp);

    //  Attempt to connect the pins.
    CComPtr<IPin> pSourcePin=0;
    CComPtr<IPin> pGrabPin;

    GetPin( pVideoInputFilter,PINDIR_OUTPUT, 0, &pSourcePin );
    GetPin( pGrabberBase, PINDIR_INPUT, 0, &pGrabPin );

    hr = pGraph->Connect( pSourcePin, pGrabPin );
    if( FAILED(hr) ) {
        return result;
    }

    //  Turn AutoShow off.
    CComQIPtr <IVideoWindow, &IID_IVideoWindow> pWindow = pGraph;
    if (pWindow) {
        hr = pWindow->put_AutoShow(OAFALSE);
    }

    // Ask for the connection media type so we know its size
    AM_MEDIA_TYPE mmt;
    hr = pGrabber->GetConnectedMediaType(&mmt);

    VIDEOINFOHEADER * mvih = (VIDEOINFOHEADER*) mmt.pbFormat;
    FreeMediaType(mmt);

    hr = pGrabber->SetBufferSamples(FALSE);

    // Grab as a sequence of frames, not just one single image
    hr = pGrabber->SetOneShot(FALSE);

    pGrabber->SetCallback(this, 1); // 1 as second arg indicates BufferCB(),
                                    // 2 indicates SampleCB().

    // If all the above junk went through, we are about ready to actually start capturing
    if( SUCCEEDED( hr ) ) {
        // Activate the threads
        CComQIPtr <IMediaControl, &IID_IMediaControl> pControl(pGraph);
        // Run the graph filter.
        hr = pControl->Run();
        if( SUCCEEDED( hr ) ) {

            //  Wait for the graph to settle
            CComQIPtr<IMediaEvent, &IID_IMediaEvent> pEvent( pGraph );
            long EvCode = 0;
            //  Don't rush video capture.
            hr = pEvent->WaitForCompletion( INFINITE , &EvCode );

            mStopFlag = false;
            //  Wait until ready, but don't wait forever.
            for(unsigned int i = 0; i < 5000; i++) {
                if(mReadyFlag)
                    break;
                Sleep(1);
            }

            this->pGrabber = pGrabber;
            result = 1;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes capture from a file video source.
///
///   \return 1 if success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DirectShowCapture::InitializeFile()
{
    int result = 0;

    CComPtr <IGraphBuilder>                pGraph;
    CComPtr <IBaseFilter>               pGrabberFilter;
    CComPtr <IBaseFilter>               pSrcFilter;
    CComPtr <IBaseFilter>               pNullRenderer;
    //CComPtr <IMediaControl>             pControl;
    CComPtr <IMediaEventEx>             pEvent;
    CComPtr <IMediaSeeking>             pSeeking;
    CComPtr <IMediaFilter>              pMediaFilter;
    CComPtr <ISampleGrabber>            pGrabber;


    HRESULT hr;

    //  Initialize interfaces
    hr = CoCreateInstance(CLSID_FilterGraph,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IGraphBuilder,
                          reinterpret_cast<void **>(&pGraph));
    if(FAILED(hr)) {
        return result;
    }

    hr = CoCreateInstance(CLSID_SampleGrabber,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IBaseFilter,
                          reinterpret_cast<void **>(&pGrabberFilter));

    hr = pGrabberFilter->QueryInterface(IID_ISampleGrabber,
                                       (void **)(&pGrabber));
    if(FAILED(hr)) {
        return result;
    }

    hr = pGraph->AddFilter(pGrabberFilter, L"SampleGrabber");
    if(FAILED(hr)) {
        return result;
    }

    //  Set the media type
    AM_MEDIA_TYPE mt;
    ZeroMemory(&mt, sizeof(AM_MEDIA_TYPE));
    mt.formattype = FORMAT_VideoInfo;
    mt.majortype = MEDIATYPE_Video;
    mt.subtype = MEDIASUBTYPE_RGB24;    //  Only accept 24-bit video bitmaps
    hr = pGrabber->SetMediaType(&mt);
    if(FAILED(hr)) {
        return result;
    }

    //  Create the src filter
    wchar_t *strFilename;
    strFilename = new wchar_t[mSourceName.size() + 1];
    memset( strFilename, 0, sizeof(wchar_t)*(mSourceName.size() + 1));
    MultiByteToWideChar(CP_ACP, 0, mSourceName.c_str(), -1, strFilename, 256);

    hr = pGraph->AddSourceFilter( strFilename, L"Source", &pSrcFilter);
    delete[] strFilename;
    if(FAILED(hr)) {
        return result;
    }

    //  Attempt to connect the pins.
    CComPtr<IPin> pGrabPin;
    CComPtr<IPin> pSourcePin;

    GetPin( pSrcFilter, PINDIR_OUTPUT, 0, &pSourcePin );
    GetPin( pGrabberFilter, PINDIR_INPUT, 0, &pGrabPin );
    hr = pGraph->Connect( pSourcePin, pGrabPin );
    if(FAILED(hr)) {
        return result;
    }

    // Create the NULL renderer and connect
    hr = CoCreateInstance(CLSID_NullRenderer,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_IBaseFilter,
                          (void **)(&pNullRenderer));
    hr = pGraph->AddFilter(pNullRenderer, L"NullRenderer");

    //  Attempt to connect the pins.
    CComPtr<IPin> pGrabPin2;
    CComPtr<IPin> pNullPin;

    GetPin( pGrabberFilter, PINDIR_OUTPUT, 0, &pNullPin );
    GetPin( pNullRenderer, PINDIR_INPUT, 0, &pGrabPin2 );
    pGraph->Connect( pNullPin, pGrabPin2 );
    if( FAILED (hr ) ) {
        return result;
    }

    // Necessary interfaces for controlling
    pGraph->QueryInterface(IID_IMediaFilter, (void **)(&pMediaFilter));
    pGraph->QueryInterface(IID_IMediaControl, (void **)(&this->pControl));
    pGraph->QueryInterface(IID_IMediaEvent, (void **)(&pEvent));
    pGraph->QueryInterface(IID_IMediaSeeking, (void **)(&pSeeking));

    // Retrieve the actual media type
    ZeroMemory(&mt, sizeof(mt));
    hr = pGrabber->GetConnectedMediaType(&mt);
    VIDEOINFOHEADER *pVih;
    if (mt.formattype == FORMAT_VideoInfo)
        pVih = reinterpret_cast<VIDEOINFOHEADER*>(mt.pbFormat);
    else {
        return result; // Something went wrong, perhaps not appropriate media type
    }
    //  Get height and width information for video
    mHeight = pVih->bmiHeader.biHeight;
    mWidth = pVih->bmiHeader.biWidth;

    //  Allocate memory for image capture buffer.
    WaitForSingleObject(mImageMutex, INFINITE);
    if(mpImageBuffer) {
        delete[] mpImageBuffer;
        mpImageBuffer = NULL;
    }
    mDataSize = mHeight*mWidth*3;
    mpImageBuffer = new unsigned char[mDataSize];
    ReleaseMutex(mImageMutex);


    // Free the media type
    FreeMediaType(mt);

    hr = pGrabber->SetBufferSamples(TRUE);
    hr = pGrabber->SetOneShot(FALSE);

    pGrabber->SetCallback(this, 1); // 1 as second arg indicates BufferCB(),
                                    // 2 indicates SampleCB().
    if(SUCCEEDED(hr))
    {
        CComQIPtr <IMediaControl, &IID_IMediaControl> pControl(pGraph);

        hr = this->pControl->Run(); // Run the graph to start the analyzing process!
        if( SUCCEEDED(hr) )
        {
            this->pGrabber = pGrabber;
            pGrabber = NULL;

            long evCode = 0;
            mStopFlag = false;
            mReadyFlag = true;
            result = 1;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Enumerate all of the video input devices.
///
///  \param gottaFilter Input filter pointer.
///  \param matchName The friendly name of the device (camera name) to
///         match to.
///
///  \return The filter with a matching friendly name.
///
////////////////////////////////////////////////////////////////////////////////////
HRESULT DirectShowCapture::GetVideoInputFilter(IBaseFilter** gottaFilter, wchar_t* matchName)
{
    BOOL done = false;

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICreateDevEnum,
                                  (void **)&pSysDevEnum);
    if (FAILED(hr)) {
        return hr;
    }

    // Obtain a class enumerator for the video input category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
                                            &pEnumCat,
                                            0);
    if (hr == S_OK)
    {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
        {
            // Bind the first moniker to an object
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                (void **)&pPropBag);
            if (SUCCEEDED(hr))
            {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    // Do a comparison, find out if it's the right one
                    if (wcsncmp(varName.bstrVal, matchName,
                        wcslen(matchName)) == 0) {

                        // We found it, so send it back to the caller
                        hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**) gottaFilter);
                        done = true;
                    }
                }
                VariantClear(&varName);
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();
    if (done) {
        return hr;    // found it, return native error
    }
    else {
        return VFW_E_NOT_FOUND;    // didn't find it error
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Enumerate all of the video input devices.
///
///  \param gottaFilter Input filter pointer.
///  \param num The video device number to get input filter for.  This is
///             [0, num video sources].
///
///  \return The filter with a matching friendly name.
///
////////////////////////////////////////////////////////////////////////////////////
HRESULT DirectShowCapture::GetVideoInputFilter(IBaseFilter** gottaFilter, const int num)
{
    BOOL done = false;

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                                  NULL,
                                  CLSCTX_INPROC_SERVER,
                                  IID_ICreateDevEnum,
                                  (void **)&pSysDevEnum);
    if (FAILED(hr)) {
        return hr;
    }

    // Obtain a class enumerator for the video input category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
                                            &pEnumCat,
                                            0);
    int count = 0;
    if (hr == S_OK)
    {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK) && (!done))
        {
            // Bind the first moniker to an object
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0, 0, IID_IPropertyBag,
                (void **)&pPropBag);
            if (SUCCEEDED(hr))
            {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr) && count == num)
                {
                    hr = pMoniker->BindToObject(NULL, NULL, IID_IBaseFilter, (void**) gottaFilter);
                    done = true;
                }
                count++;
                VariantClear(&varName);
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();
    if (done) {
        return hr;    // found it, return native error
    }
    else {
        return VFW_E_NOT_FOUND;    // didn't find it error
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Get a pin in the the graph.
///
///  \param pFilter The filter.
///  \param dirrequired The pin directoin required.
///  \param iNum Input number.
///  \param ppPin Pointer to pin.
///
///  \return Result < 0 is failure, >= 0 is success.
///
////////////////////////////////////////////////////////////////////////////////////
HRESULT DirectShowCapture::GetPin(IBaseFilter* pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    CComPtr< IEnumPins > pEnum;
    *ppPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr))
        return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;

        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;  // Return the pin's interface
                hr = S_OK;      // Found requested pin, so clear error
                break;
            }
            iNum--;
        }

        pPin->Release();
    }

    return hr;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function is part of the callbacks used by ISampleGrabberCB.
///
////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) DirectShowCapture::AddRef()
{
    return 2;   //  Return default value.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function is part of the callbacks used by ISampleGrabberCB.
///
////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP_(ULONG) DirectShowCapture::Release()
{
    return 1;   //  Return default value.
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function is part of the callbacks used by ISampleGrabberCB.
///
////////////////////////////////////////////////////////////////////////////////////
STDMETHODIMP DirectShowCapture::QueryInterface(REFIID riid, void ** ppv)
{
    if( ppv == NULL ) {
        return E_POINTER;
    }

    if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
    {
        *ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
        return NOERROR;
    }

    return E_NOINTERFACE;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function is part of the callbacks used by ISampleGrabberCB.
///
////////////////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE DirectShowCapture::SampleCB(double SampleTime,
                                                      IMediaSample *pSample)
{
    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
///
///   \brief The implemented callback function.  Gets latest frame
///   from the video feed.
///
//////////////////////////////////////////////////////////////////////////////
HRESULT STDMETHODCALLTYPE DirectShowCapture::BufferCB(double sampleTime,
                                                      BYTE *pBuffer,
                                                      long bufferLen)
{
    //  Verify we have received valid image data.
    if(pBuffer && bufferLen &&  mImageMutex) {
        //  Enter critical section
        WaitForSingleObject(mImageMutex, INFINITE);
        // Make sure we can fit the data into our buffer
        if(bufferLen >= mDataSize) {
            if(!mpImageBuffer) {
                mpImageBuffer = new unsigned char[mDataSize];
                assert(mpImageBuffer);
            }
            //  Copy data.
            memcpy(mpImageBuffer, pBuffer, mDataSize);
            mFrameNumber++;
            mFrameTimeMs= (unsigned int)(sampleTime*1000.0);
            mReadyFlag = true;            
        }
        
        //cout << "Frame\n";
        if(mCallbacks.size() > 0 
            || mpFunctionCallback)
        {
            GetFrame(&mTempImage, false);
        }
        
        if(mCallbacks.size() > 0)
        {
            std::set<CvImageCapture::Callback*>::iterator cb;
            for(cb = mCallbacks.begin();
                cb != mCallbacks.end();
                cb++)
            {
                ((CvImageCapture::Callback*)(*cb))->ProcessCameraData(((CvImageCapture::Image*)&mTempImage));
            }

        }

    #ifdef WIN32
        if(mpFunctionCallback)
        {
            mpFunctionCallback(&mTempImage, mpFunctionCallbackData);
        }
#endif

        //  Leave critical section
        ReleaseMutex(mImageMutex);
    }
    return S_OK;
}


//////////////////////////////////////////////////////////////////////////////
///
///   \brief Get a list of video sources connected to the computer (cameras).
///
///   \param sources Copy of source names.
///
///   \return The number of video sources connected to the computer.
///
//////////////////////////////////////////////////////////////////////////////
unsigned int DirectShowCapture::ScanForSources(std::vector<std::string>& sources)
{
    if(sources.size() > 0 )
        sources.clear();

    // Initialize DirectShow enough so we can enumerate the video soruces
    CoInitializeEx(NULL, COINIT_MULTITHREADED);

    USES_CONVERSION;

    // COM junk variables to capture live video
    CComPtr <ICaptureGraphBuilder2>        pCaptureGraph;
    CComPtr <IGraphBuilder>                pGraph;
    CComPtr <IMediaControl>                pControl;
    CComPtr <IBaseFilter>                pVideoInputFilter;
    CComPtr <ISampleGrabber>            pGrabber;
    CComPtr <IAMStreamConfig>            pConfig;
    CComPtr <IAMVideoProcAmp>            pProcAmp;

    HRESULT hr;

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICaptureGraphBuilder2,
                          (void **)&pCaptureGraph);

    if (FAILED(hr))
        return 0;

    // Create filter graph itself
    CoCreateInstance(CLSID_FilterGraph,
                     NULL,
                     CLSCTX_INPROC_SERVER,
                     IID_IFilterGraph,
                     (void **)&pGraph );
    if (!pGraph)
        return 0;

    // Now get the filter graph manager. That's part of the capture graph builder
    hr = pCaptureGraph->SetFiltergraph(pGraph);

    if (FAILED(hr))
        return 0;

    // Using QueryInterface on the graph builder, get the Media Control object.
    hr = pGraph->QueryInterface(IID_IMediaControl,
                                (void **)&pControl);

    // No syncronization needed so disable clock
    IMediaFilter *pMediaFilter = 0;
    pGraph->QueryInterface(IID_IMediaFilter,
                          (void**)&pMediaFilter);
    pMediaFilter->SetSyncSource(NULL);
    pMediaFilter->Release();

    if (FAILED(hr))
        return 0;

    // Create the System Device Enumerator.
    ICreateDevEnum *pSysDevEnum = NULL;
    hr = CoCreateInstance(CLSID_SystemDeviceEnum,
                          NULL,
                          CLSCTX_INPROC_SERVER,
                          IID_ICreateDevEnum,
                          (void **)&pSysDevEnum);
    if (FAILED(hr)) {
        return 0;
    }

    // Obtain a class enumerator for the video input category.
    IEnumMoniker *pEnumCat = NULL;
    hr = pSysDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory,
                                            &pEnumCat,
                                            0);
    if (hr == S_OK)
    {
        // Enumerate the monikers.
        IMoniker *pMoniker = NULL;
        ULONG cFetched;
        while ((pEnumCat->Next(1, &pMoniker, &cFetched) == S_OK))
        {
            // Bind the first moniker to an object
            IPropertyBag *pPropBag;
            hr = pMoniker->BindToStorage(0,
                                         0,
                                         IID_IPropertyBag,
                                         (void **)&pPropBag);
            if (SUCCEEDED(hr))
            {
                // To retrieve the filter's friendly name, do the following:
                VARIANT varName;
                VariantInit(&varName);
                hr = pPropBag->Read(L"FriendlyName", &varName, 0);
                if (SUCCEEDED(hr))
                {
                    USES_CONVERSION;
                    sources.push_back(W2A(varName.bstrVal));
                }
                VariantClear(&varName);
                pPropBag->Release();
            }
            pMoniker->Release();
        }
        pEnumCat->Release();
    }
    pSysDevEnum->Release();

    return (unsigned int)(sources.size());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::CvImageCapture()
{
    mpCapture = new DirectShowCapture();
    ((DirectShowCapture*)mpCapture)->mpParent = this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::~CvImageCapture()
{
    if(mpCapture)
    {
        delete ((DirectShowCapture*)mpCapture);
        mpCapture = NULL;
    }
}


///////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to start capture from a video source.
///
///   \param[in] videoSource Camera/Video source name or file path.  Examples:
///              "/dev/video0" or "Logitech Pro 9000" or a number "0", "1" for
///              the camera number.
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
    return ((DirectShowCapture*)(mpCapture))->Start(videoSource, width, height, interlace);
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
    return ((DirectShowCapture*)(mpCapture))->Stop();
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
int CvImageCapture::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    return ((DirectShowCapture*)(mpCapture))->GetFrame(dest, block);
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
int CvImageCapture::GetRawFrame(CvImageCapture::Image* dest, const bool block)
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if raw data available, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::IsRawDataSupported() const
{
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if ready to capture image data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::IsReady() const
{
    return ((DirectShowCapture*)(mpCapture))->IsReady();
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
    return DirectShowCapture::ScanForSources(sources);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive image data as it is captured.
///
///   Callbacks can only be registered before capture begins.
///
///   \param[in] cb Pointer to callback object (this is not deleted on program
///                 exit).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RegisterCallback(Callback* cb)
{
    if(IsReady() == false)
    {
        ((DirectShowCapture*)(mpCapture))->mCallbacks.insert(cb);
    }
    else
    {
        std::cout << "CvImageCapture::ERROR - Must register callback before camera init.\n";
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive image data as it is captured.
///
///   Callbacks can only be registered before capture begins.
///
///   \param[in] cb Pointer to callback object (this is not deleted on program
///                 exit).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RegisterRawCallback(RawCallback* cb)
{
    return false;
}

#ifdef WIN32
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive image data as it is captured.
///
///   Callbacks can only be registered before capture begins.
///
///   \param[in] func Pointer to callback object (this is not deleted on program
///                 exit).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
void CvImageCapture::RegisterCallback(void (*func)(Zebulon::Video::CvImageCapture::Image*, void*), void *ptr)
{
    if(IsReady() == false)
    {
        ((DirectShowCapture*)(mpCapture))->mpFunctionCallback = func;
        ((DirectShowCapture*)(mpCapture))->mpFunctionCallbackData = ptr;
    }
    else
    {
        std::cout << "CvImageCapture::ERROR - Must register callback before camera init.\n";
    }
}
#endif

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a callback to receive image data as it is captured.
///
///   \param[in] cb Pointer to callback object.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RemoveCallback(Callback* cb)
{
    std::set<Callback*>::iterator c;
    c = ((DirectShowCapture*)(mpCapture))->mCallbacks.find(cb);
    if(c != ((DirectShowCapture*)(mpCapture))->mCallbacks.end())
    {
        ((DirectShowCapture*)(mpCapture))->mCallbacks.erase(cb);
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a callback to receive image data as it is captured.
///
///   \param[in] cb Pointer to callback object.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::RemoveRawCallback(RawCallback* cb)
{
    return false;
}

/*  End of File */