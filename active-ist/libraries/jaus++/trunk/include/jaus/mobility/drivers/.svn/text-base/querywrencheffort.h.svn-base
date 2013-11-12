////////////////////////////////////////////////////////////////////////////////////
///
///  \file querywrencheffort.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 24 November 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#ifndef __JAUS_MOBILITY_QUERY_WRENCH_EFFORT__H
#define __JAUS_MOBILITY_QUERY_WRENCH_EFFORT__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryWrenchEffort
    ///   \brief This message shall cause the receiving component to reply to the
    ///          requestor with a ID 4405h: ReportWrenchEffort message.  A logical
    ///          AND shall be performed on the requested presence vector and that 
    ///          representing the available fields from the responder.  The resulting
    ///          message shall contain the fields indicated by the result of this
    ///          logical AND operation.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL QueryWrenchEffort : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_MOBILITY_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static UShort PropulsiveLinearEffortX = 0x0001;
            const static UShort PropulsiveLinearEffortY = 0x0002;
            const static UShort PropulsiveLinearEffortZ = 0x0004;
            const static UShort PropulsiveRotationalEffortX = 0x0008;
            const static UShort PropulsiveRotationalEffortY = 0x0010;
            const static UShort PropulsiveRotationalEffortZ = 0x0020;
            const static UShort ResistiveLinearEffortX = 0x0040;
            const static UShort ResistiveLinearEffortY = 0x0080;
            const static UShort ResistiveLinearEffortZ = 0x0100;
            const static UShort ResistiveRotationalEffortX = 0x0200;
            const static UShort ResistiveRotationalEffortY = 0x0400;
            const static UShort ResistiveRotationalEffortZ = 0x0800;
        };
        QueryWrenchEffort(const Address& dest = Address(), const Address& src = Address());
        QueryWrenchEffort(const QueryWrenchEffort& message);
        ~QueryWrenchEffort();
        void SetPresenceVector(const UShort presenceVector) { mPresenceVector = presenceVector; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new QueryWrenchEffort(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x0FFF; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_WRENCH_EFFORT; }
        virtual std::string GetMessageName() const { return "Query Wrench Effort"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        QueryWrenchEffort& operator=(const QueryWrenchEffort& message);
    protected:
        UShort mPresenceVector;                 ///<  Bit vector for fields present.
    };
}

#endif
/*  End of File */
