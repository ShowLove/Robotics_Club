/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gx2.h
/// \brief RS232 Interface class for the MicroStrain 3DM-GX2 sensor.
///
/// Author(s): Gary Stein<br>
/// Created: 2009<br>
/// Copyright (c) 2007<br>
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
#ifndef __ZEBULON_MICROSTRAIN_3DMGX2__H
#define __ZEBULON_MICROSTRAIN_3DMGX2__H

#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <set>
#include "microstrain/gx2/euleranglesandangularrates2.h"
#include "microstrain/gx2/setcontinuousmode2.h"
#include "microstrain/gx2/accelerationangularrateandorientationmatrix2.h"

#include <compass/compass.h>

#define GX2_IMU

namespace Zebulon
{
    namespace Compass
    {
        namespace MicroStrain
        {
            typedef Callback Callback2;  // To support previous capability.

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class GX2
            ///   \brief Interface class for the MicroStrain 3DM-GX2 Orientation Sensor using
            ///   RS232.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL GX2 : public GenericCompass
            {
            public:
                // Constructor.
                GX2(const unsigned int maxQueueSize = 15);
                // Destructor.
                virtual ~GX2();
                // Disconnects from the device and closes any active threads.
                virtual void Shutdown();
                
                // Sets the sensor to continuously send a specific type of data.
                bool SetContinuousMode(const Message::Type type);
                // Transmit message and get response.
                bool TransmitMessage(const CxUtils::Packet message, 
                                     MicroStrain::Message** response,
                                     unsigned int timeout = 1000);
            protected:
            
                virtual void ProcessingFunction();     ///<  Thread used to receive data from sensor.
                virtual bool SetContinuousMode();
                bool RunCallback(const Message* message);
                
                CxUtils::Mutex mTransmittingMutex;      ///<  Mutex to signal sending data.
                CxUtils::Mutex mMessageQueueMutex;      ///<  Mutex for message queue.
                CxUtils::Mutex mCallbackMutex;          ///<  Mutex for callbacks.
                Message::Type mContinuousModeType;      ///<  Type of continuous mode to maintain.
                Message::List mMessageQueue;            ///<  All received messages.
                unsigned int mMaxQueueSize;                 ///<  Maximum number of messages to store.
                const static unsigned char GX2Confirm1 = 0xC1;
                const static unsigned char GX2Confirm2 = 0x29;
                
            };
        }
    }
}

#endif
/* End of File */
