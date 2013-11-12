/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.cpp
/// \brief This is the main file for running the Baseline program for the
///        underwater vehicle, which does basic sensor and motor control integration.
///
/// Author(s): David Adams<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Modified by: John Reeder
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
#include "auvc/baseline/baseline.h"
#include "jaus/mobility/sensors/reportvelocitystate.h"
#include <cstring>
#include <string.h>

#ifdef WIN32
#define strcasecmp _stricmp
#endif

using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::Baseline()
{
    mpCompass = NULL;
    mpDepthSensor = NULL;
    mpLocalPoseSensor = NULL;
    mpGlobalPoseSensor = NULL;
    mpPrimitiveDriver = NULL;
    mpVisualSensor = NULL; 
    mpVelocityStateSensor = NULL; 
    mpVelocityStateDriver = NULL;
    mpRangeSensor = NULL;
    
    mpLaser = NULL;
    
    mpZMCU = NULL;
    mpAVRPower = NULL;
    mpAVRSensor = NULL;
    mpAVRPeripheral = NULL;
    mpAVRDisplay = NULL;
    
    mpPower = NULL;
    mpMotorOne = NULL;
    mpMotorTwo = NULL;
    mpSensor = NULL;
    mpPeripheral = NULL;
    mpDisplay = NULL;
    
    mJAUSSettingsXML = "settings/services.xml";
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
    if(mpDepthSensor)
    {
        delete mpDepthSensor;
    }
    if(mpLaser)
    {
        delete mpLaser;
    }
    /*if(mForwardCamera)
    {
        delete mForwardCamera;
    }
    if(mDownwardCamera)
    {
        delete mDownwardCamera;
    }*/
    //mForwardCamera.Stop();
    //mDownwardCamera.Stop();
    //mpStereoCamera.Stop();
    
    if(mpAVRPower)
    {
        delete mpAVRPower;
    }
    if(mpAVRSensor)
    {
        delete mpAVRSensor;
    }
    if(mpAVRPeripheral)
    {
        delete mpAVRPeripheral;
    }
    if(mpAVRDisplay)
    {
        delete mpAVRDisplay;
    }
    
    
    if(mpPower)
    {
        delete mpPower;
    }
    if(mpMotorOne)
    {
        delete mpMotorOne;
    }
    if(mpMotorTwo)
    {
        delete mpMotorTwo;
    }
    if(mpSensor)
    {
        delete mpSensor;
    }
    if(mpPeripheral)
    {
        delete mpPeripheral;
    }
    if(mpDisplay)
    {
        delete mpDisplay;
    }
    
    mpAVRPower = NULL;
    mpAVRSensor = NULL;
    mpAVRPeripheral = NULL;
    mpAVRDisplay = NULL;
    
    mpCompass = NULL;
    mpDepthSensor = NULL;
    mpLaser = NULL;
    mpPower = NULL;
    mpMotorOne = NULL;
    mpMotorTwo = NULL;
    mpSensor = NULL;
    mpPeripheral = NULL;
    mpDisplay = NULL;
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
    int subsystem = 20;
    int node = 1;
    int component = 1;
    std::string str;
    
    mXmlConf.GetVar("Components@subsystem", subsystem);
    mXmlConf.GetVar("Components@node", node);
    mXmlConf.GetVar("Components@identification", str);
    mBaselineComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                  str);
    /*mVectorDriverComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                      str);*/
    mChallengeComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                                 str);
                                                                                 
    mXmlConf.GetVar("Components.Baseline@component", component);
    mBaselineComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.Baseline@identification", str))
    {
        mBaselineComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    /*mXmlConf.GetVar("VectorDriver.component@identification", component);
    mVectorDriverComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.VectorDriver@identification", str))
    {
        mVectorDriverComponent.DiscoveryService()->SetComponentIdentification(str);
    }*/
    
    mXmlConf.GetVar("Components.JausChallenge@component", component);
    mChallengeComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConf.GetVar("Components.JausChallenge@identification", str))
    {
        mChallengeComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    if(!mCompassInfo.GetInfo(mXmlConf,"Devices.Compass"))
    {
        return false;
    } 
    if(!mDepthSensorInfo.GetInfo(mXmlConf,"Devices.DepthSensor"))
    {
        return false;
    } 
    if(!mLaserInfo.GetInfo(mXmlConf,"Devices.Laser"))
    {
        return false;
    }
    if(!mAVRPowerInfo.GetInfo(mXmlConf,"Devices.AVRPower"))
    {
        return false;
    }
    if(!mAVRMotorControl1Info.GetInfo(mXmlConf,"Devices.AVRMotorControl1"))
    {
        return false;
    }
    if(!mAVRMotorControl2Info.GetInfo(mXmlConf,"Devices.AVRMotorControl2"))
    {
        return false;
    }
    if(!mAVRSensorInfo.GetInfo(mXmlConf,"Devices.AVRSensor"))
    {
        return false;
    }
    if(!mAVRPeripheralInfo.GetInfo(mXmlConf,"Devices.AVRPeripheral"))
    {
        return false;
    }
    if(!mAVRDisplayInfo.GetInfo(mXmlConf,"Devices.AVRDisplay"))
    {
        return false;
    }
    /*if(!mForwardCameraInfo.GetInfo(mXmlConf,"Devices.ForwardCamera"))
    {
        return false;
    }
    if(!mDownwardCameraInfo.GetInfo(mXmlConf,"Devices.DownwardCamera"))
    {
        return false;
    }*/
    // Add stereo camera
    
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
    
    
    if(mAVRMotorControl1Info.mEnabledFlag || mAVRMotorControl2Info.mEnabledFlag)
    {
        mpPrimitiveDriver = new PrimitiveDriver();
        mBaselineComponent.AddService(mpPrimitiveDriver);
        
        mpVelocityStateDriver = new VelocityStateDriver();
        mBaselineComponent.AddService(mpVelocityStateDriver);
    }
    
    if(mCompassInfo.mEnabledFlag || mDepthSensorInfo.mEnabledFlag)
    {
        mpGlobalPoseSensor = new GlobalPoseSensor();
        mpVelocityStateSensor = new VelocityStateSensor();
        mBaselineComponent.AddService(mpGlobalPoseSensor);
        mBaselineComponent.AddService(mpVelocityStateSensor);
    }
    
    // Add the rest of the MCUs
    if(mAVRPowerInfo.mEnabledFlag || mAVRSensorInfo.mEnabledFlag || 
        mAVRPeripheralInfo.mEnabledFlag || mAVRDisplayInfo.mEnabledFlag)
    {
        mpZMCU = new Utility::Microcontroller;
        mBaselineComponent.AddService(mpZMCU);
        mpZMCU->Initialize();
    }
    
    if(mLaserInfo.mEnabledFlag)
    {
        mpRangeSensor = new RangeSensor();
        mBaselineComponent.AddService(mpRangeSensor);
    }
    
    /*if(mForwardCameraInfo.mEnabledFlag || mDownwardCameraInfo.mEnabledFlag)
    {
        mpVisualSensor = new VisualSensor();
        mBaselineComponent.AddService(mpVisualSensor);
    }*/
    
    //setup components
    mBaselineComponent.LoadSettings(mJAUSSettingsXML);
    //mVectorDriverComponent.LoadSettings(mJAUSSettingsXML);
    mChallengeComponent.LoadSettings(mJAUSSettingsXML);  
    // Setting timeout for control to 0 (disables timeout of control).
    // This is done because the JVT and OCP do not query for the timeout period
    // and may not send messages to re-acquire/maintain control within the
    // normal 2 second timeout window.  
    //mLocalWaypointListDriverComponent.AccessControlService()->SetTimeoutPeriod(0);
    return true;
}

void Baseline::SetupRawCamera(std::string name,fw1394Camera *camera)
{
    int Shutter,Red,Blue;
    int HW,SW,Debug,Avg;
    double P;
    
    
    mXmlConf.GetVar("Devices."+name+".Auto@HW", HW);
    mXmlConf.GetVar("Devices."+name+".Auto@SW", SW);
    mXmlConf.GetVar("Devices."+name+".Auto@Debug", Debug);
    mXmlConf.GetVar("Devices."+name+".Auto@Avg", Avg);
    mXmlConf.GetVar("Devices."+name+".Auto@P", P);
    
    //by default Auto HW
    //automatically
    if(HW==1)
    {
        
    }
    //use software auto settings
    else if(SW==1)
    {
        camera->enableAutoWhite(true);
        camera->enableAutoShutter(true);
        camera->debugDisplay(Debug);
        camera->SetDesiredAvg(Avg);
        camera->setPTerm(P);
    }
    //manual from the file
    else
    {
        mXmlConf.GetVar("Devices."+name+".Shutter@Value", Shutter);
        mXmlConf.GetVar("Devices."+name+".WhiteBalance@Red", Red);
        mXmlConf.GetVar("Devices."+name+".WhiteBalance@Blue", Blue);
        camera->SetShutter(Shutter);
        camera->SetWhiteBalance(Red,Blue);
        std::cout << " Shutter: " << Shutter
        << " Red: " << Red
        << " Blue: " << Blue
        << std::endl;
    }
    
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize class, components and devices from xml(calls loadxml first)
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::InitializeHardware(std::string& errorMessage)
{    
    
    if(mAVRMotorControl1Info.mEnabledFlag)
    {
        std::cout << "Beginning" << std::endl;
        mpMotorOne = new MCU::Interface;
        if(!mpMotorOne)
        {
            std::cout << "Unable to initialize MCU interface for motor control 1";
            return false;
        }
        std::cout << "Port: " << mAVRMotorControl1Info.mPort << ", Baud: " << mAVRMotorControl1Info.mBaud << std::endl;
        if(!mpMotorOne->Initialize(mAVRMotorControl1Info.mPort, mAVRMotorControl1Info.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Unable to initialize motor controller mcu 1";
            return false;
        }
        mpPrimitiveDriver->SetControllerOne(mpMotorOne);
    }
    
    if(mAVRMotorControl2Info.mEnabledFlag)
    {
        mpMotorTwo = new MCU::Interface;
        
        if(!mpMotorTwo)
        {
            std::cout << "Unable to initialize MCU interface for motor control 2";
            return false;
        }
        if(!mpMotorTwo->Initialize(mAVRMotorControl2Info.mPort, mAVRMotorControl2Info.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Unable to initialize motor controller mcu 2";
            return false;
        }
        mpPrimitiveDriver->SetControllerTwo(mpMotorTwo);
    }
        
    if(mCompassInfo.mEnabledFlag)
    {
#ifdef USE_GX3
        mpCompass = new INS::MicroStrain::GX3();
        if(mpCompass == NULL)
        {
            errorMessage.append("Could not create compass interface to " + mCompassInfo.mType + "!\n");
            return false;
        }
        //std::cout << "Port: " << mCompassInfo.mPort << ", Baud: " << mCompassInfo.mBaud << std::endl;
        if(!mpCompass->Initialize(mCompassInfo.mPort, mCompassInfo.mBaud, false, 1000))
        {
            errorMessage.append("Unable to Initialize Compass " + mCompassInfo.mName +
            " on port " + mCompassInfo.mPort + "\n");
            return false;
        }

        double roll,pitch,yaw;
        mXmlConf.GetVar("Devices.Compass.Offsets@roll", roll);
        mXmlConf.GetVar("Devices.Compass.Offsets@pitch", pitch);
        mXmlConf.GetVar("Devices.Compass.Offsets@yaw", yaw);
        mpGlobalPoseSensor->SetCompassMountOffset(CxUtils::CxToRadians(yaw),CxUtils::CxToRadians(pitch),CxUtils::CxToRadians(roll));


        mpCompass->RegisterCallback(mpGlobalPoseSensor, INS::MicroStrain::Callback::IMUCallback);
        mpCompass->SetDeclination(mCompassInfo.mDeclinationDegrees, mCompassInfo.mDeclinationMinutes);

        mpCompass->RegisterCallback(mpVelocityStateSensor, INS::MicroStrain::Callback::IMUCallback);
        mpCompass->RegisterCallback(mpVelocityStateSensor, INS::MicroStrain::Callback::VELCallback);
#else
        mpCompass = Compass::Factory::CreateCompass(mCompassInfo.mType);
        if(mpCompass == NULL)
        {
            errorMessage.append("Could not create compass interface to " + mCompassInfo.mType + "!\n");
            return false;
        }
        //std::cout << "Port: " << mCompassInfo.mPort << ", Baud: " << mCompassInfo.mBaud << std::endl;
        if(!mpCompass->Initialize(mCompassInfo.mPort, mCompassInfo.mBaud, false, 1000))
        {
            errorMessage.append("Unable to Initialize Compass " + mCompassInfo.mName + 
            " on port " + mCompassInfo.mPort + "\n");
            return false;
        }

        mpCompass->RegisterCallback(mpGlobalPoseSensor, Compass::Callback::IMUCallback);        
        mpCompass->SetDeclination(mCompassInfo.mDeclinationDegrees, mCompassInfo.mDeclinationMinutes);
#endif
    }
    
    if(mDepthSensorInfo.mEnabledFlag)
    {
        mpDepthSensor = new Depth::Keller();
        
        if(mpDepthSensor == NULL)
        {
            errorMessage.append("Could not create depth sensor interface to " + mDepthSensorInfo.mType + "!\n");
            return false;
        }
        
        if(!mpDepthSensor->Initialize(mDepthSensorInfo.mPort, mDepthSensorInfo.mRequestRate, mDepthSensorInfo.mBaud, false))
        {
            errorMessage.append("Unable to Initialize Depth Sensor " + mDepthSensorInfo.mName + 
            " on port " + mDepthSensorInfo.mPort + "\n");
            return false;
        }
        
        mpDepthSensor->RegisterCallback(mpGlobalPoseSensor);

        double depthOffset;
        mXmlConf.GetVar("Devices.DepthSensor@offset", depthOffset);
        mpGlobalPoseSensor->SetDepthSensorOffset(depthOffset);
    }
    
    if(mLaserInfo.mEnabledFlag)
    {
        mpLaser = Laser::LaserFactory::CreateLaser(mLaserInfo.mType);
        if(mpLaser == NULL)
        {
            errorMessage.append("Laser type not found " + mLaserInfo.mType + "\n");
            return false;
        }
        Zebulon::Laser::GenericLaser::Parameters laserParams;
        laserParams.mBaudRate = mLaserInfo.mBaud;
        laserParams.mSerialPort = mLaserInfo.mPort;
        if(!mpLaser->Initialize(&laserParams))
        {
            errorMessage.append("Unable to initialize Laser" + mLaserInfo.mName + 
            " on port " + mLaserInfo.mPort + "\n");
            return false;
        }
        
        JAUS::RangeSensorConfig laserConfig;
        laserConfig.mID = mLaserInfo.mID;
        laserConfig.mName = mLaserInfo.mType;
        laserConfig.mMaxRange = mpLaser->GetMaximumDistance();
        laserConfig.mMinRange = mpLaser->GetMinimumDistance();
        laserConfig.mAngleIncrement = mpLaser->GetAngularResolution();
        laserConfig.mScanAngle = mpLaser->GetScanAngle();
        laserConfig.mUnitType = (JAUS::RangeSensorConfig::UnitType)mLaserInfo.mUnits;
        mpRangeSensor->SetSensorPose(mLaserInfo.mID, mLaserInfo.mPosition, mLaserInfo.mOrientation);
        mpRangeSensor->AddRangeDevice(laserConfig);
        mpLaser->RegisterCallback(mpRangeSensor);
    }
     
    /*if(mForwardCameraInfo.mEnabledFlag)
    {
        mForwardCamera.RegisterCallback(mpVisualSensor);
        mForwardCamera.SetID(mForwardCameraInfo.mID);
        if(!mForwardCamera.Start(mForwardCameraInfo.mPort, mForwardCameraInfo.mWidth, mForwardCameraInfo.mHeight, false, true))
        {
            errorMessage.append("Could not connect to camera " + mForwardCameraInfo.mName + "!\n");
            return false;
        }
        mpForwardCameraRaw=(fw1394Camera *)mForwardCamera.GetRawCapture();
        SetupRawCamera(mForwardCameraInfo.mName,mpForwardCameraRaw);
        
        //  Wait for both cameras to have a frame before continuing.
        for(unsigned int i = 0; i < 5000; i++)
        {
            if(mForwardCamera.IsReady())
                break;
                
            CxUtils::SleepMs(1);
        }
        if(mForwardCamera.IsReady() == false)
        {
            errorMessage.append("Camera failed to get to \"Ready\" state " + mForwardCameraInfo.mName + "!\n");
            return false;
        }
    }*/
    
    /*if(mDownwardCameraInfo.mEnabledFlag)
    {
        mDownwardCamera.RegisterCallback(mpVisualSensor);
        mDownwardCamera.SetID(mDownwardCameraInfo.mID);
        if(!mDownwardCamera.Start(mDownwardCameraInfo.mPort,mDownwardCameraInfo.mWidth,mDownwardCameraInfo.mHeight,false, true))
        {
            errorMessage.append("Could not connect to camera " + mDownwardCameraInfo.mName + "!\n");
            return false;
        }
        mpDownwardCameraRaw=(fw1394Camera *)mDownwardCamera.GetRawCapture();
        SetupRawCamera(mDownwardCameraInfo.mName,mpDownwardCameraRaw);
        
        
        //  Wait for both cameras to have a frame before continuing.
        for(unsigned int i = 0; i < 5000; i++)
        {
            if(mDownwardCamera.IsReady())
                break;
                
            CxUtils::SleepMs(1);
        }
        if(mDownwardCamera.IsReady() == false)
        {
            errorMessage.append("Camera failed to get to \"Ready\" state " + mDownwardCameraInfo.mName + "!\n");
            return false;
        }
    }*/
    
    if(mAVRPowerInfo.mEnabledFlag)
    {
        mpAVRPower = new Utility::Appliance;
        mpPower = new MCU::Interface;
        //std::cout << "Port: " << mAVRPowerInfo.mPort << " Baud: " << mAVRPowerInfo.mBaud << std::endl;
        //mPower->Initialize(mAVRPowerInfo.mPort, mAVRPowerInfo.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY);

        // Set other parameters
        mpAVRPower->SetADCBits(16); // or 12?
        // Add Analog Inputs
        mpAVRPower->AddAnalogInput("ElectronicVoltage", ELEC_VOLTAGE,0.0,102.4);
        mpAVRPower->AddAnalogInput("ElectronicCurrent", ELEC_CURRENT,0,8.192);
        mpAVRPower->AddAnalogInput("MotorVoltage", MOTOR_VOLTAGE,0.0,102.4);
        mpAVRPower->AddAnalogInput("MotorCurrent", MOTOR_CURRENT,0,8.192    );
        // Add Digital Inputs
        mpAVRPower->AddDigitalInput("StartButton", START);
        mpAVRPower->AddDigitalInput("E-Stop", ESTOP);
        mpAVRPower->AddDigitalInput("ElectronicBatteryPower", BATT_E_PWR);
        mpAVRPower->AddDigitalInput("ElectronicShorePower", SHR_E_PWR);
        mpAVRPower->AddDigitalInput("MotorBatteryPower", BATT_M_PWR);
        mpAVRPower->AddDigitalInput("MotorShorePower", SHR_M_PWR);
        // Add Digital Outputs
        //mpAVRPower->AddDigitalOutput("ElectronicShoreSwitch", POWERSUPPLYELEC);
        //mpAVRPower->AddDigitalOutput("MotorShoreSwitch", POWERSUPPLYMOTOR);
        // Passes MCUInterface to Appliance and registers the callback
        
        mpAVRPower->SetMCUInterface(mpPower);
        // Passes the Appliance to ZMCU and starts the thread
        if(!mpZMCU->AddMCU(mpAVRPower))
        {
            //std::cout << "hello" << std::endl;
            std::cout << "Could not create microcontroller interface to " << mAVRPowerInfo.mName << "!\n";
            return false;
        }
        
        if(!mpPower->Initialize(mAVRPowerInfo.mPort, mAVRPowerInfo.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Could not create microcontroller interface to " << mAVRPowerInfo.mName << "!\n";
            return false;
        }
    }
    // Sensor Board
    if(mAVRSensorInfo.mEnabledFlag)
    {
        mpAVRSensor = new Utility::Appliance;
        mpSensor = new MCU::Interface;

        // Set other parameters
        mpAVRSensor->SetADCBits(16);
        // Add Analog Inputs
        mpAVRSensor->AddAnalogInput("RelativeHumidity", HUMIDITY);
        //mpAVRSensor->AddAnalogInput("Temperature", TEMPERATURE,-22.0,212.0);
        mpAVRSensor->AddAnalogInput("Temperature", TEMPERATURE,-2.20,21.20);
        mpAVRSensor->AddAnalogInput("AnalogDepth", DEPTH);
        mpAVRSensor->AddAnalogInput("AD0", AD0);
        mpAVRSensor->AddAnalogInput("AD1", AD1);
        mpAVRSensor->AddAnalogInput("AD2", AD2);
        
        mpAVRSensor->SetMCUInterface(mpSensor);
        // Passes the Appliance to ZMCU and starts the thread
        if(!mpZMCU->AddMCU(mpAVRSensor))
        {
            //std::cout << "hello" << std::endl;
            std::cout << "Could not create microcontroller interface to " << mAVRSensorInfo.mName << "!\n";
            return false;
        }
        
        if(!mpSensor->Initialize(mAVRSensorInfo.mPort, mAVRSensorInfo.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Could not create microcontroller interface to " << mAVRSensorInfo.mName << "!\n";
            return false;
        }
    }
    // Peripheral Board
    if(mAVRPeripheralInfo.mEnabledFlag)
    {
        mpAVRPeripheral = new Utility::Appliance;
        mpPeripheral = new MCU::Interface;
        
        // Add Digital Outputs
        mpAVRPeripheral->AddDigitalOutput("Torpedo1", TORPEDO1);
        mpAVRPeripheral->AddDigitalOutput("Torpedo2", TORPEDO2);
        mpAVRPeripheral->AddDigitalOutput("Dropper1", DROPPER1);
        mpAVRPeripheral->AddDigitalOutput("Dropper2", DROPPER2);
        mpAVRPeripheral->AddDigitalOutput("GrabberArm1", GRABBERARM1);
        mpAVRPeripheral->AddDigitalOutput("GrabberArm2", GRABBERARM2);
        // Passes MCUInterface to Appliance and registers the callback
        
        mpAVRPeripheral->SetMCUInterface(mpPeripheral);
        // Passes the Appliance to ZMCU and starts the thread
        if(!mpZMCU->AddMCU(mpAVRPeripheral))
        {
            //std::cout << "hello" << std::endl;
            std::cout << "Could not create microcontroller interface to " << mAVRPeripheralInfo.mName << "!\n";
            return false;
        }
        
        if(!mpPeripheral->Initialize(mAVRPeripheralInfo.mPort, mAVRPeripheralInfo.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Could not create microcontroller interface to " << mAVRPeripheralInfo.mName << "!\n";
            return false;
        }
    }
    // Display Board
    if(mAVRDisplayInfo.mEnabledFlag)
    {
        mpAVRDisplay = new Utility::Appliance;
        mpDisplay = new MCU::Interface;
        
        // Add Analog Outputs
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Left_Red", UPPER_LEFT_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Left_Green", UPPER_LEFT_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Left_Blue", UPPER_LEFT_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Forward_Arrow_Red", FORWARD_ARROW_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Forward_Arrow_Green", FORWARD_ARROW_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Forward_Arrow_Blue", FORWARD_ARROW_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Right_Red", UPPER_RIGHT_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Right_Green", UPPER_RIGHT_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Upper_Right_Blue", UPPER_RIGHT_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Left_Arrow_Red", LEFT_ARROW_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Left_Arrow_Green", LEFT_ARROW_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Left_Arrow_Blue", LEFT_ARROW_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Center_Red", CENTER_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Center_Green", CENTER_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Center_Blue", CENTER_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Right_Arrow_Red", RIGHT_ARROW_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Right_Arrow_Green", RIGHT_ARROW_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Right_Arrow_Blue", RIGHT_ARROW_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Left_Red", LOWER_LEFT_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Left_Green", LOWER_LEFT_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Left_Blue", LOWER_LEFT_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Back_Arrow_Red", BACK_ARROW_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Back_Arrow_Green", BACK_ARROW_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Back_Arrow_Blue", BACK_ARROW_B);
        
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Right_Red", LOWER_RIGHT_R);
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Right_Green", LOWER_RIGHT_G);
        mpAVRDisplay->AddAnalogOutput("Disp_Lower_Right_Blue", LOWER_RIGHT_B);
        //mpAVRDisplay->SetAnalogOut();
        // Passes MCUInterface to Appliance and registers the callback
        
        mpAVRDisplay->SetMCUInterface(mpDisplay);
        // Passes the Appliance to ZMCU and starts the thread
        if(!mpZMCU->AddMCU(mpAVRDisplay))
        {
            //std::cout << "hello" << std::endl;
            std::cout << "Could not create microcontroller interface to " << mAVRDisplayInfo.mName << "!\n";
            return false;
        }
        
        if(!mpDisplay->Initialize(mAVRDisplayInfo.mPort, mAVRDisplayInfo.mBaud, false, CxUtils::CX_SERIAL_NO_PARITY))
        {
            std::cout << "Could not create microcontroller interface to " << mAVRDisplayInfo.mName << "!\n";
            return false;
        }
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
    //result &= mVectorDriverComponent.Initialize(mVectorDriverComponentID);
    //result &= mChallengeComponent.Initialize(mChallengeComponentID);
    
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
    if(mpCompass)
    {
        mpCompass->Shutdown();
    }
    if(mpDepthSensor)
    {
        mpDepthSensor->Shutdown();
    }
    if(mpLaser)
    {
        mpLaser->Shutdown();
    }
    if(mpAVRPower)
    {
        mpAVRPower->Shutdown();
    }
    if(mpPower)
    {
        mpPower->Shutdown();
    }
    if(mpMotorOne)
    {
        mpMotorOne->Shutdown();
    }
    if(mpMotorTwo)
    {
        mpMotorTwo->Shutdown();
    }
    if(mpAVRSensor)
    {
        mpAVRSensor->Shutdown();
    }
    if(mpSensor)
    {
        mpSensor->Shutdown();
    }
    if(mpAVRPeripheral)
    {
        mpAVRPeripheral->Shutdown();
    }
    if(mpPeripheral)
    {
        mpPeripheral->Shutdown();
    }
    if(mpAVRDisplay)
    {
        mpAVRDisplay->Shutdown();
    }
    if(mpDisplay)
    {
        mpDisplay->Shutdown();
    }
    /*mForwardCamera.Stop();
    mDownwardCamera.Stop();*/
    //mpStereoCamera.Stop();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutdown JAUS.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::ShutdownJAUS()
{
    mBaselineComponent.Shutdown();
    //mVectorDriverComponent.Shutdown();
    mChallengeComponent.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Print status of class
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::PrintStatus()
{
    if(mpPrimitiveDriver)
    {
        mpPrimitiveDriver->PrintStatus();
    }
    if(mpVelocityStateDriver)
    {
        mpVelocityStateDriver->PrintStatus();
    }
    if(mpGlobalPoseSensor)
    {
        mpGlobalPoseSensor->PrintStatus();
    }
    if(mpZMCU)
    {
        mpZMCU->PrintStatus();
    }
    if(mpVelocityStateSensor)
    {
        mpVelocityStateSensor->PrintStatus();
    }
    if(mpVisualSensor)
    {
        mpVisualSensor->PrintStatus();
    }
    if(mpRangeSensor)
    {
        mpRangeSensor->PrintStatus();
    }
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
