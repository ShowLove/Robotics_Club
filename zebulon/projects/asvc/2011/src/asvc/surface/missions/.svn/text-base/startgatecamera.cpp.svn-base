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
    if(mReturnFlag)
    {
        name = ReturnName;
    }
    ID = name;

    if(mReturnFlag)
    {
        mNextMission = BuoyNavCamera::ReturnName;
    }
    else
    {
        mNextMission = BuoyNavCamera::Name;
    }

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

    mRedDebounce.Initialize(3, true);
    mGreenDebounce.Initialize(3, true);
    mBothDebounce.Initialize(3, true);

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

    mState = PreStart;
    LoadSettings();
    InitializeMaps(Mission::mpGlobalInfo, mXmlConf, name);
    mStopWatch.Initialize(mPreDriveTimeSeconds);
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
    RunVisionClassifier();

    mDistanceFromStart = AI::Utility::CalcDistance(mCurrentPosition.mEasting,
                                                   mCurrentPosition.mNorthing,
                                                   mStartPosition.mEasting,
                                                   mStartPosition.mNorthing);

    mForwardThrust = mWaypointSpeed;
    std::cout << "================================================" << std::endl;
    std::cout << GetName() << std::endl;
    std::cout << "State: " << ToString((State)mState) << std::endl;

    switch(mState)
    {
    case PreStart:
        mStartPosition = mCurrentPosition;
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

        mpGlobalCommand->SetForwardThrust(mForwardThrust);
        mpGlobalCommand->SetTurningThrust(0.0);

        std::cout << "Thrust: " << mForwardThrust << std::endl;
        std::cout << "Time: " << mStopWatch.TimeElapsed() << std::endl;
        return Mission::KeepRunning;
        break;
        // Later projection
    case Start:
        // Push this point to our trail of visited points.
        Mission::mpGlobalInfo->PushCurrentPositionToTrail();
        // Grab initial information
        mStartHeading = mCurrentHeading;

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

        // Check if any buoys are found
        if(mBothDebounce.GetDebounced())
        {
            mLockHeading = mCurrentHeading;
            mState = FoundBoth;
        }
        else if(mRedDebounce.GetDebounced())
        {
            mLockHeading = mCurrentHeading;
            mState = FoundRed;
        }
        else if(mGreenDebounce.GetDebounced())
        {
            mLockHeading = mCurrentHeading;
            mState = FoundGreen;
        }
        else
        {
            if(mDistanceFromStart >= mSecondGateDistance)
            {
                mStopWatch.Start();
                mLockHeading = mStartHeading;
                mDesiredHeading = mStartHeading;
                mState = CheckGate;
            }

            mForwardThrust = mWaypointSpeed;
            mDesiredHeading = CxUtils::CxToDegrees(CxUtils::Orientation::GetGlobalAngle(mCurrentPosition,
                                                                                        mWaypoints[mWaypointCount]));

        }


        std::cout << "Distance to current: " << mTempDistCalc
                  << " Waypoints Reached: " << mReachedWaypoint
                  << " Waypoint Count :" << mWaypointCount
                  << " Waypont Thresh " << mWaypointThresh
                  <<  std::endl;

        break;

    case FoundBoth:
        // Both gates were found

        if(!mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mFromVisionState = true;
            mState = DriveToWaypoint;
        }
        else if(!mRedDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundGreen;
        }
        else if(!mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundRed;
        }
        // If they are both still valid
        else
        {
            // get the point on the screen that is in the middle of the two centers
            mCenterI = (mGreen.mCenterI + mRed.mCenterI)/2.0;
            mCenterJ = (mGreen.mCenterJ + mRed.mCenterJ)/2.0;

            mForwardThrust = mBothBuoySpeed;

            // if the j center is above the max j assume we have passed the gates
            if (mCenterJ > mMaxJ)
            {
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }

            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
                                                               ((mCenterI - 360)*mPixelScale), false);

        }

        AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,255,255);

        std::cout << "CenterI :" << mCenterI
                  << " CenterJ :" << mCenterJ
                  << " Green area " << mGreen.mArea
                  << " Red area " << mRed.mArea
                  << std::endl;
        break;
    case FoundRed:
        if(mGreenDebounce.GetDebounced() && !mRedDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundGreen;
        }
        else if(mBothDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundBoth;
        }
        else if(!mGreenDebounce.GetDebounced() && !mRedDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mFromVisionState = true;
            mState = DriveToWaypoint;
        }
        else
        {
            // Get the center of the red object
            mCenterI = mRed.mCenterI;
            mCenterJ = mRed.mCenterJ;

            mForwardThrust = mRedBuoySpeed;

            // If the center j is above some max assume we have passed the object
            if(mCenterJ > mMaxJ)
            {
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }

            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
                                                               ((mCenterI-mLeftI)*mPixelScale),
                                                               false);
            AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,255,0,0);
        }

        std:: cout << "LeftI :"  << mLeftI
                   << " CenterI :" << mCenterI
                   << " CenterJ :" << mCenterJ
                   << " Area " << mRed.mArea
                   << std::endl;
        break;

    case FoundGreen:

        // Slow down and maintain our heading until we need to turn
        // if we lost the green gate go back to look at camera data
        if(mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundRed;
        }
        else if(mBothDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mState = FoundBoth;
        }
        else if(!mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = mStartHeading;
            mFromVisionState = true;
            mState = DriveToWaypoint;
        }
        else
        {
            // Get the center of the object
            mCenterI = mGreen.mCenterI;
            mCenterJ = mGreen.mCenterJ;

            mForwardThrust = mGreenBuoySpeed;

            // If the center j is above some max assume we have passed the object
            if(mCenterJ > mMaxJ)
            {
                mLockHeading = mCurrentHeading;
                mStopWatch.Start();
                mState = CheckGate;
            }

            mDesiredHeading = CxUtils::Orientation::AddToAngle(mCurrentHeading,
                                                               ((mCenterI-mRightI)*mPixelScale),
                                                               false);
            AI::Utility::HeadingDisplay(mFinalFrame,mCurrentHeading,mDesiredHeading,0,255,0);
        }

        std:: cout << "LeftI :"  << mLeftI
                   << " CenterI :" << mCenterI
                   << " CenterJ :" << mCenterJ
                   << " Area " << mGreen.mArea
                   << std::endl;

        break;
    case CheckGate:

        // Transition out of SpeedGate
        if(mStopWatch.IsFinished())
        {
            if(mSecondGateDistance > mDistanceFromStart)
            {
                //mForwardThrust = 0;
                mDesiredHeading = mStartHeading;
                mState = DriveToWaypoint;
            }
            else
            {
                // mForwardThrust = 0;
                mLockHeading = mStartHeading;
                std::cout << "Loading next mission\n";
                Utility::Display::CloseAllWindows();

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

    mpGlobalCommand->SetYawDegrees(mDesiredHeading);
    mpGlobalCommand->SetForwardVelocityPercent(mForwardThrust);

    this->DrawMaps();
    //this->DisplayMaps(MapBuilder::GlobalMap);
    this->DisplayMaps(MapBuilder::ObjectMap);
    //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    Utility::Display::DisplayImage("Final Frame", mFinalFrame);

    return Mission::KeepRunning;
}


/** Runs vision color segmentation code and debounces results for
    finding red and green buoys making up the gates for travelling through. */
void StartGateCamera::RunVisionClassifier()
{
    // If we haven't crated an image for the final processed image do so
    if(mFinalFrame == NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), 8, 3);
    }
    // set the entire image to black
    Vision::VisionUtil::ResetImage(mFinalFrame);
    // Green Gate
    // Setup Segment for green
    mSegment->Setup(mGreenBuoy, 50);
    // Setup threshold for green
    mThreshold->Setup(mGreenBuoy);
    // Segment/threshold forward for green
    mThreshold->Process(mInitFrame, mThreshFrame);
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
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // Get red return values
    mSegment->GetMaxPosition(mRed);
    // Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    // Debouncing Info
    mRedDebounce.Bounce(mRed.mValid == 1 && mRed.mArea >= mMinRedArea);
    mGreenDebounce.Bounce(mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea);
    mBothDebounce.Bounce((mGreen.mValid == 1 && mGreen.mArea >= mMinGreenArea) && (mRed.mValid == 1 && mRed.mArea >= mMinRedArea));

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

    // Draw vehicle on top of everything.
    mMapPose.Draw(mNavigator.GetLocalMapImage(ObjectMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}


/*  End of File */

