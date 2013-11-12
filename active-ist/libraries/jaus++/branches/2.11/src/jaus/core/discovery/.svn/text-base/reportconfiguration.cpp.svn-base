////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportconfiguration.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 November 2009
///  <br>Copyright (c) 2009
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
#include "jaus/core/discovery/reportconfiguration.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportConfiguration::ReportConfiguration(const Address& dest,
                                         const Address& src) : Message(REPORT_CONFIGURATION, dest, src)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportConfiguration::ReportConfiguration(const ReportConfiguration& message) : Message(REPORT_CONFIGURATION)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportConfiguration::~ReportConfiguration()
{

}


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
int ReportConfiguration::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE;
    total += packet.WriteByte((Byte)mNodes.size());
    
    Nodes::const_iterator node;
    for(node = mNodes.begin();
        node != mNodes.end();
        node++)
    {
        expected += BYTE_SIZE*2;
        total += packet.Write(node->first);
        total += packet.WriteByte((Byte)node->second.size());

        Record::List::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            expected += BYTE_SIZE*2;
            total += packet.Write(component->mComponent);
            total += packet.Write(component->mInstance);
        }
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
int ReportConfiguration::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE;
    Byte count1 = 0;

    total += packet.Read(count1);
    for(Byte i = 0; i < count1; i++)
    {
        expected += BYTE_SIZE*2;
        Byte node = 0;
        Byte count2 = 0;
        Record::List components;

        total += packet.Read(node);
        total += packet.Read(count2);
        for(Byte j = 0; j < count2; j++)
        {
            expected += BYTE_SIZE*2;
            Record record;
            total += packet.Read(record.mComponent);
            total += packet.Read(record.mInstance);
            components.push_back(record);
        }
        mNodes[node] = components;
    }

    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportConfiguration::ClearMessageBody()
{
    mNodes.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportConfiguration& ReportConfiguration::operator =(const ReportConfiguration& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mNodes = message.mNodes;
    }
    return *this;
}


/*  End of File */
