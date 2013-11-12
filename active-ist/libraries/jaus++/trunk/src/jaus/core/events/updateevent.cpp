////////////////////////////////////////////////////////////////////////////////////
///
///  \file updateevent.cpp
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
#include "jaus/core/events/updateevent.h"
#include "jaus/core/scaledinteger.h"
#include "jaus/core/events/confirmeventrequest.h"
#include "jaus/core/events/rejecteventrequest.h"

using namespace JAUS;

const double UpdateEvent::Limits::MaxUpdateRate = 1092.0;
const double UpdateEvent::Limits::MinUpdateRate = 0.0;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
UpdateEvent::UpdateEvent(const Address& dest,
                         const Address& src) : Message(UPDATE_EVENT, dest, src)
{
    mType = Events::EveryChange;
    mRequestID = 0;
    mEventID = 0;
    mRequestedPeriodicRate = 1;
    mQueryMessageCode = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UpdateEvent::UpdateEvent(const UpdateEvent& message) : Message(UPDATE_EVENT)
{
    mType = Events::EveryChange;
    mRequestID = 0;
    mEventID = 0;
    mRequestedPeriodicRate = 1;
    mQueryMessageCode = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UpdateEvent::~UpdateEvent()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the requested periodic update rate.  If the event type
///          is EveryChange, this value should be 0.
///
///   \param[in] rate Requested periodic rate [0,1092] Hz.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UpdateEvent::SetRequestedPeriodicRate(const double rate)
{
    if(rate >= Limits::MinUpdateRate && rate <= Limits::MaxUpdateRate)
    {
        mRequestedPeriodicRate = rate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the query message which will determine the type of Report for
///          the event.
///
///   \param[in] queryMessage Pointer to Message object to get Message Code and
///                           payload for query.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UpdateEvent::SetQueryMessage(const Message* queryMessage)
{
    mQueryMessage.Clear();
    if(queryMessage && queryMessage->WriteMessageBody(mQueryMessage))
    {
        mQueryMessageCode = queryMessage->GetMessageCode();
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the query message which will determine the type of Report for
///          the event.
///
///   \param[in] messageCode Query message code which will determine Report type.
///   \param[in] queryPayload Query payload data (serialized message body only,
///                           no 2 byte message code).
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UpdateEvent::SetQueryMessage(const UShort messageCode,
                                  const Packet& queryPayload)
{
    mQueryMessageCode = messageCode;
    mQueryMessage = queryPayload;
    return true;
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
bool UpdateEvent::IsResponseToMessage(const Message *requestingMessage) const
{
    if(mSourceID == requestingMessage->GetDestinationID())
    {
        if(requestingMessage->GetMessageCode() == CONFIRM_EVENT_REQUEST)
        {
            const ConfirmEventRequest* original = dynamic_cast<const ConfirmEventRequest*>(requestingMessage);
            if(original && original->GetRequestID() == mRequestID)
            {
                return true;
            }
        }
        else if(requestingMessage->GetMessageCode() == REJECT_EVENT_REQUEST)
        {
            const RejectEventRequest* original = dynamic_cast<const RejectEventRequest*>(requestingMessage);
            {
                return true;
            }
        }
    }
    return false;
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
int UpdateEvent::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*3 + USHORT_SIZE + UINT_SIZE + USHORT_SIZE;
    total += packet.Write(mRequestID);
    total += packet.WriteByte((Byte)mType);
    total += ScaledInteger::Write(packet,
                                  mRequestedPeriodicRate,
                                  Limits::MaxUpdateRate,
                                  Limits::MinUpdateRate,
                                  ScaledInteger::UShort);
    total += packet.Write(mEventID);
    total += packet.Write( (UInt)(mQueryMessage.Size() + USHORT_SIZE ) );
    total += packet.Write( mQueryMessageCode );
    if(mQueryMessage.Size() > 0)
    {
        expected += mQueryMessage.Size();
        total += packet.Write(mQueryMessage);
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
int UpdateEvent::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*3 + USHORT_SIZE + UINT_SIZE + USHORT_SIZE;
    UInt payloadSize = 0;
    total += packet.Read(mRequestID);
    total += packet.Read((Byte &)mType);
    total += ScaledInteger::Read(packet,
                                  mRequestedPeriodicRate,
                                  Limits::MaxUpdateRate,
                                  Limits::MinUpdateRate,
                                  ScaledInteger::UShort);
    total += packet.Read(mEventID);
    total += packet.Read( payloadSize );    
    if(payloadSize >= USHORT_SIZE)
    {
        expected += payloadSize - USHORT_SIZE;
        total += packet.Read( mQueryMessageCode );
        total += packet.Read(mQueryMessage, payloadSize - USHORT_SIZE);
    }
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void UpdateEvent::ClearMessageBody()
{
    mType = Events::EveryChange;
    mRequestID = 0;
    mEventID = 0;
    mRequestedPeriodicRate = 1;
    mQueryMessageCode = 0;
    mQueryMessage.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
UpdateEvent& UpdateEvent::operator =(const UpdateEvent& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mType = message.mType;
        mEventID = message.mEventID;
        mRequestID = message.mRequestID;
        mRequestedPeriodicRate = message.mRequestedPeriodicRate;
        mQueryMessageCode = message.mQueryMessageCode;
        mQueryMessage = message.mQueryMessage;
    }
    return *this;
}


/*  End of File */
