////////////////////////////////////////////////////////////////////////////////////
///
///  \file nodemanager.h
///  \brief Managed all connections running on the host machine and given process.
///
///  <br>Author(s): Daniel Barber
///  <br>Copyright (c) 2011
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
#ifndef __JAUS_CORE_TRANSPORT_NODE_MANAGER__H
#define __JAUS_CORE_TRANSPORT_NODE_MANAGER__H

#include "jaus/core/transport/connection.h"


namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class NodeManager
    ///   \brief Manages all communication connections between components and
    ///          other computers.  It enables all forms of JAUS transport mechanisms
    ///          from JTCP, JUDP, etc.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL NodeManager : public Connection::Callback
    {
        friend class TCP;
    public:
        /** Data structure for NodeManager settings. */
        class JAUS_CORE_DLL Parameters
        {
            friend class NodeManager;
        public:
            Parameters();
            ~Parameters() {}
            /** Load settings from file. */
            bool LoadSettings(const std::string& settingsFileXML);
            /** Method to add a custom connection to a component over a specific medium. */
            void AddCustomConnection(const Address& componentID,
                                     const Connection::Info& connectionDetails);
            /** Enable/Disable operation in single thread mode. */
            void EnableSingleThreadMode(const bool enable = false) { mSingleThreadModeFlag = enable; }
            /** Set default transport method for IP connections (TCP or UDP). UDP is default. */
            void SetTcpAsDefault(const bool enable = false) { mIsTcpDefaultFlag = enable; }
            /** Sets the default port the node should use for UDP/TCP communications. */
            void SetDefaultPortNumber(const unsigned short port) { mDefaultPortNumber = port; }
            /** Sets how long to wait before dropping an "inactive" connection. */
            void SetDropConnectionTimeMs(const unsigned int dropTimeMs) { mDisconnectTimeMs = dropTimeMs; }
            /** Sets how many connections to maintained by a single thread (if multi-threaded) */
            void SetConnectionsPerThread(const unsigned int number = 1) { mConnectionsPerThread = number; }
            /** Sets the multicast options. */
            void SetNetworkInterface(const IP4Address& ip) { mNetworkInterface = ip; }
            /** Sets the multicast options. */
            void SetMulticast(const IP4Address& ip, const unsigned char ttl = 255) { mMulticastIP = ip; mTimeToLive = ttl; }
            /** Gets map of custom/user defined connections. */
            std::map<Address, Connection::Info> GetCustomConnections() const { return mCustomConnections; }
            /** Returns true if single thread mode enabled. */
            bool IsSingleThreaded() const { return mSingleThreadModeFlag; }
            /** Returns true if TCP is default transport over network. */
            bool IsTcpDefault() const { return mIsTcpDefaultFlag; }
            /** Gets the default port number for IP based connections. */
            unsigned short GetDefaultPortNumber() const { return mDefaultPortNumber; }
            /** Gets the time before dropping "inactive" connections. */
            unsigned int GetDropConnectionTimeMs() const { return mDisconnectTimeMs; }
            /** Gets the number of connections updated via threads. 1 is default. */
            unsigned int GetConnectionsPerThread() const { return mConnectionsPerThread; }
            /** Gets the multicast IP to use */
            IP4Address GetNetworkInterface() const { return mNetworkInterface; }
            /** Gets the multicast IP to use */
            IP4Address GetMulticastIP() const { return mMulticastIP; }
            /** Gets the multicast TTL. */
            unsigned char GetMulticastTLL() const { return mTimeToLive; }
        protected:
            bool mSingleThreadModeFlag;         ///<  If true, operate in single thread mode (default is false).
            bool mIsTcpDefaultFlag;             ///<  Is TCP the default network connection type? (false = default)
            unsigned short mDefaultPortNumber;  ///<  Default port number.
            unsigned int mDisconnectTimeMs;     ///<  How long to wait before closing a connection for lack of update (ms).
            unsigned int mConnectionsPerThread; ///<  How many connections max can be updated per thread (if multi-threaded on).
            std::map<Address, Connection::Info> mCustomConnections; ///<  Connections to JAUS ID's specified by user.
            IP4Address mMulticastIP;            ///<  Multicast group.
            IP4Address mNetworkInterface;       ///<  Network interface to use.
            unsigned char mTimeToLive;          ///<  Time to Live TTL for UDP.
        };
        NodeManager(const bool singleThreadMode = false);
        virtual ~NodeManager();
        virtual bool LoadSettings(const std::string& settingsFileXML);
        virtual bool Initialize();
        virtual bool IsInitialized() const;
        virtual void Shutdown();
        virtual void UpdateServiceEvent();
        virtual bool CreateConnection(const Address& id,
                                      const Connection::Info& info,
                                      const bool permanent = true);
        virtual bool GetStatistics(Connection::Statistics::List& local,
                                   Connection::Statistics::List& remote);
        bool SetConnectionsPerThread(const unsigned int limit = 5);
        NodeManager::Parameters* GetSettings() { return &mSettings; }
        const NodeManager::Parameters* GetSettings() const { return &mSettings; }
    protected:
        virtual bool AddConnection(Connection* connection);
        bool CreateNewConnection(const Address& id,
                                 const Connection::Info* info,
                                 const Connection* originator,
                                 const bool fixed = false);
        virtual void ProcessPacket(const Packet& jausPacket,
                                   const Header& jausHeader,
                                   const Connection* connection,
                                   const Connection::Info* sourceInfo);
        virtual void UpdateConnections(const Packet& jausPacket,
                                       const Header& jausHeader,
                                       const Connection* connection,
                                       const Connection::Info* sourceInfo);
        virtual void RoutePacket(const Packet& jausPacket,
                                 const Header& jausHeader,
                                 const Connection* connection,
                                 const Connection::Info* sourceInfo);
        volatile bool mInitializedFlag;             ///<  Is NodeManager initialized?
        Parameters mSettings;                       ///<  Node manager settings.
        SharedMutex mConnectionsMutex;              ///<  Mutex for shared access to connections.

        Connection::Map mSharedMemoryConnections;   ///<  Connections to local components via shared memory.
        Connection::Map mUdpConnections;            ///<  UDP Connections discovered.
        Connection::Map mTcpConnections;            ///<  TCP Connections.

        static SharedMutex mFixedConnectionsMutex;                       ///<  Mutex for fixed connections.
        static std::map<Address, Connection::Info> mFixedConnections;    ///<  Fixed/default connections to maintain.

        Connection::Ptr mpTcpServer;        ///<  Main TCP host server for components on this node.
        Connection::Ptr mpUdpServer;        ///<  Main UDP host server/client for components on this node.
        std::string mSettingsFilename;      ///<  Name of the settings file for the node and connections.
        volatile bool mNodeShutdownFlag;    ///<  Flag to signal node manager shutdown to connections.
        
        // Connection update theads
        Thread mNodeUpdateThread;                     ///<  Node Manager update thread
        static void NodeUpdateThread(void * args);
        Connection::Thread  mSharedMemUpdateThread;   ///<  Manages the Shared Memory Server
        Connection::Thread  mTcpServerUpdateThread;   ///<  Manages the TCP Server
        Connection::Thread  mUdpServerUpdateThread;   ///<  Manages the UDP Server
        std::set<Connection::Thread*> mUpdateThreads; ///<  Connection refreshers.
    };
}

#endif
/*  End of File */
