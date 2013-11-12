/////////////////////////////////////////////////////////////////////////////////////
///
/// \file messagefactory.cpp
/// \brief Message factor class.
///
/// Author(s): Daniel Barber<br>
/// Created: 3/22/2008<br>
/// Last Modified: 4/17/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#include "sick/messagefactory.h"
#include "sick/messages/settingsmode.h"
#include "sick/messages/continuousoutputresponse.h"
#include "sick/messages/switchoperatingmoderesponse.h"
#include "sick/messages/confirmvariantswitching.h"
#include "sick/messages/confirmationconfiguration.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;


///////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a message given a packet containing a message
///
///  \param msg The packet containing the message
///
///  \return A pointer to the newly created message, or NULL if none was found
///
///////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::CreateMessage(const CxUtils::Packet& msg)
{
    Message* ptr = NULL;

    ptr = GetResponseMessage(msg);
    if( ptr == NULL )
    {
        ptr = GetCommandMessage(msg);
    }

    return ptr;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a message if the packet contains a command message
///
///  \param msg The packet containing the command message
///
///  \return A pointer to the newly created message, or NULL if none was found
///
///////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::GetCommandMessage(const CxUtils::Packet& msg)
{
    Message* ptr = NULL;
    if( msg.Length() >= 7 )
    {
        unsigned char command;
        msg.Read(command, msg.GetReadPos() + 4);
        //unsigned char command = msg.Ptr()[4];
        switch(command)
        {
        case Message::OperatingModeCommand:
            {
                unsigned char subCommand;
                msg.Read(subCommand, msg.GetReadPos() + 5);
                //unsigned char subCommand = msg.Ptr()[5];
                switch(subCommand)
                {
                case SettingsMode::InstallationMode:
                case SettingsMode::ContinuousOutput:
                case SettingsMode::StopContinuousOutput:
                    ptr = new SettingsMode(subCommand);
                    break;
                default:
                    ptr = NULL;
                    break;
                }
            }
            break;
        default:
            ptr = NULL;
            break;
        }
    }
    
    if(ptr != NULL)
    {
        if( ptr->Read(msg) == 0 )
        {
            delete ptr;
            ptr = NULL;
        }
    }

    return ptr;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a message if the packet contains a response message
///
///  \param msg The packet containing the response message
///
///  \return A pointer to the newly created message, or NULL if none was found
///
///////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::GetResponseMessage(const CxUtils::Packet& msg)
{
    Message* ptr = NULL;
    if( msg.Length() >= 7 )
    {
        unsigned char command;
        msg.Read(command, 4);
        //unsigned char command = msg.Ptr()[4];
        switch(command)
        {
        case Message::ContinuousOutput:
            ptr = new ContinuousOutputResponse();
            break;
        case Message::SwitchOperatingModeResponse:
            ptr = new SwitchOperatingModeResponse();
            break;
        case Message::ConfirmVariantSwitching:
            ptr = new ConfirmVariantSwitching();
            break;
        case Message::ConfirmationConfiguration:
            ptr = new ConfirmationConfiguration();
            break;
        default:
            ptr = NULL;
            break;
        }
    }
    
    if(ptr != NULL)
    {
        if( ptr->Read(msg) == 0 )
        {
            delete ptr;
            ptr = NULL;
        }
    }

    return ptr;
}


/*  End of File */
