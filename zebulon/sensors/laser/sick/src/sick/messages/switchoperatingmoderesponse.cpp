/////////////////////////////////////////////////////////////////////////////////////
///
/// \file switchoperatingmoderesponse.cpp
/// \brief Message class for receiving the response to a "Switch Operating
///        Mode" message.
///
/// Author(s): Jon Carbone<br>
/// Created: 3/22/2008<br>
/// Last Modified: 4/17/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: joncarbone@gmail.com<br>
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
#include "sick/messages/switchoperatingmoderesponse.h"
#include <stdio.h>

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SwitchOperatingModeResponse::SwitchOperatingModeResponse() : Message(Message::SwitchOperatingModeResponse),
                                                             mLaserStatus(0x00),
                                                             mResponse(Failure)
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SwitchOperatingModeResponse::SwitchOperatingModeResponse(const SwitchOperatingModeResponse& msg) : Message(Message::SwitchOperatingModeResponse)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a SwitchOperatingModeResponse message to a packet.
///
///  \param packet Packet to write the SwitchOperatingModeResponse message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SwitchOperatingModeResponse::Write(CxUtils::Packet& packet) const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a SwitchOperatingModeResponse message and populates the
///         internal members. TBD: populate this with useful information
///         about the responses and the stat(i?)(usi?)(uses?)...
///
///  \param packet The packet containing the ContinuousOutputResponse message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SwitchOperatingModeResponse::Read(const CxUtils::Packet& packet)
{
    unsigned char response = 0;
    unsigned short dataSize = GetDataSize(packet);

    if( dataSize > 0 )
    {
        packet.Read(response, packet.GetReadPos() + 4);

        if( Message::SwitchOperatingModeResponse == response )
        {
            packet.Read(mResponse, packet.GetReadPos() + 5);
            packet.Read(mLaserStatus, packet.GetReadPos() + 6);

            return packet.Length();
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the SwitchOperatingModeResponse message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int SwitchOperatingModeResponse::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A SwitchOperatingModeResponse message to copy values from
///
///  \return A pointer to the instance of the SwitchOperatingModeResponse message
///
///////////////////////////////////////////////////////////////////////////////
SwitchOperatingModeResponse& SwitchOperatingModeResponse::operator=(const SwitchOperatingModeResponse& msg)
{
    if( this != &msg )
    {
        mAddress = msg.mAddress;
        mResponse = msg.mResponse;
    }

    return *this;
}


void SwitchOperatingModeResponse::Print() const
{
    printf("Switch Operation Mode Response: %d, %d\n", mResponse, mLaserStatus);
    fflush(stdout);
}

/*  End of File */
