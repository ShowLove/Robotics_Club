////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_01.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to create and initialize
///         a JAUS component with the Core Service Set.
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


int main(int argc, char* argv[])
{
    // Create a component.  By default a component
    // has all the services of the Core Service set:
    // - Transport (JUDP)
    // - Control
    // - Discovery
    // - Events
    // - Liveness
    // - Time
    // - Management
    JAUS::Component component;

    // The Transport service is used to send
    // and receive messages to other JAUS components.  All
    // other services use the Transport service.  The
    // default transport type for JAUS++ is UDP communication
    // using the JUDP class.

    // The Discovery service is used to find
    // other JAUS components and services on the 
    // network using the Transport service.  In JAUS++
    // Discovery will automatically find these components,
    // make connections to them, and keep track of what
    // services they have.
    
    //  The first thing we must do for a component is
    //  configure its identification.  This is done by
    //  using the Discovery Service.  Get a pointer
    //  to the service:
    JAUS::Discovery* discoveryService = NULL;
    discoveryService = (JAUS::Discovery*)component.GetService(JAUS::Discovery::Name);
    //  Alternative method:
    //  discoveryService = component.DiscoveryService();

    // Set the type of subsystem the component is for.  Subsystem
    // types available are currently Vehicle, or OCU.  The string
    // name "Robot" represents the type or category of platform.
    // You must set the subsystem identification before you will be
    // able to initialize your component.
    discoveryService->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                 "Robot");
    // You can also set identification information for the component
    // and node that it is on.
    discoveryService->SetNodeIdentification("Primary Computer");
    discoveryService->SetComponentIdentification("Baseline");

    // Now that we have setup our identification information we
    // can initialize our component.  First, create the
    // component ID.
    JAUS::Address componentID(1000, 1, 1);
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
        // Let's check the "state" of our component. This
        // is done using the Management service.  
        // A component can be in the following states:
        // - Initialized
        // - Ready
        // - Standby
        // - Shutdown
        // - Failure
        // - Emergency
        JAUS::Management* managementService = NULL;
        managementService = (JAUS::Management*)component.GetService(JAUS::Management::Name);
        // Alternative method:
        //managementService = component.ManagementService();
        if(managementService->GetStatus() == JAUS::Management::Status::Shutdown)
        {
            // Exit program.
            break;
        }
        if(JAUS::Time::GetUtcTimeMs() - displayStatusTimeMs > 500)
        {
            std::cout << "======================================================\n";
            // Print status of the service.
            managementService->PrintStatus(); std::cout << std::endl;

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
