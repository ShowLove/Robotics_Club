////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportrangesensorconfiguration.cpp
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
#include "jaus/extras/rangesensor/reportrangesensorconfiguration.h"

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
int ReportRangeSensorConfiguration::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE;

    RangeSensorConfig::List::const_iterator config;

    total += packet.Write((Byte)mConfiguration.size());
    
    for(config = mConfiguration.begin();
        config != mConfiguration.end();
        config++)
    {
        expected += BYTE_SIZE*2 + UINT_SIZE + LONG_FLOAT_SIZE*4 + (UInt)config->mName.size();
        total += packet.WriteByte(config->mID);
        total += packet.Write( (UInt)config->mName.size() );
        total += packet.Write( config->mName);
        total += packet.Write( config->mMaxRange );         
        total += packet.Write( config->mMinRange );        
        total += packet.Write( config->mAngleIncrement );  
        total += packet.Write( config->mScanAngle );       
        total += packet.WriteByte((Byte)config->mUnitType);
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
int ReportRangeSensorConfiguration::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE;
    Byte size = 0;

    total += packet.Read(size);
    
    for(Byte i = 0; i < size; i++)
    {
        Byte bval = 0;
        UInt uval = 0;
        RangeSensorConfig config;

        expected += BYTE_SIZE*2 + UINT_SIZE + LONG_FLOAT_SIZE*4;
        total += packet.Read(bval);
        config.mID = bval;
        total += packet.Read(uval);
        if(uval > 0)
        {
            total += packet.Read(config.mName, uval);
            expected += uval;
        }
        total += packet.Read(config.mMaxRange);
        total += packet.Read(config.mMinRange);
        total += packet.Read(config.mAngleIncrement);
        total += packet.Read(config.mScanAngle);
        /*
        total += packet.Read(uval);
        config.mMaxRange = uval/1000.0;
        total += packet.Read(uval);
        config.mMinRange = uval/1000.0;
        total += packet.Read(uval);
        config.mAngleIncrement = uval/1000.0;
        total += packet.Read(uval);
        config.mScanAngle = uval/1000.0;
        */
        total += packet.Read(bval);
        config.mUnitType = (RangeSensorConfig::UnitType)bval;

        mConfiguration.push_back(config);
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportRangeSensorConfiguration::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int expected = BYTE_SIZE;

    RangeSensorConfig::List::const_iterator config;
    
    for(config = mConfiguration.begin();
        config != mConfiguration.end();
        config++)
    {
        expected += BYTE_SIZE*2 + UINT_SIZE*4;
    }

    return expected > maxPayloadSize ? true : false;
}


/* End of File */
