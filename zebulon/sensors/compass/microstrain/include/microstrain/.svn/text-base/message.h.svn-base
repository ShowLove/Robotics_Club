/////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief A simple class fordefining message structures/data for 
///        MicroStrain orientation sensors.
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
#ifndef __ZEBULON_MICROSTRAIN_MESSAGE__H
#define __ZEBULON_MICROSTRAIN_MESSAGE__H

#include <string>
#include <cxutils/packet.h>
#include <list>
#include "microstrain/libmicrostraindll.h"
#include <compass/compass.h>
#include <cxutils/math/cxmath.h>

////////////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Namespace for all software developed by the Robotics Laboratory and
///         Club at the University of Central Florida.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace Compass
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace MicroStrain 
        ///  \brief MicroStrain sensors, etc. namespace for the MicroStrain library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace MicroStrain
        {
            class Message;

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Callback
            ///   \brief Callback class to register for messages as the arrive on the
            ///   serial port.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL Callback : public Compass::Callback
            {
            public:
                virtual void ProcessMessage(const MicroStrain::Message* msg) = 0;
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Message 
            ///  \brief Message class RS232 communication for MicroStrain Sensors.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL Message
            {
            public:
                typedef std::list<Message*> List;
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum Type
                ///  \brief Enumeration of all message types.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                enum ZEB_MICROSTRAIN_DLL Type
                {
                    Null = 0x00,
                    SendRawSensorsBits = 0x01,
                    SendGyroStabilizedVectors = 0x02,
                    SendInstantaneousVectors = 0x03,
                    SendInstantaneousQuaternion = 0x04,
                    SendGyroStabilizedQuaternion = 0x05,
                    CaptureGyroBias = 0x06,
                    SendTemperature = 0x07,
                    ReadEEPROMValue = 0x08,
                    WriteEEPROMValue = 0x09,
                    SendInstantaneousOrientationMatrix = 0x0A,
                    SendGyroStabilizedOrientationMatrix = 0x0B,
                    SendGyroStabilizedQuaternionAndVectors = 0x0C,
                    SendInstantaneousEulerAngles = 0x0D,
                    SendGyroStabilizedEulerAngles = 0x0E,
                    TareCoordinateSystem = 0x0F,
                    SetContinuousMode = 0x10,
                    RemoveTare = 0x11,
                    SendGyroStabilizedQuaternionAndInstantaneousVectors = 0x12,
                    WriteSystemGains = 0x24,
                    ReadSystemGains = 0x25,
                    SelfTest = 0x27,
                    ReadEEPROMValueWithChecksum = 0x28,
                    WriteEEPROMValueWithChecksum = 0x29,
                    SendGyroStabilizedEulerAnglesAndAccelAndRateVector = 0x31,
                    InitializeHardIronFieldCalibration = 0x40,
                    CollectHardIronFieldCalibration = 0x41,
                    ComputeHardIronFieldCalibration = 0x42,
                    SendFirmwareVersionNumber = 0xF0,
                    SendDeviceSerialNumber = 0xF1,
                    
                    //G2 Commands
                    RawAccelerometerAndAngularRate2 = 0xC1,
                    AccelerationAndAngularRate2 = 0xC2,
                    DeltaAngleAndDeltaVelocity2 = 0xC3,
                    SetContinuousMode2 = 0xC4,
                    OrientationMatrix2 = 0xC5,
                    AttitudeUpdateMatrix2 = 0xC6,
                    MagnetometerVector2 = 0xC7,
                    AccelerationAngularRateAndOrientationMatrix2 = 0xC8,
                    WriteAccelerometerBiasCorrection2 = 0xC9,
                    WriteGyroBiasCorrection2 = 0xCA,
                    AccelerationAngularRateAndMagnetometerVector2 = 0xCB,
                    AccelAngRateAndMagVectorsAndOrientationMatrix2 = 0xCC,
                    CaptureGyroBias2 = 0xCD,
                    EulerAngles2 = 0xCE,
                    EulerAnglesAndAngularRates2 = 0xCF,
                    TransferQuantityToNonVolatileMemory2 = 0xD0,
                    Temperatures2 = 0xD1,
                    GyroStabilizedAccelerationAngularRateAndMagnetometerVector2 = 0xD2,
                    DeltaAngleDeltaVelocityMagnetometerVectors2 = 0xD3,
                    WriteWordToEEPROM2 = 0xE4,
                    ReadWordFromEEPROM2 = 0xE5,
                    ReadFirmwareVersionNumber2 = 0xE9,
                    StopContinuousMode2 = 0xFA,
                    BuiltinTest2 = 0xFB
                };
                static const double ANGLE_CONVERSION;
                static const double ANGLE_RATE_CONVERSION;
                static const double ACCELERATION_CONVERSION;
                
                Message(const Message::Type type);
                virtual ~Message();
                virtual void Print() const;
                virtual unsigned int Write(CxUtils::Packet& msg) const = 0;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length) = 0;
                virtual unsigned int ReadPacket(const CxUtils::Packet& msg) { return Read(msg.Ptr(), msg.Length()); }
                virtual Message* Clone() const = 0; 
                static bool IsValidMessage(const CxUtils::Packet& message);
                static bool IsValidMessage(const unsigned char* buffer,
                                           unsigned int length);
                static bool IsValidMessage2(const unsigned char* buffer,
                                           unsigned int length);
                static bool WriteChecksum(CxUtils::Packet& message);
                unsigned short GetTimerTicks() const { return mTimerTicks; }
                void SetMessageTimeStamp(const CxUtils::Time::Stamp t);
                inline CxUtils::Time::Stamp GetMessageTimeStamp() const { return mTimeStamp; }
                inline Message::Type GetType() const { return mMessageType; }
            protected:
                void CopyMessageData(const Message* msg)
                {
                    mTimeStamp = msg->mTimeStamp;
                }
                CxUtils::Time::Stamp mTimeStamp;      ///< Time of message creation.
                unsigned short mTimerTicks;           ///< Timer Ticks 
                const Message::Type mMessageType;     ///< Message type.
            };
        }
    }
}


#endif
/*  End of file */
