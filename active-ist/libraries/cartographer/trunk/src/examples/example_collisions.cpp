////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_collisions.cpp
///  \brief Program that tests and simulate object collisions.
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
#include "cartographer/pose.h"
#include "cartographer/maps/worldmap.h"
#include "cartographer/objects/sphere.h"
#include <time.h>
#include <stdlib.h>
#include <cxutils/cxutils.h>
using namespace std;
using namespace Cartographer;
double gPixelsPerMeter = 20.0;
double gMapSize = 40.0;
double gCellSize = 5;
double gMaxRadius = 2;

//#include <vld.h>

#ifdef USE_OPENCV
CvScalar gCollisionColor = CV_RGB(255, 0, 0);
CvScalar gLocalPoseColor = CV_RGB(0,0,255);
IplImage * img = cvCreateImage(cvSize((int)(gMapSize*gPixelsPerMeter), (int)(gMapSize*gPixelsPerMeter)), IPL_DEPTH_8U, 3);
#endif

int main(int argc, char* argv[])
{
    WorldMap map, copy;
    Object::List objects;
    Object::Collisions objectsHit;
    Pose localpose;

    copy.Create(gMapSize,gCellSize);
    copy.SetConsumeThreshold("Sphere", 0.01);
    srand((unsigned int)time(NULL));    

    
    //initialize map with random sphere objects 
    for(int i = 0; i < 100; i++)
    {
        double x = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
        double y = (double)rand()/(double)RAND_MAX * gMapSize - gMapSize/2.;
        double r = (double)rand()/(double)RAND_MAX * gMaxRadius;
        objects.push_back(new Objects::Sphere(Point3D(x, y), r));
    }
    
    copy.AddObjects(objects);
    // Delete temporary objects
    ClearObjectList(objects);

    

    // Set threshold for matching sphere data.

    cout << copy.ToString() << "\n";

    //creating various shapes to detect collisions with respect to local coords
    //Sphere
    Point3D scanOriginLocal(0, 11.0, 0.0);
    Point3D scanOriginWorld;
    double scanRadius = .5;
    
    //Line
    Segment3D scanLineLocal;
    scanLineLocal.mPoint1(0, -4, 0.0);
    scanLineLocal.mPoint2(0, -8.0, 0.0);
    scanLineLocal.mWidth = 2;
    Segment3D scanLineWorld;

    //Triangle
    Point3D scanTriangleLocalPoint1(0.0, 2.0, 0.0);
    Point3D scanTriangleLocalPoint2(5.0, 7.0, 0.0);
    Point3D scanTriangleLocalPoint3(-5.0, 7.0, 0.0); 
    Point3D scanTriangleWorldPoint1;
    Point3D scanTriangleWorldPoint2;
    Point3D scanTriangleWorldPoint3;

    localpose.mRotation.mZ = 0;

#ifdef USE_OPENCV
    cvNamedWindow("Map");
    double fps = 0;
    Time::Stamp time = CxUtils::GetTimeMs();
    double timeElapsed = 0;
    CvFont font;
    char charBuffer[64];
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5, 1.5);
#endif
    
    double counter = 0;
    do
    {

        // Test copy function out.
        copy.CopyTo(&map);
        //map.AddObjects(objects);

#ifdef USE_OPENCV
        cvZero(img);
#endif

        localpose.mRotation.mZ += .01;
        localpose.mPosition.mX = gMapSize/4. * sin(counter+=.01);
        localpose.mPosition.mY = gMapSize/4. * cos(counter+=.01);

        scanTriangleWorldPoint1 = Pose::ConvertFromLocalToMapCoordinates(localpose, scanTriangleLocalPoint1);
        scanTriangleWorldPoint2 = Pose::ConvertFromLocalToMapCoordinates(localpose, scanTriangleLocalPoint2);
        scanTriangleWorldPoint3 = Pose::ConvertFromLocalToMapCoordinates(localpose, scanTriangleLocalPoint3);
        
        scanOriginWorld = Pose::ConvertFromLocalToMapCoordinates(localpose, scanOriginLocal);

        scanLineWorld.mPoint1 = Pose::ConvertFromLocalToMapCoordinates(localpose, scanLineLocal.mPoint1);
        scanLineWorld.mPoint2 = Pose::ConvertFromLocalToMapCoordinates(localpose, scanLineLocal.mPoint2);
        scanLineWorld.mWidth = scanLineLocal.mWidth;
        
        map.ScanLocalRadiusInXY(localpose, Point3D(0,0,0), 1.0, &objectsHit);
#ifdef USE_OPENCV
        map.Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner());
        for(unsigned int i = 0; i < objectsHit.size(); i++)
        {
            objectsHit[i]->Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), gCollisionColor);
        }
#endif

        map.ScanLocalRadiusInXY(localpose, scanOriginLocal, scanRadius, &objectsHit);
#ifdef USE_OPENCV
        for(unsigned int i = 0; i < objectsHit.size(); i++)
        {
            objectsHit[i]->Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), gCollisionColor);
        }
#endif
               
        map.ScanLocalTriangleInXY(localpose, scanTriangleLocalPoint1, scanTriangleLocalPoint2, scanTriangleLocalPoint3,&objectsHit);
#ifdef USE_OPENCV
        for(unsigned int i = 0; i < objectsHit.size(); i++)
        {
            objectsHit[i]->Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), gCollisionColor);
        }
#endif

        map.ScanLocalSegmentInXY(localpose, scanLineLocal, &objectsHit);
#ifdef USE_OPENCV
        for(unsigned int i = 0; i < objectsHit.size(); i++)
        {
            objectsHit[i]->Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), gCollisionColor);
        }

        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), localpose.mPosition, 1.0, gLocalPoseColor);
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), scanOriginWorld, scanRadius);
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), scanTriangleWorldPoint1, scanTriangleWorldPoint2, scanTriangleWorldPoint3);
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), scanLineWorld);
        
        map.DrawCells(img, gPixelsPerMeter);
       
        fps += 1;
        timeElapsed = (CxUtils::GetTimeMs() - time)/1000.0;
      
        if(timeElapsed > 10000)
        {
            time = CxUtils::GetTimeMs();
            timeElapsed = 0;
            fps = 0;
        }
        sprintf(charBuffer, "%.2lf", (fps/timeElapsed));
        cvPutText(img,
                  charBuffer,
                  cvPoint(10,img->height-10),
                  &font,
                  CV_RGB(255,255,0));
        cvShowImage("Map",img);
    }
    
    while(cvWaitKey(1) != 27);
#else
        CxUtils::SleepMs(10);
    }
    while(!CxUtils::Keyboard::IsKeyDown(CxUtils::Keyboard::Escape));
#endif
    ClearObjectList(objects);

    return 0;
}