/////////////////////////////////////////////////////////////////////////////
///
/// \file sendgyrostabilizedimuvector.h
/// \brief A Message structure for Send Gyro Stabilized Euler Angles & Accel & Rate Vector.
///
/// Author(s): Gary Stein<br>
/// Created: 5/15/2010<br>
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
#ifndef __ZEBULON_MICROSTRAIN_SEND_GYRO_STABILIZED_EULER_ANGLES_ACCEL_RATE_VECTOR__H
#define __ZEBULON_MICROSTRAIN_SEND_GYRO_STABILIZED_EULER_ANGLES_ACCEL_RATE_VECTOR__H

#include "microstrain/message.h"


namespace Zebulon
{
    namespace Compass
    {
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class SendGyroStabilizedIMUVector 
            ///  \brief Structure for reading message response to SendGyroStabilizedIMUVector
            ///         command.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL SendGyroStabilizedIMUVector : public Message
            {
            public:
                SendGyroStabilizedIMUVector();
                SendGyroStabilizedIMUVector(const SendGyroStabilizedIMUVector& msg);
                ~SendGyroStabilizedIMUVector();
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length);
                virtual Message* Clone() const { return new SendGyroStabilizedIMUVector(*this); }
                virtual void Print() const;
                SendGyroStabilizedIMUVector& operator=(const SendGyroStabilizedIMUVector& msg);
                double mRoll;                   ///<  Roll in radians.
                double mPitch;                  ///<  Pitch in radians.
                double mYaw;                    ///<  Yaw in radians.
                
                double mDRoll;                  ///<  Angular velocity in radians/sec
                double mDPitch;
                double mDYaw;
                
                double mDDX;                    ///<  Accelerations in meters/sec^2
                double mDDY;
                double mDDZ;
            };
        }
    }
}


#endif
/*  End of File */
