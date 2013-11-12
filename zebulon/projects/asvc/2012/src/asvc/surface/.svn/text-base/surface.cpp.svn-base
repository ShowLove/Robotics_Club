////////////////////////////////////////////////////////////////////////////////////
///
/// \file surface.cpp
/// \brief Class containing all data for running the Surface Vehicle
///        program.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#include "asvc/surface/surface.h"
#include "asvc/surface/globalinfo.h"
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Surface::Surface()
{
    mpGlobalInfo = NULL;
    mpGlobalCommand = NULL;
    mLoggingEnabledFlag = true;
    mSimulateFromLogFlag = false;
    mLogDataDelayMs = mImageLogDelayMs = mLaserLogDelayMs = 250;
    mpStateMachine = new ASVC::StateMachine();
    mCameraSettingsFile = "settings/pylon_settings.txt";
    mEnablePylonCameraFlag = true;
    mCaptureWidth = 640;
    mCaptureHeight = 480;
    mDepthMapEnableFlag = true;
    mpDepthMap = new DepthMap();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Surface::~Surface()
{
    ShutdownJAUS();
    delete mpStateMachine;
    delete mpDepthMap;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads settings from XML.
///
///   \param[in] xmlFilename XML filename with configuration settings.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Surface::LoadSettings(const std::string& xmlFilename)
{
    if(mXmlConfig.Load(xmlFilename, "Surface") == false)
    {
        std::cout << "Failed to load XML configuration file.\n";
        return false;
    }

    int subsystem = 55;
    int node = 1;
    int component = 1;
    std::string str;

    mXmlConfig.GetVar("Components@subsystem", subsystem);
    mXmlConfig.GetVar("Components@node", node);
    mXmlConfig.GetVar("Components@identification", str);
    mSurfaceComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                    str);

    mXmlConfig.GetVar("Components.Surface@component", component);
    mSurfaceComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConfig.GetVar("Components.Surface@identification", str))
    {
        mSurfaceComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    mXmlConfig.GetVar("Components.GlobalPoseSensor@component", component);
    mXmlConfig.GetVar("Components.GlobalPoseSensor@node", node);
    mGlobalPoseSensorComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);

    mXmlConfig.GetVar("Components.VelocityStateSensor@component", component);
    mXmlConfig.GetVar("Components.VelocityStateSensor@node", node);
    mVelocityStateSensorComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);

    mXmlConfig.GetVar("Components.VelocityStateDriver@component", component);
    mXmlConfig.GetVar("Components.VelocityStateDriver@node", node);
    mVelocityStateDriverComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    component = node = 0;
    mXmlConfig.GetVar("Components.PrimitiveDriver@component", component);
    mXmlConfig.GetVar("Components.PrimitiveDriver@node", node);
    mPrimitiveDriverComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);

    component = node = 0;
    mXmlConfig.GetVar("Components.Microcontroller@component", component);
    mXmlConfig.GetVar("Components.Microcontroller@node", node);
    mMicrocontrollerComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);

    std::string pylon = "true";
    if(mXmlConfig.GetVar("Pylon@on", pylon, true))
    {
        if(pylon == "true" || pylon == "1" || pylon == "TRUE" || pylon == "yes")
        {
            mEnablePylonCameraFlag = true;
        }
        else
        {
            mEnablePylonCameraFlag = false;
        }
    }
    if(mXmlConfig.GetVar("Pylon@settings", pylon, true))
    {
        mCameraSettingsFile = pylon;
    }
    mXmlConfig.GetVar("Pylon@width", mCaptureWidth, true);
    mXmlConfig.GetVar("Pylon@height", mCaptureHeight, true);

    unsigned int logDelayMs = 0;
    mXmlConfig.GetVar("Logging@enabled", mLoggingEnabledFlag, true);
    if(mXmlConfig.GetVar("Logging@data_delay_ms", logDelayMs, true))
    {
        mLogDataDelayMs = (CxUtils::Time::Stamp)logDelayMs;
    }
    if(mXmlConfig.GetVar("Logging@image_delay_ms", logDelayMs, true))
    {
        mImageLogDelayMs = (CxUtils::Time::Stamp)logDelayMs;
    }
    if(mXmlConfig.GetVar("Logging@laser_delay_ms", logDelayMs, true))
    {
        mLaserLogDelayMs = (CxUtils::Time::Stamp)logDelayMs;
    }
    std::string onChangeValues;
    if(mXmlConfig.GetVar("Logging@on_change", onChangeValues, true))
    {
        mOnChangeVariables = CxUtils::FileIO::Tokenize(onChangeValues, ",");
    }
    mMaxVelocity = 4;
    mXmlConfig.GetVar("MaxVelocity", mMaxVelocity);

    mXmlConfig.GetVar("SimulateFromLog@enabled", mSimulateFromLogFlag, true);
    mXmlConfig.GetVar("SimulateFromLog.Log@folder", mSimulationLogName, true);
    int enableLaserPlayback = 0;
    if(mXmlConfig.GetVar("SimulateFromLog@laser_enabled", enableLaserPlayback, true))
    {
        mLogReader.DisablePointPlayback(enableLaserPlayback > 0 ? false : true);
    }
    int playbackRate = 1;
    if(mXmlConfig.GetVar("SimulateFromLog@rate", playbackRate, true))
    {
        mLogReader.SetPlaybackRate(playbackRate);
    }
    str = std::string();
    mXmlConfig.GetVar("SimulateFromLog.Log@start_time", str, true);
    mSimulationLogStartTime.FromString(str);
    mLogReader.SetLogStartTime(mSimulationLogStartTime);
    bool turnOnDisplay = true;
    if(mXmlConfig.GetVar("Display@enabled", turnOnDisplay, true))
    {
        Utility::Display::SetDisplay(turnOnDisplay);
    }

    TiXmlDocument document;
    document.LoadFile(xmlFilename.c_str());
    TiXmlHandle handle(&document);
    TiXmlElement* missionToLoad = handle.FirstChild("Surface").FirstChild("MissionsToRun").FirstChild("Mission").ToElement();
    while(missionToLoad)
    {
        if(missionToLoad->FirstChild())
        {
            mpStateMachine->mMissionsToRun.push_back(missionToLoad->FirstChild()->Value());
        }
        missionToLoad = missionToLoad->NextSiblingElement("Mission");
    }

    return true;
}


/** Intializes any hardware not setup by Baseline application. */
bool Surface::InitializeHardware()
{
    bool result = true;

    if(mEnablePylonCameraFlag && !mSimulateFromLogFlag)
    {
        // Set settings file data.
        mBaslerCamera.SetSettingsFile(mCameraSettingsFile);
        // Register the callback before initialization.
        mBaslerCamera.RegisterCallback(mpGlobalInfo);
        // Try start capture.
        if(mBaslerCamera.Start("pylon-0", mCaptureWidth, mCaptureHeight))
        {
            result = true;
            // Success, get the interface pointers and save.
            mpPylonCamera = (Video::ImageCapturePylon*)mBaslerCamera.GetRawCapture();
            mpGlobalInfo->SetPylonCamera(mpPylonCamera);
        }
        else
        {
            result = false;
        }
    }

    if(mDepthMapEnableFlag)
    {
        mpGlobalInfo->RegisterCallback(Microcontroller::DYNAMIXELACTUAL, mpDepthMap);
        mpGlobalInfo->RegisterPointCallback(mpDepthMap);
        mpGlobalInfo->SetDepthMap(mpDepthMap);
        mpDepthMap->Initialize("settings/intrinsics.xml", "settings/distortion.xml",
                             mCaptureWidth, mCaptureHeight,
                             mpGlobalCommand, mpGlobalInfo);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool Surface::SetupJAUS()
{
    bool result = true;
    // Create sychronizers to maintain data and keep us
    // perfectly up to date on global pose and velocity data.
    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    mSurfaceComponent.AddService(globalPoseSensor);
    globalPoseSensor->SynchronizeToComponent(mGlobalPoseSensorComponentID);

    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    mSurfaceComponent.AddService(velocityStateSensor);
    velocityStateSensor->SynchronizeToComponent(mVelocityStateSensorComponentID);

    ASVC::Microcontroller* microcontrollerService = new ASVC::Microcontroller();
    mSurfaceComponent.AddService(microcontrollerService);
    microcontrollerService->SynchronizeToComponent(mMicrocontrollerComponentID);


    mSurfaceComponent.AddService(new JAUS::VideoSubscriber());
    mSurfaceComponent.AddService(new JAUS::RangeSubscriber());

    // Add Global Info
    mpGlobalInfo = new GlobalInfo();
    mSurfaceComponent.AddService(mpGlobalInfo);
    mpGlobalInfo->LoadSettings(mXmlConfig.GetPath());
    // Map camera ID's to desired names.
    mpGlobalInfo->SetCameraName(0, GlobalInfo::MainCamera);
    // Map camera ID's to desired names.
    mpGlobalInfo->SetCameraName(1, GlobalInfo::BackupCamera);
    // Map range sensor ID's to desired names.
    mpGlobalInfo->SetRangeSensorName(0, GlobalInfo::Laser);
    // Enable logging.
    mpGlobalInfo->EnableLogging(mLoggingEnabledFlag,
                                mLogDataDelayMs,
                                mImageLogDelayMs,
                                mLaserLogDelayMs);
    std::vector<std::string>::iterator onChange;
    for(onChange = mOnChangeVariables.begin();
        onChange != mOnChangeVariables.end();
        onChange++)
    {
        mpGlobalInfo->LogVariableOnChange(*onChange);
    }
    // Save pointer to log reader.mSimulateFromLogFlag
    if(mSimulateFromLogFlag)
    {
        mLogReader.SetGlobalInfo(mpGlobalInfo);
    }

    // Add Global Command
    mpGlobalCommand = new GlobalCommand();
    mpGlobalCommand->LoadSettings(mXmlConfig.GetPath());
    mpGlobalCommand->SetDrivers(mPrimitiveDriverComponentID,
                                mVelocityStateDriverComponentID);
    mpGlobalCommand->SetMaximumVelocity(mMaxVelocity);
    mpGlobalCommand->SetMicrocontroller(mMicrocontrollerComponentID);
    mSurfaceComponent.AddService(mpGlobalCommand);

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool Surface::InitializeJAUS()
{
    bool result = false;

    // If we are simulating, we do not need to
    // initialize any JAUS components.
    if(mSimulateFromLogFlag)
    {
        return true;
    }

    result = mSurfaceComponent.Initialize(mSurfaceComponentID);

    if(result)
    {
        mSurfaceComponent.ManagementService()->SetStatus(JAUS::Management::Status::Ready);
        // Create subscriptions.
        for(unsigned int i = 0; i < 3; i++)
        {
            if(mpGlobalInfo->CreateSubscriptions())
            {
                result = true;
                break;
            }
            CxUtils::SleepMs(500);
            result = false;
        }

        for(unsigned int i = 0; i < 3; i++)
        {
            if(mpGlobalInfo->VerifyInitialization())
            {
                result = true;
                break;
            }
            CxUtils::SleepMs(500);
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Starts the program.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Surface::Start()
{
    bool result = false;

    mpGlobalCommand->Initialize(mpGlobalInfo);

    // If we are simulating, we do not need to
    // initialize any JAUS components.
    if(mSimulateFromLogFlag)
    {
        mLogReader.Initialize(mSimulationLogName);
        CxUtils::SleepMs(1000); // Allow time for logs to "kick-in"
    }

    mpStateMachine->Initialize(mpGlobalInfo, mpGlobalCommand, mXmlConfig.GetPath());

    result = true;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutdown all JAUS services and components.
///
////////////////////////////////////////////////////////////////////////////////////
void Surface::ShutdownJAUS()
{
    mSurfaceComponent.Shutdown();
}




/*  End of File */
