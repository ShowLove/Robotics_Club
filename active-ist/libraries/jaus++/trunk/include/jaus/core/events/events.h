////////////////////////////////////////////////////////////////////////////////////
///
///  \file events.h
///  \brief Contains the Events Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 19 October 2009
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
#ifndef __JAUS_CORE_EVENTS_EVENTS__H
#define __JAUS_CORE_EVENTS_EVENTS__H

#include "jaus/core/service.h"
#include "jaus/core/transport/transport.h"
#include <cxutils/timer.h>
#include <map>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Events
    ///   \brief The Events Service manages the creation and maintenance of
    ///          Event subscriptions.
    ///
    ///   This service allows clients to subscribe to changes in the information
    ///   provided by report messages supported by services that derive from this
    ///   service.  When a client succesfully creates an event, an "Internal Event"
    ///   occurs when the event creation critera are met, and as a result the matching
    ///   Event is sent to the subscribing client.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Events : public Service
    {
    public:
        const static std::string Name; ///< String name of the Service.
        // Type of Events
        enum Type
        {
            Periodic = 0,
            EveryChange
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Subscription
        ///   \brief Describes information about an Event that is being produced or
        ///          subscribed to by the Component.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Subscription
        {
            friend class Events;
        public:
            typedef std::map<UInt, Subscription> Map;
            typedef std::vector<Subscription> List;
            Subscription();
            Subscription(const Subscription& data);
            ~Subscription();
            Subscription& operator=(const Subscription& data);
            Byte mID;                   ///< Event ID Information.
            Byte mSequenceNumber;       ///< Event Sequence Number.
            double mPeriodicRate;       ///< Event Periodic Rate [0, 1092] Hz.
            Message* mpQueryMessage;    ///< Query Message associated with the Event.
            Events::Type mType;         ///< Type of event.
            Time::Stamp mUpdateTimeMs;  ///< Last time the Event was generated/received in ms.
            Address mProducer;          ///< ID of the Event producer.
            Address::Set mClients;      ///< Subscribers of the event.
        private:
            double mTriggerTimeSeconds; ///< Trigger time in seconds.
            Service* mpEventService;    ///< The service needed for the event.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Child
        ///   \brief Defines the interface for Services that inherit from the Events
        ///          Service. 
        ///
        ///   There are two virtual methods that must be implemented by inheriting
        ///   Services of Events.  They are GenerateEvent, which is called when an
        ///   Event message must be generated, and the IsEventSupported, which is
        ///   used to determine if an event can be created.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Child : public Service
        {
        public:
            Child(const ID& serviceIdentifier, const ID& parentServiceIdentifier);
            ~Child();
            // Signals the data has changed (i.e. an event has occured).
            void SignalEvent(const UShort reportMessageCode);
            // Signals the event subscription needs to be updated.
            void SignalEvent(const Subscription& info);
            // Method called when an Event has been signaled, generates an Event message.
            virtual bool GenerateEvent(const Events::Subscription& info) const = 0;
            // Method called to determine if an Event is supported by the service.
            virtual bool IsEventSupported(const Events::Type type,
                                          const double requestedPeriodicRate,
                                          const Message* queryMessage,
                                          double& confirmedPeriodicRate,
                                          std::string& errorMessage) const = 0;
            // Method for generating an event.
            void SendEvent(const Events::Subscription& info,
                           const Message* payload) const;
            // Gets a pointer to the events service.
            Events* EventsService();
            // Gets a pointer to the events service.
            const Events* EventsService() const;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Method to put hooks into Event Service to be notified when a new
        ///          event has been received.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Callback : public Transport::Callback
        {
        public:
            Callback() {}
            virtual ~Callback() {}
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Method called when an event related message has been received.
            ///
            ///   \param[in] type Type of event (EveryChange, Periodic).
            ///   \param[in] eventID Event ID number.
            ///   \param[in] sequenceNumber The sequence number for the event (update number).
            ///   \param[in] message The message data associated with the event.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual void ProcessEvent(const Events::Type type,
                                      const Byte eventID,
                                      const Byte sequenceNumber,
                                      const Message* message) {}
        };
        Events();
        ~Events();
        // Method called on Shutdown.
        virtual void Shutdown();
        // Request an Every Change Event from a service provider.
        bool RequestEveryChangeEvent(const Address& provider,                     // Whom to get the subscription from.
                                     const Message* query,                        // Query message that maps to desired Report.
                                     const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Request a Periodic Event from a service provider.
        bool RequestPeriodicEvent(const Address& provider,                        // Whom to get the subscription from.
                                  const Message* query,                           // Query message that maps to desired Report.
                                  const double desiredPeriodicRate = 1.0,         // Update rate desired.
                                  const double minimumPeriodicRate = 0.9,         // Minimum rate acceptable (slightly less for rounding error).
                                  const unsigned int waitTimeMs = DefaultWaitMs); // How long to wait for confirmation. 
        // Signals the Events service that the type of data has changed.
        void SignalEvent(const UShort reportMessageCode, const bool changeOnly = true);
        // Signals the Events service that the type of data has changed.
        void SignalEvent(const Subscription& info, const bool changeOnly = true);
        // Method used to cancel a subscriptoin.
        bool CancelSubscription(const Address& id,
                                const UShort reportMessageCode,
                                const unsigned int waitTimeMs = Service::DefaultWaitMs);
        // Method used to cancel a subscriptoin.
        bool CancelSubscription(const Address& id,
                                const UShort reportMessageCode,
                                Byte eventID,
                                const unsigned int waitTimeMs);
        // Method used to check if a subscription exists.
        bool HaveSubscription(const UShort reportMessageCode, 
                              const Address& id,
                              const bool verifyWithQuery = false,
                              const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Method to check if there are subscribers to specific events.
        bool HaveSubscribers(const UShort reportMessageCode) const;
        // Events service is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the Events service.
        virtual void Receive(const Message* message);  
        // Creates messages associated with the events service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // When called verifies that no subscriptions have been lost.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Sets how long to wait before the Service verifies event subscriptions are valid.
        void SetEventSubscriptionCheckThreshold(const Time::Stamp timeMs = 5000) { mCheckEventsTimeMs = timeMs; }
        // Gets a copy of the subscriptions from a source, filtering by type.
        Subscription::List GetSubscriptions(const Address& source = Address(),
                                            const UShort reportType = 0) const;
        // Gets all subscriptions of a type.
        Subscription::List GetSubscriptions(const UShort reportType) const;
        // Gets all events being produced of type.
        Subscription::List GetProducedEvents(const UShort reportType) const;
        // Method to register a callback for notifications of events received.
        void RegisterCallback(Events::Callback* callback);
        // Prints status about the service.
        virtual void PrintStatus() const;
    private:
        bool CancelSubscription(Subscription& sub, const unsigned int waitTimeMs);
        static void PeriodicEvent(void* args);
        CxUtils::Timer mPeriodicTimer;      ///<  Timer object used for periodic events.
        SharedMutex mEventsMutex;           ///<  Mutex for thread protection of event data.
        Subscription::Map  mEvents;         ///<  Events being produced.
        Subscription::List mSubscriptions;  ///<  Events being subscribed to.
        SharedMutex mMissingEventsMutex;    ///<  Missing events mutex.
        Subscription::List mMissingEvents;  ///<  Events that need to be re-created because they were lost.
        std::set<Byte> mRequestSet;         ///<  Set of different event request ID's.
        SharedMutex mRequestMutex;          ///<  Mutex for capture of a request ID.
        Time::Stamp mCheckEventsTimeMs;     ///<  Time stamp to verify if we need to re-create any events.
        volatile bool mShutdownFlag;        ///<  Signals that the service should shutdown.
        SharedMutex mEventCallbackMutex;    ///<  Mutex for event notifications.
        Callback::Set mEventCallbacks;      ///<  Event callbacks.
    };
}

#endif
/*  End of File */
