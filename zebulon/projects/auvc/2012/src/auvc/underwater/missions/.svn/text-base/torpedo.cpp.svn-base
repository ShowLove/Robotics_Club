#include "auvc/underwater/missions/torpedo.h"

using namespace Zebulon;
using namespace Underwater;

const std::string Torpedo::NextMission = "Pinger";

Torpedo::Torpedo()
{
    ID = "Torpedo";
    
    mForwardFrame = NULL;
    mDownwardFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mProcessedImage = NULL;
    
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
    
    mPathDebounce.Initialize(3, true);
    mCenteredDebounce.Initialize(10, true);
    mLostTargetDebounce.Initialize(8, true);
    
    mDisplayLaser = NULL;
    mDisplayLaser = cvCreateImage(cvSize(640,480), IPL_DEPTH_8U, 3);
}

Torpedo::~Torpedo()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mSegFrame);
    cvReleaseImage(&mThreshFrame);
    cvReleaseImage(&mProcessedImage);
    
    delete mSegment;
    delete mThreshold;
}

int Torpedo::Init()
{
    mState = Find;
    
    int temp;
    mXmlConf.GetVar("Underwater.Torpedo.Default@timeout", temp);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw))
    {
        printf("ERROR::Torpedo: Couldn't get Compass data");
        return 1;
    }
    
    // Default depth
    mXmlConf.GetVar("Underwater.Torpedo.Default@depth", mDesired.mDepth);
    
    mPathFinder.Initialize();
    mPathSearchTimeout.Initialize(20);
    mPathSearchTimeout.Start();
    
    return 0;
}

int Torpedo::ErrorCheck()
{
    return 0;
}

int Torpedo::FetchProcess()
{
    mXmlConf.GetVar("Underwater.Torpedo.Default@depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.Torpedo.Default@pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.Torpedo.Default@roll", mDesired.mRoll);
    
    mXmlConf.GetVar("Underwater.Torpedo.Target@red", mTarget.mRed);
    mXmlConf.GetVar("Underwater.Torpedo.Target@green", mTarget.mGreen);
    mXmlConf.GetVar("Underwater.Torpedo.Target@blue", mTarget.mBlue);
    mXmlConf.GetVar("Underwater.Torpedo.TargetThresh@red", mTarget.mRedRange);
    mXmlConf.GetVar("Underwater.Torpedo.TargetThresh@green", mTarget.mGreenRange);
    mXmlConf.GetVar("Underwater.Torpedo.TargetThresh@blue", mTarget.mBlueRange);
    
    
    
    double num, denom;
    mXmlConf.GetVar("Underwater.Torpedo.LatPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Torpedo.LatPixelScale@denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Torpedo.DepthPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Torpedo.DepthPixelScale@denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Torpedo.YawPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Torpedo.YawPixelScale@denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Torpedo.AxialPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Torpedo.AxialPixelScale@denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Torpedo.Targets@min_area", mMinArea);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@max_area", mMaxArea);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@I_thresh", mIThresh);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@J_thresh", mJThresh);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@area", mTargetArea);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@area_thresh", mAreaThresh);
    mXmlConf.GetVar("Underwater.Torpedo.Targets@J_offset", mCenterOffsetJ);
    
    mXmlConf.GetVar("Underwater.Torpedo.Advance@thrust", mAdvanceThrust);
    
    //mXmlConf.GetVar("Underwater.Torpedo.Depth@Offset", mDepthOffset);
    mDepthOffset = mDefaultDepth;
    
    mXmlConf.GetVar("Underwater.Torpedo.Search@strafe_time", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Torpedo.Search@strafe_speed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Torpedo.Search@forward_time", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.Torpedo.Search@forward_speed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Torpedo.Search@smooth_speed", mSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.Torpedo.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Torpedo.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Torpedo.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.Torpedo.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Torpedo.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.Torpedo.Path@path_exit_time", mPathExitTime);
    mXmlConf.GetVar("Underwater.Torpedo.Path@exit_path_thrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.Torpedo.Path@path_search_duration", mPathSearchDuration);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::Torpedo: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::Torpedo: Couldn't get depth data");
        return 1;
    }
    
    // Forward
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mForwardFrame))
    {
        printf("ERROR::Torpedo: Couldn't get forward camera data");
        return 1;
    }
    
    if(mProcessedImage == NULL)
    {
        mProcessedImage = cvCreateImage(cvSize(mForwardFrame->width,mForwardFrame->height), mForwardFrame->depth, mForwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mProcessedImage);
    
    mSegment->Setup(mTarget, 50, 50);
    mThreshold->Setup(mTarget);
    
    mThreshold->Process(mForwardFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    Vision::VisionUtil::CombineImages(mSegFrame, mProcessedImage);
    
    bool gotten = false;
    mVerticalCenter = 0;
    mSegment->GetMaxPosition(mTempReturn);
    mArea = mTargetArea;
    if (mTargetDebounce.Bounce(mTempReturn.mValid && mTempReturn.mArea > mMinArea && mTempReturn.mArea < mMaxArea) && mTempReturn.mValid)
    {
        mCenterI = mTempReturn.mCenterI;
        mCenterJ = mTempReturn.mCenterJ + mCenterOffsetJ;
        mArea = mTempReturn.mArea;
    }
    
    if(mState == Find || mState == DoPath || mState == PathSearch)
    {
        if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
        {
            printf("ERROR::Torpedo: Couldn't get downward camera data");
            return 1;
        }
        cvRectangle(mDownwardFrame, cvPoint(0,0), cvPoint(mDownwardFrame->width, 25), cvScalar(0,0,0), -1);
        
        if(mProcessedImage == NULL)
        {
            mProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
        }
        Vision::VisionUtil::ResetImage(mProcessedImage);
    }
    
    return 0;
}

std::string Torpedo::ExitEarly()
{
    if (mStateTimeout.IsFinished())
    {
        mGlobalCommand->SetTorpedo1(true);
        mGlobalCommand->SetTorpedo2(true);
        
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

std::string Torpedo::Execute()
{
    std::cout << "======================================" << std::endl
                << "State: " << ID << "::" << GetStateName(mState) << std::endl;
    
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
            
            break;
            
        // Search for the Torpedo
        case Search:
            if (mTargetDebounce.GetDebounced())
            {
                mState = Advance;
            }
            
            mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mStartYaw;
            
            break;
            
        // Find the depth of the target
        case Advance:
            if (mTargetDebounce.GetDebounced())
            {
                mLostTargetDebounce.Miss();
            }
            else if (mLostTargetDebounce.Hit())
            {
                mCenterI = mProcessedImage->width/2.0;
                mCenterJ = mProcessedImage->height/2.0;
                
                mState = Search;
            }
            
            //mDesired.mAxialThrust = mAdvanceThrust;
            mDesired.mAxialThrust = (mTargetArea + mAreaThresh - mArea)*mAxialPixelScale;
            //mDesired.mYaw = mStartYaw;
            mDesired.mLateralThrust = (mCenterI - mProcessedImage->width/2)*mLatPixelScale;
            
            mDesired.mYaw = mCurrent.mYaw + (mCenterI - mProcessedImage->width/2)*mYawPixelScale;
            //mDesired.mLateralThrust = 0;
            
            mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mProcessedImage->height/2)*mDepthPixelScale;
            
            if (mCenteredDebounce.Bounce(fabs(mCenterI - mProcessedImage->width/2) < mIThresh && fabs(mCenterJ - mProcessedImage->height/2) < mJThresh && (mTargetArea - mArea) < mAreaThresh))
            {
                mDepthOffset = mCurrent.mDepth;
                mState = Fire;
            }
            std::cout 
                    << " Lateral: " << mCurrent.mLateralThrust << " -> " << mDesired.mLateralThrust
                    << " Area: " << mArea << " => " << (mTargetArea - mArea)
                    << " :: " << mCenteredDebounce.GetDebounced()
                    << std::endl;
            break;
            
        // Drive through the Torpedo
        case Fire:
            mDesired.mAxialThrust = 0;
            mDesired.mYaw = mCurrent.mYaw;
            mDesired.mLateralThrust = 0;
            mDesired.mDepth = mDepthOffset;
            
            mGlobalCommand->SetTorpedo1(true);
            mGlobalCommand->SetTorpedo2(true);
            //mGlobalCommand->SetDropper2(true);
            
            mState = Exit;
            
            std::cout << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                    << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl;
            break;
            
        // Look for path or buoys
        case PathSearch:
            /*if (mTargetDebounce.GetDebounced())
            {
                mState = Advance;
                
                mDesired.mLateralThrust = 0;
                mDesired.mAxialThrust = 0;
                mDesired.mYaw = mStartYaw;
            }
            else */if (mPathFinder.GetPaths(mCurrent, mDownwardFrame, mProcessedImage).size() > 0)
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
                /*double tmp;
                mGlobalInfo->GetInfo(GlobalInfo::ExitEarlyToFinalMission, tmp);
                if (tmp != 0)
                {
                    return Zebulon::Underwater::FinalMission;
                }*/
                
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
            break;
            
        // Look for path or buoys
        case DoLineup:
            if(mTargetDebounce.GetDebounced())
            {
                mDesired.mAxialThrust = 0;
                mDesired.mLateralThrust = 0;
                mDesired.mDepth = mCurrent.mDepth + (mCenterJ - mProcessedImage->height/2)*mDepthPixelScale;
                mDesired.mYaw = ((mCenterI + mVerticalCenter)/2 - mProcessedImage->width/2)*mYawPixelScale;
            }
            
            if (fabs(mCenterI - mProcessedImage->width/2) < mIThresh)
            {
                mPathTimer.Initialize(mPathExitTime);
                mPathTimer.Start();
                mStartYaw = mCurrent.mYaw;
                mState = ExitPath;
            }
            break;
            
        // Locate & lock onto the path
        case DoPath:
            // Lock on a path, then Exit
            paths = mPathFinder.GetPaths(mCurrent, mDownwardFrame, mProcessedImage);
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
            break;
            
        case ExitPath:
        
            mDesired.mAxialThrust = mExitPathThrust;
            mDesired.mLateralThrust = 0;
            
            if(mPathTimer.IsFinished())
            {
                mState = Search;
            }
            else if(mTargetDebounce.GetDebounced())
            {
                //mState = GetDepth;
                mState = Advance;
            }
            else
            {
                mState = ExitPath;
            }
            break;
            
        // Proceed to the next state
        case Exit:
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
    
    // Please, if you are going to display an image, use Utility::Display::DisplayImage
    //cvShowImage("LaserData", mDisplayLaser);
    //cvWaitKey(10);
    
    std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
            << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
            << std::endl;
    
    AI::Utility::HeadingDisplay(mProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Raw Image FW",mForwardFrame);
    Utility::Display::DisplayImage("Post Processing FW",mProcessedImage);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}

std::string Torpedo::GetStateName(State state)
{
    switch (state)
    {
        case Find:           ///< Figure out what to do
            return "Find";
        case Search:         ///< Search for the heart
            return "Search";
        
        case GetDepth:       ///< Find the depth of the heart
            return "GetDepth";
        case Advance:        ///< Go to the depth offset and advance
            return "Advance";
        case Fire:           ///< Shoot through the heart, the robot's to blame
            return "Fire";
        
        case PathSearch:     ///< Look for path or heart
            return "PathSearch";
        case DoLineup:       ///< We saw the heart but not the path, just try to align yaw
            return "DoLineup";
        case DoPath:         ///< Do the path stuff
            return "DoPath";
        case ExitPath:       ///< Leave path towards heart
            return "ExitPath";
        
        case Exit:            ///< Exit from the mission
            return "Exit";
        default:
            return "UNKNOWN STATE";
    }
    
}
