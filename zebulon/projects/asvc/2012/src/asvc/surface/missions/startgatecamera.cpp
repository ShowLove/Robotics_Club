/////////////////////////////////////////////////////////////////////////////////////
///
/// \file startgatecamera.h
/// \brief Mission code to navigate through start gates.
///
/// <br>Author(s): Gary Stein, Jonathan Mohlenhoff, Daniel Barber
/// <br>Created: 05/31/2011<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: jmohlenh@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/startgatecamera.h"
#include "asvc/surface/missions/buoynavcamera.h"
#include "asvc/surface/buoy.h"

//#define SAVEIMAGES
using namespace Zebulon;
using namespace ASVC;

const std::string StartGateCamera::Name = "StartGateCamera";
const std::string StartGateCamera::ReturnName = "ReturnStartGateCamera";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
StartGateCamera::StartGateCamera(const bool returnNavFlag)
{
    mReturnFlag = returnNavFlag;

    std::string name = Name;
//    if(mReturnFlag)
//    {
//        name = ReturnName;
//    }
    ID = name;

//    if(mReturnFlag)
//    {
//        mNextMission = BuoyNavCamera::ReturnName;
//    }
//    else
//    {
//        mNextMission = BuoyNavCamera::Name;
//    }

    mWaypointCount = 0;

    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();

    mPreDriveTimeSeconds = 5.0;

    mSearchFlag = true;
    mStartGate = true;

    mInitFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mFinalFrame = NULL;

    mFromVisionState = false;

    mDistanceFromStart = 0;
    mTempDistCalc = 0;

    mRedDebounce.Initialize(5, true);
    mGreenDebounce.Initialize(5, true);
    mBothDebounce.Initialize(5, true);

    mGatesCounted = 0;
    mTrailName = "BuoyChannel";


}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor, clears memory.
///
////////////////////////////////////////////////////////////////////////////////////
StartGateCamera::~StartGateCamera()
{
    if(mInitFrame)
    {
        cvReleaseImage(&mInitFrame);
    }
    if(mSegFrame)
    {
        cvReleaseImage(&mSegFrame);
    }
    if(mThreshFrame)
    {
        cvReleaseImage(&mThreshFrame);
    }
    if(mFinalFrame)
    {
        cvReleaseImage(&mFinalFrame);
    }

    delete mSegment;
    delete mThreshold;

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int StartGateCamera::Init()
{
    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }
    std::string baseName = std::string("Surface.") + name + ".";
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(1);
        CxUtils::SleepMs(100);
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(0);
    }
    mState = PreStart;
    LoadSettings();
    InitializeMaps(Mission::mpGlobalInfo, mXmlConf, name);
    mStopWatch.Initialize(4);
    mpGlobalInfo->AddToSimpleTrail(mTrailName);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int StartGateCamera::ErrorCheck()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
///////////////////////////////////////////////////////////////////////////////////
int StartGateCamera::FetchProcess()
{
    LoadSettings();

    MapBuilder::UpdateMaps(true);

    mCurrentPosition = Mission::mpGlobalInfo->GetPosition();
    Mission::mpGlobalInfo->GetInfo(AI::Names::YawDegrees, mCurrentHeading);

    if(!Mission::mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mInitFrame))
    {
        std::cout << "No Camera" << std::endl;
        return 1;
    }

    return 0;
}

double StartGateCamera::AngleToGate(const Zebulon::Vision::Return& buoy)
{
    CxUtils::Point3D bottomcenter;
    CxUtils::Point3D buoycenter;
    bottomcenter.mX = mInitFrame->width/2.0;
    bottomcenter.mY = mInitFrame->height;

    buoycenter.mX = buoy.mCenterI;
    buoycenter.mY = buoy.mCenterJ;

    CxUtils::Segment3D pathtobuoy(bottomcenter,buoycenter);
    double angle = pathtobuoy.GetAngleXY(false);
    angle = CxUtils::Orientation::AddToAngle(angle,CxUtils::CX_HALF_PI);
    std::cout << "angle to buoy: " <<  CxUtils::CxToDegrees(angle ) << std::endl;
    return angle;

}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string StartGateCamera::ExitEarly()
{
    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Executes the mission data.
///
///  \return Mission::KeepRunning while running, next mission name if
///          completed.
///
////////////////////////////////////////////////////////////////////////////////////
std::string StartGateCamera::Execute()
{
    //RunVisionClassifier();
    LoadSettings();
    RunDvisionClassifier();
    double farbuoyoffsetangle = CxUtils::CxToRadians(15);
    double angle = 0;
    GlobalInfo::SimpleTrail trail;
    Zebulon::Vision::Return centerheading;
    mDistanceFromStart = AI::Utility::CalcDistance(mCurrentPosition.mEasting,
                                                   mCurrentPosition.mNorthing,
                                                   mStartPosition.mEasting,
                                                   mStartPosition.mNorthing);

    mForwardThrust = mWaypointSpeed;

    if(mState!=PreStart && mState !=PreDrive && mState != Start && mState != CheckGate)
    {
        if(mBothDebounce.GetDebounced())
        {
            if(mRed.mCenterI>mGreen.mCenterI)
            {
                mState=FoundGreenRed;
            }
            //red on correct side of green
            else
            {
                mLockHeading = mCurrentHeading;
                mState = FoundBoth;
            }
        }else if(mRedDebounce.GetDebounced())
        {
            mLockHeading = mCurrentHeading;
            mState = FoundRed;
        }else if(mGreenDebounce.GetDebounced())
        {
            mLockHeading = mCurrentHeading;
            mState = FoundGreen;
        }else if(!mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mFromVisionState = true;
            mState = DriveToWaypoint;
        }

    }
    std::cout << "================================================" << std::endl;
    std::cout << GetName() << std::endl;
    std::cout << "State: " << ToString((State)mState) << std::endl;
    std::cout << "Gates Passed: " << mGatesCounted << std::endl;
    switch(mState)
    {
    case PreStart:
        mStartPosition = mCurrentPosition;

        mStartHeading = mCurrentHeading;
        Mission::mpGlobalInfo->SetInfo(GlobalInfo::StartPosition, mStartPosition);

        mStopWatch.Start();
        mState = PreDrive;
        break;
    case PreDrive:

        if(mStopWatch.IsFinished())
        {
            mForwardThrust = 0;
            mState=Start;
        }
        else
        {
            // Set forward (no turning or heading)
            mForwardThrust = mWaypointSpeed;
        }

        //mpGlobalCommand->SetForwardThrust(mForwardThrust);
        //mpGlobalCommand->SetTurningThrust(0.0);

        mpGlobalCommand->SetYawDegrees(mStartHeading);
        mpGlobalCommand->SetForwardVelocityPercent(mForwardThrust);

        std::cout << "Thrust: " << mForwardThrust << std::endl;
        std::cout << "Time: " << mStopWatch.TimeElapsed() << std::endl;
        return Mission::KeepRunning;
        break;
        // Later projection
    case Start:
        // Push this point to our trail of visited points.
        Mission::mpGlobalInfo->PushCurrentPositionToTrail();
        // Grab initial information
        //mpGlobalInfo->AddToSimpleTrail(mTrailName);
        // Calculate waypoints
        mWaypoints.clear();

        for(int x = 1; x < mNumWaypoints + 1;x++)
        {
            CxUtils::Utm projection;
            AI::Utility::CalcProject(mStartPosition.mEasting,
                                     mStartPosition.mNorthing,
                                     mStartHeading,mWaypointDistance*x,
                                     projection.mEasting, projection.mNorthing);
            mWaypoints.push_back(projection);
        }

        mState = DriveToWaypoint;

        mForwardThrust = mWaypointSpeed;
        mDesiredHeading = mStartHeading;

        break;

    case DriveToWaypoint:
        // Check if coming out of using vision
        // adjust waypoint to go to

        mDistanceFromStart = AI::Utility::CalcDistance(mCurrentPosition.mEasting, mCurrentPosition.mNorthing, mStartPosition.mEasting, mStartPosition.mNorthing);

        if(mpGlobalInfo->GetSimpleTrail(mTrailName, trail))
        {
            //if(CxUtils::Utm::Distance2D(mCurrentPosition,mGuessPose.mPosition) > 20)
            if(trail.GuessNextPose(mGuessPose))
            {
                mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::GetGlobalAngle(mCurrentPosition,
                                                        mGuessPose.mPosition));
            }
            else
            {

                for(int i = 0; i < mNumWaypoints; i++)
                {
                    mTempDistCalc = AI::Utility::CalcDistance(mWaypoints[i].mEasting, mWaypoints[i].mNorthing,
                                                              mStartPosition.mEasting, mStartPosition.mNorthing);

                    if((mDistanceFromStart + mDistOffset) < mTempDistCalc)
                    {
                        mWaypointCount = i;
                        break;
                    }
                }
                mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::GetGlobalAngle(mCurrentPosition,
                                                                                            mWaypoints[mWaypointCount]));
            }
        }

        // Check if any buoys are found
//        if(mBothDebounce.GetDebounced())
//        {
//            mLockHeading = mCurrentHeading;
//            mState = FoundBoth;
//        }
//        else if(mRedDebounce.GetDebounced())
//        {
//            mLockHeading = mCurrentHeading;
//            mState = FoundRed;
//        }
//        else if(mGreenDebounce.GetDebounced())
//        {
//            mLockHeading = mCurrentHeading;
//            mState = FoundGreen;
//        }
//        else
//        {
            if(mDistanceFromStart >= mSecondGateDistance)
            {
                mStopWatch.Start();
                mLockHeading = mStartHeading;
                mDesiredHeading = mStartHeading;
                mState = CheckGate;
            }

            mForwardThrust = mWaypointSpeed;

//        }


        std::cout << "Distance to current: " << mTempDistCalc
                  << " Waypoints Reached: " << mReachedWaypoint
                  << " Waypoint Count :" << mWaypointCount
                  << " Waypont Thresh " << mWaypointThresh
                  <<  std::endl;

        break;
    case FoundGreenRed:
        //this state is needed because qualifiers has 2 sets of gates, for left and right course in view
        //bouys on wrong sides, choose shallowest angle and head there.
        //we assume maybe the closest one to our current orientation is correct
        if (mGreenDebounce.GetDebounced() && mRedDebounce.GetDebounced())
        {
            double angletogreen = AngleToGate(mGreen);
            double angletored = AngleToGate(mRed);
            std::cout << "angle to green " << CxUtils::CxToDegrees(angletogreen);
            std::cout << " angle to red " << CxUtils::CxToDegrees(angletored);

            angletogreen = CxUtils::Orientation::AddToAngle(-farbuoyoffsetangle, angletogreen);
            angletored = CxUtils::Orientation::AddToAngle(farbuoyoffsetangle, angletored);
            std::cout << std::endl;
            std::cout << "new angle to green " << CxUtils::CxToDegrees(angletogreen);
            std::cout << " new angle to red " << CxUtils::CxToDegrees(angletored);
            std::cout << std::endl;

            double preferredangle = 0;

            preferredangle = fabs(angletogreen) < fabs(angletored) ? angletogreen:angletored;

            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(mCurrentHeading),preferredangle));
            AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,255,255);
            mState = DriveToWaypoint;
            //AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(mCurrentHeading),angletogreen)),0,255,0);
            //AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(mCurrentHeading),angletored)),255,0,0);
        }
        break;
    case FoundBoth:
        // Both gates were found

//        if(!mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mFromVisionState = true;
//            mState = DriveToWaypoint;
//        }
//        else if(!mRedDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundGreen;
//        }
//        else if(!mGreenDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundRed;
//        }
//        // If they are both still valid
//        else
//        {
            // get the point on the screen that is in the middle of the two centers
            mCenterI = (mGreen.mCenterI + mRed.mCenterI)/2.0;
            mCenterJ = (mGreen.mCenterJ + mRed.mCenterJ)/2.0;

            mForwardThrust = mBothBuoySpeed;
            cvDrawLine(mFinalFrame,cvPoint(0,mCenterJ),cvPoint(mFinalFrame->width,mCenterJ),CV_RGB(255,0,255));
            // if the j center is above the max j assume we have passed the gates
            if (mCenterJ > mMaxJ || mGreen.mMaxJ > mMaxJ || mRed.mMaxJ > mMaxJ)
            {
                //mGatesCounted++;
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }
            centerheading.mCenterI = mCenterI;
            centerheading.mCenterJ = mCenterJ;

//            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
//                                                               ((mCenterI - 360)*mPixelScale), false);
            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(AngleToGate(centerheading),CxUtils::CxToRadians(mCurrentHeading)));


//        }

        //AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,255,255);

        std::cout << "CenterI :" << mCenterI
                  << " CenterJ :" << mCenterJ
                  << " Green area " << mGreen.mArea
                  << " Red area " << mRed.mArea
                  << std::endl;
        break;
    case FoundRed:
//        if(mGreenDebounce.GetDebounced() && !mRedDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundGreen;
//        }
//        else if(mBothDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundBoth;
//        }
//        else if(!mGreenDebounce.GetDebounced() && !mRedDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mFromVisionState = true;
//            mState = DriveToWaypoint;
//        }
//        else
//        {

            // Get the center of the red object
            mCenterI = mRed.mCenterI;
            mCenterJ = mRed.mCenterJ;

            mForwardThrust = mRedBuoySpeed;

            // If the center j is above some max assume we have passed the object
            if(mRed.mMaxJ > mMaxJ)
            {
                //mGatesCounted++;
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }

            angle = AngleToGate(mRed) ;

            if(angle < CxUtils::CxToRadians(5) && mCenterJ > mMaxJ)
            {
                angle = 0;
            }
            else
            {
                angle = CxUtils::Orientation::AddToAngle(angle, CxUtils::CxToRadians(10));
            }

            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(mCurrentHeading),angle));
//            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(mCurrentHeading,
//                                                    angle,
//                                                    false));
            //mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
            //                                                   ((mCenterI-mLeftI)*mPixelScale),
             //                                                  false);
            //AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,0,0);
//        }

        std:: cout << "LeftI :"  << mLeftI
                   << " CenterI :" << mCenterI
                   << " CenterJ :" << mCenterJ
                   << " Area " << mRed.mArea
                   << " Angle " << angle
                   << std::endl;
        break;

    case FoundGreen:

        // Slow down and maintain our heading until we need to turn
        // if we lost the green gate go back to look at camera data
//        if(mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundRed;
//        }
//        else if(mBothDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mState = FoundBoth;
//        }
//        else if(!mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
//        {
//            mDesiredHeading = mStartHeading;
//            mFromVisionState = true;
//            mState = DriveToWaypoint;
//        }
//        else
        //{
            // Get the center of the object
            mCenterI = mGreen.mCenterI;
            mCenterJ = mGreen.mCenterJ;

            mForwardThrust = mGreenBuoySpeed;

            // If the center j is above some max assume we have passed the object
            if(mGreen.mMaxJ > mMaxJ)
            {
                //mGatesCounted++;
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }

            angle = AngleToGate(mGreen);

            if(angle > CxUtils::CxToRadians(5) && mCenterJ > mMaxJ)
            {
                angle = 0;
            }
            else
            {
                angle = CxUtils::Orientation::AddToAngle(angle, CxUtils::CxToRadians(-10));
            }

            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(mCurrentHeading),angle));
//            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
//                                                              angle,
//                                                              false);
//            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
//                                                               ((mCenterI-mRightI)*mPixelScale),
//                                                               false);
            //AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,0,255,0);
        //}

        std:: cout << "LeftI :"  << mLeftI
                   << " CenterI :" << mCenterI
                   << " CenterJ :" << mCenterJ
                   << " Area " << mGreen.mArea
                   << " Angle " << angle
                   << std::endl;

        break;
    case CheckGate:

        // Transition out of SpeedGate
        if(mStopWatch.IsFinished())
        {

            mGatesCounted++;

            mpGlobalInfo->AddToSimpleTrail(mTrailName);
            //if(mSecondGateDistance > mDistanceFromStart)
           // {
                //mForwardThrust = 0;
            mDesiredHeading = mStartHeading;
            mState = DriveToWaypoint;
           // }
            if(mGatesCounted == 2)
            {
                // mForwardThrust = 0;
                mLockHeading = mStartHeading;
                std::cout << "Loading next mission\n";
                //Utility::Display::CloseAllWindows();

                mpGlobalInfo->AddToSimpleTrail(mTrailName);
                // Push this point to our trail of visited points.
                Mission::mpGlobalInfo->PushCurrentPositionToTrail();
                return mNextMission;
            }

        }

        mDesiredHeading = mLockHeading;
        mForwardThrust = mCheckGateSpeed;

        break;
    default:
        std::cout << "Entered into an unknown state\n";
        break;
    }

    std::cout
    << "Heading " << mCurrentHeading
    << " -> " << mDesiredHeading

    << " Diff " << CxUtils::Orientation::AngleDiff(mCurrentHeading, mDesiredHeading, false) << std::endl;
    std::cout << "Travel Speed " << mForwardThrust << std::endl;
    std::cout << "Distance Travelled: " << mDistanceFromStart << std::endl;

    cvDrawLine(mFinalFrame,cvPoint(0,mMaxJ),cvPoint(mFinalFrame->width,mMaxJ),CV_RGB(255,255,255));
    AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,255,255);
    mpGlobalCommand->SetYawDegrees(mDesiredHeading);
    mpGlobalCommand->SetForwardVelocityPercent(mForwardThrust);

    this->DrawMaps();
    //this->DisplayMaps(MapBuilder::GlobalMap);
    this->DisplayMaps(MapBuilder::ObjectMap);
    //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    Utility::Display::DisplayImage("Final Frame", mFinalFrame);
#ifdef SAVEIMAGES
    static int frameNumber = 0;
    std::stringstream fname;
    std::stringstream fname2;                    // Output filename for image
    fname << "logs/vision/cam/" << "startgatecam";
    fname << std::setfill('0') << std::setw(8) << frameNumber << ".png";
    cvSaveImage(fname.str().c_str(),mFinalFrame);

    fname2 << "logs/vision/map/" << "startgatemap";
    fname2 << std::setfill('0') << std::setw(8) << frameNumber << ".png";
    cvSaveImage(fname2.str().c_str(),mpNavigator->GetLocalMapImage(ObjectMap));
    frameNumber++;
#endif
    return Mission::KeepRunning;
}

void StartGateCamera::RunDvisionClassifier()
{


    if(mInitFrame == NULL) return;
   // cvFlip(mInitFrame,mInitFrame,1);
    if(mFinalFrame == NULL && mInitFrame != NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);

        //mFinalFrame = cvCloneImage(mInitFrame);
        //std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
    }
    cvSetImageROI(mInitFrame, cvRect(0,mInitFrame->height/3,mInitFrame->width,mInitFrame->height));
    IplImage * copyinit = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);
    cvCopyImage(mInitFrame, mFinalFrame);
    cvCopyImage(mInitFrame, copyinit);
    cvResetImageROI(mInitFrame);


    std::vector<Vision::Return> returnlist;
    double bestindex=-1;
    double bestangle=-1;
    double tempangle;
    Vision::Return temp;
    mRed.mValid = false;
    mGreen.mValid = false;
    mRedGateClassifier.GetVisionReturn(copyinit,returnlist,mFinalFrame);
    for(int i = 0; i < returnlist.size(); i++)
    {
        temp = returnlist[i];
        tempangle = AngleToGate(temp);
        if(bestindex == -1 || fabs(tempangle) < fabs(bestangle))
        {
            bestindex = i;
            bestangle = tempangle;
        }
    }
    if(bestindex != -1 && returnlist.size() > 0)  mRed = returnlist[bestindex];
    mGreenGateClassifier.GetVisionReturn(copyinit,returnlist,mFinalFrame);

    bestindex=-1;
    bestangle=-1;

    for(int i = 0; i < returnlist.size(); i++)
    {
        temp = returnlist[i];
        tempangle = AngleToGate(temp);
        if(bestindex == -1 || fabs(tempangle) < fabs(bestangle))
        {
            bestindex = i;
            bestangle = tempangle;
        }
    }
    if(bestindex != -1 && returnlist.size() > 0) mGreen = returnlist[bestindex];

    cvReleaseImage(&copyinit);

    mRedDebounce.Bounce(mRed.mValid == 1 && mRed.mArea >= mMinRedArea);
    mGreenDebounce.Bounce(mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea);
    mBothDebounce.Bounce((mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea) && (mRed.mValid == 1 && mRed.mArea >= mMinRedArea));

//    mYellowDebounce.Bounce(mYellow.mValid && mYellow.mArea > mMinPixelArea && mYellow.mArea < mMaxPixelArea);
//    mBlueDebounce.Bounce(mBlue.mValid && mBlue.mArea > mMinPixelArea && mBlue.mArea < mMaxPixelArea);

//    if(this->mFirstBuoyFoundFlag &&
//       (mRedDebounce.GetDebounced() || mGreenDebounce.GetDebounced()))
//    {
//        this->mFirstBuoyFoundFlag = false;
//        if(mReturnFlag == false) // Don't set values if this is the return trip version of mission.
//        {
//            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelStartPosition, Mission::mpGlobalInfo->GetPosition());
//            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelStartHeading, mMapPose.Yaw());
//        }
//    }

}

/** Runs vision color segmentation code and debounces results for
    finding red and green buoys making up the gates for travelling through. */
void StartGateCamera::RunVisionClassifier()
{
    if(mInitFrame == NULL) return;
   // cvFlip(mInitFrame,mInitFrame,1);
    if(mFinalFrame == NULL && mInitFrame != NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);

        //mFinalFrame = cvCloneImage(mInitFrame);
        //std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
    }
    cvSetImageROI(mInitFrame, cvRect(0,mInitFrame->height/3,mInitFrame->width,mInitFrame->height));
    IplImage * copyinit = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height*(2./3.)), IPL_DEPTH_8U, 3);
    cvCopyImage(mInitFrame, mFinalFrame);
    cvCopyImage(mInitFrame, copyinit);
    cvResetImageROI(mInitFrame);

    // If we haven't crated an image for the final processed image do so
//    if(mFinalFrame == NULL)
//    {
//        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), 8, 3);
//    }
    // set the entire image to black
    Vision::VisionUtil::ResetImage(mFinalFrame);
    // Green Gate
    // Setup Segment for green
    mSegment->Setup(mGreenBuoy, 50);
    // Setup threshold for green
    mThreshold->Setup(mGreenBuoy);
    // Segment/threshold forward for green
    mThreshold->Process(copyinit, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // Get green return values.
    mSegment->GetMaxPosition(mGreen);
    // Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);
    // Red Gate
    // Setup Segment for Red
    mSegment->Setup(mRedBuoy, 50);
    // Setup threshold for Red
    mThreshold->Setup(mRedBuoy);
    // Segment/threshold forward for Red
    mThreshold->Process(copyinit, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // Get red return values
    mSegment->GetMaxPosition(mRed);
    // Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    // Debouncing Info
    mRedDebounce.Bounce(mRed.mValid == 1 && mRed.mArea >= mMinRedArea);
    mGreenDebounce.Bounce(mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea);
    mBothDebounce.Bounce((mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea) && (mRed.mValid == 1 && mRed.mArea >= mMinRedArea));

    cvReleaseImage(&copyinit);

}


/** Loads XML settings from mission settings file. */
void StartGateCamera::LoadSettings()
{
    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }

    std::string baseName = std::string("Surface.") + name + ".";

    mRedGateClassifier.Initialize("RedStartGate");
    mGreenGateClassifier.Initialize("GreenStartGate");

    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);
    if(mReturnFlag)
    {
        mXmlConf.GetVar(baseName + "NextMission@return", mNextMission);
    }

    mXmlConf.GetVar(baseName + "PreDriveTimeSeconds@value", mPreDriveTimeSeconds);

    // Image Thresholds

    mXmlConf.GetVar(baseName + "BuoyRed@red", mRedBuoy.mRed);
    mXmlConf.GetVar(baseName + "BuoyRed@green", mRedBuoy.mGreen);
    mXmlConf.GetVar(baseName + "BuoyRed@blue", mRedBuoy.mBlue);
    mXmlConf.GetVar(baseName + "BuoyRedThresh@red", mRedBuoy.mRedRange);
    mXmlConf.GetVar(baseName + "BuoyRedThresh@green", mRedBuoy.mGreenRange);
    mXmlConf.GetVar(baseName + "BuoyRedThresh@blue", mRedBuoy.mBlueRange);

    mXmlConf.GetVar(baseName + "BuoyGreen@red", mGreenBuoy.mRed);
    mXmlConf.GetVar(baseName + "BuoyGreen@green", mGreenBuoy.mGreen);
    mXmlConf.GetVar(baseName + "BuoyGreen@blue", mGreenBuoy.mBlue);
    mXmlConf.GetVar(baseName + "BuoyGreenThresh@red", mGreenBuoy.mRedRange);
    mXmlConf.GetVar(baseName + "BuoyGreenThresh@green", mGreenBuoy.mGreenRange);
    mXmlConf.GetVar(baseName + "BuoyGreenThresh@blue", mGreenBuoy.mBlueRange);
    // Image scales
    mXmlConf.GetVar(baseName + "BuoyBuffers@lowest_row", mMaxJ);
    mXmlConf.GetVar(baseName + "BuoyBuffers@left_limit", mLeftI);
    mXmlConf.GetVar(baseName + "BuoyBuffers@right_limit", mRightI);

    mXmlConf.GetVar(baseName + "BuoySegmentArea@min", mMinGreenArea);
    mXmlConf.GetVar(baseName + "BuoySegmentArea@min", mMinRedArea);

    mXmlConf.GetVar(baseName + "PixelScale@numerator", mPixelScaleNum);
    mXmlConf.GetVar(baseName + "PixelScale@denominator", mPixelScaleDenom);

    // Waypoints
    mXmlConf.GetVar(baseName + "Waypoint@distance", mWaypointDistance);
    mXmlConf.GetVar(baseName + "Waypoint@thresh", mWaypointThresh);
    mXmlConf.GetVar(baseName + "Waypoint@number", mNumWaypoints);
    mXmlConf.GetVar(baseName + "Waypoint@dist_offset", mDistOffset);
    mXmlConf.GetVar(baseName + "Waypoint@second_gate_dist", mSecondGateDistance);

    // Speeds
    mXmlConf.GetVar(baseName + "Speed@check_gate_speed", mCheckGateSpeed);
    mXmlConf.GetVar(baseName + "Speed@green_buoy_speed", mGreenBuoySpeed);
    mXmlConf.GetVar(baseName + "Speed@red_buoy_speed", mRedBuoySpeed);
    mXmlConf.GetVar(baseName + "Speed@both_buoy_speed", mBothBuoySpeed);
    mXmlConf.GetVar(baseName + "Speed@waypoint_speed", mWaypointSpeed);

    mPixelScale = mPixelScaleNum / mPixelScaleDenom;
}

/** Draws maps for display purposes */
void StartGateCamera::DrawMaps()
{
    MapBuilder::DrawMaps();

    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    CxUtils::Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                                      mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);

    double vLength = 0, vWidth = 0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);

    // Draw waypoints.
    for(unsigned int i = 0; i < (unsigned int)mWaypoints.size(); i++)
    {
        Buoy marker;
        marker.SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose,
                                                                   this->mpGlobalMap->ConvertFromGlobalToMapCoordinates(mWaypoints[i])));
        if(i < mWaypointCount)
        {
            marker.SetBuoyColor(Buoy::Unknown);
            marker.SetRadius(this->mWaypointThresh/2.0);
        }
        else
        {
            marker.SetBuoyColor(Buoy::Yellow);
            marker.SetRadius(this->mWaypointThresh);
        }
        this->DrawLocalObject(true, &marker, marker.GetDrawColor());
    }
    GlobalInfo::SimpleTrail trail;
    if(mpGlobalInfo->GetSimpleTrail(mTrailName,trail))
    {
        GlobalInfo::SimplePose pose;
        for(unsigned int i = 0; i < trail.mTrail.size(); i++)
        {
            Buoy marker;
            pose = trail.mTrail[i];
            marker.SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose,
                                                                       this->mpGlobalMap->ConvertFromGlobalToMapCoordinates(pose.mPosition)));

            marker.SetBuoyColor(Buoy::Green);
            marker.SetRadius(this->mWaypointThresh/2.0);

            this->DrawLocalObject(true, &marker, marker.GetDrawColor());
        }
        Buoy marker;
        marker.SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose,
                                                                   this->mpGlobalMap->ConvertFromGlobalToMapCoordinates(mGuessPose.mPosition)));

        marker.SetBuoyColor(Buoy::Red);
        marker.SetRadius(this->mWaypointThresh/2.0);

        this->DrawLocalObject(true, &marker, marker.GetDrawColor());
    }
    // Draw vehicle on top of everything.
    mMapPose.Draw(mpNavigator->GetLocalMapImage(ObjectMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}


/*  End of File */

