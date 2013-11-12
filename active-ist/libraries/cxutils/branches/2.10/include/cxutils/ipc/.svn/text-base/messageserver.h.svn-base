////////////////////////////////////////////////////////////////////////////////////
///
///  \file messageserver.h
///  \brief This file contains software for creating shared memory Message Server
///  that will distribute received messages to Message Clients.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 April 2009
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
#ifndef _CXUTILS_IPC_MESSAGE_SERVER__H
#define _CXUTILS_IPC_MESSAGE_SERVER__H

#include "cxutils/ipc/mappedmessagebox.h"
#include "cxutils/thread.h"
#include <set>
#include <map>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class MessageServer
    ///   \brief Class to create a server that can detect clients processes and
    ///   distribute messages to them using mapped/shared memory.
    ///
    ///   The Message Server acts as the single point of entry for message data
    ///   that needs to be distributed.  Message Clients (MessageClient class) can
    ///   register to receive these message as they arrive.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL MessageServer
    {
    public:        
        static const unsigned int DefaultMaxClients = 25;   ///< Default value for max num of clients.        
        typedef MappedMemory::ID ID;                        ///<  ID Field.
        typedef std::set<ID> List;                          ///<  Client/Registry list information.   
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Registry
        ///   \brief Registry to keep track of clients who want to receive data from
        ///          the Message Server.  Message Clients must register themselves
        ///          to receive data from the Message Server.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Registry
        {
        public:
            friend class MessageServer;
            static const unsigned int MaxClients = 25;  ///<  Maximum number of clients (change if necessary).
            static const unsigned int HeaderSize = 20;  ///<  Size of registry header.
            Registry();
            ~Registry();
            int OpenRegistry(const ID registryID); 
            int OpenRegistry(const std::string& registryID);  
            int CreateRegistry(const ID registryID,
                               const unsigned int maxClients = MaxClients);
            int CreateRegistry(const std::string& registryID,
                               const unsigned int maxClients = MaxClients);
            int CloseRegistry();
            int Register(const ID id);
            int Unregister(const ID id);
            int GetRegistry(List& registry);
            bool IsRegistered(const ID id) const;
            bool IsOpen() const { return mRegistry.IsOpen(); }    
            bool IsActive(const unsigned int thresholdMs = 500) const;
        private:  
            static const unsigned int TimePos = 8;
            static const unsigned int CountPos = 16;
            bool Touch(const Time::Stamp timestampMs);
            MappedMemory mRegistry;
            bool mServerFlag;
        };
        MessageServer();
        ~MessageServer();
        int Initialize(const ID serverID, 
                       const unsigned int maxClients = DefaultMaxClients);
        int Initialize(const std::string& serverID, 
                       const unsigned int maxClients = DefaultMaxClients);
        void Shutdown();
        bool CreateConnection(const ID id);
        bool SendToClient(const ID id, const std::string& message);
        bool SendToClient(const ID id, const Packet& message);
        bool SendToAllClients(const std::string& message);
        bool SendToAllClients(const Packet& message);
        bool GetClientList(List& clients) const;
        unsigned int GetNumClients() const;
        bool IsInitialized() const;
        static bool IsServerPresent(const std::string& serverID);
    private:
        typedef std::map<ID, MappedMessageBox*> Clients;
        static void ClientDiscoveryThread(void* messageServer);
        Mutex mClientsMutex;            ///<  Mutex for thread protection of client connections.
        Thread mClientDiscoveryThread;  ///<  Threat that scans registry to create client connections.
        Clients mClients;               ///<  Map of all client connections.
        Registry mClientRegistry;       ///<  Registry for clients to connect to.
    };
};

#endif
/*  End of File */
