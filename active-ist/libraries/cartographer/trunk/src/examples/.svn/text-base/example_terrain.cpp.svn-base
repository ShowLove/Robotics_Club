////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_terrain.cpp
///  \brief Test/demo program on how to use the Terrain Map interface.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 6 September 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
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
#include "cartographer/maps/terrainmap.h"
#include <iostream>
#include <cxutils/keyboard.h>

//#include <vld.h>

using namespace Cartographer;

int main(int argc, char* argv[])
{
    TerrainMap map;
    double pixelsPerMeter = 1;
    double tileSize = 100;
    double mapSize = 200;
    double maxElev = 255.0;
    double minElev = 0.0;

    double elev = 0.0;
    double dist = 1.0/pixelsPerMeter;

    // Try create map from saved image, otherwise create one randomly from scratch.
    if(map.LoadFromImage("maps/terrain_map_image.jpg", pixelsPerMeter, 50, minElev, maxElev) == false)
    {
        map.Create(mapSize, tileSize, pixelsPerMeter);

        // Generate fake terrain data.        
        for(double x = map.GetBottomLeftCorner().mX, e1 = 0; x < map.GetTopRightCorner().mX; x += dist, e1 += dist)
        {
            for(double y = map.GetBottomLeftCorner().mY, e2 = 0; y < map.GetTopRightCorner().mY; y += dist, e2 += dist)
            {
                double e = e1*e2;
                if(e > TerrainMap::MaxElevation)
                {
                    e = TerrainMap::MaxElevation;
                }
                if(e < TerrainMap::MinElevation)
                {
                    e = TerrainMap::MinElevation;
                }
                if(map.SetElevation(Point3D(x, y, e)))
                {
                    if(map.GetElevation(x, y, elev) && fabs(elev - e) > 0.01)
                    {
                        std::cout << "Error: Elevation data does not match.\n";
                    }
                }
            }
        }
    }

    // Test saving and loading data.
    //map.SetSaveFile("maps/gmet/example_terrain.xml");
    //map.Save();
    //map.SaveToImage("maps/gmet/example_terrain_save.jpg");
    //map.SetSaveFile("maps/example_terrain.xml");
    //map.Save();
    //map.SaveToImage("maps/example_terrain_save.jpg");
    //map.Destroy();
    ////load from jpeg
    //map.Load("maps/example_terrain.xml",true);
    //map.SaveToImage("maps/example_terrain_save2.jpg");

#ifdef USE_OPENCV
    IplImage* mapImage;
    double drawPixelsPerMeter = 2;
    cvNamedWindow("Map");

    mapImage = cvCreateImage(cvSize((int)(map.GetMapSizeMeters()*drawPixelsPerMeter), 
        (int)(map.GetMapSizeMeters()*drawPixelsPerMeter)),
        IPL_DEPTH_8U,
        3);

    cvZero(mapImage);

    map.Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), true);
    IplImage* mapImageCopy = (IplImage *)cvClone(mapImage);

#endif

    Pose pose;
    double counter = 0;

    while(true)
    {
#ifdef USE_OPENCV
        // Copy map image so that we don't have to keep
        // re-creating it (because it's slow to draw terrain.
        cvCopy(mapImageCopy, mapImage);
        map.DrawCells(mapImage, drawPixelsPerMeter);
#endif
        // Keep moving the pose around the map.
        pose.mRotation.mZ -= 0.02;
        pose.X() = map.GetMapSizeMeters()/8*sin(counter += 0.01);
        pose.Y() = map.GetMapSizeMeters()/8*cos(counter += 0.01);

        Point3D localScanPoint(50, 0, 0);
        double scanRadius = 20;

        // Demonstrate how to convert from local to world coordinates.
        Point3D worldScanPoint = Pose::ConvertFromLocalToMapCoordinates(pose, localScanPoint);
        Point3D local = Pose::ConvertFromMapToLocalCoordinates(pose, worldScanPoint);
#ifdef USE_OPENCV
        // Draw the scan area.
        Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), worldScanPoint, scanRadius);        
#endif
        Point3D::List collisions;
        // Scan a radius in the map for terrain height values.
        if(map.ScanLocalRadiusInXY(pose, localScanPoint, scanRadius, &collisions, OBJECT_Z_HIGH_DEFAULT, (map.GetMaxElevation() - map.GetMinElevation())/3 + map.GetMinElevation()))
        {
#ifdef USE_OPENCV
            // If collision, draw
            map.Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), collisions);
#endif
            std::cout << "Pose Position: ";
            pose.mPosition.Print();
            std::cout << "Local Point: ";
            localScanPoint.Print();
            std::cout << "Collisions Detected: " << collisions.size() << std::endl;
        }

        Segment3D localSegment(Point3D(-25, -25, 0), Point3D(-25, 25, 0), 10);
        // How to convert from local to world coordinates relative to a pose.
        Segment3D worldSegment = Pose::ConvertFromLocalToMapCoordinates(pose, localSegment);

#ifdef USE_OPENCV
        // Draw scan area.
        Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), worldSegment);
#endif
        // Scan a line segment in the map for terrain height values.
        if(map.ScanLocalSegmentInXY(pose, localSegment, &collisions, OBJECT_Z_HIGH_DEFAULT, (map.GetMaxElevation() - map.GetMinElevation())/3 + map.GetMinElevation()))
        {
#ifdef USE_OPENCV
            // If collision, draw
            map.Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), collisions);
#endif
            std::cout << "Pose Position: ";
            pose.mPosition.Print();
            std::cout << "Local Point: ";
            localScanPoint.Print();
            std::cout << "Collisions Detected: " << collisions.size() << std::endl;
        }

        Point3D localTriP1(150, 0, 0), worldTriP1;
        Point3D localTriP2(100, -25, 0), worldTriP2;
        Point3D localTriP3(100, 25, 0), worldTriP3;

        // How to convert from local to world coordinates relative to a pose.
        worldTriP1 = Pose::ConvertFromLocalToMapCoordinates(pose, localTriP1);
        worldTriP2 = Pose::ConvertFromLocalToMapCoordinates(pose, localTriP2);
        worldTriP3 = Pose::ConvertFromLocalToMapCoordinates(pose, localTriP3);

#ifdef USE_OPENCV
        // Draw can area.
        Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), worldTriP1, worldTriP2, worldTriP3);
#endif
        
        // Scan a triangle in the map for terrain height values.
        if(map.ScanLocalTriangleInXY(pose, localTriP1, localTriP2, localTriP3, &collisions, OBJECT_Z_HIGH_DEFAULT, (map.GetMaxElevation() - map.GetMinElevation())/3 + map.GetMinElevation()))
        {
#ifdef USE_OPENCV
            // If collision, draw
            map.Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), collisions);
#endif
            std::cout << "Pose Position: ";
            pose.mPosition.Print();
            std::cout << "Local Point: ";
            localScanPoint.Print();
            std::cout << "Collisions Detected: " << collisions.size() << std::endl;
        }
#ifdef USE_OPENCV
        pose.Draw(mapImage, drawPixelsPerMeter, map.GetBottomLeftCorner(), 10, 5);
        // Display
        cvShowImage("Map", mapImage);
        int key = 0;
        if((key = cvWaitKey(10)) > 0)
        {
            break;
        }
#else
        CxUtils::SleepMs(10);
        if(CxUtils::GetChar() == 27)
            break;
#endif
    }

#ifdef USE_OPENCV
    cvDestroyAllWindows();
    cvReleaseImage(&mapImage);
    cvReleaseImage(&mapImageCopy);
#endif
    return 0;
}

/*  End of File */
