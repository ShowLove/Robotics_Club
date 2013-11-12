////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
///  <br>Author(s): Matt Znoj
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
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
#include "asvc/baseline/primitivedriver.h"

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::PrimitiveDriver()
{
    mFrontLeftThrust = 0;
    mFrontRightThrust = 0;
    mRearLeftThrust = 0;
    mRearRightThrust = 0;
    mLowerLimit = -100;
    mUpperLimit = 100;

    mMotorAngleRad[FrontLeft] = CxUtils::CxToRadians(30);
    mMotorAngleRad[FrontRight] = CxUtils::CxToRadians(-30);
    mMotorAngleRad[RearLeft] = CxUtils::CxToRadians(-30);
    mMotorAngleRad[RearRight] = CxUtils::CxToRadians(30);

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::~PrimitiveDriver()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shuts down, stops deletes anything that needs it.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::Shutdown()
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    mFrontLeftThrust = mFrontRightThrust = mRearLeftThrust = mRearRightThrust = 0;
    JAUS::Service::Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function called as callback, must do something with setwrencheffort
///          command message.
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::SetWrenchEffort(const JAUS::SetWrenchEffort* command)
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);

    //negating so boat goes forward
    double desiredForward =  -command->GetPropulsiveLinearEffortX();
    double desiredTurn =  command->GetPropulsiveRotationalEffortZ();
    double desiredLateral = command->GetPropulsiveLinearEffortY();

    mFrontLeftThrust = desiredForward-desiredLateral-desiredTurn;              ///<  Left side thrust [-100,100]%.
    mFrontRightThrust = desiredForward+desiredLateral+desiredTurn;             ///<  Right side thrust [-100,100]%.
    mRearLeftThrust = -desiredForward-desiredLateral+desiredTurn;               ///<  Lateral thrust [-100,100]%.
    mRearRightThrust = -desiredForward+desiredLateral-desiredTurn;              ///<  Lateral thrust [-100,100]%.


    //std::cout << "Desired Forward Thrust: " << desiredForward << std::endl;
    //std::cout << "Desired Turn Thrust: " << desiredTurn << std::endl;
    //std::cout << "Desired Lateral Thrust: " << desiredLateral << std::endl;
    //std::cout << desiredForwardMixer << " " << desiredTurnMixer << " " << mLeftThrust << " " << mRightThrust << std::endl;
    return  true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets standby mode states on motor controllers(stops vehicle)
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Standby()
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    mFrontLeftThrust = mFrontRightThrust = mRearLeftThrust = mRearRightThrust = 0;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets resume mode state on vehicle.
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Resume()
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    mFrontLeftThrust = mFrontRightThrust = mRearLeftThrust = mRearRightThrust = 0;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets values.
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Reset()
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    mFrontLeftThrust = mFrontRightThrust = mRearLeftThrust = mRearRightThrust = 0;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for left side of robot [-100,100]%.
///
////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetMotorThrust(Motor motor) const
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    switch(motor)
    {
        case FrontLeft:
            return mFrontLeftThrust;
        case FrontRight:
            return mFrontRightThrust;
        case RearLeft:
            return mRearLeftThrust;
        case RearRight:
            return mRearRightThrust;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints service status.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::PrintStatus() const
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    JAUS::PrimitiveDriver::PrintStatus();
    std::cout << "Desired Front Left Thrust: " << mFrontLeftThrust << std::endl;
    std::cout << "Desired Front Right Thrust: " << mFrontRightThrust << std::endl;
    std::cout << "Desired Rear Left Thrust: " << mRearLeftThrust << std::endl;
    std::cout << "Desired Rear Right Thrust: " << mRearRightThrust << std::endl;
}

/*  End of File */
