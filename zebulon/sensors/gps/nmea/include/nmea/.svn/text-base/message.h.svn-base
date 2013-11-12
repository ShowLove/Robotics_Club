////////////////////////////////////////////////////////////////////////////////////
///
/// \file message.h
/// \brief Base message class for creating NMEA message structures.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/23/2008<br>
/// Last Modified: 2/23/2008<br>
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
#ifndef __ZEBULON_NMEA_MESSAGE__H
#define __ZEBULON_NMEA_MESSAGE__H

#include <string>
#include <cxutils/packet.h>
#include <cxutils/math/coordinates.h>


//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_NMEA_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_NMEA_DLL
    #ifdef WIN32
        #define ZEB_NMEA_DLL __declspec(dllimport)
    #else
        #define ZEB_NMEA_DLL
    #endif
#endif

////////////////////////////////////////////////////////////////////////////////////
///
///  \namespace Zebulon
///  \brief Namespace for all software developed by the Robotics Laboratory and
///         Club at the University of Central Florida.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Zebulon
{
    namespace GPS
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace NMEA 
        ///  \brief National Maritime Electonrics Association (NMEA) namespace for the
        ///         NMEA Message Library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace NMEA
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Message
            ///  \brief Message class for creating NMEA message structures.
            ///
            ///  Methods you must overload are:<br>
            ///  GetPosition - If the message has World Geodetic Systems (WGS) position data
            ///                return 1, and save values to Wgs structure.  If not
            ///                supported, return 0.<br>
            ///  Write - Write data members to a Packet in the correct NMEA format.<br>
            ///  Read - Read data in supported NMEA format and save to internal data 
            ///         members.<br>
            ///  GetUTC - If message has UTC data, then return 1 and copy to input
            ///           parameter, otherwise return 0.
            ///  Clone - Make a copy of your message and return pointer to it. <br>
            ///
            ///  Optional methods to overload:<br>
            ///  RunTestCase - Perform a test of your message class. Return 1 on success
            ///                and 0 on failure.<br>
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_NMEA_DLL Message
            {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \var Type
                ///  \brief Enumerations for the different types of NMEA messages.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                enum Type
                {
                    Unknown = 0,
                    GPGLL,
                    GPGGA
                };
               
                Message(const Message::Type type, const std::string& header);
                virtual ~Message();
                virtual int GetPosition(CxUtils::Wgs& pos) const = 0;
                virtual int GetUTC(double& utc) const = 0;
                virtual unsigned int Write(CxUtils::Packet& msg) const = 0;
                virtual unsigned int Read(const CxUtils::Packet& msg) = 0;
                virtual Message* Clone() const = 0;      
                static int RunTestCase() { return 0; }
                static bool IsValidMessage(const CxUtils::Packet& msg);
                static bool ExtractMessage(const unsigned char* buff, 
                                           const unsigned int len,
                                           CxUtils::Packet& msg,
                                           std::string* header = NULL);
                static bool WriteChecksum(CxUtils::Packet& msg);
                void SetMessageTimeStamp(const CxUtils::Time::Stamp t);
                virtual void Print() const;
                CxUtils::Time::Stamp GetMessageTimeStamp() const { return mTimeStamp; }
                inline Message::Type GetType() const { return mMessageType; }
                inline std::string GetHeader() const { return mMessageHeader; }      
            protected:
                void CopyMessageData(const Message* msg)
                {
                    mTimeStamp = msg->mTimeStamp;
                }
                CxUtils::Time::Stamp mTimeStamp;                  ///< Time of message creation.
            private:
                const Message::Type mMessageType; ///<  Message type.
                const std::string mMessageHeader; ///<  Message header string.
            };
        }
    }
}


#endif
/*  End of File */
