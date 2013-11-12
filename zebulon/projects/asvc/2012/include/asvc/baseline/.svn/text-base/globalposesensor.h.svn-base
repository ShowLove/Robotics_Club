////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalposesensor.h
/// \brief Global Pose Sensor for the ASV, it combines data from the vehicles
/// compass and GPS to JAUS data.
///
/// <br>Author(s): Daniel Barber<br>
/// <br>Created: 2/27/2010<br>
/// <br>Copyright (c) 2010<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: dbarber@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
///
/// Redistribution and use in source and binary forms, with or without
/// modification, are permitted provided that the following conditions are met:
/// * Redistributions of source code must retain the above copyright
/// notice, this list of conditions and the following disclaimer.
/// * Redistributions in binary form must reproduce the above copyright
/// notice, this list of conditions and the following disclaimer in the
/// documentation and/or other materials provided with the distribution.
/// * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
/// names of its contributors may be used to endorse or promote products
/// derived from this software without specific prior written permission.
///
/// THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
/// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/// DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
/// DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/// ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_ASVC_GLOBAL_POSE_SENSOR__H
#define __ZEBULON_ASVC_GLOBAL_POSE_SENSOR__H
    
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <compass/compass.h>
#include <gps/gps.h>
#include <gx3/gx3.h>
#include <cxutils/fileio.h>

namespace Zebulon
{
     namespace ASVC
     {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GlobalPoseSensor
        ///   \brief Jaus component that collects data from Generic compass and Generic GPS.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalPoseSensor : public JAUS::GlobalPoseSensor
        {
        public:
            GlobalPoseSensor();
            ~GlobalPoseSensor();
            enum CBType
            {
                GX3 = 0,            //  GX3-45 callbacks for orientation and non filtered gps
                DGPS = 1,           //  NavCom diferential gps callback - NOT CONNECTED
                INS = 2,            //  GX3-45 ins callback for gps+imu filtering
                GPS = 3,            //  2011 GPS Novatel - NOT CONNECTED
                Compass = 4         //  3DM-GX1
            };
            /** Callback interface for sensors to get data. */
            class DataCallback : public Zebulon::INS::MicroStrain::Callback,
                                public GPS::Callback,
                                public Compass::Callback
            {
                CBType mType;
                GlobalPoseSensor *mpGlobalPoseSensor;
            public:
                DataCallback(CBType type, GlobalPoseSensor* gps)
                {
                    mType = type;
                    mpGlobalPoseSensor = gps;
                }

                virtual void ProcessCompassData(const double yaw, const double pitch, const double roll);
                virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration);
                virtual void ProcessGPSData(double latitude, double longitude, double altitude);
                virtual void ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation);
            };

            void ProcessCompassData(const double yaw, const double pitch, const double roll, CBType source);
            void ProcessGPSData(double latitude, double longitude, double altitude, CBType source);
            void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration,CBType source);
            void ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation,CBType source);

            void SetLocalPoseSensor(JAUS::LocalPoseSensor* local);
            virtual bool LoadSettings(const std::string& filename);

            void SetLogFile(FILE * file){mLogFile = file;}

            void SetPrimaryGPS(CBType type) { mPrimaryGPS = type; }
            void SetPrimaryGPSTimeout(int timeoutms){mPrimaryGPSTimeoutMS = timeoutms;}
            void SetPrimaryCompass(CBType type) { mPrimaryCompass = type; }
            virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs);

            DataCallback* GetCallback(CBType type)
            {
                switch(type)
                {
                    case GX3:
                        return mpGX3Callback;
                    case DGPS:
                        return mpDGPSCallback;
                    case GPS:
                        return mpGPSCallback;
                    case Compass:
                        return mpCompassCallback;
                        break;
                    default:
                        return NULL;
                }
                return NULL;
            }
            void PrintStatus() const;
        protected:
            DataCallback*                        mpGX3Callback;   ///< Callback for gx3.
            DataCallback*                        mpDGPSCallback;  ///< Callback for differential gps(Novatel).
            DataCallback*                        mpGPSCallback;   ///< Callback for 2011 boat gps.
            DataCallback*                        mpCompassCallback;   ///< Callback from compass.
            JAUS::Mutex mGlobalPoseSensorMutex;         ///<  For thread safe operation.
            JAUS::LocalPoseSensor* mpLocalPoseSensor;   ///<  Pointer to local pose sensor service.
            JAUS::Point3D mPositionOffset;              ///<  Position offset of GPS on vehicle (meters).
            JAUS::Mutex mLogMutex;                      ///<  For thread safe operation.
            bool mLoggingFlag;                          ///<  Is GlobalPoseSensor logging enabled.
            FILE *mLogFile;                             ///<  Self contained log file.
            CBType mPrimaryGPS;                         ///<  Primary GPS to use.
            CBType mPrimaryCompass;                     ///<  Primary compass to use.
            CxUtils::Time mTimeSinceLastPrimaryGPS;     ///<  Last time primary GPS data received (in case of drop).
            int mPrimaryGPSTimeoutMS;                   ///<  Look for any gps data if primary is gone for this many milliseconds.
            bool mUseAnyGPSFlag;    ///< if set to true, will ignore primary gps and use any gps data.
            bool mHaveCompassFlag;                            ///<  Have compass flag.
            JAUS::Point3D mCompassData;                       ///<  Compass data.
            JAUS::Point3D mGx3CompassData;                    ///<  GX3 Compass data.
            bool mHaveGx3CompassDataFlag;                     ///<  Have GX3 Compass data?
        };
    }
}
    
#endif
/* End of File */ 
