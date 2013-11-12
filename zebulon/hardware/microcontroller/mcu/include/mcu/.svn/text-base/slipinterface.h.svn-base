//////////////////////////////////////////////////////////////////////////////////////
///
/// \file slipinterface.h
/// \brief SlipInterface for Slip (Serial Line IP, RFC 1055) Transmission Protocol
///        compliant device, i.e. microcontroller.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 12/27/2010<br>
/// Copyright (c) 2010<br>
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
#ifndef _ZEBULON_MCU_SLIP_INTERFACE__H
#define _ZEBULON_MCU_SLIP_INTERFACE__H
#include <cxutils/thread.h>
#include <cxutils/circulararray.h>
#include <map>
#include <set>
#include <cxutils/serial.h>
#include "mcu/message.h"

#ifndef ZEB_MICROCONTROLLER_DLL
#ifdef WIN32
#define ZEB_MICROCONTROLLER_DLL __declspec(dllimport)
#else
#define ZEB_MICROCONTROLLER_DLL
#endif

#endif


namespace Zebulon
{
    namespace MCU
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class SlipInterface
        ///   \brief SlipInterface class for Slip compliant device.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MICROCONTROLLER_DLL SlipInterface
        {
        public:
            class ZEB_MICROCONTROLLER_DLL Callback
            {
            public:
                virtual void ProcessFrame(const unsigned char* frame, const unsigned int& length) {}
                virtual void ProcessMessage(const Message* message) {}
            };
                        
            const static unsigned char END = 192;       ///< Indicates end of packet
            const static unsigned char ESC = 219;       ///< Indicates byte stuffing
            const static unsigned char ESC_END = 220;   ///< ESC ESC_END means END data byte
            const static unsigned char ESC_ESC = 221;   ///< ESC ESC_ESC means ESC data byte
            
            SlipInterface();
            ~SlipInterface();
            bool Initialize(const std::string& port, const unsigned int baud, const unsigned int parity = CxUtils::CX_SERIAL_NO_PARITY);
            void Shutdown();
            bool Send(const unsigned char* frame, const unsigned int& length);
            bool Send(const Message* message, const unsigned int sleepMs = 0);
            unsigned char GetChar();
            bool RegisterCallback(Callback* cb);
            void RemoveCallback(Callback* cb);
            void ClearCallback();
        protected:
            static void SerialThread(void* arg);        ///<  Thread used to receive Slip frame from device.
            static void CallbackThread(void* args);     ///<  Thread for running callbacks.
            CxUtils::Thread mSerialThread;              ///<  Manages/creates thread.
            CxUtils::Thread mCallbackThread;            ///<  Thread object for callbacks.
            CxUtils::Mutex  mSerialMutex;               ///<  Mutex for thread protection of data members.
            CxUtils::Serial mSerial;                    ///<  Connection to serial port.
        private:
            CxUtils::CircularArray<CxUtils::Packet> mFrameQueue;            ///<  Frame packet queue.
            CxUtils::Mutex mCallbackMutex;                                  ///<  Mutex for thread protection of callbacks.
            std::set<Callback*> mCallbacks;                                 ///<  Set of callbacks.
        };
    }
}

#endif //_ZEBULON_SLIP_INTERFACE__H
/*  End of File */
