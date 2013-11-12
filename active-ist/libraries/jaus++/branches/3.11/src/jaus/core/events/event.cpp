////////////////////////////////////////////////////////////////////////////////////
///
///  \file event.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 October 2009
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
#include "jaus/core/events/event.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
Event::Event(const Address& dest, const Address& src) : Message(EVENT, dest, src)
{
    mEventID = mSequenceNumber = 0;
    mReportCode = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Event::Event(const Event& message) : Message(EVENT)
{
    mEventID = mSequenceNumber = 0;
    mReportCode = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Event::~Event()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the report message that the Event has been triggered by.
///
///   \param[in] reportMessage Pointer to a Report message to use for
///                            serialization of message data.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Event::SetReportMessage(const Message* reportMessage)
{
    if(reportMessage && reportMessage->WriteMessageBody(mReportMessage) >= 0)
    {
        mReportCode = reportMessage->GetMessageCode();
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the report message that the Event has been triggered by.
///
///   \param[in] messageCode The report message type.
///   \param[in] reportPayload Payload data for the Report.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Event::SetReportMessage(const UShort messageCode, const Packet& reportPayload)
{
    mReportCode = messageCode;
    mReportMessage = reportPayload;
    return true;
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
int Event::WriteMessageBody(Packet& packet) const
{
    int expected = (int)(BYTE_SIZE*2 + USHORT_SIZE + UINT_SIZE + mReportMessage.Size());
    int written = 0;

    written += packet.Write(mEventID);
    written += packet.Write(mSequenceNumber);
    written += packet.Write((UInt)(mReportMessage.Size() + USHORT_SIZE));
    written += packet.Write(mReportCode);
    if(mReportMessage.Size() > 0)
    {
        written += packet.Write(mReportMessage);
    }

    return written == expected ? written : -1;
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
int Event::ReadMessageBody(const Packet& packet)
{
    int expected = (int)(BYTE_SIZE*2 + USHORT_SIZE + UINT_SIZE);
    int read = 0;
    UInt payloadSize = 0;
    
    read += packet.Read(mEventID);
    read += packet.Read(mSequenceNumber);
    read += packet.Read(payloadSize);
    
    if(payloadSize >= USHORT_SIZE)
    {
        expected += payloadSize - USHORT_SIZE;
        read += packet.Read(mReportCode);
        read += packet.Read(mReportMessage, payloadSize - USHORT_SIZE);
    }

    return read == expected ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void Event::ClearMessageBody()
{
    mEventID = mSequenceNumber = 0;
    mReportCode = 0;
    mReportMessage.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Event::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int size = BYTE_SIZE*2 + USHORT_SIZE + UINT_SIZE + mReportMessage.Size();
    return size > maxPayloadSize;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Event& Event::operator =(const Event& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mEventID = message.mEventID;
        mReportCode = message.mReportCode;
        mSequenceNumber = message.mSequenceNumber;
        mReportMessage = message.mReportMessage;
    }
    return *this;
}

/*  End of File */
