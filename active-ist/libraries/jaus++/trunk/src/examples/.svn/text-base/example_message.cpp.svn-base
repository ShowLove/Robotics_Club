////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_message.cpp
///  \brief This file contains a test/example program for the Message class
///         to verify ability to serialize/deserialize message data to a 
///         JAUS compliant format.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 26 September 2009
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
#include "jaus/core/transport/largedataset.h"
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/discovery/discovery.h"
#include "jaus/core/control/accesscontrol.h"
#include "jaus/core/management/management.h"
#include "jaus/core/events/events.h"
#include "jaus/core/events/cancelevent.h"
#include "jaus/core/events/confirmeventrequest.h"
#include "jaus/core/events/createevent.h"
#include "jaus/core/events/event.h"
#include "jaus/core/events/queryevents.h"
#include "jaus/core/events/rejecteventrequest.h"
#include "jaus/core/events/reportevents.h"
#include "jaus/core/events/updateevent.h"
#include "jaus/mobility/list/listmanager.h"
#include "jaus/mobility/sensors/localposesensor.h"
#include "jaus/mobility/sensors/velocitystatesensor.h"
#include "jaus/mobility/drivers/localwaypointlistdriver.h"
#include "jaus/core/component.h"
#include <cxutils/keyboard.h>
#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>

using namespace std;
using namespace JAUS;

void VerifyAgainstLog();
void TestMessages();
void TestLargeDataSets();

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Entry point of example_message.
///
////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    VerifyAgainstLog();
    //TestMessages();
    //TestLargeDataSets();

    return 0;
}


class TestLocalWaypointListDriver : public LocalWaypointListDriver
{
public:
    TestLocalWaypointListDriver() {}
    ~TestLocalWaypointListDriver() {}
    virtual bool IsWaypointAchieved(const LocalPose& currentPose,
                                    const JAUS::SetLocalWaypoint& desiredWaypoint) const { return false; }
    virtual void WaypointAchieved(const JAUS::SetLocalWaypoint& waypoint) { return ; }
};

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Test method to verify ability to read/write message data from
///   an example wireshark log provided for testing systems for the JAUS
///   Interoperability Challenge hosted at several AUVSI competitions.
///
////////////////////////////////////////////////////////////////////////////////////
void VerifyAgainstLog()
{
    Packet::List packets;
    Packet::List::iterator packet;

    Component component;
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU, "OCP");
    component.AddService(new LocalPoseSensor());
    component.AddService(new VelocityStateSensor());
    component.AddService(new ListManager());    
    component.AddService(new TestLocalWaypointListDriver());

    Packet::LoadWiresharkCapturePacketExport("logs/example-capture.txt", packets);
    Address cop(42, 1, 1);
    Address sim(6000, 1, 1);
    //Packet::LoadWiresharkCapturePacketExport("logs/example_capture-2.txt", packets);

    Message::List messages;
    Message::List::iterator message;
    
    // Delete UDP header data to get to just the JAUS message.
    for(packet = packets.begin(); packet != packets.end();)
    {
        // Delete UDP header data.
        packet->Delete(43);
        // Read message header data.
        Header jausHeader;
        if(jausHeader.Read(*packet))
        {
            UShort messageCode = 0;
            packet->Read(messageCode, Header::PayloadOffset);
            Message* jausMessage = component.TransportService()->CreateMessage(messageCode);
            if(jausMessage)
            {
                packet->SetReadPos(0);
                if(jausMessage->Read(*packet))
                {
                    messages.push_back(jausMessage);
                    if(jausMessage->GetMessageCode() == SET_ELEMENT)
                    {
                        Element::List::iterator m;
                        SetElement* setElement = (SetElement *)jausMessage;
                        for(m = setElement->GetElementList()->begin();
                            m != setElement->GetElementList()->end();
                            m++)
                        {
                            UShort code = 0;
                            m->mPayload.SetReadPos(0);
                            m->mPayload.Read(code);
                            Message* element = component.TransportService()->CreateMessage(code);
                            if(element)
                            {
                                element->CopyHeaderData(jausMessage);
                                if(element->ReadMessageBody(m->mPayload))
                                {
                                    messages.push_back(element);
                                }
                                else
                                {
                                    std::cout << "Failed to Read Message Data [" << std::hex << element->GetMessageCode() << "]:\n";
                                    delete element;
                                }
                            }
                        }
                    }
                }
                else
                {
                    std::cout << "Failed to Read Message Data [" << std::hex << jausMessage->GetMessageCode() << "]:\n";
                }
            }
            else
            {
                std::cout << "Unknown Message Type [" << std::hex << messageCode << "]:\n";
            }
        }
        else
        {
            std::cout << "Bad Header!\n";
        }
        packet++;
    }

    component.Initialize(cop);

    message = messages.begin();
    for(message = messages.begin(); 
        message != messages.end(); 
        message++)
    {
        //(*message)->Print();
        if((*message)->GetSourceID() == cop)
        {
            (*message)->SetDestinationID(sim);
            component.Send((*message));
        }
        CxUtils::SleepMs(1);
        delete (*message);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calls RunTestCase for each message in testMessages.
///
////////////////////////////////////////////////////////////////////////////////////
void TestMessages()
{
    // Test Messages.
    std::vector<Message*> testMessages;
    
    // Message(s) being tested.
    testMessages.push_back(new ConfirmElementRequest());
    testMessages.push_back(new DeleteElement());
    testMessages.push_back(new ExecuteList());
    testMessages.push_back(new QueryActiveElement());
    testMessages.push_back(new QueryElement());
    testMessages.push_back(new QueryElementCount());
    testMessages.push_back(new QueryElementList());
    testMessages.push_back(new RejectElementRequest());
    testMessages.push_back(new ReportActiveElement());
    testMessages.push_back(new ReportElement());
    testMessages.push_back(new ReportElementCount());
    testMessages.push_back(new ReportElementList());
    testMessages.push_back(new SetElement());

    int success = 0;

    for(std::vector<Message*>::iterator ii = testMessages.begin(); ii < testMessages.end(); ++ii)
    {
        success += (*ii)->RunTestCase();
    }

    std::cout << success << " out of " << testMessages.size() << " Messages tested successful.\n";

    for(std::vector<Message*>::iterator ii = testMessages.begin(); ii < testMessages.end(); ++ii)
    {
        delete (*ii);
    }
    testMessages.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Tests large data set creation and merging.
///
////////////////////////////////////////////////////////////////////////////////////
void TestLargeDataSets()
{
    Packet payload;                         // Large data set payload.
    Header header;                          // Message header data.
    UShort messageCode = 50;                // Random message type.
    Packet::List stream;                    // Multi-packet stream sequence.
    Header::List streamHeaders;             // Mutli-packet stream sequence headers.
    unsigned int payloadSize = 60000;       // Payload data size to create.
    
    header.mSourceID(50, 1, 1);
    header.mDestinationID(51, 1, 1);
    for(unsigned int i = 0; i < payloadSize/UINT_SIZE; i++)
    {
        payload.Write(i);
    }
    
    LargeDataSet::CreateLargeDataSet(header, messageCode, payload, stream, streamHeaders, NULL, 1437, 30);
    payload.Destroy();
    messageCode = 0;
    header = Header();
    LargeDataSet::MergeLargeDataSet(header, messageCode, payload, stream, NULL);
    
    unsigned int data;
    for(unsigned int i = 0; i < payloadSize/UINT_SIZE; i++)
    {
        payload.Read(data);
        if(data != i)
        {
            std::cout << "Large Data Sets Error: Data Does Not Match!\n";
            return;
        }
    }

    std::random_shuffle(stream.begin(), stream.end());

    LargeDataSet dataSet;
    for(unsigned int i = 0; i < (unsigned int)stream.size(); i++)
    {
        Header header;
        UShort messageCode;
        stream[i].SetReadPos(0);
        header.Read(stream[i]);
        stream[i].Read(messageCode);
        if(dataSet.AddPacket(header, messageCode, stream[i]) == false)
        {
            std::cout << "Large Data Sets Error: Could Not Collect Stream.\n";
        }
    }
    // Now merge the data.
    LargeDataSet::MergeLargeDataSet(header, messageCode, payload, dataSet.mStream, NULL);
    for(unsigned int i = 0; i < payloadSize/UINT_SIZE; i++)
    {
        payload.Read(data);
        if(data != i)
        {
            std::cout << "Large Data Sets Error: Data Does Not Match!\n";
            return;
        }
    }
}


/* End of File */
