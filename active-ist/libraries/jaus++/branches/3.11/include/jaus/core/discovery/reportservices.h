////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportservices.h
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
#ifndef __JAUS_CORE_DISCOVERY_REPORT_SERVICES__H
#define __JAUS_CORE_DISCOVERY_REPORT_SERVICES__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"
#include "jaus/core/service.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportServices
    ///   \brief This message allows a component to publish its capabilities, 
    ///          according to the Service Dictionary presented.  If a component
    ///          ID is specified in the RA, it may report only one service beyond the
    ///          core message support, and this service must be equal to the
    ///          component ID.  If a component ID is not listed in the RA, it can
    ///          report any number of services.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL ReportServices : public Message
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
                mServices = record.mServices;
                return *this;
            }
            Byte mComponent;    ///< Component ID.
            Byte mInstance;     ///<  Instance ID (for legacy systems, otherwise 0 always).
            Service::ID::Set mServices;    ///<  Services supported by the component.
        };
        typedef std::map<Byte, Record::List > Services;
        ReportServices(const Address& dest = Address(), const Address& src = Address());
        ReportServices(const ReportServices& message);
        ~ReportServices();      
        Services* GetServices() { return &mServices; }
        const Services* GetServices() const { return &mServices; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportServices(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Services"; }
        virtual void ClearMessageBody();
        virtual void PrintMessageBody() const;
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const;
        ReportServices& operator=(const ReportServices& message);
    protected:
        Services mServices;  ///<  The services list.
    };
}

#endif
/*  End of File */
