/////////////////////////////////////////////////////////////////////////////////////
///
/// \file confirmationconfiguration.cpp
/// \brief Response after a request 0x77 message, eg Configure command part 1.
///
/// Author(s): David Adams<br>
/// Created: 2009<br>
/// Last Modified: 2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
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
#include "sick/messages/confirmationconfiguration.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
ConfirmationConfiguration::ConfirmationConfiguration() : Message(Message::ConfirmationConfiguration)
{
    mDistanceMode = 0;     
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
ConfirmationConfiguration::ConfirmationConfiguration(const ConfirmationConfiguration& msg) : Message(Message::ConfirmationConfiguration)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a ConfirmationConfiguration message to a packet.
///
///  \param packet Packet to write the ConfirmationConfiguration message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int ConfirmationConfiguration::Write(CxUtils::Packet& packet) const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a ConfirmationConfiguration message and populates the internal members
///
///  \param packet The packet containing the ConfirmationConfiguration message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int ConfirmationConfiguration::Read(const CxUtils::Packet& packet)
{

    unsigned char response = 0;
    unsigned short dataSize = GetDataSize(packet);

    if( dataSize > 0 )
    {
        packet.Read(response, packet.GetReadPos() + 4);

        if( Message::ConfirmationConfiguration == response )
        {
            packet.Read(mDistanceMode, packet.GetReadPos() + 12);    
            return packet.Length();
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the ConfirmationConfiguration message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int ConfirmationConfiguration::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A ConfirmationConfiguration message to copy values from
///
///  \return A pointer to the instance of the ConfirmationConfiguration message
///
///////////////////////////////////////////////////////////////////////////////
ConfirmationConfiguration& ConfirmationConfiguration::operator=(const ConfirmationConfiguration& msg)
{
    if( this != &msg )
    {    
        mDistanceMode = msg.mDistanceMode;           
    }
    return *this;
}

/*  End of File */
