////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_pose.cpp
///  \brief 
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

#ifdef USE_OPENCV
CvScalar gCollisionColor = CV_RGB(255, 0, 0);
CvScalar gLocalPoseColor = CV_RGB(0,0,255);
#endif

int main(int argc, char* argv[])
{
    WorldMap map;
    map.Create(gMapSize,gCellSize);
        
    // Our current position.
    // This will be in map coordinates.
    // In other words it will be our global
    // position on the map relative to the map's
    // origin.
    Pose localpose; 

    // Create a circle, line segment, and triangle.
    // The position and coordinates of these shapes are 
    // relative to our local position on the map. They 
    // are in are "local coordinates".
    //
    // Local coordinates are described as follows:
    // X: positive in front - negative behind
    // Y: positive to right - negative to left
    // Z: positive below - negative above

    Point3D circleOrigin(11.0, 0, 0.0);
    double circleRadius = .5;

    Segment3D line;
    line.mPoint1(0.0, 0.0, 0.0);
    line.mPoint2(7.0, 0, 0.0);
    line.mWidth = 2;

    Point3D trianglePt1(0.0, 0.0, 0.0);
    Point3D trianglePt2(5.0, 5.0, 0.0);
    Point3D trianglePt3(5.0, -5.0, 0.0);

    Segment3D worldScanLine;

    localpose.mRotation.mZ = 0;
    double counter = 0;

#ifdef USE_OPENCV
    cvNamedWindow("Map");
    IplImage * img = cvCreateImage(cvSize((int)(gMapSize*gPixelsPerMeter), (int)(gMapSize*gPixelsPerMeter)), IPL_DEPTH_8U, 3);
    cvZero(img);
    double fps = 0;
    Time::Stamp time = CxUtils::GetTimeMs();
    double timeElapsed = 0;
    CvFont font;
    char charBuffer[64];
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.5, 1.5);
#endif

    Point3D circleGlobal;
    Segment3D lineGlobal;
    Point3D trianglePt1Global;
    Point3D trianglePt2Global;
    Point3D trianglePt3Global;
    
    // Loop: 
    // 1. move current position
    // 2. update shape positions
    // 3. draw
    do
    {
        // Move our position around the map
        localpose.mRotation.mZ += .01;
        localpose.mPosition.mX = 5 * sin(counter+=.01);
        localpose.mPosition.mY = 5 * cos(counter+=.01);

        // The Pose class contains some useful methods for converting
        // local points to global points on the map. We will convert our
        // shape points in order to draw them.
        circleGlobal = Pose::ConvertFromLocalToMapCoordinates(localpose, circleOrigin);
        
        lineGlobal.mPoint1  = Pose::ConvertFromLocalToMapCoordinates(localpose, line.mPoint1);
        lineGlobal.mPoint2  = Pose::ConvertFromLocalToMapCoordinates(localpose, line.mPoint2);
        lineGlobal.mWidth   = line.mWidth;
        
        trianglePt1Global = Pose::ConvertFromLocalToMapCoordinates(localpose, trianglePt1);
        trianglePt2Global = Pose::ConvertFromLocalToMapCoordinates(localpose, trianglePt2);
        trianglePt3Global = Pose::ConvertFromLocalToMapCoordinates(localpose, trianglePt3);

#ifdef USE_OPENCV
        // Draw circle
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), circleGlobal, circleRadius);
        // Draw line segment
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), lineGlobal);
        // Draw triangle
        Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), trianglePt1Global, trianglePt2Global, trianglePt3Global);
        //Draw local pose
        localpose.Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(),1,1);

        /////////////////////////////////
        // Calculate Frames Per Second
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
                  CV_RGB(0,255,255));
        /////////////////////////////////

        cvShowImage("Map",img);
        cvZero(img);
    }
    while(cvWaitKey(2) != 27);
#else
    }
    while(!CxUtils::Keyboard::IsKeyDown(CxUtils::Keyboard::Escape));
#endif

    return 0;
}
