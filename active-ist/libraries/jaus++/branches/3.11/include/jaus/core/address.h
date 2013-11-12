////////////////////////////////////////////////////////////////////////////////////
///
///  \file address.h
///  \brief This file contains a data structure for storing JAUS component
///  ID information.  Each component in JAUS has an ID which is used for
///  addressing (hence Address).
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 26 September 2009
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
#ifndef __JAUS_CORE_ADDRESS_H
#define __JAUS_CORE_ADDRESS_H

#include "jaus/core/types.h"
#include <string>
#include <set>
#include <vector>
#include <iostream>
#include <sstream>
#include <cstdio>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    /// 
    ///  \class Address
    ///  \brief An address is a JAUS Identiifer to represent a communication endpoint.
    ///
    ///  The Address class (typedef ID) is used as an Identifier for a component
    ///  so that it can receive and send messages.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Address
    {
    public:     
        typedef std::set<Address> Set;                  ///<  STL set of Address structures.
        typedef std::vector<Address> List;              ///<  STL vector of Address structures.
        static const UShort GlobalBroadcast = 0xFFFF;   ///< Global Broadcast ID.
        static const Byte LocalBroadcast = 0xFF;        ///< Local Broadcast ID.
        Address(const UShort subsystem = 0, 
                const Byte node = 0, 
                const Byte component = 0) : mSubsystem(subsystem),
                                            mNode(node),
                                            mComponent(component)
        {
        }
        Address(const Address &id) : mSubsystem(id.mSubsystem),
                                     mNode(id.mNode),
                                     mComponent(id.mComponent)
        {

        }
        Address(const UInt id)
        {
            *this = id;
        }        
        ~Address() {}
        inline void Clear() 
        {
            mSubsystem =  mNode = mComponent = 0;
        }
        inline void Print() const
        {
            std::cout << ToString() << std::endl;
        }
        std::string ToString() const
        {
            std::stringstream str;
            str << mSubsystem << "." << (int)mNode << "." << (int)mComponent;
            return str.str();
        }
        static Address FromString(const std::string& str)
        {
            int s, n, c;
            s = n = c = 0;
            sscanf(str.c_str(), "%d.%d.%d", &s, &n, &c);
            return Address((Byte)s, (Byte)n, (Byte)c);
        }
        inline operator UInt() const 
        {                                           //  Combines individual
            return (UInt)( ( mSubsystem << 16 ) |   //  byte values into a 
                           ( mNode << 8 ) |         //  single 4 byte number.
                           ( mComponent ) );
        }        
        inline bool IsValid() const 
        {                        
            if ( mSubsystem == 0 ||        //  If any value is 0, then
                 mNode == 0 ||             //  the address is invalid. Made 
                 mComponent == 0 )         //  inline because of frequent use.
            {
                return false;
            }
            return true;
        }

        inline bool IsBroadcast() const 
        {
            if( mSubsystem == GlobalBroadcast ||  //  This function checks to see if
                mNode == LocalBroadcast ||        //  any of the ID values is a 
                mComponent == LocalBroadcast )    //  broadcast.  This function is
            {                                     //  made inline since it is called
                    return true;                  //  frequently.
            }
            return false;
        }
        inline UInt ToUInt() const 
        {                                           //  Combines individual
            return (UInt)( ( mSubsystem << 16 ) |   //  byte values into a 
                           ( mNode << 8 ) |         //  single 4 byte number.
                           ( mComponent ) );
        } 
        //  Check if destination ID matches the second ID.
        static inline bool DestinationMatch(const Address& dest, const Address id)
        {
            if( (dest.mSubsystem == GlobalBroadcast || dest.mSubsystem == id.mSubsystem) &&
                (dest.mNode == LocalBroadcast || dest.mNode == id.mNode) &&
                (dest.mComponent == LocalBroadcast || dest.mComponent == id.mComponent)) 
            {
                    return true;
            }

            return false;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        /// 
        ///  \brief Compares two sets of Addresses to see if there are differences.
        ///
        ///   This method is used to compare a previous list of Address with a new one
        ///   to find lost or new values in the current set versus the previous.
        ///
        ///
        ///   \param prev Previous set of addresses.
        ///   \param current The latest set of addresses.
        ///   \param newData Data that is in the current set that was not in the previous.
        ///   \param lostData Data that was in the previous set, but is not in the current.
        ///   \param clear If true, newData and lostData are cleared before a comparison
        ///                is made (default).
        ///
        ///   \return True if there is a diffance, otherwise false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline static bool SetCompare(const Set& prev, 
                                      const Set& current, 
                                      Set& newData, 
                                      Set& lostData, 
                                      const bool clear = true)
        {
            bool result = false;
            if(clear)
            {
                lostData.clear();
                newData.clear();
            }
            Address::Set::const_iterator id1, id2;
            for(id1 = current.begin(); 
                id1 != current.end(); 
                id1++)
            {
                id2 = prev.find(*id1);
                if(id2 == prev.end())
                {
                    // New ID in current, not in previous.
                    newData.insert(*id1);
                    result = true;
                }
            }
            for(id1 = prev.begin();
                id1 != prev.end();
                id1++)
            {
                id2 = current.find(*id1);
                if(id2 == current.end())
                {
                    // Data from previous not in current.
                    lostData.insert(*id1);
                    result = true;
                }
            }
            return result;
        }
        inline static Address::List ToList(const Address::Set& s)
        {
            Address::List list;
            Address::Set::const_iterator i;
            for(i = s.begin();
                i != s.end();
                i++)
            {
                list.push_back(*i);
            }
            return list;
        }
        inline static Address::Set ToSet(const Address::List& list)
        {
            Address::Set s;
            Address::List::const_iterator i;
            for(i = list.begin();
                i != list.end();
                i++)
            {
                s.insert(*i);
            }
            return s;
        }
        inline bool operator==(const Address &id) const
        {
            if( mSubsystem == id.mSubsystem &&
                mNode == id.mNode &&
                mComponent == id.mComponent)
            {
                return true;
            }
            return false;
        }
        inline bool operator==(const UInt id) const 
        { 
            return id == this->ToUInt(); 
        }
        inline bool operator!=(const Address &id) const
        {
            if( mSubsystem != id.mSubsystem ||
                mNode != id.mNode ||
                mComponent != id.mComponent )
            {
                return true;
            }
            return false;
        }
        inline bool operator!=(const UInt &id) const
        {
            return ToUInt() != id;
        }
        inline bool operator<(const Address &id) const
        {
            return ToUInt() < id.ToUInt();
        }
        inline bool operator<=(const Address &id) const
        {
            return ToUInt() <= id.ToUInt();
        }
        inline bool operator>(const Address &id) const
        {
            return ToUInt() > id.ToUInt();
        }
        inline bool operator>=(const Address &id) const
        {
            return ToUInt() >= id.ToUInt();
        }
        Address &operator()(const UShort subsystem, const Byte node, const Byte component) 
        {
            mSubsystem = subsystem;
            mNode = node;
            mComponent = component;
            return *this;
        }
        inline Address &operator=(const Address &id)
        {
            mSubsystem = id.mSubsystem;
            mNode = id.mNode;
            mComponent = id.mComponent;
            return *this;
        }
        inline Address &operator=(const UInt id) 
        { 
            mSubsystem = (UShort)((id & 0xFFFF0000) >> 16);
            mNode =      (Byte)((id & 0x00F00FF00) >> 8);
            mComponent = (Byte)((id & 0x000000FF));
            return *this;
        }
        static inline bool IsReservedComponentID(const Byte value)
        {
            // The following values are reserved component
            // IDs from JAUS RA 3.3
            if(value >= 32 && value <= 35 ||
               value >= 37 && value <= 38 ||
               value >= 40 && value <= 59)
            {
                return true;
            }
            return false;
        }
        UShort mSubsystem;  ///<  Subsystem ID.
        Byte mNode;         ///<  Node ID.
        Byte mComponent;    ///<  Component ID.
    };

} //  End of JAUS namespace

#endif

/* End of file */
