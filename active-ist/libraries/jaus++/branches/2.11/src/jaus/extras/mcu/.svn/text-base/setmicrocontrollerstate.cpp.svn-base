////////////////////////////////////////////////////////////////////////////////////
///
///  \file setmicrocontrollerstate.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 9 February 2010
///  Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the val of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/extras/mcu/setmicrocontrollerstate.h"
#include "jaus/core/scaledinteger.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes message payload to the packet.
///
///   Message contents are written to the packet following the JAUS standard.
///
///   \param[out] packet Packet to write payload to.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetMicrocontrollerState::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = 0;

    Microcontroller::DigitalStates::const_iterator digital;
    Microcontroller::AnalogStates::const_iterator analog;

    expected += UINT_SIZE;
    total += packet.Write((UInt)mDigitalStates.size());
    
    for(digital = mDigitalStates.begin();
        digital != mDigitalStates.end();
        digital++)
    {
        expected += UINT_SIZE + (UInt)digital->first.size() + BYTE_SIZE;
        total += packet.Write((UInt)digital->first.size());
        total += packet.Write((const unsigned char *)digital->first.c_str(), (UInt)digital->first.size());
        total += packet.WriteByte(digital->second);
    }

    expected += UINT_SIZE;
    total += packet.Write((UInt)mAnalogStates.size());

    for(analog = mAnalogStates.begin();
        analog != mAnalogStates.end();
        analog++)
    {
        expected += UINT_SIZE + (UInt)analog->first.size() + INT_SIZE;
        total += packet.Write((UInt)analog->first.size());
        total += packet.Write((const unsigned char *)analog->first.c_str(), (UInt)analog->first.size());
        total += ScaledInteger::Write(packet, analog->second, 100.0, -100.0, ScaledInteger::Int);
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads message payload from the packet.
///
///   Message contents are read from the packet following the JAUS standard.
///
///   \param[in] packet Packet containing message payload data to read.
///
///   \return -1 on error, otherwise number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int SetMicrocontrollerState::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = UINT_SIZE*2;
    UInt size = 0;

    total += packet.Read(size);
    
    for(UInt i = 0; i < size; i++)
    {
        UInt len = 0;
        Byte val = 0;
        expected += UINT_SIZE + BYTE_SIZE;
        total += packet.Read(len);
        expected += len;
        std::string str;
        total += packet.Read(str, len); 
        total += packet.Read(val);
        mDigitalStates[str] = val > 0 ? true : false;
    }

    size = 0;
    total += packet.Read(size);
    
    for(UInt i = 0; i < size; i++)
    {
        UInt len = 0;
        double val = 0.0;
        expected += UINT_SIZE + INT_SIZE;
        total += packet.Read(len);
        expected += len;
        std::string str;
        total += packet.Read(str, len); 
        total += ScaledInteger::Read(packet, val, 100.0, -100.0, ScaledInteger::Int);
        mAnalogStates[str] = val;
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetMicrocontrollerState::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int expected = UINT_SIZE*2;

    Microcontroller::DigitalStates::const_iterator digital;
    Microcontroller::AnalogStates::const_iterator analog;
    
    for(digital = mDigitalStates.begin();
        digital != mDigitalStates.end();
        digital++)
    {
        expected += UINT_SIZE + (UInt)digital->first.size() + BYTE_SIZE;
    }

    for(analog = mAnalogStates.begin();
        analog != mAnalogStates.end();
        analog++)
    {
        expected += UINT_SIZE + (UInt)analog->first.size() + INT_SIZE;

    }

    return expected > maxPayloadSize ? true : false;
}

/*  End of File */
