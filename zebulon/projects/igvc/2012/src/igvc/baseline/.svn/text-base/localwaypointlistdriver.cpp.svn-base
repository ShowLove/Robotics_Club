////////////////////////////////////////////////////////////////////////////////////
///
/// \file localwaypointlistdriver.cpp
/// \brief Class which implements the Local Waypoint List driver service.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "igvc/baseline/localwaypointlistdriver.h"
#include <jaus/mobility/sensors/localposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include <iostream>

using namespace Zebulon;
using namespace IGVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointListDriver::LocalWaypointListDriver()
{
    mDriverID = JAUS::Address(40, 2, 1);
    mMaxDriveVelocity = 2.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointListDriver::~LocalWaypointListDriver()
{
    mDriverThread.StopThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called periodically when the component updates.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    if(mDriverThread.IsThreadActive() == false)
    {
        mDriverThread.CreateThread(LocalWaypointListDriver::DriveThread, this);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function called within thread for driving.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::DriveThread(void* ptr)
{
    LocalWaypointListDriver* d = (LocalWaypointListDriver*)ptr;
    d->Execute();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs local waypoint list drive control.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::Execute()
{
    JAUS::LocalPoseSensor* localPoseSensor = NULL;
    JAUS::VelocityStateSensor* velocityStateSensor = NULL;

    while(mDriverThread.QuitThreadFlag() == false)
    {
        if(mDriverID.IsValid() == false)
        {
            std::cout << "LocalWaypointListDriver::ERROR Driver ID not set!";
        }
        // If we are in a ready state and executing a list of waypoints.
        if(GetComponent()->ManagementService()->GetStatus() == JAUS::Management::Status::Ready &&
           IsExecuting())
        {
            if(localPoseSensor == NULL || velocityStateSensor == NULL)
            {
                localPoseSensor = (JAUS::LocalPoseSensor *)GetComponent()->GetService(JAUS::LocalPoseSensor::Name);
                velocityStateSensor = (JAUS::VelocityStateSensor *)GetComponent()->GetService(JAUS::VelocityStateSensor::Name);
            }
            if(localPoseSensor == NULL || velocityStateSensor == NULL)
            {
                std::cout << "LocalWaypointListDriver::ERROR No Local Pose Sensor or Velocity State Sensor services";
                CxUtils::SleepMs(1);
                continue;
            }
            // If the current element in the list is 0, then we have reached the
            // end of the list.  The element ID in the list is set by the OCP using
            // the Execute message which the LocalWaypointListDriver service processes
            // for you.
            if(GetActiveListElementID() != 0)
            {
                // Get the current waypoint in the list.
                JAUS::SetLocalWaypoint wp = GetCurrentWaypoint();
                JAUS::LocalPose pose = localPoseSensor->GetLocalPose();
                // See if we reached the point.
                CxUtils::Point3D wpPos(wp.GetX(), wp.GetY(), wp.GetZ());
                CxUtils::Point3D myPos(pose.GetX(), pose.GetY(), pose.GetZ());
                double distanceToWaypoint = myPos.Distance(wpPos);
                double tolerance = 1.0;
                double driveVelocity = GetExecutionSpeed();
                if(driveVelocity > mMaxDriveVelocity)
                {
                    driveVelocity = mMaxDriveVelocity;
                }
                // If no speed given, then set one anyway.
                if(fabs(driveVelocity) < .01 || fabs(driveVelocity) > 0.9)
                {
                    driveVelocity = mMaxDriveVelocity;
                }
                if(wp.IsFieldPresent(JAUS::SetLocalWaypoint::PresenceVector::WaypointTolerance))
                {
                    tolerance = wp.GetWaypointTolerance();
                }
                /*
                if(GetActiveListElementID() == 4)
                {
                    tolerance = 0.0;
                }
                */
                if(distanceToWaypoint <= tolerance) // Add vehicle length.
                {
                    // Reached point, advance list.
                    this->AdvanceListElement();
                }
                else
                {
                    // Check for control of Primitive Driver component
                    if(GetComponent()->AccessControlService()->HaveControl(mDriverID) == false)
                    {
                        GetComponent()->AccessControlService()->RequestComponentControl(mDriverID);
                        // Put component in ready state.
                        GetComponent()->ManagementService()->Resume(mDriverID);
                    }

                    if(GetComponent()->AccessControlService()->HaveControl(mDriverID))
                    {
                        // Send drive command to make robot move!
                        // Drive to local waypoint by generating a desired
                        // heading and velocity.

                        // Get angle to waypoint.
                        double angle = atan2(wpPos.mY - myPos.mY, wpPos.mX - myPos.mX);
                        // Convert from local coordinate frame to global coordinate frame for Yaw.
                        double globalAngle = CxUtils::Orientation::AddToAngle(localPoseSensor->GetLocalPoseReference().GetYaw(), angle);
                        JAUS::SetVelocityVector finalVector(mDriverID, GetComponentID());
                        finalVector.SetYaw(globalAngle);
                        /*
                        if(CxUtils::Orientation::AngleDiff(localPoseSensor->GetLocalPose().GetYaw(), angle) >= CxUtils::CxToRadians(20))
                        {
                            driveVelocity = 0;
                        }
                        */
                        finalVector.SetVelocityX(driveVelocity);
                        Send(&finalVector);
                    }
                }
            }
        }
        else
        {
            if(GetComponent()->AccessControlService()->HaveControl(mDriverID))
            {
                GetComponent()->AccessControlService()->ReleaseComponentControl(mDriverID);
            }
        }
        CxUtils::SleepMs(250);
    }
}


/*  End of File */
