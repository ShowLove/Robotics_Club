/////////////////////////////////////////////////////////////////////////////////////
///
/// \file setcontinuousmode.cpp
/// \brief A Message structure for Set Continuous Mode.
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
#include "microstrain/gx1/setcontinuousmode.h"
#include <iostream>
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Compass;
using namespace MicroStrain;
using namespace std;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
SetContinuousMode::SetContinuousMode() : Message(Message::SetContinuousMode)
{
    mTimerTicks = 0;
    mCommandByte = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetContinuousMode::SetContinuousMode(const SetContinuousMode& msg) : Message(Message::SetContinuousMode)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetContinuousMode::~SetContinuousMode()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Serializes data to packet.
///
///  \param msg Message packet to write to.
///
///  \return Number of bytes written, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int SetContinuousMode::Write(CxUtils::Packet& msg) const
{
    msg.Clear();
    msg.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    msg.WriteByte((unsigned char)mMessageType);
    msg.WriteByte(0x00);
    msg.WriteByte(mCommandByte);
    msg.Write(mTimerTicks);
    Message::WriteChecksum(msg);
    if(msg.Length() == 7)
    {
        return 11;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads message data if within buffer.
///
///  \param buffer Pointer to array containing byte data.
///  \param length Length of the buffer in bytes.
///
///  \return Number of bytes read, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int SetContinuousMode::Read(const unsigned char* buffer,
                                     unsigned int length)
{
    short temp;
    unsigned char command;
    if(length >= 7 && 
        buffer[0] == Message::SetContinuousMode &&
        Message::IsValidMessage(buffer, 7))
    {
        unsigned int pos = 1;
        CxUtils::Packet::Read(buffer, length, pos, command, CxUtils::CX_PACKET_BIG_ENDIAN);
        pos += 1;
        CxUtils::Packet::Read(buffer, length, pos, command, CxUtils::CX_PACKET_BIG_ENDIAN);
        mCommandByte = command;
        pos += 1;
        CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mTimerTicks = temp;
        return 11;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SetContinuousMode& SetContinuousMode::operator =(const SetContinuousMode& msg)
{
    CopyMessageData(&msg);
    mCommandByte = msg.mCommandByte;
    mTimerTicks = msg.mTimerTicks;
    return *this;
}

/*  End of File */
