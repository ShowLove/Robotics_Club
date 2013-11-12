#include "auvc/underwater/missions/startbutton.h"

using namespace Zebulon;
using namespace Underwater;

StartButton::StartButton()
{
    ID = "StartButton";
}

StartButton::~StartButton()
{

}

int StartButton::Init()
{
    mButtonDebounce.Initialize(3, true);
    
    mGlobalInfo->SetInfo("ExitEarlyToCounselor", 0);
    
    mState = Wait;
    
    mGlobalInfo->SetInfo(GlobalInfo::Started, false);
    
    return 0;
}

int StartButton::ErrorCheck()
{
    return 0;
}

int StartButton::FetchProcess()
{
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::StartButton: Couldn't get Compass data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor ,mCurrentDepth))
    {
        printf("ERROR::StartButton: Couldn't get Depth data");
        return 1;
    }
    
    bool buttonState;
    if (mButtonDebounce.Bounce(mGlobalInfo->GetInfo(GlobalInfo::StartButton, buttonState) && buttonState))
    {
        mGlobalInfo->SetInfo(GlobalInfo::Started, true);
    }
    
    return 0;
}

std::string StartButton::ExitEarly()
{
    return "KeepRunning";
}

std::string StartButton::Execute()
{
    bool started;
    
    switch (mState)
    {
        case Wait:
            mDesiredDepth = 0;
            mDesiredYaw = 0;
            
            //if (mButtonDebounce.GetDebounced())
            mGlobalInfo->GetInfo(GlobalInfo::Started, started);
            if (started)
            {
                mGlobalInfo->SetInfo(GlobalInfo::StartYaw, mCurrentYaw);
                mState = NextMission;
            }
            
            std::cout << "State: " << "StartButton" << "::Wait" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " Depth: " << mCurrentDepth
                    << std::endl;
            break;
        
        case NextMission:
            return "VisionValidationGate2";
    }
    
    return "KeepRunning";
}
