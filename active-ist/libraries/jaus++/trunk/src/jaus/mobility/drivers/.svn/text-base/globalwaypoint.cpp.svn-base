////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalwaypoint.h
///  \brief This file contains the implementation of a waypoint structure.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 22 September 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
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
#include "jaus/mobility/drivers/globalwaypoint.h"
#include "jaus/mobility/drivers/setglobalwaypoint.h"
#include "jaus/mobility/drivers/reportglobalwaypoint.h"
#include <iomanip>
#include <cxutils/math/cxmath.h>

const double JAUS::GlobalWaypoint::Limits::MinLatitude = -90.0;
const double JAUS::GlobalWaypoint::Limits::MaxLatitude = 90.0;
const double JAUS::GlobalWaypoint::Limits::MinLongitude = -180.0;
const double JAUS::GlobalWaypoint::Limits::MaxLongitude = 180.0;
const double JAUS::GlobalWaypoint::Limits::MinElevation = -10000;
const double JAUS::GlobalWaypoint::Limits::MaxElevation = 35000;
const double JAUS::GlobalWaypoint::Limits::MinAngle = -CxUtils::CX_PI;
const double JAUS::GlobalWaypoint::Limits::MaxAngle = CxUtils::CX_PI;
const double JAUS::GlobalWaypoint::Limits::MinWaypointTolerance = 0.0;
const double JAUS::GlobalWaypoint::Limits::MaxWaypointTolerance = 100.0;
const double JAUS::GlobalWaypoint::Limits::MinPathTolerance = 0.0;
const double JAUS::GlobalWaypoint::Limits::MaxPathTolerance = 100000.0;

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint::GlobalWaypoint()
{
    mPresenceVector = 0;
    mLatitude = 0;
    mLongitude = 0;
    mAltitude = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mWaypointTolerance = 0;
    mPathTolerance = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint::GlobalWaypoint(const ReportGlobalWaypoint& waypoint)
{
    mPresenceVector = 0;
    mLatitude = 0;
    mLongitude = 0;
    mAltitude = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mWaypointTolerance = 0;
    mPathTolerance = 0;
    *this = waypoint;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint::GlobalWaypoint(const SetGlobalWaypoint& waypoint)
{
    mPresenceVector = 0;
    mLatitude = 0;
    mLongitude = 0;
    mAltitude = 0;
    mRoll = 0;
    mPitch = 0;
    mYaw = 0;
    mWaypointTolerance = 0;
    mPathTolerance = 0;
    *this = waypoint;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint::GlobalWaypoint(const GlobalWaypoint& waypoint)
{
    *this = waypoint;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint::~GlobalWaypoint()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets position data.
///
///   \param[in] position Position to set.
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypoint::SetPosition(const Wgs& position)
{
    bool result = true;
    result &= SetLatitude(position.mLatitude);
    result &= SetLongitude(position.mLongitude);
    result &= SetAltitude(position.mElevation);
    return result;
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
bool GlobalWaypoint::SetLatitude(const double degrees)
{
    if(degrees >= Limits::MinLatitude && degrees <= Limits::MaxLatitude)
    {
        mLatitude = degrees;
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
bool GlobalWaypoint::SetLongitude(const double degrees)
{
    if(degrees >= Limits::MinLongitude && degrees <= Limits::MaxLongitude)
    {
        mLongitude = degrees;
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
bool GlobalWaypoint::SetAltitude(const double value)
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
///   \brief Sets the roll and updates the presence vector for the message.
///
///   \param[in] radians Desired roll in radians[-PI, PI].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypoint::SetRoll(const double radians)
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
bool GlobalWaypoint::SetPitch(const double radians)
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
bool GlobalWaypoint::SetYaw(const double radians)
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
///   \brief Sets the waypoint tolerance value and updates the presence vector for
///          the message.
///
///   \param[in] value Desired waypoint tolerance in meters[0, 100].
///
///   \return true on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypoint::SetWaypointTolerance(const double value)
{
    if(value >= Limits::MinWaypointTolerance && value <= Limits::MaxWaypointTolerance)
    {
        mWaypointTolerance= value;
        mPresenceVector |= PresenceVector::WaypointTolerance;
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
bool GlobalWaypoint::SetPathTolerance(const double value)
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
///   \return Current position data set in structure.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs GlobalWaypoint::GetPosition() const
{
    return Wgs(mLatitude, mLongitude, mAltitude);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets Roll, Pitch, Yaw as a Point3D.
///
///   \return Orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D GlobalWaypoint::GetOrientation() const
{
    Point3D orientation;
    orientation.Set(mRoll, mPitch, mYaw);
    return orientation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypoint::Print() const
{
    std::cout << "WGS: [" << std::fixed << std::setprecision(5) << mLatitude << ", " 
                          << std::fixed << std::setprecision(5) << mLongitude << ", " 
                          << std::fixed << std::setprecision(2) << mAltitude << "]\n";
    if( (mPresenceVector & (PresenceVector::Yaw | PresenceVector::Pitch | PresenceVector::Roll)) > 0)
    {
        std::cout << "Orientation: [" << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mRoll) << ", " 
                                      << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mPitch) << ", " 
                                      << std::fixed << std::setprecision(1) << CxUtils::CxToDegrees(mYaw) << "]\n";
    }
    if( (mPresenceVector & (PresenceVector::PathTolerance | PresenceVector::WaypointTolerance)) > 0)
    {
        std::cout << "Tolerance (Waypoint, Path): [" << mWaypointTolerance << ", " << mPathTolerance << "]" << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if the waypoints are the same by comparing distance
///          between locations only.
///
///   \param[in] waypoint Waypoint to compare with.
///   \param[in] errorInMeters Tolerance for distance check in meters.
///
///   \return True if the same, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypoint::IsSameAs(const Waypoint* waypoint, const double errorInMeters) const
{
    const GlobalWaypoint* gp = dynamic_cast<const GlobalWaypoint*>(waypoint);
    if(gp)
    {
        Wgs a(mLatitude, mLongitude, mAltitude), b(gp->mLatitude, gp->mLongitude, gp->mAltitude);
        if(CxUtils::Wgs::GreatCircleDistance(a, b) <= errorInMeters)
        {
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint& GlobalWaypoint::operator=(const GlobalWaypoint& waypoint)
{
    if(this != &waypoint)
    {
        mActiveFlag = waypoint.mActiveFlag;
        mFinishedFlag = waypoint.mFinishedFlag;

        mPresenceVector = waypoint.mPresenceVector;
        mLatitude = waypoint.mLatitude;
        mLongitude = waypoint.mLongitude;
        mAltitude = waypoint.mAltitude;
        mRoll = waypoint.mRoll;
        mPitch = waypoint.mPitch;
        mYaw = waypoint.mYaw;
        mWaypointTolerance = waypoint.mWaypointTolerance;
        mPathTolerance = waypoint.mPathTolerance;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint& GlobalWaypoint::operator=(const SetGlobalWaypoint& waypoint)
{
    mActiveFlag = waypoint.mActiveFlag;
    mFinishedFlag = waypoint.mFinishedFlag;

    mPresenceVector = (Byte)waypoint.GetPresenceVector();
    mLatitude = waypoint.GetLatitude();
    mLongitude = waypoint.GetLongitude();
    mAltitude = waypoint.GetAltitude();
    mRoll = waypoint.GetRoll();
    mPitch = waypoint.GetPitch();
    mYaw = waypoint.GetYaw();
    mWaypointTolerance = waypoint.GetWaypointTolerance();
    mPathTolerance = waypoint.GetPathTolerance();
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypoint& GlobalWaypoint::operator=(const ReportGlobalWaypoint& waypoint)
{
    mActiveFlag = waypoint.mActiveFlag;
    mFinishedFlag = waypoint.mFinishedFlag;

    mPresenceVector = (Byte)waypoint.GetPresenceVector();
    mLatitude = waypoint.GetLatitude();
    mLongitude = waypoint.GetLongitude();
    mAltitude = waypoint.GetAltitude();
    mRoll = waypoint.GetRoll();
    mPitch = waypoint.GetPitch();
    mYaw = waypoint.GetYaw();
    mWaypointTolerance = waypoint.GetWaypointTolerance();
    mPathTolerance = waypoint.GetPathTolerance();
    return *this;
}


/*  End of File */
