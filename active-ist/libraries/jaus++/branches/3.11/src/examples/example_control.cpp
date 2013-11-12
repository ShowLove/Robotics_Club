////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_control.cpp
///  \brief This file contains a test/example program for Control transfer and
///         management in JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 December 2009
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

////////////////////////////////////////////////////////////////////////////////////
///
///   \class ControlEventCallback
///   \brief Defines behavior for the loss of control of this service.
///
////////////////////////////////////////////////////////////////////////////////////
class ControlEventCallback : public AccessControl::Callback
{
public:
    ControlEventCallback() {}
    ~ControlEventCallback() {}
    // Method called when we lose control of a component.
    virtual void ProcessLossOfControl(const Address& controlledComponent)
    {
        std::cout << "Lost control of " << controlledComponent.ToString() << std::endl;
    }
    // Method called when when the controller release control of this service.
    virtual void ProcessReleaseOfControl(const Address& controllerID)
    {
        std::cout << "Control by " << controllerID.ToString() << " Released\n";
    }
};

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Entry point of example_control.
///
////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    Component a, b;
    
    ControlEventCallback cb;
    // Register callbacks to be notified when control events happen.
    a.AccessControlService()->RegisterCallback(&cb);
    b.AccessControlService()->RegisterCallback(&cb);

    a.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU,
                                                     "OCU");
    b.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle, 
                                                     "Simulation");
    
    // Initialize a component with a given ID.
    if(a.Initialize(Address(50, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component A.\n";
        return 0;
    }
    // Set the status (or state) of the component.
    a.ManagementService()->SetStatus(Management::Status::Ready);

    // Initialize a component with a given ID.
    if(b.Initialize(Address(50, 1, 2)) == false)
    {
        std::cout << "Failed to Initialize Component B.\n";
        return 0;
    }
    // Set the status (or state) of the component.
    b.ManagementService()->SetStatus(Management::Status::Standby);

    // Enable debugging messages for testing.
    //a.AccessControlService()->EnableDebugMessages(true);    
    //b.AccessControlService()->EnableDebugMessages(true);
    //a.TransportService()->EnableDebugMessages(true);
    //b.TransportService()->EnableDebugMessages(true);
    
    Time::Stamp startTimeMs = 0;
    bool getControl = true;
    double delay = 5000; // Time between attempts in ms.
    while(CxUtils::GetChar() != 27)
    {     
        // Acquire control for delay.
        if(getControl && 
            Time::GetUtcTimeMs() - startTimeMs > delay &&
           !a.AccessControlService()->HaveControl(b.GetComponentID()))
        {
            std::cout << "Requesting Control for " << b.GetComponentID() << " ... ";
            if(a.AccessControlService()->RequestComponentControl(b.GetComponentID()))
            {
                std::cout << "Success!\n";
                a.ManagementService()->Resume(b.GetComponentID());
                getControl = false;
                startTimeMs = Time::GetUtcTimeMs();
            }
            else
            {
                std::cout << "Failure!\n";
            }
        }
        // If we had control for delay, release control and
        // do not re-acquire for delay.
        if(getControl == false && 
            Time::GetUtcTimeMs() - startTimeMs > delay &&
           a.AccessControlService()->HaveControl(b.GetComponentID()))
        {
            std::cout << "Releasing Control.\n";
            a.ManagementService()->Standby(b.GetComponentID());
            if(a.AccessControlService()->ReleaseComponentControl(b.GetComponentID()))
            {
                getControl = true;
                startTimeMs = Time::GetUtcTimeMs();
            }
        }
        std::cout << "\n====================================================\n";
        a.AccessControlService()->PrintStatus(); std::cout << std::endl;
        a.ManagementService()->PrintStatus(true); std::cout << std::endl;
        std::cout << "\n////////////////////////////////////////////////////\n";
        b.AccessControlService()->PrintStatus(); std::cout << std::endl;
        b.ManagementService()->PrintStatus(true); std::cout << std::endl;
        //a.PrintStatus();
        //b.PrintStatus();

        CxUtils::SleepMs(1000);
    }
    // Put the controlled component in standby, then release control.
    if(a.AccessControlService()->HaveControl(b.GetComponentID()))
    {
        a.ManagementService()->Standby(b.GetComponentID());
    }
    // Shutdown the components.
    a.Shutdown(); // Will automatically release control.
    b.Shutdown();

    return 0;
}


/* End of File */
