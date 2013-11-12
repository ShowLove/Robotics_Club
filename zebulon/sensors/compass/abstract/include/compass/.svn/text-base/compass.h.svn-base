/////////////////////////////////////////////////////////////////////////////////////
///
/// \file compass.h
/// \brief Generic compass that abstracts individual hardware
///
/// Author(s): Gary Stein<br>
/// Created: 2/4/2010<br>
/// Last Modified: 2/4/2010<br>
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
#ifndef ZEBULON_COMPASS_H
#define ZEBULON_COMPASS_H
#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/math/cxmath.h>
#include "compass/libcompassdll.h"

namespace Zebulon
{
    //New namespace for all compass sensors
    namespace Compass
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Overload this callback and functions to recieve events from 
        ///          generic compass
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_COMPASS_DLL Callback
        {
            public:
                Callback() {}
                virtual ~Callback() {}
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new yaw pitch and roll values are available.
                ///
                ///   \params yaw Updated yaw value in radians.
                ///           pitch Updated pitch value in radians.
                ///           roll Updates roll value in radians.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessCompassData(const double yaw, const double pitch, const double roll) { };
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new attitude gyroscope and acceleration values are available.
                ///
                ///   \param attitude Updated attitude value
                ///   \param gyroscope Updated pitch gyroscope
                ///   \param acceleration Updates acceleration value
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration) { };
                static const unsigned char DefaultCallback;
                static const unsigned char IMUCallback;
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GenericCompass
        ///   \brief GenericCompass class for a Generic Compass
        ///
        ///   This class contains functions for connecting to, configuring, and reading
        ///   data from a compass.  It should be extended to actual compasses.
        ///
        ///   This class is part of the zebsensors library.  This library
        ///   contains interfaces for various sensors used on robots. Such
        ///   sensors include TCM250, DGPS, SICK Laser, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_COMPASS_DLL GenericCompass
        {
            public:
                GenericCompass();
                virtual ~GenericCompass();
                
                //Simple inferface, to bypass callbacks
                inline double GetYaw() const { return mYaw; };
                inline double GetPitch() const { return mPitch; };
                inline double GetRoll() const { return mRoll; };
                
                // Connect to Compass on the specified port (assume serial)
                virtual int Initialize(const std::string& port, 
                                       const unsigned int ibaud = 38400,
                                       const bool tryOtherBauds = true,
                                       const unsigned int timout = 2000);
                // Disconnects from the device and closes any active threads.
                virtual void Shutdown();
                virtual bool RegisterCallback(Callback* cb, const unsigned char type = Callback::DefaultCallback);
                virtual bool RemoveCallback(Callback* cb, const unsigned char type = Callback::DefaultCallback);
                bool RunDefaultCallback();
                bool RunIMUCallback();
                void ClearCallbacks();
                bool IsConnected() const { return mValidDataFlag; }
                bool IsImuSupported() const { return mImuSupported; }
                double GetDeclination() const;
                //  Radians
                void SetDeclination(const double declination);
                void SetDeclination(const double declinationDeg, const double declinationMin);
        protected:
                //Abstract functions that must be overloaded to work
                //Set the compass to output continuously
                virtual bool SetContinuousMode() = 0;
                //real function that does work, called by static
                virtual void ProcessingFunction() = 0;
                
                //For setting True North
                void FixDeclination();
                double mYawOffset;
            
                int  mError;                                    ///<  Error from compass
                double mYaw;                                    ///<  Compass data [-PI, PI]
                double mPitch;                                  ///<  Pitch [-PI, PI]
                double mRoll;                                   ///<  Roll [-180, 180]
                
                CxUtils::Point3D mAttitude;                     ///<  Roll, Pitch, Yaw
                CxUtils::Point3D mGyroscope;                    ///<  dRoll, dPitch, dYaw
                CxUtils::Point3D mAcceleration;                 ///<  ddX, ddY, ddZ

                volatile bool mValidDataFlag;                   ///< Flag signaling valid tcm data recieved
                std::vector<CxUtils::Packet*> mPackets;         ///< Packet list
                CxUtils::Thread mSerialThread;                  ///< Thread for recieving data
                static void SerialThreadFunction(void *args);   ///< Main processing function, must exist per compass
                CxUtils::Mutex mMutex;                          ///< Mutex for data member protection
                CxUtils::Serial mSerial;                        ///<  Connection to serial port.
                std::string mName;                              ///<  Compass name.
                bool mImuSupported;                             ///<  Flag if IMU data is supported.
                std::map<unsigned char, std::set<Callback*> > mCallbacks;   ///< List of callbacks
        };
    }
}

#endif
