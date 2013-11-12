////////////////////////////////////////////////////////////////////////////////////
///
///  \file management.h
///  \brief Contains the Management Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 December 2009
///  <br>Copyright (c) 2009
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
#ifndef __JAUS_CORE_MANAGEMENT__H
#define __JAUS_CORE_MANAGEMENT__H

#include "jaus/core/control/accesscontrol.h"
#include "jaus/core/management/shutdown.h"
#include "jaus/core/management/reset.h"
#include "jaus/core/management/resume.h"
#include "jaus/core/management/standby.h"
#include "jaus/core/management/setemergency.h"
#include "jaus/core/management/clearemergency.h"
#include "jaus/core/management/querystatus.h"
#include "jaus/core/management/reportstatus.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Management
    ///   \brief The Management Service provides a state machine for the component
    ///          life-cycle maangement to help clients understand how the component
    ///          will react to commands and queries.
    ///
    ///   This implementation controls that status of the Component (e.g. ready, standby)
    ///   and has method to change the state of components.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Management : public AccessControl::Child
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Status
        ///   \brief Namespace for different states the Services can be in.
        ///
        //////////////////////////////////////////////////////////////////////////////////// 
        class Status
        {
        public:
            static const Byte Initialized = 0;
            static const Byte Ready       = 1;
            static const Byte Standby     = 2;
            static const Byte Shutdown    = 3;
            static const Byte Failure     = 4;
            static const Byte Emergency   = 5;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Child
        ///   \brief Class that all direct child Services of Management must inherit
        ///          from to support state transitions.
        ///
        //////////////////////////////////////////////////////////////////////////////////// 
        class JAUS_CORE_DLL Child : public AccessControl::Child
        {
            friend class Management;
        public:
            Child(const ID& serviceIdentifier, 
                  const ID& parentServiceIdentifier) : AccessControl::Child(serviceIdentifier, 
                                                                            parentServiceIdentifier) { mStatus = 0; }
            virtual ~Child() {}
            // Method called when transitioning to a ready state.
            virtual bool Resume() = 0;
            // Method called to transition due to reset.
            virtual bool Reset() = 0;
            // Method called when transitioning to a standby state.
            virtual bool Standby() = 0;
            // Method called when transitioning to an emergency state.
            virtual bool SetEmergency() { return true; }
            // Method called when leaving the emergency state.
            virtual bool ClearEmergency() { return true; }
            // Method called when control is released.
            virtual bool ReleaseControl() { return AccessControl::Child::ReleaseControl(); }
            // Method to get the Status of the component (ready, standby, etc.)
            inline Byte GetStatus() const { return mStatus; }
            // Method called to take control of a component.
            bool RequestComponentControl(const Address& id, 
                                         const bool sendResumeCommand,
                                         const bool reacquire = false,
                                         const unsigned int waitTimeMs = Service::DefaultWaitMs);
            // Method to release control
            bool ReleaseComponentControl(const Address& id,
                                         const bool sendStandbyCommand,
                                         const unsigned int waitTimeMs = Service::DefaultWaitMs);
        protected:
            volatile Byte mStatus;  ///<  State status of the component (ready, standby, etc.)
        };
        const static std::string Name; ///< String name of the Service.
        // Constructor.
        Management();
        // Destructor.
        ~Management();
        // Send resume command, and confirm state change.
        virtual bool Resume(const Address& id, 
                            const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Send standby command, and confirm state change.
        virtual bool Standby(const Address& id, 
                             const unsigned int waitTimeMs = 0);
        // Send set emergency command, and confirm state change.
        virtual bool SetEmergency(const Address& id, 
                                  const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Send clear emergenncy command, and confirm state change.
        virtual bool ClearEmergency(const Address& id, 
                                    const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Send shutdown command.
        virtual bool Shutdown(const Address& id);
        // Called on initialize
        virtual void Initialize();
        // Shutsdown the Service.
        virtual void Shutdown();
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Management is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the Management Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the Management Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Sets the state of the Service.
        bool SetStatus(const Byte state);
        // Gets the state of this components status.
        Byte GetStatus() const { return mStatus; }
        // Gets the state/status of a specific component.
        int GetComponentStatus(const Address& id, const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Transition state when control is requested.
        virtual bool RequestControl() { return mStatus != Status::Emergency ? true : false; }
        // Transition state when control is released.
        virtual bool ReleaseControl();
        // Prints status about the Service.
        virtual void PrintStatus() const;
        // Prints status about the Service.
        virtual void PrintStatus(const bool allData) const;
        // Method called to check status of things periodically.
        void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
    protected:
        Time::Stamp mCheckStatusTimeMs;             ///<  The last time status was checked for subsystem.
        volatile Byte mStatus;                      ///<  Status of the service (what state are we in).
        SharedMutex mStatesMutex;                   ///<  Mutex for states information.
        std::map<Address, int> mSubsystemStates;    ///<  States/Status of components for this subsystem.
    };
}

#endif
/*  End of File */
