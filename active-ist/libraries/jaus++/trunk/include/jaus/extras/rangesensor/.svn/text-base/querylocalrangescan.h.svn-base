////////////////////////////////////////////////////////////////////////////////////
///
///  \file querylocalrangescan.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 8 March 2010
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
#ifndef __JAUS_EXTRAS_RANGE_SENSOR_QUERY_RANGE_SENSOR_LOCAL_SCAN__H
#define __JAUS_EXTRAS_RANGE_SENSOR_QUERY_RANGE_SENSOR_LOCAL_SCAN__H

#include "jaus/extras/extrascodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryLocalRangeScan
    ///   \brief This message allows a component to query the current scan from a
    ///          range sensor device.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL QueryLocalRangeScan : public Message
    {
    public:
        QueryLocalRangeScan(const Address& dest = Address(), 
                            const Address& src = Address()) : Message(QUERY_LOCAL_RANGE_SCAN, dest, src)
        {
            mSensorID = 0;
        }
        QueryLocalRangeScan(const QueryLocalRangeScan& message) : Message(QUERY_LOCAL_RANGE_SCAN)
        {
            mSensorID = 0;
            *this = message;
        }
        ~QueryLocalRangeScan() {}  
        void SetSensorID(const Byte id) { mSensorID = id; }
        Byte GetSensorID() const { return mSensorID; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const { return packet.Write(mSensorID); }
        virtual int ReadMessageBody(const Packet& packet) { return packet.Read(mSensorID); }
        virtual Message* Clone() const { return new QueryLocalRangeScan(*this); }
        virtual UInt GetPresenceVector() const { return 0; }
        virtual UInt GetPresenceVectorSize() const { return 0; }
        virtual UInt GetPresenceVectorMask() const { return 0; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_LOCAL_RANGE_SCAN; }
        virtual std::string GetMessageName() const { return "Query Local Range Scan"; }
        virtual void ClearMessageBody() {}
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize) const { return false; }
        QueryLocalRangeScan& operator=(const QueryLocalRangeScan& message)
        {
            CopyHeaderData(&message);
            mSensorID = message.mSensorID;
            return *this;
        }
    protected:
        Byte mSensorID; ///<  ID of the sensor to query data from.
    };
}

#endif
/*  End of File */
