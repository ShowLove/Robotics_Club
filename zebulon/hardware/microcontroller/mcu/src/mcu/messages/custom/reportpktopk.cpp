//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportpktopk.cpp
/// \brief This message contains the value of the current Pk-to-Pk values for a 
///        particular channel.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 03/24/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu <br>
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
#include "mcu/messages/custom/reportpktopk.h"

using namespace Zebulon;
using namespace MCU;
using namespace CustomMessage;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportPkToPk::ReportPkToPk() : Message(REPORT_PK_TO_PK)
{
    mChannel = 0;
    mMax = 0;
    mMin = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportPkToPk::~ReportPkToPk()
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
int ReportPkToPk::WriteMessageBody(CxUtils::Packet& packet) const
{
    packet.WriteByte(mChannel);
    packet.WriteByte((mMax & 0x0FF0) >> 4);
    packet.WriteByte(((mMax & 0x000F) << 4) | ((mMin & 0x0F00) >> 8));
    packet.WriteByte(mMin & 0x00FF);
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
int ReportPkToPk::ReadMessageBody(const CxUtils::Packet& packet)
{
    unsigned char data[4];
    if(packet.Read(data[0]) &&
       packet.Read(data[1]) &&
       packet.Read(data[2]) &&
       packet.Read(data[3]))
    {
        mChannel = data[0];
        mMax = (data[1] << 4) | ((data[2] & 0xF0) >> 4);
        mMin = ((data[2] & 0x0F) << 8) | (data[3]);
        return 4;
    }
    return 0;
}

void ReportPkToPk::Print() const
{
    std::cout << "ReportPkToPk Message\nChannel: " << (int)mChannel << " Max: " << (int)mMax << " Min: " << (int)mMin << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ReportPkToPk::Clone() const
{
    ReportPkToPk* clone = new ReportPkToPk();
    clone->mChannel = this->mChannel;
    clone->mMax = this->mMax;
    clone->mMin = this->mMin;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
