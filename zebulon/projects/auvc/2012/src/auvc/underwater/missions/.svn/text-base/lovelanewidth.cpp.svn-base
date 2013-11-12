#include "auvc/underwater/missions/lovelanewidth.h"

using namespace Zebulon;
using namespace Underwater;

const std::string LoveLaneWidth::NextMission = "Torpedo";

LoveLaneWidth::LoveLaneWidth()
{
    ID = "LoveLane";
    
    mForwardFrame = NULL;
    mDownwardFrame = NULL;
    mProcessedImage = NULL;
    
    mHorizontileDebounce.Initialize(2, true);
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(4, true);
    mLostTargetDebounce.Initialize(3, true);
}

LoveLaneWidth::~LoveLaneWidth()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mProcessedImage);
}

int LoveLaneWidth::Init()
{
    mState = Find;
    
    int temp;
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Default@Timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw))
    {
        printf("ERROR::LoveLaneWidth: Couldn't get Compass data");
        return 1;
    }
    
    // Default depth
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Default@Depth", mDesired.mDepth);
    
    mPathFinder.Initialize();
    mPathSearchTimeout.Initialize(15);
    
    mLoveLaneObject.Initialize("LoveLane2");
    
    mMaxWidth = 0;
    
    return 0;
}

int LoveLaneWidth::ErrorCheck()
{
    return 0;
}

int LoveLaneWidth::FetchProcess()
{
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Default@Depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Default@Pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Default@Roll", mDesired.mRoll);
    
    double num, denom;
    mXmlConf.GetVar("Underwater.LoveLaneWidth.LatPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.DepthPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.DepthPixelScale@Denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.YawPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.YawPixelScale@Denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@IThresh", mIThresh);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@JThresh", mJThresh);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@TargetWidth", mTargetWidth);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@AngleThresh", mAngleThresh);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@DepthThresh", mDepthThresh);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@HedgePassedJ", mHedgePassedJ);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Buoys@VerticalOffset", mVerticalOffset);
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Advance@Thrust", mAdvanceThrust);
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.LockHeading@Thrust", mLockHeadingThrust);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.LockHeading@Duration", mLockHeadingDuration);
    
    //mXmlConf.GetVar("Underwater.LoveLaneWidth.Depth@Offset", mDepthOffset);
    mDepthOffset = mDefaultDepth;
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Search@SmoothSpeed", mSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.PathSearch@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.PathSearch@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.PathSearch@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.PathSearch@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.PathSearch@SmoothSpeed", mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Path@PathExitTime", mPathExitTime);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Path@ExitPathThrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.LoveLaneWidth.Path@PathSearchDuration", mPathSearchDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::LoveLaneWidth: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::LoveLaneWidth: Couldn't get depth data");
        return 1;
    }
    
    // Forward
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::LoveLaneWidth: Couldn't get forward camera data");
        return 1;
    }
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
    mLoveLaneObject.GetVisionReturn(mForwardFrame, mHorizontileReturn, mProcessedImage);
    if (mHorizontileDebounce.Bounce(mHorizontileReturn.mValid))
    {
        if (mHorizontileReturn.mMaxI - mHorizontileReturn.mMinI > mMaxWidth)
        {
            mMaxWidth = mHorizontileReturn.mMaxI - mHorizontileReturn.mMinI;
        }
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::LoveLaneWidth: Couldn't get downward camera data");
        return 1;
    }
    //cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    if (mState == DoPath || mState == PathSearch || mState == LockHeading)
        Vision::VisionUtil::ResetImage(mProcessedImage);
    
    //mHedgePassedJ
    cvLine( mProcessedImage, cvPoint(100, mHedgePassedJ), cvPoint(700, mHedgePassedJ), CV_RGB(255,0,255), 2);
    
    return 0;
}

std::string LoveLaneWidth::ExitEarly()
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

std::string LoveLaneWidth::Execute()
{
    std::vector<Path> paths;
    
    switch (mState)
    {
        // Figure out what to do
        case Find:
            
            mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed,mPathSearchSmoothSpeed);
            mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);
            
            mState = PathSearch;
            
            mDesired.mAxialThrust = 0;
            mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDefaultDepth;
            
            std::cout << "State: " << "LoveLaneWidth" << "::Find" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Search for the LoveLaneWidth
        case Search:
            if (mHorizontileDebounce.GetDebounced())
            {
                mState = Advance;
            }
            
            mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mStartYaw;
            
            std::cout << "State: " << "LoveLaneWidth" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Go to the depth offset and advance
        case Advance:
            if (mHorizontileDebounce.GetDebounced())
            {
                mCenterI = mHorizontileReturn.mCenterI;
                mCenterJ = mHorizontileReturn.mCenterJ - mVerticalOffset;
                
                mLostTargetDebounce.Miss();
            }
            else if (mLostTargetDebounce.Hit())
            {
                mCenterI = mForwardFrame->width/2;
                mCenterJ = mForwardFrame->height/2;
                
                mState = Search;
            }
            
            if (mCenteredDebounce.Bounce(mMaxWidth > mTargetWidth && fabs(mCenterI - mForwardFrame->width/2) < mIThresh))
            {
                mLockHeadingTimer.Initialize(mLockHeadingDuration);
                mLockHeadingTimer.Start();
                mPathDebounce.Miss();
                mState = LockHeading;
            }
            
            mDesired.mAxialThrust = mAdvanceThrust;
            mDesired.mYaw = mCurrent.mYaw + (mCenterI - mProcessedImage->width/2)*mYawPixelScale;
            //mDesired.mLateralThrust = (mCenterI - mProcessedImage->width/2)*mLatPixelScale;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDepthOffset;
            
            std::cout << "State: " << "LoveLaneWidth" << "::Advance" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesLateral: " << mDesired.mLateralThrust
                    << " I: " << mCenterI
                    << " Seen: " << mHorizontileDebounce.GetDebounced()
                    << std::endl;
            break;
            
        // Drive through the LoveLaneWidth
        case LockHeading:
            // add or sees path
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage);
            if (mLockHeadingTimer.IsFinished() || mPathDebounce.Bounce(paths.size() > 0))
            {
                mState = Exit;
            }
            
            mDesired.mAxialThrust = mLockHeadingThrust;
            mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDepthOffset;
            
            std::cout << "State: " << "LoveLaneWidth" << "::LockHeading" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Look for path or buoys
        case PathSearch:
            // minimum path time
            if (mHorizontileDebounce.GetDebounced())
            {
                //mState = DoLineup;
                mState = Advance;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mStartYaw;
            }
            else if (mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage).size() > 0)
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
            
            std::cout << "State: " << "LoveLaneWidth" << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Locate & lock onto the path
        case DoPath:
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
            
            std::cout << "State: " << "LoveLaneWidth" << "::DoPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        case ExitPath:
        
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mLateralThrust = 0;
            
            if(mPathTimer.IsFinished())
            {
                mState = Search;
            }
            else if(mHorizontileDebounce.GetDebounced())
            {
                mState = Advance;
            }
            else
            {
                mState = ExitPath;
            }
            
            std::cout << "State: " << "LoveLaneWidth" << "::ExitPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Proceed to the next state
        case Exit:
            Utility::Display::CloseAllWindows();
            return NextMission;
    }
    
    std::cout << " Max Width: " << mMaxWidth << std::endl;
    
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
