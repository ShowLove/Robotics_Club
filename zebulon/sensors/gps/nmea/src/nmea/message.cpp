/////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
/// \brief Base message class for creating NMEA message structures.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/23/2008<br>
/// Last Modified: 2/23/2008<br>
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
/////////////////////////////////////////////////////////////////////////////
#include "nmea/message.h"
#include <string.h>
#include <iostream>
#include <cxutils/cxutils.h>

using namespace std;
using namespace Zebulon;
using namespace GPS;
using namespace NMEA;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
///  \param type The message type.
///  \param header Message header string.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Message(const Message::Type type, const std::string& header) : mMessageType(type), mMessageHeader(header)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Message::~Message()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks if a message is valid or not by looking at its format.
///
///  \param msg Message within a Packet structure to check for validity.
///
///  \return True if valid, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::IsValidMessage(const CxUtils::Packet& msg)
{
    bool result = false;
    msg.SetReadPos(0);
    const char* data = (const char *)msg.Ptr();
    char *ptr = NULL;
    unsigned int len = msg.Length();
    unsigned int astrk = 0;
    unsigned int byte1, byte2, check = 0;
    // At a minimum include "$*CC"
    if(len <= 4)
        return result;
    
    if(*data != '$')
        return result;


    // If no '*' then no check sum data present.
    for(unsigned int i = 1; i < len; i++)
    {
        if(data[i] == '*')
        {
            astrk = i;
            break;
        }
    }
    // Asterisk not found.
    if(astrk == 0 || astrk + 2 >= len)
    {
        return result;
    }
    
    // Convert the ASCII Characters for checksum
    // to those readable for comparison.
    byte2 = isdigit(data[astrk + 1]) ? (data[astrk + 1] - '0') : (data[astrk + 1] - 'A' + 10);
    byte1 = isdigit(data[astrk + 2]) ? (data[astrk + 2] - '0') : (data[astrk + 2] - 'A' + 10);

    // Now calculate what the checsum should be.
    for(unsigned int i = 1; i < astrk && i < len; i++)
    {
        check ^= data[i];
    }

    if( byte1 == (check & 0x0F) && 
        byte2 == (check >> 4) )
    {
        result = true;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks the buffer for a valid message and extracts it to the packet.
///
///  \param buff Pointer to buffer containing NMEA message.
///  \param len The length of the buffer.
///  \param msg Extracted NMEA message.
///  \param header If not NULL a copy of the message header data.
///
///  \return True if a message was found and copied to the Packet, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::ExtractMessage(const unsigned char* buff,
                             const unsigned int len,
                             CxUtils::Packet& msg,
                             std::string* header)
{
    bool result = false;
    msg.SetReadPos(0);
    const char* data = (const char *)buff;
    char *ptr = NULL;
    unsigned int astrk = 0, total = 0;
    unsigned int byte1, byte2, check = 0;

    msg.Clear();

    // At a minimum include "$*CC"
    if(len <= 4)
        return result;
    
    if(*data != '$')
        return result;


    // If no '*' then no check sum data present.
    for(unsigned int i = 1; i < len; i++)
    {
        if(data[i] == '*')
        {
            astrk = i;
            break;
        }
    }
    // Asterisk not found.
    if(astrk == 0 || astrk + 2 >= len)
    {
        return result;
    }
    if(data[astrk + 1] < '0' || data[astrk + 1] > 'F' ||
       data[astrk + 2] < '0' || data[astrk + 2] > 'F')
    {
        return result;
    }
    // Convert the ASCII Characters for checksum
    // to those readable for comparison.
    byte2 = isdigit(data[astrk + 1]) ? (data[astrk + 1] - '0') : (data[astrk + 1] - 'A' + 10);
    byte1 = isdigit(data[astrk + 2]) ? (data[astrk + 2] - '0') : (data[astrk + 2] - 'A' + 10);

    total = 1;
    // Now calculate what the checsum should be.
    for(unsigned int i = 1; i < astrk && i < len; i++)
    {
        total++;
        check ^= data[i];
    }
    total += 3;
    
    if( byte1 == (check & 0x0F) && 
        byte2 == (check >> 4) )
    {
        msg.Write(buff, total);
        if(header)
        {
            header->clear();
            for(unsigned int i = 1; i < astrk && i < len; i++)
            {
                if(buff[i] != ',')
                {
                    header->push_back((char)buff[i]);
                }
                else
                {
                    break;
                }
            }
        }
        result = true;
    }

    return result;
} 

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the time stamp to associate with the message.  This has no
///         affect on the message contents.
///
///  \param t Time information in UTC.
///
////////////////////////////////////////////////////////////////////////////////////
void Message::SetMessageTimeStamp(const CxUtils::Time::Stamp t)
{
    mTimeStamp = t;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Prints message data to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void Message::Print() const
{
    CxUtils::Packet p;
    if(Write(p))
    {
        cout << (char *)p.Ptr();
    }
    else
    {
        cout << "Message::Print::Error.\n";
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a checksum to the end of the message.
///
///  Format of message must be "$DATA*" where DATA is your message data.
///
///  \param msg NMEA message needing a checksum added to the end.
///
///  \return True if written, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::WriteChecksum(CxUtils::Packet& msg)
{
    bool result = false;
    msg.SetReadPos(0);
    const char* data = (const char *)msg.Ptr();
    char *ptr = NULL;
    unsigned int len = msg.Length();
    unsigned int astrk = 0;
    unsigned int byte1, byte2, check = 0;
    // At a minimum include "$*CC"
    if(len <= 4)
        return result;
    
    if(*data != '$')
        return result;


    // If no '*' then no check sum data present.
    for(unsigned int i = 1; i < len; i++)
    {
        if(data[i] == '*')
        {
            astrk = i;
            break;
        }
    }
    // Asterisk not found.
    if(astrk == 0 || astrk < len && data[astrk] != '*')
    {
        return result;
    }
    
    // Now calculate what the checsum should be.
    for(unsigned int i = 1; i < astrk && i < len; i++)
    {
        check ^= data[i];
    }
    byte1 = (check & 0x0F);
    byte2 = (check >> 4);
    result = true;

    byte1 = byte1 < 10 ? byte1 + '0' : byte1 + 'A' - 10;
    byte2 = byte2 < 10 ? byte2 + '0' : byte2 + 'A' - 10;
    msg.SetWritePos(msg.Length());
    msg.WriteByte(byte2);
    msg.WriteByte(byte1);
    msg.WriteByte('\r');   // CR
    msg.WriteByte('\n');   // LF

    return result;
}

/*  End of File */
