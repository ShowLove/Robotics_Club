//////////////////////////////////////////////////////////////////////////////////////
///
/// \file cheatershand.cpp
/// \brief Mission identifies and shoots at a target (in this case a cheater's hand)
///
/// Author(s): Tuyen Do<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:nintwindo@knights.ucf.edu<br>
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
#include "asvc/surface/missions/cheatershand.h"
#include <cartographer/objects/line.h>
#include "asvc/surface/depthmap.h"
#include <algorithm>
#include <limits>

using namespace Zebulon;
using namespace ASVC;
using namespace Vision;
using namespace Cartographer;

const std::string CheatersHand::Name = "CheatersHand";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] returnNavFlag If true, we are returning to dock.
///
////////////////////////////////////////////////////////////////////////////////////
CheatersHand::CheatersHand(const bool returnNavFlag)
{
    ID = Name;
    mNextMission = "";
    mTimeoutTimeSeconds = 1000;
    mpMainCamera = mpDepthData = mpDepthImage = NULL;
    mCurrentState = ApproachTarget;
    mDesiredThrust = 0.0;
    mDesiredHeadingDeg = 0.0;
    mShorelineAverage = 0.0;
    mShorelineAngleDeg = 0.0;
    mNumberOfShorelinePts = 0;
    mMinPts = 100;
    mMinPtsForShore = 400;
    mShorelinePerpAngleDeg = 0.0;
    mDistanceToShoreline = 0.0;
    mDesiredLateral = 0.0;
    mDesiredLateralDir = 1.0;
    mScanLength = 10;
    mPrevGoodAngleDeg = 0.0;
    mSearchHeadingDir = -1.0;
    mSearchHeading = 0.0;
    mCoverageDistance = 12.0;
    mTargetThreshold = 1.0;
    mDesiredDistanceToShore = 3.0;
    mNumSweeps = 0;

    //mShorelineAngleFilter(1.0/20.0, 1.0);
    //mShorelineDistanceFilter(1.0/20.0, 1.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
CheatersHand::~CheatersHand()
{
    if(mpMainCamera)
    {
        cvReleaseImage(&mpMainCamera);
    }
    if(mpDepthData)
    {
        cvReleaseImage(&mpDepthData);
    }
    if(mpDepthImage)
    {
        cvReleaseImage(&mpDepthImage);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the mission.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int CheatersHand::Init()
{
    int result = 0;

    // Init network connection.
    if(mTcp.IsValid() == false)
    {
        mTcp.InitializeSocket("10.11.12.208", 5555);
    }

    Mission::Init();

    LoadSettings();
    if(mpGlobalInfo->GetPylonCamera())
    {
        mpGlobalInfo->GetPylonCamera()->LoadSettings("settings/pylon_settings.txt");
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(1);
        CxUtils::SleepMs(500);
        mpGlobalInfo->GetPylonCamera()->SetWhiteBalanceMode(0);
    }
    InitializeMaps(Mission::mpGlobalInfo,
                   mXmlConf,
                   Name);

    mTimeoutStopWatch.Initialize(mTimeoutTimeSeconds);
    mTimeoutStopWatch.Start();
    mLateralStopWatch.Initialize(80);
    mLeaveShoreStopWatch.Initialize(5);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Error Check method, does any required error checking.
///
///  \return 0 if no error, 1 if error.
///
////////////////////////////////////////////////////////////////////////////////////
int CheatersHand::ErrorCheck()
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
int CheatersHand::FetchProcess()
{
    int result = 0;

    // Get new parameters from XML if anything changes.
    LoadSettings();

    mWhite.Initialize("WhiteSquare");
    this->UpdateMaps(false);

    // Get current image
    mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mpMainCamera);
    mpGlobalInfo->GetInfo(GlobalInfo::DepthDataMainCamera, mpDepthData);
    mpGlobalInfo->GetInfo(GlobalInfo::DepthImageMainCamera, mpDepthImage);
    //GetBoxes(mpMainCamera,mpDepthImage,mpDepthData);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If there is an early exit condition, this method will handle it.
///
///  \return Mission::KeepRunning.
///
////////////////////////////////////////////////////////////////////////////////////
std::string CheatersHand::ExitEarly()
{
    if(this->mTimeoutStopWatch.IsInitialized() && this->mTimeoutStopWatch.IsFinished())
    {

        CxUtils::Wgs pos = mpGlobalInfo->GetPositionWgs();
        SendHotSuit(pos);

        // Make sure on exit of mission you release all of
        // your allocated memory!
        mpGlobalCommand->SetPump((false));
        if(mpMainCamera)
        {
            cvReleaseImage(&mpMainCamera);
            mpMainCamera = NULL;
        }
        if(mpDepthData)
        {
            cvReleaseImage(&mpDepthData);
            mpDepthData = NULL;
        }
        if(mpDepthImage)
        {
            cvReleaseImage(&mpDepthImage);
            mpDepthImage = NULL;
        }
        Utility::Display::CloseAllWindows();

        this->ReleaseMapBuilderMemory();
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
std::string CheatersHand::Execute()
{
    switch(mCurrentState)
    {
        case ApproachTarget:
        {
        std::cout << "In Approach Target: NumPts= " << mNumberOfShorelinePts << "\n";
            if(GetZoneNumber() == 0) //Course B (Left Course)
            {
                std::cout << "In Course B.\n";
                mDesiredHeadingDeg = mCourseBTargetHeading;
            }
            else
            {
                std::cout << "In Course A.\n";
                mDesiredHeadingDeg = mCourseATargetHeading;
            }

            if(fabs(CxUtils::Orientation::AngleDiff(CxUtils::CxToDegrees(mMapPose.Yaw()), mDesiredHeadingDeg, false)) < 10.0)
            {
                mDesiredThrust = 75;
            }
            else
            {
                mDesiredThrust = 0;
            }

            if(mNumberOfShorelinePts >= 400)
            {
                mCurrentState = FindTarget;
                //Save starting position
                mStartingPosition = mMapPose.mPosition; //Stored in map coordinates
            }

            mPrevGoodAngleDeg = mDesiredHeadingDeg;
            break;
        }

        case FindTarget:
        {
            std::cout << "In Find Target: NumPts= " << mNumberOfShorelinePts << " Scan Length= " << mScanLength << " Distance= " << mStartingPosition.Distance(mMapPose.mPosition) << std::endl;
            mDesiredDistanceToShore = 3.0;
            AimToShore();
            if(LinedUpWithShore())
            {
                //Roughly perpendicular and the right distance from shoreline
                //Allowed to lateral
                mDesiredLateral = 40*mDesiredLateralDir;
                mpGlobalCommand->SetPump(true);
                mDesiredThrust += 8;
                mDesiredHeadingDeg = CxUtils::Orientation::AddToAngle(mDesiredHeadingDeg, 4, false);
            }
            else
            {
                mpGlobalCommand->SetPump(false);
            }
            //Check distance from point to switch lateral directions
            if(mStartingPosition.Distance(mMapPose.mPosition) > mCoverageDistance)
            {
                 mNumSweeps++;
                 if(mNumSweeps > 2)
                 {
                     //Lateralled entire coverage, leave shore
                     mCurrentState = LeaveShore;
                 }
                 else
                 {
                     mDesiredLateralDir *= -1.0; //Now lateral to the right
                     mCoverageDistance = 20; //Change coverage distance to go on the otherside of the starting point
                     mStartingPosition = mMapPose.mPosition; //Set reference position to current pose
                 }
            }

            //Swtich to target missions
            //Exceed distance from last target before switching again
            if(mMapPose.mPosition.Distance(mTargetPose) > mTargetThreshold)
            {
                std::cout << "Allowed to swith to target.\n";
                //Do vision

                //Target switching
            }
            break;
        }

        case TargetHotSuit:
        {
            std::cout << "HotSuit: ";

            //Pass back to FindTarget
            break;
        }

        case TargetCheatersHand:
        {
            std::cout << "CheatersHand: ";

            //Pass back to FindTarget
            break;
        }

        case TargetButtonPress:
        {
            std::cout << "ButtonPress: ";

            //Pass back to FindTarget
            break;
        }

        case LeaveShore:
        {
            std::cout << "Leave shore\n";
            mpGlobalCommand->SetPump((false));
            if(!mLeaveShoreStopWatch.IsStarted()) mLeaveShoreStopWatch.Start();
            mDesiredLateral = 0.0;
            mDesiredThrust = -75.0;
            mDesiredHeadingDeg = CxUtils::CxToDegrees(mMapPose.Yaw());
            if(mLeaveShoreStopWatch.IsFinished())
            {

                CxUtils::Wgs pos = mpGlobalInfo->GetPositionWgs();
                SendHotSuit(pos);

                mpGlobalCommand->SetYawDegrees(mDesiredHeadingDeg);
                mpGlobalCommand->SetForwardVelocityPercent(0.0);
                mpGlobalCommand->SetLateralVelocityPercent(0.0);
                return mNextMission;
            }
            break;
        }
    }

    //Set outputs
    std::cout << "Desired: H= " << mDesiredHeadingDeg << " FV= " << mDesiredThrust << " LV= " << mDesiredLateral << std::endl;
    mpGlobalCommand->SetYawDegrees(mDesiredHeadingDeg);
    mpGlobalCommand->SetForwardVelocityPercent(mDesiredThrust);
    mpGlobalCommand->SetLateralVelocityPercent(mDesiredLateral);

    DrawDesiredVectors();
    this->DisplayMaps(MapBuilder::GlobalMap);

    Utility::Display::DisplayImage(GlobalInfo::MainCamera, mpMainCamera, 1);
    Utility::Display::DisplayImage(GlobalInfo::DepthImageMainCamera, mpDepthImage, 1);

    return Mission::KeepRunning;
}

bool CheatersHand::LinedUpWithShore(double distanceThreshold, double angleThreshold)
{
    bool result = true;
    result = result && mSeesShoreFlag; //Check if it sees shore
    result = result && (fabs(mDesiredDistanceToShore - mDistanceToShoreline) <= distanceThreshold); //check distance to shore
    result = result && (fabs(CxUtils::Orientation::AngleDiff(CxUtils::CxToDegrees(mMapPose.Yaw()), mShorelinePerpAngleDeg, false)) <= angleThreshold); //Check angle to shore
    return result;
}

//Sets forward thrust and headind to stay aimed at shore
void CheatersHand::AimToShore()
{
    //Sees valid number of points to set angle and distance to shore
    if(mNumberOfShorelinePts >= mMinPts)
    {
        mSeesShoreFlag = true;
        mScanLength -= 0.1;
        if(mScanLength <= 10.0)
        {
            mScanLength = 10.0;
        }
        mDesiredHeadingDeg = mShorelinePerpAngleDeg;
        double error = mDesiredDistanceToShore - mDistanceToShoreline;
        mDesiredThrust = -1.0*error*10.0;
        mPrevGoodAngleDeg = mDesiredHeadingDeg;
    }
    else //Not enough points increase search range and start sweeping to find shoreline again
    {
        mSeesShoreFlag = false;
        mDesiredLateral = 0.0;
        mSearchHeading += 1.0*mSearchHeadingDir;
        if(mSearchHeading > 50)
        {
            mSearchHeading = 50;
            mSearchHeadingDir *= -1.0;
        }
        else if(mSearchHeading < -50)
        {
            mSearchHeading = -50;
            mSearchHeadingDir *= -1.0;
        }
        std::cout << "SearchHeading= " << mSearchHeading << std::endl;
        mDesiredHeadingDeg = CxUtils::Orientation::AddToAngle(mPrevGoodAngleDeg, mSearchHeading, false);
        mDesiredThrust = 0.0;
        mScanLength += 0.1;
        if(mScanLength >= 20.0)
        {
            mScanLength = 20.0;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Updates map using information from LIDAR and any computer vision
///         data.
///
///  \param[in] clearMaps If true, old data is cleared.
///
////////////////////////////////////////////////////////////////////////////////////
void CheatersHand::UpdateMaps(const bool clearMaps)
{
    MapBuilder::UpdateMaps(clearMaps);
    // Add content to maps/filter using computer vision data.
    this->DrawMaps();
    Cartographer::Objects::Line test1(Cartographer::Segment3D(CxUtils::Point3D(0,0,0), CxUtils::Point3D(mScanLength,-(mScanLength+5),0), 0.1));
    Cartographer::Objects::Line test2(Cartographer::Segment3D(CxUtils::Point3D(0,0,0), CxUtils::Point3D(mScanLength,(mScanLength+5),0), 0.1));
    this->DrawLocalObject(false, &test1, CV_RGB(0, 0, 255));
    this->DrawLocalObject(false, &test2, CV_RGB(0, 0, 255));

    Object::Collisions collisions;
    mpGlobalMap->ScanLocalTriangleInXY(mMapPose, Point3D(0, 0, 0), Point3D(mScanLength, -(mScanLength+5), 0), Point3D(mScanLength, (mScanLength+5), 0), &collisions);
    Point3D::List pts;
    mNumberOfShorelinePts = collisions.size();
    if(mNumberOfShorelinePts >= mMinPts)
    {
        Object::Collisions::iterator it;
        for(it = collisions.begin(); it != collisions.end(); it++)
        {
            if((*it)->GetType() == Cartographer::Object::Line)
                continue;

            pts.push_back((*it)->GetOrigin());
            //this->DrawLocalObject(false, (*it)->ConvertFromMapToLocalCoordinates(mMapPose) , CV_RGB(255, 0, 0));
            this->DrawGlobalObject(false, (*it), CV_RGB(255, 0, 0));
        }
        Point3D centroid = GetCentroid(pts);
        double angle;
        angle = Point3D::GetLinearRegressionAngle(pts);
        //std::cout << "angle: " << CxUtils::CxToDegrees(angle) << " centroid: ";
        //centroid.Print();
        Objects::Sphere centroidObject(centroid, 0.5);
        this->DrawGlobalObject(false, &centroidObject, CV_RGB(0, 255, 0));

        // Half the length of the estimated shoreline
        double shoreLineLengh = 5;

        // Calculates the X offest
        double dX = shoreLineLengh * cos(angle);
        double dY = shoreLineLengh * sin(angle);
        Cartographer::Objects::Line shoreLine(Cartographer::Segment3D(CxUtils::Point3D(centroid.mX - dX, centroid.mY - dY, 0),
                                                                      CxUtils::Point3D(centroid.mX + dX, centroid.mY + dY, 0), 0.2));
        this->DrawGlobalObject(false, &shoreLine, CV_RGB(255, 255, 0));

        double distanceToShoreline = CxUtils::Segment3D::GetDistanceToPoint(mMapPose.mPosition, shoreLine.GetLine());
        CxUtils::Point3D closestShorelinePt = CxUtils::Segment3D::GetClosestPointOnSegment(mMapPose.mPosition, shoreLine.GetLine());
        Cartographer::Segment3D vToShore(mMapPose.mPosition, closestShorelinePt);
        double perpAngle = vToShore.GetAngleXY(); //::Orientation::AddToAngle(-vToShore.GetAngleXY(), -CxUtils::CX_HALF_PI);
        Cartographer::Objects::Line newHeading(Cartographer::Segment3D(mMapPose.mPosition,
                                                                       mMapPose.mPosition + CxUtils::Point3D(cos(perpAngle)*distanceToShoreline, sin(perpAngle)*distanceToShoreline, 0.0), 0.2));
        this->DrawGlobalObject(false, &newHeading, CV_RGB(0, 255, 0));
        perpAngle = -1*CxUtils::Orientation::AddToAngle(perpAngle, -CxUtils::CX_HALF_PI);
        mShorelinePerpAngleDeg = CxUtils::CxToDegrees(perpAngle);
        mDistanceToShoreline = distanceToShoreline;
    }
}


void CheatersHand::DrawDesiredVectors()
{
    double vectorMagnitude = 2.0;
    Cartographer::Objects::Line desiredVectorLine;


    // Draw lateral desired vector
    if(mDesiredLateral > 0.01)
    {
        vectorMagnitude = 4.0*mDesiredLateral/100.0;
    }
    else if(mDesiredLateral < 0.01)
    {
        vectorMagnitude = -4.0*mDesiredLateral/100.0;
    }
    else
    {
        vectorMagnitude = 0.0;
    }


    desiredVectorLine.GetLine().mPoint2(0, vectorMagnitude, 0);
    if(fabs(vectorMagnitude) > .01)
    {
        this->DrawLocalObject(false, &desiredVectorLine, cvScalar(255, 255, 255));
    }

    // Draw thrust desired vector
    if(mDesiredThrust > 0.01)
    {
        vectorMagnitude = 4.0*mDesiredLateral/100.0;
    }
    else if(mDesiredThrust < 0.01)
    {
        vectorMagnitude = -4.0*mDesiredLateral/100.0;
    }
    else
    {
        vectorMagnitude = 0.0;
    }

    desiredVectorLine.GetLine().mPoint2(vectorMagnitude, 0, 0);
    if(fabs(vectorMagnitude) > .01)
    {
        this->DrawLocalObject(false, &desiredVectorLine, cvScalar(255, 255, 255));
    }

}

Point3D CheatersHand::GetCentroid(Point3D::List pts)
{
    Point3D::List::iterator it;
    Point3D centroid(0, 0, 0);
    for(it = pts.begin(); it != pts.end(); it++)
    {
        centroid += (*it);
    }
    centroid /= pts.size();
    return centroid;
}


/** Loads settings from XML file. */
void CheatersHand::LoadSettings()
{
    std::string name = Name;

    std::string baseName = std::string("Surface.") + name + ".";
    mXmlConf.GetVar(baseName + "NextMission@name", mNextMission);

    std::string missionTimeString;
    mXmlConf.GetVar(baseName + "MissionTime", missionTimeString);
    CxUtils::Time timeToRun;
    timeToRun.FromString(missionTimeString);
    mTimeoutTimeSeconds = (unsigned int)timeToRun.ToSeconds();

    double latitude, longitude;
    mXmlConf.GetVar(baseName + "DestinationWaypoint@latitude", latitude);
    mXmlConf.GetVar(baseName + "DestinationWaypoint@longitude", longitude);
    mDestinationWaypoint = Utm(CxUtils::Wgs(latitude, longitude));

    mXmlConf.GetVar(baseName + "CourseA@targetHeading", mCourseATargetHeading);
    mXmlConf.GetVar(baseName + "CourseB@targetHeading", mCourseBTargetHeading);
}


/** Draws maps for display purposes */
void CheatersHand::DrawMaps()
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

    // Draw vehicle on top of everything.
    mMapPose.Draw(mpNavigator->GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);
}

std::vector<Zebulon::Vision::Return> CheatersHand::GetBoxes( IplImage* camimage,  IplImage* depthimage,  IplImage* depthmap)
{
    std::vector<Zebulon::Vision::Return> result;
    if(camimage == NULL || depthimage == NULL) return result;
    std::list<CvBox2D> boxes;
    //IplImage * segmented = SegmentationMask(camimage);
    IplImage * segmented = cvCloneImage(camimage);
    IplImage * blah = NULL;//
    IplImage * depthconvert = cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
    cvZero(blah);
    IplImage * depthmapclone = cvCloneImage(depthmap);
    IplImage * depthimagetrunk = cvCloneImage(depthimage);
    IplImage * debug = cvCloneImage(camimage);

    //cvThreshold(segmented,segmented,150,255,CV_THRESH_TOZERO);
    cvThreshold(depthmap,depthmapclone,5,255,CV_THRESH_TOZERO_INV);
    //cvThreshold(depthmapclonedepthmapclone,)
    mpGlobalInfo->GetDepthMap()->ToGrayImage(depthmapclone,depthimagetrunk);
    //cvConvertImage(depthimagetrunk,depthconvert);
    //cvAnd(segmented,blah,segmented);
    //cvShowImage("masked", segmented);
    //mWhite.GetVisionReturn(camimage,result,segmented);
    blah = mWhite.GetMask(camimage, NULL);
    cvAnd(depthimagetrunk,blah,depthimagetrunk);

    cvThreshold(depthimagetrunk,depthimagetrunk,50,255,CV_THRESH_BINARY);
    cvShowImage("anded video",depthimagetrunk);

//    boxes = VisionUtils::GetBlobBoxes(depthimagetrunk,.005,.5,false);

//    //cvShowImage("depth original",depthimage);
//    //IplImage * debug = NULL;
//    //debug = cvCloneImage(camimage);
    std::list<CvBox2D> squares = FindSquares(depthimagetrunk,debug,.005,.80,10,false);
    std::list<CvBox2D>::iterator it;
    cvShowImage("square output",debug);
//    for(it = boxes.begin(); it != boxes.end(); it++)
//    {

//    }
//    IplImage * temp;
//    CvScalar color;
//    CvScalar bestcolor;
//    CvRect rect;
//    for(it = boxes.begin(); it != boxes.end(); it++)
//    {
//        it->size.width += it->size.width*.25;
//        it->size.height += it->size.height*.25;
//        rect = VisionUtils::ToCvRect(*it);
//        cvSetImageROI(camimage,rect);
//        temp = cvCloneImage(camimage);
//        cvResetImageROI(camimage);
//        //std::swap(it->size.width,it->size.height);
//        //it->angle  = CxUtils::Orientation::AddToAngle(it->angle,90,false);
//        //temp = VisionUtils::Crop(camimage,*it);
//        color = cvAvg(temp);
//        if(color.val[0] > 75 && color.val[1] > 75 && color.val[2] > 75)
//        {
//            Utility::Display::DisplayImage("Crop", temp,1);
//        }
//        cvReleaseImage(&temp);
//    }
    //Utility::Display::DisplayImage("Box Output", debug,1);

    cvReleaseImage(&depthconvert);
    cvReleaseImage(&debug);
    cvReleaseImage(&depthmapclone);
    cvReleaseImage(&depthimagetrunk);
    cvReleaseImage(&segmented);
    cvReleaseImage(&blah);
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a std linked list of cvBox2D objects sorted from big to small
///          found in source image using a combination of thresholding
///          and edge detection. Original source taken from square.c in opencv
///          example programs.
///
///   \param[in] img Source image, can be color or grayscale.
///   \param[out] dest Image to draw boxes on to, can be NULL
///   \param[in] minsize min percentage of total pixels before square is considred
///   \param[in] maxsize max percentage of total pixels before square is considred
///   \param[in] cannyThreshold threshold used for edge detection
///
///   \return std linked list of cvBox2D objects sorted from bigest to smallest
///
///////////////////////////////////////////////////////////////////////////////////
std::list<CvBox2D> CheatersHand::FindSquares(const IplImage* img, IplImage *dest, const double minsize, const double maxsize, const int cannyThreshold, const bool angle3d)
{
    std::list<CvBox2D> ret;
    if(img == NULL) return ret;
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* contours;
    CvSeq* polyApprox;
    double imgArea = img->height * img->width;
    CvSize imageSize = cvSize(img->width & -2, img->height & -2 );
    IplImage* imgCopy = cvCloneImage( img ); // make a copy of input image
    IplImage* imgGray = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    IplImage * imgGraySum = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2,imageSize.height/2), imgCopy->depth, imgCopy->nChannels );
    IplImage* resultImage = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    //if(dest != NULL) cvZero(dest);
    CvBox2D tempBox;

    int thresholdsToTry = 5;
    // create empty sequence that will contain points -
    // 4 points per square (the square's vertices)
    //CvSeq* squares = cvCreateSeq( 0, sizeof(CvSeq), sizeof(CvPoint), storage );


    // select the maximum ROI in the image
    // with the width and height divisible by 2
    //cvSetImageROI( timg, cvRect( 0, 0, sz.width, sz.height ));

    // down-scale and upscale the image to filter out the noise
    cvPyrDown( imgCopy, imgSmallCopy);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //cvMorphologyEx(imgSmallCopy,imgSmallCopy,NULL,CV_SHAPE_RECT,CV_MOP_OPEN,1);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

    // These two lines may work, needs to be tested!!
    IplConvKernel* structureElement = cvCreateStructuringElementEx(3,3,1,1,CV_SHAPE_RECT);
    cvMorphologyEx(imgSmallCopy, imgSmallCopy, NULL, structureElement, CV_MOP_OPEN, 1 );

//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    cvPyrUp( imgSmallCopy, imgCopy);
    //canny = cvCreateImage( imageSize, IPL_DEPTH_8U, 1 );
    cvZero(imgGraySum);
    for(int colorPlane = 1; colorPlane <= imgCopy->nChannels+1; colorPlane++)
    {
//        if(colorPlane == imgCopy->nChannels +1)
//        {
//            cvCopy(imgGraySum, resultImage);
//        }
//        else if(imgCopy->nChannels > 1)
//        {
//            //colorplane 0 = all channels
//            cvSetImageCOI(imgCopy, colorPlane);
            cvCopy(imgCopy,imgGray);
//        }

        for(int threshLevel = 0; threshLevel < thresholdsToTry; threshLevel++)
        {
            if(colorPlane != imgCopy->nChannels +1)
            {
                switch(threshLevel)
                {
                case 0:

                    // apply Canny. Take the upper threshold from slider
                    // and set the lower to 0 (which forces edges merging)
                   // cvShowImage("boxcanny input",imgGray);
                    cvCanny(imgGray, resultImage, 0,50, 3 );
                    //cvShowImage("boxcanny output",resultImage);
                   // cvWaitKey(0);
                    // dilate canny output to remove potential
                    // holes between edge segments

                    //cvDilate( resultImage, resultImage, 0, 1);
                    cvOr(resultImage,imgGraySum,imgGraySum);

                    break;
//                case 2:

//                    // apply Canny. Take the upper threshold from slider
//                    // and set the lower to 0 (which forces edges merging)
//                   // cvShowImage("boxcanny input",imgGray);
//                    cvCanny(imgGray, resultImage, 0,150, 3 );
//                    //cvShowImage("boxcanny output",resultImage);
//                    //cvWaitKey(0);
//                    // dilate canny output to remove potential
//                    // holes between edge segments

//                    //cvDilate( resultImage, resultImage, 0, 1);
//                    cvOr(resultImage,imgGraySum,imgGraySum);

//                    break;
//                case 1:

//                    // apply Canny. Take the upper threshold from slider
//                    // and set the lower to 0 (which forces edges merging)
//                   // cvShowImage("boxcanny input",imgGray);
//                    cvCanny(imgGray, resultImage, 50,200, 3 );
//                    //cvShowImage("boxcanny output",resultImage);
//                    //cvWaitKey(0);
//                    // dilate canny output to remove potential
//                    // holes between edge segments

//                    //cvDilate( resultImage, resultImage, 0, 1);
//                    cvOr(resultImage,imgGraySum,imgGraySum);

//                    break;

                default:
                    cvThreshold(imgGray, resultImage, ((threshLevel)*255)/thresholdsToTry, 255, CV_THRESH_BINARY );
                    //cvDilate( resultImage, resultImage, 0, 1 );
                    //cvAdaptiveThreshold(imgGray,resultImage,255);
                    break;
                }
            }
            //cvCanny(imgGray, resultImage, 100, ((threshLevel+1)*255)/thresholdsToTry, 5 );
            //cvDilate( resultImage, resultImage, 0, 1 );
            //cvShowImage("Diff",resultImage);
            //cvWaitKey(10);
            // find contours and store them all as a list

            //cvShowImage("Diff",resultImage);
            //cvWaitKey(0);
            cvFindContours( resultImage, storage, &contours, sizeof(CvContour),
                            CV_RETR_LIST, CV_CHAIN_APPROX_SIMPLE, cvPoint(0,0));

            while( contours )
            {
                // approximate contour with accuracy proportional
                // to the contour perimeter
                polyApprox = cvApproxPoly( contours, sizeof(CvContour), storage,
                                           CV_POLY_APPROX_DP, cvContourPerimeter(contours)*0.02, 0 );

                //printf("poly points: %d\n", polyApprox->total);
                // square contours should have 4 vertices after approximation
                // relatively large area (to filter out noisy contours)
                // and be convex.
                // Note: absolute value of an area is used because
                // area may be positive or negative - in accordance with the
                // contour orientation
                double  contourArea =  fabs(cvContourArea(polyApprox,CV_WHOLE_SEQ));
                if( polyApprox->total == 4 &&               //total points in contour
                        contourArea > (imgArea * minsize) &&    //contour area covers greater than minsize of total image
                        contourArea < (imgArea * maxsize) &&    //contour area covers less than maxsize of total image
                        cvCheckContourConvexity(polyApprox) )   //contour is convex
                {
                    double s = 0;
                    double t = 0;
                    for(int i = 0; i < 5; i++ )
                    {
                        // find minimum angle between joint
                        // edges (maximum of cosine)
                        if( i >= 2 )
                        {
                            t = fabs(VisionUtils::GetAngle(
                                         (CvPoint*)cvGetSeqElem( polyApprox, i ),
                                         (CvPoint*)cvGetSeqElem( polyApprox, i-2 ),
                                         (CvPoint*)cvGetSeqElem( polyApprox, i-1 )));
                            s = s > t ? s : t;
                        }
                    }
                    // if cosines of all angles are small
                    // (all angles are ~90 degree) then write quandrange
                    // vertices to resultant sequence
                    if( s < 0.5 )
                    {
                        tempBox = cvMinAreaRect2(contours,storage);
                        double tempBoxArea = tempBox.size.width * tempBox.size.height;
                        std::list<CvBox2D>::iterator it;
                        for(it = ret.begin(); it != ret.end(); it++)
                        {
                            if(tempBoxArea > (it->size.width * it->size.height))
                            {
                                break;
                            }
                        }
                        if(dest != NULL)
                        {
                            if(angle3d)
                            {
                                VisionUtils::DrawSquare(dest,polyApprox,CV_RGB(255,0,0));
                            }
                            else
                            {

                                VisionUtils::DrawSquare(dest,tempBox,CV_RGB(255,0,0));
                            }

                        }
                        if(angle3d)
                        {

                            VisionUtils::Calc3DSquareAngle(polyApprox,tempBox.angle,dest);
                        }
                        ret.insert(it,tempBox);
                    }
                }
                contours = contours->h_next;
            }
        }
    }


    // release all the temporary images
    cvReleaseImage( &imgCopy );
    cvReleaseImage( &imgSmallCopy );
    cvReleaseImage( &resultImage );
    cvReleaseImage( &imgGray );
    cvReleaseMemStorage(&storage);
    return ret;
}

IplImage* CheatersHand::SegmentationMask(const IplImage * imgIn)
{
    IplImage * imgOut = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * src = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage* hsv = cvCreateImage( cvGetSize(imgIn), 8, 3 );

    IplImage * chan0 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan1 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan2 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);
    IplImage * chan3 = cvCreateImage(cvGetSize(imgIn),IPL_DEPTH_8U, 1);

    cvCvtColor( imgIn, hsv, CV_BGR2YCrCb );
    cvSplit(hsv,chan0,chan1,chan2, NULL);
    //cvConvertImage(imgIn,src);

    src = cvCloneImage(chan0);
    CvMemStorage* storage = cvCreateMemStorage(0);
    CvSeq* comp = NULL;
    //lower last param for more segments
    cvPyrSegmentation( src, imgOut, storage, &comp, 2, 200, 50 );
    cvShowImage("segmentation",imgOut);
    //cvNormalize(imgOut,imgOut,255,0, CV_MINMAX);
    //cvThreshold(imgOut,imgOut,250,255,CV_THRESH_TOZERO);
//        int n_comp = comp->total;
//        for( int i=0; i<n_comp; i++ )
//        {
//            CvConnectedComp* cc = (CvConnectedComp*) cvGetSeqElem( comp, i );
//            cvDrawRect(imgOut,cvPoint(cc->rect.x,cc->rect.y),cvPoint(cc->rect.x+cc->rect.width,cc->rect.y+cc->rect.height),CV_RGB(255,255,255));

//        //do_something_with( cc );
//        }
    cvReleaseImage(&src);
    cvReleaseImage(&hsv);
    cvReleaseImage(&chan0);
    cvReleaseImage(&chan1);
    cvReleaseImage(&chan2);
    cvReleaseImage(&chan3);
    cvReleaseMemStorage( &storage );
    return imgOut;
}


void CheatersHand::SendHotSuit(const CxUtils::Wgs& position,
                               int suit)
{
    if(mTcp.IsValid() == false)
    {
        mTcp.InitializeSocket("10.11.12.208", 5555);
    }

    std::string type;

    switch(suit)
    {
    case 1:
        type = "SPADE";
        break;
    case 2:
        type = "DIAMOND";
        break;
    case 3:
        type = "HEART";
        break;
    default:
        type = "CLUB";
        break;
    }

    std::stringstream str;
    str << "REPORT;UCF:" << type << "@"
        << std::setprecision(6) << position.mLatitude << ","
        << std::setprecision(6) << position.mLongitude
        << "\n";

    mTcp.Send(str.str().c_str(), str.str().length());
}

/*  End of File */

