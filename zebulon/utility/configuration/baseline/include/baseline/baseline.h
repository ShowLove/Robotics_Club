/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.h
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
#ifndef __ZEBULON_CONFIGURATION_BASELINE__H
#define __ZEBULON_CONFIGURATION_BASELINE__H

#include <jaus/core/component.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/extras/video/visualsensor.h>
#include <jaus/extras/rangesensor/rangesensorconfig.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <xmlconfig/xmlconfigurator.h>
#include <string>

// DLL Export Compiler flag for MSVC
#ifndef ZEB_BASELINE_UTIL_DLL
    #ifdef WIN32
        #define ZEB_BASELINE_UTIL_DLL __declspec(dllimport)
    #else
        #define ZEB_BASELINE_UTIL_DLL
    #endif
#endif


namespace Zebulon
{
    namespace Configuration
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Baseline
        ///   \brief Baseline class for all Robots developed by Robotics Club.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_BASELINE_UTIL_DLL Baseline 
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class DeviceInfo
            ///   \brief Helper data structure to store XML information about a device such
            ///          as a sensor, or motor interface.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL DeviceInfo
            {
            public:
                DeviceInfo();
                DeviceInfo(const DeviceInfo& info);
                virtual ~DeviceInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string&   );
                DeviceInfo& operator=(const DeviceInfo& info);
                std::string mName;  ///<  Name of the device
                bool mEnabledFlag;  ///<  Is the device enabled or disabled?
                std::string mType;  ///<  Abstract type of device to load from factory
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class SerialInfo
            ///   \brief Stores information about a serial device.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL SerialInfo : public DeviceInfo
            {
            public:
                SerialInfo();
                SerialInfo(const SerialInfo& info);
                virtual ~SerialInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                SerialInfo& operator=(const SerialInfo& info);
                std::string mPort;      ///<  Serial port name.
                int mBaud;              ///<  BAUD speed.
                int mDataBits;          ///<  How many bits of data.
                int mStopBits;          ///<  How many stop bits.
                int mParity;            ///<  Parity value (0 = none, 1 = event, 2 = odd).
                int mRequestRate;       ///<  Needed for the depth sensor
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class NetInfo
            ///   \brief Stores information about a network device
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL NetInfo : public DeviceInfo
            {
            public:
                NetInfo();
                NetInfo(const NetInfo& info);
                virtual ~NetInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                NetInfo& operator=(const NetInfo& info);
                int mPort;                  ///<  Network port number.
                std::string mAddress;       ///<  Network address(IP, etc).
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Stores information about a camera device.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL CameraInfo : public DeviceInfo
            {
            public:
                CameraInfo();
                CameraInfo(const CameraInfo& info);
                virtual ~CameraInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                CameraInfo& operator=(const CameraInfo& info);
                std::string mPort;      ///<  Camera port name.
                int mID;                ///<  Camera ID.
                int mWidth;             ///<  Camera image width
                int mHeight;            ///<  Camera image height
                int mDefaultLib;        ///<  Use default capture library, otherwise alternative (true by default).
                int mInterlace;         ///<  For DV cameras, do de-interlacing (true bye default).
                std::string mSettingsFile; ///< Optional Settings file parameter.
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class LaserInfo
            ///   \brief Stores information about a LIDAR device.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL LaserInfo : public SerialInfo
            {
            public:
                enum UnitType
                {
                    MM = 0,
                    CM
                };
                LaserInfo();
                LaserInfo(const LaserInfo& info);
                virtual ~LaserInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                LaserInfo& operator=(const LaserInfo& info);
                int mID;                        ///<  Laser ID.
                double mLeftLimit;              ///<  Angle in radians of left limit (-PI max).
                double mRightLimit;             ///<  Angle in radians of right limit (PI max).
                double mMinDistance;            ///<  Minimum distance for data (anything closer should be ignored) (meters).
                double mMaxDistance;            ///<  Maximum distance for data (meters).
                double mAngleIncrement;         ///<  Horizontal angle between points (radians).
                UnitType mUnits;                ///<  Units of measurement.
                CxUtils::Point3D mPosition;     ///<  Location on robot (robot coordinate frame (x+ = front, y+ = right, z+ = down).
                CxUtils::Point3D mOrientation;  ///<  Orientation on robot (x = roll, y = pitch, z = yaw).
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class CompassInfo
            ///   \brief Stores information about a compass device.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL CompassInfo : public SerialInfo
            {
            public:
                CompassInfo();
                CompassInfo(const CompassInfo& info);
                virtual ~CompassInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                CompassInfo& operator=(const CompassInfo& info);
                double mDeclinationDegrees;     ///<  Declination angle degrees value.
                double mDeclinationMinutes;    ///<  Declination angle minutes value.
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class KinectInfo
            ///   \brief Stores information about the Microsoft XBOX Kinect.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_BASELINE_UTIL_DLL KinectInfo : public LaserInfo
            {
            public:
                KinectInfo();
                KinectInfo(const KinectInfo& info);
                virtual ~KinectInfo();
                virtual bool GetInfo(XmlConfigurator& xml, const std::string& xmlNodeName);
                KinectInfo& operator=(const KinectInfo& info);
                int mDesiredPitch;     ///<  Desired pitch to set for tilt angle of Kinect (degrees).
            };
            Baseline() {}
            virtual ~Baseline() {}
            /** Load program/device settings from XML file. */
            virtual bool LoadSettings(const std::string& xmlFilename) = 0;
            /** Create JAUS services and components and configure. */
            virtual bool SetupJAUS() = 0;
            /** Connect to sensors and other hardware (e.g. motors). */
            virtual bool InitializeHardware(std::string& errorMessage) = 0;
            /** Initialize JAUS. */
            virtual bool InitializeJAUS(std::string& errorMessage) = 0;
            /** Shutdown JAUS. */
            virtual void ShutdownJAUS() = 0;
            /** Shutdown sensors and other hardware (e.g. motors) */
            virtual void ShutdownHardware() = 0;
            /** Method called when main loop updates, add whatever you want. */
            virtual void Update() = 0;
            /** Called in main loop, use to print information to consle. */
            virtual void PrintStatus() = 0;
            /** Loads XML data, and initializes hardware and JAUS */
            virtual bool Initialize(const std::string& xmlFilename)
            {
                std::cout << "Loading Program Settings From XML...";
                if(LoadSettings(xmlFilename) == false)
                {
                    std::cout << "Failed Load From File: " << xmlFilename << std::endl;
                    return false;
                }
                std::cout << "Success!\n";
                std::cout << "Setting Up JAUS Services and Component Settings...";
                if(SetupJAUS() == false)
                {
                    std::cout << "Failed!\n";
                    return false;
                }
                std::cout << "Success!\n";
                std::string errorMessage;
                std::cout << "Initializing Sensors and Other Hardware...";
                if(InitializeHardware(errorMessage) == false)
                {
                    std::cout << "Failed!\nERROR: " << errorMessage << std::endl;
                    return false;
                }
                std::cout << "Success!\n";
                std::cout << "Initializing JAUS...";
                if(InitializeJAUS(errorMessage) == false)
                {
                    std::cout << "Failed!\nERROR: " << errorMessage << std::endl;
                    return false;
                }
                std::cout << "Success!\n";
                return true;
            }
            /** Does shutdown of JAUS and hardware (e.g. sensors, motors) */
            virtual void Shutdown()
            {
                std::cout << "Shutting Down JAUS\n";
                ShutdownJAUS();
                std::cout << "Shutting Down Sensors and Other Hardware\n";
                ShutdownHardware();
            }
        };
    }
}

#endif
