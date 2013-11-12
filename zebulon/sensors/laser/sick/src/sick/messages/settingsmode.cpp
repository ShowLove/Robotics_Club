/////////////////////////////////////////////////////////////////////////////////////
///
/// \file settingsmode.cpp
/// \brief Message class for a Status Mode for the SICK LIDAR.
///
/// Author(s): Jon Carbone<br>
/// Created: 2/16/2008<br>
/// Last Modified: 2/16/2008<br>
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
#include "sick/messages/settingsmode.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SettingsMode::SettingsMode(unsigned char subCommand) : Message(Message::OperatingModeCommand),
                                                       mPassword("SICK_LMS"),
                                                       mSubCommand(subCommand)                                
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SettingsMode::SettingsMode(const SettingsMode& msg) : Message(Message::OperatingModeCommand)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a SettingsMode message to a packet.
///         Uses the message's subcommand to determine how to write the
///         message. Currently supports setting the baudrate, starting and
///         stopping continuous output, and putting the laser into Installation
///         Mode.
///
///  \param packet Packet to write the SetPermanentBaudrate message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SettingsMode::Write(CxUtils::Packet& packet) const
{
    switch(mSubCommand)
    {
    case ContinuousOutput:
    case StopContinuousOutput:
    case SetBaud38400:
    case SetBaud19200:
    case SetBaud9600:
    case SetBaud460800:
        WriteHeader(packet, 
                    mMessageType, 
                    2 );
        packet.WriteByte(mSubCommand);
        break;

    case InstallationMode:
    default:
        WriteHeader(packet, 
                    mMessageType, 
                    (unsigned short)( mPassword.size() + 2) );
        packet.WriteByte(mSubCommand);
        packet.Write((const unsigned char *)mPassword.c_str(), (unsigned int)mPassword.size());
    }

    WriteCRC(packet);

    return packet.Length();
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a SettingsMode message and populates the internal members.
///         Populates the subcommand member with data found in the
///         packet parameter. Also populates the password member if
///         the message has a subcommand of InstallationMode.
///
///  \param packet The packet containing the SettingsMode message
///
///  \return The length of the packet read, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SettingsMode::Read(const CxUtils::Packet& packet)
{
    unsigned char *data = NULL;
    unsigned short dataSize = GetDataSize(packet);
    if(dataSize > 1)
    {
        data = (unsigned char *)(packet.Ptr()) + 4;
        if( *data == mMessageType )
        {
            if ( InstallationMode == *(data+1) )
            {
                mSubCommand = InstallationMode;
                char* password = new char[dataSize];
                memset(password, 0, dataSize);
                memcpy(password, data + 2, dataSize - 2); 
                mPassword = password;
                delete password;
                password = NULL;
                return packet.Length();
            }
            else if ( ContinuousOutput == *(data+1) )
            {
                mSubCommand = ContinuousOutput;
                return packet.Length();
            }
            else if ( StopContinuousOutput == *(data+1) )
            {
                mSubCommand = StopContinuousOutput;
                return packet.Length();
            }
        }
    }
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the SettingsMode message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int SettingsMode::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A SettingsMode message to copy values from
///
///  \return A pointer to the instance of the SettingsMode message
///
///////////////////////////////////////////////////////////////////////////////
SettingsMode& SettingsMode::operator=(const SettingsMode& msg)
{
    if( this != &msg )
    {
        mPassword = msg.mPassword;
        mAddress = msg.mAddress;
        mSubCommand = msg.mSubCommand;
    }

    return *this;
}

/*  End of File */
