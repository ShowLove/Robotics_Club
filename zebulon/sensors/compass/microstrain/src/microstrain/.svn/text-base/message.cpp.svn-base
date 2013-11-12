/////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
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
/////////////////////////////////////////////////////////////////////////////////////
#include "microstrain/message.h"
#include <iostream>

using namespace std;
using namespace Zebulon;
using namespace Compass;
using namespace MicroStrain;

//Magic numbers from GX1 documentation
const double Message::ANGLE_CONVERSION = (CxUtils::CX_TWO_PI/65536.0);
const double Message::ANGLE_RATE_CONVERSION = (8500.0/32768000.0);
const double Message::ACCELERATION_CONVERSION = (7000.0/32768000.0*9.81);

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
///  \param type The type of message.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Message(const Message::Type type) : mMessageType(type), mTimerTicks(0)
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
    return IsValidMessage(msg.Ptr(), msg.Length());
}
 

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks if a message is valid or not by looking at its format.
///
///  \param buffer Pointer to serialized data.
///  \param len Length of the buffer in bytes.
///
///  \return True if valid, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::IsValidMessage(const unsigned char *buffer,
                             const unsigned int len)
{
    unsigned char command = 0;
    unsigned short temp = 0;
    unsigned short calculatedChecksum = 0, checksum = 0;
    unsigned int pos = 0;

    if(CxUtils::Packet::Read(buffer, len, pos, command, CxUtils::CX_PACKET_BIG_ENDIAN))
    {
        pos += 1;
        calculatedChecksum = command;
        while(CxUtils::Packet::Read(buffer, len, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN) &&
              pos < len - 2)
        {
            calculatedChecksum += temp;
            pos += 2;
        }
        if(CxUtils::Packet::Read(buffer, len, pos, checksum, CxUtils::CX_PACKET_BIG_ENDIAN) &&
            calculatedChecksum == checksum)
        {
            return true;
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks if a message is valid or not by looking at its format for GX2
///
///  \param buffer Pointer to serialized data.
///  \param len Length of the buffer in bytes.
///
///  \return True if valid, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::IsValidMessage2(const unsigned char *buffer,
                             const unsigned int len)
{
    unsigned char temp = 0;
    unsigned short calculatedChecksum = 0, checksum = 0;
    unsigned int pos = 0;


    calculatedChecksum = 0;
    while(CxUtils::Packet::Read(buffer, len, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN) &&
          pos < len - 2)
    {
        calculatedChecksum += temp;
        pos += 1;
    }
    if(CxUtils::Packet::Read(buffer, len, pos, checksum, CxUtils::CX_PACKET_BIG_ENDIAN) &&
        calculatedChecksum == checksum)
    {
        return true;
    }

    return false;
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
        for(unsigned int i = 0; i < p.Length(); i++)
        {
            cout << (int)(p.Ptr()[i]);
        }
        cout << endl;
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
///  \param msg Serialized message to add up the header + 16 bit integers
///             to calculate the checksum.
///
///  \return True if written, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Message::WriteChecksum(CxUtils::Packet& msg)
{
    msg.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    unsigned char command = 0;
    unsigned short temp;
    unsigned short checksum = 0;

    msg.SetReadPos(0);
    if(msg.Read(command))
    {
        checksum = command;
        while(msg.Read(temp))
        {
            checksum += temp;
        }
        msg.SetWritePos(msg.Length());
        msg.Write(checksum);
        return true;
    }

    return false;
}



/*  End of File */
