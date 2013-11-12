////////////////////////////////////////////////////////////////////////////////////
///
///  \file nodemanager.cpp
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
#include "jaus/core/transport/nodemanager.h"
#include "jaus/core/transport/tcp.h"
#include "jaus/core/transport/udp.h"
#include "jaus/core/transport/sharedmemory.h"
#include <tinyxml/tinyxml.h>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

#define NAME_MUT_NAME "JAUS++NodeManager"

using namespace JAUS;

SharedMutex NodeManager::mFixedConnectionsMutex;
std::map<Address, Connection::Info> NodeManager::mFixedConnections;


/** Constructor for initializing default parameters. */
NodeManager::Parameters::Parameters()
{
    mDefaultPortNumber = 3794;
    mSingleThreadModeFlag = false;
    mIsTcpDefaultFlag = false;
    mDisconnectTimeMs = 0;
    mConnectionsPerThread = 1;
    mMulticastIP = std::string("239.255.0.1");
    mTimeToLive = 16;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads transport settings data.
///
///   \param[in] filename File containing Node settings data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::Parameters::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }

    TiXmlHandle doc(&xml);

    TiXmlElement* element = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DefaultTransport").ToElement();
    if(element && element->Attribute("type"))
    {
        if(std::string(element->Attribute("type")) == "JUDP")
        {
            mIsTcpDefaultFlag = false;
        }
        else
        {
            mIsTcpDefaultFlag = true;
        }
    }

    TiXmlNode* node;
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DefaultPort").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 0)
    {
        mDefaultPortNumber = (UShort)atoi(node->Value());
    }

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DisconnectTimeMs").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 0)
    {
        mDisconnectTimeMs = (unsigned int)(atoi(node->Value()));
    }

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("ConnectionsPerHandler").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 1)
    {
        this->mConnectionsPerThread = (unsigned int)(atoi(node->Value()));
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("MulticastIP").FirstChild().ToNode();
    if(node && node->Value())
    {
        mMulticastIP.SetAddress(node->Value());
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("TTL").FirstChild().ToNode();
    if(node && node->Value())
    {
        mTimeToLive = (unsigned char )atoi(node->Value());
    }

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("NetAddress").FirstChild().ToNode();
    if(node && node->Value())
    {
        mNetworkInterface.SetAddress(node->Value());
    }

    element = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("Connection").ToElement();
    while(element)
    {
        if(element->Attribute("ip") && element->Attribute("id"))
        {
            Connection::Info info;
            info.mDestIP = std::string(element->Attribute("ip"));
            if(mIsTcpDefaultFlag)
            {
                info.mTransportType = Connection::Transport::JTCP;
            }
            else
            {
                info.mTransportType = Connection::Transport::JUDP;
            }
            
            // Override if specific
            if(element->Attribute("type"))
            {
                if(std::string("JUDP") == std::string(element->Attribute("type")))
                {
                    info.mTransportType = Connection::Transport::JUDP;
                }
                if(std::string("JTCP") == std::string(element->Attribute("type")))
                {
                    info.mTransportType = Connection::Transport::JUDP;
                }
                if(std::string("JSerial") == std::string(element->Attribute("type")))
                {
                    info.mTransportType = Connection::Transport::JSerial;
                }
            }
            if(element->Attribute("port"))
            {
                info.mPortName = element->Attribute("port");
                if(info.mTransportType == Connection::Transport::JUDP ||
                    info.mTransportType == Connection::Transport::JTCP)
                {
                    info.mDestPortNumber = (unsigned short)atoi(info.mPortName.c_str());
                }
            }
            if(element->Attribute("dest_port"))
            {
                info.mPortName = element->Attribute("dest_port");
                if(info.mTransportType == Connection::Transport::JUDP ||
                    info.mTransportType == Connection::Transport::JTCP)
                {
                    if(info.mPortName == "DEFAULT")
                    {
                        info.mDestPortNumber = mDefaultPortNumber;
                    }
                    else
                    {
                        info.mDestPortNumber = (unsigned short)atoi(info.mPortName.c_str());
                    }
                }
            }
            if(element->Attribute("src_port"))
            {
                if(info.mTransportType == Connection::Transport::JUDP ||
                    info.mTransportType == Connection::Transport::JTCP)
                {
                    std::string value = element->Attribute("src_port");
                    if(value == "DEFAULT")
                    {
                        info.mSourcePortNumber = mDefaultPortNumber;
                    }
                    else
                    {
                        info.mSourcePortNumber = (unsigned short)atoi(element->Attribute("src_port"));
                    }
                }
            }
            else
            {
                info.mSourcePortNumber = mDefaultPortNumber;
            }
            Address id = Address::FromString(element->Attribute("id"));
            mCustomConnections[id] = info;
        }
        element = element->NextSiblingElement("Connection");
    }

    return true;
}


/** Adds a custom connections for the Node Manager to make. */
void NodeManager::Parameters::AddCustomConnection(const Address& componentID,
                                                  const Connection::Info& connectionDetails)
{
    mCustomConnections[componentID] = connectionDetails;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] singleThreadMode If true, operate in single thread mode.
///
////////////////////////////////////////////////////////////////////////////////////
NodeManager::NodeManager(const bool singleThreadMode)
                                                        
{
    mInitializedFlag = false;
    mNodeShutdownFlag = false;

    // At least one thread for connections should be created.
    for(unsigned int i = 0; i < 1; i++)
    {
        mUpdateThreads.insert(new Connection::Thread(this, &mNodeShutdownFlag));
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
NodeManager::~NodeManager()
{
    Shutdown();

    std::set<Connection::Thread*>::iterator refresh;
    for(refresh = mUpdateThreads.begin();
        refresh != mUpdateThreads.end();
        refresh++)
    {
        delete (*refresh);
    }
    mUpdateThreads.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads transport settings data.
///
///   Must be called before Initialize method.
///
///   \param[in] filename File containing UDP settings data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::LoadSettings(const std::string& filename)
{
    if(IsInitialized())
    {
        return false;
    }

    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }

    mSettingsFilename = filename;

    if(mSettings.LoadSettings(filename))
    {
        WriteLock fwLock(NodeManager::mFixedConnectionsMutex);
        NodeManager::mFixedConnections = mSettings.GetCustomConnections();
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Node Manager.
///
///   \return True on success (or manager already running), false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::Initialize()
{
    bool result = false;

    mNodeShutdownFlag = false;

    // Setup TCP Server.
    mpTcpServer.reset(new TCP());
    mpTcpServer->SetGlobalShutdownFlag(&mNodeShutdownFlag);
    mpTcpServer->SetNodeManager(this);
    mpTcpServer->RegisterCallback(this);
    // Load/Setup Parameters
    TCP::Parameters* tcpParams = (TCP::Parameters*)mpTcpServer->CreateParameters();
    tcpParams->LoadSettings(mSettingsFilename);
    tcpParams->mClientFlag = false;
    tcpParams->mDestPortNumber = tcpParams->mSourcePortNumber = mSettings.mDefaultPortNumber;
    tcpParams->mNetworkInterface = mSettings.GetNetworkInterface();

    // Intialize
    if(mpTcpServer->Initialize(tcpParams))
    {
        result = true;

        // Setup UDP Server
        mpUdpServer.reset(new UDP());
        mpUdpServer->SetGlobalShutdownFlag(&mNodeShutdownFlag);
        mpUdpServer->SetNodeManager(this);
        mpUdpServer->RegisterCallback(this);
        // Load/Setup Parameters
        UDP::Parameters* udpParams = (UDP::Parameters*)mpUdpServer->CreateParameters();
        udpParams->LoadSettings(mSettingsFilename);
        udpParams->mDestPortNumber = udpParams->mSourcePortNumber = mSettings.mDefaultPortNumber;
        udpParams->mClientFlag = false;
        udpParams->mNetworkInterface = mSettings.GetNetworkInterface();
        udpParams->mMulticastIP = mSettings.GetMulticastIP();
        udpParams->mTimeToLive = mSettings.GetMulticastTLL();
        // Intialize
        mpUdpServer->Initialize(udpParams);

        if(mSettings.mSingleThreadModeFlag == false)
        {
            // A Connection::Thread is used to update a connection
            // to do receiving, callbacks, etc.  A number
            // of these are used to reduce the number of threads
            // generated by the process, and more are created as
            // they are needed.  Each Connection::Thread has a fixed number
            // of connections in can handle.
            mTcpServerUpdateThread.SetThreadName("TCP-Listen");
            mTcpServerUpdateThread.SetParent(this, &mNodeShutdownFlag, false);
            mTcpServerUpdateThread.AddConnection(mpTcpServer);

            mUdpServerUpdateThread.SetThreadName("UDP-Recv");
            mUdpServerUpdateThread.SetParent(this, &mNodeShutdownFlag, false);
            mUdpServerUpdateThread.AddConnection(mpUdpServer);
        }

        delete udpParams;

        mInitializedFlag = true;

        // Create fixed connections.
        std::map<Address, Connection::Info>::iterator fc;
        for(fc = NodeManager::mFixedConnections.begin();
            fc != NodeManager::mFixedConnections.end();
            fc++)
        {
            // Try create a connection.
            WriteLock wLock(mConnectionsMutex);
            CreateNewConnection(fc->first, &(fc->second), NULL, true);
        }
        for(fc = mSettings.mCustomConnections.begin();
            fc != mSettings.mCustomConnections.end();
            fc++)
        {
            // Try create a connection.
            WriteLock wLock(mConnectionsMutex);
            CreateNewConnection(fc->first, &(fc->second), NULL, true);
        }
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));

        if(mSettings.mSingleThreadModeFlag == false)
        {
            mNodeUpdateThread.CreateThread(NodeManager::NodeUpdateThread, this);
        }
    }
    else
    {
        // Server already running or some other error
        // has happened.
        mpTcpServer.reset();
    }

    delete tcpParams;

    return result;
}

/** Method to check if initialized (and running). */
bool NodeManager::IsInitialized() const
{
    return mInitializedFlag;
}


/** Does shutdown procedures. */
void NodeManager::Shutdown()
{
    if(IsInitialized() == false)
    {
        return;
    }

    Connection::Map::iterator out;
    Connection::Set::iterator in;
    Connection::Set connections;

    bool resetCounter = false;

    {
        WriteLock wLock(mConnectionsMutex);
        mNodeShutdownFlag = true;
    }

    mNodeUpdateThread.StopThread();

    // Shut everything down
    {
        ReadLock rLock(mConnectionsMutex);

        mTcpServerUpdateThread.ShutdownConnections();
        mUdpServerUpdateThread.ShutdownConnections();

        std::set<Connection::Thread*>::iterator refresh;
        for(refresh = mUpdateThreads.begin();
            refresh != mUpdateThreads.end();
            refresh++)
        {
            (*refresh)->ShutdownConnections();
        }
        if(mpTcpServer != NULL)
        {
            mpTcpServer->Shutdown();
            resetCounter = true;
        }
        if(mpUdpServer != NULL)
        {
            mpUdpServer->Shutdown();
        }
    }

    CxUtils::SleepMs(100);

    WriteLock wLock(mConnectionsMutex);

    // Ensure everything is closed, and move to
    // delete buffers.

    // Reset pointers and delete memory.

    mpTcpServer.reset();
    mpUdpServer.reset();

    // Stop/Kill Refresh System.
    mTcpServerUpdateThread.RemoveConnection(Connection::Ptr());
    mUdpServerUpdateThread.RemoveConnection(Connection::Ptr());

    /*mSharedMemUpdateThread.StopThread(10000);*/
    mTcpServerUpdateThread.StopThread(10000);
    mUdpServerUpdateThread.StopThread(10000);

    std::set<Connection::Thread*>::iterator refresh;
    for(refresh = mUpdateThreads.begin();
        refresh != mUpdateThreads.end();
        refresh++)
    {
        (*refresh)->RemoveConnection(Connection::Ptr());
        (*refresh)->StopThread(10000);
    }

    if(resetCounter)
    {
        Connection::ConnectionCounter = 0;
    }

    mInitializedFlag = false;
}


/** Called when either system event thread updates node manager services
    or manually when in a single thread mode. */
void NodeManager::UpdateServiceEvent()
{
    static Time::Stamp cleanupTime = Time::GetUtcTimeMs();
    unsigned int checkInterval = mSettings.mDisconnectTimeMs > 0 ? mSettings.mDisconnectTimeMs : 5000;

    if(mpTcpServer == NULL || mpUdpServer == NULL)
    {
        return;
    }

    // Scope locked
    {
#ifdef JAUS_USE_UPGRADE_LOCKS
        UpgradeLock upgradeLock(mConnectionsMutex);
#else
        WriteLock wLock(mConnectionsMutex);
#endif

        if(false == mNodeShutdownFlag && Time::GetUtcTimeMs() - cleanupTime >= 2000)
        {
            std::map<Address, Connection::Info> fixedCopy;
            {
                ReadLock rLock(NodeManager::mFixedConnectionsMutex);
                fixedCopy = mFixedConnections;
            }

            Connection::Map toRemove;
            Connection::Map::iterator remove;

            Connection::Map::iterator con;

             // Close invalid connections

            con = mSharedMemoryConnections.begin();
            while(con != mSharedMemoryConnections.end() && false == mNodeShutdownFlag )
            {
                Time::Stamp updateTime = con->second->GetUpdateTimeUtcMs();
                Time::Stamp currentTime = Time::GetUtcTimeMs();
                // Shared memory should be updating much more quickly, so it
                // can be removed more often.
                SharedMemory* sm = dynamic_cast<SharedMemory*>(con->second.get());
                if(sm && (sm->IsConnected() == false || 
                    (mSettings.mDisconnectTimeMs > 0 && (currentTime - updateTime) >= 60000)))
                {
                    con->second->Shutdown();
                    toRemove[con->first] = con->second;
                }
                con++;
            }

            con = mUdpConnections.begin();
            while(con != mUdpConnections.end() && false == mNodeShutdownFlag )
            {
                Time::Stamp updateTime = con->second->GetUpdateTimeUtcMs();
                Time::Stamp currentTime = Time::GetUtcTimeMs();
                // Shared memory should be updating much more quickly, so it
                // can be removed more often.
                UDP* udp = dynamic_cast<UDP*>(con->second.get());
                if(udp && udp->IsFixedConnection() == false && 
                    (udp->IsConnected() == false || 
                    (mSettings.mDisconnectTimeMs && (currentTime - updateTime) >= mSettings.mDisconnectTimeMs)))
                {
                    con->second->Shutdown();
                    toRemove[con->first] = con->second;
                }
                con++;
            }

            con = mTcpConnections.begin();
            while(con != mTcpConnections.end() && false == mNodeShutdownFlag )
            {
                Time::Stamp updateTime = con->second->GetUpdateTimeUtcMs();
                Time::Stamp currentTime = Time::GetUtcTimeMs();
                // Shared memory should be updating much more quickly, so it
                // can be removed more often.
                TCP* tcp = dynamic_cast<TCP*>(con->second.get());
                if(tcp && tcp->IsConnected() == false)
                {
                    con->second->Shutdown();
                    toRemove[con->first] = con->second;
                }
                con++;
            }

            if(toRemove.size() > 0)
            {
                //  Since we are modifying things, we must upgrade to unique
                //  access.
#ifdef JAUS_USE_UPGRADE_LOCKS
                UpgradeToUniqueLock uniqueLock(upgradeLock);
#endif
                
                for(remove = toRemove.begin();
                    remove != toRemove.end();
                    remove++)
                {
                    // Make sure connection is removed from all
                    // lists of connections
                    con = mSharedMemoryConnections.find(remove->first);
                    if(con != mSharedMemoryConnections.end())
                    {
                        mSharedMemoryConnections.erase(con);
                    }

                    con = mUdpConnections.find(remove->first);
                    if(con != mUdpConnections.end())
                    {
                        mUdpConnections.erase(con);
                    }

                    con = mTcpConnections.find(remove->first);
                    if(con != mTcpConnections.end())
                    {
                        mTcpConnections.erase(con);
                    }

                    // Remove from threads
                    std::set<Connection::Thread*>::iterator refresh;
                    for(refresh = mUpdateThreads.begin();
                        refresh != mUpdateThreads.end();
                        refresh++)
                    {
                        (*refresh)->RemoveConnection(remove->second);
                    }
                }
            }

            // See if we have fixed connection or not that needs to be made
            std::map<Address, Connection::Info>::iterator fix;
            // Merge settings loaded for connections to make.
            for(fix = mSettings.mCustomConnections.begin();
                fix != mSettings.mCustomConnections.end();
                fix++)
            {
                fixedCopy[fix->first] = fix->second;
            }
            for(fix = fixedCopy.begin();
                fix != fixedCopy.end() && false == mNodeShutdownFlag;
                fix++)
            {
                if(fix->second.mTransportType == Connection::Transport::JUDP)
                {
                    if(mUdpConnections.find(fix->first) == mUdpConnections.end())
                    {
                        //  Since we are modifying things, we must upgrade to unique
                        //  access.
#ifdef JAUS_USE_UPGRADE_LOCKS
                        UpgradeToUniqueLock uniqueLock(upgradeLock);
#endif
                        CreateNewConnection(fix->first, & (fix->second), NULL, true);
                    }
                }
                if(fix->second.mTransportType == Connection::Transport::JTCP)
                {
                    if(mTcpConnections.find(fix->first) == mTcpConnections.end())
                    {
                        //  Since we are modifying things, we must upgrade to unique
                        //  access.
#ifdef JAUS_USE_UPGRADE_LOCKS
                        UpgradeToUniqueLock uniqueLock(upgradeLock);
#endif
                        CreateNewConnection(fix->first, & (fix->second), NULL, true);
                    }
                }
            }

            cleanupTime = Time::GetUtcTimeMs();
        }
        
    }
    // If single thread mode, we must manually
    // update all receive calls
    if(mSettings.mSingleThreadModeFlag)
    {
        ReadLock readLock(mConnectionsMutex);

        if(mpTcpServer != NULL)
        {
            mpTcpServer->UpdateConnection();
        }
        Connection::Map::iterator con;
        for(con = mSharedMemoryConnections.begin();
            con != mSharedMemoryConnections.end();
            con++)
        {
            con->second->UpdateConnection();
        }
        for(con = mTcpConnections.begin();
            con != mTcpConnections.end();
            con++)
        {
            con->second->UpdateConnection();
        }
        for(con = mUdpConnections.begin();
            con != mUdpConnections.end();
            con++)
        {
            con->second->UpdateConnection();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Tries to create a new connection, and make sure it doesn't
///          get deleted.
///
///   \param[in] id JAUS destination ID.
///   \param[in] info Additional information about the source of the packet
///                         like the address it came in on.
///   \param[in] permanent If true, the connection cannot be closed due to
///                        inactivity, only when signaled by the connection
///                        directly (like tcp shutdown).
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::CreateConnection(const Address& id,
                                   const Connection::Info& info,
                                   const bool permanent)
{
    bool result = false;
    {
        WriteLock wLock(mConnectionsMutex);
        result = CreateNewConnection(id, &info, NULL, permanent);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a connection to the connection list.  
///
///   \param[in] connection Connection to add.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::AddConnection(Connection* connection)
{
    bool result = false;

    if(connection == NULL || connection->GetConnectionID().IsValid() == false)
    {
        return result;
    }

    WriteLock wLock(mConnectionsMutex);

    connection->SetGlobalShutdownFlag(&mNodeShutdownFlag);
    connection->RegisterCallback(this);
    connection->SetNodeManager(this);

    // Check for existing connections first.
    Connection::Map::iterator sm, udp, tcp;
    sm = mSharedMemoryConnections.find(connection->GetConnectionID());
    udp = mUdpConnections.find(connection->GetConnectionID());
    tcp = mTcpConnections.find(connection->GetConnectionID());

    if((sm != mSharedMemoryConnections.end() && connection->GetConnectionTransportType() == sm->second->GetConnectionTransportType()) ||
       (udp != mUdpConnections.end() && connection->GetConnectionTransportType() == udp->second->GetConnectionTransportType())  ||
       (tcp != mTcpConnections.end() && connection->GetConnectionTransportType() == tcp->second->GetConnectionTransportType()) )
    {
        // Connection already exists
        return result;
    }

    Connection::Ptr ptr(connection);

    // Assign to the right map of connections.
    switch(connection->GetConnectionTransportType())
    {
    case Connection::Transport::JSharedMemory:
        mSharedMemoryConnections[connection->GetConnectionID()] = ptr;
        break;
    case Connection::Transport::JUDP:
        mUdpConnections[connection->GetConnectionID()] = ptr;
        break;
    default:
        mTcpConnections[connection->GetConnectionID()] = ptr;
        break;
    }

    // Add to refreshers if needed
    if(mSettings.mSingleThreadModeFlag == false && ptr->IsSendOnly() == false)
    {
        bool refreshed = false;
        std::set<Connection::Thread*>::iterator refresh;
        for(refresh = mUpdateThreads.begin();
            refresh != mUpdateThreads.end();
            refresh++)
        {
            if((*refresh)->AddConnection(ptr))
            {
                refreshed = true;
                break;
            }
        }

        if(refreshed == false)
        {
            Connection::Thread* refresher = new Connection::Thread(this, &mNodeShutdownFlag);
            refresher->AddConnection(ptr);
            mUpdateThreads.insert(refresher);
        }
    }

    result = true;

    return result;
}


/** Gets local and remote connection stats for the NodeManager. */
bool NodeManager::GetStatistics(Connection::Statistics::List& local,
                                Connection::Statistics::List& remote)
{
    bool result = false;
    Connection::Set connections;
    local.clear();
    remote.clear();
    {
        ReadLock readLock(mConnectionsMutex);

        if(NULL == mpTcpServer)
        {
            return result;
        }
        
        remote.push_back(mpUdpServer->GetStatistics());

        Connection::Map::iterator con;
        // compile all connections, then sort by local/remote
        for(con = mSharedMemoryConnections.begin();
            con != mSharedMemoryConnections.end();
            con++)
        {
            local.push_back(con->second->GetStatistics());
        }
        for(con = mUdpConnections.begin();
            con != mUdpConnections.end();
            con++)
        {
            remote.push_back(con->second->GetStatistics());
        }
        for(con = mTcpConnections.begin();
            con != mTcpConnections.end();
            con++)
        {
            remote.push_back(con->second->GetStatistics());
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Within the node manager threads are used (when in multithread mode)
///          to update connections for receiving data.  A thread is allowed to
///          update more than one connection, this sets the limit.
///
///   This function only works if NodeManager is not initialized.
///
///   \param[in] limit Maximum number of connections a single thread is allowed
///                    to maintain/update.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::SetConnectionsPerThread(const unsigned int limit)
{
    if(mpTcpServer != NULL || mpUdpServer != NULL || limit < 1)
    {
        return false;
    }
    mSettings.mConnectionsPerThread = limit;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Tries to create a new connection.
///
///   \param[in] id JAUS destination ID.
///   \param[in] originator Pointer to the connection the data was received by if
///                        not available, use NULL.
///   \param[in] info Additional information about the source of the packet
///                         like the address it came in on.
///   \param[in] fixed If true, than connection cannot be deleted by timeout on
///                    receiving data.
///
////////////////////////////////////////////////////////////////////////////////////
bool NodeManager::CreateNewConnection(const Address& id,
                                      const Connection::Info* info,
                                      const Connection* originator,
                                      const bool fixed)
{
    bool result = false;
    if(info == NULL)
    {
        return result;
    }

    Connection::Ptr newConnection;

    // Check for shared memory connections on local host
    if(info->mTransportType == Connection::Transport::JSharedMemory)
    {
        Connection::Map::iterator existing = mSharedMemoryConnections.find(id);

        const SharedMemory::Parameters* smParams = dynamic_cast<const SharedMemory::Parameters*>(info);
        if(smParams)
        {
            if(existing != mSharedMemoryConnections.end())
            {
                SharedMemory* sharedMem = dynamic_cast<SharedMemory*>(existing->second.get());
                if(sharedMem->GetParameters().mCreationTime == smParams->mCreationTime)
                {
                    // Same connection, all is well.
                    return true;
                }
                existing->second->Shutdown();
                existing->second.reset();
                mSharedMemoryConnections.erase(existing);
            }

            newConnection.reset(new SharedMemory(true));

            if(originator)
            {
                newConnection->CopyConnectionData((Connection *)originator);
            }
            else
            {
                newConnection->SetGlobalShutdownFlag(&mNodeShutdownFlag);
                newConnection->SetNodeManager(this);
            }
            newConnection->RegisterCallback(this);
            newConnection->SetFixedConnection(fixed);

            if(newConnection->Initialize(smParams))
            {
                result = true;
                mSharedMemoryConnections[id] = newConnection;
            }
        }
    }
    else if(info->mTransportType == Connection::Transport::JUDP)
    {
        Connection::Map::iterator existing = mUdpConnections.find(id);

        if(existing == mUdpConnections.end() || existing->second->IsConnected() == false)
        {
            if(existing != mUdpConnections.end())
            {
                existing->second->Shutdown();
                existing->second.reset();
                mUdpConnections.erase(existing);
            }
            if(mpUdpServer != NULL && 
               (mpUdpServer.get() == originator || 
                mSettings.mDefaultPortNumber == info->mSourcePortNumber || 
                info->mSourcePortNumber == 0))
            {
                newConnection.reset(mpUdpServer->CreateConnection(id, info));
                newConnection->SetFixedConnection(fixed);
            }
            else
            {
                UDP::Parameters udpParams;
                udpParams.LoadSettings(mSettingsFilename);
                udpParams.mClientFlag = false;
                udpParams = *info;
                udpParams.mMulticastIP.Clear();
                newConnection.reset(new UDP());

                if(newConnection->Initialize(&udpParams) == false)
                {
                    newConnection.reset();
                    // Try find a UDP server we can extend
                    Connection::Map::iterator server;
                    for(server = mUdpConnections.begin();
                        server != mUdpConnections.end();
                        server++)
                    {
                        UDP* s = dynamic_cast<UDP*>(server->second.get());
                        // If the source port matches for the UDP connection, then
                        // create a new client connection from the same source port number
                        if(s && (s == originator || s->GetSourcePortNumber() == info->mSourcePortNumber))
                        {
                            newConnection.reset(s->CreateConnection(id, info));
                            break;
                        }
                    }
                }
                else
                {
                    newConnection->SetFixedConnection(true);
                }
            }
            if(newConnection != NULL)
            {
                result = true;
                newConnection->RegisterCallback(this);
                newConnection->SetGlobalShutdownFlag(&mNodeShutdownFlag);
                newConnection->SetNodeManager(this);
                mUdpConnections[id] = newConnection;
            }
        }
        else
        {
            return true;
        }
    }
    else if(info->mTransportType == Connection::Transport::JTCP && mpTcpServer != NULL)
    {
        Connection::Map::iterator existing = mTcpConnections.find(id);
        if(existing == mTcpConnections.end() || existing->second->IsConnected() == false)
        {
            if(existing != mTcpConnections.end())
            {
                existing->second->Shutdown();
                existing->second.reset();
                mTcpConnections.erase(existing);
            }

            newConnection.reset(mpTcpServer->CreateConnection(id, info));
            
            if(newConnection != NULL)
            {
                result = true;
                newConnection->RegisterCallback(this);
                newConnection->SetGlobalShutdownFlag(&mNodeShutdownFlag);
                newConnection->SetNodeManager(this);
                newConnection->SetFixedConnection(fixed);
                mTcpConnections[id] = newConnection;
            }
        }
        else
        {
            return true;
        }
    }

    if(result)
    {
        // Add to refreshers if needed
        if(mSettings.mSingleThreadModeFlag == false)
        {
            bool refreshed = false;
            std::set<Connection::Thread*>::iterator refresh;
            for(refresh = mUpdateThreads.begin();
                refresh != mUpdateThreads.end();
                refresh++)
            {
                if((*refresh)->AddConnection(newConnection))
                {
                    refreshed = true;
                    break;
                }
            }

            if(refreshed == false)
            {
                Connection::Thread* refresher = new Connection::Thread(this, &mNodeShutdownFlag);
                refresher->AddConnection(newConnection);
                mUpdateThreads.insert(refresher);
            }
        }
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Process all data as it arrives to the system and creates new
///          connections and routes to appropriate destinations as needed.
///
///   \param[in] jausPacket JAUS Packet data without the transport header.
///   \param[in] jausHeader JAUS General Header information for the packet.
///   \param[in] connection Pointer to the connection the data was received by.
///   \param[in] sourceInfo Additional information about the source of the packet
///                         like the address it came in on.
///
////////////////////////////////////////////////////////////////////////////////////
void NodeManager::ProcessPacket(const Packet& jausPacket,
                                const Header& jausHeader,
                                const Connection* connection,
                                const Connection::Info* sourceInfo)
{
    UpdateConnections(jausPacket,
                      jausHeader,
                      connection,
                      sourceInfo);
    RoutePacket(jausPacket,
                jausHeader,
                connection,
                sourceInfo);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Looks to see if any new connections need to be made based on
///          incomming data.
///
///   \param[in] jausPacket JAUS Packet data without the transport header.
///   \param[in] jausHeader JAUS General Header information for the packet.
///   \param[in] connection Pointer to the connection the data was received by.
///   \param[in] sourceInfo Additional information about the source of the packet
///                         like the address it came in on.
///
////////////////////////////////////////////////////////////////////////////////////
void NodeManager::UpdateConnections(const Packet& jausPacket,
                                    const Header& jausHeader,
                                    const Connection* connection,
                                    const Connection::Info* sourceInfo)
{
    if(this->mNodeShutdownFlag)
    {
        return;
    }

    Connection::Map::iterator con; // Connection iterator
#ifdef JAUS_USE_UPGRADE_LOCKS
    UpgradeLock upgradeLock(mConnectionsMutex);
#else
    WriteLock wLock(mConnectionsMutex);
#endif

    if(connection->GetConnectionTransportType() == Connection::Transport::JSharedMemory)
    {
        // Check if this is a new possible outbound connection to be made.
        con = mSharedMemoryConnections.find(jausHeader.mSourceID);
        // If we do not have a connection, create one
        if(con == mSharedMemoryConnections.end())
        {
            // Upgrade to a unique lock since we are making changes. 
#ifdef JAUS_USE_UPGRADE_LOCKS
            UpgradeToUniqueLock uniqueLock(upgradeLock);
#endif

            CreateNewConnection(jausHeader.mSourceID, // Destination JAUS ID
                                sourceInfo,           // Matches destination info
                                connection);          // Source where we discovered possible connection
        }
    }
    // Only create new connections to other nodes that
    // use discovery over UDP
    else if(connection->GetConnectionTransportType() == Connection::Transport::JUDP)
    {
        // See if we have a connection at all.
        Connection::Map::iterator udp, tcp;
        udp = mUdpConnections.find(jausHeader.mSourceID);
        tcp = mTcpConnections.find(jausHeader.mSourceID);

        bool createNewConnection = false;
        if(udp == mUdpConnections.end() && tcp == mTcpConnections.end())
        {
            createNewConnection = true;
        }
        if(udp != mUdpConnections.end() && 
                    (udp->second->IsConnected() == false || udp->second->ConnectionChanged(sourceInfo)))
        {
            createNewConnection = true;
        }
        if(tcp != mTcpConnections.end() && 
                    (tcp->second->IsConnected() == false || tcp->second->ConnectionChanged(sourceInfo)))
        {
            createNewConnection = true;
        }
            
        if(createNewConnection)
        {
            // Upgrade to a unique lock since we are making changes. 
#ifdef JAUS_USE_UPGRADE_LOCKS
            UpgradeToUniqueLock uniqueLock(upgradeLock);
#endif

            // Step 1 : Delete old connection if present
            Connection::Ptr oldConnection;
            if(udp != mUdpConnections.end())
            {
                createNewConnection = true;
                oldConnection = udp->second;
                mUdpConnections.erase(udp);
            }
            if(tcp != mTcpConnections.end() && 
                        (tcp->second->IsConnected() == false || tcp->second->ConnectionChanged(sourceInfo)))
            {
                createNewConnection = true;
                oldConnection = tcp->second;
                mTcpConnections.erase(tcp);
            }

            if(oldConnection != NULL)
            {
                oldConnection->Shutdown();
                // Remove the connection from any update threads
                std::set<Connection::Thread*>::iterator refresh;
                for(refresh = mUpdateThreads.begin();
                    refresh != mUpdateThreads.end();
                    refresh++)
                {
                    (*refresh)->RemoveConnection(oldConnection);
                }
            }

            // Step 2: Create new connection

            Connection::Info newConfInfo = *sourceInfo;

            // If use TCP is true, try TCP connection before
            // UDP
            if(mSettings.mIsTcpDefaultFlag == true)
            {
                newConfInfo.mTransportType = Connection::Transport::JTCP;
                newConfInfo.mDestPortNumber = TCP::Port;
            }
            else
            {
                newConfInfo.mTransportType = Connection::Transport::JUDP;
            }

            if(CreateNewConnection(jausHeader.mSourceID,
                                   &newConfInfo,
                                   connection) == false)
            {
                // If TCP attemp failed, create UDP connection.
                if(newConfInfo.mTransportType == Connection::Transport::JTCP)
                {
                    // Create UDP connection to source port of message.
                    newConfInfo.mTransportType = newConfInfo.mTransportType = Connection::Transport::JUDP;
                    newConfInfo.mPortName = sourceInfo->mDestPortNumber;
                    CreateNewConnection(jausHeader.mSourceID,
                                        &newConfInfo,
                                        connection);
                }
            }
        }
        else
        {
            if(udp != mUdpConnections.end())
            {
                udp->second->SignalConnectionUpdate(jausPacket.Length());
            }
            if(tcp != mTcpConnections.end())
            {
                tcp->second->SignalConnectionUpdate(jausPacket.Length());
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Routes the packet to the correct destination.
///
///   \param[in] jausPacket JAUS Packet data without the transport header.
///   \param[in] jausHeader JAUS General Header information for the packet.
///   \param[in] connection Pointer to the connection the data was received by.
///   \param[in] sourceInfo Additional information about the source of the packet
///                         like the address it came in on.
///
////////////////////////////////////////////////////////////////////////////////////
void NodeManager::RoutePacket(const Packet& jausPacket,
                              const Header& jausHeader,
                              const Connection* connection,
                              const Connection::Info* sourceInfo)
{
    if(this->mNodeShutdownFlag)
    {
        return;
    }

    Connection::Map::iterator con; // Connection iterator
    // Shared lock
    ReadLock readLock(mConnectionsMutex);

    // Check for broadcast message first.
    if(jausHeader.mDestinationID.IsBroadcast() && false == mNodeShutdownFlag)
    {
        bool fromLocalHost = connection->IsLocalConnection();
        bool globalBroadcastSuccess = false;
        if(fromLocalHost && jausHeader.mBroadcastFlag != Header::Broadcast::None)
        {
            globalBroadcastSuccess = mpUdpServer->SendPacket(jausPacket, jausHeader);
        }

        // Send to all matching destinations, but only send to every
        // connection if the message originated on this node,
        // otherwise it came from another network, and we should only send
        // to local node connections.  This will prevent messages being
        // fired back and forth into infiniti, and beyond!
        Header directHeader = jausHeader;
        directHeader.mBroadcastFlag = Header::Broadcast::None;
        bool resetHeader = false;

        for(con = mUdpConnections.begin();
            con != mUdpConnections.end() && fromLocalHost;
            con++)
        {
            UDP* udp = dynamic_cast<UDP *>(con->second.get());
            // IF the destination port matches the one used for
            // broadcast, then don't send the message again, but make
            // sure we send if it is a "fixed" connection
            if(udp->IsFixedConnection() == false && udp->GetDestPortNumber() == mSettings.mDefaultPortNumber)
            {
                continue;
            }
            if(Address::DestinationMatch(jausHeader.mDestinationID, con->first) &&
                con->first != jausHeader.mSourceID)
            {
                directHeader.mDestinationID = con->first;
                // Change destination address so it is no longer broadcast.
                resetHeader = true;
                Packet* ptr = (Packet *)&jausPacket;
                ptr->SetWritePos(0);
                directHeader.Write(*ptr);
                con->second->SendPacket(jausPacket, directHeader);
            }
        }
        // Broadcast to non-dynamic TCP connections
        for(con = mTcpConnections.begin();
            con != mTcpConnections.end() && fromLocalHost;
            con++)
        {
            // Only send to outgoing connections we generated.
            if(false == con->second->IsFixedConnection() || ((TCP*)con->second.get())->IsClient() == false)
            {
                continue;
            }
            if(Address::DestinationMatch(jausHeader.mDestinationID, con->first) &&
                con->first != jausHeader.mSourceID)
            {
                directHeader.mDestinationID = con->first;
                // Change destination address so it is no longer broadcast.
                resetHeader = true;
                Packet* ptr = (Packet *)&jausPacket;
                ptr->SetWritePos(0);
                directHeader.Write(*ptr);
                con->second->SendPacket(jausPacket, directHeader);
            }
        }

        // Reset packet header
        if(resetHeader)
        {
            Packet* ptr = (Packet *)&jausPacket;
            ptr->SetWritePos(0);
            jausHeader.Write(*ptr);
        }

        // Send to shared memory components
        for(con = mSharedMemoryConnections.begin();
            con != mSharedMemoryConnections.end();
            con++)
        {
            if(con->first != jausHeader.mSourceID && 
                Address::DestinationMatch(jausHeader.mDestinationID, con->first))
            {
                con->second->SendPacket(jausPacket, jausHeader);
            }
        }

    }
    else
    {
        // Look up the destination and send directly.
        bool haveConnection = false;

        con = mSharedMemoryConnections.find(jausHeader.mDestinationID);
        if( con != mSharedMemoryConnections.end())
        {
            haveConnection = true;
        }
        con = mSharedMemoryConnections.find(jausHeader.mDestinationID);
        if( haveConnection == false && (con = mUdpConnections.find(jausHeader.mDestinationID)) != mUdpConnections.end())
        {
            haveConnection = true;
        }
        if( haveConnection == false && (con = mTcpConnections.find(jausHeader.mDestinationID)) != mTcpConnections.end())
        {
            haveConnection = true;
        }

        if(haveConnection)
        {
            con->second->SendPacket(jausPacket, jausHeader);
        }
    }
}


/** Update NodeManager if required. */
void NodeManager::NodeUpdateThread(void* args)
{
    NodeManager* node = (NodeManager*)args;

    while(node && node->mNodeUpdateThread.QuitThreadFlag() == false &&
          node->mNodeShutdownFlag == false)
    {
        node->UpdateServiceEvent();
        boost::this_thread::sleep(boost::posix_time::milliseconds(100));
    }
}


/*  End of File */
