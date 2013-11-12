////////////////////////////////////////////////////////////////////////////////////
///
///  \file management.cpp
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
#include "jaus/core/management/management.h"
#include "jaus/core/component.h"
#include <iostream>

using namespace JAUS;

const std::string Management::Name = "urn:jaus:jss:core:Management";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to take control of a component, and automatically send a
///          resume command if control is acquired.
///
///   \param[in] id ID of the component to release control of.
///   \param[in] sendResumeCommand If true, a Resume command is sent after
///                                 control is acquired.
///   \param[in] reacquire If true, and control is lost, the Access Control
///                        service will continue to try and reacquire control.
///   \param[in] waitTimeMs How long in ms to wait for response from component.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::Child::RequestComponentControl(const Address &id, 
                                                const bool sendResumeCommand, 
                                                const bool reacquire, 
                                                const unsigned int waitTimeMs)
{
    if(GetComponent() == NULL)
    {
        return false;
    }
    if(!GetComponent()->AccessControlService()->HaveControl(id))
    {
        if(GetComponent()->AccessControlService()->RequestComponentControl(id, reacquire, waitTimeMs))
        {
            if(sendResumeCommand)
            {
                return GetComponent()->ManagementService()->Resume(id, waitTimeMs);
            }
            return true;
        }

        return false;
    }
    else if(sendResumeCommand) // Already have control, send resume command if needed.
    {
        return GetComponent()->ManagementService()->Resume(id, waitTimeMs);
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to release control of a component, and also send a 
///   Standby command before control is released.
///
///   Use this method to combine steps taken by sending a standby command first,
///   then releasing control of the component.
///
///   \param[in] id ID of the component to release control of.
///   \param[in] sendStandbyCommand If true, a Standby command is sent before
///                                 control is released.
///   \param[in] waitTimeMs How long in ms to wait for response from component.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::Child::ReleaseComponentControl(const Address &id, 
                                                const bool sendStandbyCommand, 
                                                const unsigned int waitTimeMs)
{
    if(GetComponent())
    {
        if(sendStandbyCommand)
        {
            if(GetComponent()->ManagementService()->Standby(id, waitTimeMs) &&
               GetComponent()->AccessControlService()->ReleaseComponentControl(id, waitTimeMs))
            {
                return true;
            }
        }
        else if(GetComponent()->AccessControlService()->ReleaseComponentControl(id, waitTimeMs))
        {
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Management::Management() : AccessControl::Child(Service::ID(Management::Name),
                                                Service::ID(AccessControl::Name))
{
    mStatus = Status::Initialized;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Management::~Management()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a Resume command and then if desired confirms the
///          state transition by sending a Query Status message.
///
///   \param[in] id ID of the component to send command to.
///   \param[in] waitTimeMs If greater than 0, how long to wait for a Report 
///                         Status response to confirm state transition.  If value
///                         is 0, then no confirmation is made.
///
///   \return True on success, false on failure.   If waitTimeMs is 0, a return
///           of true does not gaurantee the component transition states.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::Resume(const Address& id,
                        const unsigned int waitTimeMs)
{
    bool result = false;
    if(id.mSubsystem == GetComponentID().mSubsystem)
    {
        if(GetComponentStatus(id) == (int)Status::Ready)
        {
            return true;
        }
    }
    JAUS::Resume resume(id, GetComponentID());
    if(Send(&resume))
    {
        QueryStatus query(id, GetComponentID());

        if(waitTimeMs > 0)
        {
            ReportStatus response;
            if(Send(&query, &response, waitTimeMs) && response.GetStatus() == Status::Ready)
            {
                GetComponent()->DiscoveryService()->SetComponentState(id, Status::Ready);
                result = true;
            }
            else if(GetComponentStatus(id) == (int)Status::Ready)
            {
                GetComponent()->DiscoveryService()->SetComponentState(id, Status::Ready);
                result = true;
            }
        }
        else
        {
            // Send a query to get the state also.
            if(id.mSubsystem != GetComponentID().mSubsystem)
            {
                Send(&query);
            }
            result = true;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a Standby command and then if desired confirms the
///          state transition by sending a Query Status message.
///
///   \param[in] id ID of the component to send command to.  If ID is invalid
///                 (i.e. 0.0.0), then all components we have control of are
///                 sent the command.
///   \param[in] waitTimeMs If greater than 0, how long to wait for a Report 
///                         Status response to confirm state transition.  If value
///                         is 0, then no confirmation of state change
///                         is made (default).
///
///   \return True on success, false on failure.   If waitTimeMs is 0, a return
///           of true does not gaurantee the component transition states.  If
///           id is (0.0.0), True is only returned if standby command is 
///           successfully sent to all controlled components.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::Standby(const Address& id,
                         const unsigned int waitTimeMs)
{
    if(id.IsValid() == false)
    {
        Address::List inControl = GetComponent()->AccessControlService()->GetControlledComponents();
        Address::List::iterator c;
        bool result = true;
        for(c = inControl.begin(); c != inControl.end(); c++)
        {
            result &= Standby(*c, waitTimeMs);
        }
        return result;
    }

    if(id.mSubsystem == GetComponentID().mSubsystem)
    {
        if(GetComponentStatus(id) == (int)Status::Standby)
        {
            return true;
        }
    }

    bool result = false;

    JAUS::Standby standby(id, GetComponentID());
    if(Send(&standby))
    {
        QueryStatus query(id, GetComponentID());
        if(waitTimeMs > 0)
        {
            ReportStatus response;
            if(Send(&query, &response, waitTimeMs) && response.GetStatus() == Status::Standby)
            {
                GetComponent()->DiscoveryService()->SetComponentState(id, Status::Standby);
                result = true;
            }
        }
        else
        {
            Send(&query);
            result = true;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a Set Emergency command and then if desired confirms the
///          state transition by sending a Query Status message.
///
///   \param[in] id ID of the component to send command to.
///   \param[in] waitTimeMs If greater than 0, how long to wait for a Report 
///                         Status response to confirm state transition.  If value
///                         is 0, then no confirmation is made.
///
///   \return True on success, false on failure.   If waitTimeMs is 0, a return
///           of true does not gaurantee the component transition states.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::SetEmergency(const Address& id,
                              const unsigned int waitTimeMs)
{
    JAUS::SetEmergency setEmergency(id, GetComponentID());
    setEmergency.SetEmergencyCode(1);
    if(Send(&setEmergency))
    {
        if(waitTimeMs > 0)
        {
            QueryStatus query(id, GetComponentID());
            ReportStatus response;
            if(Send(&query, &response, waitTimeMs) && response.GetStatus() == Status::Emergency)
            {
                GetComponent()->DiscoveryService()->SetComponentState(id, Status::Emergency);
                return true;
            }
        }
        else
        {
            GetComponent()->DiscoveryService()->SetComponentState(id, Status::Emergency);
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a Clear Emergency command and then if desired confirms the
///          state transition by sending a Query Status message.
///
///   \param[in] id ID of the component to send command to.
///   \param[in] waitTimeMs If greater than 0, how long to wait for a Report 
///                         Status response to confirm state transition.  If value
///                         is 0, then no confirmation is made.
///
///   \return True on success, false on failure.   If waitTimeMs is 0, a return
///           of true does not gaurantee the component transition states.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::ClearEmergency(const Address& id,
                                const unsigned int waitTimeMs)
{
    JAUS::ClearEmergency clearEmergency(id, GetComponentID());
    clearEmergency.SetEmergencyCode(1);
    if(Send(&clearEmergency))
    {
        if(waitTimeMs > 0)
        {
            QueryStatus query(id, GetComponentID());
            ReportStatus response;
            if(Send(&query, &response, waitTimeMs) && response.GetStatus() != Status::Emergency)
            {
                GetComponent()->DiscoveryService()->SetComponentState(id, Status::Standby);
                return true;
            }
        }
        else
        {
            GetComponent()->DiscoveryService()->SetComponentState(id, Status::Standby);
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a Shutdown command to a component.
///
///   \param[in] id ID of the component to send command to.If ID is invalid
///                 (i.e. 0.0.0), then all components we have control of are
///                 sent the command.
///
///   \return True if message sent, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::Shutdown(const Address& id)
{
    if(id.IsValid() == false)
    {
        Address::List inControl = GetComponent()->AccessControlService()->GetControlledComponents();
        Address::List::iterator c;
        bool result = true;
        for(c = inControl.begin(); c != inControl.end(); c++)
        {
            result &= Shutdown(*c);
        }
        return result;
    }

    JAUS::Shutdown shutdown(id, GetComponentID());
    if(Send(&shutdown))
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the service.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::Initialize()
{
    mStatus = Management::Status::Initialized;
    SetStatus(mStatus);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the service.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::Shutdown()
{
    mStatus = Status::Shutdown;
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
bool Management::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_STATUS)
    {
        JAUS::ReportStatus report;
        report.SetStatus(mStatus);
        SendEvent(info, &report);
        return true;
    }
    return false; // Unsupported event.
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
bool Management::IsEventSupported(const Events::Type type,
                                  const double requestedPeriodicRate,
                                  const Message* queryMessage,
                                  double& confirmedPeriodicRate,
                                  std::string& errorMessage) const
{
    bool result = false;
    switch(queryMessage->GetMessageCode())
    {
    case QUERY_STATUS:
        confirmedPeriodicRate = requestedPeriodicRate;
        result = true;
        break;
    default:
        result = false;
        break;
    };
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports Management related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case CLEAR_EMERGENCY:
        {
            // Ensure appropriate transitions.
            if(mStatus == Status::Emergency)
            {
                Service::Map children = GetChildServices();
                Service::Map::iterator i;
                for(i = children.begin();
                    i != children.end();
                    i++)
                {
                    Management::Child* child = dynamic_cast<Management::Child*>(i->second);
                    if(child)
                    {
                        child->ClearEmergency();
                        child->mStatus = mStatus;
                    }
                }
                // Set status.
                SetStatus(Status::Standby);
            }
        }
        break;
    case QUERY_STATUS:
        {
            // Send current status.
            JAUS::ReportStatus report(message->GetSourceID(), GetComponentID());
            report.SetStatus(mStatus);
            Send(&report);
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << report.GetDestinationID().ToString() << " Requested Status at " << Time::GetUtcTime().ToString() << "\n";
            }
        }
        break;
    case REPORT_STATUS:
        {
            const JAUS::ReportStatus* report = dynamic_cast<const JAUS::ReportStatus*>(message);
            if(report)
            {
                if(report->GetSourceID().mSubsystem == GetComponentID().mSubsystem)
                {
                    WriteLock wLock(mStatesMutex);
                    mSubsystemStates[report->GetSourceID()] = (int)report->GetStatus();
                }
            }
        }
        break;
    case RESET:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << GetComponent()->AccessControlService()->GetControllerID().ToString() << " Sent Reset at " << Time::GetUtcTime().ToString() << "\n";
            }
            // Ensure appropriate transitions.
            if(mStatus != Status::Initialized)
            {
                SetStatus(Status::Standby);
            }
        }
        break;
    case RESUME:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << GetComponent()->AccessControlService()->GetControllerID().ToString() << " Sent Resume at " << Time::GetUtcTime().ToString() << "\n";
            }
            // Ensure appropriate transitions.
            if(mStatus == Status::Standby || mStatus == Status::Initialized)
            {
                SetStatus(Status::Ready);
            }
        }
        break;
    case SET_EMERGENCY:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << GetComponent()->AccessControlService()->GetControllerID().ToString() << " Sent Emergency at " << Time::GetUtcTime().ToString() << "\n";
            }
            SetStatus(Status::Emergency);
        }
        break;
    case SHUTDOWN:
        {
            mStatus = Status::Shutdown;
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << GetComponent()->AccessControlService()->GetControllerID().ToString() << " Sent Shutdown at " << Time::GetUtcTime().ToString() << "\n";
            }

            // Shutdown the component.
            if(GetComponent())
            {
                Service::Map children = GetChildServices();
                Service::Map::iterator i;
                for(i = children.begin();
                    i != children.end();
                    i++)
                {
                    Management::Child* child = dynamic_cast<Management::Child*>(i->second);
                    if(child)
                    {
                        child->mStatus = mStatus;
                    }
                }
                // You must check for shutdown in your program.
                //GetComponent()->Shutdown();
            }
        }
        break;
    case STANDBY:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - " << GetComponent()->AccessControlService()->GetControllerID().ToString() << " Sent Standby at " << Time::GetUtcTime().ToString() << "\n";
            }
            // Ensure appropriate transitions.
            if(mStatus == Status::Ready || mStatus == Status::Initialized)
            {
                SetStatus(Status::Standby);
            }
        }
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
Message* Management::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case CLEAR_EMERGENCY:
        message = new JAUS::ClearEmergency();
        break;
    case QUERY_STATUS:
        message = new JAUS::QueryStatus();
        break;
    case REPORT_STATUS:
        message = new JAUS::ReportStatus();
        break;
    case RESET:
        message = new JAUS::Reset();
        break;
    case RESUME:
        message = new JAUS::Resume();
        break;
    case SET_EMERGENCY:
        message = new JAUS::SetEmergency();
        break;
    case SHUTDOWN:
        message = new JAUS::Shutdown();
        break;
    case STANDBY:
        message = new JAUS::Standby();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the status of the Service.
///
///   \param[in] state Desired state.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::SetStatus(const Byte state)
{
    // If Byte value is valid...
    if(state <= Status::Emergency)
    {
        mStatus = state;

        Service::Map children = GetChildServices();
        Service::Map::iterator i;
        for(i = children.begin();
            i != children.end();
            i++)
        {
            Management::Child* child = dynamic_cast<Management::Child*>(i->second);
            if(child)
            {
                switch(mStatus)
                {
                case Status::Ready:
                    child->Resume();
                    break;
                case Status::Standby:
                    child->Standby();
                    break;
                case Status::Emergency:
                    child->SetEmergency();
                    break;
                default:
                    // Do nothing for shutdown or failure.
                    break;
                }
                child->mStatus = mStatus;
            }
        }
        // Signal state change event.
        SignalEvent(REPORT_STATUS);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever control is released, revert back to a Standby
///          state because we cannot be in Ready state.
///
////////////////////////////////////////////////////////////////////////////////////
bool Management::ReleaseControl()
{
    return SetStatus(Status::Standby);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the status of a component.
///
///   If the component is part of the same subsystem, then waitTimeMs is
///   usually ignored because in JAUS++ the Management Service automatically
///   keeps track of the state of other components on the same subsystem.  Only
///   if the state hasn't been found yet is a message sent to componentID.
///
///   \param[in] id ID of the component to get status of.
///   \param[in] waitTimeMs How long to wait for response from component.
///
///   \return Status of component (e.g. Status::Initialized, Status::Ready). A
///           value of -1 is returned if unable to retrieve status value.
///
////////////////////////////////////////////////////////////////////////////////////
int Management::GetComponentStatus(const Address& id, const unsigned int waitTimeMs) const
{
    int result = -1;
    if(id.mSubsystem == GetComponentID().mSubsystem)
    {
        
        std::map<Address, int>::const_iterator s;
        ReadLock rLock( *((SharedMutex*)&mStatesMutex));
        s = mSubsystemStates.find(id);
        if(s != mSubsystemStates.end())
        {
            result = (int)s->second;
        }
    }

    if(result == -1)
    {
        JAUS::QueryStatus query(id, GetComponentID());
        JAUS::ReportStatus report;
        if(Send(&query, &report, waitTimeMs))
        {
            result = (int)report.GetStatus();
            // Send again to receive response later for state tracking of this subsystem.
            if(id.mSubsystem == GetComponentID().mSubsystem)
            {
                Send(&query);
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status info about the Service to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::PrintStatus() const
{
    std::cout << "[" << GetServiceID().ToString() << "] - " << GetComponentID().ToString() << " Status: ";
    switch(mStatus)
    {
    case Status::Ready:
        std::cout << "Ready\n";
        break;
    case Status::Standby:
        std::cout << "Standby\n";
        break;
    case Status::Shutdown:
        std::cout << "Shutdown\n";
        break;
    case Status::Failure:
        std::cout << "Failure\n";
        break;
    case Status::Emergency:
        std::cout << "Emergency\n";
        break;
    default:
        std::cout << "Intialized\n";
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status info about the Service to the console.
///
///   \param[in] allData If true, state information for all components
///                      on this subsystem are displayed also.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::PrintStatus(const bool allData) const
{
    std::cout << "[" << GetServiceID().ToString() << "] - " << GetComponentID().ToString() << " Status: ";
    switch(mStatus)
    {
    case Status::Ready:
        std::cout << "Ready\n";
        break;
    case Status::Standby:
        std::cout << "Standby\n";
        break;
    case Status::Shutdown:
        std::cout << "Shutdown\n";
        break;
    case Status::Failure:
        std::cout << "Failure\n";
        break;
    case Status::Emergency:
        std::cout << "Emergency\n";
        break;
    default:
        std::cout << "Intialized\n";
        break;
    }

    if(allData)
    {
        std::cout << "State of Subsystem Components:\n";
        ReadLock rLock( *((SharedMutex*)&mStatesMutex));
        std::map<Address, int>::const_iterator s;
        for(s = mSubsystemStates.begin();
            s != mSubsystemStates.end();
            s++)
        {
            std::cout << "    " << s->first.ToString() << " - ";
            switch(s->second)
            {
            case (int)Status::Ready:
                std::cout << "Ready\n";
                break;
            case (int)Status::Standby:
                std::cout << "Standby\n";
                break;
            case (int)Status::Shutdown:
                std::cout << "Shutdown\n";
                break;
            case (int)Status::Initialized:
                std::cout << "Initialized\n";
                break;
            case (int)Status::Emergency:
                std::cout << "Emergency\n";
                break;
            default:
                std::cout << "Unknown\n";
                break;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called periodically by external classes and is used to
///          verify control of components.
///
///   \param[in] timeSinceLastCheckMs Number of milliseconds since the last time
///                                   this method was called.
///
////////////////////////////////////////////////////////////////////////////////////
void Management::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    if(mShutdownServiceFlag)
    {
        return;
    }
    if(Time::GetUtcTimeMs() - mCheckStatusTimeMs >= 2000)
    {
        // Verify status is set correctly for all child services.
        Service::Map children = GetChildServices();
        Service::Map::iterator i;
        for(i = children.begin();
            i != children.end();
            i++)
        {
            Management::Child* child = dynamic_cast<Management::Child*>(i->second);
            if(child)
            {
                child->mStatus = mStatus;
            }
        }
        
        // Query all components for this subsystem for there status.
        Address::List components = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetAddressList();
        Address::List::iterator c;
        {
            WriteLock wLock(mStatesMutex);

            std::map<Address, int> states;

            states[GetComponentID()] = mStatus;
            // Rebuild our states map since components may no longer be
            // part of the subsystem or new ones may have been discovered.
            for(c = components.begin();
                c != components.end();
                c++)
            {
                if(*c != GetComponentID())
                {
                    if(mSubsystemStates.find(*c) != mSubsystemStates.end())
                    {
                        states[*c] = mSubsystemStates[*c];
                    }
                    else
                    {
                        states[*c] = -1; // Unknown state value.
                    }
                }
            }
            mSubsystemStates = states;
        }
        // Now query everyone for the status.
        QueryStatus query(Address(), GetComponentID());
        for(c = components.begin();
            c != components.end() && mShutdownServiceFlag == false;
            c++)
        {
            if(*c != GetComponentID()) // Only query other components.
            {
                query.SetDestinationID(*c);
                if(!GetComponent()->EventsService()->HaveSubscription(REPORT_STATUS, *c))
                {
                    if(GetComponent()->EventsService()->RequestEveryChangeEvent(*c, &query) == false)
                    {
                        // Do a manual query if we can't create a subscription.
                        Send(&query);
                    }
                }
            }
        }

        Subsystem::Map subsystems;
        Subsystem::Map::iterator ss;
        GetComponent()->DiscoveryService()->GetSubsystems(subsystems);
        for(ss = subsystems.begin();
            ss != subsystems.end();
            ss++)
        {
            components = ss->second->GetComponentsWithService(Management::Name);
            for(c = components.begin();
                c != components.end() && mShutdownServiceFlag == false;
                c++)
            {
                if(*c != GetComponentID()) // Only query other components if we do not have there information
                {
                    query.SetDestinationID(*c);
                    if(ss->second->GetComponent(*c)->mStatus < 0)
                    {
                        Send(&query);
                    }
                }
            }
        }
        // Free memory
        Subsystem::DeleteSubsystemMap(subsystems);

        mCheckStatusTimeMs = Time::GetUtcTimeMs();
    }
}

/*  End of File */
