////////////////////////////////////////////////////////////////////////////////////
///
///  \file setlocalpathsegment.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 December 2009
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
#include "jaus/mobility/drivers/setlocalpathsegment.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::SetLocalPathSegment::Limits::MinX = -100000.0;
const double JAUS::SetLocalPathSegment::Limits::MaxX = 100000.0;
const double JAUS::SetLocalPathSegment::Limits::MinY = -100000.0;
const double JAUS::SetLocalPathSegment::Limits::MaxY = 100000.0;
const double JAUS::SetLocalPathSegment::Limits::MinZ = -10000;
const double JAUS::SetLocalPathSegment::Limits::MaxZ = 35000;
const double JAUS::SetLocalPathSegment::Limits::MinWeightingFactor = 0.0;
const double JAUS::SetLocalPathSegment::Limits::MaxWeightingFactor = 500.0;
const double JAUS::SetLocalPathSegment::Limits::MinPathTolerance = 0.0;
const double JAUS::SetLocalPathSegment::Limits::MaxPathTolerance = 100000.0;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalPathSegment::SetLocalPathSegment(const Address& dest, const Address& src) : Message(SET_LOCAL_PATH_SEGMENT, dest, src)
{
    mPresenceVector = 0;
    mP1X = 0;
    mP1Y = 0;
    mP1Z = 0;
    mP2X = 0;
    mP2Y = 0;
    mP2Z = 0;
    mWeightingFactor = 0;
    mPathTolerance = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalPathSegment::SetLocalPathSegment(const SetLocalPathSegment& message) : Message(SET_LOCAL_PATH_SEGMENT)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalPathSegment::~SetLocalPathSegment()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the latitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired P_1 latitude in degrees [-90, 90].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP1X(const double degrees)
{
    if(degrees >= Limits::MinX && degrees <= Limits::MaxX)
    {
        mP1X = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the longitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired P_1 longitude in degrees [-180, 180].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP1Y(const double degrees)
{
    if(degrees >= Limits::MinY && degrees <= Limits::MaxY)
    {
        mP1Y = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired P_1 Z in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP1Z(const double value)
{
    if(value >= Limits::MinZ && value <= Limits::MaxZ)
    {
        mP1Z = value;
        mPresenceVector |= PresenceVector::P1Z;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the latitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired P_2 latitude in degrees [-90, 90].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP2X(const double degrees)
{
    if(degrees >= Limits::MinX && degrees <= Limits::MaxX)
    {
        mP2X = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the longitude and updates the presence vector for the message.
///
///   \param[in] degrees Desired P_2 longitude in degrees [-180, 180].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP2Y(const double degrees)
{
    if(degrees >= Limits::MinY && degrees <= Limits::MaxY)
    {
        mP2Y = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired P_2 Z in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetP2Z(const double value)
{
    if(value >= Limits::MinZ && value <= Limits::MaxZ)
    {
        mP2Z = value;
        mPresenceVector |= PresenceVector::P2Z;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the weighting factor value and updates the presence vector for
///          the message.
///
///   \param[in] value Desired weighting factor [0, 500].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetWeightingFactor(const double value)
{
    if(value >= Limits::MinWeightingFactor && value <= Limits::MaxWeightingFactor)
    {
        mWeightingFactor= value;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the path tolerance value and updates the presence vector for
///          the message.
///
///   \param[in] value Desired path tolerance in meters[0, 100000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetLocalPathSegment::SetPathTolerance(const double value)
{
    if(value >= Limits::MinPathTolerance && value <= Limits::MaxPathTolerance)
    {
        mPathTolerance= value;
        mPresenceVector |= PresenceVector::PathTolerance;
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
int SetLocalPathSegment::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP1X, Limits::MaxX, Limits::MinX, ScaledInteger::UInt);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP1Y, Limits::MaxY, Limits::MinY, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P1Z) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mP1Z, Limits::MaxZ, Limits::MinZ, ScaledInteger::UInt);
    }

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP2X, Limits::MaxX, Limits::MinX, ScaledInteger::UInt);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP2Y, Limits::MaxY, Limits::MinY, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P2Z) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mP2Z, Limits::MaxZ, Limits::MinZ, ScaledInteger::UInt);
    }

    expected += USHORT_SIZE;
    written += ScaledInteger::Write(packet, mWeightingFactor, Limits::MaxWeightingFactor, Limits::MinWeightingFactor, ScaledInteger::UShort);

    if((mPresenceVector & PresenceVector::PathTolerance) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mPathTolerance, Limits::MaxPathTolerance, Limits::MinPathTolerance, ScaledInteger::UInt);
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
int SetLocalPathSegment::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP1X, Limits::MaxX, Limits::MinX, ScaledInteger::UInt);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP1Y, Limits::MaxY, Limits::MinY, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P1Z) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mP1Z, Limits::MaxZ, Limits::MinZ, ScaledInteger::UInt);
    }

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP2X, Limits::MaxX, Limits::MinX, ScaledInteger::UInt);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP2Y, Limits::MaxY, Limits::MinY, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P2Z) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mP2Z, Limits::MaxZ, Limits::MinZ, ScaledInteger::UInt);
    }

    expected += USHORT_SIZE;
    read += ScaledInteger::Read(packet, mWeightingFactor, Limits::MaxWeightingFactor, Limits::MinWeightingFactor, ScaledInteger::UShort);

    if((mPresenceVector & PresenceVector::PathTolerance) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mPathTolerance, Limits::MaxPathTolerance, Limits::MinPathTolerance, ScaledInteger::UInt);
    }

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void SetLocalPathSegment::ClearMessageBody()
{
   mPresenceVector = 0;
    mP1X = 0;
    mP1Y = 0;
    mP1Z = 0;
    mP2X = 0;
    mP2Y = 0;
    mP2Z = 0;
    mWeightingFactor = 0;
    mPathTolerance = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SetLocalPathSegment::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetLocalPathSegment msg1, msg2;

    msg1.SetP1Y(-60);
    msg1.SetPathTolerance(30);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::PathTolerance ))
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
SetLocalPathSegment& SetLocalPathSegment::operator=(const SetLocalPathSegment& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mP1X = message.mP1X;
        mP1Y = message.mP1Y;
        mP1Z = message.mP1Z;
        mP2X = message.mP2X;
        mP2Y = message.mP2Y;
        mP2Z = message.mP2Z;
        mWeightingFactor = message.mWeightingFactor;
        mPathTolerance = message.mPathTolerance;
        
    }
    return *this;
}


/*  End of File */
