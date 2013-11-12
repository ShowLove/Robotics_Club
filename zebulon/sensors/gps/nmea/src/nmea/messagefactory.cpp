/////////////////////////////////////////////////////////////////////////////
///
/// \file messagefactory.cpp
/// \brief Class for dynamic creation of NMEA messages.
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
/////////////////////////////////////////////////////////////////////////////
#include "nmea/messagefactory.h"
#include "nmea/messages/gpgga.h"
#include "nmea/messages/gpgll.h"
#include <string.h>

using namespace Zebulon;
using namespace GPS;
using namespace NMEA;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Based on the message header, finds out what kind of message it is.
///
///  \param header The message header.
///
///  \return Message type.  Message::Unknown if unknown message header.
///
////////////////////////////////////////////////////////////////////////////////////
Message::Type MessageFactory::GetType(const std::string& header)
{
    Message::Type t = Message::Unknown;
    if(strstr(header.c_str(), "GP") != NULL )
    {
        if(header == "GPGLL")
        {
            t = Message::GPGLL;
        }
        else if(header == "GPGGA")
        {
            t = Message::GPGGA;
        }
    }

    return t;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Based on the type passed, the correct message is allocated and
///         returned.
///
///  \param type Message type to create.
///
///  \return Pointer to newly created message, NULL on error.
///
////////////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::CreateMessage(const Message::Type type)
{
    Message* msg = NULL;
    
    switch(type)
    {
    case Message::GPGGA:
        msg = new GPGGA();
        break;
    case Message::GPGLL:
        msg = new GPGLL();
        break;
    default:
        msg = NULL;
        break;
    }

    return msg;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Based on the header passed, the correct message is allocated and
///         returned.
///
///  \param header The message header.
///
///  \return Pointer to newly created message, NULL on error.
///
////////////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::CreateMessage(const std::string& header)
{
    return CreateMessage(GetType(header));
}

/*  End of File */
