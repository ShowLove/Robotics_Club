////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportaudio.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 5 July 2010
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
#include "jaus/extras/audio/reportaudio.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAudio::ReportAudio(const Address& dest,
                         const Address& src) : Message(REPORT_AUDIO, dest, src)
{
    mDeviceID = 0;
    mFormat = Mono8;
    mFrequency = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAudio::ReportAudio(const ReportAudio& message) : Message(REPORT_AUDIO)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAudio::~ReportAudio()
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
int ReportAudio::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*2 + 
                   INT_SIZE + 
                   UINT_SIZE*2 + 
                   mAudio.Length();

    total += packet.WriteByte(mDeviceID);
    total += packet.WriteByte((Byte)mFormat);
    total += packet.Write(mFrequency);
    total += packet.Write(mTimeStamp.ToUInt());
    total += packet.Write((UInt)mAudio.Length());
    total += packet.Write(mAudio);
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
int ReportAudio::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*2 +
                   INT_SIZE + 
                   UINT_SIZE*2;
    Byte temp = 0;
    total += packet.Read(mDeviceID);
    
    total += packet.Read(temp);
    mFormat = (Format)temp;
    
    total += packet.Read(mFrequency);
    UInt tstamp = 0;
    total += packet.Read(tstamp);
    mTimeStamp.SetTime(tstamp);

    UInt len = 0;
    total += packet.Read(len);
    total += packet.Read(mAudio, len);

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportAudio::ClearMessageBody()
{
    mDeviceID = 0;
    mFormat = Mono8;
    mFrequency = 0;
    mAudio.Clear();
    mTimeStamp.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if data will fit into a standard
///          JAUS message packet or not.
///
///  \return True if large data set, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAudio::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int size = BYTE_SIZE*2 + 
                        INT_SIZE + 
                        UINT_SIZE*2 + 
                        mAudio.Length();
    return size > maxPayloadSize ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAudio& ReportAudio::operator =(const ReportAudio& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mDeviceID = message.mDeviceID;
        mFormat = message.mFormat;
        mFrequency = message.mFrequency;
        mAudio = message.mAudio;
        mTimeStamp = message.mTimeStamp;
    }
    return *this;
}


/* End of File */
