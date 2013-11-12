//////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.h
/// \brief Interface to MCU motor interface
///
/// Author(s): Daniel Barber<br>
/// Created: 1/17/2009br>
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
#ifndef _ZEBULON_MCU_INTERFACE__H
#define _ZEBULON_MCU_INTERFACE__H
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/circulararray.h>
#include <map>
#include <set>
#include "mcu/message.h"
#include "mcu/messagefactory.h"

namespace Zebulon
{
    namespace MCU
    {
        #define NO_CB_THREAD  // Revert to previous version with no callback processing thread - DB

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Interface
        ///   \brief Interface class for the Underwater Vehicle to the MCU motor control
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL Interface
        {
        public:
            class ZEB_MICROCONTROLLER_DLL Callback
            {
            public:
                virtual void ProcessMessage(const Message* message) = 0;
            };

            const static int readSize = 8;
            Interface();
            ~Interface();
            bool Initialize(const std::string& port, const unsigned int baud, const bool tryOtherBauds = true, const unsigned int parity = CxUtils::CX_SERIAL_NO_PARITY);
            void EnableRTSDTR(bool value);
            void Shutdown();
            bool Send(const Message* message, const unsigned int postSendDelayMs = 10);
            bool RegisterCallback(const Message::Code type, Callback* cb);
            void RemoveCallback(const Message::Code type, Callback* cb);
            void ClearCallback();
            static Message* CreateMessage(const Message::Code type);
        protected:
            static void SerialThread(void* arg);        ///<  Thread used to receive data from sensor.
            static void CallbackThread(void* args);     ///<  Thread for running callbacks.
            CxUtils::Thread mSerialThread;              ///<  Manages/creates thread.
            CxUtils::Thread mCallbackThread;            ///<  Thread object for callbacks.
            CxUtils::Mutex  mSerialMutex;               ///<  Mutex for thread protection of data members.
            CxUtils::Serial mSerial;                    ///<  Connection to serial port.
        private:
            CxUtils::CircularArray<CxUtils::Packet> mPacketQueue;           ///<  Message packet queue.
            CxUtils::Mutex mCallbackMutex;                                  ///<  Mutex for thread protection of callbacks.
            std::map<const Message::Code, std::set<Callback*> > mCallbacks; ///<  Map of callbacks.
            CxUtils::Packet mLargeBuffer;           ///< Packet buffer to store received byte data in.
            CxUtils::Packet mSmallBuffer;           ///< Packet buffer to store received byte data in.
        };
    }
}

#endif //_ZEBULON_MCU_INTERFACE__H
/*  End of File */
