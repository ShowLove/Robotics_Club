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
    mLeftThrust = mRightThrust = mLateralThrust = 0;
    mLowerLimit = -15;
    mUpperLimit = 25;
    mMixer.Setup(Utility::Mixer::Halved, mLowerLimit, mUpperLimit);

    mpThrustXFilter = new Utility::LowPassFilter(1/20.0, 1.0);
    mpThrustYFilter = new Utility::LowPassFilter(1/20.0, 1.0);
    mpThrustZFilter = new Utility::LowPassFilter(1/20.0, 1.5);

    mpThrustXFilter->Add(0);
    mpThrustZFilter->Add(0);
    mpThrustYFilter->Add(0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::~PrimitiveDriver()
{
    Shutdown();
    delete mpThrustXFilter;
    delete mpThrustYFilter;
    delete mpThrustZFilter;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shuts down, stops deletes anything that needs it.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::Shutdown()
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    mLeftThrust = mRightThrust = mLateralThrust = 0;
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

    double desiredForwardMixer =  command->GetPropulsiveLinearEffortX();
    double desiredTurnMixer =  command->GetPropulsiveRotationalEffortZ();
    double desiredLateralMixer = command->GetPropulsiveLinearEffortY();

    //std::cout << desiredForwardMixer << " " << desiredTurnMixer << " " << mLeftThrust << " " << mRightThrust << std::endl;

    mpThrustXFilter->Add(desiredForwardMixer);
    mpThrustXFilter->Output(desiredForwardMixer);

    mpThrustZFilter->Add(desiredTurnMixer);
    mpThrustZFilter->Output(desiredTurnMixer);

    mpThrustYFilter->Add(desiredLateralMixer);
    mpThrustYFilter->Output(desiredLateralMixer);

    mMixer.SetA(desiredForwardMixer);
    //mMixer.SetB(0);
    mMixer.SetB(desiredTurnMixer);

    mLeftThrust = mMixer.GetX();
    mRightThrust = mMixer.GetY();

    mLateralThrust = desiredLateralMixer;

    if(mLateralThrust < mLowerLimit)
    {
        mLateralThrust = mLowerLimit;
    }
    if(mLateralThrust > mUpperLimit)
    {
        mLateralThrust = mUpperLimit;
    }


    //std::cout << "Desired Left Thrust: " << mLeftThrust << std::endl;
    //std::cout << "Desired Right Thrust: " << mRightThrust << std::endl;
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
    mLeftThrust = mRightThrust = mLateralThrust = 0;
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
    mLeftThrust = mRightThrust = mLateralThrust = 0;
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
    mLeftThrust = mRightThrust = mLateralThrust = 0;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for left side of robot [-100,100]%.
///

////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetLeftThrust() const
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    return mLeftThrust;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for right side of robot [-100,100]%.
///
////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetRightThrust() const
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    return mRightThrust;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for lateral movement [-100,100]%.
///
////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetLateralThrust() const
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    return mLateralThrust;
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
    std::cout << "Desired Left Thrust: " << mLeftThrust << std::endl;
    std::cout << "Desired Right Thrust: " << mRightThrust << std::endl;
    std::cout << "Desired Lateral Thrust: " << mLateralThrust << std::endl;
}

/*  End of File */
