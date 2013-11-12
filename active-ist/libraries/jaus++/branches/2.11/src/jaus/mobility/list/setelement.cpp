////////////////////////////////////////////////////////////////////////////////////
///
///  \file setelement.cpp
///  \brief This file contains the implementation of a JAUS message.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 28 February 2010
///  <br>Copyright (c) 2010
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
#include "jaus/mobility/list/setelement.h"
#include "jaus/mobility/drivers/setwrencheffort.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
SetElement::SetElement(const Address& dest, const Address& src) : Message(SET_ELEMENT, dest, src)
{
    mRequestID = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetElement::SetElement(const SetElement& message) : Message(SET_ELEMENT)
{
    mRequestID = 0;
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SetElement::~SetElement()
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
int SetElement::WriteMessageBody(Packet& packet) const
{
    int expected = BYTE_SIZE*2;
    int written = 0;

    written += packet.Write(mRequestID);
    written += packet.WriteByte( (Byte)mElements.size() );

    Element::List::const_iterator element;

    for(element = mElements.begin();
        element != mElements.end();
        element++)
    {
        expected += USHORT_SIZE*4 + BYTE_SIZE;
        if(element->mpElement)
        {
            Packet* payload = ((Packet *)(&(element->mPayload)));
            payload->Clear();
            payload->Write(element->mpElement->GetMessageCode());
            element->mpElement->WriteMessageBody(*payload);
        }
        else if(element->mPayload.Size() == 0)
        {
            break;
        }       

        expected += element->mPayload.Size();

        written += packet.Write(element->mID);
        written += packet.Write(element->mPrevID);
        written += packet.Write(element->mNextID);
        // Variable format field
        written += packet.WriteByte(0);                                 // Format field
        written += packet.Write((UShort)(element->mPayload.Size()));    // Count field
        written += packet.Write(element->mPayload);
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
int SetElement::ReadMessageBody(const Packet& packet)
{
    int expected = BYTE_SIZE*2;
    int read = 0;

    Byte count = 0;

    read += packet.Read(mRequestID);
    read += packet.Read(count);

    for(UShort e = 0; e < count; e++)
    {
        expected += USHORT_SIZE*4 + BYTE_SIZE;
        Byte formatField = 0;
        UShort countField = 0;
        Element element;

        read += packet.Read(element.mID);
        read += packet.Read(element.mPrevID);
        read += packet.Read(element.mNextID);
        read += packet.Read(formatField);
        read += packet.Read(countField);

        expected += countField;
        read += packet.Read(element.mPayload, countField);

        mElements.push_back(element);
    }

    return expected == read ? read : -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message contents.
///
////////////////////////////////////////////////////////////////////////////////////
void SetElement::ClearMessageBody()
{
    mRequestID = 0;
    mElements.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if the message will be part of a large data set.
///
////////////////////////////////////////////////////////////////////////////////////
bool SetElement::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    int expected = BYTE_SIZE*2;

    Element::List::const_iterator element;

    for(element = mElements.begin();
        element != mElements.end();
        element++)
    {
        expected += USHORT_SIZE*4;
        if(element->mpElement)
        {
            Packet* payload = ((Packet *)(&(element->mPayload)));
            payload->Clear();
            payload->Write(element->mpElement->GetMessageCode());
            element->mpElement->WriteMessageBody(*payload);            
        }
        else if(element->mPayload.Size() == 0)
        {
            break;
        }       

        expected += element->mPayload.Size();
    }

    return expected > (int)maxPayloadSize ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a test case to validate the message class.
///
///   \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SetElement::RunTestCase() const
{
    int result = 0;

    Packet packet;

    SetElement msg1, msg2;
    SetWrenchEffort msg3, msg4, msg5, msg6;
    msg3.SetPropulsiveLinearEffortX(13.3);
    msg3.SetPropulsiveRotationalEffortZ(3.3);
    msg4.SetPropulsiveLinearEffortX(11.3);
    msg4.SetPropulsiveRotationalEffortZ(2.3);
    Element element1, element2;
    element1.mID = 12;
    element1.mNextID = 13;
    element1.mPrevID = 11;
    element1.mpElement = (Message*) msg3.Clone();
    element2.mID = 13;
    element2.mNextID = 14;
    element2.mPrevID = 12;
    element2.mpElement = (Message*) msg4.Clone();
    msg1.GetElementList()->push_back(element1);
    msg1.GetElementList()->push_back(element2);

    if((msg1.WriteMessageBody(packet) != -1) &&
        (msg2.ReadMessageBody(packet) != -1))
    {
        CxUtils::Packet packet;
        msg2.GetElementList()->front().mPayload.Read(packet,
            msg2.GetElementList()->front().mPayload.Size() - USHORT_SIZE, USHORT_SIZE);
        msg5.ReadMessageBody(packet);
        msg2.GetElementList()->back().mPayload.Read(packet,
            msg2.GetElementList()->back().mPayload.Size() - USHORT_SIZE, USHORT_SIZE);
        msg6.ReadMessageBody(packet);

        if((msg3.GetPropulsiveLinearEffortX() - msg5.GetPropulsiveLinearEffortX() < 0.01) &&
           (msg3.GetPropulsiveRotationalEffortZ() - msg5.GetPropulsiveRotationalEffortZ() < 0.01) &&
           (msg4.GetPropulsiveLinearEffortX() - msg6.GetPropulsiveLinearEffortX() < 0.01) &&
           (msg4.GetPropulsiveRotationalEffortZ() - msg6.GetPropulsiveRotationalEffortZ() < 0.01))
        {
            return true;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SetElement& SetElement::operator=(const SetElement& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mRequestID = message.mRequestID;
        mElements = message.mElements;
    }
    return *this;
}


/*  End of File */
