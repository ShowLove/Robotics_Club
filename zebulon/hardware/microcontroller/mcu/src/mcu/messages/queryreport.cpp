//////////////////////////////////////////////////////////////////////////////////////
///
/// \file queryreport.h
/// \brief Query report message.  Sent to an MCU to get specific report data back.
///
/// Author(s): David Adams<br>
/// Created: 2/14/2009br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
#include "mcu/messages/queryreport.h"
#include <string.h>

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
QueryReport::QueryReport() : Message(Message::QueryReport)
{
    memset(mPayload, 0, sizeof(unsigned char)*4);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
QueryReport::~QueryReport()
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
int QueryReport::WriteMessageBody(CxUtils::Packet& packet) const
{
    if(false == this->mUseStaticFormatFlag)
    {
        return 0;
    }

    return packet.Write((unsigned char*)mPayload, 4);
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
int QueryReport::ReadMessageBody(const CxUtils::Packet& packet)
{
    if(false == this->mUseStaticFormatFlag)
    {
        return 0;
    }

    int blank = 0;
    if(packet.Read((unsigned char*)mPayload, 4))
    {
        return 4;
    }
    return -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints message contents to console.
///
////////////////////////////////////////////////////////////////////////////////////
void QueryReport::Print() const
{
    std::cout << "Query Report Message"<< std::endl;
    std::cout << "Byte 0: "<< mPayload[0] << std::endl;
    std::cout << "Byte 1: "<< mPayload[1] << std::endl;
    std::cout << "Byte 2: "<< mPayload[2] << std::endl;
    std::cout << "Byte 3: "<< mPayload[3] << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* QueryReport::Clone() const
{
    QueryReport* clone = new QueryReport();
    memcpy(clone->mPayload, mPayload, sizeof(unsigned char)*4);
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
