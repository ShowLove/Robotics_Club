////////////////////////////////////////////////////////////////////////////////////
///
///  \file reportservices.cpp
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
#include "jaus/core/discovery/reportservices.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
///   \param[in] src Source ID of message sender.
///   \param[in] dest Destination ID of message.
///
////////////////////////////////////////////////////////////////////////////////////
ReportServices::ReportServices(const Address& dest,
                               const Address& src) : Message(REPORT_SERVICES, dest, src)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportServices::ReportServices(const ReportServices& message) : Message(REPORT_SERVICES)
{
    *this = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ReportServices::~ReportServices()
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
int ReportServices::WriteMessageBody(Packet& packet) const
{
    int total = 0;
    total += packet.WriteByte((Byte)mServices.size());
    
    Services::const_iterator node;
    for(node = mServices.begin();
        node != mServices.end();
        node++)
    {
        total += packet.Write(node->first);
        total += packet.WriteByte((Byte)node->second.size());

        Record::List::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            total += packet.Write(component->mComponent);
            total += packet.Write(component->mInstance);
            total += packet.WriteByte((Byte)component->mServices.size());

            Service::ID::Set::const_iterator s;
            for(s = component->mServices.begin();
                s != component->mServices.end();
                s++)
            {
                total += s->Write(packet);
            }
        }
    }

    return total;
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
int ReportServices::ReadMessageBody(const Packet& packet) 
{
    int total = 0;
    Byte count1 = 0;

    total += packet.Read(count1);
    for(Byte i = 0; i < count1; i++)
    {
        Byte node = 0;
        Byte count2 = 0;
        Record::List components;

        total += packet.Read(node);
        total += packet.Read(count2);
        for(Byte j = 0; j < count2; j++)
        {
            Byte count3 = 0;
            Record record;
               
            total += packet.Read(record.mComponent);
            total += packet.Read(record.mInstance);
            total += packet.Read(count3);

            for(Byte k = 0; k < count3; k++)
            {
                Service::ID id;
                total += id.Read(packet);
                record.mServices.insert(id);
            }
            components.push_back(record);
        }
        mServices[node] = components;
    }

    return total;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportServices::ClearMessageBody()
{
    mServices.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints message payload data.
///
////////////////////////////////////////////////////////////////////////////////////
void ReportServices::PrintMessageBody() const
{
    Services::const_iterator node;
    for(node = mServices.begin();
        node != mServices.end();
        node++)
    {
        Record::List::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            Service::ID::Set::const_iterator s;
            for(s = component->mServices.begin();
                s != component->mServices.end();
                s++)
            {
                std::cout << s->ToString() << std::endl;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the contents of the message will be larger than
///           maximum payload size, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool ReportServices::IsLargeDataSet(const unsigned int maxPayloadSize) const
{
    unsigned int size = BYTE_SIZE;
    Services::const_iterator node;
    for(node = mServices.begin();
        node != mServices.end();
        node++)
    {
        size += BYTE_SIZE*2;
        Record::List::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            size += BYTE_SIZE*3;
            Service::ID::Set::const_iterator s;
            for(s = component->mServices.begin();
                s != component->mServices.end();
                s++)
            {
                size += (unsigned int)(BYTE_SIZE + s->mName.size() + BYTE_SIZE*2);
            }
        }
    }
    return size > maxPayloadSize;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
ReportServices& ReportServices::operator =(const ReportServices& message)
{
    if(this != &message)
    {
        CopyHeaderData(&message);
        mServices = message.mServices;
    }
    return *this;
}


/*  End of File */
