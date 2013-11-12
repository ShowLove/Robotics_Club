////////////////////////////////////////////////////////////////////////////////////
///
///  \file updateevent.h
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
#ifndef __JAUS_CORE_EVENTS_UPDATE_EVENT__H
#define __JAUS_CORE_EVENTS_UPDATE_EVENT__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"
#include "jaus/core/events/events.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class UpdateEvent
    ///   \brief This message allows the requestor to request a rate or change
    ///          for an existing event.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL UpdateEvent : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Limits
        ///   \brief Contains limits of specific fields in message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Limits : public JAUS::Limits
        {
        public:
            static const double MaxUpdateRate;  ///<  Maximum periodic update rate.
            static const double MinUpdateRate;  ///<  Minimum periodic update rate.
        };
        UpdateEvent(const Address& dest = Address(), const Address& src = Address());
        UpdateEvent(const UpdateEvent& message);
        ~UpdateEvent();        
        inline void SetType(const Events::Type type) { mType = type; }
        inline void SetEventID(const Byte id) { mEventID = id; }
        inline void SetRequestID(const Byte id) { mRequestID = id; }
        bool SetRequestedPeriodicRate(const double rate);
        bool SetQueryMessage(const Message* queryMessage);
        bool SetQueryMessage(const UShort messageCode, const Packet& queryPayload);
        inline Events::Type GetType() const { return mType; }
        inline Byte GetEventID() const { return mEventID; }
        inline Byte GetRequestID() const { return mRequestID; }
        inline double GetRequestedPeriodicRate() const { return mRequestedPeriodicRate; }
        inline UShort GetQueryMessageCode() const { return mQueryMessageCode; }
        inline const Packet* GetQueryMessage() const { return &mQueryMessage; }
        virtual bool IsCommand() const { return false; }
        virtual bool IsResponseToMessage(const Message* requestingMessage) const;
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new UpdateEvent(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return CONFIRM_EVENT_REQUEST; }
        virtual std::string GetMessageName() const { return "Update Event"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        UpdateEvent& operator=(const UpdateEvent& message);
    protected:
        Events::Type mType;                 ///<  Event type.
        Byte mRequestID;                    ///<  Local request ID field.
        Byte mEventID;                      ///<  Event ID to be updated.
        double mRequestedPeriodicRate;      ///<  Requested periodic event rate [0-1092] Hz.
        UShort mQueryMessageCode;           ///<  Messge code for the Query Message specifying type of Report.
        Packet mQueryMessage;               ///<  Contents of the query message.
    };
}

#endif
/*  End of File */
