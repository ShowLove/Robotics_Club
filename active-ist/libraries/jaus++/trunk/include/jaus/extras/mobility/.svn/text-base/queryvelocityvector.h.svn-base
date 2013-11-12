////////////////////////////////////////////////////////////////////////////////////
///
///  \file queryvelocityvector.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Brian Valentino
///  <br>Created: 12 March 2011
///  <br>Copyright (c) 2011
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: brian_valentino@knights.ucf.edu
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

#ifndef __JAUS_EXTRAS_MOBILITY_QUERY_VELOCITY_VECTOR__H
#define __JAUS_EXTRAS_MOBILITY_QUERY_VELOCITY_VECTOR__H

#include "jaus/core/message.h"
#include "jaus/extras/extrascodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class QueryVelocityVector
    ///   \brief This message shall cause the receiving component to reply to the
    ///          requestor with a ReportVelocityVector message.  A logical AND
    ///          shall be performed on the requested presence vector and that
    ///          representating the available fields from the responder.  The resulting
    ///          message shall contain the fields indicated by the result of this
    ///          lobical AND operation.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL QueryVelocityVector : public Message
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PresenceVector
        ///   \brief This class contains bit masks for bitwise operations on the
        ///          presence vector for this message.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_EXTRAS_DLL PresenceVector : public JAUS::PresenceVector
        {
        public:
            const static UShort VelocityX = 0x0001;     ///< Presence of the VelocityX field
            const static UShort VelocityY = 0x0002;     ///< Presence of the VelocityY field
            const static UShort VelocityZ = 0x0004;     ///< Presence of the VelocityZ field
            const static UShort Roll = 0x0010;          ///< Presence of the RollRate field
            const static UShort Pitch = 0x0020;         ///< Presence of the PitchRate field
            const static UShort Yaw = 0x0040;           ///< Presence of the YawRate field
            const static UShort TimeStamp = 0x080;      ///< Presence of the TimeStamp field
            const static UShort PositionX = 0x1000;     ///< Presence of the PositionX field
            const static UShort PositionY = 0x2000;     ///< Presence of the PositionY field
            const static UShort PositionZ = 0x4000;     ///< Presence of the PositionZ field            
        };
        QueryVelocityVector(const Address& dest = Address(), const Address& src = Address());
        QueryVelocityVector(const QueryVelocityVector& message);
        ~QueryVelocityVector();
        void SetPresenceVector(const UShort presenceVector) { mPresenceVector = presenceVector; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new QueryVelocityVector(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01FF; }
        virtual UShort GetMessageCodeOfResponse() const { return REPORT_VELOCITY_VECTOR; }
        virtual std::string GetMessageName() const { return "Query Velocity Vector"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        QueryVelocityVector& operator=(const QueryVelocityVector& message);
    protected:
        UShort mPresenceVector;                   ///<  Bit vector for fields present.
    };
}

#endif
/*  End of File */
