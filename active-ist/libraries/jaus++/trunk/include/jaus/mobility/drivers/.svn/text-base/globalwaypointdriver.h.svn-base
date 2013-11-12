////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalwaypointdriver.h
///  \brief This file contains the definition of the GlobalWaypointDriver class,
///         used as an interface for the driving of platform to a target waypoint.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 10 February 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#ifndef __JAUS_MOBILITY_GLOBAL_WAYPOINT_DRIVER__H
#define __JAUS_MOBILITY_GLOBAL_WAYPOINT_DRIVER__H

#include "jaus/core/management/management.h"
#include "jaus/mobility/drivers/settravelspeed.h"
#include "jaus/mobility/drivers/setglobalwaypoint.h"
#include "jaus/mobility/drivers/querytravelspeed.h"
#include "jaus/mobility/drivers/queryglobalwaypoint.h"
#include "jaus/mobility/drivers/reporttravelspeed.h"
#include "jaus/mobility/drivers/reportglobalwaypoint.h"
#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/mobility/sensors/velocitystatesensor.h"

namespace JAUS
{
    typedef JAUS::ReportTravelSpeed TravelSpeed;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class GlobalWaypointDriver
    ///   \brief The Global Waypoint Driver allows for the driving of platform to a
    ///          target waypoint.
    ///
    //    The function of the Global Waypoint Driver is to move the platform given a
    ///   single target waypoint, desired travel speed, current platform pose and
    ///   current velocity state.  A single waypoint is provided via the Set Global
    ///   Waypoint message.  The waypoint remains unchanged until a new Set Global
    ///   Waypoint message is received.  A waypoint consists of the desired position
    ///   and orientation of the platform.  The second input consists of the desired
    ///   travel speed.  The travel speed remains unchanged unless a new Set Travel
    ///   Speed Message is received.  The travel speed may then be changed at any time
    ///   during waypoint navigation.  The travel speed is reset to zero for all
    ///   transitions from the Ready State.
    ///
    ///   <b>This implmentation of this service requires at a minimum a Global Pose
    ///      and Velocity State sensor service on the same component that this 
    ///      service belongs to.  The sensor services can be synchronizing versions
    ///      of the services also (see example_synchronize.cpp for how to do this).</b>
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL GlobalWaypointDriver : public Management::Child
    {
    public:
        const static std::string Name; ///< String name of the Service.
        GlobalWaypointDriver();
        virtual ~GlobalWaypointDriver();
        // Overloaded by the inheriting class to generate commands for the controlled driver.
        virtual Message* GenerateDriveCommand(const Byte status) = 0;
        // Overloaded by the inheriting class to generate default commands for the controlled driver.
        virtual Message* GenerateIdleDriveCommand(const Byte status) const = 0;
        // Returns true if target waypoint is achieved.
        virtual bool IsWaypointAchieved(const GlobalPose& currentPose,
                                        const JAUS::SetGlobalWaypoint& desiredWaypoint) const = 0;
        // Virtual method called whenever waypoint driver updates, overload to add additional behaviors/checks needed.
        virtual void WaypointDriverUpdateEvent(const unsigned int timeSinceLastUpdateMs) {}
        // Method is called when desired waypoint is achieved.
        virtual void WaypointAchieved(const JAUS::SetGlobalWaypoint& waypoint) = 0;
        // Method called whenever a Set Global Waypoint command is received by service.
        virtual bool SetGlobalWaypoint(const JAUS::SetGlobalWaypoint* command);
        // Method called whenever a Set Travel Speed command is received by service, overload for extra functionality.
        virtual bool SetDesiredTravelSpeed(const JAUS::SetTravelSpeed* command) {return true;}
        // Change the Desired Travel Speed without sending a SetTravelSpeed command
        virtual bool SetDesiredTravelSpeed(const double speed);
        // Try to take control of lower-level (e.g. primitive) driver, in order to move to waypoint.
        virtual bool SetDriverToControl(const Address& driver);
        // Gets the current GlobalWaypoint command.
        virtual JAUS::SetGlobalWaypoint GetGlobalWaypoint() const;
        // Gets the current GlobalPoseSensor report.
        virtual GlobalPose GetGlobalPose() const;
        // Gets the current velocity state report.
        virtual VelocityState GetVelocityState() const;
        // Gets the current travel speed command.
        virtual JAUS::SetTravelSpeed GetDesiredTravelSpeed() const; 
        // Gets the time when the last wrench effort command was received.
        virtual Time GetGlobalWaypointTime() const;
        // Gets the time when the last travel speed command was received.
        virtual Time GetTravelSpeedTime() const;
        // Gets the address of the lower-level (e.g. primitive) driver being controlled.
        virtual Address GetControlledDriverID() const;        
        // Generates Global Waypoint Driver related events.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Adds support for Report Global Waypoint events.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // By default, the Global Waypoint Driver is discoverable to other components (overload to hide).
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenever a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Prints information about the service.
        virtual void PrintStatus() const;
        // Method called when transitioning to a ready state.
        virtual bool Resume();
        // Method called to transition due to reset.
        virtual bool Reset();
        // Method called when transitioning to a standby state.
        virtual bool Standby();
        // Method called when transitioning to an emergency state.
        virtual bool SetEmergency();
        // Method called when leaving the emergency state.
        virtual bool ClearEmergency();
        // Method called when control is released.
        virtual bool ReleaseControl();
    private:
        // In this method, drive commands are generated.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Creates a ReportGlobalWaypoint from QueryGlobalWaypoint.
        void CreateReportFromQuery(const QueryGlobalWaypoint* query, 
                                   ReportGlobalWaypoint& report) const;
        // Creates a ReportTravelSpeed from QueryTravelSpeed.
        void CreateReportFromQuery(const QueryTravelSpeed* query, 
                                   TravelSpeed& report) const;
        // Checks if Driver is controlled, acquire control if necessary.
        bool CheckDriver();
        // Checks if Sensor data is availabe, acquire subscriptions if necessary.
        bool CheckSensors();
        // Removes Control of driver and stops subscriptions.
        bool ReleaseResources();
        bool mWaypointAchievedFlag;                         ///<  Flag indicating whether a Waypoint is achieved after being set.
        Mutex mGlobalWaypointDriverMutex;                   ///<  Mutex for thread protection of data.
        Address mControlledDriverID;                        ///<  Address of the controlled driver used for waypoint driving.
        GlobalPoseSensor* mpGlobalPoseSensor;               ///<  Pointer to the Velocity State Sensor if service is directly accessible.
        VelocityStateSensor* mpVelocityStateSensor;         ///<  Pointer to the Velocity State Sensor if service is directly accessible.
        Time mGlobalWaypointTime;                           ///<  Time when the last target waypoint was received.
        Time mTravelSpeedTime;                              ///<  Time when the last travel speed was received.
        JAUS::SetGlobalWaypoint mGlobalWaypoint;            ///<  The last target waypoint received.
        JAUS::SetTravelSpeed mDesiredTravelSpeed;           ///<  The last travel speed received.
        GlobalPose mGlobalPose;                             ///<  The last global pose reported by the Global Pose Sensor.
        VelocityState mVelocityState;                       ///<  The last Velocity State reported by the Velocity State Sensor.
    };
}

#endif
/*  End of File */
