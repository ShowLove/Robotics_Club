//////////////////////////////////////////////////////////////////////////////////////
///
/// \file messagefactory.h
/// \brief Message factory methods for dynamic creation of any type of MCU message.
///
/// Author(s): Daniel Barber<br>
/// Created: 4/19/2009<br>
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
#ifndef _ZEBULON_MCU_MESSAGE_FACTORY_H
#define _ZEBULON_MCU_MESSAGE_FACTORY_H

#include "mcu/message.h"
#include "mcu/messages/ping.h"
#include "mcu/messages/pong.h"
#include "mcu/messages/queryreport.h"
#include "mcu/messages/setdigitaloutputs.h"
#include "mcu/messages/setanalogoutputs.h"
#include "mcu/messages/reportdigitalinputs.h"
#include "mcu/messages/reportanaloginputs.h"
#include "mcu/messages/reportencoders.h"
#include "mcu/messages/reportid.h"
#include "mcu/messages/setservooutputs.h"
#include "mcu/messages/custom/setpingerfrequency.h"
#include "mcu/messages/custom/reportmotorstatus.h"
#include "mcu/messages/custom/setmotoraddress.h"
#include "mcu/messages/custom/setapl.h"
#include "mcu/messages/custom/reportdcoffset.h"
#include "mcu/messages/custom/reportpktopk.h"
#include "mcu/messages/custom/reportpingtime.h"
#include "mcu/messages/custom/reportdpotvalues.h"
#include "mcu/messages/custom/reportsamplefreq.h"
#include "mcu/messages/custom/reportsampleperiod.h"
#include "mcu/messages/custom/reportfft.h"
#include "mcu/messages/setmotoroutput.h"

namespace Zebulon
{
    namespace MCU
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class MessageFactory
        ///   \brief Interface for dynamic creation of messages.  This interface
        ///   is used by the Interface class when receiving new messages to 
        ///   de-serialize.  Use this interface to add Custom MCU type messages for
        ///   reading by the generic MCU Interface class.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL MessageFactory
        {
            public:
                static void Initialize();
                static Message* CreateMessage(const Message::Code code);
                static int AddToFactory(Message* message);
                static void Clear();
            private:
                static bool mIsInitialized;
        };
    }
}

#endif 
/*  End of File */
