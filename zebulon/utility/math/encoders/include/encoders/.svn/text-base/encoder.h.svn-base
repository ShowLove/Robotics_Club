////////////////////////////////////////////////////////////////////////////////////
///
/// \file encoder.h
/// \brief Data structure for storing encoder information.
///
/// Author(s): Daniel Barber<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef __ZEBULON_UTILITY_ENCODERS_ENCODER_H
#define __ZEBULON_UTILITY_ENCODERS_ENCODER_H

#include <map>
#include <cxutils/time.h>

#ifndef ZEB_ENCODERS_DLL
    #ifdef WIN32
        #define ZEB_ENCODERS_DLL __declspec(dllimport)
    #else
        #define ZEB_ENCODERS_DLL
    #endif
#endif


namespace Zebulon
{
    namespace Utility
    {
        namespace Encoders
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Encoder
            ///   \brief Data structure for storing Quadrature encoder information used in
            ///          closed loop control systems (e.g. motor control).
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_ENCODERS_DLL Encoder
            {
            public:
                typedef std::map<unsigned int, Encoder> Map;
                Encoder();
                Encoder(const Encoder& encoder);
                virtual ~Encoder();
                virtual void Clear();
                double ToMeters() const { return ToMeters(mShaftRadius, mValuesPerRevolution); }
                double ToMeters(const double radius,                                         // Radius/dist from shaft 
                                const long long int ticksPerRevolution) const;               // Encoder count for 1 full revolution of motor shaft.
                double ToLinearVelocity() const { return ToLinearVelocity(mShaftRadius, mValuesPerRevolution); }
                double ToLinearVelocity(const double radius,                                 // Radius/dist from shaft 
                                        const long long int ticksPerRevolution) const;       // Encoder count for 1 full revolution of motor shaft.
                double ToRotationalVelocity() const { return ToRotationalVelocity(mValuesPerRevolution); }
                double ToRotationalVelocity(const long long int ticksPerRevolution) const;   // Encoder count for 1 full revolution of motor shaft.
                Encoder operator+(const Encoder& encoder) const;
                Encoder& operator+=(const Encoder& encoder);
                Encoder& operator=(const Encoder& encoder);
                bool mRelativeFlag;                    ///<  True if relative encoder values, false for absolute position (default is true).
                int mCount;                            ///<  Encoder ticks/count.
                int mValuesPerRevolution;              ///<  Number of encoder values in 1 revolution of the motor shaft.
                double mShaftRadius;                   ///<  Motor shaft radius (or wheel radius).
                CxUtils::Time::Stamp mSamplePeriodMs;  ///<  Time change in milliseconds representing sampling period (for speed calculations).
                CxUtils::Time mTimeStamp;              ///<  Time of reading (UTC).
            };
        }
    }
}
#endif  //__ZEBULON_UTILITY_ENCODER

/* End of File */
