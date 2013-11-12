////////////////////////////////////////////////////////////////////////////////////
///
///  \file discovery.h
///  \brief Contains the Discovery Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 November 2009
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
#ifndef __JAUS_CORE_DISCOVERY__H
#define __JAUS_CORE_DISCOVERY__H

#include "jaus/core/discovery/vehicle.h"
#include "jaus/core/events/events.h"
#include "jaus/core/transport/transport.h"
#include "jaus/core/discovery/queryconfiguration.h"
#include "jaus/core/discovery/queryidentification.h"
#include "jaus/core/discovery/queryservices.h"
#include "jaus/core/discovery/querysubsystemlist.h"
#include "jaus/core/discovery/registerservices.h"
#include "jaus/core/discovery/reportconfiguration.h"
#include "jaus/core/discovery/reportidentification.h"
#include "jaus/core/discovery/reportservices.h"
#include "jaus/core/discovery/reportsubsystemlist.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Discovery
    ///   \brief The Discovery Service implementation of the Core Service Set
    ///          library.
    ///
    ///   The process of discovery is conducted at both the node level and the
    ///   subsystem level.  This service supports the discovery of both legacy
    ///   components defined the JAUS Reference Architecture 3.2+, and new components.
    ///   The Component IDs of legacy components were fixed at specification time
    ///   (Primitive Driver == 33 for example) and could only contain one
    ///   service beyond the core service support.  New components may use any
    ///   component ID that is not used in the Reference Architecture for legacy
    ///   component definitions.  New components can also contain two or more services
    ///   beyond the core service support.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Discovery : public Events::Child
    {
    public:
        const static std::string Name; ///< String name of the Service.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used to be notified when a Subsystem is discovered,
        ///          updated, or disconnects from system.
        ///
        ////////////////////////////////////////////////////////////////////////////////////        
        class JAUS_CORE_DLL Callback : public Transport::Callback
        {
        public:
            typedef std::set<Callback* > Set;
            Callback() {}
            virtual ~Callback() {}  
            virtual void ProcessSystemState(const Subsystem::Map& system){};
        };
        typedef std::map<UShort, std::string> List; ///<  List of discovered subsystems.
        Discovery();
        ~Discovery();
        // Loads settings from XML specific to the Service.
        virtual bool LoadSettings(const std::string& filename);
        // Initializes the service.
        virtual void Initialize();
        // Shutsdown the Service.
        virtual void Shutdown();
        // Events service is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the Events service.
        virtual void Receive(const Message* message);  
        // Creates messages associated with the events service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // When called verifies that no subscriptions have been lost.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Sets the name of the component.
        void SetComponentIdentification(const std::string& identification) { mComponentIdentification = identification; }
        // Sets the name of the node.
        void SetNodeIdentification(const std::string& identification) { mNodeIdentification = identification; }
        // Sets the identification of the subsystem.
        void SetSubsystemIdentification(const Subsystem::Type type,
                                        const std::string& identification);
        // Sets how often to broadast messages for discovery.
        bool SetDiscoveryFrequency(const double frequencyHz);
        // Gets the discovery frequency
        double GetDiscoveryFrequency() const { return 1000.0/mBroadcastDelayMs; }
        // Sets whether or not to perform a global query for other subsystems (default is on).
        void DiscoverSubsystems(const bool enable = true) { mDiscoverSubsystemsFlag = enable; }
        // Gets the subsystem type.
        Subsystem::Type GetSubsystemType() const { return mSubsystemType; }
        // Gets this subsystem identification.
        std::string GetSubsystemIdentification() const { return mSubsystemIdentification; }
        // Gets this subsystem identification.
        std::string GetNodeIdentification() const { return mNodeIdentification; }
        // Gets this subsystem identification.
        std::string GetComponentIdentification() const { return mComponentIdentification; }
        // Gets a list of subsystems by name and ID.
        List GetSubsystemList() const;
        // Gets a list of subsystems (that are Vehicles) by name and ID.
        List GetVehicleList() const;
        // Gets a list of component IDs with a service.
        Address::List GetComponentsWithService(const std::string& serviceName) const;
        // Gets a copy of a specific subsystem configuration.
        Subsystem::Ptr GetSubsystem(const UShort id) const;
        // Gets a copy of a specific subsystem configuration.
        Subsystem::Ptr GetSubsystem(const Address& id) const { return GetSubsystem(id.mSubsystem); }
        // Gets a copy of vehicle data discovered for a subsystem.
        Vehicle::Ptr GetVehicle(const UShort id) const;
        // Gets a copy of vehicle data discovered for a subsystem.
        Vehicle::Ptr GetVehicle(const Address& id) const { return GetVehicle(id.mSubsystem); }
        // Method to set the value of the state of JAUS component in the subsystem list
        bool SetComponentState(const Address& id, const Byte componentState);
        // Method to update the position, attitude, and velocity of a vehicle.
        bool SetVehiclePosition(const UShort id, 
                                const Wgs& position, 
                                const Time& time = Time(true));
        // Method to update the position, attitude, and velocity of a vehicle.
        bool SetVehicleAttitude(const UShort id, 
                                const Attitude& attitude, 
                                const Time& time = Time(true));
        // Method to update the position, attitude, and velocity of a vehicle.
        bool SetVehiclePose(const UShort id, 
                            const Wgs& wgs,
                            const Attitude& attitude, 
                            const Time& time = Time(true));
        // Method to update the position, attitude, and velocity of a vehicle.
        bool SetVehicleLinearVelocity(const UShort id, 
                                      const Point3D& velocity, 
                                      const Time& time = Time(true));
        // Method to update user specific data stored in a vehicle structure.
        bool SetCustomVehicleData(const UShort id, 
                                  const Vehicle::Info* info);
        // Gets copy of all the Subsystem data
        void GetSubsystems(Subsystem::Map& subsystems) const;
        // Gets copy of all the Vehicle data
        void GetVehicles(Vehicle::Map& vehicles) const;
        // Register to receive updates of subsystems (add or removes callback).
        void RegisterCallback(Callback* callback, const bool add = true);     
        // Prints system configuration information to the console.
        void PrintSystemConfiguration() const;
        // Prints information about the service.
        virtual void PrintStatus() const;
        // Triggers discovery callbacks.
        void TriggerDiscoveryCallbacks();
        // Sets a list of subsystems that you want Discovery to find (ignoring all others).
        void SetSubsystemsToDiscover(const std::set<UShort>& toDiscover);
        // Gets the set of subsystems you want to discover (if empty, all are discovered).
        std::set<UShort> GetSubsystemsToDiscover() const;
    private:
        volatile bool mDiscoverSubsystemsFlag;  ///<  If true, subsystems are also discovered.
        volatile bool mTriggerCallbacksFlag;    ///<  If true, trigger discovery callbacks.
        SharedMutex mSubsystemDataMutex;        ///<  Mutex for protection of subsystem data.
        SharedMutex mCallbacksMutex;            ///<  Mutex for protection of callback data.
        unsigned int mBroadcastDelayMs;         ///<  Time between broadcasts in ms.
        Time::Stamp mBroadcastTimeMs;           ///<  The last time a discovery query was broadcast.
        Subsystem::Map mSystem;                 ///<  System configuration.
        Callback::Set mCallbacks;               ///<  Discovery callbacks.
        std::string mComponentIdentification;   ///<  Identification string of the component [255 char max].
        std::string mNodeIdentification;        ///<  Identification string of the node [255 char max].
        std::string mSubsystemIdentification;   ///<  Identification string of the subsystem.
        Subsystem::Type mSubsystemType;         ///<  Subsystem type information.
        Address::Set mSubsystemList;            ///<  List of components broadcasting globally for subsystem discovery.
        std::set<UShort> mSubsystemsToDiscover; ///<  Subsystems to discover.
    };
}

#endif
/*  End of File */
