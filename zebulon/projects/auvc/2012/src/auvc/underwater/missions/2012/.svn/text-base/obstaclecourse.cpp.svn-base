#include "auvc/underwater/missions/2012/obstaclecourse.h"

using namespace Zebulon;
using namespace Underwater;

//const std::string ObstacleCourse::NextMission = "Wreath";
const std::string ObstacleCourse::NextMission = "EtTuBrute";

ObstacleCourse::ObstacleCourse()
{
    ID = "ObstacleCourse";

    mFWFrame = NULL;
    mDWFrame = NULL;
    mFWProcFrame = NULL;
    mDWProcFrame = NULL;

}

ObstacleCourse::~ObstacleCourse()
{
    cvReleaseImage(&mFWFrame);
    cvReleaseImage(&mDWFrame);
    cvReleaseImage(&mFWProcFrame);
    cvReleaseImage(&mDWProcFrame);
}

int ObstacleCourse::ErrorCheck()
{
    return 0;
}

int ObstacleCourse::Init()
{
    if(!mGlobalInfo->GetInfo(GlobalInfo::FixedYaw, mFixedYaw))
    {
        std::cout << "No Fixed Yaw Found, using current yaw" << std::endl;
        mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw);

        mFixedYaw = mCurrentYaw;
        mGlobalInfo->SetInfo(GlobalInfo::FixedYaw, mCurrentYaw);
        std::cout << "mFixedYaw: " << mFixedYaw << std::endl;
        double tmp = 0;
        mGlobalInfo->GetInfo(GlobalInfo::FixedYaw, tmp);
        std::cout << "tmp: " << tmp << std::endl;
        std::cout << "mCurrentYaw: " << mCurrentYaw << std::endl;

    }

    mState = GoToPathDepth;
    //mState = PathSearch;

    // Initialize debounces
    int debounceCount=0;
    mXmlConf.GetVar("Underwater.ObstacleCourse.Debounce@path", debounceCount);
    mPathDebounce.Initialize(debounceCount, true);
    //makes sure it is centered for a bit
    mXmlConf.GetVar("Underwater.ObstacleCourse.Debounce@pathcentered", debounceCount);
    mPathCenteredDebounce.Initialize(debounceCount, true);

    int pathTimer = 0;
    mXmlConf.GetVar("Underwater.ObstacleCourse.Path@timer", pathTimer);
    mPathSearchTimer.Initialize(pathTimer);

    mXmlConf.GetVar("Underwater.ObstacleCourse.Path@exit_timer", mPathExitTime);
    mPathExitTimer.Initialize(mPathExitTime);

    mPathFinder.Initialize();

    // Set timer to leave mission
    int temp;
    mXmlConf.GetVar("Underwater.ObstacleCourse.Default@timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();

    // Store desired depths
    mXmlConf.GetVar("Underwater.ObstacleCourse.Depths@object", mObstacleDepth);
    mXmlConf.GetVar("Underwater.ObstacleCourse.Depths@path", mPathDepth);

    mXmlConf.GetVar("Underwater.ObstacleCourse.Thresholds@depth", mDepthThresh);
    mXmlConf.GetVar("Underwater.ObstacleCourse.Thresholds@angle", mYawThresh);

    /////////// DRIVE FORWARD
    mXmlConf.GetVar("Underwater.ObstacleCourse.DriveForward@timer", mDriveForwardTime);
    mDriveForwardTimer.Initialize(mDriveForwardTime);

    mXmlConf.GetVar("Underwater.ObstacleCourse.Leave@timer", mLeaveTime);
    mLeaveTimer.Initialize(mLeaveTime);

    mLeaveMission = false;

    return 0;
}

int ObstacleCourse::FetchProcess()
{
    mXmlConf.GetVar("Underwater.ObstacleCourse.Default@pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.ObstacleCourse.Default@roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.ObstacleCourse.VisionType@Gary", mVTGary);
    mXmlConf.GetVar("Underwater.ObstacleCourse.VisionType@David", mVTDavid);
    mXmlConf.GetVar("Underwater.ObstacleCourse.VisionType@New", mVTNew);
    InitPaths(mVTGary,mVTDavid,mVTNew);

    double num, denom;
    mXmlConf.GetVar("Underwater.ObstacleCourse.DepthPixelScale@num", num);
    mXmlConf.GetVar("Underwater.ObstacleCourse.DepthPixelScale@denom", denom);
    mDepthPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.ObstacleCourse.Targets@I_thresh", mIThresh);
    mXmlConf.GetVar("Underwater.ObstacleCourse.Targets@J_thresh", mJThresh);

    //////////// PATH SEARCH
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@sine_time", mPathSearchSineTime);
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@sine_speed", mPathSearchSineSpeed);
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    mXmlConf.GetVar("Underwater.ObstacleCourse.PathSearch@init_dir", mPathSearchInitDirection);

    mXmlConf.GetVar("Underwater.ObstacleCourse.Search@sine", mPathSineSearchPattern);

    //mXmlConf.GetVar("Underwater.ObstacleCourse.Path@path_exit_time", mPathExitTime);
    mXmlConf.GetVar("Underwater.ObstacleCourse.Path@exit_path_thrust", mExitPathThrust);


    ///////////// DRIVE FORWARD
    mXmlConf.GetVar("Underwater.ObstacleCourse.DriveForward@thrust", mDriveForwardThrust);



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


    // Forward Camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mFWFrame))
    {
        printf("ERROR::Training: Couldn't get forward camera data");
        return 1;
    }
    if(mFWProcFrame == NULL)
    {
        mFWProcFrame = cvCreateImage(cvSize(mFWFrame->width,mFWFrame->height), mFWFrame->depth, mFWFrame->nChannels);
    }
    cvCopy(mFWFrame,mFWProcFrame);

    // Downward
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDWFrame))
    {
        printf("ERROR::Training: Couldn't get downward camera 2 data");
        return 1;
    }
    if(mDWProcFrame == NULL)
    {
        mDWProcFrame = cvCreateImage(cvSize(mDWFrame->width,mDWFrame->height), mDWFrame->depth, mDWFrame->nChannels);
    }
    cvCopy(mDWFrame, mDWProcFrame);

    // Grab fixed yaw, will be StartYaw or PathYaw
    if(!mGlobalInfo->GetInfo(GlobalInfo::FixedYaw, mFixedYaw))
    {
        std::cout << "FixedYaw has never been set." << std::endl;
        //mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }

    return 0;
}

std::string ObstacleCourse::ExitEarly()
{
    if (mStateTimeout.IsFinished())
    {
        mGlobalInfo->SetInfo(GlobalInfo::ExitEarlyToFinalMission, 1);
        return NextMission;
    }
    else
    {
        return "KeepRunning";
    }
}

std::string ObstacleCourse::Execute()
{
    static boost::timer _timestamp;
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState) << "\tTime: " << _timestamp.elapsed() << " " << mStateTimeout.TimeElapsed() << std::endl;
    std::cout << std::endl;

    bool pathExists = false;
    std::vector<Path> paths;

    switch (mState)
    {
        // Go to Path Depth, leave immediatley if path is found
        case GoToPathDepth:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mPathDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            pathExists = GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);


            if((fabs(AI::Utility::DepthDiff(mCurrentDepth, mPathDepth)) < mDepthThresh
                && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mFixedYaw)) <= mYawThresh))
                // Add a check if the path is seen to go directly to path found ... || )
            {

                mPathSearchTimer.Start();
                InitSearch();
                mState = PathSearch;
            }

            break;

        //Search for a path, if found center, if not timeout and look at buoys
        case PathSearch:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mPathDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            pathExists = GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);
            //Add some kind of debouncing to get here, in fetch probably
            if(pathExists)
            {
                mState = DoPath;
            }
            else if(mPathSearchTimer.IsFinished())
            {
                mState = GoToObstacleDepth;
            }
            else
            {
                SearchStep();
                //mPathSearchPattern.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
            }

            std::cout << "Number of Paths: " << paths.size() << std::endl;

            std::cout << "Timer:"
                      << "\t" << mPathSearchTimer.GetRunTime()
                      << "\t" << mPathSearchTimer.TimeElapsed()
                      << "\t" << mPathSearchTimer.GetRunTime()-mPathSearchTimer.TimeElapsed()
                      << std::endl;

            break;

        //center in on a path
        case DoPath:

            //Don't debounce in DoPath
            GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);
            if(paths.size()>0)
            {
                //if path is found change thrust
                if(mPathCenteredDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrentYaw, mDesiredYaw, mDesiredAxialThrust, mDesiredLateralThrust)))
                {

                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    //mFixedYaw = mDesiredYaw;
                    //mDesiredYaw = paths[0].mAngle;
                    mGlobalInfo->SetInfo(GlobalInfo::FixedYaw, mDesiredYaw);

                    cvCircle(mDWProcFrame, cvPoint(mDWProcFrame->width/2,mDWProcFrame->height/2), 200, cvScalar(0,255,0), 4);

                    mState = ExitPath;
                }
                else
                {
                    std::cout << "NOT CENTERED SEEN" << std::endl;
                    //mPathCenteredDebounce.Miss();
                }

                std::cout << "PATHS SEEN" << std::endl;
            }
            else
            {
                //can't be centered if didn't see it
                mPathCenteredDebounce.Miss();
                //mState=PathSearch;
                std::cout << "NO PATHS" << std::endl;
            }

            break;

        //lock the path heading for awhile
        //could be messy if did not find a path
        case ExitPath:

            //mDesiredYaw = mFixedYaw;
            mDesiredAxialThrust = mExitPathThrust;
            mDesiredDepth = mPathDepth;
            mDesiredLateralThrust = 0;

            if(mPathTimer.IsFinished())
            {
                mState = GoToObstacleDepth;
            }

            std::cout << "Timer:"
                      << "\t" << mPathTimer.GetRunTime()
                      << "\t" << mPathTimer.TimeElapsed()
                      << "\t" << mPathTimer.GetRunTime()-mPathTimer.TimeElapsed()
                      << std::endl;

            break;

    case GoToObstacleDepth:

        mDesiredYaw = mFixedYaw;
        mDesiredDepth = mObstacleDepth;
        mDesiredAxialThrust = 0;
        mDesiredLateralThrust = 0;

        pathExists = GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);


        if((fabs(AI::Utility::DepthDiff(mCurrentDepth, mObstacleDepth)) < mDepthThresh
            && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mFixedYaw)) <= mYawThresh))
            // Add a check if the path is seen to go directly to path found ... || )
        {

            //mPathSearchTimer.Start();
            mDriveForwardTimer.Start();
            mState = DriveForward;
        }

        break;

    case DriveForward:

        mDesiredYaw = mFixedYaw;
        mDesiredDepth = mObstacleDepth;
        mDesiredAxialThrust = mDriveForwardThrust;
        mDesiredLateralThrust = 0;

        pathExists = GetPaths(paths , mCurrentYaw, mDWFrame, mDWProcFrame);

        if(mDriveForwardTimer.IsFinished() || pathExists == true)
        {
            mState = LeaveMission;
        }

        std::cout << "Number of Paths: " << paths.size() << std::endl;

        std::cout << " \tDesired:\tCurrent:\tDiff" << std::endl;
        std::cout << "Timer:"
                  << "\t" << mDriveForwardTimer.GetRunTime()
                  << "\t" << mDriveForwardTimer.TimeElapsed()
                  << "\t" << mDriveForwardTimer.GetRunTime()-mDriveForwardTimer.TimeElapsed()
                  << std::endl;

        break;

        /*if(mDriveForwardTimer.IsFinished())
        {
            mState = LeaveMission;
        }

        break;*/

    case LeaveMission:

        if(mLeaveMission == false)
        {
            Utility::Display::CloseAllWindows();
            mLeaveMission = true;
        }

        return NextMission;

    default:

        std::cout << "ERROR::" << ID << " state " << mState << " does not exist" << std::endl;

        break;
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

    if (mState == PathSearch || mState == DoPath || mState == ExitPath)
    {
        AI::Utility::HeadingDisplay(mDWProcFrame, mCurrentYaw, mDesiredYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mDWProcFrame, mCurrentDepth, mDesiredDepth, 0, 192);
        AI::Utility::ThrustDisplay(mDWProcFrame, mDesiredAxialThrust, mDesiredLateralThrust);
    }
    else
    {
        AI::Utility::HeadingDisplay(mFWProcFrame, mCurrentYaw, mDesiredYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mFWProcFrame, mCurrentDepth, mDesiredDepth, 0, 192);
        AI::Utility::ThrustDisplay(mFWProcFrame, mDesiredAxialThrust, mDesiredLateralThrust);
    }

    Utility::Display::DisplayImage("Post Processing FW",mFWProcFrame);
    Utility::Display::DisplayImage("Post Processing DW",mDWProcFrame);

    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);

    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesiredLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);

    return "KeepRunning";
}

void ObstacleCourse::InitSearch()
{
    if(mPathSineSearchPattern == true)
    {
        mPathSineSearch.InitSearch(mPathSearchSineTime, mPathSearchSineSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed, mPathSearchSmoothSpeed, mPathSearchInitDirection);
    }
    /*else if(mBuoySearchSweep == true)
    {
        mPathSweepSearch.InitSearch(mFixedYaw, mBuoySearchSweepDegrees, mBuoySearchSweepSpeed, mBuoySweepForwardTime, mBuoySweepForwardSpeed, mBuoySweepInitDirection, mBuoySweepSmoothSpeed);
    }*/
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO INITIALIZE!" << std::endl;
    }
}

void ObstacleCourse::SearchStep()
{
    if(mPathSineSearchPattern == true)
    {
        mPathSineSearch.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
    }
    /*else if(mBuoySearchSweep == true)
    {
        mBuoySweepSearch.SearchStep(mDesiredYaw, mDesiredAxialThrust);
        mDesiredLateralThrust = 0;
    }*/
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO STEP THROUGH!" << std::endl;
    }
}

std::string ObstacleCourse::GetStateName(State state)
{
    switch (state)
    {
        case GoToPathDepth:
            return "GoToPathDepth";

        case GoToObstacleDepth:
            return "GoToObstacleDepth";

        case DriveForward:
            return "DriveForward";

        case Avoid:
            return "Avoid";

        case Avoided:
            return "Avoided";

        //case Leave:
        //    return "Leave";

        case PathSearch:
            return "PathSearch";

        case DoPath:
            return "DoPath";

        case ExitPath:
            return "ExitPath";

        case LeaveMission:
            return "LeaveMission";

        default:
            return "ERROR::DNE";
    }
}

/* End of file */

