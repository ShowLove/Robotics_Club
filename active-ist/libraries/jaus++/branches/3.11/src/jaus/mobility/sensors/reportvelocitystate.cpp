////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportvelocitystate.cpp
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
#include "jaus/mobility/sensors/reportvelocitystate.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <iomanip>

const double JAUS::ReportVelocityState::Limits::MinVelocity = -327.68;
const double JAUS::ReportVelocityState::Limits::MaxVelocity = 327.67;
const double JAUS::ReportVelocityState::Limits::MinVelocityRMS = 0.0;
const double JAUS::ReportVelocityState::Limits::MaxVelocityRMS = 100.0;
const double JAUS::ReportVelocityState::Limits::MinRotationalRate = -32.768;
const double JAUS::ReportVelocityState::Limits::MaxRotationalRate = 32.767;
const double JAUS::ReportVelocityState::Limits::MinRateRMS = 0.0;
const double JAUS::ReportVelocityState::Limits::MaxRateRMS = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportVelocityState::ReportVelocityState(const Address& dest, const Address& src) : Message(REPORT_VELOCITY_STATE, dest, src)
{
    mPresenceVector = 0;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mVelocityRMS = 0;
    mRollRate = 0;
    mPitchRate = 0;
    mYawRate = 0;
    mRateRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportVelocityState::ReportVelocityState(const ReportVelocityState& message) : Message(REPORT_VELOCITY_STATE)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportVelocityState::~ReportVelocityState()
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
bool ReportVelocityState::SetVelocityX(const double value)
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
bool ReportVelocityState::SetVelocityY(const double value)
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
bool ReportVelocityState::SetVelocityZ(const double value)
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
///   \brief Sets the velocity RMS and updates the presence vector for the
///          message.
///
///   \param[in] value Desired velocity RMS in meters per second [0, 100].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportVelocityState::SetVelocityRMS(const double value)
{
    if(value >= Limits::MinVelocityRMS && value <= Limits::MaxVelocityRMS)
    {
        mVelocityRMS = value;
        mPresenceVector |= PresenceVector::VelocityRMS;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the roll rate and updates the presence vector for the message.
///
///   \param[in] value Desired roll rate in radians per second [-32.768,32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportVelocityState::SetRollRate(const double value)
{
    if(value >= Limits::MinRotationalRate && value <= Limits::MaxRotationalRate)
    {
        mRollRate = value;
        mPresenceVector |= PresenceVector::RollRate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pitch rate and updates the presence vector for the message.
///
///   \param[in] value Desired pitch rate in radians per second [-32.768,32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportVelocityState::SetPitchRate(const double value)
{
    if(value >= Limits::MinRotationalRate && value <= Limits::MaxRotationalRate)
    {
        mPitchRate = value;
        mPresenceVector |= PresenceVector::PitchRate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the yaw rate and updates the presence vector for the message.
///
///   \param[in] value Desired pitch rate in radians per second [-32.768,32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportVelocityState::SetYawRate(const double value)
{
    if(value >= Limits::MinRotationalRate && value <= Limits::MaxRotationalRate)
    {
        mYawRate = value;
        mPresenceVector |= PresenceVector::YawRate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the rate RMS value and updates the presence vector for
///          the message.
///
///   \param[in] value Desired rate RMS in radians per second [0, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportVelocityState::SetRateRMS(const double value)
{
    if(value >= Limits::MinRateRMS && value <= Limits::MaxRateRMS)
    {
        mRateRMS = value;
        mPresenceVector |= PresenceVector::RateRMS;
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
bool ReportVelocityState::SetTimeStamp(const JAUS::Time& time)
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
int ReportVelocityState::WriteMessageBody(Packet& packet) const
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
    if((mPresenceVector & PresenceVector::VelocityRMS) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mVelocityRMS, Limits::MaxVelocityRMS, Limits::MinVelocityRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RollRate) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mRollRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::PitchRate) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mPitchRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::YawRate) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mYawRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::RateRMS) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mRateRMS, Limits::MaxRateRMS, Limits::MinRateRMS, ScaledInteger::UShort);
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
int ReportVelocityState::ReadMessageBody(const Packet& packet)
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
    if((mPresenceVector & PresenceVector::VelocityRMS) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mVelocityRMS, Limits::MaxVelocityRMS, Limits::MinVelocityRMS, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::RollRate) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mRollRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::PitchRate) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mPitchRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::YawRate) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mYawRate, Limits::MaxRotationalRate, Limits::MinRotationalRate, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::RateRMS) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mRateRMS, Limits::MaxRateRMS, Limits::MinRateRMS, ScaledInteger::UShort);
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
void ReportVelocityState::ClearMessageBody()
{
    mPresenceVector = 0;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mVelocityRMS = 0;
    mRollRate = 0;
    mPitchRate = 0;
    mYawRate = 0;
    mRateRMS = 0;
    mTimeStamp.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportVelocityState::RunTestCase() const
{
    int result = 0;

    Packet packet;

    ReportVelocityState msg1, msg2;

    msg1.SetVelocityZ(123);
    msg1.SetRateRMS(.155);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::VelocityZ | PresenceVector::RateRMS) )
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
void ReportVelocityState::PrintMessageBody() const
{
    std::cout << "Velocity: [" << std::fixed << std::setprecision(2) << mVelocityX << ", " 
                               << std::fixed << std::setprecision(2) << mVelocityY << ", " 
                               << std::fixed << std::setprecision(2) << mVelocityZ << "]\n";
    std::cout << "Rotation: [" << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mRollRate) << ", " 
                               << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mPitchRate) << ", " 
                               << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mYawRate) << "]\n";
    std::cout << "Timestamp: " << mTimeStamp.ToString() << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportVelocityState& ReportVelocityState::operator=(const ReportVelocityState& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mVelocityX = message.mVelocityX;
        mVelocityY = message.mVelocityY;
        mVelocityZ = message.mVelocityZ;
        mVelocityRMS = message.mVelocityRMS;
        mRollRate = message.mRollRate;
        mPitchRate = message.mPitchRate;
        mYawRate = message.mYawRate;
        mRateRMS = message.mRateRMS;
        mTimeStamp = message.mTimeStamp;
        
    }
    return *this;
}


/*  End of File */
