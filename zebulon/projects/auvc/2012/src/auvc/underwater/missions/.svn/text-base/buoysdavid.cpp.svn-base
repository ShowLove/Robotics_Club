#include <auvc/underwater/missions/buoysdavid.h>

using namespace Zebulon;
using namespace Underwater;

const std::string BuoysDavid::NextMission = "ObstacleCourse";

BuoysDavid::BuoysDavid()
{
    ID = "Buoys";
    
    mForwardFrame = NULL;
    mFWProcessedImage = NULL;
    mDWProcessedImage = NULL;
    mDownwardFrame = NULL;
    
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Initialize(2, false);
    }
    
    mFoundTwoFlag = false;
    mFoundAllThreeFlag = false;
    
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(7, true);
    mLostTargetDebounce.Initialize(8, false);
    
    mCenterI = 0;
    mCenterJ = 0;

    mAttackingBuoy = 0;
}

BuoysDavid::~BuoysDavid()
{//mAreaCloseThresh
    //cvReleaseImage(&mFWProcessedImage);
    //cvReleaseImage(&mProcessedDWImage);
}

int BuoysDavid::Init()
{
    mState = Find;
    //mState = DoPath;
    int temp;
    mXmlConf.GetVar("Underwater.BuoysDavid.Default@Timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    mXmlConf.GetVar("Underwater.BuoysDavid.BuoyToHit@Red", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Red;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Red;
    }
    
    mXmlConf.GetVar("Underwater.BuoysDavid.BuoyToHit@Green", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Green;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Green;
    }
    
    mXmlConf.GetVar("Underwater.BuoysDavid.BuoyToHit@Yellow", temp);
    if (temp == 1)
    {
        mBuoyToAttack[0] = Yellow;
    }
    else if (temp == 2)
    {
        mBuoyToAttack[1] = Yellow;
    }
    
    // Default depth
    mXmlConf.GetVar("Underwater.BuoysDavid.Default@Depth", mDesired.mDepth);
    mDefaultDepth = mDesired.mDepth;
    
    // Fixed Yaw
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mYawFix))
    {
        printf("ERROR::BuoysDavid: Couldn't get Compass data");
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

int BuoysDavid::ErrorCheck()
{
    return 0;
}

int BuoysDavid::FetchProcess()
{
    mBuoyObject[Red].Initialize("RedBuoy");
    mBuoyObject[Yellow].Initialize("YellowBuoy");
    mBuoyObject[Green].Initialize("GreenBuoy");
    mStartGate.Initialize("StartGate");
    //////Defaults////////////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.Default@Pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.BuoysDavid.Default@Roll", mDesired.mRoll);
    
    //////Color Values////////////////////////////
    
    //////Pixel Scale////////////////////////////
    double num, denom;
    mXmlConf.GetVar("Underwater.BuoysDavid.LatPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.BuoysDavid.LatPixelScale@Denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysDavid.DepthPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.BuoysDavid.DepthPixelScale@Denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysDavid.AxialPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.BuoysDavid.AxialPixelScale@Denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.BuoysDavid.YawPixelScale@Num", num);
    mXmlConf.GetVar("Underwater.BuoysDavid.YawPixelScale@Denom", denom);
    mYawPixelScale = num/denom;
    
    //////State-Specific//////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.FindActive@LateralThrust", mFindActiveLateralThrust);
    mXmlConf.GetVar("Underwater.BuoysDavid.FindActive@Duration", mFindActiveDuration);
    
    mXmlConf.GetVar("Underwater.BuoysDavid.Attack@LockDuration", mLockAttackDuration);
    mXmlConf.GetVar("Underwater.BuoysDavid.Attack@Thrust", mAttackThrust);
    
    mXmlConf.GetVar("Underwater.BuoysDavid.Unhit@LockDuration", mLockUnhitDuration);
    mXmlConf.GetVar("Underwater.BuoysDavid.Unhit@Thrust", mUnhitThrust);
    
    mXmlConf.GetVar("Underwater.BuoysDavid.Approach@Thrust", mApproachThrust);
    mXmlConf.GetVar("Underwater.BuoysDavid.Approach@AttackWidthThresh", mAttackWidthThresh);
    
    //////Search//////////////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@StrafeTime", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@StrafeSpeed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@ForwardTime", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@ForwardSpeed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@SmoothSpeed", mSearchSmoothSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.Search@InitDirection", mSearchInitDirection);
    
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@StrafeTime", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@StrafeSpeed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@ForwardTime", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@ForwardSpeed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@SmoothSpeed", mPathSearchSmoothSpeed);
    mXmlConf.GetVar("Underwater.BuoysDavid.PathSearch@InitDirection", mPathSearchInitDirection);
    
    //////Evade///////////////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.Evade@LateralThrust", mEvadeLatThrust);
    mXmlConf.GetVar("Underwater.BuoysDavid.Evade@LockTime", mEvadeLockTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.Evade@LockThrust", mEvadeLockThrust);
    
    //////Thresholds//////////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@IThresh", mIThresh);
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@JThresh", mJThresh);
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@AreaCloseThresh", mAreaCloseThresh);
    
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@MinArea", mMinArea);
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@MaxArea", mMaxArea);
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@TargetArea", mTargetArea);
    mXmlConf.GetVar("Underwater.BuoysDavid.Buoy@TargetWidth", mTargetWidth);
    
    //////Path////////////////////////////////////
    mXmlConf.GetVar("Underwater.BuoysDavid.Path@PathExitTime", mPathExitTime);
    mXmlConf.GetVar("Underwater.BuoysDavid.Path@ExitPathThrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.BuoysDavid.Path@PathSearchDuration", mPathSearchDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::BuoysDavid: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::BuoysDavid: Couldn't get depth data");
        return 1;
    }
    
//    if (mState != DoPath && mState != Unvade)
//    {
//        // Forward Camera
        if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
        {
            printf("ERROR::BuoysDavid: Couldn't get forward camera data");
            return 1;
        }
        
        if(mFWProcessedImage == NULL)
        {
            mFWProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mFWProcessedImage);
        
        cvCopy(mForwardFrame,mFWProcessedImage);
//        for (int i=0; i<3; i++)
//        {
//            mDebounce[i].Bounce(mBuoyObject[i].GetVisionReturn(mForwardFrame, mReturn[i], mFWProcessedImage));
//        }
//    }
    
//    if (mState == Find || mState == Search || mState == DoPath || mState == Unvade || mState == PathSearch)
//    {
//        // Downward
        if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
        {
            printf("ERROR::BuoysDavid: Couldn't get downward camera 2 data");
            return 1;
        }
//        //cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
        
        if(mDWProcessedImage == NULL)
        {
            mDWProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mDWProcessedImage);
        cvCopy(mDownwardFrame,mDWProcessedImage);
//    }
    
    
    
    return 0;
}

void BuoysDavid::Vision()
{
    if(mForwardFrame == NULL) return;

    mBuoyObject[Red].GetVisionReturn(mForwardFrame,mReturn[Red], mFWProcessedImage);
    mBuoyObject[Green].GetVisionReturn(mForwardFrame,mReturn[Green],mFWProcessedImage);
    mBuoyObject[Yellow].GetVisionReturn(mForwardFrame,mReturn[Yellow],mFWProcessedImage);
    //mStartGate.GetVisionReturn(mForwardFrame,mGateReturn,mFWProcessedImage);

    mDebounce[Red].Bounce(mReturn[Red].mValid);// && mBuoyObject[Red].mArea > mMinPixelArea && mBuoyObject[Red].mArea < mMaxPixelArea);
    mDebounce[Green].Bounce(mReturn[Green].mValid);// && mBuoyObject[Green].mArea > mMinPixelArea && mBuoyObject[Green].mArea < mMaxPixelArea);
    mDebounce[Yellow].Bounce(mReturn[Yellow].mValid);// && mBuoyObject[Yellow].mArea > mMinPixelArea && mBuoyObject[Yellow].mArea < mMaxPixelArea);

//    if(mInitFrame == NULL) return;
//   // cvFlip(mInitFrame,mInitFrame,1);
//    if(mFinalFrame == NULL && mInitFrame != NULL)
//    {
//        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);

//        //mFinalFrame = cvCloneImage(mInitFrame);
//        //std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
//    }
//    cvSetImageROI(mInitFrame, cvRect(0,mInitFrame->height/3,mInitFrame->width,mInitFrame->height));
//    IplImage * copyinit = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);
//    cvCopyImage(mInitFrame, mFinalFrame);
//    cvCopyImage(mInitFrame, copyinit);
//    cvResetImageROI(mInitFrame);
////    Utility::Display::DisplayImage("input frame", copyinit);
//    mRedBuoyClassifier.GetVisionReturn(copyinit,mRed,mFinalFrame);
//    mYellowBuoyClassifier.GetVisionReturn(copyinit,mYellow,mFinalFrame);
//    mGreenBuoyClassifier.GetVisionReturn(copyinit,mGreen,mFinalFrame);
//    mBlueBuoyClassifier.GetVisionReturn(copyinit,mBlue,mFinalFrame);
//    //mOrangeBuoyClassifier.GetVisionReturn(copyinit,mOrange,mFinalFrame);
//    //draw maxj line
//    cvDrawLine(mFinalFrame,cvPoint(0,(int)mMaxJ),cvPoint(mFinalFrame->width,(int)mMaxJ),CV_RGB(255,255,255));

//    //draw collision lines
//    cvDrawLine(mFinalFrame,cvPoint(mInitFrame->width/2,(int)mMaxJ),cvPoint(mFinalFrame->width/3,mFinalFrame->height),CV_RGB(128,128,128));
//    cvDrawLine(mFinalFrame,cvPoint(mInitFrame->width/2,(int)mMaxJ),cvPoint(mFinalFrame->width*(2./3.),mFinalFrame->height),CV_RGB(128,128,128));

//    //cvDrawLine(mFinalFrame,cvPoint(0,(int)mMaxJ),cvPoint(mFinalFrame->width,(int)mMaxJ),CV_RGB(255,255,255));
//    cvReleaseImage(&copyinit);

//    mRedDebounce.Bounce(mRed.mValid && mRed.mArea > mMinPixelArea && mRed.mArea < mMaxPixelArea);
//    mGreenDebounce.Bounce(mGreen.mValid && mGreen.mArea > mMinPixelArea && mGreen.mArea < mMaxPixelArea);
//    mYellowDebounce.Bounce(mYellow.mValid && mYellow.mArea > mMinPixelArea && mYellow.mArea < mMaxPixelArea);
//    mBlueDebounce.Bounce(mBlue.mValid && mBlue.mArea > mMinPixelArea && mBlue.mArea < mMaxPixelArea);
}

std::string BuoysDavid::ExitEarly()
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
//calculates a relative angle to buoy with pos being to right, neg being to left.
//used to add to current heading. should be made more generic as other missions use this.
double BuoysDavid::AngleToBuoy(const Zebulon::Vision::Return& buoy)
{
    CxUtils::Point3D bottomcenter;
    CxUtils::Point3D buoycenter;
    if(mForwardFrame == NULL) return 0;
    bottomcenter.mX = mForwardFrame->width/2.0;
    bottomcenter.mY = mForwardFrame->height;

    buoycenter.mX = buoy.mCenterI;
    buoycenter.mY = buoy.mCenterJ;

    CxUtils::Segment3D pathtobuoy(bottomcenter,buoycenter);
    double angle = pathtobuoy.GetAngleXY(false);
    angle = CxUtils::Orientation::AddToAngle(angle,CxUtils::CX_HALF_PI);
    std::cout << "angle to buoy: " <<  CxUtils::CxToDegrees(angle ) << std::endl;
    return angle;

}

std::string BuoysDavid::Execute()
{
    std::cout << "===================================================" << std::endl;
    
    int validcount = 0;
    double maxArea = 0;
    int tryDirection = 1;
    int placeMarker = 0;
    std::vector<Path> paths;
    
    bool _reset;
    Vision();
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
            
            mState = Search;//PathSearch;
            break;
            
        // Search for the buoys
        case Search:
            if (mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mState = Approach;
            }
            else
            {
                // Search pattern
                mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
                mDesired.mYaw = mYawFix;
            }
            
            std::cout << "State: " << "BuoysDavid" << "::Search" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << std::endl;
            break;
            
        // Go near a cluster of buoys
        case Approach:
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
                    
                    mBuoyOrder[i] = i;
                    
                    mCenterI += mReturn[i].mCenterI;
                    mCenterJ += mReturn[i].mCenterJ;
                    
                    double tmp_area = mReturn[i].mArea;
                    
                    if (tmp_area > maxArea)
                        maxArea = tmp_area;
                    
                    validcount++;
                }
                else
                {
                    mBuoyOrder[i] = -1;
                }
            }
            
            if (validcount > 0)
            {
                mCenterI /= validcount;
                mCenterJ /= validcount;
                
                // Sort it all out;
                for (int i=0; i<3; i++)
                {
                    for (int j=i; j<3; j++)
                    {
                        if ((mBuoyOrder[i] == -1 && mBuoyOrder[j] != -1) || (mBuoyOrder[i] != -1 && mBuoyOrder[j] != -1 && mReturn[mBuoyOrder[j]].mCenterI < mReturn[mBuoyOrder[i]].mCenterI))
                        {
                            int tmp = mBuoyOrder[i];
                            mBuoyOrder[i] = mBuoyOrder[j];
                            mBuoyOrder[j] = tmp;
                        }
                    }
                }
                
                /*for(int i = 0; i < 3; i++)
                {
                    std::cout << "mBuoyOrder " << i << ": " << mBuoyOrder[i] << std::endl;
                }*/
                
                mDesired.mAxialThrust = mApproachThrust;
                mDesired.mLateralThrust = (mCenterI - mFWProcessedImage->width/2)*mLatPixelScale;
                mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mYaw = mYawFix;
                
                if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
                {
                    mState = Attack;
                }
                else if (maxArea > mAreaCloseThresh)
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
            
            std::cout << "State: " << "BuoysDavid" << "::Approach" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << " Area: " << maxArea
                    << " I: " << mCenterI
                    << " J: " << mCenterJ
                    << " vcount: " << validcount
                    << std::endl;
            break;
            
        // Find the buoy we're looking for
        case FindActive:
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
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
                tryDirection = 1;
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
                }
                
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
            
            std::cout << "State: " << "BuoysDavid" << "::FindActive" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << " I: " << mCenterI
                    << " J: " << mCenterJ
                    << std::endl;
            break;
            
        // Attack the life vest!
        case Attack:
        
            if (mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                
                mCenterI = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterI;
                mCenterJ = mReturn[mBuoyToAttack[mAttackingBuoy]].mCenterJ;
                mWidth = 0;
                //mWidth = fabs(mReturn[mBuoyToAttack[mAttackingBuoy]].mMaxI - mReturn[mBuoyToAttack[mAttackingBuoy]].mMinI);
                //area = mReturn[mBuoyToAttack[mAttackingBuoy]].size.width * mReturn[mBuoyToAttack[mAttackingBuoy]].size.height;
                //mReturn[mBuoyToAttack[mAttackingBuoy]].mArea;
                
                mCenteredDebounce.Bounce(fabs(mCenterI - mFWProcessedImage->width/2) < mIThresh && fabs(mCenterJ - mFWProcessedImage->height/2) < mJThresh);
                if ((mReturn[mBuoyToAttack[mAttackingBuoy]].mArea >= mTargetArea) && mCenteredDebounce.GetDebounced())
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
            
            /*if (mCenteredDebounce.GetDebounced())
            {
                mDesired.mAxialThrust = 40;
                //area = mReturn[mBuoyToAttack[mAttackingBuoy]].size.width * mReturn[mBuoyToAttack[mAttackingBuoy]].size.height;
                //mDesired.mAxialThrust = ((mTargetWidth + 10 ) - mWidth);//*mAxialPixelScale;
            }
            else
            {
                mDesired.mAxialThrust = 0;
            }*/
            
            if (mCenteredDebounce.GetDebounced())
            {
                mDesired.mAxialThrust = 40;
                //area = mReturn[mBuoyToAttack[mAttackingBuoy]].size.width * mReturn[mBuoyToAttack[mAttackingBuoy]].size.height;
                //mDesired.mAxialThrust = ((mTargetWidth + 10 ) - mWidth);//*mAxialPixelScale;
            }
            else if(mDebounce[mBuoyToAttack[mAttackingBuoy]].GetDebounced())
            {
                mDesired.mAxialThrust = (8000 - mReturn[mBuoyToAttack[mAttackingBuoy]].mArea)*mAxialPixelScale;
                //mDesired.mAxialThrust = ((mTargetWidth + 10 ) - mWidth)*mAxialPixelScale;
            }
            else
            {
                mDesired.mAxialThrust = 0;
            }
            
            mDesired.mLateralThrust = (mCenterI - mFWProcessedImage->width/2)*mLatPixelScale;
            mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mFWProcessedImage->height/2-50)*mDepthPixelScale;
            mDesired.mYaw = mYawFix;
            
            std::cout << "State: " << "BuoysDavid" << "::Attack" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << " Width: " << mWidth
                    << " Area: " << mReturn[mBuoyToAttack[mAttackingBuoy]].mArea
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
            
            std::cout << "State: " << "BuoysDavid" << "::LockAttack" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
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
            
            std::cout << "State: " << "BuoysDavid" << "::Unhit" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
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
            
            std::cout << "State: " << "BuoysDavid" << "::Evade" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
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
            
            std::cout << "State: " << "BuoysDavid" << "::EvadeLock" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
                    << std::endl;
            break;
            
        // Lateral back the other direction
        case Unvade:
            mDesired.mAxialThrust = 0;
            mDesired.mLateralThrust = -mEvadeLatThrust;
            mDesired.mYaw = mYawFix;
            
            // Drive around, then go to path
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage);
            if (paths.size() > 0 || mEvadeTimer.IsFinished())
            {
                mState = Exit;
            }
            
            std::cout << "State: " << "BuoysDavid" << "::Unvade" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
                    << std::endl;
            break;
            
        // Look for path or buoys
        case PathSearch:
            /*if (mDebounce[Red].GetDebounced() || mDebounce[Green].GetDebounced() || mDebounce[Yellow].GetDebounced())
            {
                mState = DoLineup;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mYawFix;
            }
            else */if (mPathsDebounce.Bounce(mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0))
            {
                mState = DoPath;
                
                mPathSearchTimeout.Initialize(30);
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
            
            std::cout << "State: " << "BuoysDavid" << "::PathSearch" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
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
            
            std::cout << "State: " << "BuoysDavid" << "::DoLineup" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
                    << std::endl;
            break;
            
        // Do the path stuff
        case DoPath:
            mDesired.mDepth = mDefaultDepth;
            // Lock on a path, then Exit
            paths = mPathFinder.GetPathsDavid(mCurrent, mDownwardFrame, mDWProcessedImage);
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
            
            std::cout << "State: " << "BuoysDavid" << "::Path" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << " AxialDesired: " << mDesired.mAxialThrust
                    << " LatDesired: " << mDesired.mLateralThrust
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
            
            std::cout << "State: " << "BuoysDavid" << "::ExitPath" << std::endl;
            std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
                    << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth << std::endl
                    << " AxialDesired: " << mDesired.mAxialThrust
                    << " LatDesired: " << mDesired.mLateralThrust
                    << std::endl;
            break;
            
        // Go onto the next state
        case Exit:
            std::cout << "State: " << "BuoysDavid" << "::Exit" << std::endl;
            std::cout << "Going to love lane state"
                    << std::endl;
            Utility::Display::CloseAllWindows();
            return NextMission;
    }
    
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
