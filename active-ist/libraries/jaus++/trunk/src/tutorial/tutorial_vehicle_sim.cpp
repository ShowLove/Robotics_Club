////////////////////////////////////////////////////////////////////////////////////
///
///  \file tutorial_vehicle_sim.cpp
///  \brief This file is part of a set of tutorials for learning how to use
///         JAUS++.  This program demonstrates a simple simulated robot for
///         testing.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 February 2011
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
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/mobility/drivers/localwaypointlistdriver.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/core/component.h>
#include <cxutils/keyboard.h>
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <cstring>
#include <cstdio>
#include <cmath>


////////////////////////////////////////////////////////////////////////////////////
///
///   \class RobotPrimitiveDriver
///   \brief An example of how to create a Primitive Driver service.
///
///   This fake Primitive Driver is designed to show how you would receive
///   Wrench Effort commands for a simple ground robot with thrust and steering
///   capabilities.
///
////////////////////////////////////////////////////////////////////////////////////
class RobotPrimitiveDriver : public JAUS::PrimitiveDriver
{
public:
    RobotPrimitiveDriver();
    ~RobotPrimitiveDriver();
    virtual bool Resume();
    virtual bool Reset();
    virtual bool Standby();
    virtual bool SetEmergency();
    virtual bool ClearEmergency();
    virtual bool ReleaseControl();
    virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command);
    void UpdatePhysics(const double timeSeconds = 0.33);
    volatile double mThrust;                ///< Linear thrust on X axis.
    volatile double mSteering;              ///< Rotational thrust on Z axis.
    JAUS::GlobalPose mGlobalPose;           ///<  Vehicle global pose (GPS, Compass).
    JAUS::VelocityState mVelocityState;     ///<  Vehicle velocity state information (speed).
};  //  End of class RobotPrimitiveDriver


////////////////////////////////////////////////////////////////////////////////////
///
///   \class RobotSim
///   \brief Simple simulated robot.
///
///   This fake robot contains two components:
///
///   Baseline - Contains Primitive Driver and Position/Velocity Sensors.
///   LocalWaypointListDriver - Contains services for Local Waypoint List Driver.
///
////////////////////////////////////////////////////////////////////////////////////
class RobotSim
{
public:
    RobotSim()
    {
        mSubsystemID = 152;
        mpGlobalPoseSensor = NULL;
        mpLocalPoseSensor = NULL;
        mpVelocityStateSensor = NULL;
        mpLocalWaypointListDriver = NULL;
    }
    ~RobotSim()
    {
        mBaselineComponent.Shutdown();
    }
    void AddServices()
    {
        // Add the services we want/need for our mBaselineComponent. By
        // default, the component object already has the Core Service set
        // (e.g. Discovery, Events, Access Control, Management).
        // The Discovery service automatically handles finding other JAUS
        // components, so you do not need to generate any Query Messages
        // (e.g. Query Identification) yourself, because Discovery does it
        // for you.

        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        //  Baseline Component

        // In this test program, we are making a simulated robot, which
        // requires the following mobility services.
        mpGlobalPoseSensor = new JAUS::GlobalPoseSensor();
        mpGlobalPoseSensor->SetSensorUpdateRate(25);      // Updates at 25 Hz (used for periodic events).
        mBaselineComponent.AddService(mpGlobalPoseSensor);
        
        mpLocalPoseSensor = new JAUS::LocalPoseSensor();
        mpLocalPoseSensor->SetSensorUpdateRate(25);       // Updates at 25 Hz (used for periodic events).
        mBaselineComponent.AddService(mpLocalPoseSensor);

        mpVelocityStateSensor = new JAUS::VelocityStateSensor();
        mpVelocityStateSensor->SetSensorUpdateRate(25);   // Updates at 25 Hz (used for periodic events).
        mBaselineComponent.AddService(mpVelocityStateSensor);

        // Create primitive driver service.
        mpPrimitiveDriver = new RobotPrimitiveDriver();
        mBaselineComponent.AddService(mpPrimitiveDriver);


        /////////////////////////////////////////////////////////////////////////////////
        /////////////////////////////////////////////////////////////////////////////////
        //  Local Waypoint Driver Component

        // Need a List Manager Service for Local Waypoint List Driver.
        mLocalWaypointDriverComponent.AddService(new JAUS::ListManager());
        mpLocalWaypointListDriver = new JAUS::LocalWaypointListDriver();
        mLocalWaypointDriverComponent.AddService(mpLocalWaypointListDriver);

    }
    bool Initialize()
    {
        // Setting timeout for control to 0 (disables timeout of control).
        // This is done because the JVT and OCP do not query for the timeout period
        // and may not send messages to re-acquire/maintain control within the
        // normal 2 second timeout window.
        mBaselineComponent.AccessControlService()->SetTimeoutPeriod(0);
        mLocalWaypointDriverComponent.AccessControlService()->SetTimeoutPeriod(0);

        // Set Vehicle Identification Information.  Available options for the
        // parameters are JAUS::Subsystem::OCU and JAUS::Subsystem::Vehicle.  Since
        // we are not an OCU, we use Vehicle.  Finally, the string represents the type
        // of robot you have (e.g. Segway RMP, XUV).  This is different than the
        // name of your vehicle, but you can use that if you want.  Your subsystem number
        // is your unique identifier.
        mBaselineComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                 "Simulation");
        mBaselineComponent.DiscoveryService()->SetNodeIdentification("Main PC");
        mBaselineComponent.DiscoveryService()->SetComponentIdentification("Baseline");

        mLocalWaypointDriverComponent.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                                "Simulation");
        mLocalWaypointDriverComponent.DiscoveryService()->SetNodeIdentification("Main PC");
        mLocalWaypointDriverComponent.DiscoveryService()->SetComponentIdentification("Local Waypoint Driver");
   
        // Initialize component with a given ID.  Remember, you must
        // add all your services before you initialize the component, because you 
        // cannot add them after initialization.  All services will be deleted by
        // the component on program exit.
        if(mBaselineComponent.Initialize(JAUS::Address(mSubsystemID, 1, 1)) == false)
        {
            std::cout << "Failed to Initialize Component.\n";
            return false;
        }

        // Initialize our Local Waypint Driver component.  This component contains
        // the Local Waypoint Driver service, and when driving to waypoints, will acquire
        // control of the "Baseline" component to manipulate the Primitive Driver service.
        if(mLocalWaypointDriverComponent.Initialize(JAUS::Address(mSubsystemID, 1, 2)) == false)
        {
            std::cout << "Failed to Initialize Component.\n";
            return false;
        }

        // Set state to Standby since we have initialized OK.
        mBaselineComponent.ManagementService()->SetStatus(JAUS::Management::Status::Standby);
        mLocalWaypointDriverComponent.ManagementService()->SetStatus(JAUS::Management::Status::Standby);

        // Set the initial global pose of the robot.
        // This data is coming from the Primitive Driver service in
        // this example, because it is simulating the physics.
        mpGlobalPoseSensor->SetGlobalPose(mpPrimitiveDriver->mGlobalPose);
        // You can set local pose data using global pose
        // as it is automatically converted to local pose data for you.
        // How convenient!
        mpLocalPoseSensor->SetLocalPose(mpPrimitiveDriver->mGlobalPose);
        // Set an initial velocity state
        mpVelocityStateSensor->SetVelocityState(mpPrimitiveDriver->mVelocityState);

        //UseFakeWaypoints();

        return true;
    }
    void UpdateWaypointDriver()
    {
        double thrust = 0;
        double steering = 0;
        // Are we in an execute state (e.g. received execute command and are
        // in ready state for driving).  This part is not needed if you are
        // doing the Underwater Vehicle Competition, since you are not required to
        // drive to local waypoints.
        if(mpLocalWaypointListDriver->IsExecuting())
        {
            // If the current element in the list is 0, then we have reached the
            // end of the list.  The element ID in the list is set by the OCP using
            // the Execute message which the LocalWaypointListDriver service processes
            // for you.
            if(mpLocalWaypointListDriver->GetActiveListElementID() != 0)
            {
                // Get the current waypoint in the list.
                JAUS::SetLocalWaypoint wp = mpLocalWaypointListDriver->GetCurrentWaypoint();
                JAUS::LocalPose pose = mpLocalPoseSensor->GetLocalPose();
                // See if we reached the point.
                CxUtils::Point3D wpPos(wp.GetX(), wp.GetY(), wp.GetZ());
                CxUtils::Point3D myPos(pose.GetX(), pose.GetY(), pose.GetZ());
                double distanceToWaypoint = myPos.Distance(wpPos);
                double tolerance = 0.25;
                // See if the waypoint has a desired tolerance, if not use the
                // default value of 0.25 meter radius.
                if(wp.IsFieldPresent(JAUS::SetLocalWaypoint::PresenceVector::WaypointTolerance))
                {
                    tolerance = wp.GetWaypointTolerance();
                }
                if(distanceToWaypoint <= tolerance)
                {
                    // Reached point, advance list.
                    mpLocalWaypointListDriver->AdvanceListElement();
                }
                else
                {
                    // Drive to local waypoint by generating a desired
                    // thrust and steering.  For this demo/example program
                    // an open-loop control system is used.
                    
                    // Get angle to waypoint.
                    double angle = atan2(wpPos.mY - myPos.mY, wpPos.mX - myPos.mX);
                    double angleDiff = CxUtils::Orientation::AngleDiff(pose.GetYaw(), angle);
                    // Steer to angle
                    if(fabs(angleDiff) > CxUtils::CxToRadians(5))
                    {
                        steering = 30;
                        if(angleDiff < 0)
                        {
                            steering *= -1;
                        }
                    }
                    else
                    {
                        // Within 5 degrees of desired heading
                        // set thrust to 30 to drive, and
                        // stop rotating.
                        thrust = 30;
                        steering = 0;
                    }
                }
            }

            // Check for control of Primitive Driver component
            if(mLocalWaypointDriverComponent.AccessControlService()->HaveControl(mBaselineComponent.GetComponentID()) == false)
            {
                mLocalWaypointDriverComponent.AccessControlService()->RequestComponentControl(mBaselineComponent.GetComponentID());
                // Put component in ready state.
                mLocalWaypointDriverComponent.ManagementService()->Resume(mBaselineComponent.GetComponentID());
            }
            if(mLocalWaypointDriverComponent.AccessControlService()->HaveControl(mBaselineComponent.GetComponentID()))
            {
                // Send drive command to make robot move!
                JAUS::SetWrenchEffort wrenchCommand(mBaselineComponent.GetComponentID(), mLocalWaypointDriverComponent.GetComponentID());
                wrenchCommand.SetPropulsiveLinearEffortX(thrust);
                wrenchCommand.SetPropulsiveRotationalEffortZ(steering);
                mLocalWaypointDriverComponent.Send(&wrenchCommand);
            }
        }
        else
        {
            // Make sure we release control if we don't need it.
            if(mLocalWaypointDriverComponent.AccessControlService()->HaveControl(mBaselineComponent.GetComponentID()))
            {
                mLocalWaypointDriverComponent.AccessControlService()->ReleaseComponentControl(mBaselineComponent.GetComponentID());
            }
        }
    }
    void UpdateSensorServices()
    {
        // Set the global pose of the robot.
        // This data is coming from the Primitive Driver service in
        // this example, because it is simulating the physics.
        mpGlobalPoseSensor->SetGlobalPose(mpPrimitiveDriver->mGlobalPose);
        // You can set local pose data using global pose
        // as it is automatically converted to local pose data for you.
        // How convenient!
        mpLocalPoseSensor->SetLocalPose(mpPrimitiveDriver->mGlobalPose);
        // Set an velocity state
        mpVelocityStateSensor->SetVelocityState(mpPrimitiveDriver->mVelocityState);
    }
    void UpdatePhysics(const double timeSeconds = 0.33)
    {
        mpPrimitiveDriver->UpdatePhysics(timeSeconds);
    }
    void UseFakeWaypoints();

    JAUS::UShort mSubsystemID;                                  ///<  Vehicle Number.
    JAUS::Component mBaselineComponent;                         ///<  Main component with basic services.
    JAUS::GlobalPoseSensor* mpGlobalPoseSensor;                 ///<  Global Pose service.
    JAUS::LocalPoseSensor* mpLocalPoseSensor;                   ///<  Local Pose service
    JAUS::VelocityStateSensor* mpVelocityStateSensor;           ///<  Velocity State service.
    RobotPrimitiveDriver* mpPrimitiveDriver;                    ///<  Robot primitive driver.

    JAUS::Component mLocalWaypointDriverComponent;              ///<  Waypoint driving component services.
    JAUS::LocalWaypointListDriver* mpLocalWaypointListDriver;   ///<  Local Waypoint List Driver service.
};


int main(int argc, char* argv[])
{
    RobotSim robot;

    // Add all services needed to component(s)
    // This must be done before component initialization.
    robot.AddServices();

    // Initialize the robot and JAUS services.
    if(robot.Initialize() == false)
    {
        return 0;
    }

    JAUS::Time::Stamp printTimeMs = 0;
    double timeDiff = 0.33;             // Used for simulation of robot physics.

    while(CxUtils::GetChar() != 27 && 
          robot.mBaselineComponent.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown &&
          robot.mLocalWaypointDriverComponent.ManagementService()->GetStatus() != JAUS::Management::Status::Shutdown)
    {
        // Update Waypoint Driver service.
        robot.UpdateWaypointDriver();
        // Update simulation physics.
        robot.UpdatePhysics(timeDiff);
        // Update sensor services with current position
        // and velocity information.
        robot.UpdateSensorServices();

        if(JAUS::Time::GetUtcTimeMs() - printTimeMs > 500)
        {
            // Print status of services.
            std::cout << "\n||||||||||||||||||||||||||||||||||||||||||||||||||||\n\n";
            robot.mBaselineComponent.AccessControlService()->PrintStatus(); std::cout << std::endl;
            robot.mBaselineComponent.ManagementService()->PrintStatus(); std::cout << std::endl;
            robot.mpLocalPoseSensor->PrintStatus(); std::cout << std::endl;
            robot.mpVelocityStateSensor->PrintStatus(); std::cout << std::endl;
            robot.mpPrimitiveDriver->PrintStatus(); std::cout << std::endl;
            std::cout << "======================================================\n";
            robot.mLocalWaypointDriverComponent.AccessControlService()->PrintStatus(); std::cout << std::endl;
            robot.mLocalWaypointDriverComponent.ManagementService()->PrintStatus(); std::cout << std::endl;
            robot.mpLocalWaypointListDriver->PrintStatus();
            printTimeMs = JAUS::Time::GetUtcTimeMs();
        }

        // Exit if escape key pressed.
        if(CxUtils::GetChar() == 27)
        {
            break;
        }

        CxUtils::SleepMs((unsigned int)(timeDiff*1000.0));
    }

    return 0;
}


RobotPrimitiveDriver::RobotPrimitiveDriver()
{
    Reset();
    // Set the initial global pose of the robot.
    mGlobalPose.SetLatitude(32.703356);
    mGlobalPose.SetLongitude(-117.253919);
    mGlobalPose.SetAltitude(300);
    mGlobalPose.SetPositionRMS(0.0);
    mGlobalPose.SetRoll(0.0);
    mGlobalPose.SetPitch(0.0);
    mGlobalPose.SetYaw(CxUtils::CxToRadians(45));
    mGlobalPose.SetAttitudeRMS(0.0);
    mGlobalPose.SetTimeStamp(JAUS::Time::GetUtcTime());

    // Set an initial velocity state.
    mVelocityState.SetVelocityX(0.0);
    mVelocityState.SetYawRate(0.0);
    mVelocityState.SetVelocityRMS(0.0);
    mVelocityState.SetTimeStamp(JAUS::Time::GetUtcTime());
}
RobotPrimitiveDriver::~RobotPrimitiveDriver()
{
    ReleaseControl();
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever the component transitions to a "Ready" state, this method
///          is called to signal the Service to transition to the "Ready" state.
///
///   In this example, the FakePrimtitiveDriver doesn't need to do anything.
///   However, in a real implementation, you may want to re-enable a connection
///   to your motors, or trigger a relay, etc.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::Resume()
{
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever the component receives a "Reset" command transition, this
///          is called to signal the Service to reset.
///
///   In this example, the FakePrimtitiveDriver goes back to an initial state
///   which in this case sets the thrust and steering to 0.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::Reset()
{
    mThrust = 0;
    mSteering = 0;
    // Send values to actual motor interface here...
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever the component receives a "Standby" command transition, this
///          is called to signal the Service to perform whatever behavior is 
///          needed to go to a Standby state.
///
///   In this example, the FakePrimtitiveDriver goes back to an initial state
///   which in this case sets the thrust and steering to 0.  In a real
///   application (e.g. ground robot) you would probably want your robot to
///   stop driving or turn on brakes, etc.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::Standby()
{
    mThrust = 0;
    mSteering = 0;
    // Send values to actual motor interface here...
    return false;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever the component receives a "Emergency" command transition, this
///          is called to signal the Service to perform whatever behavior is 
///          needed to go to an Emergency state.
///
///   In this example, the FakePrimtitiveDriver goes back to an initial state
///   which in this case sets the thrust and steering to 0.  In a real
///   application (e.g. ground robot) you would probably want your robot to
///   stop driving or turn on brakes, etc.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::SetEmergency()
{
    mThrust = 0;
    mSteering = 0;
    // Send values to actual motor interface here...
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever the component receives a "Clear Emergency" command transition, 
///          this is called to tell the service to transition out of the "Emergency"
///          state.
///
///   Once this is called, the component will transition back to a Standy state
///   automatically.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::ClearEmergency()
{
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called whenever control is released (or timed out) from
///   a controlling component.  It is up to you to implement whatever behavior
///   you want when this happens.
///
///   In this example, the FakePrimtitiveDriver sets the thrust and steering back
///   to zero so that our fake ground robot stops driving.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::ReleaseControl()
{
    mThrust = 0;
    mSteering = 0;
    // Send values to actual motor interface here...
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called whenever a controlling component sends a
///          Wrench Effort command.  It is up to the developer to implement
///          that command based on their hardware, etc.
///
///   \return True on success (command processed), false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RobotPrimitiveDriver::SetWrenchEffort(const JAUS::SetWrenchEffort* command)
{
    bool result = false;
    // Check to see if we are in a "Ready" state, and if so
    // implement the command.
    JAUS::Byte status = GetComponent()->ManagementService()->GetStatus();
    if(status == JAUS::Management::Status::Ready)
    {
        if(command->IsFieldPresent(JAUS::SetWrenchEffort::PresenceVector::PropulsiveLinearEffortX))
        {
            mThrust = command->GetPropulsiveLinearEffortX();
            result = true;
        }
        if(command->IsFieldPresent(JAUS::SetWrenchEffort::PresenceVector::PropulsiveRotationalEffortZ))
        {
            mSteering = command->GetPropulsiveRotationalEffortZ();
            result = true;
        }
    }

    return result;
}  
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This is a helper method to simulate some physics.
///
////////////////////////////////////////////////////////////////////////////////////
void RobotPrimitiveDriver::UpdatePhysics(const double timeSeconds)
{
    // Simulate some simple physics based on 
    // desired mThrust and mSteering 
    // to reach local waypoints.
        
    double linearDistance = timeSeconds*mThrust*0.025;
    double linearVelocity = linearDistance/timeSeconds;
    double rotation = timeSeconds*mSteering*CxUtils::CxToRadians(1);
    double rotationRate = rotation/timeSeconds;
    double yaw = mGlobalPose.GetYaw();
        
    JAUS::Point3D localPosChange(linearDistance, 0, 0);        
    localPosChange = localPosChange.Rotate(yaw, JAUS::Point3D::Z);
        
    CxUtils::Wgs worldPositionWgs(mGlobalPose.GetLatitude(),
                                    mGlobalPose.GetLongitude(),
                                    mGlobalPose.GetAltitude());
    CxUtils::Utm worldPositionUtm(worldPositionWgs);
        
    worldPositionUtm.mNorthing += localPosChange.mX;
    worldPositionUtm.mEasting += localPosChange.mY;
    yaw = CxUtils::Orientation::AddToAngle(yaw, rotation);
    // Convert to WGS
    worldPositionWgs << worldPositionUtm;
        
    // Save newly calculated position and orientation.
    mGlobalPose.SetYaw(yaw);
    mGlobalPose.SetLatitude(worldPositionWgs.mLatitude);
    mGlobalPose.SetLongitude(worldPositionWgs.mLongitude);
    mGlobalPose.SetAltitude(worldPositionWgs.mElevation);
    mGlobalPose.SetTimeStamp(JAUS::Time(true));
        
    mVelocityState.SetVelocityX(linearVelocity);
    mVelocityState.SetYawRate(rotationRate);
    mVelocityState.SetTimeStamp(JAUS::Time(true));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates fake waypoints to simulate control unit for testing.
///
////////////////////////////////////////////////////////////////////////////////////
void RobotSim::UseFakeWaypoints()
{
    JAUS::Address controllerID(42, 1, 1);
    mLocalWaypointDriverComponent.AccessControlService()->SetControllerID(controllerID);
    mLocalWaypointDriverComponent.ManagementService()->SetStatus(JAUS::Management::Status::Ready);

    JAUS::SetElement command(mLocalWaypointDriverComponent.GetComponentID(), controllerID);
    command.SetRequestID(1);
    JAUS::Point3D::List localWaypoints;
    // Add more waypoints for testing as needed.
    localWaypoints.push_back(JAUS::Point3D(10, 0, 0));
    localWaypoints.push_back(JAUS::Point3D(10, 10, 0));
    localWaypoints.push_back(JAUS::Point3D(0, 10, 0));
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
    
    // Set the command.
    mLocalWaypointDriverComponent.TransportService()->Receive(&command);
    // Now tell it to start executing.
    JAUS::ExecuteList executeCommand(mLocalWaypointDriverComponent.GetComponentID(), controllerID);
    executeCommand.SetElementUID(1); // Start at beginning.
    executeCommand.SetSpeed(2.0);    // Maximum speed.
    mLocalWaypointDriverComponent.TransportService()->Receive(&executeCommand);
}

/* End of File */
