////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportstatus.h
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
#ifndef __JAUS_CORE_MANAGEMENT_REPORT_STATUS__H
#define __JAUS_CORE_MANAGEMENT_REPORT_STATUS__H

#include "jaus/core/corecodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportStatus
    ///   \brief This message is used to report the status of the service/component.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class ReportStatus : public Message
    {
    public:
        enum Status
        {
            Init = 0,
            Ready,
            Standby,
            Shutdown,
            Failure,
            Emergency
        };
        ReportStatus(const Address& dest = Address(), 
                     const Address& src = Address()) : Message(REPORT_STATUS, dest, src)
        {
            mStatus = Shutdown;
            mReserved = 0;
        }
        ReportStatus(const ReportStatus& message) : Message(REPORT_STATUS)
        {
            *this = message;
        }
        ~ReportStatus() 
        {
        }
        void SetStatus(const Byte status) { mStatus = status; }
        Byte GetStatus() const { return mStatus; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const
        {
            int expected = BYTE_SIZE + UINT_SIZE;
            int written = packet.Write(mStatus);
            written += packet.Write(mReserved);
            return expected == written ? written : -1;
        }
        virtual int ReadMessageBody(const Packet& packet)
        {
            int expected = BYTE_SIZE + UINT_SIZE;
            int read = packet.Read(mStatus);
            read += packet.Read(mReserved);
            return expected == read ? read : -1;
        }
        virtual Message* Clone() const { return new ReportStatus(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Status"; }
        virtual void ClearMessageBody() 
        {
            mStatus = Shutdown;
            mReserved = 0;
        }
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        ReportStatus& operator=(const ReportStatus& message)
        {
            CopyHeaderData(&message);
            mStatus = message.mStatus;
            mReserved = message.mReserved;
            return *this;
        }
    protected:
        Byte mStatus;   ///<  See status enumerations.
        UInt mReserved; ///<  Reserved for future versions of standard.
    };
}

#endif
/*  End of File */
