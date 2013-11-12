////////////////////////////////////////////////////////////////////////////////////
///
///  \file queryevents.h
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
#include "jaus/core/events/queryevents.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
QueryEvents::QueryEvents(const Address& dest,
                         const Address& src) : Message(QUERY_EVENTS, dest, src)
{
    mQueryType = MessageID;
    mQueryFilter = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
QueryEvents::QueryEvents(const QueryEvents& message) : Message(QUERY_EVENTS)
{
    mQueryType = MessageID;
    mQueryFilter = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
QueryEvents::~QueryEvents()
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
int QueryEvents::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE;
    total += packet.WriteByte((Byte)mQueryType);
    switch(mQueryType)
    {
    case MessageID:
        expected += USHORT_SIZE;
        total += packet.Write(mQueryFilter);
        break;
    case EventType:
    case EventID:
    case AllEvents:
    default:
        expected += BYTE_SIZE;
        total += packet.WriteByte( (Byte)(mQueryFilter) );
        break;
    };
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
int QueryEvents::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE;
    total += packet.Read((Byte &)mQueryType);
    switch(mQueryType)
    {
    case MessageID:
        expected += USHORT_SIZE;
        total += packet.Read(mQueryFilter);
        break;
    case EventType:
    case EventID:
    case AllEvents:
    default:
        {
            Byte data = 0;
            expected += BYTE_SIZE;
            total += packet.Read( data );
            mQueryFilter = data;
        }
        break;
    };
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void QueryEvents::ClearMessageBody()
{
    mQueryType = MessageID;
    mQueryFilter = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
QueryEvents& QueryEvents::operator =(const QueryEvents& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mQueryType = message.mQueryType;
        mQueryFilter = message.mQueryFilter;
    }
    return *this;
}


/*  End of File */
