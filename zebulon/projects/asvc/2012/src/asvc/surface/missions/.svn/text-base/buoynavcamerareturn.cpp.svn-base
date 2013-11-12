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
#include "asvc/surface/missions/buoynavcamerareturn.h"
#include <cartographer/objects/line.h>
#include <statemachine/utility/utility.h>
#include <algorithm>
#include <limits>
//#define SAVEIMAGES
using namespace Zebulon;
using namespace ASVC;
using namespace Cartographer;

const std::string BuoyNavCameraReturn::Name = "BuoyNavCameraReturn";
//const std::string BuoyNavCamera::ReturnName = "ReturnBuoyNavCamera";

const double FOUND_SIZE = 0.5;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNavCameraReturn::BuoyNavCameraReturn()
{

    std::string name = Name;
    mTrailName= "BuoyChannel";
//    if(mReturnFlag)
//    {
//        name = ReturnName;
//    }
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
    mTrailName = "BuoyChannel";

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BuoyNavCameraReturn::~BuoyNavCameraReturn()
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
int BuoyNavCameraReturn::Init()
{
    int result = 0;

    std::string name = Name;
//    if(mReturnFlag)
//    {
//        name = ReturnName;
//    }

    Mission::Init();
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
//        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(1);
//        CxUtils::SleepMs(500);
//        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(0);
    }
    mCurrentState = Find;

    if(mpGlobalInfo->GetSimpleTrail(mTrailName,mTrail))
    {
        mWPCountdown = mTrail.mTrail.size()-1;
    }
    InitializeMaps(Mission::mpGlobalInfo, mXmlConf, name);

    for(unsigned int i = 0; i < (unsigned int)mZones.size(); i++)
    {
        mSweepDir[i] = 1;
    }

    LoadSettings();

    mBuoyCount = mMaxBuoyCount;

    mRedDebounce.Initialize(mVisionDebounceLimit, false);
    mGreenDebounce.Initialize(mVisionDebounceLimit, false);
    mYellowDebounce.Initialize(4, false);
    mBlueDebounce.Initialize(5, false);

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
int BuoyNavCameraReturn::ErrorCheck()
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
int BuoyNavCameraReturn::FetchProcess()
{
    int result = 0;

    // Get new parameters from XML if anything changes.
    LoadSettings();

    // Get main camera image.
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mInitFrame);
    mCurrentPosition = Mission::mpGlobalInfo->GetPosition();
    // Update our maps (raw laser data, and object detection).
    UpdateMaps(true);

    // Run vision classification
    //this->RunVisionClassifier();
    this->RunDvisionClassifier();
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

double BuoyNavCameraReturn::AngleToBuoy(const Zebulon::Vision::Return& buoy)
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
std::string BuoyNavCameraReturn::ExitEarly()
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
std::string BuoyNavCameraReturn::Execute()
{
    // Load any settings for dynamic changes.
    LoadSettings();
    double travelSpeed = mTravelSpeed;
    double farbuoyoffsetangle = CxUtils::CxToRadians(20);
    mPrevState = mCurrentState;
    Zebulon::Vision::Return centerheading;
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

    //this->mpGlobalCommand->SetWaterCannonTilt(75);
    if(mGlobalExitTime.TimeElapsed() >= mGlobalMinDuration)
    {
        mState = ExitHeading;
    }

    std::cout << "================================================" << std::endl;
    std::cout << GetName() << std::endl;
    std::cout << "State: " << ToString((State)mState) << std::endl;
    GlobalInfo::SimpleTrail trail;
    if(mpGlobalInfo->GetSimpleTrail(mTrailName,trail))
    {
        std::cout << "Current WP: " << mWPCountdown << std::endl;
    }

    GlobalInfo::SimplePose currentWP;
    if(mWPCountdown >= 0)
    {
        currentWP = mTrail.mTrail[mWPCountdown];
        if(CxUtils::Utm::Distance2D(currentWP.mPosition,mCurrentPosition) < 5)
        {
            if(mWPCountdown == 0)
            {
                Utility::Display::CloseAllWindows();
                return mNextMission;
            }
            //if(mLockHeadingTimer.IsFinished() && !mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
            //{
                mWPCountdown--;
            //}
            currentWP = mTrail.mTrail[mWPCountdown];
        }
        if(mLockHeadingTimer.IsFinished() && !mRedDebounce.GetDebounced() && !mGreenDebounce.GetDebounced())
        {
            mDesiredHeading = CxUtils::Orientation::GetGlobalAngle(mCurrentPosition,
                                                currentWP.mPosition);
        }
    }
    else
    {
        mWPCountdown = 0;
    }


    double disttoignorebuoym = 10;
    switch (mState)
    {
        // Figure out combination of buoys found.
        case Find:

            if (mRedDebounce.GetDebounced() && mGreenDebounce.GetDebounced())
            {
                //red on correct side of green
                if(mRed.mCenterI>mGreen.mCenterI)
                {
                    //if(CxUtils::Utm::Distance(mCurrentPosition,currentWP.mPosition) < disttoignorebuoym)
                    {
                        mState=GreenRed;
                    }
                }
                //red on wrong side of green
                else
                {
                    mState = RedGreen;
                }
            }
            else if (mRedDebounce.GetDebounced())
            {

                //if(CxUtils::Utm::Distance(mCurrentPosition,currentWP.mPosition) < disttoignorebuoym)
                {
                    mState = Red;
                }
            }
            else if (mGreenDebounce.GetDebounced())
            {
                //if(CxUtils::Utm::Distance(mCurrentPosition,currentWP.mPosition) < disttoignorebuoym)
                {
                    mState = Green;
                }
            }
//            else
//            {
//                mState = NextWaypoint;
//            }
            break;
        //bouys on wrong sides, choose shallowest angle and head there.
        //we assume maybe the closest one to our current orientation is correct
        case RedGreen:
        {
            if (mGreenDebounce.GetDebounced() && mRedDebounce.GetDebounced())
            {
                double angletogreen = AngleToBuoy(mGreen);
                double angletored = AngleToBuoy(mRed);

                CxUtils::Orientation::AddToAngle(farbuoyoffsetangle, angletogreen);
                CxUtils::Orientation::AddToAngle(-farbuoyoffsetangle, angletored);
                double preferredangle = 0;

                preferredangle = fabs(angletogreen) < fabs(angletored)? angletogreen:angletored;

                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),preferredangle);
            }
            else
            {
                mState = Find;
            }
            break;
        }

        // We see both the Red and Green Buoys!
        case GreenRed:
            if (mRedDebounce.GetDebounced() && mGreenDebounce.GetDebounced())
            {
                // Find center between green and red
                mCenterI = (mGreen.mCenterI + mRed.mCenterI)/2;
                mCenterJ = (mGreen.mCenterJ + mRed.mCenterJ)/2;
                centerheading.mCenterI = mCenterI;
                centerheading.mCenterJ = mCenterJ;


                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                                                                      AngleToBuoy(centerheading));



                cvDrawLine(mFinalFrame,cvPoint(0,mGreen.mMaxJ),cvPoint(mFinalFrame->width,mGreen.mMaxJ),CV_RGB(0,255,0));
                cvDrawLine(mFinalFrame,cvPoint(0,mRed.mMaxJ),cvPoint(mFinalFrame->width,mRed.mMaxJ),CV_RGB(255,0,0));
                if ((mRed.mMaxJ > mMaxJ) || (mGreen.mMaxJ > mMaxJ))
                {
                    // Push this point to our trail of visited points.
                    //Mission::mpGlobalInfo->PushCurrentPositionToTrail();

                    if(mLockHeadingTimer.IsFinished())
                    {
                        mState = LockHeading;
                    }
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
                double angle = CxUtils::Orientation::AddToAngle(AngleToBuoy(mRed),-farbuoyoffsetangle) ;
                //double closeness = mRed.mCenterJ/mInitFrame->height;
                cvDrawLine(mFinalFrame,cvPoint(0,mRed.mMaxJ),cvPoint(mFinalFrame->width,mRed.mMaxJ),CV_RGB(255,0,0));
                if(angle > CxUtils::CxToRadians(5) && mRed.mMaxJ > mMaxJ) angle = 0;
                //closeness *= closeness;

                //std::cout << "red nearness percent: " << closeness << std::endl;
                //mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
                //                                                   CxUtils::CxToRadians((mRed.mCenterI  - mLeftI)*mPixelScale));

                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),angle);


                if (mRed.mMaxJ > mMaxJ)
                {

                    if(mLockHeadingTimer.IsFinished())
                    {
                        mState = LockHeading;
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
                double angle = CxUtils::Orientation::AddToAngle(AngleToBuoy(mGreen),farbuoyoffsetangle) ;

                //double closeness = mGreen.mCenterJ/mInitFrame->height;

                cvDrawLine(mFinalFrame,cvPoint(0,mGreen.mMaxJ),cvPoint(mFinalFrame->width,mGreen.mMaxJ),CV_RGB(0,255,0));
                if(angle < CxUtils::CxToRadians(5) && mGreen.mMaxJ > mMaxJ) angle = 0;
                //closeness *= closeness;
                //std::cout << "green nearness percent: " << closeness << std::endl;
//                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
//                                                                   CxUtils::CxToRadians((mGreen.mCenterI- mFinalFrame->width + mRightI)*mPixelScale));

               // mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),
               //                                                CxUtils::Orientation::AddToAngle(CxUtils::CxToRadians(-45*closeness),angle));
                mDesiredHeading = CxUtils::Orientation::AddToAngle(mMapPose.Yaw(),angle);

                if (mGreen.mMaxJ > mMaxJ)
                {
                    // Push this point to our trail of visited points.
                    //Mission::mpGlobalInfo->PushCurrentPositionToTrail();
                    if(mLockHeadingTimer.IsFinished())
                    {
                        mState = LockHeading;
                    }
                }
            }
            else
            {
                mState = Find;
            }
            break;

        case Yellow:
            mState = Find;
            break;
        case Blue:
            mState = Find;
            break;
        case Search:
            mState = Find;
        break;
        // Lay in the course, Make it so (in Captain Picard's voice)
        case LockHeading:
            if(mLockHeadingTimer.IsFinished())
            {
                mState = Find;
                //mWPCountdown--;
                break;
            }
            if(mLockHeadingTimer.IsFinished())
            {
                mLockHeadingTimer.Initialize(mLockHeadingDuration);
                mLockHeadingTimer.Start();
            }
            break;
       case ExitHeading:
            {
                mWPCountdown = 0;
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

    //this->DisplayMaps(MapBuilder::GlobalMap);
    this->DisplayMaps(MapBuilder::ObjectMap);
    //Utility::Display::DisplayImage(GlobalInfo::MainCamera, mInitFrame);
    Utility::Display::DisplayImage("Final Frame", mFinalFrame);

#ifdef SAVEIMAGES
    static int frameNumber = 0;
    std::stringstream fname;
    std::stringstream fname2;                    // Output filename for image
    fname << "logs/vision/cam/" << "buoydowncam";
    fname << std::setfill('0') << std::setw(8) << frameNumber << ".png";
    cvSaveImage(fname.str().c_str(),mFinalFrame);

    fname2 << "logs/vision/map/" << "buoydownmap";
    fname2 << std::setfill('0') << std::setw(8) << frameNumber << ".png";
    cvSaveImage(fname2.str().c_str(),mpNavigator->GetLocalMapImage(ObjectMap));
    frameNumber++;
#endif
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
void BuoyNavCameraReturn::UpdateMaps(const bool clearMaps)
{
    MapBuilder::UpdateMaps(clearMaps);
    // Add content to maps/filter using computer vision data.
}

void BuoyNavCameraReturn::RunDvisionClassifier()
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

    mRedBuoyClassifier.GetVisionReturn(copyinit,mRed,mFinalFrame);
    mYellowBuoyClassifier.GetVisionReturn(copyinit,mYellow,mFinalFrame);
    mGreenBuoyClassifier.GetVisionReturn(copyinit,mGreen,mFinalFrame);
    mBlueBuoyClassifier.GetVisionReturn(copyinit,mBlue,mFinalFrame);
    cvDrawLine(mFinalFrame,cvPoint(0,(int)mMaxJ),cvPoint(mFinalFrame->width,(int)mMaxJ),CV_RGB(255,255,255));
    cvReleaseImage(&copyinit);

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

/** Runs GVision classification of buoys by colors. */
void BuoyNavCameraReturn::RunVisionClassifier()
{
    // If we haven't created an image for the final processed image do so
    if(mFinalFrame == NULL && mInitFrame != NULL)
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
    mBlueDebounce.Bounce(false && mBlue.mValid && mBlue.mArea > mMinPixelArea && mBlue.mArea < mMaxPixelArea);

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
void BuoyNavCameraReturn::LoadSettings()
{

    mRedBuoyClassifier.Initialize("RedBuoy");
    mGreenBuoyClassifier.Initialize("GreenBuoy");
    mYellowBuoyClassifier.Initialize("YellowBuoy");
    mBlueBuoyClassifier.Initialize("BlueBuoy");

    std::string name = Name;
//    if(mReturnFlag)
//    {
//        name = ReturnName;
//    }
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
void BuoyNavCameraReturn::DrawMaps()
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

    Draw(this->mpNavigator->GetLocalMapImage(GlobalMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         beam);

    Cartographer::Object::Collisions::iterator collision;
    for(collision = collisions.begin();
        collision != collisions.end();
        collision++)
    {
        Draw(this->mpNavigator->GetLocalMapImage(GlobalMap),
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

    Draw(this->mpNavigator->GetLocalMapImage(GlobalMap),
         this->mDrawResolution,
         bottomLeftCorner,
         mMapPose,
         heading,
         CV_RGB(255, 255, 255));


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
            if(i == mWPCountdown)
            {
                marker.SetBuoyColor(Buoy::Blue);
            }
            else
            {
                marker.SetBuoyColor(Buoy::Green);
            }
            marker.SetRadius(1);

            this->DrawLocalObject(true, &marker, marker.GetDrawColor());
        }
        Buoy marker;
        marker.SetOrigin(mMapPose.ConvertFromMapToLocalCoordinates(mMapPose,
                                                                   this->mpGlobalMap->ConvertFromGlobalToMapCoordinates(mGuessPose.mPosition)));

        marker.SetBuoyColor(Buoy::Red);
        marker.SetRadius(1);

        this->DrawLocalObject(true, &marker, marker.GetDrawColor());
    }

    // Draw vehicle on top of everything.
    mMapPose.Draw(mpNavigator->GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);
//    bool Cartographer::Draw(IplImage* image,
//                            const double pixelsPerMeter,
//                            const Cartographer::Point3D& mapBottomCorner,
//                            const Cartographer::Segment3D& segment,
//                            const CvScalar color)
//    GlobalInfo::SimpleTrail trail;
//    if(mpGlobalInfo->GetSimpleTrail(mTrailName,trail))
//    {
//        GlobalInfo::SimplePose pose;
//        Cartographer::Pose blah;
//        Cartographer::Segment3D breadcrumb;
//        breadcrumb.mWidth = .1;
//        for(int i = 0; i < trail.mTrail.size(); i++)
//        {
//            pose = trail.mTrail[i];
//            breadcrumb = pose.Segment();
//            blah.mPosition = breadcrumb.mPoint1;
//            blah.mRotation.mZ = pose.mYawRads;
//            blah.Draw(mNavigator.GetLocalMapImage(GlobalMap),
//                          this->mDrawResolution,
//                          bottomLeftCorner,
//                          vLength,
//                          vWidth);
////            Cartographer::Draw(this->mNavigator.GetLocalMapImage(GlobalMap),
////                 this->mDrawResolution,
////                 bottomLeftCorner,
////                 breadcrumb,
////                 CV_RGB(128, 128, 128));


//        }
//    }
}


/*  End of File */

