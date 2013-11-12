//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportencoders.cpp
/// \brief This message contains the values of an 8 bit digital input port. 
///        Can be used for general purpose inputs like buttons and switches.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
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
#include "mcu/messages/reportencoders.h"

using namespace Zebulon;
using namespace MCU;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEncoders::ReportEncoders() : Message(Message::ReportEncoders)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportEncoders::~ReportEncoders()
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
int ReportEncoders::WriteMessageBody(CxUtils::Packet& packet) const
{
    int bytes = 0;
	unsigned char count = (unsigned char)mEncoders.size();
    bytes += packet.WriteByte(count);
	for(unsigned char c = 0; c < count; c++)
	{
		bytes += packet.Write((short)mEncoders[c]);
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
int ReportEncoders::ReadMessageBody(const CxUtils::Packet& packet)
{
    int bytes = 0;
	unsigned char count = 0;
	short value;
	mEncoders.clear();
	//((CxUtils::Packet *)&packet)->SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
    bytes += packet.Read(count);
	for(unsigned char c = 0; c < count; c++)
	{
		bytes += packet.Read(value);
		mEncoders.push_back(value);
	}
    return bytes;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportEncoders::Print() const
{
    std::cout << "Report Encoders: \n";
	for(unsigned int i = 0; i < (unsigned int)mEncoders.size(); i++)
	{
		std::cout << "\t" << mEncoders[i] << std::endl;
	}
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ReportEncoders::Clone() const
{
    ReportEncoders* clone = new ReportEncoders();
    clone->mEncoders = mEncoders;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
