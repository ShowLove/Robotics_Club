/////////////////////////////////////////////////////////////////////////////
///
/// \file nedvelocity.cpp
/// \brief
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

#include "gx3/gpsdataset/nedvelocity.h"

using namespace Zebulon;
using namespace INS;
using namespace MicroStrain;

NEDVelocity::NEDVelocity(const Message::Type type,const unsigned char length) : Message(type,length)
{
    mNorth=0;
    mEast=0;
    mDown=0;
    mSpeed=0;
    mGroundSpeed=0;
    mHeading=0;
    mSpeedAccuracy=0;
    mHeadingAccuracy=0;
    mValidFlags=0;
}

NEDVelocity::~NEDVelocity()
{

}
void NEDVelocity::Print() const
{
    Message::Print();
    std::cout << "Speeds: " << mNorth << " " << mEast << " " << mDown << std::endl;
    std::cout << "Total: " << mSpeed << " Ground: " << mGroundSpeed << " Heading: " << mHeading << std::endl;
    std::cout << "Accuracy SH: " << mSpeedAccuracy<< " " << mHeading << std::endl;
    std::cout << "Valid Flags: " << std::hex << mValidFlags << std::dec << std::endl;
}

unsigned int NEDVelocity::Write(CxUtils::Packet& msg) const
{
    return 0;
}

unsigned int NEDVelocity::Read(const unsigned char* buffer,
                          unsigned int length)
{
    int read=2;
    read+=CxUtils::Packet::Read(buffer,length,2+0,mNorth,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+4,mEast,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+8,mDown,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+12,mSpeed,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+16,mGroundSpeed,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+20,mHeading,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+24,mSpeedAccuracy,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+28,mHeadingAccuracy,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+32,mValidFlags,CxUtils::CX_PACKET_BIG_ENDIAN);
    return read;

}

Message* NEDVelocity::Clone() const
{
    //own values
    NEDVelocity *ret=new NEDVelocity(mMessageType,mFieldLength);
    //copy the rest
    ret->mNorth=mNorth;
    ret->mEast=mEast;
    ret->mDown=mDown;
    ret->mSpeed=mSpeed;
    ret->mGroundSpeed=mGroundSpeed;
    ret->mHeading=mHeading;
    ret->mSpeedAccuracy=mSpeedAccuracy;
    ret->mHeadingAccuracy=mHeadingAccuracy;
    ret->mValidFlags=mValidFlags;

    return ret;
}


/*  End of file */

