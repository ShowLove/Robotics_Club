/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gx3.h
/// \brief RS232 Interface class for the MicroStrain 3DM-GX3 sensor.
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_MICROSTRAIN_3DMGX3__H
#define __ZEBULON_MICROSTRAIN_3DMGX3__H


#include <stdio.h>
#include <string.h>
#include <vector>
#include <set>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/math/cxmath.h>
#include <set>

#include "gx3/message.h"

#define GX3_IMU

/* Long spiel about nothing to no one

No one else seems to write these drivers...so whoever want to read them.

Basically the framework needs to be split/extended from the callbacks versus
the drivers such that things like the gx3 are not a problem.

If the gx3 could borrow both the callbacks from the compass and gps, then it
could be its own device, and fulfills those callbacks.  This can be done, but
could entail alot of work and a global list of callbacks/structure like Player/Stage.

The other thing is the sensor structure.  The GPS and Compass generics are like 95% the
same (I know, I wrote them both) and needs to be compressed to GenericSensor or something
similar.  But that also requires a reorganization.

Thus for now, gx3 will sit on its own, both a compass, gps, and ins seperate sharing a
good 95% of code with both til I or someone else gets fed up, and recombines it back in

-gary
*/

namespace Zebulon
{
    namespace INS
    {
        namespace MicroStrain
        {
            class Callback
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
                    ////////////////////////////////////////////////////////////////////////////////////
                    ///
                    ///   \brief Event called when new local velocity information is given.
                    ///
                    ///   \param attitude Updated velocity value
                    ///
                    ////////////////////////////////////////////////////////////////////////////////////
                    virtual void ProcessVelocityData(const CxUtils::Point3D velocity) { };
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
                    ////////////////////////////////////////////////////////////////////////////////////
                    ///
                    ///   \brief Event called when new ins values are available.
                    ///
                    ///   \param LLHposition Updated Lat, Long, Height from filter (like gps)
                    ///   \param NEDVelocity Updated North, East, and Down velocity from filter
                    ///   \param Orientation Updates Yaw, Pitch and Roll from filter
                    ///
                    ////////////////////////////////////////////////////////////////////////////////////
                    virtual void ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation) {};


                    enum CallbackList
                    {
                        //I want everything
                        DefaultCallback = 0xFF,
                        CompassCallback = 0x01,
                        GPSCallback = 0x02,
                        IMUCallback = 0x04,
                        INSCallback = 0x08,
                        VELCallback = 0x10
                    };
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class GX3
            ///   \brief Interface class for the MicroStrain 3DM-GX3 AHRS/GPS/INS using
            ///   RS232.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class GX3
            {
            public:
                // Constructor.
                GX3();
                // Destructor.
                virtual ~GX3();
                
                // Sets the sensor to continuously send a specific type of data.
                //bool SetContinuousMode(const Message::Type type);
                // Transmit message and get response.
                //bool TransmitMessage(const CxUtils::Packet message,
                //                     MicroStrain::Message** response,
                //                     unsigned int timeout = 1000);

                // Connect to Device on the specified port (assume serial)
                virtual int Initialize(const std::string& port,
                                       const unsigned int ibaud = 115200,
                                       const bool tryOtherBauds = true,
                                       const unsigned int timout = 2000);
                // Disconnects from the device and closes any active threads.
                virtual void Shutdown();
                virtual bool RegisterCallback(Callback* cb, const unsigned char type = Callback::DefaultCallback);
                virtual bool RemoveCallback(Callback* cb, const unsigned char type = Callback::DefaultCallback);
                void ClearCallbacks();
                bool IsConnected() const { return mValidDataFlag; }
                double GetDeclination() const;
                void SetDeclination(const double declination);
                void SetDeclination(const double declinationDeg, const double declinationMin);

            protected:
            
                virtual void ProcessingFunction();     ///<  Thread used to receive data from sensor.
                virtual bool SetContinuousMode();
                bool InitializeINS();
                //bool RunCallback(const Message* message);
                bool RunCallbacks();

                double mYawOffset; // Declination angle offset correction.

                //Message::Type mContinuousModeType;      ///<  Type of continuous mode to maintain.

                //Internal Recent Storage

                //IMU Informtation
                bool mNewIMUData;
                CxUtils::Point3D mAttitude;
                CxUtils::Point3D mGyroscope;
                CxUtils::Point3D mAcceleration;
                //needs a hack for now, gx3 acting weird
                CxUtils::Point3D mVelocity;

                //GPS Information
                bool mNewGPSData;
                CxUtils::Point3D mLLHPositionGPS;
                CxUtils::Point3D mNEDVelocityGPS;

                //INS Information
                bool mNewINSData;
                CxUtils::Point3D mLLHPosition;
                CxUtils::Point3D mNEDVelocity;
                CxUtils::Point3D mOrientation;


                volatile bool mValidDataFlag;                   ///< Flag signaling valid tcm data recieved
                CxUtils::Thread mSerialThread;                  ///< Thread for recieving data
                static void SerialThreadFunction(void *args);   ///< Main processing function, must exist per gps
                CxUtils::Mutex mMutex;                          ///< Mutex for data member protection
                std::map<unsigned char, std::set<Callback*> > mCallbacks;   ///< List of callbacks
                CxUtils::Serial mSerial;                                    ///<  Connection to serial port.
                std::string mName;                                          ///<  Device Name.

                bool mINSInit;

                bool mINSFirstData;
                double mLastDataTime;

                std::string mPort;
                int mBaud;
            };
        }
    }
}

#endif
/* End of File */
