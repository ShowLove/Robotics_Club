////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportidentification.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 November 2009
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
#ifndef __JAUS_CORE_DISCOVERY_REPORT_IDENTIFICATION__H
#define __JAUS_CORE_DISCOVERY_REPORT_IDENTIFICATION__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportIdentification
    ///   \brief This message shall provide the requesting component an identification
    ///          summary of the Subsystem, Node, or Component.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL ReportIdentification : public Message
    {
    public:
        // Filters for types of event data to request.
        enum QueryType
        {
            Reserved = 0,
            SystemIdentification,
            SubsystemIdentification,
            NodeIdentification,
            ComponentIdentification
        };
        // The values for the particular unmanned vehicle (subsystem) type, node type or
        // component type.
        enum IdentificationType
        {
            Vehicle = 10001,
            OCU = 20001,
            OtherSubsystem = 30001,
            Node = 40001,
            Payload = 50001,
            Component = 60001
        };
        ReportIdentification(const Address& dest = Address(), const Address& src = Address());
        ReportIdentification(const ReportIdentification& message);
        ~ReportIdentification();      
        void SetQueryType(const QueryType type) { mQueryType = type; }
        void SetType(const IdentificationType type) { mIdentificationType = type; }
        bool SetIdentification(const std::string& identification);
        QueryType GetQueryType() const { return mQueryType; }
        IdentificationType GetType() const { return mIdentificationType; }
        std::string GetIdentification() const { return mIdentification; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportIdentification(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Identification"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        ReportIdentification& operator=(const ReportIdentification& message);
    protected:
        QueryType mQueryType;                   ///<  Type of query the report is responding to.
        IdentificationType mIdentificationType; ///<  Type of subsystem, node type, or component type.
        std::string mIdentification;            ///<  Identification name [up to 255 characters].
    };
}

#endif
/*  End of File */
