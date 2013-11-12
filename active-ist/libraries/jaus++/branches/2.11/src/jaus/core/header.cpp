////////////////////////////////////////////////////////////////////////////////////
///
///  \file header.CPP
///  \brief This file contains a data structure for storing JAUS transport header
///  information.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 26 September 2009
///  <br>Copyright (c) 2009
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
#include "jaus/core/header.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Header::Header() : mMessageType(MessageType::Normal),
                   mSequenceNumber(0),
                   mSize(0),
                   mPriorityFlag(Priority::Standard),
                   mBroadcastFlag(Broadcast::None),
                   mAckNackFlag(AckNack::None),
                   mControlFlag(DataControl::Single),
                   mCompressionFlag(Compression::None)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Header::Header(const Header& header)
{
    *this = header;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Header::~Header()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes header data to a packet for transmission.
///
///   Writes from the current Write Position in the packet.
///
///   \param[out] packet The packet to write to.
///
///   \return Number of bytes written to packet, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Header::Write(Packet& packet) const
{
    int result = 0;
    unsigned int startPos = packet.GetWritePos();
    // Make sure packet is big enought.
    packet.SetLength(startPos + mSize);
    packet.SetWritePos(startPos);

    result += packet.Write((Byte)(mMessageType | (mCompressionFlag << 6)));
    result += packet.Write(mSize);
    Byte flags = 0;
    flags = mControlFlag   << 6 |
            mAckNackFlag   << 4 |
            mBroadcastFlag << 2 |
            mPriorityFlag;
    result += packet.Write(flags);
    result += packet.Write(mDestinationID.ToUInt());
    result += packet.Write(mSourceID.ToUInt());
    // Sequence number goes at the end of the packet (this is retarded).
    packet.SetWritePos(startPos + mSize - USHORT_SIZE);
    result += packet.Write(mSequenceNumber);
    packet.SetWritePos(startPos + PayloadOffset);
    return result >= MinSize ? result : 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads header data from the current read position in the packet.
///
///   \param[in] packet The packet to read from.
///
///   \return Number of bytes read from packet, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Header::Read(const Packet& packet)
{
    int result = 0;

    Byte type = 0;
    Byte flags = 0;
    UInt id = 0;
    unsigned int startPos = packet.GetReadPos();

    result += packet.Read(type);
    mMessageType = type & 0x3F;
    mCompressionFlag = type >> 6;

    result += packet.Read(mSize);
    result += packet.Read(flags);
    mControlFlag   = (flags >> 6);
    mAckNackFlag   = (flags >> 4) & 0x03;
    mBroadcastFlag = (flags >> 2) & 0x03;
    mPriorityFlag  = (flags & 0x03);

    result += packet.Read(id);
    mDestinationID = id;
    result += packet.Read(id);
    mSourceID = id;
    packet.SetReadPos(startPos + mSize - USHORT_SIZE);
    result += packet.Read(mSequenceNumber);
    // Reset to where the payload is.
    packet.SetReadPos(startPos + PayloadOffset);
    return result >= MinSize ? result : 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks for valid header data.
///
///   \param[out] errorMessage If not NULL, then the reason for a header being
///                            invalid is saved to this string.
///
///   \return True if a valid header, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Header::IsValid(std::string* errorMessage) const
{
    if(mSourceID.IsValid() == false ||
       mSourceID.IsBroadcast() ||
       mDestinationID.IsValid() == false)
    {
        if(errorMessage)
        {
            *errorMessage = "Invalid Source or Destination ID";
        }
        return false;
    }

    if(mMessageType > MessageType::Normal)
    {
        if(errorMessage)
        {
            *errorMessage = "Invalid Message Type";
        }
        return false;
    }

    if(mSize < MinSize)
    {
        if(errorMessage)
        {
            *errorMessage = "Invalid Packet Size";
        }
        return false;
    }

    if(mSize > MinSize && (mAckNackFlag == AckNack::Nack || mAckNackFlag == AckNack::Ack))
    {
        if(errorMessage)
        {
            *errorMessage = "Invalid ACK/NACK Header";
        }
        return false;
    }

    if(mSequenceNumber == 0 && (mControlFlag != DataControl::Single && mControlFlag != DataControl::First))
    {
        if(errorMessage)
        {
            *errorMessage = "Invalid Sequence Number with Data Control Flag";
        }
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Header& Header::operator=(const Header& header)
{
    if(this != &header)
    {
        mMessageType = header.mMessageType;
        mDestinationID = header.mDestinationID;
        mSourceID = header.mSourceID;
        mSequenceNumber = header.mSequenceNumber;
        mSize = header.mSize;
        mPriorityFlag = header.mPriorityFlag;
        mBroadcastFlag = header.mBroadcastFlag;
        mAckNackFlag = header.mAckNackFlag;
        mControlFlag = header.mControlFlag;
        mCompressionFlag = header.mCompressionFlag;
    }
    return *this;
}

/* End of File */
