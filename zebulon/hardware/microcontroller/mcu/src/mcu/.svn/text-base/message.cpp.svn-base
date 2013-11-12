//////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
/// \brief Message class for deriving Underwater Vehicle communication messages.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/14/2009br>
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
#include "mcu/message.h"

using namespace Zebulon;
using namespace MCU;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets message type code.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Message(const Message::Code messageCode) : mMessageCode(messageCode),
                                                    mUseStaticFormatFlag(true)
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Message::~Message()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the message contents to an MCU packet format for transmision
///   over a network or serial connection.
///
///   \param[out] packet The packet to write data to.
///
///   \return Number of bytes written, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Write(CxUtils::Packet& packet) const
{
    CxUtils::Packet payload;
    packet.Clear();
    if(mUseStaticFormatFlag == false)
    {
        unsigned char length = (unsigned char)(WriteMessageBody(payload) + DynamicHeaderLength + DynamicTrailerLength);
        packet.WriteByte(length);
        packet.WriteByte(mMessageCode);
        packet.Write(payload);
        packet.WriteByte(CalculateChecksum(packet, false));
    }
    else
    {
        packet.WriteByte(Message::StaticHeader1);
        packet.WriteByte(Message::StaticHeader2);
        packet.WriteByte(mMessageCode);
        WriteMessageBody(payload);
        // This allows us to default messages to dynamic
        // sizes in their implementation, and then they
        // can be fixed to support static message lengths
        // for very simple microcontrollers that can't support
        // dynamic messages like in SLIP.
        if(payload.Length() > 4)
        {
            return 0;
        }
        while(payload.Length() < 4)
        {
            payload.WriteByte(0);
        }
        packet.Write(payload);        
        packet.WriteByte(CalculateChecksum(packet));
    }
    
    return (int)packet.Length();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the contents of the packet, overwriting internal data 
///   members.
///
///   \param[in] packet The packet to read data from.
///
///   \return Number of bytes read, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Read(const CxUtils::Packet& packet)
{
    packet.SetReadPos(0);
    if(mUseStaticFormatFlag == false)
    {
        unsigned int bytesRead = 0;
        unsigned char checksum;
        unsigned char length = 0;
        bytesRead += packet.Read(length);
        bytesRead += packet.Read(mMessageCode);
        CxUtils::Packet payload;
        bytesRead += packet.Read(payload, (unsigned int)length - DynamicHeaderLength - DynamicTrailerLength);
        ReadMessageBody(payload);
        bytesRead += packet.Read(checksum);
        return bytesRead;
    }
    else
    {
        unsigned char header = 0;
        unsigned char messageCode = 0;
        unsigned char checksum = 0;
        
        if(IsValid(packet, true) &&
           packet.Read(header) &&
           packet.Read(header) &&
           packet.Read(messageCode) && 
           messageCode == mMessageCode)
       {
           return packet.Length();
       }
    }
   return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to calculate a checksum for an MCU message.
///
///   \param[in] packet The packet data to calcualte a checksum for.
///   \param[in] staticMessage If true, read from a static (fixed size) message
///                            encoding scheme.
///
///   \return Calculated checksum.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char Message::CalculateChecksum(const CxUtils::Packet& packet, const bool staticMessage)
{
    unsigned char checksum = 0;
    const unsigned char* ptr = packet.Ptr();
    if(staticMessage)
    {
        for(unsigned int i = 0; i < packet.Length(); i++)
        {
            checksum += ptr[i];
        }
    }
    else
    {
        unsigned char length = ptr[DynamicLengthOffset];
        for(unsigned int i = 0; i < (int)(length - DynamicTrailerLength); i++)
        {
            checksum += ptr[i];
        }
    }
    return checksum;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Verifies packet contents as valid message.
///
///   \param[in] packet Packet to verify.
///   \param[in] staticMessage If true, read from a static (fixed size) message
///                            encoding scheme.
///   \return True if the packet has a valid checksum, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::IsValid(const CxUtils::Packet& packet, const bool staticMessage)
{
    unsigned char checksum = 0;
    const unsigned char* ptr = packet.Ptr();
    if(staticMessage)
    {
        if(packet.Length() >= Message::StaticMessageSize)
        {
            if(ptr[0] == Message::StaticHeader1 && ptr[1] == Message::StaticHeader2)
            {
                for(unsigned int i = 0; i < Message::StaticMessageSize - 1; i++)
                {
                    checksum += ptr[i];
                }
                if(checksum == ptr[Message::StaticMessageSize - 1])
                {
                    return true;
                }
            }
        }
    }
    else
    {
        if(packet.Length() >= DynamicHeaderLength + DynamicTrailerLength)
        {
            unsigned char length = ptr[DynamicLengthOffset];
            if((length >= DynamicHeaderLength + DynamicTrailerLength) && 
                (length == packet.Length()) && (CalculateChecksum(packet, false) == ptr[length-1]))
            {
                return true;
            }
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to extract valid MCU message data from a byte
///   stream.
///
///   This method checks for the MCU header '#', and then validates
///   the data contents with the checksum value.  The number of bytes
///   extracted from stream is returned.
///
///   \param[in] buffer Pointer to byte stream possibly containing message data
///                     starting at index 0.
///   \param[in] length The size of buffer.
///   \param[out] type The type of message extracted.
///   \param[out] message The extracted message contents.  Includes Coral
///                       Header, Message Type, Length, Data, Checksum.
///   \param[in] staticMessage If true, read as fixed format message.
///
///   \return Number of bytes extracted.  0 if nothing read or failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Message::ExtractMessage(const unsigned char* buffer, 
                                     const unsigned int length,
                                     Message::Code& type, CxUtils::Packet& message,
                                     const bool staticMessage)
{
    unsigned int bytesRead = 0;
    message.Clear();
    if(staticMessage)
    {
        if(length >= Message::StaticMessageSize)
        {
            message.Write(buffer, Message::StaticMessageSize);
            if(IsValid(message, staticMessage))
            {
                type = (Message::Code)buffer[StartOfStaticCommandType];
                bytesRead = Message::StaticMessageSize;
            }
        }
    }

    return bytesRead;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the message code in the stream.
///
///   \param[in] buffer Pointer to byte stream possibly containing message data
///                     starting at index 0.
///   \param[in] length The size of buffer.
///   \param[out] type The type of message extracted.
///   \param[in] staticMessage If true, read as fixed format message.
///
///   \return Number of bytes extracted.  0 if nothing read or failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::GetMessageCode(const unsigned char* buffer, 
                             const unsigned int length,
                             Message::Code& type,
                             const bool staticMessage)
{
    CxUtils::Packet::Wrapper wrapper((unsigned char *)buffer, length);
    if(staticMessage)
    {
        if(length >= Message::StaticMessageSize)
        {
            if(IsValid(*wrapper.GetData(), staticMessage))
            {
                type = (Message::Code)buffer[StartOfStaticCommandType];
                return true;
            }
        }
    }
    else
    {
        if(IsValid(*wrapper.GetData(), false))
        {
            type = (Message::Code)buffer[Message::DynamicMessageCodeOffset];
            return true;
        }
    }
    return false;
}


/*  End of File */
