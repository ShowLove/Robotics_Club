//////////////////////////////////////////////////////////////////////////////////////
///
/// \file buoynav.cpp
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
#include "asvc/surface/missions/buoynav.h"
#include <cartographer/objects/line.h>
#include <algorithm>
#include <limits>

using namespace Zebulon;
using namespace ASVC;
using namespace Cartographer;

const std::string BuoyNav::Name = "BuoyNav";
const std::string BuoyNav::ReturnName = "ReturnBuoyNav";

const double FOUND_SIZE = 0.5;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNav::BuoyNav(const bool returnNavFlag)
{
    mReturnFlag = returnNavFlag;

    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }
    ID = name;

    mCurrentState = SearchBuoys;
    mPrevState = mPrevState;
    mNextMission = "";
    mCurrentState = SearchBuoys;
    mBuoyCount = 0;
    mMaxBuoys = 10;
    mDebounceLimit = 10;
    mDriveThroughDistanceThresh = 0.5;
    mDriveTimeSeconds = 1;
    mTravelSpeed = 30;
    mSearchAngle = CxUtils::CxToRadians(90);
    mSearchDepth = 10.0;
    mTimeoutTimeSeconds = 120;
    mHaveBuoyVectorFlag = false;
    for(unsigned int i = 0; i < 3; i++)
    {
        mBuoyFlags.push_back(false);
        mBuoys.push_back(Buoy());
    }

    mSearchSweepSpanDegrees = 60;
    mSearchSweepSpeed = 20;
    mSearchForwardTime = 5;
    mSearchForwardSpeed = 25;

    // Init vision stuff.
    mSegment = new Vision::Segment();
    mThreshold = new Vision::ThreshRatio();

    mInitFrame=NULL;
    mSegFrame=NULL;
    mThreshFrame=NULL;
    mFinalFrame=NULL;

    mVisionDebounceLimit = 3;
    mBuoyMatchDistance = 1.25;

    mMinPixelArea = 45;
    mMaxPixelArea = 90000;

    mExitHeading = CxUtils::CxToRadians(210);

    SetBuoyDrawColors();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNav::~BuoyNav()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int BuoyNav::Init()
{
    int result = 0;

    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }

    Mission::Init();

    mCurrentState = SearchBuoys;
    mBuoyCount = 0;

    std::string baseName = std::string("Surface.") + name + ".";
    std::string homoFile;
    mXmlConf.GetVar(baseName + "Homography", homoFile);
    this->mHomography.LoadSettings(homoFile);

    LoadSettings();

    mDebounce.Initialize(mDebounceLimit, false);
    mRedDebounce.Initialize(mVisionDebounceLimit, false);
    mGreenDebounce.Initialize(mVisionDebounceLimit, false);
    mYellowDebounce.Initialize(mVisionDebounceLimit, false);
    mBlueDebounce.Initialize(mVisionDebounceLimit, false);

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
int BuoyNav::ErrorCheck()
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
int BuoyNav::FetchProcess()
{
    int result = 0;
    // Reset flags
    std::vector<bool>::iterator flag;
    for(flag = mBuoyFlags.begin(); flag != mBuoyFlags.end(); flag++)
    {
        *flag = false;
    }

    // Get new parameters from XML if anything changes.
    LoadSettings();

    // Get main camera image.
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mInitFrame);

    // Update our maps (raw laser data, and object detection).
    UpdateMaps(true);

    // Run vision classification
    this->RunVisionClassifier();

    // Find the closest buoys in the maps and assign
    // left and right colors based on data form vision and laser
    FindClosestBuoys();

    // Make sure we draw stuff we find in the right color.
    SetBuoyDrawColors();

    // Set the computer light to even/odd zone.
    // Zones we are not in get cleared of objects (not raw data).
    if(GetZoneNumber() >= 0)
    {
        // Indexing starts at 0
//        if(GetZoneNumber()%2)
//        {
//            mpGlobalCommand->SetComputerLightOne(true);
//            mpGlobalCommand->SetComputerLightTwo(false);

//        }
//        else
//        {
//            mpGlobalCommand->SetComputerLightOne(false);
//            mpGlobalCommand->SetComputerLightTwo(true);
//        }
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
std::string BuoyNav::ExitEarly()
{
    if((this->mTimeoutStopWatch.IsInitialized() && this->mTimeoutStopWatch.IsFinished())
        || mBuoyCount >= mMaxBuoys)
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
std::string BuoyNav::Execute()
{
    double travelSpeed = mTravelSpeed;
    double heading = mMapPose.mRotation.mZ;

    //this->mpGlobalCommand->SetWaterCannonTilt(75);
    // Reset flags
    this->mHaveBuoyVectorFlag = false;

    switch(mCurrentState)
    {
    case SearchBuoys:
        {
            std::cout << "=================================================" << std::endl;
            std::cout << "State: Searching for Buoys" << std::endl;

            if(mPrevState != SearchBuoys)
            {
                mSweepSearch.InitSearch(CxUtils::CxToDegrees(mMapPose.Yaw()),
                                        this->mSearchSweepSpanDegrees,
                                        this->mSearchSweepSpeed,
                                        this->mSearchForwardTime,
                                        this->mSearchForwardSpeed);
            }

            if(mBuoyFlags[Left] && mBuoyFlags[Right])
            {
                if(mDebounce.Bounce(true))
                {
                    mCurrentState = FoundBuoys;
                    mDebounce.Initialize(mDebounceLimit, false);
                    heading = mMapPose.Yaw();
                }
            }
            else
            {
                // Must convert from degrees to radians.
                mSweepSearch.SearchStep(heading, travelSpeed);
                heading = CxUtils::CxToRadians(heading);
            }
        }
        break;
    case FoundBuoys:
        {
            std::cout << "=================================================" << std::endl;
            std::cout << "State: Found Buoys" << std::endl;
            if(mBuoyFlags[Left] && mBuoyFlags[Right])
            {
                this->mHaveBuoyVectorFlag = true;
                this->mBuoyMidpoint = Point3D::Midpoint(mBuoys[Left].GetOrigin(), mBuoys[Right].GetOrigin());
                // Buoy location is relative to boat at this point.
                double distanceToMidpoint = mBuoyMidpoint.Distance();
                std::cout << "Distance to buoy pair: " << distanceToMidpoint << " required [" << mDriveThroughDistanceThresh << "]\n";
                if(distanceToMidpoint <= mDriveThroughDistanceThresh)
                {
                    mCurrentState = DriveThroughBuoys;
                }
                else
                {
                    // Pick new speed and heading.
                    double midpointAngle = atan2(mBuoyMidpoint.mY, mBuoyMidpoint.mX);
                    std::cout << "Angle to Channel Center: " << CxUtils::CxToDegrees(midpointAngle) << std::endl;
                    std::cout << "Distance to Channel Center: " << distanceToMidpoint << std::endl;
                    heading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(), midpointAngle);
                }
            }
            else
            {
                this->mHaveBuoyVectorFlag = false;
                mCurrentState = SearchBuoys;
            }

        }
        break;
    default:    // Drive through
        {
            std::cout << "=================================================" << std::endl;
            std::cout << "State: Drive Through Buoys" << std::endl;

            // Reset debouncers...
            mDebounce.Initialize(mDebounceLimit, false);
            mRedDebounce.Initialize(mVisionDebounceLimit, false);
            mGreenDebounce.Initialize(mVisionDebounceLimit, false);
            mYellowDebounce.Initialize(mVisionDebounceLimit, false);
            mBlueDebounce.Initialize(mVisionDebounceLimit, false);

            if(mDrivingStopWatch.IsStarted() == false)
            {
                mDrivingStopWatch.Initialize(mDriveTimeSeconds);
                mDrivingStopWatch.Start();
            }
            if(mDrivingStopWatch.IsFinished())
            {
                mCurrentState = SearchBuoys;
                mBuoyCount++; // Went through gate.
            }

            if(CxUtils::Orientation::AngleDiff(mMapPose.Yaw(), heading) >= CxUtils::CxToRadians(45))
            {
                travelSpeed = mTravelSpeed/2.0;
            }
            std::cout << "Time remaining: " << mDriveTimeSeconds - mDrivingStopWatch.TimeElapsed() << " s\n";
        }
        break;
    }

    mPrevState = mCurrentState;

    mpGlobalCommand->SetForwardVelocityPercent(travelSpeed);
    mpGlobalCommand->SetYaw(heading);
    std::cout << "Travel Speed: " << travelSpeed << std::endl;
    std::cout << "Desired Heading: "
              << CxUtils::CxToDegrees(heading)
              << " Actual: " << CxUtils::CxToDegrees(mMapPose.Yaw())<< std::endl;
    std::cout << "Buoys Traversed : " << mBuoyCount << std::endl;
    std::cout << "Vehicle Pitch: " << CxUtils::CxToDegrees(mMapPose.Pitch()) << std::endl;


    this->DrawMaps();
    this->DisplayMaps(MapBuilder::GlobalMap);
    this->DisplayMaps(MapBuilder::ObjectMap);
    //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    //Utility::Display::DisplayImage("Final Frame", mFinalFrame);

    if(mBuoyCount >= mMaxBuoys)
    {
        Utility::Display::CloseAllWindows();
        return mNextMission;
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
void BuoyNav::UpdateMaps(const bool clearMaps)
{
    MapBuilder::UpdateMaps(clearMaps);
    // Add content to maps/filter using computer vision data.
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Looks at the ObjectMap to find best candidates for buoys, and
///         then updates map drawings.
///
////////////////////////////////////////////////////////////////////////////////////
void BuoyNav::FindClosestBuoys()
{
    // Define the scan region in object map.
    Point3D upperLeft, upperRight, start, top;
    double vLength = 1.0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    start(-vLength/2.0, 0, 0);
    top(mSearchDepth, 0, 0);
    upperLeft(mSearchDepth, 0, 0);
    upperLeft = upperLeft.Rotate(-1*mSearchAngle/2.0, Point3D::Z);
    upperRight(upperLeft.mX, fabs(upperLeft.mY), 0.0);

    Object::Collisions collisions, collisions2;
    std::set<const Object*> allObjects;
    std::set<const Object*>::iterator obj;
    this->GetObjectMap()->ScanLocalTriangleInXY(mMapPose, start, upperLeft, top, &collisions);
    this->GetObjectMap()->ScanLocalTriangleInXY(mMapPose, start, top, upperRight, &collisions2);
    Object::Collisions::iterator object;

    // Add the secon scan.
    for(object = collisions.begin();
        object != collisions.end();
        object++)
    {
        allObjects.insert(*object);
    }
    for(object = collisions2.begin();
        object != collisions2.end();
        object++)
    {
        allObjects.insert(*object);
    }
    mBuoysFound.clear();
    obj = allObjects.begin();
    while(obj != allObjects.end())
    {
        const Buoy* buoy = dynamic_cast<const Buoy *>(*obj);
        if(buoy == NULL)
        {
            // Do nothing.
        }
        else
        {
            mBuoysFound.push_back(*buoy);
            // Change to relative to vehicle since collisions returned are in global coordinates.
            mBuoysFound[mBuoysFound.size() - 1].SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose, buoy->GetOrigin()));
        }
        obj++;
    }


    // Sort objects by distance to current pose.
    std::sort(mBuoysFound.begin(),
              mBuoysFound.end(),
              Buoy::Compare);

    // Try match found buoys with LIDAR to data found through vision classifications so
    // that the colors are matched correctly.

    // These can be functions.
    if(mRedDebounce.GetDebounced())
    {
        MatchBuoy(mRed, Buoy::Red, mBuoysFound);
    }
    if(mGreenDebounce.GetDebounced())
    {
        MatchBuoy(mGreen, Buoy::Green, mBuoysFound);
    }
    if(mYellowDebounce.GetDebounced())
    {
        MatchBuoy(mYellow, Buoy::Yellow, mBuoysFound);
    }
    // Assign buoys to left and right.
    std::set<BuoyType> found;
    for(unsigned int i = 0; i < (unsigned int)mBuoysFound.size(); i++)
    {
        if(mBuoysFound[i].GetBuoyColor() != Buoy::Unknown)
        {
            BuoyType type = GetTypeFromColor(mBuoysFound[i].GetBuoyColor());
            // If we have already found a buoy of this type
            // see if it is closer and take the closest.
            if(mBuoyFlags[type])
            {
                if(mBuoys[type].GetOrigin().Distance() > mBuoysFound[i].GetOrigin().Distance())
                {
                    mBuoys[type] = mBuoysFound[i];
                }
            }
            else
            {
                mBuoyFlags[type] = true;
                mBuoys[type] = mBuoysFound[i];
            }
            found.insert(type);
        }
    }

    Buoy::List::iterator foundBuoy;
    if(found.find(Left) == found.end()) // Find the left buoy.
    {
        mBuoys[Left].SetOrigin(Point3D(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0));
        for(unsigned int i = 0; i < (unsigned int)mBuoysFound.size(); i++)
        {
            if(mBuoysFound[i].GetBuoyColor() == Buoy::Unknown)
            {
                if(mBuoysFound[i].GetOrigin().Distance() < mBuoys[Left].GetOrigin().Distance() &&
                   mBuoysFound[i].GetOrigin().mY < 0)
                {
                    mBuoys[Left] = mBuoysFound[i];
                    mBuoyFlags[Left] = true;
                }
            }
        }
    }
    if(found.find(Right) == found.end()) // Find the right buoy.
    {
        mBuoys[Right].SetOrigin(Point3D(std::numeric_limits<double>::max(), std::numeric_limits<double>::max(), 0));
        for(unsigned int i = 0; i < (unsigned int)mBuoysFound.size(); i++)
        {
            if(mBuoysFound[i].GetBuoyColor() == Buoy::Unknown)
            {
                if(mBuoysFound[i].GetOrigin().Distance() < mBuoys[Right].GetOrigin().Distance() &&
                   mBuoysFound[i].GetOrigin().mY > 0)
                {
                    if(mBuoyFlags[Left] == false ||
                       mBuoys[Left].GetOrigin().Distance(mBuoysFound[i].GetOrigin()) > 0.01)
                    {
                        mBuoys[Right] = mBuoysFound[i];
                        mBuoyFlags[Right] = true;
                    }
                }
            }
        }
    }

}


/** Trys to match a given buoy classified in the image to
    data in the ground plane. **/
void BuoyNav::MatchBuoy(const Vision::Return& buoy,
                        const Buoy::Color color,
                        Buoy::List& buoysFound)
{
    Point3D::List polygon;
    Point3D vertex;

    vertex = this->mHomography.FromImageToGround(Point3D(buoy.mMinI, buoy.mMinJ, 0), mInitFrame);
    polygon.push_back(vertex);

    vertex = this->mHomography.FromImageToGround(Point3D(buoy.mMaxI, buoy.mMinJ, 0), mInitFrame);
    polygon.push_back(vertex);

    vertex = this->mHomography.FromImageToGround(Point3D(buoy.mMaxI, buoy.mMaxJ, 0), mInitFrame);
    polygon.push_back(vertex);

    vertex = this->mHomography.FromImageToGround(Point3D(buoy.mMinI, buoy.mMaxJ, 0), mInitFrame);
    polygon.push_back(vertex);

    vertex = this->mHomography.FromImageToGround(Point3D((buoy.mMaxI + buoy.mMinI)/2.0, buoy.mMaxJ, 0), mInitFrame);

    int matched = -1;
    double minDistance = std::numeric_limits<double>::max();
    for(unsigned int i = 0; i < (unsigned int)buoysFound.size(); i++)
    {
        if(buoysFound[i].GetBuoyColor() == Buoy::Unknown)
        {
            // Try determine the best match within reason.

            double distance = vertex.Distance(buoysFound[i].GetOrigin());
            if( (distance < mBuoyMatchDistance && distance < minDistance) ||
                    buoysFound[i].GetOrigin().IsInside2D(polygon))
            {
                matched = (int)i;
                minDistance = distance;
            }
        }
    }
    if(matched >= 0)
    {
        buoysFound[matched].SetBuoyColor(color);
    }
    else
    {
    }
}


/** Runs GVision classification of buoys by colors. */
void BuoyNav::RunVisionClassifier()
{
    // If we haven't created an image for the final processed image do so
    if(mFinalFrame == NULL)
    {
        mFinalFrame = cvCreateImage(cvSize(mInitFrame->width,mInitFrame->height), IPL_DEPTH_8U, 3);
        std::cout << "Created Frame: " << mFinalFrame->width << "x " << mFinalFrame->height <<std::endl;
    }
    // set the entire image to black
    Vision::VisionUtil::ResetImage(mFinalFrame);

    // Green Buoy
    // Setup Segment for green
    mSegment->Setup(mGreenBuoy, 50);
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
    mSegment->Setup(mRedBuoy, 50);
    // Setup threshold for Red
    mThreshold->Setup(mRedBuoy);
    // segment/threshold forward for Red
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // get red return values
    mSegment->GetMaxPosition(mRed);
    //Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    // Yellow Buoy
    // Setup Segment for Yellow
    mSegment->Setup(mYellowBuoy, 100);
    // Setup threshold for Yellow
    mThreshold->Setup(mYellowBuoy);
    // segment/threshold forward for Yellow
    mThreshold->Process(mInitFrame, mThreshFrame);
    mSegment->Process(mThreshFrame, mSegFrame);
    // get red return values
    mSegment->GetMaxPosition(mYellow);
    //Copy green into final frame
    Vision::VisionUtil::CombineImages(mSegFrame, mFinalFrame);

    mRedDebounce.Bounce(mRed.mValid && mRed.mArea > mMinPixelArea && mRed.mArea < mMaxPixelArea);
    mGreenDebounce.Bounce(mGreen.mValid && mGreen.mArea > mMinPixelArea && mGreen.mArea < mMaxPixelArea);
    mYellowDebounce.Bounce(mYellow.mValid && mYellow.mArea > mMinPixelArea && mYellow.mArea < mMaxPixelArea);

}


/** Loads settings from XML file. */
void BuoyNav::LoadSettings()
{
    std::string name = Name;
    if(mReturnFlag)
    {
        name = ReturnName;
    }
    std::string baseName = std::string("Surface.") + name + ".";
    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);
    mXmlConf.GetVar(baseName + "BuoyDebounceLimit", mDebounceLimit);
    mXmlConf.GetVar(baseName + "BuoyDriveThroughDistanceThresh", mDriveThroughDistanceThresh);
    mXmlConf.GetVar(baseName + "BuoyDriveThroughTimeSeconds", mDriveTimeSeconds);
    mXmlConf.GetVar(baseName + "TravelSpeed", mTravelSpeed);
    mXmlConf.GetVar(baseName + "SearchAngleDegrees", mSearchAngle);
    mSearchAngle = CxUtils::CxToRadians(mSearchAngle);
    mXmlConf.GetVar(baseName + "SearchDepthMeters", mSearchDepth);
    mXmlConf.GetVar(baseName + "SweepSearch@sweep_degrees", mSearchSweepSpanDegrees);
    mXmlConf.GetVar(baseName + "SweepSearch@sweep_speed", mSearchSweepSpeed);
    mXmlConf.GetVar(baseName + "SweepSearch@time_forward", mSearchForwardTime);
    mXmlConf.GetVar(baseName + "SweepSearch@forward_speed", mSearchForwardSpeed);

    std::string missionTimeString;
    mXmlConf.GetVar(baseName + "MissionTime", missionTimeString);
    CxUtils::Time timeToRun;
    timeToRun.FromString(missionTimeString);
    mTimeoutTimeSeconds = (unsigned int)timeToRun.ToSeconds();

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

    mXmlConf.GetVar(baseName + "BuoySegmentArea@max", mMaxPixelArea);
    mXmlConf.GetVar(baseName + "BuoySegmentArea@min", mMinPixelArea);

    mXmlConf.GetVar(baseName + "BuoyMatchDistanceMeters", this->mBuoyMatchDistance);

    if(mXmlConf.GetVar(baseName + "ExitHeading@value_degrees", mExitHeading, true))
    {
        mExitHeading = CxUtils::CxToRadians(mExitHeading);
    }
    else
    {
        mExitHeading = mMapPose.Yaw();
    }

}


/** Sets the appropriate draw colors for found Buoys. */
void BuoyNav::SetBuoyDrawColors()
{
    if(mReturnFlag == false) // Leaving dock.
    {
        mBuoys[Left].SetBuoyColor(Buoy::Red);
        mBuoys[Right].SetBuoyColor(Buoy::Green);
    }
    else // Returning to dock.
    {
        mBuoys[Left].SetBuoyColor(Buoy::Green);
        mBuoys[Right].SetBuoyColor(Buoy::Red);
    }
    mBuoys[Bad].SetBuoyColor(Buoy::Yellow);

    for(unsigned int i = 0; i < (unsigned int)mBuoys.size(); i++)
    {
        mBuoys[i].SetRadius(FOUND_SIZE);
    }
}

/** Draws maps for display purposes */
void BuoyNav::DrawMaps()
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

    // Draw scan area in map.

    Draw(mpNavigator->GetLocalMapImage(ObjectMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         start,
         upperLeft,
         top);

    Draw(mpNavigator->GetLocalMapImage(ObjectMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         start,
         top,
         upperRight);

    // Draw found buoys.
    for(unsigned int i = 0; i < (unsigned int)mBuoysFound.size(); i++)
    {
        // Draw buoy
        mBuoysFound[i].SetRadius(0.30);
        this->DrawLocalObject(true, &mBuoysFound[i], mBuoysFound[i].GetDrawColor());
    }
    // Draw classified buoys

    for(unsigned int i = 0; i < (unsigned int)mBuoys.size(); i++)
    {
        if(mBuoyFlags[i])
        {
            // Draw buoy
            this->DrawLocalObject(true, &mBuoys[i], mBuoys[i].GetDrawColor());
            /*
            double range = .788;
            CvPoint p1 = cvPoint(mInitFrame->width/2.0 + mBuoys[i].GetOrigin().mY*(mInitFrame->width/2.0)/range, 0);
            CvPoint p2 = cvPoint(p1.x, mInitFrame->height - 1);
            cvLine(mInitFrame, p1, p2, mBuoys[i].GetDrawColor(), 30);
            */
        }
    }

    if(mHaveBuoyVectorFlag)
    {

        Segment3D vectorToMidpoint(Point3D(0, 0, 0), mBuoyMidpoint, vWidth/3.0);

        // Draw scan area in map.
        Draw(mpNavigator->GetLocalMapImage(ObjectMap),
             this->mDrawResolution,
             bottomLeftCorner,
             mMapPose,
             vectorToMidpoint,
             CV_RGB(125, 125, 255));
    }


    // Draw vehicle.
    mMapPose.Draw(mpNavigator->GetLocalMapImage(ObjectMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}

/*  End of File */

