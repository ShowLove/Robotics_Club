#include "auvc/underwater/missions/lovelane.h"

//#define LOVELANE_GVISION
#define LOVELANE_DVISION

using namespace Zebulon;
using namespace Underwater;

const std::string LoveLane::NextMission = "Torpedo";

LoveLane::LoveLane()
{
    ID = "LoveLane";
    
    mForwardFrame = NULL;
    mDownwardFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mProcessedImage = NULL;
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
    
    mHorizontileDebounce.Initialize(2, true);
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(10, true);
    mLostTargetDebounce.Initialize(8, true);
}

LoveLane::~LoveLane()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mSegFrame);
    cvReleaseImage(&mThreshFrame);
    cvReleaseImage(&mProcessedImage);
    
    delete mSegment;
    delete mThreshold;
}

int LoveLane::Init()
{
    mState = PathSearch;
    
    int temp;
    mXmlConf.GetVar("Underwater.LoveLane.Default@Timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw))
    {
        printf("ERROR::LoveLane: Couldn't get Compass data");
        return 1;
    }
    
    mYawFix = mStartYaw;
    
    // Default depth
    mXmlConf.GetVar("Underwater.LoveLane.Default@Depth", mDesired.mDepth);
    
    mPathFinder.Initialize();
    mPathSearchTimeout.Initialize(15);
    
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@SmoothSpeed", mPathSearchSmoothSpeed);
    mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed, mPathSearchSmoothSpeed);
    
    mLoveLaneObject.Initialize("LoveLane");
    
    return 0;
}

int LoveLane::ErrorCheck()
{
    return 0;
}

int LoveLane::FetchProcess()
{
    mXmlConf.GetVar("Underwater.LoveLane.Default@Depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.LoveLane.Default@Pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.LoveLane.Default@Roll", mDesired.mRoll);
    
    mXmlConf.GetVar("Underwater.LoveLane.Target@Red", mTarget.mRed);
    mXmlConf.GetVar("Underwater.LoveLane.Target@Green", mTarget.mGreen);
    mXmlConf.GetVar("Underwater.LoveLane.Target@Blue", mTarget.mBlue);
    mXmlConf.GetVar("Underwater.LoveLane.TargetThresh@Red", mTarget.mRedRange);
    mXmlConf.GetVar("Underwater.LoveLane.TargetThresh@Green", mTarget.mGreenRange);
    mXmlConf.GetVar("Underwater.LoveLane.TargetThresh@Blue", mTarget.mBlueRange);
    
    double num, denom;
    mXmlConf.GetVar("Underwater.LoveLane.LatPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLane.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLane.DepthPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLane.DepthPixelScale@Denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLane.YawPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLane.YawPixelScale@Denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@MinArea", mMinArea);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@MaxArea", mMaxArea);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@IThresh", mIThresh);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@JThresh", mJThresh);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@TargetArea", mTargetArea);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@AngleThresh", mAngleThresh);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@DepthThresh", mDepthThresh);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@HedgePassedJ", mHedgePassedJ);
    mXmlConf.GetVar("Underwater.LoveLane.Buoys@LeftVerticalOffset", mVerticalOffset);
    
    mXmlConf.GetVar("Underwater.LoveLane.Advance@Thrust", mAdvanceThrust);
    mXmlConf.GetVar("Underwater.LoveLane.ReAlign@YawThresh", mYawThresh);
    
    mXmlConf.GetVar("Underwater.LoveLane.LockHeading@Thrust", mLockHeadingThrust);
    mXmlConf.GetVar("Underwater.LoveLane.LockHeading@Duration", mLockHeadingDuration);
    
    //mXmlConf.GetVar("Underwater.LoveLane.Depth@Offset", mDepthOffset);
    mDepthOffset = mDefaultDepth;
    
    mXmlConf.GetVar("Underwater.LoveLane.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.LoveLane.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.LoveLane.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.Search@SmoothSpeed", mSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.LoveLane.PathSearch@SmoothSpeed", mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.LoveLane.Path@PathExitTime", mPathExitTime);
    mXmlConf.GetVar("Underwater.LoveLane.Path@ExitPathThrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.LoveLane.Path@PathSearchDuration", mPathSearchDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::LoveLane: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::LoveLane: Couldn't get depth data");
        return 1;
    }
    
    // Forward
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::LoveLane: Couldn't get forward camera data");
        return 1;
    }
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
#ifdef LOVELANE_GVISION
    mSegment->Setup(mTarget, 50, 50);
    mThreshold->Setup(mTarget);
    
    mThreshold->Process(mForwardFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    Vision::VisionUtil::CombineImages(mSegFrame, mProcessedImage);
    
    bool gotten = false;
    mSegment->GetMaxPosition(mTempReturn);
    mLeftBound = mProcessedImage->width;///2.0;
    for (int i=1; mTempReturn.mValid; i++, mSegment->GetMaxPosition(mTempReturn, i))
    {
        if (fabs(mTempReturn.mAngle) >= 90 - mAngleThresh)
        {
            mHorizontileDebounce.Bounce(mTempReturn.mValid && mTempReturn.mArea >= mMinArea && mTempReturn.mArea <= mMaxArea);
            mHorizontileReturn = mTempReturn;
            gotten = true;
        }
        else if( i < 2 && fabs(mTempReturn.mAngle) <= mAngleThresh)
        {
            mVerticalDebounce.Bounce(mTempReturn.mValid && mTempReturn.mArea >= mMinArea && mTempReturn.mArea <= mMaxArea);
            mVerticalReturn = mTempReturn;
            if (mTempReturn.mCenterI < mLeftBound)
            {
                mLeftBound = mTempReturn.mCenterI;
            }
        }
        else if(i > 2)
        {
            break;
        }
    }
    
    if (!gotten)
    {
        mHorizontileDebounce.Miss();
    }
#endif // LOVELANE_GVISION
    
#ifdef LOVELANE_DVISION
    mLoveLaneObject.GetVisionReturn(mForwardFrame, mHorizontileReturn, mVerticalReturn, mProcessedImage);
    mHorizontileDebounce.Bounce(mHorizontileReturn.mValid);
    if (mVerticalDebounce.Bounce(mVerticalReturn.mValid))
    {
        mLeftBound = mVerticalReturn.mCenterI;
    }
#endif // LOVELANE_DVISION
    
    // Figure out if it is in the L or J pattern
    if (mVerticalDebounce.GetDebounced() && mHorizontileDebounce.GetDebounced())
    {
        // +1 == L, -1 == J
        mLaneOrientation = (mVerticalReturn.mCenterI < mHorizontileReturn.mCenterI)? +1 : -1;
    }
    
    if(mState == DoPath || mState == PathSearch) //mState == Find || 
    {
        if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
        {
            printf("ERROR::LoveLane: Couldn't get downward camera data");
            return 1;
        }
        //cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
        
        if(mProcessedImage == NULL)
        {
            mProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mProcessedImage);
    }
    
    //mHedgePassedJ
    cvLine( mProcessedImage, cvPoint(100, mHedgePassedJ), cvPoint(700, mHedgePassedJ), CV_RGB(255,0,255), 2);
    
    return 0;
}

std::string LoveLane::ExitEarly()
{
    if (mStateTimeout.IsFinished())
    {
        double tmp;
        mGlobalInfo->GetInfo(GlobalInfo::ExitEarlyToFinalMission, tmp);
        if (tmp != 0)
        {
            return Underwater::FinalMission;
        }
        
        mGlobalInfo->SetInfo(GlobalInfo::ExitEarlyToFinalMission, 1);
        return NextMission;
    }
    else
    {
        return "KeepRunning";
    }
}

std::string LoveLane::Execute()
{
    std::vector<Path> paths;
    
    switch (mState)
    {
        // Figure out what to do
        /*case Find:
            
            mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed,mPathSearchSmoothSpeed);
            mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);
            
            mState = PathSearch;
            
            mDesired.mAxialThrust = 0;
            mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDefaultDepth;
            
            std::cout << "State: " << "LoveLane" << "::Find" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;*/
            
        // Search for the LoveLane
        case Search:
            if (mHorizontileDebounce.GetDebounced() && mVerticalDebounce.GetDebounced())
            {
                //mState = GetDepth;
                mState = Advance;
            }
            //else if(mPathFinder.GetPaths(mDownwardFrameB, mProcessedImage, mDownwardFrameF).size() > 0)
            //{
            //    mState = DoPath;
            //}
            
            mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mStartYaw;
            
            std::cout << "State: " << "LoveLane" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Find the depth of the LoveLane
        /*case GetDepth:
            if (mHorizontileDebounce.GetDebounced())
            {
                mCenterI = mHorizontileReturn.mCenterI;
                mCenterJ = mHorizontileReturn.mCenterJ;
                
                mLostTargetDebounce.Miss();
            }
            else if (mLostTargetDebounce.Hit())
            {
                mCenterI = mProcessedImage->width/2.0;
                
                mState = Search;
            }
            
            mDesired.mAxialThrust = 0;
            mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = (mCenterI - mProcessedImage->width/2)*mLatPixelScale;
            mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mProcessedImage->height/2)*mDepthPixelScale;
            
            if (mCenteredDebounce.Bounce(fabs(mCenterI - mProcessedImage->width/2) < mIThresh && fabs(mCenterJ - mProcessedImage->height/2) < mJThresh))
            {
                mDepthOffset += mCurrent.mDepth;
                mState = Advance;
                mCenteredDebounce.Miss();
            }
            
            std::cout << "State: " << "LoveLane" << "::GetDepth" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesLateral: " << mDesired.mLateralThrust
                    << std::endl;
            break;*/
            
        // Go to the depth offset and advance
        case Advance:
            if (mHorizontileDebounce.GetDebounced() && mVerticalDebounce.GetDebounced())
            {
                mCenterI = mVerticalReturn.mCenterI + mLaneOrientation*mVerticalOffset;
                mCenterJ = mHorizontileReturn.mCenterJ;
                
                mLostTargetDebounce.Miss();
            }
            else if (mVerticalDebounce.GetDebounced())
            {
                mCenterI = mVerticalReturn.mCenterI + mLaneOrientation*mVerticalOffset;
                mCenterJ = mForwardFrame->height/2;
            }
            else if (mHorizontileDebounce.GetDebounced())
            {
                mCenterI = mHorizontileReturn.mCenterI;
                mCenterJ = mHorizontileReturn.mCenterJ;
            }
            else if (mLostTargetDebounce.Hit())
            {
                mCenterI = mForwardFrame->width/2;
                mCenterJ = mForwardFrame->height/2;
                
                mState = Search;
            }
            
            if (mCenterJ >= mHedgePassedJ)
            {
                mDesired.mYaw = mStartYaw;
                mState = ReAlign;
            }
            
            mDesired.mAxialThrust = mAdvanceThrust;
            //mDesired.mYaw = mStartYaw;
            mDesired.mYaw = mCurrent.mYaw + (mCenterI - mProcessedImage->width/2)*mYawPixelScale;
            //mDesired.mLateralThrust = (mCenterI - mProcessedImage->width/2)*mLatPixelScale;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDepthOffset;
            
            std::cout << "State: " << "LoveLane" << "::Advance" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesLateral: " << mDesired.mLateralThrust
                    << " I: " << mCenterI
                    << " Seen: " << mHorizontileDebounce.GetDebounced()
                    << std::endl;
            break;
        
        // Line up with start yaw
        case ReAlign:
            // need true
            if(fabs(Zebulon::AI::Utility::AngleDiff(mCurrent.mYaw, mStartYaw)) <= mYawThresh)
            {
                mLockHeadingTimer.Initialize(mLockHeadingDuration);
                mLockHeadingTimer.Start();
                mState = LockHeading;
            }
            
            mDesired.mYaw = mStartYaw;
            mDesired.mDepth = mDepthOffset;
            
            if(Zebulon::AI::Utility::AngleDiff(mCurrent.mYaw, mStartYaw) > 0)
            {
                mDesired.mLateralThrust = -50;
            }
            else
            {
                mDesired.mLateralThrust = 50;
            }
            
            std::cout << "State: " << "LoveLane" << "::ReAlign" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                << " Desired Yaw: " << mDesired.mYaw
                << " Depth: " << mCurrent.mDepth
                << " DesLateral: " << mDesired.mLateralThrust
                << std::endl;
            break;
            
        // Drive through the LoveLane
        case LockHeading:
            // add or sees path
            if (mLockHeadingTimer.IsFinished())
            {
                mState = Exit;
            }
            
            mDesired.mAxialThrust = mLockHeadingThrust;
            mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDepthOffset;
            
            std::cout << "State: " << "LoveLane" << "::LockHeading" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Look for path or buoys
        case PathSearch:
            
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mStartYaw;
            
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage);
            if (paths.size() > 0)
            {
                mState = DoPath;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            }
            
            std::cout << "State: " << "LoveLane" << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Axial: " << mDesired.mAxialThrust
                    << " Lateral: " << mDesired.mLateralThrust
                    << "Path Num: " << paths.size()
                    << std::endl;
            
            break;
            
        case DoPath:
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrent, mDesired)))
                {
                    mState = ExitPath;
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mDesired.mYaw;
                    
                    cvCircle(mProcessedImage, cvPoint(mProcessedImage->width/2,mProcessedImage->height/2), 200, cvScalar(0,255,0), 4);
                }
            }
            else
            {
                mPathDebounce.Miss();
            }
            
            std::cout << "State: " << "LoveLane" << "::DoPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Axial: " << mDesired.mAxialThrust
                    << " Lateral: " << mDesired.mLateralThrust
                    << " Desired Yaw: " << mDesired.mYaw
                    << "Path Num: " << paths.size()
                    << std::endl;
            
            break;
            
        case ExitPath:
            mDesired.mYaw = mYawFix;
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mDepth = mDefaultDepth;
            mDesired.mLateralThrust = 0;
            
            if (mPathTimer.IsFinished())
            {
                mState = Search;
                
                mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);
            
                //mSearchPattern.InitSearch(mYawFix, mSearchSweepDegrees, mSearchSweepSpeed, mSearchForwardTime, mSearchForwardSpeed, 1, mSearchSmoothSpeed);
            }
            
            std::cout << "State: " << "LoveLane" << "::ExitPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Desired Yaw: " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Axial: " << mDesired.mAxialThrust
                    << " Lateral: " << mDesired.mLateralThrust
                    << " PathTimer: " << mPathTimer.TimeElapsed()
                    << std::endl;
            
            break;
            
            
            // minimum path time
            /*if (mHorizontileDebounce.GetDebounced())
            {
                //mState = DoLineup;
                mState = Advance;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mStartYaw;
            }
            else if (mPathFinder.GetPaths(mCurrent, mDownwardFrame, mProcessedImage).size() > 0)
            {
                mState = DoPath;
                
                mPathSearchTimeout.Initialize(20);
                mPathSearchTimeout.Start();
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mStartYaw;
            }
            else if (mPathSearchTimeout.IsFinished())
            {
                //double tmp;
                //mGlobalInfo->GetInfo(GlobalInfo::ExitEarlyToFinalMission, tmp);
                //if (tmp != 0)
                //{
                //    return Zebulon::Underwater::FinalMission;
                //}
                
                mState = Search;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mStartYaw;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
                mDesired.mYaw = mStartYaw;
            }
            
            std::cout << "State: " << "LoveLane" << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;*/
            
        // Look for path or buoys
        /*case DoLineup:
            if(mHorizontileDebounce.GetDebounced())
            {
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mCurrent.mDepth + (mHorizontileReturn.mCenterJ - mProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mYaw = (mHorizontileReturn.mCenterI - mProcessedImage->width/2)*mYawPixelScale;
            }
            
            if (fabs(mCenterI - mProcessedImage->width/2) < mIThresh)
            {
                mPathTimer.Initialize(mPathExitTime);
                mPathTimer.Start();
                mStartYaw = mCurrent.mYaw;
                mState = ExitPath;
            }
            
            std::cout << "State: " << "LoveLane" << "::DoLineup" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " DesiredYaw: " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;*/
            
        // Locate & lock onto the path
        /*case DoPath:
            // Lock on a path, then Exit
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrent, mDesired)))
                {
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mStartYaw = mCurrent.mYaw;
                    mState = ExitPath;
                }
            }
            else
            {
                mPathDebounce.Miss();
            }
            
            if (mPathSearchTimeout.IsFinished())
            {
                mState = Search;
            }
            
            std::cout << "State: " << "LoveLane" << "::DoPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;*/
            
        /*case ExitPath:
        
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mLateralThrust = 0;
            
            if(mPathTimer.IsFinished())
            {
                mState = Search;
            }
            else if(mHorizontileDebounce.GetDebounced())
            {
                //mState = GetDepth;
                mState = Advance;
            }
            else
            {
                mState = ExitPath;
            }
            
            std::cout << "State: " << "LoveLane" << "::ExitPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;*/
            
        // Proceed to the next state
        case Exit:
            Utility::Display::CloseAllWindows();
            return NextMission;
    }
    
    AI::Utility::HeadingDisplay(mProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Post Raw Forward",mForwardFrame);
    Utility::Display::DisplayImage("Post Processing FW",mProcessedImage);
    
    Utility::Display::DisplayImage("Post Raw Downward",mDownwardFrame);
    //Utility::Display::DisplayImage("Post Processing FW",mProcessedImage);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
