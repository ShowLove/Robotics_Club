////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_05.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to use the Events service
///         to request events from another component.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 3 July 2010
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
#include <jaus/core/component.h>
#include <cxutils/keyboard.h>
#include <iostream>

// NOTE - Run another JAUS program like tutorial_01
// so that this program can talk to it.

// In this tutorial, a callback is used to show that
// event messages have been received.

// After looking at this tutorial program, try and modify
// it to learn how to get other event subscriptions like
// on change.
//
// Do the following:
// Find a component with the Time Service, and request
// just the Date Stamp information, at 1 Hz, then try
// create an On Change Event for the Time Stamp (it should
// update a lot).

class EventTutorialCallback : public JAUS::Events::Callback
{
public:
    EventTutorialCallback()
    {
    }
    ~EventTutorialCallback()
    {
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Method called when an event related message has been received.
    ///
    ///   \param[in] type Type of event (EveryChange, Periodic).
    ///   \param[in] eventID Event ID number.
    ///   \param[in] sequenceNumber The sequence number for the event (update number).
    ///   \param[in] message The message data associated with the event.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void ProcessEvent(const JAUS::Events::Type type,
                              const JAUS::Byte eventID,
                              const JAUS::Byte sequenceNumber,
                              const JAUS::Message* message)
    {
        if(message->GetMessageCode() == JAUS::REPORT_HEARTBEAT_PULSE)
        {
            const JAUS::ReportHeartbeatPulse* report = 
                dynamic_cast<const JAUS::ReportHeartbeatPulse*>(message);

            std::cout << "\tReceived Event Message [" << (int)sequenceNumber << "]!\n\t";
            report->Print();
        }
    }
};

int main(int argc, char* argv[])
{
    JAUS::Component component;

    // Setup identification info.  For questions about this,
    // see the previous tutorial(s).
    JAUS::Discovery* discoveryService = NULL;
    discoveryService = component.DiscoveryService();
    discoveryService->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                 "Robot");
    discoveryService->SetNodeIdentification("Primary Computer");
    discoveryService->SetComponentIdentification("Baseline");

    // The Events service is used to create and maintain
    // subscriptions to other components that support events.
    // It also acts as an interface that other services within
    // your component inherit from so that they can support
    // generation of events (see Liveness).  In this
    // tutorial example, we will use the Events service
    // to subscribe to Report Heartbeat Pulse messages from
    // another component.
    JAUS::Events* eventsService = NULL;
    eventsService = component.EventsService();

    // Register a callback to be notified when event messages
    // have been received.
    EventTutorialCallback callback;
    eventsService->RegisterCallback(&callback);

    JAUS::Address componentID(1000, 1, 5);
    // Initialize!
    std::cout << "Initializing component...";
    if(component.Initialize(componentID) == false)
    {
        std::cout << "Failed to initialize component [" << componentID.ToString() << "]\n";
        return 0;
    }
    std::cout << "Success!\n";

    // Now go into your main computer loop until the
    // component has been told to shutdown.
    JAUS::Time::Stamp displayStatusTimeMs = JAUS::Time::GetUtcTimeMs();
    while(true)
    {
        // See if we have any subscriptions for the type of
        // data we want (from any source), and if not, create one.
        if(eventsService->HaveSubscription(JAUS::REPORT_HEARTBEAT_PULSE, JAUS::Address()) == false)
        {
            // Find components with the service that supports
            // the type of data we want.
            JAUS::Address::List availableComponents;
            JAUS::Discovery* discoveryService = NULL;
            discoveryService = component.DiscoveryService();
            availableComponents = 
                discoveryService->GetSubsystem(component.GetComponentID())->GetComponentsWithService(JAUS::Liveness::Name);

            JAUS::Address::List::iterator c;
            for(c = availableComponents.begin();
                c != availableComponents.end();
                c++)
            {
                // Don't subscribe to ourselves!
                if((*c) == component.GetComponentID())
                {
                    continue;
                }
                // Create a periodic event.  A periodic event is
                // an event that happens at a fixed rate.  The
                // alternative is an Every Change event, which
                // occurs whenever the data is modified.
                
                // First we must setup the query message for the
                // type of report we want.  Since we want
                // Report Heartbeat Pulse, we need a 
                // Query Heartbeat Pulse message.
                JAUS::QueryHeartbeatPulse query;
                std::cout << "Requesting Periodic Event From " << c->ToString();
                // We are request a rate of 5 Hz, with a minimum acceptable rate
                // of 1 Hz.  If the component we are requesting cannot support
                // at least 1 Hz, the request will be canceled.
                if(eventsService->RequestPeriodicEvent((*c), &query, 5, 1))
                {
                    std::cout << "Success!\n";
                    break;
                    // Now that we have the subscription, you should
                    // see the callback being triggered when the
                    // data arrives at 5Hz!
                }
                else
                {
                    std::cout << "Failure!\n";
                }
            }
        }

        JAUS::Management* managementService = NULL;
        managementService = component.ManagementService();
        if(managementService->GetStatus() == JAUS::Management::Status::Shutdown)
        {
            // Exit program.
            break;
        }

        if(JAUS::Time::GetUtcTimeMs() - displayStatusTimeMs > 500)
        {
            // Print out status of the service.  You may see more
            // events being displayed than just the one we created in
            // this program.  That is because the Discovery Service
            // creates events to other components to be notified when
            // the system configuration changes.  This is part of 
            // the features within JAUS++ to support redundancy.
            eventsService->PrintStatus(); std::cout << std::endl;
            displayStatusTimeMs = JAUS::Time::GetUtcTimeMs();
        }

        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        CxUtils::SleepMs(1);
    }

    // Shutdown your component completely.  Any
    // services added or belonging to the component
    // will be deleted.
    component.Shutdown();

    return 0;
}



/* End of File */
