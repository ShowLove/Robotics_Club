////////////////////////////////////////////////////////////////////////////////////
///
///  \file setaccelerationlimit.h
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 2 December 2009
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
#ifndef __JAUS_MOBILITY_SET_ACCELERATION_LIMIT__H
#define __JAUS_MOBILITY_SET_ACCELERATION_LIMIT__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SetAccelerationLimit
    ///   \brief This message is used to command the linear and rotational
    ///          acceleration limits of the platform.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL SetAccelerationLimit : public Message
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
            const static Byte AccelerationX = 0x01;
            const static Byte AccelerationY = 0x02;
            const static Byte AccelerationZ = 0x04;
            const static Byte RollAcceleration = 0x08;
            const static Byte PitchAcceleration = 0x10;
            const static Byte YawAcceleration = 0x20;
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
            const static double MinLinearAcceleration;            ///<  Minimum Linear Acceleration. (-1310.68)
            const static double MaxLinearAcceleration;            ///<  Maximum Linear Acceleration. (1310.68)
            const static double MinRotationalAcceleration;        ///<  Minimum Rotational Acceleration. (-13106.8)
            const static double MaxRotationalAcceleration;        ///<  Maximum Rotational Acceleration. (13106.8)
        };

        // Command types when issuing commands.
        enum Command
        {
            SetMaximumAllowedValues = 0,
            SetMinimumAllowedValues
        };
        SetAccelerationLimit(const Address& dest = Address(), const Address& src = Address());
        SetAccelerationLimit(const SetAccelerationLimit& message);
        ~SetAccelerationLimit();
        inline Byte SetCommandType(const Byte type)  { return mCommandType = type; } 
        bool SetAccelerationX(const double value);
        bool SetAccelerationY(const double value);
        bool SetAccelerationZ(const double value);
        bool SetRollAcceleration(const double value);
        bool SetPitchAcceleration(const double value);
        bool SetYawAcceleration(const double value);
        inline Byte GetCommandType() const { return mCommandType; } 
        inline double GetAccelerationX() const { return mAccelerationX; }
        inline double GetAccelerationY() const { return mAccelerationY; }
        inline double GetAccelerationZ() const { return mAccelerationZ; }
        inline double GetRollAcceleration() const { return mRollAcceleration; }
        inline double GetPitchAcceleration() const { return mPitchAcceleration; }
        inline double GetYawAcceleration() const { return mYawAcceleration; }        
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new SetAccelerationLimit(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x3F; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Set Acceleration Limit"; }
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        SetAccelerationLimit& operator=(const SetAccelerationLimit& message);
    protected:
        Byte mPresenceVector;                   ///<  Bit vector for fields present.
        Byte mCommandType;                      ///<  CommandType
        double mAccelerationX;                  ///<  Acceleration X in meters per second squared [-1310.68, 1310.68]
        double mAccelerationY;                  ///<  Acceleration Y in meters per second squared [-1310.68, 1310.68]
        double mAccelerationZ;                  ///<  Acceleration Z in meters per second squared [-1310.68, 1310.68]
        double mRollAcceleration;               ///<  Roll Acceleration in Radians per second squared [-13106.8, 13106.8]
        double mPitchAcceleration;              ///<  Pitch Acceleration in Radians per second squared [-13106.8, 13106.8]
        double mYawAcceleration;                ///<  Yaw Acceleration in Radians per second squared [-13106.8, 13106.8]
    };
}

#endif
/*  End of File */
