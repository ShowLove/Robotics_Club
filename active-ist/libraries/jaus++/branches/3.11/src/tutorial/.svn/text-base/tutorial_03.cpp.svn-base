////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_03.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates an alternative method to
///         discover components and receives messages than 
///         tutorial_02.cpp demonstrates.
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

// An altnernative method to get the
// current system information and receive messages
// is to use a callback.  This can be done through
// inheritance of the Discovery::Callback class as
// show below:

// Question:  Modify this program to receive the following 
// messages via Callbacks:
//      ReportConfiguration (Already queried by Discovery)
//      ReportIdentification (Already queried by Discovery)
//      QueryIdentification (Already queried by Discovery)
//      Any others you want to add (remember, you may have to query)

class TutorialCallback : public JAUS::Discovery::Callback
{
public:
    TutorialCallback()
    {
    }
    ~TutorialCallback()
    {
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is called whenver the transport service receives a
    ///          message you have registered the callback for.
    ///
    ///   You must use the transport service to register this callback for specific
    ///   types of messages you want to receive.
    ///
    ///   \param[in] message Pointer to a JAUS message receives.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void ProcessMessage(const JAUS::Message* message)
    {
        if(message->GetMessageCode() == JAUS::REPORT_HEARTBEAT_PULSE)
        {
            const JAUS::ReportHeartbeatPulse* report = 
                dynamic_cast<const JAUS::ReportHeartbeatPulse*>(message);

            std::cout << "\tReceived Message!\n\t";
            report->Print();
        }
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief This method is called whenever the Discovery service recieves
    ///          updated information about a subsystem, node, or component.  Updated
    ///          information includes services, identification, new and lost 
    ///          connections to components.
    ///
    ///   \param[in] system The current known JAUS system information.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void ProcessSystemState(const JAUS::Subsystem::Map& system)
    {
        std::cout << "======================================================\n";

        JAUS::Subsystem::Map::const_iterator subsystem;
        // The map is indexed by the subsystem number.
        for(subsystem = system.begin();
            subsystem != system.end();
            subsystem++)
        {
            std::cout << "Subsystem: " 
                      << subsystem->first 
                      << " Identification: " 
                      << subsystem->second->mIdentification 
                      << std::endl;

            JAUS::Address::List componentsWithLiveness;
            componentsWithLiveness = subsystem->second->GetComponentsWithService(JAUS::Liveness::Name);
            JAUS::Address::List::iterator c;
            for(c = componentsWithLiveness.begin();
                c != componentsWithLiveness.end();
                c++)
            {
                if( (*c) != mpComponent->GetComponentID())
                {
                    // Setup the query message to send.
                    JAUS::QueryHeartbeatPulse query;
                    query.SetDestinationID( (*c) );
                    query.SetSourceID(mpComponent->GetComponentID());

                    std::cout << "\tSending Query to " << c->ToString() << std::endl;
                    mpComponent->Send(&query);
                }
            }
        }
    }

    JAUS::Component* mpComponent;   ///<  Pointer to the component to use.
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

    // Always register callbacks before initialization.

    // Create a callback object, and register it with the
    // two services we want to use (Transport and Discovery).
    TutorialCallback callback;

    callback.mpComponent = &component;
    // Register the callback to be triggered when
    // Report Heartbeat Pulse messages are received.
    component.TransportService()->RegisterCallback(JAUS::REPORT_HEARTBEAT_PULSE,
                                                   &callback);
    // Register with Discovery to be notified of system changes.
    component.DiscoveryService()->RegisterCallback(&callback);

    JAUS::Address componentID(1000, 1, 3);
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
        JAUS::Management* managementService = NULL;
        managementService = component.ManagementService();
        if(managementService->GetStatus() == JAUS::Management::Status::Shutdown)
        {
            // Exit program.
            break;
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
