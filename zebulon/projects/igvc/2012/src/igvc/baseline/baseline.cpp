/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.cpp
/// \brief This is the main file for running the Baseline program for the
///        ground vehicle, which does basic sensor and motor control integration.
///
/// Author(s): David Adams<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
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
#include "igvc/baseline/baseline.h"
#include <jaus/mobility/list/listmanager.h>
#include <cstring>
#include <string.h>

#ifdef WIN32
#define strcasecmp _stricmp
#endif

using namespace Zebulon;
using namespace IGVC;

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
    mpLocalWaypointListDriver = NULL;
    mpMicrocontroller = NULL;
    mpFrontMotor = NULL;
    mpRearMotor = NULL;
    mJAUSSettingsXML = "settings/services.xml";
    mFrontMotorID = 0;
    mRearMotorID = 1;
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
    if(mpFrontMotor)
    {
        delete mpFrontMotor;
    }
    if(mpRearMotor)
    {
        delete mpRearMotor;
    }
    
    mpCompass = NULL;
    mpGPS = NULL;
    mpLaser = NULL;
    mpFrontMotor = NULL;
    mpRearMotor = NULL;
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
    int subsystem = 55;
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

    if(!mARMmiteInfo.GetInfo(mXmlConf,"Devices.Microcontroller"))
    {
        return false;
    }

    if(!mCompassInfo.GetInfo(mXmlConf,"Devices.Compass"))
    {
        return false;
    } 
    if(!mLaserInfo.GetInfo(mXmlConf,"Devices.Laser"))
    {
        return false;
    }
    if(!mFrontMotorInfo.GetInfo(mXmlConf,"Devices.FrontMotor"))
    {
        return false;
    }
    else
    {
        mXmlConf.GetVar("Devices.FrontMotor@id", mFrontMotorID);
    }
    if(!mRearMotorInfo.GetInfo(mXmlConf,"Devices.RearMotor"))
    {
        return false;
    }
    else
    {
        mXmlConf.GetVar("Devices.RearMotor@id", mRearMotorID);
    }
    if(!mCameraInfo.GetInfo(mXmlConf,"Devices.Camera"))
    {
        return false;
    }
    if(!mGpsInfo.GetInfo(mXmlConf,"Devices.GPS"))
    {
        return false;
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

    if(mARMmiteInfo.mEnabledFlag)
    {
        mpMicrocontroller = new Microcontroller();
        mLocalWaypointListDriverComponent.AddService(mpMicrocontroller);
    }
    
    if(mFrontMotorInfo.mEnabledFlag || mRearMotorInfo.mEnabledFlag)
    {
        mpVelocityStateSensor = new VelocityStateSensor();
        //mpVelocityStateSensor->EnableDebugMessages(true);
        mLocalWaypointListDriverComponent.AddService(mpVelocityStateSensor);
        
        // Create closed-loop controller.
        mpVelocityStateDriver = new VelocityStateDriver();
        mVectorDriverComponent.AddService(mpVelocityStateDriver);
        //mpVelocityStateDriver->EnableDebugMessages(true);
        mpVelocityStateDriver->SetVelocityStateSensor(mpVelocityStateSensor);
    }
    
    if(mCompassInfo.mEnabledFlag || mGpsInfo.mEnabledFlag)
    {
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
    
    if(mCameraInfo.mEnabledFlag)
    {
        mpVisualSensor = new VisualSensor();
        mBaselineComponent.AddService(mpVisualSensor);
    }
    // Local Waypoint List Driver services.
    mLocalWaypointListDriverComponent.AddService(new JAUS::ListManager());
    mpLocalWaypointListDriver = new LocalWaypointListDriver();
    mpLocalWaypointListDriver->SetDriverID(mVectorDriverComponentID);
    mLocalWaypointListDriverComponent.AddService(mpLocalWaypointListDriver);

    // Setup components
    mBaselineComponent.LoadSettings(mJAUSSettingsXML);
    mVectorDriverComponent.LoadSettings(mJAUSSettingsXML);
    mLocalWaypointListDriverComponent.LoadSettings(mJAUSSettingsXML);
   
    // Setting timeout for control to 0 (disables timeout of control).
    // This is done because the JVT and OCP do not query for the timeout period
    // and may not send messages to re-acquire/maintain control within the
    // normal 2 second timeout window.  
    mLocalWaypointListDriverComponent.AccessControlService()->SetTimeoutPeriod(0);
    //((JAUS::JUDP*)mLocalWaypointListDriverComponent.TransportService())->AddConnection("192.168.1.42", JAUS::Address(42, 1, 1));
    return true;
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize class, components and devices from xml(calls loadxml first)
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::InitializeHardware(std::string& errorMessage)
{    
    if(mARMmiteInfo.mEnabledFlag)
    {
        if(!mARMmite.Initialize(mARMmiteInfo.mPort, mARMmiteInfo.mBaud))
        {
            errorMessage.append("Unable to initialize ARMmite controller\n");
            return false;
        }
        mARMmite.RegisterCallback(MCU::Message::All, mpMicrocontroller);
    }

    if(mFrontMotorInfo.mEnabledFlag)
    {
        mpFrontMotor = new Motor::Roboteq::AX3500();
        if(!mpFrontMotor->Initialize(mFrontMotorInfo.mPort, mFrontMotorID))
        {
            errorMessage.append("Unable to initialize front motor controller\n");
            return false;
        }
        mpFrontMotor->RegisterCallback(mpVelocityStateSensor);
    }
    
    if(mRearMotorInfo.mEnabledFlag)
    {
        mpRearMotor = new Motor::Roboteq::AX3500();
        if(!mpRearMotor->Initialize(mRearMotorInfo.mPort, mRearMotorID))
        {
            errorMessage.append("Unable to initialize rear motor controller");
            return false;
        }
        mpRearMotor->RegisterCallback(mpVelocityStateSensor);
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
        laserConfig.mMinRange = mpLaser->GetMinimumDistance();
        laserConfig.mAngleIncrement = mpLaser->GetAngularResolution();
        laserConfig.mScanAngle = fabs(mLaserInfo.mLeftLimit) + fabs(mLaserInfo.mRightLimit); //mpLaser->GetScanAngle();
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
    
    // Initialize thread for communication with Roboteq Device
    if (mpFrontMotor || mpRearMotor)
    {
        mRoboteqThread.CreateThread(Baseline::RoboteqThread, this);
        mRoboteqThread.SetThreadName("Roboteq Thread");
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
    result &= mLocalWaypointListDriverComponent.Initialize(mLocalWaypointListDriverComponentID, 25.0);
    /* Deprecated
    JAUS::JUDP* judp = (JAUS::JUDP*)mLocalWaypointListDriverComponent.TransportService();
    if(judp)
    {
        //judp->AddConnection(CxUtils::IP4Address("192.168.1.42"), JAUS::Address(42, 1, 1));
    }
    */
    result &= mBaselineComponent.Initialize(mBaselineComponentID);
    // Vector Driver has higher update frequency for closed loop control operations.
    result &= mVectorDriverComponent.Initialize(mVectorDriverComponentID, 25.0);

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
    mRoboteqThread.StopThread();
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
    if(mpFrontMotor)
    {
        mpFrontMotor->Shutdown();
    }
    if(mpRearMotor)
    {
        mpRearMotor->Shutdown();
    }
    mCamera.Stop();
    mARMmite.Shutdown();
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
    if(mpRangeSensor)
    {
        mpRangeSensor->PrintStatus();
    }
    std::cout << "-------------------------------------------------------" << std::endl;

    mVectorDriverComponent.AccessControlService()->PrintStatus();
    if(mpVelocityStateDriver)
    {
        mpVelocityStateDriver->PrintStatus();
    }

    std::cout << "-------------------------------------------------------" << std::endl;
    mLocalWaypointListDriverComponent.AccessControlService()->PrintStatus();
    mLocalWaypointListDriverComponent.ManagementService()->PrintStatus();

    if(mpMicrocontroller)
    {
        mpMicrocontroller->PrintStatus();
    }
    if(mpVelocityStateSensor)
    {
        mpVelocityStateSensor->PrintStatus();
    }

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
void Baseline::RoboteqThread(void *args)
{
    Baseline * baseline  = (Baseline*)args;
    JAUS::Time::Stamp motorUpdateTimeMs = 0;
    JAUS::Time::Stamp encoderUpdateTimeMs = 0;
    JAUS::Time::Stamp samplePeriodMs = 50;
    while (baseline && 
           baseline->mRoboteqThread.QuitThreadFlag() == false)
    {
        double leftThrust = 0, rightThrust = 0;
        if(baseline->mBaselineComponent.ManagementService()->GetStatus() == JAUS::Management::Status::Ready)
        {
            if(baseline->mpPrimitiveDriver)
            {
                leftThrust = baseline->mpPrimitiveDriver->GetLeftThrust();
                rightThrust = baseline->mpPrimitiveDriver->GetRightThrust();
            }
        }
                
        // Send motor thrust values.
        if(JAUS::Time::GetUtcTimeMs() - motorUpdateTimeMs >= samplePeriodMs)
        {
            /* Testing outputs.
            static double t = 0;
            t += .001;
            leftThrust = rightThrust = sin(t)*100.0;
            */
            /*
            if(leftThrust >= 40 && rightThrust >= 40)
            {
                leftThrust = 100;
                rightThrust = 100;
            }
            */
            if(baseline->mpFrontMotor)
            {
                baseline->mpFrontMotor->SendDualMotorCommand(leftThrust, rightThrust, false);
            }
            if(baseline->mpRearMotor)
            {
                baseline->mpRearMotor->SendDualMotorCommand(leftThrust, rightThrust, false);
            }
            motorUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        
        if(baseline->mpVelocityStateSensor)
        {
            samplePeriodMs = (JAUS::Time::Stamp)baseline->mpVelocityStateSensor->GetParameter(IGVC::VelocityStateSensor::SamplePeriodMs);
            if((JAUS::Time::GetUtcTimeMs() - encoderUpdateTimeMs) >= samplePeriodMs)
            {
                if(baseline->mpFrontMotor)
                {
                    //query roboteq for encoder values
                    //process them as a callback to velocitystatesensor
                    baseline->mpFrontMotor->QueryEncoder(0, true, 0);
                    CxUtils::SleepMs(10);
                    baseline->mpFrontMotor->QueryEncoder(1, true, 0);
                }
                if(baseline->mpRearMotor)
                {
                    baseline->mpRearMotor->QueryEncoder(0, true, 0);
                    CxUtils::SleepMs(10);
                    baseline->mpRearMotor->QueryEncoder(1, true, 0);
                }
                encoderUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
            }
        }
        CxUtils::SleepMs(1);
    }
}

/*  End of File */
