//////////////////////////////////////////////////////////////////////////////////////
///
/// \file returntochannel.cpp
/// \brief Returns to the location and heading of the marked end of Buoy Channel
///        for returning to dock.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/returntochannel.h"
#include "asvc/surface/missions/buoynavcamerareturn.h"

using namespace Zebulon;
using namespace ASVC;

const std::string ReturnToChannel::Name = "ReturnToChannel";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
ReturnToChannel::ReturnToChannel()
{
    mTravelSpeed = 40;
    mNextMission = BuoyNavCameraReturn::Name;
    ID = Name;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReturnToChannel::~ReturnToChannel()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int ReturnToChannel::Init()
{
    Mission::Init();
    mCurrentState = Waypoint;

    std::string baseName = std::string("Surface.") + ReturnToChannel::Name + ".";

    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);
    mXmlConf.GetVar(baseName + "DistanceToChannel@value", mDistanceThresh);
    mXmlConf.GetVar(baseName + "TravelSpeed@value", mTravelSpeed);
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int ReturnToChannel::ErrorCheck()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int ReturnToChannel::FetchProcess()
{
    this->UpdateMaps(true);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::ChannelEndPosition, mChannelStartUtm);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::ChannelEndHeading, mChannelStartHeading);
    mChannelStartHeading = CxUtils::Orientation::AddToAngle(mChannelStartHeading,
                                                            CxUtils::CxToRadians(180));
    mCurrentPosition = Mission::mpGlobalInfo->GetPosition();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string ReturnToChannel::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string ReturnToChannel::Execute()
{
    double speed = mTravelSpeed;
    double heading = mMapPose.Yaw();

    switch(mCurrentState)
    {
    case Waypoint:
        std::cout << GetName()
                  << " Desired Heading: " << CxUtils::CxToDegrees(heading)
                  << " Desired Speed: " << speed
                  << std::endl;
        std::cout << "Driving to waypoint.\n";
        if(CxUtils::Utm::Distance2D(mCurrentPosition, mChannelStartUtm) <= mDistanceThresh)
        {
            // Reached waypoint.
            speed = 0.0;
            mCurrentState = Heading;
        }
        break;
    case Heading:
        speed = 0.0;
        heading = mChannelStartHeading;
        if(CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), heading) < CxUtils::CxToRadians(15))
        {
            std::cout << "Finsihed.\n";
            return mNextMission;
        }

        std::cout << GetName()
                  << " Desired Heading: " << CxUtils::CxToDegrees(heading)
                  << " Desired Speed: " << speed
                  << " Heading Error: " << CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), heading)
                  << std::endl;
        std::cout << "Going to heading.\n";

        break;
    default:
        std::cout << "Unknown state.\n";
        break;
    }

    // Send drive values.
    mpGlobalCommand->SetForwardVelocityPercent(speed);
    mpGlobalCommand->SetYaw(heading);

    return Mission::KeepRunning;
}


/** Draws maps for display purposes */
void ReturnToChannel::DrawMaps()
{
    MapBuilder::DrawMaps();

    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    CxUtils::Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                                      mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);

    double vLength = 0, vWidth = 0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);

    /*
    // Draw waypoints.
    for(unsigned int i = 0; i < (unsigned int)mWaypoints.size(); i++)
    {
        Buoy marker;
        marker.SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose,
                                                                   this->mpGlobalMap->ConvertFromGlobalToMapCoordinates(mWaypoints[i])));
        if(i < mWaypointCount)
        {
            marker.SetBuoyColor(Buoy::Unknown);
            marker.SetRadius(this->mWaypointThresh/2.0);
        }
        else
        {
            marker.SetBuoyColor(Buoy::Yellow);
            marker.SetRadius(this->mWaypointThresh);
        }
        this->DrawLocalObject(true, &marker, marker.GetDrawColor());
    }
    */

    // Draw vehicle on top of everything.
    mMapPose.Draw(mpNavigator->GetLocalMapImage(ObjectMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}

/*  End of File */
