#include "auvc/underwater/missions/2012/training.h"

using namespace Zebulon;
using namespace Underwater;


const std::string Training::NextMission = "ObstacleCourse";
//const std::string Training::NextMission = "Wreath";

Training::Training()
{
    ID = "Training";

    mFWFrame = NULL;
    mDWFrame = NULL;
    mFWProcFrame = NULL;
    mDWProcFrame = NULL;

    mDisplayLaser = NULL;
    mDisplayLaser = cvCreateImage(cvSize(800,600), IPL_DEPTH_8U, 3);
    mIsLaserAvailable = false;
}

Training::~Training()
{
    cvReleaseImage(&mFWFrame);
    cvReleaseImage(&mDWFrame);
    cvReleaseImage(&mFWProcFrame);
    cvReleaseImage(&mDWProcFrame);
    cvReleaseImage(&mDisplayLaser);
}

int Training::ErrorCheck()
{
    return 0;
}

int Training::Init()
{
    // Use as default yaw if no path is found / when searching for the path
    if(!mGlobalInfo->GetInfo(GlobalInfo::StartYaw, mStartYaw))
    {
        std::cout << "StartYaw was not activated! Grab current yaw." << std::endl;
        if(!mGlobalInfo->GetInfo(GlobalInfo::FixedYaw, mFixedYaw))
        {
            std::cout << "FixedYaw was not activated! Grab current yaw." << std::endl;
            mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw);
            mFixedYaw = mCurrentYaw;
        }
        //mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }
    else
    {
        mFixedYaw = mStartYaw;
    }

    mGlobalInfo->SetInfo(GlobalInfo::FixedYaw, mFixedYaw);
    mDesiredYaw = mFixedYaw;
    //mPathFoundFlag = false;

    // Initial state
    mState = GoToPathDepth;

    //mState = Leave;
    //mState = InitAttack;
    //mState = PathSearch;
    //mState = Search;

    // Initialize debounces
    int debounceCount=0;
    mXmlConf.GetVar("Underwater.Training.Debounce@buoy", debounceCount);
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Initialize(debounceCount, false);
    }
    mXmlConf.GetVar("Underwater.Training.Debounce@path", debounceCount);
    mPathDebounce.Initialize(debounceCount, true);
    //makes sure it is centered for a bit
    mXmlConf.GetVar("Underwater.Training.Debounce@pathcentered", debounceCount);
    mPathCenteredDebounce.Initialize(debounceCount, true);


    mXmlConf.GetVar("Underwater.Training.Debounce@centered", debounceCount);
    mCenteredDebounce.Initialize(debounceCount, true);
    mXmlConf.GetVar("Underwater.Training.Debounce@lost", debounceCount);
    mLostTargetDebounce.Initialize(debounceCount, true);


    int pathTimer = 0;
    mXmlConf.GetVar("Underwater.Training.Path@timer", pathTimer);
    mPathSearchTimer.Initialize(pathTimer);


    //mObjectScanData.Setup(0.33, 1, 5, 0.04);
    mXmlConf.GetVar("Underwater.Training.Laser@maxDist", mMaxDistance);
    mXmlConf.GetVar("Underwater.Training.Laser@minDist", mMinDistance);
    mXmlConf.GetVar("Underwater.Training.Laser@angleRange", mAngleRange);
    mXmlConf.GetVar("Underwater.Training.Laser@minAngle", mStartAngle);
    mXmlConf.GetVar("Underwater.Training.Laser@maxAngle", mEndAngle);
    mObjectScanData.SimpleSetup(mMaxDistance, mMinDistance, mAngleRange, mStartAngle, mEndAngle);

    mPathFinder.Initialize();

    /*
    mXmlConf.GetVar("Underwater.Training.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Training.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Training.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.Training.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Training.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed, mPathSearchSmoothSpeed);

    mXmlConf.GetVar("Underwater.Training.Search@sweep_degrees", mSearchSweepDegrees);
    mXmlConf.GetVar("Underwater.Training.Search@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar("Underwater.Training.Search@forward_time", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.Training.Search@forward_speed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Training.Search@smooth_speed", mSearchSmoothSpeed);
    mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);*/

    // Set timer to leave mission
    int temp;
    mXmlConf.GetVar("Underwater.Training.Default@timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();

    // Find what buoys to hit
    mXmlConf.GetVar("Underwater.Training.BuoyToHit@red", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Red;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Red;
    }

    mXmlConf.GetVar("Underwater.Training.BuoyToHit@green", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Green;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Green;
    }

    mXmlConf.GetVar("Underwater.Training.BuoyToHit@yellow", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Yellow;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Yellow;
    }

    // Store desired depths
    mXmlConf.GetVar("Underwater.Training.Depths@buoy", mBuoyDepth);
    mXmlConf.GetVar("Underwater.Training.Depths@path", mPathDepth);
    mXmlConf.GetVar("Underwater.Training.Depths@leave", mLeaveDepth);
    mXmlConf.GetVar("Underwater.Training.Thresholds@depth", mDepthThresh);
    //mDefaultDepth = mDesired.mDepth;

    mXmlConf.GetVar("Underwater.Training.Thresholds@angle", mYawThresh);

    // Fixed Yaw
    /*if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::Training: Couldn't get Compass data");
        return 1;
    }*/

    ///////////// INITATTACK
    mXmlConf.GetVar("Underwater.Training.InitAttack@timer", mInitAttackTime);
    mInitAttackTimer.Initialize(mInitAttackTime);

    mAttackingBuoy = 0;

    mInitCameras = true;
    mLeaveMission = false;

    // Buoy Search ///////////////////////////////////
    mXmlConf.GetVar("Underwater.Training.BuoySearch@strafe", mBuoySearchStrafe);
    mXmlConf.GetVar("Underwater.Training.BuoySearch@sweep", mBuoySearchSweep);

    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@strafe_time", mBuoySearchStrafeTime);
    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@strafe_speed", mBuoySearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@forward_time", mBuoyStrafeForwardTime);
    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@forward_speed", mBuoyStrafeForwardSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@smooth_speed", mBuoyStrafeSmoothSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoyStrafe@init_dir", mBuoyStrafeInitDirection);

    mXmlConf.GetVar("Underwater.Training.BuoySweep@sweep_degrees", mBuoySearchSweepDegrees);
    mXmlConf.GetVar("Underwater.Training.BuoySweep@sweep_speed", mBuoySearchSweepSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoySweep@forward_time", mBuoySweepForwardTime);
    mXmlConf.GetVar("Underwater.Training.BuoySweep@forward_speed", mBuoySweepForwardSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoySweep@smooth_speed", mBuoySweepSmoothSpeed);
    mXmlConf.GetVar("Underwater.Training.BuoySweep@init_dir", mBuoySweepInitDirection);
    ///////////////////////////////////

    mXmlConf.GetVar("Underwater.Training.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Training.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Training.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.Training.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Training.PathSearch@smooth_speed", mPathSearchSmoothSpeed);

    mPathSearchPattern.InitSearch(mPathSearchStrafeTime,mPathSearchStrafeSpeed,mPathSearchForwardTime,mPathSearchForwardSpeed,mPathSearchSmoothSpeed);

    mXmlConf.GetVar("Underwater.Training.Path@path_exit_time", mPathExitTime);
    mXmlConf.GetVar("Underwater.Training.Path@exit_path_thrust", mExitPathThrust);
    //mXmlConf.GetVar("Underwater.Training.Path@timer", mPathSearchTimer);
    InitSearch();
    return 0;
}

int Training::FetchProcess()
{
//    mXmlConf.GetVar("Underwater.Training.Default@depth", mDesiredDepth);
    mXmlConf.GetVar("Underwater.Training.Default@pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.Training.Default@roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.Training.VisionType@Gary", mVTGary);
    mXmlConf.GetVar("Underwater.Training.VisionType@David", mVTDavid);
    mXmlConf.GetVar("Underwater.Training.VisionType@New", mVTNew);
    InitPaths(mVTGary,mVTDavid,mVTNew);

    double num, denom;
    mXmlConf.GetVar("Underwater.Training.LatPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Training.LatPixelScale@denom", denom);
    mLatPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Training.DepthPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Training.DepthPixelScale@denom", denom);
    mDepthPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Training.YawPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Training.YawPixelScale@denom", denom);
    mYawPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Training.AxialPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Training.AxialPixelScale@denom", denom);
    mAxialPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Training.Targets@I_thresh", mIThresh);
    mXmlConf.GetVar("Underwater.Training.Targets@J_thresh", mJThresh);
    mXmlConf.GetVar("Underwater.Training.Targets@area", mTargetArea);
//    mXmlConf.GetVar("Underwater.Training.Targets@area_thresh", mAreaCloseThresh);

    ///////////// ATTACK
    mXmlConf.GetVar("Underwater.Training.Attack@yaw", mYawAttack);
    mXmlConf.GetVar("Underwater.Training.Attack@lateral", mLateralAttack);
    mXmlConf.GetVar("Underwater.Training.Attack@laser_dist", mAttackLaserDist);
    mXmlConf.GetVar("Underwater.Training.Attack@thrust", mAttackThrust);

    ///////////// LOCK ATTACK
    mXmlConf.GetVar("Underwater.Training.LockAttack@timer", mLockAttackTime);
    mXmlConf.GetVar("Underwater.Training.LockAttack@thrust", mLockAttackThrust);

    ////////////// UNHIT
    mXmlConf.GetVar("Underwater.Training.Unhit@timer", mUnhitTime);
    mXmlConf.GetVar("Underwater.Training.Unhit@thrust", mUnhitThrust);

    ////////////// UNHITLEAVE
    mXmlConf.GetVar("Underwater.Training.UnhitLeave@timer", mUnhitLeaveTime);
    mXmlConf.GetVar("Underwater.Training.UnhitLeave@thrust", mUnhitLeaveThrust);

    ////////////// LEAVE
    mXmlConf.GetVar("Underwater.Training.Leave@timer", mLeaveTime);
    mXmlConf.GetVar("Underwater.Training.Leave@timer", mLeaveThrust);


    //Laser
    ////mXmlConf.GetVar("Underwater.Training.Laser@CloseDist", mLaserCloseDist);
    ////mXmlConf.GetVar("Underwater.Training.Laser@MinRadius", mMinLaserObjectRadius);
    ////mXmlConf.GetVar("Underwater.Training.Laser@MaxRadius", mMaxLaserObjectRadius);

    mBuoyObject[Red].Initialize("RedBuoy");
    mBuoyObject[Yellow].Initialize("YellowBuoy");
    mBuoyObject[Green].Initialize("GreenBuoy");


    // Laser
    if(mGlobalInfo->GetInfo(GlobalInfo::Laser, mRawLaserData))
    {
        mIsLaserAvailable = true;
    }
    else
    {
        //        printf("ERROR::TestLaserTracking: Couldn't get laser data");
    }

    // Grab fixed yaw, will be StartYaw or PathYaw
    if(!mGlobalInfo->GetInfo(GlobalInfo::FixedYaw, mFixedYaw))
    {
        std::cout << "FixedYaw has never been set." << std::endl;
        //mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }

    //SimpleSetup(double minAngle, double maxAngle, double maxDistance, double minDistance);
    //double SimpleDistance(const CxUtils::Point3D::List& rangeScan, bool &valid);


    //mObjectScanData.CalculateCartesian(mRawLaserData, true, 100, -50, 50);
    //std::cout << "Not Adjusted Laser: " << mCurrLaserDist << std::endl;
    if(mIsLaserAvailable)
    {
        mCurrLaserDist = mObjectScanData.SimpleDistance(mRawLaserData, mLaserObjectValid)*39.37007874015748;
        std::cout << "Adjusted Laser: " << mCurrLaserDist << std::endl;
    }
    /*if(mLaserObjectValid)
    {

    }
    else
    {

    }*/


    /*mLaserObjects = mObjectScanData.GetObjects(mLaserObjectValid, mObjectScanData.BIGGEST);
    if(mLaserObjectValid && mLaserObjects[0].mZ > mMinLaserObjectRadius && mLaserObjects[0].mZ < mMaxLaserObjectRadius)
    {
        // in inches
        mCurrLaserDist = sqrt(mLaserObjects[0].mX*mLaserObjects[0].mX + mLaserObjects[0].mY*mLaserObjects[0].mY)*39.37007874015748;
    }
    else
    {
        mCurrLaserDist = 1000;
    }*/

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
    /*
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Bounce(mBuoyObject[i].GetVisionReturn(mFWFrame, mReturn[i], mFWProcFrame));
    }*/
    if (mState == GoToBuoyDepth || mState == BuoySearch || mState == InitAttack || mState == Attack || mState == Unhit || mState == LockAttack || mState == Leave)
    {
        mDebounce[mBuoyToAttack[mAttackingBuoy]].Bounce(mBuoyObject[mBuoyToAttack[mAttackingBuoy]].GetVisionReturn(mFWFrame, mReturn[mBuoyToAttack[mAttackingBuoy]], mFWProcFrame));
    }

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

    //////////////////////////////////////
    // Add grab fix yaw here
    //////////////////////////////////////

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

std::string Training::ExitEarly()
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

std::string Training::Execute()
{
    static boost::timer _timestamp;
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState) << "\tTime: " << _timestamp.elapsed() << std::endl;
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

            //pathExists = GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);
            /*if(pathExists)
            {

            }*/

            if((fabs(AI::Utility::DepthDiff(mCurrentDepth, mPathDepth)) < mDepthThresh
                && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mFixedYaw)) <= mYawThresh))
                // Add a check if the path is seen to go directly to path found ... || )
            {

                mPathSearchTimer.Start();
                mState = PathSearch;
            }

            break;

        //Search for a path, if found center, if not timeout and look at buoys
        case PathSearch:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mPathDepth;

            pathExists = GetPaths(paths, mCurrentYaw, mDWFrame, mDWProcFrame);
            //Add some kind of debouncing to get here, in fetch probably
            if(pathExists)
            {
                mState = DoPath;
            }
            else if(mPathSearchTimer.IsFinished())
            {
                mState = BuoySearch;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
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
                    mGlobalInfo->SetInfo(GlobalInfo::FixedYaw, mDesiredYaw); // should be current yaw

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
                //InitSearch();
                mState = GoToBuoyDepth;
            }

            std::cout << "Timer:"
                      << "\t" << mPathTimer.GetRunTime()
                      << "\t" << mPathTimer.TimeElapsed()
                      << "\t" << mPathTimer.GetRunTime()-mPathTimer.TimeElapsed()
                      << std::endl;

            break;

        //go to a good depth for buoys
        case GoToBuoyDepth:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mBuoyDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            if((fabs(AI::Utility::DepthDiff(mCurrentDepth, mBuoyDepth)) < mDepthThresh
                && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mDesiredYaw)) <= mYawThresh))
                // Add a check if the path is seen to go directly to path found ... || )
            {
                mState = BuoySearch;
            }

            break;

        //start searching for one color buoy
        case BuoySearch:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mBuoyDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            //if found attack it
            if(mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mInitAttackTimer.Start();
                mState = InitAttack;
                //mState = Attack;
            }
            //or search
            else
            {
                SearchStep();
            }

            std::cout << "Current Buoy Debounce: " << mDebounce[mBuoyToAttack[mAttackingBuoy]].GetCount() << std::endl;

            break;

        case InitAttack:
            mCenterI=mFWProcFrame->width/2;
            mCenterJ=mFWProcFrame->height/2;
            if(mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mCenterI = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterI;
                mCenterJ = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterJ;
                mArea = mReturn[mBuoyToAttack[mAttackingBuoy]].mArea;

//                if (mCenteredDebounce.Bounce( fabs(mCenterI - mFWProcFrame->width/2) < mIThresh &&
//                     fabs(mCenterJ - mFWProcFrame->height/2) < mJThresh &&
                //     (mTargetArea - mArea) < mAreaCloseThresh ))
                /*if (mCurrLaserDist < mAttackLaserDist || mArea > mAreaCloseThresh)
                {
                    mState = LockAttack;
                    mLockAttackTimer.Initialize(mLockAttackTime);
                    mLockAttackTimer.Start();

                    if(mYawAttack)
                    {
                        mLockAttackYaw = mCurrentYaw;
                    }
                    else if(mLateralAttack)
                    {
                        mLockAttackYaw = mFixedYaw;
                    }
                    mLockAttackDepth = mCurrentDepth;
                }*/

                mLostTargetDebounce.Miss();
            }
            else
            {
                if(mLostTargetDebounce.Hit())
                {
                    //InitSearch();
                    //mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);

                    mState = BuoySearch;
                    break;
                }
            }

            if(mYawAttack)
            {
                mDesiredYaw = mCurrentYaw + (mCenterI - mFWProcFrame->width/2)*mYawPixelScale;
                mDesiredLateralThrust = 0;

            }
            else if(mLateralAttack)
            {
                mDesiredYaw = mFixedYaw;
                mDesiredLateralThrust = (mCenterI - mFWProcFrame->width/2)*mLatPixelScale;;
            }

            if(mInitAttackTimer.IsFinished())
            {
                mState = Attack;
            }

            //mDesiredDepth = mCurrentDepth + (mCenterJ - mFWProcFrame->height/2 + 100)*mDepthPixelScale;
            mDesiredDepth = mCurrentDepth + (mCenterJ - mFWProcFrame->height/2)*mDepthPixelScale;
            //mDesired.mAxialThrust = (mTargetArea + 1000 - mArea)*mAxialPixelScale;
            mDesiredAxialThrust = mInitAxialThrust;
            // or constant axial thrust?

            std::cout << "Target Data  i: " << mCenterI << " j: " << mCenterJ << " area: " << mArea << std::endl;
            std::cout << "Laser Dist: " << mCurrLaserDist << std::endl;

            break;

        //attack the buoy through either L/A or A/Y
        //if laser sees it, lock heading
        //or if the area is big, lock heading
        case Attack:
            mCenterI=mFWProcFrame->width/2;
            mCenterJ=mFWProcFrame->height/2;
            if(mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mCenterI = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterI;
                mCenterJ = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterJ;
                mArea = mReturn[mBuoyToAttack[mAttackingBuoy]].mArea;

                if (mCenteredDebounce.Bounce( fabs(mCenterI - mFWProcFrame->width/2) < mIThresh &&
                     fabs(mCenterJ - mFWProcFrame->height/2) < mJThresh && mArea > mTargetArea))
                    //     (mTargetArea - mArea) < mAreaCloseThresh ))
                {
                    mState = LockAttack;
                    mLockAttackTimer.Initialize(mLockAttackTime);
                    mLockAttackTimer.Start();

                    if(mYawAttack)
                    {
                        mLockAttackYaw = mCurrentYaw;
                    }
                    else if(mLateralAttack)
                    {
                        mLockAttackYaw = mFixedYaw;
                    }
                    mLockAttackDepth = mCurrentDepth;
                }

                mLostTargetDebounce.Miss();
            }
            else
            {
                if(mLostTargetDebounce.Hit())
                {
                    //InitSearch();
                    //mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);

                    mState = BuoySearch;
                    break;
                }
            }

            if(mYawAttack)
            {
                mDesiredYaw = mCurrentYaw + (mCenterI - mFWProcFrame->width/2)*mYawPixelScale;
                mDesiredLateralThrust = 0;

            }
            else if(mLateralAttack)
            {
                mDesiredYaw = mFixedYaw;
                mDesiredLateralThrust = (mCenterI - mFWProcFrame->width/2)*mLatPixelScale;;
            }

            //mDesiredDepth = mCurrentDepth + (mCenterJ - mFWProcFrame->height/2 + 100)*mDepthPixelScale;
            mDesiredDepth = mCurrentDepth + (mCenterJ - mFWProcFrame->height/2)*mDepthPixelScale;
            //mDesired.mAxialThrust = (mTargetArea + 1000 - mArea)*mAxialPixelScale;
            mDesiredAxialThrust = mAttackThrust;
            // or constant axial thrust?

            std::cout << "Target Data  i: " << mCenterI << " j: " << mCenterJ << " area: " << mArea << std::endl;
            std::cout << "Laser Dist: " << mCurrLaserDist << std::endl;

            break;

        //drive at it for a fixed time
        //if first buoy do unhit
        //if second buoy do unhitandleave
        case LockAttack:

            mDesiredAxialThrust = mLockAttackThrust;
            mDesiredYaw = mLockAttackYaw;
            mDesiredLateralThrust = 0;
            mDesiredDepth = mLockAttackDepth;

            if(mLockAttackTimer.IsFinished())
            {
                //second buoy, leave
                if(mAttackingBuoy)
                {
                    mUnhitLeaveTimer.Initialize(mUnhitLeaveTime);
                    mUnhitLeaveTimer.Start();
                    //GoToLeaveDepth, change depth first?
                    mState = UnhitLeave;
                }
                //if first buoy, unhit
                else
                {
                    mUnhitTimer.Initialize(mUnhitTime);
                    mUnhitTimer.Start();
                    mState = Unhit;
                }
            }

            std::cout << "Timer:"
                      << "\t" << mLockAttackTimer.GetRunTime()
                      << "\t" << mLockAttackTimer.TimeElapsed()
                      << "\t" << mLockAttackTimer.GetRunTime()-mLockAttackTimer.TimeElapsed()
                      << std::endl;

            break;

        case Unhit:

            mDesiredAxialThrust = mUnhitThrust;
            mDesiredYaw = mLockAttackYaw;
            mDesiredLateralThrust = 0;
            // Maybe change to buoy depth
            mDesiredDepth = mLockAttackDepth;

            if(mUnhitTimer.IsFinished())
            {
                mDesiredYaw = mFixedYaw;
                mAttackingBuoy++;
                InitSearch();
                mState = BuoySearch;
            }

            std::cout << "Timer:"
                      << "\t" << mUnhitTimer.GetRunTime()
                      << "\t" << mUnhitTimer.TimeElapsed()
                      << "\t" << mUnhitTimer.GetRunTime()-mUnhitTimer.TimeElapsed()
                      << std::endl;

            break;

        case UnhitLeave:

            mDesiredAxialThrust = mUnhitLeaveThrust;
            mDesiredYaw = mLockAttackYaw;
            //mDesiredYaw = mFixedYaw;
            mDesiredLateralThrust = 0;
            // Maybe change to buoy depth
            mDesiredDepth = mLockAttackDepth;

            if(mUnhitLeaveTimer.IsFinished())
            {
                mDesiredYaw = mFixedYaw;
                mAttackingBuoy++;
                mState = GoToLeaveDepth;
            }

            std::cout << "Timer:"
                      << "\t" << mUnhitLeaveTimer.GetRunTime()
                      << "\t" << mUnhitLeaveTimer.TimeElapsed()
                      << "\t" << mUnhitLeaveTimer.GetRunTime()-mUnhitLeaveTimer.TimeElapsed()
                      << std::endl;

            break;

        case GoToLeaveDepth:

            mDesiredYaw = mFixedYaw;
            mDesiredDepth = mLeaveDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            if((fabs(AI::Utility::DepthDiff(mCurrentDepth, mLeaveDepth)) < mDepthThresh
                && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mFixedYaw)) <= mYawThresh))
                // Add a check if the path is seen to go directly to path found ... || )
            {
                mLeaveTimer.Initialize(mLeaveTime);
                mLeaveTimer.Start();
                mState = Leave;
            }

            break;

        case Leave:
            mDesiredDepth = mLeaveDepth;
            mDesiredAxialThrust = mLeaveThrust;
            mDesiredYaw = mFixedYaw;

            if (mLeaveTimer.IsFinished() || mPathDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrentYaw, mDWFrame, mDWProcFrame).size() > 0))
            {
                //mDesiredDepth = mDefaultDepth;
                mState = LeaveMission;
            }

            break;

        case LeaveMission:

            //mGlobalInfo->SetInfo(GlobalInfo::FixedYaw, mFixedYaw);

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

    std::cout << "Num Buoys Attacked: " << mAttackingBuoy << std::endl;

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

    /*std::cout << " Yaw " << mCurrentYaw << " -> " << mDesiredYaw
              << " Depth " << mCurrentDepth << " -> " << mDesiredDepth << std::endl
              << " Axial " << mDesiredAxialThrust << " Lat " << mDesiredLateralThrust << std::endl
              << std::endl;*/


    std::cout << "mCurrLaserDist: " << mCurrLaserDist << std::endl;
    cvZero(mDisplayLaser);
    double x, y;
    for(int i = 0; i < mRawLaserData.size(); i++)
    {
        //cout << mRawLaserData[i].mZ << ", ";
        //cvPoint(mRawLaserData[i].mX, mRawLaserData[i].mY);
        /*x = (mRawLaserData[i].mX*sin(mRawLaserData[i].mZ))*30;
        y = (mRawLaserData[i].mX*cos(mRawLaserData[i].mZ))*30;
        cvCircle(mDisplayLaser, cvPoint(x+320, y+240), 1, cvScalar(0,255,0), 1);*/
        x = (mRawLaserData[i].mX*(0) - mRawLaserData[i].mY*(-1))*500;
        y = (mRawLaserData[i].mX*(-1) + mRawLaserData[i].mY*(0))*500;

        //if(sqrt(mRawLaserData[i].mX*mRawLaserData[i].mX - mRawLaserData[i].mY*mRawLaserData[i].mY) > 0.1)
        {
            cvCircle(mDisplayLaser, cvPoint(x+320, y+470), 1, cvScalar(0,255,0), 1);
        }

        //cout << mRawLaserData[i].mZ << ", ";
    }
    //cout << endl;

    //std::cout << "mLaserDist: " << mLaserDist << std::endl;
    for(int i = 0; i < mLaserObjects.size(); i++)
    {
        x = (mLaserObjects[i].mX*(0) - mLaserObjects[i].mY*(-1))*500;
        y = (mLaserObjects[i].mX*(-1) + mLaserObjects[i].mY*(0))*500;
        CvFont font;
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 0.5,0.5,0,1);

        //std::cout << "mCurrLaserDist: " << mCurrLaserDist << std::endl;
        if(mCurrLaserDist > 0)
        {
            cvLine(mDisplayLaser, cvPoint(320, 470), cvPoint(x+320, y+470), cvScalar(0,0,255), 1);
            char dist[10];
            sprintf(dist, "Dist: %lf", mCurrLaserDist);
            cvPutText (mDisplayLaser, dist, cvPoint((x+320)/2, (y+470)/2), &font, cvScalar(255,255,0));
        }
        cvCircle(mDisplayLaser, cvPoint(x+320, y+470), mLaserObjects[i].mZ*500, cvScalar(0,0,255), 2);

        char radius[10];
        sprintf(radius, "Rad: %lf", mLaserObjects[i].mZ*39.37007874015748);
        cvPutText (mDisplayLaser, radius, cvPoint(x+320, y+470), &font, cvScalar(255,255,0));
    }

    //////////////////////////////////////////////////////
    // DO NOT USE THIS THIS WILL CRASH THE PROGRAM
    // WHEN IT IS IN AUTONOMOUS MODE
    //////
    //cvShowImage("LaserData", mDisplayLaser);
    //cvWaitKey(10);
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    // USE THIS
    Utility::Display::DisplayImage("LaserData", mDisplayLaser);
    ///////////////////////////////////////////////////////

    if (mState == DoPath || mState == PathSearch || mState == Leave)
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

/*
bool Training::GetPaths(std::vector<Path> &paths)
{
    if(mVTGary)
    {
        std::cout << "Basic Threshold and Segmentation" << std::endl;
        std::cout << std::endl;
        paths = mPathFinder.GetPaths(mCurrentYaw, mDWFrame, mDWProcFrame);
        return mPathDebounce.Bounce(paths.size() > 0);
    }
    else if(mVTDavid)
    {
        std::cout << "David's CV Functions" << std::endl;
        std::cout << std::endl;
        paths = mPathFinder.GetPathsDavid(mCurrentYaw, mDWFrame, mDWProcFrame);
        return mPathDebounce.Bounce(paths.size() > 0);
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

void Training::InitSearch()
{
    if(mBuoySearchStrafe == true)
    {
        mBuoyStrafeSearch.InitSearch(mBuoySearchStrafeTime, mBuoySearchStrafeSpeed, mBuoyStrafeForwardTime, mBuoyStrafeForwardSpeed, mBuoyStrafeSmoothSpeed, mBuoyStrafeInitDirection);
    }
    else if(mBuoySearchSweep == true)
    {
        mBuoySweepSearch.InitSearch(mFixedYaw, mBuoySearchSweepDegrees, mBuoySearchSweepSpeed, mBuoySweepForwardTime, mBuoySweepForwardSpeed, mBuoySweepInitDirection, mBuoySweepSmoothSpeed);
    }
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO INITIALIZE!" << std::endl;
    }
}

void Training::SearchStep()
{
    if(mBuoySearchStrafe == true)
    {
        mBuoyStrafeSearch.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
    }
    else if(mBuoySearchSweep == true)
    {
        mBuoySweepSearch.SearchStep(mDesiredYaw, mDesiredAxialThrust);
        mDesiredLateralThrust = 0;
    }
    else
    {
        std::cout << "NO SEARCH TYPE EXISTS TO STEP THROUGH!" << std::endl;
    }
}

std::string Training::GetStateName(State state)
{
    switch (state)
    {
        case GoToPathDepth:
            return "GoToPathDepth";

        case GoToBuoyDepth:
            return "GoToBuoyDepth";

        case GoToLeaveDepth:
            return "GoToLeaveDepth";

        case BuoySearch:
            return "Search";

        case InitAttack:
            return "InitAttack";

        case Attack:
            return "Attack";

        case LockAttack:
            return "LockAttack";

        case Unhit:
            return "Unhit";

        case UnhitLeave:
            return "UnhitLeave";

        case Leave:
            return "Leave";

        case PathSearch:
            return "PathSearch";

        case DoPath:
            return "DoPath";

        case ExitPath:
            return "ExitPath";

        case LeaveMission:
            return "Exit";

        default:
            return "ERROR::DNE";
    }
}
//calculates a relative angle to buoy with pos being to right, neg being to left.
//used to add to current heading. should be made more generic as other missions use this.
double Training::AngleToBuoy(const Zebulon::Vision::Return& buoy)
{
    CxUtils::Point3D bottomcenter;
    CxUtils::Point3D buoycenter;
    if(mFWFrame == NULL) return 0;
    bottomcenter.mX = mFWFrame->width/2.0;
    bottomcenter.mY = mFWFrame->height;

    buoycenter.mX = buoy.mCenterI;
    buoycenter.mY = buoy.mCenterJ;

    CxUtils::Segment3D pathtobuoy(bottomcenter,buoycenter);
    double angle = pathtobuoy.GetAngleXY(false);
    angle = CxUtils::Orientation::AddToAngle(angle,CxUtils::CX_HALF_PI);
    std::cout << "angle to buoy: " <<  CxUtils::CxToDegrees(angle ) << std::endl;
    return angle;

}
/* End of file */
