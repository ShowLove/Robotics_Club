/////////////////////////////////////////////////////////////////////////////////////
///
/// \file settingsmode.h
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
#ifndef __SICK_SETTINGS_MODE_REQUEST_H
#define __SICK_SETTINGS_MODE_REQUEST_H

#include "sick/message.h"
#include <string>
#include <cstring>
namespace Zebulon
{
    namespace Laser
    {
        namespace SICK
        {
            class ZEB_SICK_DLL SettingsMode : public Message
            {
            public:
                /////////////////////////////////////////////////////////////////////////////
                ///
                /// \var Type
                /// \brief The Type enumeration lists the different kinds of messages
                ///        available for communication with the SICK LIDAR.
                ///
                /////////////////////////////////////////////////////////////////////////////
                enum SubCommand
                {
                    InstallationMode     = 0x00,
                    ContinuousOutput     = 0x24,
                    StopContinuousOutput = 0x25,
                    SetBaud38400         = 0x40,
                    SetBaud19200         = 0x41,
                    SetBaud9600          = 0x42,
                    SetBaud460800        = 0x48,
                };

                SettingsMode(unsigned char subCommand = 0x00);
                SettingsMode(const SettingsMode& msg);
                int SetPassword(const std::string& psswd) { mPassword = psswd; return 1;};
                std::string GetPassword() const { return mPassword; }
                virtual unsigned int Write(CxUtils::Packet& packet) const;
                virtual unsigned int Read(const CxUtils::Packet& packet);
                virtual int RunTestCase() const;
                virtual Message* Clone() const { return new SettingsMode(*this); };
                SettingsMode& operator=(const SettingsMode& msg);
            private:
                std::string mPassword;
                unsigned char mSubCommand;
            };
        }
    }
}

#endif
/*  End of File */
