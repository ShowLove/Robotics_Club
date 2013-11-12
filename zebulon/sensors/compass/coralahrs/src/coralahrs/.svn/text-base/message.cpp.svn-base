//////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
/// \brief Interface class for describing Coral AHRS Messages.
///
/// Author(s): Daniel Barber<br>
/// Created: 1/17/2009<br>
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
#include "coralahrs/message.h"
#include <iostream>

using namespace Zebulon;
using namespace Compass;
using namespace CoralAHRS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Message(const unsigned char type) : mMessageType(type)
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
///   \brief Writes the message contents to a packet including header and
///   checksum, etc.
///
///   \param[in] packet Packet to write serialized message contents to.
///
///   \return Number of bytes write.  0 if nothing written or failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Write(CxUtils::Packet& packet) const
{
    CxUtils::Packet data;
    packet.Clear();
    
    WriteMessageBody(data);
  
    packet.Write(CoralMessageHeader);
    packet.Write(mMessageType);
    if(data.Length()>0)
    {
        packet.Write(((unsigned char)data.Length()));
        packet.Write(data);
    }
    packet.Write(CalculateChecksum(packet));

    return (int)packet.Length();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads a serialized Coral AHRS message and saves to internal
///   data members.
///
///   This method checks for the Coral Header 0xFF, and then validates
///   the data contents with the checksum value.  The number of bytes
///   extracted from stream is returned.
///
///   \param[in] packet Packet to read serialized message contents from.
///
///   \return Number of bytes read.  0 if nothing read or failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Message::Read(const CxUtils::Packet& packet)
{
    packet.SetReadPos(0);
    unsigned char coralHeader = 0;
    unsigned char messageType = 0;
    unsigned char dataSize = 0;
    if(packet.Read(coralHeader) && coralHeader == CoralMessageHeader)
    {
        if(packet.Read(messageType) && messageType == mMessageType)
        {
            packet.Read(dataSize);
            int bytesRead = ReadMessageBody(packet);
            if(dataSize == bytesRead)
            {
                return packet.Length();
            }
        }
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to extract valid Coral AHRS message data from a byte
///   stream.
///
///   This method checks for the Coral Header 0xFF, and then validates
///   the data contents with the checksum value.  The number of bytes
///   extracted from stream is returned.
///
///   \param[in] buffer Pointer to byte stream possibly containing message data
///                     starting at index 0.
///   \param[in] length The size of buffer.
///   \param[out] type The type of message extracted.
///   \param[out] message The extracted message contents.  Includes Coral
///                       Header, Message Type, Length, Data, Checksum.
///
///   \return Number of bytes extracted.  0 if nothing read or failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Message::ExtractMessage(const unsigned char* buffer, const unsigned int length,
                                     unsigned char& type, CxUtils::Packet& message)
{
    unsigned int bytesRead = 0;
    message.Clear();
    type = 0;
    
    if(buffer[0] == CoralMessageHeader && length >= 4)
    {
        type = buffer[1];
        unsigned char msgLength = buffer[2];
        unsigned char checksum = 0;
        // Calculate checksum to validate message.
        for(unsigned int i = 0; i < (unsigned int)(msgLength + 3); i++)
        {
            checksum += buffer[i];
        }
        //hack to reduce bad reads
        if(checksum == buffer[3 + msgLength] && msgLength < 40)
        {
            bytesRead = 4 + msgLength;
            //std::cout << "bytesRead: " << (int)bytesRead << std::endl;
            message.Write(buffer, bytesRead);
        }
    }
    
    return bytesRead;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates a checksum for the data.
///
///   \param[in] packet Packet containing coral message data (except the
///              checksum).
///
///   \return Calculated checksum.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char Message::CalculateChecksum(const CxUtils::Packet& packet)
{
    unsigned char checksum = 0;
    unsigned char* ptr = (unsigned char *)packet.Ptr();
    for(unsigned int i = 0; i < packet.Length(); i++)
    {
        checksum += ptr[i];
    }
    return checksum;
}




/*  End of File */
