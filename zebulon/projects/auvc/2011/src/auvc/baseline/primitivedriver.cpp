////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.cpp
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Modified By: John Reeder<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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

#include "auvc/baseline/primitivedriver.h"

using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::PrimitiveDriver()
{
    mMCUInterface1 = NULL;
    mMCUInterface2 = NULL;
    
    mServoAxialRight.mChannel = AXIALRIGHT;
    mServoAxialLeft.mChannel = AXIALLEFT;
    mServoLateralTop.mChannel = LATERALTOP;
    mServoVertFrontRight.mChannel = VERTICALFRONTRIGHT;
    mServoVertFrontLeft.mChannel = VERTICALFRONTLEFT;
    mServoVertBackRight.mChannel = VERTICALBACKRIGHT;
    mServoVertBackLeft.mChannel = VERTICALBACKLEFT;
    
    SetMotorZero();
    
    Reset();
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
    JAUS::Service::Shutdown();
    
    mDriverThreadOne.StopThread();
    mDriverThreadTwo.StopThread();

    if(mMCUInterface1)
    {
        mMCUInterface1->Shutdown();
    }
    if(mMCUInterface2)
    {
        mMCUInterface1->Shutdown();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function called as callback, must do something with setwrencheffort
///          command message.
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::SetWrenchEffort(const JAUS::SetWrenchEffort* command)
{
    mMutex.Lock();
    mWrenchEffort = *command;
    //mMutex.Unlock();
    
    mAxialMotion = mWrenchEffort.GetPropulsiveLinearEffortX();
    mLateralMotion = mWrenchEffort.GetPropulsiveLinearEffortY();
    mVerticalMotion = mWrenchEffort.GetPropulsiveLinearEffortZ();
    mYawMotion = mWrenchEffort.GetPropulsiveRotationalEffortZ();
    mPitchMotion = mWrenchEffort.GetPropulsiveRotationalEffortY();
    mRollMotion = mWrenchEffort.GetPropulsiveRotationalEffortX();
    
    if(fabs(mLateralMotion) > 5)
    {
        mAxialMotionYaw = mLateralMotion*-1.0;
    }
    else
    {
        mAxialMotionYaw = 0;
    }
    
    /*if(fabs(mLateralMotion) > 5)
    {
        mRollMotion -= mLateralMotion*0.25;
    }*/

    mMutex.Unlock();
    
    return  JAUS::OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pointer to the rear motor controller object.
///
////////////////////////////////////////////////////////////////////////////////////


bool PrimitiveDriver::SetControllerOne(MCU::Interface* MCU1)
{
    if(MCU1 != NULL)
    {
        mMCUInterface1 = MCU1;
        if (mDriverThreadOne.IsThreadActive() == false)
        {
            // JAUS::PrimitiveDriver
            mDriverThreadOne.CreateThread(PrimitiveDriver::DriverThreadOne, this);
            mDriverThreadOne.SetThreadName("Underwater Primitive Driver 1");
        }
        return true;
    }
    else
    {
        return false;
    }
}

bool PrimitiveDriver::SetControllerTwo(MCU::Interface* MCU2)
{
    if(MCU2 != NULL)
    {
        mMCUInterface2 = MCU2;
        if (mDriverThreadTwo.IsThreadActive() == false)
        {
            mDriverThreadTwo.CreateThread(PrimitiveDriver::DriverThreadTwo, this);
            mDriverThreadTwo.SetThreadName("Underwater Primitive Driver 2");
        }
        return true;
    }
    else
    {
        return false;
    }
}

void PrimitiveDriver::ForceVectorReady(bool ready)
{
    mIsReady = ready;
}

void PrimitiveDriver::DriverThreadOne(void *args)
{
    PrimitiveDriver* driver = (PrimitiveDriver*)(args);
    
    while(driver && driver->mDriverThreadOne.QuitThreadFlag() == false)
    {
        if(driver->GetStatus() == JAUS::Management::Status::Ready || driver->mIsReady)
        {
            //std::cout << "Primitive Driver: Ready\n";
            if(driver->mMCUInterface1 != NULL)
            {
                
                double leftMotion = 0;
                double rightMotion = 0;
                double offsetMotion = 0.5;
                
                if(fabs(driver->mAxialMotionYaw)>0.1)
                {
                    leftMotion = (-(1.0-offsetMotion)*driver->mAxialMotion+offsetMotion*driver->mAxialMotionYaw);
                    rightMotion = (-(1.0-offsetMotion)*driver->mAxialMotion-offsetMotion*driver->mAxialMotionYaw);
                }
                else
                {
                    leftMotion = (-1*driver->mAxialMotion);
                    rightMotion = (-1*driver->mAxialMotion);
                }
                
                driver->mMutex.Lock();
                // Axial Motors
                driver->mServoAxialRight.mValue = driver->SeabotixConvert(rightMotion);
                driver->mServoAxialLeft.mValue = driver->SeabotixConvert(leftMotion);
                // Lateral Motors
                driver->mServoLateralTop.mValue = driver->SeabotixConvert(driver->mLateralMotion);

                driver->mMutex.Unlock();
                
                // Axial Motors
                driver->mMCUInterface1->Send(&driver->mServoAxialRight);
                driver->mMCUInterface1->Send(&driver->mServoAxialLeft);
                // Lateral Motors
                driver->mMCUInterface1->Send(&driver->mServoLateralTop);
            }
        }
        else
        {
            //std::cout << "Primitive Driver: NOT READY\n";
            driver->SetMotorZero();
        }
        CxUtils::SleepMs(50);
    }
}


void PrimitiveDriver::DriverThreadTwo(void *args)
{
    PrimitiveDriver* driver = (PrimitiveDriver*)(args);
    
    while (driver && driver->mDriverThreadTwo.QuitThreadFlag() == false)
    {
        if(driver->GetStatus() == JAUS::Management::Status::Ready || driver->mIsReady)
        {
            //std::cout << "Primitive Driver: Ready\n";
            if(driver->mMCUInterface2 != NULL)
            {
                driver->mMutex.Lock();
                
                double motor1Roll = 0.25;
                double motor2Roll = -0.25;
                double motor3Roll = 0.25;
                double motor4Roll = -0.25;
                
                /*if(fabs(driver->mLateralMotion) > 5)
                {
                    //std::cout << "Primitive Driver: NEED MORE ROLL\n";
                    motor1Roll = 0.5;
                    motor2Roll = -0.5;
                    motor3Roll = 0.5;
                    motor4Roll = -0.5;
                }*/
                
                
                
                driver->mMotor1 = (0.5)*driver->mVerticalMotion + (motor1Roll)*driver->mRollMotion + (-0.25)*driver->mPitchMotion + (0.25)*driver->mYawMotion;
                driver->mMotor2 = (0.5)*driver->mVerticalMotion + (motor2Roll)*driver->mRollMotion + (-0.25)*driver->mPitchMotion + (-0.25)*driver->mYawMotion;
                driver->mMotor3 = (0.5)*driver->mVerticalMotion + (motor3Roll)*driver->mRollMotion + (0.25)*driver->mPitchMotion + (-0.25)*driver->mYawMotion;
                driver->mMotor4 = (0.5)*driver->mVerticalMotion + (motor4Roll)*driver->mRollMotion + (0.25)*driver->mPitchMotion + (0.25)*driver->mYawMotion;
                
                driver->mMotor1=(driver->mMotor1>100.0)?100.0:(driver->mMotor1<-100)?-100.0:driver->mMotor1;
                driver->mMotor2=(driver->mMotor2>100.0)?100.0:(driver->mMotor2<-100)?-100.0:driver->mMotor2;
                driver->mMotor3=(driver->mMotor3>100.0)?100.0:(driver->mMotor3<-100)?-100.0:driver->mMotor3;
                driver->mMotor4=(driver->mMotor4>100.0)?100.0:(driver->mMotor4<-100)?-100.0:driver->mMotor4;
                
                // Vertical Motors
                driver->mServoVertFrontRight.mValue = driver->SeabotixConvert(driver->mMotor1);
                driver->mServoVertFrontLeft.mValue = driver->SeabotixConvert(-driver->mMotor2);
                driver->mServoVertBackRight.mValue = driver->SeabotixConvert(-driver->mMotor3);
                driver->mServoVertBackLeft.mValue = driver->SeabotixConvert(-driver->mMotor4);
                
                driver->mMutex.Unlock();
                
                // Was this? river->mMCUInterface1->Send(&driver->mServoVertBackLeft,delayms);
                // Vertical Motors
                driver->mMCUInterface2->Send(&driver->mServoVertFrontRight);
                driver->mMCUInterface2->Send(&driver->mServoVertFrontLeft);
                driver->mMCUInterface2->Send(&driver->mServoVertBackRight);
                driver->mMCUInterface2->Send(&driver->mServoVertBackLeft);
            }
        }
        else
        {
            //std::cout << "Primitive Driver: NOT READY\n";
            driver->SetMotorZero();
        }
        CxUtils::SleepMs(50);
    }
}

unsigned char PrimitiveDriver::SeabotixConvert(double motorValue)
{
    unsigned char value;
    
    value = (unsigned char)(128 + ((motorValue/100.0) * 102));
    
    return value;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets standby mode states on motor controllers(stops vehicle)
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Standby()
{
    if(mMCUInterface1)
    {
        mServoAxialRight.mValue = MOTOR_STOP;
        mServoAxialLeft.mValue = MOTOR_STOP;
        mServoLateralTop.mValue = MOTOR_STOP;
        
        mMCUInterface1->Send(&mServoAxialLeft);
        mMCUInterface1->Send(&mServoAxialRight);
        mMCUInterface1->Send(&mServoLateralTop);
    }
    if(mMCUInterface2)
    {
        mServoVertFrontRight.mValue = MOTOR_STOP;
        mServoVertFrontLeft.mValue = MOTOR_STOP;
        mServoVertBackRight.mValue = MOTOR_STOP;
        mServoVertBackLeft.mValue = MOTOR_STOP;
        
        mMCUInterface2->Send(&mServoVertFrontRight);
        mMCUInterface2->Send(&mServoVertFrontLeft);
        mMCUInterface2->Send(&mServoVertBackRight);
        mMCUInterface2->Send(&mServoVertBackLeft);
    }
   
    return JAUS::OK;
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets resume mode state on vehicle.
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Resume()
{
    return JAUS::OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets?
///
////////////////////////////////////////////////////////////////////////////////////
bool PrimitiveDriver::Reset()
{
    return JAUS::OK;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status of this class.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::PrintStatus() const
{
    JAUS::PrimitiveDriver::PrintStatus();
    if(mIsReady)
    {
            mWrenchEffort.PrintMessageBody();
    }
    
    //std::cout << "AT: " << mAxial << "; LT: " << mLateral << "; VT: " << mVertical << "; Y: " << mYaw << "; P: " << mPitch << "; R: " << mRoll << std::endl;
    //std::cout << "ART: " << mAxialRight << "; ALT: " << mAxialLeft << "; LTT: " << mLateralTop << "; LBT: " << mLateralBottom << "; VFT: " << mVerticalFront << "; VBT: " << mVerticalBack << std::endl;
}

void PrimitiveDriver::SetMotorZero()
{
    mMutex.Lock();
    
    mAxialMotion = 0.0;
    mLateralMotion = 0.0;
    mVerticalMotion = 0.0;
    
    mServoAxialRight.mValue = MOTOR_STOP;
    mServoAxialLeft.mValue = MOTOR_STOP;
    mServoLateralTop.mValue = MOTOR_STOP;
    
    mMutex.Unlock();
    
    mMutex.Lock();
    
    mYawMotion = 0.0;
    mPitchMotion = 0.0;
    mRollMotion = 0.0;
    
    mServoVertFrontRight.mValue = MOTOR_STOP;
    mServoVertFrontLeft.mValue = MOTOR_STOP;
    mServoVertBackRight.mValue = MOTOR_STOP;
    mServoVertBackLeft.mValue = MOTOR_STOP;
    
    mMutex.Unlock();
    
    if(mMCUInterface1)
    {
        mMCUInterface1->Send(&mServoAxialLeft);
        mMCUInterface1->Send(&mServoAxialRight);
        mMCUInterface1->Send(&mServoLateralTop);
    }
    if(mMCUInterface2)
    {
        mMCUInterface2->Send(&mServoVertFrontRight);
        mMCUInterface2->Send(&mServoVertFrontLeft);
        mMCUInterface2->Send(&mServoVertBackRight);
        mMCUInterface2->Send(&mServoVertBackLeft);
    }
}
