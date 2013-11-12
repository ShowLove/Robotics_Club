#include "auvc/underwater/missions/buoysclean.h"

using namespace Zebulon;
using namespace Underwater;

const std::string BuoysClean::NextMission = "LoveLane";

BuoysClean::BuoysClean()
{
    ID = "Buoys";
    
    mForwardFrame = NULL;
    mDownwardFrame = NULL;
    mFWProcessedImage = NULL;
    mDWProcessedImage = NULL;
}

BuoysClean::~BuoysClean()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mFWProcessedImage);
    cvReleaseImage(&mDWProcessedImage);
    
}

int BuoysClean::ErrorCheck()
{
    return 0;
}

int BuoysClean::Init()
{
    //mYawFix = 80;
    // Initial state
    mState = PathSearch;
    //mState = Search;
    
    // Initialize debounces
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Initialize(2, false);
    }
    
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(3, true);
    mLostTargetDebounce.Initialize(5, true);
    
    mPathFinder.Initialize();
    
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed, mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.BuoysClean.Search@sweep_degrees", mSearchSweepDegrees);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@forward_time", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@forward_speed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@smooth_speed", mSearchSmoothSpeed);
    mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);
    
    int temp;
    mXmlConf.GetVar("Underwater.BuoysClean.Default@timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    // Find what buoys to hit
    mXmlConf.GetVar("Underwater.BuoysClean.BuoyToHit@red", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Red;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Red;
    }
    
    mXmlConf.GetVar("Underwater.BuoysClean.BuoyToHit@green", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Green;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Green;
    }
    
    mXmlConf.GetVar("Underwater.BuoysClean.BuoyToHit@yellow", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Yellow;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Yellow;
    }
    
    // Default depth
    mXmlConf.GetVar("Underwater.BuoysClean.Default@depth", mDesired.mDepth);
    mDefaultDepth = mDesired.mDepth;
    
    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::BuoysClean: Couldn't get Compass data");
        return 1;
    }
    
    mAttackingBuoy = 0;
    
    return 0;
}

int BuoysClean::FetchProcess()
{
    mXmlConf.GetVar("Underwater.BuoysClean.Default@depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.BuoysClean.Default@pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.BuoysClean.Default@roll", mDesired.mRoll);
    
    double num, denom;
    mXmlConf.GetVar("Underwater.BuoysClean.LatPixelScale@num", num);
    mXmlConf.GetVar("Underwater.BuoysClean.LatPixelScale@denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysClean.DepthPixelScale@num", num);
    mXmlConf.GetVar("Underwater.BuoysClean.DepthPixelScale@denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysClean.YawPixelScale@num", num);
    mXmlConf.GetVar("Underwater.BuoysClean.YawPixelScale@denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysClean.AxialPixelScale@num", num);
    mXmlConf.GetVar("Underwater.BuoysClean.AxialPixelScale@denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysClean.Targets@I_thresh", mIThresh);
    mXmlConf.GetVar("Underwater.BuoysClean.Targets@J_thresh", mJThresh);
    mXmlConf.GetVar("Underwater.BuoysClean.Targets@area", mTargetArea);
    mXmlConf.GetVar("Underwater.BuoysClean.Targets@area_thresh", mAreaCloseThresh);
    
    mXmlConf.GetVar("Underwater.BuoysClean.Search@sweep_degrees", mSearchSweepDegrees);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@forward_time", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@forward_speed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.Search@smooth_speed", mSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysClean.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.BuoysClean.Path@path_exit_time", mPathExitTime);
    mXmlConf.GetVar("Underwater.BuoysClean.Path@exit_path_thrust", mExitPathThrust);
    
    mXmlConf.GetVar("Underwater.BuoysClean.Leave@depth", mLeaveDepth);
    mXmlConf.GetVar("Underwater.BuoysClean.Leave@duration", mLeaveDuration);
    
    mXmlConf.GetVar("Underwater.BuoysClean.LockAttack@duration", mLockAttackDuration);
    mXmlConf.GetVar("Underwater.BuoysClean.LockAttack@thrust", mLockAttackThrust);
    
    mBuoyObject[Red].Initialize("RedBouy");
    mBuoyObject[Yellow].Initialize("YellowBouy");
    mBuoyObject[Green].Initialize("GreenBouy");
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::BuoysClean: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::BuoysClean: Couldn't get depth data");
        return 1;
    }
    
    // Forward Camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::BuoysClean: Couldn't get forward camera data");
        return 1;
    }
    
    if(mFWProcessedImage == NULL)
    {
        mFWProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    cvCopy(mForwardFrame,mFWProcessedImage);
    /*
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Bounce(mBuoyObject[i].GetVisionReturn(mForwardFrame, mReturn[i], mFWProcessedImage));
    }*/
    if (mState == Search || mState == Attack || mState == Unhit || mState == LockAttack || mState == Leave)
    {
        mDebounce[mBuoyToAttack[mAttackingBuoy]].Bounce(mBuoyObject[mBuoyToAttack[mAttackingBuoy]].GetVisionReturn(mForwardFrame, mReturn[mBuoyToAttack[mAttackingBuoy]], mFWProcessedImage));
    }
    
    // Downward
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::BuoysClean: Couldn't get downward camera 2 data");
        return 1;
    }
    
    if(mDWProcessedImage == NULL)
    {
        mDWProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    cvCopy(mDownwardFrame, mDWProcessedImage);
    
    return 0;
}

std::string BuoysClean::ExitEarly()
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

std::string BuoysClean::Execute()
{
    static double _timestamp = 0;
    
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState)<< " time: " << (CxUtils::GetTimeMs() - _timestamp) << std::endl;
    _timestamp = CxUtils::GetTimeMs();
    
    std::vector<Path> paths;
    
    switch (mState)
    {
        case Search:
            mDesired.mDepth = mDefaultDepth;
            mDesired.mLateralThrust = 0;
            
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mState = Attack;
            }
            else
            {
                mSearchPattern.SearchStep(mDesired.mYaw, mDesired.mAxialThrust);
            }
            break;
        
        case Attack:
            
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mCenterI = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterI;
                mCenterJ = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterJ;
                mArea = mReturn[mBuoyToAttack[mAttackingBuoy]].mArea;
                
                if (mCenteredDebounce.Bounce( fabs(mCenterI - mFWProcessedImage->width/2) < mIThresh &&
                     fabs(mCenterJ - mFWProcessedImage->height/2) < mJThresh &&
                     (mTargetArea - mArea) < mAreaCloseThresh ))
                {
                    mState = LockAttack;
                    mLockAttackTimer.Initialize(mLockAttackDuration);
                    mLockAttackTimer.Start();
                    
                    mLockAttackYaw = mCurrent.mYaw;
                    mLockAttackDepth = mCurrent.mDepth;
                }
                
                mLostTargetDebounce.Miss();
            }
            else
            {
                if (mLostTargetDebounce.Hit())
                {
                    mState = Search;
                    mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);
                }
            }
            
            mDesired.mYaw = mCurrent.mYaw + (mCenterI - mFWProcessedImage->width/2)*mYawPixelScale;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2)*mDepthPixelScale;
            mDesired.mAxialThrust = (mTargetArea + 1000 - mArea)*mAxialPixelScale;
            
            std::cout << " i: " << mCenterI << " j: " << mCenterJ << " area: " << mArea << std::endl;
            
            break;
        
        case LockAttack:
            mDesired.mAxialThrust = mLockAttackThrust;
            mDesired.mYaw = mLockAttackYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mLockAttackDepth;
            
            if (mLockAttackTimer.IsFinished())
            {
                mState = Unhit;
                mLockAttackTimer.Initialize(mLockAttackDuration);
                mLockAttackTimer.Start();
            }
            break;
        
        case Unhit:
            mDesired.mAxialThrust = -mLockAttackThrust;
            mDesired.mYaw = mLockAttackYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mLockAttackDepth;
            
            if (mLockAttackTimer.IsFinished())
            {
                mDesired.mYaw = mYawFix;
                if (mAttackingBuoy)
                {
                    mState = Leave;
                    mPathDebounce.Miss();
                    mLockAttackTimer.Initialize(mLeaveDuration);
                    mLockAttackTimer.Start();
                }
                else
                {
                    mAttackingBuoy++;
                    mState = Search;
                    
                    mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);
                }
            }
            break;
        
        case Leave:
            mDesired.mDepth = mLeaveDepth;
            mDesired.mAxialThrust = mLockAttackThrust;
            mDesired.mYaw = mYawFix;
            
            if (mLockAttackTimer.IsFinished() || mPathDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0))
            {
                mDesired.mDepth = mDefaultDepth;
                mState = Exit;
            }
            
            break;
        
        case PathSearch:
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mYawFix;
            
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage);
            if (paths.size() > 0)
            {
                mState = DoPath;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            }
            break;
        
        case DoPath:
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrent, mDesired)))
                {
                    mState = ExitPath;
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mDesired.mYaw;
                    
                    cvCircle(mDWProcessedImage, cvPoint(mDWProcessedImage->width/2,mDWProcessedImage->height/2), 200, cvScalar(0,255,0), 4);
                }
            }
            else
            {
                mPathDebounce.Miss();
            }
            break;
        
        case ExitPath:
            mDesired.mYaw = mYawFix;
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mDepth = mDefaultDepth;
            mDesired.mLateralThrust = 0;
            
            if (mPathTimer.IsFinished())
            {
                mState = Search;
                
                mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);
            }
            break;
        
        case Exit:
            Utility::Display::CloseAllWindows();
            return NextMission;
        
        default:
            std::cout << "ERROR::" << ID << " state " << mState << " does not exist" << std::endl;
            break;
    }
    
    std::cout << " Yaw " << mCurrent.mYaw << " -> " << mDesired.mYaw
              << " Depth " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
              << " Axial " << mDesired.mAxialThrust << " Lat " << mDesired.mLateralThrust << std::endl
              << std::endl;
    
    if (mState == DoPath || mState == PathSearch || Leave)
    {
        AI::Utility::HeadingDisplay(mDWProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mDWProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
        AI::Utility::ThrustDisplay(mDWProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    }
    else
    {
        AI::Utility::HeadingDisplay(mFWProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mFWProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
        AI::Utility::ThrustDisplay(mFWProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    }
    
    Utility::Display::DisplayImage("Post Processing FW",mFWProcessedImage);
    Utility::Display::DisplayImage("Post Processing DW",mDWProcessedImage);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}

std::string BuoysClean::GetStateName(State state)
{
    switch (state)
    {
        case Search:
            return "Search";
        
        case Attack:
            return "Attack";
        
        case LockAttack:
            return "LockAttack";
        
        case Unhit:
            return "Unhit";
        
        case Leave:
            return "Leave";
        
        case PathSearch:
            return "PathSearch";
        
        case DoPath:
            return "DoPath";
        
        case ExitPath:
            return "ExitPath";
        
        case Exit:
            return "Exit";
        
        default:
            return "ERROR::DNE";
    }
}

/* End of file */
