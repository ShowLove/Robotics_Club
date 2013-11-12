////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportimage.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 21 January 2010
///  Copyright (c) 2010
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
#include "jaus/extras/video/reportimage.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportImage::ReportImage(const Address& dest,
                         const Address& src) : Message(REPORT_IMAGE, dest, src)
{
    mCameraID = 0;
    mFormat = Image::RAW;
    mFrameNumber = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportImage::ReportImage(const ReportImage& message) : Message(REPORT_IMAGE)
{
    mCameraID = 0;
    mFormat = Image::RAW;
    mFrameNumber = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportImage::~ReportImage()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportImage::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*2 + UINT_SIZE*2 + mImage.Length();

    total += packet.WriteByte(mCameraID);
    total += packet.Write(mFrameNumber);
    total += packet.WriteByte((Byte)mFormat);
    total += packet.Write(mImage.Length());
    total += packet.Write(mImage);

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads message payload from the packet.
///
///   Message contents are read from the packet following the JAUS standard.
///
///   \param[in] packet Packet containing message payload data to read.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportImage::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*2 + UINT_SIZE*2;
    unsigned int length = 0;

    total += packet.Read(mCameraID);
    total += packet.Read(mFrameNumber);
    total += packet.Read((Byte &)mFormat);
    total += packet.Read(length);

    if(length > 0)
    {
        total += packet.Read(mImage, length);
        expected += length;
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportImage::ClearMessageBody()
{
    mFormat = Image::RAW;
    mCameraID = 0;
    mFrameNumber = 0;
    mImage.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportImage::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int size = BYTE_SIZE + UINT_SIZE + mImage.Length();
    return size > maxPayloadSize;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportImage& ReportImage::operator =(const ReportImage& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mCameraID = message.mCameraID;
        mFrameNumber = message.mFrameNumber;
        mFormat = message.mFormat;
        mImage = message.mImage;
    }
    return *this;
}


/* End of File */
