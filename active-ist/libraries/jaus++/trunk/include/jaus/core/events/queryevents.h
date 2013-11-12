////////////////////////////////////////////////////////////////////////////////////
///
///  \file queryevents.h
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
#ifndef __JAUS_CORE_EVENTS_QUERY_EVENTS__H
#define __JAUS_CORE_EVENTS_QUERY_EVENTS__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"
#include "jaus/core/events/events.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryEvents
    ///   \brief This message is used to request detail on events.  Queries can be 
    ///   made by message ID, event type or Event ID.  If no filter is specified, all 
    ///   events should be reported.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL QueryEvents : public Message
    {
    public:
        // Filters for types of event data to request.
        enum Type
        {
            MessageID = 0,
            EventType,
            EventID,
            AllEvents
        };
        QueryEvents(const Address& dest = Address(), const Address& src = Address());
        QueryEvents(const QueryEvents& message);
        ~QueryEvents();      
        Type GetQueryType() const { return mQueryType; }
        UShort GetQueryFilter() const { return mQueryFilter; }
        void SetQueryType(const Type queryType) { mQueryType = queryType; }
        void SetQueryFilter(const UShort filterData) { mQueryFilter = filterData; }     
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new QueryEvents(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_EVENTS; }
        virtual std::string GetMessageName() const { return "Query Events"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        QueryEvents& operator=(const QueryEvents& message);
    protected:
        Type mQueryType;        ///< Query filter for requested event details.
        UShort mQueryFilter;    ///< Variable type based on mQueryType (could be message ID, event type, etc.)
    };
}

#endif
/*  End of File */
