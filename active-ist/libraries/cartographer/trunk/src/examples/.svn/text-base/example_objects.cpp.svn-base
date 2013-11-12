////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_objects.cpp
///  \brief Test/demo program on how to create/manipulate objects.
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
#include <iostream>
#include "cartographer/object.h"
#include "cartographer/pose.h"
#include "cartographer/objects/sphere.h"
#include "cartographer/objects/line.h"
#include "cartographer/boundingbox.h"

using namespace std;
using namespace Cartographer;
int main(int argc, char* argv[])
{
    // Test conversion of points between local and map/global coordinates.
    Pose pose;
    
    Objects::Sphere sphere;
    sphere.SetOrigin(Point3D(0, 0, 0));
    sphere.SetRadius(2);

    Objects::Line line;
    line.GetLine().mPoint1(10, 10, 0);
    line.GetLine().mPoint2(20, 20, 0);
    line.GetLine().mWidth = 2;
    
#ifdef USE_OPENCV
    double pixelsPerMeter = 10.0;
    IplImage * img = cvCreateImage(cvSize(640,640), IPL_DEPTH_8U, 3);
    Point3D mapBottomCorner(-img->width/pixelsPerMeter/2.0, -img->height/pixelsPerMeter/2.0, 0);
    cvNamedWindow("Objects",0);

    sphere.Draw(img, pixelsPerMeter, mapBottomCorner);
    line.Draw(img, pixelsPerMeter, mapBottomCorner);

    BoundingBox bbox = line.GetBoundingBox();

    Draw(img, pixelsPerMeter, mapBottomCorner, bbox);

#endif
    // Set some position in 3D space and
    // an Orientation.
    pose.mPosition(-10.0, -10.0, 0.0);
    pose.mRotation(0, 0, CxUtils::CxToRadians(45)); // 0 roll, 0 pitch, 45 degree yaw.

#ifdef USE_OPENCV
    pose.Draw(img, pixelsPerMeter, mapBottomCorner, 2, 1.5);
#endif
    
    Point3D localPoint; // Point relative to the pose (local to object or vehicle).
    Point3D globalPoint;// Point in global map space (relative to map origin).
    
    localPoint(1, 0, 0);// 1 meter in front of pose.
    
    // Check conversion to/from map and local coordinates.
    localPoint.Print();
    globalPoint = Pose::ConvertFromLocalToMapCoordinates(pose, localPoint);
    globalPoint.Print();
    localPoint = Pose::ConvertFromMapToLocalCoordinates(pose, globalPoint);
    localPoint.Print(); 

#ifdef USE_OPENCV
    cvShowImage("Objects",img);
    cvWaitKey(0);
#endif
    return 0;
}

/*  End of File */
