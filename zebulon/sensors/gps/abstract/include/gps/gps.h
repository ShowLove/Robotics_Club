/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gps.h
/// \brief Generic gps that abstracts individual hardware
///
/// Author(s): Gary Stein<br>
/// Created: 2/27/2010<br>
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
#ifndef _ZEBULON_GPS_H
#define _ZEBULON_GPS_H

#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include "gps/libgpsdll.h"

namespace Zebulon
{
    //New namespace for all gps sensors
    namespace GPS
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Overload this callback and functions to recieve events from generic gps
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_GPS_DLL Callback
        {
            public:
                Callback() {}
                virtual ~Callback() {}
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new gps values are available.
                ///
                ///   \params latitude Updated latitude value
                ///           longitude Updated longitude value
                ///           altitude Updates altitude value
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessGPSData(double latitude, double longitude, double altitude) {};
                static const unsigned char DefaultCallback;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GenericGPS
        ///   \brief GenericGPS class for a Generic GPS
        ///
        ///   This class contains functions for connecting to, configuring, and reading
        ///   data from a gps.  It should be extended to actual gpses.
        ///
        ///   This class is part of the zebsensors library.  This library
        ///   contains interfaces for various sensors used on robots. Such
        ///   sensors include TCM250, DGPS, SICK Laser, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_GPS_DLL GenericGPS
        {
            public:
                GenericGPS();
                virtual ~GenericGPS();
                
                //Simple inferface, to bypass callbacks
                inline double GetLatitude() const { return mLatitude; };
                inline double GetLongitude() const { return mLongitude; };
                inline double GetAltitude() const { return mAltitude; };

                // Connect to Compass on the specified port (assume serial)
                virtual int Initialize(const std::string& port, 
                                       const unsigned int ibaud = 38400,
                                       const bool tryOtherBauds = true,
                                       const unsigned int timout = 2000);
                // Disconnects from the device and closes any active threads.
                virtual void Shutdown();
                virtual bool RegisterCallback(Callback* cb, const unsigned char type=Callback::DefaultCallback);
                virtual bool RemoveCallback(Callback* cb, const unsigned char type=Callback::DefaultCallback);
                bool RunDefaultCallback();
                void ClearCallbacks();
                bool IsConnected() const { return mValidDataFlag; }
        protected:
                //Abstract functions that must be overloaded to work
                //Set the gps to output continuously (defined by gps)
                virtual bool SetContinuousMode() = 0;
                //real function that does work, called by static
                virtual void ProcessingFunction() = 0;
            
                int  mError;                                    ///<  Error from gps
                double mLatitude;                               ///<  Latitude from gps
                double mLongitude;                              ///<  Longitude from gps
                double mAltitude;                               ///<  Altitude from gps

                volatile bool mValidDataFlag;                   ///< Flag signaling valid tcm data recieved
                std::vector<CxUtils::Packet*> mPackets;         ///< Packet list
                CxUtils::Thread mSerialThread;                  ///< Thread for recieving data
                static void SerialThreadFunction(void *args);   ///< Main processing function, must exist per gps
                CxUtils::Mutex mMutex;                          ///< Mutex for data member protection
                std::map<unsigned char, std::set<Callback*> > mCallbacks;   ///< List of callbacks
                CxUtils::Serial mSerial;                                    ///<  Connection to serial port.
                std::string mName;                                          ///<  Compass Name.
        };
    }
}

#endif
