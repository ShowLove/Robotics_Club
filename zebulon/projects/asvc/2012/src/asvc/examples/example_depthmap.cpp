//////////////////////////////////////////////////////////////////////////////////////////
///
/// \file depthmap.cpp
/// \brief Class for creating a depth map image from lidar data.
///
/// Author(s): Jonathan Mohlenhoff, Tuyen Do<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////////

#include "asvc/surface/depthmap.h"

using namespace Zebulon;
using namespace ASVC;

int main()
{
    DepthMap mp;
    mp.Initialize("intrinsics.xml", "distortion.xml", 640, 480);

    CxUtils::Point3D::List list;
    CxUtils::Point3D point;
    point.mX = 1;
    point.mY = 0;
    point.mZ = 0;
    list.push_back(point);
    point.mX = 1;
    point.mY = -.5;
    point.mZ = 0;
    list.push_back(point);


    mp.ProcessData("Dynamixel", CxUtils::CxToRadians(126));
    mp.ProcessData("Hokuyo", list);

    while(1)
    {
        CxUtils::SleepMs(1);
    }

    return 0;
} // main method
