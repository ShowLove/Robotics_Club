#include "auvc/underwater/missions/2012/startswitch.h"

using namespace Zebulon;
using namespace Underwater;

const std::string StartSwitch::NextMission = "StartGate";

StartSwitch::StartSwitch()
{
    ID = "StartSwitch";
}

StartSwitch::~StartSwitch()
{

}

int StartSwitch::ErrorCheck()
{
    return 0;
}

int StartSwitch::Init()
{
    //mButtonDebounce.Initialize(2, false);
    
    mGlobalInfo->SetInfo("ExitEarlyToCounselor", 0);
    
    mState = Wait;
    
    mGlobalInfo->SetInfo(GlobalInfo::Started, false);
    
    return 0;
}

int StartSwitch::FetchProcess()
{
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::StartSwitch: Couldn't get Compass data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor ,mCurrentDepth))
    {
        printf("ERROR::StartSwitch: Couldn't get Depth data");
        return 1;
    }
    
    bool buttonState;
    if (mButtonDebounce.Bounce(mGlobalInfo->GetInfo(GlobalInfo::StartButton, buttonState) && buttonState))
    {
        mGlobalInfo->SetInfo(GlobalInfo::Started, true);
    }

    /*bool buttonState;
    mGlobalInfo->GetInfo(GlobalInfo::StartButton, buttonState);
    if(buttonState)
    {
        mGlobalInfo->SetInfo(GlobalInfo::Started, true);
    }*/

    std::cout << "buttonState: " << buttonState << std::endl;
    
    return 0;
}

std::string StartSwitch::ExitEarly()
{
    return "KeepRunning";
}

std::string StartSwitch::Execute()
{
    bool started;
    static boost::timer _timestamp;

    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState)<< "\tTime: " << _timestamp.elapsed() << std::endl;
    
    switch (mState)
    {
        case Wait:
            mDesiredDepth = 0;
            mDesiredYaw = 0;

            mGlobalInfo->GetInfo(GlobalInfo::Started, started);
            if(started)
            {
                mGlobalInfo->SetInfo(GlobalInfo::StartYaw, mCurrentYaw);
                mState = LeaveMission;
            }

            std::cout << " Yaw: " << mCurrentYaw
                      << " Depth: " << mCurrentDepth
                      << std::endl;
            break;
        
        case LeaveMission:
            return NextMission;
    }
    
    return "KeepRunning";
}

std::string StartSwitch::GetStateName(State state)
{
    switch (state)
    {
        case Wait:
            return "Wait";

        case LeaveMission:
            return "LeaveMission";

        default:
            return "ERROR::DNE";
    }
}

