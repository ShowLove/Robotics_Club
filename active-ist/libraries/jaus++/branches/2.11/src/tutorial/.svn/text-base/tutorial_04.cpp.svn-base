////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_04.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to use the Access
///         Control Service.  Access Control is used to take control
///         of another component for exclusive operations.  Finally, this
///         example also shows how to change the state of a 
///         controlled component.
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

    // The Access Control service manages what other components
    // are allowed to control this component, and what
    // components our component is in control of. Taking control
    // of a component allows for exclusive access to its services,
    // blocking any other commands from other services (except for
    // event commands).  If we take control of a component, we
    // will have it until we release control, or another component
    // with higher authority takes it from us.

    // Let's change options for the Access Control service of our 
    // component.  In this tutorial I am saving a pointer
    // to the Access Control service, just like in the previous
    // tutorials for Discovery and Management.  This is not
    // necessary, and is only done to show users that you can
    // do this if you want, but you can just as easily keep
    // calling component.AccessControlService() method to get the
    // pointer and call the related methods if you want.
    JAUS::AccessControl* accessControlService = NULL;
    accessControlService = component.AccessControlService();

    // Use the SetAuthorityCode method to set the authority level
    // of our component.  We must have equal too or greater
    // than authority than the component we are trying to
    // control.  This is also a way to prevent low authority
    // level components from taking control of us!  Authority
    // values range from 0-255, with 255 being the highest.
    accessControlService->SetAuthorityCode(100);
    // The SetTimeoutPeriod method sets how often any controlling
    // component must attempt to reacquire control of our
    // component before we timeout control.  For example
    // if in this example, if a controller doesn't try to
    // reacquire control within 1 second, they will lose
    // control.
    accessControlService->SetTimeoutPeriod(1);

    JAUS::Address componentID(1000, 1, 4);
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
    JAUS::Time::Stamp acquiredControlTimeMs = 0;
    JAUS::Time::Stamp releaseControlTimeMs = 0;
    while(true)
    {
        JAUS::Management* managementService = NULL;
        managementService = component.ManagementService();
        if(managementService->GetStatus() == JAUS::Management::Status::Shutdown)
        {
            // Exit program.
            break;
        }

        // In this tutorial, the following code shows how to take
        // and release control of a component.  In this example
        // we take control for 3 seconds, and then release control
        // for 3 seconds, then repeat.

        // Let's see if we are in control of any components.
        JAUS::Address::List controlledComponents;
        controlledComponents = accessControlService->GetControlledComponents();
        if(controlledComponents.size() == 0 && 
           JAUS::Time::GetUtcTimeMs() - releaseControlTimeMs > 3000)
        {
            // Find a component to take control of on our subsystem.
            JAUS::Address::List availableComponents;
            JAUS::Discovery* discoveryService = NULL;
            discoveryService = component.DiscoveryService();
            availableComponents = 
                discoveryService->GetSubsystem(component.GetComponentID())->GetComponentsWithService(JAUS::AccessControl::Name);

            JAUS::Address::List::iterator c;
            for(c = availableComponents.begin();
                c != availableComponents.end();
                c++)
            {
                // Don't take control of ourself!
                if( *c == component.GetComponentID() )
                {
                    continue;
                }
                std::cout << "Requesting control of " << c->ToString() << std::endl;
                if(accessControlService->RequestComponentControl( (*c) ) == true)
                {
                    std::cout << "Success!\n";
                    acquiredControlTimeMs = JAUS::Time::GetUtcTimeMs();
                    
                    // Now that we have control, lets send a command to
                    // change the controlled component to the "Ready"
                    // state.
                    component.ManagementService()->Resume((*c));

                    break;
                }
                else
                {
                    std::cout << "Failure!\n";
                }
            }
        }
        // In this tutorial, this part is only here to show how to
        // release control of a component.  In this case, we will release
        // control after 3 seconds.
        else if(JAUS::Time::GetUtcTimeMs() - acquiredControlTimeMs > 3000)
        {
            JAUS::Address::List::iterator c;
            for(c = controlledComponents.begin();
                c != controlledComponents.end();
                c++)
            {
                // Before releasing control, we should always put the
                // component back in a standby state.
                if(component.ManagementService()->GetComponentStatus( (*c) ) !=
                    JAUS::Management::Status::Standby)
                {
                    // Sends a standby command.
                    component.ManagementService()->Standby( (*c) );
                }
                std::cout << "Releasing control of " << c->ToString() << std::endl;
                if(accessControlService->ReleaseComponentControl( (*c) ) == true)
                {
                    std::cout << "Success!\n";
                    releaseControlTimeMs = JAUS::Time::GetUtcTimeMs();
                    break;
                }
                else
                {
                    std::cout << "Failure!\n";
                }
            }
        }

        if(JAUS::Time::GetUtcTimeMs() - displayStatusTimeMs > 500)
        {
            // Print out status of control service.  This will show
            // who we are controlling.
            accessControlService->PrintStatus(); std::cout << std::endl;
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
