////////////////////////////////////////////////////////////////////////////////////
///
///  \file listmanager.cpp
///  \brief This file contains the implementation of the List Manager service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2 March 2010
///  <br>Copyright (c) 2010
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
#include "jaus/mobility/list/listmanager.h"
#include "jaus/mobility/list/confirmelementrequest.h"
#include "jaus/mobility/list/rejectelementrequest.h"
#include "jaus/mobility/list/deleteelement.h"
#include "jaus/mobility/list/executelist.h"
#include "jaus/mobility/list/queryelement.h"
#include "jaus/mobility/list/queryelementcount.h"
#include "jaus/mobility/list/queryelementlist.h"
#include "jaus/mobility/list/queryactiveelement.h"
#include "jaus/mobility/list/reportelement.h"
#include "jaus/mobility/list/reportelementlist.h"
#include "jaus/mobility/list/reportelementcount.h"
#include "jaus/mobility/list/reportactiveelement.h"
#include "jaus/mobility/list/setelement.h"
#include "jaus/core/component.h"

using namespace JAUS;

const std::string ListManager::Name = "urn:jaus:jss:mobility:ListManager";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] serviceIdentifier The name of your service.
///   \param[in] parentServiceIdentifier The parent service (ListManager).
///
////////////////////////////////////////////////////////////////////////////////////
ListManager::Child::Child(const Service::ID& serviceIdentifier,
                          const Service::ID& parentServiceIdentifier) : Management::Child(serviceIdentifier,
                                                                                          parentServiceIdentifier)
{
    mActiveElement = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ListManager::Child::~Child()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current active list element.
///
///   \param[in] uid ID of the list element to set as active.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
void ListManager::Child::SetActiveListElement(const UShort uid)
{
    Mutex::ScopedLock lock(&mListMutex);
    Element::Map::iterator e = mElementList.find(uid);
    // Safety check.
    if(e != mElementList.end())
    {
        mActiveElement = uid;
        Events::Child::SignalEvent(REPORT_ACTIVE_ELEMENT);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes the desired element from the list.
///
///   \param[in] id The ID of the element to delete.  After deletion the 
///                 list must either be empty, or not have any orphaned
///                 elements.  A value of JAUS_USHORT_MAX will delete all
///                 elements.
///   \param[out] rejectReason On failure, this is the reason why.  See
///                            RejectElementRequest::Response enumeration for
///                            values.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ListManager::Child::DeleteListElement(const UShort id, Byte& rejectReason)
{
    Mutex::ScopedLock lock(&mListMutex);
    Element::Map::iterator e;
    if(id == JAUS_USHORT_MAX)
    {
        mElementList.clear();
        mActiveElement = 0;
        return true;
    }
    e = mElementList.find(id);
    if(e != mElementList.end())
    {
        Element::Map::iterator prev;
        Element::Map::iterator next;
        prev = mElementList.find(e->second.mPrevID);
        next = mElementList.find(e->second.mNextID);
        if((e->second.mPrevID == 0 || prev != mElementList.end()) && 
           (e->second.mNextID == 0 || next != mElementList.end()))
        {
            if(prev != mElementList.end())
            {
                prev->second.mNextID = e->second.mNextID;
            }
            if(next != mElementList.end())
            {
                next->second.mPrevID = e->second.mPrevID;
            }
            mElementList.erase(e);
            return true;
        }
    }
    else
    {
        rejectReason = RejectElementRequest::InvalidElementID;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Inserts element(s) into the list.
///
///   \param[in] elements Element(s) to set.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ListManager::Child::SetElements(const Element::List& elements)
{
    bool result = false;

    Mutex::ScopedLock lock(&mListMutex);

    // Backup current element list.
    Element::Map backup = mElementList;
    //  Now insert into the list.
    Element::List::const_iterator ins;
    Element::Map::iterator prev;
    Element::Map::iterator next;

    for(ins = elements.begin();
        ins != elements.end();
        ins++)
    {
        if(ins->mID == 0) // Invalid element UID.
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << GetComponentID().ToString() << "] - Received Invalid Element ID\n";
            }
            return false;
        }
        mElementList[ins->mID] = (*ins);
        // Update any existing elements in the list.
        prev = mElementList.find(ins->mPrevID);
        if(prev != mElementList.end())
        {
            prev->second.mNextID = ins->mID;
        }
        next = mElementList.find(ins->mNextID);
        if(next != mElementList.end())
        {
            next->second.mPrevID = ins->mID;
        }
    }
    
    // Now validate the list.
    Element::Map::iterator e;
    
    std::vector<UShort> attached;

    bool success = false;
    bool foundLast = false;
    for(e = mElementList.begin();
        e != mElementList.end();
        e++)
    {
        if(e->second.mPrevID == 0)
        {
            // Found first, now iterator in order.
            success = true;
            attached.push_back(e->first);
            prev = e;
            next = mElementList.find(e->second.mNextID);
            while(next != mElementList.end())
            {
                // Verify next points back to prev.
                if(next->second.mPrevID == prev->first)
                {
                    attached.push_back(next->first);
                }
                prev = next;
                if(next->second.mNextID == 0)
                {
                    foundLast = true;
                    break;
                }
                next = mElementList.find(next->second.mNextID);
            }
            break;
        }
    }
    if(mElementList.size() == 1 && foundLast == false)
    {
        if(mElementList.begin()->second.mNextID == 0)
        {
            foundLast = true;
        }
    }

    if(success == false || attached.size() != mElementList.size() || foundLast == false)
    {
        mElementList = backup;
        return false;
    }
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current element in the list to execute.  If the element
///          is completed, then then call teh AdvanceListElement method
///          to advance to the next element.
///
///   \return Current element to execute (ID is 0 if no element or list is
///            complete).
///
////////////////////////////////////////////////////////////////////////////////////
Element ListManager::Child::GetActiveListElement() const
{
    Mutex::ScopedLock lock(&mListMutex);
    if(mActiveElement != 0)
    {
        Element::Map::const_iterator e;
        e = mElementList.find(mActiveElement);
        if(e != mElementList.end())
        {
            return e->second;
        }
    }
    /*
    else if(mElementList.size() > 0) //  Get the first element in the list.
    {
        Element::Map::const_iterator e;
        for(e = mElementList.begin();
            e != mElementList.end();
            e++)
        {
            if(e->second.mPrevID == 0)
            {
                return e->second;
            }
        }
    }
    */
    return Element(); // Blank element.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the desired element from the list.  If the value is 0, the 
///          first element in the list is returned.
///
///   \param[in] id The ID of the desired element to get.  If 0, the first
///                 in the list is returned.
///
///   \return Desired element (ID is 0 if no element or list is
///            complete).
///
////////////////////////////////////////////////////////////////////////////////////
Element ListManager::Child::GetElement(const UShort id) const
{
    Mutex::ScopedLock lock(&mListMutex);
    if(id != 0)
    {
        Element::Map::const_iterator e;
        e = mElementList.find(id);
        if(e != mElementList.end())
        {
            return e->second;
        }
    }
    /*
    else if(mElementList.size() > 0) //  Get the first element in the list.
    {
        Element::Map::const_iterator e;
        for(e = mElementList.begin();
            e != mElementList.end();
            e++)
        {
            if(e->second.mPrevID == 0)
            {
                return e->second;
            }
        }
    }
    */
    return Element(); // Blank element.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Advances the current active element in the list.  If the list
///          is complete (or execution just began), then the first
///          element is set as the active element.
///
////////////////////////////////////////////////////////////////////////////////////
void ListManager::Child::AdvanceListElement()
{
    Mutex::ScopedLock lock(&mListMutex);
    if(mActiveElement != 0)
    {
        Element::Map::const_iterator e;
        e = mElementList.find(mActiveElement);
        if(e != mElementList.end())
        {
            mActiveElement = e->second.mNextID;
            EventsService()->SignalEvent(REPORT_ACTIVE_ELEMENT);
        }
    }
    /*
    else if(mElementList.size() > 0) //  Get the first element in the list.
    {
        Element::Map::const_iterator e;
        for(e = mElementList.begin();
            e != mElementList.end();
            e++)
        {
            if(e->second.mPrevID == 0)
            {
                mActiveElement = e->first;
                EventsService()->SignalEvent(REPORT_ACTIVE_ELEMENT);
            }
        }
    }  
    */
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the element list for execution.
///
////////////////////////////////////////////////////////////////////////////////////
Element::Map ListManager::Child::GetElementList() const
{
    Mutex::ScopedLock lock(&mListMutex); 
    return mElementList;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the element list for execution.
///
////////////////////////////////////////////////////////////////////////////////////
void ListManager::Child::GetElementList(std::vector<UShort>& list) const
{
    Mutex::ScopedLock lock(&mListMutex); 
    list.clear();
    Element::Map::const_iterator e;
    for(e = mElementList.begin();
        e != mElementList.end();
        e++)
    {
        list.push_back(e->first);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the number of elements in the list.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int ListManager::Child::GetElementCount() const
{ 
    Mutex::ScopedLock lock(&mListMutex); 
    return (unsigned int)mElementList.size(); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ListManager::ListManager() : Management::Child(Service::ID(ListManager::Name),
                                               Service::ID(Management::Name))
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ListManager::~ListManager()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets the service (clears all lists).
///
////////////////////////////////////////////////////////////////////////////////////
bool ListManager::Reset()
{
    // Clears all lists.
    Service::Map::iterator child;
    Service::Map children = this->GetChildServices();
    for(child = children.begin();
        child != children.end();
        child++)
    {
        ListManager::Child* c = dynamic_cast<ListManager::Child*>(child->second);
        if(c)
        {
            Byte responseCode = RejectElementRequest::UnspecifiedError;
            c->DeleteListElement(JAUS_USHORT_MAX, responseCode);
        }
    }
    return true;
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
bool ListManager::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_ELEMENT_COUNT)
    {
        JAUS::ReportElementCount report;
        
        Service::Map::const_iterator child;
        const Service::Map children = this->GetChildServices();
        for(child = children.begin();
            child != children.end();
            child++)
        {
            ListManager::Child* c = dynamic_cast<ListManager::Child*>(child->second);
            if(c)
            {
                report.SetElementCount(c->GetElementCount());
                break;
            }
        }

        SendEvent(info, &report);
        return true;
    }
    if(info.mpQueryMessage->GetMessageCode() == QUERY_ELEMENT_COUNT)
    {
        JAUS::ReportElementList report;
        
        Service::Map::const_iterator child;
        const Service::Map children = this->GetChildServices();
        for(child = children.begin();
            child != children.end();
            child++)
        {
            ListManager::Child* c = dynamic_cast<ListManager::Child*>(child->second);
            if(c)
            {
                c->GetElementList(*report.GetElementList());
                break;
            }
        }

        SendEvent(info, &report);
        return true;
    }
    if(info.mpQueryMessage->GetMessageCode() == QUERY_ACTIVE_ELEMENT)
    {
        JAUS::ReportActiveElement report;
        
        Service::Map::const_iterator child;
        const Service::Map children = this->GetChildServices();
        for(child = children.begin();
            child != children.end();
            child++)
        {
            ListManager::Child* c = dynamic_cast<ListManager::Child*>(child->second);
            if(c)
            {
                report.SetElementUID(c->GetActiveListElement().mID);
                break;
            }
        }

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
bool ListManager::IsEventSupported(const Events::Type type,
                                   const double requestedPeriodicRate,
                                   const Message* queryMessage,
                                   double& confirmedPeriodicRate,
                                   std::string& errorMessage) const
{
    bool result = false;
    switch(queryMessage->GetMessageCode())
    {
    case QUERY_ELEMENT_COUNT:
    case QUERY_ELEMENT_LIST:
    case QUERY_ACTIVE_ELEMENT:
        {
            result = true;
            confirmedPeriodicRate = requestedPeriodicRate;
        }
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
void ListManager::Receive(const Message* message)
{
    // Find our ListManager child to perform operations with.

    Service::Map::iterator child;
    Service::Map children = this->GetChildServices();
    ListManager::Child* list = NULL;
    Byte responseCode = RejectElementRequest::UnspecifiedError;
    for(child = children.begin();
        child != children.end();
        child++)
    {
        list = dynamic_cast<ListManager::Child*>(child->second);
        if(list)
        {
            break;
        }
    }

    switch(message->GetMessageCode())
    {    
    case DELETE_ELEMENT:
        {
            const JAUS::DeleteElement* command = dynamic_cast<const JAUS::DeleteElement*>(message);
            if(command)
            {
                bool success = false;

                Byte responseCode = RejectElementRequest::UnspecifiedError;

                if(list)
                {
                    success = true;
                    for(std::vector<UShort>::const_iterator e = command->GetElementList()->begin();
                        e != command->GetElementList()->end();
                        e++)
                    {
                        if(list->DeleteListElement(*e, responseCode) == false)
                        {
                            success = false;
                            break;
                        }
                    }
                }

                if(success == false)
                {
                    RejectElementRequest reject(command->GetSourceID(), GetComponentID());
                    reject.SetRequestID(command->GetRequestID());
                    reject.SetResponseCode((RejectElementRequest::Response)responseCode);
                    Send(&reject);
                }
                else
                {
                    ConfirmElementRequest confirm(command->GetSourceID(), GetComponentID());
                    confirm.SetRequestID(command->GetRequestID());
                    Send(&confirm);
                    Events::Child::SignalEvent(REPORT_ELEMENT_COUNT);
                    Events::Child::SignalEvent(REPORT_ELEMENT_LIST);
                }
            }
        }
        break;
    case QUERY_ELEMENT:
        {
            const JAUS::QueryElement* query = dynamic_cast<const JAUS::QueryElement*>(message);
            if(query)
            {
                JAUS::ReportElement report(message->GetSourceID(), GetComponentID());

                if(list)
                {
                    Element e = list->GetElement(query->GetElementUID());
                    if(e.mID != 0)
                    {
                        report.SetElement(e);
                        Send(&report);
                    }
                }
            }
        }
        break;
    case QUERY_ELEMENT_COUNT:
        {
            const JAUS::QueryElementCount* query = dynamic_cast<const JAUS::QueryElementCount*>(message);
            if(query)
            {
                JAUS::ReportElementCount report(message->GetSourceID(), GetComponentID());
                if(list)
                {
                    report.SetElementCount(list->GetElementCount());
                    Send(&report);
                }

            }
        }
        break;
    case QUERY_ELEMENT_LIST:
        {
            const JAUS::QueryElementList* query = dynamic_cast<const JAUS::QueryElementList*>(message);
            if(query)
            {
                JAUS::ReportElementList report(message->GetSourceID(), GetComponentID());
                if(list)
                {
                    list->GetElementList(*report.GetElementList());
                    Send(&report);
                }
            }
        }
        break; 
    case QUERY_ACTIVE_ELEMENT:
        {
            const JAUS::QueryActiveElement* query = dynamic_cast<const JAUS::QueryActiveElement*>(message);
            if(query)
            {
                JAUS::ReportActiveElement report(message->GetSourceID(), GetComponentID());
                if(list)
                {
                    report.SetElementUID(list->mActiveElement);
                    Send(&report);
                }
            }
        }
        break;
    case SET_ELEMENT:
        {
            const JAUS::SetElement* command = dynamic_cast<const JAUS::SetElement*>(message);
            if(command)
            {
                bool success = false;
                Byte responseCode = RejectElementRequest::UnspecifiedError;

                if(list)
                {
                    success = true;
                    Element::List final;
                    for(Element::List::const_iterator e = command->GetElementList()->begin();
                        e != command->GetElementList()->end();
                        e++)
                    {
                        Element copy = (*e);
                        if(copy.mpElement == NULL)
                        {
                            copy.mpElement = GetComponent()->TransportService()->CreateMessageFromPacket(copy.mPayload);
                        }
                        if(copy.mpElement)
                        {
                            copy.mpElement->CopyHeaderData(command);
                        }
                        if(copy.mpElement == NULL ||
                           list->IsElementSupported(copy.mpElement) == false)
                        {
                            success = false;
                            break;
                        }
                        final.push_back(copy);
                    }
                    // Now try to set the data.
                    if(list->SetElements(final) == false)
                    {
                        success = false;
                    }
                }
                if(success == false)
                {
                    RejectElementRequest reject(command->GetSourceID(), GetComponentID());
                    reject.SetRequestID(command->GetRequestID());
                    reject.SetResponseCode((RejectElementRequest::Response)responseCode);
                    Send(&reject);
                }
                else
                {
                    ConfirmElementRequest confirm(command->GetSourceID(), GetComponentID());
                    confirm.SetRequestID(command->GetRequestID());
                    Send(&confirm);
                    Events::Child::SignalEvent(REPORT_ELEMENT_COUNT);
                    Events::Child::SignalEvent(REPORT_ELEMENT_LIST);
                }
            }
        }
        break;
    case CONFIRM_ELEMENT_REQUEST:
        {
            const JAUS::ConfirmElementRequest* command = dynamic_cast<const JAUS::ConfirmElementRequest*>(message);
            if(command)
            {
            }
        }
        break;
    case REJECT_ELEMENT_REQUEST:
    case REPORT_ELEMENT:
    case REPORT_ELEMENT_COUNT:
    case REPORT_ELEMENT_LIST:
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
Message* ListManager::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case SET_ELEMENT:
        message = new JAUS::SetElement();
        break;
    case CONFIRM_ELEMENT_REQUEST:
        message = new JAUS::ConfirmElementRequest();
        break;
    case DELETE_ELEMENT:
        message = new JAUS::DeleteElement();
        break;
    case EXECUTE_LIST:
        message = new JAUS::ExecuteList();
        break;
    case QUERY_ELEMENT:
        message = new JAUS::QueryElement();
        break;
    case QUERY_ACTIVE_ELEMENT:
        message = new JAUS::QueryActiveElement();
        break;
    case QUERY_ELEMENT_COUNT:
        message = new JAUS::QueryElementCount();
        break;
    case QUERY_ELEMENT_LIST:
        message = new JAUS::QueryElementList();
        break;
    case REJECT_ELEMENT_REQUEST:
        message = new JAUS::RejectElementRequest();
        break;
    case REPORT_ELEMENT:
        message = new JAUS::ReportElement();
        break;
    case REPORT_ELEMENT_COUNT:
        message = new JAUS::ReportElementCount();
        break;
    case REPORT_ELEMENT_LIST:
        message = new JAUS::ReportElementList();
        break;
    case REPORT_ACTIVE_ELEMENT:
        message = new JAUS::ReportActiveElement();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}



/*  End of File */
