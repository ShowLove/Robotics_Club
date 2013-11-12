#include "auvc/underwater/testpath.h"

using namespace Zebulon;
using namespace Underwater;

TestPath::TestPath()
{
    ID = "TestPath";
    
    mForwardFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mProcessedImage = NULL;
    mProcessedDWImage = NULL;
    mDownwardFrame = NULL;
    mDownwardFrameF = NULL;
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
    
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Initialize(3, false);
    }
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(10, false);
    mLostTargetDebounce.Initialize(8, false);
    
    mCenterI = 0;
    mCenterJ = 0;

    mAttackingBuoy = 0;
}

TestPath::~TestPath()
{
    
}

int TestPath::Init()
{
    mState = Find;
    
    // Default depth
    mXmlConf.GetVar("Underwater.TestPath.Default@Depth", mDesired.mDepth);
    mDefaultDepth = mDesired.mDepth;
    
    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesired.mPitch);
    bool ROLL = mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesired.mRoll);
    
    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }
    
    mPathFinder.Initialize();
    
    for (int i=0; i<3; i++)
        mBuoyOrder[i] = -1;
    
    mFindActiveTimerActive = false;
    mFindActiveModifier = 1;
    mFindActiveFirstTime = true;
    
    return 0;
}

int TestPath::ErrorCheck()
{
    return 0;
}

int TestPath::FetchProcess()
{
    //////Defaults////////////////////////////////
    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesired.mRoll);
    
    //////Search//////////////////////////////////
    mXmlConf.GetVar("Underwater.TestPath.Search@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.TestPath.Search@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.TestPath.Search@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.TestPath.Search@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.TestPath.Search@SmoothSpeed", mPathSearchSmoothSpeed);
    
    //////Path////////////////////////////////////
    mXmlConf.GetVar("Underwater.TestPath.Path@PathExitTime", mPathExitTime);
    mXmlConf.GetVar("Underwater.TestPath.Path@ExitPathThrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.TestPath.Path@PathSearchDuration", mPathSearchDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::TestPath: Couldn't get depth data");
        return 1;
    }
    
    // Downward
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::TestPath: Couldn't get downward camera data");
        return 1;
    }
    
    if(mProcessedDWImage == NULL)
    {
        mProcessedDWImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedDWImage);
    
    return 0;
}

std::string TestPath::ExitEarly()
{
    return "KeepRunning";
}

std::string TestPath::Execute()
{
    //double centerI = 0;
    //double centerJ = 0;
    int validcount = 0;
    double maxArea = 0;
    int tryDirection = 1;
    int placeMarker = 0;
    std::vector<Path> paths;
    
    switch (mState)
    {
        // Entry state, figure out what to do
        case Find:
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = 0;
            mDesired.mYaw = mYawFix;
            mDesired.mDepth = mDefaultDepth;
            
            mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed,mPathSearchSmoothSpeed);
            mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);
            
            mPathSearchTimeout.Initialize(mPathSearchDuration);
            mPathSearchTimeout.Start();
            
            mState = PathSearch;
            
            std::cout << "State: " << ID << "::Find" << std::endl;
            
        // Look for path or buoys
        case PathSearch:
            if (mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedDWImage).size() > 0)
            {
                mState = DoPath;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
            }
            else if (mPathSearchTimeout.IsFinished())
            {
                std::cout << "Path Timeout" << std::endl;
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
                mDesired.mYaw = mYawFix;
            }
            
            std::cout << "State: " << ID << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    
                    << std::endl;
            break;
            
        // Do the path stuff
        case DoPath:
            mDesired.mDepth = mDefaultDepth;
            // Lock on a path, then Exit
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedDWImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrent, mDesired)))
                {
                    //std::cout << "exit" << std::endl;
                    //mState = Exit;
                    
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mCurrent.mYaw;
                    
                    cvCircle(mProcessedDWImage, cvPoint(mProcessedDWImage->width/2,mProcessedDWImage->height/2), 200, cvScalar(0,255,0), 4);
                }
            }
            else
            {
                mPathDebounce.Miss();
            }
            
            std::cout << "State: " << ID << "::Path" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DepthDesired: " << mDesired.mDepth
                    << " AxialDesired: " << mDesired.mAxialThrust
                    << " LatDesired: " << mDesired.mLateralThrust
                    << " YawDesired: " << mDesired.mYaw
                    << std::endl;
            break;
    }
    
    AI::Utility::HeadingDisplay(mProcessedDWImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedDWImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedDWImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Post Processing (Downward)",mProcessedDWImage);
    Utility::Display::DisplayImage("Raw Processing",mDownwardFrame);
     
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
