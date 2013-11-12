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
        class GlobalPoseSensor : public JAUS::GlobalPoseSensor,
                                 public Zebulon::Compass::Callback,
                                 public Zebulon::GPS::Callback
        {
        public:
            GlobalPoseSensor();
            ~GlobalPoseSensor();
            virtual void ProcessGPSData(double latitude, double longitude, double altitude);
            virtual void ProcessCompassData(const double yaw, const double pitch, const double roll);
            void SetLocalPoseSensor(JAUS::LocalPoseSensor* local);
            virtual bool LoadSettings(const std::string& filename);
        protected:
            JAUS::Mutex mGlobalPoseSensorMutex;         ///<  For thread safe operation.
            JAUS::LocalPoseSensor* mpLocalPoseSensor;   ///<  Pointer to local pose sensor service.
            JAUS::Point3D mPositionOffset;              ///<  Position offset of GPS on vehicle (meters).
        };
    }
}
    
#endif
/* End of File */ 
