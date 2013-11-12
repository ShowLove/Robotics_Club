////////////////////////////////////////////////////////////////////////////////////
///
///  \file setvelocitycommand.cpp
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
#include "jaus/mobility/drivers/setvelocitycommand.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::SetVelocityCommand::Limits::MinVelocity = -327.68;
const double JAUS::SetVelocityCommand::Limits::MaxVelocity = 327.67;
const double JAUS::SetVelocityCommand::Limits::MinRotationalRate = -32.768;
const double JAUS::SetVelocityCommand::Limits::MaxRotationalRate = 32.767;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand::SetVelocityCommand(const Address& dest, const Address& src) : Message(SET_VELOCITY_COMMAND, dest, src)
{
    mPresenceVector = 0;
    mCommandType = SetCurrentCommand;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mRollRate = 0;
    mPitchRate = 0;
    mYawRate = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand::SetVelocityCommand(const SetVelocityCommand& message) : Message(SET_VELOCITY_COMMAND)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand::~SetVelocityCommand()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the velocity x and update the presence vector for the message
///
///   \param[in] value Desired velocity x in meters per second [-327.68, 327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetVelocityX(const double value)
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
///   \brief Sets the velocity y and update the presence vector for the message
///
///   \param[in] value Desired velocity y in meters per second [-327.68, 327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetVelocityY(const double value)
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
///   \brief Sets the velocity z and update the presence vector for the message
///
///   \param[in] value Desired velocity z in meters per second [-327.68, 327.67].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetVelocityZ(const double value)
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
///   \brief Sets the roll rate and update the presence vector for the message
///
///   \param[in] value Desired roll rate in Radians per second [-32.768, 32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetRollRate(const double value)
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
///   \brief Sets the pitch rate and update the presence vector for the message
///
///   \param[in] value Desired pitch rate in Radians per second [-32.768, 32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetPitchRate(const double value)
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
///   \brief Sets the yaw rate and update the presence vector for the message
///
///   \param[in] value Desired yaw rate in Radians per second [-32.768, 32.767].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetVelocityCommand::SetYawRate(const double value)
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
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetVelocityCommand::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    expected += BYTE_SIZE;
    written += packet.Write((Byte)mCommandType);

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
int SetVelocityCommand::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    expected += BYTE_SIZE;
    read += packet.Read((Byte &)mCommandType);

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

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void SetVelocityCommand::ClearMessageBody()
{
    mPresenceVector = 0;
    mCommandType = SetCurrentCommand;
    mVelocityX = 0;
    mVelocityY = 0;
    mVelocityZ = 0;
    mRollRate = 0;
    mPitchRate = 0;
    mYawRate = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SetVelocityCommand::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetVelocityCommand msg1, msg2;

    msg1.SetCommandType(SetMaximumAllowedValues);
    msg1.SetVelocityY(56);
    msg1.SetYawRate(-15.25);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::VelocityY | PresenceVector::YawRate) &&
            msg1.GetCommandType() == msg2.GetCommandType())
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
void SetVelocityCommand::PrintMessageBody() const
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
    if((mPresenceVector & PresenceVector::RollRate) > 0)
    {
        std::cout << "Roll Rate: " << CxUtils::CxToDegrees(mRollRate) << std::endl;
    }
    if((mPresenceVector & PresenceVector::PitchRate) > 0)
    {
        std::cout << "Pitch Rate: " << CxUtils::CxToDegrees(mPitchRate) << std::endl;
    }
    if((mPresenceVector & PresenceVector::YawRate) > 0)
    {
        std::cout << "Yaw Rate: " << CxUtils::CxToDegrees(mYawRate) << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand& SetVelocityCommand::operator=(const SetVelocityCommand& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mCommandType = message.mCommandType;
        mVelocityX = message.mVelocityX;
        mVelocityY = message.mVelocityY;
        mVelocityZ = message.mVelocityZ;
        mRollRate = message.mRollRate;
        mPitchRate = message.mPitchRate;
        mYawRate = message.mYawRate;
        
    }
    return *this;
}


/*  End of File */
