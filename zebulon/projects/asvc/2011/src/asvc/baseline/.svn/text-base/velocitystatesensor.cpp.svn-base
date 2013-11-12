/////////////////////////////////////////////////////////////////////////////////////
///
/// \file velocitystatesensor.cpp
/// \brief This class is the Velocity State Sensor service for the ASV, and
///          it combines data received from the vehicles compass and GPS. to output
///          an estimation of the current velocity of the vehicle.
///
/// <br>Author(s): Matthew Znoj<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: mattznoj@gmail.com<br>
/// <br>Web: http://robotics.ucf.edu <br>
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
#include "asvc/baseline/velocitystatesensor.h"
#include <jaus/core/component.h>

using namespace Zebulon;
using namespace ASVC;

const std::string VelocityStateSensor::SamplePeriodMs = "Sample Period Ms";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::VelocityStateSensor()
{
    mpGlobalPoseSensor = NULL;
    // 50 ms, rc = time constant like for a capacitor
    mpLowPassFilter = new Utility::LowPassFilter(.05, 0.5);
    mSamplePeriodMs = 50;
    
    mVelocityThread.CreateThread(VelocityStateSensor::VelocityThread, this);
    mVelocityThread.SetThreadName("Velocity Thread");
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
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
    if(mXML.GetVar("VelocityStateSensor.SamplePeriodMs", val))
    {
        mSamplePeriodMs = (JAUS::Time::Stamp)(val);
    }
    
    return true;
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
    JAUS::Time::Stamp prevUpdateTimeMs = 0;
    
    while(sensor && sensor->mVelocityThread.QuitThreadFlag() == false)
    {
        // Every sample period, try to calculate velocity and rotational rates.
        if(JAUS::Time::GetUtcTimeMs() - prevUpdateTimeMs >= sensor->mSamplePeriodMs && sensor->GetComponent())
        {
            // Get current orientation.
            if(sensor->mpGlobalPoseSensor == NULL)
            {
                sensor->mpGlobalPoseSensor = dynamic_cast<JAUS::GlobalPoseSensor*>(sensor->GetComponent()->GetService(JAUS::GlobalPoseSensor::Name));
            }
            if(sensor->mpGlobalPoseSensor)
            {
                CxUtils::Mutex::ScopedLock lock(&sensor->mMutex);
                JAUS::GlobalPose curGlobalPose = sensor->mpGlobalPoseSensor->GetGlobalPose();

                // Use compass data for calculation of rotational rates
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
                    
                    JAUS::Utm prevPos(JAUS::Wgs(sensor->mPrevGlobalPose.GetLatitude(),
                                                sensor->mPrevGlobalPose.GetLongitude(),
                                                sensor->mPrevGlobalPose.GetAltitude()));
                    JAUS::Utm curPos(JAUS::Wgs(curGlobalPose.GetLatitude(),
                                               curGlobalPose.GetLongitude(),
                                               curGlobalPose.GetAltitude()));
                    // Convert global position changes to local position changes relative
                    // to the vehicle coordinate frame.
                    
                    /*  For testing math only.
                    CxUtils::Point3D localPosChange(-1, // Y
                                 0,    // X
                                 0);
                    localPosChange = localPosChange.Rotate(-CxUtils::CxToRadians(180), CxUtils::Point3D::Z);
                    localPosChange.Print();    
                    */
                    // 1) Get global position change.
                    JAUS::Point3D localPosChange(curPos.mNorthing - prevPos.mNorthing,  // Y
                                                 curPos.mEasting - prevPos.mEasting,    // X
                                                 0);                                    // Z doesn't matter/
                    // 2) Rotate to vehicle perspective with X positive front, Y positive right.
                    localPosChange = localPosChange.Rotate(-sensor->mPrevGlobalPose.GetYaw(), JAUS::Point3D::Z);
                    // 3) Convert to velocity
                    localPosChange /= (curGlobalPose.GetTimeStamp().ToSeconds() - sensor->mPrevGlobalPose.GetTimeStamp().ToSeconds());
                    // 4) Save results.
                    velocity.SetVelocityX(localPosChange.mX);
                    velocity.SetVelocityY(localPosChange.mY);
                    velocity.SetVelocityZ(localPosChange.mZ);
                }
                else
                {
                    velocity.SetRollRate(0.0);
                    velocity.SetPitchRate(0.0);
                    velocity.SetYawRate(0.0);
                }
                
                velocity.SetVelocityRMS(0.0);
                velocity.SetTimeStamp(JAUS::Time(true));

                sensor->SetVelocityState(velocity);
                sensor->mPrevGlobalPose = curGlobalPose;
            }
            
            prevUpdateTimeMs = JAUS::Time::GetUtcTimeMs() ;
        }
        CxUtils::SleepMs(1);
    }
}


/* End of File */
