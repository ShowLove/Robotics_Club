////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_path.cpp
///  \brief Demonstrates path making and path angle offset. Press
///         spacebar to continue on to make the next path.
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
#include "cartographer/path.h"
#include "cartographer/maps/worldmap.h"
#include <time.h>
#include <stdlib.h>
#include <cxutils/cxutils.h>
#include <iostream>

using namespace std;
using namespace Cartographer;

unsigned int gTimeToLiveMs = 1000;
double gPixelsPerMeter = 20.0;
double gMapSize = 40.0;
double gCellSize = 5;
double gPathWidth = .5;

#ifdef USE_OPENCV
CvScalar gPathColor = CV_RGB(255, 0, 0);
#endif

Pose CreateRandomPose()
{
    rand();
    rand();
    rand();
    rand();
    double x = ((double)rand()/(double)RAND_MAX) * gMapSize - gMapSize/2.;
    double y = ((double)rand()/(double)RAND_MAX) * gMapSize - gMapSize/2.;
    double orientation = (double)rand()/(double)RAND_MAX * CxUtils::CX_TWO_PI;
    Pose pose;
    pose.mPosition.mX = x;
    pose.mPosition.mY = y;
    pose.mRotation.mZ = orientation;
    return pose;
}
void ExtendPath(Path& p)
{
    double d = 3;
    if(p.Size() == 0)
    {
        p.Push( CreateRandomPose());    
        return;
    }
    Pose pose = p.Last();
    
    p.Push(p.CalculateNextPoint(pose,d,CxUtils::CX_PI/10));
    
}
int main(int argc, char** argv)
{
    Path path;
    path.SetPathWidth(gPathWidth);
    WorldMap map;
    map.Create(gMapSize,gCellSize);

    srand((unsigned int)time(NULL)); // seed random # generator   

#ifdef USE_OPENCV
    cvNamedWindow("Map");  
    IplImage * img = cvCreateImage(cvSize((int)(gMapSize*gPixelsPerMeter), (int)(gMapSize*gPixelsPerMeter)), IPL_DEPTH_8U, 3);
    double fps = 0;
    Time::Stamp time = CxUtils::GetTimeMs();
    double timeElapsed = 0;
#endif

    do
    {
        ExtendPath(path);
        cout << "Path Direction: " << CxUtils::CxToDegrees(path.GetDirection()) 
             << "\nTotal Distance: " << path.GetDistance() 
             << "\nDistance from Start to End: " << path.GetDistanceStartToEnd() << "\n\n";
#ifdef USE_OPENCV
        cvZero(img);
        path.Draw(img,gPixelsPerMeter,map.GetBottomLeftCorner(),gPathColor);
        cvShowImage("Map",img);
        
    }
    while(cvWaitKey(0) != 27);
#else
        CxUtils::SleepMs(5000);
    }
    while(!CxUtils::Keyboard::IsKeyDown(CxUtils::Keyboard::Escape));
#endif
    return 0;
}