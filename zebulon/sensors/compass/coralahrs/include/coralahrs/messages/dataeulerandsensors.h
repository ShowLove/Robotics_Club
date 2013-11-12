//////////////////////////////////////////////////////////////////////////////////////
///
/// \file dataeulerandsensors.h
/// \brief This file contains the message CORAL_DATA_EULER_AND_SENSORS.
///
/// Author(s): Daniel Barber<br>
/// Created: 1/23/2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_CORAL_AHRS_DATA_EULER_AND_SENSORS__H
#define _ZEBULON_CORAL_AHRS_DATA_EULER_AND_SENSORS__H

#include <cxutils/math/point3d.h>
#include "coralahrs/message.h"

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class DataEulerAndSensors
            ///   \brief This message is sent when the system output mode has been set to 
            ///   Coral Euler or Coral Sensors.  It containts system time, euler angle 
            ///   orientation data, and calibrated sensor data.  These packets are sent at
            ///   a constant rate equal to the standard system output rate divided by the
            ///   output rate divisor.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL DataEulerAndSensors : public Message
            {
            public:
                DataEulerAndSensors();
                DataEulerAndSensors(const DataEulerAndSensors& message);
                ~DataEulerAndSensors();
                virtual int WriteMessageBody(CxUtils::Packet& packet) const;
                virtual int ReadMessageBody(const CxUtils::Packet& packet);
                virtual Message* Clone() const { return (Message*)new DataEulerAndSensors(*this); }
                virtual void Print() const;
                DataEulerAndSensors& operator=(const DataEulerAndSensors& message);
                unsigned short mSystemTimeMs;       ///<  System time from Coral AHRS internal clock (milliseconds).
                CxUtils::Point3D mEulerAngles;      ///<  Euler angles x = roll [-pi,pi], y = pitch [-pi/2, pi/2], z = yaw [-pi, pi](in radians).
                CxUtils::Point3D mGyros;            ///<  Calibrated output of the roll, pitch, and heading gyros.
                CxUtils::Point3D mAccelerometers;   ///<  A vector with the calibraed output of the X, Y, and Z accelerometers.
                CxUtils::Point3D mMagnetometers;    ///<  A vector with the calibrated output of the X, Y, and Z magnetometers.
            };
        }
    }
}


#endif
/*  End of File */
