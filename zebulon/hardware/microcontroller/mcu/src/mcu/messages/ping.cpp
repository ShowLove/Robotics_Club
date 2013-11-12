//////////////////////////////////////////////////////////////////////////////////////
///
/// \file ping.cpp
/// \brief Message command to get a Pong response from an MCU.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/14/2009br>
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
#include "mcu/messages/ping.h"

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Ping::Ping() : Message(Message::Ping)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Ping::~Ping()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes the contents of the message payload to the packet.
///
///   \param[in] packet Packet to write to.
///
///   \return Bytes processed, - 1 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Ping::WriteMessageBody(CxUtils::Packet& packet) const
{
    packet.WriteByte('P');
    packet.WriteByte('I');
    packet.WriteByte('N');
    packet.WriteByte('G');
    return 4;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the contents of the message payload from the packet, and saves
///   to internal data members.
///
///   \param[in] packet Packet to read from.
///
///   \return Bytes processed, - 1 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Ping::ReadMessageBody(const CxUtils::Packet& packet)
{
    int blank = 0;
    if(packet.Read(blank) &&
       packet.Read(blank) &&
       packet.Read(blank) &&
       packet.Read(blank))
    {
        return 4;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints message contents to console.
///
////////////////////////////////////////////////////////////////////////////////////
void Ping::Print() const
{
    std::cout << "Ping Message\n";
}

/*  End of File */
