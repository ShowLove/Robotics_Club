////////////////////////////////////////////////////////////////////////////////////
///
///  \file subsystem.h
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
#ifndef __JAUS_CORE_DISCOVERY_SUBSYSTEM__H
#define __JAUS_CORE_DISCOVERY_SUBSYSTEM__H

#include "jaus/core/service.h"


namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Subsystem
    ///   \brief Describes a Subsystem and it's configuration and identification.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Subsystem
    {
    public:        
        // The values for the particular unmanned vehicle (subsystem) type, node type or
        // component type.
        enum Type
        {
            Vehicle = 10001,
            OCU = 20001,
            OtherSubsystem = 30001
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Component
        ///   \brief Describes a Component and its' supported Services.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Component
        {
        public:
            typedef std::vector<Component> List;
            typedef std::set<Component> Set;
            Component();
            Component(const Address& id);
            Component(const Address& id, const Service::ID::Set& services);
            Component(const Component& component);
            ~Component();
            Component& operator=(const Component& component);
            bool operator <(const Component& component) const { return mID < component.mID; }
            Address mID;                    ///< Component ID.
            int mAuthorityLevel;            ///< Authority level of the component (-1) if not set.
            int mStatus;                    ///< Status of the component (init, standby, etc.) -1 if not set.
            std::string mIdentification;    ///< Identification string for component.
            Service::ID::Set mServices;     ///< Component services.
            Time mUpdateTime;               ///< Last time we received heartbeat/data from the component.
        };
        typedef boost::shared_ptr<JAUS::Subsystem> Ptr;
        typedef std::map<Byte, Component::Set> Configuration;
        typedef std::map<UShort, boost::shared_ptr<JAUS::Subsystem> > Map;
        Subsystem();
        Subsystem(const Subsystem& configuration);
        ~Subsystem();
        inline bool IsValid() const { return mSubsystemID != 0; }
        inline bool IsVehicle() const { return (mType >= Vehicle && mType < OCU); }
        void AddComponent(const Address& component, const Service::ID::Set& services);
        void RemoveComponent(const Address& component);
        bool HaveService(const std::string& name, Address* id = NULL) const;
        bool HaveComponent(const Address& id) const;
        Component* GetComponent(const Address& id);
        const Component* GetComponent(const Address& id) const;
        Address::List GetComponentsWithService(const std::string& name) const;
        Component::List GetConfiguration() const;
        Address::List GetAddressList() const;
        virtual Subsystem* Clone() const { return new Subsystem(*this) ; }
        Subsystem& operator=(const Subsystem& configuration);
        void CopySubsystemData(const Subsystem* subsystem);
        void CopySubsystemData(const Subsystem::Ptr& subsystem);
        virtual bool HasChanged(const Subsystem& previousState) const;
        static void DeleteSubsystemMap(Map& list);
        static void CopySubsystemMap(const Map& src, Map& dest);
        UShort mSubsystemID;            ///<  Subsystem ID that configuration is for.
        Configuration mConfiguration;   ///<  Node configurations for this subsystem.
        Type mType;                     ///<  Type of subsystem.
        Time mUpdateTime;               ///<  The last time the contents of the Subsystem were updated.
        std::string mIdentification;    ///<  Identification name of Subsystem [up to 255 characters].
    };
}

#endif
/*  End of File */
