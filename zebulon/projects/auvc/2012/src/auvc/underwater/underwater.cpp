bool gDisplayHydroFlag = false;
bool gLogData = false;                   // Turns on data logging
bool gPlaybackLogData = true;          // Playsback data from log to simulate a run
//#define USE_APL
//#define GRAPHING

/////////////////////////////////////////////////////////////////////////////////////
///
/// \file underwater.cpp
/// \brief This is the main file the underwater vehicle.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 4/08/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
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
#include "auvc/underwater/underwater.h"
#include "auvc/baseline/baseline.h"
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <iostream>


using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UnderwaterVehicle::UnderwaterVehicle()
{
    mpGlobalInfo = NULL;
    mpGlobalCommand = NULL;
    mLoggingEnabledFlag = true;
    mSimulateFromLogFlag = false;
    mLogDataDelayMs = mImageLogDelayMs = mLaserLogDelayMs = 250;
    mpForwardCameraRaw = NULL;
    mpDownwardCameraRaw = NULL;
    //mpStateMachine = NULL;
    //mpAPL = NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UnderwaterVehicle::~UnderwaterVehicle()
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
bool UnderwaterVehicle::LoadSettings(const std::string& xmlFilename)
{
    if(mXmlConfig.Load(xmlFilename, "Underwater") == false)
    {
        std::cout << "Failed to load XML configuration file.\n";
        return false;
    }
    mXMLFilename = xmlFilename;
    int subsystem = 20;
    int node = 1;
    int component = 1;
    std::string str;
    
    mXmlConfig.GetVar("Components@subsystem", subsystem);
    mXmlConfig.GetVar("Components@node", node);
    mXmlConfig.GetVar("Components@identification", str);
    
    std::cout << "subsystem: : " << subsystem << std::endl;
    std::cout << "node: : " << node << std::endl;
    std::cout << "str: : " << str << std::endl;
    mUnderwaterComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                        str);

    mXmlConfig.GetVar("Components.Underwater@component", component);
    std::cout << "component: : " << component << std::endl;
    mUnderwaterComponentID = JAUS::Address((JAUS::UShort)subsystem, (JAUS::Byte)node, (JAUS::Byte)component);
    if(mXmlConfig.GetVar("Components.Underwater@identification", str))
    {
        mUnderwaterComponent.DiscoveryService()->SetComponentIdentification(str);
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

    bool turnOnDisplay = false;
    Utility::Display::SetDisplay(turnOnDisplay);
    double scaleDisplay= 1.0;
    if(mXmlConfig.GetVar("Display@enabled", turnOnDisplay, true))
    {
        Utility::Display::SetDisplay(turnOnDisplay);
        mXmlConfig.GetVar("Display@scale", scaleDisplay, true);
        Utility::Display::SetScale(scaleDisplay);
        
    }
    
    mEnableAPL=false;
    mXmlConfig.GetVar("APL@enabled", mEnableAPL, false);



    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool UnderwaterVehicle::SetupJAUS()
{
    bool result = true;
    // Create sychronizers to maintain data and keep us
    // perfectly up to date on global pose and velocity data.
    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    mUnderwaterComponent.AddService(globalPoseSensor);
    globalPoseSensor->SynchronizeToComponent(mGlobalPoseSensorComponentID);
    
    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    mUnderwaterComponent.AddService(velocityStateSensor);
    velocityStateSensor->SynchronizeToComponent(mVelocityStateSensorComponentID);
    
    Utility::Microcontroller* microcontrollerService = new Utility::Microcontroller();
    mUnderwaterComponent.AddService(microcontrollerService);
    microcontrollerService->SynchronizeToComponent(mMicrocontrollerComponentID);

    mUnderwaterComponent.AddService(new JAUS::VideoSubscriber());
    mUnderwaterComponent.AddService(new JAUS::RangeSubscriber());
    
    // Add Global Info
    mpGlobalInfo = new GlobalInfo();
    mUnderwaterComponent.AddService(mpGlobalInfo);
    mpGlobalInfo->LoadSettings(mXmlConfig.GetPath());
    // Map camera ID's to desired names.
    mpGlobalInfo->SetCameraName(1, GlobalInfo::ForwardCamera);
    mpGlobalInfo->SetCameraName(0, GlobalInfo::DownwardCamera);
    // Map range sensor ID's to desired names.
    mpGlobalInfo->SetRangeSensorName(0, GlobalInfo::Laser);
    // Enable logging.
    mpGlobalInfo->EnableLogging(mLoggingEnabledFlag);
    // Save pointer to log reader.mSimulateFromLogFlag
    if(mSimulateFromLogFlag)
    {
        mLogReader.SetGlobalInfo(mpGlobalInfo);
        mLogReader.SetLogStartTime(mSimulationLogStartTime);
    }
    
    // Add Global Command
    mpGlobalCommand = new GlobalCommand();
    mpGlobalCommand->LoadSettings(mXmlConfig.GetPath());
    mpGlobalCommand->SetDrivers(mPrimitiveDriverComponentID,
                                mVelocityStateDriverComponentID);
    mUnderwaterComponent.AddService(mpGlobalCommand);
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize all JAUS services.
///
////////////////////////////////////////////////////////////////////////////////////
bool UnderwaterVehicle::InitializeJAUS()
{
    bool result = false;
    
    // If we are simulating, we do not need to
    // initialize any JAUS components.
    if(mSimulateFromLogFlag)
    {
        return true;
    }
    
    result = mUnderwaterComponent.Initialize(mUnderwaterComponentID);
    //std::cout << "STUFFFFFFFFFFFFFFFFFFFFFF" << std::endl;
    if(result)
    {
        //std::cout << "IM IN HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEE" << std::endl;
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

void UnderwaterVehicle::SetupRawCamera(std::string name,fw1394Camera *camera,Configuration::XmlConfigurator &CameraXmlConfig)
{
    int Shutter,Red,Blue;
    int HW,SW,Debug,Avg;
    double P;

    bool ReSizeFlag;
    int NewWidth,NewHeight;

    CameraXmlConfig.GetVar("CameraReSize@resize", ReSizeFlag);
    CameraXmlConfig.GetVar("CameraReSize@width", NewWidth);
    CameraXmlConfig.GetVar("CameraReSize@height", NewHeight);
    mpGlobalInfo->mCameraCallback->SetReSizeInfo(ReSizeFlag,NewWidth,NewHeight);

    CameraXmlConfig.GetVar("Devices."+name+".Auto@HW", HW);
    CameraXmlConfig.GetVar("Devices."+name+".Auto@SW", SW);
    CameraXmlConfig.GetVar("Devices."+name+".Auto@Debug", Debug);
    CameraXmlConfig.GetVar("Devices."+name+".Auto@Avg", Avg);
    CameraXmlConfig.GetVar("Devices."+name+".Auto@P", P);

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
        CameraXmlConfig.GetVar("Devices."+name+".Shutter@Value", Shutter);
        CameraXmlConfig.GetVar("Devices."+name+".WhiteBalance@Red", Red);
        CameraXmlConfig.GetVar("Devices."+name+".WhiteBalance@Blue", Blue);
        camera->SetShutter(Shutter);
        camera->SetWhiteBalance(Red,Blue);
        std::cout << " Shutter: " << Shutter
        << " Red: " << Red
        << " Blue: " << Blue
        << std::endl;
    }


}
bool UnderwaterVehicle::InitializeHardware()
{
    bool result = true;
    if(!mSimulateFromLogFlag)
    {
        //Identification
        int width,height,id;
        string port;
        bool enabled;

        //Hacky, but open baseline XML for Underwater raw camera
        Configuration::XmlConfigurator CameraXmlConfig;  ///<  XML config data.
        if(CameraXmlConfig.Load(mXMLFilename, "Camera") == false)
        {
            std::cout << "Failed to load XML configuration file.\n";
            return false;
        }

        //type="CvImageCapture"   port="/dev/video1394/1" id="1"  width="800" height="600" interlace="0" default_lib="1" >

        std::string ForwardName="ForwardCamera";
        CameraXmlConfig.GetVar("Devices."+ForwardName+"@port", port);
        CameraXmlConfig.GetVar("Devices."+ForwardName+"@width", width);
        CameraXmlConfig.GetVar("Devices."+ForwardName+"@height", height);
        CameraXmlConfig.GetVar("Devices."+ForwardName+"@id", id);
        CameraXmlConfig.GetVar("Devices."+ForwardName+"@enabled", enabled);

        if(enabled && !mSimulateFromLogFlag)
        {

            //Callbacks registered before start called
            //mForwardCamera.RegisterCallback(mpGlobalInfo->mCameraCallback);
            mForwardCamera.SetID(id);
            if(!mForwardCamera.Start(port,width,height,false, true))
            {
                cout << "Could not connect to camera " + ForwardName + "!\n";
                return false;
            }
            else
            {
                cout << "Camera started: " + ForwardName + "!\n";
            }
            mpForwardCameraRaw=(fw1394Camera *)mForwardCamera.GetRawCapture();
            SetupRawCamera(ForwardName,mpForwardCameraRaw,CameraXmlConfig);

            //  Wait for both cameras to have a frame before continuing.
            for(unsigned int i = 0; i < 5000; i++)
            {
                if(mForwardCamera.IsReady())
                    break;

                CxUtils::SleepMs(1);
            }
            if(mForwardCamera.IsReady() == false)
            {
                cout << "Camera failed to get to \"Ready\" state " + ForwardName + "!\n";
                result &= false;
            }
            mpGlobalInfo->mpForwardCameraRaw = mpForwardCameraRaw;
        }


        //type="CvImageCapture"   port="/dev/video1394/0" id="0"  width="800" height="600" interlace="0" default_lib="1" >
        std::string DownwardName="DownwardCamera";
        CameraXmlConfig.GetVar("Devices."+DownwardName+"@port", port);
        CameraXmlConfig.GetVar("Devices."+DownwardName+"@width", width);
        CameraXmlConfig.GetVar("Devices."+DownwardName+"@height", height);
        CameraXmlConfig.GetVar("Devices."+DownwardName+"@id", id);
        CameraXmlConfig.GetVar("Devices."+DownwardName+"@enabled", enabled);
        if(enabled)
        {
            //Callbacks registered before start called
            //mDownwardCamera.RegisterCallback(mpGlobalInfo->mCameraCallback);
            mDownwardCamera.SetID(id);

            if(!mDownwardCamera.Start(port,width,height,false, true))
            {
                cout << "Could not connect to camera " + DownwardName + "!\n";
                return false;
            }
            else
            {
                cout << "Camera started: " + DownwardName + "!\n";
            }

            mpDownwardCameraRaw=(fw1394Camera *)mDownwardCamera.GetRawCapture();
            SetupRawCamera(DownwardName,mpDownwardCameraRaw,CameraXmlConfig);


            //  Wait for both cameras to have a frame before continuing.
            for(unsigned int i = 0; i < 5000; i++)
            {
                if(mDownwardCamera.IsReady())
                    break;

                CxUtils::SleepMs(1);
            }
            if(mDownwardCamera.IsReady() == false)
            {
                //errorMessage.append("Camera failed to get to \"Ready\" state " + mDownwardCameraInfo.mName + "!\n");
                result &= false;
            }
            mpGlobalInfo->mpDownwardCameraRaw = mpDownwardCameraRaw;
        }
        if(!mCameraThread.CreateThread(&CameraThread,this))
        {
            std::cout << "Failed to create capture thread" << std::endl;
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
bool UnderwaterVehicle::Start()
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


    //    if(mEnableAPL)
    //    {
    //        //Setup APL
    //        std::cout << "Initializing APL..." << std::endl;
    //        mpAPL = new AcousticPingerLocator();
    //        mpAPL->Initialize(mpGlobalInfo);
    //    }
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
void UnderwaterVehicle::ShutdownJAUS()
{
    mUnderwaterComponent.Shutdown();
}

void UnderwaterVehicle::CameraThread(void *arg)
{
    UnderwaterVehicle *uv=(UnderwaterVehicle *)arg;
    uv->CaptureThread();
}

void UnderwaterVehicle::CaptureThread()
{
    Zebulon::Video::CvImageCapture::Image dest;
    //mpGlobalInfo->get
    //mpGlobalInfo->GetReSizeInfo(mReSizeFlag, mNewWidth, mNewHeight);
    int counter = 0;
    double starttime = CxUtils::GetTimeSecondsSince1Jan1970();
    while(!mCameraThread.QuitThreadFlag())
    {

        //cout << "Capture" << endl;
        //capture sequencially
        if(mpForwardCameraRaw!=NULL)
        {
            mForwardCamera.GetFrame(&dest);
            mpGlobalInfo->mCameraCallback->ProcessCameraData(&dest);
            counter++;
        }
        if(mpDownwardCameraRaw!=NULL)
        {
            mDownwardCamera.GetFrame(&dest);
            mpGlobalInfo->mCameraCallback->ProcessCameraData(&dest);
            counter++;
        }
//        std::cout << counter/(CxUtils::GetTimeSecondsSince1Jan1970() - starttime) << std::endl;
        //CxUtils::SleepMs((1000/15)/2);
        CxUtils::SleepMs(1);
    }
}

/* End of File */









