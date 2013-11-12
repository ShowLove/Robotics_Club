/////////////////////////////////////////////////////////////////////////////
///
/// \file dgps.h
/// \brief A simple GPS class capable of receiving data from any NMEA
/// compliant GPS using a serial connection.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/28/2008<br>
/// Last Modified: 2/28/2008<br>
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
/////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_GPS_DGPS__H
#define _ZEBULON_GPS_DGPS__H

#include <string>
#include <list>
#include <set>
#include <vector>
#include <cxutils/serial.h>
#include <cxutils/thread.h>
#include <nmea/nmea.h>
#include <gps/gps.h>

namespace Zebulon
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///  \namespace GPS
    ///  \brief Namespace for all GPS related software in library.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    namespace GPS
    {
        namespace NMEA
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Callback
            ///  \brief Callback class used to register for message events.
            ///
            ///  Inherit from this class and overload the process message method and pass a
            ///  pointer to your child class to the RegisterCallback method of GPS and you
            ///  will get every message received as it arrives as a callback.
            ///
            ////////////////////////////////////////////////////////////////////////////////////            
            class ZEB_NMEA_DLL Callback : public GPS::Callback
            {
                public:
                    virtual void ProcessMessage(const Message* message) = 0;
            };
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class DGPS
            ///  \brief Simple Differential GPS class which reads incomming NMEA messages 
            ///   from a GPS device over a serial connection.
            ///
            ///  It is possible to configure this interface to only listen for specific
            ///  types of messages.  By default it only listens for GPGGA NMEA messages, so use
            ///  the AddMessageType method to add more.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_NMEA_DLL DGPS : public GenericGPS
            {
            public:

                typedef std::set<NMEA::Message::Type> TypeSet;
                typedef std::list<NMEA::Message*> MessageList;
                typedef std::vector<NMEA::Message*> MessageVector;
                
                DGPS();
                virtual ~DGPS();
                
                //Not currently used...in v3
                virtual int Initialize(const std::string& xml);
                
                virtual void Shutdown();
                int SetMessageTypes(const DGPS::TypeSet types);
                int AddMessageType(const NMEA::Message::Type type);
                int RemoveMessageType(const NMEA::Message::Type type);
                int SetMaximumListSize(const unsigned int size);
                bool HaveSignal() const { return mHaveSignalFlag; }
                bool LostSignal() const { return !mHaveSignalFlag; }
                unsigned int GetMaximumListSize() const { return mListSize; }
                NMEA::Message* GetLastMessage() const;
                DGPS::MessageVector GetMessages(const unsigned int num = 1) const;
            protected:
                virtual void ProcessingFunction();
                virtual bool SetContinuousMode();
                
                bool RunCallback(const Message* message);
                
                volatile bool mHaveSignalFlag;  ///<  Flag used to check for valid GPS data.
                volatile bool mRunningFlag;     ///<  Thread is running?
                volatile unsigned int mListSize;///<  Maximum number of messages to store in list.
                TypeSet mMessageTypes;          ///<  Types of messages to listen for (all others ignored).
                MessageList mMessages;          ///<  List of all messages from GPS.
            };
        }
    }
}

#endif
/*  End of File */
