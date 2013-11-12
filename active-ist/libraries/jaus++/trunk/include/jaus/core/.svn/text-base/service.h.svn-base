////////////////////////////////////////////////////////////////////////////////////
///
///  \file service.h
///  \brief This file contains the definition of the Service class which is
///  used to develop JAUS Services within JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 September 2009
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
#ifndef __JAUS_CORE_SERVICE__H
#define __JAUS_CORE_SERVICE__H

#include "jaus/core/message.h"
#include <string>
#include <set>
#include <map>

namespace JAUS
{
    class Transport;
    class Component;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Service
    ///   \brief A Service defines a JAUS Service which contains an identifier,
    ///   version, message set, protocol, and associated information.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Service
    {
        friend class Transport;
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ID
        ///   \brief ID is the Service Identifier information for a Service.  It is a 
        ///   globally unique string that identifies a specific Service Definition.  Since
        ///   a Service mandates a message set and associated protocol, the Service 
        ///   Identifier and version number are sufficient to identify the service
        ///   interface.  Service Identifiers are based on a Uniform Resource Identifier
        ///   (URI), and are specified for each service by the SAE JAUS standard.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL ID
        {
        public:
            typedef std::vector<ID> List;
            typedef std::set<ID> Set;
            ID(const std::string& name = "", const double verion = 1.0);
            ID(const ID& id);
            ~ID();
            int Write(Packet& packet) const;
            int Read(const Packet& packet);
            void Clear();
            std::string ToString(const bool nameOnly = true, const bool trim = true) const;
            ID& operator=(const ID& id);
            bool operator<(const ID& id) const { return mName < id.mName; }
            std::string mName;      ///<  Name of the service.
            double mVersion;        ///<  Version number.
        };
        static const int NoBroadcast     = 0;   // No broadcasting over IP (default)
        static const int LocalBroadcast  = 1;   // Use local broadcast transport layer options for sending.
        static const int GlobalBroadcast = 2;   // Use global broadcast transport layer options for sending.
        static const unsigned int DefaultWaitMs = 250;
        typedef std::map<std::string, Service*> Map;
        // Constructor, initializes ID, and any parent service we inherit from.
        Service(const ID& serviceIdentifier, const ID& parentServiceIdentifier);
        // Destructor.
        virtual ~Service();  
        // Called on component initialization.
        virtual void Initialize() {}
        // Shutsdown the Service.
        virtual void Shutdown() {}
        // Returns true if components are allowed to discover this serivice (used bye Discovery).
        virtual bool IsDiscoverable() const = 0;
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename) { return true; }
        // Sends a message.
        virtual bool Send(const Message* message, const int broadcastFlags = NoBroadcast) const;
        // Sends a message, then waits for a response.
        virtual bool Send(const Message* message, 
                          Message* response, 
                          const unsigned int waitTimeMs = DefaultWaitMs) const;
        // Sends a message and waits for one of multiple responses.
        virtual bool Send(const Message* message,
                          Message::List& possibleResponses,
                          const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Sends a message to multiple places.
        virtual bool SendToList(const Address::Set& destinations,
                                const Message* message, const int broadcastFlags = Service::NoBroadcast) const;
        // Method called whenever a message is received/given to the Service for processing.
        virtual void Receive(const Message* message);  
        // Create a message based on the message code.
        virtual Message* CreateMessage(const UShort messageCode) const = 0;
        // Gets the Service ID information.
        inline ID GetServiceID() const { return mServiceID; }
        // Sets the compondent ID.
        bool SetComponentID(const Address& id);
        // Gets the component ID.
        inline Address GetComponentID() const { return mComponentID; }
        // Checks if this Service inherits from another.
        bool InheritsFrom(const Service::ID& id) const;
        // Adds a child service connection if it inherits from this Service.
        bool AddChildService(Service* childService);
        // Sets the parent service (if this Service inherits from it).
        bool SetParentService(Service* parentService);
        // Gets the parent Service.
        Service* GetParentService();
        // Gets the parent Service.
        const Service* GetParentService() const;
        // Gets a pointer to a child Service.
        Service* GetChildService(const Service::ID& id = Service::ID());
        // Gets a pointer to a child Service.
        const Service* GetChildService(const Service::ID& id = Service::ID()) const;
        // Gets all the service information for this service and its children.
        Service::ID::Set GetServices() const;
        // Turn on Debug Messages.
        virtual void EnableDebugMessages(const bool on = true) { mDebugMessagesFlag = on; }
        // Method called periodically by Component or other Services, can be used for periodic udpdates.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs) { }
        // Method called to update communications and events within the service.
        virtual void UpdateServiceEvent() {}
        // Toggles a service on or off.
        void EnableService(const bool enable) { mServiceEnabledFlag = enable; }
        // Turns a service on or off.
        bool IsEnabled() const { return mServiceEnabledFlag; }
        // Shutsdown all children of this Service first, followed by itself.
        virtual void RecursiveShutdown();
        // Sets a pointer to the component the service (and children) belong to.
        void SetComponent(Component* component);
        // Gets a pointer to the component the service belongs to.
        inline Component* GetComponent() { return mpComponent; }
        // Gets a pointer to the component the service belongs to.
        inline const Component* GetComponent() const { return mpComponent; }
        // Prints information about the service.
        virtual void PrintStatus() const {}
        // Signal to services they must shutdown.
        void SignalServiceToShutdown(const bool enable = true) { mShutdownServiceFlag = enable; }
        // Get the service shutdown flag.
        bool IsServiceShuttingDown() const { return mShutdownServiceFlag; }
        // Method that can be run in a thread to periodically call UpdateServiceEvent method.
        static void ServiceUpdateThreadFunc(void* servicePointer);
    protected:
        void StartServiceUpdateEventThread();
        void StopServiceUpdateEventThread();
        void PushMessageToChildren(const Message* message);
        Map GetChildServices();
        const Map GetChildServices() const;
        Transport* GetTransportService() { return mpTransportService; }
        const Transport* GetTransportService() const { return mpTransportService; }
        static SharedMutex mDebugMessagesMutex;   ///<  Mutex to lock console for message display.
        volatile bool mDebugMessagesFlag;   ///<  If true, show debugging messages.
        Address mComponentID;               ///<  ID of the JAUS component the transport is for.   
        Component* mpComponent;             ///<  Pointer to the component the service belongs to.
        volatile bool mShutdownServiceFlag; ///<  If true, service must shutdown.
        Thread mServiceUpdateThread;        ///<  Thread for the service to perform updates within.
        unsigned int mServiceUpdateThreadDelayMs;   ///<  Delay time within the services updates in ms.
        volatile bool mSingleThreadModeFlag;        ///<  Running in single thread mode?
    private:
        volatile bool mServiceEnabledFlag;  ///<  If true, the service is ON, otherwise OFF.
        Service::ID mServiceID;       ///<  Service identifier.
        Service::ID mParentServiceID; ///<  Service identifer for parent service we inherit from.
        Map mJausChildServices;       ///<  Child services (inherit from this Service 1-to-N).
        Service* mpJausParentService; ///<  Parent service (Service interface we inherit from).
        Transport* mpTransportService;///<  Pointer to the transport service.
    };
}

#endif
/*  End of File */
