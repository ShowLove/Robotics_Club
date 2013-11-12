////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  imagecapturedv1394.cpp
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
#include "cvcapture/imagecapturedv1394.h"

using namespace std;
using namespace Zebulon;
using namespace Video;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureDV1394::ImageCaptureDV1394()
{
    mInterlacedFlag = true;
    mReader = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ImageCaptureDV1394::~ImageCaptureDV1394()
{
    Stop();
    mReader = 0;
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
int ImageCaptureDV1394::Start(const std::string& videoSource,
                              const int width,
                              const int height,
                              const bool interlace)
{
    int result = 0;

    mStopCaptureFlag = false;
    // Check to see if the videoSource is a filename for a video.

    int device;
    /*if(sscanf(videoSource.c_str(),"/dev/video1394/%d",&device)==0)
    {
        return 0;
    }*/
    //good so far
    //Passing 0 would be camera 1, 1 would be camera 2
    //mCamera=new fw1394Camera(device);
    /*if(!mCamera)
    {
        return 0;
    }*/

    mFirst=0;
    //mCamera->printCameraInfo();
    //assume 640x480 for now
    /*if(width==0 && height==0)
    {
        //mCamera->printCameraInfo();
    }
    //doesn't support other than 640x480 currently
    else
    {
        std::cout << "Camera Settting not Supported\n";
        return 0;
    }*/

    /*if(mReader)
    {
        mReader->StopThread();
        delete mReader;

        mReader = 0;
    }*/

    mPort=0;
    mChannel=63;
    mBuffers=100;
    mNode=-1;

    /*iec61883Connection::CheckConsistency( mPort, mNode );
    printf("%d %d\n",mPort,mNode);

    mConnection = new iec61883Connection( mPort, mNode );
    mChannel = mConnection->GetChannel();*/

    //have to figure out settings
    mReader=new iec61883Reader(mPort,mChannel,mBuffers);
    mReader->StartThread();


    printf("%d %d %d\n",mPort,mChannel,mBuffers);
    //wait to get a frame one
    //while(mReader->GetOutQueueSize()<mBuffers)
    mReader->WaitForAction();

    printf("%d %d with ",mReader->GetInQueueSize(),mReader->GetOutQueueSize());
    mFrame=(DVFrame *)mReader->GetFrame();

    if(mFrame->IsComplete())
    {
        printf("Data: %d\n",mFrame->GetDataLen());
    }

    mWidth=mFrame->GetWidth();
    mHeight=mFrame->GetHeight();
    mReader->DoneWithFrame(mFrame);
    printf("%dx%d\n",mWidth,mHeight);

    mImage.Create(mWidth, mHeight, 3);
    //size from above
    //mImage.Create(mWidth, mHeight, 3);
    mInterlacedFlag = interlace;
    //assume interlaced for DV always
    //mInterlacedFlag = 1;
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
int ImageCaptureDV1394::Stop()
{
    mStopCaptureFlag = true;
    CxUtils::SleepMs(500);
    if(mReader)
    {
        mReader->StopThread();
        delete mReader;
    }
    mReader = 0;
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
int ImageCaptureDV1394::GetFrame(CvImageCapture::Image* dest, const bool block)
{
    int result = 0;

    int dropped = mReader->GetDroppedFrames();
    int badFrames = mReader->GetBadFrames();

    //get data from the camera
    //mCamera->acquire(mImage.mpData);
    //last frame?
    while(mStopCaptureFlag == false)
    {
        // Wait for the reader to indicate that something has happened
        mReader->WaitForAction( );
        mFrame=(DVFrame *)mReader->GetFrame();

        //if(mFrame->IsComplete())
        if(mReader->GetOutQueueSize()==0)
        {
            if(mFrame!=NULL)
            {
                mFrame->ExtractRGB(mImage.mpData);

                //SwapRGBImage(mImage);
                mReader->DoneWithFrame(mFrame);
            }

            break;
        }
        else
        {
            //printf("%d %d with ",mReader->GetInQueueSize(),mReader->GetOutQueueSize());
        }
        mReader->DoneWithFrame(mFrame);
    }

    mFirst = 1;
    if(dest)
    {
        CvImageCapture::FlipChannels(mImage);
        //this doesn't look right, make memory? -gary
        if(mInterlacedFlag)
        {
            dest->Create(mImage.mWidth, mImage.mHeight, mImage.mChannels);
            CvImageCapture::DeInterlaceData(mImage.mpData, dest->mpData, mImage.mWidth, mImage.mHeight);
        }
        else
        {
            //set up for same frame, fix -gary
            //CvImageCapture::DeInterlaceData(mImage.mpData, mImage.mpData, mImage.mWidth, mImage.mHeight);
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
bool ImageCaptureDV1394::IsReady() const
{
    return mImage.mpData != NULL ? true : false;
}


/*  End of File */
