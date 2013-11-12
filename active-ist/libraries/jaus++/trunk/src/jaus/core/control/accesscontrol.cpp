////////////////////////////////////////////////////////////////////////////////////
///
///  \file accesscontrol.cpp
///  \brief Contains the Access Control Service implementation.
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
#include "jaus/core/control/accesscontrol.h"
#include "jaus/core/events/event.h"
#include "jaus/core/component.h"

#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace JAUS;

const std::string AccessControl::Name = "urn:jaus:jss:core:AccessControl";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccessControl::AccessControl() : Events::Child(Service::ID(AccessControl::Name),
                                               Service::ID(Events::Name))
{
    mControllableFlag = true;
    mAuthorityCode = 0;
    mControllerAuthorityCode = 0;
    mTimeoutPeriod = 5;
    mTimeoutThreshold = 25; // % threshold.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccessControl::~AccessControl()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if the message can be processed or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::AcceptCommandMessage(const Message* commandMessage) const
{
    if(commandMessage->IsCommand())
    {
        // Check for exclusive control before
        // passing command to child services for
        // processing.
        ReadLock rLock(*((SharedMutex*)&mControlMutex));
        if(mControllerID.IsValid() && mControllerID != commandMessage->GetSourceID())
        {
            // The only command to allow through is Request Control.
            if(commandMessage->GetMessageCode() != REQUEST_CONTROL)
            {
                return false;
            }
        }
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads UDP configuration values from an XML file.  Only loads values
///          if transport has not been initialized.
///
///   \param[in] filename File containing Service settings data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);
    TiXmlNode* node;
    node = doc.FirstChild("JAUS").FirstChild("AccessControl").FirstChild("Controllable").FirstChild().ToNode();
    if(node && node->Value())
    {
        mControllableFlag = atoi(node->Value()) > 0 ? true : false;
    }
    TiXmlElement* element = NULL;
    element = doc.FirstChild("JAUS").FirstChild("AccessControl").FirstChild("TimeoutSeconds").ToElement();
    if(element && element->FirstChild() && element->FirstChild()->Value())
    {
        mTimeoutPeriod = (Byte)atoi(element->FirstChild()->Value());

        if(element->Attribute("threshold"))
        {
            mTimeoutThreshold = atof(element->Attribute("threshold"));
            if(mTimeoutThreshold < 0.5 || mTimeoutThreshold > 50.0)
            {
                mTimeoutThreshold = 5; // 5 %
            }
        }
    }
    node = doc.FirstChild("JAUS").FirstChild("AccessControl").FirstChild("AuthorityLevel").FirstChild().ToNode();
    if(node && node->Value())
    {
        mAuthorityCode = (Byte)atoi(node->Value());
    }
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the service.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::Shutdown()
{
    // Release control of all components.
    ReleaseComponentControl(Address());
    // Send loss of control to controller.
    if(mControllerID.IsValid())
    {
        RejectControl release(mControllerID, GetComponentID());
        release.SetResponseCode(RejectControl::ControlReleased);
        Send(&release);
    }
} 


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to request exclusive control of a component.
///
///   \param[in] id The ID of the component to request control of.
///   \param[in] reacquire If true, and control is lost for any reason, then
///                        we will try to re-acquire control.  If false, and
///                        control is lost, we forget about the component.
///   \param[in] waitTimeMs How long to wait for a response in milliseconds
///                         from the component being requested.
///
///   \return True if control is established, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::RequestComponentControl(const Address& id, 
                                            const bool reacquire,
                                            const unsigned int waitTimeMs)
{
    RequestControl request(id, GetComponentID());
    request.SetAuthorityCode(mAuthorityCode);
    Message::List responses;
    ConfirmControl confirm;
    RejectControl reject;
    responses.push_back(&confirm);
    responses.push_back(&reject);

    if(Send(&request, responses, waitTimeMs) && responses.size() == 1)
    {
        // See if control was acquired.
        if(responses.front()->GetMessageCode() == CONFIRM_CONTROL)
        {
            if(confirm.GetResponseCode() == ConfirmControl::ControlAccepted)
            {
                {
                    WriteLock wLock(mControlMutex);
                    mControlledComponents.insert(id);
                    mMaintainFlags[id] = reacquire;    
                    mControlFlags[id] = true;
                    // Set the time we verified we have control.
                    mControlCheckTimes[id].SetCurrentTime();
                    // Set the time we confirmed control.
                    mControlConfirmTimes[id].SetCurrentTime();
                    // Set a default value for timeout.
                    mTimeoutPeriods[id] = mTimeoutPeriod;
                }

                // Query the timout period for the component.
                QueryTimeout query(id, GetComponentID());
                Send(&query);

                // Trigger acquisition of control events!

                Service::Map children = GetChildServices();
                Service::Map::iterator child;
                for(child = children.begin();
                    child != children.end();
                    child++)
                {
                    Child* controlChild = dynamic_cast<Child *>(child->second);
                    if(controlChild)
                    {
                        controlChild->ProcessAcquisitionOfControl(id);
                    }
                }

                {
                    ReadLock cbrLock(mCallbacksMutex);
                    Callback::Set::iterator cb;
                    for(cb = mCallbacks.begin();
                        cb != mCallbacks.end();
                        cb++)
                    {
                        (*cb)->ProcessAcquisitionfControl(id);
                    }
                }
                return true;
            }
        }
        else
        {
            return false;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to release exclusive control of a component.
///
///   \param[in] id The ID of the component to request control of.  If ID is
///                 invalid (all zeros), then control is released to all
///                 components.
///   \param[in] waitTimeMs How long to wait for a response in milliseconds
///                         from the component being requested.
///
///   \return True if control is established, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::ReleaseComponentControl(const Address& id, 
                                            const unsigned int waitTimeMs)
{
    bool result = false;

    if(id.IsValid() == false)
    {
        result = true;
        Address::Set controlled;
        {
            ReadLock rLock(mControlMutex);
            controlled = mControlledComponents;
        }
        Address::Set::iterator component;
        for(component = controlled.begin();
            component != controlled.end();
            component++)
        {
            result &= ReleaseComponentControl(*component, waitTimeMs);
        }
    }
    else
    {
        {
            std::map<Address, bool>::iterator flag;
            WriteLock wLock(mControlMutex);
            flag = mControlFlags.find(id);
            if(flag != mControlFlags.end())
            {
                // If we lost control at some point, then
                // we don't need to release it, just remove
                // the component from our lists.
                if(flag->second == false)
                {
                    result = true;
                    // Remove the component from our lists.
                    EraseComponentControlInfo(id);
                }
            }
            else
            {
                // Can't release control of a component
                // we never requested control from.
                result = true;
                EraseComponentControlInfo(id);
            }
        }

        if(!result)
        {
            // Signal control routines to not
            // try and maintain control for this component anymore.
            {
                WriteLock wLock(mControlMutex);
                mToReleaseControl.insert(id);
            }
            // Send request to release control, and verify response.
            ReleaseControl release(id, GetComponentID());

            RejectControl reject;

            if(Send(&release, &reject, waitTimeMs))
            {
                if(reject.GetResponseCode() == RejectControl::ControlReleased)
                {
                    result = true;
                    WriteLock wLock(mControlMutex);
                    EraseComponentControlInfo(id);
                }
            }
            {
                // Remove ID.
                WriteLock wLock(mControlMutex);
                mToReleaseControl.erase(mToReleaseControl.find(id));
            }
        }
    }
    return result;
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
bool AccessControl::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_AUTHORITY)
    {
        ReportAuthority report;
        report.SetAuthorityCode(mAuthorityCode);
        SendEvent(info, &report);
        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_CONTROL)
    {
        ReportControl report;
        report.SetAuthorityCode(mControllerAuthorityCode);
        report.SetControllingComponent(mControllerID);
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
bool AccessControl::IsEventSupported(const Events::Type type,
                                     const double requestedPeriodicRate,
                                     const Message* queryMessage,
                                     double& confirmedPeriodicRate,
                                     std::string& errorMessage) const
{
    bool result = false;
    switch(queryMessage->GetMessageCode())
    {
    case QUERY_AUTHORITY:
    case QUERY_CONTROL:
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
///   \brief Method to enable control of the service/component by other
///          components.  By default, all components are controllable.
///
///   \param[in] on If true, control is enabled, false it disable.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::SetControllable(const bool on)
{
    mControllableFlag = on;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports AccessControl related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::Receive(const Message* message)
{
    if(AcceptCommandMessage(message) == false)
    {
        return;
    }

    if(mControllerID == message->GetSourceID())
    {
        mControllerUpdateTime.SetCurrentTime();
    }

    switch(message->GetMessageCode())
    {
    case CONFIRM_CONTROL:
        {
            const ConfirmControl* command = dynamic_cast<const ConfirmControl*>(message);
            if(command == NULL)
                return;
            
            if(command->GetResponseCode() == ConfirmControl::ControlAccepted)
            {
                bool firstTime = true;

                    // Lock data.
                {
                    WriteLock wLock(mControlMutex);

                    // See if we already had control, and if not, then
                    // we re-acquired control of this component and must
                    // signal via callbacks, etc. of this event.
                    Address::Set::iterator controlledComponent = mControlledComponents.find(message->GetSourceID());
                    if(controlledComponent != mControlledComponents.end())
                    {
                        // If we get here, we have been trying to control this
                        // component. Check the control status flag.
                        std::map<Address, bool>::iterator flag = mControlFlags.find(message->GetSourceID());
                        if(flag->second == true)
                        {
                            // We already had control, nothing new here.
                            // So don't trigger events.
                            firstTime = false;
                        }
                    }

                    mControlledComponents.insert(message->GetSourceID());
                    mControlConfirmTimes[message->GetSourceID()].SetCurrentTime();
                    mControlFlags[message->GetSourceID()] = true;

                }
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    if(firstTime)
                    {
                        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                                  << "] - Acquired Control of " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
                    }
                    else
                    {
                        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                                  << "] - Maintained Control of " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
                    }
                }

                // Trigger callbacks if needed.
                if(firstTime)
                {
                    Service::Map children = GetChildServices();
                    Service::Map::iterator child;
                    for(child = children.begin();
                        child != children.end();
                        child++)
                    {
                        Child* controlChild = dynamic_cast<Child *>(child->second);
                        if(controlChild)
                        {
                            controlChild->ProcessAcquisitionOfControl(message->GetSourceID());
                        }
                    }
                    {
                        ReadLock cbrLock(mCallbacksMutex);
                        Callback::Set::iterator cb;
                        for(cb = mCallbacks.begin();
                            cb != mCallbacks.end();
                            cb++)
                        {
                            (*cb)->ProcessAcquisitionfControl(message->GetSourceID());
                        }
                    }
                }
            }
            else
            {
                Address::Set::iterator component;

                // Lock data.
                WriteLock wLock(mControlMutex);

                component = mControlledComponents.find(message->GetSourceID());
                if(component != mControlledComponents.end())
                {                    
                    std::map<Address, bool>::iterator flag;
                    flag = mMaintainFlags.find(message->GetSourceID());
                    if(flag != mMaintainFlags.end())
                    {
                        // If we didn't get control, and we are not
                        // trying to maintain control, remove the component
                        // from our lists. If we don't have high enough authority
                        // then don't bother trying to re-acquire control.
                        if(flag->second == false || command->GetMessageCode() == ConfirmControl::InsufficientAuthority)
                        {
                            EraseComponentControlInfo(message->GetSourceID());
                        }
                        else // Update control status
                        {
                            mControlFlags[message->GetSourceID()] = false;
                        }
                    }
                    // Delete junk data.
                    else
                    {
                        EraseComponentControlInfo(message->GetSourceID());
                    }
                }
            }
        }
        break;
    case QUERY_AUTHORITY:
        {
            if(mShutdownServiceFlag)
            {
                break;
            }
            
            ReportAuthority report(message->GetSourceID(), GetComponentID());
            {
                ReadLock rLock(mControlMutex);
                report.SetAuthorityCode(mAuthorityCode);
            }
            Send(&report);
        }
        break;
    case QUERY_CONTROL:
        {
            ReportControl report(message->GetSourceID(), GetComponentID());
            {
                ReadLock rLock(mControlMutex);
                report.SetAuthorityCode(mAuthorityCode);
                report.SetControllingComponent(mControllerID);
            }
            Send(&report);
        } 
        break;
    case QUERY_TIMEOUT:
        {
            ReportTimeout report(message->GetSourceID(), GetComponentID());
            {
                ReadLock rLock(mControlMutex);
                report.SetTimeoutSeconds(mTimeoutPeriod);
            }
            Send(&report);
        }
        break;
    case REJECT_CONTROL:
        {
            const RejectControl* command = dynamic_cast<const RejectControl*>(message);
            if(mShutdownServiceFlag || command == NULL)
                return;

            bool signalCallbacks = false;
            {
                WriteLock wLock(mControlMutex);
                // Update state of controlled components.
                Address::Set::iterator component;
                component = mControlledComponents.find(message->GetSourceID());
                if(component != mControlledComponents.end())
                {                
                    std::map<Address, bool>::iterator flag;
                    flag = mMaintainFlags.find(message->GetSourceID());
                    if(flag != mMaintainFlags.end())
                    {
                        // If we lost control, and we are not
                        // trying to maintain control, remove the component
                        // from our lists.
                        if(flag->second == false)
                        {
                            mMaintainFlags.erase(flag);
                            mControlFlags.erase(mControlFlags.find(message->GetSourceID()));
                            mControlConfirmTimes.erase(mControlConfirmTimes.find(message->GetSourceID()));
                            mControlledComponents.erase(component);
                        }
                        else // Update control status
                        {
                            mControlFlags[message->GetSourceID()] = false;
                        }
                        signalCallbacks = true;
                    }
                    // Delete junk data.
                    else
                    {
                        if(mControlFlags.find(*component) != mControlFlags.end())
                        {
                            mControlFlags.erase(*component);
                        }
                        mControlledComponents.erase(*component);
                    }
                }
            }
            if(signalCallbacks)
            {
                // Generate events and callbacks.
                ReadLock cbrLock(mCallbacksMutex);
                Callback::Set::iterator cb;
                for(cb = mCallbacks.begin();
                    cb != mCallbacks.end();
                    cb++)
                {
                    (*cb)->ProcessLossOfControl(message->GetSourceID());
                }
            }

            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - Lost Control of " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
            }
        }
        break;
    case RELEASE_CONTROL:
        {
            const ReleaseControl* command = dynamic_cast<const ReleaseControl*>(message);
            if(mShutdownServiceFlag || command == NULL)
                return;

            if(command->GetSourceID() == mControllerID)
            {
                RejectControl response(mControllerID, GetComponentID());

                bool controlReleased = true;
                Service::Map children = GetChildServices();
                Service::Map::iterator child;
                for(child = children.begin();
                    child != children.end();
                    child++)
                {
                    Child* controlChild = dynamic_cast<Child *>(child->second);
                    if(controlChild)
                    {
                        if(controlChild->ReleaseControl())
                        {
                            controlReleased;
                        }
                    }
                }
                if(controlReleased)
                {
                    response.SetResponseCode(RejectControl::ControlReleased);
                    Send(&response);

                    // Update data members of who was in control to nobody!
                    {
                        WriteLock wLock(mControlMutex);
                        mControllerID = Address();
                        mControllerAuthorityCode = 0;
                        mControllerUpdateTime.Clear();
                        mControllerCheckTime.Clear();
                    }
                    if(mDebugMessagesFlag)
                    {
                        WriteLock printLock(mDebugMessagesMutex);
                        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                            << "] - Control Released from " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
                    }

                    // Signal a change in control event
                    SignalEvent(REPORT_CONTROL);

                    // Notify children of control release.
                    {
                        // Generate events and callbacks.
                        ReadLock cbrLock(mCallbacksMutex);
                        Callback::Set::iterator cb;
                        for(cb = mCallbacks.begin();
                            cb != mCallbacks.end();
                            cb++)
                        {
                            (*cb)->ProcessReleaseOfControl(message->GetSourceID());
                        }
                    }
                }
                else
                {
                    response.SetResponseCode(RejectControl::NotAvailable);
                    Send(&response);
                }
            }
        }
        break;
    case REPORT_AUTHORITY:
        {
            const JAUS::ReportAuthority* report = dynamic_cast<const JAUS::ReportAuthority*>(message);
            if(report)
            {
                // Used by Discovery service to keep track of information
                // about components on the netework
            }
        }
        break;
    case REPORT_CONTROL:
        // Not used.
        break;
    case REPORT_TIMEOUT:
        {
            const ReportTimeout* report = dynamic_cast<const ReportTimeout*>(message);
            if(report == NULL)
                return;

            WriteLock wLock(mControlMutex);
            mTimeoutPeriods[message->GetSourceID()] = report->GetTimeoutSeconds();
            if(mTimeoutPeriods[message->GetSourceID()] == 0)
            {
                // We always want to check for control.
                mTimeoutPeriods[message->GetSourceID()] = 2;
            }
        }
        break;
    case REQUEST_CONTROL:
        {
            const RequestControl* command = dynamic_cast<const RequestControl*>(message);
            if(mShutdownServiceFlag || command == NULL)
                return;
            
            ConfirmControl response(message->GetSourceID(), GetComponentID());
#ifdef JAUS_USE_UPGRADE_LOCKS
            UpgradeLock upLock(mControlMutex);
#else
            WriteLock wLock(mControlMutex);
#endif
            // If they have minimum authority and we are
            // not under control or component has greater authority than
            // the current controller, accept.
            if(mControllableFlag && 
               command->GetAuthorityCode() >= mAuthorityCode &&
               (mControllerID == command->GetSourceID() || mControllerID.IsValid() == false || command->GetAuthorityCode() > mControllerAuthorityCode))
            {
                bool controlAccepted = true;
                Service::Map children = GetChildServices();
                Service::Map::iterator child;
                for(child = children.begin();
                    child != children.end();
                    child++)
                {
                    Child* controlChild = dynamic_cast<Child *>(child->second);
                    if(controlChild)
                    {
                        controlAccepted = controlChild->RequestControl();
                        if(controlAccepted)
                        {
                            break;
                        }
                    }
                }
                // This check is needed in case any child
                // service (Management) is in an Emergency state
                // and cannot transition.
                if(controlAccepted == false)
                {
                    response.SetResponseCode(ConfirmControl::NotAvailable);
                }
                else
                {
                    response.SetResponseCode(ConfirmControl::ControlAccepted);
                    if(mControllerID != command->GetSourceID() && mControllerID.IsValid())
                    {
                        // Notify controller that they have lost control.
                        RejectControl reject(mControllerID, GetComponentID());
                        reject.SetResponseCode(RejectControl::ControlReleased);
                        Send(&reject);
                    }
                    {
#ifdef JAUS_USE_UPGRADE_LOCKS
                        UpgradeToUniqueLock uniqueLock(upLock);
#endif
                        mControllerID = message->GetSourceID();
                        mControllerAuthorityCode = command->GetAuthorityCode();
                        mControllerCheckTime.SetCurrentTime();
                        mControllerUpdateTime.SetCurrentTime();
                    }
                    if(mDebugMessagesFlag)
                    {
                        WriteLock printLock(mDebugMessagesMutex);
                        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                            << "] - Control Granted to " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
                    }
                }
            }
            else
            {    
                if(mControllableFlag == false)
                {
                    response.SetResponseCode(ConfirmControl::NotAvailable);
                }
                else
                {                    
                    response.SetResponseCode(ConfirmControl::InsufficientAuthority);
                }      
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                              << "] - Control Rejected from " << message->GetSourceID().ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
                }
            }
            Send(&response);
        }
        break;
    case SET_AUTHORITY:
        {
            const SetAuthority* command = dynamic_cast<const SetAuthority*>(message);
            if(command == NULL)
                return;
            
            SetAuthorityCode(command->GetAuthorityCode());
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
Message* AccessControl::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case CONFIRM_CONTROL:
        message = new ConfirmControl();
        break;
    case QUERY_AUTHORITY:
        message = new QueryAuthority();
        break;
    case QUERY_CONTROL:
        message = new QueryControl();
        break;
    case QUERY_TIMEOUT:
        message = new QueryTimeout();
        break;
    case REJECT_CONTROL:
        message = new RejectControl();
        break;
    case RELEASE_CONTROL:
        message = new ReleaseControl();
        break;
    case REPORT_AUTHORITY:
        message = new ReportAuthority();
        break;
    case REPORT_CONTROL:
        message = new ReportControl();
        break;
    case REPORT_TIMEOUT:
        message = new ReportTimeout();
        break;
    case REQUEST_CONTROL:
        message = new RequestControl();
        break;
    case SET_AUTHORITY:
        message = new SetAuthority();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the value of the authority for the component/service.  This
///          value determines what components can request control and what
///          components you can take control of.
///
///   \param[in] authorityCode Authority level of the component.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::SetAuthorityCode(const Byte authorityCode)
{
    {
        WriteLock wLock(mControlMutex);
        mAuthorityCode = authorityCode;
    }
    SignalEvent(REPORT_AUTHORITY);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Check if control has been established for a component.
///
///   \param[in] id The ID of the component to check for control of.  If a
///                 ID of 0.0.0 is passed, then the method will check if we
///                 are in control of any components.
///
///   \return True if you have control, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::HaveControl(const Address& id) const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    
    if(id.IsValid() == false && mControlledComponents.size() != 0)
    {
        return true;
    }

    Address::Set::const_iterator c;
    c = mControlledComponents.find(id);
    if(c != mControlledComponents.end())
    {
        return mControlFlags.find(*c)->second;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the component is controlled, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccessControl::IsControlled() const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    return mControllerID.IsValid();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set the ID of controlling component.
///
///   \param[in] ID of the controlling component.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::SetControllerID(const Address& id)
{
    if(id.IsValid() && !id.IsBroadcast())
    {
        WriteLock wLock(mControlMutex);
        mControllerID = id;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return ID of the controlling component.
///
////////////////////////////////////////////////////////////////////////////////////
Address AccessControl::GetControllerID() const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    return mControllerID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Authority level of the controlling component.
///
////////////////////////////////////////////////////////////////////////////////////
Byte AccessControl::GetControllerAuthorityCode() const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    return mControllerAuthorityCode;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return List of all components currently controlled by the service.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List AccessControl::GetControlledComponents() const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    Address::List result;
    std::map<Address, bool>::const_iterator flags;
    for(flags = mControlFlags.begin();
        flags != mControlFlags.end();
        flags++)
    {
        if(flags->second)
        {
            result.push_back(flags->first);
        }
    }
    return result;
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
void AccessControl::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    RequestControl request;
    Address::Set lostComponents;
    Time currentTime;

    // Upgrade/Read lock
    {
#ifdef JAUS_USE_UPGRADE_LOCKS
        UpgradeLock uLock(mControlMutex);
#else
        WriteLock wLock(mControlMutex);
#endif

        request.SetSourceID(GetComponentID());
        request.SetAuthorityCode(mAuthorityCode);

        Address::Set::iterator component;
        
        currentTime.SetCurrentTime();
    
        for(component = mControlledComponents.begin();
            component != mControlledComponents.end();
            component++)
        {
            // If we are releasing control, do not check.
            if(mToReleaseControl.find(*component) != mToReleaseControl.end())
            {
                continue;
            }
            // If we have not checked within timeout - .x seconds, then re-acquire control to
            // maintain constant control of component.
            double thresh = mTimeoutPeriods[*component]*mTimeoutThreshold/100.0;
            double checkTimeDiff = currentTime.ToSeconds() - mControlCheckTimes[*component].ToSeconds();
            double timeThresh = (mTimeoutPeriods[*component] - thresh);
            bool checkConfirmation = true;
            if(mTimeoutPeriods[*component] > 0 && checkTimeDiff >= thresh)
            {
                request.SetDestinationID(*component);
                if(Send(&request))
                {
                    // Update the check time.
#ifdef JAUS_USE_UPGRADE_LOCKS
                    UpgradeToUniqueLock upLock(uLock);
#endif
                    mControlCheckTimes[*component].SetCurrentTime();
                    checkConfirmation = false;
                }
            }
            double confirmTimeDiff = currentTime.ToSeconds() - mControlConfirmTimes[*component].ToSeconds();
            timeThresh = (mTimeoutPeriods[*component] + thresh);
            // If we haven't been confirmed in this time, then we have lost control.
            if(checkConfirmation && confirmTimeDiff > timeThresh)
            {
                mControlFlags[*component] = false;
                lostComponents.insert(*component);
            }
        }
    
    }

    // Notify subscribers of any lost control events.
    {
        Address::Set::iterator lost;
        for(lost = lostComponents.begin();
            lost != lostComponents.end();
            lost++)
        {
            // Generate events and callbacks.
            ReadLock cbrLock(mCallbacksMutex);
            Callback::Set::iterator cb;
            for(cb = mCallbacks.begin();
                cb != mCallbacks.end();
                cb++)
            {
                (*cb)->ProcessLossOfControl(*lost);
            }
        }
    }

    Address controller; // Current controlling component. 

    bool timeout = false;
    {
#ifdef JAUS_USE_UPGRADE_LOCKS
        UpgradeLock uLock(mControlMutex);
#else
        WriteLock wLock(mControlMutex);
#endif

        controller = mControllerID;
        
        currentTime.SetCurrentTime();
        double timeSinceRequest = currentTime - mControllerCheckTime;
        if(mTimeoutPeriod > 0 && 
           controller.IsValid() && 
           timeSinceRequest >= (double)mTimeoutPeriod + mTimeoutPeriod*mTimeoutThreshold/100.0)
        {
            timeout = true;
            // Release control due to timeout.
            RejectControl reject(controller, GetComponentID());
            reject.SetResponseCode(RejectControl::ControlReleased);
            Send(&reject);
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() 
                          << "] - Control Time Out From " << controller.ToString() << " at " << Time::GetUtcTime().ToString() << "\n";
            }
            {
#ifdef JAUS_USE_UPGRADE_LOCKS
                UpgradeToUniqueLock upLock(uLock);
#endif
                // Clear values.
                mControllerID.Clear();
                mControllerAuthorityCode = 0;
                mControllerCheckTime.Clear();
                mControllerUpdateTime.Clear();
            }
        }
    }

    if(timeout)
    {
        {
            ReadLock cbrLock(mCallbacksMutex);
            Callback::Set::iterator cb;
            for(cb = mCallbacks.begin();
                cb != mCallbacks.end();
                cb++)
            {
                (*cb)->ProcessReleaseOfControl(controller);
            }
        }
        Service::Map children = GetChildServices();
        Service::Map::iterator child;
        for(child = children.begin();
            child != children.end();
            child++)
        {
            Child* controlChild = dynamic_cast<Child *>(child->second);
            if(controlChild)
            {
                controlChild->ReleaseControl();
            }
        }
        SignalEvent(REPORT_CONTROL);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the timeout period for controlling components to lose control
///          of this component.  If timeSeconds is greater than 0, then controlling
///          components must continuously re-acquire control within this window or
///          they will lose it due to timeout.
///
///   \param[in] timeSeconds How long before timeout occurs in seconds.  A value
///                          of 0 disables timeout.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::SetTimeoutPeriod(const Byte timeSeconds)
{
    WriteLock wLock(mControlMutex);
    mTimeoutPeriod = timeSeconds;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Timeout period for this component (seconds).
///
////////////////////////////////////////////////////////////////////////////////////
Byte AccessControl::GetTimeoutPeriod() const
{
    ReadLock rLock(*((SharedMutex*)&mControlMutex));
    return mTimeoutPeriod;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to register a callback to receive Control Events.
///
///   Control events include loss of control to a component we had control of
///   or release of control by a controlling component.
///
///   \param[in] callback Pointer to callback to add/remove.
///   \param[in] add If true, callback is added, if false, it is removed.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::RegisterCallback(Callback* callback, const bool add)
{
    WriteLock wLock(mCallbacksMutex);
    if(add)
    {
        mCallbacks.insert(callback);
    }
    else
    {
        Callback::Set::iterator cb;
        cb = mCallbacks.find(callback);
        if(cb != mCallbacks.end())
        {
            mCallbacks.erase(cb);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status info about the Service to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::PrintStatus() const
{

    std::cout << "[" << GetServiceID().ToString() << "] - " << GetComponentID().ToString() << "\n";

    Address controller;
    Byte authority;
    Address::Set controlled;
    std::map<Address, bool> flags;

    {
        ReadLock rLock(*((SharedMutex*)&mControlMutex));
        controller = mControllerID;
        authority = mControllerAuthorityCode;
        controlled = mControlledComponents;
        flags = mControlFlags;
    }
    if(controller.IsValid())
    {
        std::cout << "Controlled by: " 
                  << controller.ToString() 
                  << " Authority Code: " 
                  << (int)authority << std::endl;
    }
    if(controlled.size() > 0)
    {
        Address::Set::const_iterator c;
        std::cout << "In Control of the Following:\n";
        for(c = controlled.begin();
            c != controlled.end();
            c++)
        {
            if(flags.find(*c)->second)
            {
                std::cout << " - " << c->ToString() << std::endl;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all stored infor related to a component we are controlling.
///
////////////////////////////////////////////////////////////////////////////////////
void AccessControl::EraseComponentControlInfo(const Address& id)
{
    std::map<Address, Byte>::iterator tp;
    std::map<Address, Time>::iterator ct;
    std::map<Address, bool>::iterator flag;
    Address::Set::iterator c;

    tp = mTimeoutPeriods.find(id);
    if(tp != mTimeoutPeriods.end())
    {
        mTimeoutPeriods.erase(tp);
    }
    ct = mControlCheckTimes.find(id);
    if(ct != mControlCheckTimes.end())
    {
        mControlCheckTimes.erase(ct);
    }
    ct = mControlConfirmTimes.find(id);
    if(ct != mControlConfirmTimes.end())
    {
        mControlConfirmTimes.erase(ct);
    }

    flag = mMaintainFlags.find(id);
    if(flag != mMaintainFlags.end())
    {
        mMaintainFlags.erase(flag);
    }
    flag = mControlFlags.find(id);
    if(flag != mControlFlags.end())
    {
        mControlFlags.erase(flag);
    }

    c = mControlledComponents.find(id);
    if(c != mControlledComponents.end())
    {
        mControlledComponents.erase(c);
    }
}
/*  End of File */
