////////////////////////////////////////////////////////////////////////////////////
///
///  \file accesscontrol.h
///  \brief Contains the Access Control Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 22 December 2009
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
#ifndef __JAUS_CORE_ACCESS_CONTROL__H
#define __JAUS_CORE_ACCESS_CONTROL__H

#include "jaus/core/discovery/discovery.h"
#include "jaus/core/control/confirmcontrol.h"
#include "jaus/core/control/queryauthority.h"
#include "jaus/core/control/querycontrol.h"
#include "jaus/core/control/querytimeout.h"
#include "jaus/core/control/rejectcontrol.h"
#include "jaus/core/control/releasecontrol.h"
#include "jaus/core/control/reportauthority.h"
#include "jaus/core/control/reportcontrol.h"
#include "jaus/core/control/reporttimeout.h"
#include "jaus/core/control/requestcontrol.h"
#include "jaus/core/control/setauthority.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class AccessControl
    ///   \brief This service offers a basic interface for acquiring preemptable
    ///          exclusive control to one or more related services that
    ///          utilize this function.  
    ///          
    ///   Once exclusive control is established, the related services shall only
    ///   execute commands originating from the controlling component.  Service
    ///   is always granted to components of higher authority that are 
    ///   requesting exclusive control, all other Command messages will be
    ///   ignored.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL AccessControl : public Events::Child
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used to be notified when AccessControl service loses
        ///          control of a component or a component releases control of the
        ///          Service.
        ///
        //////////////////////////////////////////////////////////////////////////////////// 
        class JAUS_CORE_DLL Callback : public Discovery::Callback
        {
        public:
            typedef std::set<Callback* > Set;
            Callback() {}
            virtual ~Callback() {}
            // Method called when we lose control of a component.
            virtual void ProcessLossOfControl(const Address& controlledComponent) {}
            // Method called when we gain, or re-gain control of a component.
            virtual void ProcessAcquisitionfControl(const Address& controlledComponent) {}
            // Method called when when the controller release control of this service.
            virtual void ProcessReleaseOfControl(const Address& controllerID) {}
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Child
        ///   \brief Class that all direct child Services of AccessControl must inherit
        ///          from to support state transitions in control and system management.
        ///
        //////////////////////////////////////////////////////////////////////////////////// 
        class JAUS_CORE_DLL Child : public Events::Child
        {
        public:
            Child(const ID& serviceIdentifier, 
                  const ID& parentServiceIdentifier) : Events::Child(serviceIdentifier, 
                                                                     parentServiceIdentifier) {}
            virtual ~Child() {}
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief This method is called whenever control of the parent component for
            ///          your services has been requested.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual bool RequestControl() { return true; }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Method called whenever control of the parent component has been
            ///          released.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual bool ReleaseControl() { return true; }
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief This method is called whenever the parent component (or service) 
            ///          establishes control of a component (or re-establishes).
            ///
            ///   Overload this method to be notified when control is established so you can
            ///   send Resume commands, etc.
            ///
            ///   \param[in] controlledComponent The ID of the component control has been
            ///                                  acquired for.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual void ProcessAcquisitionOfControl(const Address& controlledComponent) {}
        };
        const static std::string Name; ///< String name of the Service.
        // Constructor.
        AccessControl();
        // Destructor.
        ~AccessControl();
        // Method called to check if the command can be processed.
        bool AcceptCommandMessage(const Message* commandMessage) const;
        // Loads settings from XML specific to the Service.
        virtual bool LoadSettings(const std::string& filename);
        // Shutsdown the Service.
        virtual void Shutdown();
        // Method called to take control of a component.
        bool RequestComponentControl(const Address& id, 
                                     const bool reacquire = true,
                                     const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Method to release control
        bool ReleaseComponentControl(const Address& id,
                                     const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Method to turn off control abilities from other compoments (default is on).
        void SetControllable(const bool on = true);
        // Is the Control of Service available?
        inline bool IsControllable() const { return mControllableFlag; }
        // Access Control is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the Access Control Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the Access Control Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Sets the authority level of the Service.
        void SetAuthorityCode(const Byte authorityCode);
        // Gets the authority level of the Service.
        inline Byte GetAuthorityCode() const { return mAuthorityCode; }
        // Method to check if control has been established.
        bool HaveControl(const Address& id) const;
        // Method to check if Service/Component is being controlled.
        bool IsControlled() const;
        // Method to force control of the component.
        void SetControllerID(const Address& id);
        // Method to get the ID of the controlling component.
        Address GetControllerID() const;
        // Method to get the authority code of controllign component.
        Byte GetControllerAuthorityCode() const;
        // Gets the components being controlled by the Component.
        Address::List GetControlledComponents() const;
        // When called verifies status of controlled components.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);  
        // Get the amount of time before a timeout occurs (seconds).
        Byte GetTimeoutPeriod() const;
        // Sets the amount of time before a timeout occurs (seconds).
        void SetTimeoutPeriod(const Byte timeSeconds);
        // Get the amount of time between checks for control of components.
        Time::Stamp GetControlCheckPeriod() const;
        // Get the last time a command was recived from the controlling component.
        Time GetControllerUpdateTime() const { return mControllerUpdateTime; }
        // Register to receive updates of subsystems (add or removes callback).
        void RegisterCallback(Callback* callback, const bool add = true);   
        // Prints status about the service.
        virtual void PrintStatus() const;
    protected:
        bool mControllableFlag;                 ///<  Is the Service controllable?
        Byte mAuthorityCode;                    ///<  Authority code of the service.
        SharedMutex mControlMutex;              ///<  Mutex for thread protection of data.
        Address::Set mControlledComponents;     ///<  Components being controlled.
        Address::Set mToReleaseControl;         ///<  Components control is being released for.
        std::map<Address, bool> mMaintainFlags; ///<  Flags to indicate control should be re-established.
        std::map<Address, bool> mControlFlags;  ///<  Flags to indicate which components we have verified control of.
        Address mControllerID;                  ///<  ID of the controlling component.
        Byte mControllerAuthorityCode;          ///<  Authority code of controlling component.
        Time mControllerUpdateTime;             ///<  The last time the controller sent a command message.
        Time mControllerCheckTime;              ///<  The last time the controller updated control.
        Byte mTimeoutPeriod;                    ///<  Timeout period in seconds.
        double mTimeoutThreshold;               ///<  Threshold for timeout requests/checking (percentage of timeout period).
        std::map<Address, Byte> mTimeoutPeriods;///<  Timeout periods for controlled components.
        std::map<Address, Time> mControlCheckTimes;     ///<  The last time a control check occurred for controlled components.
        std::map<Address, Time> mControlConfirmTimes;   ///<  The last time when control was confirmed froma controlled component.
        Callback::Set mCallbacks;                       ///<  Callbacks for control events.
        SharedMutex mCallbacksMutex;                    ///<  Mutex for thread protection of callbacks.
    private:
        void EraseComponentControlInfo(const Address& id);
    };
}

#endif
/*  End of File */
