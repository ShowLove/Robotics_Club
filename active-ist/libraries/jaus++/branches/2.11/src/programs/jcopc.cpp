////////////////////////////////////////////////////////////////////////////////////
///
///  \file jcopc.cpp
///  \brief This is a test program for doing the JAUS Challenge sponsored by
///         different AUVSI competitions.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2011
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
#include "jaus/core/component.h"
#include "jaus/mobility/sensors/setlocalpose.h"
#include "jaus/mobility/drivers/setlocalwaypoint.h"
#include "jaus/mobility/list/listmanager.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>


using namespace JAUS;

int main(int argc, char* argv[])
{
    UShort subsystemToControl = 0;
    Address driver;
    Component component;
    // Add the services we want our component to have
    // beyond the core set.

    if(argc > 1)
    {
        subsystemToControl = (unsigned int)atoi(argv[1]);
    }

    // In this example, lets make an OCU subsystem subscribing to video.
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU,
                                                             "OCP");
    component.AccessControlService()->SetAuthorityCode(200);
    // Initialize component component with component given ID.
    if(component.InitializeWithUniqueID() == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    //CxUtils::SleepMs(2000);

    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    int key = 0;
    while(key != 27)
    {
        if(subsystemToControl == 0)
        {
            Vehicle::Map vehicles;
            component.DiscoveryService()->GetVehicles(vehicles);
            if(vehicles.size() > 0)
            {
                std::cout << "Please Select a Vehicle to Control:\n";
                Vehicle::Map::iterator v;
                for(v = vehicles.begin();
                    v != vehicles.end();
                    v++)
                {
                    std::cout << v->first << " - " << v->second->mIdentification << std::endl;
                }
                std::cout << "0 for Refresh\n";
                std::cout << "Enter Vehicle Selection: ";
                std::cin >> subsystemToControl;
                std::cout << "Press Any Key to Reset\n";
            }
            Vehicle::DeleteVehicleMap(vehicles);
        }
        else
        {
            driver = Address(subsystemToControl, 1, 1);
        }

        if(driver.IsValid())
        {
            if(component.AccessControlService()->HaveControl(driver) == false)
            {
                for(int i = 0; i < 5; i++)
                {
                    if(component.AccessControlService()->RequestComponentControl(driver))
                    {
                        CxUtils::SleepMs(100);
                        break;
                    }
                    CxUtils::SleepMs(250);
                }
                if(component.AccessControlService()->HaveControl(driver) == false)
                {
                    std::cout << "Unable to take control of component: " << driver.ToString() << std::endl;
                    driver.Clear();
                    subsystemToControl = 0;
                    continue;
                }
            }

            int option = 1;
            std::cout << "Menu Options:\n";
            std::cout << "0 - Send Resume\n";
            std::cout << "1 - Send Standby\n";
            std::cout << "2 - Send Shutdown\n";
            std::cout << "3 - Reset Local Pose\n";
            std::cout << "4 - Send Waypoints\n";
            std::cout << "5 - Send Execute Mission\n";
            std::cout << "6 - Release control\n";
            std::cin >> option;

            switch(option)
            {
            case 0:
            {
                std::cout << "Sending Resume to " << driver.ToString() << std::endl;
                component.ManagementService()->Resume(driver);
            }
                break;
            case 1:
            {
                std::cout << "Sending Standby to " << driver.ToString() << std::endl;
                component.ManagementService()->Standby(driver);
            }
                break;
            case 2:
            {
                std::cout << "Sending Shutdown to " << driver.ToString() << std::endl;
                component.ManagementService()->Shutdown(driver);
            }
                break;
            case 3:
            {
                std::cout << "Setting Local Pose to <0, 0, 0>\n";
                SetLocalPose setLocalPose(driver, component.GetComponentID());
                setLocalPose.SetX(0);
                setLocalPose.SetY(0);
                setLocalPose.SetZ(0);
                setLocalPose.SetYaw(0);
                setLocalPose.SetPitch(0);
                setLocalPose.SetRoll(0);
                component.Send(&setLocalPose);
            }
                break;
            case 4:
            {
                std::cout << "Sending 4 Local Waypoints (Square) to robot\n";
                JAUS::SetElement command(driver, component.GetComponentID());
                command.SetRequestID(1);
                JAUS::Point3D::List localWaypoints;
                double size = 5;
                // Add more waypoints for testing as needed.
                localWaypoints.push_back(JAUS::Point3D(size, 0, 0));
                localWaypoints.push_back(JAUS::Point3D(size, size, 0));
                localWaypoints.push_back(JAUS::Point3D(0, size, 0));
                localWaypoints.push_back(JAUS::Point3D(0, 0, 0));

                for(unsigned int i = 0; i < (unsigned int)localWaypoints.size(); i++)
                {
                    JAUS::Element e;
                    // Set the element ID.
                    e.mID = i + 1;
                    // If this is the last element (and we are not looping) set the
                    // next ID to 0, otherwise the value of the next element ID.
                    if(i < (unsigned int)localWaypoints.size() - 1)
                    {
                        e.mNextID = e.mID + 1;
                    }
                    // Set previous element.
                    e.mPrevID = i;
                    // Populate the element message which is the command to be executed.
                    JAUS::SetLocalWaypoint* message = new JAUS::SetLocalWaypoint();
                    message->SetX(localWaypoints[i].mX);
                    message->SetY(localWaypoints[i].mY);
                    // Save the pointer
                    e.mpElement = message;
                    // Push completed element onto the list.
                    command.GetElementList()->push_back(e);
                }

                component.Send(&command);
            }
                break;
            case 5:
            {
                // Now tell it to start executing.
                JAUS::ExecuteList executeCommand(driver, component.GetComponentID());
                executeCommand.SetElementUID(1); // Start at beginning.
                executeCommand.SetSpeed(1.0);    // Maximum speed.
                component.Send(&executeCommand);
            }
                break;
            case 6:
            {
                component.AccessControlService()->ReleaseComponentControl(driver);
                driver.Clear();
                subsystemToControl = 0;
            }
            break;
            default:
                std::cout << "Invalid Option " << option << std::endl;
                break;
            }
        }
        key = CxUtils::GetChar();
        CxUtils::SleepMs(100);
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */

