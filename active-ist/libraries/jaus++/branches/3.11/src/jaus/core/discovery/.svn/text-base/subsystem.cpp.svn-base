////////////////////////////////////////////////////////////////////////////////////
///
///  \file subsystem.cpp
///  \brief This file contains the Subsystem class used to describe subsystem
///         configurations and supported services.
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
#include "jaus/core/discovery/subsystem.h"
#include "jaus/core/discovery/vehicle.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::Component()
{
    mIdentification = "Component";
    mAuthorityLevel = -1;
    mStatus = -1;
    mUpdateTime.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::Component(const Address& id)
{
    mID = id;
    mAuthorityLevel = -1;
    mStatus = -1;
    mIdentification = "Component";
    mUpdateTime.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::Component(const Address& id, const Service::ID::Set& services)
{
    mID = id;
    mServices = services;
    mIdentification = "Component";
    mAuthorityLevel = -1;
    mStatus = -1;
    mUpdateTime.SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::Component(const Subsystem::Component& component)
{
    *this = component;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::~Component()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component& Subsystem::Component::operator =(const Subsystem::Component& component)
{
    if(this != &component)
    {
        mID = component.mID;
        mServices = component.mServices;
        mIdentification = component.mIdentification;
        mAuthorityLevel = component.mAuthorityLevel;
        mStatus = component.mStatus;
        mUpdateTime = component.mUpdateTime;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Subsystem()
{
    mSubsystemID = 0;
    mType = OtherSubsystem;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Subsystem(const Subsystem& subsystem)
{
    *this = subsystem;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::~Subsystem()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a component and its services to the subsystem.
///
///   \param[in] component Component ID.
///   \param[in] services The Service set for the component.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::AddComponent(const Address& component, const Service::ID::Set& services)
{
    mSubsystemID = component.mSubsystem;
    mConfiguration[component.mNode].insert(Component(component, services));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Removes a component and its services to the subsystem.
///
///   \param[in] component Component ID.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::RemoveComponent(const Address& component)
{
    mSubsystemID = component.mSubsystem;
    Component::Set::iterator c;
    Configuration::iterator node = mConfiguration.find(component.mNode);
    if(node != mConfiguration.end())
    {
        c = node->second.find(Component(component));
        if(c != node->second.end())
        {
            node->second.erase(c);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a subsystem has a Service.
///
///   \param[in] name Name of Service to look for.
///   \param[out] id Optional parameter.  If not NULL, then the ID of
///               the component with the service is returned.
///
///   \return True if Service is supported by subsystem, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Subsystem::HaveService(const std::string& name, Address* id) const
{
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        Component::Set::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            Service::ID::Set::const_iterator service;
            service = component->mServices.find(Service::ID(name));
            if(service != component->mServices.end())
            {
                if(id)
                {
                    *id = component->mID;
                }
                return true;
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a component is part of the subsystem configuration.
///
///   \param[in] id ID of the component to look for.
///
///   \return True if the component is part of the configuration, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Subsystem::HaveComponent(const Address& id) const
{
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        Component::Set::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            if(component->mID == id)
            {
                return true;
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the Component within the configuration.
///
///   If the component doesn't exist, it is created and added to the configuration.
///
///   \param[in] id ID of the component to look for.
///
///   \return Pointer to component data.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component* Subsystem::GetComponent(const Address& id)
{
    if(id.IsValid() == false)
    {
        throw Exception();
        return NULL;
    }
    Configuration::iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        if(node->first == id.mNode)
        {
            Component::Set::iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                if(component->mID == id)
                {
                    return (Subsystem::Component *)&(*component);
                }
            }
            // If the component doesn't exist, create it.
            node->second.insert(Component(id));
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                if(component->mID == id)
                {
                    return (Subsystem::Component *)&(*component);
                }
            }

        }
        break;
    }
    // If we got here, then we don't have any nodes created yet.
    mConfiguration[id.mNode].insert(Component(id));
    return (Subsystem::Component *)&(*mConfiguration[id.mNode].begin());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the Component within the configuration.
///
///   \param[in] id ID of the component to look for.
///
///   \return Pointer to component data, NULL if not part of configuration.
///
////////////////////////////////////////////////////////////////////////////////////
const Subsystem::Component* Subsystem::GetComponent(const Address& id) const
{
    if(id.IsValid() == false)
    {
        throw Exception();
        return NULL;
    }
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        if(node->first == id.mNode)
        {
            Component::Set::const_iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                if(component->mID == id)
                {
                    return &(*component);
                }
            }
        }
        break;
    }
    throw Exception();
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param[in] name Name of Service to look for.
///
///   \return List of component IDs for those supporting a specific Service.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List Subsystem::GetComponentsWithService(const std::string& name) const
{
    Address::List results;
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        Component::Set::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            Service::ID::Set::const_iterator service;
            service = component->mServices.find(Service::ID(name));
            if(service != component->mServices.end())
            {
                results.push_back(component->mID);
            }
        }
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the Subsystem::Configuration of the subsystem.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem::Component::List Subsystem::GetConfiguration() const
{
    Component::List results;
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        Component::Set::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            results.push_back(*component);
        }
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the Subsystem::Configuration of the subsystem.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List Subsystem::GetAddressList() const
{
    Address::List results;
    Configuration::const_iterator node;
    for(node = mConfiguration.begin();
        node != mConfiguration.end();
        node++)
    {
        Component::Set::const_iterator component;
        for(component = node->second.begin();
            component != node->second.end();
            component++)
        {
            results.push_back(component->mID);
        }
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Subsystem& Subsystem::operator =(const Subsystem& subsystem)
{
    if(this != &subsystem)
    {
        mSubsystemID = subsystem.mSubsystemID;
        mConfiguration = subsystem.mConfiguration;
        mType = subsystem.mType;
        mIdentification = subsystem.mIdentification;
        mUpdateTime = subsystem.mUpdateTime;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies data to internal data members.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::CopySubsystemData(const Subsystem* subsystem)
{
    mSubsystemID = subsystem->mSubsystemID;
    mConfiguration = subsystem->mConfiguration;
    mType = subsystem->mType;
    mIdentification = subsystem->mIdentification;
    mUpdateTime = subsystem->mUpdateTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies data to internal data members.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::CopySubsystemData(const Subsystem::Ptr& subsystem)
{
    CopySubsystemData(subsystem.get());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compare current subsystem data to previous data for changes.
///
////////////////////////////////////////////////////////////////////////////////////
bool Subsystem::HasChanged(const Subsystem& previousState) const
{
    if(previousState.mSubsystemID != mSubsystemID ||
        previousState.mIdentification != mIdentification ||
        previousState.mType != mType ||
        previousState.mConfiguration.size() != mConfiguration.size())
    {
        return true;
    }

    Configuration::const_iterator p, c;

    for(p = previousState.mConfiguration.begin();
        p != previousState.mConfiguration.end();
        p++)
    {
        // Check for node
        c = mConfiguration.find(p->first);
        if(c == mConfiguration.end())
        {
            return true;
        }
        if(c->second.size() != p->second.size())
        {
            return true;
        }
        // Check for component servic changes
        Component::Set::const_iterator pc, cc;
        for(pc = p->second.begin();
            pc != p->second.end();
            pc++)
        {
            bool found = false;
            for(cc = c->second.begin();
                cc != c->second.end();
                cc++)
            {
                if(cc->mID == pc->mID)
                {
                    found = true;
                    if(cc->mServices.size() != pc->mServices.size() ||
                       cc->mIdentification != pc->mIdentification)
                    {
                        return true;
                    }
                }
            }
            if(found == false)
            {
                return true;
            }
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all data and clears list.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::DeleteSubsystemMap(Map& list)
{
    list.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies data from src to dest.
///
////////////////////////////////////////////////////////////////////////////////////
void Subsystem::CopySubsystemMap(const Map& src, Map& dest)
{
    Map::const_iterator s;
    DeleteSubsystemMap(dest);
    for(s = src.begin();
        s != src.end();
        s++)
    {
        dest[s->first].reset(s->second->Clone());
    }
}


/* End of File */
