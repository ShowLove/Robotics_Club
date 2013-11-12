////////////////////////////////////////////////////////////////////////////////////
///
///  \file SetVelocityVector.h
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

#ifndef __JAUS_EXTRAS_MOBILITY_SET_VELOCITY_VECTOR__H
#define __JAUS_EXTRAS_MOBILITY_SET_VELOCITY_VECTOR__H

#include "jaus/core/message.h"
#include "jaus/extras/extrascodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SetVelocityVector
    ///   \brief This message is used to provide the receiver the current values of
    ///          the commanded Velocity and heading.  The message data and mapping of the
    ///          presence vector for this message are identical to the Query Velocity Vector message
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL SetVelocityVector : public Message
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
            const static UShort Roll = 0x0010;          ///< Presence of the Roll field
            const static UShort Pitch = 0x0020;         ///< Presence of the Pitch field
            const static UShort Yaw = 0x0040;           ///< Presence of the Yaw field
            const static UShort TimeStamp = 0x080;      ///< Presence of the TimeStamp field
            const static UShort PositionX = 0x1000;     ///< Presence of the PositionX field
            const static UShort PositionY = 0x2000;     ///< Presence of the PositionY field
            const static UShort PositionZ = 0x4000;     ///< Presence of the PositionZ field
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Limits
        ///   \brief Contains constants for limit values of data members of class.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_EXTRAS_DLL Limits : public JAUS::Limits
        {
        public:
            const static double MinVelocity;            ///<  Minimum velocity of X,Y,Z in meters per second. (-327.68)
            const static double MaxVelocity;            ///<  Maximum velocity of X,Y,Z in meters per second. (327.67)
            const static double MinAngle;               ///<  Minimum angle value in radians. (-PI)
            const static double MaxAngle;               ///<  Maximum angle value in radians. (PI)
        };
        SetVelocityVector(const Address& dest = Address(), const Address& src = Address());
        SetVelocityVector(const SetVelocityVector& message);
        ~SetVelocityVector();
        bool SetVelocityX(const double value);
        bool SetVelocityY(const double value);
        bool SetVelocityZ(const double value);
        bool SetRoll(const double radians);
        bool SetPitch(const double value);
        bool SetYaw(const double value);
        bool SetPositionX(const double value);
        bool SetPositionY(const double value);
        bool SetPositionZ(const double value);
        bool SetTimeStamp(const Time& time);
        inline double GetVelocityX() const { return mVelocityX; }
        inline double GetVelocityY() const { return mVelocityY; }
        inline double GetVelocityZ() const { return mVelocityZ; }
        inline double GetRoll() const { return mRoll; }
        inline double GetPitch() const { return mPitch; }
        inline double GetYaw() const { return mYaw; }
        inline double GetPositionX() const { return mPositionX; }
        inline double GetPositionY() const { return mPositionY; }
        inline double GetPositionZ() const { return mPositionZ; }
        inline Time GetTimeStamp() const { return mTimeStamp; }
        virtual bool IsCommand() const { return true; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual void PrintMessageBody() const;
        virtual Message* Clone() const { return new SetVelocityVector(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01FF; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Velocity Vector"; }        
        virtual void ClearMessageBody();
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        SetVelocityVector& operator=(const SetVelocityVector& message);
        protected:
        UShort mPresenceVector;                 ///<  Bit vector for fields present.
        double mVelocityX;                      ///<  Velocity X in meters per second [-327.68, 327.67].
        double mVelocityY;                      ///<  Velocity Y in meters per second [-327.68, 327.67].
        double mVelocityZ;                      ///<  Velocity Z in meters per second [-327.68, 327.67].
        double mRoll;                           ///<  Roll in radians per second [-32.768, 32.767].
        double mPitch;                          ///<  Pitch in radians per second [-32.768, 32.767].
        double mYaw;                            ///<  Yaw in radians per second [-32.768, 32.767].
        double mPositionX;                      ///<  Position X in Meters (RHR)
        double mPositionY;                      ///<  Position Y in Meters (RHR)
        double mPositionZ;                      ///<  Position Z in Meters (RHR)
        Time mTimeStamp;                        ///<  TimeStamp, a tuple in (ms,sec,min,hour,day)

    };
}

#endif
/*  End of File */
