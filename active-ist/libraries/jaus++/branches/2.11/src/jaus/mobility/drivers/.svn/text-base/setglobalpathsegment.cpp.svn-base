////////////////////////////////////////////////////////////////////////////////////
///
///  \file setglobalpathsegment.cpp
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
#include "jaus/mobility/drivers/setglobalpathsegment.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

const double JAUS::SetGlobalPathSegment::Limits::MinLatitude = -90.0;
const double JAUS::SetGlobalPathSegment::Limits::MaxLatitude = 90.0;
const double JAUS::SetGlobalPathSegment::Limits::MinLongitude = -180.0;
const double JAUS::SetGlobalPathSegment::Limits::MaxLongitude = 180.0;
const double JAUS::SetGlobalPathSegment::Limits::MinElevation = -10000;
const double JAUS::SetGlobalPathSegment::Limits::MaxElevation = 35000;
const double JAUS::SetGlobalPathSegment::Limits::MinWeightingFactor = 0.0;
const double JAUS::SetGlobalPathSegment::Limits::MaxWeightingFactor = 500.0;
const double JAUS::SetGlobalPathSegment::Limits::MinPathTolerance = 0.0;
const double JAUS::SetGlobalPathSegment::Limits::MaxPathTolerance = 100000.0;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPathSegment::SetGlobalPathSegment(const Address& dest, const Address& src) : Message(SET_GLOBAL_PATH_SEGMENT, dest, src)
{
    mPresenceVector = 0;
    mP1Latitude = 0;
    mP1Longitude = 0;
    mP1Altitude = 0;
    mP2Latitude = 0;
    mP2Longitude = 0;
    mP2Altitude = 0;
    mWeightingFactor = 0;
    mPathTolerance = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPathSegment::SetGlobalPathSegment(const SetGlobalPathSegment& message) : Message(SET_GLOBAL_PATH_SEGMENT)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalPathSegment::~SetGlobalPathSegment()
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
bool SetGlobalPathSegment::SetP1Latitude(const double degrees)
{
    if(degrees >= Limits::MinLatitude && degrees <= Limits::MaxLatitude)
    {
        mP1Latitude = degrees;
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
bool SetGlobalPathSegment::SetP1Longitude(const double degrees)
{
    if(degrees >= Limits::MinLongitude && degrees <= Limits::MaxLongitude)
    {
        mP1Longitude = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired P_1 Altitude in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPathSegment::SetP1Altitude(const double value)
{
    if(value >= Limits::MinElevation && value <= Limits::MaxElevation)
    {
        mP1Altitude = value;
        mPresenceVector |= PresenceVector::P1Altitude;
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
bool SetGlobalPathSegment::SetP2Latitude(const double degrees)
{
    if(degrees >= Limits::MinLatitude && degrees <= Limits::MaxLatitude)
    {
        mP2Latitude = degrees;
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
bool SetGlobalPathSegment::SetP2Longitude(const double degrees)
{
    if(degrees >= Limits::MinLongitude && degrees <= Limits::MaxLongitude)
    {
        mP2Longitude = degrees;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the altitude and updates the presence vector for the
///          message.
///
///   \param[in] value Desired P_2 Altitude in meters [-10000, 35000].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetGlobalPathSegment::SetP2Altitude(const double value)
{
    if(value >= Limits::MinElevation && value <= Limits::MaxElevation)
    {
        mP2Altitude = value;
        mPresenceVector |= PresenceVector::P2Altitude;
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
bool SetGlobalPathSegment::SetWeightingFactor(const double value)
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
bool SetGlobalPathSegment::SetPathTolerance(const double value)
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
int SetGlobalPathSegment::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE;
    int written = 0;

    written += packet.Write(mPresenceVector);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP1Latitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP1Longitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P1Altitude) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mP1Altitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP2Latitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);

    expected += UINT_SIZE;
    written += ScaledInteger::Write(packet, mP2Longitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P2Altitude) > 0)
    {
        expected += UINT_SIZE;
        written += ScaledInteger::Write(packet, mP2Altitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
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
int SetGlobalPathSegment::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE;
    int read = 0;

    read += packet.Read(mPresenceVector);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP1Latitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP1Longitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P1Altitude) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mP1Altitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
    }

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP2Latitude, Limits::MaxLatitude, Limits::MinLatitude, ScaledInteger::UInt);

    expected += UINT_SIZE;
    read += ScaledInteger::Read(packet, mP2Longitude, Limits::MaxLongitude, Limits::MinLongitude, ScaledInteger::UInt);

    if((mPresenceVector & PresenceVector::P2Altitude) > 0)
    {
        expected += UINT_SIZE;
        read += ScaledInteger::Read(packet, mP2Altitude, Limits::MaxElevation, Limits::MinElevation, ScaledInteger::UInt);
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
void SetGlobalPathSegment::ClearMessageBody()
{
   mPresenceVector = 0;
    mP1Latitude = 0;
    mP1Longitude = 0;
    mP1Altitude = 0;
    mP2Latitude = 0;
    mP2Longitude = 0;
    mP2Altitude = 0;
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
int SetGlobalPathSegment::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetGlobalPathSegment msg1, msg2;

    msg1.SetP1Altitude(60);
    msg1.SetP2Altitude(30);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(msg2.AreFieldsPresent(PresenceVector::P1Altitude | PresenceVector::P2Altitude) )
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
SetGlobalPathSegment& SetGlobalPathSegment::operator=(const SetGlobalPathSegment& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mPresenceVector = message.mPresenceVector;
        mP1Latitude = message.mP1Latitude;
        mP1Longitude = message.mP1Longitude;
        mP1Altitude = message.mP1Altitude;
        mP2Latitude = message.mP2Latitude;
        mP2Longitude = message.mP2Longitude;
        mP2Altitude = message.mP2Altitude;
        mWeightingFactor = message.mWeightingFactor;
        mPathTolerance = message.mPathTolerance;
        
    }
    return *this;
}


/*  End of File */
