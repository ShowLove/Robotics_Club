//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportid.h
/// \brief This message contains the values of what MCU is running.
///
/// Author(s): Gary Stein<br>
/// Created: 6/23/2010br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
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
#ifndef _ZEBULON_MCU_REPORTID__H
#define _ZEBULON_MCU_REPORTID__H

#include "mcu/message.h"
#include "string.h"

namespace Zebulon
{
    namespace MCU
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ReportID
        ///   \brief Confirms which MCU is reported.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL ReportID : public Message
        {
        public:
            ReportID();
            virtual ~ReportID();
            virtual int WriteMessageBody(CxUtils::Packet& packet) const;
            virtual int ReadMessageBody(const CxUtils::Packet& packet);
            virtual void Print() const;
            virtual Message* Clone() const;
            char mID[5];
        }; 

        typedef ReportID ReportMcuId;
    }
}
#endif //_ZEBULON_MCU_REPORTMCUID__H
/*  End of File */
