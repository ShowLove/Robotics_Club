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
    }
    ~UnitTestCallback() {}
    virtual void ProcessMessage(const JAUS::Message* message)
    {
        if(message->GetMessageCode() == JAUS::REPORT_HEARTBEAT_PULSE)
        {
            mCount++;
            if(mCount == 1)
            {
                mStartTimeSeconds = CxUtils::Timer::GetTimeSeconds();
            }
            mFinishTimeSeconds = CxUtils::Timer::GetTimeSeconds();
        }
    }
    unsigned int mCount;
    double mStartTimeSeconds;
    double mFinishTimeSeconds;
};

void Client(int argc, char* argv[]);
void Server(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    std::cout << "Program Usage:\n";
    std::cout << "Client Mode:\n";
    std::cout << argv[0] << " <SERVER IP>\n";
    std::cout << "Server Mode:\n";
    std::cout << argv[0] << "\n";
    if(argc > 1)
    {
        std::cout << "Running Client...\n";
        Client(argc, argv);
    }
    else
    {
        std::cout << "Running Server...\n";
        Server(argc, argv);
    }
    return 0;
}


void Client(int argc, char* argv[])
{
    JAUS::Component a;
    UnitTestCallback callback;

    JAUS::Connection::Info info;

    // Create a fixed connection to a JAUS component.
    info.mTransportType = JAUS::Connection::Transport::JTCP;
    info.mDestIP = std::string(argv[1]);
    a.TransportService()->AddConnection(JAUS::Address(2, 1, 1), info);

    a.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");

    // Disable all services except basic send/receive
    // for this test.
    a.AccessControlService()->EnableService(false);
    a.DiscoveryService()->EnableService(false);
    a.EventsService()->EnableService(false);
    a.ManagementService()->EnableService(false);
    a.GetTimeService()->EnableService(false);

    a.TransportService()->RegisterCallback(JAUS::REPORT_HEARTBEAT_PULSE, &callback);

    a.Initialize(JAUS::Address(1, 1, 1));

    unsigned int count = 0;
    unsigned int limit = 2000;

    std::cout << "Testing Query/Response Average Speed...\n";

    JAUS::QueryHeartbeatPulse query(JAUS::Address(2, 1, 1), a.GetComponentID());
    while(CxUtils::GetChar() != 27 && callback.mCount < limit)
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
        CxUtils::SleepMs(1);
    }

    double totalTime = (callback.mFinishTimeSeconds - callback.mStartTimeSeconds)/limit;
    std::cout << "Average Query/Response Time: " << totalTime << std::endl;
}


void Server(int argc, char* argv[])
{
    JAUS::Component b;

    b.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");

    // Disable all services except basic send/receive
    // for this test.
    b.DiscoveryService()->EnableService(false);
    b.ManagementService()->EnableService(false);
    b.EventsService()->EnableService(false);
    b.GetTimeService()->EnableService(false);

    b.Initialize(JAUS::Address(2, 1, 1));

    unsigned int count = 0;
    unsigned int limit = 2000;

    std::cout << "Unit Test Hearbeat Server Running...\n";

    while(CxUtils::GetChar() != 27)
    {
        CxUtils::SleepMs(1);
    }

}


/* End of File */
