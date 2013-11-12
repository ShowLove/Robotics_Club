//////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief Interface class for describing Coral AHRS Messages.
///
/// Author(s): Daniel Barber<br>
/// Created: 1/17/2009br>
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
#ifndef _ZEBULON_CORAL_AHRS_MESSAGE__H
#define _ZEBULON_CORAL_AHRS_MESSAGE__H

#include <cxutils/math/cxmath.h>
#include <cxutils/packet.h>
#include <vector>

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_CORALAHRS_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_CORALAHRS_DLL
    #ifdef WIN32
        #define ZEB_CORALAHRS_DLL __declspec(dllimport)
    #else
        #define ZEB_CORALAHRS_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            #define DoubleToCoral(x) (short)((x)*4096.0)
            #define CoralToDouble(x) ((double)(x)/4096.0)
            const unsigned char CoralMessageHeader = 0xFF; ///<  Header for all Coral Messages.
            
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Message
            ///   \brief Class for describing/defining messages sent to/from the Coral 
            ///   AHRS.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL Message
            {
            public:
                enum From
                {
                    DataQuat = 0x21,
                    DataEuler = 0x22,
                    DataMatrix = 0x23,
                    DataSensors = 0x24,
                    DataQuatAndSensors = 0x25,
                    DataEulerAndSensors = 0x26,
                    DataMatrixAndSensors = 0x27,
                    DataRawSensors = 0x28,
                    DataQuatAndRawSensors = 0x29,
                    DataEulerAndRawSensors = 0x2A,
                    DataMatrixAndRawSensors = 0x2B,
                    IDString = 0x15,
                    Configuration = 0x1A,
                    Calibration = 0x1C,
                    Pong = 0xEE,
                };
                enum To
                {
                    SetOutputMode = 0x01,
                    SetCalibration = 0x02,
                    CaptureGyroBias = 0x03,
                    RequestID = 0x05,
                    RestoreUserSettings = 0x06,
                    RestoreFactorySettings = 0x07,
                    SetOutputRateDivisor = 0x08,
                    SetSerialRate = 0x09,
                    RequestConfiguration = 0x0A,
                    RequestCalibration = 0x0C,
                    SaveSettings = 0x0F,
                    Ping = 0xDD
                };
                typedef std::vector<Message*> List;  ///<  List of messages.
                Message(const unsigned char type);
                virtual ~Message();
                int Write(CxUtils::Packet& packet) const;
                virtual int WriteMessageBody(CxUtils::Packet& packet) const = 0;
                int Read(const CxUtils::Packet& packet);
                virtual int ReadMessageBody(const CxUtils::Packet& packet) = 0;
                virtual Message* Clone() const = 0;
                virtual void Print() const {}
                static unsigned int ExtractMessage(const unsigned char* buffer, const unsigned int length, 
                                                unsigned char& type, CxUtils::Packet& message);
                static unsigned char CalculateChecksum(const CxUtils::Packet& packet);
                const unsigned char mMessageType;   ///<  Type of Coral Message.
            };
        }
    }
}

#endif
/*  End of File */
