////////////////////////////////////////////////////////////////////////////////////
///
///  \file microcontroller.cpp
///  \brief Contains the Microcontroller service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 9 February 2010
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
#include "jaus/extras/mcu/microcontroller.h"
#include "jaus/extras/mcu/querymicrocontrollerstate.h"
#include "jaus/extras/mcu/reportmicrocontrollerstate.h"
#include "jaus/extras/mcu/setmicrocontrollerstate.h"

using namespace JAUS;

const std::string Microcontroller::Name = "urn:jaus:jss:jpp:extras:Microcontroller";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller::Microcontroller() : Management::Child(Service::ID(Microcontroller::Name),
                                                       Service::ID(Management::Name))
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller::~Microcontroller()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to set the current value of a digital input of
///          the Microcontroller. 
///
///   If this digital input is set for the first time, or the value changes
///   events will be generated automatically.
///
///   \param[in] name Name of the digital input that changed.
///   \param[in] value The current value of the input.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::SetDigitalInput(const std::string& name, const bool value)
{
    bool signalEvent = false;
    if(name.empty())
    {
        std::cout << "JAUS::Microcontroller::SetDigitalInput - Empty String Argument Error\n";
        return;
    }
    Mutex::ScopedLock lock(&mMcuMutex);
    DigitalStates::iterator pin = mDigitalStates.find(name);
    if(pin == mDigitalStates.end() || pin->second != value)
    {
        signalEvent = true;
    }
    mDigitalStates[name] = value;
    if(signalEvent)
    {
        SignalEvent(true, name);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to set the current value of a analog input of
///          the Microcontroller. 
///
///   If this analog input is set for the first time, or the value changes
///   events will be generated automatically.
///
///   \param[in] name Name of the analog input that changed.
///   \param[in] value The current value of the input [-100, 100]%.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::SetAnalogInput(const std::string& name, const double value)
{
    if(name.empty())
    {
        std::cout << "JAUS::Microcontroller::SetAnalogInput - Empty String Argument Error\n";
        return;
    }
    if(value < -100.0 || value > 100.0)
    {
        std::cout << "JAUS::Microcontroller::SetAnalogInput - Input Value Out of Bounds\n";
        return;
    }

    bool signalEvent = false;
    Mutex::ScopedLock lock(&mMcuMutex);
    AnalogStates::iterator pin = mAnalogStates.find(name);
    if(pin == mAnalogStates.end() || pin->second != value)
    {
        signalEvent = true;
    }
    mAnalogStates[name] = value;
    if(signalEvent)
    {
        SignalEvent(false, name);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the value of the state by name.  Values for these states
///   are set usin gthe SetDigitalInput method.
///
///   \param[in] name Name of the device to get value for.
///
///   \return True if state is on, false if off.  
///
////////////////////////////////////////////////////////////////////////////////////
bool Microcontroller::GetDigitalState(const std::string& name) const
{
    Mutex::ScopedLock lock(&mMcuMutex);
    DigitalStates::const_iterator digital = mDigitalStates.find(name);
    if(digital != mDigitalStates.end())
    {
        return digital->second;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the value of the state by name.  Values for these states
///   are set usin gthe SetAnalogInput method.
///
///   \param[in] name Name of the device to get value for.
///
///   \return Value of the analog state.  If not set, value returned is 0.0.
///
////////////////////////////////////////////////////////////////////////////////////
double Microcontroller::GetAnalogState(const std::string& name) const
{
    Mutex::ScopedLock lock(&mMcuMutex);
    AnalogStates::const_iterator analog = mAnalogStates.find(name);
    if(analog != mAnalogStates.end())
    {
        return analog->second;
    }
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates an event for the given information.
///
///   \param[in] info The event information (ID, Sequence #, etc.) for generation.
///
///   \return True if event generated, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Microcontroller::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_MICROCONTROLLER_STATE)
    {
        const QueryMicrocontrollerState* query = dynamic_cast<const QueryMicrocontrollerState*>(info.mpQueryMessage);
        Mutex::ScopedLock lock(&mMcuMutex);

        DigitalStates::const_iterator digital;
        AnalogStates::const_iterator analog;

        ReportMicrocontrollerState report;

        if(query->GetDigitalStates()->size() == 0)
        {
            *report.GetDigitalStates() = mDigitalStates;
        }
        else
        {
            std::set<std::string>::const_iterator name;
            for(name = query->GetDigitalStates()->begin();
                name != query->GetDigitalStates()->end();
                name++)
            {
                digital = mDigitalStates.find(*name);
                if(digital != mDigitalStates.end())
                {
                    (*report.GetAnalogStates())[*name] = digital->second;
                }
            }
        }

        if(query->GetAnalogStates()->size() == 0)
        {
            *report.GetAnalogStates() = mAnalogStates;
        }
        else
        {
            std::set<std::string>::const_iterator name;
            for(name = query->GetAnalogStates()->begin();
                name != query->GetAnalogStates()->end();
                name++)
            {
                analog = mAnalogStates.find(*name);
                if(analog!= mAnalogStates.end())
                {
                    (*report.GetAnalogStates())[*name] = analog->second;
                }
            }
        }

        SendEvent(info, &report);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the event is supported by the Service.
///
///   \param[in] type The event type (Periodic/EveryChange).
///   \param[in] requestedPeriodicRate If type == Periodic, then this is the
///                                    desired update rate.
///   \param[in] queryMessage The query message associated with the event.
///   \param[out] confirmedPeriodicRate This is the confirmed periodic rate 
///                                     supported by the Service.
///   \param[out] errorMessage If not supported, this is an optional error message.
///
///   \return True if event supported, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Microcontroller::IsEventSupported(const Events::Type type,
                                       const double requestedPeriodicRate,
                                       const Message* queryMessage,
                                       double& confirmedPeriodicRate,
                                       std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_MICROCONTROLLER_STATE)
    {
        const QueryMicrocontrollerState* query = dynamic_cast<const QueryMicrocontrollerState*>(queryMessage);
        Mutex::ScopedLock lock(&mMcuMutex);

        DigitalStates::const_iterator digital;
        AnalogStates::const_iterator analog;

        ReportMicrocontrollerState report;

        if(query->GetDigitalStates()->size() != 0)
        {
            std::set<std::string>::const_iterator name;
            for(name = query->GetDigitalStates()->begin();
                name != query->GetDigitalStates()->end();
                name++)
            {
                digital = mDigitalStates.find(*name);
                if(digital == mDigitalStates.end())
                {
                    errorMessage = "Digital Device Not Supported."; return false;
                }
            }
        }

        if(query->GetAnalogStates()->size() != 0)
        {
            std::set<std::string>::const_iterator name;
            for(name = query->GetAnalogStates()->begin();
                name != query->GetAnalogStates()->end();
                name++)
            {
                analog = mAnalogStates.find(*name);
                if(analog == mAnalogStates.end())
                {
                    errorMessage = "Analog Device Not Supported."; return false;
                }
            }
        }

        confirmedPeriodicRate = requestedPeriodicRate;

        return true;
    }

    return false;
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
Message* Microcontroller::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_MICROCONTROLLER_STATE:
        message = new QueryMicrocontrollerState();
        break;
    case REPORT_MICROCONTROLLER_STATE:
        message = new ReportMicrocontrollerState();
        break;
    case SET_MICROCONTROLLER_STATE:
        message = new SetMicrocontrollerState();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_MICROCONTROLLER_STATE:
        {
            const QueryMicrocontrollerState* query = dynamic_cast<const QueryMicrocontrollerState*>(message);
            if(query)
            {
                Mutex::ScopedLock lock(&mMcuMutex);

                DigitalStates::const_iterator digital;
                AnalogStates::const_iterator analog;

                ReportMicrocontrollerState report(message->GetSourceID(), GetComponentID());

                if(query->GetDigitalStates()->size() == 0)
                {
                    *report.GetDigitalStates() = mDigitalStates;
                }
                else
                {
                    std::set<std::string>::const_iterator name;
                    for(name = query->GetDigitalStates()->begin();
                        name != query->GetDigitalStates()->end();
                        name++)
                    {
                        digital = mDigitalStates.find(*name);
                        if(digital != mDigitalStates.end())
                        {
                            (*report.GetAnalogStates())[*name] = digital->second;
                        }
                    }
                }

                if(query->GetAnalogStates()->size() == 0)
                {
                    *report.GetAnalogStates() = mAnalogStates;
                }
                else
                {
                    std::set<std::string>::const_iterator name;
                    for(name = query->GetAnalogStates()->begin();
                        name != query->GetAnalogStates()->end();
                        name++)
                    {
                        analog = mAnalogStates.find(*name);
                        if(analog!= mAnalogStates.end())
                        {
                            (*report.GetAnalogStates())[*name] = analog->second;
                        }
                    }
                }
                Send(&report);
            }
        }
        break;
    case SET_MICROCONTROLLER_STATE:
        {
            const SetMicrocontrollerState* command = dynamic_cast<const SetMicrocontrollerState*>(message);
            if(command)
            {
                DigitalStates::const_iterator digital;
                AnalogStates::const_iterator analog;

                for(digital = command->GetDigitalStates()->begin();
                    digital != command->GetDigitalStates()->end();
                    digital++)
                {
                    SetDigitalOut(digital->first,
                                  digital->second);
                    Mutex::ScopedLock lock(&mMcuMutex);
                    mDigitalStates[digital->first] = digital->second;
                    SignalEvent(true,
                                digital->first);
                }

                for(analog = command->GetAnalogStates()->begin();
                    analog != command->GetAnalogStates()->end();
                    analog++)
                {
                    SetAnalogOut(analog->first,
                                 analog->second);
                    Mutex::ScopedLock lock(&mMcuMutex);
                    mAnalogStates[analog->first] = analog->second;
                    SignalEvent(false,
                                analog->first);
                }
            }
        }
        break;
    case REPORT_MICROCONTROLLER_STATE:
        {
            const ReportMicrocontrollerState* report = dynamic_cast<const ReportMicrocontrollerState*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                Mutex::ScopedLock lock(&mMcuMutex);
                mDigitalStates = *report->GetDigitalStates();
                mAnalogStates = *report->GetAnalogStates();
                Events::Child::SignalEvent(REPORT_MICROCONTROLLER_STATE);
            }
        }
        break;
    default:
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mMcuMutex);
    if(GetSynchronizeID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Synchronized to [" << GetSynchronizeID().ToString() << "]:\n";
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Pin States:\n";
    }
    mMcuMutex.Lock();
    DigitalStates digitalStates = mDigitalStates;
    AnalogStates analogStates = mAnalogStates;
    mMcuMutex.Unlock();

    DigitalStates::const_iterator digital;
    for(digital = digitalStates.begin();
        digital != digitalStates.end();
        digital++)
    {
        std::cout << "Digital - " << digital->first << " - " << (int)digital->second << std::endl;
    }
    AnalogStates::const_iterator analog;
    for(analog = analogStates.begin();
        analog != analogStates.end();
        analog++)
    {
        std::cout << "Analog - " << analog->first << " - " << analog->second << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief If synchronization is enabled, then this method is used to
///          subscribe to sensor data from the component we are synchronizing
///          with.
///
///   \param[in] timeSinceLastCheckMs Time since last update in ms.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription
        if(EventsService()->HaveSubscription(REPORT_MICROCONTROLLER_STATE, syncID) == false)
        {
            QueryMicrocontrollerState query;
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to signal an event has occured.
///
///   \param[in] digital Type of change (digital data = true, analog = false).
///   \param[in] name The name of the device/pin that changed.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::SignalEvent(const bool digital,
                                  const std::string& name)
{
    // Normally we could use the generic SignalEvent method, however
    // we only want to signal events for a change to this specific
    // data.  That way we don't generate an event for all data when
    // only 1 has updated.
    Events::Subscription::List myEvents = GetComponent()->EventsService()->GetProducedEvents(REPORT_IMAGE);
    Events::Subscription::List::iterator e;
    for(e = myEvents.begin();
        e != myEvents.end();
        e++)
    {
        const QueryMicrocontrollerState* query = dynamic_cast<const QueryMicrocontrollerState*>(e->mpQueryMessage);
        if(query)
        {
            if(digital)
            {
                if(query->GetDigitalStates()->size() == 0 || query->GetDigitalStates()->find(name) != query->GetDigitalStates()->end())
                {
                    Events::Child::SignalEvent((*e));
                }
            }
            else
            {
                if(query->GetAnalogStates()->size() == 0 || query->GetAnalogStates()->find(name) != query->GetAnalogStates()->end())
                {
                    Events::Child::SignalEvent((*e));
                }
            }
        }
    }
}

/*  End of File */
