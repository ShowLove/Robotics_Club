////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportaccelerationstate.cpp
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
#include "jaus/mobility/sensors/reportaccelerationstate.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>
#include <iomanip>

const double JAUS::ReportAccelerationState::Limits::MinLinearAcceleration = -1310.68;
const double JAUS::ReportAccelerationState::Limits::MaxLinearAcceleration = 1310.68;
const double JAUS::ReportAccelerationState::Limits::MinAccelerationRMS = 0.0;
const double JAUS::ReportAccelerationState::Limits::MaxAccelerationRMS = 100.0;
const double JAUS::ReportAccelerationState::Limits::MinRotationalAcceleration = -13106.8;
const double JAUS::ReportAccelerationState::Limits::MaxRotationalAcceleration = 13106.8;
const double JAUS::ReportAccelerationState::Limits::MinRotationalAccelerationRMS = 0.0;
const double JAUS::ReportAccelerationState::Limits::MaxRotationalAccelerationRMS = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationState::ReportAccelerationState(const Address& dest, const Address& src) : Message(REPORT_ACCELERATION_STATE, dest, src)
{
    mPresenceVector = 0;
    mAccelerationX = 0;
    mAccelerationY = 0;
    mAccelerationZ = 0;
    mAccelerationRMS = 0;
    mRollAcceleration = 0;
    mPitchAcceleration = 0;
    mYawAcceleration = 0;
    mRotationalAccelerationRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationState::ReportAccelerationState(const ReportAccelerationState& message) : Message(REPORT_ACCELERATION_STATE)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationState::~ReportAccelerationState()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration X and updates the presence vector for the message.
///
///   \param[in] value Desired acceleration X in meters per second squared
///                    [-1310.68,1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetAccelerationX(const double value)
{
    if(value >= Limits::MinLinearAcceleration && value <= Limits::MaxLinearAcceleration)
    {
        mAccelerationX = value;
        mPresenceVector |= PresenceVector::AccelerationX;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration Y and updates the presence vector for the message.
///
///   \param[in] value Desired acceleration Y in meters per second squared
///                    [-1310.68,1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetAccelerationY(const double value)
{
    if(value >= Limits::MinLinearAcceleration && value <= Limits::MaxLinearAcceleration)
    {
        mAccelerationY = value;
        mPresenceVector |= PresenceVector::AccelerationY;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration Z and updates the presence vector for the
///          message.
///
///   \param[in] value Desired acceleration Z in meters per second squared
///                    [-1310.68,1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetAccelerationZ(const double value)
{
    if(value >= Limits::MinLinearAcceleration && value <= Limits::MaxLinearAcceleration)
    {
        mAccelerationZ = value;
        mPresenceVector |= PresenceVector::AccelerationZ;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration RMS and updates the presence vector for the
///          message.
///
///   \param[in] value Desired acceleration RMS in meters per second squared
///                    [0, 100].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetAccelerationRMS(const double value)
{
    if(value >= Limits::MinAccelerationRMS && value <= Limits::MaxAccelerationRMS)
    {
        mAccelerationRMS = value;
        mPresenceVector |= PresenceVector::AccelerationRMS;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the roll accelerationand updates the presence vector for the
///          message.
///
///   \param[in] value Desired roll acceleration in radians per second squared
///                    [-32.768,32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetRollAcceleration(const double value)
{
    if(value >= Limits::MinRotationalAcceleration && value <= Limits::MaxRotationalAcceleration)
    {
        mRollAcceleration = value;
        mPresenceVector |= PresenceVector::RollAcceleration;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pitch acceleration and updates the presence vector for the
///          message.
///
///   \param[in] value Desired pitch acceleration in radians per second squared
///                    [-32.768,32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetPitchAcceleration(const double value)
{
    if(value >= Limits::MinRotationalAcceleration && value <= Limits::MaxRotationalAcceleration)
    {
        mPitchAcceleration = value;
        mPresenceVector |= PresenceVector::PitchAcceleration;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the yaw acceleration and updates the presence vector for the
///          message.
///
///   \param[in] value Desired yaw acceleration in radians per second squared
///              [-13106.8,13106.8].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetYawAcceleration(const double value)
{
    if(value >= Limits::MinRotationalAcceleration && value <= Limits::MaxRotationalAcceleration)
    {
        mYawAcceleration = value;
        mPresenceVector |= PresenceVector::YawAcceleration;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the rotational acceleration RMS value and updates the presence
///          vector for the message.
///
///   \param[in] value Desired rotational acceleration RMS in radians per second
///                    squared [0, PI]
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationState::SetRotationalAccelerationRMS(const double value)
{
    if(value >= Limits::MinRotationalAccelerationRMS && value <= Limits::MaxRotationalAccelerationRMS)
    {
        mRotationalAccelerationRMS = value;
        mPresenceVector |= PresenceVector::RotationalAccelerationRMS;
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
bool ReportAccelerationState::SetTimeStamp(const JAUS::Time& time)
{
    mTimeStamp = time;
    mPresenceVector |= PresenceVector::TimeStamp;
    return true;
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
int ReportAccelerationState::WriteMessageBody(Packet& packet) const
{
    int expected = USHORT_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    if((mPresenceVector & PresenceVector::AccelerationX) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mAccelerationX, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationY) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mAccelerationY, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationZ) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mAccelerationZ, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationRMS) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mAccelerationRMS, Limits::MaxAccelerationRMS, Limits::MinAccelerationRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RollAcceleration) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mRollAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::PitchAcceleration) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mPitchAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::YawAcceleration) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mYawAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RotationalAccelerationRMS) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mRotationalAccelerationRMS, Limits::MaxRotationalAccelerationRMS, Limits::MinRotationalAccelerationRMS, ScaledInteger::UShort);
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
int ReportAccelerationState::ReadMessageBody(const Packet& packet)
{
    int expected = USHORT_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    if((mPresenceVector & PresenceVector::AccelerationX) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mAccelerationX, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationY) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mAccelerationY, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationZ) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mAccelerationZ, Limits::MaxLinearAcceleration, Limits::MinLinearAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::AccelerationRMS) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mAccelerationRMS, Limits::MaxAccelerationRMS, Limits::MinAccelerationRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RollAcceleration) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mRollAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::PitchAcceleration) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mPitchAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::YawAcceleration) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mYawAcceleration, Limits::MaxRotationalAcceleration, Limits::MinRotationalAcceleration, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RotationalAccelerationRMS) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mRotationalAccelerationRMS, Limits::MaxRotationalAccelerationRMS, Limits::MinRotationalAccelerationRMS, ScaledInteger::UShort);
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
void ReportAccelerationState::ClearMessageBody()
{
    mPresenceVector = 0;
    mAccelerationX = 0;
    mAccelerationY = 0;
    mAccelerationZ = 0;
    mAccelerationRMS = 0;
    mRollAcceleration = 0;
    mPitchAcceleration = 0;
    mYawAcceleration = 0;
    mRotationalAccelerationRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportAccelerationState::RunTestCase() const
{
    int result = 0;

    Packet packet;

    ReportAccelerationState msg1, msg2;

    msg1.SetAccelerationX(1023);
    msg1.SetAccelerationRMS(.155);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::AccelerationX | PresenceVector::AccelerationRMS) )
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
void ReportAccelerationState::PrintMessageBody() const
{
    std::cout << "Acceleration (XYZ): [" << std::fixed << std::setprecision(2) << mAccelerationX << ", " 
                                         << std::fixed << std::setprecision(2) << mAccelerationY << ", " 
                                         << std::fixed << std::setprecision(2) << mAccelerationZ << "]\n";
    std::cout << "Acceleration (RPY): [" << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mRollAcceleration) << ", " 
                                         << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mPitchAcceleration) << ", " 
                                         << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mYawAcceleration) << "]\n";
    std::cout << "Timestamp: " << mTimeStamp.ToString() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationState& ReportAccelerationState::operator=(const ReportAccelerationState& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mAccelerationX = message.mAccelerationX;
        mAccelerationY = message.mAccelerationY;
        mAccelerationZ = message.mAccelerationZ;
        mAccelerationRMS = message.mAccelerationRMS;
        mRollAcceleration = message.mRollAcceleration;
        mPitchAcceleration = message.mPitchAcceleration;
        mYawAcceleration = message.mYawAcceleration;
        mRotationalAccelerationRMS = message.mRotationalAccelerationRMS;
        mTimeStamp = message.mTimeStamp;
        
    }
    return *this;
}


/*  End of File */
