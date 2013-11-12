////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_waypoint.cpp
///  \brief Demonstrates waypoint navigation in a randomized map. click with the
///         mouse to create a waypoint, and red point will automatically move to it.
///
///  <br>Author(s): David Adams
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include "cartographer/maps/worldmap.h"
#include "cartographer/objects/sphere.h"
#include "cartographer/planning/beamsearch.h"
#include <time.h>
#include <stdlib.h>
#include <cxutils/cxutils.h>
using namespace std;
using namespace Cartographer;

#ifdef WIN32
#ifdef _DEBUG
//#include <vld.h>
#endif
#endif

double gPixelsPerMeter = 20.0;
double gMapSize = 40.0;
double gCellSize = 5;
double gMaxRadius = 2;
int gNumObjects = 100;
unsigned int gTimeToLiveMs = 1000000;

BeamSearch gNavigator;
Path gWaypoints;
Mutex gWaypointsMutex;

/*
path search parameters
*/
double gPathWidth = .5;
int gSearchDepth = 3;
double gPathLengthMeters = 3;
double gScansPerSweep = 15;
double gScanAngleRads = CxUtils::CX_TWO_PI;
int gBranchesPerDepth = 5;
double gAngleBetweenBranchesRads = CxUtils::CxToRadians(10);
double gEndpointThreshold = 0.5;
double gLocalMapSizeMeters = 20;

#ifdef USE_OPENCV
IplImage * img = NULL;
CvScalar gCollisionColor = CV_RGB(255, 0, 0);
CvScalar gLocalPoseColor = CV_RGB(0,0,255);
CvScalar gPathColor = CV_RGB(255, 0, 0);

//converts mouse click coordinates on the image to map coordinates in meters
Point3D ConvertPixelToMap(const double pixelx, const double pixely, const double pixelsPerMeter, const Point3D& mapBottomLeft)
{
    Point3D result;
    result.mX = pixelx/pixelsPerMeter + mapBottomLeft.mX;
    result.mY = -(pixely/pixelsPerMeter + mapBottomLeft.mY);
    return result;
}

//function called when mouse is clicked on image
void MouseEvent(int event, int x, int y, int flags, void* param)
{
    if(event==CV_EVENT_LBUTTONDOWN)
    {
        Point3D point = ConvertPixelToMap(x,y,gPixelsPerMeter, gNavigator.GetObjectMap()->GetBottomLeftCorner());
        if(gNavigator.GetObjectMap()->IsCollisionInXY(point,gPathWidth))
        {
            return;
        }
        gWaypointsMutex.Lock();
        gWaypoints.Push(Pose(point));
        gWaypointsMutex.Unlock();
        cvWaitKey(1);
    }
}
#endif

//creates a sphere object with random coordinates and random bounded radius
Object* CreateRandomSphere()
{
    double x = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
    double y = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
    double r = (double)rand()/(double)RAND_MAX * gMaxRadius;
    Object * o = new Objects::Sphere(Point3D(x, y), r);
    o->SetTimeStamp(Time(true));
    o->SetTimeToLive(gTimeToLiveMs);
    return o;
}


// Creates a pose with a random x,y and orientation
Pose CreateRandomPose()
{
    double x = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
    double y = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
    double orientation = (double)rand()/(double)RAND_MAX * CxUtils::CX_TWO_PI;
    Pose pose;
    pose.mPosition.mX = x;
    pose.mPosition.mY = y;
    pose.mRotation.mZ = orientation;
    return pose;
}


//////////////////////////////////////////////////////////////////////////
///
/// To use this program, left click anwhere on the map to create a waypoint.
/// Vehicle should move to waypoint automatically.
///
/////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    Path bestPath;
    Path::List pathFoundList;

    // Create a map for exploring.
    Pose currentPose = Pose(0,0,0);
    Pose currentDest = Pose(0,0,0);
    gNavigator.AddMap(new WorldMap(), "Map");
    gNavigator.GetObjectMap("Map")->Create(gMapSize, gCellSize);
    gNavigator.GetObjectMap("Map")->SetConsumeThreshold(Objects::Sphere::Name, 0.01);

    // Seed random number generator for creating random map.
    srand((unsigned int)time(NULL));

    Object::List objects;
    // Populates the map with random sphere objects,
    // will not overlap the starting position
    for(int i = 0; i < gNumObjects; i ++)
    {
        Object* o = CreateRandomSphere();
        if(!o->IsCollisionInXY(currentPose.mPosition,gPathWidth) &&
           !o->IsCollisionInXY(currentDest.mPosition,gPathWidth))
        {
            objects.push_back(o);
        }
        else
        {
            delete o;
        }
    }

    gNavigator.GetObjectMap("Map")->AddObjects(objects);
    // Set search parameters.
    Navigator::Settings *settings = new Navigator::Settings;
    //settings.Set();
    settings->Set(BeamSearch::PathWidthMeters, gPathWidth);
    settings->Set(BeamSearch::SearchDepth,gSearchDepth);
    settings->Set(BeamSearch::BranchLengthMeters, gPathLengthMeters);
    settings->Set(BeamSearch::ScanAngleRads,gScanAngleRads);
    settings->Set(BeamSearch::BranchesPerDepth,gBranchesPerDepth);
    settings->Set(BeamSearch::AngleBetweenBranchesRads,gAngleBetweenBranchesRads);
    settings->Set(BeamSearch::EndpointThresholdMeters, gEndpointThreshold);
    gNavigator.SetSettings(settings);

    double counter = 0;

#ifdef USE_OPENCV
    int key = 0;
    cvNamedWindow("Map");
    // Register callback to get mouse events for dropping waypoints.
    cvSetMouseCallback("Map",MouseEvent,NULL);
    // Set draw characteristics.
    gNavigator.SetDrawResolution("Map", gPixelsPerMeter);
    gNavigator.SetDrawPoseSize(1.5, 1.0);
#endif

    gWaypoints.Push(currentDest);

    do
    {
        gNavigator.GetObjectMap("Map")->AddObjects(objects, true);

#ifdef USE_OPENCV
        if(bestPath.Size() > 0)
        {
            gNavigator.DrawMap("Map", &pathFoundList, &bestPath, &bestPath.First(), &gWaypoints);
            gNavigator.DrawLocalMap("Map", gLocalMapSizeMeters, &pathFoundList, &bestPath, &bestPath.First(), &gWaypoints);
        }
        else
        {
            gNavigator.DrawMap("Map", NULL, NULL, &currentPose, &gWaypoints);
            gNavigator.DrawLocalMap("Map", gLocalMapSizeMeters, NULL, NULL, &currentPose, &gWaypoints);
        }
        gNavigator.DisplayMap("Map");
        gNavigator.DisplayLocalMap("Map");
#endif
        bestPath.Clear();
        pathFoundList.clear();
        // Have we reached the waypoint?
        if(currentPose.Intersects(currentDest, gEndpointThreshold) == false)
        {
            //if we havent found it, create the best path to it, and move to the end of that path.
            if(gNavigator.GetPath(currentPose, currentDest, bestPath, &pathFoundList) > 0)
            {
                // Move to the next point on the path.
                if(bestPath.Size() > 1)
                {
                    bestPath.Get(1, currentPose);
                }
            }
        }
        // Get the next waypoint.
        else
        {
            gWaypointsMutex.Lock();
            //if we have reached our destination, get the next waypoint and set it as new currentDest.
            if(gWaypoints.Size() > 0)
            {
                while(currentPose.Intersects(gWaypoints.First(), gEndpointThreshold))
                {
                    gWaypoints.PopFront();
                    if(gWaypoints.Size() == 0)
                    {
                        break;
                    }

                }
                if(gWaypoints.Size())
                {
                    currentDest = gWaypoints.First();
                }
            }
            gWaypointsMutex.Unlock();
        }
        cout << "Number of Waypoints: " << gWaypoints.Size() << "\n";

#ifdef USE_OPENCV
    }
    while((key = cvWaitKey(500)) != 27);
#else
    }
    while(!CxUtils::Keyboard::IsKeyDown(CxUtils::Keyboard::Escape));
#endif

    ClearObjectList(objects);

    return 0;
}

/*  End of File */
