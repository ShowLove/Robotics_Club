////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalposesensor.cpp
///  \brief This file contains software for creating a JAUS Global Pose
///  Sensor component (ID 38) using microstrain gx1 compass.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Modified By: John Reeder<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "auvc/baseline/globalposesensor.h"
#include <math.h>
#include <cxutils/math/cxmath.h>
#include <iostream>
using namespace Zebulon;
using namespace Underwater;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPoseSensor::GlobalPoseSensor()
{  
    mOffset=0;
    SetGlobalOrientation(CxUtils::Point3D(0, 0, 0));
}

void GlobalPoseSensor::SetLiveOffset(double Offset)
{
    mOffset=Offset;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPoseSensor::~GlobalPoseSensor()
{

}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback from depth sensor that updates depth information
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::ProcessDepthData(const double depth)
{
    JAUS::GlobalPose tempGlobalPose;

    tempGlobalPose = GetGlobalPose();

    tempGlobalPose.SetAltitude(depth);

    SetGlobalPose(tempGlobalPose);

    //GlobalPose GetGlobalPose() const;

    //JAUS::ReportGlobalPose::SetAltitude(const double value);
    //std::cout << "hello" << std::endl;
    //SetGlobalOrientation(attitude);
    //std::cout << "Attitude (deg): Yaw " << CxUtils::CxToDegrees(attitude.mZ)<< " Pitch " << CxUtils::CxToDegrees(attitude.mY) << " Roll " << CxUtils::CxToDegrees(attitude.mX) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback from compass that updates orientation
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration)
{
    //std::cout << "hello" << std::endl;
    CxUtils::Point3D tempAttitude;
    /*tempAttitude.mX = attitude.mX; //CxUtils::Orientation::AddToAngle(attitude.mX, CxUtils::CX_PI, true);
    tempAttitude.mY = attitude.mY; //CxUtils::Orientation::AddToAngle(attitude.mY,-CxUtils::CxToRadians(0),true);  //was 5
    tempAttitude.mZ = attitude.mZ;*/

    //flipped around compass
    tempAttitude.mX = -attitude.mX; 
    tempAttitude.mY = -attitude.mY;
    tempAttitude.mZ = CxUtils::Orientation::AddToAngle(attitude.mZ, CxUtils::CX_PI+mOffset, true);
    
    
    SetGlobalOrientation(tempAttitude);
    //std::cout << "Attitude (deg): Yaw " << CxUtils::CxToDegrees(attitude.mZ)<< " Pitch " << CxUtils::CxToDegrees(attitude.mY) << " Roll " << CxUtils::CxToDegrees(attitude.mX) << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Prints status of sensor
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::PrintStatus() const
{

    std::cout << "------------------------------------------------------\n";
    std::cout << "Global Pose Sensor - ";
    std::cout << (int)GetComponentID().mSubsystem << "." << (int)GetComponentID().mNode << "." << (int)GetComponentID().mComponent << std::endl;
    std::cout << "Roll: " << CxUtils::CxToDegrees(GetGlobalPose().GetRoll() )<< " Pitch: " << CxUtils::CxToDegrees(GetGlobalPose().GetPitch()) << " Yaw: " << CxUtils::CxToDegrees(GetGlobalPose().GetYaw()) << std::endl;
    std::cout << "Depth: " << GetGlobalPose().GetAltitude()*39.37 << std::endl;

}

/*  End of File */