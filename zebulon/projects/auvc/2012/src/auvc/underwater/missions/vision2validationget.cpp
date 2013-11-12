#include "auvc/underwater/missions/vision2validationgate.h"

using namespace Zebulon;
using namespace Underwater;

Vision2ValidationGate::Vision2ValidationGate()
{
    ID = "Vision2ValidationGate";
    
    mForwardFrame = NULL;
    mFWProcessedImage = NULL;
    
    mDownwardFrame = NULL;
    mDWProcessedImage = NULL;
    
    mGateSegImage = NULL;
    mGateThreshFrame = NULL;
}

Vision2ValidationGate::~Vision2ValidationGate()
{

}

int Vision2ValidationGate::Init()
{
    double watchtime = 0;
    
    // Set stopwatch for inittravel
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.InitTimer@Duration", watchtime);
    mStopWatchInitTravel.Initialize(watchtime);
    
    // Set stopwatch for throughgate
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Timer@Duration", watchtime);
    mStopWatchThroughGate.Initialize(watchtime);
    
    // Record initial yaw
    mGlobalInfo->GetInfo("StartYaw", mStartYaw);
    
    // Init debounce for gates
    mLeftLegDebounce.Initialize(3, true);
    mRightLegDebounce.Initialize(3, true);
    // Init pathfinder and debounce
    mPathFinder.Initialize();
    mPathDebounce.Initialize(5, true);
    
    mState = GoToDepth;
    return 0;
}

int Vision2ValidationGate::ErrorCheck()
{
    return 0;
}

int Vision2ValidationGate::FetchProcess()
{
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.InitialYaw@Threshold", mYawThresh);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.InitialDepth@Depth", mStartDepth);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.InitialDepth@Threshold", mDepthThresh);
    
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Speed@ValidGateSpeed", mGateSpeed);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Orientation@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Orientation@Roll", mDesiredRoll);
    
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Timer@MinDuration", mMinDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::Vision2ValidationGate: Couldn't get Compass data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::Vision2ValidationGate: Couldn't get Depth data");
        return 1;
    }
    
    // Forward camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::Window: Couldn't get forward camera data");
        return 1;
    }
    
    // Downward camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::Window: Couldn't get downward camera data");
        return 1;
    }
    
    // Block edges of image (when own vehicle is seen)
    //cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 50), cvScalar(0,0,0), -1);
    
    if(mFWProcessedImage == NULL)
    {
        mFWProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    
    if(mDWProcessedImage == NULL)
    {
        mDWProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    
    Vision::VisionUtil::ResetImage(mFWProcessedImage);
    Vision::VisionUtil::ResetImage(mDWProcessedImage);
    
    // Find paths
    mPathDebounce.Bounce(mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0);
    
    // Find gates
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Gate@Red", mTarget.mRed);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Gate@Green", mTarget.mGreen);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.Gate@Blue", mTarget.mBlue);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.GateThresh@Red", mTarget.mRedRange);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.GateThresh@Green", mTarget.mGreenRange);
    mXmlConf.GetVar("Underwater.Vision2ValidationGate.GateThresh@Blue", mTarget.mBlueRange);
    
    mSegment->Setup(mTarget, 50);
    mThreshold->Setup(mTarget);
    mThreshold->Process(mForwardFrame, mGateThreshFrame);
    mSegment->Process(mGateThreshFrame, mGateSegImage);
    for(int i = 0; i < 4; i++)
    {
        mSegment->GetMaxPosition(mReturn[i]);
        Vision::VisionUtil::CombineImages(mGateSegImage, mFWProcessedImage);
    }
    
    return 0;
}

std::string Vision2ValidationGate::ExitEarly()
{
    return "KeepRunning";
}

std::string Vision2ValidationGate::Execute()
{
    switch (mState)
    {
        case GoToDepth:
            mDesiredYaw = mStartYaw;
            mDesiredDepth = mStartDepth;
            
            if (fabs(AI::Utility::DepthDiff(mCurrentDepth, mStartDepth)) < mDepthThresh && fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mStartYaw)) <= mYawThresh)
            {
                mState = ThroughGate;
                //mStopWatch.Start();
            }
            
            std::cout << "State: " << "Vision2ValidationGate" << "::GoToDepth" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " DesiredYaw: " << mDesiredYaw
                    << " Depth: " << mCurrentDepth
                    << " DesiredDepth: " << mDesiredDepth
                    << std::endl;
            break;
        
        case ThroughGate:
            mDesiredYaw = mStartYaw;
            mDesiredDepth = mStartDepth;
            mDesiredThrust = mGateSpeed;
            
            //if (mStopWatch.IsFinished() || (mDebounce.GetDebounced() && mStopWatch.TimeElapsed() > mMinDuration))
            if(1)
            {
                mState = NextMission;
            }
            
            std::cout << "State: " << "Vision2ValidationGate" << "::ThroughGate" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " Depth: " << mCurrentDepth
                    << " Thrust: " << mDesiredThrust
                    //<< " Timer: " << mStopWatch.TimeElapsed()
                    << std::endl;
            break;
        
        case NextMission:
            return "Buoys";
    }
    
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);
    mGlobalCommand->SetDesiredAxialVel(mDesiredThrust);
    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    
    Utility::Display::DisplayImage("Raw Forward",mForwardFrame);
    Utility::Display::DisplayImage("Post Processing Forward",mFWProcessedImage);
    
    Utility::Display::DisplayImage("Raw Downward",mDownwardFrame);
    Utility::Display::DisplayImage("Post Processing Downward",mDWProcessedImage);
    
    return "KeepRunning";
}
