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
#include "jaus/core/component.h"
#include "jaus/core/sensor.h"
#include <cxutils/networking/socket.h>
#include <tinyxml/tinyxml.h>
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <time.h>

#ifndef WIN32

#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

void JAUSUnixSignalHandler(int signum)
{
    if(signum == SIGHUP)
    {
        fprintf(stderr, "Caught signal HUP\n");
        fprintf(stderr, "Now ignoring SIGHUP\n");
        /* SIG_IGN is used to ignore signals.  SIGKILL and SIGSTOP can
         not be ignored. */
        signal(SIGHUP, SIG_IGN);
    }
}

#endif

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes the default services supported by the
///   Component.
///
////////////////////////////////////////////////////////////////////////////////////
Component::Component()
{
#ifndef WIN32
    // Register the signals
    signal(SIGHUP, SIG_IGN);
#endif
    mInitializedFlag = false;
    mCheckServiceTimer.RegisterTimerEvent(Component::CheckServiceStatusEvent, this);
    mCheckCoreServicesTimer.RegisterTimerEvent(Component::CheckCoreServicesStatusEvent, this);
    mCoreServicesCheckTimeMs = 0;
    mServices[Transport::Name] = mpTransportService = new Transport();

    mpEventsService = new Events();
    mpLivenessService = new Liveness();
    mpDiscoveryService = new Discovery();
    mpAccessControlService = new AccessControl();
    mpManagementService = new Management();
    mpTimeService = new TimeService();

    AddService(mpEventsService);
    AddService(mpLivenessService);
    AddService(mpDiscoveryService);
    AddService(mpAccessControlService);
    AddService(mpManagementService);
    AddService(mpTimeService);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Component::~Component()
{
    Shutdown();
    mCheckCoreServicesTimer.Stop();
    if(mCheckServiceTimer.IsActive())
    {
        // Stop checking services.
        mCheckServiceTimer.Stop();
        // Shutdown the Transport Service.
        mpTransportService->RecursiveShutdown();
        mpTransportService->Shutdown();
    }
    Service::Map::iterator s;

    s = mServices.find(Transport::Name);
    if(s != mServices.end())
    {
        delete s->second;
        mServices.erase(s);
    }

    mpTransportService = NULL;

    for(s = mServices.begin();
        s != mServices.end();
        s++)
    {
        delete s->second;
    }
    mServices.clear();
    mServiceCheckTimeMs = 0;
    mInitializedFlag = false;
    mComponentID(0, 0, 0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Component and all its' Services with a given
///          Component ID.
///
///   Once initialized, all components can create a periodic event which will call
///   the CheckServiceStatus method of each Service.  This is designed in the
///   Component interface so developers do not need to create their own threads
///   within there Services for performing status checks at low frequency
///   intervals.  Status checks include verifying event subscriptions or control
///   status.
///
///   \param[in] id The Component ID to use.
///   \param[in] serviceUpdateFrequency This is the frequency (Hz) to call the
///              CheckServiceStatus method on each Service.  This is useful if any
///              Service needs to verify connections or check what state it is in
///              periodically. This value will depend on the types of Service your
///              Component includes and defaults to 10Hz.  If value is less than 0,
///              no periodic checking happens.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::Initialize(const Address& id, const double serviceUpdateFrequency)
{
    bool result = false;

    Shutdown();
    // Check to see if the ID is locked on the local host.
    CxUtils::MappedMemory mappedMem;
    std::stringstream memName;
    memName << "JAUS" << (int)id.mSubsystem << (int)id.mNode << (int)id.mComponent;
    if(mLockID.CreateMappedMemory(memName.str(), 128) == 0)
    {
        // ID is in use, exit.
        //throw Exception("Subsystem Number in Use");
        return false;
    }

    mComponentID = id;
    if(DiscoveryService()->IsEnabled() && DiscoveryService()->GetSubsystemIdentification().empty())
    {
        std::cout << "Component::ERROR - Subsystem Identification Not Set.\n";
        return false;
    }
    if(mpTransportService->Initialize(id))
    {
        result = true;
        if(serviceUpdateFrequency >= 1.0)
        {
            std::stringstream str;
            str << mComponentID.ToString() << ":Check Services";
            mCheckServiceTimer.SetName(str.str());
            mCheckServiceTimer.Start(serviceUpdateFrequency);
        }

        std::stringstream str;
        str << mComponentID.ToString() << ":Check Core Services";
        mCheckCoreServicesTimer.SetName(str.str());
        mCheckCoreServicesTimer.Start(10);
        mInitializedFlag = result = true;
        Service::Map::iterator s;
        for(s = mServices.begin();
            s != mServices.end();
            s++)
        {
            if(s->second != mpTransportService)
            {
                s->second->Initialize();
            }
        }
        ManagementService()->SetStatus(Management::Status::Standby);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Component and all its' Services by trying to discover
///          an ID that is not currently in use on the network or host machine.
///
///   This method uses the last octect of the IP address of the host machine
///   as a method for ID generation.  For example:  If the IP address of the
///   host PC is 10.171.191.55, then the subsystem number chosen will be
///   55 + 10,000 = 10,055.  If this ID is reserved on the host machine
///   already, then the component ID of the JAUS ID will be incremented
///   until an available ID is found.
///
///   \param[in] waitTimeMs How long to listen for existing components. A Typical
///                         value is 2000 ms.
///   \param[in] serviceUpdateFrequency This is the frequency (Hz) to call the
///              CheckServiceStatus method on each Service.  This is useful if any
///              Service needs to verify connections or check what state it is in
///              periodically. This value will depend on the types of Service your
///              Component includes and defaults to 10Hz.  If value is less than 0,
///              no periodic checking happens.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::InitializeWithUniqueID(const unsigned int waitTimeMs,
                                       const double serviceUpdateFrequency)
{
    CxUtils::IP4Address::List available;
    if(CxUtils::Socket::GetHostAddresses(available) && available.size() > 0)
    {
        for(unsigned int i = 0; i < (unsigned int)available.size(); i++)
        {
            if(available[i] != "127.0.0.1")
            {
                JAUS::Address unique((JAUS::UShort)(available[i].mData[3] + 10000), 1, 1);
                if(Initialize(unique, serviceUpdateFrequency))
                {
                    return true;
                }
                for(int c = 254; c > 0; c--)
                {
                    for(int n = 254; n > 0; n--)
                    {
                        if(Address::IsReservedComponentID((JAUS::Byte)c) == false)
                        {
                            unique.mNode = (JAUS::Byte)n;
                            unique.mComponent = (JAUS::Byte)c;
                            if(Initialize(unique, serviceUpdateFrequency))
                            {
                                return true;
                            }
                        }
                    }
                }
            }
        }

    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method calls the LoadSettings method of each service with the
///          given filename containing XML data.
///
///   Make sure you have added all Services to the component before calling
///   this method.
///
///   \param[in] filename Name of XML file to load settings from.
///
///   \return True if all Services loaded successfully, otherwise false on error.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::LoadSettings(const std::string& filename)
{
    if(mInitializedFlag == false)
    {
        TiXmlDocument xml;
        Service::Map::iterator s;

        if(xml.LoadFile(filename.c_str()) == false)
        {
            return false;
        }
        TiXmlHandle doc(&xml);

        this->mpTransportService->LoadSettings(filename);

        TiXmlElement* service = doc.FirstChild("JAUS").ToElement();
        if(service && service->FirstChildElement())
        {
            service = service->FirstChildElement();
        }
        while(service)
        {
            std::string name = service->Value();
            for(s = mServices.begin(); s != mServices.end(); s++)
            {
                if(strstr(s->first.c_str(), name.c_str()) != NULL)
                {
                    if(s->first != Transport::Name &&
                       service->Attribute("on") &&
                       atoi(service->Attribute("on")) == 0)
                    {
                        s->second->EnableService(false);
                    }
                    else if(s->second->LoadSettings(filename) == false)
                    {
                        return false;
                    }

                }
            }
            service = service->NextSiblingElement();
        }

        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Turns off all services and ends initialization.  This method does
///          not delete any memory or services.
///
////////////////////////////////////////////////////////////////////////////////////
void Component::Shutdown()
{
    if(IsInitialized())
    {
        mLockID.CloseMappedMemory();
        // Stop timers.
        mCheckCoreServicesTimer.Stop();
        mCheckServiceTimer.Stop();

        mInitializedFlag = false; // Stops checking services.

        // Tell services to shutdown.
        Service::Map::iterator s;
        for(s = mServices.begin();
            s != mServices.end();
            s++)
        {
            s->second->SignalServiceToShutdown();
        }

        AccessControlService()->ReleaseComponentControl(Address());
        EventsService()->CancelSubscription(Address(), 0);
        // Actually shutdown the services (from Transport down).
        mpTransportService->RecursiveShutdown();
        mServiceCheckTimeMs = 0;
        mComponentID(0, 0, 0);
        CxUtils::SleepMs(250);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Events service.
///
////////////////////////////////////////////////////////////////////////////////////
Events* Component::EventsService()
{
    return mpEventsService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Liveness service.
///
////////////////////////////////////////////////////////////////////////////////////
Liveness* Component::LivenessService()
{
    return mpLivenessService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery* Component::DiscoveryService()
{
    return mpDiscoveryService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
AccessControl* Component::AccessControlService()
{
    return mpAccessControlService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
Management* Component::ManagementService()
{
    return mpManagementService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
TimeService* Component::GetTimeService()
{
    return mpTimeService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Events service.
///
////////////////////////////////////////////////////////////////////////////////////
const Events* Component::EventsService() const
{
    return mpEventsService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Liveness service.
///
////////////////////////////////////////////////////////////////////////////////////
const Liveness* Component::LivenessService() const
{
    return mpLivenessService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
const Discovery* Component::DiscoveryService() const
{
    return mpDiscoveryService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
const AccessControl* Component::AccessControlService() const
{
    return mpAccessControlService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
const Management* Component::ManagementService() const
{
    return mpManagementService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to the Discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
const TimeService* Component::GetTimeService() const
{
    return mpTimeService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message.
///
///   \param[in] message JAUS Message to send.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g.
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::Send(const Message* message, const int broadcastFlags) const
{
    return mpTransportService->Send(message, broadcastFlags);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message, and blocks until a response is received.
///
///   \param[in] message JAUS Message to send.
///   \param[out] response Pointer to message that is the response to the
///                        message sent which will read received data.
///   \param[in] waitTimeMs How long to wait in ms for a response to be
///                         received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::Send(const Message* message,
                     Message* response,
                     const unsigned int waitTimeMs) const
{
    return mpTransportService->Send(message, response, waitTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to enable/disable a service.  Disabling a parent service
///   disables all Services which inherit from it.
///
///   \param[in] name Service to enable/disable.
///   \param[in] enable If true, service is enabled, false to disable.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::EnableService(const std::string& name, const bool enable)
{
    Service::Map::iterator s;
    s = mServices.find(name);
    if(s != mServices.end())
    {
        s->second->EnableService(enable);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a new Service to the Component.  This method can only be called
///          before the Initialize method is called.
///
///   \param[in] service The Service to add to the component.  This will be
///                      deleted when the Component is deleted or goes out of scope.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Component::AddService(Service* service)
{
    bool result = false;

    if(mInitializedFlag == false)
    {
        if(mpTransportService && service->GetServiceID().mName == Transport::Name)
        {
            std::cout << "Component::ERROR - Cannot replace Transport service directly.\n";
            return false;
        }
        Service::Map::iterator s;
        s = mServices.find(service->GetServiceID().mName);
        if(s == mServices.end())
        {
            mServices[service->GetServiceID().mName] = service;
        }
        else
        {
            Transport* t = dynamic_cast<Transport*>(service);
            Transport* t2 = dynamic_cast<Transport*>(s->second);
            if(t && t2)
            {
                // Copy callbacks, etc.
                t->CopyRegisteredItems(t2);
            }
            delete s->second;
            s->second = service;
        }
        // Now attach services that inherit from each other.
        for(s = mServices.begin();
            s != mServices.end();
            s++)
        {
            if(service->InheritsFrom(s->second->GetServiceID()))
            {
                service->SetParentService(s->second);
                s->second->AddChildService(service);
            }
            if(s->second->InheritsFrom(service->GetServiceID()))
            {
                s->second->SetParentService(service);
                service->AddChildService(s->second);
            }
        }

        // Make sure all services have a pointer to the Component and Transport
        if(mpTransportService)
        {
            mpTransportService->SetComponent(this);
        }

        result = true;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to Service, NULL if Service not part of Component.
///
////////////////////////////////////////////////////////////////////////////////////
Service* Component::GetService(const std::string& name)
{
    Service::Map::iterator s;
    s = mServices.find(name);
    if(s != mServices.end())
    {
        return s->second;
    }
    // Try find the closest match.
    for(s = mServices.begin(); s != mServices.end(); s++)
    {
        if(strstr(s->first.c_str(), name.c_str()) != NULL)
        {
            return s->second;
        }
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Pointer to Service, NULL if Service not part of Component.
///
////////////////////////////////////////////////////////////////////////////////////
const Service* Component::GetService(const std::string& name) const
{
    Service::Map::const_iterator s;
    s = mServices.find(name);
    if(s != mServices.end())
    {
        return s->second;
    }
    // Try find the closest match.
    for(s = mServices.begin(); s != mServices.end(); s++)
    {
        if(strstr(s->first.c_str(), name.c_str()) != NULL)
        {
            return s->second;
        }
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return List of all Enabled services on Component.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::List Component::GetServiceList() const
{
    Service::ID::List result;
    Service::Map::const_iterator s;
    for(s = mServices.begin();
        s != mServices.end();
        s++)
    {
        if(s->second->IsEnabled())
        {
            result.push_back(s->second->GetServiceID());
        }
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Set of all Enabled services on Component.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::Set Component::GetServiceSet() const
{
    Service::ID::Set result;
    Service::Map::const_iterator s;
    for(s = mServices.begin();
        s != mServices.end();
        s++)
    {
        if(s->second->IsEnabled())
        {
            result.insert(s->second->GetServiceID());
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called by the mCheckServiceTimer.  It calls the
///          CheckServiceStatus method of each Service so that they can do
///          updates regularly if needed.
///
////////////////////////////////////////////////////////////////////////////////////
void Component::CheckServiceStatusEvent(void* args)
{
    Component* component = (Component *)args;
    if(component->mInitializedFlag)
    {
        Service::Map::iterator s;
        for(s = component->mServices.begin();
            s != component->mServices.end() && component->mInitializedFlag;
            s++)
        {
            if(s->second != component->mpEventsService &&
               s->second != component->mpLivenessService &&
               s->second != component->mpDiscoveryService &&
               s->second != component->mpAccessControlService &&
               s->second != component->mpManagementService &&
               s->second != component->mpTimeService &&
               s->second != component->mpTransportService)
            {
                s->second->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mServiceCheckTimeMs));
                Sensor* sensor = dynamic_cast<Sensor *>(s->second);
                if(sensor)
                {
                    sensor->CheckServiceSynchronization((unsigned int)(Time::GetUtcTimeMs() - component->mServiceCheckTimeMs));
                }
            }
        }
        component->mServiceCheckTimeMs = Time::GetUtcTimeMs();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called by the mCheckServiceTimer.  It calls the
///          CheckServiceStatus method of each Service so that they can do
///          updates regularly if needed.
///
////////////////////////////////////////////////////////////////////////////////////
void Component::CheckCoreServicesStatusEvent(void* args)
{
    Component* component = (Component *)args;
    if(component->mInitializedFlag)
    {
        if(component->mpTransportService->IsEnabled())
            component->mpTransportService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpEventsService->IsEnabled())
            component->mpEventsService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpLivenessService->IsEnabled())
            component->mpLivenessService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpDiscoveryService->IsEnabled())
            component->mpDiscoveryService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpAccessControlService->IsEnabled())
            component->mpAccessControlService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpManagementService->IsEnabled())
            component->mpManagementService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        if(component->mpTimeService->IsEnabled())
            component->mpTimeService->CheckServiceStatus((unsigned int)(Time::GetUtcTimeMs() - component->mCoreServicesCheckTimeMs));

        component->mCoreServicesCheckTimeMs = Time::GetUtcTimeMs();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints component status information for services to console.
///
////////////////////////////////////////////////////////////////////////////////////
void Component::PrintStatus() const
{
    Service::Map::const_iterator service;

    std::cout << "========================================================================\n";
    std::cout << "Component Status [" << GetComponentID().ToString() << "]" << std::endl;
    for(service = mServices.begin();
        service != mServices.end();
        service++)
    {
        std::cout << service->second->GetServiceID().ToString() << std::endl;
        service->second->PrintStatus();
    }
}


/*  End of File */
