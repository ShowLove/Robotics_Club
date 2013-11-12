////////////////////////////////////////////////////////////////////////////////////
///
///  \file deleteelement.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 March 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#include "jaus/mobility/list/deleteelement.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
DeleteElement::DeleteElement(const Address& dest, const Address& src) : Message(DELETE_ELEMENT, dest, src)
{
    mRequestID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DeleteElement::DeleteElement(const DeleteElement& message) : Message(DELETE_ELEMENT)
{
    mRequestID = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
DeleteElement::~DeleteElement()
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
int DeleteElement::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE*2;
    int written = 0;

    written += packet.Write(mRequestID);
    written += packet.WriteByte( (Byte)mElementUIDs.size() );

    List::const_iterator element;

    for(element = mElementUIDs.begin();
        element != mElementUIDs.end();
        element++)
    {
        expected += USHORT_SIZE;
        written += packet.Write((*element));
    }

    return expected == written ? written : -1;
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
int DeleteElement::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE*2;
    int read = 0;
    int tRead = 0;

    Byte count = 0;
    UShort EUID = 0;

    read += packet.Read(mRequestID);
    read += packet.Read(count);

    for(UShort e = 0; e < count; e++)
    {
        expected += USHORT_SIZE;
        if((tRead = packet.Read(EUID)) > 0)
        {
            read += tRead;
            mElementUIDs.push_back(EUID);
        }
    }

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message contents.
///
////////////////////////////////////////////////////////////////////////////////////
void DeleteElement::ClearMessageBody()
{
    mRequestID = 0;
    mElementUIDs.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DeleteElement::RunTestCase() const
{
    int result = 0;

    Packet packet;

    DeleteElement msg1, msg2;
    msg1.GetElementList()->push_back(34);
    msg1.GetElementList()->push_back(43);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        if(*msg1.GetElementList() == *msg2.GetElementList())
        {
            result = 1;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
DeleteElement& DeleteElement::operator=(const DeleteElement& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mRequestID = message.mRequestID;
        mElementUIDs = message.mElementUIDs;
    }
    return *this;
}


/*  End of File */
