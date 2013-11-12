/////////////////////////////////////////////////////////////////////////////////////
///
/// \file gx1.h
/// \brief RS232 Interface class for the MicroStrain 3DM-GX1 sensor.
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
#ifndef __ZEBULON_MICROSTRAIN_3DMGX1__H
#define __ZEBULON_MICROSTRAIN_3DMGX1__H

#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <set>
#include "microstrain/gx1/sendgyrostabilizedeulerangles.h"
#include "microstrain/gx1/sendgyrostabilizedimuvector.h"
#include "microstrain/gx1/setcontinuousmode.h"

#include <compass/compass.h>

//This should almost always be defined
//it really never hurts to have IMU data
#define GX1_IMU

namespace Zebulon
{
    namespace Compass
    {
        namespace MicroStrain
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class GX1
            ///   \brief Interface class for the MicroStrain 3DM-GX1 Orientation Sensor using
            ///   RS232.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_MICROSTRAIN_DLL GX1 : public GenericCompass
            {
            public:
                // Constructor.
                GX1(const unsigned int maxQueueSize = 15);
                // Destructor.
                virtual ~GX1();
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
                unsigned int mMaxQueueSize;             ///<  Maximum number of messages to store.
            };
        }
    }
}

#endif
/* End of File */
