////////////////////////////////////////////////////////////////////////////////////
///
///  \file setemergency.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 December 2009
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
#ifndef __JAUS_CORE_MANAGEMENT_SET_EMERGENCY__H
#define __JAUS_CORE_MANAGEMENT_SET_EMERGENCY__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SetEmergency
    ///   \brief This message is used to alert the component to a safety critical
    ///          situation.  The component that sends the emergency command shall
    ///          set the message priority to safety critical priority range as defined
    ///          by the Transport.  Receiving of an emergency command shall result
    ///          in the component transitioning into the emergency state.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class SetEmergency : public Message
    {
    public:
        SetEmergency(const Address& dest = Address(), 
                     const Address& src = Address()) : Message(SET_EMERGENCY, dest, src)
        {
            mEmergencyCode = 0;
        }
        SetEmergency(const SetEmergency& message) : Message(SET_EMERGENCY)
        {
            mEmergencyCode = 0;
            *this = message;
        }
        ~SetEmergency() 
        {
        }
        void SetEmergencyCode(const UShort code) { mEmergencyCode = code; }
        UShort GetEmergencyCode() const { return mEmergencyCode; }
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const { return packet.Write(mEmergencyCode); }
        virtual int ReadMessageBody(const Packet& packet) { return packet.Read(mEmergencyCode); }
        virtual Message* Clone() const { return new SetEmergency(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Set Emergency"; }
        virtual void ClearMessageBody() { mEmergencyCode = 0; }
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        SetEmergency& operator=(const SetEmergency& message)
        {
            CopyHeaderData(&message);
            mEmergencyCode = message.mEmergencyCode;
            return *this;
        }
    protected:
        UShort mEmergencyCode; ///< 1 = Stop.
    };
}

#endif
/*  End of File */
