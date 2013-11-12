/////////////////////////////////////////////////////////////////////////////////////
///
/// \file confirmationconfiguration.h
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
#ifndef __SICK_MESSAGES_CONFIRMATION_CONFIGURATION_H
#define __SICK_MESSAGES_CONFIRMATION_CONFIGURATION_H

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
            ///  \class ConfirmationConfiguration
            ///  \brief Message response from the SICK LMS2xx when sent a Configure
            ///         message(0x77).
            ///
            /////////////////////////////////////////////////////////////////////////////
            class ZEB_SICK_DLL ConfirmationConfiguration : public Message
            {
            public:
                enum DistanceModes
                {
                    CM = 0x00,            ///< Centimeter mode
                    MM = 0x01,            ///< Millimeter mode
                };
                ConfirmationConfiguration();
                ConfirmationConfiguration(const ConfirmationConfiguration& msg);
                virtual unsigned int Write(CxUtils::Packet& packet) const;
                virtual unsigned int Read(const CxUtils::Packet& packet);
                virtual int RunTestCase() const;
                virtual Message* Clone() const { return new ConfirmationConfiguration(*this); };
                ConfirmationConfiguration& operator=(const ConfirmationConfiguration& msg);
                unsigned char GetDistanceMode() const { return mDistanceMode > 0? true:false;}
            private:
                unsigned char mDistanceMode;    ///<  Distance mode laser is set to.
            };
        }
    }
}

#endif //__SICK_MESSAGES_CONFIRMATION_CONFIGURATION_01_H
/*  End of File */
