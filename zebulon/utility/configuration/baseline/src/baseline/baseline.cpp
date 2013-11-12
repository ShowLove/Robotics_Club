/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.cpp
/// \brief This is the main file for running the Baseline program for robots.  This
///        includes common settings loading and initialization routines that
///        should be made to keep things consistent across platforms.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
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
#include "baseline/baseline.h"
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Configuration;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::DeviceInfo::DeviceInfo()
{
    mName = "Unnamed Device";
    mEnabledFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::DeviceInfo::DeviceInfo(const Baseline::DeviceInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::DeviceInfo::~DeviceInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::DeviceInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    std::string str;
    if(xml.GetVar(xmlNodeName + "@enabled", str))
    {
        result = true;
        if(str == "1" || str == "true" || str == "TRUE")
        {
            mEnabledFlag = true;
        }
        else
        {
            mEnabledFlag = false;
        }
    }

    if(xml.GetVar(xmlNodeName + "@name", str))
    {
        result = true;
        mName = str;
    }
    if(xml.GetVar(xmlNodeName + "@type", str))
    {
        result = true;
        mType = str;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::DeviceInfo& Baseline::DeviceInfo::operator=(const Baseline::DeviceInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::SerialInfo::SerialInfo()
{
    mName = "Serial Device";
    mPort = "/dev/ttyUSB0";
    mBaud = 9600;

    mDataBits = 8;
    mStopBits = 1;
    mParity = 0;
    mRequestRate = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::SerialInfo::SerialInfo(const Baseline::SerialInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::SerialInfo::~SerialInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::SerialInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(DeviceInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + "@port", mPort);
        result |= xml.GetVar(xmlNodeName + "@baud", mBaud);
        result |= xml.GetVar(xmlNodeName + "@databits", mDataBits, true);
        result |= xml.GetVar(xmlNodeName + "@stopbits", mStopBits, true);
        result |= xml.GetVar(xmlNodeName + "@parity", mParity, true);
        result |= xml.GetVar(xmlNodeName + "@requestrate", mRequestRate, true);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::SerialInfo& Baseline::SerialInfo::operator=(const Baseline::SerialInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
        mPort = info.mPort;
        mBaud = info.mBaud;
        mDataBits = info.mDataBits;
        mStopBits = info.mStopBits;
        mParity = info.mParity;
        mRequestRate = info.mRequestRate;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::NetInfo::NetInfo()
{
    mName = "Network Device";
    mPort = 80;
    mAddress = "localhost";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::NetInfo::NetInfo(const Baseline::NetInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::NetInfo::~NetInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::NetInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(DeviceInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + "@port", mPort);
        result |= xml.GetVar(xmlNodeName + "@address", mAddress);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::NetInfo& Baseline::NetInfo::operator=(const Baseline::NetInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
        mPort = info.mPort;
        mAddress = info.mAddress;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CameraInfo::CameraInfo()
{
    mName = "Camera Device";
    mPort = "/dev/video0";
    mID = 0;
    mWidth = 640;
    mHeight = 480;
    mDefaultLib = true;
    mInterlace = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CameraInfo::CameraInfo(const Baseline::CameraInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CameraInfo::~CameraInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::CameraInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(DeviceInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + "@port", mPort);
        result |= xml.GetVar(xmlNodeName + "@id", mID);
        result |= xml.GetVar(xmlNodeName + "@width", mWidth);
        result |= xml.GetVar(xmlNodeName + "@height", mHeight);
        result |= xml.GetVar(xmlNodeName + "@default_lib", mDefaultLib, true);
        result |= xml.GetVar(xmlNodeName + "@interlace", mInterlace, true);
    }
    result |= xml.GetVar(xmlNodeName + "@settingsfile", mSettingsFile, true);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CameraInfo& Baseline::CameraInfo::operator=(const Baseline::CameraInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
        mPort = info.mPort;
        mID = info.mID;
        mWidth = info.mWidth;
        mHeight = info.mHeight;
        mDefaultLib = info.mDefaultLib;
        mInterlace = info.mInterlace;
        mSettingsFile = info.mSettingsFile;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::LaserInfo::LaserInfo()
{
    mName = "Laser Device";
    mPort = "/dev/ttyUSB0";
    mBaud = 9600;
    mID = 0;
    mUnits = CM;
    mType = "Hokuyo";
    mLeftLimit = -CxUtils::CX_PI;
    mRightLimit = CxUtils::CX_PI;
    mMinDistance = 0;
    mMaxDistance = 100.0;
    mAngleIncrement = 0.090625;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::LaserInfo::LaserInfo(const Baseline::LaserInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::LaserInfo::~LaserInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::LaserInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(DeviceInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + "@port", mPort);
        result |= xml.GetVar(xmlNodeName + "@baud", mBaud, true);

        std::string str;
        if(xml.GetVar(xmlNodeName + ".UnitType", str))
        {
            result |= true;
            if(str == "cm" || str == "CM")
            {
                mUnits = CM;
            }
            else
            {
                mUnits = MM;
            }
        }
        result |= xml.GetVar(xmlNodeName + ".Position@x", mPosition.mX);
        result |= xml.GetVar(xmlNodeName + ".Position@y", mPosition.mY);
        result |= xml.GetVar(xmlNodeName + ".Position@z", mPosition.mZ);
        result |= xml.GetVar(xmlNodeName + ".Orientation@roll", mOrientation.mX);
        result |= xml.GetVar(xmlNodeName + ".Orientation@pitch", mOrientation.mY);
        result |= xml.GetVar(xmlNodeName + ".Orientation@yaw", mOrientation.mZ);
        result |= xml.GetVar(xmlNodeName + ".Orientation@pitch", mOrientation.mY);
        if(xml.GetVar(xmlNodeName + ".ScanRange@left_limit", mLeftLimit))
        {
            mLeftLimit = CxUtils::CxToRadians(mLeftLimit);
        }
        if(xml.GetVar(xmlNodeName + ".ScanRange@right_limit", mRightLimit))
        {
            mRightLimit = CxUtils::CxToRadians(mRightLimit);
        }
        xml.GetVar(xmlNodeName + ".ScanRange@min_dist", mMinDistance, true); // Optional
        xml.GetVar(xmlNodeName + ".ScanRange@max_dist", mMaxDistance, true); // Optional
        xml.GetVar(xmlNodeName + ".ScanRange@angle_res", mAngleIncrement, true); // Optional
        mAngleIncrement = CxUtils::CxToRadians(mAngleIncrement);

        xml.GetVar(xmlNodeName + "@databits", mDataBits, true);
        xml.GetVar(xmlNodeName + "@stopbits", mStopBits, true);
        xml.GetVar(xmlNodeName + "@parity", mParity, true);
        xml.GetVar(xmlNodeName + "@id", mID);
        mOrientation.mX = CxUtils::CxToRadians(mOrientation.mX);
        mOrientation.mY = CxUtils::CxToRadians(mOrientation.mY);
        mOrientation.mZ = CxUtils::CxToRadians(mOrientation.mZ);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::LaserInfo& Baseline::LaserInfo::operator=(const Baseline::LaserInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
        mPort = info.mPort;
        mID = info.mID;
        mBaud = info.mBaud;
        mUnits = info.mUnits;
        mPosition = info.mPosition;
        mOrientation = info.mOrientation;
        mMinDistance = info.mMinDistance;
        mMaxDistance = info.mMaxDistance;
        mAngleIncrement = info.mAngleIncrement;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CompassInfo::CompassInfo()
{
    mName = "Compass Device";
    mDeclinationDegrees = mDeclinationMinutes = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CompassInfo::CompassInfo(const Baseline::CompassInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CompassInfo::~CompassInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::CompassInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(SerialInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + ".Declination@degrees", mDeclinationDegrees);
        result |= xml.GetVar(xmlNodeName + ".Declination@minutes", mDeclinationMinutes);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::CompassInfo& Baseline::CompassInfo::operator=(const Baseline::CompassInfo& info)
{
    if(this != &info)
    {
        mName = info.mName;
        mEnabledFlag = info.mEnabledFlag;
        mPort = info.mPort;
        mBaud = info.mBaud;
        mDataBits = info.mDataBits;
        mStopBits = info.mStopBits;
        mParity = info.mParity;
        mDeclinationDegrees = info.mDeclinationDegrees;
        mDeclinationMinutes = info.mDeclinationMinutes;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::KinectInfo::KinectInfo()
{
    mName = "Kinect Device";
    mDesiredPitch = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::KinectInfo::KinectInfo(const Baseline::KinectInfo& info)
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::KinectInfo::~KinectInfo()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about device from XML.
///
///   \param[in] xml XML file data.
///   \param[in] xmlNodeName Node name to get data from in XML.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Baseline::KinectInfo::GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName)
{
    bool result = false;
    if(LaserInfo::GetInfo(xml, xmlNodeName))
    {
        result |= xml.GetVar(xmlNodeName + "@desired_pitch", mDesiredPitch);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Baseline::KinectInfo& Baseline::KinectInfo::operator=(const Baseline::KinectInfo& info)
{
    if(this != &info)
    {
        *( (LaserInfo *)this) = *( (LaserInfo*)&info);
        mDesiredPitch = info.mDesiredPitch;
    }
    return *this;
}


/*  End of File */
