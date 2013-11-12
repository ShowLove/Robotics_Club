
#include "igvc/ground/missions/cartographerbeam.h"

using namespace Zebulon;
using namespace IGVC;

const std::string CartographerBeam::MapName = "Default";

CartographerBeam::CartographerBeam()
{
    mForwardCamera = NULL;
}

CartographerBeam::~CartographerBeam()
{
    mNavigator.GetMap(MapName)->Clear();
}

int CartographerBeam::Init()
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
    mXmlConf.GetVar("Ground.CartographerBeam.Map@MapSize", mapSize);
    mXmlConf.GetVar("Ground.CartographerBeam.Map@CellSize", cellSize);
    mMap->Create(mapSize, cellSize);
    //mNavigator.GetMap("Map")->Create(mpGlobalInfo->MapSizeMeters, mpGlobalInfo->MapCellSizeMeters);
    mXmlConf.GetVar("Ground.CartographerBeam.Map@ConsumeThreshold", mConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.CartographerBeam.Drawing@PixelsPerMeter", mPixelsPerMeter);
    
    mNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    mNavigator.SetDrawPoseSize(.75, .25);
    
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@PathWidth", mPathWidth);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@SearchDepth", mSearchDepth);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@ScansPerSweep", mScansPerSweep);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@AngleScannedRads", mAngleScannedRads);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@BranchesPerDepth", mBranchesPerDepth);
    mXmlConf.GetVar("Ground.CartographerBeam.BeamSearch@EndpointThreshold", mEndpointThreshold);




    // Waypoints to go to, this is kinda a hack for now
    /** DC
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88979, -77.01469)));
    
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88968, -77.01466)));
    
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(38.88965, -77.01480)));
/*
    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58521, -81.19915)));

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58518, -81.19906)));

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58507, -81.19904)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58513, -81.19908)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58504, -81.19901)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58503, -81.19907)));*/

    
    mDesiredPositionIterator = mDesiredPositionList.begin();

    mState = GoToWaypoint;

    return 0;
}

int CartographerBeam::FetchProcess()
{
    static bool firstRun = true;
    
    mXmlConf.GetVar("Ground.WayPointList.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.WayPointList.Controls@DistanceThresh", mDistanceThresh);

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
        firstRun = false;
    }

    // Get our desired destination waypoint
    double northing, easting;
    int zoneNum, zoneLetter;
    mDesiredPositionIterator->Get(northing, easting, zoneNum, zoneLetter);
    mDesiredPosition(northing, easting, zoneNum, zoneLetter);

    // Get our current forward (local x) velocity
    mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    // Get the data from the laser range finder
    mpGlobalInfo->GetInfo(GlobalInfo::Laser, mRangeData);

    CxUtils::Point3D::List::iterator rawPoint;
    mNavigator.GetMap(MapName)->Clear();
    Cartographer::Pose myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.05);
        mMap->AddObject(&sphere, myPose);
    }

    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    //static std::vector<CxUtils::Segment3D> GroundVision::FindLaneLines(const IplImage* imgIn, IplImage* imgOut = NULL, int rows = 0, int cols = 2)
    
    //mLaneLines = IGVC::GroundVision::FindLaneLines(mForwardCamera);

    return 0;
}

std::string CartographerBeam::ExitEarly()
{
    return "KeepRunning";
}

std::string CartographerBeam::Execute()
{
    double angle;
    double laneAngle;
    double distance;

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

            Cartographer::Path waypoints;
            waypoints.Push(destPose);

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
                //Cartographer::WorldMap* worldMap = (Cartographer::WorldMap*)mNavigator.GetMap(MapName);
                //worldMap->ScanSegmentInXY(*path.GetSegments().begin(), 0);

                mNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, &waypoints);
                Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());
                
                if(path.Size() >= 2)
                {
                    Cartographer::Pose A;
                    path.PopFront(&A);

                    Cartographer::Pose B;
                    path.PopFront(&B);

                    angle = atan2(B.mPosition.mX - A.mPosition.mX, B.mPosition.mY - A.mPosition.mY);
                    //path.GetSegments().begin()->GetAngleXY();
                    //CxUtils::Orientation::AddToAngle(mCurrentOrientation.mZ, path.GetSegments().begin()->GetAngleXY());
                    //atan2(B.mPosition.mX - A.mPosition.mX, B.mPosition.mY - A.mPosition.mY);//atan2(B.mPosition.mY - A.mPosition.mY, B.mPosition.mX - A.mPosition.mX);
                    

                    // Do I need to do this?
                    //delete A;
                    //delete B;

                    // Average the avoidance angle with the lane line angle
                    //mDesiredOrientation = mCurrentOrientation;
                    //mDesiredOrientation.mZ += (angle + laneAngle)/2;
                    
                    //mDesiredOrientation = (angle + CxUtils::Orientation::AddToAngle(mCurrentOrientation.mZ, laneAngle))/2;
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

                mNavigator.DrawLocalMap(MapName, 16, NULL, NULL, &myPose, &waypoints);
                Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());
                
                printf("CartographerBeam::Error: Unable to find path\n");
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

    printf("CartographerBeam:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Path Angle: %lf, Beam Angle: %lf", distance, laneAngle, angle);
    return "KeepRunning";
}

int CartographerBeam::ErrorCheck()
{
    return 0;
}
