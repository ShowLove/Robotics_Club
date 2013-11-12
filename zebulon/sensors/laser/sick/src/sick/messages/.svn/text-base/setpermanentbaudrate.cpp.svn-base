/////////////////////////////////////////////////////////////////////////////////////
///
/// \file setpermanentbaudrate.cpp
/// \brief Message class for a Status Mode for the SICK LIDAR.
///
/// Author(s): Jon Carbone<br>
/// Created: 4/19/2008<br>
/// Last Modified: 4/19/2008<br>
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
#include "sick/messages/setpermanentbaudrate.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SetPermanentBaudrate::SetPermanentBaudrate(unsigned char subCommand) : Message(Message::SetPermanentBaud),
                                                       mSubCommand(subCommand)                                
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SetPermanentBaudrate::SetPermanentBaudrate(const SetPermanentBaudrate& msg) : Message(Message::SetPermanentBaud)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a SetPermanentBaudrate message to a packet.
///         Uses the message's subcommand to determine how to write the
///         message. Currently only supports resetting and setting to the
///         currently set baudrate.
///
///  \param packet Packet to write the SetPermanentBaudrate message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SetPermanentBaudrate::Write(CxUtils::Packet& packet) const
{
    switch(mSubCommand)
    {
    case ResetTo9600:
    case SetToCurrent:
    default:
        WriteHeader(packet, 
                    mMessageType, 
                    2 );
        packet.WriteByte(mSubCommand);
        break;
    }

    WriteCRC(packet);

    return packet.Length();
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a SetPermanentBaudrate message and populates the internal members
///
///  \param packet The packet containing the SetPermanentBaudrate message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SetPermanentBaudrate::Read(const CxUtils::Packet& packet)
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the SetPermanentBaudrate message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int SetPermanentBaudrate::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A SetPermanentBaudrate message to copy values from
///
///  \return A pointer to the instance of the SetPermanentBaudrate message
///
///////////////////////////////////////////////////////////////////////////////
SetPermanentBaudrate& SetPermanentBaudrate::operator=(const SetPermanentBaudrate& msg)
{
    if( this != &msg )
    {
        mSubCommand = msg.mSubCommand;
    }

    return *this;
}

/*  End of File */
