////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_synchronize.cpp
///  \brief This file contains a test/example program to demonstrate how
///  to synchronize sensor data between components.  In this specific
///  example, a component synchronizes its sensor data with that generated
///  by the component in the example_subsystem.cpp program.  Synchronization
///  is required to use some of the command components (e.g. GlobalWaypointDriver),
//   as those services are not designed to make subscriptions to sensor
///  data they need, but instead access it directly from a synchronized
///  service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 24 April 2010
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
#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/mobility/sensors/localposesensor.h"
#include "jaus/mobility/sensors/velocitystatesensor.h"
#include "jaus/mobility/sensors/accelerationstatesensor.h"
#include "jaus/extras/mcu/microcontroller.h"
#include "jaus/core/component.h"
#include <cxutils/keyboard.h>
#include <iostream>


#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


JAUS::UShort gSubsystemID   = 152;  // ID of our subsystem to use.
JAUS::Byte gNodeID          = 1;
JAUS::Byte gComponentID     = 2;      // ID of the our component.

JAUS::Byte gSyncNodeID      = 1;
JAUS::Byte gSyncComponentID = 1;      // ID of the component our our subsystem to sync to.


////////////////////////////////////////////////////////////////////////////////////
///
///   \class EventCallback
///   \brief This is an example of an Events callback so you can be notified
///          whenever a component (or the Events service) receives an event
///          message from an active subscription.  
///
///          This example is used to demonstrate another way to be notified
///          when events arrive.  You can still get the same data by overloading
///          the Receive method of your own service.
///
////////////////////////////////////////////////////////////////////////////////////
class EventCallback : public JAUS::Events::Callback
{
public:
    EventCallback() {}
    ~EventCallback() {}
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Method called when an event related message has been received.
    ///
    ///   \param[in] type Type of event (EveryChange, Periodic).
    ///   \param[in] eventID Event ID number.
    ///   \param[in] sequenceNumber The sequence number for the event (update number).
    ///   \param[in] message The message data associated with the event.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    virtual void ProcessEvent(const JAUS::Events::Type type,
                              const JAUS::Byte eventID,
                              const JAUS::Byte sequenceNumber,
                              const JAUS::Message* message)
    {
        /* Example code
        std::cout << "Received an Event!\n";
        const JAUS::ReportGlobalPose* reportGlobalPose = dynamic_cast<const JAUS::ReportGlobalPose*>(message);
        if(reportGlobalPose)
        {
            reportGlobalPose->Print();
        }
        */
    }
    unsigned int mCount;
};

// Testing MCU Service code.
class FakeMicrocontroller : public JAUS::Microcontroller
{
public:
    FakeMicrocontroller() {}
    ~FakeMicrocontroller() {}
    virtual void SetDigitalOut(const std::string& name,
                               const bool value) {}
    virtual void SetAnalogOut(const std::string& name,
                              const double value) {}
};

int main(int argc, char* argv[])
{
    JAUS::Component component; 
    
    // Add the services our component needs.  This has to
    // be done before component initialization.
    // Second, you cannot add the same service twice.

    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    JAUS::LocalPoseSensor* localPoseSensor = new JAUS::LocalPoseSensor();
    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    JAUS::AccelerationStateSensor* accelerationStateSensor = new JAUS::AccelerationStateSensor();
    FakeMicrocontroller* mcu = new FakeMicrocontroller();

    // Add to our component.
    component.AddService(globalPoseSensor);
    component.AddService(localPoseSensor);
    component.AddService(velocityStateSensor);
    component.AddService(accelerationStateSensor);
    component.AddService(mcu);

    // Tell our sensor services what component we want them
    // to sync there data with (i.e. who to subscribe to.
    JAUS::Address syncID(gSubsystemID, gSyncNodeID, gSyncComponentID);
    globalPoseSensor->SynchronizeToComponent(syncID);
    localPoseSensor->SynchronizeToComponent(syncID);
    velocityStateSensor->SynchronizeToComponent(syncID);
    accelerationStateSensor->SynchronizeToComponent(syncID);
    mcu->SynchronizeToComponent(syncID);

    // Example of how to use events callback
    EventCallback callback;
    component.EventsService()->RegisterCallback(&callback);


    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    if(component.LoadSettings("settings/services.xml") == false)
    {
        // Working directory probably not set (or not running from output directory), so
        // use default values.
        component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                 "Simulation");
        // Set optional identification info for this component.
        component.DiscoveryService()->SetComponentIdentification("Synchronize");
    }

    // Initialize component with component given ID.
    if(component.Initialize(JAUS::Address(gSubsystemID, gNodeID, gComponentID)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }
    
    bool foundSyncID = false;
    CxUtils::Time::Stamp printTimeMs = 0;
    while(CxUtils::GetChar() != 27)
    {
        // This is a short example code to dynamically lookup a
        // component ID for the service you want to synchronize on your
        // subsystem (like in your AI program if the services may run on different computers).
#if 0
        // Enable synchronization for each sensor.
        if(globalPoseSensor->IsSynchronizing() == false)
        {
            // Find sensor to sync to.
            JAUS::Address::List idList = component.DiscoveryService()->GetSubsystem(gSubsystemID)->GetComponentsWithService(JAUS::GlobalPoseSensor::Name);
            if(idList.size() > 0)
            {
                globalPoseSensor->SynchronizeToComponent(idList.front());
            }
        }
#endif
        if(CxUtils::Time::GetUtcTimeMs() - printTimeMs > 500)
        {
            // Print status of services.
            std::cout << "\n======================================================\n";
            globalPoseSensor->PrintStatus(); std::cout << std::endl;
            localPoseSensor->PrintStatus(); std::cout << std::endl;
            velocityStateSensor->PrintStatus(); std::cout << std::endl;
            accelerationStateSensor->PrintStatus(); std::cout << std::endl;
            mcu->PrintStatus(); std::cout << std::endl;
            printTimeMs = CxUtils::Time::GetUtcTimeMs();
        }
        CxUtils::SleepMs(1);
    }

    // Don't delete services, they are
    // deleted by the Component class.

    // Shutdown any components associated with our subsystem.
    component.Shutdown();

    return 0;
}

/* End of File */
