////////////////////////////////////////////////////////////////////////////////////
///
///  \file transportmanager.cpp
///  \brief The "Transport" TransportManager is a program that handles UDP, TCP, and 
///         other transport communcations on a host PC.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 October 2010
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
#include "jaus/core/transport/transportmanager.h"
#include "jaus/core/transport/judp.h"
#include <iomanip>
#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace JAUS;

    
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TransportManager::TransportManager()
{
    mDebugMessagesFlag = false;             // Turns on/off debug messages.
    mUseTcpFlag = true;                     // Try to make TCP connections first, fail to UDP
    mDisconnectTimeMs = 3600000;            // 1 Hour
    mDelayTimeMs = 0;                       // As fast as possible.
    mMulticastIP.SetAddress("239.255.0.1"); // Multicast group (default)
    mTimeToLive = 16;                       // Default mutlicast TTL
    mUseBroadcastingFlag = false;           // If true, create broadcast instead of multicast for discovery.
    mDebugMessagesMutex.Create("JAUS");     // Debug messages mutex.
    mGlobalShutdownFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TransportManager::~TransportManager()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads UDP configuration values from an XML file.  Only loads values
///          if transport has not been initialized.
///
///   \param[in] filename File containing UDP settings data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TransportManager::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);

    TiXmlElement* element = doc.FirstChild("JAUS").FirstChild("Transport").ToElement();
    if(element && element->Attribute("type"))
    {
        if(std::string(element->Attribute("type")) == "JUDP")
        {
            mUseTcpFlag = false;
        }
        else
        {
            mUseTcpFlag = true;
        }
    }
    TiXmlNode* node;

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("BroadcastIP").FirstChild().ToNode();
    if(node && node->Value())
    {
        mUseBroadcastingFlag = true;
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("UseBroadcasting").FirstChild().ToNode();
    if(node && node->Value())
    {
        mUseBroadcastingFlag = atoi(node->Value()) > 0 ? true : false;
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
        mHostIP.SetAddress(node->Value());
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("MaxPacketSizeBytes").FirstChild().ToNode();
    if(node && node->Value())
    {
        mMaxPayloadSize = (unsigned int)atoi(node->Value());
    }
    /*node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("Logger").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) > 0)
    {
        EnableLogging(true);
    }
    else
    {
        EnableLogging(false);
    }*/
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DisconnectTimeMs").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 0)
    {
        mDisconnectTimeMs = (unsigned int)(atoi(node->Value()));
    }
    element = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("Connection").ToElement();
    while(element)
    {
        if(element->Attribute("ip") && element->Attribute("id"))
        {
            IP4Address ip = element->Attribute("ip");
            Address id = Address::FromString(element->Attribute("id"));
            Mutex::ScopedLock lock(&mConnectionsMutex);
            mPermanentConnections[id] = ip;
        }
        element = element->NextSiblingElement("Connection");
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the transport manager.
///
///   \return False on failure, true on success.
///
////////////////////////////////////////////////////////////////////////////////////
bool TransportManager::Initialize()
{
    bool result = false;
    
    Shutdown();

    mGlobalShutdownFlag = false;
    IP4Address::List hostnames;
    IP4Address::List::iterator eth0;
    CxUtils::Socket::GetHostAddresses(hostnames);
    if(hostnames.size() == 0)
    {
        Mutex::ScopedLock lock(&mDebugMessagesMutex);
        std::cout << "\nTransportManager: No Network Interface Found.\n";
    }

    // Initialize socket.
    for(eth0 = hostnames.begin();
        eth0 != hostnames.end();
        eth0++)
    {
        // Find matching network interface
        if(mHostIP.mString == "0.0.0.0" ||
           mHostIP.mString == "127.0.0.1" ||
           mHostIP.mString.empty() ||
           mHostIP == *eth0)
        {
            mListenSocket.SetNetworkInterface(mHostIP);
            mUdpSocket.SetNetworkInterface(mHostIP);
            
            if(mListenSocket.InitializeSocket(JTCP::Port,   // JAUS Port
                                              255,          // Pending listen queue of 255
                                              0,            // Infinite wait on sending
                                              500,          // 500 ms max wait on receive
                                              1))           // Blocking socket
            {
                if(mUdpSocket.InitializeMulticastSocket(mMulticastIP,           // Multicast IP
                                                        JTCP::Port,             // JAUS Port
                                                        mTimeToLive,            // Multicast TTL
                                                        mUseBroadcastingFlag,   // Turns on true broadcast
                                                        JTCP::Port,             // UDP messages come from JAUS port
                                                        false) == 0)            // Do not allow port re-use
                {
                    std::cout << "JUDP Multicast Discovery will be unvailable.\n";
                }
                
                // Reserved JAUS Port, Yay!
                // Create threads
                mTcpListenThread.CreateThread(TransportManager::TcpListenThread, this);
                mTcpListenThread.SetThreadName("JTCP-Listen");
                mUdpReceiveThread.CreateThread(TransportManager::UdpReceiveThread, this);
                mUdpReceiveThread.SetThreadName("JUDP-RCV");
                mUdpReceiveThread.SetThreadPriority(25);

                mUpdateThread.CreateThread(TransportManager::UpdateThread, this);
                mUpdateThread.SetThreadName("TransportManager::Update");

                result = true;
                // Allow time for threads to start.
                CxUtils::SleepMs(100);
                
            }
            // Bail-OUT
            break;
        }
    }

    if(result == false)
    {
        Shutdown();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the transport manager.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::Shutdown()
{
    mGlobalShutdownFlag = true;

    {
        Mutex::ScopedLock lock(&mConnectionsMutex);

        std::set<JTCP*> tcpToDelete;
        std::map<Address, JTCP*>::iterator tcp;
        for(tcp = mTcpConnections.begin();
            tcp != mTcpConnections.end();
            tcp++)
        {
            tcp->second->Shutdown();
        }
        std::vector<JTCP*>::iterator unknown;
        for(unknown = mUnknownConnections.begin();
            unknown != mUnknownConnections.end();
            unknown++)
        {
            (*unknown)->Shutdown();
        }
    }

    mUdpSocket.Shutdown();

    for(int i = 0; i < 250; i++)
    {
        if(mUdpReceiveThread.IsThreadActive() == false &&
           mTcpListenThread.IsThreadActive() == false &&
           mUpdateThread.IsThreadActive() == false)
        {
            break;
        }
        CxUtils::SleepMs(1);
    }
    mTcpListenThread.StopThread();
    mUdpReceiveThread.StopThread();
    mUpdateThread.StopThread();

    {
        Mutex::ScopedLock lock(&mConnectionsMutex);

        std::map<Address, CxUtils::UdpClient*>::iterator udp;
        for(udp = mUdpConnections.begin();
            udp != mUdpConnections.end();
            udp++)
        {
            udp->second->Shutdown();
        }

        for(udp = mUdpConnections.begin();
            udp != mUdpConnections.end();
            udp++)
        {
            delete udp->second;
        }

        mUdpConnections.clear();
    }
    {
        Mutex::ScopedLock lock(&mConnectionsMutex);

        std::set<JTCP*> tcpToDelete;
        std::map<Address, JTCP*>::iterator tcp;
        for(tcp = mTcpConnections.begin();
            tcp != mTcpConnections.end();
            tcp++)
        {
            //delete tcp->second;
            tcpToDelete.insert(tcp->second);
        }
        std::vector<JTCP*>::iterator unknown;
        for(unknown = mUnknownConnections.begin();
            unknown != mUnknownConnections.end();
            unknown++)
        {
            //delete *unknown;
            tcpToDelete.insert(*unknown);
        }

        mTcpConnections.clear();
        mUnknownConnections.clear();

        std::set<JTCP*>::iterator toDelete;
        for(toDelete = tcpToDelete.begin();
            toDelete != tcpToDelete.end();
            toDelete++)
        {
            delete *toDelete;
        }
    }
    
    mListenSocket.Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if any connections need to be closed out or
///          created.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::CleanupConnections()
{
    if(mGlobalShutdownFlag == true)
    {
        return;
    }
    // Check TCP data.
    {
        Mutex::ScopedLock lock(&mConnectionsMutex);
        // Try sort connections by JAUS ID
        std::vector<JTCP*>::iterator u = mUnknownConnections.begin();
        while(u != mUnknownConnections.end())
        {
            if((*u)->GetLastSourceID().IsValid())
            {
                // Sort and remove from list.
                std::map<Address, JTCP*>::iterator i;
                i = mTcpConnections.find((*u)->GetLastSourceID());
                if(i == mTcpConnections.end())
                {
                    mTcpConnections[(*u)->GetLastSourceID()] = (*u);
                }
                else
                {
                    // Resolve conflicting JAUS ID
                    Time::Stamp updateTimeMs = (*u)->GetUpdateTime().ToMs();
                    if(Time::GetUtcTimeMs() - updateTimeMs > mDelayTimeMs)
                    {
                        // Delete connection and remove.
                        if(mDebugMessagesFlag)
                        {
                            Mutex::ScopedLock lock(&mDebugMessagesMutex);
                            std::cout << "\nTransportManager: Dropping conflicting connection [" << i->first.ToString() << "]\n";
                        }
                        delete i->second;
                        i->second = (*u);
                    }
                }
                if(mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "\nTransportManager: Identified connection [" << (*u)->GetLastSourceID().ToString() << "]\n";
                }
                // Remove from vector.
                u = mUnknownConnections.erase(u);
            }
            else
            {
                u++;
            }
        }

        // Check for dropped/closed out connections.
        std::map<Address, JTCP*>::iterator tcp = mTcpConnections.begin();
        while(tcp != mTcpConnections.end())
        {
            if(tcp->second->GetSocket()->IsValid() == false)
            {
                if(mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "\nTransportManager: Dropping JTCP Connection [" << tcp->first.ToString() << "]\n";
                }

                // This connection may be re-used by multiple
                // sources, so if that is the case, we need to delete
                // all references to it.
                JTCP* toDelete = tcp->second;
                tcp = mTcpConnections.begin();
                while(tcp != mTcpConnections.end())
                {
                    if(tcp->second == toDelete)
                    {
                        mTcpConnections.erase(tcp);
                        tcp = mTcpConnections.begin();
                    }
                    else
                    {
                        tcp++;
                    }
                }
                // Delete the connection.
                delete toDelete;
            }
            else
            {
                tcp++;
            }
        }
    }

    {
        Mutex::ScopedLock lock(&mConnectionsMutex);
        std::map<Address, bool>::iterator perm;
        std::map<Address, Time::Stamp>::iterator utime;

        utime = mUpdateTimes.begin();
        while(utime != mUpdateTimes.end() && mDisconnectTimeMs > 0)
        {
            // If a permanent connection, don't check for closing (skip).
            if(mPermanentConnections.find(utime->first) != mPermanentConnections.end())
            {
                utime++;
                continue;
            }
            Time::Stamp diff = Time::GetUtcTimeMs() - utime->second;
            if(diff >= mDisconnectTimeMs)
            {
                if(mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "\nTransportManager: Connection Timeout to " << utime->first.ToString() << "\n";
                }
                std::map<Address, CxUtils::UdpClient*>::iterator judp;
                judp = mUdpConnections.find(utime->first);
                if(judp != mUdpConnections.end())
                {
                    delete judp->second;
                    mUdpConnections.erase(judp);
                }
                mUpdateTimes.erase(utime);
                utime = mUpdateTimes.begin();
            }
            else
            {
                utime++;
            }
        }
    }

    std::map<Address, IP4Address> perm;
    {
        Mutex::ScopedLock lock(&mConnectionsMutex);
        perm= mPermanentConnections;
    }
    // See if we need to create any connections.
    std::map<Address, IP4Address>::iterator p;
    for(p = perm.begin();
        p != perm.end();
        p++)
    {
        std::map<Address, JTCP*>::iterator tcp;
        std::map<Address, CxUtils::UdpClient*>::iterator udp;

        {
            Mutex::ScopedLock lock(&mConnectionsMutex);
            
            tcp = mTcpConnections.find(p->first);
            if(tcp != mTcpConnections.end())
            {
                continue;
            }
        }
        
        {
            Mutex::ScopedLock lock(&mConnectionsMutex);
            
            udp = mUdpConnections.find(p->first);
            if(udp != mUdpConnections.end())
            {
                continue;
            }
        }
        
        // Try TCP
        if(mUseTcpFlag)
        {
            CxUtils::TcpClient* sock = new CxUtils::TcpClient();
            if(sock->InitializeSocket(p->second, JTCP::Port))
            {
                JTCP* jtcp = new JTCP();
                jtcp->Initialize(this, sock);
                Mutex::ScopedLock lock(&mConnectionsMutex);
                mTcpConnections[p->first] = jtcp;
                continue;
            }
            delete sock;
        }
        else
        // Fall back to UDP on failure or default state.
        {
            Mutex::ScopedLock lock(&mConnectionsMutex);
            CxUtils::UdpClient* sock = mUdpSocket.CreateNewDestination(p->second, JUDP::Port);
            if(sock)
            {
                mUdpConnections[p->first] = sock;
                mUpdateTimes[p->first] = Time::GetUtcTimeMs();
            }
        }
    }

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Process a packet received by a transport medium.  This will
///          route the packet to the appropriate destination.
///
///   \param[in] packet JAUS formatted packket which includes transport
///                     header and general header data.
///   \param[in] header JAUS general header data for message.
///   \param[in] ipAddress IP address of the source of the message.
///   \param[in] sourcePort The source port the data arrived from.
///   \param[in] transport The transport medium the data arrived on.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::ProcessPacket(Packet& packet,
                                     Header& header,
                                     const IP4Address& ipAddress,
                                     const unsigned short sourcePort,
                                     const int transport)
{
    // Check to see if we have a connection to this source.
    CheckForConnection(header.mSourceID, ipAddress, sourcePort, transport);

    // Route the packet by sending it to all destinations.
    if(header.mDestinationID.IsBroadcast() && mGlobalShutdownFlag == false)
    {
        bool fromLocalHost = false;
        if(CxUtils::Socket::IsHostAddress(ipAddress))
        {
            // Message originated from this computer
            fromLocalHost = true;
        }

        // Send to all matching destinations.
        {
            Mutex::ScopedLock lock(&mConnectionsMutex);
            std::map<Address, JTCP*>::iterator tcp;
            std::set<IP4Address> ipBlocksSentTo;
            for(tcp = mTcpConnections.begin(); 
                tcp != mTcpConnections.end() && mGlobalShutdownFlag == false;
                tcp++)
            {
                if(Address::DestinationMatch(header.mDestinationID, tcp->first) &&
                    tcp->first != header.mSourceID)
                {
                    // If the message originated here, blast to everyone
                    // otherwise, only transmit to components on this computer because
                    // it is the originators job to send to everyone else.
                    if(fromLocalHost)
                    {
                        if(tcp->second->IsLocalConnection())
                        {
                            tcp->second->SendTCP(packet);
                        }
                        else
                        {
                            IP4Address addr = tcp->second->GetIP();
                            if(ipBlocksSentTo.find(addr) == ipBlocksSentTo.end())
                            {
                                // We have not sent this message to the IP address listed,
                                // therefore we can send.  This check prevents us from 
                                // constantly blasting a destination IP which will then keep resending
                                // internally, flooding traffic.
                                ipBlocksSentTo.insert(addr);
                                tcp->second->SendTCP(packet);
                            }
                        }
                    }
                    else
                    {
                        // Only send to local connections, not off of this
                        // local computer, otherwise we will be bouncing messages
                        // back.
                        if(tcp->second->IsLocalConnection())
                        {
                            tcp->second->SendTCP(packet);
                        }
                    }
                }
            }
        }
        // Let mutex unlock here so we don't block forever!
        {
            Mutex::ScopedLock lock(&mConnectionsMutex);
            std::map<Address, CxUtils::UdpClient*>::iterator udp;
            for(udp = mUdpConnections.begin();
                    udp != mUdpConnections.end() &&
                    header.mBroadcastFlag == Header::Broadcast::None &&
                    mGlobalShutdownFlag == false;
                udp++)
            {
                if(Address::DestinationMatch(header.mDestinationID, udp->first) &&
                    udp->first != header.mSourceID)
                {
                    udp->second->Send(packet);
                }
            }
        }
        // If the global or local broadcast flags are set and
        // the source IP was this computer, and it came from the
        // TCP transport service, than we can multicast/broadcast out
        if(header.mBroadcastFlag != Header::Broadcast::None &&
           fromLocalHost &&
           transport == TransportManager::TCP)
        {
            mUdpSocket.Send(packet);
        }
    }
    else
    {
        {   // Lookup TCP destination and send.
            Mutex::ScopedLock lock(&mConnectionsMutex);
            std::map<Address, JTCP*>::iterator tcp = mTcpConnections.find(header.mDestinationID);
            if(tcp != mTcpConnections.end())
            {
                if(tcp->second->SendTCP(packet) == 0)
                {
                    if(mDebugMessagesFlag)
                    {
                        Mutex::ScopedLock lock(&mDebugMessagesMutex);
                        std::cout << "\nTransportManager: ERROR Sending Over TCP - [SRC:" << header.mSourceID.ToString() << ", DST:" << header.mDestinationID.ToString() << "] Size: " << header.mSize << "\n";
                        std::cout << "\nTransportManager: Dropping JTCP Connection [" << tcp->first.ToString() << "]\n";
                    }
                    delete tcp->second;
                    mTcpConnections.erase(tcp);
                }
                return;
            }
            else
            {
                if(mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "\nTransportManager: ERROR Sending Over TCP - [SRC:" << header.mSourceID.ToString() << ", DST:" << header.mDestinationID.ToString() << "] Size: " << header.mSize << "\n";
                    std::cout << "\nTransportManager: No TCP Connection Found\n";
                }
            }
        }
        {   // Try UDP destination and send.
            Mutex::ScopedLock lock(&mConnectionsMutex);
            std::map<Address, CxUtils::UdpClient*>::iterator udp = mUdpConnections.find(header.mDestinationID);
            if(udp != mUdpConnections.end())
            {
                if(udp->second->Send(packet) == 0)
                {
                    if(mDebugMessagesFlag)
                    {
                        Mutex::ScopedLock lock(&mDebugMessagesMutex);
                        std::cout << "\nTransportManager: ERROR Sending Over JUDP - [SRC:" << header.mSourceID.ToString() << ", DST:" << header.mDestinationID.ToString() << "] Size: " << header.mSize << "\n";
                    }
                }
                return;
            }
        }
        if(mDebugMessagesFlag)
        {
            Mutex::ScopedLock lock(&mDebugMessagesMutex);
            std::cout << "\nTransportManager: Unknown Destination - [SRC:" << header.mSourceID.ToString() << ", DST:" << header.mDestinationID.ToString() << "] Size: " << header.mSize << "\n";
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds to the list of JAUS and network address to try and create
///          connections to using UDP or TCP.  Method will only have an affect if 
///          this transport manager is active on the current component.
///
///   \param[in] networkIP Network IP address.
///   \param[in] jausID JAUS ID.
///
///   \return True if added to list, false otherwise.  True does not mean the
///           mean the connection has actually been made.
///
////////////////////////////////////////////////////////////////////////////////////
bool TransportManager::AddConnection(const IP4Address& networkIP,
                                     const Address& jausID)
{
    if(jausID.IsValid())
    {
        Mutex::ScopedLock lock(&mConnectionsMutex);
        mPermanentConnections[jausID] = networkIP;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Number of local TCP client connections.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int TransportManager::GetNumLocalClients() const
{
    std::map<Address, JTCP*>::const_iterator c;
    Mutex::ScopedLock lock(&mConnectionsMutex);
    unsigned int count = 0;
    for(c = mTcpConnections.begin();
        c != mTcpConnections.end();
        c++)
    {
        CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>(c->second->GetSocket());
        if(server)
        {
            if(CxUtils::Socket::IsHostAddress(server->GetClientAddress()))
            {
                count++;
            }
        }
    }
    return count;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a connection exits, and if not, creates one.
///
///   \param[in] id JAUS ID.
///   \param[in] sourceAddress IP Address.
///   \param[in] sourcePort Source port where data is coming from.
///   \param[in] transport The transport layer discovered on.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::CheckForConnection(const Address& id,
                                          const IP4Address& sourceAddress,
                                          const unsigned short sourcePort,
                                          const int transport)
{
    if(mGlobalShutdownFlag)
    {
        return;
    }
    bool createConnection = true;

    Mutex::ScopedLock lock(&mConnectionsMutex);
    // See if a UDP connection exists or not.

    mUpdateTimes[id] = Time::GetUtcTimeMs();
    std::map<Address, CxUtils::UdpClient*>::iterator udp;
    udp = mUdpConnections.find(id);
    if(udp != mUdpConnections.end())
    {
        createConnection = false; // Exists.
        // See if the settings changed at all.
        if(udp->second->GetDestinationPort() != sourcePort ||
           udp->second->GetConnectionAddress() != sourceAddress)
        {
            // Delete connection.
            Mutex::ScopedLock lock(&mDebugMessagesMutex);
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";
            std::cout << "JAUS ID DUPLICATED BY " << sourceAddress.mString << std::endl;
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";
            std::cout << "=============================================================\n";

            // Reset flag.
            createConnection = true;
            delete udp->second;
            mUdpConnections.erase(udp);
        }
    }

    // Check if a TCP connection exists already, we do not need to
    // create one if already exists

    if(mUnknownConnections.size() > 0)
    {
        std::vector<JTCP*>::iterator unknown;
        for(unknown = mUnknownConnections.begin();
            unknown != mUnknownConnections.end();
            unknown++)
        {
            IP4Address addr = (*unknown)->GetIP();
            int port = (*unknown)->GetPort();
            if(addr == sourceAddress && port == sourcePort)
            {
                mTcpConnections[id] = (*unknown);
                mUnknownConnections.erase(unknown);
                createConnection = false;
                return;
            }
        }
    }

    std::map<Address, JTCP*>::iterator tcp;

    tcp = mTcpConnections.find(id);
    if(tcp != mTcpConnections.end())
    {
        createConnection = false; // Have a connection.
    }
    /*
    else if(reuseFlag)
    {
        for(tcp = mTcpConnections.begin();
            tcp != mTcpConnections.end();
            tcp++)
        {
            IP4Address addr = tcp->second->GetIP();
            if(addr == sourceAddress)
            {
                // Connection already exists to the same IP address.
                // Re-use it.
                createConnection = false;
                JTCP* sock = tcp->second;
                mTcpConnections[id] = sock;
                break;
            }
        }
    }
    */
    if(createConnection && mGlobalShutdownFlag == false)
    {
        // If we do not have a connection to this source yet
        // make either a TCP or UDP client.  Prioritize to
        // TCP if specified in settings.
        if(mUseTcpFlag)
        {
            // Try create a connection.
            CxUtils::TcpClient* sock = new CxUtils::TcpClient();
            if(sock->InitializeSocket(sourceAddress, JTCP::Port) ||
               (transport == TransportManager::UDP && sock->InitializeSocket(sourceAddress, sourcePort)) )
            {
                JTCP* jtcp = new JTCP();
                jtcp->Initialize(this, sock);
                mTcpConnections[id] = jtcp;
                createConnection = false;
            }
            else
            {
                delete sock;
                sock = NULL;
            }
        }
        // If we could not create a TCP connection, make a UDP connection.
        if(createConnection)
        {
            // Create a UDP connection.
            CxUtils::UdpClient* sock = mUdpSocket.CreateNewDestination(sourceAddress, sourcePort);
            if(sock)
            {
                mUdpConnections[id] = sock;
            }
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief TCP Listening thread which creates connections to TCP clients.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::TcpListenThread(void* args)
{
    TransportManager* manager = (TransportManager*)args;
    CxUtils::TcpServer* sock = NULL;
    while(manager && manager->mTcpListenThread.QuitThreadFlag() == false && manager->mGlobalShutdownFlag == false)
    {
        sock = NULL;

        // This will block until a connection is made.
        sock = manager->mListenSocket.AwaitConnection();

        if(sock && manager->mGlobalShutdownFlag == false)
        {
            // Create a thread to process the data.
            JTCP* tcp = new JTCP();
            tcp->SetPacketPollingDelayMs(manager->mDelayTimeMs);
            if(tcp->Initialize(manager, sock))
            {
                if(manager->mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&manager->mDebugMessagesMutex);
                    std::cout << "\nTransportManager: New  incomming JTCP connection made.\n";
                }
                {
                    Mutex::ScopedLock lock(&manager->mConnectionsMutex);
                    // Add to the unknown connections since we do
                    // not yet know what JAUS ID is associated with
                    // this connection.
                    manager->mUnknownConnections.push_back(tcp);
                }
                // Updates connections.
                manager->CleanupConnections();
            }
            else
            {
                delete tcp;
                tcp = NULL;
            }
        }
        CxUtils::SleepMs(1);
    }

    if(sock)
    {
        delete sock;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief UDP Receiving Thread.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::UdpReceiveThread(void* args)
{
    TransportManager* manager = (TransportManager*)args;
    Packet udpMessage;
    CxUtils::IP4Address sourceAddress;
    unsigned short sourcePort = 0;
    long int timeoutMs = 100;

#ifdef WIN32
    int loopCounter = 0;
#endif

    udpMessage.Reserve(5000);

    while(manager && manager->mUdpReceiveThread.QuitThreadFlag() == false && manager->mGlobalShutdownFlag == false)
    {
        udpMessage.Clear(false);
        sourcePort = 0;

        if(manager->mUdpSocket.Recv(udpMessage, 
                                    5000, 
                                    timeoutMs, 
                                    &sourceAddress, 
                                    &sourcePort) > 0 && manager->mGlobalShutdownFlag == false)
        {

            // Check for JUDP Version header.
            if(CxUtils::Socket::IsHostAddress(sourceAddress) == false &&
               udpMessage.Length() > Header::MinSize + BYTE_SIZE && *udpMessage.Ptr() == JUDP::Version)
            {
                // Strip the JUDP Version header using a wrapper packet (more efficient).
                Packet::Wrapper stripped((unsigned char *)(udpMessage.Ptr() + 1), udpMessage.Length() - 1);
                Header header;
                stripped->SetReadPos(0);
                std::string errorMessage;
                if(header.Read(*stripped.GetData()) && header.IsValid(&errorMessage))
                {
                    manager->ProcessPacket(udpMessage, header, sourceAddress, sourcePort, TransportManager::UDP);
                }
            }
        }

        if(manager->mDelayTimeMs == 0)
        {
#ifdef WIN32
            // Only sleep every N loops
            if( loopCounter++ == 250)
            {
                loopCounter = 0;
                CxUtils::SleepMs(1);
            }
#else
            usleep(500);
#endif
        }
        else
        {
            CxUtils::SleepMs(manager->mDelayTimeMs);
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief UDP Receiving Thread.
///
////////////////////////////////////////////////////////////////////////////////////
void TransportManager::UpdateThread(void* args)
{
    TransportManager* manager = (TransportManager*)args;
    while(manager && manager->mUpdateThread.QuitThreadFlag() == false && manager->mGlobalShutdownFlag == false)
    {
        manager->CleanupConnections();
        CxUtils::SleepMs(100);
    }
}


/*  End of File */
