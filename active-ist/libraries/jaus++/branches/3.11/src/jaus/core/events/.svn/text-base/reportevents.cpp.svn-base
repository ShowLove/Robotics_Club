////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportevents.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 25 October 2009
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
#include "jaus/core/events/reportevents.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::Record::Record() : mEventType(Events::Periodic),
                                 mEventID(0),
                                 mQueryCode(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::Record::Record(const ReportEvents::Record& record)
{
    *this = record;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::Record::~Record()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::Record& ReportEvents::Record::operator =(const ReportEvents::Record& record)
{
    if(this != &record)
    {
        mEventType = record.mEventType;
        mEventID = record.mEventID;
        mQueryCode = record.mQueryCode;
        mQueryMessage = record.mQueryMessage;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::ReportEvents(const Address& dest, 
                           const Address& src) : Message(REPORT_EVENTS, dest, src)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::ReportEvents(const ReportEvents& message) : Message(REPORT_EVENTS)
{
    *this = message;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents::~ReportEvents()
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
int ReportEvents::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE;

    total += packet.WriteByte( (const Byte)mEvents.size() );

    Record::List::const_iterator record;
    for(record = mEvents.begin();
        record != mEvents.end();
        record++)
    {
        expected += BYTE_SIZE*2 + UINT_SIZE + USHORT_SIZE + record->mQueryMessage.Size();
        total += packet.Write((const Byte)record->mEventType);
        total += packet.Write((const Byte)record->mEventID);
        total += packet.Write((UInt)(USHORT_SIZE + record->mQueryMessage.Size()));
        total += packet.Write(record->mQueryCode);
        if(record->mQueryMessage.Size() > 0)
        {
            total += packet.Write(record->mQueryMessage);
        }
    }

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
int ReportEvents::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE;
    Byte numEvents = 0;
    total += packet.Read( numEvents );

    for(Byte r = 0;
        r < numEvents;
        r++)
    {
        expected += BYTE_SIZE*2 + UINT_SIZE + USHORT_SIZE;
        UInt payloadSize = 0;
        Record record;
        total += packet.Read((Byte &)record.mEventType);
        total += packet.Read(record.mEventID);
        total += packet.Read(payloadSize);
        if(payloadSize >= USHORT_SIZE)
        {
            expected += payloadSize - USHORT_SIZE;
            total += packet.Read(record.mQueryCode);
            total += packet.Read(record.mQueryMessage, payloadSize - USHORT_SIZE);
        }
        mEvents.push_back(record);
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportEvents::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int total = BYTE_SIZE;

    Record::List::const_iterator record;
    for(record = mEvents.begin();
        record != mEvents.end();
        record++)
    {
        total += BYTE_SIZE*2 + UINT_SIZE + USHORT_SIZE + record->mQueryMessage.Size();
    }

    return total > maxPayloadSize;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEvents& ReportEvents::operator =(const ReportEvents &message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mEvents = message.mEvents;
    }
    return *this;
}


/* End of File */
