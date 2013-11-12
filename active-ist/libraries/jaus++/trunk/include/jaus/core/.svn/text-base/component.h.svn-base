////////////////////////////////////////////////////////////////////////////////////
///
///  \file component.h
///  \brief Contains the Component class implementation.
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
#ifndef __JAUS_CORE_COMPONENT__H
#define __JAUS_CORE_COMPONENT__H

#include "jaus/core/transport/transport.h"
#include "jaus/core/events/events.h"
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/discovery/discovery.h"
#include "jaus/core/control/accesscontrol.h"
#include "jaus/core/management/management.h"
#include "jaus/core/time/timeservice.h"
#include <cxutils/ipc/mappedmemory.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Component
    ///   \brief The Component class is a functional unit in the JAUS architecture
    ///          which includes single or Multiple services as a collection.
    ///
    ///   All Services are owned by a component, which defines the ID used for 
    ///   communications by the Transport Service.  By default, a Component supports
    ///   the entire Core service set defined by AS5710 standard.  It is up to
    ///   the developer to disable any of the Core Services they do not wish
    ///   to use.  See the example_component.cpp program for basic usage of
    ///   this class.
    ///
    ///   New Services are added using the AddService method, however this must be
    ///   done before a Component is initialized using the Initialize method.  It is
    ///   best practice to add new Services before any other operations are called on
    ///   the Component class, that way proper settings and initialization procedures
    ///   can take place.
    ///
    ///   By default, all Components use the JUDP Transport Service. However, this
    ///   can be changed by adding a new Transport Service using the AddService
    ///   method before Initialize is called.
    ///
    ///   Once added, a Service cannot be removed from a Component, however it can
    ///   be disabled using the EnableService method.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Component
    {
    public:
        // Constructor.
        Component();
        // Destructor.
        virtual ~Component();
        // Initializes the component with a given ID.
        virtual bool Initialize(const Address& id, 
                                const double serviceUpdateFrequency = 10.0);    
        // Initializes the component with a given ID.
        virtual bool InitializeWithUniqueID(const unsigned int waitTimeMs = 3000,
                                            const double serviceUpdateFrequency = 10.0); 
        // Check to see if component is initialized.
        inline bool IsInitialized() const { return mpTransportService->IsInitialized(); }
        // Loads settings for the component and it's services.
        bool LoadSettings(const std::string& filename);
        // Shutsdown the Component.
        void Shutdown();
        // Gets a pointer to the service.
        inline Transport* TransportService() { return mpTransportService; }
        // Gets a pointer to the service.
        Events* EventsService();
        // Gets a pointer to the service.
        Liveness* LivenessService();
        // Gets a pointer to the service.
        Discovery* DiscoveryService();
        // Gets a pointer to the service.
        AccessControl* AccessControlService();
        // Gets a pointer to the service.
        Management* ManagementService();
        // Gets a pointer to the service.
        TimeService* GetTimeService();
        // Gets a pointer to the service.
        inline const Transport* TransportService() const { return mpTransportService; }
        // Gets a pointer to the service.
        const Events* EventsService() const;
        // Gets a pointer to the service.
        const Liveness* LivenessService() const;
        // Gets a pointer to the service.
        const Discovery* DiscoveryService() const;
        // Gets a pointer to the service.
        const AccessControl* AccessControlService() const;
        // Gets a pointer to the service.
        const Management* ManagementService() const;
        // Gets a pointer to the service.
        const TimeService* GetTimeService() const;
        // Sends a message.
        bool Send(const Message* message, const int broadcastFlags = Service::NoBroadcast) const;
        // Sends a message, then waits for a response.
        bool Send(const Message* message, 
                  Message* response, 
                  const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Toggles a Service on or off.  By default, all services are enabled.
        bool EnableService(const std::string& name, const bool enable = true);
        // Adds a service to the component.  Service pointer is owned by Component.
        bool AddService(Service* service);
        // Gets a pointer to a service.
        Service* GetService(const std::string& name);
        // Gets a pointer to a service.
        const Service* GetService(const std::string& name) const;
        // Gets all services.
        inline Service::Map GetServices() { return mServices; }
        // Gets a list of all services for the Component.
        Service::ID::List GetServiceList() const;
        // Gets a list of all services for the Component.
        Service::ID::Set GetServiceSet() const;
        // Gets the component ID.
        inline Address GetComponentID() const { return mComponentID; }
        // Prints the status information for all services.
        virtual void PrintStatus() const;
    private:
        static void CheckServiceStatusEvent(void* args);
        static void CheckCoreServicesStatusEvent(void* args);
        Address mComponentID;                   ///< Component ID.
        volatile bool mInitializedFlag;         ///< Signals Component Initialization.
        Time::Stamp mServiceCheckTimeMs;        ///< The last time Service status was checked.
        Time::Stamp mCoreServicesCheckTimeMs;   ///< The last time core Services were checked.
        Transport* mpTransportService;          ///< Transport service.
        Service::Map mServices;                 ///< Component services.
        CxUtils::Timer mCheckServiceTimer;      ///< Timer object for checking Service status.
        CxUtils::Timer mCheckCoreServicesTimer; ///< Timer object for updating core services.
        Events* mpEventsService;                ///< Pointer to the events Service.
        Liveness* mpLivenessService;            ///< Pointer to Liveness Service.
        Discovery* mpDiscoveryService;          ///< Pointer to discovery Service.
        AccessControl * mpAccessControlService; ///< Pointer to Access Control Service.
        Management* mpManagementService;        ///< Pointer to Management Service.
        TimeService* mpTimeService;             ///< Pointer to the Time Service.
        CxUtils::MappedMemory mLockID;          ///< Memory space lock.
    };
}

#endif
/*  End of File */
