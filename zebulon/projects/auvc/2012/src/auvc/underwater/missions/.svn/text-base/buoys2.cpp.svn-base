#include <auvc/underwater/missions/buoys2.h>

using namespace Zebulon;
using namespace Underwater;

const std::string Buoys2::NextMission = "LoveLane";

Buoys2::Buoys2()
{
    ID = "Buoys2";
    
    mForwardFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mFWProcessedImage = NULL;
    mDWProcessedImage = NULL;
    mDownwardFrame = NULL;
    
    mDisplayLaser = NULL;
    mDisplayLaser = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
    
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Initialize(2, false);
    }
    
    mFoundTwoFlag = false;
    mFoundAllThreeFlag = false;
    
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(10, false);
    mLostTargetDebounce.Initialize(8, false);
    
    mCenterI = 0;
    mCenterJ = 0;

    mAttackingBuoy = 0;
}

Buoys2::~Buoys2()
{
    //cvReleaseImage(&mFWProcessedImage);
    //cvReleaseImage(&mProcessedDWImage);
}

int Buoys2::Init()
{
    mState = Find;
    //mState = DoPath;
    int temp;
    mXmlConf.GetVar("Underwater.Buoys2.Default@Timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    mObjectScanData.Setup(0.33, 1, 5, 0.04); //0.075
    
    mXmlConf.GetVar("Underwater.Buoys2.BuoyToHit@Red", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Red;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Red;
    }
    
    mXmlConf.GetVar("Underwater.Buoys2.BuoyToHit@Green", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Green;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Green;
    }
    
    mXmlConf.GetVar("Underwater.Buoys2.BuoyToHit@Yellow", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Yellow;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Yellow;
    }
    
    // Default depth
    mXmlConf.GetVar("Underwater.Buoys2.Default@Depth", mDesired.mDepth);
    mDefaultDepth = mDesired.mDepth;
    
    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::Buoys2: Couldn't get Compass data");
        return 1;
    }
    
    
    
    mPathFinder.Initialize();
    mPathsDebounce.Initialize(3, true);
    
    for (int i=0; i<3; i++)
        mBuoyOrder[i] = -1;
    
    mFindActiveTimerActive = false;
    mFindActiveModifier = 1;
    mFindActiveFirstTime = true;
    
    for(int i = 0; i < 3; i++)
    {
        std::vector<mTempBuoyInfo> tempVector;
        mTempBuoyInfo initVar;
        mBuoyPositionVectorAll.push_back(initVar);
    }
    
    for(int i = 0; i < 2; i++)
    {
        std::vector<mTempBuoyInfo> tempVector;
        mTempBuoyInfo initVar;
        mBuoyPositionVectorTwo.push_back(initVar);
    }
    
    mBuoyPositionVectorTwo.clear();
    mBuoyPositionVectorAll.clear();
    
    //std::cout << "size: " << mBuoyPositionVectorAll.size() << std::endl;
    //std::vector< vector<int> > mBuoyPositionVectorAll;
    
    return 0;
}

int Buoys2::ErrorCheck()
{
    return 0;
}

int Buoys2::FetchProcess()
{
    //////Defaults////////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Default@Pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.Buoys2.Default@Roll", mDesired.mRoll);
    
    //////Color Values////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Red@Red", mTarget[Red].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.Red@Green", mTarget[Red].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.Red@Blue", mTarget[Red].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.RedThresh@Red", mTarget[Red].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.RedThresh@Green", mTarget[Red].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.RedThresh@Blue", mTarget[Red].mBlueRange);
    
    mXmlConf.GetVar("Underwater.Buoys2.Green@Red", mTarget[Green].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.Green@Green", mTarget[Green].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.Green@Blue", mTarget[Green].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.GreenThresh@Red", mTarget[Green].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.GreenThresh@Green", mTarget[Green].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.GreenThresh@Blue", mTarget[Green].mBlueRange);
    
    mXmlConf.GetVar("Underwater.Buoys2.Yellow@Red", mTarget[Yellow].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.Yellow@Green", mTarget[Yellow].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.Yellow@Blue", mTarget[Yellow].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.YellowThresh@Red", mTarget[Yellow].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.YellowThresh@Green", mTarget[Yellow].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.YellowThresh@Blue", mTarget[Yellow].mBlueRange);
    
    //////Color Values (Far)//////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.FarRed@Red", mTargetFar[Red].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.FarRed@Green", mTargetFar[Red].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.FarRed@Blue", mTargetFar[Red].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.FarRedThresh@Red", mTargetFar[Red].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarRedThresh@Green", mTargetFar[Red].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarRedThresh@Blue", mTargetFar[Red].mBlueRange);
    
    mXmlConf.GetVar("Underwater.Buoys2.FarGreen@Red", mTargetFar[Green].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.FarGreen@Green", mTargetFar[Green].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.FarGreen@Blue", mTargetFar[Green].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.FarGreenThresh@Red", mTargetFar[Green].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarGreenThresh@Green", mTargetFar[Green].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarGreenThresh@Blue", mTargetFar[Green].mBlueRange);
    
    mXmlConf.GetVar("Underwater.Buoys2.FarYellow@Red", mTargetFar[Yellow].mRed);
    mXmlConf.GetVar("Underwater.Buoys2.FarYellow@Green", mTargetFar[Yellow].mGreen);
    mXmlConf.GetVar("Underwater.Buoys2.FarYellow@Blue", mTargetFar[Yellow].mBlue);
    mXmlConf.GetVar("Underwater.Buoys2.FarYellowThresh@Red", mTargetFar[Yellow].mRedRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarYellowThresh@Green", mTargetFar[Yellow].mGreenRange);
    mXmlConf.GetVar("Underwater.Buoys2.FarYellowThresh@Blue", mTargetFar[Yellow].mBlueRange);
    
    //////Pixel Scale////////////////////////////
    double num, denom;
    mXmlConf.GetVar("Underwater.Buoys2.LatPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.Buoys2.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Buoys2.DepthPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.Buoys2.DepthPixelScale@Denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Buoys2.AxialPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.Buoys2.AxialPixelScale@Denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Buoys2.YawPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.Buoys2.YawPixelScale@Denom", denom);
    mYawPixelScale = num/denom;
    
    //////State-Specific//////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.FindActive@LateralThrust", mFindActiveLateralThrust);
    mXmlConf.GetVar("Underwater.Buoys2.FindActive@Duration", mFindActiveDuration);
    
    mXmlConf.GetVar("Underwater.Buoys2.Attack@LockDuration", mLockAttackDuration);
    mXmlConf.GetVar("Underwater.Buoys2.Attack@Thrust", mAttackThrust);
    
    mXmlConf.GetVar("Underwater.Buoys2.Unhit@LockDuration", mLockUnhitDuration);
    mXmlConf.GetVar("Underwater.Buoys2.Unhit@Thrust", mUnhitThrust);
    
    mXmlConf.GetVar("Underwater.Buoys2.Approach@Thrust", mApproachThrust);
    mXmlConf.GetVar("Underwater.Buoys2.Approach@AttackWidthThresh", mAttackWidthThresh);
    
    //////Search//////////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Buoys2.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.Buoys2.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.Search@SmoothSpeed", mSearchSmoothSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.Search@InitDirection", mSearchInitDirection);
    
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@SmoothSpeed", mPathSearchSmoothSpeed);
    mXmlConf.GetVar("Underwater.Buoys2.PathSearch@InitDirection", mPathSearchInitDirection);
    
    //////Evade///////////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Evade@LateralThrust", mEvadeLatThrust);
    mXmlConf.GetVar("Underwater.Buoys2.Evade@LockTime", mEvadeLockTime);
    mXmlConf.GetVar("Underwater.Buoys2.Evade@LockThrust", mEvadeLockThrust);
    
    //////Thresholds//////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@IThresh", mIThresh);
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@JThresh", mJThresh);
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@AreaCloseThresh", mAreaCloseThresh);
    
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@MinArea", mMinArea);
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@MaxArea", mMaxArea);
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@TargetArea", mTargetArea);
    mXmlConf.GetVar("Underwater.Buoys2.Buoy@TargetWidth", mTargetWidth);
    
    //////Path////////////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Path@PathExitTime", mPathExitTime);
    mXmlConf.GetVar("Underwater.Buoys2.Path@ExitPathThrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.Buoys2.Path@PathSearchDuration", mPathSearchDuration);
    
    //////Sensors/////////////////////////////////
    mXmlConf.GetVar("Underwater.Buoys2.Laser@CloseDist", mLaserCloseDist);
    mXmlConf.GetVar("Underwater.Buoys2.Laser@MinRadius", mMinLaserObjectRadius);
    mXmlConf.GetVar("Underwater.Buoys2.Laser@MaxRadius", mMaxLaserObjectRadius);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::Buoys2: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::Buoys2: Couldn't get depth data");
        return 1;
    }
    
    // Laser
    if(!mGlobalInfo->GetInfo(GlobalInfo::Laser, mRawLaserData))
    {
        printf("ERROR::TestLaserTracking: Couldn't get laser data");
        return 1;
    }
    mObjectScanData.CalculateCartesian(mRawLaserData, true, 100, -50, 50);
    mLaserObjects = mObjectScanData.GetObjects(mLaserObjectValid, mObjectScanData.BIGGEST);
    if(mLaserObjectValid && mLaserObjects[0].mZ > mMinLaserObjectRadius && mLaserObjects[0].mZ < mMaxLaserObjectRadius)
    {
        // in inches
        mCurrLaserDist = sqrt(mLaserObjects[0].mX*mLaserObjects[0].mX + mLaserObjects[0].mY*mLaserObjects[0].mY)*39.37007874015748;
    }
    else
    {
        mCurrLaserDist = 1000;
    }
    
    if (mState != DoPath && mState != Unvade)
    {
        // Forward Camera
        if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
        {
            printf("ERROR::Buoys2: Couldn't get forward camera data");
            return 1;
        }
        
        if(mFWProcessedImage == NULL)
        {
            mFWProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mFWProcessedImage);
        
        for (int i=0; i<3; i++)
        {
            if (mState == Approach || mState == DoLineup || mState == Find || mState == PathSearch)
            {
                mSegment->Setup(mTargetFar[i], mMinArea);
                mThreshold->Setup(mTargetFar[i]);
            }
            else
            {
                mSegment->Setup(mTarget[i], mMinArea);
                mThreshold->Setup(mTarget[i]);
            }
            
            mThreshold->Process(mForwardFrame, mThreshFrame);
            mSegment->Process(mThreshFrame, mSegFrame);
            
            mSegment->GetMaxPosition(mReturn[i]);
            
            Vision::VisionUtil::CombineImages(mSegFrame, mFWProcessedImage);
            
            // Add more checks here
            mDebounce[i].Bounce(mReturn[i].mValid && mReturn[i].mArea >= mMinArea && mReturn[i].mArea <= mMaxArea);
            
            //std::cout << "mReturn[i]: " << i << " : " << mReturn[i].mCenterI << std::endl;
        }
        
        if(mDebounce[Red].GetDebounced() && mDebounce[Green].GetDebounced() && mDebounce[Yellow].GetDebounced() && mFoundAllThreeFlag == false)
        {
            mTempBuoyInfo tempBuoyInfo[3];
            for(int i = 0; i < 3; i++)
            {
                tempBuoyInfo[i].tempColor = i;
                tempBuoyInfo[i].tempReturn = mReturn[i].mCenterI;
            }
            
            bool swapped = true;
            // Bubble Sort
            while(swapped)
            {
                swapped = false;
                for(int i = 1; i < 3; i++)
                {
                    if(tempBuoyInfo[i-1].tempReturn > tempBuoyInfo[i].tempReturn)
                    {
                        mTempBuoyInfo tempBuoyInfo2;
                        
                        tempBuoyInfo2 = tempBuoyInfo[i-1];
                        tempBuoyInfo[i-1] = tempBuoyInfo[i];
                        tempBuoyInfo[i] = tempBuoyInfo2;
                        swapped = true;
                    }
                }
            }
            
            mBuoyPositionVectorAll.clear();
            for(int i = 0; i < 3; i++)
            {
                mBuoyPositionVectorAll.push_back(tempBuoyInfo[i]);
            }
            
            std::cout << "BUOY POSITIONS ALL: " << std::endl;
            for(int i = 0; i < 3; i++)
            {
                {
                    std::cout << i << ": " << mBuoyPositionVectorAll[i].tempColor << ", " << mBuoyPositionVectorAll[i].tempReturn << std::endl;
                }
            }
            
            mFoundAllThreeFlag = true;
            for(int i = 0; i < 3; i++)
            {
                mBuoyOrder[i] = mBuoyPositionVectorAll[i].tempColor;
            }
            
            
            // Have seen three flag, set variables
        }
        else if((mDebounce[Red].GetDebounced() && mDebounce[Green].GetDebounced()
                || mDebounce[Red].GetDebounced() && mDebounce[Yellow].GetDebounced()
                || mDebounce[Green].GetDebounced() && mDebounce[Yellow].GetDebounced())
                && mFoundTwoFlag == false)
        {
            int buoyOne = -1;
            int buoyTwo = -1;
            mTempBuoyInfo tempBuoyInfo[2];
            
            // Red and Green
            if(mDebounce[Red].GetDebounced() == true && mDebounce[Green].GetDebounced() == true)
            {
                if(mReturn[Red].mCenterI <= mReturn[Green].mCenterI)
                {
                    tempBuoyInfo[0].tempColor = Red;
                    tempBuoyInfo[0].tempReturn = mReturn[Red].mCenterI;
                    tempBuoyInfo[1].tempColor = Green;
                    tempBuoyInfo[1].tempReturn = mReturn[Green].mCenterI;
                }
                else
                {
                    tempBuoyInfo[1].tempColor = Red;
                    tempBuoyInfo[1].tempReturn = mReturn[Red].mCenterI;
                    tempBuoyInfo[0].tempColor = Green;
                    tempBuoyInfo[0].tempReturn = mReturn[Green].mCenterI;
                }
            }
            // Red and Yellow
            else if(mDebounce[Red].GetDebounced() == true && mDebounce[Yellow].GetDebounced() == true)
            {
                if(mReturn[Red].mCenterI <= mReturn[Yellow].mCenterI)
                {
                    tempBuoyInfo[0].tempColor = Red;
                    tempBuoyInfo[0].tempReturn = mReturn[Red].mCenterI;
                    tempBuoyInfo[1].tempColor = Yellow;
                    tempBuoyInfo[1].tempReturn = mReturn[Yellow].mCenterI;
                }
                else
                {
                    tempBuoyInfo[1].tempColor = Red;
                    tempBuoyInfo[1].tempReturn = mReturn[Red].mCenterI;
                    tempBuoyInfo[0].tempColor = Yellow;
                    tempBuoyInfo[0].tempReturn = mReturn[Yellow].mCenterI;
                }
            }
            // Green and Yellow
            else if(mDebounce[Green].GetDebounced() == true && mDebounce[Yellow].GetDebounced() == true)
            {
                if(mReturn[Green].mCenterI <= mReturn[Yellow].mCenterI)
                {
                    tempBuoyInfo[0].tempColor = Green;
                    tempBuoyInfo[0].tempReturn = mReturn[Green].mCenterI;
                    tempBuoyInfo[1].tempColor = Yellow;
                    tempBuoyInfo[1].tempReturn = mReturn[Yellow].mCenterI;
                }
                else
                {
                    tempBuoyInfo[1].tempColor = Green;
                    tempBuoyInfo[1].tempReturn = mReturn[Green].mCenterI;
                    tempBuoyInfo[0].tempColor = Yellow;
                    tempBuoyInfo[0].tempReturn = mReturn[Yellow].mCenterI;
                }
            }
            
            /*if(mDebounce[Red].GetDebounced() == true && mReturn[Red].mCenterI <= (mReturn[Green].mCenterI || mReturn[Yellow].mCenterI))
            {
                tempBuoyInfo[i].tempColor = i;
                if(buoyOne >= 0)
                {
                    
                }
                else
                {
                    buoyOne = Red;
                }
            }
            if(mDebounce[Red].GetDebounced())
            {
                buoyOne = Red;
            }
            if(mDebounce[Red].GetDebounced())
            {
                buoyOne = Red;
            }
            
            mTempBuoyInfo tempBuoyInfo[2];*/
            
            /*for(int i = 0; i < 2; i++)
            {
                //tempBuoyInfo[i].tempColor = i;
                tempBuoyInfo[i].tempReturn = mReturn[i].mCenterI;
                
                //std::cout << "tempBuoyInfo[i].tempColor: " << tempBuoyInfo[i].tempColor << std::endl;
                //std::cout << "tempBuoyInfo[i].tempReturn: " << tempBuoyInfo[i].tempReturn << std::endl;
            }*/
            
            // this is what i wanted
            /*if(tempBuoyInfo[0].tempReturn > tempBuoyInfo[1].tempReturn)
            {
                mTempBuoyInfo tempBuoyInfo2;
                tempBuoyInfo2 = tempBuoyInfo[0];
                tempBuoyInfo[0] = tempBuoyInfo[1];
                tempBuoyInfo[1] = tempBuoyInfo2;
            }*/
            /*std::cout << "AFTER" << std::endl;
            std::cout << "tempBuoyInfo[0].tempColor: " << tempBuoyInfo[0].tempColor << std::endl;
            std::cout << "tempBuoyInfo[0].tempReturn: " << tempBuoyInfo[0].tempReturn << std::endl;
            std::cout << "tempBuoyInfo[1].tempColor: " << tempBuoyInfo[1].tempColor << std::endl;
            std::cout << "tempBuoyInfo[1].tempReturn: " << tempBuoyInfo[1].tempReturn << std::endl;*/
            
            mBuoyPositionVectorTwo.clear();
            for(int i = 0; i < 2; i++)
            {
                mBuoyPositionVectorTwo.push_back(tempBuoyInfo[i]);
            }
            
            std::cout << "BUOY POSITIONS TWO: " << std::endl;
            for(int i = 0; i < 2; i++)
            {
                {
                    std::cout << i << ": " << mBuoyPositionVectorTwo[i].tempColor << ", " << mBuoyPositionVectorTwo[i].tempReturn << std::endl;
                }
            }
            
            if((mBuoyPositionVectorTwo[0].tempColor == mBuoyToAttack[0] || mBuoyPositionVectorTwo[0].tempColor == mBuoyToAttack[1])
                && (mBuoyPositionVectorTwo[1].tempColor == mBuoyToAttack[0] || mBuoyPositionVectorTwo[1].tempColor == mBuoyToAttack[1]))
            {
                mFoundTwoFlag = true;
            }
            else
            {
                mFoundTwoFlag = false;
            }
        }
        
        /*else if(mDebounce[Red].GetDebounced() && mDebounce[Yellow].GetDebounced())
        {
            
        }
        
        else if(mDebounce[Green].GetDebounced() && mDebounce[Yellow].GetDebounced())
        {
            
        }*/
        
    }
    
    if (mState == Find || mState == Search || mState == DoPath || mState == Unvade || mState == PathSearch)
    {
        // Downward
        if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
        {
            printf("ERROR::Buoys2: Couldn't get downward camera 2 data");
            return 1;
        }
        //cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
        
        if(mDWProcessedImage == NULL)
        {
            mDWProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mDWProcessedImage);
    }
    
    
    
    return 0;
}

std::string Buoys2::ExitEarly()
{
    if (mStateTimeout.IsFinished())
    {
        mGlobalInfo->SetInfo("ExitEarlyToCounselor", 1);
        return NextMission;
    }
    else
    {
        return "KeepRunning";
    }
}

std::string Buoys2::Execute()
{
    //double centerI = 0;
    //double centerJ = 0;
    int validcount = 0;
    //double maxArea = 0;
    double maxWidth = 0;
    int tryDirection = 1;
    int placeMarker = 0;
    std::vector<Path> paths;
    
    double tempWidth = 0;
    double buoyClose = 1e6;
    double currBuoyClose = 0;
    int currBuoy = 0;
    
    bool _reset;
    
    switch (mState)
    {
        // Entry state, figure out what to do
        case Find:
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = 0;
            mDesired.mYaw = mYawFix;
            mDesired.mDepth = mDefaultDepth;
            
            mPathSearchPattern.InitSearch(mPathSearchStrafeTime, mPathSearchStrafeSpeed, mPathSearchForwardTime, mPathSearchForwardSpeed,mPathSearchSmoothSpeed, mPathSearchInitDirection);
            mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed, mSearchInitDirection);
            
            mPathSearchTimeout.Initialize(mPathSearchDuration);
            mPathSearchTimeout.Start();
            
            mState = PathSearch;
            
            /*
            if (mDebounce[Red].getDebounced() || mDebounce[Green].getDebounced() || mDebounce[Yellow].getDebounced())
            {
                mState = Approach;
            }
            else if(mPathFinder.GetPaths(mDownwardFrame, mFWProcessedImage, mDownwardFrameF).size() > 0)
            {
                mState = DoPath;
            }
            else
            {
                mState = Search;
            }*/
            break;
            
        // Search for the buoys
        case Search:
            if (mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mState = Approach;
            }
           /* else if(mPathsDebounce.Bounce(mPathFinder.GetPaths(mCurrent, mDownwardFrame, mProcessedDWImage, mDownwardFrameF).size() > 0))
            {
                //mState = DoPath;
            }*/
            else
            {
                // Search pattern
                mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
                mDesired.mYaw = mYawFix;
            }
            
            std::cout << "State: " << "Buoys2" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Go near a cluster of buoys
        case Approach:
            
            if(mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mCenterI = 0;
                mCenterJ = 0;
                for(int i = 0; i < 3; i++)
                {
                    if(mDebounce[i].GetDebounced())
                    {
                        mCenterI += mReturn[i].mCenterI;
                        mCenterJ += mReturn[i].mCenterJ;
                        validcount++;
                        tempWidth = mReturn[i].mMaxJ - mReturn[i].mMinJ;
                        if(tempWidth > maxWidth)
                        {
                            maxWidth = tempWidth;
                        }
                    }
                }
                
                mCenterI /= validcount;
                mCenterJ /= validcount;
                
                mDesired.mAxialThrust = mApproachThrust;
                mDesired.mLateralThrust = (mCenterI - mFWProcessedImage->width/2)*mLatPixelScale;
                mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mYaw = mYawFix;
                
                if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced() && fabs(mReturn[mBuoyToAttack[mAttackingBuoy]].mMaxJ - mReturn[mBuoyToAttack[mAttackingBuoy]].mMinJ) > mAttackWidthThresh)
                {
                    mState = Attack;
                }
                else if (maxWidth > mAttackWidthThresh)
                {
                    mState = FindActive;
                }
            }
            else
            {
                mState = Search;
                mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed, mSearchInitDirection);
                
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mDefaultDepth;
                mDesired.mYaw = mYawFix;
            }
            
            std::cout << "State: " << "Buoys2" << "::Approach" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " I: " << mCenterI
                    << " J: " << mCenterJ
                    << " vcount: " << validcount
                    << std::endl;
            break;
            
        // Find the buoy we're looking for
        case FindActive:
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced() && fabs(mReturn[mBuoyToAttack[mAttackingBuoy]].mMaxJ - mReturn[mBuoyToAttack[mAttackingBuoy]].mMinJ) > mAttackWidthThresh)
            {
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mDefaultDepth;
                mDesired.mYaw = mYawFix;
                
                mFindActiveFirstTime = true;
                mState = Attack;
            }
            else
            {
                if(mFoundAllThreeFlag)
                {
                    
                    for(int i = 0; i < 3; i++)
                    {
                        currBuoyClose = fabs(mFWProcessedImage->width/2 - mReturn[i].mCenterI);
                        if(currBuoyClose < buoyClose)
                        {
                            buoyClose = currBuoyClose;
                            currBuoy = i;
                        }
                    }
                    
                    if(buoyClose < 1e6)
                    {
                        if(currBuoy == mBuoyOrder[0])
                        {
                            tryDirection = 1;
                        }
                        else if(currBuoy == mBuoyOrder[2])
                        {
                            tryDirection = -1;
                        }
                        else
                        {
                            if(mBuoyToAttack[mAttackingBuoy] == mBuoyOrder[0])
                            {
                                tryDirection = -1;
                            }
                            else
                            {
                                tryDirection = 1;
                            }
                        }
                    }
                    /*else
                    {
                        //choose dir random
                        tryDirection = 1;
                    }*/
                }
                else if(mFoundTwoFlag == true && mFoundAllThreeFlag == false)
                {
                    for(int i = 0; i < 2; i++)
                    {
                        currBuoyClose = fabs(mFWProcessedImage->width/2 - mReturn[(int)mBuoyPositionVectorTwo[i].tempColor].mCenterI);
                        if(currBuoyClose < buoyClose)
                        {
                            buoyClose = currBuoyClose;
                            currBuoy = i;
                        }
                    }
                    
                    if(buoyClose < 1e6)
                    {
                        if(currBuoy == mBuoyPositionVectorTwo[0].tempColor)
                        {
                            tryDirection = 1;
                        }
                        else if(currBuoy == mBuoyPositionVectorTwo[1].tempColor)
                        {
                            tryDirection = -1;
                        }
                        /*else
                        {
                            if(mBuoyToAttack[mAttackingBuoy] == mBuoyOrder[0])
                            {
                                tryDirection = -1;
                            }
                            else
                            {
                                tryDirection = 1;
                            }
                        }*/
                    }
                }
                else
                {
                    tryDirection = 1;
                }
                
                
                
                /*tryDirection = 1;
                placeMarker = -1;
                for (int i=0; i<3; i++)
                {
                    if (mBuoyOrder[i] != -1 && mBuoyOrder[i] == mBuoyToAttack[mAttackingBuoy])
                    {
                        placeMarker = i;
                    }
                    if (mBuoyOrder[i] != -1 && placeMarker != -1 && mDebounce[mBuoyOrder[i]].GetDebounced())
                    {
                        tryDirection = -1;
                    }
                }*/
                
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mDefaultDepth;
                mDesired.mYaw = mYawFix;
                
                if (!mFindActiveTimerActive)
                {
                    mFindActiveTimerActive = true;
                    mFindActiveTimer.Initialize((mFindActiveFirstTime)? mFindActiveDuration/2 : mFindActiveDuration);
                    mFindActiveFirstTime = false;
                    mFindActiveTimer.Start();
                }
                else if (mFindActiveTimer.IsFinished())
                {
                    mFindActiveTimerActive = false;
                    mFindActiveModifier = -mFindActiveModifier;
                }
                
                mDesired.mLateralThrust = mFindActiveModifier*tryDirection*mFindActiveLateralThrust;
                mDesired.mAxialThrust = 0;
            }
            
            std::cout << "State: " << "Buoys2" << "::FindActive" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " Axial: " << mDesired.mAxialThrust
                    << " Lat: " << mDesired.mLateralThrust
                    << " currBuoy: " << currBuoy
                    << " Dir: " << tryDirection
                    << std::endl;
                    /*std::cout 
                    //<< " I: " << mCenterI
                    //<< " J: " << mCenterJ
                    
                    << std::endl;*/
            break;
            
        // Attack the life vest!
        case Attack:
        double area;
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mCenterI = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterI;
                mCenterJ = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterJ;
                mWidth = fabs(mReturn[mBuoyToAttack[mAttackingBuoy]].mMaxI - mReturn[mBuoyToAttack[mAttackingBuoy]].mMinI);
                area = mReturn[mBuoyToAttack[mAttackingBuoy]].mArea;
                
                //if ((mLaserDist <= mLaserCloseDist && mLaserDist >= 1.0) && mCenteredDebounce.Bounce(fabs(mCenterI - mFWProcessedImage->width/2) < mIThresh && fabs(mCenterJ - mFWProcessedImage->height/2) < mJThresh))
                mCenteredDebounce.Bounce(fabs(mCenterI - mFWProcessedImage->width/2) < mIThresh && fabs(mCenterJ - mFWProcessedImage->height/2) < mJThresh);
                if ((mWidth >= mTargetWidth) && mCenteredDebounce.GetDebounced() == true || mCurrLaserDist <= mLaserCloseDist) //  || (mLaserDist <= mLaserCloseDist && mLaserDist >= 1.0)
                {
                    mLockAttackTimer.Initialize(mLockAttackDuration);
                    mLockAttackTimer.Start();
                    mState = LockAttack;
                    
                    mCenteredDebounce.Miss();
                }
            
                mLostTargetDebounce.Miss();
            }
            else if (mLostTargetDebounce.Hit())
            {
                mState = FindActive;
            }
            
            if (mCenteredDebounce.GetDebounced())
            {
                mDesired.mAxialThrust = ((mTargetWidth + 10 ) - mWidth);//*mAxialPixelScale;
            }
            else
            {
                mDesired.mAxialThrust = 0;
            }
            
            //mDesired.mAxialThrust = 50;
            mDesired.mLateralThrust = (mCenterI - mFWProcessedImage->width/2)*mLatPixelScale;
            mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2-50)*mDepthPixelScale;
            mDesired.mYaw = mYawFix;
            
            std::cout << "State: " << "Buoys2" << "::Attack" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    //<< " Laser: " << mLaserDist
                    << " Width: " << mWidth
                    << " Area: " << area
                    //<< " mAxialPixelScale: " << mAxialPixelScale
                    << " DesiredAxialThrust: " << mDesired.mAxialThrust
                    << " LateralThrust: " << mDesired.mLateralThrust
                    << std::endl;
            break;
            
        // Lock in the attack and drive forward!
        case LockAttack:
            mDesired.mLateralThrust = 0;
            mDesired.mAxialThrust = mAttackThrust;
            mDesired.mYaw = mYawFix;
            
            if (mLockAttackTimer.IsFinished())
            {
                mState = Unhit;
                
                mLockAttackTimer.Initialize(mLockUnhitDuration);
                mLockAttackTimer.Start();
            }
            
            std::cout << "State: " << "Buoys2" << "::LockAttack" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Back up
        case Unhit:
            mDesired.mLateralThrust = 0;
            mDesired.mAxialThrust = mUnhitThrust;
            mDesired.mYaw = mYawFix;
            
            if (mLockAttackTimer.IsFinished())
            {
                mAttackingBuoy++;
                if (mAttackingBuoy == 1)
                    mState = FindActive;
                else
                {
                    mState = Evade;
                    mEvadeTime = CxUtils::GetTimeSeconds();
                }
            }
            
            std::cout << "State: " << "Buoys2" << "::Unhit" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Lateral until you can't see the buoys
        case Evade:
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = mEvadeLatThrust;
            mDesired.mYaw = mYawFix;
            if (!(mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced()))
            {
                mEvadeTime = CxUtils::GetTimeSeconds() - mEvadeTime;
                mEvadeTimer.Initialize(mEvadeLockTime);
                mEvadeTimer.Start();
                mState = EvadeLock;
            }
            
            std::cout << "State: " << "Buoys2" << "::Evade" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Drive forward for a bit
        case EvadeLock:
            mDesired.mLateralThrust = 0;
            mDesired.mAxialThrust = mEvadeLockThrust;
            mDesired.mYaw = mYawFix;
            if (mEvadeTimer.IsFinished())
            {
                mEvadeTimer.Initialize(mEvadeTime);
                mEvadeTimer.Start();
                mState = Unvade;
            }
            
            std::cout << "State: " << "Buoys2" << "::EvadeLock" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Lateral back the other direction
        case Unvade:
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = -mEvadeLatThrust;
            mDesired.mYaw = mYawFix;
            
            // Drive around, then go to path
            paths = mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage);
            if (paths.size() > 0 || mEvadeTimer.IsFinished())
            {
                mState = Exit;
            }
            
            std::cout << "State: " << "Buoys2" << "::Unvade" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Look for path or buoys
        case PathSearch:
            if (mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mState = DoLineup;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
            }
            else if (mPathsDebounce.Bounce(mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0))
            {
                mState = DoPath;
                
                mPathSearchTimeout.Initialize(20);
                mPathSearchTimeout.Start();
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
            }
            else if (mPathSearchTimeout.IsFinished())
            {
                mState = Search;
                mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed, mSearchSmoothSpeed, mSearchInitDirection);
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
                
                
            }
            else
            {
                mPathSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
                mDesired.mYaw = mYawFix;
            }
            
            std::cout << "State: " << "Buoys2" << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Look for path or buoys
        case DoLineup:
            validcount = 0;
            _reset = true;
            for (int i=0; i<3; i++)
            {
                if (mDebounce[i].GetDebounced())
                {
                    if (_reset)
                    {
                        mCenterI = 0;
                        mCenterJ = 0;
                        _reset = false;
                    }
                    
                    mCenterI += mReturn[i].mCenterI;
                    mCenterJ += mReturn[i].mCenterJ;
                    
                    if (fabs(mReturn[mBuoyToAttack[i]].mMaxJ - mReturn[mBuoyToAttack[i]].mMinJ) > maxWidth)
                        maxWidth = fabs(mReturn[mBuoyToAttack[i]].mMaxJ - mReturn[mBuoyToAttack[i]].mMinJ);
                    
                    validcount++;
                }
            }
            
            if (validcount > 0)
            {
                mCenterI = mCenterI/((double)validcount);
                mCenterJ = mCenterJ/((double)validcount);
                
                //mDesired.mAxialThrust = 40;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2)*mDepthPixelScale;
                mYawFix = mDesired.mYaw = mCurrent.mYaw + (mCenterI - mFWProcessedImage->width/2)*mYawPixelScale;
            }
            
            if (fabs(mCenterI - mFWProcessedImage->width/2) < mIThresh)
            {
                mPathTimer.Initialize(mPathExitTime);
                mPathTimer.Start();
                mYawFix = mDesired.mYaw;
                mState = Approach;
            }
            
            mDesired.mAxialThrust = 40;
            
            std::cout << "State: " << "Buoys2" << "::DoLineup" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " DesiredYaw: " << mDesired.mYaw
                    << " DesiredDepth: " << mDesired.mDepth
                    << " Depth: " << mCurrent.mDepth
                    << std::endl;
            break;
            
        // Do the path stuff
        case DoPath:
            mDesired.mDepth = mDefaultDepth;
            // Lock on a path, then Exit
            paths = mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage);
            if (paths.size() > 0)
            {
                if (mPathDebounce.Bounce(mPathFinder.StepPath(&paths[0], mCurrent, mDesired)))
                {
                    //std::cout << "exit" << std::endl;
                    //mState = Exit;
                    
                    mPathTimer.Initialize(mPathExitTime);
                    mPathTimer.Start();
                    mYawFix = mCurrent.mYaw;
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
                mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed, mSearchInitDirection);
            }
            
            std::cout << "State: " << "Buoys2" << "::Path" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " DepthDesired: " << mDesired.mDepth
                    << " AxialDesired: " << mDesired.mAxialThrust
                    << " LatDesired: " << mDesired.mLateralThrust
                    << " YawDesired: " << mDesired.mYaw
                    << std::endl;
            break;
            
        // Exit Path
        case ExitPath:
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mLateralThrust = 0;
        
            if(mPathTimer.IsFinished())
            {
                mState = Search;
                mSearchPattern.InitSearch(mSearchStrafeTime, mSearchStrafeSpeed, mSearchForwardTime, mSearchForwardSpeed,mSearchSmoothSpeed, mSearchInitDirection);
            }
            else if(mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mState = Approach;
            }
            else
            {
                mState = ExitPath;
            }
            
            std::cout << "State: " << "Buoys2" << "::ExitPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw
                    << " Depth: " << mCurrent.mDepth
                    << " AxialDesired: " << mDesired.mAxialThrust
                    << " LatDesired: " << mDesired.mLateralThrust
                    << " YawDesired: " << mDesired.mYaw
                    << std::endl;
            break;
            
        // Go onto the next state
        case Exit:
            std::cout << "State: " << "Buoys2" << "::Exit" << std::endl;
            std::cout << "Going to love lane state"
                    << std::endl;
            Utility::Display::CloseAllWindows();
            return NextMission;
    }
    
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
    
    for(int i = 0; i < mLaserObjects.size(); i++)
    {
        x = (mLaserObjects[i].mX*(0) - mLaserObjects[i].mY*(-1))*500;
        y = (mLaserObjects[i].mX*(-1) + mLaserObjects[i].mY*(0))*500;
        CvFont font;
        cvInitFont(&font,CV_FONT_HERSHEY_SIMPLEX, 0.5,0.5,0,1);
        
        std::cout << "mCurrLaserDist: " << mCurrLaserDist << std::endl;
        if(mCurrLaserDist < 1000)
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
    
    cvShowImage("LaserData", mDisplayLaser);
    cvWaitKey(10);
    
    AI::Utility::HeadingDisplay(mFWProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mFWProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mFWProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    
    Utility::Display::DisplayImage("Post Processing FW",mFWProcessedImage);
    Utility::Display::DisplayImage("Post Processing DW",mDWProcessedImage);
    Utility::Display::DisplayImage("Raw Image FW",mForwardFrame);
    Utility::Display::DisplayImage("Raw Image DW",mDownwardFrame);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}
