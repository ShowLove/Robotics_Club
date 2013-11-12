////////////////////////////////////////////////////////////////////////////////////
///
///  \file events.cpp
///  \brief Contains the Events Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 October 2009
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
#include "jaus/core/events/events.h"
#include "jaus/core/events/cancelevent.h"
#include "jaus/core/events/confirmeventrequest.h"
#include "jaus/core/events/createevent.h"
#include "jaus/core/events/event.h"
#include "jaus/core/events/queryevents.h"
#include "jaus/core/events/rejecteventrequest.h"
#include "jaus/core/events/reportevents.h"
#include "jaus/core/events/updateevent.h"
#include "jaus/core/transport/transport.h"
#include "jaus/core/component.h"
#include <cxutils/math/cxmath.h>
#include <cstring>
#include <iomanip>
#include <iostream>


using namespace JAUS;

const std::string Events::Name = "urn:jaus:jss:core:Events";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::Subscription()
{
    mID = mSequenceNumber = 0;
    mPeriodicRate = 0.0;
    mpQueryMessage = NULL;
    mType = Events::Periodic;
    mUpdateTimeMs = 0;
    mpEventService = NULL;
    mTriggerTimeSeconds = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::Subscription(const Events::Subscription& data)
{
    mID = mSequenceNumber = 0;
    mPeriodicRate = 0.0;
    mpQueryMessage = NULL;
    mType = Events::Periodic;
    mUpdateTimeMs = 0;
    mpEventService = NULL;
    mTriggerTimeSeconds = 0;
    *this = data;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::~Subscription()
{
    if(mpQueryMessage)
    {
        delete mpQueryMessage;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription& Events::Subscription::operator =(const Events::Subscription& data)
{
    if(this != &data)
    {
        mID = data.mID;
        mSequenceNumber = data.mSequenceNumber;
        mPeriodicRate = data.mPeriodicRate;
        mType = data.mType;
        mUpdateTimeMs = data.mUpdateTimeMs;
        mProducer = data.mProducer;
        mClients = data.mClients;
        if(mpQueryMessage)
        {
            delete mpQueryMessage;
            mpQueryMessage = NULL;
        }
        if(data.mpQueryMessage)
        {
            mpQueryMessage = data.mpQueryMessage->Clone();
        }
        mpEventService = data.mpEventService;
        mTriggerTimeSeconds = data.mTriggerTimeSeconds;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///  \param[in] serviceIdentifier Service name identifier.
///  \param[in] parentServiceIdentifier Service version number.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Child::Child(const ID& serviceIdentifier, 
                     const ID& parentServiceIdentifier) : Service(serviceIdentifier,
                                                                  parentServiceIdentifier)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Child::~Child()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Signals to the Transport Service that data of the type 
///          passed has been updated, and an Every Change event should be
///          generated.
///
///   If there are no Event subscriptions for this type of data, then this
///   method will do nothing.
///
///   \param[in] reportMessageCode The type of report message whos data has
///                                changed (signal event for this data).
///
////////////////////////////////////////////////////////////////////////////////////
void Events::Child::SignalEvent(const UShort reportMessageCode)
{
    Component* component = GetComponent();
    if(component)
    {
        component->EventsService()->SignalEvent(reportMessageCode);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Signals to the Event Service that the type of data has changed
///          and any EveryChange type events should be generated.
///
///   Any time your report data changes, and your Service is a child of the
///   Event Service, you can signal the change in the data using this method.
///
///   \param[in] info The specific event to trigger.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::Child::SignalEvent(const Subscription& info)
{
    Component* component = GetComponent();
    if(component)
    {
        component->EventsService()->SignalEvent(info);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to Events Service.
///
////////////////////////////////////////////////////////////////////////////////////
Events* Events::Child::EventsService()
{
    return this->GetComponent()->EventsService();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to Events Service.
///
////////////////////////////////////////////////////////////////////////////////////
const Events* Events::Child::EventsService() const
{
    return this->GetComponent()->EventsService();
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Given event information and the report message payload, this
///          method will create an Event message with the provided payload and
///          subscription information and transmit to all subscribers of the
///          event.
///
///   \param[in] info Subscription information.
///   \param[in] payload Report message payload to include in Event message.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::Child::SendEvent(const Events::Subscription& info,
                              const Message* payload) const
{
    JAUS::Event eventMessage;
    eventMessage.SetSourceID(GetComponentID());
    eventMessage.SetSequenceNumber(info.mSequenceNumber);
    eventMessage.SetEventID(info.mID); 
    eventMessage.SetReportMessage(payload);
    // Send to all destinations.
    SendToList(info.mClients, &eventMessage);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Events() : Service(Service::ID(Events::Name, 1.0), Service::ID(Transport::Name, 1.0))
{
    mCheckEventsTimeMs = Time::GetUtcTimeMs();
    mPeriodicTimer.RegisterTimerEvent(Events::PeriodicEvent, this);
    mShutdownFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Events::~Events()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the Service.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::Shutdown()
{
    mShutdownFlag = true;
    
    if(mDebugMessagesFlag)
    {
        WriteLock printLock(mDebugMessagesMutex);
        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Stopping Events Timer\n";
    }
    mPeriodicTimer.Stop();
    if(mDebugMessagesFlag)
    {
        WriteLock printLock(mDebugMessagesMutex);
        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Events Timer Stopped\n";
    }

    Subscription::List copy;
    Subscription::List removed;

    // Make a copy of the current subscriptions list so 
    // that we don't block any other thread.

    mEventsMutex.lock();
    mEvents.clear();

    Subscription::List::iterator sub;

    if(mDebugMessagesFlag)
    {
        WriteLock printLock(mDebugMessagesMutex);
        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Canceling Event Subscriptions.\n";
    }    

    // Remove events we successfully canceled.
    sub = mSubscriptions.begin();
    while(sub != mSubscriptions.end())
    {
        CancelSubscription(*sub, Service::DefaultWaitMs);
        sub = mSubscriptions.erase(sub);
    }    
    
    mSubscriptions.clear();
    mEventsMutex.unlock();   
    
    if(mDebugMessagesFlag)
    {
        WriteLock printLock(mDebugMessagesMutex);
        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Finished Canceling Event Subscriptions.\n";
    }

    mShutdownFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Requests an Every Change event subscription from the component
///          specified.
///
///   \param[in] provider The component that will provide the event.
///   \param[in] query The type of data to query (used to determine type of
///                    report information desired).
///   \param[in] waitTimeMs How long to wait for a response from the component
///                         sending confirmation/rejection.
///
///   \return True if subscription created, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::RequestEveryChangeEvent(const Address& provider,
                                     const Message* query,
                                     const unsigned int waitTimeMs)
{
    bool result = false;

    CreateEvent request(provider, GetComponentID());

    request.SetType(Events::EveryChange);
    request.SetQueryMessage(query);
    
    // Generate a local request ID, then add to
    // list of pending responses.
    Byte requestID = 0;
    std::set<Byte>::iterator riter;
    {
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        while(riter != mRequestSet.end() )
        {
            requestID++;
            riter = mRequestSet.find(requestID);
        }
        mRequestSet.insert(requestID);
    }
    request.SetRequestID(requestID);
    
    // Setup list of possible responses to command.
    Message::List responses;
    ConfirmEventRequest confirm;
    RejectEventRequest reject;
    responses.push_back(&confirm);
    responses.push_back(&reject);
    
    if(Send(&request, responses, waitTimeMs))
    {
        if(confirm.GetSourceID().IsValid()) // Confirmation.
        {
            Subscription sub;             
            Subscription::List::iterator siter;

            sub.mID = confirm.GetEventID();
            sub.mSequenceNumber = 0;
            sub.mpQueryMessage = query->Clone();
            sub.mType = EveryChange;
            sub.mPeriodicRate = confirm.GetConfirmedPeriodicRate();
            sub.mUpdateTimeMs = Time::GetUtcTimeMs();
            sub.mProducer = provider;
            sub.mClients.insert(GetComponentID());

            WriteLock wLock(mEventsMutex);

            // Don't add duplicate events.
            bool exists = false;
            for(siter = mSubscriptions.begin();
                siter != mSubscriptions.end();
                siter++)
            {
                if(siter->mID == sub.mID &&
                   siter->mProducer == provider &&
                   siter->mpQueryMessage->GetMessageCode() == query->GetMessageCode())
                {
                    *siter = sub;
                    exists = true;
                    break;
                }
            }
            if(!exists)
            {
                mSubscriptions.push_back(sub);
            }

            result = true;
        }
        else if(mDebugMessagesFlag)
        {
            WriteLock printLock(mDebugMessagesMutex);
            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Could not create subscription to " << provider.ToString() << " for Query: " << query->GetMessageName() << "\n";
        }
    }
    {
        // Remove local request ID from queue.
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        if(riter != mRequestSet.end() )
        {
            mRequestSet.erase(riter);
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Requests an Periodic event subscription from the component
///          specified.
///
///   \param[in] provider The component that will provide the event.
///   \param[in] query The type of data to query (used to determine type of
///                    report information desired).
///   \param[in] desiredPeriodicRate The desired periodic update rate for the
///                                  event.
///   \param[in] minimumPeriodicRate The minimum periodic rate that you will
///                                  accept, anything less than this number
///                                  and the event will be canceled.
///   \param[in] waitTimeMs How long to wait for a response from the component
///                         sending confirmation/rejection.
///
///   \return True if subscription created, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::RequestPeriodicEvent(const Address &provider, 
                                  const Message *query, 
                                  const double desiredPeriodicRate, 
                                  const double minimumPeriodicRate, 
                                  const unsigned int waitTimeMs)
{
    bool result = false;

    CreateEvent request(provider, GetComponentID());

    request.SetRequestedPeriodicRate(desiredPeriodicRate);
    request.SetType(Events::Periodic);
    request.SetQueryMessage(query);
    
    // Generate a local request ID, then add to
    // list of pending responses.
    Byte requestID = 0;
    std::set<Byte>::iterator riter;
    {
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        while(riter != mRequestSet.end() )
        {
            requestID++;
            riter = mRequestSet.find(requestID);
        }
        mRequestSet.insert(requestID);
    }
    
    request.SetRequestID(requestID);
    
    // Setup list of possible responses to command.
    Message::List responses;
    ConfirmEventRequest confirm;
    RejectEventRequest reject;
    responses.push_back(&confirm);
    responses.push_back(&reject);
    
    if(Send(&request, responses, waitTimeMs))
    {
        if(confirm.GetSourceID().IsValid()) // Confirmation.
        {
            if(confirm.GetConfirmedPeriodicRate() < minimumPeriodicRate)
            {
                // Cancel requested event since it does not 
                // meet minimum requirements.
                CancelEvent cancel(confirm.GetSourceID(), GetComponentID());
                cancel.SetEventID(confirm.GetEventID());
                cancel.SetRequestID(requestID);
                ConfirmEventRequest confirmResponse;
                // Cancel the requested event.
                Send(&cancel, &confirmResponse, waitTimeMs);
            }
            else
            {
                Subscription sub;
                Subscription::List::iterator siter;

                sub.mID = confirm.GetEventID();
                sub.mSequenceNumber = 0;
                sub.mpQueryMessage = query->Clone();
                sub.mType = Periodic;
                sub.mPeriodicRate = confirm.GetConfirmedPeriodicRate();
                sub.mUpdateTimeMs = Time::GetUtcTimeMs();
                sub.mProducer = provider;
                sub.mClients.insert(GetComponentID());

                WriteLock wLock(mEventsMutex);

                // Don't add duplicate events.
                bool exists = false;
                for(siter = mSubscriptions.begin();
                    siter != mSubscriptions.end();
                    siter++)
                {
                    if(siter->mProducer == provider &&
                       siter->mpQueryMessage->GetMessageCode() == query->GetMessageCode() &&
                       siter->mpQueryMessage->GetPresenceVector() == query->GetPresenceVector())
                    {
                        *siter = sub;
                        exists = true;
                        break;
                    }
                }
                if(!exists)
                {
                    mSubscriptions.push_back(sub);
                }

                result = true;
            }
        }
        else if(mDebugMessagesFlag)
        {
            WriteLock printLock(mDebugMessagesMutex);
            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Could not create subscription to " << provider.ToString() << " for Query: " << query->GetMessageName() << "\n";
        }
    }
    
    {
        // Remove local request ID from queue.
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        if(riter != mRequestSet.end() )
        {
            mRequestSet.erase(riter);
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Signals to the Event Service that the type of data has changed
///          and any EveryChange type events should be generated.
///
///   Any time your report data changes, and your Service is a child of the
///   Event Service, you should signal the change in the data using this method.
///
///   \param[in] reportMessageCode The type of data that has changed.
///   \param[in] changeOnly If true, only Every Change events are signaled. This
///                         is the default behavior.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::SignalEvent(const UShort reportMessageCode,
                         const bool changeOnly)
{
    Subscription::Map toGenerate;
    {
        ReadLock rLock(mEventsMutex);
        toGenerate = mEvents;
    }

    Subscription::Map::iterator sub;
    for(sub = toGenerate.begin();
        sub != toGenerate.end();
        sub++)
    {
        if(reportMessageCode == (UShort)(sub->first & 0xFFFF) &&
            (sub->second.mType == EveryChange || changeOnly == false) &&
            sub->second.mpEventService != NULL)
        {
            ((Child *)(sub->second.mpEventService))->GenerateEvent(sub->second);
            // Update the status of the event generated.
            {
                WriteLock wLock(mEventsMutex);
                Subscription::Map::iterator e = mEvents.find(sub->first);
                if(e != mEvents.end())
                {
                    e->second.mSequenceNumber++;
                    e->second.mTriggerTimeSeconds = CxUtils::Timer::GetTimeSeconds();
                    e->second.mUpdateTimeMs = Time::GetUtcTimeMs();
                }
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Signals to the Event Service that the type of data has changed
///          and any EveryChange type events should be generated.
///
///   Any time your report data changes, and your Service is a child of the
///   Event Service, you can signal the change in the data using this method.
///
///   \param[in] info The specific event to trigger.
///   \param[in] changeOnly If true, only Every Change events are signaled. This
///                         is the default behavior.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::SignalEvent(const Subscription& info,
                         const bool changeOnly)
{
    Subscription::Map toGenerate;
    {
        ReadLock rLock(mEventsMutex);
        toGenerate = mEvents;
    }

    UInt key = info.mID;
    key <<= USHORT_BITS;
    key |= info.mpQueryMessage->GetMessageCodeOfResponse();

    Subscription::Map::iterator sub;
    for(sub = toGenerate.begin();
        sub != toGenerate.end();
        sub++)
    {
        if(sub->first == key)
        {
            ((Child *)(sub->second.mpEventService))->GenerateEvent(sub->second);
            // Update the status of the event generated.
            {
                WriteLock wLock(mEventsMutex);
                Subscription::Map::iterator e = mEvents.find(sub->first);
                if(e != mEvents.end())
                {
                    e->second.mSequenceNumber++;
                    e->second.mTriggerTimeSeconds = CxUtils::Timer::GetTimeSeconds();
                    e->second.mUpdateTimeMs = Time::GetUtcTimeMs();
                }
            }
            break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status info about the Service to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::PrintStatus() const
{
    unsigned int supporting;
    unsigned int subscribing;
    std::cout << "[" << GetServiceID().ToString() << "] - " << GetComponentID().ToString() << "\n";
    {
        ReadLock rLock(* ((SharedMutex*)&mEventsMutex));
        supporting = (unsigned int)mEvents.size();
        subscribing = (unsigned int)mSubscriptions.size();
    }
    std::cout << "Supporting " << supporting << " and Requesting " << subscribing << " Events.\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to cancel any existing subscriptions from a component.
///
///   If the reportMessageCode is set to 0, then all subscriptions from the
///   specified component are canceled.
///
///   \param[in] reportMessageCode The type of data for the event.
///   \param[in] id The ID of the component generating the event that needs
///                 to be canceled.
///   \param[in] waitTimeMs How long to wait for a confirm or reject
///                         response to be received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::CancelSubscription(const Address& id,
                                const UShort reportMessageCode,
                                const unsigned int waitTimeMs)
{
    bool result = true;
    
    Subscription::List copy;
    Subscription::List removed;

    // Make a copy of the current subscriptions list so 
    // that we don't block any other thread.
    {
        ReadLock rLock(mEventsMutex);
        copy = mSubscriptions;
    }

    Subscription::List::iterator sub;

    for(sub = copy.begin();
        sub != copy.end();
        sub++)
    {
        if((sub->mProducer == id || id == Address())&&
            (reportMessageCode == 0 || sub->mpQueryMessage->GetMessageCodeOfResponse() == reportMessageCode))
        {
            if(CancelSubscription(*sub, waitTimeMs))
            {
                removed.push_back(*sub);
            }
            else
            {
                result = false;
            }
        }
    }

    // Remove events we successfully canceled.
    {
        WriteLock wLock(mEventsMutex);
        for(sub = removed.begin();
            sub != removed.end();
            sub++)
        {
            Subscription::List::iterator rem;
            for(rem = mSubscriptions.begin();
                rem != mSubscriptions.end();
                rem++)
            {
                if(sub->mProducer == rem->mProducer &&
                    sub->mID == rem->mID &&
                    sub->mpQueryMessage->GetMessageCode() == rem->mpQueryMessage->GetMessageCode())
                {
                    mSubscriptions.erase(rem);
                    break;
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to cancel any existing subscriptions from a component.
///
///   If the reportMessageCode is set to 0, then all subscriptions from the
///   specified component are canceled.
///
///   \param[in] reportMessageCode The type of data for the event.
///   \param[in] id The ID of the component generating the event that needs
///                 to be canceled.
///   \param[in] eventID Specific event ID.
///   \param[in] waitTimeMs How long to wait for a confirm or reject
///                         response to be received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::CancelSubscription(const Address& id,
                                const UShort reportMessageCode,
                                const Byte eventID,
                                const unsigned int waitTimeMs)
{
    bool result = true;
    
    Subscription::List copy;
    Subscription::List removed;

    // Make a copy of the current subscriptions list so 
    // that we don't block any other thread.

    {
        ReadLock rLock(mEventsMutex);
        copy = mSubscriptions;
    }

    Subscription::List::iterator sub;

    for(sub = copy.begin();
        sub != copy.end();
        sub++)
    {
        if(sub->mProducer == id &&
            sub->mID == eventID && 
            (reportMessageCode == 0 || sub->mpQueryMessage->GetMessageCodeOfResponse() == reportMessageCode))
        {
            if(CancelSubscription(*sub, waitTimeMs))
            {
                removed.push_back(*sub);
            }
            else
            {
                result = false;
            }
        }
    }

    {
        WriteLock wLock(mEventsMutex);
        for(sub = removed.begin();
            sub != removed.end();
            sub++)
        {
            Subscription::List::iterator rem;
            for(rem = mSubscriptions.begin();
                rem != mSubscriptions.end();
                rem++)
            {
                if(sub->mProducer == rem->mProducer &&
                    sub->mID == rem->mID &&
                    sub->mpQueryMessage->GetMessageCode() == rem->mpQueryMessage->GetMessageCode())
                {
                    mSubscriptions.erase(rem);
                    break;
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to confirm, or check to see if an event subscription
///          exists.
///
///   \param[in] reportMessageCode The type of data for the event.
///   \param[in] id The ID of the component providing the event.  If set to 0.0.0.0
///                 then any component with the type of event is checked.
///   \param[in] verifyWithQuery If true, a Query Events message is sent to the
///                              component to verify that the event actually 
///                              exists.
///   \param[in] waitTimeMs How long to wait in ms for a response to be recevied
///                         when verifyWithQuery is true.
///
///   \return True if the subscription exists, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::HaveSubscription(const UShort reportMessageCode, 
                              const Address &id, 
                              const bool verifyWithQuery, 
                              const unsigned int waitTimeMs) const
{
    bool result = false;
    if(verifyWithQuery && id.IsValid())
    {
        QueryEvents query(id, GetComponentID());
        query.SetQueryType(QueryEvents::MessageID);
        query.SetQueryFilter(reportMessageCode);

        ReportEvents report;

        if(Send(&query, &report, waitTimeMs))
        {
            if(report.GetRecords()->size() > 0)
            {
                result = true;
            }
        }
    }
    else
    {
        ReadLock wLock(*((SharedMutex*)&mEventsMutex));
        Subscription::List::const_iterator sub;
        for(sub = mSubscriptions.begin();
            sub != mSubscriptions.end() && false == result;
            sub++)
        {
            if((id == Address() || sub->mProducer == id) && 
                sub->mpQueryMessage->GetMessageCodeOfResponse() == reportMessageCode)
            {
                result = true;
                break;
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to check if there are subscribers for a type of event.
///
///   \param[in] reportMessageCode The type of data for the event.
///
///   \return True if the subscription exists, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::HaveSubscribers(const UShort reportMessageCode) const
{
    bool result = false;

    ReadLock wLock(*((SharedMutex*)&mEventsMutex));
    Subscription::Map::const_iterator sub;
    for(sub = mEvents.begin();
        sub != mEvents.end() && false == result;
        sub++)
    {
        if(sub->second.mpQueryMessage->GetMessageCodeOfResponse() == reportMessageCode)
        {
            result = true;
            break;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports the following message: Cancel Event, Confirm
///   Event Request, Create Event, Event, Query Events, Reject Event Request,
///   Report Events, UpdateEvent.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::Receive(const Message* message)
{
    if(mShutdownServiceFlag)
    {
        return;
    }
    switch(message->GetMessageCode())
    {
    case CANCEL_EVENT:
        {
            const CancelEvent* input = dynamic_cast<const CancelEvent*>(message);
            double maxFrequencyHz = 0.5;
            bool canceled = false;
            {
                WriteLock wLock(mEventsMutex);
                Subscription::Map::iterator sub;
                
                for(sub = mEvents.begin();
                    sub != mEvents.end() && false == canceled;
                    sub++)
                {
                    if(input->GetEventID() == sub->second.mID)
                    {
                        Address::Set::iterator client = sub->second.mClients.find(input->GetSourceID());
                        if(client != sub->second.mClients.end())
                        {
                            sub->second.mClients.erase(client);
                            canceled = true;
                        }
                        // If no more clients, erase the event alltogether.
                        if(sub->second.mClients.size() == 0)
                        {
                            mEvents.erase(sub);
                        }
                        if(canceled)
                        {
                            break;
                        }
                    }
                }
              
                // Look up how what the highest update frequency is
                // for events, so if needed, we can slow down the
                // periodic timer.
                for(sub = mEvents.begin();
                    sub != mEvents.end() && true == canceled;
                    sub++)
                {
                    if(sub->second.mPeriodicRate > maxFrequencyHz)
                    {
                        maxFrequencyHz = sub->second.mPeriodicRate;
                    }
                }                
            }

            // Send confirmation or rejection of cancelation
            if(canceled)
            {
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Event for " << input->GetSourceID().ToString() << " was Canceled.\n";
                }
                ConfirmEventRequest response(input->GetSourceID(), GetComponentID());
                response.SetRequestID(input->GetRequestID());
                response.SetEventID(input->GetEventID());
                Send(&response);   
                
                // See if we need to change the periodic timer update frequency.  Throttle 
                // down as needed.
                if(mPeriodicTimer.IsActive() && mPeriodicTimer.GetFrequency() > maxFrequencyHz)
                {
                    mPeriodicTimer.ChangeFrequency(maxFrequencyHz);
                }   
            }
            else
            {
                RejectEventRequest response(input->GetSourceID(), GetComponentID());
                response.SetRequestID(input->GetRequestID());
                response.SetResponseCode(RejectEventRequest::InvalidEventID);
                Send(&response);
            }
        }
        break;
    case UPDATE_EVENT:
    case CREATE_EVENT:
        {
            const CreateEvent* create = dynamic_cast<const CreateEvent*>(message);
            const UpdateEvent* update = dynamic_cast<const UpdateEvent*>(message);
            Service::Map children = GetChildServices();
            Service::Map::iterator citer;
            Events::Child* child = NULL;
            Message* queryMessage = NULL;
            double confirmedPeriodicRate = 0.0;
            std::string errorMessage;
            UShort signalEventCode = 0;
        
            bool sentResponse = false;

            for(citer = children.begin();
                citer != children.end();
                citer++)
            {
                child = dynamic_cast<Events::Child*>(citer->second);
                if(child && (queryMessage = child->CreateMessage(create->GetQueryMessageCode())) != NULL)
                {
                    
                    if(queryMessage->ReadMessageBody(*create->GetQueryMessage()) < 0)
                    {
                        // Failed to read message data.
                        return;
                    }
                    if((create && child->IsEventSupported(create->GetType(),
                                                          create->GetRequestedPeriodicRate(),
                                                          queryMessage,
                                                          confirmedPeriodicRate,
                                                          errorMessage)) ||
                       (update && child->IsEventSupported(update->GetType(),
                                                          update->GetRequestedPeriodicRate(),
                                                          queryMessage,
                                                          confirmedPeriodicRate,
                                                          errorMessage)) )
                    {                        
                        
                        Subscription::Map::iterator siter;
                        Subscription subscription;
                        bool exists = false;

                        // Lock data.
                        {
                            WriteLock wLock(mEventsMutex);
                            for(siter = mEvents.begin();
                                siter != mEvents.end();
                                siter ++)
                            {
                                // Keep trying to generate a "unique" subscription ID that is
                                // not already used for this type of data.
                                if(siter->second.mpQueryMessage->GetMessageCode() == queryMessage->GetMessageCode())
                                {
                                    subscription.mID = siter->second.mID + 1;
                                }
                                // See if this event already exists, and if so, add the
                                // subscriber to it.
                                Packet queryBody1, queryBody2;
                                siter->second.mpQueryMessage->WriteMessageBody(queryBody1);
                                queryMessage->WriteMessageBody(queryBody2);
                                if(siter->second.mpQueryMessage->GetMessageCode() == queryMessage->GetMessageCode() &&
                                    siter->second.mpQueryMessage->GetPresenceVector() == queryMessage->GetPresenceVector() &&
                                    siter->second.mType == create->GetType() &&
                                    queryBody1.Length() == queryBody2.Length() &&
                                    memcmp(queryBody1.Ptr(), queryBody2.Ptr(), queryBody1.Length()) == 0)
                                {
                                    exists = true;
                                    Type type = create ? create->GetType() : update->GetType();
                                    if(type == Periodic)
                                    {
                                        if(confirmedPeriodicRate > siter->second.mPeriodicRate)
                                        {
                                            siter->second.mPeriodicRate = confirmedPeriodicRate;
                                        }
                                    }
                                    // Make sure this source is in our list, just in case.
                                    siter->second.mClients.insert(message->GetSourceID());
                                    // Use previous information.
                                    subscription = siter->second;
                                    break;
                                }
                            }
                        }

                        // If this is an Update Event message, and
                        // the event doesn't exist, return error.
                        if(update && (exists == false || subscription.mType != update->GetType()))
                        {
                            RejectEventRequest reject(update->GetSourceID(), GetComponentID());
                            if(subscription.mType != update->GetType())
                            {
                                reject.SetResponseCode(RejectEventRequest::InvalidSetup);
                                errorMessage = "Change of Event Type (Periodic/Every Change) Not Supported";
                            }
                            else
                            {
                                reject.SetResponseCode(RejectEventRequest::InvalidEventID);
                            }
                            reject.SetRequestID(update->GetRequestID());
                            if(errorMessage.size())
                            {
                                reject.SetErrorMessage(errorMessage);
                            }
                            Send(&reject);
                            return;
                        }                        
                        // Create new event if it doesn't exist.
                        else if(create && exists == false)
                        {
                            subscription.mPeriodicRate = confirmedPeriodicRate;
                            subscription.mpQueryMessage = queryMessage; queryMessage = NULL;
                            subscription.mType = create->GetType();
                            subscription.mUpdateTimeMs = Time::GetUtcTimeMs();
                            subscription.mProducer = GetComponentID();
                            subscription.mClients.insert(create->GetSourceID());
                            subscription.mpEventService = child;
                            UInt key = subscription.mID;
                            key <<= USHORT_BITS;
                            key |= subscription.mpQueryMessage->GetMessageCodeOfResponse();

                            WriteLock wLock(mEventsMutex);
                            mEvents[key] = subscription;
                        }
                        
                        ConfirmEventRequest confirm(message->GetSourceID(), GetComponentID());
                        confirm.SetRequestID((create ? create->GetRequestID() : update->GetRequestID()));
                        confirm.SetConfirmedPeriodicRate(confirmedPeriodicRate);
                        confirm.SetEventID(subscription.mID);
                        if(true == (sentResponse = Send(&confirm)))
                        {
                            signalEventCode = subscription.mpQueryMessage->GetMessageCodeOfResponse();
                        }
                        // Start periodic timer to either generate periodic
                        // events, or to verify events are still valid (or re-create subscriptions).
                        if(subscription.mType == Events::Periodic && mPeriodicTimer.IsActive() == false)
                        {
                            std::stringstream tname;
                            tname << GetComponentID().ToString() << ":Events";
                            mPeriodicTimer.SetName(tname.str());
                            mPeriodicTimer.Start(subscription.mPeriodicRate);
                        }
                        else if(mPeriodicTimer.IsActive() && 
                                subscription.mPeriodicRate > mPeriodicTimer.GetFrequency())
                        {
                            mPeriodicTimer.ChangeFrequency(subscription.mPeriodicRate);
                        }
                    }
                    
                    // If the child can create the query message
                    // then it is the Service that we need to check.
                    // Make sure to delete any memory we created.
                    if(queryMessage)
                    {
                        delete queryMessage;
                        queryMessage = NULL;
                        break;
                    }   
                    
                }
                if(signalEventCode != 0)
                {
                    SignalEvent(signalEventCode, false);
                }
            }
            if(sentResponse == false)
            {
                RejectEventRequest reject(message->GetSourceID(), GetComponentID());
                reject.SetResponseCode(RejectEventRequest::MessageNotSupported);
                reject.SetRequestID((create ? create->GetRequestID() : update->GetRequestID()));
                if(errorMessage.size())
                {
                    reject.SetErrorMessage(errorMessage);
                }
                Send(&reject);
            }
        }
        break;
    case EVENT:
        {
            const Event* input = dynamic_cast<const Event*>(message);
            Transport* transport = dynamic_cast<Transport *>(GetParentService());
            Message* reportMessage = NULL;
            if(transport)
            {
                // Read the Report Data, first create a message to read it.
                reportMessage = transport->CreateMessage(input->GetReportMessageCode());
                if(reportMessage)
                {
                    reportMessage->CopyHeaderData(input);
                    if(reportMessage->ReadMessageBody(*input->GetReportMessage()) >= 0)
                    {
                        // Push the message to child services who will process.
                        GetParentService()->Receive(reportMessage);
                    }
                }
                else if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received unsupported Report [" << std::setbase(16) << input->GetReportMessageCode() << std::setbase(10) << "] within Event Message\n";
                }
                
                
            }
            // Find and update the specific subscription this
            // event is for, get it's data, and update it's status.
            Subscription subscriptionInformation;
            Subscription::List::iterator subscription;
            {
                WriteLock wLock(mEventsMutex);

                for(subscription = mSubscriptions.begin();
                    subscription != mSubscriptions.end();
                    subscription++)
                {
                    if(subscription->mID == input->GetEventID() &&
                        subscription->mProducer == input->GetSourceID())
                    {
                        subscription->mSequenceNumber = input->GetSequenceNumber();
                        subscription->mUpdateTimeMs = Time::GetUtcTimeMs();
                        subscriptionInformation = *subscription;
                        break;
                    }
                }
            }
            // Trigger callbacks for subscribing entities
            {
                Callback::Set::iterator cb;
                ReadLock rLock(mEventCallbackMutex);
                for(cb = mEventCallbacks.begin();
                    cb != mEventCallbacks.end() && reportMessage;
                    cb++)
                {
                    Events::Callback* myCB = dynamic_cast<Events::Callback*>(*cb);
                    if(myCB)
                    {
                        myCB->ProcessEvent(subscriptionInformation.mType,
                                           subscriptionInformation.mID,
                                           subscriptionInformation.mSequenceNumber,
                                           reportMessage);
                    }
                }
            }
            // Delete message when finished.
            if(reportMessage)
            {
                delete reportMessage;
            }
        }
        break;
    case QUERY_EVENTS:
        {
            const QueryEvents* input = dynamic_cast<const QueryEvents*>(message);
            ReportEvents report(input->GetSourceID(), GetComponentID());
            {
                ReadLock rLock(mEventsMutex);

                Events::Subscription::Map::iterator sub;
                ReportEvents::Record record;
                // Add events to report and send.
                for(sub = mEvents.begin();
                    sub != mEvents.end();
                    sub++)
                {
                    // Only report events associated with the component
                    // requesting.
                    if(sub->second.mClients.find(message->GetSourceID()) != sub->second.mClients.end())
                    {
                        record.mEventID = sub->second.mID;
                        record.mEventType = sub->second.mType;
                        record.mQueryCode = sub->second.mpQueryMessage->GetMessageCode();
                        record.mQueryMessage.Clear();
                        sub->second.mpQueryMessage->WriteMessageBody(record.mQueryMessage);
                        if(input->GetQueryType() == QueryEvents::AllEvents)
                        {
                            report.GetRecords()->push_back(record);
                        }
                        else if(input->GetQueryType() == QueryEvents::EventID && 
                                record.mEventID == (Byte)input->GetQueryFilter())
                        {
                            report.GetRecords()->push_back(record);
                        }
                        else if(input->GetQueryType() == QueryEvents::EventType &&
                                input->GetQueryFilter() == (UShort)record.mEventType)
                        {
                            report.GetRecords()->push_back(record);
                        }
                        else if(input->GetQueryType() == QueryEvents::MessageID &&
                                (record.mQueryCode == input->GetQueryFilter() ||
                                sub->second.mpQueryMessage->GetMessageCodeOfResponse() == input->GetQueryFilter()))
                        {
                            report.GetRecords()->push_back(record);
                        }
                    }
                }
            }
            // Send the report
            Send(&report);
        }
        break;
    case CONFIRM_EVENT_REQUEST:
    case REJECT_EVENT_REQUEST:
        break;
    case REPORT_EVENTS:
        {
            const ReportEvents* input = dynamic_cast<const ReportEvents*>(message);
            Subscription::List subscriptions;
            Subscription::List::iterator sub;
            
            {
                ReadLock rLock(mEventsMutex);
                subscriptions = mSubscriptions;
            }

            // Go through the list of subscriptions we have
            // and verify the subscription is still there.
            for(sub = subscriptions.begin();
                sub != subscriptions.end();
                sub++)
            {
                if(sub->mProducer == input->GetSourceID())
                {
                    ReportEvents::Record::List::const_iterator record;
                    bool exists = false;
                    for(record = input->GetRecords()->begin();
                        record != input->GetRecords()->end();
                        record++)
                    {
                        if(record->mEventType == sub->mType &&
                           record->mQueryCode == sub->mpQueryMessage->GetMessageCode())
                        {
                            exists = true;
                            break;
                        }
                    }
                    if(exists == false)
                    {
                        WriteLock wLock(mMissingEventsMutex);
                        mMissingEvents.push_back(*sub);
                    }
                }
            }
        }
        break;
    default:
        break;
    };
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Attempts to create the message desired.  Only message supported
///          by this Service can be created by this Service.
///
///   \param[in] messageCode Message to create.
///
///   \return Pointer to newly allocated Message data, NULL if message is not
///           supported by the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Events::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;

    switch(messageCode)
    {
    case CANCEL_EVENT:
        message = new CancelEvent();
        break;
    case CONFIRM_EVENT_REQUEST:
        message = new ConfirmEventRequest();
        break;
    case CREATE_EVENT:
        message = new CreateEvent();
        break;
    case EVENT:
        message = new Event();
        break;
    case QUERY_EVENTS:
        message = new QueryEvents();
        break;
    case REJECT_EVENT_REQUEST:
        message = new RejectEventRequest();
        break;
    case REPORT_EVENTS:
        message = new ReportEvents();
        break;
    case UPDATE_EVENT:
        message = new UpdateEvent();
        break;
    default:
        message = NULL;
        break;
    };

    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called periodically by external classes and is used to
///          verify subscriptions are still valid.
///
///   \param[in] timeSinceLastCheckMs Number of milliseconds since the last time
///                                   this method was called.
///
////////////////////////////////////////////////////////////////////////////////////
void Events::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    // Check to see if we need to re-create any event
    // subscriptions, but don't do this too very often.
    unsigned int disconnectIntervalMs = this->GetComponent()->TransportService()->GetDisconnectTimeMs();
    if(disconnectIntervalMs < 5000)
    {
        disconnectIntervalMs = 5000;
    }
    if(disconnectIntervalMs > 10000)
    {
        disconnectIntervalMs = 5000;
    }

    if(mShutdownFlag == false && Time::GetUtcTimeMs() - mCheckEventsTimeMs >= disconnectIntervalMs)
    {
        // Query status of events and re-create as needed.
        Subscription::List subscriptions;
        Subscription::List::iterator sub;
        {
            ReadLock rLock(mEventsMutex);
            subscriptions = mSubscriptions;
        }

        QueryEvents query;
        query.SetSourceID(GetComponentID());
        // Go through each subscription and verify with
        // the producer of the event.
        for(sub = subscriptions.begin();
            sub != subscriptions.end() && mShutdownFlag == false;
            sub++)
        {
            query.SetDestinationID(sub->mProducer);
            query.SetQueryType(QueryEvents::EventID);
            query.SetQueryFilter(sub->mID);
            Send(&query);
        }
        // Update check time.
        mCheckEventsTimeMs = Time::GetUtcTimeMs();
    }

    // Try to re-create events that are missing.
    Subscription::List missing;
    Subscription::List::iterator miss;
    {
        WriteLock wLock(mMissingEventsMutex);
        missing = mMissingEvents;
        mMissingEvents.clear();
    }
    CreateEvent request;
    for(miss = missing.begin();
        miss != missing.end();
        miss++)
    {
        request.SetDestinationID(miss->mProducer);
        request.SetSourceID(GetComponentID());
        request.SetType(miss->mType);
        request.SetQueryMessage(miss->mpQueryMessage);
        request.SetRequestID(miss->mID);
        Send(&request);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a list of event subscriptions.
///
///   It's possible to filter by source and by message report type.  If
///   source is a 0 ID (invalid), then all event producers are considered, 
///   otherwise event producers with matching ID as source.  
///
///   To filter by type, set reportType to a non 0 value, and only events
///   of the type will be returned.
///
///   \param[in] source ID of the source of the event to get subscriptions
///                     from.  Set to 0.0.0 for any source.
///   \param[in] reportType The type of event messages to get info on.
///                         set to 0 for any events.
///
///   \return List of subscriptions the service has.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::List Events::GetSubscriptions(const Address& source,
                                                    const UShort reportType) const
{
    ReadLock rLock(*( (SharedMutex*)&mEventsMutex));
    Subscription::List result;
    Subscription::List::const_iterator s;
    for(s = mSubscriptions.begin();
        s != mSubscriptions.end();
        s++)
    {
        if(source.IsValid() == false ||
           s->mProducer == source)
        {
            if(reportType == 0 || 
               reportType == (int)s->mpQueryMessage->GetMessageCodeOfResponse())
            {
                result.push_back(*s);
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a list of event subscriptions.
///
///   It's possible to filter by source and by message report type.  If
///   source is a 0 ID (invalid), then all event producers are considered, 
///   otherwise event producers with matching ID as source.  
///
///   To filter by type, set reportType to a non 0 value, and only events
///   of the type will be returned.
///
///   \param[in] reportType The type of event messages to get info on.
///                         set to 0 for any events.
///
///   \return List of subscriptions the service has.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::List Events::GetSubscriptions(const UShort reportType) const
{
    ReadLock rLock(*( (SharedMutex*)&mEventsMutex));
    Subscription::List result;
    Subscription::List::const_iterator s;
    for(s = mSubscriptions.begin();
        s != mSubscriptions.end();
        s++)
    {
        if(reportType == 0 || 
           reportType == (int)s->mpQueryMessage->GetMessageCodeOfResponse())
        {
            result.push_back(*s);
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a list of event subscriptions being produced by
///          this service.
///
///   It's possible to filter by source and by message report type.  If
///   source is a 0 ID (invalid), then all event producers are considered, 
///   otherwise event producers with matching ID as source.  
///
///   To filter by type, set reportType to a non 0 value, and only events
///   of the type will be returned.
///
///   \param[in] reportType The type of event messages to get info on.
///                         set to 0 for any events.
///
///   \return List of subscriptions the service has.
///
////////////////////////////////////////////////////////////////////////////////////
Events::Subscription::List Events::GetProducedEvents(const UShort reportType) const
{
    ReadLock rLock(*( (SharedMutex*)&mEventsMutex));
    Subscription::List result;
    Subscription::Map::const_iterator s;
    for(s = mEvents.begin();
        s != mEvents.end();
        s++)
    {
        if(((UShort)(s->first & reportType)) == reportType)
        {
            result.push_back(s->second);
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to register a callback for an event.
///
///   \param[in] callback Pointer to callback instance (is not deleted by service).
///
////////////////////////////////////////////////////////////////////////////////////
void Events::RegisterCallback(Events::Callback* callback)
{
    WriteLock wLock(mEventCallbackMutex);
    mEventCallbacks.insert(callback);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Cancels the event.
///
///   \param[in] sub The subscription to cancel.
///   \param[in] waitTimeMs How long to wait for a response.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Events::CancelSubscription(Subscription& sub, const unsigned int waitTimeMs)
{
    bool result = false;
   
    // Generate a local request ID, then add to
    // list of pending responses.
    Byte requestID = 0;
    std::set<Byte>::iterator riter;
    {
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        while(riter != mRequestSet.end() )
        {
            requestID++;
            riter = mRequestSet.find(requestID);
        }
        mRequestSet.insert(requestID);
    }
    
    CancelEvent request(sub.mProducer, GetComponentID());
    request.SetEventID(sub.mID);
    request.SetRequestID(requestID);
    
    // Setup list of possible responses to command.
    Message::List responses;
    ConfirmEventRequest confirm;
    RejectEventRequest reject;
    responses.push_back(&confirm);
    responses.push_back(&reject);
    
    if(Send(&request, responses, waitTimeMs))
    {
        if(confirm.GetSourceID().IsValid()) // Received confirmation.
        {                   
            result = true;
        }
        else if(mDebugMessagesFlag)
        {
            WriteLock printLock(mDebugMessagesMutex);
            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Could not cancel subscription to " << sub.mProducer.ToString() << "\n";
        }
    }
    
    {
        // Remove local request ID from queue.
        WriteLock wLock(mRequestMutex);
        riter = mRequestSet.find(requestID);
        if(riter != mRequestSet.end() )
        {
            mRequestSet.erase(riter);
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called whenever a Periodic event needs is generated from
///          the Timer..
///
////////////////////////////////////////////////////////////////////////////////////
void Events::PeriodicEvent(void *args)
{
    Events* service = (Events *)args;
    Events::Subscription::Map::iterator local;
    
    if(service->mShutdownFlag)
    {
        return;
    }
    
    Subscription::Map eventsCopy;
    // Make a copy of events so that we do not call other services
    // code within our events mutex.  This will avoid deadlock 
    // situations
    {
        ReadLock rLock(service->mEventsMutex);
        eventsCopy = service->mEvents;
    }
    for(local = eventsCopy.begin();
        local != eventsCopy.end() && service->mShutdownFlag == false && service->mPeriodicTimer.IsShuttingDown() == false;
        local++)
    {
        Events::Child* child = dynamic_cast<Events::Child*>(local->second.mpEventService);
        if(local->second.mType == Events::Periodic && child)
        {
            double delay = 1.0/(local->second.mPeriodicRate + CxUtils::CX_EPSILON);
            if(CxUtils::Timer::GetTimeSeconds() - local->second.mTriggerTimeSeconds >= delay)
            {
                // Trigger periodic event. 
                child->GenerateEvent(local->second);
                // Update states in actual map of data within the mutex.
                {
                    WriteLock wLock(service->mEventsMutex);
                    Events::Subscription::Map::iterator actual = service->mEvents.find(local->first);
                    if(actual != service->mEvents.end() &&
                       actual->second.mSequenceNumber == local->second.mSequenceNumber &&
                       actual->second.mType == local->second.mType)
                    {
                        actual->second.mUpdateTimeMs = Time::GetUtcTimeMs();
                        actual->second.mTriggerTimeSeconds = CxUtils::Timer::GetTimeSeconds();
                        actual->second.mSequenceNumber++;
                    }
                }
            }
        }
    }
}




/*  End of File */
