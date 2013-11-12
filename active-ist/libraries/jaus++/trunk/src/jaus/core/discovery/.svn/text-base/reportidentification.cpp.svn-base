////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportidentification.cpp
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
#include "jaus/core/discovery/reportidentification.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportIdentification::ReportIdentification(const Address& dest,
                                           const Address& src) : Message(REPORT_IDENTIFICATION, dest, src)
{
    mQueryType = ComponentIdentification;
    mIdentificationType = Vehicle;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportIdentification::ReportIdentification(const ReportIdentification& message) : Message(REPORT_IDENTIFICATION)
{
    mQueryType = ComponentIdentification;
    mIdentificationType = Vehicle;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportIdentification::~ReportIdentification()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the identification.
///
///   \param[in] identification Identification string in human-recognizable 
///                             format.  Size must be <= 255 characters.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportIdentification::SetIdentification(const std::string& identification)
{
    if(identification.size() <= 255)
    {
        mIdentification = identification;
        return true;
    }
    return false;
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
int ReportIdentification::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    int expected = BYTE_SIZE*2 + USHORT_SIZE;
    total += packet.WriteByte((Byte)mQueryType);
    total += packet.Write((UShort)mIdentificationType);
    total += packet.WriteByte((Byte)mIdentification.size());
    if(mIdentification.size() > 0)
    {
        expected += (int)mIdentification.size();
        total += packet.Write(mIdentification);
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
int ReportIdentification::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    int expected = BYTE_SIZE*2 + USHORT_SIZE;
    Byte count;
    total += packet.Read((Byte &)mQueryType);
    total += packet.Read((UShort&)mIdentificationType);
    total += packet.Read(count);
    if(count > 0)
    {
        expected += count;
        total += packet.Read(mIdentification, (unsigned int)count);
    }
    return total == expected ? total : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportIdentification::ClearMessageBody()
{
    mQueryType = ComponentIdentification;
    mIdentificationType = Vehicle;
    mIdentification.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportIdentification& ReportIdentification::operator =(const ReportIdentification& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mQueryType = message.mQueryType;
        mIdentificationType = message.mIdentificationType;
        mIdentification = message.mIdentification;
    }
    return *this;
}


/*  End of File */
