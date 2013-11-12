/////////////////////////////////////////////////////////////////////////////
///
/// \file eulerangles.cpp
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

#include "gx3/ahrsdataset/eulerangles.h"

using namespace Zebulon;
using namespace INS;
using namespace MicroStrain;

EulerAngles::EulerAngles(const Message::Type type,const unsigned char length) : Message(type,length)
{
    mRoll=mPitch=mYaw=0.0;
}

EulerAngles::~EulerAngles()
{

}
void EulerAngles::Print() const
{
    Message::Print();
    std::cout << "Euler Angles: " << mRoll << " " << mPitch << " " << mYaw <<std::endl;
}

unsigned int EulerAngles::Write(CxUtils::Packet& msg) const
{
    return 0;
}

unsigned int EulerAngles::Read(const unsigned char* buffer,
                          unsigned int length)
{
    int read=2;
    read+=CxUtils::Packet::Read(buffer,length,2+0,mRoll,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+4,mPitch,CxUtils::CX_PACKET_BIG_ENDIAN);
    read+=CxUtils::Packet::Read(buffer,length,2+8,mYaw,CxUtils::CX_PACKET_BIG_ENDIAN);
    return read;

}

Message* EulerAngles::Clone() const
{
    //own values
    EulerAngles *ret=new EulerAngles(mMessageType,mFieldLength);
    //copy the rest
    ret->mRoll=mRoll;
    ret->mPitch=mPitch;
    ret->mYaw=mYaw;

    return ret;
}


/*  End of file */

