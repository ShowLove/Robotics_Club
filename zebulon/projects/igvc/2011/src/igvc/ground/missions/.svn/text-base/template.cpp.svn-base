
#include "igvc/ground/missions/template.h"

using namespace Zebulon;
using namespace IGVC;

const std::string TemplateMission::MapName = "Default";

const unsigned int TemplateMission::VisionSourceID = 1;
const unsigned int TemplateMission::LaserSourceID = 2;

TemplateMission::TemplateMission()
{
    mForwardCamera = NULL;
    mProcessedImage = NULL;
}

TemplateMission::~TemplateMission()
{
    mMap->Destroy();
    cvReleaseImage(&mForwardCamera);
    cvReleaseImage(&mProcessedImage);
    mRangeData.clear();

    delete mpLineAngleFilter;
}

int TemplateMission::Init()
{
    // XML Loaded Values
    double cellSize;

    mMap = new Cartographer::WorldMap();


    mXmlConf.GetVar("Ground.TemplateMission.Map@MapSize", mMapSize);
    mXmlConf.GetVar("Ground.TemplateMission.Map@CellSize", cellSize);
    mMap->Create(mMapSize, cellSize);

    mXmlConf.GetVar("Ground.TemplateMission.Map.Consume@Laser", mLaserConsumeThreshold);
    mXmlConf.GetVar("Ground.TemplateMission.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);
    //mMap->SetConsumeThreshold(Cartographer::Objects::Line::Name, mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.TemplateMission.Drawing@PixelsPerMeter", mPixelsPerMeter);

    std::string wayPointsFile;
    mXmlConf.GetVar("Ground.TemplateMission.Waypoints@File", wayPointsFile);
    mDesiredPositionList = AI::Mission::LoadUtmWaypointsFromFile(wayPointsFile);

    mDesiredPositionIterator = mDesiredPositionList.begin();

    // Start in the standby state
    mState = Standby;

    mWaypointDebounce.Initialize(3, true);

    return 0;
}

int TemplateMission::FetchProcess()
{
    static bool firstRun = true;

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Map
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.TemplateMission.Map.Consume@Laser", mLaserConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);

    mXmlConf.GetVar("Ground.TemplateMission.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    mXmlConf.GetVar("Ground.TemplateMission.Map.TimeToLive@Laser", mLaserTimeToLive);
    mXmlConf.GetVar("Ground.TemplateMission.Map.TimeToLive@Vision", mVisionTimeToLive);
    mMap->Update();

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Navigator
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.TemplateMission.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.TemplateMission.Controls@DistanceThresh", mDistanceThresh);

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Kinematics
    ///////////////////////////////////////////////////////////////////////////////////////

    // Get current position and orientation
    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation(); // Orientation in rads

    if (firstRun)
    {
        mMap->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
        mProcessedImage = cvCreateImage(cvSize(500,500), IPL_DEPTH_8U, 3);
        firstRun = false;
    }

    // Get our desired destination waypoint
    double northing, easting;
    int zoneNum, zoneLetter;
    mDesiredPositionIterator->Get(northing, easting, zoneNum, zoneLetter);
    mDesiredPosition(northing, easting, zoneNum, zoneLetter);

    // Get our current forward (local x) velocity
    //mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Laser
    ///////////////////////////////////////////////////////////////////////////////////////

    // Get the data from the laser range finder
    mRangeData.clear();
    mpGlobalInfo->GetInfo(GlobalInfo::Laser, mRangeData);

    CxUtils::Point3D::List::iterator rawPoint;
    Cartographer::Pose myPose = mMap->CreatePose(mCurrentPosition, mCurrentOrientation);

    mMap->ClearObjectsBySource(LaserSourceID);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.1, LaserSourceID);
        //sphere.SetTimeToLive(mLaserTimeToLive);
        mMap->AddObject(&sphere, myPose);
    }

    cvSet(mProcessedImage, cvScalar(0,0,0));

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Camera
    ///////////////////////////////////////////////////////////////////////////////////////

    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    if(mForwardCamera != NULL)
    {
        Utility::Display::DisplayImage("Forward Camera", mForwardCamera);

        // DO COMPUTER VISION HERE!

        cvReleaseImage(&mForwardCamera);
        mForwardCamera = NULL;
    }

    return 0;
}

std::string TemplateMission::ExitEarly()
{
    return "KeepRunning";
}

std::string TemplateMission::Execute()
{
    double angle;
    double distance;

    Cartographer::Path::List allPaths;

    switch (mState)
    {
    case Standby:
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            // Wait to be put into autonomous mode
            ///////////////////////////////////////////////////////////////////////////////////////

            mState = Finish;

            break;
        }
    case GoToWaypoint:
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            // Lane lines
            //
            ///////////////////////////////////////////////////////////////////////////////////////


            ///////////////////////////////////////////////////////////////////////////////////////
            // Obstacle Avoidance
            //  get the heading associated with avoiding obstacles
            ///////////////////////////////////////////////////////////////////////////////////////
            Cartographer::Pose myPose;
            CxUtils::Point3D offset(0.0, 0, 0);

            myPose = mMap->CreatePose(mCurrentPosition, mCurrentOrientation);
            offset = myPose.ConvertFromLocalToMapCoordinates(myPose, offset);
            Cartographer::Pose startPathPose = myPose;
            startPathPose.mPosition = offset;

            Cartographer::Pose destPose;
            destPose = mMap->CreatePose(mDesiredPosition, CxUtils::Point3D());
            distance = Utm::Distance(mDesiredPosition, mCurrentPosition);

            Cartographer::Path waypoints;
            waypoints.Push(destPose);

            CxUtils::Segment3D seg(myPose.mPosition, destPose.mPosition, mPathWidthMeters);
            if (distance < 4 && !mMap->IsCollisionInXY(seg))
            {
                mDesiredOrientation.mZ = 0;

                mDesiredVelocity = mMaxVelocity;
            }
            else
            {
                angle = atan2(destPose.mPosition.mX - myPose.mPosition.mX, destPose.mPosition.mY - myPose.mPosition.mY);

                mDesiredOrientation.mZ = angle;

                mDesiredVelocity = mMaxVelocity;
            }

            CxUtils::Point3D _corner = myPose.mPosition;
            _corner.mX = _corner.mX - fabs(mMapSize/2.0);
            _corner.mZ = _corner.mY - fabs(mMapSize/2.0);
            mMap->Draw(mProcessedImage, mPixelsPerMeter, _corner);
            Utility::Display::DisplayImage(MapName, mProcessedImage);

            // Reached our destination, next waypoint
            mWaypointDebounce.Bounce(distance < mDistanceThresh);

            if (mWaypointDebounce.GetDebounced())
            {
                mDesiredVelocity = 0;
                mState = NextWaypoint;
            }

            break;
        }
    case NextWaypoint:
        {
            mDesiredVelocity = 0;
            mDesiredPositionIterator++;
            if (mDesiredPositionIterator == mDesiredPositionList.end())
            {
                mState = Finish;
            }
            else
            {
                mState = GoToWaypoint;
            }
            break;
        }
    case Finish:
        {
            mDesiredVelocity = 0;
            mDesiredPositionIterator = mDesiredPositionList.begin();
            mState = GoToWaypoint;
            break;
        }
    }

    mpGlobalCommand->SetVelocity(mDesiredVelocity);
    mpGlobalCommand->SetYaw(mDesiredOrientation.mZ);

    printf("TemplateMission:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Beam Angle: %lf, Line Angle: %lf\n", distance, angle, mLineAngle);
    printf("  Paths found: %lf\n", distance, allPaths.size());
    return "KeepRunning";
}

int TemplateMission::ErrorCheck()
{
    return 0;
}
