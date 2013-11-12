////////////////////////////////////////////////////////////////////////////////////
///
///  \file jaus_challenge_2010.cpp
///  \brief This file contains a test/example program for showing JAUS compliance
///         using the JAUS Validation Tool (or other similar progam) used by
///         AUVSI at there different competitions.
/// 
///  By following this program, you should be able to complete the 2010 JAUS
///  chanllenge.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 29 May 2010
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
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/mobility/drivers/localwaypointlistdriver.h>
#include <jaus/core/component.h>
#include <cxutils/keyboard.h>
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>

JAUS::UShort gSubsystemID   = 152;    // ID of our subsystem to use.
JAUS::Byte gNodeID          = 1;      // ID of our node to use.
JAUS::Byte gComponentID     = 1;      // ID of the our component.

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Entry point of jaus_challenge_2010.cpp
///
///   This program demonstrates everything required to complete the JAUS
///   Interoperability Challenge for 2010.  In this program a simulated robot
///   is created which can drive to local waypoints.  This program has
///   run against the JAUS Validation Tool (JVT) and the OCP of the
///   JAUS Interopability Challange during the Autonomous Surface Vehicle
///   Competition, passing all requirements.
///
///   All you have to do is integrate on your own robot, providing real
///   sensor values and generating real control of your bot!
///
////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char* argv[])
{
    JAUS::Component component;

    //component.AddService(new JAUS::JTCPClient());

    // Setting timeout for control to 0 (disables timeout of control).
    // This is done because the JVT and OCP do not query for the timeout period
    // and may not send messages to re-acquire/maintain control within the
    // normal 2 second timeout window.
    component.AccessControlService()->SetTimeoutPeriod(0);

    // Add the services we want/need for our component. By
    // default, the component object already has the Core Service set
    // (e.g. Discovery, Events, Access Control, Management).
    // The Discovery service automatically handles finding other JAUS
    // components, so you do not need to generate any Query Messages
    // (e.g. Query Identification) yourself, because Discovery does it
    // for you.

    // In this test program, we are making a simulated robot, which
    // requires the following mobility services.

    JAUS::GlobalPoseSensor* globalPoseSensor = new JAUS::GlobalPoseSensor();
    globalPoseSensor->SetSensorUpdateRate(25);      // Updates at 25 Hz (used for periodic events).
    component.AddService(globalPoseSensor);

    JAUS::LocalPoseSensor* localPoseSensor = new JAUS::LocalPoseSensor();
    localPoseSensor->SetSensorUpdateRate(25);       // Updates at 25 Hz (used for periodic events).
    component.AddService(localPoseSensor);

    JAUS::VelocityStateSensor* velocityStateSensor = new JAUS::VelocityStateSensor();
    velocityStateSensor->SetSensorUpdateRate(25);   // Updates at 25 Hz (used for periodic events).
    component.AddService(velocityStateSensor);

    // Need a List Manager Service for Local Waypoint List Driver.  You can
    // ignore this service for the Underwater Vehicle Competition.
    component.AddService(new JAUS::ListManager());
    JAUS::LocalWaypointListDriver* localWaypointListDriver = new JAUS::LocalWaypointListDriver();
    component.AddService(localWaypointListDriver);

    // Set Vehicle Identification Information.  Available options for the
    // parameters are JAUS::Subsystem::OCU and JAUS::Subsystem::Vehicle.  Since
    // we are not an OCU, we use Vehicle.  Finally, the string represents the type
    // of robot you have (e.g. Segway RMP, XUV).  This is different than the
    // name of your vehicle, but you can use that if you want.  Your subsystem number
    // is your unique identifier.
    component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                             "Simulation");
   
    // Initialize component with a given ID.  Remember, you must
    // add all your services before you initialize the component, because you 
    // cannot add them after initialization.  All services will be deleted by
    // the component on program exit.
    if(component.Initialize(JAUS::Address(gSubsystemID, gNodeID, gComponentID)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    // Set state to Standby since we have initialized OK.
    component.ManagementService()->SetStatus(JAUS::Management::Status::Standby);

    // Now that we are initialized, lets create a fixed connection to the
    // JVT for testing.  Since the JVT and the OCP of the JAUS 
    // Interoperability Challenge do not support multicast for Discovery, you
    // must make a direct connection to them.
    JAUS::Transport* transportService = NULL;
    transportService = component.TransportService();
    JAUS::Connection::Info connectionInfo;
    connectionInfo.mDestIP = std::string("192.168.1.42");
    // Port defaults to 3794, so this is not needed
    connectionInfo.mDestPortNumber = 3794;
    connectionInfo.mTransportType = JAUS::Connection::Transport::JUDP;
    // Create connection to OCP for the JAUS Interoperability Challenge using JUDP.
    transportService->AddConnection(JAUS::Address(42, 1, 1), connectionInfo);
    
    // Set an initial global pose.
    JAUS::GlobalPose globalPose;
    globalPose.SetLatitude(32.703356);
    globalPose.SetLongitude(-117.253919);
    globalPose.SetAltitude(300);
    globalPose.SetPositionRMS(0.0);
    globalPose.SetRoll(0.0);
    globalPose.SetPitch(0.0);
    globalPose.SetYaw(CxUtils::CxToRadians(45));
    globalPose.SetAttitudeRMS(0.0);
    globalPose.SetTimeStamp(JAUS::Time::GetUtcTime());
    // Save the data to the service.
    globalPoseSensor->SetGlobalPose(globalPose);
    // You can set local pose data using global pose
    // as it is automatically converted to local pose data for you.
    // How convenient!
    localPoseSensor->SetLocalPose(globalPose);

    // Set an initial velocity state.
    JAUS::VelocityState velocityState;
    velocityState.SetVelocityX(0.0);
    velocityState.SetYawRate(0.0);
    velocityState.SetVelocityRMS(0.0);
    velocityState.SetTimeStamp(JAUS::Time::GetUtcTime());
    // Save the data to the service.
    velocityStateSensor->SetVelocityState(velocityState);

    JAUS::Time::Stamp printTimeMs = 0;

    double timeDiff = 0.33; // Used for simulation of robot physics.
    
    while(CxUtils::GetChar() != 27 && 
          component.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown)
    {
        double thrust = 25;
        double steering = 0;

        // Simulate some simple physics based on 
        // desired thrust and steering 
        // to reach local waypoints.
        
        double linearDistance = timeDiff*thrust*0.025;
        double linearVelocity = linearDistance/timeDiff;
        double rotation = timeDiff*steering*CxUtils::CxToRadians(1);
        double rotationRate = rotation/timeDiff;
        double yaw = globalPose.GetYaw();
        
        JAUS::Point3D localPosChange(linearDistance, 0, 0);        
        localPosChange = localPosChange.Rotate(yaw, JAUS::Point3D::Z);
        
        CxUtils::Wgs worldPositionWgs(globalPose.GetLatitude(),
                                      globalPose.GetLongitude(),
                                      globalPose.GetAltitude());
        CxUtils::Utm worldPositionUtm(worldPositionWgs);
        
        worldPositionUtm.mNorthing += localPosChange.mX;
        worldPositionUtm.mEasting += localPosChange.mY;
        yaw = CxUtils::Orientation::AddToAngle(yaw, rotation);
        // Convert to WGS
        worldPositionWgs << worldPositionUtm;
        
        // Save newly calculated position and orientation.
        globalPose.SetYaw(yaw);
        globalPose.SetLatitude(worldPositionWgs.mLatitude);
        globalPose.SetLongitude(worldPositionWgs.mLongitude);
        globalPose.SetAltitude(worldPositionWgs.mElevation);
        globalPose.SetTimeStamp(JAUS::Time(true));
        
        velocityState.SetVelocityX(linearVelocity);
        velocityState.SetYawRate(rotationRate);
        velocityState.SetTimeStamp(JAUS::Time(true));
        
        // Save global pose information.
        globalPoseSensor->SetGlobalPose(globalPose);
        // The Local Pose Sensor service supports converting
        // global pose data to local pose data.  It automatically
        // calculates offsets and relative changes for you.
        localPoseSensor->SetLocalPose(globalPose);
        // Save velocity state information.
        velocityStateSensor->SetVelocityState(velocityState);

        if(JAUS::Time::GetUtcTimeMs() - printTimeMs > 500)
        {
            // Print status of services.
            std::cout << "\n======================================================\n";
            component.AccessControlService()->PrintStatus(); std::cout << std::endl;
            component.ManagementService()->PrintStatus(); std::cout << std::endl;
            globalPoseSensor->PrintStatus(); std::cout << std::endl;
            localPoseSensor->PrintStatus(); std::cout << std::endl;
            velocityStateSensor->PrintStatus(); std::cout << std::endl;
            localWaypointListDriver->PrintStatus();
            printTimeMs = JAUS::Time::GetUtcTimeMs();
        }

        // Exit if escape key pressed.
        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        CxUtils::SleepMs((unsigned int)(timeDiff*1000.0));
    }

    // Don't delete services, they are
    // deleted by the Component class.

    // Shutdown any components associated with our subsystem.
    component.Shutdown();

    return 0;
}

/*  End of File */
