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
#include <mcu/messages/setanalogoutputs.h>
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
    mpPrimitiveDriver = NULL;
    mpVisualSensor = NULL;
    mpVelocityStateSensor = NULL;
    mpRangeSensor = NULL;
    mpVelocityStateDriver = NULL;
    mpMicroInterface = NULL;
    mpJAUSMicro = NULL;
    mpGX3 = NULL;
    mpRaytekInterface = NULL;
    mpDynamixelInterface = NULL;
    mLogFlag = false;
    mJAUSSettingsXML = "settings/services.xml";
    mPoseFileStream = NULL;
    mVelocityFileStream = NULL;
    mMaxDynamixel = 150; // Tilt down
    mMinDynamixel = 100; // Tilt up
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
    if(mpGX3)
    {
        delete mpGX3;
    }
    if(mpMicroInterface)
    {
        delete mpMicroInterface;
    }
    if(mpJAUSMicro)
    {
        delete mpJAUSMicro;
    }
    if(mpRaytekInterface)
    {
        delete mpRaytekInterface;
    }
    if(mpDynamixelInterface)
    {
        delete mpDynamixelInterface;
    }

    mpCompass = NULL;
    mpGPS = NULL;
    mpLaser = NULL;
    mpGX3 = NULL;
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
    mXmlConf.GetVar("Devices@sensorlog",mLogFlag);
    if(!mMicrocontrollerInfo.GetInfo(mXmlConf,"Devices.Microcontroller"))
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
    if(mGX3Info.GetInfo(mXmlConf,"Devices.GX3"))
    {
        std::cout << "GX3 settings loaded!" << std::endl;
    }
    else
    {
        return false;
    }
    if(!mRaytekInfo.GetInfo(mXmlConf,"Devices.Raytek"))
    {
        return false;
    }
    if(!mDynamixelInfo.GetInfo(mXmlConf,"Devices.Dynamixel"))
    {
        return false;
    }
    if(!mCameraInfo.GetInfo(mXmlConf,"Devices.Camera"))
    {
        return false;
    }
    // Optional
    mBackupCameraInfo.GetInfo(mXmlConf,"Devices.BackupCamera");

    mMotorControllerAInfo.GetInfo(mXmlConf,"Devices.MotorControllerA");
    if(mMotorControllerAInfo.mEnabledFlag)
    {
        std::cout << "Motorcontroller A settings loaded!" << std::endl;
    }

    mMotorControllerBInfo.GetInfo(mXmlConf,"Devices.MotorControllerB");
    if(mMotorControllerBInfo.mEnabledFlag)
    {
        std::cout << "Motorcontroller B settings loaded!" << std::endl;
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

    if(mMicrocontrollerInfo.mEnabledFlag)
    {
        mpJAUSMicro = new Microcontroller();
        mLocalWaypointListDriverComponent.AddService(mpJAUSMicro);
    }

    if(mGX3Info.mEnabledFlag || mCompassInfo.mEnabledFlag || mGpsInfo.mEnabledFlag)
    {
        mpVelocityStateSensor = new VelocityStateSensor();
        //mpVelocityStateSensor->EnableDebugMessages(true);
        mLocalWaypointListDriverComponent.AddService(mpVelocityStateSensor);

        // Create closed-loop controller.
        mpVelocityStateDriver = new VelocityStateDriver();
        mVectorDriverComponent.AddService(mpVelocityStateDriver);
        mpVelocityStateDriver->EnableDebugMessages(true);
        mpVelocityStateDriver->SetVelocityStateSensor(mpVelocityStateSensor);

        mpGlobalPoseSensor = new GlobalPoseSensor();
        mpGlobalPoseSensor->LoadSettings(mXmlConf.GetPath());
        mBaselineComponent.AddService(mpGlobalPoseSensor);
        if(mGX3Info.mEnabledFlag || mGpsInfo.mEnabledFlag)
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

    if(mCameraInfo.mEnabledFlag || mBackupCameraInfo.mEnabledFlag)
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
        mpCompass->RegisterCallback((Zebulon::Compass::Callback*)mpGlobalPoseSensor->GetCallback(mpGlobalPoseSensor->Compass));
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
        mpGPS->RegisterCallback(mpGlobalPoseSensor->GetCallback(mpGlobalPoseSensor->DGPS));
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
        if(!mCamera.Start(mCameraInfo.mPort, mCameraInfo.mWidth, mCameraInfo.mHeight, true, true))
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

    if(mBackupCameraInfo.mEnabledFlag)
    {
        mBackupCamera.RegisterCallback(mpVisualSensor );
        mBackupCamera.SetID(mBackupCameraInfo.mID);
        if(!mBackupCamera.Start(mBackupCameraInfo.mPort,
                          mBackupCameraInfo.mWidth,
                          mBackupCameraInfo.mHeight,
                          mBackupCameraInfo.mInterlace > 0 ? true : false,
                          mBackupCameraInfo.mDefaultLib > 0 ? true : false))
        {
            errorMessage.append("Could not connect to camera " + mBackupCameraInfo.mName + "!\n");
            return false;
        }

        //  Wait for both cameras to have a frame before continuing.
        for(unsigned int i = 0; i < 5000; i++)
        {
            if(mBackupCamera.IsReady())
                break;

            CxUtils::SleepMs(1);
        }
        if(mBackupCamera.IsReady() == false)
        {
            errorMessage.append("Camera failed to get to \"Ready\" state " + mBackupCameraInfo.mName + "!\n");
            return false;
        }
    }

    if(mMotorControllerAInfo.mEnabledFlag)
    {
        if(mMotorControlA.Initialize(mMotorControllerAInfo.mPort, 0) == false)
        {
            bool init = false;
            for(int i = 0; i < 5; i++)
            {
                if(mMotorControlA.Initialize(mMotorControllerAInfo.mPort, 0))
                {
                    init = true;
                    break;
                }
            }
            if(init == false)
            {
                errorMessage.append("Unable to initialize Motor Controller A\n");
                return false;
            }
        }
    }

    if(mMotorControllerBInfo.mEnabledFlag)
    {
        if(mMotorControlB.Initialize(mMotorControllerBInfo.mPort, 1) == false)
        {
            bool init = false;
            for(int i = 0; i < 5; i++)
            {
                if(mMotorControlB.Initialize(mMotorControllerBInfo.mPort, 1))
                {
                    init = true;
                    break;
                }
            }
            if(init == false)
            {
                errorMessage.append("Unable to initialize Motor Controller B\n");
                return false;
            }
        }
    }

    if (mMicrocontrollerInfo.mEnabledFlag)
    {
        if(mpMicroInterface == NULL)
        {
            mpMicroInterface = new Zebulon::MCU::SlipInterface();
            mpMicroInterface->RegisterCallback(mpJAUSMicro);
        }
        if(!mpMicroInterface->Initialize(mMicrocontrollerInfo.mPort, mMicrocontrollerInfo.mBaud))
        {
            errorMessage.append("Unable to initialize MicroController\n");
            return false;
        }
    }

    if (mRaytekInfo.mEnabledFlag)
    {
        if(mpRaytekInterface == NULL)
        {
            mpRaytekInterface = new Zebulon::Raytek::Interface();
        }
        if(!mpRaytekInterface->Initialize(mRaytekInfo.mPort, mRaytekInfo.mBaud))
        {
            errorMessage.append("Unable to initialize Raytek\n");
            return false;
        }
    }

    if (mDynamixelInfo.mEnabledFlag)
    {
        if(mpDynamixelInterface == NULL)
        {
            mpDynamixelInterface = new Zebulon::Motor::Dynamixel();
        }
        if(!mpDynamixelInterface->Initialize(mDynamixelInfo.mPort, false, 40.0))
        //if(!mpDynamixelInterface->Initialize("/dev/ttyUSB2", false, 10.0))
        {
            errorMessage.append("Unable to initialize Dynamixel\n");
            return false;
        }
        else
        {
            mpDynamixelInterface->SetServoSpeed(1, 10);
            //mpDynamixelInterface->SetServoSpeed(1, 13.32); // 2 deg/s at 40 Hz for LIDAR
            //mpDynamixelInterface->SetServoSpeed(1, 26.64);
            mpDynamixelInterface->SetTorqueLimit(1, 1023); // Max torque
            mpDynamixelInterface->RegisterCallback(this);
        }
    }
    if(mGX3Info.mEnabledFlag)
    {
        mpGX3 = new Zebulon::INS::MicroStrain::GX3;

        mpGX3->SetDeclination(mCompassInfo.mDeclinationDegrees, mCompassInfo.mDeclinationMinutes);

        if(!mpGX3->Initialize(mGX3Info.mPort, mGX3Info.mBaud,false))
        {
            errorMessage.append("Unable to initialize GX3 controller\n");
            return false;
        }
        mpGX3->RegisterCallback(mpGlobalPoseSensor->GetCallback(mpGlobalPoseSensor->GX3));
        mpGX3->RegisterCallback(mpVelocityStateSensor);
        //mpGX3->ProcessGPSData(jaus gps);
        //mpGX3->ProcessINSData(jaus compass and gps when gps active);
    }

    // Initialize thread for communication with microcontroller
    mBaselineThread.CreateThread(Baseline::BaselineThread, this);
    mBaselineThread.SetThreadName("Basline MC Thread");

    mDynamixelThread.CreateThread(Baseline::DynamixelThread, this);
    mDynamixelThread.SetThreadName("Dynamixel Thread");

    std::string logDirectory;
    std::stringstream fname;

    if(mLogFlag)
    {
        if(mPoseFileStream == NULL)
        {
            std::stringstream dname;
            // Create unique directory name.
            time_t rawtime;
            tm * ptm;

            time ( &rawtime );

            ptm = gmtime ( &rawtime );

            dname << "baselinelogs/"
                  << std::setw(4) << std::setfill('0') << ptm->tm_year + 1900 << "."
                  << std::setw(2) << std::setfill('0') << ptm->tm_mon + 1 << "."
                  << CxUtils::Time::GetUtcTime().ToString(true, true, true, true, false) << "/";
            logDirectory = dname.str();
            CxUtils::FileIO::CreateDir(logDirectory);
            if(mPoseFileStream)
            {
                fclose(mPoseFileStream);
                mPoseFileStream = NULL;
            }
            fname.clear();
            fname.str(std::string());
            fname << dname.str() << "pose_info_log";
            mPoseFileStream = fopen(fname.str().c_str(), "wt");
        }
        if(mVelocityFileStream == NULL)
        {
            std::stringstream dname;
            // Create unique directory name.
            time_t rawtime;
            tm * ptm;

            time ( &rawtime );

            ptm = gmtime ( &rawtime );

            dname << "baselinelogs/"
                  << std::setw(4) << std::setfill('0') << ptm->tm_year + 1900 << "."
                  << std::setw(2) << std::setfill('0') << ptm->tm_mon + 1 << "."
                  << CxUtils::Time::GetUtcTime().ToString(true, true, true, true, false) << "/";
            logDirectory = dname.str();
            CxUtils::FileIO::CreateDir(logDirectory);
            if(mVelocityFileStream)
            {
                fclose(mVelocityFileStream);
                mVelocityFileStream = NULL;
            }
            fname.clear();
            fname.str(std::string());
            fname << dname.str() << "velocity_info_log";
            mVelocityFileStream = fopen(fname.str().c_str(), "wt");
        }
        if(mpGlobalPoseSensor)
        {
            mpGlobalPoseSensor->SetLogFile(mPoseFileStream);
        }
        if(mpVelocityStateSensor)
        {
            mpVelocityStateSensor->SetLogFile(mVelocityFileStream);
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
    result &= mBaselineComponent.Initialize(mBaselineComponentID, 25.0);
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
    mDynamixelThread.StopThread();
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
    if(mpMicroInterface)
    {
        mpMicroInterface->Shutdown();
    }
    if(mpRaytekInterface)
    {
        mpRaytekInterface->Shutdown();
    }
    if(mpDynamixelInterface)
    {
        mpDynamixelInterface->Shutdown();
    }
    if(mCamera.IsReady())
    {
        mCamera.Stop();
    }
    mMotorControlA.Shutdown();
    mMotorControlB.Shutdown();
    //    mCamera.Stop();
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
    if(mPoseFileStream)
    {
        fclose(mPoseFileStream);
        mPoseFileStream = NULL;
    }
    if(mVelocityFileStream)
    {
        fclose(mVelocityFileStream);
        mVelocityFileStream = NULL;
    }
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
    //    //mLocalWaypointListDriverComponent.ManagementService()->PrintStatus();
    //    /*
    if(mpJAUSMicro)
    {
        mpJAUSMicro->PrintStatus();
    }
    //    if(mpVelocityStateSensor)
    //    {
    //        mpVelocityStateSensor->PrintStatus();
    //    }
    //    */
    //    /*
    //    if(mpLocalPoseSensor)
    //    {
    //        mpLocalPoseSensor->PrintStatus();
    //    }
    //    if(mpLocalWaypointListDriver)
    //    {
    //        mpLocalWaypointListDriver->PrintStatus();
    //    }
    //    */
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
    JAUS::Time::Stamp mcuUpdatePeriod = 5;
    CxUtils::SleepMs(100);

    //    //baseline->SetStatus(JAUS::Management::Status::Ready);

    while (baseline &&
           baseline->mBaselineThread.QuitThreadFlag() == false)
    {
        //        // Send motor messages.
        if(JAUS::Time::GetUtcTimeMs() - motorUpdateTimeMs >= motorUpdatePeriod &&
                baseline->mpPrimitiveDriver)
        {
            short frontLeftThrust = 0;
            short frontRightThrust = 0;
            short rearLeftThrust = 0;
            short rearRightThrust = 0;

            frontLeftThrust = baseline->mpPrimitiveDriver->GetMotorThrust(PrimitiveDriver::FrontLeft);
            frontRightThrust = baseline->mpPrimitiveDriver->GetMotorThrust(PrimitiveDriver::FrontRight);
            rearLeftThrust = baseline->mpPrimitiveDriver->GetMotorThrust(PrimitiveDriver::RearLeft);
            rearRightThrust = baseline->mpPrimitiveDriver->GetMotorThrust(PrimitiveDriver::RearRight);

            bool pump = false;
            if(baseline->mpJAUSMicro)
            {
                pump = baseline->mpJAUSMicro->GetDigitalOutputs(Microcontroller::Pump).mValue;
            }

            if(!baseline->mMicrocontrollerInfo.mEnabledFlag)
            {
                if(baseline->mMotorControllerAInfo.mEnabledFlag && baseline->mMotorControlA.IsConnected())
                {
                    baseline->mMotorControlA.SendDualMotorCommand(frontLeftThrust, frontRightThrust);
                }
                if(baseline->mMotorControllerBInfo.mEnabledFlag && baseline->mMotorControlB.IsConnected())
                {
                    baseline->mMotorControlB.SendDualMotorCommand(rearLeftThrust, rearRightThrust);
                }
            }
            else
            {
                frontLeftThrust =  frontLeftThrust * 10;
                frontRightThrust =  (10 *frontRightThrust);
                rearLeftThrust =  (10 *rearLeftThrust);
                rearRightThrust =  (10 *rearRightThrust);
                if(baseline->mpMicroInterface)
                {
                    // Could change to just create on stack so you don't
                    // have to do a delete.
                    MCU::SetMotorOutput * msg = new MCU::SetMotorOutput();
                    msg->mUseStaticFormatFlag = false;
                    msg->mMotors[0] = frontLeftThrust;
                    msg->mMotors[1] = frontRightThrust;
                    msg->mMotors[2] = rearLeftThrust;
                    msg->mMotors[3] = rearRightThrust;
                    if(pump)
                    {
                        msg->mMotors[4] = 1;
                    }
                    else
                    {
                        msg->mMotors[4] = 0;
                    }
                    baseline->mpMicroInterface->Send(msg);
                    delete(msg);
                }
            }


            motorUpdateTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        //        // Send microcontroller commands and query.
        if(JAUS::Time::GetUtcTimeMs() - mcuUpdateTimeMs >= mcuUpdatePeriod &&
                baseline->mpJAUSMicro)
        {
            if(baseline->mRaytekInfo.mEnabledFlag && baseline->mpRaytekInterface)
            {
                double temp = baseline->mpRaytekInterface->GetTemp();
                baseline->mpJAUSMicro->SetAnalogInput(Microcontroller::Temperature,(temp/10.0));
            }

            if(baseline->mLogFlag)
            {
                fflush(baseline->mPoseFileStream);
                fflush(baseline->mVelocityFileStream);
            }
            mcuUpdateTimeMs = JAUS::Time::GetUtcTimeMs();

            CxUtils::SleepMs(1);
        }
    }
}


void Baseline::ProcessServoData(const double pos,const unsigned int id)
{
    if(mpJAUSMicro)
    {
        Microcontroller::PortNumber name = Microcontroller::DynamixelActual;
        double value = CxUtils::CxToRadians(Motor::Dynamixel::ToDegrees(pos));
        mpJAUSMicro->SetAnalogInput(name, value);
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Dedicated thread function for communication with the Dynamixel servo.
///
////////////////////////////////////////////////////////////////////////////////////
void Baseline::DynamixelThread(void *args)
{
    Baseline * baseline  = (Baseline*)args;
    JAUS::Time::Stamp dynamixelUpdatePeriodMs = 20;
    JAUS::Time::Stamp updateTimeMs = 0;
    CxUtils::SleepMs(100);

    while (baseline &&
           baseline->mDynamixelThread.QuitThreadFlag() == false)
    {
        //        // Send microcontroller commands and query.
        if(JAUS::Time::GetUtcTimeMs() - updateTimeMs >= dynamixelUpdatePeriodMs &&
           baseline->mpDynamixelInterface && baseline->mDynamixelInfo.mEnabledFlag &&
           baseline->mpJAUSMicro)
        {
            double degrees = baseline->mpDynamixelInterface->GetServoDegrees(1);
            //baseline->mpJAUSMicro->SetAnalogInput(Microcontroller::DynamixelActual,CxUtils::CxToRadians(degrees));
            double desired = degrees;  // Default.
            if(baseline->mpJAUSMicro->GetComponent()->AccessControlService()->IsControlled() &&
                    baseline->mpJAUSMicro->GetComponent()->ManagementService()->GetStatus() == JAUS::Management::Status::Ready)
            {
                desired = CxUtils::CxToDegrees(baseline->mpJAUSMicro->GetAnalogOutputs(Microcontroller::DynamixelDesired));
                if(desired > baseline->mMaxDynamixel)
                {
                    desired = baseline->mMaxDynamixel;
                }
                if(desired < baseline->mMinDynamixel)
                {
                    desired = baseline->mMinDynamixel;
                }
                baseline->mpDynamixelInterface->SetServoDegrees(1,desired);
            }


            updateTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        CxUtils::SleepMs(1);
    }
}

/*  End of File */
