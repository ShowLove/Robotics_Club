/////////////////////////////////////////////////////////////////////////////////////
///
/// \file accelerationangularrateandorientationmatrix2.cpp
/// \brief A Message structure for IMU Data
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
#include "microstrain/gx2/accelerationangularrateandorientationmatrix2.h"
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
AccelerationAngularRateAndOrientationMatrix2::AccelerationAngularRateAndOrientationMatrix2() : Message(Message::AccelerationAngularRateAndOrientationMatrix2)
{
    mRoll = mPitch = mYaw = 0;
    mDRoll = mDPitch = mDYaw = 0;
    mDDX = mDDY = mDDZ = 0;
    mTimerTicks = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccelerationAngularRateAndOrientationMatrix2::AccelerationAngularRateAndOrientationMatrix2(const AccelerationAngularRateAndOrientationMatrix2& msg) : Message(Message::AccelerationAngularRateAndOrientationMatrix2)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccelerationAngularRateAndOrientationMatrix2::~AccelerationAngularRateAndOrientationMatrix2()
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
unsigned int AccelerationAngularRateAndOrientationMatrix2::Write(CxUtils::Packet& msg) const
{
    msg.Clear();
    msg.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    msg.WriteByte((unsigned char)mMessageType);
    float temp = 0;
    temp = (float)(mDDX);
    msg.Write(temp);
    temp = (float)(mDDY);
    msg.Write(temp);
    temp = (float)(mDDZ);
    msg.Write(temp);
    temp = (float)(mDRoll);
    msg.Write(temp);
    temp = (float)(mDPitch);
    msg.Write(temp);
    temp = (float)(mDYaw);
    msg.Write(temp);

    //Matrix (Invent)?
    temp = (float)(mOrientationMatrix[0][0]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[0][1]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[0][2]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[1][0]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[1][1]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[1][2]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[2][0]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[2][1]);
    msg.Write(temp);
    temp = (float)(mOrientationMatrix[2][2]);
    msg.Write(temp);
    
    msg.Write(mTimerTicks);
    Message::WriteChecksum(msg);
    if(msg.Length() == 67)
    {
        return 67;
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
unsigned int AccelerationAngularRateAndOrientationMatrix2::Read(const unsigned char* buffer,
                                                 unsigned int length)
{
    float temp;
    unsigned int tempTime;
    unsigned short tempCheck;

    if(length >= 67 && 
        buffer[0] == Message::AccelerationAngularRateAndOrientationMatrix2 &&
        Message::IsValidMessage2(buffer, 67))
    {
        unsigned int pos = 1;
        int bytesRead = 1;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDDX = temp*9.81;
        pos += 4;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDDY = temp*9.81;
        pos += 4;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        //mDDZ = temp*9.81;
        //Remove Gravity
        mDDZ = temp*9.81+9.81;
        pos += 4;

        //gyros
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDRoll = temp;
        pos += 4;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDPitch = temp;
        pos += 4;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
        mDYaw = temp;
        pos += 4;
        
        //Orientation Matrix
        for(int j=0;j<3;j++)
        {
            for(int i=0;i<3;i++)
            {
                bytesRead += CxUtils::Packet::Read(buffer, length, pos, temp, CxUtils::CX_PACKET_BIG_ENDIAN);
                mOrientationMatrix[j][i] = temp;
                pos += 4;
            }
        }
        
        double val=mOrientationMatrix[0][2];
        if(val>1.0)
        {
            val=1.0;
        }
        if(val<-1.0)
        {
            val=-1.0;
        }
        mPitch=asin(-val);
        mRoll=atan2(mOrientationMatrix[1][2],mOrientationMatrix[2][2]);
        mYaw=atan2(mOrientationMatrix[0][1],mOrientationMatrix[0][0]);
        
        
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, tempTime, CxUtils::CX_PACKET_BIG_ENDIAN);
        mTimerTicks = tempTime;
        pos += 4;
        bytesRead += CxUtils::Packet::Read(buffer, length, pos, tempCheck, CxUtils::CX_PACKET_BIG_ENDIAN);
        
        if(bytesRead == 67)
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
void AccelerationAngularRateAndOrientationMatrix2::Print() const
{
    printf("Gyro-Stabilized Euler Angles: ");
    printf("%4.3lf, %4.3lf, %4.3lf, %u\n", 
            CxUtils::CxToDegrees(mRoll),
            CxUtils::CxToDegrees(mPitch),
            CxUtils::CxToDegrees(mYaw),
            mTimerTicks);
    printf("Gyro-Angles Rates: ");
    printf("%4.3lf, %4.3lf, %4.3lf, %u\n", 
            CxUtils::CxToDegrees(mDRoll),
            CxUtils::CxToDegrees(mDPitch),
            CxUtils::CxToDegrees(mDYaw),
            mTimerTicks);
    printf("Accelerations: ");
    printf("%4.3lf, %4.3lf, %4.3lf, %u\n", 
            mDDX,
            mDDY,
            mDDZ,
            mTimerTicks);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
AccelerationAngularRateAndOrientationMatrix2& AccelerationAngularRateAndOrientationMatrix2::operator =(const AccelerationAngularRateAndOrientationMatrix2& msg)
{
    CopyMessageData(&msg);
    mRoll = msg.mRoll;
    mPitch = msg.mPitch;
    mYaw = msg.mYaw;

    mDRoll = msg.mDRoll;
    mDPitch = msg.mDPitch;
    mDYaw = msg.mDYaw;

    mDDX = msg.mDDX;
    mDDY = msg.mDDY;
    mDDZ = msg.mDDZ;
    
    mTimerTicks = msg.mTimerTicks;
    return *this;
}

/*  End of File */
