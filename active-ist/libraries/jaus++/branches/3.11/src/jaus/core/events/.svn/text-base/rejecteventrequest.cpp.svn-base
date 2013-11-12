////////////////////////////////////////////////////////////////////////////////////
///
///  \file rejecteventrequest.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 November 2009
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
#include "jaus/core/events/rejecteventrequest.h"
#include <cstring>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
RejectEventRequest::RejectEventRequest(const Address& dest,
                                       const Address& src) : Message(REJECT_EVENT_REQUEST, dest, src)
{
    mRequestID = mPresenceVector = 0;
    mResponseCode = MessageNotSupported;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RejectEventRequest::RejectEventRequest(const RejectEventRequest& message) : Message(REJECT_EVENT_REQUEST)
{
    mRequestID = mPresenceVector = 0;
    mResponseCode = MessageNotSupported;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RejectEventRequest::~RejectEventRequest()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the rejection error message.
///
///   \param[in] errorMessage Error message associated with rejection of event.
///                           Cannot be greater than 80 characters.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RejectEventRequest::SetErrorMessage(const std::string& errorMessage)
{
    if(errorMessage.size() <= 80)
    {
        mErrorMessage = errorMessage;
        mPresenceVector |= PresenceVector::ErrorMessage;
        return true;
    }
    mPresenceVector = 0;
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
int RejectEventRequest::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*3;
    total += packet.Write(mPresenceVector);
    total += packet.Write(mRequestID);
    total += packet.WriteByte((Byte)mResponseCode);
    if( (mPresenceVector & PresenceVector::ErrorMessage) > 0)
    {
        expected += 80;
        char buffer[81];
        memset(buffer, 0, 81);
        memcpy(buffer, mErrorMessage.c_str(), mErrorMessage.size());
        total += packet.Write((const unsigned char *)buffer, 80);
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
int RejectEventRequest::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*3;
    total += packet.Read(mPresenceVector);
    total += packet.Read(mRequestID);
    total += packet.Read((Byte &)mResponseCode);
    if( (mPresenceVector & PresenceVector::ErrorMessage) > 0)
    {
        expected += 80;
        char buffer[81];
        memset(buffer, 0, 81);
        total += packet.Read((unsigned char *)buffer, 80);
        mErrorMessage = buffer;        
    }
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void RejectEventRequest::ClearMessageBody()
{
    mRequestID = mPresenceVector = 0;
    mResponseCode = MessageNotSupported;
    mErrorMessage.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
RejectEventRequest& RejectEventRequest::operator =(const RejectEventRequest& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mRequestID = message.mRequestID;
        mPresenceVector = message.mPresenceVector;
        mErrorMessage = message.mErrorMessage;
        mResponseCode = message.mResponseCode;
    }
    return *this;
}


/*  End of File */
