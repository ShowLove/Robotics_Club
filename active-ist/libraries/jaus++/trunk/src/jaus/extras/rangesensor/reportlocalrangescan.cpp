////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportlocalrangescan.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  Created: 8 March 2010
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
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
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
#include "jaus/extras/rangesensor/reportlocalrangescan.h"
#include "jaus/core/scaledinteger.h"
#include <cxutils/math/cxmath.h>

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
int ReportLocalRangeScan::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE + UINT_SIZE*3 + USHORT_SIZE*3 + UINT_SIZE + UINT_SIZE;

    total += packet.Write(mSensorID);
    total += ScaledInteger::Write(packet, mLocation.mX, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Write(packet, mLocation.mY, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Write(packet, mLocation.mZ, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Write(packet, mOrientation.mX, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    total += ScaledInteger::Write(packet, mOrientation.mY, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    total += ScaledInteger::Write(packet, mOrientation.mZ, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    total += packet.Write(mTimeStamp.ToUInt());
    total += packet.Write( (UInt)(mScan.size()) );
       
    for(Scan::const_iterator s = mScan.begin();
        s != mScan.end();
        s++)
    {
        expected += USHORT_SIZE;
        total += packet.Write(*s);
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
int ReportLocalRangeScan::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE + UINT_SIZE*3 + USHORT_SIZE*3 + UINT_SIZE + UINT_SIZE;

    total += packet.Read(mSensorID);
    total += ScaledInteger::Read(packet, mLocation.mX, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Read(packet, mLocation.mY, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Read(packet, mLocation.mZ, 100000.0, -100000.0, ScaledInteger::UInt);
    total += ScaledInteger::Read(packet, mOrientation.mX, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    total += ScaledInteger::Read(packet, mOrientation.mY, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    total += ScaledInteger::Read(packet, mOrientation.mZ, CxUtils::CX_PI, -CxUtils::CX_PI, ScaledInteger::UShort);
    UInt timestamp = 0;
    total += packet.Read(timestamp);
    mTimeStamp.SetTime(timestamp);
    UInt size = 0;
    total += packet.Read(size);
    UShort val = 0;
    mScan.reserve(size);
    for(UInt i = 0; i < size; i++)
    {
        expected += USHORT_SIZE;
        total += packet.Read(val);
        mScan.push_back(val);
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportLocalRangeScan::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int expected = BYTE_SIZE + UINT_SIZE*3 + USHORT_SIZE*3 + UINT_SIZE + USHORT_SIZE*mScan.size();

    return expected > maxPayloadSize ? true : false;
}



/*  End of File */
