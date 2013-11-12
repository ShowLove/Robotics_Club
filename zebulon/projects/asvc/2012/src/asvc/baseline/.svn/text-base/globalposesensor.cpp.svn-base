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
#include <boost/algorithm/string.hpp>

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
    mpGX3Callback = new DataCallback(GX3,this);  ///< Callback for gx3.
    mpDGPSCallback = new DataCallback(DGPS,this); ///< Callback for differential gps(Novatel).
    mpGPSCallback = new DataCallback(GPS,this);   ///< Callback for 2011 boat gps.
    mpCompassCallback = new DataCallback(Compass,this);   ///< Callback from compass.
    mLoggingFlag = false;
    mLogFile = NULL;
    mPrimaryGPS= DGPS;
    mTimeSinceLastPrimaryGPS.SetCurrentTime();
    mPrimaryGPSTimeoutMS = 10000; //10 seconds
    mUseAnyGPSFlag = false;
    mPrimaryCompass = GX3;
    mHaveCompassFlag = false;
    mHaveGx3CompassDataFlag = false;
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
///   \brief Event called when new yaw pitch and roll values are available.
///
///   \param yaw Updated yaw value in radians.
///   \param pitch Updated pitch value in radians.
///   \param roll Updates roll value in radians.
///   \param source Source type.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::DataCallback::ProcessCompassData(const double yaw, const double pitch, const double roll)
{
    // std::cout << "Compass Data " << yaw << " " << pitch << " " << roll << std::endl;
    mpGlobalPoseSensor->ProcessCompassData(yaw,pitch,roll, mType);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new attitude gyroscope and acceleration values are available.
///
///   \param attitude Updated attitude value
///   \param gyroscope Updated pitch gyroscope
///   \param acceleration Updates acceleration value
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::DataCallback::ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration)
{

    //std::cout << "IMU Data Attitude: "; attitude.Print();
    //std::cout << "IMU Data Gyroscope: ";  gyroscope.Print();
    //std::cout << "IMU Data Acceleration: "; acceleration.Print();
    mpGlobalPoseSensor->ProcessIMUData(attitude,gyroscope,acceleration, mType);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new gps values are available.
///
///   \params latitude Updated latitude value
///           longitude Updated longitude value
///           altitude Updates altitude value
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::DataCallback::ProcessGPSData(double latitude, double longitude, double altitude)
{
    //std::cout << "GPS Data " << latitude << " " << longitude << " " << altitude << std::endl;
    mpGlobalPoseSensor->ProcessGPSData(latitude,longitude,altitude, mType);

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new ins values are available.
///
///   \param LLHposition Updated Lat, Long, Height from filter (like gps)
///   \param NEDVelocity Updated North, East, and Down velocity from filter
///   \param Orientation Updates Yaw, Pitch and Roll from filter
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::DataCallback::ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation)
{

    //std::cout << "INS Data Position: "; LLHposition.Print();
    //std::cout << "INS Data Velocity: ";  NEDVelocity.Print();
    //std::cout << "INS Data Orientation: ";  Orientation.Print();
    mpGlobalPoseSensor->ProcessINSData(LLHposition,NEDVelocity,Orientation,GlobalPoseSensor::INS);

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new yaw pitch and roll values are available.
///
///   \param yaw Updated yaw value in radians.
///   \param pitch Updated pitch value in radians.
///   \param roll Updates roll value in radians.
///   \param source Source type.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::ProcessCompassData(const double yaw, const double pitch, const double roll, CBType source)
{

    // Must rotate due to mounting of
    // sensor on the compass.
    CxUtils::Point3D rotated;

    // Change if compass is not mounted normal.
    if(source == Compass)
    {
        rotated.Set(pitch, -roll, CxUtils::Orientation::AddToAngle(yaw,CxUtils::CX_PI/2.0));
    }
    else if(source == GX3)
    {
        rotated.Set(CxUtils::Orientation::AddToAngle(roll, CxUtils::CX_PI), pitch, CxUtils::Orientation::AddToAngle(yaw, CxUtils::CX_PI));
    }

    if(mLogFile != NULL)
    {
        mLogMutex.Lock();
        fprintf(mLogFile, "compassdata, %llu,",CxUtils::Time::GetUtcTimeMs());
        fprintf(mLogFile, "%d, %20.16lf, %20.16lf, %20.16lf\n",(int)source, rotated.mX, rotated.mY, rotated.mZ);
        mLogMutex.Unlock();
    }
    if(source == Compass)
    {
        CxUtils::Mutex::ScopedLock lock(&mGlobalPoseSensorMutex);
        mHaveCompassFlag = true;
        mCompassData = rotated;
    }
    if(source == GX3)
    {
        CxUtils::Mutex::ScopedLock lock(&mGlobalPoseSensorMutex);
        mHaveGx3CompassDataFlag = true;
        mGx3CompassData = rotated;
    }
    if(source != mPrimaryCompass)
    {
        // Don't save values to JAUS services.
        return;
    }

    SetGlobalOrientation(rotated);
    if(mpLocalPoseSensor)
    {
        mpLocalPoseSensor->SetLocalPose(this->GetGlobalPose());
    }

}

void GlobalPoseSensor::ProcessGPSData(double latitude, double longitude, double altitude, CBType source)
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

    if(mLogFile != NULL)
    {
        mLogMutex.Lock();
        fprintf(mLogFile, "gpsdata, %llu,",CxUtils::Time::GetUtcTimeMs());
        fprintf(mLogFile, "%d, %20.16lf, %20.16lf, %20.16lf\n",(int)source,wgs.mLatitude,wgs.mLongitude,wgs.mElevation);
        mLogMutex.Unlock();
    }
    if(source == mPrimaryGPS || mUseAnyGPSFlag)
    {

        if(mPrimaryGPS == source) mTimeSinceLastPrimaryGPS.SetCurrentTime();

        //std::cout << wgs.ToString() << std::endl;
        SetGlobalPosition(wgs);
        if(mpLocalPoseSensor)
        {
            mpLocalPoseSensor->SetLocalPose(this->GetGlobalPose());
        }
    }

}

void GlobalPoseSensor::ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration,CBType source)
{

}

void GlobalPoseSensor::ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation,CBType source)
{
    CxUtils::Wgs wgs(LLHposition.mX, LLHposition.mY, LLHposition.mZ);
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

    if(mPrimaryGPS == source || mUseAnyGPSFlag)
    {
        if(mPrimaryGPS == source) mTimeSinceLastPrimaryGPS.SetCurrentTime();

        SetGlobalPosition(wgs);
        if(mpLocalPoseSensor)
        {
            mpLocalPoseSensor->SetLocalPose(this->GetGlobalPose());
        }
    }
}

void GlobalPoseSensor::CheckServiceStatus(const unsigned int timeSinceLastUpdateMs)
{
    int timediff = CxUtils::Time::DifferenceInMs(CxUtils::Time(true),mTimeSinceLastPrimaryGPS);
    if(timediff > mPrimaryGPSTimeoutMS)
    {
        mUseAnyGPSFlag = true;
    }
    else
    {
        mUseAnyGPSFlag = false;
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
    std::string val;
    if(xmlConfig.GetVar("Devices@primary_gps", val, true))
    {
        if (boost::iequals(val, "any"))
        {
            mUseAnyGPSFlag = true;
        }
        if (boost::iequals(val, "dgps"))
        {
            mPrimaryGPS = DGPS;
        }
        if (boost::iequals(val, "ins"))
        {
            mPrimaryGPS = INS;
        }
        if (boost::iequals(val, "gx3"))
        {
            mPrimaryGPS = GX3;
        }
    }
    xmlConfig.GetVar("Devices@primary_gps_timeout", mPrimaryGPSTimeoutMS, true);
    if(xmlConfig.GetVar("Devices@primary_compass", val, true))
    {
        if (boost::iequals(val, "any"))
        {
            mPrimaryCompass = GX3;
        }
        if (boost::iequals(val, "gx3"))
        {
            mPrimaryCompass = GX3;
        }
        if (boost::iequals(val, "generic"))
        {
            mPrimaryCompass = Compass;
        }
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::PrintStatus() const
{
    JAUS::GlobalPoseSensor::PrintStatus();

    if(mHaveCompassFlag)
    {
        std::cout << "Generic Compass Values: ";
        JAUS::Point3D degrees;
        {
            CxUtils::Mutex::ScopedLock lock(&mGlobalPoseSensorMutex);
            degrees = mCompassData;
        }
        degrees.mX = CxUtils::CxToDegrees(degrees.mX);
        degrees.mY = CxUtils::CxToDegrees(degrees.mY);
        degrees.mZ = CxUtils::CxToDegrees(degrees.mZ);
        degrees.Print();
    }
    if(mHaveGx3CompassDataFlag)
    {
        std::cout << "GX3-45 Compass Values: ";
        JAUS::Point3D degrees;
        {
            CxUtils::Mutex::ScopedLock lock(&mGlobalPoseSensorMutex);
            degrees = mGx3CompassData;
        }
        degrees.mX = CxUtils::CxToDegrees(degrees.mX);
        degrees.mY = CxUtils::CxToDegrees(degrees.mY);
        degrees.mZ = CxUtils::CxToDegrees(degrees.mZ);
        degrees.Print();
    }

    std::string val;
    switch(mPrimaryGPS)
    {
    case DGPS:
        val = "DGPS";
        break;
    case INS:
        val = "INS";
        break;
    case GX3:
        val = "GX3";
        break;
    }
    std::cout << "Primary GPS: " << val << " Primary Available?: " << (!mUseAnyGPSFlag ? "True" : "False") << std::endl;
    std::string compassUsed = "GX-1";
    switch(mPrimaryCompass)
    {
    case GX3:
        compassUsed = "GX3-45";
        break;
    default:
        compassUsed = "GX-1";
        break;
    }
    std::cout << "Primary Compass: " << compassUsed << std::endl;
}

/* End of File */
