////////////////////////////////////////////////////////////////////////////////////
///
///  \file confirmeventrequest.cpp
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
#include "jaus/core/events/confirmeventrequest.h"
#include "jaus/core/scaledinteger.h"

using namespace JAUS;

const double ConfirmEventRequest::Limits::MaxUpdateRate = 1092.0;
const double ConfirmEventRequest::Limits::MinUpdateRate = 0.0;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ConfirmEventRequest::ConfirmEventRequest(const Address& dest,
                                         const Address& src) : Message(CONFIRM_EVENT_REQUEST, dest, src)
{
    mRequestID = mEventID = 0;
    mConfirmedPeriodicRate = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ConfirmEventRequest::ConfirmEventRequest(const ConfirmEventRequest& message) : Message(CONFIRM_EVENT_REQUEST)
{
    mRequestID = mEventID = 0;
    mConfirmedPeriodicRate = 0.0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ConfirmEventRequest::~ConfirmEventRequest()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the confirmed periodic update rate.  If the event type
///          is EveryChange, this value should be 0 (default).
///
///   \param[in] rate Requested periodic rate [0,1092] Hz.
///
///   \return True if set, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ConfirmEventRequest::SetConfirmedPeriodicRate(const double rate)
{
    if(rate >= Limits::MinUpdateRate && rate <= Limits::MaxUpdateRate)
    {
        mConfirmedPeriodicRate = rate;
        return true;
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
int ConfirmEventRequest::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*2 + USHORT_SIZE;
    total += packet.Write(mRequestID);
    total += packet.Write(mEventID);
    total += ScaledInteger::Write(packet,
                                  mConfirmedPeriodicRate,
                                  Limits::MaxUpdateRate,
                                  Limits::MinUpdateRate,
                                  ScaledInteger::UShort);
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
int ConfirmEventRequest::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*2 + USHORT_SIZE;
    total += packet.Read(mRequestID);
    total += packet.Read(mEventID);
    total += ScaledInteger::Read(packet,
                                 mConfirmedPeriodicRate,
                                 Limits::MaxUpdateRate,
                                 Limits::MinUpdateRate,
                                 ScaledInteger::UShort);
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ConfirmEventRequest::ClearMessageBody()
{
    mRequestID = mEventID = 0;
    mConfirmedPeriodicRate = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ConfirmEventRequest& ConfirmEventRequest::operator =(const ConfirmEventRequest& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mRequestID = message.mRequestID;
        mEventID = message.mEventID;
        mConfirmedPeriodicRate = message.mConfirmedPeriodicRate;
    }
    return *this;
}


/*  End of File */
