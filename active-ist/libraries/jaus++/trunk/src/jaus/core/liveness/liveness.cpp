////////////////////////////////////////////////////////////////////////////////////
///
///  \file liveness.cpp
///  \brief Contains the Liveness Service implementation.
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
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/events/event.h"
#include <iostream>

using namespace JAUS;

const std::string Liveness::Name = "urn:jaus:jss:core:Liveness";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Liveness::Liveness() : Child(Service::ID(Liveness::Name, 1.0), 
                             Service::ID(Events::Name, 1.0))
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Liveness::~Liveness()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates an event for the given information.
///
///   \param[in] info The event information (ID, Sequence #, etc.) for generation.
///
///   \return True if event generated, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Liveness::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_HEARTBEAT_PULSE)
    {
        ReportHeartbeatPulse report;
        SendEvent(info, &report);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the event is supported by the Service.
///
///   \param[in] type The event type (Periodic/EveryChange).
///   \param[in] requestedPeriodicRate If type == Periodic, then this is the
///                                    desired update rate.
///   \param[in] queryMessage The query message associated with the event.
///   \param[out] confirmedPeriodicRate This is the confirmed periodic rate 
///                                     supported by the Service.
///   \param[out] errorMessage If not supported, this is an optional error message.
///
///   \return True if event supported, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Liveness::IsEventSupported(const Events::Type type,
                                const double requestedPeriodicRate,
                                const Message* queryMessage,
                                double& confirmedPeriodicRate,
                                std::string& errorMessage) const
{
    // Support any type of event for REPORT_HEARTBEAT_PULSE data.
    if(queryMessage->GetMessageCode() == QUERY_HEARTBEAT_PULSE)
    {
        confirmedPeriodicRate = requestedPeriodicRate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports the following message: Query Heartbeat Pulse and
///   Report Heartbeat Pulse.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Liveness::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_HEARTBEAT_PULSE:
        {
            ReportHeartbeatPulse response(message->GetSourceID(), GetComponentID());
            Send(&response);
        }
        break;
    case REPORT_HEARTBEAT_PULSE:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Heartbeat Pulse from " << message->GetSourceID().ToString() << std::endl;
            }
        }
        break;
    default:
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Attempts to create the message desired.  Only message supported
///          by this Service can be created by this Service.
///
///   This Service supports the following message: Query Heartbeat Pulse and
///   Report Heartbeat Pulse.
///
///   \param[in] messageCode Message to create.
///
///   \return Pointer to newly allocated Message data, NULL if message is not
///           supported by the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Liveness::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_HEARTBEAT_PULSE:
        message = new QueryHeartbeatPulse();
        break;
    case REPORT_HEARTBEAT_PULSE:
        message = new ReportHeartbeatPulse();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
}


/*  End of File */
