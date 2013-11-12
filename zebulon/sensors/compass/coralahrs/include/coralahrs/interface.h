//////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.h
/// \brief Interface to Coral AHRS Digital Compass.
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
#ifndef _ZEBULON_CORAL_AHRS_INTERFACE__H
#define _ZEBULON_CORAL_AHRS_INTERFACE__H

#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <map>
#include <set>
#include "coralahrs/message.h"
#include "coralahrs/messages/dataquat.h"
#include "coralahrs/messages/dataeuler.h"
#include "coralahrs/messages/dataeulerandsensors.h"
#include "coralahrs/messages/configuration.h"
#include "coralahrs/messages/pong.h"
#include "coralahrs/messages/setoutputmode.h"
#include "coralahrs/messages/requestconfiguration.h"
#include "coralahrs/messages/ping.h"

#include <compass/compass.h>

namespace Zebulon
{
    namespace Compass
    {
        namespace CoralAHRS
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Callback
            ///   \brief Interface to get callback events from interface.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL Callback : public Compass::Callback
            {
                public:
                    virtual void ProcessMessage(const Message* message) = 0;
            };

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Interface
            ///   \brief Interface class for the Coral AHRS Digital Compass.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_CORALAHRS_DLL Interface : public GenericCompass
            {
            public:
                Interface();
                virtual ~Interface();
                bool SetOutputModeEuler();
                bool Send(const Message* message);
                bool RunCallback(const Message* message);
                static Message* CreateMessage(const unsigned char type);
            protected:
                virtual void ProcessingFunction();      ///<  Thread used to receive data from sensor.
                virtual bool SetContinuousMode();
                Message::List mMessageQueue;            ///<  Queue of received Coral AHRS messages.
            private:
                CxUtils::Packet mLargeBuffer;           ///< Packet buffer to store received byte data in.
                CxUtils::Packet mSmallBuffer;           ///< Packet buffer to store received byte data in.
            };
        }
    }
}

#endif
/*  End of File */
