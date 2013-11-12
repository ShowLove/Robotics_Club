//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportid.cpp
/// \brief This message contains the values of what MCU is running.
///
/// Author(s): Gary Stein<br>
/// Created: 6/23/2010br>
/// Copyright (c) 2010<br>
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
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include "mcu/messages/reportid.h"

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportID::ReportID() : Message(Message::ReportID)
{
    mID[0]='N';
    mID[1]='O';
    mID[2]='N';
    mID[3]='E';
    mID[4]='\0';
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportID::~ReportID()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the contents of the message payload to the packet.
///
///   \param[in] packet Packet to write to.
///
///   \return Bytes processed, - 1 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportID::WriteMessageBody(CxUtils::Packet& packet) const
{
    packet.WriteByte(mID[0]);
    packet.WriteByte(mID[1]);
    packet.WriteByte(mID[2]);
    packet.WriteByte(mID[3]);
    return 4;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the contents of the message payload from the packet, and saves
///   to internal data members.
///
///   \param[in] packet Packet to read from.
///
///   \return Bytes processed, - 1 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int ReportID::ReadMessageBody(const CxUtils::Packet& packet)
{
    if(packet.Read(mID[0]) &&
       packet.Read(mID[1])&&
       packet.Read(mID[2]) &&
       packet.Read(mID[3]))
    {
        return 4;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints to console.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportID::Print() const
{
    std::cout << "ReportID Message\tMCU: " << mID << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ReportID::Clone() const
{
    ReportID* clone = new ReportID();
    memcpy(clone->mID,mID,5);
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
