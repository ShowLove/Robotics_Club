////////////////////////////////////////////////////////////////////////////////////
///
///  \file service.cpp
///  \brief This file contains the implementation of the Service class which is
///  used to develop JAUS Services within JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 October 2009
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
#include "jaus/core/service.h"
#include "jaus/core/transport/transport.h"
#include "jaus/core/component.h"
#include "jaus/core/sensor.h"
#include <cstdio>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

using namespace JAUS;

SharedMutex Service::mDebugMessagesMutex;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///  \param[in] name Service name identifier.
///  \param[in] version Service version number.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::ID(const std::string& name, const double version) : mName(name),
                                                                 mVersion(version)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::ID(const ID& id)
{
    *this = id;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::~ID()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Writes the Service UID information to a packet following
///         conventions defined by the Report Services message.
///
///  \param[in] packet Packet to write data to.
///
///  \return Number of bytes written, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Service::ID::Write(Packet& packet) const
{
    Byte majorVersionNumber = (Byte)mVersion;
    Byte minorVersionNumber = (Byte)((mVersion - majorVersionNumber)*10);
    int result = 0;
    result += packet.Write( (Byte)(mName.size()) );
    result += packet.Write(mName);
    result += packet.Write(majorVersionNumber);
    result += packet.Write(minorVersionNumber);

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads the Service UID information from a packet following
///         conventions defined by the Report Services message.
///
///  \param[in] packet Packet to read data from.
///
///  \return Number of bytes read, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Service::ID::Read(const Packet& packet)
{
    Byte strLen = 0;
    Byte majorVersionNumber = 0, minorVersionNumber = 0;
    int result = 0;

    result += packet.Read(strLen);
    if(result > 0 && strLen > 0)
    {
        result += packet.Read(mName, (unsigned int)strLen);
        result += packet.Read(majorVersionNumber);
        result += packet.Read(minorVersionNumber);
        mVersion = majorVersionNumber + minorVersionNumber/10.0;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets values to defaults.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::ID::Clear()
{
    mName.clear();
    mVersion = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return A string with the following formate "name - version=number."
///
////////////////////////////////////////////////////////////////////////////////////
std::string Service::ID::ToString(const bool nameOnly, const bool trim) const
{
    char buffer[512];
    size_t found;
    found = mName.find_last_of(":");
    std::string n = mName.substr(found + 1);
    if(trim &&  n.size() > 0)
    {
        if(nameOnly)
        {
            return n;
        }
        else
        {
            sprintf(buffer, "%s - version=%.2lf", n.c_str(), mVersion);
        }
    }
    else
    {
        if(nameOnly)
        {
            return mName;
        }
        else
        {
            sprintf(buffer, "%s - version=%.2lf", mName.c_str(), mVersion);
        }
    }
    return std::string(buffer);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID& Service::ID::operator =(const Service::ID& id)
{
    if(this != &id)
    {
        mName = id.mName;
        mVersion = id.mVersion;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
///  \param[in] serviceIdentifier The Service ID information.
///  \param[in] parentServiceIdentifier Service ID of the parent Service to this
///                                     Service class. Who this Service inherits
///                                     from.
///
////////////////////////////////////////////////////////////////////////////////////
Service::Service(const ID& serviceIdentifier, const ID& parentServiceIdentifier) : mServiceID(serviceIdentifier),
                                                                                   mParentServiceID(parentServiceIdentifier)
{
    mServiceEnabledFlag = true;
    mpJausParentService = NULL;
    mDebugMessagesFlag = false;
    mpTransportService = dynamic_cast<Transport *>(this);
    mpComponent = NULL;
    mShutdownServiceFlag = false;
    mServiceUpdateThreadDelayMs = 0;
    mSingleThreadModeFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Service::~Service()
{
    mServiceID.Clear();
    mParentServiceID.Clear();
    mJausChildServices.clear();
    mpJausParentService = NULL;
    mShutdownServiceFlag = true;
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
bool Service::Send(const Message* message, const int broadcastFlags) const
{
    if(mpTransportService)
    {
        return mpTransportService->Send(message, broadcastFlags);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message to multiple destinations, reducing the 
///   number of times the data must be re-serialized.
///
///   This method is good for when you needs to send a large chunk of the same
///   data to multiple places (like range data or video).
///
///   \param[in] destinations All destinations the message must go to.
///   \param[in] message JAUS Message to send.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g.
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::SendToList(const Address::Set& destinations,
                         const Message* message, const int broadcastFlags) const
{
    if(mpTransportService)
    {
        return mpTransportService->SendToList(destinations, message, broadcastFlags);
    }
    return false;
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
bool Service::Send(const Message* message,
                   Message* response,
                   const unsigned int waitTimeMs) const
{
    if(mpTransportService)
    {
        return mpTransportService->Send(message, response, waitTimeMs);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message, and blocks until a response is received.  You
///          must pass pointers to the response message object by adding them
///          to the list passed.
///
///   \param[in] message JAUS Message to send.
///   \param[out] possibleResponses Lists of messages that are possible
///                                 responses to the message sent. For example
///                                 Create Event can be responded with
///                                 Confirm or Reject Event Request.
///   \param[in] waitTimeMs How long to wait in ms for a response to be
///                         received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::Send(const Message* message,
                   Message::List& possibleResponses,
                   const unsigned int waitTimeMs) const
{
    if(mpTransportService)
    {
        return mpTransportService->Send(message, possibleResponses, waitTimeMs);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes messages passed-to or received by this Service.
///
///   Whenever a Service receives a message, this method is called.  New
///   Services must overload this method to add processing capabilities.
///
///   If the service does not support the Message, then it must use the
///   PushMessageToChildren method to pass it to inheriting Services that
///   do support it (this is the default behavior of the method).
///
///   \param[in] message JAUS Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::Receive(const Message* message)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the component ID of the Service.
///
///  \param[in] id Component ID the Service belongs to.
///
///  \return True if valid ID, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::SetComponentID(const Address& id)
{
    if(id.IsValid() && !id.IsBroadcast())
    {
        mComponentID = id;
        Service::Map::iterator child;
        Service::Map *children = &mJausChildServices;
        for(child = children->begin();
            child != children->end();
            child++)
        {
            child->second->SetComponentID(id);
        }
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks to see if the Service inherits from the given Service.
///
///  \param[in] id ID of the Service to check for direct inheritance from.
///
///  \return True if the Service inhertis from the ID of the provided
///          Service, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::InheritsFrom(const Service::ID& id) const
{
    if(mParentServiceID.mName == id.mName)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Add a Service this inherits from (i.e. is child of) this Service.
///
///  \param[in] childService The child Service to add.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::AddChildService(Service* childService)
{
    bool result = false;

    if(childService->InheritsFrom(mServiceID) == false)
        return result;

    if(mpTransportService == NULL)
    {
        mpTransportService = dynamic_cast<Transport *>(this);
        Service* parent = mpJausParentService;
        while(mpTransportService == NULL || parent != NULL)
        {
            Transport* transport = dynamic_cast<Transport *>(parent);
            if(transport)
            {
                mpTransportService = transport;
                break;
            }
            parent = parent->mpJausParentService;
        }
    }
    Map::iterator child = mJausChildServices.find(childService->mServiceID.mName);
    if(child == mJausChildServices.end())
    {
        mJausChildServices[childService->mServiceID.mName] = childService;
        if(mpTransportService)
        {
            childService->mpTransportService = mpTransportService;
        }
        childService->mpJausParentService = this;
        childService->SetComponent(mpComponent);
        result = true;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets a pointer to the Service that this Service inherits from.
///
///  \param[in] parentService Parent to this Service.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Service::SetParentService(Service* parentService)
{
    if(InheritsFrom(parentService->mServiceID) == false)
    {
        return false;
    }

    mpJausParentService = parentService;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return A pointer to the parent service.
///
////////////////////////////////////////////////////////////////////////////////////
Service* Service::GetParentService()
{
    return mpJausParentService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return A pointer to the parent service.
///
////////////////////////////////////////////////////////////////////////////////////
const Service* Service::GetParentService() const
{
    return mpJausParentService;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \param[in] id ID of the child Service to get a pointer to.
///
///  \return A pointer to the child Service.
///
////////////////////////////////////////////////////////////////////////////////////
Service* Service::GetChildService(const Service::ID& id)
{
    Map::iterator child;
    child = mJausChildServices.find(id.mName);
    if(child != mJausChildServices.end())
    {
        return child->second;
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \param[in] id ID of the child Service to get a pointer to.
///
///  \return A pointer to the child Service.
///
////////////////////////////////////////////////////////////////////////////////////
const Service* Service::GetChildService(const Service::ID& id) const
{
    Map::const_iterator child;
    child = mJausChildServices.find(id.mName);
    if(child != mJausChildServices.end())
    {
        return child->second;
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets a set of all the Service information for this Service and its
///         child services.  Services that are marked as not discoverable through
///         the IsDiscoverable() method are not put in this set.  Also, if
///         the service is set in a synchronize mode, it is not discoverable
///         either.  This only applies if your class is using the Sensor
///         interface.
///
///  \return List of service information.
///
////////////////////////////////////////////////////////////////////////////////////
Service::ID::Set Service::GetServices() const
{
    Service::ID::Set services;
    Service::ID::Set::iterator s;

    services.insert(this->GetServiceID());

    Map::const_iterator child;
    for(child = mJausChildServices.begin();
        child != mJausChildServices.end();
        child++)
    {
        if(child->second->IsDiscoverable() && child->second->IsEnabled())
        {
            const Sensor* sensor = dynamic_cast<Sensor *>(child->second);
            if(sensor && sensor->IsSynchronizing())
            {
                continue;
            }
            Service::ID::Set childServices = child->second->GetServices();
            for(s = childServices.begin();
                s != childServices.end();
                s++)
            {
                services.insert(*s);
            }
        }
    }
    return services;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Performs a recursive shutdown where all child services are shutdown
///         from the bottom up (lowest leaf) followed by this service.
///
///  This type of shutdown is necessary to make sure that lower level
///  serives shutdown before their parent services do.  Failure to do so may
///  result in disabling a service capability needed by a child service to
///  perform safe cleanup.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::RecursiveShutdown()
{
    Map::iterator child;
    child = mJausChildServices.begin();
    while(child != mJausChildServices.end())
    {
        child->second->RecursiveShutdown();
        child++;
    }
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets a pointer to the component the Service belongs to.
///
///  When called, the component pointer to passed to all child services also.
///
///  \param[in] component Pointer to component Service belongs to.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::SetComponent(Component* component)
{
    if(component == NULL) { return; }
    mpComponent = component;
    if(component->TransportService())
    {
        mpTransportService = component->TransportService();
    }
    Map::iterator child;
    for(child = mJausChildServices.begin();
        child != mJausChildServices.end();
        child++)
    {
        child->second->SetComponent(component);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This is a generic thread function for all services that is used
///         to call the UpdateServiceEvent method automatically for any
///         given service at a specified update interval.
///
///  \param[in] servicePtr Pointer to the service to update.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::ServiceUpdateThreadFunc(void* servicePtr)
{
    Service* service = (Service *)servicePtr;

    while(service &&
          service->mServiceUpdateThread.QuitThreadFlag() == false)
    {
        // Update service event
        if(service->mpComponent)
        {
            service->UpdateServiceEvent();
        }

        // Sleep!
        if(service->mServiceUpdateThreadDelayMs == 0)
        {
            boost::this_thread::sleep(boost::posix_time::microseconds(1000));
        }
        else
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(service->mServiceUpdateThreadDelayMs));
        }
    }
}


/** Starts the service update thread. */
void Service::StartServiceUpdateEventThread()
{
    mServiceUpdateThread.CreateThread(ServiceUpdateThreadFunc, this);
    mServiceUpdateThread.SetThreadName(this->GetComponentID().ToString() + ":" + this->GetServiceID().ToString() );
}


/** Starts the service update thread. */
void Service::StopServiceUpdateEventThread()
{
    mServiceUpdateThread.StopThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If the message is not supported by this Service, use this method
///         to pass it to all children to this Service.
///
///  \param[in] message The message to push to child Services.
///
////////////////////////////////////////////////////////////////////////////////////
void Service::PushMessageToChildren(const Message* message)
{
    Transport* transport = dynamic_cast<Transport *>(this);
    if(transport)
    {
        transport->TriggerMessageCallbacks(message);
        // Do command filtering here.  Access control service determiens if
        // the command should pass through or not.
        if(message->IsCommand() &&
           mpComponent->AccessControlService()->AcceptCommandMessage(message) == false)
        {
            return;
        }
        // Let all services receives message, even if they ignore it.
        Service::Map services = mpComponent->GetServices();
        Service::Map::iterator s;
        for(s = services.begin();
            s != services.end();
            s++)
        {
            if(s->second != transport && s->second->mServiceEnabledFlag)
            {
                s->second->Receive(message);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Recursively gets all child services.
///
///  \return Map of all children to the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Service::Map Service::GetChildServices()
{
    Service::Map children = mJausChildServices;
    Service::Map::iterator child;
    for(child = children.begin();
        child != children.end();
        child++)
    {
        Service::Map children2 = child->second->GetChildServices();
        Service::Map::iterator child2;
        for(child2 = children2.begin();
            child2 != children2.end();
            child2++)
        {
            children[child2->first] = child2->second;
        }
    }
    return children;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Recursively gets all child services.
///
///  \return Map of all children to the Service.
///
////////////////////////////////////////////////////////////////////////////////////
const Service::Map Service::GetChildServices() const
{
    Service::Map children = mJausChildServices;
    Service::Map::const_iterator child;
    for(child = children.begin();
        child != children.end();
        child++)
    {
        Service::Map children2 = child->second->GetChildServices();
        Service::Map::iterator child2;
        for(child2 = children2.begin();
            child2 != children2.end();
            child2++)
        {
            children[child2->first] = child2->second;
        }
    }
    return children;
}

/*  End of File */
