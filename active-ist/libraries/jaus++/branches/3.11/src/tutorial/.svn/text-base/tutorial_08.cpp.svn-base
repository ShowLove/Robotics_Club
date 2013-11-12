////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_07.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to find a component with the
///         Primitive Driver service, and send it a wrench (i.e. drive) 
///         command.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 24 Jan 2012
///  <br>Copyright (c) 2012
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
#include <jaus/mobility/drivers/primitivedriver.h>
#include <cxutils/keyboard.h>
#include <iostream>

// NOTE - Run another JAUS program that has a primitive driver like vehicle_sim to
// talk to!


int main(int argc, char* argv[])
{
    JAUS::Component component;

    // Add our own services to the component.
    // By default the Component will already have
    // the Core Service Set (e.g. Events, Discovery, AccessControl).
    // as needed

    // Setup identification (types are Subsystem::Vehicle or Subsystem::OCU).
    // string name is whatever your platform/ocu identification is (e.g. XUV, AUV, OCU).
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::OCU,
                                                             "OCU");

    // Initialize component.
    if(component.Initialize(JAUS::Address(1000, 1, 7)) == false)
    {
        return 0;
    }

    std::cout << "Component Initialized!\n";
    bool finished = false;
    while(!finished)
    {
        // Find robots with primitive driver service.
        JAUS::Address::List componentsWithPrimitiveDrivers;
        
        componentsWithPrimitiveDrivers = component.DiscoveryService()->GetComponentsWithService(JAUS::PrimitiveDriver::Name);

        if(componentsWithPrimitiveDrivers.size() > 0)
        {
            // For this tutorial, just take the first ID found, change
            // to what you need for your project.
            JAUS::Address primitiveDriverID = componentsWithPrimitiveDrivers[0];
            // Before we can send a command, we must be in control of the
            // component, and it must be in a ready state for Wrench Effort commands.
            while(!finished)
            {
                // I'm only saving pointers here to reduce typing, you can
                // just keep calling the function from component if you want in
                // your own applications.
                JAUS::AccessControl* controlService = component.AccessControlService();
                JAUS::Management* managementService = component.ManagementService();

                // I'm setting a higher authority code than 0 here so I
                // can override any other program on the network that
                // wants to control.  My programs authority must be
                // greater than or equal to that of the robots to take
                // control (default is 0).
                controlService->SetAuthorityCode(200);

                bool areWeReady = false;

                // If we do not have control, request it.
                if(controlService->HaveControl(primitiveDriverID) == false)
                {
                    std::cout << "Taking control of: " << primitiveDriverID.ToString() << std::endl;
                    // Try take control
                    if(controlService->RequestComponentControl(primitiveDriverID))
                    {
                        // Put the driver in a ready state so we can send commands.
                        if(managementService->Resume(primitiveDriverID))
                        {
                            std::cout << "Putting " << primitiveDriverID.ToString() << " in a Ready state\n";
                            areWeReady = true;
                        }
                    }
                }
                else
                {
                    // Make sure robot is in a ready state
                    if(managementService->GetComponentStatus(primitiveDriverID) != JAUS::Management::Status::Ready)
                    {
                        // Put the driver in a ready state so we can send commands.
                        if(managementService->Resume(primitiveDriverID))
                        {
                            areWeReady = true;
                        }
                    }
                    else
                    {
                        areWeReady = true;
                    }
                }

                // If we have control and everything is ready
                // send drive commands 
                if(areWeReady)
                {
                    JAUS::SetWrenchEffort setWrench(primitiveDriverID, component.GetComponentID());

                    // Remember, local coordinate system for the robot is
                    // x - positive in front
                    // y - positive to the right
                    // z - positive down

                    setWrench.SetPropulsiveLinearEffortX(100);     // Full forward
                    //setWrench.SetPropulsiveRotationalEffortZ(100); // Turn 100% right

                    std::cout << "Current Command:\n";
                    setWrench.PrintMessageBody();

                    component.Send(&setWrench);
                }

                // Check fo escape key
                if(CxUtils::GetChar() == 27)
                {
                    finished = true;
                    // If we are done, let's release control
                    // of the robot to play nice.  This will
                    // automatically put the robot component we
                    // were controlling in a standby state
                    controlService->ReleaseComponentControl(primitiveDriverID);
                    break;
                }

                // Only send commands at 10 Hz in this example
                // change based on your specific system
                CxUtils::SleepMs(100);
            }
        }


        if(CxUtils::GetChar() == 27)
        {
            finished = true;
            break;
        }
        CxUtils::SleepMs(1);
    }

    // Shutdown component.  Services will be deleted automatically.
    component.Shutdown();

    return 0;
}


/* End of File */
