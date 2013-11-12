////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
///  <br>Author(s): David Adams
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
#include "igvc/baseline/primitivedriver.h"

using namespace Zebulon;
using namespace IGVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::PrimitiveDriver()
{
    mLeftThrust = mRightThrust = 0;
    mMixer.Setup(Utility::Mixer::Halved, -100, 100);
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
    mLeftThrust = mRightThrust = 0;
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

    mMixer.SetA(command->GetPropulsiveLinearEffortX());
    mMixer.SetB(command->GetPropulsiveRotationalEffortZ());
    mLeftThrust = mMixer.GetX();
    mRightThrust = mMixer.GetY();
    //mLeftThrust = mRightThrust = 50;
    
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
    mLeftThrust = mRightThrust = 0;
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
    mLeftThrust = mRightThrust = 0;
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
    mLeftThrust = mRightThrust = 0;;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for left side of robot.
///
////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetLeftThrust() const 
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    return mLeftThrust;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired thrust for right side of robot.
///
////////////////////////////////////////////////////////////////////////////////////
double PrimitiveDriver::GetRightThrust() const 
{
    JAUS::Mutex::ScopedLock lock(&mThrustMutex);
    return mRightThrust;
}


/*  End of File */
