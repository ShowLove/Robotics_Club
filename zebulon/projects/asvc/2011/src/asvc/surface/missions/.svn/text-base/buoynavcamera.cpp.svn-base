//////////////////////////////////////////////////////////////////////////////////////
///
/// \file buoynavcamera.cpp
/// \brief Mission 3, goes through buoy channel.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
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
#include "asvc/surface/missions/buoynavcamera.h"
#include <cartographer/objects/line.h>
#include <statemachine/utility/utility.h>
#include <algorithm>
#include <limits>

using namespace Zebulon;
using namespace ASVC;
using namespace Cartographer;

const std::string BuoyNavCamera::Name = "BuoyNavCamera";
const std::string BuoyNavCamera::ReturnName = "ReturnBuoyNavCamera";

const double FOUND_SIZE = 0.5;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNavCamera::BuoyNavCamera(const bool returnNavFlag)
{
    mReturnFlag = returnNavFlag;

    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }
    ID = name;

    mFirstBuoyFoundFlag = false;

    mCurrentState = Find;
    mPrevState = mPrevState;
    mNextMission = "Extinguish";
    mCurrentState = Find;

    mSearchSweepSpanDegrees = 60;
    mSearchSweepSpeed = 20;
    mSearchForwardTime = 5;
    mSearchForwardSpeed = 25;

    // Init vision stuff.
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();

    mInitFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mFinalFrame = NULL;

    mVisionDebounceLimit = 3;

    mMinPixelArea = 45;
    mMaxPixelArea = 90000;

    mBlueExitMinSize = 100;
    mBlueDebounceLimit = 10;

    mGlobalExitDuration = 240;
    mGlobalMinDuration = 60;

    mLockHeadingDuration = 3;

    mMaxBuoyCount = 8;

    mExitHeading = CxUtils::CxToRadians(210);

    mBeamLengthMeters = 2.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNavCamera::~BuoyNavCamera()
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

    mInitFrame = NULL;
    mSegFrame = NULL;
    mThreshFrame = NULL;
    mFinalFrame = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BuoyNavCamera::Init()
{
    int result = 0;

    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }

    Mission::Init();

    mCurrentState = Find;

    InitializeMaps(Mission::mpGlobalInfo, mXmlConf, name);

    for(unsigned int i = 0; i < (unsigned int)mZones.size(); i++)
    {
        mSweepDir[i] = 1;
    }

    LoadSettings();

    mBuoyCount = mMaxBuoyCount;

    mRedDebounce.Initialize(mVisionDebounceLimit, false);
    mGreenDebounce.Initialize(mVisionDebounceLimit, false);
    mYellowDebounce.Initialize(mVisionDebounceLimit, false);
    mBlueDebounce.Initialize(mBlueDebounceLimit, false);

    mTravelSpeed = 20;



    mGlobalExitTime.Initialize(mGlobalExitDuration);
    mGlobalExitTime.Start();

    mSearchFlag = false;

    mDesiredHeading = Mission::mpGlobalInfo->GetOrientation().mZ;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BuoyNavCamera::ErrorCheck()
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
int BuoyNavCamera::FetchProcess()
{
    int result = 0;

    // Get new parameters from XML if anything changes.
    LoadSettings();

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
std::string BuoyNavCamera::ExitEarly()
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
std::string BuoyNavCamera::Execute()
{
    // Load any settings for dynamic changes.
    LoadSettings();
    double travelSpeed = mTravelSpeed;
    mPrevState = mCurrentState;

    Cartographer::Object::Collisions collisions;
    Cartographer::Object::Collisions::iterator collision;
    Cartographer::Segment3D beam;
    beam.mPoint2(mBeamLengthMeters, 0, 0);
    double vWidth;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);
    beam.mWidth = vWidth/2.0;

    bool isCollision = this->mpGlobalMap->ScanLocalSegmentInXY(mMapPose, beam, &collisions);
    Object* closestObject = NULL;
    if(isCollision)
    {
        double distance = 5000;
        double minDistance = distance;
        for(collision = collisions.begin();
            collision != collisions.end();
            collision++)
        {
            distance = (*collision)->GetDistance(mMapPose.mPosition);
            if(distance < minDistance)
            {
                minDistance = distance;
                if(closestObject)
                {
                    delete closestObject;
                }
                closestObject = (*collision)->ConvertFromMapToLocalCoordinates(mMapPose);
            }
        }
    }

    this->mpGlobalCommand->SetWaterCannonTilt(75);

    std::cout << "================================================" << std::endl;
    std::cout << GetName() << std::endl;
    std::cout << "State: " << ToString((State)mState) << std::endl;
    switch (mState)
    {
        // Figure out combination of buoys found.
        case Find:
            if (mRedDebounce.GetDebounced() && mGreenDebounce.GetDebounced())
            {
                mState = RedGreen;
            }
            else if (mRedDebounce.GetDebounced())
            {
                mState = Red;
            }
            else if (mGreenDebounce.GetDebounced())
            {
                mState = Green;
            }
            else if (mYellowDebounce.GetDebounced())
            {
                mState = Yellow;
            }
            else
            {
                mNewStateTimer.Initialize(mNewStateDuration);
                mNewStateTimer.Start();
                mSearchFlag = true;
                mState = Search;
            }
            break;
        // We see both the Red and Green Buoys!
        case RedGreen:
            if (mRedDebounce.GetDebounced() && mGreenDebounce.GetDebounced())
            {
                // Find center between green and red
                mCenterI = (mGreen.mCenterI + mRed.mCenterI)/2;
                mCenterJ = (mGreen.mCenterJ + mRed.mCenterJ)/2;

                // No yellow, you're okay fellow
                if (!closestObject || !mYellowDebounce.GetDebounced() || (mYellow.mCenterI < mRed.mCenterI || mYellow.mCenterI > mGreen.mCenterI))
                {
                    mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                      CxUtils::CxToRadians(((mCenterI - mFinalFrame->width/2)*mPixelScale)));
                }
                // We see yellow (oh noes!) go between it and another bouy on whichever side has more room
                else
                {
                    if ((closestObject && closestObject->GetOrigin().mY <= 0) || mYellow.mCenterI < mCenterI)    // Area larger on right side
                    {
                        // Red right return
                        mCenterI = (mGreen.mCenterI + mYellow.mCenterI)/2;
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                           CxUtils::CxToRadians(((mCenterI - mFinalFrame->width/2)*mPixelScale)));
                    }
                    else                                // Area larger on left side
                    {
                        mCenterI = (mRed.mCenterI + mYellow.mCenterI)/2;
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                           CxUtils::CxToRadians(((mCenterI - mFinalFrame->width/2)*mPixelScale)));
                    }
                }

                if ((mRed.mCenterJ > mMaxJ) || (mGreen.mCenterJ > mMaxJ))
                {
                    // Push this point to our trail of visited points.
                    Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                    mBuoyCount--;
                    mState = LockHeading;

                    mLockHeadingTimer.Initialize(mLockHeadingDuration);
                    mLockHeadingTimer.Start();
                }
            }
            else
            {
                mState = Find;
            }
            break;
        // Only see red
        case Red:
            if (!mGreenDebounce.GetDebounced() && mRedDebounce.GetDebounced())
            {
                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                   CxUtils::CxToRadians((mRed.mCenterI  - mLeftI)*mPixelScale));

                if (mRed.mCenterJ > mMaxJ)
                {
                    // Push this point to our trail of visited points.
                    Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                    mBuoyCount--;
                    mState = LockHeading;

                    mLockHeadingTimer.Initialize(mLockHeadingDuration);
                    mLockHeadingTimer.Start();
                }

                if(closestObject || (mYellowDebounce.GetDebounced() && (mRed.mCenterI < mYellow.mCenterI)))
                {
                    if((closestObject && closestObject->GetOrigin().mY <= 0) || mYellow.mCenterI > mRed.mCenterI + (mFinalFrame->width-mRed.mCenterI)/2)
                    {
                        // Go to left of yellow and red
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, mYellowOffset);
                    }
                    else
                    {
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, -mYellowOffset);
                    }
                }
            }
            else
            {
                mState = Find;
            }
            break;
        // Only see Green
        case Green:
            if (mGreenDebounce.GetDebounced() && !mRedDebounce.GetDebounced())
            {
                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                   CxUtils::CxToRadians((mGreen.mCenterI- mFinalFrame->width + mRightI)*mPixelScale));


                if (mGreen.mCenterJ > mMaxJ)
                {
                    // Push this point to our trail of visited points.
                    Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                    mBuoyCount--;
                    mState = LockHeading;

                    mLockHeadingTimer.Initialize(mLockHeadingDuration);
                    mLockHeadingTimer.Start();
                }

                if(closestObject || (mYellowDebounce.GetDebounced() && (mGreen.mCenterI >  mYellow.mCenterI)))
                {
                    if((closestObject && closestObject->GetOrigin().mY <= 0) || mYellow.mCenterI < mGreen.mCenterI/2)
                    {
                        // Go to left of yellow and red
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, mYellowOffset);
                    }
                    else
                    {
                        mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, -mYellowOffset);
                    }
                }
            }
            else
            {
                mState = Find;
            }
            break;

        case Yellow:
            /*
            if (mGreenDebounce.GetDebounced() || mRedDebounce.GetDebounced())
            {
                mState = Find;
            }
            else if(mYellowDebounce.GetDebounced())
            {
                if (mYellow.mCenterI > mFinalFrame->width/2)
                {
                    mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, -mYellowOffset);
                }
                else
                {

                    mDesiredHeading = CxUtils::Orientation::AddToAngle(mDesiredHeading, mYellowOffset);
                }
            }
            else
            {
                mState  = Find;
            }
            */
            mState = Find;
            break;

        case Search:
            if(mSearchFlag)
            {
                std::map<unsigned int, int>::iterator zoneDir;
                zoneDir = mSweepDir.find(this->GetZoneNumber());
                int startTurnDirection = 1;
                if(zoneDir != mSweepDir.end())
                {
                    startTurnDirection = zoneDir->second;
                }
                mSweepSearch.InitSearch(CxUtils::CxToDegrees(mMapPose.Yaw()),
                                        mSearchSweepSpanDegrees,
                                        mSearchSweepSpeed,
                                        mSearchForwardTime,
                                        mSearchForwardSpeed,
                                        startTurnDirection);
                mSearchFlag = false;
            }
            mSweepSearch.SearchStep(mDesiredHeading, travelSpeed);
            mDesiredHeading = CxUtils::CxToRadians(mDesiredHeading);

            if(mGreenDebounce.GetDebounced() ||
               mRedDebounce.GetDebounced() ||
               mYellowDebounce.GetDebounced())
            {
                mState = Find;
            }

            if (mGlobalExitTime.TimeElapsed() >= mGlobalMinDuration &&
                (mNewStateTimer.IsFinished() || mGlobalExitTime.IsFinished()))
            {
                // Go to exit heading.
                mDesiredHeading = mExitHeading;
                //CxUtils::SleepMs()
                if(mReturnFlag == false) // Don't set values if this is the return trip version of mission.
                {
                    Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndPosition, Mission::mpGlobalInfo->GetPosition());
                    Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndHeading, mMapPose.Yaw());
                }
                mState = ExitHeading;
            }

        break;
        // Lay in the course, Make it so (in Captain Picard's voice)
        case LockHeading:
            if (mLockHeadingTimer.IsFinished())
            {
                if (mBuoyCount > 0)
                    mState = Find;
                else
                {
                    if(mReturnFlag == false) // Don't set values if this is the return trip version of mission.
                    {
                        // Push this point to our trail of visited points.
                        Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                        Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndPosition, Mission::mpGlobalInfo->GetPosition());
                        Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndHeading, mMapPose.Yaw());
                    }
                    if(mGlobalExitTime.TimeElapsed() >= mGlobalMinDuration)
                    {
                        mState = ExitHeading;
                    }
                    else
                    {
                        mState = Search;
                        mBuoyCount++;
                    }
                }
            }
            break;
       case ExitHeading:
            {
                mDesiredHeading = mExitHeading;
                mTravelSpeed = 0;
                double error = fabs(CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), mExitHeading, true));
                std::cout << GetName()
                          << " State " << ToString((State)mState)
                          << " Going to exit heading : " << CxUtils::CxToDegrees(mExitHeading)
                          << " Error: " << CxUtils::CxToDegrees(error) << std::endl;
                if(error <= CxUtils::CxToRadians(10))
                {
                    if(closestObject)
                    {
                        delete closestObject;
                    }

                    // Push this point to our trail of visited points.
                    Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                    Utility::Display::CloseAllWindows();
                    return mNextMission;
                }
            }
        break;
    default:
        std::cout << GetName() << " ERROR, Unknown State\n";
        break;
    }

    std::cout
    << "Heading " << CxUtils::CxToDegrees(mMapPose.Yaw())
    << " -> " << CxUtils::CxToDegrees(mDesiredHeading)
    << " Diff " << CxUtils::CxToDegrees(CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), mDesiredHeading)) << std::endl;
    std::cout << "Travel Speed " << travelSpeed
    << " Time Remaining: " << mGlobalExitDuration - mGlobalExitTime.TimeElapsed()
    << " Buoys Left " << mBuoyCount
    << std::endl;

    // Only exit when not returning through channel when we
    // hit the blue buoy.
    if(mGlobalExitTime.TimeElapsed() >= mGlobalMinDuration &&
       mReturnFlag == false &&
       mBlueDebounce.GetDebounced() && mBlue.mArea >= mBlueExitMinSize)
    {
        std::cout << GetName()
                  << " Hit Blue Buoy, Exiting...\n";
        if(mReturnFlag == false)  // Don't set values if this is the return trip version of mission.
        {
            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndPosition, Mission::mpGlobalInfo->GetPosition());
            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndHeading, mMapPose.Yaw());
        }
        mState = ExitHeading;
    }

    /*  Size difference not good enought to use laser for
        doing exit.
    Buoy::List buoys = this->GetClosestBuoys(5);
    Buoy::List::iterator buoy;
    for(buoy = buoys.begin();
        buoy != buoys.end();
        buoy++)
    {
        if(buoy->GetRadius() >= .35 && buoy->GetOrigin().Distance() <= 1.0)
        {
            std::cout << GetName()
                      << " Hit Blue Buoy, Exiting...\n";
            if(mReturnFlag == false)  // Don't set values if this is the return trip version of mission.
            {
                Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndPosition, Mission::mpGlobalInfo->GetPosition());
                Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelEndHeading, mMapPose.Yaw());
            }
            mState = ExitHeading;
        }
    }
    */
    this->DrawMaps();

    mpGlobalCommand->SetYaw(mDesiredHeading);
    mpGlobalCommand->SetForwardVelocityPercent(travelSpeed);
    AI::Utility::HeadingDisplay(mFinalFrame,
                                CxUtils::CxToDegrees(mMapPose.Yaw()),
                                CxUtils::CxToDegrees(mDesiredHeading),
                                255, 0, 255);

    this->DisplayMaps(MapBuilder::GlobalMap);
    //this->DisplayMaps(MapBuilder::ObjectMap);
    Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    Utility::Display::DisplayImage("Final Frame", mFinalFrame);

    if(closestObject)
    {
        delete closestObject;
    }
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
void BuoyNavCamera::UpdateMaps(const bool clearMaps)
{
    MapBuilder::UpdateMaps(clearMaps);
    // Add content to maps/filter using computer vision data.
}


/** Runs GVision classification of buoys by colors. */
void BuoyNavCamera::RunVisionClassifier()
{
    // If we haven't created an image for the final processed image do so
    if(mFinalFrame == NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), IPL_DEPTH_8U, 3);
        //std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
    }
    // set the entire image to black
    Vision::VisionUtil::ResetImage(mFinalFrame);

    // Green Buoy
    // Setup Segment for green
    mSegment->Setup(mGreenBuoy, this->mMinGreenPixelArea);
    // Setup threshold for green
    mThreshold->Setup(mGreenBuoy);
    // segment/threshold forward for green
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // Get green return values.
    mSegment->GetMaxPosition(mGreen);
    // Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    // Red Buoy
    // Setup Segment for Red
    mSegment->Setup(mRedBuoy, mMinRedPixelArea);
    // Setup threshold for Red
    mThreshold->Setup(mRedBuoy);
    // segment/threshold forward for Red
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // get red return values
    mSegment->GetMaxPosition(mRed);
    //Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    if(mReturnFlag) // Swap values here so that we will keep red on right when returning.
    {
        // Swap read and green.
        Vision::Return temp;
        temp = mGreen;
        mGreen = mRed;
        mRed = temp;
    }

    // Yellow Buoy
    // Setup Segment for Yellow
    mSegment->Setup(mYellowBuoy, mMinYellowPixelArea);
    // Setup threshold for Yellow
    mThreshold->Setup(mYellowBuoy);
    // segment/threshold forward for Yellow
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // get red return values
    mSegment->GetMaxPosition(mYellow);
    //Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    // Yellow Buoy
    // Setup Segment for Blue
    mSegment->Setup(mBlueBuoy, mMinBluePixelArea);
    // Setup threshold for Blue
    mThreshold->Setup(mBlueBuoy);
    // segment/threshold forward for Blue
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // get red return values
    mSegment->GetMaxPosition(mBlue);
    //Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    mRedDebounce.Bounce(mRed.mValid && mRed.mArea > mMinPixelArea && mRed.mArea < mMaxPixelArea);
    mGreenDebounce.Bounce(mGreen.mValid && mGreen.mArea > mMinPixelArea && mGreen.mArea < mMaxPixelArea);
    mYellowDebounce.Bounce(mYellow.mValid && mYellow.mArea > mMinPixelArea && mYellow.mArea < mMaxPixelArea);
    mBlueDebounce.Bounce(mBlue.mValid && mBlue.mArea > mMinPixelArea && mBlue.mArea < mMaxPixelArea);

    if(this->mFirstBuoyFoundFlag &&
       (mRedDebounce.GetDebounced() || mGreenDebounce.GetDebounced()))
    {
        this->mFirstBuoyFoundFlag = false;
        if(mReturnFlag == false) // Don't set values if this is the return trip version of mission.
        {
            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelStartPosition, Mission::mpGlobalInfo->GetPosition());
            Mission::mpGlobalInfo->SetInfo(GlobalInfo::ChannelStartHeading, mMapPose.Yaw());
        }
    }

}


/** Loads settings from XML file. */
void BuoyNavCamera::LoadSettings()
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
    mXmlConf.GetVar(baseName + "TravelSpeed", mTravelSpeed);
    mXmlConf.GetVar(baseName + "BeamLength@distance", mBeamLengthMeters);
    mXmlConf.GetVar(baseName + "MaxMissionDuration", mGlobalExitDuration);
    mXmlConf.GetVar(baseName + "MinMissionDuration", mGlobalMinDuration);
    mXmlConf.GetVar(baseName + "Buoys@count", mMaxBuoyCount);

    mXmlConf.GetVar(baseName + "Buoys@lock_duration", mLockHeadingDuration);

    mXmlConf.GetVar(baseName + "SweepSearch@sweep_degrees", mSearchSweepSpanDegrees);
    mXmlConf.GetVar(baseName + "SweepSearch@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar(baseName + "SweepSearch@time_forward", mSearchForwardTime);
    mXmlConf.GetVar(baseName + "SweepSearch@forward_speed", mSearchForwardSpeed);

    int sweepDir = 1;

    if(mXmlConf.GetVar(baseName + "SweepSearch@zone_one_direction", sweepDir, true) &&
            (sweepDir == 1 || sweepDir == -1))
    {
        mSweepDir[0] = sweepDir;
    }

    if(mXmlConf.GetVar(baseName + "SweepSearch@zone_two_direction", sweepDir, true) &&
            (sweepDir == 1 || sweepDir == -1))
    {
        mSweepDir[1] = sweepDir;
    }

    mXmlConf.GetVar(baseName + "SweepSearch@forward_speed", mSearchForwardSpeed);

    mXmlConf.GetVar(baseName + "BuoyVisionDebouncLimit", mVisionDebounceLimit);
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

    mXmlConf.GetVar(baseName + "BuoyYellow@red", mYellowBuoy.mRed);
    mXmlConf.GetVar(baseName + "BuoyYellow@green", mYellowBuoy.mGreen);
    mXmlConf.GetVar(baseName + "BuoyYellow@blue", mYellowBuoy.mBlue);
    mXmlConf.GetVar(baseName + "BuoyYellowThresh@red", mYellowBuoy.mRedRange);
    mXmlConf.GetVar(baseName + "BuoyYellowThresh@green", mYellowBuoy.mGreenRange);
    mXmlConf.GetVar(baseName + "BuoyYellowThresh@blue", mYellowBuoy.mBlueRange);

    mXmlConf.GetVar(baseName + "BuoyBlue@red", mBlueBuoy.mRed);
    mXmlConf.GetVar(baseName + "BuoyBlue@green", mBlueBuoy.mGreen);
    mXmlConf.GetVar(baseName + "BuoyBlue@blue", mBlueBuoy.mBlue);
    mXmlConf.GetVar(baseName + "BuoyBlueThresh@red", mBlueBuoy.mRedRange);
    mXmlConf.GetVar(baseName + "BuoyBlueThresh@green", mBlueBuoy.mGreenRange);
    mXmlConf.GetVar(baseName + "BuoyBlueThresh@blue", mBlueBuoy.mBlueRange);
    mXmlConf.GetVar(baseName + "BlueBuoyExit@min_size", mBlueExitMinSize);
    mXmlConf.GetVar(baseName + "BlueBuoyExit@debounce", mBlueDebounceLimit);

    mXmlConf.GetVar(baseName + "BuoySegmentArea@max", mMaxPixelArea);
    mXmlConf.GetVar(baseName + "BuoySegmentArea@min", mMinPixelArea);
    mXmlConf.GetVar(baseName + "RedBuoySegmentArea@min", mMinRedPixelArea);
    mXmlConf.GetVar(baseName + "GreenBuoySegmentArea@min", mMinGreenPixelArea);
    mXmlConf.GetVar(baseName + "YellowBuoySegmentArea@min", mMinYellowPixelArea);
    mXmlConf.GetVar(baseName + "BlueBuoySegmentArea@min", mMinBluePixelArea);

    mXmlConf.GetVar(baseName + "BuoyBuffers@left_limit", mLeftI);           ///< Left pixel value for channel nav.
    mXmlConf.GetVar(baseName + "BuoyBuffers@right_limit", mRightI);         ///< Right pixel value for channel nav.
    mXmlConf.GetVar(baseName + "BuoyBuffers@lowest_row", mMaxJ);            ///< Max value of J before we consider we have passed the gates.

    mXmlConf.GetVar(baseName + "PixelScale@numerator", mPixelScaleNum);
    mXmlConf.GetVar(baseName + "PixelScale@denominator", mPixelScaleDenom);
    mPixelScale = mPixelScaleNum / mPixelScaleDenom;

    mXmlConf.GetVar(baseName + "Timer@new_state", mNewStateDuration);

    mXmlConf.GetVar(baseName + "YellowTolerances@center", mYellowBuoyCenter);
    mXmlConf.GetVar(baseName + "YellowTolerances@heading_offset", mYellowOffset);

    mYellowOffset = CxUtils::CxToRadians(mYellowOffset);
    double zone1, zone2;
    if(mXmlConf.GetVar(baseName + "ExitHeading@zone_one_value_degrees", zone1, true) &&
       mXmlConf.GetVar(baseName + "ExitHeading@zone_two_value_degrees", zone2, true))
    {
        if(this->GetZoneNumber() == 0)
        {
            mExitHeading = CxUtils::CxToRadians(zone1);
        }
        else if(this->GetZoneNumber() == 1)
        {
            mExitHeading = CxUtils::CxToRadians(zone2);
        }
        else
        {
            mExitHeading = mMapPose.Yaw();
        }
    }
    else
    {
        mExitHeading = mMapPose.Yaw();
    }
}


/** Draws maps for display purposes */
void BuoyNavCamera::DrawMaps()
{
    MapBuilder::DrawMaps();

    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }
    Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                             mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);
    double vLength = 0, vWidth = 0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);

    Cartographer::Object::Collisions collisions;
    Cartographer::Segment3D beam;
    beam.mPoint2(mBeamLengthMeters, 0, 0);
    beam.mWidth = vWidth/2.0;

    this->mpGlobalMap->ScanLocalSegmentInXY(mMapPose, beam, &collisions);

    Draw(this->mNavigator.GetLocalMapImage(GlobalMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         beam);

    Cartographer::Object::Collisions::iterator collision;
    for(collision = collisions.begin();
        collision != collisions.end();
        collision++)
    {
        Draw(this->mNavigator.GetLocalMapImage(GlobalMap),
             this->mDrawResolution,
             bottomLeftCorner,
             (*collision)->GetOrigin(),
             0.25,
             CV_RGB(255, 0, 0));
    }

    Cartographer::Segment3D heading;
    heading.mWidth = .1;
    heading.mPoint2(2.0, 0, 0);
    heading.mPoint2 = heading.mPoint2.Rotate(-CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), mDesiredHeading), Cartographer::Point3D::Z, true);

    Draw(this->mNavigator.GetLocalMapImage(GlobalMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         heading,
         CV_RGB(255, 255, 255));

    // Draw vehicle on top of everything.
    mMapPose.Draw(mNavigator.GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}


/*  End of File */


