/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.cpp
/// \brief This is the main file for running the Baseline program for the
///        surface vehicle, which does basic sensor and motor control integration.
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
#include "asvc/baseline/baseline.h"
#include <jaus/mobility/list/listmanager.h>
#include <cstring>
#include <string>

#ifdef WIN32
#define strcasecmp _stricmp
#endif

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::Baseline()
{
    mpCompass = NULL;
    mpGPS = NULL;
    mpLaser = NULL;
    mpLocalPoseSensor = NULL;
    mpGlobalPoseSensor = NULL;
    mpVisualSensor = NULL;
    mpVelocityStateSensor = NULL;
    mpRangeSensor = NULL;
    mpVelocityStateDriver = NULL;
    mpMicrocontroller = NULL;

    mpFPGA = NULL;

    mMaxServoFPGA = 170;
    mMinServoFPGA = 85;

    mJAUSSettingsXML = "settings/services.xml";
    mLateralDynamixelID = 0;
    mArmDynamixelID = 1;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::~Baseline()
{
    Shutdown();

    if(mpCompass)
    {
        delete mpCompass;
    }
    if(mpGPS)
    {
        delete mpGPS;
    }
    if(mpLaser)
    {
        delete mpLaser;
    }
    if(mpFPGA)
    {
        delete mpFPGA;
    }

    mpCompass = NULL;
    mpGPS = NULL;
    mpLaser = NULL;
    mpFPGA = NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads xml path/filename into local variables using custom xmlconfig
///          library.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::LoadSettings(const std::string& xmlFile)
{
    if (!mXmlConf.Load(xmlFile, "Baseline"))
    {
        std::cout << "Failed loading xml configuration" << std::endl;
        return false;
    }
    int subsystem = 30;
    int node = 1;
    int component = 1;
    std::string str;

    mXmlConf.GetVar("Components@subsystem", subsystem);
    mXmlConf.GetVar("Components@node", node);
    mXmlConf.GetVar("Components@identification", str);
    mBaselineComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                  str);
    mVectorDriverComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                      str);
    mLocalWaypointListDriverComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                                 str);

    mXmlConf.GetVar("Components.Baseline@component", component);
    mBaselineComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.Baseline@identification", str))
    {
        mBaselineComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    mXmlConf.GetVar("Components.VectorDriver@component", component);
    mVectorDriverComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.VectorDriver@identification", str))
    {
        mVectorDriverComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    mXmlConf.GetVar("Components.JausChallenge@component", component);
    mLocalWaypointListDriverComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.JausChallenge@identification", str))
    {
        mLocalWaypointListDriverComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    if(!mFPGAInfo.GetInfo(mXmlConf,"Devices.FPGA"))
    {
        return false;
    }
    else
    {
        mXmlConf.GetVar("Devices.FPGA.ServoLimits@max", mMaxServoFPGA);
        mXmlConf.GetVar("Devices.FPGA.ServoLimits@min", mMinServoFPGA);
    }
    if(!mCameraInfo.GetInfo(mXmlConf,"Devices.Camera"))
    {
        return false;
    }
    if(!mTiltCameraInfo.GetInfo(mXmlConf,"Devices.TiltCamera"))
    {
        return false;
    }
    if(!mLaserInfo.GetInfo(mXmlConf,"Devices.Laser"))
    {
        return false;
    }
    if(!mCompassInfo.GetInfo(mXmlConf,"Devices.Compass"))
    {
        return false;
    }
    if(!mGpsInfo.GetInfo(mXmlConf,"Devices.GPS"))
    {
        return false;
    }
    mDynamixelInfo.GetInfo(mXmlConf,"Devices.Dynamixel");
    if(mDynamixelInfo.mEnabledFlag)
    {
        mXmlConf.GetVar("Devices.Dynamixel@lateral_id", mLateralDynamixelID);
        mXmlConf.GetVar("Devices.Dynamixel@arm_id", mArmDynamixelID);
    }

    return true;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Create JAUS services and components and configure.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::SetupJAUS()
{

    // Initialize services and devices.
    // Add services to components.
    mpPrimitiveDriver = new PrimitiveDriver();
    mBaselineComponent.AddService(mpPrimitiveDriver);

    if(mFPGAInfo.mEnabledFlag || mDynamixelInfo.mEnabledFlag)
    {
        mpMicrocontroller = new ASVC::Microcontroller();
        mLocalWaypointListDriverComponent.AddService(mpMicrocontroller);
    }

    if(mCompassInfo.mEnabledFlag || mGpsInfo.mEnabledFlag)
    {
        mpVelocityStateSensor = new VelocityStateSensor();
        //mpVelocityStateSensor->EnableDebugMessages(true);
        mLocalWaypointListDriverComponent.AddService(mpVelocityStateSensor);

        // Create closed-loop controller.
        mpVelocityStateDriver = new VelocityStateDriver();
        mVectorDriverComponent.AddService(mpVelocityStateDriver);
        //mpVelocityStateDriver->EnableDebugMessages(true);
        mpVelocityStateDriver->SetVelocityStateSensor(mpVelocityStateSensor);

        mpGlobalPoseSensor = new GlobalPoseSensor();
        mpGlobalPoseSensor->LoadSettings(mXmlConf.GetPath());
        mBaselineComponent.AddService(mpGlobalPoseSensor);
        if(mCompassInfo.mEnabledFlag)
        {
            // Save pointer to velocity sensor also, so it can
            // get compass data for rotational rate calculations.
            if(mpVelocityStateSensor)
            {
                mpVelocityStateSensor->SetGlobalPoseSensor(mpGlobalPoseSensor);
            }
            if(mpVelocityStateDriver)
            {
                mpVelocityStateDriver->SetGlobalPoseSensor(mpGlobalPoseSensor);
            }
        }
        mpLocalPoseSensor = new JAUS::LocalPoseSensor();
        mLocalWaypointListDriverComponent.AddService(mpLocalPoseSensor);
        mpGlobalPoseSensor->SetLocalPoseSensor(mpLocalPoseSensor);
    }

    if(mLaserInfo.mEnabledFlag)
    {
        mpRangeSensor = new RangeSensor();
        mBaselineComponent.AddService(mpRangeSensor);
    }

    if(mCameraInfo.mEnabledFlag || mTiltCameraInfo.mEnabledFlag)
    {
        mpVisualSensor = new VisualSensor();
        mBaselineComponent.AddService(mpVisualSensor);
    }

    mLocalWaypointListDriverComponent.AddService(new JAUS::ListManager());

    // Setup components
    mBaselineComponent.LoadSettings(mJAUSSettingsXML);
    mVectorDriverComponent.LoadSettings(mJAUSSettingsXML);
    mLocalWaypointListDriverComponent.LoadSettings(mJAUSSettingsXML);
    if(mpVelocityStateSensor)
    {
        mpVelocityStateSensor->LoadSettings(mJAUSSettingsXML);
    }

    // Setting timeout for control to 0 (disables timeout of control).
    // This is done because the JVT and OCP do not query for the timeout period
    // and may not send messages to re-acquire/maintain control within the
    // normal 2 second timeout window.
    mLocalWaypointListDriverComponent.AccessControlService()->SetTimeoutPeriod(0);
    return true;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize class, components and devices from xml(calls loadxml first)
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::InitializeHardware(std::string& errorMessage)
{
    if(mFPGAInfo.mEnabledFlag)
    {
        mpFPGA = new MCU::Interface();
        if(!mpFPGA->Initialize(mFPGAInfo.mPort, mFPGAInfo.mBaud, false, mFPGAInfo.mParity))
        {
            errorMessage.append("Unable to initialize FPGA controller\n");
            return false;
        }
        mpFPGA->RegisterCallback(MCU::Message::All, mpMicrocontroller);
    }

    if(mCompassInfo.mEnabledFlag)
    {
        mpCompass = Compass::Factory::CreateCompass(mCompassInfo.mType);
        if(mpCompass == NULL)
        {
            errorMessage.append("Could not create compass interface to " + mCompassInfo.mType + "!\n");
            return false;
        }
        if(mpCompass->Initialize(mCompassInfo.mPort,mCompassInfo.mBaud,false,1000) == false)
        {
            errorMessage.append("Unable to Initialize Compass " + mCompassInfo.mName +
            " on port " + mCompassInfo.mPort + "\n");
            return false;
        }
        mpCompass->RegisterCallback((Zebulon::Compass::Callback*)mpGlobalPoseSensor);
        mpCompass->SetDeclination(mCompassInfo.mDeclinationDegrees, mCompassInfo.mDeclinationMinutes);
    }

    if(mGpsInfo.mEnabledFlag)
    {
        mpGPS = GPS::Factory::CreateGPS(mGpsInfo.mType);
        if(mpGPS == NULL)
        {
            errorMessage.append("Could not create GPS interface to " + mGpsInfo.mType + "!\n");
            return false;
        }
        if(mpGPS->Initialize(mGpsInfo.mPort,mGpsInfo.mBaud, false, 2000) == false)
        {
            errorMessage.append("Unable to Initialize GPS " + mGpsInfo.mName +
            " on port " + mGpsInfo.mPort + "\n");
            return false;
        }
        mpGPS->RegisterCallback(mpGlobalPoseSensor);
    }

    if(mLaserInfo.mEnabledFlag)
    {
        mpLaser = Laser::LaserFactory::CreateLaser(mLaserInfo.mType);
        if(mpLaser == NULL)
        {
            errorMessage.append("Laser type not found " + mLaserInfo.mType + "\n");
            return false;
        }
        Zebulon::Laser::GenericLaser::Parameters lowLaserParams;
        lowLaserParams.mBaudRate = mLaserInfo.mBaud;
        lowLaserParams.mSerialPort = mLaserInfo.mPort;
        if(!mpLaser->Initialize(&lowLaserParams))
        {
            errorMessage.append("Unable to initialize Laser" + mLaserInfo.mName +
            " on port " + mLaserInfo.mPort + "\n");
            return false;
        }
        JAUS::RangeSensorConfig laserConfig;
        laserConfig.mID = mLaserInfo.mID;
        laserConfig.mName = mLaserInfo.mType;
        laserConfig.mMaxRange = mpLaser->GetMaximumDistance();
        laserConfig.mMinRange = mLaserInfo.mMinDistance;
        laserConfig.mAngleIncrement = mpLaser->GetAngularResolution();
        laserConfig.mScanAngle = fabs(mLaserInfo.mLeftLimit) + fabs(mLaserInfo.mRightLimit);
        laserConfig.mUnitType = (JAUS::RangeSensorConfig::UnitType)mLaserInfo.mUnits;
        mpRangeSensor->SetSensorPose(mLaserInfo.mID, mLaserInfo.mPosition, mLaserInfo.mOrientation);
        mpRangeSensor->AddRangeDevice(laserConfig);
        mpRangeSensor->SetScanLimits(mLaserInfo.mLeftLimit, mLaserInfo.mRightLimit);
        mpLaser->RegisterCallback(mpRangeSensor);
    }

    if(mCameraInfo.mEnabledFlag)
    {
        mCamera.RegisterCallback(mpVisualSensor );
        mCamera.SetID(mCameraInfo.mID);
        if(!mCamera.Start(mCameraInfo.mPort, 0, 0, true, true))
        {
            errorMessage.append("Could not connect to camera " + mCameraInfo.mName + "!\n");
            return false;
        }

        //  Wait for both cameras to have a frame before continuing.
        for(unsigned int i = 0; i < 5000; i++)
        {
            if(mCamera.IsReady())
                break;

            CxUtils::SleepMs(1);
        }
        if(mCamera.IsReady() == false)
        {
            errorMessage.append("Camera failed to get to \"Ready\" state " + mCameraInfo.mName + "!\n");
            return false;
        }
    }
    if(mTiltCameraInfo.mEnabledFlag)
    {
        mTiltCamera.RegisterCallback(mpVisualSensor );
        mTiltCamera.SetID(mTiltCameraInfo.mID);
        if(!mTiltCamera.Start(mTiltCameraInfo.mPort,
                              mTiltCameraInfo.mWidth,
                              mTiltCameraInfo.mHeight,
                              true,
                              true))
        {
            errorMessage.append("Could not connect to camera " + mTiltCameraInfo.mName + "!\n");
            return false;
        }

        //  Wait for both cameras to have a frame before continuing.
        for(unsigned int i = 0; i < 5000; i++)
        {
            if(mTiltCamera.IsReady())
                break;

            CxUtils::SleepMs(1);
        }
        if(mTiltCamera.IsReady() == false)
        {
            errorMessage.append("Camera failed to get to \"Ready\" state " + mTiltCameraInfo.mName + "!\n");
            return false;
        }
    }

    if(mDynamixelInfo.mEnabledFlag)
    {
        if(mDynamixel.Initialize(mDynamixelInfo.mPort, 50, (int)mDynamixelInfo.mBaud) == false)
        {
            bool init = false;
            for(int i = 0; i < 5; i++)
            {
                if(mDynamixel.Initialize(i, 50, (int)mDynamixelInfo.mBaud))
                {
                    init = true;
                    break;
                }
            }
            if(init == false)
            {
                errorMessage.append("Unable to initialize DYNAMIXEL controller\n");
                return false;
            }
        }
    }

    // Initialize thread for communication with FPGA Device
    if (mpFPGA)
    {
        mBaselineThread.CreateThread(Baseline::BaselineThread, this);
        mBaselineThread.SetThreadName("Basline Thread");
    }

    return true;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize JAUS.
///
///   \param[in] errorMessage Error string if method fails.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::InitializeJAUS(std::string& errorMessage)
{
    bool result = true;
    result &= mBaselineComponent.Initialize(mBaselineComponentID);
    // Vector Driver has higher update frequency for closed loop control operations.
    result &= mVectorDriverComponent.Initialize(mVectorDriverComponentID, 25.0);
    result &= mLocalWaypointListDriverComponent.Initialize(mLocalWaypointListDriverComponentID, 25.0);

    if(result == false)
    {
        errorMessage.append("At least 1 JAUS component failed to initialize.\n");
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutdown Hardware.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::ShutdownHardware()
{
    mBaselineThread.StopThread();
    if(mpCompass)
    {
        mpCompass->Shutdown();
    }
    if(mpGPS)
    {
        mpGPS->Shutdown();
    }
    if(mpLaser)
    {
        mpLaser->Shutdown();
    }
    if(mpFPGA)
    {
        mpFPGA->Shutdown();
    }

    mCamera.Stop();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutdown JAUS.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::ShutdownJAUS()
{
    mBaselineComponent.Shutdown();
    mVectorDriverComponent.Shutdown();
    mLocalWaypointListDriverComponent.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Print status of class
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::PrintStatus()
{
    std::cout << "///////////////////////////////////////////////////////" << std::endl;
    std::cout << "///////////////////////////////////////////////////////" << std::endl;
    mBaselineComponent.AccessControlService()->PrintStatus();
    if(mpPrimitiveDriver)
    {
        mpPrimitiveDriver->PrintStatus();
    }
    if(mpGlobalPoseSensor)
    {
        mpGlobalPoseSensor->PrintStatus();
    }
    if(mpVisualSensor)
    {
        mpVisualSensor->PrintStatus();
    }
    /*
    if(mpRangeSensor)
    {
        mpRangeSensor->PrintStatus();
    }
    */
    std::cout << "-------------------------------------------------------" << std::endl;

    mVectorDriverComponent.AccessControlService()->PrintStatus();
    if(mpVelocityStateDriver)
    {
        mpVelocityStateDriver->PrintStatus();
    }

    std::cout << "-------------------------------------------------------" << std::endl;
    mLocalWaypointListDriverComponent.AccessControlService()->PrintStatus();
    //mLocalWaypointListDriverComponent.ManagementService()->PrintStatus();
    /*
    if(mpMicrocontroller)
    {
        mpMicrocontroller->PrintStatus();
    }
    if(mpVelocityStateSensor)
    {
        mpVelocityStateSensor->PrintStatus();
    }
    */
    /*
    if(mpLocalPoseSensor)
    {
        mpLocalPoseSensor->PrintStatus();
    }
    if(mpLocalWaypointListDriver)
    {
        mpLocalWaypointListDriver->PrintStatus();
    }
    */
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when main loop updates.
///
///   This method does any additional communication with other microcontrollers
///   where time is not critical.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::Update()
{


}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread that continually updates various components and communicates
///          with various devices if need be.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::BaselineThread(void *args)
{
    Baseline * baseline  = (Baseline*)args;
    JAUS::Time::Stamp motorUpdateTimeMs = 0;
    JAUS::Time::Stamp mcuUpdateTimeMs = 0;
    JAUS::Time::Stamp motorUpdatePeriod = 50;
    JAUS::Time::Stamp mcuUpdatePeriod = 100;

    //baseline->SetStatus(JAUS::Management::Status::Ready);

    while (baseline &&
           baseline->mBaselineThread.QuitThreadFlag() == false)
    {
        /*
        while(true)
        {
            MCU::SetServoOutputs servoOut;

            servoOut.mChannel = 0;
            servoOut.mValue = 0xFF;
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = 1;
            servoOut.mValue = 0xFF;
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = 2;
            servoOut.mValue = 0xFF;
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = 3;
            servoOut.mValue = 0xFF;
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = 4;
            servoOut.mValue = 0xFF;
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = 5;
            servoOut.mValue = 0x00;
            baseline->mpFPGA->Send(&servoOut);
        }
        */
        // Send motor messages.
        if(JAUS::Time::GetUtcTimeMs() - motorUpdateTimeMs >= motorUpdatePeriod &&
           baseline->mpPrimitiveDriver && baseline->mpFPGA)
        {
            double leftThrust, rightThrust, lateralThrust;
            leftThrust = (127 + ((baseline->mpPrimitiveDriver->GetLeftThrust()/100.0)*127));
            rightThrust = (127 + ((baseline->mpPrimitiveDriver->GetRightThrust()/100.0)*127));


            double rawLateralThrust = baseline->mpPrimitiveDriver->GetLateralThrust();

            lateralThrust = (127 + ((fabs(rawLateralThrust)/100.0)*127));
            MCU::SetServoOutputs rotateServo;
            rotateServo.mChannel = ASVC::Microcontroller::PinNumbers::LateralRotate;

            if(baseline->mDynamixelInfo.mEnabledFlag) // Rotate laterally using dynamixel.
            {
                baseline->mDynamixel.SetServo(baseline->mLateralDynamixelID, -20);
                /*
                if(fabs(rawLateralThrust) < 1)
                {
                    baseline->mDynamixel.SetServo(baseline->mLateralDynamixelID, -20);
                }
                else if(rawLateralThrust > 0) // Angle servo for right movement
                {
                    baseline->mDynamixel.SetServo(baseline->mLateralDynamixelID, 42);
                }
                else if(rawLateralThrust < 0) // Angle servo for left movement
                {
                    baseline->mDynamixel.SetServo(baseline->mLateralDynamixelID, -79);
                }
                */
            }
            else
            {
                /*
                if(fabs(rawLateralThrust) < 1) // Leave lateral forward
                {
                    rotateServo.mValue = 128;
                }
                else if(rawLateralThrust > 0) // Angle servo for right movement
                {
                    rotateServo.mValue = 0;
                }
                else if(rawLateralThrust < 0) // Angle servo for left movement
                {
                    rotateServo.mValue = 255;
                }

                baseline->mpFPGA->Send(&rotateServo);
                */
            }
            // CAP VALUES

            leftThrust = leftThrust < 0 ? 0 : leftThrust;
            leftThrust = leftThrust > 255 ? 255 : leftThrust;

            rightThrust = rightThrust < 0 ? 0 : rightThrust;
            rightThrust = rightThrust > 255 ? 255 : rightThrust;

            lateralThrust = lateralThrust < 0 ? 0 : lateralThrust;
            lateralThrust = lateralThrust > 255 ? 255 : lateralThrust;

            double max = baseline->mMaxServoFPGA;
            double min = baseline->mMinServoFPGA;
            leftThrust = (leftThrust*(max - min)/255.0) + min;
            rightThrust = (rightThrust*(max - min)/255.0) + min;
            lateralThrust = (lateralThrust*(max - min)/255.0) + min;

            MCU::SetServoOutputs servoOut;

            servoOut.mChannel = ASVC::Microcontroller::PinNumbers::LeftServo;
            servoOut.mValue = static_cast<unsigned char>(leftThrust);
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = ASVC::Microcontroller::PinNumbers::RightServo;
            servoOut.mValue = static_cast<unsigned char>(rightThrust);
            baseline->mpFPGA->Send(&servoOut);

            servoOut.mChannel = ASVC::Microcontroller::PinNumbers::LateralServo;
            servoOut.mValue = static_cast<unsigned char>(lateralThrust);
            baseline->mpFPGA->Send(&servoOut);

            motorUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        // Send microcontroller commands and query.
        if(JAUS::Time::GetUtcTimeMs() - mcuUpdateTimeMs >= mcuUpdatePeriod &&
           baseline->mpMicrocontroller && baseline->mpFPGA)
        {
            MCU::SetDigitalOutputs digital;

            digital = baseline->mpMicrocontroller->GetDigitalOutputs(ASVC::Microcontroller::PinNames::ComputerLightOne);
            digital.mPort = Microcontroller::PinNumbers::ComputerLightOne;
            baseline->mpFPGA->Send(&digital);

            digital = baseline->mpMicrocontroller->GetDigitalOutputs(ASVC::Microcontroller::PinNames::ComputerLightTwo);
            digital.mPort = Microcontroller::PinNumbers::ComputerLightTwo;
            baseline->mpFPGA->Send(&digital);

            MCU::SetServoOutputs servo;

            servo = baseline->mpMicrocontroller->GetServoOutputs(ASVC::Microcontroller::PinNames::WaterCannon);

            baseline->mpFPGA->Send(&servo);

            servo = baseline->mpMicrocontroller->GetServoOutputs(ASVC::Microcontroller::PinNames::WaterCannonTilt);

            /*
            for(unsigned char b = 97; b >= 1; b--)
            {
                servo.mValue = b;
                baseline->mpFPGA->Send(&servo);
                CxUtils::SleepMs(100);
            }
            */
            baseline->mpFPGA->Send(&servo);

            // Arm position
            servo = baseline->mpMicrocontroller->GetServoOutputs(ASVC::Microcontroller::PinNames::ArmServo);

            //servo.mValue = 127;

            baseline->mpFPGA->Send(&servo);


            // Query for data.

            // Requests all Digital Inputs
            MCU::ReportDigitalInputs request;
            request.mPort = 0;
            baseline->mpFPGA->Send(&request);

            // Add code here to query analog inputs if in use.

            mcuUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        CxUtils::SleepMs(1);
    }
}
/*  End of File */
