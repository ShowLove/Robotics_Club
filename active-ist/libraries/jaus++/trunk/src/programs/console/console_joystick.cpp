////////////////////////////////////////////////////////////////////////////////////
///
///  \file joystick.cpp
///  \brief This file contains a test/example program for a simple console based
///         joystick controller.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 3 February 2010
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
#include "jaus/extras/controllers/joystick.h"
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
    UShort subsystemToControl = 0;
    Component component;
    // Add the services we want our component to have
    // beyond the core set.

    // In this example, we are testing the Video Subscriber
    // service, so lets add it.
    Joystick* control = new Joystick();
    component.AddService(control);

    if(argc > 1)
    {
        subsystemToControl = (unsigned int)atoi(argv[1]);
    }

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    component.LoadSettings("settings/joysticks/default.xml");

    // In this example, lets make an OCU subsystem subscribing to video.
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU,
                                                             "Joystick");
    component.AccessControlService()->SetAuthorityCode(255);
    // Initialize component component with component given ID.
    if(component.InitializeWithUniqueID() == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    //CxUtils::SleepMs(2000);

    bool loadedJoystickFile = false;
    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    while(CxUtils::GetChar() != 27)
    {      
        //component.DiscoveryService()->PrintSystemConfiguration();
        if(control->GetDriverID().IsValid() == false)
        {
            if(subsystemToControl == 0)
            {
                Discovery::List subsystems = component.DiscoveryService()->GetSubsystemList();
                Discovery::List::iterator s;
                std::map<UShort, std::string> vehicles;
                for(s = subsystems.begin();
                    s != subsystems.end();
                    s++)
                {
                    if(component.DiscoveryService()->GetSubsystem(s->first)->mIdentification.empty() == false &&
                       component.DiscoveryService()->GetSubsystem(s->first)->HaveService(PrimitiveDriver::Name))
                    {
                        vehicles[s->first] = component.DiscoveryService()->GetSubsystem(s->first)->mIdentification;
                        //subsystemToControl = s->first;
                        //break;
                    }
                }
                if(vehicles.size() > 0)
                {
                    std::cout << "Please Select a Vehicle to Control:\n";
                    std::map<UShort, std::string>::iterator v;
                    for(v = vehicles.begin();
                        v != vehicles.end();
                        v++)
                    {
                        std::cout << v->first << " - " << v->second << std::endl;
                    }
                    std::cout << "0 for Refresh\n";
                    std::cout << "Enter Selection: ";
                    std::cin >> subsystemToControl;
                    std::cout << "Press Any Key to Reset\n";
                }
                if(vehicles.find((UShort)subsystemToControl) == vehicles.end())
                {
                    subsystemToControl = 0; // Invalid number types
                }
            }
            control->SetSubsystemToControl(subsystemToControl);
        }
        if(loadedJoystickFile == false)
        {
            Subsystem::Ptr toControl = component.DiscoveryService()->GetSubsystem(subsystemToControl);
            if(toControl->IsValid())
            {
                loadedJoystickFile = control->LoadSettingsForVehicle(toControl->mIdentification, "settings/joysticks/");
                if(loadedJoystickFile == false &&
                   toControl->mIdentification != "")
                {
                    loadedJoystickFile = control->LoadSettings("settings/joysticks/default.xml");
                }  
            }
        }
        control->PrintStatus();
        if(control->GetDriverID().IsValid())
        {
            std::cout << "Authority Control: " << (int) component.AccessControlService()->GetAuthorityCode() << std::endl;
        }
        else if(subsystemToControl == 0)
        {
            std::cout << "Looking for vehicles...\n";
        }
        CxUtils::SleepMs(500); 
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */
