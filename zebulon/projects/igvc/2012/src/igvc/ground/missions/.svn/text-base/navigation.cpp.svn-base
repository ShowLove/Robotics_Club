
#include "igvc/ground/missions/navigation.h"

using namespace Zebulon;
using namespace IGVC;

const std::string Navigation::MapName = "Default";

const unsigned int Navigation::VisionSourceID = 1;
const unsigned int Navigation::LaserSourceID = 2;
const unsigned int Navigation::BorderSourceID = 3;

Navigation::Navigation()
{
    mForwardCamera = NULL;
}

Navigation::~Navigation()
{
    mMap->Destroy();
    cvReleaseImage(&mForwardCamera);
    mRangeData.clear();
}

int Navigation::Init()
{
    // Default Values
    mPathWidthMeters = .5;
    mAngleIncrementDegrees = 15;
    mSweepAngleStartDegrees = -90;
    mSweepAngleEndDegrees = 90;
    mSegmentLengthMeters = 2;
    mHistoryDepth = 5;
    mSearchDepth = 2;

    mCurrentVelocity = mDesiredVelocity = 0;

    // XML Loaded Values
    double mapSize, cellSize;

    mNavigator.AddMap(mMap = new Cartographer::WorldMap(), MapName);
    //mNavigator.AddMap(mMap = new QuadTreeMap(), MapName);

    mXmlConf.GetVar("Ground.Navigation.Map@MapSize", mapSize);
    mXmlConf.GetVar("Ground.Navigation.Map@CellSize", cellSize);
    mMap->Create(mapSize, cellSize);
    mXmlConf.GetVar("Ground.Navigation.Map.Consume@Laser", mLaserConsumeThreshold);
    mXmlConf.GetVar("Ground.Navigation.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.Navigation.Drawing@PixelsPerMeter", mPixelsPerMeter);

    mNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    //mNavigator.SetMapImageSizeMeters(35);
    mNavigator.SetDrawPoseSize(.75, .25);

    std::string wayPointsFile;
    mXmlConf.GetVar("Ground.Navigation.Waypoints@File", wayPointsFile);
    mDesiredPositionList = AI::Mission::LoadUtmWaypointsFromFile(wayPointsFile);

    std::string bWayPointsFile;
    mXmlConf.GetVar("Ground.Navigation.BorderWaypoints@File", bWayPointsFile);
    mBorderWaypointsVector = AI::Mission::LoadUtmWaypointsFromFile(bWayPointsFile);
    mLastWaypoint = false;

    mDesiredPositionIterator = mDesiredPositionList.begin();

    // Start in the standby state
    mState = Standby;

    mWaypointDebounce.Initialize(3, true);

    return 0;
}

int Navigation::FetchProcess()
{
    static bool firstRun = true;

    if (!mpGlobalInfo->IsAutonomous())
    {
        mState = Standby;
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Map
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.Navigation.Map.Consume@Laser", mLaserConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);

    mXmlConf.GetVar("Ground.Navigation.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    mXmlConf.GetVar("Ground.Navigation.Map.TimeToLive@Laser", mLaserTimeToLive);
    mXmlConf.GetVar("Ground.Navigation.Map.TimeToLive@Vision", mVisionTimeToLive);
    mMap->Update();


    // Add the borders around the course
    mMap->ClearObjectsBySource(BorderSourceID);

    if (!mLastWaypoint)
    {
        std::vector<Utm>::iterator iter = mBorderWaypointsVector.begin();
        iter++;
        for (; iter != mBorderWaypointsVector.end();)
        {
            CxUtils::Segment3D seg;
            Cartographer::Pose p1 = mMap->CreatePose(*iter, CxUtils::Point3D());

            iter++;
            Cartographer::Pose p2;
            if (iter == mBorderWaypointsVector.end())
            {
                p2 = mMap->CreatePose(mBorderWaypointsVector.front(), CxUtils::Point3D());
            }
            else
            {
                p2 = mMap->CreatePose(*iter, CxUtils::Point3D());
            }

            seg.mPoint1 = p1.mPosition;
            seg.mPoint2 = p2.mPosition;
            seg.mWidth = .2;
            Cartographer::Objects::Line borderLine(seg, BorderSourceID);

            mMap->AddObject(&borderLine);
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Navigator
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.Navigation.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.Navigation.Controls@DistanceThresh", mDistanceThresh);

    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@PathWidthMeters", mPathWidthMeters);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@AngleIncrementDegrees", mAngleIncrementDegrees);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@SweepAngleStartDegrees", mSweepAngleStartDegrees);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@SweepAngleEndDegrees", mSweepAngleEndDegrees);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@HistoryDepth", mHistoryDepth);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam@SearchDepth", mSearchDepth);

    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@Straightness", mStraightnessWeight);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@Distance", mDistanceWeight);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@Simplicity", mSimplicityWeight);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@Angle", mAngleWeight);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@Heading", mHeadingWeight);
    mXmlConf.GetVar("Ground.Navigation.HistoryBeam.Weights@LinePotential", mLinePotentialWeight);

    Cartographer::Navigator::Settings *settings = new Cartographer::Navigator::Settings;
    settings->Set(HistoryBeam::PathWidthMeters, mPathWidthMeters);
    settings->Set(HistoryBeam::AngleIncrementDegrees, mAngleIncrementDegrees);
    settings->Set(HistoryBeam::SweepAngleStartDegrees, mSweepAngleStartDegrees);
    settings->Set(HistoryBeam::SweepAngleEndDegrees, mSweepAngleEndDegrees);
    settings->Set(HistoryBeam::SegmentLengthMeters, mSegmentLengthMeters);
    settings->Set(HistoryBeam::HistoryDepth, mHistoryDepth);
    settings->Set(HistoryBeam::SearchDepth, mSearchDepth);

    settings->Set(HistoryBeam::StraightnessWeight, mStraightnessWeight);
    settings->Set(HistoryBeam::DistanceWeight, mDistanceWeight);
    settings->Set(HistoryBeam::SimplicityWeight, mSimplicityWeight);
    settings->Set(HistoryBeam::AngleWeight, mAngleWeight);
    settings->Set(HistoryBeam::HeadingWeight, mHeadingWeight);
    settings->Set(HistoryBeam::LinePotentialWeight, 0);

    // For some reason the previous settings are not deleted when new ones are loaded
    delete mNavigator.GetSettings();
    mNavigator.SetSettings(settings);

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Kinematics
    ///////////////////////////////////////////////////////////////////////////////////////

    // Get current position and orientation
    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation(); // Orientation in rads

    if (firstRun)
    {
        mNavigator.GetMap(MapName)->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
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
    Cartographer::Pose myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
    //mMap->ClearObjectsBySource(LaserSourceID);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.1, LaserSourceID);
        sphere.SetTimeToLive(mLaserTimeToLive);
        mMap->AddObject(&sphere, myPose);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Camera
    ///////////////////////////////////////////////////////////////////////////////////////

    cvReleaseImage(&mForwardCamera);
    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    if(mForwardCamera != NULL)
    {
        mMap->ClearObjectsBySource(VisionSourceID);
    }

    return 0;
}

std::string Navigation::ExitEarly()
{
    return "KeepRunning";
}

std::string Navigation::Execute()
{
    double angle;
    double distance;

    Cartographer::Path::List allPaths;

    switch (mState)
    {
    case Standby:
        {
            mDesiredOrientation.mZ = mCurrentOrientation.mZ;
            mDesiredVelocity = 0;

            mDesiredPositionList.clear();
            mBorderWaypointsVector.clear();

            std::string wayPointsFile;
            mXmlConf.GetVar("Ground.Navigation.Waypoints@File", wayPointsFile);
            mDesiredPositionList = AI::Mission::LoadUtmWaypointsFromFile(wayPointsFile);

            std::string bWayPointsFile;
            mXmlConf.GetVar("Ground.Navigation.BorderWaypoints@File", bWayPointsFile);
            mBorderWaypointsVector = AI::Mission::LoadUtmWaypointsFromFile(bWayPointsFile);
            mLastWaypoint = false;



            if (mpGlobalInfo->IsAutonomous())
            {
                mState = Start;
            }

            break;
        }
    case Start:
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            // Record our starting position
            ///////////////////////////////////////////////////////////////////////////////////////

            mStartPosition = mCurrentPosition;
            mDesiredOrientation = mCurrentOrientation;

            mStopWatch.Initialize(4);
            mStopWatch.Start();

            mState = DriveOut;

            break;
        }
    case DriveOut:
        {
            mDesiredVelocity = mMaxVelocity;

            if (mStopWatch.IsFinished())
            {
                mDesiredVelocity = 0;

                mStartSecondaryPosition = mCurrentPosition;
                mDesiredPositionList.push_back(mStartSecondaryPosition);
                mDesiredPositionList.push_back(mStartPosition);
                mDesiredPositionIterator = mDesiredPositionList.begin();
                mState = GoToWaypoint;
            }

            break;
        }
    case GoToWaypoint:
        {
            ///////////////////////////////////////////////////////////////////////////////////////
            // Obstacle Avoidance
            //  get the heading associated with avoiding obstacles
            ///////////////////////////////////////////////////////////////////////////////////////

            Cartographer::Path path;
            Cartographer::Pose myPose;
            CxUtils::Point3D offset(0.0, 0, 0);

            myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
            offset = myPose.ConvertFromLocalToMapCoordinates(myPose, offset);
            Cartographer::Pose startPathPose = myPose;
            startPathPose.mPosition = offset;

            Cartographer::Pose destPose;
            destPose = mNavigator.CreatePose(MapName, mDesiredPosition, CxUtils::Point3D());
            distance = Utm::Distance(mDesiredPosition, mCurrentPosition);

            Cartographer::Path waypoints;
            waypoints.Push(destPose);

            // const Wgs& startPosition,
            // const Orientation startOrientation,
            // const Wgs& endPosition,
            // Path& pathFound,
            // Path::List * allPaths = NULL,
            // const std::string& mapName = "Default");
            CxUtils::Segment3D seg(myPose.mPosition, destPose.mPosition, mPathWidthMeters);
            if (distance < 4 && !mMap->IsCollisionInXY(seg))
            {
                Cartographer::Path solution;
                solution.Push(myPose);
                solution.Push(destPose);
                solution.SetPathWidth(mPathWidthMeters);

                Cartographer::Path::List solutions;
                solutions.push_back(solution);

                mNavigator.DrawLocalMap(MapName, 70, &solutions, NULL, &myPose, &waypoints);

                angle = atan2(destPose.mPosition.mX - myPose.mPosition.mX, destPose.mPosition.mY - myPose.mPosition.mY);

                mDesiredOrientation.mZ = angle;

                mDesiredVelocity = mMaxVelocity;
            }
            else if (mNavigator.GetPath(startPathPose, destPose,
                                        path,
                                        &allPaths,
                                        MapName) > 0)
            {
                mNavigator.DrawLocalMap(MapName, 70, NULL, &path, &myPose, &waypoints);

                if(path.Size() >= 2)
                {
                    Cartographer::Pose A;
                    path.PopFront(&A);

                    Cartographer::Pose B;
                    path.PopFront(&B);

                    angle = atan2(B.mPosition.mX - A.mPosition.mX, B.mPosition.mY - A.mPosition.mY);

                    mDesiredOrientation.mZ = angle;

                    mDesiredVelocity = mMaxVelocity;
                }
                else
                {
                    CxUtils::SleepMs(1);
                }
            }
            else
            {
                // No avoidance angle...
                ///mDesiredOrientation = mCurrentOrientation; // stay the course i guess

                // Might have to change to back up or something, maybe we'll just have it spin in place till it figures itself out...
                mDesiredVelocity = 0;//mMaxVelocity;
                mDesiredOrientation.mZ = CxUtils::Orientation::AddToAngle(mCurrentOrientation.mZ, 30);

                mNavigator.DrawLocalMap(MapName, 70, NULL, NULL, &myPose, &waypoints);

                printf("Navigation::Error: Unable to find path\n");
            }

            IplImage* img = mNavigator.GetLocalMapImage();
            if (img != NULL)
            {
                Utility::Display::DisplayImage(MapName, img);
            }

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
                mDesiredPositionIterator++;
                if (mDesiredPositionIterator == mDesiredPositionList.end())
                {
                    mLastWaypoint = true;
                }
                else
                {
                    mLastWaypoint = false;
                }
                mDesiredPositionIterator--;
            }
            break;
        }
    case Finish:
        {
            mDesiredVelocity = 0;
            mDesiredOrientation.mZ = 0;//CxUtils::Orientation::AddToAngle(mDesiredOrientation.mZ, 10, false);
            break;
        }
    }

    mpGlobalCommand->SetVelocity(mDesiredVelocity);
    mpGlobalCommand->SetYaw(mDesiredOrientation.mZ);

    printf("Navigation:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Beam Angle: %lf\n", distance, angle);
    printf("  Paths found: %lf\n", distance, allPaths.size());
    return "KeepRunning";
}

int Navigation::ErrorCheck()
{
    return 0;
}
