////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportconfiguration.h
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
#ifndef __JAUS_CORE_DISCOVERY_REPORT_CONFIGURATION__H
#define __JAUS_CORE_DISCOVERY_REPORT_CONFIGURATION__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"
#include <map>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportConfiguration
    ///   \brief This message shall provide the receiver a table of all existing
    ///          components located on the source's subsystem or node depending
    ///          on the value of Query Configuration message.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL ReportConfiguration : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Record
        ///   \brief Component information record for report.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Record
        {
        public:
            typedef std::vector<Record> List;
            Record(const Byte c = 0, const Byte i = 0)
            {
                mComponent = c;
                mInstance = i;
            }
            Record(const Record& record)
            {
                *this = record;
            }
            ~Record(){}
            Record& operator=(const Record& record)
            {
                mComponent = record.mComponent;
                mInstance = record.mInstance;
                return *this;
            }
            Byte mComponent;    ///< Component ID.
            Byte mInstance;     ///<  Instance ID (for legacy systems, otherwise 0 always).
        };
        typedef std::map<Byte, Record::List > Nodes;
        ReportConfiguration(const Address& dest = Address(), const Address& src = Address());
        ReportConfiguration(const ReportConfiguration& message);
        ~ReportConfiguration();      
        Nodes* GetNodes() { return &mNodes; }
        const Nodes* GetNodes() const { return &mNodes; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportConfiguration(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Configuration"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        ReportConfiguration& operator=(const ReportConfiguration& message);
    protected:
        Nodes mNodes; ///<  Node configuration data.
    };
}

#endif
/*  End of File */
