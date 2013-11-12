#include "auvc/underwater/missions/2012/startgate.h"

using namespace Zebulon;
using namespace Underwater;

const std::string StartGate::NextMission = "Training";

StartGate::StartGate()
{
    ID = "StartGate";

    mDWFrame = NULL;
    mDWProcFrame = NULL;
}

StartGate::~StartGate()
{
    cvReleaseImage(&mDWFrame);
    cvReleaseImage(&mDWProcFrame);
}

int StartGate::Init()
{
    double time = 0;
    mXmlConf.GetVar("Underwater.StartGate.Timer@InitTravel", time);
    mInitTravel.Initialize(time);
    mXmlConf.GetVar("Underwater.StartGate.Timer@PathTravel", time);
    mPathTravel.Initialize(time);

    if(!mGlobalInfo->GetInfo(GlobalInfo::StartYaw, mStartYaw))
    {
        std::cout << "StartSwitch was not activated! Grab current yaw." << std::endl;
        mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }

    mPathFinder.Initialize();
    int debounce = 0;
    mXmlConf.GetVar("Underwater.StartGate.Debounce@path", debounce);
    mPathDebounce.Initialize(debounce, true);

    mInitCameras = true;
    mLeaveMission = false;

    mState = GoToDepth;

    return 0;
}

int StartGate::ErrorCheck()
{
    return 0;
}

int StartGate::FetchProcess()
{
    mXmlConf.GetVar("Underwater.StartGate.Orientation@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.StartGate.Orientation@Roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.StartGate.InitialPower@Axial", mDesiredAxialThrust);
    mXmlConf.GetVar("Underwater.StartGate.InitialPower@Lateral", mDesiredLateralThrust);

    mXmlConf.GetVar("Underwater.StartGate.InitialDepth@Depth", mInitDepth);
    mXmlConf.GetVar("Underwater.StartGate.InitialDepth@Threshold", mDepthThresh);
    mXmlConf.GetVar("Underwater.StartGate.InitialYaw@Threshold", mYawThresh);

    mXmlConf.GetVar("Underwater.StartGate.Speed@InitTravelSpeed", mInitTravelSpeed);
    mXmlConf.GetVar("Underwater.StartGate.Speed@PathTravelSpeed", mPathTravelSpeed);

    mXmlConf.GetVar("Underwater.StartGate.VisionType@Gary", mVTGary);
    mXmlConf.GetVar("Underwater.StartGate.VisionType@David", mVTDavid);
    mXmlConf.GetVar("Underwater.StartGate.VisionType@New", mVTNew);

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

    //if(mState == PathTravel || mInitCameras)
    {
        // Downward
        if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDWFrame))
        {
            std::cout << "ERROR IN " << GetStateName(mState) << "Couldn't Get DownwardCamera Data" << std::endl;
            return 1;
        }
        if(mDWProcFrame == NULL)
        {
            mDWProcFrame = cvCreateImage(cvSize(mDWFrame->width,mDWFrame->height), mDWFrame->depth, mDWFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mDWProcFrame);

        if(mInitCameras)
        {
            mInitCameras = false;
        }
    }

    return 0;
}

std::string StartGate::ExitEarly()
{
    return "KeepRunning";
}

std::string StartGate::Execute()
{
    static boost::timer _timestamp;
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState) << "\tTime: " << _timestamp.elapsed() << std::endl;
    std::cout << std::endl;

    bool pathExists = false;
    std::vector<Path> paths;

    switch (mState)
    {
        case GoToDepth:

            mDesiredYaw = mStartYaw;
            mDesiredDepth = mInitDepth;

            if(fabs(AI::Utility::DepthDiff(mCurrentDepth, mInitDepth)) < mDepthThresh && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mStartYaw)) <= mYawThresh)
            {

                mInitTravel.Start();
                mState = InitTravel;
            }

            break;

        case InitTravel:

            mDesiredYaw = mStartYaw;
            mDesiredDepth = mInitDepth;
            mDesiredAxialThrust = mInitTravelSpeed;

            if(mInitTravel.IsFinished())
            {
                mPathTravel.Start();
                mState = PathTravel;
            }

            std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
            std::cout << "Timer:"
                      << "\t" << mInitTravel.GetRunTime()
                      << "\t" << mInitTravel.TimeElapsed()
                      << "\t" << mInitTravel.GetRunTime()-mInitTravel.TimeElapsed()
                      << std::endl;

            break;

        case PathTravel:

            mDesiredYaw = mStartYaw;
            mDesiredDepth = mInitDepth;
            mDesiredAxialThrust = mPathTravelSpeed;

            pathExists = GetPaths(paths , mCurrentYaw, mDWFrame, mDWProcFrame);


            if(mPathTravel.IsFinished() || pathExists == true)
            {
                mState = LeaveMission;
            }

            std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
            std::cout << "Timer:"
                      << "\t" << mPathTravel.GetRunTime()
                      << "\t" << mPathTravel.TimeElapsed()
                      << "\t" << mPathTravel.GetRunTime()-mPathTravel.TimeElapsed()
                      << std::endl;

            break;

        case LeaveMission:

            if(mLeaveMission == false)
            {
                mGlobalInfo->SetInfo(GlobalInfo::FixedYaw,mStartYaw);
                //mGlobalInfo->SetInfo(GlobalInfo::StartYaw,mStartYaw);
                Utility::Display::CloseAllWindows();
                mLeaveMission = true;
            }

            return NextMission;

            break;

        default:

            std::cout << "ERROR::" << ID << " state " << mState << " does not exist!" << std::endl;

            break;

    }

    //if (mState == PathTravel)
    {
        AI::Utility::HeadingDisplay(mDWProcFrame, mCurrentYaw, mDesiredYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mDWProcFrame, mCurrentDepth, mDesiredDepth, 0, 192);
        AI::Utility::ThrustDisplay(mDWProcFrame, mDesiredAxialThrust, mDesiredLateralThrust);

        Utility::Display::DisplayImage("DW Frame",mDWFrame);
        Utility::Display::DisplayImage("Processed DW Frame",mDWProcFrame);
    }

    std::cout << std::endl;
    std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
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

    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);
    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);

    return "KeepRunning";
}

/*
bool StartGate::PathsCount()
{
    if(mVTGary)
    {
        std::cout << "Basic Threshold and Segmentation" << std::endl;
        std::cout << std::endl;
        return mPathDebounce.Bounce(mPathFinder.GetPaths(mCurrentYaw, mDWFrame, mDWProcFrame).size() > 0);
    }
    else if(mVTDavid)
    {
        std::cout << "David's CV Functions" << std::endl;
        std::cout << std::endl;
        return mPathDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrentYaw, mDWFrame, mDWProcFrame).size() > 0);

    }
    else if(mVTNew)
    {
        std::cout << "Nothing is implemented here!" << std::endl;
        std::cout << std::endl;
        return 0;
    }
    else
    {
        std::cout << "Vision Algo DNE!" << std::endl;
        std::cout << std::endl;
        return 0;
    }
}
*/

std::string StartGate::GetStateName(State state)
{
    switch (state)
    {
        case GoToDepth:
            return "GoToDepth";

        case InitTravel:
            return "InitTravel";

        case PathTravel:
            return "PathTravel";

        case LeaveMission:
            return "LeaveMission";

        default:
            return "ERROR::DNE";
    }
}
