/////////////////////////////////////////////////////////////////////////////
///
/// \file accelerationangularrateandorientationmatrix2.h
/// \brief A Message structure for IMU Information
///
/// Author(s): Daniel Barber<br>
/// Created: 5/28/2008<br>
/// Last Modified: 5/28/2008<br>
/// Copyright (c) 2007<br>
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
#ifndef __ZEBULON_MICROSTRAIN_ACCELERATION_ANGULAR_RATE_AND_ORIENTATION_MATRIX2__H
#define __ZEBULON_MICROSTRAIN_ACCELERATION_ANGULAR_RATE_AND_ORIENTATION_MATRIX2__H

#include "microstrain/message.h"

namespace Zebulon
{
    namespace Compass
    {
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class AccelerationAngularRateAndOrientationMatrix2 
            ///  \brief Structure for reading message response to AccelerationAngularRateAndOrientationMatrix2
            ///         command.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL AccelerationAngularRateAndOrientationMatrix2 : public Message
            {
            public:
                AccelerationAngularRateAndOrientationMatrix2();
                AccelerationAngularRateAndOrientationMatrix2(const AccelerationAngularRateAndOrientationMatrix2& msg);
                ~AccelerationAngularRateAndOrientationMatrix2();
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length);
                virtual Message* Clone() const { return new AccelerationAngularRateAndOrientationMatrix2(*this); }
                virtual void Print() const;
                AccelerationAngularRateAndOrientationMatrix2& operator=(const AccelerationAngularRateAndOrientationMatrix2& msg);
                double mRoll;                   ///<  Roll in radians.
                double mPitch;                  ///<  Pitch in radians.
                double mYaw;                    ///<  Yaw in radians.
                
                double mDRoll;                  ///<  Angular velocity in radians/sec
                double mDPitch;
                double mDYaw;
                
                double mDDX;                    ///<  Accelerations in meters/sec^2
                double mDDY;                    ///<  Accelerations in meters/sec^2
                double mDDZ;                    ///<  Accelerations in meters/sec^2
                
                
                //Matrix format to convert
                double mOrientationMatrix[3][3];
                
                
                unsigned int mTimerTicks;       ///<  Timer Ticks value.
            };
        }
    }
}


#endif
/*  End of File */
