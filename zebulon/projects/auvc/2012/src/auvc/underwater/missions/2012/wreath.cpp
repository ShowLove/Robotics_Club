#include "auvc/underwater/missions/2012/wreath.h"

using namespace Zebulon;
using namespace Underwater;

Wreath::Wreath()
{
    ID = "Wreath";

    mFWFrame = NULL;
    mDWFrame = NULL;
    mFWProcFrame = NULL;
    mDWProcFrame = NULL;
}

Wreath::~Wreath()
{
    cvReleaseImage(&mFWFrame);
    cvReleaseImage(&mDWFrame);
    cvReleaseImage(&mFWProcFrame);
    cvReleaseImage(&mDWProcFrame);
}

int Wreath::ErrorCheck()
{
    return 0;
}

int Wreath::Init()
{
    // Use as default yaw if no path is found / when searching for the path
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mBackUpYaw))
    {
        std::cout << "Cannot grab yaw information" << std::endl;
        //mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }

    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor, mBackUpDepth))
    {
        std::cout << "Cannot grab depth information" << std::endl;
        //mGlobalInfo->GetInfo(AI::Names::YawDegrees, mStartYaw);
    }

    mEndobject.Initialize("PathHack");
    mDebounceWreath.Initialize(7,true);
    // Initial state
    mState = BackUp;
    //mState = BlindDrive;

    double tmp = 0;

    mXmlConf.GetVar("Underwater.Wreath.Yaw@north", mPingerYaw);
    mXmlConf.GetVar("Underwater.Wreath.Depths@pinger", mPingerDepth);

    mXmlConf.GetVar("Underwater.Wreath.BackUp@timer", tmp);
    mBackUpTimer.Initialize(tmp);
    mXmlConf.GetVar("Underwater.Wreath.BackUp@thrust", mBackUpThrust);

    mXmlConf.GetVar("Underwater.Wreath.BlindDrive@timer", tmp);
    if(mGlobalInfo->GetInfo(GlobalInfo::PingerDriveTime, mPingerDriveTime))
    {
        mBlindDriveTimer.Initialize(mPingerDriveTime);
    }
    else
    {
        mBlindDriveTimer.Initialize(tmp);
    }
    mXmlConf.GetVar("Underwater.Wreath.BlindDrive@thrust", mBlindDriveThrust);



    mXmlConf.GetVar("Underwater.Wreath.Surface@timer", tmp);
    mSurfaceTimer.Initialize(tmp);



    return 0;
}

int Wreath::FetchProcess()
{

//    mXmlConf.GetVar("Underwater.Training.Default@depth", mDesiredDepth);
    mXmlConf.GetVar("Underwater.Wreath.Default@pitch", mDesiredPitch);
    mXmlConf.GetVar("Underwater.Wreath.Default@roll", mDesiredRoll);

    mXmlConf.GetVar("Underwater.Wreath.VisionType@Gary", mVTGary);
    mXmlConf.GetVar("Underwater.Wreath.VisionType@David", mVTDavid);
    mXmlConf.GetVar("Underwater.Wreath.VisionType@New", mVTNew);
    //InitPaths(mVTGary,mVTDavid,mVTNew);




    double num, denom;
    mXmlConf.GetVar("Underwater.Wreath.LatPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Wreath.LatPixelScale@denom", denom);
    mLatPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Wreath.AxialPixelScale@num", num);
    mXmlConf.GetVar("Underwater.Wreath.AxialPixelScale@denom", denom);
    mAxialPixelScale = num/denom;

    mXmlConf.GetVar("Underwater.Wreath.Targets@I_thresh", mIThresh);
    mXmlConf.GetVar("Underwater.Wreath.Targets@J_thresh", mJThresh);



    // Get Current Compass Data
    if(!mGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentYaw))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::PitchDegrees, mCurrentPitch))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    if(!mGlobalInfo->GetInfo(AI::Names::RollDegrees, mCurrentRoll))
    {
        printf("ERROR::StartGate: Could not get Compass data");
        return 1;
    }
    // Get Current Depth Data
    if(!mGlobalInfo->GetInfo(GlobalInfo::DepthSensor,mCurrentDepth))
    {
        printf("ERROR::StartGate: Could not get Depth data");
        return 1;
    }



    // Forward Camera
    if(!mGlobalInfo->GetInfo(GlobalInfo::ForwardCamera, mFWFrame))
    {
        printf("ERROR::Training: Couldn't get forward camera data");
        return 1;
    }

    if(mFWProcFrame == NULL)
    {
        mFWProcFrame = cvCreateImage(cvSize(mFWFrame->width,mFWFrame->height), mFWFrame->depth, mFWFrame->nChannels);
    }
    cvCopy(mFWFrame,mFWProcFrame);
    /*
    for (int i=0; i<3; i++)
    {
        mDebounce[i].Bounce(mBuoyObject[i].GetVisionReturn(mFWFrame, mReturn[i], mFWProcFrame));
    }*/
    /*if (mState == LookForWreath)
    {
        mDebounce[mBuoyToAttack[mAttackingBuoy]].Bounce(mBuoyObject[mBuoyToAttack[mAttackingBuoy]].GetVisionReturn(mFWFrame, mReturn[mBuoyToAttack[mAttackingBuoy]], mFWProcFrame));
    }*/

    // Downward
    if(!mGlobalInfo->GetInfo(GlobalInfo::DownwardCamera, mDWFrame))
    {
        printf("ERROR::Training: Couldn't get downward camera 2 data");
        return 1;
    }

    if(mDWProcFrame == NULL)
    {
        mDWProcFrame = cvCreateImage(cvSize(mDWFrame->width,mDWFrame->height), mDWFrame->depth, mDWFrame->nChannels);
    }
    cvCopy(mDWFrame, mDWProcFrame);

    mObjectReturn = mEndobject.GetVisionReturn(mDWFrame,mDWProcFrame);
    mDebounceWreath.Bounce(mObjectReturn.size() > 0);

    //////////////////////////////////////
    // Add grab fix yaw here
    //////////////////////////////////////

    return 0;
}

std::string Wreath::ExitEarly()
{
    /*if (mStateTimeout.IsFinished())
    {
        mGlobalInfo->SetInfo(GlobalInfo::ExitEarlyToFinalMission, 1);
        return NextMission;
    }
    else*/
    {
        return "KeepRunning";
    }
}

std::string Wreath::Execute()
{
    static boost::timer _timestamp;
    std::cout << "===========================================================" << std::endl;
    std::cout << "State: " << ID << "::" << GetStateName(mState) << "\tTime: " << _timestamp.elapsed() << std::endl;
    std::cout << std::endl;

    mDesiredPitch=0;
    mDesiredRoll=0;

    switch (mState)
    {
        // Go to Path Depth, leave immediatley if path is found
        case BackUp:

            mDesiredYaw = mBackUpYaw;
            mDesiredDepth = mBackUpDepth-5;
            mDesiredAxialThrust = mBackUpThrust;
            mDesiredLateralThrust = 0;

            if(mBackUpTimer.IsFinished())
                // Add a check if the path is seen to go directly to path found ... || )
            {
                mState = TurnNorth;
            }

            break;

        //Search for a path, if found center, if not timeout and look at buoys
        case TurnNorth:

            mDesiredYaw = mPingerYaw;
            mDesiredDepth = mPingerDepth;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            if(fabs(Zebulon::AI::Utility::AngleDiff(mCurrentYaw, mPingerYaw)) <= mYawThresh)
            {
                mBlindDriveTimer.Start();
                mState = BlindDrive;
            }


            //std::cout << "Number of Paths: " << paths.size() << std::endl;

            /*std::cout << "Timer:"
                      << "\t" << mPathSearchTimer.GetRunTime()
                      << "\t" << mPathSearchTimer.TimeElapsed()
                      << "\t" << mPathSearchTimer.GetRunTime()-mPathSearchTimer.TimeElapsed()
                      << std::endl;*/

            break;

        //center in on a path
        case BlindDrive:

            mDesiredYaw = mPingerYaw;
            mDesiredDepth = mPingerDepth;
            mDesiredAxialThrust = mBlindDriveThrust;
            mDesiredLateralThrust = 0;

            if(mBlindDriveTimer.IsFinished() ||
              (mDebounceWreath.GetDebounced() &&
               mBlindDriveTimer.TimeElapsed() > mBlindDriveTimer.GetRunTime()/2))
            {
                mSurfaceTimer.Start();
                mState = Surface;
            }

            std::cout << "Timer:"
                      << "\t" << mBlindDriveTimer.GetRunTime()
                      << "\t" << mBlindDriveTimer.TimeElapsed()
                      << "\t" << mBlindDriveTimer.GetRunTime()-mBlindDriveTimer.TimeElapsed()
                      << std::endl;

            break;

        /*case LookForWreath:

            if(mDebounceWreath.GetDebounced())
            {

            }



            break;*/

        case Surface:

            mDesiredYaw = mPingerYaw;
            mDesiredDepth = 0;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            if(mSurfaceTimer.IsFinished())
            {
                mState = EndSurface;
            }

            break;

        //go to a good depth for buoys
        case EndSurface:

            mDesiredYaw = mPingerYaw;
            mDesiredDepth = 0;
            mDesiredAxialThrust = 0;
            mDesiredLateralThrust = 0;

            mGlobalCommand->Surface();

            break;

        //start searching for one color buoy


        default:

            std::cout << "ERROR::" << ID << " state " << mState << " does not exist" << std::endl;

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
    fflush(stdout);



    //////////////////////////////////////////////////////
    // DO NOT USE THIS THIS WILL CRASH THE PROGRAM
    // WHEN IT IS IN AUTONOMOUS MODE
    //////
    //cvShowImage("LaserData", mDisplayLaser);
    //cvWaitKey(10);
    ///////////////////////////////////////////////////////

    ///////////////////////////////////////////////////////
    // USE THIS
    ///////////////////////////////////////////////////////

    if (mState == LookForWreath)
    {
        AI::Utility::HeadingDisplay(mDWProcFrame, mCurrentYaw, mDesiredYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mDWProcFrame, mCurrentDepth, mDesiredDepth, 0, 192);
        AI::Utility::ThrustDisplay(mDWProcFrame, mDesiredAxialThrust, mDesiredLateralThrust);
    }
    else
    {
        AI::Utility::HeadingDisplay(mFWProcFrame, mCurrentYaw, mDesiredYaw, 0, 255, 255);
        AI::Utility::DepthDisplay(mFWProcFrame, mCurrentDepth, mDesiredDepth, 0, 192);
        AI::Utility::ThrustDisplay(mFWProcFrame, mDesiredAxialThrust, mDesiredLateralThrust);
    }

    Utility::Display::DisplayImage("Post Processing FW",mFWProcFrame);
    Utility::Display::DisplayImage("Post Processing DW",mDWProcFrame);

    mGlobalCommand->SetDesiredPitch(mDesiredPitch);
    mGlobalCommand->SetDesiredRoll(mDesiredRoll);
    mGlobalCommand->SetDesiredYaw(mDesiredYaw);

    mGlobalCommand->SetDesiredAxialVel(mDesiredAxialThrust);
    mGlobalCommand->SetDesiredLateralVel(mDesiredLateralThrust);
    mGlobalCommand->SetDesiredDepth(mDesiredDepth);

    return "KeepRunning";
}


std::string Wreath::GetStateName(State state)
{
    switch (state)
    {
        case BackUp:
            return "BackUp";

        case TurnNorth:
            return "TurnNorth";

        case PointAtPinger:
            return "PointAtPinger";

        case BlindDrive:
            return "BlindDrive";

        case LookForWreath:
            return "LookForWreath";

        case Surface:
            return "Surface";

        case EndSurface:
            return "EndSurface";

        default:
            return "ERROR::DNE";
    }
}
