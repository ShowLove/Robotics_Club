////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_02_a.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to find what components
///         the Discovery service has found, and how to send/receive
///         messages.  This program is a follow on to tutorial_02.cpp
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 February 2011
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
#include <jaus/core/component.h>
#include <jaus/core/time/querytime.h>
#include <jaus/core/time/reporttime.h>
#include <cxutils/keyboard.h>
#include <iostream>

// NOTE - Run another JAUS program like tutorial_01
// so that this program can talk to it.

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

    JAUS::Address componentID(1000, 1, 2);
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

        if(JAUS::Time::GetUtcTimeMs() - displayStatusTimeMs > 500)
        {
            // Use the discovery service to get a list of 
            // discovered subsystems. (see below for how to clear map).
            JAUS::Subsystem::Map discoveredSubsystems;
            discoveryService->GetSubsystems(discoveredSubsystems);
            std::cout << "======================================================\n";

            JAUS::Subsystem::Map::iterator subsystem;
            // The map is indexed by the subsystem number.
            for(subsystem = discoveredSubsystems.begin();
                subsystem != discoveredSubsystems.end();
                subsystem++)
            {
                std::cout << "Subsystem: " 
                          << subsystem->first 
                          << " Identification: " 
                          << subsystem->second->mIdentification 
                          << std::endl;

                // Lets see if it has specific service we 
                // want to communicate with.  For this tutorial
                // let's check for the Time service, which
                // supports the Query Time and 
                // Report Time messages.
                // We can use the subsystem data structure to
                // get a list of components on the subsystem that
                // have the service, so we can send a message to it.
                JAUS::Address::List componentsWithTime;
                componentsWithTime = subsystem->second->GetComponentsWithService(JAUS::TimeService::Name);
                JAUS::Address::List::iterator c;
                for(c = componentsWithTime.begin();
                    c != componentsWithTime.end();
                    c++)
                {
                    // First, make sure it is not the 
                    // component we are using, because we
                    // want to talk to a different one.
                    if( (*c) != component.GetComponentID())
                    {
                        // Now that we have the ID of
                        // component with the Time
                        // service, lets send a query message
                        // and wait for the response.

                        // Setup the query message to send.
                        JAUS::QueryTime query;
                        query.SetDestinationID( (*c) );
                        query.SetSourceID(component.GetComponentID());
                        // Request time and date data, by modifying the
                        // presence vector field to request them.
                        query.SetPresenceVector(JAUS::QueryTime::PresenceVector::Time | JAUS::QueryTime::PresenceVector::Date);
                        
                        // This is the response message we want.
                        JAUS::ReportTime response;

                        // Send and see if we got a
                        // response, but only wait up to 1 second
                        // for a response.  Default value for waiting
                        // is 100 ms.
                        std::cout << "\tSending Query to " << c->ToString() << std::endl;
                        if(component.Send(&query, &response, 1000))
                        {
                            std::cout << "\tReceived Response Message!\n\t";
                            response.Print();
                            if(response.IsFieldPresent(JAUS::ReportTime::PresenceVector::Time))
                            {
                                std::cout << "\tTime: " << response.GetTimeStamp().ToString() << std::endl;
                            }
                            if(response.IsFieldPresent(JAUS::ReportTime::PresenceVector::Date))
                            {
                                std::cout << "\tDate: " 
                                          << response.GetDateStamp().mYear 
                                          << ", " << response.GetDateStamp().mMonth 
                                          << ", " << response.GetDateStamp().mDay << std::endl;
                            }
                        }
                    }
                }
                // The above steps can be used to find any component
                // with a specific service you wish to communicate with.
            }

            // Make sure you delete the subsystem map when
            // you are done with it, otherwise you will have a 
            // memory leak.
            JAUS::Subsystem::DeleteSubsystemMap(discoveredSubsystems);

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
