//////////////////////////////////////////////////////////////////////////////////////
///
/// \file setoutputmode.h
/// \brief This file contains the message CORAL_SET_OUTPUT_MODE.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 1/28/2009<br>
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
#ifndef _ZEBULON_CORAL_AHRS_SET_OUTPUT_MODE__H
#define _ZEBULON_CORAL_AHRS_SET_OUTPUT_MODE__H

#include "coralahrs/message.h"

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class SetOutputMode
            ///   \brief This message changes the output mode on the Coral AHRSTM module. The
            ///          output mode is a 4-bit field in which bits 0 and 1 choose an
            ///          orientation data type (00 = None, 01 = Quaternion, 10 = Euler Angles,
            ///          11 = Orientation Matrix), and bits 2 and 3 choose a sensor data type
            ///          (00 = None, 01 = Calibrated, 10 = Raw). If an invalid mode is
            ///          selected, no data will be output until a proper mode is selected.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL SetOutputMode : public Message
            {
            public:
                enum OrientationType
                {
                    None = 0,
                    Quaternion = 0x01,
                    EulerAngles = 0x02,
                    OrientationMatrix = 0x03
                };
                enum SensorType
                {
                    Calibrated = 0x04,
                    Raw = 0x08
                };
                SetOutputMode();
                SetOutputMode(const SetOutputMode& message);
                ~SetOutputMode();
                virtual int WriteMessageBody(CxUtils::Packet& packet) const;
                virtual int ReadMessageBody(const CxUtils::Packet& packet);
                virtual Message* Clone() const { return (Message*)new SetOutputMode(*this); }
                virtual void Print() const;
                SetOutputMode& operator=(const SetOutputMode& message);
                unsigned char mMode;  ///<  Mode to output in (e.g. Quaternion | Calibrated). 
            };
        }
    }
}


#endif
/*  End of File */
