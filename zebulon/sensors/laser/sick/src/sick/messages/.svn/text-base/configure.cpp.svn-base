/////////////////////////////////////////////////////////////////////////////////////
///
/// \file configure.cpp
/// \brief Message class for configuring the SICK LIDAR.
///
/// Author(s): Jon Carbone<br>
/// Created: 4/5/2008<br>
/// Last Modified: 4/5/2008<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: joncarbone@gmail.com<br>
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
#include "sick/messages/configure.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Configure::Configure() : Message(Message::Configure),
                         mDistanceMode(MM)
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
Configure::Configure(const Configure& msg) : Message(Message::Configure)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a Configure message to a packet
///
///  \param packet Packet to write the Configure message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int Configure::Write(CxUtils::Packet& packet) const
{
    packet.Clear();

    WriteHeader(packet, mMessageType, 33);

    packet.Write((unsigned short)0x0000);
    packet.Write((unsigned short)0x0000);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(mDistanceMode);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x02);
    packet.WriteByte(0x02);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte(0x00);
    packet.WriteByte((unsigned short)0x0000);
    packet.WriteByte((unsigned short)0x0000);

    //packet.Write((unsigned short)0x0000); //a
    //packet.Write((unsigned short)0x0046); //b
    //packet.WriteByte(0x00); //c
    //packet.WriteByte(0x00); //d
    //packet.WriteByte(mDistanceMode); //e
    //packet.WriteByte(0x00); //f
    //packet.WriteByte(0x00); //g
    //packet.WriteByte(0x02); //h
    //packet.WriteByte(0x02); //i
    //packet.WriteByte(0x02); //j
    //packet.WriteByte(0x00); //k
    //packet.WriteByte(0x00); //l
    //packet.WriteByte(0x0A); //m
    //packet.WriteByte(0x0A); //n
    //packet.WriteByte(0x50); //o
    //packet.WriteByte(0x64); //p
    //packet.WriteByte(0x00); //q
    //packet.WriteByte(0x0A); //r
    //packet.WriteByte(0x0A); //s
    //packet.WriteByte(0x50); //t
    //packet.WriteByte(0x64); //u
    //packet.WriteByte(0x00); //v
    //packet.WriteByte(0x0A); //w
    //packet.WriteByte(0x0A); //x
    //packet.WriteByte(0x50); //y
    //packet.WriteByte(0x64); //z
    //packet.WriteByte(0x00); //a1
    //packet.WriteByte(0x00); //a2
    //packet.WriteByte((unsigned short)0x0000); //a3
    //packet.WriteByte((unsigned short)0x0002); //a4

    WriteCRC(packet);

    return packet.Length();
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a Configure message and populates the internal members
///
///  \param packet The packet containing the Configure message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int Configure::Read(const CxUtils::Packet& packet)
{

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the Configure message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int Configure::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A Configure message to copy values from
///
///  \return A pointer to the instance of the Configure message
///
///////////////////////////////////////////////////////////////////////////////
Configure& Configure::operator=(const Configure& msg)
{
    if( this != &msg )
    {
        mAddress = msg.mAddress;
        mDistanceMode = msg.mDistanceMode;
    }

    return *this;
}


/*  End of File */
