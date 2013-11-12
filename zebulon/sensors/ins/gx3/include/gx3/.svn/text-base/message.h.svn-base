/////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief A simple class fordefining message structures/data for 
///        MicroStrain gx3.
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
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
#ifndef __ZEBULON_GX3_MESSAGE__H
#define __ZEBULON_GX3_MESSAGE__H

#include <string>
#include <cxutils/packet.h>
#include <map>
#include <cxutils/math/cxmath.h>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Namespace for all software developed by the Robotics Laboratory and
///         Club at the University of Central Florida.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace INS
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace MicroStrain 
        ///  \brief MicroStrain sensors, etc. namespace for the MicroStrain library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Message 
            ///  \brief Message class RS232 communication for MicroStrain Sensors.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class Message
            {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum Type
                ///  \brief Enumeration of all message types.
                ///
                ////////////////////////////////////////////////////////////////////////////////////

                /* Two part message
                First: Who is it for/general type of message
                Second: What type of message (these repeat)
                And multiple sub-types can be in a single packet
                */

                enum Type
                {
                    //Commands

                    BaseCommandSet = 0x01,
                    Ping = 0x0101,
                    SetToIdle = 0x0102,
                    GetDeviceInformation = 0x0103,
                    GetDeviceDescriptorSets = 0x0104,
                    DeviceBuiltInTest = 0x0105,
                    Resume = 0x0106,
                    DeviceReset = 0x017E,

                    ThreeDMCommandSet = 0x0C,
                    PollAHRSData = 0x0C01,
                    PollGPSData = 0x0C02,
                    PollNAVData = 0x0C03,
                    GetAHRSDataRateBase = 0x0C06,
                    GetGPSDataRateBase = 0x0C07,
                    GetNAVDataRateBase = 0x0C0B,
                    AHRSMessageFormat = 0x0C08,
                    GPSMessageFormat = 0x0C09,
                    NAVMessageFormat = 0x0C0A,
                    EnableDisableDeviceContinuousDataStream = 0x0C11,
                    DeviceStartupSettings = 0x0C30,
                    AHRSSignalConditioningSettings = 0x0C35,
                    ChangeUARTBAUDrate = 0x0C40,
                    DeviceStatus = 0x0C64,

                    NavigationFilterCommandSet = 0x0D,
                    ResetFilter = 0x0D01,
                    SetInitialAttitude = 0x0D02,
                    SetInitialHeading = 0x0D03,
                    SetInitialAttitudefromAHRS = 0x0D04,
                    VehicleDynamicsMode = 0x0D10,
                    SensortoVehicleFrameTransformation = 0x0D11,
                    SensortoVehicleFrameOffset = 0x0D12,
                    AntennaOffset = 0x0D13,
                    BiasEstimationControl = 0x0D14,
                    GPSSourceControl = 0x0D15,
                    ExternalGPSUpdate = 0x0D16,
                    ExternalHeadingUpdate = 0x0D17,
                    HeadingUpdateControl = 0x0D18,
                    AutoInitializationControl = 0x0D19,
                    AccelerometerWhiteNoiseStandardDeviation = 0x0D1A,
                    GyroscopeWhiteNoiseStandardDeviation = 0x0D1B,
                    GyroscopeBiasModelParameters = 0x0D1D,

                    SystemCommandSet = 0x7F,
                    CommunicationMode = 0x7F10,

                    //Data

                    AHRSDataSet = 0x80,
                    ScaledAccelerometerVector = 0x8004,
                    ScaledGyroVector = 0x8005,
                    ScaledMagnetometerVector = 0x8006,
                    DeltaThetaVector = 0x8007,
                    DeltaVelocityVector = 0x8008,
                    OrientationMatrix = 0x8009,
                    Quaternion = 0x800A,
                    EulerAngles = 0x800C,
                    GPSCorrelatedTimestamp = 0x8012,

                    GPSDataSet = 0x81,
                    LLHPosition = 0x8103,
                    NEDVelocity = 0x8105,
                    UTCTime = 0x8108,
                    GPSTime = 0x8109,
                    HardwareStatus = 0x810D,

                    NAVDataSet = 0x82,
                    FilterStatus = 0x8210,
                    GPSTimestamp = 0x8211,
                    EstimatedLLHPosition = 0x8201,
                    EstimatedNEDVelocity = 0x8202,
                    EstimatedOrientationQuaternion = 0x8203,
                    EstimatedOrientationMatrix = 0x8204,
                    EstimatedOrientationEulerAngles = 0x8205,
                    EstimatedGyroBias = 0x8206,
                    EstimatedLLHPositionUncertainty = 0x8208,
                    EstimatedNEDVelocityUncertainty = 0x8209,
                    EstimatedAttitudeUncertainty = 0x820A,
                    EstimatedGyroBiasUncertainty = 0x820B,
                    EstimatedLinearAcceleration = 0x820D,
                    EstimatedAngularRate = 0x820E,
                    WGS84LocalGravityMagnitude = 0x820F,
                    EstimatedGravityVector = 0x8213,
                    HeadingUpdateSourceState = 0x8214,
                    MagneticModelSolution = 0x8215,

                    Null = 0x00

                };
                
                Message(const Message::Type type,const unsigned char length);
                virtual ~Message();
                virtual void Print() const;
                virtual unsigned int Write(CxUtils::Packet& msg) const = 0;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length) = 0;
                virtual unsigned int ReadPacket(const CxUtils::Packet& msg) { return Read(msg.Ptr(), msg.Length()); }
                virtual Message* Clone() const = 0; 
                static bool IsValidMessage(const CxUtils::Packet& message) { return IsValidMessage(message.Ptr(), message.Length()); }
                static bool IsValidMessage(const unsigned char* buffer,
                                           unsigned int length);
                static bool WriteChecksum(CxUtils::Packet& message);
                unsigned short GetTimerTicks() const { return mTimerTicks; }
                void SetMessageTimeStamp(const CxUtils::Time::Stamp t);
                inline CxUtils::Time::Stamp GetMessageTimeStamp() const { return mTimeStamp; }
                inline Message::Type GetType() const { return mMessageType; }

                //start bytes
                const static unsigned char GX3Sync1 = 0x75;
                const static unsigned char GX3Sync2 = 0x65;

                static unsigned short CalcCheckSum(const unsigned char *msg,unsigned int length);
                //requested messages (one per device)
                static CxUtils::Packet ConstructCommand(std::vector<unsigned short> Messages,double Frequency);
                static CxUtils::Packet ConstructCommand(std::string Data);

            protected:

                void CopyMessageData(const Message* msg)
                {
                    mTimeStamp = msg->mTimeStamp;
                }
                CxUtils::Time::Stamp mTimeStamp;      ///< Time of message creation.
                unsigned short mTimerTicks;           ///< Timer Ticks 
                const Message::Type mMessageType;     ///< Message type
                const unsigned char mFieldLength;
            };
        }
    }
}


#endif
/*  End of file */
