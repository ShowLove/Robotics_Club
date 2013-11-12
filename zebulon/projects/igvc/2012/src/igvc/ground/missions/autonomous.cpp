
#include "igvc/ground/missions/autonomous.h"

using namespace Zebulon;
using namespace IGVC;

const std::string Autonomous::MapName = "Default";

const unsigned int Autonomous::VisionSourceID = 1;
const unsigned int Autonomous::LaserSourceID = 2;

Autonomous::Autonomous()
{
    mForwardCamera = NULL;

    mpLineAngleFilter = new Utility::AverageFilter(4);
    //mpLineAngleFilter = new Utility::LowPassFilter(1./20., 1.);

    //Cartographer::Object::Factory.AddObject(LaneLine::Name, )
}

Autonomous::~Autonomous()
{
    mMap->Destroy();
    cvReleaseImage(&mForwardCamera);
    mRangeData.clear();

    delete mpLineAngleFilter;
}

int Autonomous::Init()
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

    mNavigator.AddMap(mMap = new Cartographer::WorldMap(), MapName);
    //mNavigator.AddMap(mMap = new QuadTreeMap(), MapName);

    mXmlConf.GetVar("Ground.Autonomous.Map@MapSize", mapSize);
    mXmlConf.GetVar("Ground.Autonomous.Map@CellSize", cellSize);
    mMap->Create(mapSize, cellSize);
    //mNavigator.GetMap("Map")->Create(mpGlobalInfo->MapSizeMeters, mpGlobalInfo->MapCellSizeMeters);
    mXmlConf.GetVar("Ground.Autonomous.Map.Consume@Laser", mLaserConsumeThreshold);
    mXmlConf.GetVar("Ground.Autonomous.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);
    //mMap->SetConsumeThreshold(Cartographer::Objects::Line::Name, mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.Autonomous.Drawing@PixelsPerMeter", mPixelsPerMeter);

    mNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    mNavigator.SetDrawPoseSize(.75, .25);

    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@PathWidth", mPathWidth);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@SearchDepth", mSearchDepth);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@ScansPerSweep", mScansPerSweep);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@AngleScannedRads", mAngleScannedRads);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@BranchesPerDepth", mBranchesPerDepth);
    mXmlConf.GetVar("Ground.Autonomous.BeamSearch@EndpointThreshold", mEndpointThreshold);

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58513, -81.19908)));

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58504, -81.19901)));

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58503, -81.19907)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88953, -77.01457)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88964, -77.01487)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88992, -77.01476)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88973, -77.01455)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88962, -77.01458)));

    mDesiredPositionIterator = mDesiredPositionList.begin();

    // Start in the standby state
    mState = Standby;

    return 0;
}

int Autonomous::FetchProcess()
{
    static bool firstRun = true;

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Map
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.Autonomous.Map.Consume@Laser", mLaserConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);

    mXmlConf.GetVar("Ground.Autonomous.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    mXmlConf.GetVar("Ground.Autonomous.Map.TimeToLive@Laser", mLaserTimeToLive);
    mXmlConf.GetVar("Ground.Autonomous.Map.TimeToLive@Vision", mVisionTimeToLive);
    mMap->Update();

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Navigator
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.Autonomous.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.Autonomous.Controls@DistanceThresh", mDistanceThresh);

    Cartographer::Navigator::Settings *settings = new Cartographer::Navigator::Settings;
    settings->Set(Cartographer::BeamSearch::PathWidthMeters, mPathWidth);
    settings->Set(Cartographer::BeamSearch::SearchDepth, mSearchDepth);
    settings->Set(Cartographer::BeamSearch::BranchLengthMeters, mSegmentLengthMeters);
    settings->Set(Cartographer::BeamSearch::ScanAngleRads, mAngleScannedRads);
    settings->Set(Cartographer::BeamSearch::BranchesPerDepth, mBranchesPerDepth);
    settings->Set(Cartographer::BeamSearch::AngleBetweenBranchesRads, mAngleSweepIncrementRads);
    settings->Set(Cartographer::BeamSearch::EndpointThresholdMeters, mEndpointThreshold);

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
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.05, LaserSourceID);
        sphere.SetTimeToLive(mLaserTimeToLive);
        mMap->AddObject(&sphere, myPose);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Camera
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.Autonomous.Vision@LineAngleThresh", mLineAngleThreshold);
    mLineAngleThreshold = CxUtils::CxToRadians(mLineAngleThreshold);
    cvReleaseImage(&mForwardCamera);
    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    if(mForwardCamera != NULL)
    {
//        CvSize imageSize = cvSize(mForwardCamera->width & -2, mForwardCamera->height & -2 );
//        IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2, imageSize.height/2), mForwardCamera->depth, mForwardCamera->nChannels );
//        cvPyrDown(mForwardCamera,imgSmallCopy);
//        IplImage * imgOut = NULL;
//        CxUtils::Segment3D::List lines;
//        lines = mGroundVision.FindPerspectiveLaneLines(imgSmallCopy,imgOut);

        IplImage * imgOut = NULL;
        CxUtils::Segment3D::List lines;
        lines = mGroundVision.FindLaneLines(mForwardCamera,imgOut);

        double avgLineAngle;
        int valid = mpLineAngleFilter->Output(avgLineAngle);
        for(int i = 0; i < lines.size(); i++)
        {
            double angle = lines[i].GetAngleXY();
            if (valid
                && fabs(CxUtils::Orientation::AngleDiff(angle, avgLineAngle)) < mLineAngleThreshold)
            {
                LaneLine line(lines[i], VisionSourceID);
                line.SetTimeToLive(mVisionTimeToLive);
                mMap->AddObject(&line, myPose);
            }
            mpLineAngleFilter->Add(angle);
        }
        //LaneLine line(CxUtils::Segment3D(CxUtils::Point3D(2,0,0),CxUtils::Point3D(5,0,0)));
        //mMap->AddObject(&line, myPose);

        //Cartographer::Objects::Line line(CxUtils::Segment3D(CxUtils::Point3D(0,0,0),CxUtils::Point3D(10,0,0)));
        //mMap->AddObject(&line,myPose);

        Utility::Display::DisplayImage("OutPut", imgOut);
        Utility::Display::DisplayImage("input", mForwardCamera);
        lines.clear();
        cvReleaseImage(&imgOut);
        cvReleaseImage(&mForwardCamera);
        //cvReleaseImage(&imgSmallCopy);
    }

    return 0;
}

std::string Autonomous::ExitEarly()
{
    return "KeepRunning";
}

std::string Autonomous::Execute()
{
    double angle;
    double distance;

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

            Cartographer::Path path;
            Cartographer::Pose myPose;
            myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);

            Cartographer::Pose destPose;
            destPose = mNavigator.CreatePose(MapName, mDesiredPosition, CxUtils::Point3D());
            //mNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, NULL);
            //Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());

            Cartographer::Path waypoints;
            waypoints.Push(destPose);

            // const Wgs& startPosition,
            // const Orientation startOrientation,
            // const Wgs& endPosition,
            // Path& pathFound,
            // Path::List * allPaths = NULL,
            // const std::string& mapName = "Default");
            if (mNavigator.GetPathToWorldCoordinate(mCurrentPosition,
                                                    mCurrentOrientation,
                                                    mDesiredPosition,
                                                    path,
                                                    NULL,
                                                    MapName))
            {
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

                    mDesiredVelocity = mMaxVelocity;
                }
            }
            else
            {
                // No avoidance angle...
                ///mDesiredOrientation = mCurrentOrientation; // stay the course i guess

                // Might have to change to back up or something, maybe we'll just have it spin in place till it figures itself out...
                mDesiredVelocity = mMaxVelocity;

                mNavigator.DrawLocalMap(MapName, 16, NULL, NULL, &myPose, &waypoints);

                printf("Autonomous::Error: Unable to find path\n");
            }

            Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());

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

    printf("Autonomous:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Beam Angle: %lf\n", distance, angle);
    return "KeepRunning";
}

int Autonomous::ErrorCheck()
{
    return 0;
}

