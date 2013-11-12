////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_08.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates how to use the JTCP transport
///         service.  It is a dual program, one being a client (ocu) the other
///         a server (robot).
///
///  <br>Author(s): Daniel Barber
///  <br>Created: April 2011
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
#include <jaus/core/transport/jtcpclient.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <cxutils/keyboard.h>
#include <iostream>

#include <stdio.h>
#include <sys/timeb.h>
#include <time.h>

JAUS::UShort gSubsystemID = 8;

int RunServer(JAUS::UShort subsystemID = gSubsystemID, JAUS::Byte nodeID = 1);
int RunClient(JAUS::UShort subsystemID = gSubsystemID, JAUS::Byte nodeID = 2);

int main(int argc, char* argv[])
{
    std::cout << "Program Usage:\n";
    std::cout << argv[0] << " <0 = server, 1,2,..254 = client>\n";
    if(argc > 1)
    {
        if(atoi(argv[1]) == 0)
        {
            return RunServer();
        }
        else
        {
            return RunClient(gSubsystemID, (JAUS::Byte)(atoi(argv[1])) + 1);
        }
    }
    else
    {
        // Default
        return RunServer();
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Simple simulation of a component with a Local Pose Sensor using
///          JTCP.
///
///   \param[in] subsystemID Subsystem ID number.
///   \param[in] nodeID Node ID number.
///
////////////////////////////////////////////////////////////////////////////////////
int RunServer(JAUS::UShort subsystemID, JAUS::Byte nodeID)
{
    JAUS::Component component;
    JAUS::Address id(subsystemID, nodeID, 1);
    if(id.IsValid() == false)
    {
        std::cout << "Invalid JAUS ID: " << id.ToString() << std::endl;
    }

    // Setup identification data.
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                             "Local Pose JTCP Tutorial");

    //  Change the transport service to use JTCP
    component.AddService(new JAUS::JTCPClient());

    // Add any other services.  In this case local pose.
    JAUS::LocalPoseSensor* localPoseSensor = new JAUS::LocalPoseSensor();
    component.AddService(localPoseSensor);


    // Initialize component and communication.
    if(component.Initialize(id) == false)
    {
        std::cout << "Failed to initialize JAUS component with ID: " << id.ToString() << std::endl;
        return 0;
    }

    // Configure local pose information.
    JAUS::LocalPose localPose;
    localPose.SetX(0);
    localPose.SetY(0);
    localPose.SetZ(0);
    localPose.SetYaw(0);
    localPose.SetTimeStamp(JAUS::Time(true));

    JAUS::Time printTime(true);
    while(CxUtils::GetChar() != 27 && component.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown)
    {
        // Fake a position change.
        localPose.SetY(localPose.GetY() + .01);
        localPose.SetTimeStamp(JAUS::Time(true));
        // Save the data.  Anyone who is subscribing will be
        // notified of the change automatically.
        localPoseSensor->SetLocalPose(localPose);

        // Only print to screen every now and then.
        if(JAUS::Time(true) - printTime > .5)
        {
            std::cout << "=======================================================\n";
            localPoseSensor->PrintStatus();
            component.EventsService()->PrintStatus();
            printTime.SetCurrentTime();
        }
        // Don't overload the CPU
        CxUtils::SleepMs(1);
    }

    // Do shutdown
    component.Shutdown();

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback class to get message data as it arrives.
///
////////////////////////////////////////////////////////////////////////////////////
class LocalPoseCallback : public JAUS::Transport::Callback
{
public:
    LocalPoseCallback() {}
    ~LocalPoseCallback() {}
    virtual void ProcessMessage(const JAUS::Message* message)
    {
        const JAUS::ReportLocalPose* reportLocalPose = dynamic_cast<const JAUS::ReportLocalPose*>(message);
        if(reportLocalPose)
        {
            reportLocalPose->Print();
        }
    }
};


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Simple simulation of a component that subscribes to any Local
///          Pose Sensor data on network using JTCP.
///
///   \param[in] subsystemID Subsystem ID number.
///   \param[in] nodeID Node ID number.
///
////////////////////////////////////////////////////////////////////////////////////
int RunClient(JAUS::UShort subsystemID, JAUS::Byte nodeID)
{
    JAUS::Component component;
    JAUS::Address id(subsystemID, nodeID, 1);

    if(id.IsValid() == false)
    {
        std::cout << "Invalid JAUS ID: " << id.ToString() << std::endl;
    }

    // Setup identification data.
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                             "Subscriber JTCP Tutorial");

    //  Change the transport service to use JTCP
    component.AddService(new JAUS::JTCPClient());
    //((JAUS::JTCPClient*)component.TransportService())->AddConnection("10.171.190.61", JAUS::Address(subsystemID, 1, 1));
    //((JAUS::JTCPClient*)component.TransportService())->AddConnection("127.0.0.1", JAUS::Address(subsystemID, 1, 1));

    // Add support for reading Local Pose messages without having to
    // add a custom service.
    component.TransportService()->AddMessageTemplate(new JAUS::ReportLocalPose());
    component.TransportService()->AddMessageTemplate(new JAUS::QueryLocalPose());

    // Add a callback to get messages when they arrive.
    LocalPoseCallback localPoseCallback;
    component.TransportService()->RegisterCallback(JAUS::REPORT_LOCAL_POSE, &localPoseCallback);

    // Initialize component and communication.
    if(component.Initialize(id) == false)
    {
        std::cout << "Failed to initialize JAUS component with ID: " << id.ToString() << std::endl;
        return 0;
    }

    JAUS::Time printTime(true);

    while(CxUtils::GetChar() != 27 && component.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown)
    {
        // Look for any subsystems.
        JAUS::Subsystem::Map subsystems;
        JAUS::Subsystem::Map::iterator subsystem;

        component.DiscoveryService()->GetSubsystems(subsystems);

        for(subsystem = subsystems.begin();
            subsystem != subsystems.end();
            subsystem++)
        {
            // Look for local pose sensors to subscribe to.
            JAUS::Address::List sensors = subsystem->second->GetComponentsWithService(JAUS::LocalPoseSensor::Name);
            if(sensors.size() > 0)
            {
                JAUS::Address::List::iterator id;
                for(id = sensors.begin();
                    id != sensors.end();
                    id++)
                {
                    if(*id != component.GetComponentID() && 
                       component.EventsService()->HaveSubscription(JAUS::REPORT_LOCAL_POSE, *id) == false)
                    {
                        std::cout << "Found new Local Pose Sensors on " << id->ToString() << std::endl;
                        JAUS::QueryLocalPose queryLocalPose;
                        // Request all fields.
                        queryLocalPose.SetPresenceVector(queryLocalPose.GetPresenceVectorMask());
                        component.EventsService()->RequestPeriodicEvent(*id, &queryLocalPose, 5);
                    }
                }
            }
        }

        JAUS::Subsystem::DeleteSubsystemMap(subsystems);
        // Only print to screen every now and then.
        if(JAUS::Time(true) - printTime > .5)
        {
            //std::cout << "=======================================================\n";
            //component.EventsService()->PrintStatus();
            //printTime.SetCurrentTime();
        }
        // Don't overload the CPU
        CxUtils::SleepMs(1);
    }

    // Do shutdown
    component.Shutdown();

    return 0;
}

/* End of File */
