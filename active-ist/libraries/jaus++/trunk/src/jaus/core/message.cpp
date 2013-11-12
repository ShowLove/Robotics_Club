////////////////////////////////////////////////////////////////////////////////////
///
///  \file message.cpp
///  \brief This file contains the definition of the Message class which
///         all JAUS++ messages are based from.
///
///  All messages are written into Packet structures, however the Message
///  base class is used for creating data structures that store read
///  message data and contain methods for  reading/riting from/to
///  a Packet structure.
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
#include "jaus/core/message.h"
#include "jaus/core/transport/largedataset.h"
#include <iostream>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] messageCode The type of message being defined.
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Message(const UShort messageCode,
                 const Address& dest,
                 const Address& src) :  mPriority(Priority::Standard),
                                        mMessageCode(messageCode),
                                        mSourceID(src),
                                        mDestinationID(dest)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Message::~Message()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets message priority.  Priority affects how messages are
///          routed by the transport layer.
///
///   \param[in] priority The priority value to set (see Header::Priority).
///
///   \return OK on success, FAILURE on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::SetPriority(const Byte priority)
{
    if(priority <= Priority::SafetyCritical)
    {
         mPriority = priority;
         return OK;
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the source ID of the message.
///
///   \param[in] src Source ID.  Must be valid and non-broadcast.
///
///   \return OK on success, FAILURE on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::SetSourceID(const Address& src)
{
    if(src.IsValid() && !src.IsBroadcast())
    {
        mSourceID = src;
        return OK;
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the destination ID of the message.
///
///   \param[in] dest Destination ID.  Must be a valid ID.
///
///   \return OK on success, FAILURE on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::SetDestinationID(const Address& dest)
{
    if(dest.IsValid())
    {
        mDestinationID = dest;
        return OK;
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Swaps source and destination ID's of message.  Will fail if
///          original destination ID was broadcast.
///
///   \return OK on success, FAILURE on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::SwapSourceAndDestination()
{
    if(mDestinationID.IsBroadcast() == false)
    {
        Address temp = mDestinationID;
        mDestinationID = mSourceID;
        mSourceID = temp;
        return OK;
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies message header data (source/dest/priority).
///
///   \param[in] msg Message to copy data from.
///
////////////////////////////////////////////////////////////////////////////////////
void Message::CopyHeaderData(const Message* msg)
{
    if(this != msg)
    {
        mSourceID = msg->mSourceID;
        mDestinationID = msg->mDestinationID;
        mPriority = msg->mPriority;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message header data.
///
////////////////////////////////////////////////////////////////////////////////////
void Message::ClearMessageHeader()
{
    mSourceID.Clear();
    mDestinationID.Clear();
    mPriority = Priority::Standard;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all message content data (header and payload).
///
////////////////////////////////////////////////////////////////////////////////////
void Message::ClearMessage()
{
    mSourceID.Clear();
    mDestinationID.Clear();
    mPriority = Priority::Standard;
    ClearMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints message info to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void Message::Print() const
{
    std::cout << GetMessageName() << " - SRC[" << mSourceID.ToString() << "] DEST[" << mDestinationID.ToString() << "]" << std::endl;
    PrintMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to verify if this Message is a response to 
///          a requesting Query or Command message.
///
///   This method verifies that the destination ID of the requestingMessage
///   matches the source ID of this message and that the result of
///   GetMessageCodeOfResponse for the requestingMessage matchings the
///   message code of this method.  Overload this method to check other
///   parameters.
///
///   \param[in] requestingMessage Original query or command message that
///                                you want to see if this message is a
///                                response to.
///
///   \return True if this message is a response, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::IsResponseToMessage(const Message *requestingMessage) const
{
    if(mMessageCode == requestingMessage->GetMessageCodeOfResponse() &&
       mSourceID == requestingMessage->mDestinationID)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes message contents to packet for the transport layer to
///          send.  Begins writing from current write position in packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write header and payload data to.
///   \param[out] header Packet transport header data.
///   \param[in] transportHeader Optional parameter for transport header data to
///                              write before the general transport header.
///   \param[in] clearPacket If true, packet contents are cleared before
///                          writing takes place.
///   \param[in] startingSequenceNumber Sequence number to use for packets.
///   \param[in] broadcastFlag Values to use to signify if message should be
///                            sent using any broadcast options (e.g. 
///                            multicast). 0 = no options, 1 = local broadcast,
///                            2 = global broadcast.
///
///   \return FAILURE on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Write(Packet& packet, 
                   Header& header,
                   const Packet* transportHeader, 
                   const bool clearPacket,
                   const UShort startingSequenceNumber,
                   const Byte broadcastFlag) const
{
    int total = 0;
    //  Build JAUS header data.
    header.mSourceID = mSourceID;
    header.mDestinationID = mDestinationID;
    header.mPriorityFlag = mPriority;
    header.mControlFlag = Header::DataControl::Single;
    header.mCompressionFlag = Header::Compression::None;
    header.mSequenceNumber = startingSequenceNumber;
    header.mBroadcastFlag = broadcastFlag;

    //  Clear out any previous message data.
    if(clearPacket)
    {
        packet.Clear();
        packet.Reserve(Header::MinSize + USHORT_SIZE + 1);
    }
    unsigned int writePos = packet.GetWritePos();
    
    if(transportHeader && transportHeader->Length() > 0)
    {
        writePos += (unsigned int)packet.Write(*transportHeader);
    }
    //  The first thing we must do is advance
    //  the write position to after the JAUS Header
    //  data.  The header is not written first because
    //  we do not know how large the message body will
    //  be yet.
    packet.SetLength(writePos + Header::PayloadOffset);
    packet.SetWritePos(writePos + Header::PayloadOffset);
    total += packet.Write(mMessageCode);
    int payloadSize = 0;
    if( (payloadSize = WriteMessageBody(packet)) >= 0)
    {
        total += payloadSize;
        // Check for large data set.
        if(total + Header::MinSize > Header::MaxPacketSize)
        {
            return FAILURE;
        }
        else
        {
            header.mSize = total + Header::MinSize;
            packet.SetWritePos(writePos);
            // Go back, and re-write the header since 
            // we now know the size of the message.
            if(header.Write(packet))
            {
                // Set the write position to the end of message we just wrote.
                packet.SetWritePos(writePos + header.mSize);
                //  Return the number of bytes written.
                if(transportHeader)
                {
                    return header.mSize + transportHeader->Length();
                }
                else
                {
                    return header.mSize;
                }
            }
        }
    }

    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief If the IsLargeDataSet method returns true, then this method is used
///          to convert message payload data into a multi-packet stream
///          according to rules defined by the JAUS standards documents.
///
///   This method should only be overloaded if your message contains data
///   larger than Header::MaxSize - Header::MinSize (e.g. image data) and you wish
///   to optimize how data is written.  By default, this method calls the
///   WriteMessageBody method to generate a single large byte array, which is then
///   broken up into a Packet::List for the transport layer.
///
///   \param[out] stream Multi-packet stream containing serialized message 
///                      following rules of large data sets.
///   \param[out] streamHeaders Headers for packet in the stream.
///   \param[in] maxPayloadSize This is the maximum packet size including 
///                             payload, does not include transport, General
///                             Transport Header, or Message Code (USHORT).
///   \param[in] transportHeader Additional transport header data to precede
///                              the general transport header of each packet.
///   \param[in] startingSequenceNumber Sequence number to use for packets.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g. 
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return FAILURE on error, otherwise number of packets written.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::WriteLargeDataSet(Packet::List& stream,
                               Header::List& streamHeaders,
                               const UShort maxPayloadSize,
                               const Packet* transportHeader,
                               const UShort startingSequenceNumber,
                               const Byte broadcastFlags) const
{
    Header header;
    header.mDestinationID = mDestinationID;
    header.mSourceID = mSourceID;
    header.mPriorityFlag = mPriority;
    header.mBroadcastFlag = broadcastFlags;

    stream.clear();
    streamHeaders.clear();
    Packet* temp = ((Packet *)(&mStreamPayload));
    temp->Clear();
    if(IsLargeDataSet() && WriteMessageBody(*temp) >= 0)
    {
        LargeDataSet::CreateLargeDataSet(header, 
                                         mMessageCode, 
                                         *temp, 
                                         stream, 
                                         streamHeaders, 
                                         transportHeader, 
                                         maxPayloadSize,
                                         startingSequenceNumber);
        return (int)stream.size();
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads transport header and payload data from packet, saving to
///          data members of class.
///
///   \param[in] packet Serialized JAUS packet data to read.
///   \param[in] transportHeader Transport header data in front of general
///                              transport header.
///
///   \return Number of bytes read on success, FAILURE on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Read(const Packet& packet,
                  const Packet* transportHeader)
{
    unsigned int readPos = packet.GetReadPos();
    
    Header header;
    int total = 0;
    
    if(transportHeader && transportHeader->Length() > 0)
    {
        packet.SetReadPos(readPos + transportHeader->Length());
        total = transportHeader->Length();
    }
    
    // Try read message transport header data
    if(header.Read(packet) > 0)
    {
        UShort messageCode;
        total += Header::MinSize;
        total += packet.Read(messageCode);
        // Verify this is a message we can actually read.
        if(messageCode == mMessageCode)
        {
            int payloadSize = 0;
            if( (payloadSize = ReadMessageBody(packet)) >= 0)
            {
                total += payloadSize;
                mSourceID = header.mSourceID;
                mDestinationID = header.mDestinationID;
                mPriority = header.mPriorityFlag;
                return total;
            }
        }
    }
    
    // Reset read position on failure.
    packet.SetReadPos(readPos);
    
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief If the Transport services of a component receive a multi-packet
///          stream for a message, this method is used to read the contents.
///
///   This method should only be overloaded if you wish to optimize the
///   de-serialization of mutli-packet streams for your message (e.g. video data).
///   By default, this method converts the stream to a larger continuous packet
///   to be used by the ReadMessageBody method.
///
///   \param[in] stream Multi-packet stream containing serialized message 
///                     following rules of large data sets.
///   \param[in] transportHeader Additional transport header data that precedes
///                              the general transport header of each packet.
///
///   \return FAILURE on error, otherwise number of packets read.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::ReadLargeDataSet(const Packet::List& stream,
                              const Packet* transportHeader)
{
    Header header;
    UShort messageCode;
    if(LargeDataSet::MergeLargeDataSet(header, messageCode, mStreamPayload, stream, transportHeader))
    {
        if(ReadMessageBody(mStreamPayload))
        {
            mSourceID = header.mSourceID;
            mDestinationID = header.mDestinationID;
            mPriority = header.mPriorityFlag;
            return (int)stream.size();
        }
    }
    return FAILURE;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief If the Transport services of a component receive a multi-packet
///          stream for a message, this method is used to read the contents.
///
///   This method should only be overloaded if you wish to optimize the
///   de-serialization of mutli-packet streams for your message (e.g. video data).
///   By default, this method converts the stream to a larger continuous packet
///   to be used by the ReadMessageBody method.
///
///   \param[in] stream Multi-packet stream containing serialized message 
///                     following rules of large data sets.
///   \param[in] transportHeader Additional transport header data that precedes
///                              the general transport header of each packet.
///
///   \return FAILURE on error, otherwise number of packets read.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::ReadLargeDataSet(const std::map<UShort, Packet>& stream,
                              const Packet* transportHeader)
{
    Header header;
    UShort messageCode;
    if(LargeDataSet::MergeLargeDataSet(header, messageCode, mStreamPayload, stream, transportHeader))
    {
        if(ReadMessageBody(mStreamPayload))
        {
            mSourceID = header.mSourceID;
            mDestinationID = header.mDestinationID;
            mPriority = header.mPriorityFlag;
            return (int)stream.size();
        }
    }
    return FAILURE;
}

/*  End of File */
