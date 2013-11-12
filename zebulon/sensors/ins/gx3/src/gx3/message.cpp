/////////////////////////////////////////////////////////////////////////////
///
/// \file message.cpp
/// \brief A simple class fordefining message structures/data for
///        MicroStrain gx3.
///
/// Author(s): Gary Stein<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
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
////////////////////////////////////////////////////////////////////////////////////

#include "gx3/message.h"
#include <stdio.h>

using namespace Zebulon;
using namespace INS;
using namespace MicroStrain;

Message::Message(const Message::Type type,const unsigned char length) : mMessageType(type), mFieldLength(length)
{

}

Message::~Message()
{

}
void Message::Print() const
{
    std::cout << "Message Type: " << std::hex << mMessageType << std::dec << " Message Length: " << (int)mFieldLength << std::endl;
}

bool Message::IsValidMessage(const unsigned char* buffer, unsigned int length)
{
    //should probably push the checksum check inside here
    return false;

}

bool Message::WriteChecksum(CxUtils::Packet& message)
{
    //is this even needed, only for sending messages?
    return true;
}

void Message::SetMessageTimeStamp(const CxUtils::Time::Stamp t)
{
    mTimeStamp=t;
}

unsigned short Message::CalcCheckSum(const unsigned char *msg,unsigned int length)
{
    //16bit flecher checksum from doc
    unsigned char check1=0;
    unsigned char check2=0;

    for(int i=0; i<length; i++)
    {
        check1 += msg[i];
        check2 += check1;
    }
    unsigned short checksum = ((unsigned short)check1 << 8) + (unsigned short)check2;
    return checksum;

}


CxUtils::Packet Message::ConstructCommand(std::vector<unsigned short> Messages,double Frequency)
{
    CxUtils::Packet cmd;
    cmd.Clear();
    cmd.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    cmd.WriteByte(GX3Sync1);
    cmd.WriteByte(GX3Sync2);
    cmd.WriteByte(ThreeDMCommandSet);
    unsigned char length=4+3*Messages.size();
    //two on purpose
    cmd.WriteByte(length);
    cmd.WriteByte(length);

    //make sure all commands are for the same device
    unsigned short device=0;
    for(int i=0;i<Messages.size();i++)
    {
        if(device==0)
        {
            device=Messages[i]>>8;
        }
        if(device!=Messages[i]>>8)
        {
            //printf("GX3: devices do not agree\n");
            cmd.Clear();
            return cmd;
        }
        //printf("Device: %02X %04X\n",(int)device,Messages[i]);
    }

    unsigned char format=0;
    unsigned short rate=0;
    switch(device)
    {
        case AHRSDataSet:
            format=AHRSMessageFormat&0x00FF;
            rate=(unsigned short)floor(100.0/Frequency);
            break;
        case GPSDataSet:
            format=GPSMessageFormat&0x00FF;
            rate=(unsigned short)floor(4.0/Frequency);
            break;
        case NAVDataSet:
            format=NAVMessageFormat&0x00FF;
            rate=(unsigned short)floor(100.0/Frequency);
            break;
        default:
            printf("Unknown format: %02X",device);
            cmd.Clear();
            return cmd;
    }

    cmd.WriteByte(format);
    //use new format now
    cmd.WriteByte(0x01);
    unsigned char count=Messages.size();
    cmd.WriteByte(count);
    for(int i=0;i<Messages.size();i++)
    {
        unsigned char tmp=Messages[i]&0x00FF;
        cmd.WriteByte(tmp);
        cmd.Write(rate);
    }

    unsigned short checksum=CalcCheckSum(cmd.Ptr(),cmd.Length());
    cmd.Write(checksum);

    return cmd;
}

CxUtils::Packet Message::ConstructCommand(std::string Data)
{
    CxUtils::Packet cmd;
    cmd.Clear();
    for(int i=0;i<Data.size();i+=2)
    {
        if(Data[i]==' ')
        {
            i++;
        }

        unsigned char tmp;

        if(Data[i]>'9')
        {
            tmp=(Data[i]-'A' +10)*16;
        }
        else
        {
            tmp=(Data[i]-'0')*16;
        }

        if(Data[i+1]>'9')
        {
            tmp+=(Data[i+1]-'A' +10);
        }
        else
        {
            tmp+=(Data[i+1]-'0');
        }

        //printf("%02X",tmp);
        cmd.WriteByte(tmp);
    }
    //printf("\n");
    return cmd;
}


/*  End of file */
