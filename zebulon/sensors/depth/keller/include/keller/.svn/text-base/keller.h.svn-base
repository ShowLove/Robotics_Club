/////////////////////////////////////////////////////////////////////////////////////
///
/// \file keller.h
/// \brief Keller America Depth Sensor Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 4/16/2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu<br>
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
#ifndef KELLER_H
#define KELLER_H

//Basic class for the depth sensor
//Potentially want to make a generic depth sensor
//and inherit this off of it

#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/math/cxmath.h>

namespace Zebulon
{
    namespace Depth
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Overload this callback and functions to recieve events from 
        ///          generic depth
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Callback
        {
            public:
                Callback() {}
                virtual ~Callback() {}
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///   \brief Event called when new yaw pitch and roll values are available.
                ///
                ///   \params depth Updated depth value in meters.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                virtual void ProcessDepthData(const double depth) { };

                static const unsigned char DefaultCallback;
        };        
        
        class Keller
        {
            public:
                Keller();
                ~Keller();
                
                // Connect to Compass on the specified port (assume serial)
                virtual int Initialize(const std::string& port, const int Rate,
                                       const unsigned int ibaud = 9600,
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
                
                double ReqPerSec;
                double RecPerSec;
                
                
            protected:
            
                //Abstract functions that must be overloaded to work
                //real function that does work, called by static
                virtual void ProcessingFunction();

                //from documentation
                void CalcCRC16(unsigned char *SC_Buffer,int SC_Amount,unsigned char* CRC_H, unsigned char* CRC_L);
                //full length (not packet length without checksum)
                bool CheckCRC(unsigned char *buffer,int length);
                int mAddress;
                double mDepth;
                double mPeriod;
                volatile bool mValidDataFlag;                   ///< Flag signaling valid tcm data recieved
                std::vector<CxUtils::Packet*> mPackets;         ///< Packet list
                CxUtils::Thread mSerialThread;                  ///< Thread for recieving data
                static void SerialThreadFunction(void *args);   ///< Main processing function

                CxUtils::Thread mRequestThread;                  ///< Thread for recieving data
                static void SerialRequestFunction(void *args);   ///< Main processing function
                virtual void RequestingFunction();
                
                CxUtils::Mutex mMutex;                          ///< Mutex for data member protection
                CxUtils::Serial mSerial;                        ///<  Connection to serial port.
                std::string mName;                              ///<  Device name.
                std::map<unsigned char, std::set<Callback*> > mCallbacks;   ///< List of callbacks
                double mDepthOffset;
                int mDepthOffsetCount;
                int mFailCount;
        };
    }
}

#endif