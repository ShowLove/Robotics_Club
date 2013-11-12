////////////////////////////////////////////////////////////////////////////////////
///
///  \file sharedimage.cpp
///  \brief This file contains the main data structure for storing raw image
///  data within shared memory for transfering between components on the
///  same machine.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 March 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/extras/video/sharedimage.h"
#include <string.h>
#include <cstdio>
#include <cstdlib>

using namespace JAUS;

const UInt JAUS_SHARED_IMAGE_HEADER_SIZE = ULONG_SIZE + UINT_SIZE*2 + USHORT_SIZE*2 + BYTE_SIZE;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedImage::SharedImage()
{
    mCameraID = 0;
    mWriteFlag = false;
    mpCallback = NULL;
    mpCallbackArgs = NULL;
    mExitFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedImage::~SharedImage()
{
    CloseSharedImage();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Created a shared image memory space.
///
///  \param[in] src Source ID of the provider of the data.
///  \param[in] cameraID The ID of the camera on the source.
///  \param[in] size How many bytes needed to store image.
///
///  \return OK if created, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::CreateSharedImage(const Address& src, 
                                   const Byte cameraID,
                                   const unsigned int size)
{
    CloseSharedImage();

    if( src.IsValid() && !src.IsBroadcast() && size > JAUS_SHARED_IMAGE_HEADER_SIZE )
    {
        char sname[256];
        sprintf(sname, "SharedImage_%d.%d.%d.%d", src.mSubsystem,
                                                  src.mNode,
                                                  src.mComponent,
                                                  cameraID);
        if( mImageBuffer.CreateMappedMemory(sname, size + JAUS_SHARED_IMAGE_HEADER_SIZE) ||
            mImageBuffer.OpenMappedMemory(sname, CxUtils::MappedMemory::ReadWriteAccess, size + JAUS_SHARED_IMAGE_HEADER_SIZE))
        {
            Time::Stamp tstamp;
            UInt number;
            UShort width = 0, height = 0;
            Byte channels = 0;
            tstamp = Time::GetUtcTimeMs();
            number = 0;
            mImageBuffer.Lock();
            mImageBuffer->SetWritePos(0);
            mImageBuffer->Write(tstamp);
            mImageBuffer->Write(number);
            mImageBuffer->Write(width);
            mImageBuffer->Write(height);
            mImageBuffer->Write(channels);
            mImageBuffer.Unlock();

            mWriteFlag = true;
            mSourceID = src;
            mCameraID = cameraID;
            return OK;
        }
    }

    CloseSharedImage();
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Created a shared image memory space.
///
///  \param[in] src Source ID of the provider of the data.
///  \param[in] cameraID Camera ID on the sensor.
///
///  \return OK if created, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::OpenSharedImage(const Address& src, const Byte cameraID)
{
    if(src.IsValid() && !src.IsBroadcast())
    {
        char sname[256];
        sprintf(sname, "SharedImage_%d.%d.%d.%d", src.mSubsystem,
                                                  src.mNode,
                                                  src.mComponent,
                                                  cameraID);
        if( mImageBuffer.OpenMappedMemory(sname) )
        {
            mWriteFlag = false;
            mSourceID = src;
            mCameraID = cameraID;
            return OK;
        }
    }

    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Closes shared image.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::CloseSharedImage()
{
    mExitFlag = true;
    for(unsigned int i = 0; i < 500; i++)
    {
        if(mCallbackThread.IsThreadActive() == false)
        {
            break;
        }
        CxUtils::SleepMs(1);
    }
    mCallbackThread.StopThread();

    {
        Mutex::ScopedLock lock(&mCallbackMutex);
        mpCallback = NULL;
        mpCallbackArgs = NULL;
    }

    if( mWriteFlag )
    {
        mImageBuffer.Lock();
        if(mImageBuffer.IsOpen() && mImageBuffer.GetMemory())
        {
            memset((void *)mImageBuffer.GetMemory(), 0, mImageBuffer->Size());
        }
        mImageBuffer.Unlock();
    }
    mImageBuffer.CloseMappedMemory();

    mWriteFlag = false;
    mExitFlag = false;

    return OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Registers a callback to be done every time the shared image
///  updates.
///
///  \param func Callback function (arguments are source of image data and
///              a raw image, plus additional arguments).
///  \param fargs Arguments to function callback.
///
///  \return OK if registered, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::RegisterCallback(void (*func)(const Address& src, 
                                               const Byte cameraID, 
                                               const Image& img, 
                                               const unsigned int frameNumber,
                                               void* args), void *fargs)
{
    if( func )
    {
        mCallbackMutex.Lock();
        mpCallback = func;
        mpCallbackArgs = fargs;
        mCallbackMutex.Unlock();

        if( mCallbackThread.IsThreadActive() == false )
        {
            mCallbackThread.CreateThread(&SharedImage::SharedImageUpdate, this);
        }
        return OK;
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Clears the callback.
///
///  \return OK if cleared, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::ClearCallback()
{
    mCallbackMutex.Lock();
    mpCallback = NULL;
    mpCallbackArgs = NULL;
    mCallbackMutex.Unlock();

    return OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of the image data.
///
///  Image data can only be set if you create the shared image using
///  the CreateSharedImage method.
///
///  \return OK if created, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::SetFrame(const unsigned char* rawImage,
                          const unsigned int width,
                          const unsigned int height,
                          const unsigned char channels,
                          const unsigned int frameNumber)
{
    int result = FAILURE;

    if( mWriteFlag &&
        mImageBuffer.IsOpen() &&
        width*height*channels <= mImageBuffer->Size() - JAUS_SHARED_IMAGE_HEADER_SIZE)
    {
        Time::Stamp imgTimeStamp;

        imgTimeStamp = Time::GetUtcTimeMs();
        mImageBuffer.Lock();
        mImageBuffer->SetWritePos(0);
        mImageBuffer->Write(imgTimeStamp);
        mImageBuffer->Write(frameNumber);
        mImageBuffer->Write(width);
        mImageBuffer->Write(height);
        mImageBuffer->Write(channels);
        mImageBuffer->Write(rawImage, width*height*channels );
        mImageBuffer.Unlock();
        result = OK;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the image from shared memory.
///
///  \param img Current image data.
///
///  \return OK if created, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::GetFrame(Image& img) const
{
    int result = FAILURE;
    Time::Stamp tstamp;
    UInt number;
    unsigned int width, height;
    Byte channels;

    if( mImageBuffer.IsOpen() )
    {
        mImageBuffer.Lock();
        mImageBuffer->SetReadPos(0);
        if( mImageBuffer->Read( tstamp ) &&
            mImageBuffer->Read( number ) &&
            mImageBuffer->Read( width ) &&
            mImageBuffer->Read( height ) &&
            mImageBuffer->Read( channels ) )
        {
            if( width && height && channels &&
                mImageBuffer->Size() - JAUS_SHARED_IMAGE_HEADER_SIZE == (unsigned int)((unsigned int)(width)*height*channels))
            {
                result = img.Create(width, height, channels, mImageBuffer.GetMemory() + JAUS_SHARED_IMAGE_HEADER_SIZE );
            }
        }
        mImageBuffer.Unlock();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the image from shared memory, reading directly into an
///         existing buffer.
///
///  \param[out] image Pointer to array to copy data to.  If pointer is NULL, or
///               array is not large enough, it is automatically resized.
///  \param[out] size The size of the array.
///  \param[out] width Image width (pixels).
///  \param[out] height Image height (pixels).
///  \param[out] channels Number of channels in the image.
///  \param[out] frameNumber Image frame number.
///
///  \return OK if created, otherwise FAILURE.
///
////////////////////////////////////////////////////////////////////////////////////
int SharedImage::GetFrame(unsigned char*& image,
                          unsigned int& size,
                          unsigned int& width,
                          unsigned int& height,
                          unsigned char& channels,
                          unsigned int& frameNumber) const
{
    int result = FAILURE;
    Time::Stamp tstamp;

    if( mImageBuffer.IsOpen() )
    {
        mImageBuffer.Lock();
        mImageBuffer->SetReadPos(0);
        if( mImageBuffer->Read( tstamp ) &&
            mImageBuffer->Read( frameNumber ) &&
            mImageBuffer->Read( width ) &&
            mImageBuffer->Read( height ) &&
            mImageBuffer->Read( channels ) )
        {
            if( width && height && channels )
            {
                if(size < width*height*channels || image == NULL)
                {
                    if(image)
                    {
                        delete[] image;
                        image = NULL;
                    }
                    size = width*height*channels;
                    image = new unsigned char[size + 256];
                }
                result = mImageBuffer->Read(image, width*height*channels) == width*height*channels ? OK : FAILURE;
            }
        }
        mImageBuffer.Unlock();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Get the sequence number for the image.
///
///  \return Image sequence number (0) if no image data.
///
////////////////////////////////////////////////////////////////////////////////////
UInt SharedImage::GetFrameNumber() const
{
    int result = FAILURE;
    UInt number = 0;

    if( mImageBuffer.IsOpen() )
    {
        mImageBuffer.Lock();
        if( mImageBuffer->Read( number, UINT_SIZE ) )
            result = OK;
        mImageBuffer.Unlock();
    }

    if( result )
        return number;
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if shared image is open/active, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedImage::IsOpen() const
{
    if( mImageBuffer.IsOpen() ||
        mCallbackThread.IsThreadActive() )
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method checks to see if the shared image buffer is being updated or
///  not.
///
///  \return True if active, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedImage::IsActive(const unsigned int timeout) const
{
    if(mImageBuffer.IsOpen())
    {
        Time::Stamp imgTimeStamp = 0;
        mImageBuffer.Lock();
        mImageBuffer->SetReadPos(0);
        mImageBuffer->Read(imgTimeStamp);
        mImageBuffer.Unlock();
        Time::Stamp currentTime = Time::GetUtcTimeMs();
        if(currentTime > imgTimeStamp && currentTime - imgTimeStamp < timeout)
        {
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Function called by thread which keeps trying to get the latest
///  data from shared memory image.
///
////////////////////////////////////////////////////////////////////////////////////
void SharedImage::SharedImageUpdate(void *args)
{
    SharedImage *img = (SharedImage*)args;
    UInt imgNumber = 0, prevImgNumber = 0;
    Time::Stamp imgTimeStamp = 0, currentTime = 0;
    unsigned int width, height;
    Byte channels;

    bool haveCallback  = true;
    while( img->mExitFlag == false && img && !img->mCallbackThread.QuitThreadFlag() && haveCallback)
    {
        if( img->mImageBuffer.IsOpen() )
        {
            bool haveImage = false;
            width = height = channels = 0;
            img->mImageBuffer.Lock();
            img->mImageBuffer->SetReadPos(0);
            if( img->mImageBuffer->Read(imgTimeStamp) &&
                img->mImageBuffer->Read(imgNumber) &&
                img->mImageBuffer->Read(width) &&
                img->mImageBuffer->Read(height) &&
                img->mImageBuffer->Read(channels) )
                {
                    if(imgTimeStamp &&
                        imgNumber != prevImgNumber &&
                        width > 0 && height > 0 && channels >= 1 &&
                        width*height*((unsigned int)channels) <= img->mImageBuffer->Size() - JAUS_SHARED_IMAGE_HEADER_SIZE)
                    {
                        if( img->mTempImage.Create(width,
                                                   height,
                                                   channels,
                                                   img->mImageBuffer.GetMemory() + JAUS_SHARED_IMAGE_HEADER_SIZE))
                        {
                            haveImage = true;
                        }
                    }
                    prevImgNumber = imgNumber;
                }
            img->mImageBuffer.Unlock();

            if(haveImage && img->mExitFlag == false)
            {
                img->mCallbackMutex.Lock();
                if( img->mpCallback )
                {
                    img->mpCallback(img->mSourceID, img->mCameraID, img->mTempImage, imgNumber, img->mpCallbackArgs);
                }
                else
                {
                    haveCallback = false;
                }
                img->mCallbackMutex.Unlock();
            }

            currentTime = Time::GetUtcTimeMs();
            if(imgTimeStamp == 0 || (imgTimeStamp < currentTime && currentTime - imgTimeStamp > 10000))
            {
                img->mImageBuffer.CloseMappedMemory();
                // Wait a while for others to close there
                // view of shared memory.
                for(unsigned int i = 0; i < 1000; i++)
                {
                    if( img->mCallbackThread.QuitThreadFlag() || img->mExitFlag)
                        break;

                    CxUtils::SleepMs(1);
                }
            }
        }
        else
        {
            //  Try open shared memory.
            img->OpenSharedImage(img->mSourceID, img->mCameraID);
        }

        CxUtils::SleepMs(10);
    }
}

/*  End of File */
