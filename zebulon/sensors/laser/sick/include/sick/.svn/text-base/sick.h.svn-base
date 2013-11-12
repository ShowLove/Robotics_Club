/////////////////////////////////////////////////////////////////////////////////////
///
/// \file sick.h
/// \brief LIDAR Interface.
///
/// Author(s): Daniel Barber<br>
/// Created: 3/22/2008<br>
/// Last Modified: 4/17/2010<br>
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
#ifndef _ZEBULON_LASER_SICK__H
#define _ZEBULON_LASER_SICK__H

#include "sick/message.h"
#include "laser/laser.h"
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <list>
#include <vector>
#include <set>

namespace Zebulon
{
    namespace Laser
    {
        namespace SICK
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Sick
            ///  \brief Sick is the main interface for connecting to a SICK LIDAR.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_SICK_DLL Sick : public GenericLaser
            {
            public:                
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \class Callback
                ///  \brief Callback class used to register for message events.
                ///
                ///  Inherit from this class and overload the ProcessMessage method and pass a
                ///  pointer to your child class to the RegisterCallback method of Laser and you
                ///  will get every message received as it arrives as a callback.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                class ZEB_SICK_DLL Callback : public Laser::Callback
                {
                public:
                    Callback() {}
                    virtual ~Callback() {}
                    ////////////////////////////////////////////////////////////////////////////////////
                    ///
                    ///   \brief Event called whenever a message is received by the laser.
                    ///
                    ///   \param[in] msg SICK LMS2xx Message.
                    ///
                    ////////////////////////////////////////////////////////////////////////////////////
                    virtual void ProcessMessage(const Zebulon::Laser::SICK::Message* msg) {}
                };
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum DistanceModes
                ///  \brief Possible distance mode settings values that the SICK Laser can use
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                enum DistanceModes
                {
                    CM = 0x00,            ///< Centimeter mode
                    MM = 0x01,            ///< Millimeter mode
                };
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum ScanningAngle
                ///  \brief Possible angle values that the SICK Laser can scan with
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                enum ScanningAngle
                {
                    OneHundred = 0x0064, ///< 100 Degree Sweep
                    OneEighty = 0x00B4,  ///< 180 Degree Sweep
                };
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum AngularResolution
                ///  \brief Possible angular resolution values that the SICK Laser can use.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                enum AngularResolution
                {
                    One = 0x0064,     ///< 1 Degree Resolution
                    Half = 0x0032,    ///< 0.5 Degree Resolution
                    Quarter = 0x0019, ///< 0.25 Degree Resolution
                };
                Sick();
                virtual ~Sick();
                virtual bool Initialize(const std::string& port, 
                                        const int baud = 460800, 
                                        const unsigned char id = 0x00, 
                                        const int tries = 1);
                virtual bool InitializeWithDefaults(const std::string& port,
                                                    const int baud = 38400);
                virtual bool Initialize(const GenericLaser::Parameters* params);
                void Shutdown();                
                bool RequestContinuousOutput(const unsigned int timeoutMs = 5000);
                bool StopContinuousOutput(const unsigned int timeoutMs = 5000);
                bool ChangeBaudRate(int baud, int cbaud = 0);
                //bool SetPermanentBaudRate(bool cur = true);
                bool SetDistanceMode(const Sick::DistanceModes mode);
                bool SetScanningMode(const Sick::ScanningAngle angle, 
                                     const Sick::AngularResolution resolution);
                virtual double GetMaximumDistance() const { if(mDistanceMode >= 0.01) { return 81.91; } return 8.191; }
                virtual double GetMinimumDistance() const { return 0.0; }
                virtual double GetDistanceResolution() const { return mDistanceMode; }
                virtual double GetScanAngle() const { return mScanningAngle; }
                virtual double GetAngularResolution() const { return mAngularResolution; }
                double GetStartAngle() const { return mStartAngle; }
                Message* Transmit(const Message* message,
                                  Message::Type desiredResponse,
                                  const unsigned int waitTimeMs = 5000);
            private:
                Message* FindAndCreateMessage(const CxUtils::Packet& msg);

                // Function for receiving messages from LIDAR.
                static void ReceiveThread(void* args);

                Message::Queue mMessageQueue;
                Message::Queue mContinousOutputMessageQueue;
                // Thread object that will handle communication with the SICK laser
                CxUtils::Thread mReceiveThread;
                // Mutex for the message queue so that the JAUS interface and the thread
                // don't access it at the same time
                CxUtils::Mutex mMessageQueueMutex;
                unsigned char mLaserID;
                unsigned int mMaxQueueSize;
                std::vector<int> mPossibleBaudRates;
                float mLaserAngle;
                double mDistanceMode;       ///<  Current SICK Laser configuration, measured relative to meters, centimeter mode (0.01) and millimeter mode (0.001).
                double mScanningAngle;      ///<  Current SICK Laser configuration, measured in radians. 
                double mStartAngle;         ///<  Starting angle of scan in coordinate frame.
                double mAngularResolution;  ///<  Current SICK Laser configuration, measured in radians for resolution.
                std::string mPort;          ///<  Current Port to laser.
                CxUtils::Serial mSerial;                    ///<  Connection to serial port.
            };
        }
    }
}


#endif
