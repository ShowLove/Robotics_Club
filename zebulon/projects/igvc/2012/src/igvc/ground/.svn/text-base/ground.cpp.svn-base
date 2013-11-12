////////////////////////////////////////////////////////////////////////////////////
///
/// \file ground.cpp
/// \brief Class containing all data for running the Ground Vehicle 
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
#include "igvc/ground/ground.h"
#include "igvc/baseline/microcontroller.h"
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <iostream>


using namespace Zebulon;
using namespace IGVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Ground::Ground()
{
    mpGlobalInfo = NULL;
    mpGlobalCommand = NULL;
    mLoggingEnabledFlag = true;
    mSimulateFromLogFlag = false;
    mLogDataDelayMs = mImageLogDelayMs = mLaserLogDelayMs = 250;
    //mpStateMachine = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Ground::~Ground()
{
    ShutdownJAUS();
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
bool Ground::LoadSettings(const std::string& xmlFilename)
{
    if(mXmlConfig.Load(xmlFilename, "Ground") == false)
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
    mGroundComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                    str);
                                                                            
    mXmlConfig.GetVar("Components.Ground@component", component);
    mGroundComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConfig.GetVar("Components.Ground@identification", str))
    {
        mGroundComponent.DiscoveryService()->SetComponentIdentification(str);
    }

    mXmlConfig.GetVar("Components.Microcontroller@component", component);
    mXmlConfig.GetVar("Components.Microcontroller@node", node);
    mMicrocontrollerComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    
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
    
    mXmlConfig.GetVar("SimulateFromLog@enabled", mSimulateFromLogFlag, true);
    mXmlConfig.GetVar("SimulateFromLog.Log@folder", mSimulationLogName, true);
    str = std::string();
    mXmlConfig.GetVar("SimulateFromLog.Log@start_time", str, true);
    mSimulationLogStartTime.FromString(str);
    mLogReader.SetLogStartTime(mSimulationLogStartTime);

    bool turnOnDisplay = true;
    if(mXmlConfig.GetVar("Display@enabled", turnOnDisplay, true))
    {
        Utility::Display::SetDisplay(turnOnDisplay);
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool Ground::SetupJAUS()
{
    bool result = true;
    // Create sychronizers to maintain data and keep us
    // perfectly up to date on global pose and velocity data.
    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    mGroundComponent.AddService(globalPoseSensor);
    globalPoseSensor->SynchronizeToComponent(mGlobalPoseSensorComponentID);
    
    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    mGroundComponent.AddService(velocityStateSensor);
    velocityStateSensor->SynchronizeToComponent(mVelocityStateSensorComponentID);
    
    IGVC::Microcontroller* microcontrollerService = new IGVC::Microcontroller();
    mGroundComponent.AddService(microcontrollerService);
    microcontrollerService->SynchronizeToComponent(mMicrocontrollerComponentID);

    mGroundComponent.AddService(new JAUS::VideoSubscriber());
    mGroundComponent.AddService(new JAUS::RangeSubscriber());
    
    // Add Global Info
    mpGlobalInfo = new GlobalInfo();
    mGroundComponent.AddService(mpGlobalInfo);
    mpGlobalInfo->LoadSettings(mXmlConfig.GetPath());
    // Map camera ID's to desired names.
    mpGlobalInfo->SetCameraName(0, GlobalInfo::Camera);
    // Map range sensor ID's to desired names.
    mpGlobalInfo->SetRangeSensorName(0, GlobalInfo::Laser);
    // Enable logging.
    mpGlobalInfo->EnableLogging(mLoggingEnabledFlag);
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
    mGroundComponent.AddService(mpGlobalCommand);
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool Ground::InitializeJAUS()
{
    bool result = false;
    
    // If we are simulating, we do not need to
    // initialize any JAUS components.
    if(mSimulateFromLogFlag)
    {
        return true;
    }
    
    result = mGroundComponent.Initialize(mGroundComponentID);
    if(result)
    {
        // Create subscriptions.
        for(unsigned int i = 0; i < 2; i++)
        {
            if(mpGlobalInfo->CreateSubscriptions())
            {
                result = true;
                break;
            }
            result = false;
        }
        
        for(unsigned int i = 0; i < 2; i++)
        {
            if(mpGlobalInfo->VerifyInitialization())
            {
                result = true;
                break;
            }
            CxUtils::SleepMs(100);
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
bool Ground::Start()
{
    bool result = false;
    
    mpGlobalCommand->Initialize(mpGlobalInfo);
    
    // If we are simulating, we do not need to
    // initialize any JAUS components.
    if(mSimulateFromLogFlag)
    {
        mLogReader.Initialize(mSimulationLogName);

        //return true;
    }
    
    //mpStateMachine = new IGVC::StateMachine();
    //mpStateMachine->Initialize(mpGlobalInfo, mpGlobalCommand, "settings/ground.xml");
    
    result = true;
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutdown all JAUS services and components.
///
////////////////////////////////////////////////////////////////////////////////////
void Ground::ShutdownJAUS()
{
    mGroundComponent.Shutdown();
}




/*  End of File */
