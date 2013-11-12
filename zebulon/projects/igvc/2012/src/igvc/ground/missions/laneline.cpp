
#include "igvc/ground/missions/laneline.h"

using namespace Zebulon;
using namespace IGVC;

const std::string LaneLineMission::MapName = "Default";

LaneLineMission::LaneLineMission()
{
    mForwardCamera = NULL;
}

LaneLineMission::~LaneLineMission()
{
    mNavigator.GetMap(MapName)->Clear();
}

int LaneLineMission::Init()
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

    return 0;
}

int LaneLineMission::FetchProcess()
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


    // Get our current forward (local x) velocity
    mCurrentVelocity = mpGlobalInfo->GetVelocityX();

    // Get the data from the laser range finder
    mpGlobalInfo->GetInfo(GlobalInfo::Laser, mRangeData);

    CxUtils::Point3D::List::iterator rawPoint;
    mNavigator.GetMap(MapName)->Clear();
    Cartographer::Pose myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);
//    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
//    {
//        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
//        // since we are located at the center of the map (no rotation needed).
//        Cartographer::Objects::Sphere sphere(*rawPoint, 0.05);
//        mNavigator.GetMap(MapName)->AddObject(&sphere, myPose);
//    }

    mpGlobalInfo->GetInfo(GlobalInfo::Camera, mForwardCamera);
    if(mForwardCamera == NULL) return 0;
    CvSize imageSize = cvSize(mForwardCamera->width & -2, mForwardCamera->height & -2 );
    IplImage* imgSmallCopy = cvCreateImage( cvSize(imageSize.width/2,imageSize.height/2), mForwardCamera->depth, mForwardCamera->nChannels );
    cvPyrDown(mForwardCamera,imgSmallCopy);
    IplImage * imgOut = NULL;
    CxUtils::Segment3D::List lines;
    lines = gVision.FindLaneLines(imgSmallCopy,imgOut);
    for(int i = 0; i < lines.size(); i++)
    {
        Cartographer::Objects::Line line(lines[i]);
        mMap->AddObject(&line,myPose);
    }
    Cartographer::Objects::Line line(CxUtils::Segment3D(CxUtils::Point3D(0,0,0),CxUtils::Point3D(10,0,0)));
    mMap->AddObject(&line,myPose);
    Utility::Display::DisplayImage("OutPut", imgOut);
    Utility::Display::DisplayImage("input", imgSmallCopy);
    cvReleaseImage(&imgOut);

    return 0;
}

std::string LaneLineMission::ExitEarly()
{
    return "KeepRunning";
}

std::string LaneLineMission::Execute()
{
    double angle;
    double laneAngle;
    double distance;


    Cartographer::Path path;
    Cartographer::Pose myPose;
    myPose = mNavigator.CreatePose(MapName, mCurrentPosition, mCurrentOrientation);

    Cartographer::Path waypoints;
    mNavigator.DrawLocalMap(MapName, 16, NULL, &path, &myPose, &waypoints);
    Utility::Display::DisplayImage(MapName, mNavigator.GetLocalMapImage());



    printf("CartographerBeam:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Path Angle: %lf, Beam Angle: %lf", distance, laneAngle, angle);
    return "KeepRunning";
}

int LaneLineMission::ErrorCheck()
{
    return 0;
}
