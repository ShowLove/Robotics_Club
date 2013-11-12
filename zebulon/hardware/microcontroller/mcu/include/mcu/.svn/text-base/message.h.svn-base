//////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief Message class for deriving messages to send to/from and Micrcontroller.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/14/2009<br>
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
#ifndef _ZEBULON_MCU_MESSAGE__H
#define _ZEBULON_MCU_MESSAGE__H

#include <cxutils/packet.h>
#include <vector>
#include <iostream>

#ifndef ZEB_MICROCONTROLLER_DLL
#ifdef WIN32
#define ZEB_MICROCONTROLLER_DLL __declspec(dllimport)
#else
#define ZEB_MICROCONTROLLER_DLL
#endif

#endif

namespace Zebulon
{
    namespace MCU
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Message
        ///   \brief Interface class for creating messages structures for serialization
        ///          and de-serialization of data sent to/from a Microcontroller.
        ///
        ///   MCU messages are designed to be very simple to make it easy to add
        ///   support for them on any type of Microcontroller.  Messages
        ///   can be encoded in two formats depending on your need.  By default messages
        ///   are of a fixed size and have a unique header (two bytes) and 
        ///   a simple checksum at the end.  As an alternative you can create a
        ///   dynamic message as well using the SLIP format.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL Message
        {        
        public:
            typedef std::vector<Message*> List;                ///<  Type definition for a Vector of Messages.
            typedef unsigned char Code;                        ///<  Type definition for message command code.
            
            static const Code All                   = 0xFF;
            static const Code QueryReport           = 0x00;       
            static const Code Ping                  = 0x0D;       

            static const Code SetServoOutputs       = 0x0B;
            static const Code SetDigitalOutputs     = 0x0C;
            static const Code SetAnalogOutputs      = 0x0E;
            static const Code SetMotorOutput        = 0x0F;
            
            static const Code ReportDigitalInputs   = 0x82;
            static const Code ReportAnalogInputs    = 0x83;
            static const Code Pong                  = 0x84; 
            static const Code ReportEncoders        = 0x85;

            static const Code ReportID              = 0x8F;

            // Constructor (must set message type here).
            Message(const Message::Code messageCode);
            // Destructor.
            virtual ~Message();
            // Converts message data to byte array format (calls WriteMessageBody to write payload).
            virtual int Write(CxUtils::Packet& packet) const;
            // Writes message payload data to packet, returning bytes written (overload this for your data).
            virtual int WriteMessageBody(CxUtils::Packet& packet) const { return 0; }
            // Converts from data in byte array (calls ReadMessageBody to read payload).
            virtual int Read(const CxUtils::Packet& packet);
            // Reads message payload data from packet, returning bytes read (overload this for your data).
            virtual int ReadMessageBody(const CxUtils::Packet& packet) { return 0; }
            // Method to calculate a checksum for a message in a packet.
            static unsigned char CalculateChecksum(const CxUtils::Packet& packet,
                                                   const bool staticMessage = true);
            // Returns true if the packet contains a valid message.
            static bool IsValid(const CxUtils::Packet& packet,
                                const bool staticMessage = true);    
            // Extracts a complete message from the buffer if present, returns number of bytes read.
            static unsigned int ExtractMessage(const unsigned char* buffer, 
                                               const unsigned int length, 
                                               Message::Code& type, CxUtils::Packet& message,
                                               const bool staticMessage = true);
            static bool GetMessageCode(const unsigned char* buffer, 
                                       const unsigned int length, 
                                       Message::Code& type,
                                       const bool staticMessage = true);
            virtual void Print() const {};
            virtual Message* Clone() const = 0;
   
            Message::Code mMessageCode;     ///<  Message code (type of message).
            bool mUseStaticFormatFlag;      ///<  If true, read and write using static message formats.
            CxUtils::Time mTimeStamp;       ///<  Time when message was received.

            // Constants used for message reading/writing.
            static const unsigned char StaticHeader1 = '#';         ///<  First Header byte for all Messages.
            static const unsigned char StaticHeader2 = '%';         ///<  Second Header byte for all Messages.
            static const unsigned int StartOfStaticCommandType = 2; ///<  Starting index of command type.
            static const unsigned int StartOfStaticData = 3;        ///<  Starting index of data in byte array.
            static const unsigned int StaticMessageSize = 8;        ///<  Size of all messages in bytes.
            static const unsigned int StaticDataSize = 4;           ///<  Size of all data in bytes.
            
            static const unsigned int DynamicHeaderLength = 2;         ///<  Number of bytes in packet header: [length][type]
            static const unsigned int DynamicTrailerLength = 1;        ///<  Number of bytes in packet trailer: [checksum]
            static const unsigned int DynamicLengthOffset = 0;         ///<  Number of bytes from frame[0] to [length]
            static const unsigned int DynamicMessageCodeOffset = 1;           ///<  Number of bytes from frame[0] to [type]
            static const unsigned int DynamicDataOffset = 2;           ///<  Number of bytes from frame[0] to first byte of [data]
            static const unsigned int DynamicChecksumOffset = 3;       ///<  Number of bytes from frame[0]+length to [checksum]
        }; 

        static const Message::Code QUERY_REPORT             = Message::QueryReport;       
        static const Message::Code PING                     = Message::Ping;       
        static const Message::Code PONG                     = Message::Pong;       

        static const Message::Code SET_SERVO_OUTPUTS        = Message::SetServoOutputs;
        static const Message::Code SET_DIGITAL_OUTPUTS      = Message::SetDigitalOutputs;
        static const Message::Code SET_ANALOG_OUTPUTS       = Message::SetAnalogOutputs;
        
        static const Message::Code REPORT_DIGITAL_INPUTS    = Message::ReportDigitalInputs;
        static const Message::Code REPORT_ANALOG_INPUTS     = Message::ReportAnalogInputs;

        static const Message::Code REPORT_ID                = Message::ReportID;
    }
}
#endif //_ZEBULON_MCU_MESSAGE__H
/*  End of File */
