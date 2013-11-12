////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportaccelerationstate.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 3 December 2009
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
#ifndef __JAUS_MOBILITY_QUERY_ACCELERATION_STATE__H
#define __JAUS_MOBILITY_QUERY_ACCELERATION_STATE__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryAccelerationState
    ///   \brief This message shall cause the receiving component to reply to the
    ///          requestor with a ID 4417: ReportAccelerationState message.  A logical
    ///          AND shall be performed on the requested presence vector and that
    ///          representating the available fields from the responder.  The resulting
    ///          message shall contain the fields indicated by the result of this
    ///          lobical AND operation.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL QueryAccelerationState : public Message
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
            const static UShort AccelerationX = 0x0001;             ///< Presence of the AccelerationX field
            const static UShort AccelerationY = 0x0002;             ///< Presence of the AccelerationY field
            const static UShort AccelerationZ = 0x0004;             ///< Presence of the AccelerationZ field
            const static UShort AccelerationRMS = 0x0008;           ///< Presence of the AccelerationRMS field
            const static UShort RollAcceleration = 0x0010;          ///< Presence of the RollAcceleration field
            const static UShort PitchAcceleration = 0x0020;         ///< Presence of the PitchAcceleration field
            const static UShort YawAcceleration = 0x0040;           ///< Presence of the YawAcceleration field
            const static UShort RotationalAccelerationRMS = 0x0080; ///< Presence of the RotationalAccelerationRMS field
            const static UShort TimeStamp = 0x0100;                 ///< Presence of the TimeStamp field
        };
        QueryAccelerationState(const Address& dest = Address(), const Address& src = Address());
        QueryAccelerationState(const QueryAccelerationState& message);
        ~QueryAccelerationState();
        void SetPresenceVector(const UShort presenceVector) { mPresenceVector = presenceVector; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new QueryAccelerationState(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01FF; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_ACCELERATION_STATE; }
        virtual std::string GetMessageName() const { return "Query Acceleration State"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        QueryAccelerationState& operator=(const QueryAccelerationState& message);
    protected:
        UShort mPresenceVector;                ///<  Bit vector for fields present.
    };
}

#endif
/*  End of File */
