/////////////////////////////////////////////////////////////////////////////
///
/// \file setcontinuousmode.h
/// \brief A Message structure for the response to the Set Continuous Mode
///        command for the MicroStrain sensor.
///
/// Author(s): Daniel Barber<br>
/// Created: 5/28/2008<br>
/// Last Modified: 5/28/2008<br>
/// Copyright (c) 2007<br>
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
#ifndef __ZEBULON_MICROSTRAIN_SET_CONTINUOUS_MODE__H
#define __ZEBULON_MICROSTRAIN_SET_CONTINUOUS_MODE__H

#include "microstrain/message.h"


namespace Zebulon
{
    namespace Compass
    {
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class SetContinuousMode 
            ///  \brief Structure for reading message response to Set Continuous Mode
            ///         command.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL SetContinuousMode : public Message
            {
            public:
                SetContinuousMode();
                SetContinuousMode(const SetContinuousMode& msg);
                ~SetContinuousMode();
                virtual unsigned int Write(CxUtils::Packet& msg) const;
                virtual unsigned int Read(const unsigned char* buffer,
                                          unsigned int length);
                virtual Message* Clone() const { return new SetContinuousMode(*this); }
                SetContinuousMode& operator=(const SetContinuousMode& msg);
                unsigned char mCommandByte;     ///<  Command byte.           
                unsigned short mTimerTicks;     ///<  Timer Ticks value.
            };
        }
    }
}


#endif
/*  End of File */
