
#include "asvc/surface/missions/cartographerbeam.h"

using namespace Zebulon;
using namespace ASVC;

const std::string CartographerBeam::Name = "CartographerBeam";
const std::string CartographerBeam::MapName = "Default";

CartographerBeam::CartographerBeam()
{
    mForwardCamera = NULL;
    this->ID = CartographerBeam::Name;
    mMaxVelocity = 40;
}

CartographerBeam::~CartographerBeam()
{
    mpNavigator.GetMap(MapName)->Clear();
}

int CartographerBeam::Init()
{
    Mission::Init();
    MapBuilder::InitializeMaps(mpGlobalInfo, mXmlConf, Name);

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
    //mNavigator.AddMap(mMap = new Cartographer::WorldMap(), MapName);
    mpNavigator.AddMap(mMap = this->GetWorldMap(), MapName);
    mXmlConf.GetVar("Surface.CartographerBeam.Map@MapSize", mapSize);
    mXmlConf.GetVar("Surface.CartographerBeam.Map@CellSize", cellSize);
    //mMap->Create(mapSize, cellSize);
    //mNavigator.GetMap("Map")->Create(mpGlobalInfo->MapSizeMeters, mpGlobalInfo->MapCellSizeMeters);
    mXmlConf.GetVar("Surface.CartographerBeam.Map@ConsumeThreshold", mConsumeThreshold);
    //mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Surface.CartographerBeam.Drawing@PixelsPerMeter", mPixelsPerMeter);
    
    mpNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    mpNavigator.SetDrawPoseSize(.75, .25);
    
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@PathWidth", mPathWidth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@SearchDepth", mSearchDepth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@ScansPerSweep", mScansPerSweep);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@AngleScannedRads", mAngleScannedRads);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@BranchesPerDepth", mBranchesPerDepth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@EndpointThreshold", mEndpointThreshold);

    mDesiredPositionList = mpGlobalInfo->GetPositionTrail(true);
    
    mDesiredPositionIterator = mDesiredPositionList.begin();

    mState = GoToWaypoint;

    return 0;
}

int CartographerBeam::FetchProcess()
{
    static bool firstRun = true;
    
    LoadSettings();
    
    // Get current position and orientation
    mCurrentPosition = mpGlobalInfo->GetPosition();
    mCurrentOrientation = mpGlobalInfo->GetOrientation(); // Orientation in rads

    if (firstRun)
    {
        mpNavigator.GetMap(MapName)->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
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
    mpNavigator.GetMap(MapName)->Clear();
    Cartographer::Pose myPose = mpNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.05);
        mMap->AddObject(&sphere, myPose);
    }

    mpGlobalInfo->GetInfo(GlobalInfo::MainCamera, mForwardCamera);

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
            // Obstacle Avoidance
            //  get the heading associated with avoiding obstacles
            //////

            Cartographer::Path path;
            Cartographer::Pose myPose;
            myPose = mpNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
            
            Cartographer::Pose destPose;
            destPose = mpNavigator.CreatePose(MapName, mDesiredPosition, CxUtils::Point3D());

            Cartographer::Path waypoints;
            waypoints.Push(destPose);

            /* const Wgs& startPosition,
               const Orientation startOrientation,
               const Wgs& endPosition,
               Path& pathFound,
               Path::List * allPaths = NULL,
               const std::string& mapName = "Default");*/
            if (mpNavigator.GetPathToWorldCoordinate(mCurrentPosition,
                                                    mCurrentOrientation,
                                                    mDesiredPosition,
                                                    path,
                                                    NULL,
                                                    MapName))
            {
                mpNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, &waypoints);
                Utility::Display::DisplayImage(MapName, mpNavigator.GetLocalMapImage());
                
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
                // No avoidance angle, so we're just going with the angle from the lane line
                //mDesiredOrientation = mCurrentOrientation; // stay the course i guess
                angle = atan2(destPose.mPosition.mX - myPose.mPosition.mX, destPose.mPosition.mY - myPose.mPosition.mY);
                mDesiredOrientation.mZ = angle;

                distance = Utm::Distance(mDesiredPosition, mCurrentPosition);

                mDesiredVelocity = mMaxVelocity;

                mpNavigator.DrawLocalMap(MapName, 16, NULL, NULL, &myPose, &waypoints);
                Utility::Display::DisplayImage(MapName, mpNavigator.GetLocalMapImage());
                
                printf("CartographerBeam::Error: Unable to find path\n");
            }
            
            // Reached our destination, next waypoint
            if (distance < mDistanceThresh)
            {
                mDesiredVelocity = 0;
                mState = NextWaypoint;
            }
        }
        break;

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
        }
        break;

    case Finish:
        {
            mDesiredVelocity = 0;
            mDesiredOrientation.mZ = CxUtils::Orientation::AddToAngle(mCurrentOrientation.mZ, .1);
        }
        break;
    }

    mpGlobalCommand->SetForwardVelocityPercent(mDesiredVelocity);
    mpGlobalCommand->SetYaw(mDesiredOrientation.mZ);

    printf("CartographerBeam:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Path Angle: %lf, Beam Angle: %lf", distance, laneAngle, angle);
    return "KeepRunning";
}

int CartographerBeam::ErrorCheck()
{
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Updates map using information from LIDAR and any computer vision
///         data.
///
///  \param[in] clearMaps If true, old data is cleared.
///
////////////////////////////////////////////////////////////////////////////////////
void CartographerBeam::UpdateMaps(const bool clearMaps)
{
    //MapBuilder::UpdateMaps(clearMaps);
    MapBuilder::UpdateMaps(clearMaps, true);
    // Add content to maps/filter using computer vision data.
}

/** Runs GVision classification of buoys by colors. */
void CartographerBeam::RunVisionClassifier()
{

}

/** Loads settings from XML file. */
void CartographerBeam::LoadSettings()
{
    mXmlConf.GetVar("Surface.CartographerBeam.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Surface.CartographerBeam.Controls@DistanceThresh", mDistanceThresh);

    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@PathWidth", mPathWidth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@SearchDepth", mSearchDepth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@ScansPerSweep", mScansPerSweep);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@AngleScannedRads", mAngleScannedRads);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@BranchesPerDepth", mBranchesPerDepth);
    mXmlConf.GetVar("Surface.CartographerBeam.BeamSearch@EndpointThreshold", mEndpointThreshold);

    Cartographer::Navigator::Settings *settings = new Cartographer::Navigator::Settings;
    settings->Set(Cartographer::BeamSearch::PathWidthMeters, mPathWidth);
    settings->Set(Cartographer::BeamSearch::SearchDepth, mSearchDepth);
    settings->Set(Cartographer::BeamSearch::BranchLengthMeters, mSegmentLengthMeters);
    settings->Set(Cartographer::BeamSearch::ScanAngleRads, mAngleScannedRads);
    settings->Set(Cartographer::BeamSearch::BranchesPerDepth, mBranchesPerDepth);
    settings->Set(Cartographer::BeamSearch::AngleBetweenBranchesRads, mAngleSweepIncrementRads);
    settings->Set(Cartographer::BeamSearch::EndpointThresholdMeters, mEndpointThreshold);
    mpNavigator.SetSettings(settings);
}

/** Draws maps for display purposes */
void CartographerBeam::DrawMaps()
{
    MapBuilder::DrawMaps();

    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    CxUtils::Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                             mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);

    double vLength = 0, vWidth = 0;
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    Mission::mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);

    // Draw vehicle.
    mMapPose.Draw(mpNavigator.GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);

}

/*  End of File */

