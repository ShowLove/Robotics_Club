//////////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalcommand.cpp
/// \brief Class for sending commands to baseline.
///
/// Author(s): Matt Znoj<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: mattznoj@gmail.com<br>
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
//////////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/globalcommand.h"
#include "asvc/surface/globalinfo.h"
#include <jaus/mobility/drivers/primitivedriver.h>
#include <statemachine/utility/utility.h>
#include <statemachine/names.h>
#include <jaus/core/management/management.h>

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::GlobalCommand()
{
    mCommandMode = NoControl;
    mCommandLoops = 0;
    mStandardModeFlag = true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::~GlobalCommand()
{
    mCommandThread.StopThread(500);
    ReleaseComponentControl(JAUS::Address(), true);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes GlobalCommand.
///
///  \param[in] globalInfo A pointer to the subsystems Global Info class
///  \param[in] delayTimeMS Amount of time to wait before sending commands.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalCommand::Initialize(AI::GlobalInfo* globalInfo, unsigned int delayTimeMs)
{
    return AI::GlobalCommand::Initialize(globalInfo, delayTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetXVelocity, sets the value to be sent off to the forward motors
///
///  \param[in]  thrust The velocity to be maintained
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetForwardVelocity(double velocity)
{
    mCommandMutex.Lock();
    mVelocityCommand.SetVelocityX(velocity);
    mCommandMode = AI::GlobalCommand::Velocity;
    mVelocityVector.SetVelocityX(velocity);
    mCommandMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets lateral velocity.
///
///  CURRENTLY DISABLED.
///
///  \param[in]  thrust The value to be sent to the motor
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetLateralVelocity(double velocity)
{
    //return;

    mCommandMutex.Lock();
    mVelocityCommand.SetVelocityY(velocity);
    mVelocityVector.SetVelocityY(velocity);
    mCommandMode = AI::GlobalCommand::Velocity;
    mCommandMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetForwardThrust, sets the value to be sent off to the forward motors
///
///  \param[in]  thrust The value to be sent to the motor
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetForwardThrust(double thrust)
{
    mCommandMutex.Lock();
    mWrenchEffort.SetPropulsiveLinearEffortX(thrust);
    mCommandMode = AI::GlobalCommand::Power;
    mCommandMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets lateral thrust.
///
///  CURRENTLY DISABLED.
///
///  \param[in]  thrust The value to be sent to the motor
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetLateralThrust(double thrust)
{
    mCommandMutex.Lock();
    mWrenchEffort.SetPropulsiveLinearEffortY(thrust);
    mCommandMode = AI::GlobalCommand::Power;
    mCommandMutex.Unlock();

}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetTurningThrust, sets the value to be sent off to the motors to make
///                           the vechile turn.
///
///  \param[in]  thrust The value to be sent to the motor
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetTurningThrust(double thrust)
{
    mCommandMutex.Lock();
    mWrenchEffort.SetPropulsiveRotationalEffortZ(thrust);
    mCommandMode = AI::GlobalCommand::Power;
    mCommandMutex.Unlock();
}

void GlobalCommand::SetDynamixel(const double value)
{
    mCommandMutex.Lock();
    //(*mMicrocontrollerState.GetDigitalStates())[GlobalInfo::PinNames::ComputerLightOne] = value;
    (*mMicrocontrollerState.GetAnalogStates())[Microcontroller::ToString(Microcontroller::DynamixelDesired)] = value;
    //mMicrocontrollerState
    mCommandMutex.Unlock();
}

void GlobalCommand::SetDynamixelDegrees(const double value)
{
    SetDynamixel(CxUtils::CxToRadians(value));
}


// Set actuator length 0 retracted - 100 extended
void GlobalCommand::SetActuator(double value)
{
    mCommandMutex.Lock();
    //(*mMicrocontrollerState.GetDigitalStates())[GlobalInfo::PinNames::ComputerLightOne] = value;
    (*mMicrocontrollerState.GetAnalogStates())[Microcontroller::ToString(Microcontroller::Actuator)] = value;
    //mMicrocontrollerState
    mCommandMutex.Unlock();

}

// Turn the water cannon on/off)
void GlobalCommand::SetPump(bool value)
{
    mCommandMutex.Lock();
    //(*mMicrocontrollerState.GetDigitalStates())[GlobalInfo::PinNames::ComputerLightOne] = value;
    (*mMicrocontrollerState.GetDigitalStates())[Microcontroller::ToString(Microcontroller::Pump)] = value;
    //mMicrocontrollerState
    mCommandMutex.Unlock();

}

// Sets the positon (northing,easting) we want to go to
bool GlobalCommand::SetDesiredWaypoint(const CxUtils::Utm waypoint, double velocity, double angleThresholdDeg, double distanceThreshold)
{
    CxUtils::Utm currentPosition = GetGlobalInfo()->GetPosition();

    double distance = Utm::Distance(currentPosition, waypoint);
    double angleToWaypoint = CxUtils::Orientation::GetGlobalAngle(currentPosition, waypoint, false);
    double currentAngle = GetGlobalInfo()->GetOrientation(true).mZ;

    SetYawUtm(waypoint);

    if(distance < distanceThreshold)
    {
        SetForwardVelocityPercent(0);
        return true;
    }

    if(fabs(CxUtils::Orientation::AngleDiff(currentAngle, angleToWaypoint, false)) < angleThresholdDeg)
    {
        SetForwardVelocityPercent(velocity);
    }
    else
    {
        SetForwardVelocityPercent(0);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetComputerLightOne, sets the status of the green light to be sent
///                        to the MCU
///
///  \param[in]  value The status to be sent to the MCU
///
////////////////////////////////////////////////////////////////////////////////////
//void GlobalCommand::SetComputerLightOne(bool value)
//{
//    mCommandMutex.Lock();
//    (*mMicrocontrollerState.GetDigitalStates())[GlobalInfo::PinNames::ComputerLightOne] = value;
//    mCommandMutex.Unlock();
//}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetComputerLightTwo, sets the status of the yellow light to be sent
///                        to the MCU
///
///  \param[in]  value The status to be sent to the MCU
///
////////////////////////////////////////////////////////////////////////////////////
//void GlobalCommand::SetComputerLightTwo(bool value)
//{
//    mCommandMutex.Lock();
//    (*mMicrocontrollerState.GetDigitalStates())[GlobalInfo::PinNames::ComputerLightTwo] = value;
//    mCommandMutex.Unlock();
//}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetWaterCannon  Sets the motor power value of the water cannon pump
///                        to be sent to the MCU.
///
///  \param[in]  value The value to be sent to the MCU [0, 100]. 100% full on.
///
////////////////////////////////////////////////////////////////////////////////////
//void GlobalCommand::SetWaterCannon(double value)
//{
//    if(value >= -100 && value <= 100.0)
//    {
//        mCommandMutex.Lock();
//        (*mMicrocontrollerState.GetAnalogStates())[GlobalInfo::PinNames::WaterCannon] = value;
//        mCommandMutex.Unlock();
//    }
//}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the tilt for the water cannon.
///
///  \param[in]  value The value to be sent to the MCU [-100, 100]% 100% is
///                    tilt up.
///
////////////////////////////////////////////////////////////////////////////////////
//void GlobalCommand::SetWaterCannonTilt(double value)
//{
//    if(value >= -100 && value <= 100.0)
//    {
//        mCommandMutex.Lock();
//        (*mMicrocontrollerState.GetAnalogStates())[GlobalInfo::PinNames::WaterCannonTilt] = value;
//        mCommandMutex.Unlock();
//    }
//}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the position of the arm.
///
///  \param[in]  value Position value 100% is put away, 0 is rotated all the
///              way out.
///
////////////////////////////////////////////////////////////////////////////////////
//void GlobalCommand::SetArmPosition(const double value)
//{
//    if(value >= 0 && value <= 100.0)
//    {
//        mCommandMutex.Lock();
//        (*mMicrocontrollerState.GetAnalogStates())[GlobalInfo::PinNames::ArmServo] = value;
//        mCommandMutex.Unlock();
//    }
//}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Hover at a waypoint facing a constant direction. Will turn away from heading
///         when outside of hoverzoneradius.
///  \param[in] wgs WGS waypoint to hover about.
///  \param[in] heading Heading to maintan in radians.
///  \param[in] hoverzoneradius Radius around waypoint in which heading is maintained.
///             Otherwise heading will change to most quickly reach waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::Hover(const Wgs& wgs, const double heading, const double hoverzoneradius)
{

}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief SetDesiredHeading  Sets the heading we want to go to.
///
///  \param[in]  heading The heading we want.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetYaw(const double heading)
{
    mCommandMutex.Lock();
    mVelocityVector.SetYaw(heading);
    mCommandMode = AI::GlobalCommand::Velocity;
    mStandardModeFlag = false;
    mCommandMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets yaw based off of destination point.
///
///  \param[in]  northing UTM northing of destination point to face.
///  \param[in]  easting UTM easting of destination point to face.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::SetYawUtm(const double northing, const double easting)
{
    Utm src = GetGlobalInfo()->GetPosition();
    Utm dest = src;
    dest.mNorthing = northing;
    dest.mEasting = easting;
    double yaw = CxUtils::Orientation::GetGlobalAngle(src, dest);

    SetYaw(yaw);
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
    mStandardModeFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Command Thread, creates and sends messages  to a Primitive Driver
///                         or Microcontroller
///
////////////////////////////////////////////////////////////////////////////////////
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

            /* debugging
            if(control->HaveControl(driverToControl))
            {
                std::cout << "We have control of Primitive Driver!\n";
            }
            else
            {
                std:: cout << "We DON'T have control of Primitive Driver!\n";
            }
            */
        }
        else if(mCommandMode == AI::GlobalCommand::Velocity)
        {
            driverToControl = mVelocityStateDriverComponentID;
            if(control->HaveControl(mPrimitiveDriverComponentID))
            {
                control->ReleaseComponentControl(mPrimitiveDriverComponentID);
                /* std::cout << "We have control of primitive driver!\n"; */
            }
            if(!control->HaveControl(driverToControl))
            {
                this->RequestComponentControl(driverToControl, true, true);
            }

            /* debugging
            if(control->HaveControl(driverToControl))
            {
                std::cout << "We have control of Velocity State Driver!\n";
            }
            else
            {
                std:: cout << "We DON'T have control of Velocity State Driver!\n";
            }
            */

            if(mStandardModeFlag)
            {
                driveCommandToSend = &mVelocityCommand;
                /*
                std::cout << "Rate Control"
                          << "X: " << mVelocityCommand.GetVelocityX() << " "
                          << "Y: " << mVelocityCommand.GetVelocityY() << " "
                          << "YawRate: " << mVelocityCommand.GetYawRate() << std::endl;
                */

            }
            else
            {
                driveCommandToSend = &mVelocityVector;
                /*
                std::cout << "Pos Control"
                          << "X: " << mVelocityVector.GetVelocityX() << " "
                          << "Y: " << mVelocityVector.GetVelocityY() << " "
                          << "Yaw: " << mVelocityVector.GetYaw() << std::endl;
                */
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

        if(!control->HaveControl(mMicrocontrollerComponentID))
        {
            this->RequestComponentControl(mMicrocontrollerComponentID, true, true);
        }
        {
            CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
            mMicrocontrollerState.SetSourceID(mGlobalInfo->GetComponentID());
            mMicrocontrollerState.SetDestinationID(mMicrocontrollerComponentID);
            this->Send(&mMicrocontrollerState);
        }

        CxUtils::SleepMs(mDelayTimeMs);
    }
}

/* end of file */
