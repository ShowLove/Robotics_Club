////////////////////////////////////////////////////////////////////////////////////
///
///  \file transport_services.cpp
///  \brief This file is a unit test program to verify communication over 
///          various JAUS transport layers.
///
///  <br>Author(s): Daniel Barber
///  <br>Copyright (c) 2011
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
#include "jaus/core/component.h"
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/discovery/discovery.h"
#include "jaus/core/transport/nodemanager.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>


#ifdef VLD_ENABLED
#include <vld.h>
#endif

class UnitTestCallback : public JAUS::Transport::Callback
{
public:
    UnitTestCallback() 
    {
        mCount = 0;
        mStartTimeSeconds = 0;
        mFinishTimeSeconds = 0;
        mMessageCode = JAUS::REPORT_SERVICES;
    }
    ~UnitTestCallback() {}
    virtual void ProcessMessage(const JAUS::Message* message)
    {
        if(message->GetMessageCode() == mMessageCode)
        {
            mCount++;
            if(mCount == 1)
            {
                mStartTimeSeconds = CxUtils::Timer::GetTimeSeconds();
            }
            mFinishTimeSeconds = CxUtils::Timer::GetTimeSeconds();
            /*
            const JAUS::ReportServices * services = dynamic_cast<const JAUS::ReportServices*>(message);
            if(services)
            {
                std::cout << cout << "Services\n";
                services->PrintMessageBody();
            }
            */
        }
    }
    volatile unsigned int mCount;
    double mStartTimeSeconds;
    double mFinishTimeSeconds;
    JAUS::UShort mMessageCode;
};

void SharedMemoryTest();

int main(int argc, char* argv[])
{
    JAUS::Component a, b;
    JAUS::NodeManager nodeManager;
    UnitTestCallback callbackA, callbackB;

    // Try run a node manager in case none is present.  It
    // is better to have one not running within the components.
    nodeManager.Initialize();

    a.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");
    b.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");
    b.TransportService()->AddMessageTemplate(new JAUS::ReportHeartbeatPulse());

    // Disable all services except basic send/receive 
    // for this test.
    a.AccessControlService()->EnableService(false);
    a.DiscoveryService()->EnableService(true);
    a.EventsService()->EnableService(false);
    a.ManagementService()->EnableService(false);
    a.GetTimeService()->EnableService(false);
    a.LivenessService()->EnableService(false);

    b.AccessControlService()->EnableService(false);
    b.DiscoveryService()->EnableService(true);
    b.ManagementService()->EnableService(false);
    b.EventsService()->EnableService(false);
    b.GetTimeService()->EnableService(false);
    b.LivenessService()->EnableService(false);

    callbackA.mMessageCode = JAUS::REPORT_SERVICES;
    callbackB.mMessageCode = JAUS::QUERY_SERVICES;

    a.TransportService()->RegisterCallback(JAUS::REPORT_SERVICES, &callbackA);
    b.TransportService()->RegisterCallback(JAUS::QUERY_SERVICES, &callbackB);

    a.Initialize(JAUS::Address(1, 1, 1));
    b.Initialize(JAUS::Address(2, 1, 1));

    unsigned int count = 0;
    unsigned int limit = 500;

    std::cout << "Testing Query/Response Average Speed...\n";

    JAUS::QueryServices query(b.GetComponentID(), a.GetComponentID());

    double startTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    bool firstA = true;
    bool firstB = true;
    double sendFinishTimeSeconds = 0;
    while(CxUtils::GetChar() != 27 && callbackA.mCount < limit)
    {
        if(count < limit)
        {
            if(a.Send(&query))
            {
                count++;
            }
            else
            {
                std::cout << "SEND FAILURE!\n";
            }
        }
        else if (firstA)
        {
            sendFinishTimeSeconds =  CxUtils::Timer::GetTimeSeconds();
            double totalSendTime = sendFinishTimeSeconds - startTimeSeconds;
            double averageSendTime = totalSendTime/limit;
            std::cout << "Sending Query Complete\n";
            std::cout << "Total time to send " << limit << ": " << totalSendTime << " Average: " << averageSendTime << std::endl;
            firstA = false;
        }
        if(firstB && callbackB.mCount == limit)
        {
            sendFinishTimeSeconds =  CxUtils::Timer::GetTimeSeconds();
            double totalSendTime = sendFinishTimeSeconds - startTimeSeconds;
            double averageSendTime = totalSendTime/limit;
            std::cout << "Receiving Query Complete\n";
            std::cout << "Total time to get queries " << limit << ": " << totalSendTime << " Average: " << averageSendTime << std::endl;
            firstB = false;
        }
        CxUtils::SleepMs(1);
    }

    double endTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    double totalTime = (endTimeSeconds - startTimeSeconds)/count;
    std::cout << "Average Query/Response Time 1: " << totalTime << std::endl;
    totalTime = (callbackA.mFinishTimeSeconds - callbackA.mStartTimeSeconds)/limit;
    std::cout << "Average Query/Response Time 2: " << totalTime << std::endl;

    while(CxUtils::GetChar() != 27)
    {
        CxUtils::SleepMs(1);
    }

    return 0;
}

#include "jaus/core/transport/sharedmemory.h"

class CB : public JAUS::Connection::Callback
{
public:
    CB() : mCount(0) {}
    ~CB() {}
    virtual void ProcessPacket(const JAUS::Packet& jausPacket,
                               const JAUS::Header& jausHeader,
                               const JAUS::Connection* connection,
                               const JAUS::Connection::Info* sourceInfo)
    {
        mCount++;
        //std::cout << "Count: " << mCount << "\n";
    }
    volatile int mCount;
};

void SharedMemoryTest()
{
    JAUS::SharedMemory server(false);
    JAUS::SharedMemory client(false);

    CB cb;

    server.RegisterCallback(&cb);

    JAUS::SharedMemory::Parameters params;

    params.mComponentID(1, 1, 1);
    params.mSharedConnectionType = JAUS::SharedMemory::Parameters::Host;

    server.Initialize(&params);

    JAUS::ReportHeartbeatPulse msg(JAUS::Address(1, 1, 1), JAUS::Address(1, 2, 1));
    JAUS::Packet packet;
    JAUS::Header header;
    msg.Write(packet, header, &server.GetTransportHeader(), true);

    params.mSharedConnectionType = JAUS::SharedMemory::Parameters::Client;
    params.mComponentID = header.mDestinationID;
    client.Initialize(&params);

    std::cout << "Start...\n";
    double startTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    unsigned int count = 1000;
    for(unsigned int i = 0; i < count; i++)
    {
        if(client.SendPacket(packet, header) == false)
        {
            std::cout << "Send Failure!\n";
        }
    }
    
    //std::cout << "Send Complete...\n";
    while(count  != cb.mCount)
    {
        //std::cout << "Count: " << cb.mCount << "\n";
    }
    
    double endTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    double totalTime = (endTimeSeconds - startTimeSeconds)/count;
    std::cout << "Average Send: " << totalTime << std::endl;
}


/* End of File */
