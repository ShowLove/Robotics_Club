#include "auvc/underwater/missions/validationgate.h"

using namespace Zebulon;
using namespace Underwater;

ValidationGate::ValidationGate()
{
    ID = "ValidationGate";
    
    mDownwardFrame = NULL;
    mProcessedImage = NULL;
}

ValidationGate::~ValidationGate()
{

}

int ValidationGate::Init()
{
    double watchtime = 0;
    mXmlConf.GetVar("Underwater.ValidationGate.Timer@Duration", watchtime);
    mStopWatch.Initialize(watchtime);
    
    mGlobalInfo->GetInfo(GlobalInfo::StartYaw, mStartYaw);
    //mPathFinder.Initialize();
    
    mDebounce.Initialize(5, true);
    
    mState = GoToDepth;
    return 0;
}

int ValidationGate::ErrorCheck()
{
    return 0;
}

int ValidationGate::FetchProcess()
{
    mXmlConf.GetVar("Underwater.ValidationGate.InitialYaw@Threshold", mYawThresh);
    mXmlConf.GetVar("Underwater.ValidationGate.InitialDepth@Depth", mStartDepth);
    mXmlConf.GetVar("Underwater.ValidationGate.InitialDepth@Threshold", mDepthThresh);
    
    mXmlConf.GetVar("Underwater.ValidationGate.Speed@ValidGateSpeed", mGateSpeed);
    mXmlConf.GetVar("Underwater.ValidationGate.Orientation@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.ValidationGate.Orientation@Roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.ValidationGate.Timer@MinDuration", mMinDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::ValidationGate: Couldn't get Compass data");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth))
    {
        printf("ERROR::ValidationGate: Couldn't get Depth data");
        return 1;
    }
    
    // Downward
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::Window: Couldn't get downward camera 1 data");
        return 1;
    }
    cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 50), cvScalar(0,0,0), -1);
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
    //mDebounce.Bounce(mPathFinder.GetPaths(mCurrent, mDownwardFrame, mProcessedImage, mDownwardFrameF).size() > 0);
    
    return 0;
}

std::string ValidationGate::ExitEarly()
{
    return "KeepRunning";
}

std::string ValidationGate::Execute()
{
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
            
            std::cout << "State: " << "ValidationGate" << "::GoToDepth" << std::endl;
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
            
            if(mStopWatch.IsFinished() || (mDebounce.GetDebounced() && mStopWatch.TimeElapsed() > mMinDuration))
            {
                mState = NextMission;
            }
            
            std::cout << "State: " << "ValidationGate" << "::ThroughGate" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " Depth: " << mCurrentDepth
                    << " Thrust: " << mDesiredThrust
                    << " Timer: " << mStopWatch.TimeElapsed()
                    << std::endl;
            break;
        
        case NextMission:
            Utility::Display::CloseAllWindows();
            return "Buoys";
    }
    
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);
    mGlobalCommand->SetDesiredAxialVel(mDesiredThrust);
    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    
    Utility::Display::DisplayImage("Raw Downward",mDownwardFrame);
    Utility::Display::DisplayImage("Post Processing Downward",mProcessedImage);
    
    return "KeepRunning";
}
