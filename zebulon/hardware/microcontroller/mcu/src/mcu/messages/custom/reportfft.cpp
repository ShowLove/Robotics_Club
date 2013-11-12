//////////////////////////////////////////////////////////////////////////////////////
///
/// \file reportfft.h
/// \brief This message contains the values of the pinger fft.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 07/03/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
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
#include "mcu/messages/custom/reportfft.h"

using namespace Zebulon;
using namespace MCU;
using namespace CustomMessage;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportFFT::ReportFFT() : Message(REPORT_FFT)
{
    mDataType = 0;
    mData.mUInt = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportFFT::~ReportFFT()
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
int ReportFFT::WriteMessageBody(CxUtils::Packet& packet) const
{
    unsigned char blank = 0;
    packet.Write(mDataType);
    packet.Write(mData.mUChars.mByte0);
    packet.Write(mData.mUChars.mByte1);
    packet.Write(mData.mUChars.mByte2);
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
int ReportFFT::ReadMessageBody(const CxUtils::Packet& packet)
{
    unsigned char blank = 0;
    if(packet.Read(mDataType) &&
       packet.Read(mData.mUChars.mByte0) &&
       packet.Read(mData.mUChars.mByte1) &&
       packet.Read(mData.mUChars.mByte2))
    {
        if (mData.mUChars.mByte2 & 0x80)
        {
            mData.mUChars.mByte3 = 0xFF;
        }
        else
        {
            mData.mUChars.mByte3 = 0x00;
        }
        return 4;
    }
    return 0;
}

void ReportFFT::Print() const
{
    std::cout << "ReportFFT Message\nDataType: " << (int) mDataType << ", Data: " << (int) mData.mUInt << std::endl;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes a clone of the message.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ReportFFT::Clone() const
{
    ReportFFT* clone = new ReportFFT();
    clone->mDataType = this->mDataType;
    clone->mData = this->mData;
    clone->mTimeStamp = mTimeStamp;
    return clone;
}
/*  End of File */
