/////////////////////////////////////////////////////////////////////////////////////
///
/// \file velocitystatesensor.h
/// \brief This class is the Velocity State Sensor service for the ASV, and
///          it combines data received from the vehicles compass and GPS. to output
///          an estimation of the current velocity of the vehicle.
///
/// <br>Author(s): Matthew Znoj<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: mattznoj@gmail.com<br>
/// <br>Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_SURFACE_VELOCITY_STATE_SENSOR__H
#define __ZEBULON_SURFACE_VELOCITY_STATE_SENSOR__H

#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/mobility/sensors/globalposesensor.h>
#include <filter/rcfilter.h>
#include <xmlconfig/xmlconfigurator.h>
#include <compass/compass.h>
#include <gps/gps.h>
#include <gx3/gx3.h>
#include "cxutils/fileio.h"
namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VelocityStateSensor
        ///   \brief This class is the Velocity State Sensor service for the ASV, and
        ///          it combines data received from the vehicles compass and GPS. to output
        ///          an estimation of the current velocity of the vehicle.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VelocityStateSensor : public JAUS::VelocityStateSensor, public INS::MicroStrain::Callback
        {
        public:


            virtual void ProcessCompassData(const double yaw, const double pitch, const double roll){}
            virtual void ProcessGPSData(double latitude, double longitude, double altitude){}
            virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration){}
            virtual void ProcessINSData(const CxUtils::Point3D LLHposition,const CxUtils::Point3D NEDVelocity,const CxUtils::Point3D Orientation);

            static const std::string SamplePeriodMs;    ///<  Sampling period in ms (parameter name).
            VelocityStateSensor();
            ~VelocityStateSensor();
            virtual void Shutdown();
            virtual bool LoadSettings(const std::string& filename);
            /** Set pointer to source for global pose data */
            void SetGlobalPoseSensor(JAUS::GlobalPoseSensor* globalPoseSensor)
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);
                mpGlobalPoseSensor = globalPoseSensor;
            }
            void SetLogFile(FILE* file){mLogFile = file;}
        protected:
            static void VelocityThread(void* args);
            CxUtils::Thread mVelocityThread;            ///< Thread for calculations of objects.
            CxUtils::Mutex mMutex;                      ///< Synchronize GPS data with callback and other needs.
            Configuration::XmlConfigurator mXML;        ///< XML reader.
            JAUS::GlobalPoseSensor* mpGlobalPoseSensor; ///< Pointer to global pose sensor (used for yaw rate).
            JAUS::GlobalPose mPrevGlobalPose;           ///< Previous global pose.
            Utility::Filter* mpLowPassFilter;           ///< Low pass filter.
            JAUS::Time::Stamp mSamplePeriodMs;          ///< Sampling period in ms for calculations.
            FILE* mLogFile;
            CxUtils::Mutex mLogMutex;
        };
    }
}

#endif 
/*  End of File */
