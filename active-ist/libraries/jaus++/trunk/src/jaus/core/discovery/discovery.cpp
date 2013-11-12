////////////////////////////////////////////////////////////////////////////////////
///
///  \file discovery.cpp
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
#include "jaus/core/discovery/discovery.h"
#include "jaus/core/discovery/queryconfiguration.h"
#include "jaus/core/discovery/queryidentification.h"
#include "jaus/core/discovery/queryservices.h"
#include "jaus/core/discovery/querysubsystemlist.h"
#include "jaus/core/discovery/registerservices.h"
#include "jaus/core/discovery/reportconfiguration.h"
#include "jaus/core/discovery/reportidentification.h"
#include "jaus/core/discovery/reportservices.h"
#include "jaus/core/discovery/reportsubsystemlist.h"
#include "jaus/core/management/querystatus.h"
#include "jaus/core/management/reportstatus.h"
#include "jaus/core/control/queryauthority.h"
#include "jaus/core/control/setauthority.h"
#include "jaus/core/control/reportauthority.h"
#include "jaus/core/events/event.h"
#include "jaus/core/transport/transport.h"
#include "jaus/core/component.h"

#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace JAUS;

const std::string Discovery::Name = "urn:jaus:jss:core:Discovery";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery::Discovery() : Events::Child(Service::ID(Discovery::Name), Service::ID(Events::Name))
{
    mBroadcastDelayMs = 5000;
    mBroadcastTimeMs = 0;
    mDiscoverSubsystemsFlag = true;
    mComponentIdentification = "Component";
    mNodeIdentification = "Node";
    mSubsystemType = Subsystem::OtherSubsystem;
    mTriggerCallbacksFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery::~Discovery()
{
    {
        WriteLock cbLock(mCallbacksMutex);
        mCallbacks.clear();
    }
    {
        WriteLock wLock(mSubsystemDataMutex);
        Subsystem::DeleteSubsystemMap(mSystem);
    }
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
bool Discovery::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);
    TiXmlNode* node;
    TiXmlElement* element;
    node = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("Frequency").FirstChild().ToNode();
    if(node && node->Value())
    {
        SetDiscoveryFrequency(atof(node->Value()));
    }
    node = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("IntervalMs").FirstChild().ToNode();
    if(node && node->Value())
    {
        SetDiscoveryFrequency(1000.0/atof(node->Value()));
    }
    node = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("DiscoverSubsystems").FirstChild().ToNode();
    if(node && node->Value())
    {
        DiscoverSubsystems(atoi(node->Value()) > 0 ? true : false);
    }
    node = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("ComponentIdentification").FirstChild().ToNode();
    if(node && node->Value())
    {
        mComponentIdentification = node->Value();
    }
    node = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("NodeIdentification").FirstChild().ToNode();
    if(node && node->Value())
    {
        mNodeIdentification = node->Value();
    }
    element = doc.FirstChild("JAUS").FirstChild("Discovery").FirstChild("SubsystemIdentification").ToElement();
    if(element && element->Attribute("type") && element->FirstChild() && element->FirstChild()->Value())
    {
        mSubsystemType = (Subsystem::Type)atoi(element->Attribute("type"));
        mSubsystemIdentification = element->FirstChild()->Value();
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called on initialization.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::Initialize()
{
    if(mSystem.size() == 0)
    {
        if(mDiscoverSubsystemsFlag)
        {
            mSubsystemList.insert(GetComponentID());
        }
        if(mSubsystemType == Subsystem::Vehicle)
        {
            mSystem[GetComponentID().mSubsystem].reset(new Vehicle());
        }
        else
        {
            mSystem[GetComponentID().mSubsystem].reset(new Subsystem());
        }
        mSystem[GetComponentID().mSubsystem]->mIdentification = mSubsystemIdentification;
        Service::ID::Set services = GetComponent()->GetServiceSet();
        mSystem[GetComponentID().mSubsystem]->AddComponent(GetComponentID(), services);
        mSystem[GetComponentID().mSubsystem]->GetComponent(GetComponentID())->mIdentification = mComponentIdentification;
        mSystem[GetComponentID().mSubsystem]->GetComponent(GetComponentID())->mAuthorityLevel = this->GetComponent()->AccessControlService()->GetAuthorityCode();
        mSystem[GetComponentID().mSubsystem]->mUpdateTime.SetCurrentTime();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the service.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::Shutdown()
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::DeleteSubsystemMap(mSystem);
    mSubsystemList.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports Discovery related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::Receive(const Message* message)
{
    Subsystem::Map::iterator myInfo = mSystem.find(GetComponentID().mSubsystem);
    if(myInfo == mSystem.end())
    {
        Subsystem* ptr = NULL;
        // Initialize configuration and services information
        // for this subsystem.
        if(mSubsystemType == Subsystem::Vehicle)
        {
            ptr = new Vehicle();
            mSystem[GetComponentID().mSubsystem].reset(ptr);
            ((Vehicle*)ptr)->mAuthorityLevel = GetComponent()->AccessControlService()->GetAuthorityCode();
        }
        else
        {
            ptr = new Subsystem();
            mSystem[GetComponentID().mSubsystem].reset(ptr);
        }
        ptr->mType = mSubsystemType;
        ptr->mIdentification = mSubsystemIdentification;
        ptr->mSubsystemID = GetComponentID().mSubsystem;
        ptr->GetComponent(GetComponentID())->mIdentification = mComponentIdentification;
        ptr->GetComponent(GetComponentID())->mServices = GetTransportService()->GetServices();
        ptr->GetComponent(GetComponentID())->mAuthorityLevel = GetComponent()->AccessControlService()->GetAuthorityCode();
    }

    switch(message->GetMessageCode())
    {
    case QUERY_CONFIGURATION:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Message Type [QueryConfiguration] from " << message->GetSourceID().ToString() << std::endl;
            }
            const QueryConfiguration* query = dynamic_cast<const QueryConfiguration*>(message);
            if(query)
            {
                ReportConfiguration response(query->GetSourceID(), GetComponentID());

                ReadLock rLock(mSubsystemDataMutex);
                Subsystem::Map::const_iterator s = mSystem.find(GetComponentID().mSubsystem);
                if(s != mSystem.end())
                {
                    Subsystem::Configuration::const_iterator node;
                    Subsystem::Component::Set::const_iterator component;

                    // Specific node.
                    if(query->GetQueryType() == QueryConfiguration::NodeConfiguration)
                    {
                        node = s->second->mConfiguration.find(GetComponentID().mNode);
                        if(node != s->second->mConfiguration.end())
                        {
                            for(component = node->second.begin();
                                component != node->second.end();
                                component++)
                            {
                                (*(response.GetNodes()))[node->first].push_back(ReportConfiguration::Record(component->mID.mComponent, 0));
                            }
                        }
                    }
                    // All nodes on subsystem.
                    else
                    {
                        for(node = s->second->mConfiguration.begin();
                            node != s->second->mConfiguration.end();
                            node++)
                        {
                            for(component = node->second.begin();
                                component != node->second.end();
                                component++)
                            {
                                (*(response.GetNodes()))[node->first].push_back(ReportConfiguration::Record(component->mID.mComponent, 0));
                            }
                        }
                    }
                }

                Send(&response);
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Sent Message Type [ReportConfiguration] to " << message->GetSourceID().ToString() << std::endl;
                    response.PrintMessageBody();
                }
            }
        }
        break;
    case QUERY_IDENTIFICATION:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Message Type [QueryIdentification] from " << message->GetSourceID().ToString() << std::endl;
            }
            const QueryIdentification* query = dynamic_cast<const QueryIdentification*>(message);
            bool haveData = false;
            ReportIdentification response(query->GetSourceID(), GetComponentID());
                
            response.SetQueryType((ReportIdentification::QueryType)query->GetQueryType());

            switch(query->GetQueryType())
            {
            case QueryIdentification::SubsystemIdentification:
                {
                    if(mSubsystemIdentification.size() > 0)
                    {
                        haveData = true;
                        response.SetIdentification(mSubsystemIdentification);
                        response.SetType((ReportIdentification::IdentificationType)mSubsystemType);
                    }

                    ReadLock rLock(mSubsystemDataMutex);
                    // Make sure we have required information from subsystems.
                    Subsystem::Map::const_iterator s = mSystem.find(query->GetSourceID().mSubsystem);
                    if(s == mSystem.end() || s->second->mIdentification.size() == 0)
                    {
                        QueryIdentification myQuery(query->GetSourceID(),  query->GetDestinationID());
                        myQuery.SetQueryType(QueryIdentification::SubsystemIdentification);
                        Send(&myQuery);
                    }
                }
                break;
            case QueryIdentification::NodeIdentification:
                if(mNodeIdentification.size() > 0)
                {
                    haveData = true;
                    response.SetIdentification(mNodeIdentification);
                    response.SetType(ReportIdentification::Node);
                }
                break;
            case QueryIdentification::ComponentIdentification:
                if(mNodeIdentification.size() > 0)
                {
                    haveData = true;
                    response.SetIdentification(mComponentIdentification);
                    response.SetType(ReportIdentification::Component);
                }
                break;
            default:
                break;
            }
            // Do as much as possible to get information about a subsystem.
            {
                ReadLock rLock(mSubsystemDataMutex);
                Subsystem::Map::iterator subsystem = mSystem.find(query->GetSourceID().mSubsystem);
                if(subsystem == mSystem.end() || subsystem->second->mConfiguration.size() == 0)
                {
                    QueryConfiguration queryConfig(query->GetSourceID(), GetComponentID());
                    queryConfig.SetQueryType(QueryConfiguration::SubsystemConfiguration);
                    Send(&queryConfig);
                }
            }

            if(haveData)
            {
                Send(&response);
            }
            {
                WriteLock wLock(mSubsystemDataMutex);
                mSubsystemList.insert(query->GetSourceID());
                Subsystem::Map::iterator s = mSystem.find(GetComponentID().mSubsystem);
                if(s != mSystem.end())
                {
                    s->second->mUpdateTime.SetCurrentTime();
                }
            }
            if(query->GetSourceID().mSubsystem == GetComponentID().mSubsystem)
            {
                this->SignalEvent(REPORT_SUBSYSTEM_LIST);
            }
        }
        break;
    case QUERY_SERVICES:
        {
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Message Type [QueryServices] from " << message->GetSourceID().ToString() << std::endl;
            }
            const QueryServices* query = dynamic_cast<const QueryServices*>(message);
            if(query)
            {
                ReadLock rLock(mSubsystemDataMutex);
                Subsystem::Map::const_iterator s = mSystem.find(GetComponentID().mSubsystem);
                if(s != mSystem.end())
                {
                    ReportServices report(query->GetSourceID(), GetComponentID());
                    Subsystem::Configuration::const_iterator node;
                    QueryServices::List::const_iterator queryNode;

                    // For each node the query wants data for.
                    for(queryNode = query->GetNodeList()->begin();
                        queryNode != query->GetNodeList()->end();
                        queryNode++)
                    {
                        node = s->second->mConfiguration.find(queryNode->first);
                        if(node != s->second->mConfiguration.end())
                        {
                            // For each component on the node being requested
                            std::vector<Byte>::const_iterator queryComponent;
                            for(queryComponent = queryNode->second.begin();
                                queryComponent != queryNode->second.end();
                                queryComponent++)
                            {
                                // If we have the component service data, add it to the report.
                                Subsystem::Component::Set::const_iterator component;
                                for(component = node->second.begin();
                                    component != node->second.end();
                                    component++)
                                {
                                    if(component->mID.mComponent == *queryComponent)
                                    {
                                        ReportServices::Record record(component->mID.mComponent);
                                        record.mServices = component->mServices;
                                        (*report.GetServices())[node->first].push_back(record);
                                        break;
                                    }
                                }
                            }
                        }
                    }
                    if(query->GetNodeList()->size() == 0)
                    {
                        for(node = s->second->mConfiguration.begin();
                            node != s->second->mConfiguration.end();
                            node++)
                        {
                            // If we have the component service data, add it to the report.
                            Subsystem::Component::Set::const_iterator component;
                            for(component = node->second.begin();
                                component != node->second.end();
                                component++)
                            {
                                ReportServices::Record record(component->mID.mComponent);
                                record.mServices = component->mServices;
                                (*report.GetServices())[node->first].push_back(record);
                            }
                        }
                    }
                    Send(&report);
                }
            }
        }
        break;
    case QUERY_SUBSYSTEM_LIST:
        {
            ReportSubsystemList report(message->GetSourceID(), GetComponentID());

            {
                ReadLock rLock(mSubsystemDataMutex);
                *report.GetSubsystemList() = Address::ToList(mSubsystemList);
            }
            Send(&report);
        }
        break;
    case REGISTER_SERVICES:
        {
            const RegisterServices* registerServices = dynamic_cast<const RegisterServices*>(message);
            if(registerServices)
            {
                Subsystem::Map::iterator subsystem;
                WriteLock wLock(mSubsystemDataMutex);

                subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                
                if(subsystem == mSystem.end())
                {
                    mSystem[message->GetSourceID().mSubsystem].reset(new Subsystem());
                    subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                    subsystem->second->mSubsystemID = subsystem->first;
                }
                // Update known services and component/subsystem update times.
                subsystem->second->GetComponent(message->GetSourceID())->mServices = *registerServices->GetServices();
                subsystem->second->GetComponent(message->GetSourceID())->mUpdateTime.SetCurrentTime();
                subsystem->second->mUpdateTime.SetCurrentTime();
            }
        }
        break;
    case REPORT_CONFIGURATION:
        {
            const ReportConfiguration* report = dynamic_cast<const ReportConfiguration*>(message);
            if(report)
            {
                Subsystem::Map::iterator subsystem;                
                QueryServices query(report->GetSourceID(), GetComponentID());
                {
                    WriteLock wLock(mSubsystemDataMutex);

                    subsystem = mSystem.find(message->GetSourceID().mSubsystem);

                    if(subsystem == mSystem.end())
                    {
                        mSystem[message->GetSourceID().mSubsystem].reset(new Subsystem());
                        subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                        subsystem->second->mSubsystemID = subsystem->first;
                    }

                    Address id(report->GetSourceID().mSubsystem, 0, 0);

                    // Create any components needed, and send appropriate queries.
                    ReportConfiguration::Nodes::const_iterator node;
                    for(node = report->GetNodes()->begin();
                        node != report->GetNodes()->end();
                        node++)
                    {
                        ReportConfiguration::Record::List::const_iterator record;
                        id.mNode = node->first;                    
                        for(record = node->second.begin();
                            record != node->second.end();
                            record++)
                        {
                            id.mComponent = record->mComponent;
                            subsystem->second->GetComponent(id);

                            (*(query.GetNodeList()))[id.mNode].push_back(id.mComponent);

                            // Query additional data for each component.
                            QueryAuthority queryAuthority(id, GetComponentID());
                            Send(&queryAuthority);
                            QueryStatus queryStatus(id, GetComponentID());
                            Send(&queryStatus);
                        }
                    }

                    subsystem->second->mUpdateTime.SetCurrentTime();
                }

                if(query.GetNodeList()->size() == 0)
                {
                    (*(query.GetNodeList()))[report->GetSourceID().mNode].push_back(report->GetSourceID().mComponent);
                }
                Send(&query);

                if(report->GetSourceID().mSubsystem == GetComponentID().mSubsystem)
                {
                    this->SignalEvent(REPORT_CONFIGURATION);
                }
            }
        }
        break;
    case REPORT_IDENTIFICATION:
        {
            const ReportIdentification* report = dynamic_cast<const ReportIdentification*>(message);
            if(report)
            {                
                Subsystem::Map::iterator subsystem;

                // Publish services for the component.
                RegisterServices registerServices(report->GetSourceID(), GetComponentID());
                *registerServices.GetServices() = GetTransportService()->GetServices();
                Send(&registerServices);  
                
                WriteLock wLock(mSubsystemDataMutex);

                subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                
                if(subsystem != mSystem.end())
                {
                    if(report->GetQueryType() == ReportIdentification::SubsystemIdentification &&
                        report->GetType() == ReportIdentification::Vehicle)
                    {
                        // If we are getting the subsystem identification after we
                        // already created memory for the subsystem, then we must
                        // convert to a Vehicle Subsystem type to store other data
                        // in it at a future time.
                        Vehicle* vehicle = dynamic_cast<Vehicle*>(subsystem->second.get());
                        if(vehicle == NULL)
                        {
                            vehicle = new Vehicle();
                            vehicle->mSubsystemID = subsystem->first;
                            vehicle->CopySubsystemData(subsystem->second.get());
                            subsystem->second.reset(vehicle);
                        }
                    }
                }
                else
                {
                    if(report->GetType() == ReportIdentification::Vehicle)
                    {
                        mSystem[message->GetSourceID().mSubsystem].reset(new Vehicle());
                    }
                    else
                    {
                        mSystem[message->GetSourceID().mSubsystem].reset(new Subsystem());
                    }
                    subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                    subsystem->second->mSubsystemID = subsystem->first;
                }

                switch(report->GetQueryType())
                {
                case ReportIdentification::SubsystemIdentification:
                    {
                        subsystem->second->mIdentification = report->GetIdentification();
                        subsystem->second->mType = (Subsystem::Type)report->GetType();

                        // Query the configuration of the subsystem.
                        QueryConfiguration querySubsystemConfig(report->GetSourceID(), GetComponentID());
                        querySubsystemConfig.SetQueryType(QueryConfiguration::SubsystemConfiguration);
                        Send(&querySubsystemConfig);

                        if(subsystem->first == GetComponentID().mSubsystem)
                        {
                            mSubsystemIdentification = report->GetIdentification();
                            mSubsystemType = (Subsystem::Type)report->GetType();
                        }
                    }
                    break;
                case ReportIdentification::NodeIdentification:
                    {
                        if(subsystem->first == GetComponentID().mSubsystem)
                        {
                            mNodeIdentification = report->GetIdentification();
                        }
                        // Query the configuration of the subsystem.
                        QueryConfiguration queryConfig(report->GetSourceID(), GetComponentID());
                        if(subsystem->second->mConfiguration.size() == 0)
                        {
                            queryConfig.SetQueryType(QueryConfiguration::SubsystemConfiguration);
                        }
                        else
                        {
                            queryConfig.SetQueryType(QueryConfiguration::NodeConfiguration);
                        }
                        Send(&queryConfig);
                    }

                    break;
                case ReportIdentification::ComponentIdentification:
                    {
                        Subsystem::Component* component = subsystem->second->GetComponent(report->GetSourceID());
                        component->mID = report->GetSourceID();
                        component->mIdentification = report->GetIdentification();
                        component->mUpdateTime.SetCurrentTime();

                        if(subsystem->second->mConfiguration.size() == 0)
                        {
                            QueryConfiguration queryConfig(report->GetSourceID(), GetComponentID());
                            queryConfig.SetQueryType(QueryConfiguration::SubsystemConfiguration);
                            Send(&queryConfig);
                        }
                    }
                    break;
                default:
                    break;
                }
                
                subsystem->second->mUpdateTime.SetCurrentTime();
            }
        }
        break;
    case REPORT_SERVICES:
        {
            const ReportServices* report = dynamic_cast<const ReportServices*>(message);
            if(report)
            {
                WriteLock wLock(mSubsystemDataMutex);

                Subsystem::Map::iterator subsystem;
                subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                
                if(subsystem == mSystem.end())
                {
                    mSystem[message->GetSourceID().mSubsystem].reset(new Subsystem());
                    subsystem = mSystem.find(message->GetSourceID().mSubsystem);
                    subsystem->second->mSubsystemID = subsystem->first;
                }

                ReportServices::Services::const_iterator service;
                for(service = report->GetServices()->begin();
                    service != report->GetServices()->end();
                    service++)
                {
                    Address id(report->GetSourceID().mSubsystem,
                               service->first, 0);

                    ReportServices::Record::List::const_iterator record;
                    for(record = service->second.begin();
                        record != service->second.end();
                        record++)
                    {
                        id.mComponent = record->mComponent;
                        subsystem->second->GetComponent(id)->mServices = record->mServices;
                        subsystem->second->GetComponent(id)->mUpdateTime.SetCurrentTime();
                    }
                }

                subsystem->second->mUpdateTime.SetCurrentTime();

            }
        }
        break;
    case SET_AUTHORITY:
        {
            const SetAuthority* command = dynamic_cast<const SetAuthority*>(message);
            if(command == NULL)
                return;

            WriteLock wLock(mSubsystemDataMutex);
            Subsystem::Map::iterator s;
            s = mSystem.find(command->GetDestinationID().mSubsystem);
            if(s != mSystem.end())
            {
                s->second->GetComponent(GetComponentID())->mAuthorityLevel = command->GetAuthorityCode();
                Vehicle* vehicle = dynamic_cast<Vehicle*>(s->second.get());
                if(vehicle && vehicle->mAuthorityLevel < command->GetAuthorityCode())
                {
                    vehicle->mAuthorityLevel = command->GetAuthorityCode();
                }
            }
        };
        break;
    case REPORT_AUTHORITY:
        {
            const ReportAuthority* report = dynamic_cast<const ReportAuthority*>(message);
            if(report == NULL)
                return;

            WriteLock wLock(mSubsystemDataMutex);
            Subsystem::Map::iterator s;
            s = mSystem.find(report->GetSourceID().mSubsystem);
            if(s != mSystem.end())
            {
                s->second->GetComponent(report->GetSourceID())->mAuthorityLevel = report->GetAuthorityCode();
                Vehicle* vehicle = dynamic_cast<Vehicle*>(s->second.get());
                if(vehicle && vehicle->mAuthorityLevel < report->GetAuthorityCode())
                {
                    vehicle->mAuthorityLevel = report->GetAuthorityCode();
                }
            }
        };
        break;
    case REPORT_STATUS:
        {
            const ReportStatus* report = dynamic_cast<const ReportStatus*>(message);
            if(report == NULL)
                return;

            WriteLock wLock(mSubsystemDataMutex);
            Subsystem::Map::iterator s;
            s = mSystem.find(report->GetSourceID().mSubsystem);
            if(s != mSystem.end())
            {
                s->second->GetComponent(report->GetSourceID())->mStatus = report->GetStatus();
            }
        };
        break;
    case REPORT_SUBSYSTEM_LIST:
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
Message* Discovery::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_CONFIGURATION:
        message = new QueryConfiguration();
        break;
    case QUERY_IDENTIFICATION:
        message = new QueryIdentification();
        break;
    case QUERY_SERVICES:
        message = new QueryServices();
        break;
    case QUERY_SUBSYSTEM_LIST:
        message = new QuerySubsystemList();
        break;
    case REGISTER_SERVICES:
        message = new RegisterServices();
        break;
    case REPORT_CONFIGURATION:
        message = new ReportConfiguration();
        break;
    case REPORT_IDENTIFICATION:
        message = new ReportIdentification();
        break;
    case REPORT_SERVICES:
        message = new ReportServices();
        break;
    case REPORT_SUBSYSTEM_LIST:
        message = new ReportSubsystemList();
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
void Discovery::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    bool updateSystemInfo = false;
    if(Time::GetUtcTimeMs() - mBroadcastTimeMs >= mBroadcastDelayMs)
    {
        if(mDiscoverSubsystemsFlag)
        {
            QueryIdentification query(Address(Address::GlobalBroadcast, 255, 255), GetComponentID());
            query.SetQueryType(QueryIdentification::SubsystemIdentification);
            // Global broadcast for discovery.
            Send(&query, Service::GlobalBroadcast);
        }
        else
        {
            QueryIdentification query(Address(GetComponentID().mSubsystem, 255, 255), GetComponentID());
            query.SetQueryType(QueryIdentification::ComponentIdentification);
            // Local broadcast.
            Send(&query, Service::LocalBroadcast);
        }

        mBroadcastTimeMs = Time::GetUtcTimeMs();

        updateSystemInfo = true;
    }

    // Make sure we have set the authority level and status of
    // our subsystem/component.
    {
        WriteLock myLock(mSubsystemDataMutex);
        Subsystem::Map::iterator me = mSystem.find(GetComponentID().mSubsystem);
        if(me != mSystem.end())
        {
            me->second->GetComponent(GetComponentID())->mAuthorityLevel = GetComponent()->AccessControlService()->GetAuthorityCode();
            me->second->GetComponent(GetComponentID())->mStatus = GetComponent()->ManagementService()->GetStatus();
            me->second->GetComponent(GetComponentID())->mServices = GetTransportService()->GetServices();
            Vehicle* vehicle = dynamic_cast<Vehicle*>(me->second.get());
            if(vehicle != NULL && vehicle->mAuthorityLevel < GetComponent()->AccessControlService()->GetAuthorityCode())
            {
                vehicle->mAuthorityLevel = GetComponent()->AccessControlService()->GetAuthorityCode();
            }
        }
    }
    
    // Only do the following at the same interval
    // we query for new data from system.
    if(updateSystemInfo == false && mTriggerCallbacksFlag == false)
    {
        return;
    }

    Subsystem::Map system;
    Address::List lostComponents;
    

    {
        WriteLock wLock(mSubsystemDataMutex);

        Time::Stamp disconnectTimeMs = this->GetComponent()->TransportService()->GetDisconnectTimeMs();
        // Delete all components and subsystems we no
        // longer have connections to.
        Subsystem::Map::iterator subsystem = mSystem.begin();
        while(subsystem != mSystem.end())
        {
            if(mBroadcastDelayMs*2.0 >= (unsigned int)(Time::GetUtcTimeMs() - subsystem->second->mUpdateTime.ToMs()))
            {
                subsystem++;
                continue;
            }
            Subsystem::Configuration::iterator node = subsystem->second->mConfiguration.begin();
            while(node != subsystem->second->mConfiguration.end() && disconnectTimeMs > 0)
            {
                Subsystem::Component::Set::iterator component = node->second.begin();
                while(component != node->second.end())
                {
                    if(component->mID == GetComponentID() || 
                        (disconnectTimeMs >= (Time::Stamp)(Time::GetUtcTimeMs() - component->mUpdateTime.ToMs())))
                    {
                        component++;
                    }
                    else
                    {
                        lostComponents.push_back(component->mID);
                        node->second.erase(component);
                        component = node->second.begin();
                    }
                }
                if(node->second.size() == 0)
                {
                    subsystem->second->mConfiguration.erase(node);
                    node = subsystem->second->mConfiguration.begin();
                }
                else
                {
                    node++;
                }
            }
            if(subsystem->second->mConfiguration.size() == 0)
            {
                mSystem.erase(subsystem);
                subsystem = mSystem.begin();
            }
            else
            {
                subsystem++;
            }
        }

        // Make a copy of the current state so we can
        // generate non-blocking callbacks on the data.
        Subsystem::CopySubsystemMap(mSystem, system);

        // Remove from subsystem list.
        Address::List::iterator lostComponent;
        for(lostComponent = lostComponents.begin();
            lostComponent != lostComponents.end();
            lostComponent++)
        {
            if(mSubsystemList.find(*lostComponent) != mSubsystemList.end())
            {
                mSubsystemList.erase(mSubsystemList.find(*lostComponent));
            }
        }

    }

    
    {
        // Do standard update on callbacks.
        ReadLock cbLock(mCallbacksMutex);
        Callback::Set::iterator cb;
        for(cb = mCallbacks.begin();
            cb != mCallbacks.end();
            cb++)
        {
            (*cb)->ProcessSystemState(system);
        }
    }
    // Delete copies made.
    Subsystem::DeleteSubsystemMap(system);

    mTriggerCallbacksFlag = false;
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
bool Discovery::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_CONFIGURATION)
    {
        ReportConfiguration report;
        ReportConfiguration::Record record;
        const QueryConfiguration* query = dynamic_cast<const QueryConfiguration*>(info.mpQueryMessage);
        // Get data to send
        {
            ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));

            Subsystem::Map::const_iterator s = mSystem.find(GetComponentID().mSubsystem);
            if(s != mSystem.end())
            {
                Address::List list = s->second->GetAddressList();
                for(Address::List::iterator id = list.begin();
                    id != list.end();
                    id++)
                {
                    // Populate message with data.
                    if(query->GetQueryType() == QueryConfiguration::SubsystemConfiguration)
                    {
                        if(id->mNode == GetComponentID().mNode)
                        {
                            record.mComponent = id->mComponent;
                            record.mInstance = 0;
                            (*report.GetNodes())[GetComponentID().mNode].push_back(record);
                        }
                    }
                    else
                    {
                        record.mComponent = id->mComponent;
                        record.mInstance = 0;
                        (*report.GetNodes())[GetComponentID().mNode].push_back(record);
                    }
                }
            }
            else
            {
                record.mComponent = GetComponentID().mComponent;
                (*report.GetNodes())[GetComponentID().mNode].push_back(record);
            }
        }
        // Send data
        SendEvent(info, &report);
        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_SUBSYSTEM_LIST)
    {
        ReportSubsystemList report;
        {
            ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));
            *report.GetSubsystemList() = Address::ToList(mSubsystemList);
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
bool Discovery::IsEventSupported(const Events::Type type,
                                 const double requestedPeriodicRate,
                                 const Message* queryMessage,
                                 double& confirmedPeriodicRate,
                                 std::string& errorMessage) const
{
    bool result = false;
    switch(queryMessage->GetMessageCode())
    {
    case QUERY_CONFIGURATION:
    case QUERY_SUBSYSTEM_LIST:
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
///   \brief Sets the identification information for the subsystem.  If this
///          data is not set, then Subsystem Discovery won't work correctly.
///
///   \param[in] type Type of subsystem.
///   \param[in] identification Subsystem identification string [255 char max].
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::SetSubsystemIdentification(const Subsystem::Type type,
                                           const std::string& identification)
{
    mSubsystemType = type;
    mSubsystemIdentification = identification;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets how often the service to send a query to identify new 
///          components through the discovery process.
///
///   \param[in] frequencyHz The frequency to broadcast a query message in Hz.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetDiscoveryFrequency(const double frequencyHz)
{
    if(frequencyHz > 0)
    {
        mBroadcastDelayMs = (unsigned int)(1000.0/frequencyHz);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A map list of all subsystems and their identification names.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery::List Discovery::GetSubsystemList() const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));

    Discovery::List list;
    Subsystem::Map::const_iterator subsystem;

    for(subsystem = mSystem.begin();
        subsystem != mSystem.end();
        subsystem++)
    {
        if(mSubsystemsToDiscover.size() == 0 ||
            mSubsystemsToDiscover.find(subsystem->first) != mSubsystemsToDiscover.end())
        {
            list[subsystem->first] = subsystem->second->mIdentification;
        }
    }

    return list;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A map list of all vehicles and their identification names.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery::List Discovery::GetVehicleList() const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));

    Discovery::List list;
    Subsystem::Map::const_iterator subsystem;

    for(subsystem = mSystem.begin();
        subsystem != mSystem.end();
        subsystem++)
    {
        if(subsystem->second->mType == Subsystem::Vehicle)
        {
            if(mSubsystemsToDiscover.size() == 0 ||
                mSubsystemsToDiscover.find(subsystem->first) != mSubsystemsToDiscover.end())
            {
                list[subsystem->first] = subsystem->second->mIdentification;
            }
        }
    }

    return list;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A list of any components discovered with a given service.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List Discovery::GetComponentsWithService(const std::string& serviceName) const
{
    Address::List list;
    Subsystem::Map subsystems;
    this->GetSubsystems(subsystems);
    Subsystem::Map::iterator s;
    for(s = subsystems.begin();
        s != subsystems.end();
        s++)
    {
        Address::List components;
        components = s->second->GetComponentsWithService(serviceName);
        for(unsigned int i = 0; i < (unsigned int)components.size(); i++)
        {
            list.push_back(components[i]);
        }
    }
    Subsystem::DeleteSubsystemMap(subsystems);

    return list;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the subsystem data.
///
///   \param[in] id The subsystem ID number of the subsystem.
///
///   \return Subsystem data if found, otherwise blank subsystem data.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Ptr Discovery::GetSubsystem(const UShort id) const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        return Subsystem::Ptr(subsystem->second->Clone());
    }
    return Subsystem::Ptr(new Subsystem());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the know vehicle data for a subsystem, if it
///          is identified as a vehicle. Vehicle data is similar to
///          a Subsystem, except it contains additional information such as
///          position and orientation which is set by other services.
///
///   \param[in] id The subsystem ID number of the vehicle.
///
///   \return Vehicle information if found, otherwise invalid subsystem ID.
///
////////////////////////////////////////////////////////////////////////////////////
Vehicle::Ptr Discovery::GetVehicle(const UShort id) const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            return Vehicle::Ptr((Vehicle *)vehicle->Clone());
        }
    }
    return Vehicle::Ptr(new Vehicle());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method allows you to store the status of a components state.
///          This just saves the value, it does not put a component in the
///          desired state.
///
///   \param[in] id The component ID.
///   \param[in] componentState Component status (ready, standby, etc.) value
///              to store.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetComponentState(const Address& id,
                                  const Byte componentState)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id.mSubsystem);
    if(subsystem != mSystem.end())
    {
        if(subsystem->second->HaveComponent(id))
        {
            subsystem->second->GetComponent(id)->mStatus = componentState;
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Vehicle structures can store additional information such as position
///          attitude (i.e. orientation), etc.  Use this method to add the data
///          to the Subsystem/Vehicle data stored within Discovery.
///
///   \param[in] id The subsystem ID number of the vehicle.
///   \param[in] wgs Position data.
///   \param[in] time Time of data points.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetVehiclePosition(const UShort id, 
                                   const Wgs& wgs,
                                   const Time& time)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            vehicle->mPosition = wgs;
            vehicle->mUpdateTime = time;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::Position;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::TimeStamp;
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Vehicle structures can store additional information such as position
///          attitude (i.e. orientation), etc.  Use this method to add the data
///          to the Subsystem/Vehicle data stored within Discovery.
///
///   \param[in] id The subsystem ID number of the vehicle.
///   \param[in] attitude Orientation (roll, pitch, yaw).
///   \param[in] time Time of data points.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetVehicleAttitude(const UShort id, 
                                   const Attitude& attitude, 
                                   const Time& time)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            vehicle->mAttitude = attitude;
            vehicle->mUpdateTime = time;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::Attitude;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::TimeStamp;
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Vehicle structures can store additional information such as position
///          attitude (i.e. orientation), etc.  Use this method to add the data
///          to the Subsystem/Vehicle data stored within Discovery.
///
///   \param[in] id The subsystem ID number of the vehicle.
///   \param[in] velocity Linear velocity of vehicle.
///   \param[in] time Time of data points.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetVehicleLinearVelocity(const UShort id, 
                                         const Point3D& velocity, 
                                         const Time& time)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            vehicle->mLinearVelocity = velocity;
            vehicle->mUpdateTime = time;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::LinearVelocity;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::TimeStamp;
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Vehicle structures can store additional information such as position
///          attitude (i.e. orientation), etc.  Use this method to add the data
///          to the Subsystem/Vehicle data stored within Discovery.
///
///   \param[in] id The subsystem ID number of the vehicle.
///   \param[in] velocity Linear velocity of vehicle.
///   \param[in] time Time of data points.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetCustomVehicleData(const UShort id, 
                                     const Vehicle::Info* info)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            vehicle->SetAdditionalInfo(info);
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Vehicle structures can store additional information such as position
///          attitude (i.e. orientation), etc.  Use this method to add the data
///          to the Subsystem/Vehicle data stored within Discovery.
///
///   \param[in] id The subsystem ID number of the vehicle.
///   \param[in] wgs Position data.
///   \param[in] attitude Orientation (roll, pitch, yaw).
///   \param[in] time Time of data points.
///
///   \return True if able to set data, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Discovery::SetVehiclePose(const UShort id, 
                               const Wgs& wgs,
                               const Attitude& attitude, 
                               const Time& time)
{
    WriteLock wLock(mSubsystemDataMutex);
    Subsystem::Map::const_iterator subsystem = mSystem.find(id);
    if(subsystem != mSystem.end())
    {
        Vehicle* vehicle = dynamic_cast<Vehicle *>(subsystem->second.get());
        if(vehicle)
        {
            vehicle->mPosition = wgs;
            vehicle->mAttitude = attitude;
            vehicle->mUpdateTime = time;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::Position;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::Attitude;
            vehicle->mPresenceVector |= Vehicle::PresenceVector::TimeStamp;
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the subsystem data.
///
///   \param[in] subsystems Where to save the system configuration to.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::GetSubsystems(Subsystem::Map& subsystems) const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));

    Subsystem::DeleteSubsystemMap(subsystems);
    
    Subsystem::CopySubsystemMap(mSystem, subsystems);

    // Remove ignored subsystems.
    Subsystem::Map::iterator si = subsystems.begin();
    while(si != subsystems.end() && mSubsystemsToDiscover.size() > 0)
    {
        if(mSubsystemsToDiscover.find(si->first) == mSubsystemsToDiscover.end())
        {
            subsystems.erase(si);
            si = subsystems.begin();
        }
        else
        {
            si++;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the subsystem data.
///
///   \param[in] vehicles Where to save the system configuration to.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::GetVehicles(Vehicle::Map& vehicles) const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));

    Vehicle::DeleteVehicleMap(vehicles);
    Subsystem::Map::const_iterator subsystem;
    for(subsystem = mSystem.begin();
        subsystem != mSystem.end();
        subsystem++)
    {
        if(subsystem->second->mType == Subsystem::Vehicle)
        {
            if(mSubsystemsToDiscover.size() == 0 || 
               mSubsystemsToDiscover.find(subsystem->first) != mSubsystemsToDiscover.end())
            {
                vehicles[subsystem->first].reset((Vehicle *)subsystem->second->Clone());
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to register a callback to receive Discovery Events.
///
///   Discovery events include changes in configuration, identification, etc.
///   to a subsystem.
///
///   \param[in] callback Pointer to callback to add/remove.
///   \param[in] add If true, callback is added, if false, it is removed.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::RegisterCallback(Callback* callback, const bool add)
{
    WriteLock cbLock(mCallbacksMutex);
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
///   \brief Prints system information to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::PrintSystemConfiguration() const
{
    // Print out a copy so we do not block other processes while we
    // print to the console. 

    Subsystem::Map system;

    this->GetSubsystems(system);

    Subsystem::Map::const_iterator subsystem;
    for(subsystem = system.begin();
        subsystem != system.end();
        subsystem++)
    {
        std::cout << "========================================================================\n";
        std::cout << "Subsystem [" << subsystem->first << "] - " << subsystem->second->mIdentification << std::endl;
        Subsystem::Configuration::const_iterator node;
        for(node = subsystem->second->mConfiguration.begin();
            node != subsystem->second->mConfiguration.end();
            node++)
        {
            Subsystem::Component::Set::const_iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                std::cout << "    " << component->mIdentification << " - " << component->mID.ToString() << std::endl;
                std::cout << "    " << component->mIdentification << " - Authority Level - " << component->mAuthorityLevel << std::endl;
                std::cout << "    " << component->mIdentification << " - Status - ";
                switch((Byte)component->mStatus)
                {
                case Management::Status::Ready:
                    std::cout << "Ready\n";
                    break;
                case Management::Status::Standby:
                    std::cout << "Standby\n";
                    break;
                case Management::Status::Shutdown:
                    std::cout << "Shutdown\n";
                    break;
                case Management::Status::Failure:
                    std::cout << "Failure\n";
                    break;
                case Management::Status::Emergency:
                    std::cout << "Emergency\n";
                    break;
                default:
                    std::cout << "Intialized\n";
                    break;
                }
                Service::ID::Set::const_iterator service;
                unsigned int count = 0;
                for(service = component->mServices.begin();
                    service != component->mServices.end();
                    service++)
                {
                    std::cout << "        " << ++count << " - " << service->mName << std::endl;
                }
            }
        }
        std::cout << std::endl;
    }

    Subsystem::DeleteSubsystemMap(system);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints status info about the Service to the console.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::PrintStatus() const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));
    std::cout << "Subsystem Identification: " << mSubsystemIdentification << std::endl;
    std::cout << "There are " << mSystem.size() << " Subsystems on Network.\n";
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Triggers Discovery callbacks.  Only use if you are updating
///          subsystem data externally from Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::TriggerDiscoveryCallbacks()
{
    mTriggerCallbacksFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief By default, all subsystems are discovered.  Use this method to
///   filter out to only a specific set of subsystems you are interested in.
///
///   \param[in] toDiscover Subsystems to discovery.
///
////////////////////////////////////////////////////////////////////////////////////
void Discovery::SetSubsystemsToDiscover(const std::set<UShort>& toDiscover)
{
    WriteLock wLock(mSubsystemDataMutex);
    mSubsystemsToDiscover = toDiscover;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return List of subsystems being discovered (if emtpy, all are being
///           discovered - default).
///
////////////////////////////////////////////////////////////////////////////////////
std::set<UShort> Discovery::GetSubsystemsToDiscover() const
{
    ReadLock rLock(*( (SharedMutex*)&mSubsystemDataMutex));
    return mSubsystemsToDiscover;
}

/*  End of File */
