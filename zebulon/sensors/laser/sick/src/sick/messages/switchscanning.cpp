/////////////////////////////////////////////////////////////////////////////////////
///
/// \file switchscanning.cpp
/// \brief Message class for configuring the SICK LIDAR.
///
/// Author(s): Jon Carbone<br>
/// Created: 4/10/2008<br>
/// Last Modified: 4/10/2008<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: joncarbone@gmail.com<br>
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
#include "sick/messages/switchscanning.h"

using namespace Zebulon;
using namespace Laser;
using namespace SICK;

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SwitchScanning::SwitchScanning(unsigned short scanningAngle,
                               unsigned short angularResolution) : 
                                                   Message(Message::SwitchScanning),
                                                   mScanningAngle(scanningAngle),
                                                   mAngularResolution(angularResolution)
{
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy Constructor.
///
///////////////////////////////////////////////////////////////////////////////
SwitchScanning::SwitchScanning(const SwitchScanning& msg) : Message(Message::SwitchScanning)
{
    *this = msg;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes a SwitchScanning message to a packet.
///
///  \param packet Packet to write the SwitchScanning message to
///
///  \return The number of bytes written to the packet
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SwitchScanning::Write(CxUtils::Packet& packet) const
{
    packet.Clear();

    WriteHeader(packet, mMessageType, 5);

    packet.Write(mScanningAngle);
    packet.Write(mAngularResolution);

    WriteCRC(packet);

    return packet.Length();
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a SwitchScanning message and populates the internal members
///
///  \param packet The packet containing the SwitchScanning message
///
///  \return 1 if the message was successfully parsed, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
unsigned int SwitchScanning::Read(const CxUtils::Packet& packet)
{

    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the test case of the SwitchScanning message
///
///  \return 1 if the test case passes, 0 otherwise
///
///////////////////////////////////////////////////////////////////////////////
int SwitchScanning::RunTestCase() const
{
    return 0;
}

///////////////////////////////////////////////////////////////////////////////
///
///  \brief Assignment operator overload to copy to internal members
///
///  \param msg A SwitchScanning message to copy values from
///
///  \return A pointer to the instance of the SwitchScanning message
///
///////////////////////////////////////////////////////////////////////////////
SwitchScanning& SwitchScanning::operator=(const SwitchScanning& msg)
{
    if( this != &msg )
    {
        mAddress = msg.mAddress;
        mScanningAngle = msg.mScanningAngle;
        mAngularResolution = msg.mAngularResolution;
    }

    return *this;
}

/*  End of File */
