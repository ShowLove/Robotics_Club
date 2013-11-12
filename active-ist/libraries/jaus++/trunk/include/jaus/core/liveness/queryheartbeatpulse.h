////////////////////////////////////////////////////////////////////////////////////
///
///  \file queryheartbeatpulse.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 17 October 2009
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
#ifndef __JAUS_CORE_LIVENESS_QUERY_HEARTBEAT_PULSE__H
#define __JAUS_CORE_LIVENESS_QUERY_HEARTBEAT_PULSE__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryHeartbeatPulse
    ///   \brief This message shall be used to query for a heartbeat pulse.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class QueryHeartbeatPulse : public Message
    {
    public:
        QueryHeartbeatPulse(const Address& dest = Address(), 
                            const Address& src = Address()) : Message(QUERY_HEARTBEAT_PULSE, dest, src)
        {
        }
        QueryHeartbeatPulse(const QueryHeartbeatPulse& message) : Message(QUERY_HEARTBEAT_PULSE)
        {
            *this = message;
        }
        ~QueryHeartbeatPulse() {}
        // Return true if a command message, otherwise false.
        virtual bool IsCommand() const { return false; }
        // Writes message payload data to the packet at the current write position.
        virtual int WriteMessageBody(Packet& packet) const { return 0; }
        // Reads message payload data from the packets current read position.
        virtual int ReadMessageBody(const Packet& packet) { return 0; }
        // Make a copy of the message and returns pointer to it.
        virtual Message* Clone() const { return new QueryHeartbeatPulse(*this); }
        // Gets the presence vector data for the message.
        virtual UInt GetPresenceVector() const { return 0; }
        // Get the size of the message presence vector. 0 value indicates no presence vector.
        virtual UInt GetPresenceVectorSize() const { return 0; }
        // Get the mask associated with presence vector.  Indicates what bits are used.
        virtual UInt GetPresenceVectorMask() const { return 0; }
        // Gets the response type to the associated message, 0 if message is a response type.
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_HEARTBEAT_PULSE; }
        // Gets the name of the message in human readable format (for logging, etc.)
        virtual std::string GetMessageName() const { return "Query Heartbeat Pulse"; }
        // Clears only message body information.
        virtual void ClearMessageBody() {}
        // Return true if payload is greater than maxPaylodSize (payload == message data only).
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        QueryHeartbeatPulse& operator=(const QueryHeartbeatPulse& message)
        {
            CopyHeaderData(&message);
            return *this;
        }
    };
}

#endif
/*  End of File */
