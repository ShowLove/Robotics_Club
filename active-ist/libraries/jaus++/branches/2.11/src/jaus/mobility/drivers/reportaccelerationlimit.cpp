////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportaccelerationlimit.cpp
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
#include "jaus/mobility/drivers/reportaccelerationlimit.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::ReportAccelerationLimit::Limits::MinLinearAcceleration = -1310.68;
const double JAUS::ReportAccelerationLimit::Limits::MaxLinearAcceleration = 1310.68;
const double JAUS::ReportAccelerationLimit::Limits::MinRotationalAcceleration = -13106.8;
const double JAUS::ReportAccelerationLimit::Limits::MaxRotationalAcceleration = 13106.8;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationLimit::ReportAccelerationLimit(const Address& dest, const Address& src) : Message(REPORT_ACCELERATION_LIMIT, dest, src)
{
    mPresenceVector = 0;
    mCommandType = SetMaximumAllowedValues;
    mAccelerationX = 0;
    mAccelerationY = 0;
    mAccelerationZ = 0;
    mRollAcceleration = 0;
    mPitchAcceleration = 0;
    mYawAcceleration = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationLimit::ReportAccelerationLimit(const ReportAccelerationLimit& message) : Message(REPORT_ACCELERATION_LIMIT)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationLimit::~ReportAccelerationLimit()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Acceleration x and update the presence vector for the message
///
///   \param[in] value Desired Acceleration x in meters per second squared
///                    [-1310.68, 1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetAccelerationX(const double value)
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
///   \brief Sets the Acceleration y and update the presence vector for the message
///
///   \param[in] value Desired Acceleration y in meters per second squared
///                    [-1310.68, 1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetAccelerationY(const double value)
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
///   \brief Sets the Acceleration z and update the presence vector for the message
///
///   \param[in] value Desired Acceleration z in meters per second squared
///                    [-1310.68, 1310.68].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetAccelerationZ(const double value)
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
///   \brief Sets the roll Acceleration and update the presence vector for the message
///
///   \param[in] value Desired roll Acceleration in Radians per second squared
///                    [-13106.8, 13106.8].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetRollAcceleration(const double value)
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
///   \brief Sets the pitch Acceleration and update the presence vector for the
///          message
///
///   \param[in] value Desired pitch Acceleration in Radians per second squared 
///                    [-13106.8, 13106.8].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetPitchAcceleration(const double value)
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
///   \brief Sets the yaw Acceleration and update the presence vector for the message
///
///   \param[in] value Desired yaw Acceleration in Radians per second squared
///                    [-13106.8, 13106.8].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportAccelerationLimit::SetYawAcceleration(const double value)
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
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportAccelerationLimit::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    expected += BYTE_SIZE;
    written += packet.Write((Byte)mCommandType);

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
int ReportAccelerationLimit::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    expected += BYTE_SIZE;
    read += packet.Read((Byte &)mCommandType);

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

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportAccelerationLimit::ClearMessageBody()
{
    mPresenceVector = 0;
    mCommandType = SetMaximumAllowedValues;
    mAccelerationX = 0;
    mAccelerationY = 0;
    mAccelerationZ = 0;
    mRollAcceleration = 0;
    mPitchAcceleration = 0;
    mYawAcceleration = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportAccelerationLimit::RunTestCase() const
{
    int result = 0;

    Packet packet;

    ReportAccelerationLimit msg1, msg2;

    msg1.SetAccelerationX(60);
    msg1.SetAccelerationY(30);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
         if(msg2.AreFieldsPresent(PresenceVector::AccelerationX | PresenceVector::AccelerationY) )
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
ReportAccelerationLimit& ReportAccelerationLimit::operator=(const ReportAccelerationLimit& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mCommandType = message.mCommandType;
        mAccelerationX = message.mAccelerationX;
        mAccelerationY = message.mAccelerationY;
        mAccelerationZ = message.mAccelerationZ;
        mRollAcceleration = message.mRollAcceleration;
        mPitchAcceleration = message.mPitchAcceleration;
        mYawAcceleration = message.mYawAcceleration;
        
    }
    return *this;
}


/*  End of File */
