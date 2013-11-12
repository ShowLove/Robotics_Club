//////////////////////////////////////////////////////////////////////////////////////
///
/// \file setservooutputs.cpp
/// \brief his command will cause the receiver to set the pinger frequency..
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 03/14/2010br>
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
#include "mcu/messages/setservooutputs.h"

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetServoOutputs::SetServoOutputs() : Message(Message::SetServoOutputs)
{
    mChannel = 0;
    mValue = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetServoOutputs::~SetServoOutputs()
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
int SetServoOutputs::WriteMessageBody(CxUtils::Packet& packet) const
{
    int bytes = 0;
    bytes += packet.Write(mChannel);
    bytes += packet.Write(mValue);
    return bytes;
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
int SetServoOutputs::ReadMessageBody(const CxUtils::Packet& packet)
{
    int bytes = 0;
    bytes += packet.Read(mChannel);
    bytes += packet.Read(mValue);
    return bytes;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void SetServoOutputs::Print() const
{
    std::cout << "SetServoOutputs Message\nChannel: "<< (int)mChannel << "\tValue: " << (int)mValue << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* SetServoOutputs::Clone() const
{
    SetServoOutputs* clone = new SetServoOutputs();
    clone->mChannel = this->mChannel;
    clone->mValue = this->mValue;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Takes in a percentage and converts it to the one byte mValue for the
///   message. Input range = [-100.0, 100.0] maps to output range of [0, 255].
///
///   \param[in] value Input value by percentage, [-100.0, 100.0].
///
////////////////////////////////////////////////////////////////////////////////////
void SetServoOutputs::SetWithPercent(double value)
{
    int convertedValue = (int)(((value + 100.0)/200.0)*255.0);
    if (convertedValue > 255)
    {
        convertedValue = 255;
    }
    else if (convertedValue < 0)
    {
        convertedValue = 0;
    }
    mValue = (unsigned char)(convertedValue);
}

/*  End of File */
