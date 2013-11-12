/////////////////////////////////////////////////////////////////////////////////////
///
/// \file switchoperatingmoderesponse.h
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
#ifndef __SICK_MESSAGES_SWITCH_OPERATING_MODE_RESPONSE_H
#define __SICK_MESSAGES_SWITCH_OPERATING_MODE_RESPONSE_H

#include "sick/message.h"
#include <string>
#include <vector>

namespace Zebulon
{
    namespace Laser
    {
        namespace SICK
        {
            /////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class SwitchOperatingModeResponse
            ///  \brief Message response from the SICK LMS2xx when sent a Switch
            ///         Operating Mode Command (0x20).
            ///
            /////////////////////////////////////////////////////////////////////////////
            class ZEB_SICK_DLL SwitchOperatingModeResponse : public Message
            {
            public:
                /////////////////////////////////////////////////////////////////////////////
                ///
                ///  \enum Response
                ///  \brief Possible response values.
                ///
                /////////////////////////////////////////////////////////////////////////////
                enum Response
                {
                    Success = 0,
                    Failure     ///  Invalid password or fault in LMS2xx.
                };
                SwitchOperatingModeResponse();
                SwitchOperatingModeResponse(const SwitchOperatingModeResponse& msg);
                virtual unsigned int Write(CxUtils::Packet& packet) const;
                virtual unsigned int Read(const CxUtils::Packet& packet);
                virtual int RunTestCase() const;
                virtual Message* Clone() const { return new SwitchOperatingModeResponse(*this); };
                SwitchOperatingModeResponse& operator=(const SwitchOperatingModeResponse& msg);
                Response GetResponse() const {return static_cast<Response>(mResponse);};
                unsigned char GetStatus() const {return mLaserStatus;};
                virtual void Print() const;
            private:
                unsigned char mResponse;
                unsigned char mLaserStatus;
            };
        }
    }
}

#endif
/*  End of File */
