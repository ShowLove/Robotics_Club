#include "igvc/ground/missions/persistent.h"

using namespace Zebulon;
using namespace IGVC;

const std::string Persistent::MapName = "Default";

Persistent::Persistent()
{
    mForwardCamera = NULL;
    mVisualizationImage = NULL;
    mMap = NULL;
}

Persistent::~Persistent()
{
    mNavigator.GetMap(MapName)->Clear();
}

int Persistent::Init()
{
    // Default Values
    mPathWidth = .5;
    mSearchDepth = 3;
    mSegmentLengthMeters = 3;
    mScansPerSweep = 15;
    mAngleScannedRads = CxUtils::CX_TWO_PI;
    mBranchesPerDepth = 5;
    mAngleSweepIncrementRads = mAngleScannedRads/mScansPerSweep;
    mEndpointThreshold = 0.5;

    mCurrentVelocity = mDesiredVelocity = 0;

    // XML Loaded Values
    double mapSize, cellSize;
    mMap = new QuadTreeMap();
    mNavigator.AddMap(mMap, MapName);
    mXmlConf.GetVar("Ground.Persistent.Map@MapSize", mapSize);
    mXmlConf.GetVar("Ground.Persistent.Map@CellSize", cellSize);
    mMap->Create(mapSize, cellSize);
    //mNavigator.GetMap("Map")->Create(mpGlobalInfo->MapSizeMeters, mpGlobalInfo->MapCellSizeMeters);
    mXmlConf.GetVar("Ground.Persistent.Map@ConsumeThreshold", mConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.Persistent.Drawing@PixelsPerMeter", mPixelsPerMeter);

    mNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    mNavigator.SetDrawPoseSize(.75, .25);

    mXmlConf.GetVar("Ground.Persistent.BeamSearch@PathWidth", mPathWidth);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@SearchDepth", mSearchDepth);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@ScansPerSweep", mScansPerSweep);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@AngleScannedRads", mAngleScannedRads);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@BranchesPerDepth", mBranchesPerDepth);
    mXmlConf.GetVar("Ground.Persistent.BeamSearch@EndpointThreshold", mEndpointThreshold);




    // Waypoints to go to, this is kinda a hack for now
    /** DC */
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88979, -77.01469)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88968, -77.01466)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88965, -77.01480)));

    /*
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58521, -81.19915)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58518, -81.19906)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58507, -81.19904)));*/

    mDesiredPositionIterator = mDesiredPositionList.begin();

    mState = GoToWaypoint;

    return 0;
}

int Persistent::FetchProcess()
{
    static bool firstRun = true;

    mXmlConf.GetVar("Ground.Persistent.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.Persistent.Controls@DistanceThresh", mDistanceThresh);

    Cartographer::Navigator::Settings *settings = new Cartographer::Navigator::Settings;
    settings->Set(Cartographer::BeamSearch::PathWidthMeters, mPathWidth);
    settings->Set(Cartographer::BeamSearch::SearchDepth, mSearchDepth);
    settings->Set(Cartographer::BeamSearch::BranchLengthMeters, mSegmentLengthMeters);
    settings->Set(Cartographer::BeamSearch::ScanAngleRads, mAngleScannedRads);
    settings->Set(Cartographer::BeamSearch::BranchesPerDepth, mBranchesPerDepth);
    settings->Set(Cartographer::BeamSearch::AngleBetweenBranchesRads, mAngleSweepIncrementRads);
    settings->Set(Cartographer::BeamSearch::EndpointThresholdMeters, mEndpointThreshold);
    mNavigator.SetSettings(settings);

    // Get current position and orientation
    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation(); // Orientation in rads

    if (firstRun)
    {
        mNavigator.GetMap(MapName)->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
        mVisualizationImage = cvCreateImage(cvSize(200, 200), 8, 3);
        firstRun = false;
    }

    // Get our desired destination waypoint
    double northing, easting;
    int zoneNum, zoneLetter;
    mDesiredPositionIterator->Get(northing, easting, zoneNum, zoneLetter);
    mDesiredPosition(northing, easting, zoneNum, zoneLetter);

    // Get our current forward (local x) velocity
    //mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    // Get the data from the laser range finder
    mpGlobalInfo->GetInfo(GlobalInfo::Laser, mRangeData);

    CxUtils::Point3D::List::iterator rawPoint;

    Cartographer::Pose myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.05);
        sphere.SetTimeToLive(10);
        mMap->AddObject(&sphere, myPose);
    }

    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    //static std::vector<CxUtils::Segment3D> GroundVision::FindLaneLines(const IplImage* imgIn, IplImage* imgOut = NULL, int rows = 0, int cols = 2)

    //mLaneLines = IGVC::GroundVision::FindLaneLines(mForwardCamera);

    return 0;
}

std::string Persistent::ExitEarly()
{
    return "KeepRunning";
}

std::string Persistent::Execute()
{
    double angle;
    double laneAngle;
    double distance;

    //const QuadTreeMap* quadMap = static_cast<const QuadTreeMap*>(mNavigator.GetMap(MapName));
    //quadMap->Update();
    mMap->Update();
    //mMap->Draw(mVisualizationImage, 1, CxUtils::Point3D());

    switch (mState)
    {
    case GoToWaypoint:
        {
            //////
            // Lane lines
            //  first get the average of the angle of the lane lines
            //////

            int iterations=0;
            std::vector<CxUtils::Segment3D>::iterator iter = mLaneLines.begin();
            for (; iter != mLaneLines.end(); iter++)
            {
                // lane lines have angle with respect to top left of screen
                // o X-->
                // Y
                // |
                // v

                //laneAngle += iter->GetAngleXY();
                // I think this is right?...
                laneAngle += CxUtils::Orientation::AngleDiff(CxUtils::CX_HALF_PI, iter->GetAngleXY());
                iterations++;
            }

            if (iterations > 0) // lane lines detected, average them
            {
                laneAngle /= iterations;
            }
            else // no lane lines detected, so keep going strait
            {
                laneAngle = 0;
            }

            //////
            // Obstacle Avoidance
            //  get the heading associated with avoiding obstacles
            //////

            Cartographer::Path path;
            Cartographer::Pose myPose;
            myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);

            Cartographer::Pose destPose;
            destPose = mNavigator.CreatePose(MapName, mDesiredPosition, CxUtils::Point3D());
            //mNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, NULL);
            //Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());

            /* const Wgs& startPosition,
               const Orientation startOrientation,
               const Wgs& endPosition,
               Path& pathFound,
               Path::List * allPaths = NULL,
               const std::string& mapName = "Default");*/
            if (mNavigator.GetPathToWorldCoordinate(mCurrentPosition,
                                                    mCurrentOrientation,
                                                    mDesiredPosition,
                                                    path,
                                                    NULL,
                                                    MapName))
            {
                Cartographer::Path waypoints;
                waypoints.Push(destPose);
                mNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, &waypoints);


                if(path.Size() >= 2)
                {
                    Cartographer::Pose A;
                    path.PopFront(&A);

                    Cartographer::Pose B;
                    path.PopFront(&B);

                    angle = atan2(B.mPosition.mX - A.mPosition.mX, B.mPosition.mY - A.mPosition.mY);

                    mDesiredOrientation.mZ = angle;

                    distance = Utm::Distance(mDesiredPosition, mCurrentPosition);

                    //mDesiredVelocity = 0;
                    //if (fabs(CxUtils::Orientation::AngleDiff(mCurrentOrientation.mZ,angle)) < CxUtils::CX_PI/3.0)
                        mDesiredVelocity = mMaxVelocity;
                }
            }
            else
            {
                // No avoidance angle, so we're just going with the angle from the lane line
                mDesiredOrientation = mCurrentOrientation; // stay the course i guess
                mDesiredOrientation.mZ += laneAngle;

                //if (fabs(CxUtils::Orientation::AngleDiff(mCurrentOrientation.mZ,angle)) < CxUtils::CX_PI/3.0)
                    mDesiredVelocity = mMaxVelocity;

                printf("Persistent::Error: Unable to find path\n");
            }

            // Reached our destination, next waypoint
            if (distance < mDistanceThresh)
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

    Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());
    //Utility::Display::DisplayImage("Visualizer", mVisualizationImage);

    printf("Persistent:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, state: %d\n", distance, mState);
    printf("  %s\n", mMap->ToString().c_str());
    return "KeepRunning";
}

int Persistent::ErrorCheck()
{
    return 0;
}
