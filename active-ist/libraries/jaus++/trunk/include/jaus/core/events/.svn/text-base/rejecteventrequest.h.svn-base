////////////////////////////////////////////////////////////////////////////////////
///
///  \file rejecteventrequest.h
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
#ifndef __JAUS_CORE_EVENTS_REJECT_EVENT_REQUEST__H
#define __JAUS_CORE_EVENTS_REJECT_EVENT_REQUEST__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"
#include <string>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RejectEventRequest
    ///   \brief This message is used to reject an event request.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL RejectEventRequest : public Message
    {
    public:
        // Response types for rejection of event request.
        enum Response
        {
            NoPeriodicEvents = 0,
            NoEveryChangeEvents,
            ConnectionRefused,
            InvalidSetup,
            MessageNotSupported,
            InvalidEventID
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static Byte ErrorMessage = 0x01; ///< Bitmask for ErrorMessage field.
        };
        RejectEventRequest(const Address& dest = Address(), const Address& src = Address());
        RejectEventRequest(const RejectEventRequest& message);
        ~RejectEventRequest();      
        inline Byte GetRequestID() const { return mRequestID; }  
        inline Byte GetResponseCode() const { return mResponseCode; } 
        inline std::string GetErrorMessage() const { return mErrorMessage; }
        inline Byte SetRequestID(const Byte id)  { return mRequestID = id; }  
        inline Byte SetResponseCode(const Response code)  { return mResponseCode = code; } 
        bool SetErrorMessage(const std::string& message);
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new RejectEventRequest(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Reject Event Request"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        RejectEventRequest& operator=(const RejectEventRequest& message);
    protected:
        Byte mPresenceVector;           ///<  Presence vector.
        Byte mRequestID;                ///<  Local request ID.
        Response mResponseCode;         ///<  Response code.
        std::string mErrorMessage;      ///<  Error message.
    };
}

#endif
/*  End of File */
