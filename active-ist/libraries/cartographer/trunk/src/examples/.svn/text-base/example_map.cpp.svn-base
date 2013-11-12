////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_map.cpp
///  \brief Demonstrates adding random objects to map that are removed over time.
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
#include <time.h>
#include <stdlib.h>
#include <cxutils/cxutils.h>
using namespace std;
using namespace Cartographer;

//#include <vld.h>

double gPixelsPerMeter = 20.0;
double gMapSize = 40.0;
double gCellSize = 5;
double gMaxRadius = 2;
#ifdef USE_OPENCV
CvScalar gCollisionColor = CV_RGB(255, 0, 0);
CvScalar gLocalPoseColor = CV_RGB(0,0,255);
#endif
unsigned int gTimeToLiveMs = 1000;

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

int main(int argc, char* argv[])
{
    WorldMap map;
    Object::List objects;
    Object::Collisions objectsHit;
    Pose localpose;
    Object * o;
    map.Create(gMapSize,gCellSize);
    map.SetConsumeThreshold("Sphere", 0.01);
    srand((unsigned int)time(NULL));
    map.AddObjects(objects);
    cout << map.ToString() << "\n";
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

    do
    {
        o = CreateRandomSphere();
        map.AddObject(o);
        map.Update();
        delete(o);
        //creates a clear area in a section of the map
        map.Clear(Point3D(-5.0, 5.0, 0.0), 5.0);

#ifdef USE_OPENCV
        fps += 1;
        timeElapsed = (CxUtils::GetTimeMs() - time)/1000.0;

        if(timeElapsed > 10)
        {
            time = CxUtils::GetTimeMs();
            timeElapsed = 0;
            fps = 0;
            cout << "Number objects in map: " << map.GetObjects()->size() << "\n";
            //counter = 0;
        }
        sprintf(charBuffer, "%.2lf", (fps/timeElapsed));
        cvZero(img);
        map.Draw(img, gPixelsPerMeter, map.GetBottomLeftCorner(), false);
        map.DrawCells(img,gPixelsPerMeter);
        cvPutText(img,
                  charBuffer,
                  cvPoint(10,img->height-10),
                  &font,
                  CV_RGB(255,255,0));
        cvShowImage("Map",img);
    }
    while(cvWaitKey(1) != 27);
#else
    }
    while(!CxUtils::Keyboard::IsKeyDown(CxUtils::Keyboard::Escape));
#endif
    return 0;
}

/*  End of File */
