//////////////////////////////////////////////////////////////////////////////////////
///
/// \file configuration.h
/// \brief This file contains the message CORAL_CONFIGURATION.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 1/24/2009<br>
/// Copyright (c) 2009<br>
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
#ifndef _ZEBULON_CORAL_AHRS_CONFIGURATION__H
#define _ZEBULON_CORAL_AHRS_CONFIGURATION__H

#include "coralahrs/message.h"

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Configuration
            ///   \brief This packet is sent in reponse to a CORAL REQUEST CONFIGURATION packet. It
            ///          returns the current serial speed setting, output mode, and output rate divisor.
            ///          The output mode setting is 1 for CORAL QUAT output mode, 2 for CORAL SENSORS
            ///          output mode, 3 for (CORAL SENSORS | CORAL QUAT) output mode, 4 for
            ///          CORAL RAW SENSORS, and 0 for no output. The serial speed setting corresponds
            ///          to an actual baud rate according to the following chart:
            ///          - 0 = 4800
            ///          - 1 = 9600
            ///          - 2 = 19200
            ///          - 3 = 38400
            ///          - 4 = 57600
            ///          - 5 = 115200
            ///          - 6 = 230400
            ///          - 7 = 460800
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL Configuration : public Message
            {
                public:
                Configuration();
                Configuration(const Configuration& message);
                ~Configuration();
                virtual int WriteMessageBody(CxUtils::Packet& packet) const;
                virtual int ReadMessageBody(const CxUtils::Packet& packet);
                virtual Message* Clone() const { return (Message*)new Configuration(*this); }
                virtual void Print() const;
                Configuration& operator=(const Configuration& message);
                unsigned char mSerialSpeed;
                unsigned char mOutputMode;
                unsigned char mOutputRateDivisor;
            };
        }
    }
}


#endif
/*  End of File */
