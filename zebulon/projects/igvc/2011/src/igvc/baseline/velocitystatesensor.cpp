////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatesensor.cpp
///  \brief Contains the Velocity State Sensor Service implementation.
///
///  <br>Author(s): David Adams, Daniel Barber
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@gmail.com, dbarber@ist.ucf.edu
///  <br>Web:  http://robotics.ucf.edu
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
#include "igvc/baseline/velocitystatesensor.h"
#include <jaus/core/component.h>

using namespace Zebulon;
using namespace IGVC;
using namespace Utility;
using namespace Encoders;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::VelocityStateSensor()
{
    mParameterMap.clear();
    // Set default values.
    mParameterMap[WheelBase] =  0.7366;
    mParameterMap[WheelRadius] = 0.1524;
    mParameterMap[MaxEncoderTicks] = 23040; // 180*32*4 (CPR*gear ratio*4).
    mParameterMap[FrontBoardID] = 0;
    mParameterMap[RearBoardID] = 1;
    mParameterMap[LeftEncoderID] = 4;
    mParameterMap[RightEncoderID] = 5;
    mParameterMap[SamplePeriodMs] = 50;
    mParameterMap[NumEncoders] = 4;

    mRigidBody.SetWidth(GetParameter(WheelBase));

    mpGlobalPoseSensor = NULL;
    // 50 ms, rc = time constant like for a capacitor
    mpLowPassFilter = new Utility::LowPassFilter(.05, 0.5);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::~VelocityStateSensor()
{
    mVelocityThread.StopThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the sensor service.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::Shutdown()
{
    mVelocityThread.StopThread();
    JAUS::VelocityStateSensor::Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Load settings from settings/services.xml specific to vehicle dimensions
///          and other things necessary for calculating velocity from encoders.
///          The file is necessary for velocity calculations.
///
///   \param[in] filename Name of XML file containing service specific settings.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateSensor::LoadSettings(const std::string& filename)
{
    if (!mXML.Load(filename, "JAUS"))
    {
        std::cout << "Failed loading XML configuration" << std::endl;
        return false;
    }
    double val = 0;
    if(mXML.GetVar("VelocityStateSensor.Dimensions.WheelBase",val))
        SetParameter(WheelBase,val);    
        
    if(mXML.GetVar("VelocityStateSensor.Dimensions.WheelRadius",val))
        SetParameter(WheelRadius,val);
    
    if(mXML.GetVar("VelocityStateSensor.Encoder@max_ticks",val))
        SetParameter(MaxEncoderTicks,val);
    if(mXML.GetVar("VelocityStateSensor.Encoder@front_board_id",val))
        SetParameter(FrontBoardID,val);
    if(mXML.GetVar("VelocityStateSensor.Encoder@rear_board_id",val))
        SetParameter(RearBoardID,val);   
    if(mXML.GetVar("VelocityStateSensor.Encoder@left_encoder_id",val))
        SetParameter(LeftEncoderID,val);
    if(mXML.GetVar("VelocityStateSensor.Encoder@right_encoder_id",val))
        SetParameter(RightEncoderID,val);  
    if(mXML.GetVar("VelocityStateSensor.Encoder@sample_period_ms",val))
    {
        SetParameter(SamplePeriodMs,val);  
        if(mpLowPassFilter)
        {
            delete mpLowPassFilter;
        }
        if(mXML.GetVar("VelocityStateSensor.Encoder@low_pass_filter_rc_ms",val))
        {
            // time change in seconds, rc = time constant like for a capacitor
            mpLowPassFilter = new Utility::LowPassFilter(GetParameter(SamplePeriodMs)/1000.0, val/1000.0);            
        }
        else
        {
            // time change in seconds, rc = time constant like for a capacitor
            mpLowPassFilter = new Utility::LowPassFilter(val/1000.0, val/1000.0);
        }
    }
    if(mXML.GetVar("VelocityStateSensor.Encoder@num_encoders",val))
        SetParameter(NumEncoders,val);  

    mRigidBody.SetWidth(GetParameter(WheelBase));
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Message callback to roboteq controller.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::ProcessMessage(const Motor::Roboteq::AX3500::Message* message, const unsigned int boardID)
{
    int value = 0;
    int encoderID = 0;
    if(Motor::Roboteq::AX3500::ExtractEncoderData(message->mString, encoderID, value) == false)
    {
        return;
    }

    if(mDebugMessagesFlag)
    {
        CxUtils::Mutex::ScopedLock dmLock(&mDebugMessagesMutex);
        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                  << "] - Encoder Update: [" << boardID << "," << encoderID << "] - " << value << "\n";

    }

    if(boardID < 20 && encoderID < 20)
    {
        CxUtils::Mutex::ScopedLock lock(&mMutex);
        Encoder encoder;
        encoder.mRelativeFlag = true;
        encoder.mTimeStamp.SetCurrentTime();
        encoder.mSamplePeriodMs = (CxUtils::Time::Stamp)mParameterMap[SamplePeriodMs];
        encoder.mShaftRadius = mParameterMap[WheelRadius];
        encoder.mValuesPerRevolution = (long long int)mParameterMap[MaxEncoderTicks];
        encoder.mCount = (long long int)value;
        
        if(encoderID == (unsigned int)mParameterMap[LeftEncoderID])
        {
            mRigidBody.UpdateLeftEncoder((unsigned int)boardID, encoder);
        }
        else if(encoderID == (unsigned int)mParameterMap[RightEncoderID])
        {
            mRigidBody.UpdateRightEncoder((unsigned int)boardID, encoder);
        }
    }
    else
    {
        std::cout << "VelocityStateSensor::ERROR - Invalid Board ID or Encoder ID " << boardID << " - " << encoderID << "\n";
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called periodically and is used to calculate velocity
///   data for sensor.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    if(mVelocityThread.IsThreadActive() == false)
    {
        mVelocityThread.CreateThread(VelocityStateSensor::VelocityThread, this);
        mVelocityThread.SetThreadName("Velocity State Sensor");
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread used to calculate velocity of platform using recorded
///          encoder data.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::VelocityThread(void* args)
{
    VelocityStateSensor* sensor = (VelocityStateSensor*)args;
    JAUS::VelocityState prevVelocity = sensor->GetVelocityState();
    JAUS::VelocityState velocity;
    
    while(sensor && sensor->mVelocityThread.QuitThreadFlag() == false)
    {
        bool update = false;
        // Get current orientation.
        if(sensor->mpGlobalPoseSensor == NULL)
        {
            sensor->mpGlobalPoseSensor = dynamic_cast<JAUS::GlobalPoseSensor*>(sensor->GetComponent()->GetService(JAUS::GlobalPoseSensor::Name));
        }
        {
            CxUtils::Mutex::ScopedLock lock(&sensor->mMutex);
            if(sensor->mRigidBody.IsDirty() && sensor->mRigidBody.GetEncoderCount() == (unsigned int)sensor->mParameterMap[NumEncoders])
            {
                update = true;
                // Calculate values.
                sensor->mRigidBody.Update();
                sensor->mpLowPassFilter->Add(sensor->mRigidBody.GetLinearVelocity());
                double filteredVelocity = 0.0;
                sensor->mpLowPassFilter->Output(filteredVelocity);
                /*
                if(filteredVelocity >= 1.0)
                {
                    filteredVelocity = 1.0;
                }
                */
                velocity.SetVelocityX(filteredVelocity);
                velocity.SetVelocityY(0.0);
                velocity.SetVelocityZ(0.0);
                // If this is the first time generated, or no global pose sensor data
                // available, use the encoders for rotational velocity
                if(prevVelocity.GetTimeStamp().ToMs() == 0 || sensor->mpGlobalPoseSensor == NULL)
                {
                    velocity.SetRollRate(0.0);
                    velocity.SetPitchRate(0.0);
                    velocity.SetYawRate(sensor->mRigidBody.GetRotationalVelocity());
                }
                else
                {
                    JAUS::GlobalPose curGlobalPose = sensor->mpGlobalPoseSensor->GetGlobalPose();

                    // Use compass data for calculation of rotational rates since it is
                    // mus more accurate and won't be influenced by spinning wheels in dirt.
                    if(sensor->mPrevGlobalPose.GetTimeStamp().ToMs() != 0)
                    {
                        JAUS::Point3D rotation(CxUtils::Orientation::AngleDiff(sensor->mPrevGlobalPose.GetRoll(), curGlobalPose.GetRoll()),
                                               CxUtils::Orientation::AngleDiff(sensor->mPrevGlobalPose.GetPitch(), curGlobalPose.GetPitch()),
                                               CxUtils::Orientation::AngleDiff(sensor->mPrevGlobalPose.GetYaw(), curGlobalPose.GetYaw()));
                        // Convert angle change to rate.
                        rotation /= (curGlobalPose.GetTimeStamp().ToSeconds() - sensor->mPrevGlobalPose.GetTimeStamp().ToSeconds());
                        velocity.SetRollRate(rotation.mX);
                        velocity.SetPitchRate(rotation.mY);
                        velocity.SetYawRate(rotation.mZ);
                    }
                    else
                    {
                        velocity.SetRollRate(0.0);
                        velocity.SetPitchRate(0.0);
                        velocity.SetYawRate(sensor->mRigidBody.GetRotationalVelocity());
                    }

                    sensor->mPrevGlobalPose = curGlobalPose;
                }
                velocity.SetVelocityRMS(0.0);
                velocity.SetTimeStamp(JAUS::Time(true));
            }
        }
        if(update)
        {
            sensor->SetVelocityState(velocity);
        }
    
        CxUtils::SleepMs(1);
    }
}

/*  End of File */
