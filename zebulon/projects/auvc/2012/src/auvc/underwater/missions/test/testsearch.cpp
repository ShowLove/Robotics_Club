#include "auvc/underwater/missions/test/testsearch.h"

using namespace Zebulon;
using namespace Underwater;

const std::string TestSearch::NextMission = "None";

TestSearch::TestSearch()
{
    ID = "TestSearch";
}

TestSearch::~TestSearch()
{
}

int TestSearch::ErrorCheck()
{
    return 0;
}

int TestSearch::Init()
{
    mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    mDesiredYaw = mStartYaw;

    mState=DEPTH;

    return 0;
}

int TestSearch::FetchProcess()
{
    // Buoy Search ///////////////////////////////////
    mXmlConf.GetVar("Underwater.TestSearch.BuoySearch@strafe", mBuoySearchStrafe);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySearch@sweep", mBuoySearchSweep);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySearch@sine", mBuoySearchSine);



    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@strafe_time", mBuoySearchStrafeTime);
    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@strafe_speed", mBuoySearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@forward_time", mBuoyStrafeForwardTime);
    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@forward_speed", mBuoyStrafeForwardSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@smooth_speed", mBuoyStrafeSmoothSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoyStrafe@init_dir", mBuoyStrafeInitDirection);

    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@sweep_degrees", mBuoySearchSweepDegrees);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@sweep_speed", mBuoySearchSweepSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@forward_time", mBuoySweepForwardTime);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@forward_speed", mBuoySweepForwardSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@smooth_speed", mBuoySweepSmoothSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySweep@init_dir", mBuoySweepInitDirection);

    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@sine_time", mBuoySearchSineTime);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@sine_speed", mBuoySearchSineSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@forward_time", mBuoySineForwardTime);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@forward_speed", mBuoySineForwardSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@smooth_speed", mBuoySineSmoothSpeed);
    mXmlConf.GetVar("Underwater.TestSearch.BuoySine@init_dir", mBuoySineInitDirection);
    ///////////////////////////////////

    mXmlConf.GetVar("Underwater.TestSearch.Depth@depth", mDesiredDepth);

    // Get Current Compass Data
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::PitchDegrees, mCurrentPitch))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::RollDegrees, mCurrentRoll))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    // Get Current Depth Data
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth))
    {
        printf("ERROR::StartGate: Could not get Depth data");
        return 1;
    }

    return 0;
}

std::string TestSearch::ExitEarly()
{
    return "KeepRunning";
}

std::string TestSearch::Execute()
{
    //Fixed
    mDesiredPitch=0;
    mDesiredRoll=0;
    //Hard coded, fix me
    mDesiredDepth=mDesiredDepth;
    //Initially zero, search changes
    mDesiredAxialThrust=0;
    mDesiredLateralThrust=0;
    switch(mState)
    {
        case DEPTH:
            std::cout << "I'm IN DEPTH" << std::endl;
            if(fabs(mDesiredDepth-mCurrentDepth)<3)
            {
                mState=INIT;
            }
            break;
        case INIT:
            std::cout << "I'm IN INIT" << std::endl;
            InitSearch();
            mState=SEARCH;
            break;
        case SEARCH:
            std::cout << "I'm IN SEARCH" << std::endl;
            SearchStep();
            break;

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

void TestSearch::InitSearch()
{
    std::cout << "IN ININT" << std::endl;
    std::cout << "Strafe: " << mBuoySearchStrafe << std::endl;
    std::cout << "Sweep: " << mBuoySearchSweep << std::endl;
    std::cout << "Sine: " << mBuoySearchSine << std::endl;
    if(mBuoySearchStrafe == true)
    {
        mBuoyStrafeSearch.InitSearch(mBuoySearchStrafeTime, mBuoySearchStrafeSpeed, mBuoyStrafeForwardTime, mBuoyStrafeForwardSpeed, mBuoyStrafeSmoothSpeed, mBuoyStrafeInitDirection);
    }
    else if(mBuoySearchSweep == true)
    {
        mBuoySweepSearch.InitSearch(mStartYaw, mBuoySearchSweepDegrees, mBuoySearchSweepSpeed, mBuoySweepForwardTime, mBuoySweepForwardSpeed, mBuoySweepInitDirection, mBuoySweepSmoothSpeed);
    }
    else if(mBuoySearchSine == true)
    {
        mBuoySineSearch.InitSearch(mBuoySearchSineTime, mBuoySearchSineSpeed, mBuoySineForwardTime, mBuoySineForwardSpeed, mBuoySineSmoothSpeed, mBuoySineInitDirection);
    }
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO INITIALIZE!" << std::endl;
    }
}

void TestSearch::SearchStep()
{
    std::cout << "IN SEARCH" << std::endl;
    std::cout << "Strafe: " << mBuoySearchStrafe << std::endl;
    std::cout << "Sweep: " << mBuoySearchSweep << std::endl;
    std::cout << "Sine: " << mBuoySearchSine << std::endl;

    if(mBuoySearchStrafe == true)
    {
        mBuoyStrafeSearch.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
        mDesiredYaw=mStartYaw;
    }
    else if(mBuoySearchSweep == true)
    {
        mBuoySweepSearch.SearchStep(mDesiredYaw, mDesiredAxialThrust);
        mDesiredLateralThrust = 0;
    }
    else if(mBuoySearchSine == true)
    {
        mBuoySineSearch.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
        mDesiredYaw=mStartYaw;
    }
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO STEP THROUGH!" << std::endl;
    }
}

/* End of file */
