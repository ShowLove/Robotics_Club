////////////////////////////////////////////////////////////////////////////////////
///
///  \file SetVelocityVector.cpp
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
///  <br>Email: brian_valenitno@knights.ucf.edu
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
#include "jaus/extras/mobility/setvelocityvector.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <iomanip>

const double JAUS::SetVelocityVector::Limits::MinVelocity = -327.68;
const double JAUS::SetVelocityVector::Limits::MaxVelocity = 327.67;
const double JAUS::SetVelocityVector::Limits::MinAngle = -CxUtils::CX_PI;
const double JAUS::SetVelocityVector::Limits::MaxAngle = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityVector::SetVelocityVector(const Address& dest, const Address& src) : Message(SET_VELOCITY_VECTOR, dest, src)
{
    mPresenceVector = 0;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mPositionX = 0;
    mPositionY = 0;
    mPositionZ = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityVector::SetVelocityVector(const SetVelocityVector& message) : Message(REPORT_VELOCITY_VECTOR)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityVector::~SetVelocityVector()
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the velocity X and updates the presence vector for the message.
///
///   \param[in] value Desired velocity X in meters per second [-327.68,327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetVelocityX(const double value)
{
    if(value >= Limits::MinVelocity && value <= Limits::MaxVelocity)
    {
        mVelocityX = value;
        mPresenceVector |= PresenceVector::VelocityX;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the velocity Y and updates the presence vector for the message.
///
///   \param[in] value Desired velocity Y in meters per second [-327.68,327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetVelocityY(const double value)
{
    if(value >= Limits::MinVelocity && value <= Limits::MaxVelocity)
    {
        mVelocityY = value;
        mPresenceVector |= PresenceVector::VelocityY;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the velocity Z and updates the presence vector for the
///          message.
///
///   \param[in] value Desired velocity Z in meters per second [-327.68,327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetVelocityZ(const double value)
{
    if(value >= Limits::MinVelocity && value <= Limits::MaxVelocity)
    {
        mVelocityZ = value;
        mPresenceVector |= PresenceVector::VelocityZ;
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
bool SetVelocityVector::SetRoll(const double radians)
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
bool SetVelocityVector::SetPitch(const double radians)
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
bool SetVelocityVector::SetYaw(const double radians)
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
///   \brief Sets the Time Stamp value and updates the presence vector for
///          the message.
///
///   \param[in] time Desired Time Stamp as a JAUS::Time instance.
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetTimeStamp(const JAUS::Time& time)
{
    mTimeStamp = time;
    mPresenceVector |= PresenceVector::TimeStamp;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position X and updates the presence vector for the message.
///
///   \param[in] value Desired position X in meters RHR
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetPositionX(const double value)
{
    mPositionX = value;
    mPresenceVector |= PresenceVector::PositionX;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position Y and updates the presence vector for the message.
///
///   \param[in] value Desired position Y in meters RHR
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetPositionY(const double value)
{
    mPositionY = value;
    mPresenceVector |= PresenceVector::PositionY;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position Z and updates the presence vector for the message.
///
///   \param[in] value Desired position Z in meters RHR
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityVector::SetPositionZ(const double value)
{
    mPositionZ = value;
    mPresenceVector |= PresenceVector::PositionZ;
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
int SetVelocityVector::WriteMessageBody(Packet& packet) const
{
    int expected = USHORT_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    if((mPresenceVector & PresenceVector::VelocityX) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mVelocityX, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::VelocityY) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mVelocityY, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::VelocityZ) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mVelocityZ, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
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
    if((mPresenceVector & PresenceVector::TimeStamp) > 0)
    {
        expected += UINT_SIZE;
        written += packet.Write(mTimeStamp.ToUInt());
    }

    if((mPresenceVector & PresenceVector::PositionX) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        written += packet.Write(mPositionX);
    }
    if((mPresenceVector & PresenceVector::PositionY) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        written += packet.Write(mPositionY);
    }
    if((mPresenceVector & PresenceVector::PositionZ) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        written += packet.Write(mPositionZ);
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
int SetVelocityVector::ReadMessageBody(const Packet& packet)
{
    int expected = USHORT_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    if((mPresenceVector & PresenceVector::VelocityX) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mVelocityX, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::VelocityY) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mVelocityY, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::VelocityZ) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mVelocityZ, Limits::MaxVelocity, Limits::MinVelocity, ScaledInteger::UInt);
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
    if((mPresenceVector & PresenceVector::TimeStamp) > 0)
    {
        UInt time = 0;
        expected += UINT_SIZE;
        read += packet.Read(time);
        mTimeStamp.SetTime(time);
    }
    if((mPresenceVector & PresenceVector::PositionX) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        read += packet.Read(mPositionX);
    }
    if((mPresenceVector & PresenceVector::PositionY) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        read += packet.Read(mPositionY);
    }

    if((mPresenceVector & PresenceVector::PositionZ) > 0)
    {
        expected += LONG_FLOAT_SIZE;
        read += packet.Read(mPositionZ);
    }

    return expected == read ? read : -1;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void SetVelocityVector::ClearMessageBody()
{
    mPresenceVector = 0;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mPositionX = 0;
    mPositionY = 0;
    mPositionZ = 0;    
    mTimeStamp.SetCurrentTime();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityVector& SetVelocityVector::operator=(const SetVelocityVector& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mVelocityX = message.mVelocityX;
        mVelocityY = message.mVelocityY;
        mVelocityZ = message.mVelocityZ;
        mRoll = message.mRoll;
        mPitch = message.mPitch;
        mYaw = message.mYaw;
        mPositionX = message.mPositionX;
        mPositionY = message.mPositionY;
        mPositionZ = message.mPositionZ;
        mTimeStamp = message.mTimeStamp;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void SetVelocityVector::PrintMessageBody() const
{
    if((mPresenceVector & PresenceVector::VelocityX) > 0)
    {
        std::cout << "Velocity X: " << mVelocityX << std::endl;
    }
    if((mPresenceVector & PresenceVector::VelocityY) > 0)
    {
        std::cout << "Velocity Y: " << mVelocityY << std::endl;
    }
    if((mPresenceVector & PresenceVector::VelocityZ) > 0)
    {
        std::cout << "Velocity Z: " << mVelocityZ << std::endl;
    }
    if((mPresenceVector & PresenceVector::Roll) > 0)
    {
        std::cout << "Roll: " << CxUtils::CxToDegrees(mRoll) << std::endl;
    }
    if((mPresenceVector & PresenceVector::Pitch) > 0)
    {
        std::cout << "Pitch: " << CxUtils::CxToDegrees(mPitch) << std::endl;
    }
    if((mPresenceVector & PresenceVector::Yaw) > 0)
    {
        std::cout << "Yaw: " << CxUtils::CxToDegrees(mYaw) << std::endl;
    }
    if((mPresenceVector & PresenceVector::PositionX) > 0)
    {
        std::cout << "Position X: " << mPositionX << std::endl;
    }
    if((mPresenceVector & PresenceVector::PositionY) > 0)
    {
        std::cout << "Position Y: " << mPositionY << std::endl;
    }
    if((mPresenceVector & PresenceVector::PositionZ) > 0)
    {
        std::cout << "Position Z: " << mPositionZ << std::endl;
    }
}

/*  End of File */

