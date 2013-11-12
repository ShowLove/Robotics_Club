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


int main(int argc, char* argv[])
{
    JAUS::Component a, b;
    JAUS::NodeManager nodeManager;
    // Try run a node manager in case none is present.  It
    // is better to have one not running within the components.
    nodeManager.SetConnectionsPerThread(1);
    nodeManager.Initialize();

    a.TransportService()->GetNodeManager()->SetConnectionsPerThread(1);
    a.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");
    b.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "UNIT_TEST");

    // Enable/Disable specific services for test
    a.AccessControlService()->EnableService(true);
    a.DiscoveryService()->EnableService(true);
    a.EventsService()->EnableService(true);
    a.ManagementService()->EnableService(true);
    a.GetTimeService()->EnableService(true);

    b.DiscoveryService()->EnableService(true);
    b.ManagementService()->EnableService(true);
    b.EventsService()->EnableService(true);
    b.GetTimeService()->EnableService(true);

    
    a.Initialize(JAUS::Address(1, 1, 1));
    b.Initialize(JAUS::Address(2, 1, 1));

    unsigned int count = 0;
    unsigned int limit = 5000;

    std::cout << "Testing Query/Response Average Speed...\n";

    JAUS::QueryHeartbeatPulse query(b.GetComponentID(), a.GetComponentID());
    JAUS::ReportHeartbeatPulse report;
    unsigned int loop = 0;

    double startTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    while(CxUtils::GetChar() != 27 && loop < limit)
    {
        // Wait indefinitely
        if(a.Send(&query, &report, 0))
        {
            count++;
        }
        else
        {
            std::cout << "SEND FAILURE!\n";
        }
        loop++;
        //CxUtils::SleepMs(1);
    }
    double endTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    double totalTime = (endTimeSeconds - startTimeSeconds)/count;
    std::cout << "Average Query/Response Receipt Time: " << totalTime << std::endl;
    std::cout << "Total Failed Attempts: " << limit - count << std::endl;

    while(CxUtils::GetChar() != 27)
    {
        CxUtils::SleepMs(1);
    }

    return 0;
}


/* End of File */
