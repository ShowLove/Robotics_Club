//////////////////////////////////////////////////////////////////////////////////////
///
/// \file extinguish.cpp
/// \brief Mission identifies and shoots at a target (in this case a boat)
///
/// Author(s): Michael Scherer<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:michael.scherer@knights.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
//////////////////////////////////////////////////////////////////////////////////////
#include "asvc/surface/missions/extinguish.h"
#include <cartographer/objects/line.h>
#include <algorithm>
#include <limits>

using namespace Zebulon;
using namespace ASVC;
using namespace Cartographer;

const std::string Extinguish::Name = "Extinguish";

const double FOUND_SIZE = 0.5;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
Extinguish::Extinguish(const bool returnNavFlag)
{
    std::string name = Name;
    ID = name;

    mNextMission = "";
    mCurrentState = Search;
    mDebounceLimit = 3;
    mDriveThroughDistanceThresh = 0.5;
    mDriveTimeSeconds = 1;
    mTravelSpeed = 30;
    mSearchAngle = CxUtils::CxToRadians(90);
    mSearchDepth = 10.0;
    mTimeoutTimeSeconds = 120;
    mSearchFlag = true;

    mSearchSweepSpanDegrees = 60;
    mSearchSweepSpeed = 20;
    mSearchForwardTime = 5;
    mSearchForwardSpeed = 25;

    mInitFrame      = NULL;
    mFinalFrame     = NULL;
    mGrayFrame      = NULL;
    mSegFrame       = NULL;
    mThreshFrame    = NULL;

    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Extinguish::~Extinguish()
{
    cvReleaseImage(&mInitFrame);
    cvReleaseImage(&mFinalFrame);
    cvReleaseImage(&mGrayFrame);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Extinguish::Init()
{
    int result = 0;

    std::string name = Name;
    Mission::Init();

    mCurrentState = Search;
    mSearchFlag = true;

    std::string baseName = std::string("Surface.") + name + ".";
    std::string templateImage;
    mXmlConf.GetVar(baseName + "SURF@template", templateImage);
    IplImage* tmpTemplate = cvLoadImage(templateImage.c_str());
    mSURFTemplateImage = cvCreateImage(cvSize(tmpTemplate->width, tmpTemplate->height), IPL_DEPTH_8U, 1);
    Vision::OpenCvSurf::CvtBGRtoHSGray(tmpTemplate, mSURFTemplateImage);
    cvReleaseImage(&tmpTemplate);
    //mSURFTemplateImage = cvLoadImage(templateImage.c_str(), CV_LOAD_IMAGE_GRAYSCALE);

    LoadSettings();

    mDebounce.Initialize(mDebounceLimit, true);
    mHeadingDebounce.Initialize(mHeadingDebounceLimit, true);

    InitializeMaps(Mission::mpGlobalInfo, mXmlConf, name);
    mTimeoutStopWatch.Initialize(mTimeoutTimeSeconds);
    mTimeoutStopWatch.Start();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Extinguish::ErrorCheck()
{
    int result = 0;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Fetches data required for execution.
///
///  Gets start button state, and checks to see if GPS data is available.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int Extinguish::FetchProcess()
{
    int result = 0;

    // Get new parameters from XML if anything changes.
    LoadSettings();

    // Kinematics
    mOrientation = Mission::mpGlobalInfo->GetOrientation();

    // Get main camera image.
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mInitFrame);

    // Update our maps (raw laser data, and object detection).
    UpdateMaps(true);

    // Run vision classification
    this->RunVisionClassifier();

    // Set the computer light to even/odd zone.
    // Zones we are not in get cleared of objects (not raw data).
    if(GetZoneNumber() >= 0)
    {
        // Indexing starts at 0
        if(GetZoneNumber()%2)
        {
            mpGlobalCommand->SetComputerLightOne(true);
            mpGlobalCommand->SetComputerLightTwo(false);

        }
        else
        {
            mpGlobalCommand->SetComputerLightOne(false);
            mpGlobalCommand->SetComputerLightTwo(true);
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Extinguish::ExitEarly()
{
    if(this->mTimeoutStopWatch.IsInitialized() && this->mTimeoutStopWatch.IsFinished())
    {
        Utility::Display::CloseAllWindows();
        return mNextMission;
    }
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
std::string Extinguish::Execute()
{
    double travelSpeed = mTravelSpeed;
    double heading = mMapPose.mRotation.mZ;
    double lateralSpeed = 0;

    this->mpGlobalCommand->SetWaterCannonTilt(85);

    double closeDist;

    switch(mCurrentState)
    {
    // We don't see the boat, look around for it
    case Search:
        {
            std::cout << "=======================================" << std::endl
                      << "Extinguish::Search" << std::endl
                      << "I: " << mBoatI << std::endl;

            if (mSearchFlag)
            {
                mSweepSearch.InitSearch(CxUtils::CxToDegrees(mMapPose.Yaw()),
                                        this->mSearchSweepSpanDegrees,
                                        this->mSearchSweepSpeed,
                                        this->mSearchForwardTime,
                                        this->mSearchForwardSpeed);
                mSearchFlag = false;
            }

            lateralSpeed = 0;

            //if the boat is found
            if(mDebounce.GetDebounced())
            {
                mCurrentState = ApproachBoat;
                heading = mMapPose.Yaw();
            }
            else
            {
                // Must convert from degrees to radians.
                mSweepSearch.SearchStep(heading, travelSpeed);
                heading = CxUtils::CxToRadians(heading);
            }
        }
        break;

    // Boat identified, go close to it
    case ApproachBoat:
        {
            std::cout << "=======================================" << std::endl
                      << "Extinguish::ApproachBoat" << std::endl
                      << "I: " << mBoatI << std::endl;

            double diff = CxUtils::CxToRadians(mTurnWeight*(mBoatI - (mInitFrame->width/2)));
            heading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(), diff);
            std::cout << "Diff: " << diff << std::endl;

            lateralSpeed = mLateralWeight*(mBoatI - (mInitFrame->width/2));

            travelSpeed = mTravelSpeed;

            double vehicleWidth;
            Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vehicleWidth);
            CxUtils::Segment3D seg(CxUtils::Point3D(), CxUtils::Point3D(mShoreDist, 0, 0), vehicleWidth);
            Cartographer::Object::Collisions collisions;
            GetWorldMap()->ScanLocalSegmentInXY(this->GetCurrentPose(), seg, &collisions);
            closeDist = 10000;
            Cartographer::Object::Collisions::iterator colObj;
            for (colObj = collisions.begin(); colObj != collisions.end(); colObj++)
            {
                double dist = (*colObj)->GetDistance(this->GetCurrentPose().mPosition);
                if (dist < closeDist)
                    closeDist = dist;
            }

            travelSpeed = mTravelSpeedP*(closeDist - mShoreDist);
            travelSpeed = AI::Utility::Saturate(travelSpeed, -mTravelSpeed, mTravelSpeed);

            if (!mDebounce.GetDebounced())
            {
                mCurrentState = Search;
                mSearchFlag = true;
            }

            if (mHeadingDebounce.Bounce(fabs(diff) < mHeadingThreshold) && closeDist < mShoreDist + mShoreDistThreshold && closeDist > mShoreDist - mShoreDistThreshold)
            {
                mCurrentState = SquirtBoat;
                mSquirtStopWatch.Initialize(mSquirtTime);
                mSquirtStopWatch.Start();
            }
        }
        break;

    // We've approached, now time to get the boat all wet
    case SquirtBoat:
        {
            std::cout << "=======================================" << std::endl
                      << "Extinguish::SquirtBoat" << std::endl
                      << "I: " << mBoatI << std::endl
                      << "Time: " << mSquirtStopWatch.TimeElapsed() << std::endl;

            mpGlobalCommand->SetWaterCannon(100);

            lateralSpeed = mLateralWeight*(mBoatI - (mInitFrame->width/2));

            travelSpeed = mTravelSpeed;

            double vehicleWidth;
            Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vehicleWidth);
            CxUtils::Segment3D seg(CxUtils::Point3D(), CxUtils::Point3D(mShoreDist, 0, 0), vehicleWidth);
            Cartographer::Object::Collisions collisions;
            GetWorldMap()->ScanLocalSegmentInXY(this->GetCurrentPose(), seg, &collisions);
            closeDist = 10000;
            Cartographer::Object::Collisions::iterator colObj;
            for (colObj = collisions.begin(); colObj != collisions.end(); colObj++)
            {
                double dist = (*colObj)->GetDistance(this->GetCurrentPose().mPosition);
                if (dist < closeDist)
                    closeDist = dist;
            }

            travelSpeed = mTravelSpeedP*(closeDist - mShoreDist);
            travelSpeed = AI::Utility::Saturate(travelSpeed, -mTravelSpeed, mTravelSpeed);

            cvDrawCircle(mFinalFrame, cvPoint(mFinalFrame->width/2, mFinalFrame->height/2), 50, cvScalar(0,255,0));

            if (mSquirtStopWatch.IsFinished())
            {
                mpGlobalCommand->SetWaterCannon(0);
                mCurrentState = Leave;
            }
        }
        break;

    // Get out of here and away from the shore line
    case Leave:
        std::cout << "=======================================" << std::endl
                  << "Extinguish::Leave" << std::endl;

        lateralSpeed = 0;
        travelSpeed = 0;
        heading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(), CxUtils::PI);

        Utility::Display::CloseAllWindows();
        return mNextMission;
    }

    mpGlobalCommand->SetForwardVelocityPercent(travelSpeed);
    mpGlobalCommand->SetLateralVelocityPercent(lateralSpeed);
    mpGlobalCommand->SetYaw(heading);
    std::cout << "Heading " << CxUtils::CxToDegrees(mMapPose.Yaw())
              << " -> " << CxUtils::CxToDegrees(heading)
              << " Diff " << CxUtils::CxToDegrees(CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), heading)) << std::endl
              << "Travel Speed " << travelSpeed
              << " Lateral Speed " << lateralSpeed << std::endl
              << "Shore Dist " << closeDist
              << " Time Remaining: " << mTimeoutTimeSeconds - mTimeoutStopWatch.TimeElapsed()
    << std::endl;

    AI::Utility::HeadingDisplay(mFinalFrame,
                                CxUtils::CxToDegrees(mOrientation.mZ),
                                CxUtils::CxToDegrees(heading),
                                255, 0, 255);
    this->DrawMaps();
    //this->DisplayMaps(MapBuilder::GlobalMap);
    this->DisplayMaps(MapBuilder::GlobalMap);
    //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    Utility::Display::DisplayImage("Final Frame", mFinalFrame);

    return Mission::KeepRunning;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Updates map using information from LIDAR and any computer vision
///         data.
///
///  \param[in] clearMaps If true, old data is cleared.
///
////////////////////////////////////////////////////////////////////////////////////
void Extinguish::UpdateMaps(const bool clearMaps)
{
    //MapBuilder::UpdateMaps(clearMaps);
    MapBuilder::UpdateMaps(clearMaps, false);
    // Add content to maps/filter using computer vision data.
}

/** Runs GVision classification of buoys by colors. */
void Extinguish::RunVisionClassifier()
{
    // If we haven't created an image for the final processed image do so
    if(mFinalFrame == NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), IPL_DEPTH_8U, 3);
        mGrayFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), IPL_DEPTH_8U, 1);
        std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
    }
    cvCopy(mInitFrame, mFinalFrame);
    Vision::OpenCvSurf::CvtBGRtoHSGray(mInitFrame, mGrayFrame);
    //cvCvtColor(mInitFrame, mGrayFrame, CV_RGB2GRAY);

    Vision::SurfReturn surf = mSURF.FindObject(mGrayFrame, mSURFTemplateImage, mSURFParam1, mSURFParam2);

    // Blue Boat
    // Setup Segment for green
    mSegment->Setup(mBlueTarget, mBlueMinArea, mBlueComboDist);
    // Setup threshold for green
    mThreshold->Setup(mBlueTarget);
    // segment/threshold forward for green
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // Get green return values.
    mSegment->GetMaxPosition(mBlueReturn);

    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    if (mDebounce.Bounce(surf.Valid || mBlueReturn.mValid))
    {
        if (surf.Valid && mBlueReturn.mValid)
        {
            mBoatI = (surf.CenterI + mBlueReturn.mCenterI)/2.;
            mBoatJ = (surf.CenterJ + mBlueReturn.mCenterJ)/2.;
        }
        else if (surf.Valid)
        {
            mBoatI = surf.CenterI;
            mBoatJ = surf.CenterJ;
        }
        else
        {
            mBoatI = mBlueReturn.mCenterI;
            mBoatJ = mBlueReturn.mCenterJ;
        }

        cvDrawRect(mFinalFrame, cvPoint(mBoatI-100, mBoatJ-100), cvPoint(mBoatI+100, mBoatJ+100), cvScalar(0, 255*mBlueReturn.mValid, 255*surf.Valid), 2);
    }
}

/** Loads settings from XML file. */
void Extinguish::LoadSettings()
{
    std::string name = Name;

    std::string baseName = std::string("Surface.") + name + ".";
    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);
    mXmlConf.GetVar(baseName + "TravelSpeed", mTravelSpeed);
    mXmlConf.GetVar(baseName + "SearchAngleDegrees", mSearchAngle);
    mSearchAngle = CxUtils::CxToRadians(mSearchAngle);
    mXmlConf.GetVar(baseName + "SearchDepthMeters", mSearchDepth);
    mXmlConf.GetVar(baseName + "SweepSearch@sweep_degrees", mSearchSweepSpanDegrees);
    mXmlConf.GetVar(baseName + "SweepSearch@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar(baseName + "SweepSearch@time_forward", mSearchForwardTime);
    mXmlConf.GetVar(baseName + "SweepSearch@forward_speed", mSearchForwardSpeed);

    // SURF
    mXmlConf.GetVar(baseName + "SURF@param_1", mSURFParam1);
    mXmlConf.GetVar(baseName + "SURF@param_2", mSURFParam2);
    mXmlConf.GetVar(baseName + "SURF@min_pairs", mSURFMinPairs);
    mXmlConf.GetVar(baseName + "SURF@debounce", mDebounceLimit);
    mSURF.SetMinPointPairs(mSURFMinPairs);

    // GVision
    mXmlConf.GetVar(baseName + "BoatColors@red", mBlueTarget.mRed);
    mXmlConf.GetVar(baseName + "BoatColors@green", mBlueTarget.mGreen);
    mXmlConf.GetVar(baseName + "BoatColors@blue", mBlueTarget.mBlue);
    mXmlConf.GetVar(baseName + "BoatColors@red_thresh", mBlueTarget.mRedRange);
    mXmlConf.GetVar(baseName + "BoatColors@green_thresh", mBlueTarget.mGreenRange);
    mXmlConf.GetVar(baseName + "BoatColors@blue_thresh", mBlueTarget.mBlueRange);
    mXmlConf.GetVar(baseName + "BoatColors@min_area", mBlueMinArea);
    mXmlConf.GetVar(baseName + "BoatColors@combo_dist", mBlueComboDist);

    // Squirt!
    mXmlConf.GetVar(baseName + "Squirt@heading_thresh", mHeadingThreshold);
    mXmlConf.GetVar(baseName + "Squirt@heading_debounce", mHeadingDebounceLimit);
    mXmlConf.GetVar(baseName + "Squirt@duration", mSquirtTime);
    mXmlConf.GetVar(baseName + "Squirt@turn_weight", mTurnWeight);
    mXmlConf.GetVar(baseName + "Squirt@lateral_weight", mLateralWeight);
    mXmlConf.GetVar(baseName + "Squirt@shore_dist", mShoreDist);
    mXmlConf.GetVar(baseName + "Squirt@shore_dist_thresh", mShoreDistThreshold);
    mXmlConf.GetVar(baseName + "Squirt@travel_p", mTravelSpeedP);

    std::string missionTimeString;
    mXmlConf.GetVar(baseName + "MissionTime", missionTimeString);
    CxUtils::Time timeToRun;
    timeToRun.FromString(missionTimeString);
    mTimeoutTimeSeconds = (unsigned int)timeToRun.ToSeconds();
}

/** Draws maps for display purposes */
void Extinguish::DrawMaps()
{
    MapBuilder::DrawMaps();

    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                             mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);

    // Define the scan region in object map.
    Point3D upperLeft, upperRight, start, top;

    double vLength = 0, vWidth = 0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);

    start(-vLength/2.0, 0, 0);
    top(mSearchDepth, 0, 0);
    upperLeft = top;
    upperLeft = upperLeft.Rotate(-1*mSearchAngle/2.0, Point3D::Z);
    upperRight(upperLeft.mX, fabs(upperLeft.mY), 0.0);

    // Draw vehicle.
    mMapPose.Draw(mNavigator.GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}

/*  End of File */

