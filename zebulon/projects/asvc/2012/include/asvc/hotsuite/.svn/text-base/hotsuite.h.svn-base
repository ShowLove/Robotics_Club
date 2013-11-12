//////////////////////////////////////////////////////////////////////////////////////
///
/// \file hotsuite.h
/// \brief Interface to send hotsuite message
///
/// Author(s): Andrew Watson<br>
/// Created: 5/10/2012br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: andrewwatson@knights.ucf.edu <br>
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
#ifndef _ZEBULON_HOTSUITE_INTERFACE__H
#define _ZEBULON_HOTSUITE_INTERFACE__H
#include <cxutils/circulararray.h>
#include <cxutils/packet.h>
#include <cxutils/math/coordinates.h>
#include <map>
#include <set>
#include <string>

namespace Zebulon
{
    namespace Hotsuite
    {

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Interface
        ///   \brief Interface class for the hotsuite message
        ////////////////////////////////////////////////////////////////////////////////////
        class  Interface
        {
        public:
            Interface();
            ~Interface();
            bool Initialize(std::string ipAddress);
            bool Send(const char* message, CxUtils::Wgs position, std::string targetName, const unsigned int postSendDelayMs = 10);
            //bool RegisterCallback(const Message::Code type, Callback* cb);
            //void RemoveCallback(const Message::Code type, Callback* cb);
            //void ClearCallback();
        protected:
            //static void SerialThread(void* arg);        ///<  Thread used to receive data from sensor.
            //CxUtils::Thread mSerialThread;              ///<  Manages/creates thread.
            //CxUtils::Thread mCallbackThread;            ///<  Thread object for callbacks.
            //CxUtils::Mutex  mSerialMutex;               ///<  Mutex for thread protection of data members.
            //CxUtils::Serial mSerial;                    ///<  Connection to serial port.
            CxUtils::Packet mPacket;
        private:
            //double mTemperature;                      ///< The raytek temperature
        };
    }
}

#endif //_ZEBULON_MCU_INTERFACE__H
/*  End of File */

