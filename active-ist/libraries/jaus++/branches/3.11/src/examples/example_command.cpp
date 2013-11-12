////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_command.cpp
///  \brief This file contains a test/example program for the SubsystemCommand
///  service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 15 February 2010
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
#include "jaus/core/component.h"
#include "jaus/extras/command/subsystemcommand.h"
#include "jaus/mobility/drivers/primitivedriver.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


using namespace JAUS;

int main(int argc, char* argv[])
{
    Component component;
    // Add the services we want our component to have
    // beyond the core set.

    // In this example, we are testing a custom service.
    SubsystemCommand* command = new SubsystemCommand(Service::ID(SubsystemCommand::Name));
    // Set up to get global pose data from vehicles every
    // time the pose changes (must be done before initialization).
    command->SetGlobalPoseSubscriptionType(true);

    // Add to component.
    component.AddService(command);

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    component.LoadSettings("settings/services.xml");

    // In this example, lets make an OCU subsystem subscribing to video.  You
    // could just as easily make the SubsystemCommand part of a Vehicle, acting as
    // the A.I.
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU,
                                                             "SubsystemCommand");

    // Initialize component component with component given ID.
    if(component.Initialize(Address(1000, 1, 1)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    component.AccessControlService()->SetAuthorityCode(255);
    //component.TransportService()->EnableDebugMessages(true);
    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    while(CxUtils::GetChar() != 27)
    {      
        component.DiscoveryService()->PrintSystemConfiguration();

        //command->PrintStatus();

        // Get a copy of all vehicles info.
        Vehicle::Map vehicles;
        command->DiscoveryService()->GetVehicles(vehicles);

        // Look up pose, configuration info, etc. for all 
        // discovered vehicles.

        // Send any commands if needed. here is a bad example.
        Vehicle::Map::iterator vehicle;
        SetWrenchEffort message;
        for(vehicle = vehicles.begin();
            vehicle != vehicles.end();
            vehicle++)
        {
            Address::List drivers = vehicle->second->GetComponentsWithService(PrimitiveDriver::Name);
            if(drivers.size() > 0)
            {
                message.SetSourceID(component.GetComponentID());
                message.SetDestinationID(drivers.front());
                message.SetPropulsiveLinearEffortX(50);
                // Send command method takes control and can resume destination component automatically.
                command->SendCommand(&message, true);
            }
        }

        // When done delete copies.
        Vehicle::DeleteVehicleMap(vehicles);
        
        CxUtils::SleepMs(250); 
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */
