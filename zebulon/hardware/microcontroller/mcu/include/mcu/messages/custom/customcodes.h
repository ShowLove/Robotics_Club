//////////////////////////////////////////////////////////////////////////////////////
///
/// \file customcodes.h
/// \brief List of constants for custom message codes.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/14/2009<br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
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
#ifndef _ZEBULON_MCU_MESSAGE_CUSTOM_CODES__H
#define _ZEBULON_MCU_MESSAGE_CUSTOM_CODES__H

#include "mcu/message.h"

namespace Zebulon
{
    namespace MCU
    {
        namespace CustomMessage
        {
            static const Message::Code REPORT_MOTOR_STATUS     = 0x81;
            static const Message::Code SET_MOTOR_ADDRESS       = 0x0A;
            
            static const Message::Code REPORT_DC_OFFSET        = 0x85;
            static const Message::Code REPORT_PK_TO_PK         = 0x86;
            static const Message::Code REPORT_PING_TIME        = 0x87;
            static const Message::Code REPORT_SERVO_INPUT      = 0x88;
            static const Message::Code REPORT_D_POT_VALUES     = 0x89;
            static const Message::Code REPORT_SAMPLE_FREQ      = 0x8A;
            static const Message::Code REPORT_SAMPLE_PERIOD    = 0x8B;
            static const Message::Code REPORT_FFT              = 0x8C;

            static const Message::Code SET_PINGER_FREQUENCY    = 0x0F;
            static const Message::Code SET_APL                 = 0x10;
        }
    }
}
#endif //_ZEBULON_MCU_MESSAGE__H
/*  End of File */
