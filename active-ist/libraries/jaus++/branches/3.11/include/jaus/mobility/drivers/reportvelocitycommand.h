////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportvelocitycommand.h
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
#ifndef __JAUS_MOBILITY_REPORT_VELOCITY_COMMAND__H
#define __JAUS_MOBILITY_REPORT_VELOCITY_COMMAND__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportVelocityCommand
    ///   \brief This message is used to command the linear velocity and rotational
    ///          rate of the platform.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ReportVelocityCommand : public Message
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
            const static Byte VelocityX = 0x01;
            const static Byte VelocityY = 0x02;
            const static Byte VelocityZ = 0x04;
            const static Byte RollRate = 0x08;
            const static Byte PitchRate = 0x10;
            const static Byte YawRate = 0x20;
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
            const static double MinVelocity;            ///<  Minimum velocity. (-327.68)
            const static double MaxVelocity;            ///<  Maximum velocity. (327.67)
            const static double MinRotationalRate;      ///<  Minimum Rotational Rate. (-32.768)
            const static double MaxRotationalRate;      ///<  Maximum Rotational Rate. (32.767)
        };
        // Command types when issuing commands.
        enum Command
        {
            SetCurrentCommand = 0,
            SetMaximumAllowedValues,
            SetMinimumAllowedValues,
            SetDefaultCommand
        };
        ReportVelocityCommand(const Address& dest = Address(), const Address& src = Address());
        ReportVelocityCommand(const ReportVelocityCommand& message);
        ~ReportVelocityCommand();
        inline Byte SetCommandType(const Byte type)  { return mCommandType = type; } 
        bool SetVelocityX(const double value);
        bool SetVelocityY(const double value);
        bool SetVelocityZ(const double value);
        bool SetRollRate(const double value);
        bool SetPitchRate(const double value);
        bool SetYawRate(const double value);
        inline Byte GetCommandType() const { return mCommandType; } 
        inline double GetVelocityX() const { return mVelocityX; }
        inline double GetVelocityY() const { return mVelocityY; }
        inline double GetVelocityZ() const { return mVelocityZ; }
        inline double GetRollRate() const { return mRollRate; }
        inline double GetPitchRate() const { return mPitchRate; }
        inline double GetYawRate() const { return mYawRate; }        
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportVelocityCommand(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return BYTE_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x3F; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Velocity Command"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        ReportVelocityCommand& operator=(const ReportVelocityCommand& message);
    protected:
        Byte mPresenceVector;                   ///<  Bit vector for fields present.
        Byte mCommandType;                      ///<  CommandType
        double mVelocityX;                      ///<  Velocity X in meters per second [-327.68, 327.67]
        double mVelocityY;                      ///<  Velocity Y in meters per second [-327.68, 327.67]
        double mVelocityZ;                      ///<  Velocity Z in meters per second [-327.68, 327.67]
        double mRollRate;                       ///<  Roll Rate in Radians per second [-32.768, 32.767]
        double mPitchRate;                      ///<  Pitch Rate in Radians per second [-32.768, 32.767]
        double mYawRate;                        ///<  Yaw Rate in Radians per second [-32.768, 32.767]
    };
}

#endif
/*  End of File */
