#include "auvc/underwater/dropper.h"

using namespace Zebulon;
using namespace Underwater;

const std::string Dropper::NextMission = Underwater::FinalMission;

const std::string Dropper::BigX = "BigX";
const std::string Dropper::SmallX = "SmallX";
const std::string Dropper::BigO = "BigO";
const std::string Dropper::SmallO = "SmallO";

Dropper::Dropper()
{
    ID = "Dropper";
    
    mForwardFrame = NULL;                    ///< The forward facing camera's frame
    mDownwardFrame = NULL;                   ///< The downward facing camera's frame
    mFWProcessedImage = NULL;                ///< The final frame
    mDWProcessedImage = NULL;                ///< The final downward frame
}

Dropper::~Dropper()
{
    cvReleaseImage(&mForwardFrame);
    cvReleaseImage(&mDownwardFrame);
    cvReleaseImage(&mFWProcessedImage);
    cvReleaseImage(&mDWProcessedImage);
    
    mTargets.clear();
}

int Dropper::Init()
{
    double temp;
    
    mXmlConf.GetVar("Underwater.Dropper.Default@timeout", temp, false);
    mStateTimeout.Initialize(temp);
    mStateTimeout.Start();
    
    mXmlConf.GetVar("Underwater.Dropper.Default@path_timeout", temp, false);
    mPathFinder.Initialize();
    mPathSearchTimeout.Initialize(temp);
    mPathSearchTimeout.Start();
    
    mXmlConf.GetVar("Underwater.Dropper.Drop@debounce", temp, false);
    mCenteredDebounce.Initialize((int)temp,false);
    
    // Set the starting yaw to the current value as a default
    mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    
    Zebulon::Vision::VisualTarget* vt1 = new Zebulon::Vision::VisualTarget(BigX);
    vt1->Load("settings/vision/x.xml");
    mTargets.push_back(vt1);
    
    Zebulon::Vision::VisualTarget* vt2 = new Zebulon::Vision::VisualTarget(BigO);
    vt2->Load("settings/vision/o.xml");
    mTargets.push_back(vt2);
    
    // These two are for the small ones, may want to build separate templates in the future?
    
    Zebulon::Vision::VisualTarget* vt3 = new Zebulon::Vision::VisualTarget(SmallX);
    vt3->Load("settings/vision/x.xml");
    mTargets.push_back(vt3);
    
    Zebulon::Vision::VisualTarget* vt4 = new Zebulon::Vision::VisualTarget(SmallO);
    vt4->Load("settings/vision/o.xml");
    mTargets.push_back(vt4);
    
    mState = PathSearch;
}

int Dropper::ErrorCheck()
{
    return 0;
}

int Dropper::FetchProcess()
{
    mXmlConf.GetVar("Underwater.Dropper.Default@depth", mDefaultDepth);
    mXmlConf.GetVar("Underwater.Dropper.Default@pitch", mDesired.mPitch);
    mXmlConf.GetVar("Underwater.Dropper.Default@roll", mDesired.mRoll);
    
    double num, denom;
    mXmlConf.GetVar("Underwater.Dropper.LatPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Dropper.LatPixelScale@denom", denom);
    mLatPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Dropper.DepthPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Dropper.DepthPixelScale@denom", denom);
    mDepthPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Dropper.YawPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Dropper.YawPixelScale@denom", denom);
    mYawPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Dropper.AxialPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Dropper.AxialPixelScale@denom", denom);
    mAxialPixelScale = num/denom;
    
    mXmlConf.GetVar("Underwater.Dropper.Approach@speed", mApproachSpeed);
    mXmlConf.GetVar("Underwater.Dropper.Approach@time", mApproachTime);
    
    mXmlConf.GetVar("Underwater.Dropper.Search@strafe_time", mSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Dropper.Search@strafe_speed", mSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Dropper.Search@forward_time", mSearchForwardTime);
    mXmlConf.GetVar("Underwater.Dropper.Search@forward_speed", mSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Dropper.Search@smooth_speed", mSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.Dropper.PathSearch@strafe_time", mPathSearchStrafeTime);
    mXmlConf.GetVar("Underwater.Dropper.PathSearch@strafe_speed", mPathSearchStrafeSpeed);
    mXmlConf.GetVar("Underwater.Dropper.PathSearch@forward_time", mPathSearchForwardTime);
    mXmlConf.GetVar("Underwater.Dropper.PathSearch@forward_speed", mPathSearchForwardSpeed);
    mXmlConf.GetVar("Underwater.Dropper.PathSearch@smooth_speed", mPathSearchSmoothSpeed);
    
    mXmlConf.GetVar("Underwater.Dropper.Path@path_exit_time", mPathExitTime);
    mXmlConf.GetVar("Underwater.Dropper.Path@exit_path_thrust", mExitPathThrust);
    mXmlConf.GetVar("Underwater.Dropper.Path@path_search_duration", mPathSearchDuration);
    
    std::string target;
    mXmlConf.GetVar("Underwater.Dropper.Targets@first", target);
    mFirstTarget = Vision::DVision::ConvertID(target);
    mXmlConf.GetVar("Underwater.Dropper.Targets@second", target);
    mFirstTarget = Vision::DVision::ConvertID(target);
    
    // Compass
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrent.mYaw))
    {
        printf("ERROR::Dropper: Couldn't get Compass data");
        return 1;
    }
    
    // Depth
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mCurrent.mDepth))
    {
        printf("ERROR::Dropper: Couldn't get depth data");
        return 1;
    }
    
    // Downward camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDownwardFrame))
    {
        printf("ERROR::Dropper: Couldn't get downward camera data");
        return 1;
    }
    
    // Downward processed frame
    if(mDWProcessedImage == NULL)
    {
        mDWProcessedImage = cvCreateImage(cvSize(mDownwardFrame->width,mDownwardFrame->height), mDownwardFrame->depth, mDownwardFrame->nChannels);
    }
    Vision::VisionUtil::ResetImage(mDWProcessedImage);
    
    return 0;
}

std::string Dropper::ExitEarly()
{
    if (mStateTimeout.IsFinished())
    {
        return Underwater::FinalMission;
    }
    else
    {
        return "KeepRunning";
    }
}

std::string Dropper::Execute()
{
    std::cout << "====================================================" << std::endl
            << ID << "::" << GetStateName(mState) << std::endl;
    
    int done = 0;
    Vision::VisionReturn desiredTarget;
    std::vector<Path> paths;
    switch (mState)
    {
        // Entry state: figure out what to do
        case Find:
            
            break;
        
        // Search for the targets
        case Search:
            
            mSearchPattern.SearchStep(mDesired.mLateralThrust, mDesired.mAxialThrust);
            mDesired.mDepth = mDefaultDepth;
            mDesired.mYaw = mStartYaw;
            
            if (mDVision.BombingObject(mTargets, mDownwardFrame, mDWProcessedImage).size() > 0)
            {
                mState = FindActive;
            }
            
            break;
        
        // Go near a cluster of targets
        case Approach:
            
            mDesired.mDepth = mDefaultDepth;
            mDesired.mAxialThrust = mApproachSpeed;
            mDesired.mLateralThrust = 0;
            mDesired.mYaw = mStartYaw;
            
            if (mDVision.BombingObject(mTargets, mDownwardFrame, mDWProcessedImage).size() > 0)
            {
                mState = FindActive;
            }
            else if (mApproachTimer.IsFinished())
            {
                mState = Search;
            }
            
            break;
        
        // Find the target we're looking for
        case FindActive:
            mVisionReturn = mDVision.BombingObject(mTargets, mDownwardFrame, mDWProcessedImage);
            
            if (mLostTargetDebounce.Bounce(!mVisionReturn.size()))
            {
                mState = Search;
            }
            else for(std::list<Vision::VisionReturn>::iterator target=mVisionReturn.begin(); target != mVisionReturn.end(); target++)
            {
                if (target->Valid == mFirstTarget && !mFirstTargetAttempted)
                {
                    desiredTarget = *target;
                    done = 3;
                    break;
                }
                else if (target->Valid == mSecondTarget)
                {
                    desiredTarget = *target;
                    done = 2;
                }
                else if (target->Valid && done <= 1)
                {
                    desiredTarget = *target;
                    done = 1;
                }
            }
            
            if (done)
            {
                mDesired.mAxialThrustEnable = true;
                mDesired.mAxialThrust = (mDownwardFrame->height/2 - desiredTarget.CenterJ)*mAxialPixelScale;
                
                mDesired.mLateralThrustEnable = true;
                mDesired.mLateralThrust = (desiredTarget.CenterI - mDownwardFrame->width/2)*mLatPixelScale;
                
                if (mCenteredDebounce.Bounce( fabs(mDownwardFrame->width/2 - desiredTarget.CenterI) < mCenterIThresh &&
                                              fabs(mDownwardFrame->height/2 - desiredTarget.CenterJ) < mCenterJThresh ))
                {
                    std::cout << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl
                            << "FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE FIRE" << std::endl;
                    mGlobalCommand->SetDropper1(true);
                    mGlobalCommand->SetDropper2(true);
                    if (desiredTarget.Valid == mFirstTarget && !mFirstTargetAttempted)
                    {
                        mFirstTargetAttempted == true;
                    }
                }
            }
            
            break;
        
        // Attack the target!
        case Attack:
            
            break;
        
        // Look for path
        case PathSearch:
             if (mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage).size() > 0)
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
            break;
        
        // Do the path stuff
        case DoPath:
            paths = mPathFinder.GetPaths(mCurrent, mDownwardFrame, mDWProcessedImage);
            
            if (paths.size() > 0)
            {
                if (mCenteredDebounce.Bounce(mPathFinder.StepPath(&(paths.front()), mCurrent, mDesired)))
                {
                    mStartYaw = mCurrent.mYaw;
                    mApproachTimer.Initialize(mApproachTime);
                    mApproachTimer.Start();
                    mState = Approach;
                }
                mLostTargetDebounce.Miss();
            }
            else if (mLostTargetDebounce.Hit())
            {
                mState = Search;
            }
            
            break;
        
        // Leave path towards targets
        case ExitPath:
            
            break;
        
        // Go onto the next state
        case Exit:
            Utility::Display::CloseAllWindows();
            return NextMission;
            break;
        
        // Error!
        default:
            std::cout << "ERROR: STATE DOES NOT EXIST" << std::endl;
            break;
    }
    
    std::cout << " Yaw: " << mCurrent.mYaw << " -> " << mDesired.mYaw
            << " Depth: " << mCurrent.mDepth << " -> " << mDesired.mDepth
            << std::endl;
    
    AI::Utility::HeadingDisplay(mDWProcessedImage, mCurrent.mYaw, mDesired.mYaw, 0, 255, 255);
    AI::Utility::DepthDisplay(mDWProcessedImage, mCurrent.mDepth, mDesired.mDepth, 0, 192);
    AI::Utility::ThrustDisplay(mDWProcessedImage, mDesired.mAxialThrust, mDesired.mLateralThrust);
    Utility::Display::DisplayImage("Raw Image DW",mDownwardFrame);
    Utility::Display::DisplayImage("Post Processing DW",mDWProcessedImage);
    
    mGlobalCommand->SetDesiredPitch(mDesired.mPitch);
    mGlobalCommand->SetDesiredRoll(mDesired.mRoll);
    mGlobalCommand->SetDesiredYaw(mDesired.mYaw);
    
    mGlobalCommand->SetDesiredAxialVel(mDesired.mAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesired.mLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesired.mDepth);
    
    return "KeepRunning";
}

std::string Dropper::GetStateName(State state)
{
    switch (state)
    {
        case Find:
            return "Find";
        case Search:
            return "Search";
        case Approach:
            return "Approach";
        case FindActive:
            return "FindActive";
        case Attack:
            return "Attack";
        case PathSearch:
            return "PathSearch";
        case DoPath:
            return "DoPath";
        case ExitPath:
            return "ExitPath";
        case Exit:
            return "Exit";
        default:
            return "UnknownState";
    }
}

/* End of file */
