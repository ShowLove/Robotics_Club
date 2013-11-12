////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_06.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to add additional services
///         to a component.  Specifically, this tutorial uses the Global
///         Pose Sensor service of the mobility library.
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
#include <jaus/mobility/sensors/globalposesensor.h>
#include <cxutils/keyboard.h>
#include <iostream>


int main(int argc, char* argv[])
{
    JAUS::Component component;

    // Add services to a component is done using the AddService
    // method.  Services can only be added before a 
    // component is initialized using the Initialize method.
    // Any attempt to add after will fail.  Finally, you
    // cannot add multiple of the same service.

    // Create a Global Pose Sensor service.
    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    // Set the update rate of the sensor (Hz).  This
    // is used to determine what type of periodic
    // events the sensor can support.
    globalPoseSensor->SetSensorUpdateRate(25);

    // Set some global pose values.
    JAUS::GlobalPose globalPose;
    globalPose.SetLatitude(34.12345);
    globalPose.SetLongitude(-116.12345);
    globalPose.SetAltitude(100);
    globalPose.SetTimeStamp(JAUS::Time(true));
    // Set the values.
    globalPoseSensor->SetGlobalPose(globalPose);

    // Add the sensor service to the component. Remember
    // this must be done before initialization.  Also,
    // the component will delete the service for us
    // automatically, so we don't have to!
    component.AddService(globalPoseSensor);

    // Setup identification info.  For questions about this,
    // see the previous tutorial(s).
    JAUS::Discovery* discoveryService = NULL;
    discoveryService = component.DiscoveryService();
    discoveryService->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                 "Robot");
    discoveryService->SetNodeIdentification("Primary Computer");
    discoveryService->SetComponentIdentification("Baseline");

    JAUS::Address componentID(1000, 1, 6);
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
        // To "simulate" a real sensor, lets modify
        // the latitude like the robot is moving north.
        globalPose.SetLatitude(globalPose.GetLatitude() + 0.00001);
        globalPose.SetTimeStamp(JAUS::Time(true));
        // Update the sensor with the new data.  This will
        // automatically trigger events if someone (i.e. another
        // component) is subscribing to the data from our sensor.
        // this is because the Global Pose Sensor class inherits
        // from the Events service and has implemented all the
        // methods needed to support subscriptions.
        globalPoseSensor->SetGlobalPose(globalPose);

        JAUS::Management* managementService = NULL;
        managementService = component.ManagementService();
        if(managementService->GetStatus() == JAUS::Management::Status::Shutdown)
        {
            // Exit program.
            break;
        }

        if(JAUS::Time::GetUtcTimeMs() - displayStatusTimeMs > 500)
        {
            // Print out status of the service.
            component.GetService(JAUS::GlobalPoseSensor::Name)->PrintStatus(); std::cout << std::endl;
            displayStatusTimeMs = JAUS::Time::GetUtcTimeMs();
        }

        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        CxUtils::SleepMs(100);
    }

    // Shutdown your component completely.  Any
    // services added or belonging to the component
    // will be deleted.
    component.Shutdown();

    return 0;
}



/* End of File */
