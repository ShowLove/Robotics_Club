////////////////////////////////////////////////////////////////////////////////////
///
///  \file transport.h
///  \brief This file contains the definition for creating Transport Services
///  in JAUS++.
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
#ifndef __JAUS_CORE_TRANSPORT_BASE__H
#define __JAUS_CORE_TRANSPORT_BASE__H

#include "jaus/core/service.h"
#include "jaus/core/time.h"
#include "jaus/core/transport/connection.h"

#include <set>


namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Transport
    ///   \brief Transport is an interface class for creating Transport Services
    ///          defined by the SAE-JAUS standard.  All Transport Service 
    ///          implementations are based from this class.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Transport : public Service,
                                    public Connection::Callback
    {
        friend class Component;
    public:        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used to register to get a copy of a message
        ///          received by the Transport Service.  Messages received through 
        ///          the Callback are still received by the Component Services.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Callback
        {
        public:
            Callback() {}
            virtual ~Callback() {}
            typedef std::set<Callback* > Set;
            typedef std::map<UShort, Set > Map;
            virtual void ProcessMessage(const JAUS::Message* message) {};
        };
        static const std::string Name;                  ///<  Name of the service.
        Transport();
        virtual ~Transport();
        // Transport Service does not response to any messages.
        virtual bool IsDiscoverable() const;
        // Initializes the transport with a given ID for a component.
        virtual bool Initialize(const Address& componentID);
        // Returns true if transport has been initialized.
        virtual bool IsInitialized() const;
        // Shutdown the transport service.
        virtual void Shutdown();
        // Loads settings
        virtual bool LoadSettings(const std::string& filename);
        // Adds a new connection and tries to connect.
        bool AddConnection(const Address& id,
                           const Connection::Info& info);
        // Sends to all children.
        virtual void Receive(const Message* message);
        // Send a serialized message.
        virtual bool SendPacket(const Packet& packet, 
                                const Header& header)  const;
        // Serialize the JAUS message (add transport headers, serialize payload, etc.) 
        virtual bool SerializeMessage(const Message* message, 
                                      Packet::List& stream,
                                      Header::List& streamHeaders,
                                      const UShort startingSequenceNumber,
                                      const int broadcastFlags) const;
        // Method called to update communications and events within the service.
        virtual void UpdateServiceEvent();
        // Searches inheriting Services (child Services) and factories to create a message for processing.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Sends a message.
        virtual bool Send(const Message* message, const int broadcastFlags = Service::NoBroadcast) const;
        // Sends a message to multiple places.
        virtual bool SendToList(const Address::Set& destinations,
                                const Message* message, const int broadcastFlags = Service::NoBroadcast) const;
        // Sends a message, then waits for a response.
        virtual bool Send(const Message* message, 
                          Message* response, 
                          const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Sends a message and waits for one of multiple responses.
        virtual bool Send(const Message* message,
                          Message::List& possibleResponses,
                          const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Register to receive copies of messages when received by Transport.
        void RegisterCallback(const UShort messageCode, Callback* callback);
        // Reads the payload data which includes a message code, and converts to Message structure.
        Message* CreateMessageFromPacket(const Packet& packet) const;
        // Triggers any callbacks for a message.
        void TriggerMessageCallbacks(const Message* message);
        // Adds new message template.
        void AddMessageTemplate(Message* message);
        // Sets how long a connection lives before being dropped from inactivity 0 = no timeout.
        void SetDisconnectTimeMs(const unsigned int timeMs = 0);
        // Gets how long a connection lives before being dropped from inactivity 0 = no timeout.
        unsigned int GetDisconnectTimeMs() const;
        /** Sets how many threads to use for processing messages received, this is good if
            you expect to receive a lot of messages by your component. */
        bool SetMaxMessageProcessingThreads(unsigned int limit = 1);
        /*  Gets a pointer to the node manager, don't use if initialized. */
        NodeManager* GetNodeManager();
    protected:
        // Copies message template and callbacks.
        void CopyRegisteredItems(Transport* transport);
        // Called when data packets arrive from a connection.
        virtual void ProcessPacket(const Packet& jausPacket,
                                   const Header& jausHeader,
                                   const Connection* connection,
                                   const Connection::Info* sourceInfo);
    private:
        // Recursively try to create the message.
        Message* CreateMessageFromService(const UShort messageCode, const Service* service) const;
        // Create a message using templates.
        Message* GetMessageFromTemplate(const UShort messageCode) const;
        // Gets a message structure for reading packet data (pre-allocated).
        Message* GetCachedMessage(const UShort messageCode);
        // Processes a single packet.
        virtual void ProcessSinglePackets(Packet* packet = NULL);
        // Process multi-packet stream data
        virtual void ProcessMultiPackets(Packet* packet = NULL);
        // Check for a thread/procedure call waiting for an incomming message inline.
        bool CheckPendingReceipts(const Header& header, const UShort messageCode, const Packet& packet);
        // Method to notify Node Manager of our existence.
        void NotifyNodeManager(const unsigned int waitForResponseTimeMs = 1000);
        
        void* mpData;          ///<  Pointer to data used by the Transport layer.
    };
}

#endif
/*  End of File */
