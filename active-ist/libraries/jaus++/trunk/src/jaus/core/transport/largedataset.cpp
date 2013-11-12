////////////////////////////////////////////////////////////////////////////////////
///
///  \file largedataset.cpp
///  \brief This file contains an interface for storing and generating
///  Large Data Sets in JAUS.
///
///  Author(s): Daniel Barber
///  Created: 27 September 2009
///  Copyright (c) 2009
///  Applied Cognition and Training in Immersive Virtual Environments
///  (ACTIVE) Laboratory
///  Institute for Simulation and Training (IST)
///  University of Central Florida (UCF)
///  Email: dbarber@ist.ucf.edu
///  Web:  http://active.ist.ucf.edu
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
#include "jaus/core/transport/largedataset.h"
#include <string.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source of the large data set.
///   \param[in] code Message code (type of payload data).
///   \param[in] pv Presence vector data.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::Key::Key(const Address& src,
                       const UShort code,
                       const UInt pv)
{
    mSourceID = src;
    mMessageCode = code;
    mPresenceVector = pv;
    mIdentifier = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::Key::Key(const LargeDataSet::Key& key)
{
    *this = key;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::Key::~Key()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the key data for comparisons.
///
////////////////////////////////////////////////////////////////////////////////////
void LargeDataSet::Key::Update() const
{
    Packet * p = ( (Packet *)(&mKey) );
    p->Clear();
    p->Reserve(UINT_SIZE + USHORT_SIZE + ULONG_SIZE + BYTE_SIZE);
    p->Write(mSourceID.ToUInt());
    p->Write(mMessageCode);
    p->Write(mPresenceVector);
    p->Write(mIdentifier);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares key data.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::Key::operator<(const LargeDataSet::Key& key) const
{
    Update();
    key.Update();

    if(memcmp(mKey.Ptr(), key.mKey.Ptr(), mKey.Length()) < 0)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares key data.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::Key::operator<=(const LargeDataSet::Key& key) const
{
    Update();
    key.Update();

    if(memcmp(mKey.Ptr(), key.mKey.Ptr(), mKey.Length()) <= 0)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares key data.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::Key& LargeDataSet::Key::operator=(const LargeDataSet::Key& key)
{
    if(this != &key)
    {
        mSourceID = key.mSourceID;
        mMessageCode = key.mMessageCode;
        mPresenceVector = key.mPresenceVector;
        mIdentifier = key.mIdentifier;
        mKey = key.mKey;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::LargeDataSet()
{
    mCompleteFlag = false;
    mUpdateTimeMs = 0;
    mMessageCode = 0;
    mHaveLastFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LargeDataSet::~LargeDataSet()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears contents of LDS.
///
////////////////////////////////////////////////////////////////////////////////////
void LargeDataSet::Clear()
{
    mHeader = Header();
    mMessageCode = 0;
    mHaveLastFlag = false;
    mCompleteFlag = false;
    mStream.clear();
    mMissing.clear();
    mUpdateTimeMs = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a multi-stream packet to the data set.
///
///   \param[in] message JAUS Message with JAUS Transport Header.
///
///   \return True if added, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::AddPacket(const Packet& message)
{
    Header header;
    UShort messageCode;
    message.SetReadPos(0);
    if(header.Read(message) &&
       message.Read(messageCode))
    {
        if(header.mControlFlag != Header::DataControl::Single)
        {
            return AddPacket(header, messageCode, message);
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a multi-stream packet to the data set.
///
///   \param[in] header Message header data.
///   \param[in] messageCode Message type.
///   \param[in] packet Serialized JAUS message data.
///
///   \return True if added, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::AddPacket(const Header& header, 
                             const UShort messageCode, 
                             const Packet& packet)
{
    if(header.mControlFlag == Header::DataControl::Single || mCompleteFlag)
    {
        return false; // Not multi-packet stream.
    }
    if(mStream.size() == 0)
    {
        mMessageCode = messageCode;
        mHeader.mSourceID = header.mSourceID;
        mHeader.mDestinationID = header.mDestinationID;
        mHeader.mSequenceNumber = header.mSequenceNumber;
        /*
        for(UShort m = 0; m < header.mSequenceNumber; m++)
        {
            mMissing.insert(m);
        }
        */
        mStream[header.mSequenceNumber] = packet;
        mUpdateTimeMs = Time::GetUtcTimeMs();
        return true;
    }
    
    if(messageCode != mMessageCode || 
       header.mSourceID != mHeader.mSourceID)
    {
        return false; // Wrong data set.
    }
    /*
    std::set<UShort>::iterator m;
    // Update count of packets missing.
    if(header.mSequenceNumber > mHeader.mSequenceNumber)
    {
        for(UShort n = mHeader.mSequenceNumber + 1; n < header.mSequenceNumber; n++)
        {
            mMissing.insert(n);
        }
        mHeader.mSequenceNumber = header.mSequenceNumber;
    }
    else
    {
        m = mMissing.find(header.mSequenceNumber);
        if(m == mMissing.end())
        {
            return false; // This packet is not missing from the data set.
        }
        else
        {
            // Remove found data.
            mMissing.erase(m);
        }
    }
    */

    if(header.mControlFlag == Header::DataControl::Last)
    {
        mHaveLastFlag = true;
    }

    mStream[header.mSequenceNumber] = packet;
    if(/*mMissing.size() == 0 && */mHaveLastFlag)
    {
        // See if all the data is present.
        Stream::iterator p;
        UShort prevSequenceNumber = 0;
        bool first = true;
        mCompleteFlag = true;
        for(p = mStream.begin(); p != mStream.end(); p++)
        {
            if(first)
            {
                first = false;
            }
            else if(prevSequenceNumber + 1 != p->first)
            {
                mCompleteFlag = false;
                break;
            }
            prevSequenceNumber = p->first;
        }
    }
    mUpdateTimeMs = Time::GetUtcTimeMs();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates a multi-packet sequence following the rules of the
///          JAUS standard given a large data set.
///
///   \param[in] header Message header data to use (e.g. src/dest/priority).
///   \param[in] messageCode Message type (payload type).
///   \param[in] payload Message payload data.
///   \param[out] stream Multi-packet stream sequence constructed.
///   \param[out] streamHeaders headers for the stream sequence constructed.
///   \param[in] transportHeader Additional transport header data to add
///                              to each packet for the transport layer. The
///                              typical value is 0x02 for JUDP.
///   \param[in] maxPayloadSize This is maximum allowed size for each packet
///                             payload (data only).  This value does not include
///                             transport size, General Transport Header, or message
///                             code size.
///   \param[in] startingSequenceNumber The starting sequence number to use for
///                                     messages. Default is 0.
///
////////////////////////////////////////////////////////////////////////////////////
void LargeDataSet::CreateLargeDataSet(const Header& header,
                                      const UShort messageCode,
                                      const Packet& payload,
                                      Packet::List& stream,
                                      Header::List& streamHeaders,
                                      const Packet* transportHeader,
                                      const UShort maxPayloadSize,
                                      const UShort startingSequenceNumber)
{
    stream.clear();
    streamHeaders.clear();

    Header sHeader(header);
    Packet sPacket;
    
    unsigned int total = 0;                     // Total number of bytes in payload converted.
    unsigned int toWrite = 0;                   // How much data to write for a given packet.
    const unsigned char* ptr = payload.Ptr();   // Pointer to payload data to write.
    unsigned int transportHeaderSize = transportHeader ? transportHeader->Length() : 0;

    // Reserve memory for data
    sPacket.Reserve(maxPayloadSize + Header::MinSize + transportHeaderSize + USHORT_SIZE);
    sHeader.mSequenceNumber = startingSequenceNumber;

    while(total < payload.Length())
    {
        // Set the data control flags.
        if(sHeader.mSequenceNumber == startingSequenceNumber)
        {
            sHeader.mControlFlag = Header::DataControl::First;
        }
        else
        {
            sHeader.mControlFlag = Header::DataControl::Normal;
        }
        // Calculate the amount of data being written.
        toWrite = maxPayloadSize;
        if(total + toWrite > payload.Length())
        {
            toWrite = payload.Length() - total;
            // Check for last packet in sequence or if
            // this is not really a large data set.
            if(sHeader.mSequenceNumber != 0)
            {
                sHeader.mControlFlag = Header::DataControl::Last;
            }
            else
            {
                sHeader.mControlFlag = Header::DataControl::Single;
            }
        }
        sHeader.mSize = (UShort)(Header::MinSize + USHORT_SIZE + toWrite);
        // Clear the stream packet
        sPacket.Clear();
        if(transportHeaderSize > 0)
        {
            sPacket.Write(*transportHeader);  //  Transport Header.
        }
        sHeader.Write(sPacket);              //  General Transport Header.
        sPacket.Write(messageCode);          //  Message type.
        total += (unsigned int)sPacket.Write((unsigned char *)(ptr), toWrite); // Write payload data
        stream.push_back(sPacket);           //  Add to multi-packet sequence.
        streamHeaders.push_back(sHeader);    //  Add header to sequence.
        ptr += toWrite;                      //  Advance the pointer.
        sHeader.mSequenceNumber++;           //  Increase the sequence number.        
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates a multi-packet sequence following the rules of the
///          JAUS standard given a large data set.
///
///   \param[out] header Message header data to use (e.g. src/dest/priority).
///   \param[out] messageCode Message type (payload type).
///   \param[out] payload Message payload data.
///   \param[in] stream Multi-packet stream sequence constructed.
///   \param[in] transportHeader Additional transport header data to add
///                              to each packet for the transport layer. The
///                              typical value is 0x02 for JUDP.
///
///   \return true on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::MergeLargeDataSet(Header& header,
                                     UShort& messageCode,
                                     Packet& payload,
                                     const Packet::List& stream,
                                     const Packet* transportHeader)
{
    Header sHeader;
    Packet::List::const_iterator sPacket;
    UShort sequenceNumber = 0;
    std::map<UShort, const Packet*> orderedPackets;
    std::map<UShort, Header> orderedHeaders;

    // Clear data.
    header.mSize = Header::MinSize;
    header.mAckNackFlag = Header::AckNack::None;
    header.mSequenceNumber = 0;
    header.mControlFlag = Header::DataControl::Single;
    payload.Clear();
    
    unsigned int transportHeaderSize = transportHeader ? transportHeader->Length() : 0;
    for(sPacket = stream.begin(); sPacket != stream.end(); sPacket++)
    {
        // Read after the transport header (don't do validation).
        if(transportHeaderSize > 0)
        {
            sPacket->SetReadPos(transportHeaderSize);
        }
        else
        {
            sPacket->SetReadPos(0);
        }
        if(sHeader.Read(*sPacket) && sPacket->Read(messageCode))
        {
            orderedPackets[sHeader.mSequenceNumber] = &(*sPacket);
            orderedHeaders[sHeader.mSequenceNumber] = sHeader;
        }
        else
        {
            payload.Clear();
            break;
        }
    }
    if(stream.size() == orderedPackets.size())
    {
        std::map<UShort, const Packet*>::iterator data;
        std::map<UShort, Header>::iterator dataHeader;

        bool first = true;
        UShort prevSequenceNumber = 0;
        for(data = orderedPackets.begin(), dataHeader = orderedHeaders.begin(); 
            data != orderedPackets.end() && dataHeader != orderedHeaders.end(); 
            data++, dataHeader++)
        {
            if(first)
            {
                if(dataHeader->second.mControlFlag != Header::DataControl::First)
                {
                    break;
                }
                header.mSourceID = dataHeader->second.mSourceID;
                header.mDestinationID = dataHeader->second.mDestinationID;
                header.mPriorityFlag = dataHeader->second.mPriorityFlag;
                header.Write(payload);
                payload.Write(messageCode);
                first = false;
            }
            else if(prevSequenceNumber + 1 != data->first)
            {
                break;
            }
            prevSequenceNumber = header.mSequenceNumber = data->first;
            // Write the data to the payload.
            payload.Write((unsigned char *)(data->second->Ptr() + data->second->GetReadPos()), 
                                            dataHeader->second.mSize - (Header::MinSize + USHORT_SIZE));
        }
    }
    
    return payload.Length() > 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates a multi-packet sequence following the rules of the
///          JAUS standard given a large data set.
///
///   \param[out] header Message header data to use (e.g. src/dest/priority).
///   \param[out] messageCode Message type (payload type).
///   \param[out] payload Message payload data.
///   \param[in] stream Multi-packet stream sequence constructed.
///   \param[in] transportHeader Additional transport header data to add
///                              to each packet for the transport layer. The
///                              typical value is 0x02 for JUDP.
///
///   \return true on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool LargeDataSet::MergeLargeDataSet(Header& header,
                                     UShort& messageCode,
                                     Packet& payload,
                                     const Stream& stream,
                                     const Packet* transportHeader)
{
    Header sHeader;
    Stream::const_iterator sPacket;
    UShort prevSequenceNumber = 0;
    
    // Clear data.
    header.mSize = Header::MinSize;
    header.mAckNackFlag = Header::AckNack::None;
    header.mSequenceNumber = 0;
    header.mControlFlag = Header::DataControl::Single;
    payload.Clear();
    
    bool first = true;
    unsigned int transportHeaderSize = transportHeader ? transportHeader->Length() : 0;
    for(sPacket = stream.begin(); sPacket != stream.end(); sPacket++)
    {
        // Read after the transport header (don't do validation).
        if(transportHeaderSize > 0)
        {
            sPacket->second.SetReadPos(transportHeaderSize);
        }
        else
        {
            sPacket->second.SetReadPos(0);
        }
        if(sHeader.Read(sPacket->second) && sPacket->second.Read(messageCode))
        {
            if(first)
            {
                header.mSourceID = sHeader.mSourceID;
                header.mDestinationID = sHeader.mDestinationID;
                header.mPriorityFlag = sHeader.mPriorityFlag;
                header.Write(payload);
                payload.Write(messageCode);
                first = false;
            }
            // Check for out of order data.
            else if(prevSequenceNumber + 1 != sHeader.mSequenceNumber)
            {
                payload.Clear();
                break;
            }
            // Write the data to the payload.
            payload.Write((unsigned char *)(sPacket->second.Ptr() + sPacket->second.GetReadPos()), 
                                            sHeader.mSize - (Header::MinSize + USHORT_SIZE));
            header.mSequenceNumber = prevSequenceNumber = sHeader.mSequenceNumber;
        }
        else
        {
            payload.Clear();
            break;
        }
    }

    return payload.Length() > 0;
}

/*  End of File */
