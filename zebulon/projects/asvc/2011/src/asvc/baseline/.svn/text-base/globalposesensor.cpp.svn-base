/////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalposesensor.cpp
/// \brief Global Pose Sensor for the ASV, it combines data from the vehicles
///        compass and GPS to JAUS data.
///
/// <br>Author(s): Daniel Barber<br>
/// <br>Created: 2/27/2010<br>
/// <br>Copyright (c) 2010<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: dbarber@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
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
#include "asvc/baseline/globalposesensor.h"
#include <jaus/core/component.h>
#include <xmlconfig/xmlconfigurator.h>

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPoseSensor::GlobalPoseSensor()
{    
    SetGlobalPosition(CxUtils::Wgs(28.586255, -81.199780, 0));
    SetGlobalOrientation(CxUtils::Point3D(0, 0, 0));
    mpLocalPoseSensor = NULL;
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
///  \brief Sets a reference to a local pose sensor for automatic generation of
///         local pose data.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::SetLocalPoseSensor(JAUS::LocalPoseSensor* local)
{
    JAUS::Mutex::ScopedLock lock(&mGlobalPoseSensorMutex);
    mpLocalPoseSensor = local;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback from GPS Device.
///
///  \param[in] lattitude Latitude in decimal degrees.
///  \param[in] longitude Longitude in decimal degrees.
///  \param[in] altitude Altitude in meters.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::ProcessGPSData(double latitude, double longitude, double altitude)
{
    CxUtils::Wgs wgs(latitude, longitude, altitude);
    CxUtils::Utm utm(wgs);
    // Rotate the offset in local frame, and add to global frame.
    CxUtils::Quaternion rot;
    rot.SetRotationZ(-GetGlobalPose().GetYaw());
    CxUtils::Point3D offset = rot.Rotate(mPositionOffset);
    //offset.Print();
    utm.mNorthing += offset.mX;
    utm.mEasting += offset.mY;
    utm.mElevation += offset.mZ;
    wgs << utm;
    //std::cout << wgs.ToString() << std::endl;
    SetGlobalPosition(wgs);
    if(mpLocalPoseSensor)
    {
        mpLocalPoseSensor->SetLocalPose(this->GetGlobalPose());
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback from compass that updates orientation.
///
///  \param[in] yaw Yaw in radians.
///  \param[in] pitch Pitch in radians.
///  \param[in] roll Roll in radians.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::ProcessCompassData(const double yaw, const double pitch, const double roll)
{
    // Must rotate due to mounting of
    // sensor on the compass.
    CxUtils::Point3D rotated;
    /*
    rotated.mX = pitch;
    rotated.mY = -roll;
    rotated.mZ = CxUtils::Orientation::AddToAngle(yaw, CxUtils::CX_HALF_PI, true);
    */
    // Change if compass is not mounted normal.
    rotated.Set(roll, pitch, yaw);
    
    SetGlobalOrientation(rotated);
    if(mpLocalPoseSensor)
    {
        mpLocalPoseSensor->SetLocalPose(this->GetGlobalPose());
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Callback from compass that updates orientation.
///
///  \param[in] x X position offset in meters (x is positive front of robot)
///  \param[in] y Y position offset in meters (y is positive right of robot)
///  \param[in] z Z position offset in meters (z is positive down of robot)
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalPoseSensor::LoadSettings(const std::string& filename)
{
    JAUS::GlobalPoseSensor::LoadSettings(filename);
    Zebulon::Configuration::XmlConfigurator xmlConfig;
    if(xmlConfig.Load(filename, "Baseline") == 0)
    {
        return false;
    }
    xmlConfig.GetVar("Devices.GPS.Position@x", mPositionOffset.mX, true);
    xmlConfig.GetVar("Devices.GPS.Position@y", mPositionOffset.mY, true);
    xmlConfig.GetVar("Devices.GPS.Position@z", mPositionOffset.mZ, true);
    return true;
}


/* End of File */