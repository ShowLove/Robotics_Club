////////////////////////////////////////////////////////////////////////////////////
///
///  \file setlocalvector.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 25 November 2009
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
#include "jaus/mobility/drivers/setlocalvector.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::SetLocalVector::Limits::MinSpeed = 0.0;
const double JAUS::SetLocalVector::Limits::MaxSpeed = 327.67;
const double JAUS::SetLocalVector::Limits::MinElevation = -10000;
const double JAUS::SetLocalVector::Limits::MaxElevation = 35000;
const double JAUS::SetLocalVector::Limits::MinAngle = -CxUtils::CX_PI;
const double JAUS::SetLocalVector::Limits::MaxAngle = CxUtils::CX_PI;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalVector::SetLocalVector(const Address& dest, const Address& src) : Message(SET_LOCAL_VECTOR, dest, src)
{
    mPresenceVector = 0;
    mSpeed = 0;
    mZ = 0;
    mHeading = 0;
    mRoll = 0;
    mPitch = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalVector::SetLocalVector(const SetLocalVector& message) : Message(SET_LOCAL_VECTOR)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalVector::~SetLocalVector()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the speed and updates the presence vector for the message.
///
///   \param[in] value Desired speed in meters per second [0, 327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalVector::SetSpeed(const double value)
{
    if(value >= Limits::MinSpeed && value <= Limits::MaxSpeed)
    {
        mSpeed = value;
        mPresenceVector |= PresenceVector::Speed;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired Z in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalVector::SetZ(const double value)
{
    if(value >= Limits::MinElevation && value <= Limits::MaxElevation)
    {
        mZ = value;
        mPresenceVector |= PresenceVector::Z;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the heading and updates the presence vector for the message.
///
///   \param[in] radians Desired heading in radians[-PI, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalVector::SetHeading(const double radians)
{
    if(radians >= Limits::MinAngle && radians <= Limits::MaxAngle)
    {
        mHeading = radians;
        mPresenceVector |= PresenceVector::Heading;
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
bool SetLocalVector::SetRoll(const double radians)
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
bool SetLocalVector::SetPitch(const double radians)
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
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetLocalVector::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    if((mPresenceVector & PresenceVector::Speed) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Z) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mZ, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Heading) > 0)
    {
        expected += USHORT_SIZE;
        written += ScaledInteger::Write(packet, mHeading, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
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
int SetLocalVector::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    if((mPresenceVector & PresenceVector::Speed) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mSpeed, Limits::MaxSpeed, Limits::MinSpeed, ScaledInteger::UShort);
    }
    if((mPresenceVector & PresenceVector::Z) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mZ, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }
    if((mPresenceVector & PresenceVector::Heading) > 0)
    {
        expected += USHORT_SIZE;
        read += ScaledInteger::Read(packet, mHeading, Limits::MaxAngle, Limits::MinAngle, ScaledInteger::UShort);
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

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void SetLocalVector::ClearMessageBody()
{
    mPresenceVector = 0;
    mSpeed = 0;
    mZ = 0;
    mHeading = 0;
    mRoll = 0;
    mPitch = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SetLocalVector::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetLocalVector msg1, msg2;

    msg1.SetSpeed(60);
    msg1.SetHeading(-1.333);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::Heading | PresenceVector::Speed) )
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
SetLocalVector& SetLocalVector::operator=(const SetLocalVector& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mSpeed = message.mSpeed;
        mZ = message.mZ;
        mHeading = message.mHeading;
        mRoll = message.mRoll;
        mPitch = message.mPitch;
        
    }
    return *this;
}


/*  End of File */
