////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportvelocitystate.h
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
#ifndef __JAUS_MOBILITY_REPORT_VELOCITY_STATE__H
#define __JAUS_MOBILITY_REPORT_VELOCITY_STATE__H

#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportVelocityState
    ///   \brief This message is used to provide the receiver the linear velocity and
    ///          rotational rate of the platform.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ReportVelocityState : public Message
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
            const static UShort VelocityX = 0x0001;     ///< Presence of the VelocityX field
            const static UShort VelocityY = 0x0002;     ///< Presence of the VelocityY field
            const static UShort VelocityZ = 0x0004;     ///< Presence of the VelocityZ field
            const static UShort VelocityRMS = 0x0008;   ///< Presence of the VelocityRMS field
            const static UShort RollRate = 0x0010;      ///< Presence of the RollRate field
            const static UShort PitchRate = 0x0020;     ///< Presence of the PitchRate field
            const static UShort YawRate = 0x0040;       ///< Presence of the YawRate field
            const static UShort RateRMS = 0x0080;       ///< Presence of the RateRMS field
            const static UShort TimeStamp = 0x0100;     ///< Presence of the TimeStamp field
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
            const static double MinVelocity;            ///<  Minimum velocity of X,Y,Z in meters per second. (-327.68)
            const static double MaxVelocity;            ///<  Maximum velocity of X,Y,Z in meters per second. (327.67)
            const static double MinVelocityRMS;         ///<  Minimum velocity RMS inmeters per second (0)
            const static double MaxVelocityRMS;         ///<  Maximum velocity RMS in meters per second (100)
            const static double MinRotationalRate;      ///<  Minimum rotational rate in radians per second. (32.768)
            const static double MaxRotationalRate;      ///<  Maximum rotational rate in radians per second. (-32.767)
            const static double MinRateRMS;             ///<  Minimum rate RMS in radians per second. (0)
            const static double MaxRateRMS;             ///<  Maximum rate RMS in radians per second. (PI)
        };
        ReportVelocityState(const Address& dest = Address(), const Address& src = Address());
        ReportVelocityState(const ReportVelocityState& message);
        ~ReportVelocityState();
        bool SetVelocityX(const double value);
        bool SetVelocityY(const double value);
        bool SetVelocityZ(const double value);
        bool SetVelocityRMS(const double value);
        bool SetRollRate(const double radians);
        bool SetPitchRate(const double value);
        bool SetYawRate(const double value);
        bool SetRateRMS(const double value);
        bool SetTimeStamp(const Time& time);
        inline double GetVelocityX() const { return mVelocityX; }
        inline double GetVelocityY() const { return mVelocityY; }
        inline double GetVelocityZ() const { return mVelocityZ; }
        inline double GetVelocityRMS() const { return mVelocityRMS; }
        inline double GetRollRate() const { return mRollRate; }
        inline double GetPitchRate() const { return mPitchRate; }
        inline double GetYawRate() const { return mYawRate; }
        inline double GetRateRMS() const { return mRateRMS; }
        inline Time GetTimeStamp() const { return mTimeStamp; }
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportVelocityState(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01FF; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Velocity State"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        virtual void PrintMessageBody() const;
        ReportVelocityState& operator=(const ReportVelocityState& message);
    protected:
        UShort mPresenceVector;                 ///<  Bit vector for fields present.
        double mVelocityX;                      ///<  Velocity X in meters per second [-327.68, 327.67].
        double mVelocityY;                      ///<  Velocity Y in meters per second [[-327.68, 327.67].
        double mVelocityZ;                      ///<  Velocity Z in meters per second [[-327.68, 327.67].
        double mVelocityRMS;                    ///<  Velocity RMS value in meters per second [0, 100].
        double mRollRate;                       ///<  Roll Rate in radians per second [-32.768, 32.767].
        double mPitchRate;                      ///<  PitchR ate in radians per second [-32.768, 32.767].
        double mYawRate;                        ///<  Yaw Rate in radians per second [-32.768, 32.767].
        double mRateRMS;                        ///<  Rate RMS value in radians per second [0, PI].
        Time mTimeStamp;                        ///<  TimeStamp, a tuple in (ms,sec,min,hour,day)

    };
}

#endif
/*  End of File */
