////////////////////////////////////////////////////////////////////////////////////
///
///  \file cancelevent.cpp
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
#include "jaus/core/events/cancelevent.h"
#include "jaus/core/events/confirmeventrequest.h"
#include "jaus/core/events/rejecteventrequest.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
CancelEvent::CancelEvent(const Address& dest,
                         const Address& src) : Message(CANCEL_EVENT, dest, src)
{
    mRequestID = mEventID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
CancelEvent::CancelEvent(const CancelEvent& message) : Message(CANCEL_EVENT)
{
    mRequestID = mEventID = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
CancelEvent::~CancelEvent()
{

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
bool CancelEvent::IsResponseToMessage(const Message *requestingMessage) const
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
int CancelEvent::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*2;
    total += packet.Write(mRequestID);
    total += packet.Write(mEventID);
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
int CancelEvent::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*2;
    total += packet.Read(mRequestID);
    total += packet.Read(mEventID);
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void CancelEvent::ClearMessageBody()
{
    mRequestID = mEventID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
CancelEvent& CancelEvent::operator =(const CancelEvent& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mRequestID = message.mRequestID;
        mEventID = message.mEventID;
    }
    return *this;
}


/*  End of File */
