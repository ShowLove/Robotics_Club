#include "auvc/underwater/missions/test/testpath.h"

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

    mPathFinder.Initialize();

    // Default depth
    mXmlConf.GetVar("Underwater.TestPath.Default@Depth", mDesiredDepth);
    mDefaultDepth = mDesiredDepth;

    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesiredPitch);
    bool ROLL = mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesiredRoll);

    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }


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
    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesiredRoll);

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
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }

    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrentDepth))
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

    if(mProcessedDWImage == NULL && mDownwardFrame != NULL)
    {
        mProcessedDWImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    else
    {
        cvDrawRect(mDownwardFrame,cvPoint(0,0),
                   cvPoint(mDownwardFrame->width,35),
                   CV_RGB(1,1,1),-1);

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

    //mPathFinder.Initialize();
    //double centerI = 0;
    //double centerJ = 0;
    int validcount = 0;
    double maxArea = 0;
    int tryDirection = 1;
    int placeMarker = 0;
    std::vector<Path> paths;
    bool pathExists=false;

    switch (mState)
    {
        // Entry state, figure out what to do
        case Find:
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;
            mDesiredYaw = mYawFix;
            mDesiredDepth = mDefaultDepth;

            mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed,mPathSearchSmoothSpeed);
            mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed);

            mPathSearchTimeout.Initialize(mPathSearchDuration);
            mPathSearchTimeout.Start();

            mState = PathSearch;

            std::cout << "State: " << ID << "::Find" << std::endl;

        // Look for path or buoys
        case PathSearch:
            mDesiredYaw = mYawFix;
            mDesiredDepth = mDefaultDepth;

            pathExists=GetPaths(paths,mCurrentYaw, mDownwardFrame, mProcessedDWImage);
            if(pathExists)
            {
                mState = DoPath;

                /*mDesiredLateralThrust = 0;
                mDesiredAxialThrust = 0;
                mDesiredYaw = mYawFix;*/
            }
            else if (mPathSearchTimeout.IsFinished())
            {
                std::cout << "Path Timeout" << std::endl;
                mDesiredLateralThrust = 0;
                mDesiredAxialThrust = 0;
                mDesiredYaw = mYawFix;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
                //mDesiredYaw = mYawFix;
            }
            //mDesiredDepth = mDefaultDepth;

            /*std::cout << "State: " << ID << "::PathSearch" << std::endl;
            std::cout << " DESIRED YAW: " << mDesiredYaw << "->"
                      << " CURRENT YAW: " << mCurrentYaw << std::endl;

            std::cout << " DESIRED PITCH: " << mDesiredPitch << "->"
                      << " CURRENT PITCH: " << mCurrentPitch << std::endl;

            std::cout << " DESIRED DEPTH: " << mDesiredDepth << "->"
                      << " CURRENT DEPTH: " << mCurrentDepth << std::endl;*/
            break;

        // Do the path stuff
        case DoPath:
            mDesiredDepth = mDefaultDepth;
            // Lock on a path, then Exit
            pathExists=GetPaths(paths,mCurrentYaw, mDownwardFrame, mProcessedDWImage);
            if (paths.size()>0) // pathexists
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrentYaw, mDesiredYaw, mDesiredAxialThrust, mDesiredLateralThrust)))
                {
                    //std::cout << "exit" << std::endl;


                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mCurrentYaw;
                    mDesiredYaw = mCurrentYaw;
                    cvCircle(mProcessedDWImage, cvPoint(mProcessedDWImage->width/2,mProcessedDWImage->height/2), 200, cvScalar(0,255,0), 4);

                     //mState = Exit;

                }


            }
            else
            {
                // was mPathDebounce.Miss();
               //mPathCenteredDebounce.Miss();
            }

            std::cout << "State: " << ID << "::Path" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " Depth: " << mCurrentDepth
                    << " Pitch: " << mCurrentPitch
                    << " Roll: " << mCurrentRoll
                    << " DepthDesired: " << mDesiredDepth
                    << " PitchDesired: " << mDesiredPitch
                    << " RollDesired: " << mDesiredRoll
                    << " YawDesired: " << mDesiredYaw
                    << " AxialDesired: " << mDesiredAxialThrust
                    << " LatDesired: " << mDesiredLateralThrust
                    << " YawDesired: " << mDesiredYaw
                    << std::endl;
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

    AI::Utility::HeadingDisplay(mProcessedDWImage, mCurrentYaw, mDesiredYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedDWImage, mCurrentDepth, mDesiredDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedDWImage, mDesiredAxialThrust, mDesiredLateralThrust);
    Utility::Display::DisplayImage("Post Processing (Downward)",mProcessedDWImage);
    Utility::Display::DisplayImage("Raw Processing",mDownwardFrame);

    //mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    //mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);

    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesiredLateralThrust);
    //mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);

    return "KeepRunning";
}

#if 0
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

    mPathFinder.Initialize();
    
    // Default depth
    mXmlConf.GetVar("Underwater.TestPath.Default@Depth", mDesiredDepth);
    mDefaultDepth = mDesiredDepth;
    
    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesiredPitch);
    bool ROLL = mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesiredRoll);
    
    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }

    
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
    mPathFinder.Initialize();
    //////Defaults////////////////////////////////
    mXmlConf.GetVar("Underwater.TestPath.Default@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.TestPath.Default@Roll", mDesiredRoll);
    
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
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::TestPath: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrentDepth))
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
    
    if(mProcessedDWImage == NULL && mDownwardFrame != NULL)
    {
        mProcessedDWImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    else
    {
        cvDrawRect(mDownwardFrame,cvPoint(0,0),
                   cvPoint(mDownwardFrame->width,35),
                   CV_RGB(1,1,1),-1);

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

    //mPathFinder.Initialize();
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
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;
            mDesiredYaw = mYawFix;
            mDesiredDepth = mDefaultDepth;
            
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
                
                mDesiredLateralThrust = 0;
                mDesiredAxialThrust = 0;
                mDesiredYaw = mYawFix;
            }
            else if (mPathSearchTimeout.IsFinished())
            {
                std::cout << "Path Timeout" << std::endl;
                mDesiredLateralThrust = 0;
                mDesiredAxialThrust = 0;
                mDesiredYaw = mYawFix;
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesiredLateralThrust, mDesiredAxialThrust);
                mDesiredYaw = mYawFix;
            }
            mDesiredDepth = mDefaultDepth;
            
            std::cout << "State: " << ID << "::PathSearch" << std::endl;
            std::cout << " DESIRED YAW: " << mDesiredYaw << "->"
                      << " CURRENT YAW: " << mCurrentYaw << std::endl;

            std::cout << " DESIRED PITCH: " << mDesiredPitch << "->"
                      << " CURRENT PITCH: " << mCurrentPitch << std::endl;

            std::cout << " DESIRED DEPTH: " << mDesiredDepth << "->"
                      << " CURRENT DEPTH: " << mCurrentDepth << std::endl;
            break;
            
        // Do the path stuff
        case DoPath:
            mDesiredDepth = mDefaultDepth;
            // Lock on a path, then Exit
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mProcessedDWImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrentYaw, mDesiredYaw, mDesiredAxialThrust, mDesiredLateralThrust)))
                {
                    //std::cout << "exit" << std::endl;
                    //mState = Exit;
                    
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mCurrentYaw;
                    mDesiredYaw = mCurrentYaw;
                    cvCircle(mProcessedDWImage, cvPoint(mProcessedDWImage->width/2,mProcessedDWImage->height/2), 200, cvScalar(0,255,0), 4);
                }
            }
            else
            {
                mPathDebounce.Miss();
            }
            
            std::cout << "State: " << ID << "::Path" << std::endl;
            std::cout << " Yaw: " << mCurrentYaw
                    << " Depth: " << mCurrentDepth
                    << " Pitch: " << mCurrentPitch
                    << " Roll: " << mCurrentRoll
                    << " DepthDesired: " << mDesiredDepth
                    << " PitchDesired: " << mDesiredPitch
                    << " RollDesired: " << mDesiredRoll
                    << " YawDesired: " << mDesiredYaw
                    << " AxialDesired: " << mDesiredAxialThrust
                    << " LatDesired: " << mDesiredLateralThrust
                    << " YawDesired: " << mDesiredYaw
                    << std::endl;
            break;
    }
    
    AI::Utility::HeadingDisplay(mProcessedDWImage, mCurrentYaw, mDesiredYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedDWImage, mCurrentDepth, mDesiredDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedDWImage, mDesiredAxialThrust, mDesiredLateralThrust);
    Utility::Display::DisplayImage("Post Processing (Downward)",mProcessedDWImage);
    Utility::Display::DisplayImage("Raw Processing",mDownwardFrame);
     
    //mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    //mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesiredLateralThrust);
    //mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);
    
    return "KeepRunning";
}

#endif
