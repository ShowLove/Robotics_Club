//////////////////////////////////////////////////////////////////////////////////////
///
/// \file boatstart.cpp
/// \brief Mission 1.* Waits for start button to be pushed
///
/// Author(s): Brian Valentino<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:brian_valentino@knights.ucf.edu<br>
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
//////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/boatstart.h"
#include "asvc/surface/missions/startgatecamera.h"
#include <display/display.h>

using namespace Zebulon;
using namespace ASVC;

const std::string BoatStart::Name = "BoatStart";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
BoatStart::BoatStart()
{
    mStartButton = false;
    ID = Name;
    mDebounce.Initialize(3, true);
    mRadioDebounce.Initialize(3, true);
    mNextMission = StartGateCamera::Name;
    mDriveTimeSeconds = 1.0;
    mDriveThrust = 30;
    mDisplayFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BoatStart::~BoatStart()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BoatStart::Init()
{
    Mission::Init();
    ID = Name;
    mXmlConf.GetVar("Surface.BoatStart.NextMission@name", mNextMission);
    mXmlConf.GetVar("Surface.BoatStart.StartDriving@thrust", mDriveThrust);
    mXmlConf.GetVar("Surface.BoatStart.StartDriving@time_seconds", mDriveTimeSeconds);
    //
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(1);
        CxUtils::SleepMs(3000);
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(0);
    }
    mDisplayFlag = Utility::Display::GetDisplayFlag();
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BoatStart::ErrorCheck()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BoatStart::FetchProcess()
{
    mpGlobalInfo->GetMicroInfo(Zebulon::ASVC::Microcontroller::ResetButton, mStartButton);
    //mpGlobalInfo->GetMicroInfo(Zebulon::ASVC::Microcontroller::RadioTriState, mRadioState);
    mStartPosition = mpGlobalInfo->GetPosition();

    mDebounce.Bounce(mStartButton);
    //autonomous mode
    if(mRadioState == 2)
    {
        mRadioDebounce.Bounce(true);
    }

    if(!Mission::mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mInitFrame))
    {
        std::cout << "No Camera" << std::endl;
        return 1;
    }
    Utility::Display::SetDisplay(true);
    Utility::Display::DisplayImage("Camera", mInitFrame,1);
    Utility::Display::SetDisplay(mDisplayFlag);

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string BoatStart::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string BoatStart::Execute()
{
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
    }
    // Save current starting position.
    mpGlobalInfo->SetInfo(GlobalInfo::StartPosition, mStartPosition);
    //this->mpGlobalCommand->SetWaterCannonTilt(75);

    std::cout << "Waiting for Start Button ...\n";
    if(mDebounce.Hit()/* || mRadioDebounce.Hit()*/)
    {
        std::cout << "Start Button has been pressed\n";
        std::cout <<  "Driving...\n";
        mDriveStopWatch.Initialize(mDriveTimeSeconds);
        mDriveStopWatch.Start();
        while(mDriveStopWatch.IsFinished() == false)
        {
            /*
            std::cout << GetName()
                      << " is driving at "
                      << mDriveThrust
                      << " seconds remaining: "
                      << mDriveTimeSeconds - mDriveStopWatch.TimeElapsed();*/
            mpGlobalCommand->SetForwardThrust(mDriveThrust);
            CxUtils::SleepMs(1);
        }
        Utility::Display::CloseAllWindows();
        return mNextMission;
    }
    return Mission::KeepRunning;
}
/* End of File */
