////////////////////////////////////////////////////////////////////////////////////
///
///  \file setglobalpose.cpp
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
#include "jaus/mobility/sensors/setglobalpose.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::SetGlobalPose::Limits::MinLatitude = -90.0;                  
const double JAUS::SetGlobalPose::Limits::MaxLatitude = 90.0;
const double JAUS::SetGlobalPose::Limits::MinLongitude = -180.0;
const double JAUS::SetGlobalPose::Limits::MaxLongitude = 180.0;
const double JAUS::SetGlobalPose::Limits::MinElevation = -10000.0;
const double JAUS::SetGlobalPose::Limits::MaxElevation = 35000.0;
const double JAUS::SetGlobalPose::Limits::MinPositionRMS = 0.0;
const double JAUS::SetGlobalPose::Limits::MaxPositionRMS = 100.0;
const double JAUS::SetGlobalPose::Limits::MinAngle = -CxUtils::CX_PI;
const double JAUS::SetGlobalPose::Limits::MaxAngle = CxUtils::CX_PI;
const double JAUS::SetGlobalPose::Limits::MinAttitudeRMS = 0.0;
const double JAUS::SetGlobalPose::Limits::MaxAttitudeRMS = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPose::SetGlobalPose(const Address& dest, const Address& src) : Message(SET_GLOBAL_POSE, dest, src)
{
    mPresenceVector = 0;
    mLatitude = 0;
    mLongitude = 0;
    mAltitude = 0;
    mRoll = 0;
    mPositionRMS = 0;
    mPitch = 0;
    mYaw = 0;
    mAttitudeRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPose::SetGlobalPose(const SetGlobalPose& message) : Message(SET_GLOBAL_POSE)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPose::~SetGlobalPose()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the latitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired latitude in degrees [-90, 90].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetLatitude(const double degrees)
{
    if(degrees >= Limits::MinLatitude && degrees <= Limits::MaxLatitude)
    {
        mLatitude = degrees;
        mPresenceVector |= PresenceVector::Latitude;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the longitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired longitude in degrees [-180, 180].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetLongitude(const double degrees)
{
    if(degrees >= Limits::MinLongitude && degrees <= Limits::MaxLongitude)
    {
        mLongitude = degrees;
        mPresenceVector |= PresenceVector::Longitude;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired Altitude in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetAltitude(const double value)
{
    if(value >= Limits::MinElevation && value <= Limits::MaxElevation)
    {
        mAltitude = value;
        mPresenceVector |= PresenceVector::Altitude;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Position RMS and updates the presence vector for the
///          message.
///
///   \param[in] value Desired Position RMS in meters [0, 100].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetPositionRMS(const double value)
{
    if(value >= Limits::MinPositionRMS && value <= Limits::MaxPositionRMS)
    {
        mPositionRMS = value;
        mPresenceVector |= PresenceVector::PositionRMS;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the roll and updates the presence vector for the message.
///
///   \param[in] radians Desired roll in radians[-PI, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetRoll(const double radians)
{
    if(radians >= Limits::MinAngle && radians <= Limits::MaxAngle)
    {
        mRoll = radians;
        mPresenceVector |= PresenceVector::Roll;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pitch value and updates the presence vector for the message.
///
///   \param[in] radians Desired pitch in radians[-PI, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetPitch(const double radians)
{
    if(radians >= Limits::MinAngle && radians <= Limits::MaxAngle)
    {
        mPitch = radians;
        mPresenceVector |= PresenceVector::Pitch;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the yaw value and updates the presence vector for the message.
///
///   \param[in] radians Desired pitch in radians[-PI, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetYaw(const double radians)
{
    if(radians >= Limits::MinAngle && radians <= Limits::MaxAngle)
    {
        mYaw = radians;
        mPresenceVector |= PresenceVector::Yaw;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the attitude rms value and updates the presence vector for
///          the message.
///
///   \param[in] radians Desired attitude RMS in radians [0, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetAttitudeRMS(const double radians)
{
    if(radians >= Limits::MinAttitudeRMS && radians <= Limits::MaxAttitudeRMS)
    {
        mAttitudeRMS = radians;
        mPresenceVector |= PresenceVector::AttitudeRMS;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Time Stamp value and updates the presence vector for
///          the message.
///
///   \param[in] time Desired Time Stamp as a JAUS::Time instance.
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetTimeStamp(const JAUS::Time& time)
{
    mTimeStamp = time;
    mPresenceVector |= PresenceVector::TimeStamp;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets Lattitude, Longitude, Altitude, Roll, Pitch, Yaw, and Time.
///          Overrides any existing data.
///
///   \param[in] position Desired Lattitude(X), Longitude(Y), Altitude(Z) stored
///              as a Point3D.
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetPose(const Wgs& position,
                            const Point3D& orientation,
                            const Time& time)
{
    //Check bounds before proceeding.
    if(position.mLatitude >= Limits::MinLatitude && position.mLatitude <= Limits::MaxLatitude &&
       position.mLongitude >= Limits::MinLongitude && position.mLongitude <= Limits::MaxLongitude && 
       position.mElevation >= Limits::MinElevation && position.mElevation <= Limits::MaxElevation &&
       orientation.mX >= Limits::MinAngle && orientation.mX <= Limits::MaxAngle && 
       orientation.mY >= Limits::MinAngle && orientation.mY <= Limits::MaxAngle && 
       orientation.mZ >= Limits::MinAngle && orientation.mZ <= Limits::MaxAngle)
    {
        mLatitude = position.mLatitude;
        mLongitude = position.mLongitude;
        mAltitude = position.mElevation;
        mRoll = orientation.mX;
        mPitch = orientation.mY;
        mYaw = orientation.mZ;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::Latitude | PresenceVector::Longitude | PresenceVector::Altitude |
                           PresenceVector::Roll | PresenceVector::Pitch | PresenceVector::Yaw |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets Lattitude, Longitude, Altitude, and Time.
///          Overrides any existing data.
///
///   \param[in] position Desired Lattitude(X), Longitude(Y), Altitude(Z) stored
///              as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetPosition(const Wgs& position, const Time& time)
{
    //Check bounds before proceeding.
    if(position.mLatitude >= Limits::MinLatitude && position.mLatitude <= Limits::MaxLatitude &&
       position.mLongitude >= Limits::MinLongitude && position.mLongitude <= Limits::MaxLongitude && 
       position.mElevation >= Limits::MinElevation && position.mElevation <= Limits::MaxElevation)
    {
        mLatitude = position.mLatitude;
        mLongitude = position.mLongitude;
        mAltitude = position.mElevation;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::Latitude | PresenceVector::Longitude | PresenceVector::Altitude |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets Roll, Pitch, Yaw, and Time.  Overrides any existing data.
///
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPose::SetOrientation(const Point3D& orientation, const Time& time)
{
    //Check bounds before proceeding.
    if(orientation.mX >= Limits::MinAngle && orientation.mX <= Limits::MaxAngle && 
       orientation.mY >= Limits::MinAngle && orientation.mY <= Limits::MaxAngle && 
       orientation.mZ >= Limits::MinAngle && orientation.mZ <= Limits::MaxAngle)
    {
        mRoll = orientation.mX;
        mPitch = orientation.mY;
        mYaw = orientation.mZ;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::Roll | PresenceVector::Pitch | PresenceVector::Yaw |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets Lattitude, Longitude, Altitude as a Point3D.
///
///   \return Position Desired Lattitude(X), Longitude(Y), Altitude(Z) stored
///           as a Point3D.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs SetGlobalPose::GetPosition() const
{
    Wgs position;
    position.Set(mLatitude, mLongitude, mAltitude);
    return position;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets Roll, Pitch, Yaw as a Point3D.
///
///   \return Orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D SetGlobalPose::GetOrientation() const
{
    Point3D orientation;
    orientation.Set(mRoll, mPitch, mYaw);
    return orientation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetGlobalPose::WriteMessageBody(Packet& packet) const
{
    int expected = USHORT_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    if((mPresenceVector & PresenceVector::Latitude) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mLatitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Longitude) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mLongitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Altitude) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mAltitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::PositionRMS) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mPositionRMS, Limits::MaxPositionRMS, Limits::MinPositionRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Roll) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mRoll, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Pitch) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mPitch, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Yaw) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mYaw, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::AttitudeRMS) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mAttitudeRMS, Limits::MaxAttitudeRMS, Limits::MinAttitudeRMS, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::TimeStamp) > 0)
    {
        expected += UINT_SIZE;
        written += packet.Write(mTimeStamp.ToUInt());
    }

    return expected == written ? written : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads message payload from the packet.
///
///   Message contents are read from the packet following the JAUS standard.
///
///   \param[in] packet Packet containing message payload data to read.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetGlobalPose::ReadMessageBody(const Packet& packet)
{
    int expected = USHORT_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    if((mPresenceVector & PresenceVector::Latitude) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mLatitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Longitude) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mLongitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Altitude) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mAltitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::PositionRMS) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mPositionRMS, Limits::MaxPositionRMS, Limits::MinPositionRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Roll) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mRoll, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Pitch) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mPitch, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Yaw) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mYaw, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::AttitudeRMS) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mAttitudeRMS, Limits::MaxAttitudeRMS, Limits::MinAttitudeRMS, ScaledInteger::UShort);
    }

    if((mPresenceVector & PresenceVector::TimeStamp) > 0)
    {
        UInt time = 0;
        expected += UINT_SIZE;
        read += packet.Read(time);
        mTimeStamp.SetTime(time);
    }

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void SetGlobalPose::ClearMessageBody()
{
    mPresenceVector = 0;
    mLatitude = 0;
    mLongitude = 0;
    mAltitude = 0;
    mPositionRMS = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mAttitudeRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SetGlobalPose::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetGlobalPose msg1, msg2;

    msg1.SetLongitude(8);
    msg1.SetPositionRMS(2);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::Longitude | PresenceVector::PositionRMS) )
         {
                result = 1;
         }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPose& SetGlobalPose::operator=(const SetGlobalPose& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mLatitude = message.mLatitude;
        mLongitude = message.mLongitude;
        mAltitude = message.mAltitude;
        mPositionRMS = message.mPositionRMS;
        mRoll = message.mRoll;
        mPitch = message.mPitch;
        mYaw = message.mYaw;
        mAttitudeRMS = message.mAttitudeRMS;
        mTimeStamp = message.mTimeStamp;
        
    }
    return *this;
}


/*  End of File */
