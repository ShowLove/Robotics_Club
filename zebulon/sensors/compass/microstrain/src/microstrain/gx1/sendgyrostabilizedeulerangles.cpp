/////////////////////////////////////////////////////////////////////////////////////
///
/// \file sendgyrostabilizedeulerangles.cpp
/// \brief A Message structure for Send Gyro Stabilized Euler Angles.
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
#include "microstrain/gx1/sendgyrostabilizedeulerangles.h"
#include <iostream>
#include <cxutils/math/cxmath.h>
#include <stdio.h>

using namespace Zebulon;
using namespace Compass;
using namespace MicroStrain;
using namespace std;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedEulerAngles::SendGyroStabilizedEulerAngles() : Message(Message::SendGyroStabilizedEulerAngles)
{
    mRoll = mPitch = mYaw = 0;
    mTimerTicks = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedEulerAngles::SendGyroStabilizedEulerAngles(const SendGyroStabilizedEulerAngles& msg) : Message(Message::SendGyroStabilizedEulerAngles)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedEulerAngles::~SendGyroStabilizedEulerAngles()
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
unsigned int SendGyroStabilizedEulerAngles::Write(CxUtils::Packet& msg) const
{
    msg.Clear();
    msg.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    msg.WriteByte((unsigned char)mMessageType);
    short temp = 0;
    temp = (short)(mRoll/ANGLE_CONVERSION);
    msg.Write(temp);
    temp = (short)(mPitch/ANGLE_CONVERSION);
    msg.Write(temp);
    temp = (short)(mYaw/ANGLE_CONVERSION);
    msg.Write(temp);
    msg.Write(mTimerTicks);
    Message::WriteChecksum(msg);
    if(msg.Length() == 11)
    {
        return 11;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If a Send Gyro-Stabilized Euler Angles is in the buffer, the
///         contents are read and saved to internal data members.
///
///  \param buffer Pointer to array containing byte data.
///  \param length Length of the buffer in bytes.
///
///  \return Number of bytes read, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int SendGyroStabilizedEulerAngles::Read(const unsigned char* buffer,
                                                 unsigned int length)
{
    short temp;

    if(length >= 11 && 
        buffer[0] == Message::SendGyroStabilizedEulerAngles &&
        Message::IsValidMessage(buffer, 11))
    {
        //std::cout << "Euler Checksum Good\n";
        unsigned int pos = 1;
        int bytesRead = 1;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mRoll = temp*ANGLE_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mPitch = temp*ANGLE_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mYaw = temp*ANGLE_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mTimerTicks = temp;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        if(bytesRead == 11)
        {
            return bytesRead;
        }
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void SendGyroStabilizedEulerAngles::Print() const
{
    printf("Gyro-Stabilized Euler Angles: ");
    printf("%4.3lf, %4.3lf, %4.3lf, %5d\n", 
            CxUtils::CxToDegrees(mRoll),
            CxUtils::CxToDegrees(mPitch),
            CxUtils::CxToDegrees(mYaw),
            mTimerTicks);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedEulerAngles& SendGyroStabilizedEulerAngles::operator =(const SendGyroStabilizedEulerAngles& msg)
{
    CopyMessageData(&msg);
    mRoll = msg.mRoll;
    mPitch = msg.mPitch;
    mYaw = msg.mYaw;
    mTimerTicks = msg.mTimerTicks;
    return *this;
}

/*  End of File */
