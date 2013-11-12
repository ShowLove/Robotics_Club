#include "auvc/underwater/missions/2012/simplestartgate.h"

using namespace Zebulon;
using namespace Underwater;

const std::string SimpleStartGate::NextMission = "Training";

SimpleStartGate::SimpleStartGate()
{
    ID = "StartGateSimple";
}

SimpleStartGate::~SimpleStartGate()
{

}

int SimpleStartGate::Init()
{
    double time = 0;
    mXmlConf.GetVar("Underwater.StartGateSimple.Timer@Duration", time);
    time = 35;
    mStopWatch.Initialize(time);
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::StartYaw, mStartYaw))
    {
        std::cout << "StartSwitch was not activated! Grab current yaw." << std::endl;
        mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }
    
    mState = GoToDepth;
    return 0;
}

int SimpleStartGate::ErrorCheck()
{
    return 0;
}

int SimpleStartGate::FetchProcess()
{
    mXmlConf.GetVar("Underwater.SimpleStartGate.Orientation@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.StartGateSimple.Orientation@Roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.StartGateSimple.InitialPower@Axial", mDesiredAxialThrust);
    mXmlConf.GetVar("Underwater.StartGateSimple.InitialPower@Lateral", mDesiredLateralThrust);

    mXmlConf.GetVar("Underwater.StartGateSimple.InitialYaw@Threshold", mYawThresh);

    mXmlConf.GetVar("Underwater.StartGateSimple.InitialDepth@Depth", mStartDepth);
    mXmlConf.GetVar("Underwater.StartGateSimple.InitialDepth@Threshold", mDepthThresh);
    mXmlConf.GetVar("Underwater.StartGateSimple.Speed@ValidGateSpeed", mGateSpeed);
    mGateSpeed = 100;

    
    // Get Current Compass Data
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::StartGateSimple: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::PitchDegrees, mCurrentPitch))
    {
        printf("ERROR::StartGateSimple: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::RollDegrees, mCurrentRoll))
    {
        printf("ERROR::StartGateSimple: Could not get Compass data");
        return 1;
    }
    
    // GetK Current Depth Data
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth))
    {
        printf("ERROR::StartGateSimple: Could not get Depth data");
        return 1;
    }
    
    return 0;
}

std::string SimpleStartGate::ExitEarly()
{
    return "KeepRunning";
}

std::string SimpleStartGate::Execute()
{
    static boost::timer _timestamp;
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState)<< "\tTime: " << _timestamp.elapsed() << std::endl;
    std::cout << std::endl;


    switch (mState)
    {
        case GoToDepth:

            mDesiredYaw = mStartYaw;
            mDesiredDepth = mStartDepth;
            
            if(fabs(AI::Utility::DepthDiff(mCurrentDepth, mStartDepth)) < mDepthThresh && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mStartYaw)) <= mYawThresh)
            {
                mState = ThroughGate;
                mStopWatch.Start();
            }

            break;
        
        case ThroughGate:

            mDesiredYaw = mStartYaw;
            mDesiredDepth = mStartDepth;
            mDesiredAxialThrust = mGateSpeed;
            
            if(mStopWatch.IsFinished())
            {
                mState = LeaveMission;
            }

            std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
            //std::cout << std::endl;
            std::cout << "Timer:"
                      << "\t" << mStopWatch.GetRunTime()
                      << "\t" << mStopWatch.TimeElapsed()
                      << "\t" << mStopWatch.GetRunTime()-mStopWatch.TimeElapsed()
                      << std::endl;

            break;
        
        case LeaveMission:

            return NextMission;

    }

    std::cout << std::endl;
    std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
    //std::cout << std::endl;
    std::cout << "Yaw:"
              << "\t" << mDesiredYaw
              << "\t" << mCurrentYaw
              << "\t" << Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mDesiredYaw)
              << std::endl;
    std::cout << "Pitch:"
              << "\t" << mDesiredPitch
              << "\t" << mCurrentPitch
              << "\t" << Zebulon::AI::Utility::AngleDiff(mCurrentPitch, mDesiredPitch)
              << std::endl;
    std::cout << "Roll:"
              << "\t" << mDesiredRoll
              << "\t" << mCurrentRoll
              << "\t" << Zebulon::AI::Utility::AngleDiff(mCurrentRoll, mDesiredRoll)
              << std::endl;
    std::cout << "Depth:"
              << "\t" << mDesiredDepth
              << "\t" << mCurrentDepth
              << "\t" << Zebulon::AI::Utility::AngleDiff(mCurrentDepth, mDesiredDepth)
              << std::endl;
    std::cout << std::endl;
    std::cout << "Forward:"
              << "\t" << mDesiredAxialThrust
              << std::endl;
    std::cout << "Lateral:"
              << "\t" << mDesiredLateralThrust
              << std::endl;
    
    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);

    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesiredLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    
    return "KeepRunning";
}

std::string SimpleStartGate::GetStateName(State state)
{
    switch (state)
    {
        case GoToDepth:
            return "GoToDepth";

        case ThroughGate:
            return "ThroughGate";

        case LeaveMission:
            return "LeaveMission";

        default:
            return "ERROR::DNE";
    }
}
