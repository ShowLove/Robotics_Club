//////////////////////////////////////////////////////////////////////////////////////
///
/// \file SetMotorOutput.cpp
/// \brief This command will cause the receiver to set an 8 bit Analog output port 
///        to the desired values. Can be used for setting servos or motors to a
///        desired value. 
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 05/05/2012<br>
/// Copyright (c) 2012<br>
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
#include "mcu/messages/setmotoroutput.h"

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetMotorOutput::SetMotorOutput() : Message(Message::SetMotorOutput)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetMotorOutput::~SetMotorOutput()
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
int SetMotorOutput::WriteMessageBody(CxUtils::Packet& packet) const
{
    int bytes = 0;
    bytes += packet.Write((unsigned char) mMotors.size());
    map<unsigned char, short>::const_iterator it;
    for (it = mMotors.begin(); it != mMotors.end(); it++)
    {
        bytes += packet.Write(it->first); //Writes Motor ID (unsigned char)
        bytes += packet.Write(it->second); //Writes Motor Thrust (signed short)
    }
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
int SetMotorOutput::ReadMessageBody(const CxUtils::Packet& packet)
{
    int bytes = 0;
    unsigned char numMotors;
    bytes += packet.Read(numMotors);
    for (unsigned char i = 0; i < numMotors; i++)
    {
        unsigned char motorID;
        short motorThrust;
        bytes += packet.Read(motorID);
        bytes += packet.Read(motorThrust);
        mMotors[motorID] = motorThrust;
    }
    return bytes;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints to console.
///
////////////////////////////////////////////////////////////////////////////////////
void SetMotorOutput::Print() const
{
    std::cout << "SetMotorOutput Message\n";
    map<unsigned char, short>::const_iterator it;
    for (it = mMotors.begin(); it != mMotors.end(); it++)
    {
        std::cout << "\tMotor ID: " << (int)(it->first) << " Motor Thrust: " << (int)(it->second) << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* SetMotorOutput::Clone() const
{
    SetMotorOutput* clone = new SetMotorOutput();
    clone->mMotors = this->mMotors;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
