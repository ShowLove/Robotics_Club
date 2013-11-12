/////////////////////////////////////////////////////////////////////////////////////
///
/// \file hokuyo.h
/// \brief LIDAR Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 3/10/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#ifndef URG_HOKUYO__H
#define URG_HOKUYO__H

#include <string>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <laser/laser.h>
#include "hokuyourg/libhokuyourgdll.h"


namespace Zebulon
{
    namespace Laser
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class HokuyoURG
        ///  \brief Wrapper interface for communication with a Hokuyo LIDAR.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_URG_DLL HokuyoURG : public GenericLaser
        {
            public:
                HokuyoURG();
                virtual ~HokuyoURG();
                virtual bool Initialize(const GenericLaser::Parameters* params);
                virtual bool Initialize(const std::string& port = 
                                        #ifdef WIN32 
                                        "COM3", 
                                        #else 
                                        "/dev/ttyACM0",
                                        #endif
                                        const int baud = 115200,
                                        const int tries = 1);
                virtual void Shutdown();
                // Get the maximum scan distance in meters.
                virtual double GetMaximumDistance() const
                {
                    return mDistanceMax;
                }
                // Get the minimu scan distance in meters.
                virtual double GetMinimumDistance() const
                {
                    return mDistanceMin;
                }
                // Get the resolution (0.01 for cm, 0.001 for mm, etc.)
                virtual double GetDistanceResolution() const
                {
                    return mDistanceMode;
                }
                // Get the scan angle in XY plane. A value of PI would result in -PI/2 to PI/2 scan.
                virtual double GetScanAngle() const
                {
                    return mScanningAngle;
                }
                // Get the angle between scans in radians.
                virtual double GetAngularResolution() const
                {
                    return mAngularResolution;
                }
            private:
                virtual void ProcessingFunction();
                CxUtils::Thread mThread;                    ///< Thread for recieving data
                static void ThreadFunction(void *args);     ///< Main processing function, must exist per compass
                CxUtils::Mutex mMutex;                      ///< Mutex for data member protection

                double mDistanceMin;        ///<  Current Laser configuration, minimum distance in meters.
                double mDistanceMax;        ///<  Current laser configuration, maximum distance in meters.
                double mDistanceMode;       ///<  Current Laser configuration, measured relative to meters, centimeter mode (0.01) and millimeter mode (0.001).
                double mScanningAngle;      ///<  Current Laser configuration, measured in radians.
                double mAngularResolution;  ///<  Current Laser configuration, measured in radians for resolution.
                int mBaudRate;              ///<  Current Baud.
                std::string mPort;          ///<  Current Port to laser.

                // Hokuyo URG specific
                void* mpURG;           ///< Wrapped library interface.
        };
    }
}


#endif
