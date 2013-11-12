////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_subsystem.cpp
///  \brief This file contains a test/example program for a subsystem in
///         JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 13 January 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/mobility/drivers/primitivedriver.h"
#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/mobility/sensors/localposesensor.h"
#include "jaus/mobility/sensors/velocitystatesensor.h"
#include "jaus/mobility/sensors/accelerationstatesensor.h"
#include "jaus/extras/video/visualsensor.h"
#include "jaus/extras/mcu/microcontroller.h"
#include "jaus/extras/rangesensor/rangesensor.h"
#include "jaus/core/component.h"
#include <cxutils/keyboard.h>
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


using namespace JAUS;

#ifdef WIN32
JAUS::UShort gSubsystemID   = 50000;  // ID of our subsystem to use (diff for Windows for debuging).
std::string identificationName = "Win32 Sim";
#else
JAUS::UShort gSubsystemID   = 50001;  // ID of our subsystem to use (diff for Linux for debuging).
std::string identificationName = "Linux Sim";
#endif

JAUS::Byte gNodeID          = 1;
JAUS::Byte gComponentID     = 1;      // ID of the our component.

////////////////////////////////////////////////////////////////////////////////////
///
///   \class FakePrimitiveDriver
///   \brief An example of how to create a Primitive Driver service.
///
///   This fake Primitive Driver is designed to show how you would receive
///   Wrench Effort commands for a simple ground robot with thrust and steering
///   capabilities.
///
////////////////////////////////////////////////////////////////////////////////////
class FakePrimitiveDriver : public PrimitiveDriver
{
public:
    FakePrimitiveDriver()
    {
        Reset();
    }
    ~FakePrimitiveDriver()
    {
        ReleaseControl();
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Whenever the component transitions to a "Ready" state, this method
    ///          is called to signal the Service to transition to the "Ready" state.
    ///
    ///   In this example, the FakePrimtitiveDriver doesn't need to do anything.
    ///   However, in a real implementation, you may want to re-enable a connection
    ///   to your motors, or trigger a relay, etc.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool Resume()
    {
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Whenever the component receives a "Reset" command transition, this
    ///          is called to signal the Service to reset.
    ///
    ///   In this example, the FakePrimtitiveDriver goes back to an initial state
    ///   which in this case sets the thrust and steering to 0.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool Reset()
    {
        mThrust = 0;
        mSteering = 0;
        // Send values to actual motor interface here...
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Whenever the component receives a "Standby" command transition, this
    ///          is called to signal the Service to perform whatever behavior is 
    ///          needed to go to a Standby state.
    ///
    ///   In this example, the FakePrimtitiveDriver goes back to an initial state
    ///   which in this case sets the thrust and steering to 0.  In a real
    ///   application (e.g. ground robot) you would probably want your robot to
    ///   stop driving or turn on brakes, etc.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool Standby()
    {
        mThrust = 0;
        mSteering = 0;
        // Send values to actual motor interface here...
        return false;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Whenever the component receives a "Emergency" command transition, this
    ///          is called to signal the Service to perform whatever behavior is 
    ///          needed to go to an Emergency state.
    ///
    ///   In this example, the FakePrimtitiveDriver goes back to an initial state
    ///   which in this case sets the thrust and steering to 0.  In a real
    ///   application (e.g. ground robot) you would probably want your robot to
    ///   stop driving or turn on brakes, etc.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool SetEmergency()
    {
        mThrust = 0;
        mSteering = 0;
        // Send values to actual motor interface here...
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Whenever the component receives a "Clear Emergency" command transition, 
    ///          this is called to tell the service to transition out of the "Emergency"
    ///          state.
    ///
    ///   Once this is called, the component will transition back to a Standy state
    ///   automatically.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool ClearEmergency()
    {
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is called whenever control is released (or timed out) from
    ///   a controlling component.  It is up to you to implement whatever behavior
    ///   you want when this happens.
    ///
    ///   In this example, the FakePrimtitiveDriver sets the thrust and steering back
    ///   to zero so that our fake ground robot stops driving.
    ///
    ///   \return True on success, false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool ReleaseControl()
    {
        mThrust = 0;
        mSteering = 0;
        // Send values to actual motor interface here...
        return true;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is called whenever a controlling component sends a
    ///          Wrench Effort command.  It is up to the developer to implement
    ///          that command based on their hardware, etc.
    ///
    ///   \return True on success (command processed), false on failure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command)
    {
        bool result = false;
        // Check to see if we are in a "Ready" state, and if so
        // implement the command.
        Byte status = GetComponent()->ManagementService()->GetStatus();
        if(status == Management::Status::Ready)
        {
            if(command->IsFieldPresent(JAUS::SetWrenchEffort::PresenceVector::PropulsiveLinearEffortX))
            {
                mThrust = command->GetPropulsiveLinearEffortX();
                result = true;
            }
            if(command->IsFieldPresent(JAUS::SetWrenchEffort::PresenceVector::PropulsiveRotationalEffortZ))
            {
                mSteering = command->GetPropulsiveRotationalEffortZ();
                result = true;
            }
        }

        return result;
    }   

    volatile double mThrust;        ///< Linear thrust on X axis.
    volatile double mSteering;      ///< Rotational thrust on Z axis.
};  //  End of class FakePrimitiveDriver

// Testing MCU Service code.
class FakeMicrocontroller : public JAUS::Microcontroller
{
public:
    FakeMicrocontroller() {}
    ~FakeMicrocontroller() {}
    virtual void SetDigitalOut(const std::string& name,
                               const bool value) {}
    virtual void SetAnalogOut(const std::string& name,
                              const double value) {}
};


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Entry point of example_subsystem.
///
////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    Component component;

    // Add the services we want our component to have
    // beyond the core set.

    // In this example, we are making a fake/simulated robot.  It
    // has the following services...
    FakePrimitiveDriver* primitiveDriver = new FakePrimitiveDriver();    
    component.AddService(primitiveDriver);

    GlobalPoseSensor* globalPoseSensor = new GlobalPoseSensor();
    globalPoseSensor->SetSensorUpdateRate(25); // Updates at 25 Hz.
    component.AddService(globalPoseSensor);

    LocalPoseSensor* localPoseSensor = new LocalPoseSensor();
    localPoseSensor->SetSensorUpdateRate(25); // Updates at 25 Hz.
    component.AddService(localPoseSensor);

    VelocityStateSensor* velocityStateSensor = new VelocityStateSensor();
    velocityStateSensor->SetSensorUpdateRate(25); // Updates at 25 Hz.
    component.AddService(velocityStateSensor);

    AccelerationStateSensor* accelerationStateSensor = new AccelerationStateSensor();
    accelerationStateSensor->SetSensorUpdateRate(25); // Updates at 25 Hz.
    component.AddService(accelerationStateSensor);

    FakeMicrocontroller* mcu = new FakeMicrocontroller();
    component.AddService(mcu);

    // Create a fake visual sensor, and provide image data using 
    // images on disk.
    VisualSensor* visualSensor = new VisualSensor();
    // In this example there is only 1 camera.
    visualSensor->SetCameraCount(1);
    component.AddService(visualSensor);

    // Fake range sensor.
    RangeSensor* rangeSensor = new RangeSensor();
    // Setup the configuration of a range sensing device.
    RangeSensorConfig rangeSensorConfig;
    rangeSensorConfig.mID = 0;
    rangeSensorConfig.mName = "LIDAR";
    rangeSensorConfig.mMinRange = 0.0;
    rangeSensorConfig.mMaxRange = 9.0;                              // 9 meter range.
    rangeSensorConfig.mUnitType = RangeSensorConfig::MM;            // Range values are in mm.
    rangeSensorConfig.mScanAngle = CxUtils::CxToRadians(270);       // [-135, 135] degrees.
    rangeSensorConfig.mAngleIncrement = CxUtils::CxToRadians(0.25); // Degree increment.
    rangeSensor->AddRangeDevice(rangeSensorConfig);
    // Add to component.
    component.AddService(rangeSensor);

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    if(component.LoadSettings("settings/services.xml") == false)
    {
        // Working directory probably not set (or not running from output directory), so
        // use default values.
        component.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
                                                                 identificationName);
    }

    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
                                                             identificationName);

    // Initialize component with component given ID.
    if(component.Initialize(Address(gSubsystemID, gNodeID, gComponentID)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    // Set mgsImage data to simulate a video feed.
    Image mgsImage;
    int mgsFrameNumber = 0;
    char fname[512];

    // Data/Values for simple physics simulation.

    CxUtils::Point3D orientation;       // Orientation of fake/simulated robot.    
    double velocityX = 0.0;             // Velocity of fake/simulated robot.
    double rotationZ = 0.0;             // Rotation rate of fake/simulated robot.
    double accelerationX = 0.0;         // Acceleration of fake/simulated robot.
    double yawAcceleration = 0.0;       // Yaw acceleration of fake/simulated robot.
    double timeDelta = 0.015;           // Time between updates for simulation.
    CxUtils::Wgs wgsPosition;           // Position in the world.
    CxUtils::Utm utmPosition;           // UTM position in world.

    wgsPosition.mLatitude = 32.703356;
    wgsPosition.mLongitude = -117.253919;
    wgsPosition.mElevation = 300;
    utmPosition << wgsPosition;

    GlobalPose globalPose;
    globalPose.SetLatitude(32.703356);
    globalPose.SetLongitude(-117.253919);
    globalPose.SetAltitude(300);
    globalPose.SetPositionRMS(0.0);
    globalPose.SetRoll(0.0);
    globalPose.SetPitch(0.0);
    globalPose.SetYaw(CxUtils::CxToRadians(45));
    globalPose.SetAttitudeRMS(0.0);
    globalPose.SetTimeStamp(Time::GetUtcTime());

    globalPoseSensor->SetGlobalPose(globalPose);

    ReportVelocityState velocityState;
    velocityState.SetVelocityX(0.0);
    velocityState.SetYawRate(0.0);
    velocityState.SetVelocityRMS(0.0);
    velocityState.SetTimeStamp(Time::GetUtcTime());

    velocityStateSensor->SetVelocityState(velocityState);

    ReportAccelerationState accelerationState;
    accelerationState.SetAccelerationX(0.0);
    accelerationState.SetYawAcceleration(0.0);
    accelerationState.SetAccelerationRMS(0.0);
    accelerationState.SetTimeStamp(Time::GetUtcTime());    
    accelerationStateSensor->SetAccelerationState(accelerationState);

    Time::Stamp printTimeMs = 0;

    double cuttOff = 0.0;

    while(CxUtils::GetChar() != 27)
    {    
        // Load the next simulated frame.  If this was a real
        // vehicle you could get the image data from a camera, etc.        
        sprintf(fname, "video/mgs_frames/mgs_image_%03d.jpg", mgsFrameNumber++);
        if(mgsImage.Load(fname) == FAILURE)
        {
            // Loop forever (start over at mgsImage 0).
            mgsFrameNumber = 0;
            sprintf(fname, "video/mgs_frames/mgs_image_%03d.jpg", mgsFrameNumber++);
            mgsImage.Load(fname);
        }
        // If we were able to load image data...
        if(mgsImage.mDataSize > 0)
        {
            // Camera 0
            visualSensor->SetCurrentFrame(mgsImage.mpImage,
                mgsImage.mWidth,
                mgsImage.mHeight,
                mgsImage.mChannels,
                0,
                mgsFrameNumber - 1,
                30);
        }        

        // These are some really simple/fake physics so we can
        // simulated a moving vehicle in the world.

        double distance = (primitiveDriver->mThrust + 10)*0.005/(100.0*timeDelta);
        double prevVelocityX = velocityX;
        velocityX = distance/timeDelta;
        accelerationX = (velocityX - prevVelocityX) / timeDelta;

        double rotation = CxUtils::CxToRadians(primitiveDriver->mSteering*0.0005 /(100.0*timeDelta));
        double prevRotationZ = rotationZ;
        rotationZ = rotation/timeDelta;
        yawAcceleration = (rotationZ - prevRotationZ) / timeDelta;

        // Now that we know velocity and rotation rates, update
        // the Velocity Acceleration Sensors.
        velocityState.SetVelocityX(velocityX);
        velocityState.SetYawRate(rotationZ);
        velocityState.SetVelocityRMS(0.0);
        velocityState.SetTimeStamp(Time::GetUtcTime());

        velocityStateSensor->SetVelocityState(velocityState);

        accelerationState.SetAccelerationX(accelerationX);
        accelerationState.SetYawAcceleration(yawAcceleration);
        accelerationState.SetAccelerationRMS(0.0);
        accelerationState.SetTimeStamp(Time::GetUtcTime());

        accelerationStateSensor->SetAccelerationState(accelerationState);

        CxUtils::Point3D localPosChange(distance, 0, 0);
        localPosChange = localPosChange.Rotate(globalPose.GetYaw(), CxUtils::Point3D::Z);

        globalPose = globalPoseSensor->GetGlobalPose();

        // Current position.
        wgsPosition(globalPose.GetLatitude(),
            globalPose.GetLongitude(),
            globalPose.GetAltitude());

        // Current orientation
        orientation(globalPose.GetRoll(),
            globalPose.GetPitch(),
            globalPose.GetYaw());

        utmPosition << wgsPosition;
        // Update position.
        utmPosition.mEasting += localPosChange.mY;
        utmPosition.mNorthing += localPosChange.mX;
        wgsPosition << utmPosition;

        globalPose.SetLatitude(wgsPosition.mLatitude);
        globalPose.SetLongitude(wgsPosition.mLongitude);
        globalPose.SetAltitude(wgsPosition.mElevation);

        // Update orientation.
        orientation.mZ = CxUtils::Orientation::AddToAngle(orientation.mZ, rotation);
        globalPose.SetYaw(orientation.mZ);
        // Update the time stamp.
        globalPose.SetTimeStamp(Time::GetUtcTime());

        // Save the result.
        globalPoseSensor->SetGlobalPose(globalPose);
        // You can update the local pose using global pose data if there 
        // is a Global Pose Sensor on your subsystem.
        localPoseSensor->SetLocalPose(globalPose);

        // Update fake MCU numbers.
        mcu->SetDigitalInput("Digital 0", true);
        static double analog = 0.0;
        analog += 0.25;
        if(analog > 100.0)
        {
            analog = 0.0;
        }
        mcu->SetAnalogInput("Analog 0", analog);

        // Simulate fake range sensor data.
        std::vector<double> rangeScan;

        for(double angle = -rangeSensorConfig.mScanAngle/2.0;
            angle <= rangeSensorConfig.mScanAngle/2.0;
            angle += rangeSensorConfig.mAngleIncrement)
        {
            //rangeScan.push_back(fabs(sin(t++)*rangeSensorConfig.mMaxRange*1000.0));

            if(angle < cuttOff)
            {
                rangeScan.push_back(3.0); // Meters.
            }
            else
            {
                rangeScan.push_back(6.0); // Meters.
            }

        }

        cuttOff += 0.01;
        if(cuttOff >= 1)
        {
            cuttOff = -0.5;
        }

        rangeSensor->SetLocalRangeScan(rangeSensorConfig.mID,
            Point3D(0, 0, 0),   // At origin of vehicle.
            Point3D(0, 0, 0),   // Facing front with no roll or pitch change.
            rangeScan);
        
        
        if(Time::GetUtcTimeMs() - printTimeMs > 1000)
        {
            // Print status of services.
            std::cout << "\n======================================================\n";
            component.AccessControlService()->PrintStatus(); std::cout << std::endl;
            primitiveDriver->PrintStatus(); std::cout << std::endl;
            globalPoseSensor->PrintStatus(); std::cout << std::endl;
            localPoseSensor->PrintStatus(); std::cout << std::endl;
            velocityStateSensor->PrintStatus(); std::cout << std::endl;
            accelerationStateSensor->PrintStatus(); std::cout << std::endl;
            mcu->PrintStatus(); std::cout << std::endl;
            printTimeMs = Time::GetUtcTimeMs();
        }

        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        CxUtils::SleepMs((unsigned int)(timeDelta*1000.0));
    }

    // Don't delete services, they are
    // deleted by the Component class.

    // Shutdown any components associated with our subsystem.
    component.Shutdown();

    return 0;
}

/*  End of File */
