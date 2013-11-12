////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportglobalpose.h
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
#ifndef __JAUS_MOBILITY_REPORT_GLOBAL_POSE__H
#define __JAUS_MOBILITY_REPORT_GLOBAL_POSE__H

#include <cxutils/math/point3d.h>
#include "jaus/core/message.h"
#include "jaus/mobility/mobilitycodes.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ReportGlobalPose
    ///   \brief This message is used to provide the receiver the position and
    ///          attitude of the platform.  The position of the platform is given in
    ///          lattitude, longitude, and altitude, in accordance with the WGS 84
    ///          standard.  All times are in Coordinated Universal Time (UTC).  The
    ///          message data and mapping of the presence vector for this message are
    ///          identical to ID 0402h: SetGlobalPose.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL ReportGlobalPose : public Message
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
            const static UShort Latitude = 0x0001;      ///< Presence of the Lattitude field
            const static UShort Longitude = 0x0002;     ///< Presence of the Longitude field
            const static UShort Altitude = 0x0004;      ///< Presence of the Altitude field
            const static UShort PositionRMS = 0x0008;   ///< Presence of the PositionRMS field
            const static UShort Roll = 0x0010;          ///< Presence of the Roll field
            const static UShort Pitch = 0x0020;         ///< Presence of the Pitch field
            const static UShort Yaw = 0x0040;           ///< Presence of the Yaw field
            const static UShort AttitudeRMS = 0x0080;   ///< Presence of the AttitudeRMS field
            const static UShort TimeStamp = 0x0100;     ///< Presence of the Timestamp field
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
            const static double MinLatitude;            ///<  Minimum latitude. (-90)
            const static double MaxLatitude;            ///<  Maximum latitude. (90)
            const static double MinLongitude;           ///<  Minimum longitude. (-180)
            const static double MaxLongitude;           ///<  Maximum longitude. (180)
            const static double MinElevation;           ///<  Minimum altitude. (-10000)
            const static double MaxElevation;           ///<  Maximum altitude. (35000)
            const static double MinPositionRMS;         ///<  Minimum position RMS (0)
            const static double MaxPositionRMS;         ///<  Maximum position RMS (100)
            const static double MinAngle;               ///<  Minimum angle value in radians. (-PI)
            const static double MaxAngle;               ///<  Maximum angle value in radians. (PI)
            const static double MinAttitudeRMS;         ///<  Minimum attitude RMS in radians. (0)
            const static double MaxAttitudeRMS;         ///<  Maximum attitude RMS in radians. (PI)
        };
        ReportGlobalPose(const Address& dest = Address(), const Address& src = Address());
        ReportGlobalPose(const ReportGlobalPose& message);
        ~ReportGlobalPose();
        bool SetLatitude(const double degrees);
        bool SetLongitude(const double degrees);
        bool SetAltitude(const double value);
        bool SetPositionRMS(const double value);
        bool SetRoll(const double radians);
        bool SetPitch(const double radians);
        bool SetYaw(const double radians);
        bool SetAttitudeRMS(const double radians);
        bool SetTimeStamp(const JAUS::Time& time);
        bool SetPose(const Wgs& position,
                     const Point3D& orientation,
                     const Time& time = Time(true));
        bool SetPosition(const Wgs& position, const Time& time = Time(true));
        bool SetOrientation(const Point3D& orientation, const Time& time = Time(true));
        inline double GetLatitude() const { return mLatitude; }
        inline double GetLongitude() const { return mLongitude; }
        inline double GetAltitude() const { return mAltitude; }
        inline double GetPositionRMS() const { return mPositionRMS; }
        inline double GetRoll() const { return mRoll; }
        inline double GetPitch() const { return mPitch; }
        inline double GetYaw() const { return mYaw; }
        inline double GetAttitudeRMS() const { return mAttitudeRMS; }
        inline Time GetTimeStamp() const { return mTimeStamp; }
        Wgs GetPosition() const;
        Point3D GetOrientation() const;
        virtual bool IsCommand() const { return false; }
        virtual int WriteMessageBody(Packet& packet) const;
        virtual int ReadMessageBody(const Packet& packet);
        virtual Message* Clone() const { return new ReportGlobalPose(*this); }
        virtual UInt GetPresenceVector() const { return mPresenceVector; }
        virtual UInt GetPresenceVectorSize() const { return USHORT_SIZE; }
        virtual UInt GetPresenceVectorMask() const { return 0x01FF; }
        virtual UShort GetMessageCodeOfResponse() const { return 0; }
        virtual std::string GetMessageName() const { return "Report Global Pose"; }        
        virtual void ClearMessageBody();
        virtual void PrintMessageBody() const;
        virtual bool IsLargeDataSet(const unsigned int maxPayloadSize = 1437) const { return false; }
        virtual int RunTestCase() const;
        ReportGlobalPose& operator=(const ReportGlobalPose& message);
    protected:
        UShort mPresenceVector;                 ///<  Bit vector for fields present.
        double mLatitude;                       ///<  Latitude in degrees [-90, 90].
        double mLongitude;                      ///<  Longitude in degrees [-180, 180].
        double mAltitude;                       ///<  Altitude in meters [-10000, 35000].
        double mPositionRMS;                    ///<  Position RMS value in meters [0, 100].
        double mRoll;                           ///<  Roll in radians [-PI, PI].
        double mPitch;                          ///<  Pitch in radians [-PI, PI].
        double mYaw;                            ///<  Yaw in radians [-PI, PI].
        double mAttitudeRMS;                    ///<  Attitude RMS value in Radians [0, PI].
        Time mTimeStamp;                        ///<  TimeStamp, a tuple in (ms,sec,min,hour,day)

    };
}

#endif
/*  End of File */
