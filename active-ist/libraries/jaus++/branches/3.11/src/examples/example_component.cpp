////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_component.cpp
///  \brief This file contains a test/example program for Components in
///         JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 18 October 2009
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
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/events/event.h"
#include "jaus/core/discovery/discovery.h"
#include "jaus/core/component.h"
#include <cxutils/keyboard.h>
#include <iostream>
#include <algorithm>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


using namespace JAUS;

void TestJUDP();
void TestLiveness();

Mutex gPrintLock;

class CallbackTest : public Transport::Callback
{
public:
    CallbackTest(){}
    ~CallbackTest() {}
    virtual void ProcessMessage(const Message* message)
    {
        const Event* heartbeat = dynamic_cast<const Event *>(message);
        if(heartbeat)
        {
            Mutex::ScopedLock lock(&gPrintLock);
            //std::cout << Time::GetUtcTime().ToString() << " - Heartbeat Event From " << heartbeat->GetSourceID().ToString() << std::endl;
        }
    }
};

int main(int argc, char* argv[])
{
    Component a, b, c;

    // Force components to use JTCP...

    //a.AddService(new JTCPClient());
    //b.AddService(new JTCPClient());
    //c.AddService(new JTCPClient());

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    if(a.LoadSettings("settings/services.xml") == false ||
       b.LoadSettings("settings/services.xml") == false ||
       c.LoadSettings("settings/services.xml") == false)
    {
        // Working directory probably not set (or not running from output directory), so
        // use default values.
        a.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
            "Simulation");
        b.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
            "Simulation");
        c.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
            "Simulation");
    }

    // Use a callback to know when heartbeat messages were receive
    CallbackTest callback;
    a.TransportService()->RegisterCallback(EVENT, &callback);

    // Initialize a component with any ID.
    std::cout << "Initializing Component A...";
    if(a.Initialize(Address(60001, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component A.\n";
        return 0;
    }
    std::cout << "Success!\n";

    std::cout << "Initializing Component B...";
    // Initialize a component with a given ID.
    if(b.Initialize(Address(60002, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component B.\n";
        return 0;
    }
    std::cout << "Success!\n";

    std::cout << "Initializing Component C...";
    // Initialize a component with a given ID.
    if(c.Initialize(Address(60003, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component C.\n";
        return 0;
    }
    std::cout << "Success!\n";

    // Enable Debug Messages for testing.
    //a.TransportService()->EnableDebugMessages(true);
    //b.TransportService()->EnableDebugMessages(true);
    //a.LivenessService()->EnableDebugMessages(true);
    //a.EventsService()->EnableDebugMessages(true);
    //b.EventsService()->EnableDebugMessages(true);

    //a.TransportService()->EnableLogging(true);
    
    // Allow time for connections to establish before trying
    // to send a query.
    CxUtils::SleepMs(1000);

    // Example inline message query - Get subsystem ID of component B
    QueryIdentification queryIdent(b.GetComponentID(), a.GetComponentID());
    queryIdent.SetQueryType(QueryIdentification::SubsystemIdentification);
    ReportIdentification reportIdent;
    if(a.Send(&queryIdent, &reportIdent, 1000))
    {
        reportIdent.Print();
    }


    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    while(CxUtils::GetChar() != 27)
    {      
        static bool createdEvent = false;
        if(!createdEvent && !a.EventsService()->HaveSubscription(REPORT_HEARTBEAT_PULSE, b.GetComponentID()))
        {
            // Request Heartbeat Events from component b.
            QueryHeartbeatPulse query;
            createdEvent = a.EventsService()->RequestPeriodicEvent(b.GetComponentID(),
                &query,
                10,
                1); 
        }
        gPrintLock.Lock();
        //a.DiscoveryService()->PrintSystemConfiguration();
        //b.DiscoveryService()->PrintSystemConfiguration();
        c.DiscoveryService()->PrintSystemConfiguration();
        gPrintLock.Unlock();

        // Testing Discovery Service.  This is for testing purposes only. 
        // What should happen is that after a timeout period A will no longer
        // see Component b until it is re-initialized.  Upon re-discovery, a should
        // automatically re-create the Report Heartbeat Pulse event.
        static bool didShutdown = false;
        //if(!didShutdown && b.IsInitialized() && Time::GetUtcTimeMs() - startTimeMs > 5000)
        //{
        //    std::cout << "Testing Loss of Component...\n";
        //    b.Shutdown();
        //    didShutdown = true;
        //}

        //if(!b.IsInitialized() && Time::GetUtcTimeMs() - startTimeMs > 15000)
        //{
        //    std::cout << "Testing Discovery of Component...\n";
        //    b.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
        //                                                     "Simulation");
        //    b.Initialize(Address(60000, 1, 2));
        //}

        CxUtils::SleepMs(1000);
    }

    // Shutdown the components.
    a.Shutdown();
    b.Shutdown();

    return 0;
}

/* End of File */
