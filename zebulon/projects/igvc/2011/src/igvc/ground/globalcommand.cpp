////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalcommand.cpp
/// \brief Global Command interface for AI program to drive the
///        ground vehicle.
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
#include "igvc/ground/globalcommand.h"
#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace Zebulon;
using namespace IGVC;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::GlobalCommand()
{
    mDelayTimeMs = 50;
    mCommandMode = AI::GlobalCommand::Power;
    mWrenchEffort.SetPropulsiveLinearEffortX(0);
    mWrenchEffort.SetPropulsiveRotationalEffortZ(0);
    mVelocityCommand.SetVelocityX(0);
    mVelocityCommand.SetYawRate(0);
    mVelocityVector.SetVelocityX(0);
    mStandardsModeFlag = true;
    mMaxVelocityX = 1.28468501;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::~GlobalCommand()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads XML settings.
///
///   \param[in] xmlFilename Name of XML file to load.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalCommand::LoadSettings(const std::string& xmlFilename)
{
    Configuration::XmlConfigurator xml;
    if(xml.Load(xmlFilename, "Ground") == false)
    {
        return false;
    }

    xml.GetVar("MaxVelocityX", mMaxVelocityX, true);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to power control drive mode and sets vehicle thrust.
///
///   \param[in] thrust Vehicle thrust [-100,100]% with positive forward.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetThrust(const double thrust)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    mWrenchEffort.SetPropulsiveLinearEffortX(thrust);
    mVelocityVector.ClearMessageBody();
    mVelocityCommand.ClearMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to power control drive mode and sets vehicle steering.
///
///   \param[in] steering Vehicle steering [-100,100]% with negative to the left.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetSteering(const double steering)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Power;
    mWrenchEffort.SetPropulsiveRotationalEffortZ(steering);
    mVelocityVector.ClearMessageBody();
    mVelocityCommand.ClearMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to closed-loop control drive mode and sets vehicle yaw.
///
///   \param[in] yaw Desired vehicle yaw in radians [-PI, PI].
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetYaw(const double yaw)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Velocity;
    mVelocityVector.SetYaw(yaw);
    mWrenchEffort.ClearMessageBody();
    mStandardsModeFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets control mode to closed loop, and sets desired velocity.
///
///   \param[in] velocity Desired velocity [-max, max] meters/sec.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetVelocity(const double velocity)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Velocity;
    mVelocityVector.SetVelocityX(velocity);
    mVelocityCommand.SetVelocityX(velocity);
    mWrenchEffort.ClearMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets control mode to closed loop, and sets desired rotation rate.
///
///   \param[in] rotationRate Desired rotation rate in rads/s.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetRotationRate(const double rotationRate)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Velocity;
    mVelocityCommand.SetYawRate(rotationRate);
    mWrenchEffort.ClearMessageBody();
    mStandardsModeFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs simple waypoint navigation.  Can do obstacle avoidance if
///          LIDAR data is available.
///
///   \param[in] destination Waypoint to drive to.
///   \param[in] thresh Distance to get to within radius of waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetDestination(const Wgs& destination, const double thresh)
{
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    mCommandMode = AI::GlobalCommand::Position;
    mWaypointThreshold = thresh;
    mDestinationWaypoint = destination;
}


/** Prints status to console window. */
void GlobalCommand::PrintStatus() const
{
    AI::GlobalCommand::PrintStatus();
    CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
    switch(mCommandMode)
    {
    case AI::GlobalCommand::Power:
        mWrenchEffort.PrintMessageBody();
        break;
    default:
        if(mStandardsModeFlag)
        {
            mVelocityCommand.PrintMessageBody();
        }
        else
        {
            mVelocityVector.PrintMessageBody();
        }
    break;
    }
}


/** Loop for generating drive commands. */
void GlobalCommand::CommandThread()
{
    JAUS::AccessControl* control = NULL;
    JAUS::Management* management = NULL;
    control = GetComponent()->AccessControlService();
    management = GetComponent()->ManagementService();
    while(mCommandThread.QuitThreadFlag() == false && control)
    {
        JAUS::Address driverToControl;
        JAUS::Message* driveCommandToSend = NULL;
        // Take control of the correct component based on the
        // type of control mode we are.  If Power, control Primitive Driver
        // component, and release control of Velocity State Driver.  Do the
        // reverse for Velocity State Driver
        if(mCommandMode == AI::GlobalCommand::Power)
        {
            driverToControl = mPrimitiveDriverComponentID;
            if(control->HaveControl(mVelocityStateDriverComponentID))
            {
                control->ReleaseComponentControl(mVelocityStateDriverComponentID);
            }
            if(!control->HaveControl(driverToControl))
            {
                this->RequestComponentControl(driverToControl, true, true);
            }
            driveCommandToSend = &mWrenchEffort;
        }
        else if(mCommandMode == AI::GlobalCommand::Velocity)
        {
            driverToControl = mVelocityStateDriverComponentID;
            if(control->HaveControl(mPrimitiveDriverComponentID))
            {
                control->ReleaseComponentControl(mPrimitiveDriverComponentID);
            }
            if(!control->HaveControl(driverToControl))
            {
                this->RequestComponentControl(driverToControl, true, true);
            }
            if(mStandardsModeFlag)
            {
                driveCommandToSend = &mVelocityCommand;
            }
            else
            {
                driveCommandToSend = &mVelocityVector;
            }
        }
        // Send drive command.
        if(driveCommandToSend)
        {
            CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
            driveCommandToSend->SetSourceID(GetComponentID());
            driveCommandToSend->SetDestinationID(driverToControl);
            Send(driveCommandToSend);
        }

        CxUtils::SleepMs(mDelayTimeMs);
    }
}

/*  End of File */
