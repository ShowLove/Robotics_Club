//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportmotorstatus.cpp
/// \brief This message contains the information provided by the seabotix motors.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 4/03/2010br>
/// Copyright (c) 2009<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
#include "mcu/messages/custom/reportmotorstatus.h"

using namespace Zebulon;
using namespace MCU;
using namespace CustomMessage;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportMotorStatus::ReportMotorStatus() : Message(REPORT_MOTOR_STATUS)
{
    mPort = 0;
    //mAddress = 0;
    mFault.FByte = 0;
    mCurrent = 0;
    mTempurature = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportMotorStatus::~ReportMotorStatus()
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
int ReportMotorStatus::WriteMessageBody(CxUtils::Packet& packet) const
{
    packet.WriteByte(mPort);
    packet.WriteByte(0);
    packet.WriteByte(0);
    packet.WriteByte(0);
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
int ReportMotorStatus::ReadMessageBody(const CxUtils::Packet& packet)
{
    //unsigned char blank = 0;
    if(packet.Read(mPort) &&
       packet.Read(mFault.FByte)&&
       packet.Read(mCurrent) &&
       packet.Read(mTempurature))
    {
        return 4;
    }
    return 0;
}

void ReportMotorStatus::Print() const
{
    double tempCurr, tempTemp;
    tempCurr = mCurrent/10.0;
    tempTemp = (mTempurature*9.0)/5.0 + 32;
    
    
    std::cout << "ReportMotorStatus Message\tPort: " << std::hex << "0x" << (int)mPort << "\tFault: " << "0x" << (int)mFault.FByte << std::dec << "\tCurrent: " << tempCurr << "\tTemperature: " << tempTemp << std::endl;
    std::cout << "ReportMotorStatus Message: " << (int)mFault.FBits.nStalled << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ReportMotorStatus::Clone() const
{
    ReportMotorStatus* clone = new ReportMotorStatus();
    clone->mPort = this->mPort;
    clone->mFault = this->mFault;
    clone->mCurrent = this->mCurrent;
    clone->mTempurature = this->mTempurature;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
