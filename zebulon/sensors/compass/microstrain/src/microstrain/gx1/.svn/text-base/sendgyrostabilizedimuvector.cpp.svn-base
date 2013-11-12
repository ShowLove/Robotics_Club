/////////////////////////////////////////////////////////////////////////////
///
/// \file sendgyrostabilizedimuvector.cpp
/// \brief A Message structure for Send Gyro Stabilized Euler Angles & Accel & Rate Vector.
///
/// Author(s): Gary Stein<br>
/// Created: 5/15/2010<br>
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
/////////////////////////////////////////////////////////////////////////////////////
#include "microstrain/gx1/sendgyrostabilizedimuvector.h"
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
SendGyroStabilizedIMUVector::SendGyroStabilizedIMUVector() : Message(Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector)
{
    mRoll = mPitch = mYaw = 0.0;
    mDRoll = mDPitch = mDYaw = 0.0;
    mDDX = mDDY = mDDZ = 0.0;
    mTimerTicks = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedIMUVector::SendGyroStabilizedIMUVector(const SendGyroStabilizedIMUVector& msg) : Message(Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedIMUVector::~SendGyroStabilizedIMUVector()
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
unsigned int SendGyroStabilizedIMUVector::Write(CxUtils::Packet& msg) const
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
    
    temp = (short)(mDDX/ACCELERATION_CONVERSION);
    msg.Write(temp);
    temp = (short)(mDDY/ACCELERATION_CONVERSION);
    msg.Write(temp);
    temp = (short)(mDDZ/ACCELERATION_CONVERSION);
    msg.Write(temp);
    
    temp = (short)(mDRoll/ANGLE_RATE_CONVERSION);
    msg.Write(temp);
    temp = (short)(mDPitch/ANGLE_RATE_CONVERSION);
    msg.Write(temp);
    temp = (short)(mDYaw/ANGLE_RATE_CONVERSION);
    msg.Write(temp);
    
    msg.Write(mTimerTicks);
    Message::WriteChecksum(msg);
    if(msg.Length() == 23)
    {
        return 23;
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
unsigned int SendGyroStabilizedIMUVector::Read(const unsigned char* buffer,
                                                 unsigned int length)
{
    short temp;

    if(length >= 23 && 
        buffer[0] == Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector &&
        Message::IsValidMessage(buffer, 23))
    {
        //std::cout << "IMU Checksum Good\n";
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
        mDDX = temp*ACCELERATION_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDDY = temp*ACCELERATION_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDDZ = temp*ACCELERATION_CONVERSION;
        pos += 2;        
        
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDRoll = temp*ANGLE_RATE_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDPitch = temp*ANGLE_RATE_CONVERSION;
        pos += 2;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDYaw = temp*ANGLE_RATE_CONVERSION;
        pos += 2;
        
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mTimerTicks = temp;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        if(bytesRead == 23)
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
void SendGyroStabilizedIMUVector::Print() const
{
    printf("Gyro-Stabilized Euler Angles: ");
    printf("%4.3lf, %4.3lf, %4.3lf", 
            CxUtils::CxToDegrees(mRoll),
            CxUtils::CxToDegrees(mPitch),
            CxUtils::CxToDegrees(mYaw));
    printf(" Acceleration: ");
    printf("%4.3lf, %4.3lf, %4.3lf", 
            mDDX,
            mDDY,
            mDDZ);
    printf(" Gyro: ");
    printf("%4.3lf, %4.3lf, %4.3lf", 
            mDRoll,
            mDPitch,
            mDYaw);
    printf(", %5d\n",mTimerTicks);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SendGyroStabilizedIMUVector& SendGyroStabilizedIMUVector::operator =(const SendGyroStabilizedIMUVector& msg)
{
    CopyMessageData(&msg);
    mRoll = msg.mRoll;
    mPitch = msg.mPitch;
    mYaw = msg.mYaw;
    
    mDDX = msg.mDDX;
    mDDY = msg.mDDY;
    mDDZ = msg.mDDZ;
    
    mDRoll = msg.mDRoll;
    mDPitch = msg.mDPitch;
    mDYaw = msg.mDYaw;
    
    
    mTimerTicks = msg.mTimerTicks;
    return *this;
}

/*  End of File */
