////////////////////////////////////////////////////////////////////////////////////
///
///  Filename:  cvimagecaptureimage.cpp
///
///  Copyright 2009 Daniel Barber
///                 Robotics Laboratory
///                 University of Central Florida
///                 http://robotics.ucf.edu
///
///  Program Contents:
///  -------------------------------------------------------------------------------
///  Contains the the implementation of CvImageCapture::Image.
///  -------------------------------------------------------------------------------
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
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace Zebulon;
using namespace Video;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::Image::Image() : mWidth(0),
                                 mHeight(0),
                                 mChannels(0),
                                 mpData(0),
                                 mStoredBytes(0),
                                 mID(0)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::Image::Image(const Image& img) : mWidth(0),
                                                 mHeight(0),
                                                 mChannels(0),
                                                 mpData(0),
                                                 mStoredBytes(0),
                                                 mID(0)
{
    *this = img;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::Image::~Image()
{
    if(mpData)
    {
        delete[] mpData;
        mpData = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
CvImageCapture::Image& CvImageCapture::Image::operator =(const CvImageCapture::Image& img)
{
    if(this != &img)
    {
        if(mWidth != img.mWidth || mHeight != img.mHeight || mChannels != img.mChannels)
        {
            if(mpData)
            {
                delete[] mpData;
                mpData = NULL;
            }
        }
        mWidth = img.mWidth;
        mHeight = img.mHeight;
        mChannels = img.mChannels;
        mStoredBytes = img.mStoredBytes;
        if(mWidth > 0 && mHeight > 0 && mChannels > 0)
        {
            if(mpData == NULL)
            {
                mpData = new unsigned char[mWidth*mHeight*mChannels];
            }
            if(mStoredBytes == 0)
            {
                mStoredBytes = mWidth*mHeight*mChannels;
            }
            memcpy(mpData, img.mpData, mStoredBytes);
        }
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes allocated memory.
///
////////////////////////////////////////////////////////////////////////////////////
void CvImageCapture::Image::Destroy()
{
    if(mpData)
    {
        delete[] mpData;
        mpData = NULL;
    }
    mWidth = mHeight = mChannels = 0;
    mStoredBytes = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates an image with the given parameters.
///
///   \param[in] width Desired image width in pixels.
///   \param[in] height Desired image height in pixels.
///   \param[in] channels Desired number of channels in pixels.
///
////////////////////////////////////////////////////////////////////////////////////
void CvImageCapture::Image::Create(const unsigned short width,
                                   const unsigned short height,
                                   const unsigned short channels)
{
    if(mWidth != width || mHeight != height || mChannels != channels)
    {
        if(mpData)
        {
            delete[] mpData;
            mpData = NULL;
        }
        mWidth = mHeight = 0;
        mChannels = 0;
    }
    mWidth = width;
    mHeight = height;
    mChannels = channels;
    if(mWidth > 0 && mHeight > 0 && mChannels > 0)
    {
        if(mpData == NULL)
        {
            mpData = new unsigned char[mWidth*mHeight*mChannels];
            memset(mpData, 0, mWidth*mHeight*mChannels); 
        }
        mStoredBytes = mWidth*mHeight*mChannels;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the camera unique identifier.
///
///   \param[in] id Camera ID.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool CvImageCapture::SetID(unsigned char id)
{
    mID = id;
    return true;
}

/*  End of File */
