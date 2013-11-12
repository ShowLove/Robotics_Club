#include "auvc/underwater/visionvalidationgate2.h"

using namespace Zebulon;
using namespace Underwater;
using namespace std;

VisionValidationGate2::VisionValidationGate2()
{
    ID = "VisionValidationGate2";
    
    mForwardFrame = NULL;
    mFWProcessedImage = NULL;
    
    mDownwardFrame = NULL;
    mDWProcessedImage = NULL;
    
    mGateSegImage = NULL;
    mGateThreshFrame = NULL;
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
}

VisionValidationGate2::~VisionValidationGate2()
{

}

int VisionValidationGate2::Init()
{
    double watchtime = 0;
    
    // Set stopwatch for inittravel
    mXmlConf.GetVar("Underwater.VisionValidationGate2.InitTimer@Duration", watchtime);
    mStopWatchInitTravel.Initialize(watchtime);
    
    // Set stopwatch for lineup
    mXmlConf.GetVar("Underwater.VisionValidationGate2.LineUpTimer@Duration", watchtime);
    mStopWatchLineUp.Initialize(watchtime);
    
    // Set stopwatch for throughgate
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Timer@Duration", watchtime);
    mStopWatchThroughGate.Initialize(watchtime);
    
    // Record initial yaw
    mGlobalInfo->GetInfo("StartYaw", mStartYaw);
    // Testing
    //mStartYaw = -111;
    
    // Init debounce for gates
    mGateDebounce.Initialize(3, true);
    //mLeftLegDebounce.Initialize(3, true);
    //mRightLegDebounce.Initialize(3, true);
    // Init pathfinder and debounce
    mPathFinder.Initialize();
    mPathDebounce.Initialize(5, true);
    
    mCenterGateDebounce.Initialize(5, true);
    
    mState = GoToDepth;
    return 0;
}

int VisionValidationGate2::ErrorCheck()
{
    return 0;
}

int VisionValidationGate2::FetchProcess()
{
    mXmlConf.GetVar("Underwater.VisionValidationGate2.InitialYaw@Threshold", mYawThresh);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.InitialDepth@Depth", mStartDepth);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.InitialDepth@Threshold", mDepthThresh);
    
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Speed@ValidGateSpeed", mGateSpeed);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Orientation@Pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Orientation@Roll", mDesiredRoll);
    
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Timer@MinDuration", mMinDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::VisionValidationGate2: Couldn't get Compass data\n");
        return 1;
    }
    
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::VisionValidationGate2: Couldn't get Depth data\n");
        return 1;
    }
    
    // Forward camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::Window: Couldn't get forward camera data\n");
        return 1;
    }
    
    // Downward camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::Window: Couldn't get downward camera data\n");
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
    mPathDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0);
    
    // Find gates
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Gate@Red", mTarget.mRed);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Gate@Green", mTarget.mGreen);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.Gate@Blue", mTarget.mBlue);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateThresh@Red", mTarget.mRedRange);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateThresh@Green", mTarget.mGreenRange);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateThresh@Blue", mTarget.mBlueRange);
    
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateExtra@GateAngleThresh", mGateAngleThresh);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateExtra@GateAngleThresh", mGateCenterThresh);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateControl@LineUpSpeed", mLineUpSpeed);
    
    double num = 0, denom = 0;
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateControl@Num", num);
    mXmlConf.GetVar("Underwater.VisionValidationGate2.GateControl@Denom", denom);
    mLatPixelScale = num/denom;
    
    //mXmlConf.GetVar("Underwater.VisionValidationGate2.GateControl@LatPixelScale", mLatPixelScale);
    
    mSegment->Setup(mTarget, 50, 0, 4);
    mThreshold->Setup(mTarget);
    
    mThreshold->Process(mForwardFrame, mGateThreshFrame);
    mSegment->Process(mGateThreshFrame, mGateSegImage);
    
    for(int i = 0; i < 2; i++)
    {
        // check is valid
        mSegment->GetMaxPosition(mReturn[i],i);
    }
    
    Vision::VisionUtil::CombineImages(mGateSegImage, mFWProcessedImage);
    
    for(int i = 0; i < 2; i++)
    {
        //cout << "GATE LEG: " << i << endl;
        //cout << "valid: " << mReturn[i].mValid << ", mReturn[i].mAngle: " << mReturn[i].mAngle << endl;
        if(mReturn[i].mValid == true && fabs(Zebulon::AI::Utility::AngleDiff(mReturn[i].mAngle, ((CxUtils::PI/180)*90))) <= mGateAngleThresh) // && mReturn[i].mCenterI < mGateHeightThresh
        {
            //cout << "Meets criteria" << endl;
            //mGateI[0] = 
            mGateValid[i] = mReturn[i].mValid;
            mGateI[i] = mReturn[i].mCenterI;
            //mGate
        }
        else
        {
            mGateValid[i] = false;
            mGateI[i] = 0;
        }
    }
    
    mGateDebounce.Bounce(mGateValid[0] == true || mGateValid[1] == true);
    mLeftGateFlag = false;
    mRightGateFlag = false;
    
    if(mGateDebounce.GetDebounced() && mGateValid[0] == true && mGateValid[1] == true)
    {
        //cout << "Both" << endl;
        mCurrentGate = (mGateI[0]+mGateI[1])/2;
        /*if(mGateI[0] < mGateI[1])
        {
            mLeftGateI = mGateI[0];
        }
        else
        {
            mRightGateI = mGateI[0];
        }*/
    }
    else if(mGateDebounce.GetDebounced() == true && mGateValid[0] == true)
    {
        if(mGateI[0] > 0 && mGateI[0] < mFWProcessedImage->width/2)
        {
            mLeftGateFlag = true;
        }
        else
        {
            mRightGateFlag = true;
        }
    }
    else if(mGateDebounce.GetDebounced() == true && mGateValid[1] == true)
    {
        if(mGateI[1] > 0 && mGateI[1] < mFWProcessedImage->width/2)
        {
            mLeftGateFlag = true;
        }
        else
        {
            mRightGateFlag = true;
        }
    }
    /*else if(mGateDebounce.GetDebounced() == true && mGateValid[0] == true)
    {
        //cout << "Zero" << endl;
        mCurrentGate = mGateI[0];
        if(mGateI[0] > mFWProcessedImage->width/2)
        {
            mCurrentGate  = mGateI[0] - mFWProcessedImage->width/8;
        }
        else
        {
            mCurrentGate  = mGateI[0] + mFWProcessedImage->width/8;
        }
    }
    else if(mGateDebounce.GetDebounced() == true && mGateValid[1] == true)
    {
        //cout << "One" << endl;
        mCurrentGate = mGateI[1];
        if(mGateI[1] > mFWProcessedImage->width/2)
        {
            mCurrentGate  = mGateI[1] - mFWProcessedImage->width/8;
        }
        else
        {
            mCurrentGate  = mGateI[1] + mFWProcessedImage->width/8;
        }
    }*/
    else
    {
        //cout << "None" << endl;
        mCurrentGate = 0;
    }
    
    cvCircle(mFWProcessedImage, cvPoint(mCurrentGate, mFWProcessedImage->height/2), 5, cvScalar(0, 0, 255), 5 );
    
    
    return 0;
}

std::string VisionValidationGate2::ExitEarly()
{
    return "KeepRunning";
}

std::string VisionValidationGate2::Execute()
{
    switch (mState)
    {
        case GoToDepth:
            
            if(fabs(AI::Utility::DepthDiff(mCurrent.mDepth, mStartDepth)) < mDepthThresh && fabs(Zebulon::AI::Utility::AngleDiff(mCurrent.mYaw, mStartYaw)) <= mYawThresh)
            {
                mState = InitialTravel;
                mStopWatchInitTravel.Start();
            }
            
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mStartDepth;
            mDesired.mYaw = mStartYaw;
            mDesired.mPitch = mDesiredPitch;
            mDesired.mRoll = mDesiredRoll;
            
            std::cout << "State: " << "VisionValidationGate2" << "::GoToDepth" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " DesiredYaw: " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesiredDepth: " << mDesired.mDepth
                    << std::endl;
                    
            break;
            
        case InitialTravel:
            
            if(mStopWatchInitTravel.IsFinished())
            {
                mState = LineUp;
                mStopWatchLineUp.Start();
            }
            
            mDesired.mAxialThrust = mGateSpeed;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mStartDepth;
            mDesired.mYaw = mStartYaw;
            mDesired.mPitch = mDesiredPitch;
            mDesired.mRoll = mDesiredRoll;
            
            std::cout << "State: " << "VisionValidationGate2" << "::InitialTravel" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " AxialThrust: " << mDesired.mAxialThrust
                    << " DesiredYaw: " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesiredDepth: " << mDesired.mDepth
                    << std::endl;
            
            break;
            
        case LineUp:
            
            // If centered enough or time runs out
            mCenterGateDebounce.Bounce(fabs(mCurrentGate - mFWProcessedImage->width/2) < mGateCenterThresh);
            
            if(mCenterGateDebounce.GetDebounced() || mStopWatchLineUp.IsFinished())
            {
                mState = ThroughGate;
                mStopWatchThroughGate.Start();
            }
            
            if(mCurrentGate > 0)
            {
                mDesired.mLateralThrust = (mCurrentGate - mFWProcessedImage->width/2)*mLatPixelScale;;
            }
            else if(mLeftGateFlag)
            {
                mDesired.mLateralThrust = 50;
            }
            else if(mRightGateFlag)
            {
                mDesired.mLateralThrust = -50;
            }
            else
            {
                mDesired.mLateralThrust = 0;
            }
            
            mDesired.mAxialThrust = mLineUpSpeed;
            mDesired.mDepth = mStartDepth;
            mDesired.mYaw = mStartYaw;
            mDesired.mPitch = mDesiredPitch;
            mDesired.mRoll = mDesiredRoll;
            
            std::cout << "State: " << "VisionValidationGate2" << "::LineUp" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " DesiredYaw: " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DesiredDepth: " << mDesired.mDepth
                    << " DesiredLateral: " << mDesired.mLateralThrust
                    << " Timer: " << mStopWatchLineUp.TimeElapsed()
                    << " CurrGate: " << mCurrentGate
                    << std::endl;
            
            break;
        
        case ThroughGate:
            
            if(mStopWatchThroughGate.IsFinished() || mPathDebounce.GetDebounced()) // && mStopWatch.TimeElapsed() > mMinDuration)
            {
                mState = NextMission;
            }
            
            mDesired.mAxialThrust = mGateSpeed;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mStartDepth;
            mDesired.mYaw = mStartYaw;
            mDesired.mPitch = mDesiredPitch;
            mDesired.mRoll = mDesiredRoll;
            
            std::cout << "State: " << "VisionValidationGate2" << "::ThroughGate" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Thrust: " << mDesired.mAxialThrust
                    << " Timer: " << mStopWatchThroughGate.TimeElapsed()
                    << std::endl;
                    
            break;
        
        case NextMission:
            Utility::Display::CloseAllWindows();
            return "Buoys";
    }
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    
    AI::Utility::HeadingDisplay(mFWProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mFWProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mFWProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    
    Utility::Display::DisplayImage("Raw Forward",mForwardFrame);
    Utility::Display::DisplayImage("Post Processing Forward",mFWProcessedImage);
    
    Utility::Display::DisplayImage("Raw Downward",mDownwardFrame);
    Utility::Display::DisplayImage("Post Processing Downward",mDWProcessedImage);
    
    return "KeepRunning";
}
