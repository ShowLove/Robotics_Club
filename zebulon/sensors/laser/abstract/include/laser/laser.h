/////////////////////////////////////////////////////////////////////////////////////
///
/// \file laser.h
/// \brief Generic laser that abstracts individual hardware
///
/// Author(s): Gary Stein<br>
/// Created: 2/13/2010<br>
/// Last Modified: 4/17/2010 by Daniel Barber<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEBULON_LASER_H
#define ZEBULON_LASER_H
#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>

#include <cxutils/math/point3d.h>
#include "laser/liblaserdll.h"

namespace Zebulon
{
    //New namespace for all laser sensors
    namespace Laser
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Overload this callback and functions to recieve events from generic laser
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_LASER_DLL Callback
        {
            public:
                Callback() {}
                virtual ~Callback() {}
                typedef std::set<Callback*> Set;
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new data is available, stored in a vector in 
                ///          polar coordinates.
                ///
                ///   The coordinate frame for a laser is as follows: X positive in front, Y
                ///   positive to the right, Z positive down.  A positive Z rotation is a
                ///   rotation to the right, a positive Y rotation is a tilt up.
                ///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
                ///
                ///   \param[in] scan Laser scan data in polar coordinates relative to the
                ///                   laser.  Values are as follows: X = range/distance in
                ///                   meters, Y = pitch [-pi/2, pi/2], Z = yaw [-PI,PI].
                ///                   values are in order from left to right.
                ///   \param[in] timestamp Time when the data was captured (UTC).
                ///   \param[in] id Laser sensor ID number.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                              const CxUtils::Time& timestamp,
                                              const unsigned int id = 0) = 0;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GenericLaser
        ///   \brief GenericLaser class for a Generic Laser
        ///
        ///   This class contains functions for connecting to, configuring, and reading
        ///   data from a laser.  It should be extended to actual laseres.
        ///
        ///   This class is part of the zebsensors library.  This library
        ///   contains interfaces for various sensors used on robots. Such
        ///   sensors include TCM250, DGPS, SICK Laser, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_LASER_DLL GenericLaser
        {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \class Parameters
                ///   \brief Generic parameters data structure for LIDARS.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                class ZEB_LASER_DLL Parameters
                {
                public:
                    Parameters() 
                    { 
                        mBaudRate = 38400; 
                        mTryOtherBaudsFlag = false; 
                        mTries = 3;
                        mSerialPort = "COM3";
                    }
                    virtual ~Parameters() {}
                    std::string mSerialPort;    ///<  Serial port (if used).
                    int mBaudRate;              ///<  Baudrate to use.
                    bool mTryOtherBaudsFlag;    ///<  If true, try different baud rates.
                    int mTries;                 ///<  How many tries to connect.
                };
                GenericLaser(const std::string& name) : mName(name), mID(0) { mValidDataFlag = false; }
                virtual ~GenericLaser() {}                
                // Initialize with whatever settings have been set.
                virtual bool Initialize(const Parameters* params) = 0;
                virtual bool Initialize(const std::string& port = "/dev/ttyACM0", 
                                        const int baud = 115200, 
                                        const int tries = 1) { return false; }
                // Disconnects from the device and closes any active threads.
                virtual void Shutdown() = 0;
                // Get the maximum scan distance in meters.
                virtual double GetMaximumDistance() const = 0;
                // Get the minimu scan distance in meters.
                virtual double GetMinimumDistance() const = 0;
                // Get the resolution (0.01 for cm, 0.001 for mm, etc.)
                virtual double GetDistanceResolution() const = 0;
                // Get the scan angle in XY plane. A value of PI would result in -PI/2 to PI/2 scan.
                virtual double GetScanAngle() const = 0;
                // Get the angle between scans in radians.
                virtual double GetAngularResolution() const = 0;
                // Method to register callbacks.
                virtual bool RegisterCallback(Callback* cb)
                {
                    CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                    mCallbacks.insert(cb);
                    return true;
                }
                // Method to remove a callback.
                virtual bool RemoveCallback(Callback* cb)
                {
                    CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                    Callback::Set::iterator c;
                    c = mCallbacks.find(cb);
                    if(c != mCallbacks.end())
                    {
                        mCallbacks.erase(c);
                        return true;
                    }
                    return false;
                }
                // Method to clear callbacks.
                void ClearCallbacks()
                {
                    CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                    mCallbacks.clear();
                }
                // Method to assign an ID.
                void SetDeviceID(const unsigned int id) { mID = id; }
                inline bool IsConnected() const { return mValidDataFlag; }
                inline std::string GetName() const { return mName; }
                
                CxUtils::Point3D::List GetData()
                {
                    CxUtils::Mutex::ScopedLock lock(&mLaserMutex);
                    return mLaserScan;
                }

        protected:
                bool mValidDataFlag;            ///<  If true, data is valid and we are connected to device.
                Callback::Set mCallbacks;       ///<  Registered callbacks for data.
                CxUtils::Mutex mCallbacksMutex; ///<  Mutex for thread protection of callbacks.
                std::string mName;              ///<  Name of device.
                unsigned int mID;               ///<  ID of device.
                
                //local version, non callback
                CxUtils::Mutex mLaserMutex;
                CxUtils::Point3D::List mLaserScan;
        };
    }
}

#endif