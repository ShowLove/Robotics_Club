////////////////////////////////////////////////////////////////////////////////////
///
///  \file executelist.h
///  \brief This file contains the header of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 March 2010
///  <br>Copyright (c) 2010
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
#ifndef __JAUS_CORE_MOBILITY_EXECUTE_LIST__H
#define __JAUS_CORE_MOBILITY_EXECUTE_LIST__H

#include "jaus/mobility/mobilitycodes.h"
#include "jaus/core/message.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ExecuteList
    ///   \brief This message is used to delete one or more elements from a list.
    ///          Each element is uniquely identified by the UID.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ExecuteList : public Message
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
            const static Byte ElementUID = 0x01;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Limits
        ///   \brief Contains constants for limit values of data members of class.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_MOBILITY_DLL Limits : public JAUS::Limits
        {
        public:
            const static double MinSpeed;       ///<  Minimum speed. (0)
            const static double MaxSpeed;       ///<  Maximum speed. (327.67)
        };
        ExecuteList(const Address& dest = Address(), const Address& src = Address());
        ExecuteList(const ExecuteList& message);
        ~ExecuteList();
        bool SetSpeed(const double speed);
        UShort SetElementUID(const UShort uid);
        double GetSpeed() const { return mSpeed; }
        UShort GetElementUID() const { return mElementUID; }
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ExecuteList(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Execute List"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        ExecuteList& operator=(const ExecuteList& message);
    protected:
        Byte mPresenceVector;  ///<  Bit vector for fields present.
        double  mSpeed;        ///< Desired speed of the platform in m/s [0, 327.67].
        UShort  mElementUID;   ///< The starting UID optionally specified, where 0 is the head element.
    };
}

#endif
/*  End of File */
