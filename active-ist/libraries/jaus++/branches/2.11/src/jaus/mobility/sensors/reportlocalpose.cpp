////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportlocalpose.cpp
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
#include "jaus/mobility/sensors/reportlocalpose.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <iomanip>

const double JAUS::ReportLocalPose::Limits::MinPoint = -100000.0;
const double JAUS::ReportLocalPose::Limits::MaxPoint = 100000.0;
const double JAUS::ReportLocalPose::Limits::MinPositionRMS = 0.0;
const double JAUS::ReportLocalPose::Limits::MaxPositionRMS = 100.0;
const double JAUS::ReportLocalPose::Limits::MinAngle = -CxUtils::CX_PI;
const double JAUS::ReportLocalPose::Limits::MaxAngle = CxUtils::CX_PI;
const double JAUS::ReportLocalPose::Limits::MinAttitudeRMS = 0.0;
const double JAUS::ReportLocalPose::Limits::MaxAttitudeRMS = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportLocalPose::ReportLocalPose(const Address& dest, const Address& src) : Message(REPORT_LOCAL_POSE, dest, src)
{
    mPresenceVector = 0;
    mX = 0;
    mY = 0;
    mZ = 0;
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
ReportLocalPose::ReportLocalPose(const ReportLocalPose& message) : Message(REPORT_LOCAL_POSE)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportLocalPose::~ReportLocalPose()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the X and updates the presence vector for the message.
///
///   \param[in] value Desired X in meters [-100000,1000000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::SetX(const double value)
{
    if(value >= Limits::MinPoint && value <= Limits::MaxPoint)
    {
        mX = value;
        mPresenceVector |= PresenceVector::X;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Y and updates the presence vector for the message.
///
///   \param[in] value Desired Y in meters [-100000,1000000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::SetY(const double value)
{
    if(value >= Limits::MinPoint && value <= Limits::MaxPoint)
    {
        mY = value;
        mPresenceVector |= PresenceVector::Y;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired Z in meters [-100000,1000000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::SetZ(const double value)
{
    if(value >= Limits::MinPoint && value <= Limits::MaxPoint)
    {
        mZ = value;
        mPresenceVector |= PresenceVector::Z;
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
bool ReportLocalPose::SetPositionRMS(const double value)
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
bool ReportLocalPose::SetRoll(const double radians)
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
bool ReportLocalPose::SetPitch(const double radians)
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
bool ReportLocalPose::SetYaw(const double radians)
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
bool ReportLocalPose::SetAttitudeRMS(const double radians)
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
bool ReportLocalPose::SetTimeStamp(const JAUS::Time& time)
{
    mTimeStamp = time;
    mPresenceVector |= PresenceVector::TimeStamp;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets X, Y, Z, Roll, Pitch, Yaw, and Time.  Overrides any existing
///          data.
///
///   \param[in] position Desired X, Y, Z stored as a Point3D.
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::SetPose(const Point3D& position,
                              const Point3D& orientation,
                              const Time& time)
{
    //Check bounds before proceeding.
    if(position.mX >= Limits::MinPoint && position.mX <= Limits::MaxPoint &&
       position.mY >= Limits::MinPoint && position.mY <= Limits::MaxPoint && 
       position.mZ >= Limits::MinPoint && position.mZ <= Limits::MaxPoint &&
       orientation.mX >= Limits::MinAngle && orientation.mX <= Limits::MaxAngle && 
       orientation.mY >= Limits::MinAngle && orientation.mY <= Limits::MaxAngle && 
       orientation.mZ >= Limits::MinAngle && orientation.mZ <= Limits::MaxAngle)
    {
        mX = position.mX;
        mY = position.mY;
        mZ = position.mZ;
        mRoll = orientation.mX;
        mPitch = orientation.mY;
        mYaw = orientation.mZ;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::X | PresenceVector::Y | PresenceVector::Z |
                           PresenceVector::Roll | PresenceVector::Pitch | PresenceVector::Yaw |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets X, Y, Z, and Time.  Overrides any existing data.
///
///   \param[in] position Desired X, Y, Z stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::SetPosition(const Point3D& position, const Time& time)
{
    //Check bounds before proceeding.
    if(position.mX >= Limits::MinPoint && position.mX <= Limits::MaxPoint &&
       position.mY >= Limits::MinPoint && position.mY <= Limits::MaxPoint && 
       position.mZ >= Limits::MinPoint && position.mZ <= Limits::MaxPoint)
    {
        mX = position.mX;
        mY = position.mY;
        mZ = position.mZ;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::X | PresenceVector::Y | PresenceVector::Z |
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
bool ReportLocalPose::SetOrientation(const Point3D& orientation, const Time& time)
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
///   \brief Adds change in X, Y, Z, Roll, Pitch, Yaw to the currently stored local
///          pose and updates the time stamp.
///
///   \param[in] position Desired delta X, Y, and Z stored as a Point3D.
///   \param[in] orientation Desired delta Roll(X), Pitch(Y), Yaw(Z) stored as a
///              Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::AddToPose(const Point3D& position,
                                const Point3D& orientation,
                                const Time& time)
{
    double newX = position.mX + mX;
    double newY = position.mY + mY;
    double newZ = position.mZ + mZ;
    double newRoll = CxUtils::Orientation::AddToAngle(orientation.mX, mRoll);
    double newPitch = CxUtils::Orientation::AddToAngle(orientation.mY, mPitch);
    double newYaw = CxUtils::Orientation::AddToAngle(orientation.mZ, mYaw);

    //Check bounds before proceeding.
    if(newX >= Limits::MinPoint && newX <= Limits::MaxPoint &&
       newY >= Limits::MinPoint && newY <= Limits::MaxPoint && 
       newZ >= Limits::MinPoint && newZ <= Limits::MaxPoint &&
       newRoll >= Limits::MinAngle && newRoll <= Limits::MaxAngle && 
       newPitch >= Limits::MinAngle && newPitch <= Limits::MaxAngle && 
       newYaw >= Limits::MinAngle && newYaw <= Limits::MaxAngle)
    {
        mX = newX;
        mY = newY;
        mZ = newZ;
        mRoll = newRoll;
        mPitch = newPitch;
        mYaw = newYaw;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::X | PresenceVector::Y | PresenceVector::Z |
                           PresenceVector::Roll | PresenceVector::Pitch | PresenceVector::Yaw |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds change in X, Y, Z to the currently stored local pose and updates
///          the time stamp.
///
///   \param[in] position Desired delta X, Y, and Z stored as a Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::AddToPosition(const Point3D& position, const Time& time)
{    
    double newX = position.mX + mX;
    double newY = position.mY + mY;
    double newZ = position.mZ + mZ;

    //Check bounds before proceeding.
    if(newX >= Limits::MinPoint && newX <= Limits::MaxPoint &&
       newY >= Limits::MinPoint && newY <= Limits::MaxPoint && 
       newZ >= Limits::MinPoint && newZ <= Limits::MaxPoint)
    {
        mX = newX;
        mY = newY;
        mZ = newZ;
        mTimeStamp = time;
        mPresenceVector |= PresenceVector::X | PresenceVector::Y | PresenceVector::Z |
                           PresenceVector::TimeStamp;
        return true;
    }
    //Parameters are out of bound.
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds change in Roll, Pitch, Yaw to the currently stored local pose
///          and updates the time stamp.
///
///   \param[in] orientation Desired delta Roll(X), Pitch(Y), Yaw(Z) stored as a
///              Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if params are accepted, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalPose::AddToOrientation(const Point3D& orientation, const Time& time)
{    
    double newRoll = CxUtils::Orientation::AddToAngle(orientation.mX, mRoll);
    double newPitch = CxUtils::Orientation::AddToAngle(orientation.mY, mPitch);
    double newYaw = CxUtils::Orientation::AddToAngle(orientation.mZ, mYaw);

    //Check bounds before proceeding.
    if(newRoll >= Limits::MinAngle && newRoll <= Limits::MaxAngle && 
       newPitch >= Limits::MinAngle && newPitch <= Limits::MaxAngle && 
       newYaw >= Limits::MinAngle && newYaw <= Limits::MaxAngle)
    {
        mRoll = newRoll;
        mPitch = newPitch;
        mYaw = newYaw;
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
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportLocalPose::WriteMessageBody(Packet& packet) const
{
    int expected = USHORT_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    if((mPresenceVector & PresenceVector::X) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mX, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Y) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mY, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Z) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mZ, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
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
int ReportLocalPose::ReadMessageBody(const Packet& packet)
{
    int expected = USHORT_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    if((mPresenceVector & PresenceVector::X) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mX, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Y) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mY, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Z) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mZ, Limits::MaxPoint, Limits::MinPoint, ScaledInteger::UInt);
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
void ReportLocalPose::ClearMessageBody()
{
    mPresenceVector = 0;
    mX = 0;
    mY = 0;
    mZ = 0;
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
int ReportLocalPose::RunTestCase() const
{
    int result = 0;

    Packet packet;

    ReportLocalPose msg1, msg2;

    msg1.SetAttitudeRMS(.54);
    msg1.SetRoll(.02);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::AttitudeRMS| PresenceVector::Roll) )
         {
                result = 1;
         }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportLocalPose::PrintMessageBody() const
{
    std::cout << "XYZ: [" << std::fixed << std::setprecision(2) << mX << ", " 
                          << std::fixed << std::setprecision(2) << mY << ", " 
                          << std::fixed << std::setprecision(2) << mZ << "]\n";
    std::cout << "Orientation: [" << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mRoll) << ", " 
                                  << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mPitch) << ", " 
                                  << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mYaw) << "]\n";
    std::cout << "Timestamp: " << mTimeStamp.ToString() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportLocalPose& ReportLocalPose::operator=(const ReportLocalPose& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mX = message.mX;
        mY = message.mY;
        mZ = message.mZ;
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
