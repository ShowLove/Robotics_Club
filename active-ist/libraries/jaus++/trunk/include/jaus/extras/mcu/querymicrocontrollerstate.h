////////////////////////////////////////////////////////////////////////////////////
///
///  \file querymicrocontrollerstate.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 9 February 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_MCU_QUERY_MICROCONTROLLER__H
#define __JAUS_EXTRAS_MCU_QUERY_MICROCONTROLLER__H

#include "jaus/extras/extrascodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryMicrocontrollerState
    ///   \brief This message allows a component to query the state of digital
    ///          and analogue pins on a microcontroller.
    ///
    ///   If no digital or analog states are set in the query (e.g. 0 digital or 0
    ///   analog) then all digital or all analog or all of both are reported back.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL QueryMicrocontrollerState : public Message
    {
    public:
        QueryMicrocontrollerState(const Address& dest = Address(), 
                                  const Address& src = Address()) : Message(QUERY_MICROCONTROLLER_STATE, dest, src)
        {
        }
        QueryMicrocontrollerState(const QueryMicrocontrollerState& message) : Message(QUERY_MICROCONTROLLER_STATE)
        {
            *this = message;
        }
        ~QueryMicrocontrollerState() {}  
        void SetDigitalStates(const std::set<std::string>& digital) { mDigitalDevices = digital; }
        void SetAnalogStates(const std::set<std::string>& analog) { mAnalogDevices = analog; }
        std::set<std::string>* GetDigitalStates() { return &mDigitalDevices; }
        const std::set<std::string>* GetDigitalStates() const { return &mDigitalDevices; }
        std::set<std::string>* GetAnalogStates() { return &mAnalogDevices; }
        const std::set<std::string>* GetAnalogStates() const { return &mAnalogDevices; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new QueryMicrocontrollerState(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_MICROCONTROLLER_STATE; }
        virtual std::string GetMessageName() const { return "Query Microcontroller State"; }
        virtual void ClearMessageBody() {}
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const;
        QueryMicrocontrollerState& operator=(const QueryMicrocontrollerState& message)
        {
            CopyHeaderData(&message);
            mDigitalDevices = message.mDigitalDevices;
            mAnalogDevices = message.mAnalogDevices;
            return *this;
        }
    protected:
        std::set<std::string> mDigitalDevices;    ///<  The digital devices to query state for (if empty, all are returned).
        std::set<std::string> mAnalogDevices;     ///<  The analog devices to query state for (if empty, all are returned).
    };
}

#endif
/*  End of File */
