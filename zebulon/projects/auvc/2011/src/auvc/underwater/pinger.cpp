#include "auvc/underwater/pinger.h"

using namespace Zebulon;
using namespace Underwater;

const std::string Pinger::NextMission = "GNDN";

Pinger::Pinger()
{
    ID = "Pinger";
    
    mDownwardFrame = NULL;
    mProcessedImage = NULL;
    
    mCenteredDebounce.Initialize(10, true);
    mPingerDebounce.Initialize(3, false);
    mPingerGainDebounce.Initialize(3, false);
}

Pinger::~Pinger()
{
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mProcessedImage);
}

int Pinger::Init()
{
    mState = Listen;
    
    int temp;
    mXmlConf.GetVar("Underwater.Pinger.Default@timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw))
    {
        printf("ERROR::Pinger: Couldn't get Compass data");
        return 1;
    }
    
    mXmlConf.GetVar("Underwater.Pinger.Ignore@duration", mIgnoreDuration);
    mIgnoreTimer.Initialize(mIgnoreDuration);
    mIgnoreTimer.Start();
    
    // Default depth
    mXmlConf.GetVar("Underwater.Pinger.Default@depth", mDesired.mDepth);
    
    mGlobalInfo->SetInfo(GlobalInfo::PingerSelected, 0);
    
    mPathFinder.Initialize();
    
    mDestPinger = 0;
    mPingCount = 0;
    
    mFancyTimer.Initialize(.5);
    mFancyTimer.Start();
    
    mCurrent.mLateralThrust = 0;
    mCurrent.mAxialThrust = 0;
    
    return 0;
}

int Pinger::ErrorCheck()
{
    return 0;
}

int Pinger::FetchProcess()
{
    mXmlConf.GetVar("Underwater.Pinger.Default@depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.Pinger.Default@pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.Pinger.Default@roll", mDesired.mRoll);
    
    mXmlConf.GetVar("Underwater.Pinger.Default@depth", mDesired.mDepth);
    
    mXmlConf.GetVar("Underwater.Pinger.Ping@update_thresh", mPingerUpdateTimeThresh);
    mXmlConf.GetVar("Underwater.Pinger.Ping@min_dist", mMinPingerDist);
    mXmlConf.GetVar("Underwater.Pinger.Ping@max_dist", mMaxPingerDist);
    mXmlConf.GetVar("Underwater.Pinger.Ping@lock_time", mLockTime);
    mXmlConf.GetVar("Underwater.Pinger.Ping@gain_thresh", mPingerGainThresh);
    
    mXmlConf.GetVar("Underwater.Pinger.Actuate@travel_speed", mTravelSpeed);
    mXmlConf.GetVar("Underwater.Pinger.Actuate@grab_depth", mGrabDepth);
    
    ///// Compass /////////////////////////////////////////////////////
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::Pinger: Couldn't get compass data");
        return 1;
    }
    
    ///// Depth ///////////////////////////////////////////////////////
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::Pinger: Couldn't get depth data");
        return 1;
    }
    
    ///// Downward Camera /////////////////////////////////////////////
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::Pinger: Couldn't get downward camera data");
        return 1;
    }
    cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
    ///// Pinger //////////////////////////////////////////////////////
    if(!mGlobalInfo->GetInfo(GlobalInfo::PingerYaw, mPingerYaw))
    {
        mPingerYaw=mPreviousPingerYaw;
    }
    else
    {
        mPreviousPingerYaw=mPingerYaw;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::PingerUpdateTime, mPingerUpdateTime))
    {
        printf("ERROR::Pinger: Couldn't get pinger update time\n");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::PingerDistance, mPingerDistance))
    {
        printf("ERROR::Pinger: Couldn't get pinger distance\n");
        return 1;
    }
    
    if (!mGlobalInfo->GetInfo(GlobalInfo::PingerGain, mPingerGain))
    {
        printf("ERROR::Pinger: Couldn't get pinger gain\n");
        return 1;
    }
    
    return 0;
}

std::string Pinger::ExitEarly()
{
    return "KeepRunning";
}

std::string Pinger::Execute()
{
    std::cout << "===============================================" << std::endl
            << ID << "::" << std::endl;
    
    std::vector<Path> paths;
   
 
    mDesired.mDepthEnable = true;
    switch (mState)
    {
        // Listen for the pinger
        case Listen:
            
            std::cout << "Listening, Got (" << mPingCount << ") Pings" << std::endl;
            
            mDesired.mDepth = mDefaultDepth;
            
            //XML this up!
            mDesired.mYaw = -10;
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = 0;
            
            //if (mPingerDebounce.Bounce( (CxUtils::GetTimeSeconds() - mPingerUpdateTime) < mPingerUpdateTimeThresh ))
            if (mPingerPreviousUpdateTime < mPingerUpdateTime)
            {
                mPingCount++;
                mPingerPreviousUpdateTime = mPingerUpdateTime;
            }
            
            if (mPingCount > 3)
            {
                if (mIgnoreTimer.IsFinished() && mPingerGainDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage).size() > 0))
                {
                    mState = AlignToPayload;
                    /*
                    if (mDestPinger)
                    {
                        mState = Surface;
                    }
                    else
                    {
                        mState = AlignToPayload;
                    }*/
                }
                else
                {
                    mDesired.mYaw = mPingerLockYaw = mPingerYaw;
                    mState = GoToPinger;
                    mLockTimer.Initialize(mLockTime);
                    mLockTimer.Start();
                }
            }
            
            break;
        
        // Go to the designated pinger
        case GoToPinger:
            mDesired.mDepth = mDefaultDepth;
            /*
            if ((CxUtils::GetTimeSeconds() - mPingerUpdateTime) > mPingerUpdateTimeThresh)
            {
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                std::cout << "Pinger timeout" << std::endl;
                mState = Listen;
            }
            else*/
            {
                if (fabs(CxUtils::Orientation::AngleDiff(mCurrent.mYaw, mPingerYaw, false)) < 10)
                {
                    mDesired.mYaw = mPingerLockYaw;
                    mDesired.mAxialThrust = mTravelSpeed;
                    mDesired.mLateralThrust = 0;
                    std::cout << "Pinger locked" << std::endl;
                }
                else
                {
                    mDesired.mYaw = mPingerLockYaw;
                    mDesired.mAxialThrust = 0;
                    mDesired.mLateralThrust = 0;
                    std::cout << "Pinger found, turning to face" << std::endl;
                }
            }
            
            if (mLockTimer.IsFinished())
            {
                mPingerPreviousUpdateTime = mPingerUpdateTime;
                mPingCount = 0;
                mState = Listen;
            }
            
            //if (mPingerDistance > mMaxPingerDist || mPingerDistance < mMinPingerDist)
            //{
                //mState = AlignToPayload;
            //}
            
            if (mIgnoreTimer.IsFinished() && mPingerGainDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage).size() > 0))
            {
                mState = AlignToPayload;
                //mGlobalCommand->Surface();
            }
            
            break;
        
        // Line up with the payload before attempting to pick it up
        case AlignToPayload:
        {
            mDesired = mCurrent;
            mDesired.mDepth = mDefaultDepth;
            
            std::vector<Path> paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedImage);
            if (mCenteredDebounce.Bounce(paths.size() > 0 && mPathFinder.StepPath(&(paths.front()), mCurrent, mDesired)))
            {
                mState = Surface;
            }
            
            //mDesired.mYaw = CxUtils::Orientation::AddToAngle(mDesired.mYaw, 90, false);
            
            break;
        }
        // Pick up the payload
        case GrabPayload:
            mDesired = mCurrent;
            mDesired.mDepth = mGrabDepth;
            
            if (fabs(mGrabDepth - mCurrent.mDepth) < 1)
            {
                mDestPinger = 1;
                mGlobalInfo->SetInfo(GlobalInfo::PingerSelected, 1);
                mState = GoToPinger;
            }
            
            break;
        
        // Surface with the payload
        case Surface:
            mDesired = mCurrent;
            mDesired.mDepthEnable = false;
            mGlobalCommand->Surface();
            
            if (mFancyTimer.IsFinished())
            {
                mFancyTimer.Start();
                switch (mFancySwitch = (mFancySwitch+1) % 3)
                {
                    case 0:
                        mGlobalCommand->SetDispUpperLeft(255,0,0);
                        mGlobalCommand->SetDispUpperRight(255,0,0);
                        mGlobalCommand->SetDispLowerLeft(255,0,0);
                        mGlobalCommand->SetDispLowerRight(255,0,0);
                        break;
                    
                    case 1:
                        mGlobalCommand->SetDispUpperLeft(0,255,0);
                        mGlobalCommand->SetDispUpperRight(0,255,0);
                        mGlobalCommand->SetDispLowerLeft(0,255,0);
                        mGlobalCommand->SetDispLowerRight(0,255,0);
                        break;
                    
                    case 2:
                        mGlobalCommand->SetDispUpperLeft(0,0,255);
                        mGlobalCommand->SetDispUpperRight(0,0,255);
                        mGlobalCommand->SetDispLowerLeft(0,0,255);
                        mGlobalCommand->SetDispLowerRight(0,0,255);
                        break;
                    
                    default:
                        mGlobalCommand->SetDispUpperLeft(255,0,0);
                        mGlobalCommand->SetDispUpperRight(255,0,0);
                        mGlobalCommand->SetDispLowerLeft(255,0,0);
                        mGlobalCommand->SetDispLowerRight(255,0,0);
                        break;
                }
            }
            
            break;
        
        // Proceed to the next state
        case Exit:
            Utility::Display::CloseAllWindows();
            return NextMission;
    }
    
    mDesired.mAxialThrust = 75;
    
    std::cout << " yaw " << mCurrent.mYaw << " -> " << mDesired.mYaw << std::endl
            << " time_diff: " << (CxUtils::GetTimeSeconds() - mPingerUpdateTime) << " dist: " << mPingerDistance << std::endl
            << " time: " << mIgnoreTimer.TimeElapsed() << "/" << mIgnoreDuration << std::endl;
    
    AI::Utility::HeadingDisplay(mProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Post Processing DW",mProcessedImage);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    if (mDesired.mDepthEnable)
        mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
