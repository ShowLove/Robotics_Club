////////////////////////////////////////////////////////////////////////////////////
///
///  \file subsystemcommand.h
///  \brief Contains the Subsystem service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 10 February 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_SUBSYSTEM_COMMAND__H
#define __JAUS_EXTRAS_SUBSYSTEM_COMMAND__H


#include "jaus/extras/jausextrasdll.h"
#include "jaus/core/management/management.h"
#include "jaus/core/discovery/vehicle.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SubsystemCommand
    ///   \brief Service for command and control of single or multiple subsystems.
    ///
    ///   It is up to the developer to add specific other options and capabilities
    ///   to this Service.  However, it does add capabilities that are most likely
    ///   common for all command interfaces.  For example, SubsystemCommand will 
    ///   collect Global Pose and state information for all discovered Mobility
    ///   platforms.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL SubsystemCommand : public Management::Child,
                                             public Discovery::Callback
    {
    public:
        const static std::string Name;  ///< Name of the Service.
        SubsystemCommand(const Service::ID& id);
        virtual ~SubsystemCommand();
        // Called upon initialization (if overloaded, call parent method).
        virtual void Initialize();
        // Sets the type of global pose subscriptions to make.
        void SetGlobalPoseSubscriptionType(const bool everyChange = false, 
                                           const double desiredPeriodicRateHz = 25);
        // Method called when transitioning to a ready state (overload to add behavior).
        virtual bool Resume() { return true; };
        // Method called to transition due to reset (overload to add behavior).
        virtual bool Reset() { return true; };
        // Method called when transitioning to a standby state (overload to add behavior).
        virtual bool Standby() { return true; };
        // Method called when transitioning to an emergency state (overload to add behavior).
        virtual bool SetEmergency() { return true; };
        // Method called when leaving the emergency state (overload to add behavior).
        virtual bool ClearEmergency() { return true; };
        // Method called when control is released (overload to add behavior).
        virtual bool ReleaseControl() { return true; };
        // Gets the Discovery service.
        const Discovery* DiscoveryService() const;
        // Gets the Discovery service.
        Discovery* DiscoveryService();
        // Gets the Discovery service.
        const AccessControl* AccessControlService() const;
        // Gets the Discovery service.
        AccessControl* AccessControlService();
        // Sends a command to a component by taking control (if not established) first.
        virtual bool SendCommand(const Message* commandMessage,
                                 const bool sendResumeFlag,
                                 const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Method called whenever this service acquires control of a component.
        virtual void ProcessAcquisitionOfControl(const Address& controlledComponent) {}
        // This is not a discoverable service at this time.
        virtual bool IsDiscoverable() const { return false; }
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const { return false; }
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const { return false; }
        // Receives global pose data and updates status of subsystems in Discovery Service.
        virtual void Receive(const Message* message);
        // Creates messages supported by the Service.
        virtual Message* CreateMessage(const UShort messageCode) const;  
        // When called, checks to see if new subscriptions need to be made.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Prints information about the service.
        virtual void PrintStatus() const;
    protected:
        bool mOnChangeEventsFlag;       ///<  If true, use EveryChange events for Global Pose.
        double mDesiredPeriodicRateHz;  ///<  If requesting periodic events, this is the desired frequency.
    private:
        virtual void ProcessSystemState(const Subsystem::Map& system);
    };
}

#endif
/*  End of File */
