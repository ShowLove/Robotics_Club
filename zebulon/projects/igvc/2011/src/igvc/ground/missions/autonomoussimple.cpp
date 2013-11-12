
#include "igvc/ground/missions/autonomoussimple.h"

using namespace Zebulon;
using namespace IGVC;

const std::string AutonomousSimple::MapName = "Default";

const unsigned int AutonomousSimple::VisionSourceID = 1;
const unsigned int AutonomousSimple::LaserSourceID = 2;

AutonomousSimple::AutonomousSimple()
{
    mForwardCamera = NULL;

    mpLineAngleFilter = new Utility::AverageFilter(4);
    //mpLineAngleFilter = new Utility::LowPassFilter(1./20., 1.);

    //Cartographer::Object::Factory.AddObject(LaneLine::Name, )
}

AutonomousSimple::~AutonomousSimple()
{
    mMap->Destroy();
    cvReleaseImage(&mForwardCamera);
    mRangeData.clear();

    delete mpLineAngleFilter;
}

int AutonomousSimple::Init()
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

    mXmlConf.GetVar("Ground.SimpleMission.Map@MapSize", mapSize);
    mXmlConf.GetVar("Ground.SimpleMission.Map@CellSize", cellSize);
    mMap->Create(mapSize, cellSize);
    //mNavigator.GetMap("Map")->Create(mpGlobalInfo->MapSizeMeters, mpGlobalInfo->MapCellSizeMeters);
    mXmlConf.GetVar("Ground.SimpleMission.Map.Consume@Laser", mLaserConsumeThreshold);
    mXmlConf.GetVar("Ground.SimpleMission.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);
    //mMap->SetConsumeThreshold(Cartographer::Objects::Line::Name, mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    cvNamedWindow(MapName.c_str());
    // Set draw characteristics.
    mXmlConf.GetVar("Ground.SimpleMission.Drawing@PixelsPerMeter", mPixelsPerMeter);

    mNavigator.SetDrawResolution(MapName, mPixelsPerMeter);
    //mNavigator.SetMapImageSizeMeters(35);
    mNavigator.SetDrawPoseSize(.75, .25);

    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@PathWidthMeters", mPathWidthMeters);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@AngleIncrementDegrees", mAngleIncrementDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SweepAngleStartDegrees", mSweepAngleStartDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SweepAngleEndDegrees", mSweepAngleEndDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@HistoryDepth", mHistoryDepth);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SearchDepth", mSearchDepth);

    // IST
    /*mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58513, -81.19908)));

    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58504, -81.19901)));

    //mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58503, -81.19907)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs(28.58518, -81.19903)));*/

    // Washington DC
    /*mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88953, -77.01457)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88964, -77.01487)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88992, -77.01476)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88973, -77.01455)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 38.88962, -77.01458)));*/

    // Michigan Qualifier
    /*mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 42.67820, -83.19558)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 42.67790, -83.19557)));*/

    // Michigan Simple Test
    /*mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 42.67831, -83.19547)));
    mDesiredPositionList.push_back(Utm(CxUtils::Wgs( 42.67824, -83.19521)));*/

    std::string wayPointsFile;
    mXmlConf.GetVar("Ground.SimpleMission.Waypoints@File", wayPointsFile);
    mDesiredPositionList = AI::Mission::LoadUtmWaypointsFromFile(wayPointsFile);

    mDesiredPositionIterator = mDesiredPositionList.begin();

    // Start in the standby state
    mState = Standby;

    mWaypointDebounce.Initialize(3, true);

    return 0;
}

int AutonomousSimple::FetchProcess()
{
    static bool firstRun = true;

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Map
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.SimpleMission.Map.Consume@Laser", mLaserConsumeThreshold);
    mMap->SetConsumeThreshold(Cartographer::Objects::Sphere::Name, mLaserConsumeThreshold);

    mXmlConf.GetVar("Ground.SimpleMission.Map.Consume@Vision", mVisionConsumeThreshold);
    mMap->SetConsumeThreshold(LaneLine::Name, mVisionConsumeThreshold);

    mXmlConf.GetVar("Ground.SimpleMission.Map.TimeToLive@Laser", mLaserTimeToLive);
    mXmlConf.GetVar("Ground.SimpleMission.Map.TimeToLive@Vision", mVisionTimeToLive);
    mMap->Update();

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Navigator
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.SimpleMission.Controls@MaxVelocity", mMaxVelocity);
    mXmlConf.GetVar("Ground.SimpleMission.Controls@DistanceThresh", mDistanceThresh);

    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@PathWidthMeters", mPathWidthMeters);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@AngleIncrementDegrees", mAngleIncrementDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SweepAngleStartDegrees", mSweepAngleStartDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SweepAngleEndDegrees", mSweepAngleEndDegrees);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SegmentLengthMeters", mSegmentLengthMeters);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@HistoryDepth", mHistoryDepth);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam@SearchDepth", mSearchDepth);

    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@Straightness", mStraightnessWeight);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@Distance", mDistanceWeight);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@Simplicity", mSimplicityWeight);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@Angle", mAngleWeight);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@Heading", mHeadingWeight);
    mXmlConf.GetVar("Ground.SimpleMission.HistoryBeam.Weights@LinePotential", mLinePotentialWeight);

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
    settings->Set(HistoryBeam::LinePotentialWeight, mLinePotentialWeight);

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
    mMap->ClearObjectsBySource(LaserSourceID);
    for(rawPoint = mRangeData.begin(); rawPoint != mRangeData.end(); rawPoint++)
    {
        // Remember, scan data is in "local to vehicle" coordinates, so flipped for world
        // since we are located at the center of the map (no rotation needed).
        Cartographer::Objects::Sphere sphere(*rawPoint, 0.1, LaserSourceID);
        //sphere.SetTimeToLive(mLaserTimeToLive);
        mMap->AddObject(&sphere, myPose);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    //  Camera
    ///////////////////////////////////////////////////////////////////////////////////////

    mXmlConf.GetVar("Ground.SimpleMission.Vision@LineAngleThresh", mLineAngleThreshold);
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
        //valid = mpLineAngleFilter->Output(avgLineAngle);
        bool goodLine = false;
        //Cartographer::WorldMap* wmap = dynamic_cast<Cartographer::WorldMap*>(mMap);
        mMap->ClearObjectsBySource(VisionSourceID);

        /*for(int i = 0; i < lines.size(); i++)
        {
            double angle = lines[i].GetAngleXY();
            if (valid
                && fabs(CxUtils::Orientation::AngleDiff(angle, avgLineAngle)) < mLineAngleThreshold)
            {

            }
            mLineAngle = angle;
            goodLine = true;

            LaneLine line(lines[i], VisionSourceID);
            //line.GetLine().mWidth = .1;
            line.SetTimeToLive(mVisionTimeToLive);
            mMap->AddObject(&line, myPose);

            mpLineAngleFilter->Add(angle);
        }*/

        /*double filteredAngle;
        int valid = mpLineAngleFilter->Output(filteredAngle);
        if (lines.size() > 1)// && (fabs(CxUtils::Orientation::AngleDiff(lines[0].GetAngleXY(), lines[1].GetAngleXY())) < mLineAngleThreshold))
        {
            avgLineAngle = (lines[0].GetAngleXY() + lines[1].GetAngleXY())/2.0;
            avgLineAngle = fmod(avgLineAngle,CxUtils::CX_TWO_PI);
            if(avgLineAngle > CxUtils::CX_PI)
            {
                avgLineAngle -= CxUtils::CX_TWO_PI;
            }
            else if (avgLineAngle <= -CxUtils::CX_PI)
            {
                avgLineAngle += CxUtils::CX_TWO_PI;
            }

            avgLineAngle = CxUtils::Orientation::AddToAngle(myPose.mRotation.mZ, avgLineAngle);
            //if (valid && fabs(CxUtils::Orientation::AngleDiff(avgLineAngle, filteredAngle)) < mLineAngleThreshold)
            //{
                LaneLine line1(lines[0], VisionSourceID);
                mMap->AddObject(&line1, myPose);

                LaneLine line2(lines[1], VisionSourceID);
                mMap->AddObject(&line2, myPose);

                goodLine = true;
            //}
        }
        else if (lines.size() == 1)
        {
            avgLineAngle = lines[0].GetAngleXY();

            avgLineAngle = CxUtils::Orientation::AddToAngle(myPose.mRotation.mZ, avgLineAngle);
            //if (valid && fabs(CxUtils::Orientation::AngleDiff(avgLineAngle, filteredAngle)) < mLineAngleThreshold)
            //{
                LaneLine line1(lines[0], VisionSourceID);
                mMap->AddObject(&line1, myPose);
                goodLine = true;
            //}
        }*/

        std::vector<CxUtils::Segment3D>::iterator liter;
        for (liter=lines.begin(); liter != lines.end(); liter++)
        {
            avgLineAngle += liter->GetAngleXY();

            LaneLine line(*liter, VisionSourceID);
            mMap->AddObject(&line, myPose);
            goodLine = true;
        }
        avgLineAngle /= lines.size();
        avgLineAngle = fmod(avgLineAngle,CxUtils::CX_TWO_PI);
        if(avgLineAngle > CxUtils::CX_PI)
        {
            avgLineAngle -= CxUtils::CX_TWO_PI;
        }
        else if (avgLineAngle <= -CxUtils::CX_PI)
        {
            avgLineAngle += CxUtils::CX_TWO_PI;
        }

        // convert to global coordinates
        avgLineAngle = CxUtils::Orientation::AddToAngle(myPose.mRotation.mZ, avgLineAngle);



        mpLineAngleFilter->Add(avgLineAngle);

        HistoryBeam* hnav = dynamic_cast<HistoryBeam*>(&mNavigator);
        //valid = mpLineAngleFilter->Output(avgLineAngle);
        if (hnav != NULL && goodLine)
        {
            hnav->SetLaneHeading(avgLineAngle);
        }
        else
        {
            //hnav->ResetLaneHeading();
        }

        Utility::Display::DisplayImage("OutPut", imgOut);
        //Utility::Display::DisplayImage("input", mForwardCamera);
        lines.clear();
        cvReleaseImage(&imgOut);
        cvReleaseImage(&mForwardCamera);
        //cvReleaseImage(&imgSmallCopy);
    }

    return 0;
}

std::string AutonomousSimple::ExitEarly()
{
    return "KeepRunning";
}

std::string AutonomousSimple::Execute()
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
            /*
            else if (mNavigator.GetPathToWorldCoordinate(mCurrentPosition,
                                                    mCurrentOrientation,
                                                    mDesiredPosition,
                                                    path,
                                                    &allPaths,
                                                    MapName) > 0)*/
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
                mDesiredVelocity = mMaxVelocity;

                mNavigator.DrawLocalMap(MapName, 70, NULL, NULL, &myPose, &waypoints);

                printf("AutonomousSimple::Error: Unable to find path\n");
            }



            IplImage* img = mNavigator.GetLocalMapImage();
            if (img != NULL)
            {
                if(((HistoryBeam*)&mNavigator)->IsLaneHeadingSet())
                {
                    // Draw predicted lane direction
                    Cartographer::Segment3D laneDir;
                    laneDir.mPoint2(2, 0, 0);
                    double localLaneAngle = CxUtils::Orientation::AngleDiff(myPose.mRotation.mZ, ((HistoryBeam*)&mNavigator)->GetLaneHeading());
                    laneDir = laneDir.Rotate(localLaneAngle, CxUtils::Point3D::Z);
                    Cartographer::Point3D bottomLeftCorner = myPose.mPosition;
                    bottomLeftCorner.mX -= 70/2.0;
                    bottomLeftCorner.mY -= 70/2.0;
                    Cartographer::Draw(img,
                                       this->mPixelsPerMeter,
                                       bottomLeftCorner,
                                       myPose,
                                       laneDir,
                                       CV_RGB(50, 50, 255));
                }
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

    printf("AutonomousSimple:: Heading: %lf -> %lf \n Velocity: %lf -> %lf\n", CxUtils::CxToDegrees(mCurrentOrientation.mZ), CxUtils::CxToDegrees(mDesiredOrientation.mZ), mCurrentVelocity, mDesiredVelocity);
    printf("  Distance: %lf, Beam Angle: %lf, Line Angle: %lf\n", distance, angle, mLineAngle);
    printf("  Paths found: %lf\n", distance, allPaths.size());
    return "KeepRunning";
}

int AutonomousSimple::ErrorCheck()
{
    return 0;
}
