////////////////////////////////////////////////////////////////////////////////////
///
///  \file judp.cpp
///  \brief Contains the definition for the JUDP Transport Service for JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 17 October 2009
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
#include "jaus/core/transport/judp.h"
#include "jaus/core/management/management.h"
#include "jaus/core/component.h"
#include <iostream>
#include <tinyxml/tinyxml.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
JUDP::JUDP()
{
    mDisconnectTimeMs = 5000;
    mMulticastIP.SetAddress("239.255.0.1");
    mTimeToLive = 16;
    mTransportHeader.Write(Version);
    mMaxPayloadSize = 1500 - OverheadSizeBytes - USHORT_SIZE;
    mDelayTimeMs = 0;
    mUseBroadcastingFlag = false;
    mInternalDiscoveryPort = 49152;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
JUDP::~JUDP()
{
    Shutdown();
    mPrimaryThread.StopThread();
    mSecondaryThread.StopThread();
    mInput.Shutdown();
    mMulticast.Shutdown();
    mPrimaryThreadCreatedFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the delay time between checks for new packets to process by
///          each message processing thread.  
///
///   The default value is 1 ms, but can be set to 0 for no delay, or larger
///   for less frequent checking of messages (use larger numbers on slower 
///   hardware).
///
///   \param[in] delayTimeMs Delay time between checks in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetPacketPollingDelayMs(const unsigned int delayTimeMs)
{
    if(mPrimaryThread.IsThreadActive() == false)
    {
        mDelayTimeMs = delayTimeMs;
    }
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
bool JUDP::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);
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
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("Logger").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) > 0)
    {
        EnableLogging(true);
    }
    else
    {
        EnableLogging(false);
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DisconnectTimeMs").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 0)
    {
        SetDisconnectTimeMs(atoi(node->Value()));
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Transport Service based on configuration values
///          set.
///
///   \param[in] componentID ID of the component using the Transport Service.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::Initialize(const Address& componentID)
{
    if(componentID.IsValid() == false || componentID.IsBroadcast())
    {
        return false;
    }
    
    Shutdown();
    
    // Save ID.
    mComponentID = componentID;
    CxUtils::IP4Address::List hostnames;
    CxUtils::IP4Address::List::iterator eth0;
    CxUtils::Socket::GetHostAddresses(hostnames);
    if(hostnames.size() == 0 && mDebugMessagesFlag)
    {
        Mutex::ScopedLock lock(&mDebugMessagesMutex);
        std::cout << "[JUDP-" << mComponentID.ToString() << "] - No Network Interface Found.\n";
    }
    unsigned int count = 0;
    for(eth0 = hostnames.begin();
        eth0 != hostnames.end();
        eth0++, count++)
    {
        // Use any or specified interface.
        if(mHostIP.mString == "0.0.0.0" ||
           mHostIP.mString == "127.0.0.1" ||
           mHostIP.mString.empty() ||
           mHostIP == *eth0)
        {
            // Initialize UDP receiving.
            if(mInput.InitializeMulticastSocket(Port, mMulticastIP, true))
            {
                mMulticast.SetNetworkInterface(*eth0);
                if(mMulticast.InitializeMulticastSocket(mMulticastIP, Port, mTimeToLive, mUseBroadcastingFlag, 0))
                {
                    SetComponentID(componentID);
                    
                    mPrimaryThread.CreateThread(JUDP::ReceiveThread, this);
                    CxUtils::SleepMs(250);
                    mSecondaryThread.CreateThread(JUDP::ReceiveThread, this);
                    // Set thread names.
                    mPrimaryThread.SetThreadName(std::string("JUDP 1 ") + componentID.ToString());
                    mSecondaryThread.SetThreadName(std::string("JUDP 2 ") + componentID.ToString());
                    return true;
                }
            }
            /*
            mPrimarySocket.SetNetworkInterface(mHostIP);
            mSecondarySocket.SetNetworkInterface(mHostIP);
            // Try initialize on 3794 first, but accept any available.
            if(mPrimarySocket.InitializeMulticastSocket(mMulticastIP, Port, mTimeToLive, mUseBroadcastingFlag, Port, true) &&
               mSecondarySocket.InitializeMulticastSocket(mMulticastIP, Port, mTimeToLive, mUseBroadcastingFlag, 0, false))
            {
                    SetComponentID(componentID);
                    mPrimaryThread.CreateThread(JUDP::PrimaryThread, this);
                    std::string threadName = std::string("JUDP 1 ") + componentID.ToString();
                    mPrimaryThread.SetThreadName(threadName);

                    // Have a thread to discover new UDP clients that are transmitting.
                    mSecondaryThread.CreateThread(JUDP::SecondaryThread, this);
                    threadName = std::string("JUDP 2 ") + componentID.ToString();
                    mSecondaryThread.SetThreadName(threadName);

                    return true;
            }
            */
        }
    }
    
    Shutdown();

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if Transport is initialized, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::IsInitialized() const
{
    return mInput.IsValid();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the Transport.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::Shutdown()
{
    {
        Mutex::ScopedLock lock(&mClientsMutex);
        std::map<Address, CxUtils::UdpClient*>::iterator client;
        for(client = mClients.begin();
            client != mClients.end();
            client++)
        {
            client->second->Shutdown();
            delete client->second;
        }
        mClients.clear();
    }
    /*
    for(unsigned int i = 0; i < 500; i++)
    {
        if(mRecvThread.IsThreadActive() == false)
        {
            break;
        }
        CxUtils::SleepMs(1);
    }

    // Make sure threads are stopped.
    mRecvThread.KillThread();
    */
    for(unsigned int i = 0; i < 500; i++)
    {
        if(mPrimaryThread.IsThreadActive() == false &&
           mSecondaryThread.IsThreadActive() == false)
           {
               break;
           }
           CxUtils::SleepMs(1);
    }
    
    // Make sure threads are stopped.
    mPrimaryThread.KillThread();
    mSecondaryThread.KillThread();
    
    mInput.Shutdown();
    mMulticast.Shutdown();

    mComponentID.Clear();
    mPrimaryThreadCreatedFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a list of connections to other components.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List JUDP::GetConnections() const
{
    Mutex::ScopedLock lock(&mClientsMutex);
    Address::List results;
    std::map<Address, CxUtils::UdpClient *>::const_iterator client;
    for(client = mClients.begin();
        client != mClients.end();
        client++)
    {
        results.push_back(client->first);
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param[in] id Component ID to check connection for.
///
///   \return True if a connection to the component exists.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::HaveConnection(const Address& id) const
{
    Mutex::ScopedLock lock(&mClientsMutex);
    Address::List results;
    std::map<Address, CxUtils::UdpClient *>::const_iterator client;
    client = mClients.find(id);
    if(client != mClients.end())
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a serialized JAUS packet that has the correct JUDP header
///          setup.  If the destination is a local broadcast, then it is sent
///          using multicast, if destination is global, then it is sent using
///          a broadcast socket.
///
///   \param[in] packet JUDP Packet to send.
///   \param[in] header JAUS Transport header information.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g. 
///                             multicast). 0 = no options, 1/2 = multicast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::SendPacket(const Packet& packet, 
                      const Header& header,
                      const int broadcastFlags) const
{
    bool result = false;
    
    if(broadcastFlags != Service::NoBroadcast)
    {
        result = mMulticast.Send(packet) > 0;

        if(result == false && mDebugMessagesFlag)
        {
            Mutex::ScopedLock lock(&mDebugMessagesMutex);
            std::cout << "[JUDP-" << mComponentID.ToString() << "] - Failed to Broadcast Message to " << header.mDestinationID.ToString() << "\n";
        }
        if(header.mDestinationID.IsBroadcast() == false)
        {
            return result;
        }
    }

    if(header.mDestinationID.IsBroadcast())
    {
        // Send to all destination matches.
        result = true;
        std::map<Address, CxUtils::UdpClient*>::const_iterator client;
        Mutex::ScopedLock lock(&mClientsMutex);
        for(client = mClients.begin(); client != mClients.end(); client++)
        {
            if(Address::DestinationMatch(header.mDestinationID, client->first))
            {
                result &= client->second->Send(packet) > 0;
            }
        }

        if(result == false && mDebugMessagesFlag)
        {
            Mutex::ScopedLock plock(&mDebugMessagesMutex);
            std::cout << "[JUDP-" << mComponentID.ToString() << "] - Failed to Send Unicast Message to " << header.mDestinationID.ToString() << "\n";
        }
    }
    else
    {
        // Lookup destination and send it.
        std::map<Address, CxUtils::UdpClient*>::const_iterator client;
        Mutex::ScopedLock lock(&mClientsMutex);

        client = mClients.find(header.mDestinationID);
        if(client != mClients.end())
        {
            result = client->second->Send(packet) > 0;
        }
        if(result == false && mDebugMessagesFlag)
        {
            Mutex::ScopedLock plock(&mDebugMessagesMutex);
            std::cout << "[JUDP-" << mComponentID.ToString() << "] - Failed to Send Unicast Message to " << header.mDestinationID.ToString() << "\n";
        }
    }   
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the message data into JUDP packets for transmission.
///
///   \param[in] message Message to convert/serialize.
///   \param[out] stream Packets to send.
///   \param[out] streamHeaders Header info for packets.
///   \param[in] startingSequenceNumber Sequence number to use for packets.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g. 
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::SerializeMessage(const Message* message,
                            Packet::List& stream,
                            Header::List& streamHeaders,
                            const UShort startingSequenceNumber,
                            const int broadcastFlags) const
{
    Packet packet;
    Header header;

    // Pre-allocate memory.
    packet.Reserve(mMaxPayloadSize + OverheadSizeBytes);

    // Clear stream/headers.
    stream.clear();
    streamHeaders.clear();

    // If the message is a large data set, create a multi-packet stream.
    if(message->IsLargeDataSet(mMaxPayloadSize))
    {
        return message->WriteLargeDataSet(stream, 
                                          streamHeaders, 
                                          mMaxPayloadSize, 
                                          &mTransportHeader,
                                          startingSequenceNumber) > 0;
    }
    // Single packet.
    else if(message->Write(packet, header, &mTransportHeader, true, startingSequenceNumber) > 0)
    {
        stream.push_back(packet);
        streamHeaders.push_back(header);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return List of manually created connections to JAUS components.  These
///           are connections not made through the discovery process and cannot
///           be closed through discovery either (only manually).
///
////////////////////////////////////////////////////////////////////////////////////
Address::List JUDP::GetManualConnections() const
{
    Address::List copy;
    {
        Mutex::ScopedLock lock(&mClientsMutex);
        std::map<Address, bool>::const_iterator p;
        for(p = mPermanentConnections.begin();
            p != mPermanentConnections.end();
            p++)
        {
            copy.push_back(p->first);
        }
    }
    return copy;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the maximum packet size value to use for UDP packets. This
///          value should match the MTU of your network interface.  The
///          default value is 1500.
///
///   \param[in] maxSizeBytes The maximum packet size for sending over
///                           UDP.  This should match the MTU of your network
///                           interface.  Must be greater than OverheadSizeBytes.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetMaxPacketSize(const unsigned int maxSizeBytes)
{
    if(IsInitialized() == false && maxSizeBytes > (OverheadSizeBytes + USHORT_SIZE))
    {
        mMaxPayloadSize = maxSizeBytes - OverheadSizeBytes - USHORT_SIZE;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the time in milliseconds used to determine if a connection
///          should be closed. Only closes connection if no data has been received
///          for this time value set.
///
///   \param[in] timeMs If a connection has not updated in this amount of time
///                     it is closed.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetDisconnectTimeMs(const unsigned int timeMs)
{
    if(IsInitialized() == false)
    {
        mDisconnectTimeMs = timeMs;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the TTL for multicast.  Only set if 
///          Transport is not initialized.
///
///   \param[in] ttl Time To Live (TTL).  Default value is 1, meaning the data only
///              goes as far as the local network, increase to go further out.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetTTL(const unsigned char ttl)
{
    if(IsInitialized() == false && ttl > 0)
    {
        mTimeToLive = ttl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the address to use for multicast data.  Only set if 
///          Transport is not initialized.
///
///   \param[in] multicastIP The multicast group to connect to.  This is an IP 
///              address in the range of "224.0.0.0-239.255.255.255"
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetMulticastIP(const CxUtils::IP4Address& multicastIP)
{
    if(IsInitialized() == false)
    {
        mMulticastIP = multicastIP;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Enable/Disable broadcasting over multicast.  Only set if 
///          Transport is not initialized.
///
///   \param[in] enabled If true, broadcasting is turned on.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::EnableBroadcasting(const bool enabled)
{
    mUseBroadcastingFlag = enabled;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the IP address of the network interface on the host machine to
///          use.  Set to blank if any address is fine.
///
///   \param[in] networkIP The broadcast group to connect to.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::SetInterfaceIP(const CxUtils::IP4Address& networkIP)
{
    if(IsInitialized() == false)
    {
        mHostIP = networkIP;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to add a new connection manually to a component at an
///          IP address.
///
///   \param[in] networkIP IP Address of component.
///   \param[in] jausID JAUS ID of component.
///   \param[in] port Network port to use (default is 3794).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::AddConnection(const CxUtils::IP4Address& networkIP,
                         const Address& jausID,
                         const unsigned short port)
{
    bool result = false;

    if(jausID.IsValid() && !jausID.IsBroadcast())
    {
        Mutex::ScopedLock lock(&mClientsMutex);
        std::map<Address, CxUtils::UdpClient*>::iterator citer;
        citer = mClients.find(jausID);
        if(citer == mClients.end())
        {
            CxUtils::UdpClient* udp = mMulticast.CreateNewDestination(networkIP, port);
            if(udp)
            {
                mClients[jausID] = udp;
                mUpdateTimes[jausID] = Time::GetUtcTimeMs();
                mPermanentConnections[jausID] = true;
                udp = NULL;
                result = true;
            }
            else
            {
                if(mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - New Connection Made to " << jausID << " at IP:" << networkIP.mString << std::endl;
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to disconnect from a component.
///
///   \param[in] jausID JAUS ID of component.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::CloseConnection(const Address& jausID)
{
    bool result = false;
    std::map<Address, Time::Stamp>::iterator times;
    std::map<Address, CxUtils::UdpClient*>::iterator client;

    // Go through all client connections we have, and 
    // delete any connections that have not updated.
    Mutex::ScopedLock lock(&mClientsMutex);
    times = mUpdateTimes.find(jausID);
    client = mClients.find(jausID);
    if(client != mClients.end())
    {
        if(client != mClients.end())
        {
            delete client->second;
            mClients.erase(client);
        }
        if(times != mUpdateTimes.end())
        {
            mUpdateTimes.erase(times);
        }
        if(mPermanentConnections.find(jausID) != mPermanentConnections.end())
        {
            mPermanentConnections.erase(mPermanentConnections.find(jausID));
        }
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a multicast listening socket to try and discover
///          subsystems on the network for dynamic selection of a component
///          ID.
///
///   \param[in] discovered List of discovered components performing Discovery.
///   \param[in] waitTimeMs How long to listen for.
///   \param[in] multicastGroup Multicast group to listen on.
///
///   \return True if was able to listen, false on failure to listen.
///
////////////////////////////////////////////////////////////////////////////////////
bool JUDP::ListenForSubsystems(Address::Set& discovered,
                               const unsigned int waitTimeMs,
                               const CxUtils::IP4Address& multicastGroup)
{
    discovered.clear();

    CxUtils::UdpSharedServer listen;
    if(listen.InitializeSocket(JUDP::Port, multicastGroup))
    {
        Packet packet;
        Header header;
        UShort messageCode;
        CxUtils::IP4Address ip;
        Time::Stamp start = Time::GetUtcTimeMs();
        while(Time::GetUtcTimeMs() - start < waitTimeMs)
        {
            if(listen.Recv(packet, ip))
            {
                packet.SetReadPos(1);
                if(header.Read(packet) &&
                   packet.Read(messageCode))
                {
                    discovered.insert(header.mSourceID);
                }
            }
        }
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if any connections need to be closed out.
///
///   \param[in] timeSinceLastCheckMs How many milliseconds that have past since
///              the last time the CheckServiceStatus method was called.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    if(mDisconnectTimeMs > 0)
    {
        std::map<Address, Time::Stamp>::iterator times;
        std::map<Address, CxUtils::UdpClient*>::iterator client;

        // Go through all client connections we have, and 
        // delete any connections that have not updated.
        Mutex::ScopedLock lock(&mClientsMutex);
        Time::Stamp currentTimeMs = Time::GetUtcTimeMs();
        times = mUpdateTimes.begin();
        while(times != mUpdateTimes.end())
        {
            std::map<Address, bool>::iterator perm = mPermanentConnections.find(times->first);
            if(perm == mPermanentConnections.end() && 
               currentTimeMs - times->second >= mDisconnectTimeMs)
            {
                client = mClients.find(times->first);
                if(client != mClients.end())
                {
                    delete client->second;
                    mClients.erase(client);
                }
                mUpdateTimes.erase(times);
                times = mUpdateTimes.begin();
            }
            else
            {
                times++;
            }
        }
    }

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called whenever a UDP packet is received by the
///          service.
///
///   \param[in] packet JAUS UDP Data to process.
///   \param[in] ipAddress IP Address of source of message.
///   \param[in] sourcePort The source port the data was received on.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::ProcessUDP(const Packet& packet, 
                      const CxUtils::IP4Address& ipAddress,
                      const unsigned short sourcePort)
{
    // Check for JUDP Version header.
    if(packet.Length() > Header::MinSize + mTransportHeader.Length() && *packet.Ptr() == Version)
    {
        // Strip the JUDP Version header using a wrapper packet (more efficient).
        Packet::Wrapper stripped((unsigned char *)(packet.Ptr() + 1), packet.Length() - 1);
        Header header;
        stripped->SetReadPos(0);
        std::string errorMessage;
        if(header.Read(*stripped.GetData()) && header.IsValid(&errorMessage))
        {
            // Don't process messages from ourselves. This happens
            // when a message is sent via Multicast/Broadcast or to another
            // component on the same computer via UDP.
            if(header.mSourceID == mComponentID)
            {
                if(sourcePort != mMulticast.GetSourcePort() && sourcePort != Port)
                {
                    Mutex::ScopedLock lock(&mDebugMessagesMutex);
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                    std::cout << "JAUS ID DUPLICATED BY " << ipAddress.mString << std::endl;
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                    std::cout << "=============================================================\n";
                }
                return;
            }
            
            // Update receive times for clients, and make new 
            // connections if needed.
            UpdateClientConnections(header.mSourceID,
                                    ipAddress,
                                    sourcePort);

            // Proces the packet data.
            ProcessPacket(*stripped.GetData(), header);
            /*
            // If this component has reserved the primary JAUS port.
            if(udpSocket->GetSourcePort() == Port)
            {
                UShort messageCode = 0;
                stripped->Read(messageCode, Header::PayloadOffset);
                // Check for a discovery related message.
                if(messageCode == QUERY_IDENTIFICATION)
                {
                    // Share this message to other components
                    // on this host machine so that they can discover
                    // new connections too.
                    if(mInternalDiscoverySocket.IsValid() == false)
                    {
                        // Set TTL to 0 so we do not transmit outside the host machine.
                        mInternalDiscoverySocket.InitializeMulticastSocket(mMulticastIP, 
                                                                           mInternalDiscoveryPort,
                                                                           0,
                                                                           false,
                                                                           0,
                                                                           true);
                    }
                    Packet share;
                    share.Write(GetComponentID().ToUInt());
                    share.Write(ipAddress.mData, 4);
                    share.Write(sourcePort);
                    share.Write(*stripped.GetData());
                    mInternalDiscoverySocket.Send(share);
                }
            }
            */
            if(header.mSize < stripped->Size())
            {
                // Multiple packets are included, we must extract them also.
                unsigned int offset = header.mSize;
                while(offset < stripped->Size())
                {
                    if(mDebugMessagesFlag)
                    {
                        Mutex::ScopedLock lock(&mDebugMessagesMutex);
                        std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Multi-Message Packet\n";
                    }
                    Packet::Wrapper child((unsigned char *)(stripped->Ptr()), stripped->Size() + offset);
                    if(header.Read(*child.GetData()) && header.IsValid(&errorMessage))
                    {
                        ProcessPacket(*child.GetData(), header);
                        offset += header.mSize;
                    }
                    else
                    {
                        break; // Can't read anymore data.
                    }
                }
            }
        }
        else if(mDebugMessagesFlag)
        {
            Mutex::ScopedLock lock(&mDebugMessagesMutex);
            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Bad JAUS Header [" << errorMessage << "]\n";
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to update client connections or create new ones.
///
///   \param[in] id The ID of the JAUS component to update connections for.
///   \param[in] ipAddress IP Address of source of message.
///   \param[in] sourcePort The source port the data was received on.
///
///   \return 1 if a new connection was made, 0 otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
int JUDP::UpdateClientConnections(const Address& id,
                                  const CxUtils::IP4Address& ipAddress,
                                  const unsigned short sourcePort)
{
    int result = 0;

    std::map<Address, Time::Stamp>::iterator times;
    std::map<Address, CxUtils::UdpClient*>::iterator client;
    mClientsMutex.Lock();
    client = mClients.find(id);
    if(client == mClients.end())
    {
        // Create connection to the socket, but using the socket that received
        // the message.  This is done so we are sending from the same port that we
        // received on.
        CxUtils::UdpClient* udp = mMulticast.CreateNewDestination(ipAddress, sourcePort);
        if(udp)
        {
            mClients[id] = udp;
            mUpdateTimes[id] = Time::GetUtcTimeMs();
            udp = NULL;
            result = 1;
        }
        else
        {
            if(mDebugMessagesFlag)
            {
                Mutex::ScopedLock lock(&mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - New Connection Made to " << id.ToString() << " at IP:" << ipAddress.mString << std::endl;
            }
        }
    }
    else
    {   
        times = mUpdateTimes.find(id);
        // Check to see if the IP address has changed from this
        // source, but only check if we haven't received data from
        // this source for more than a second or so.
        if(ipAddress != client->second->GetConnectionAddress())
        {
            // Re-create connection.
            CxUtils::UdpClient* newClient = mMulticast.CreateNewDestination(ipAddress, sourcePort);
            CxUtils::UdpClient* oldClient = client->second;
            client->second = newClient;
            delete oldClient;
        }
        // Check that I am sending to the same port they are sending from.
        else if(sourcePort != client->second->GetDestinationPort())
        {
            // Re-create connection.
            CxUtils::UdpClient* newClient = mMulticast.CreateNewDestination(ipAddress, sourcePort);
            CxUtils::UdpClient* oldClient = client->second;
            client->second = newClient;
            delete oldClient;
        }
        // Update receive time so we don't close this
        // connection.
        times->second = Time::GetUtcTimeMs();
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method for receiving UDP messages in a continuous loop.
///
////////////////////////////////////////////////////////////////////////////////////
void JUDP::ReceiveThread(void* args)
{
    JUDP* transport = (JUDP*)args;
    Packet udpMessage;
    CxUtils::IP4Address sourceAddress;
    unsigned short sourcePort = 0;
    long int timeoutMs = 100;
    
#ifdef WIN32
    int loopCounter = 0;
#endif

    udpMessage.Reserve(5000);
    Thread* thread = NULL;
    bool primary = false;
    
    if(transport->mPrimaryThreadCreatedFlag == false)
    {
        primary = true;
        transport->mPrimaryThreadCreatedFlag = true;
        thread = &transport->mPrimaryThread;
    }
    else
    {
        thread = &transport->mSecondaryThread;
    }

    while(transport && 
          thread->QuitThreadFlag() == false &&
          transport->mShutdownServiceFlag == false)
    {
        udpMessage.Clear(false);
        sourcePort = 0;
        if(primary)
        {
            if(transport->mInput.Recv(udpMessage, 
                                      5000, 
                                      timeoutMs, 
                                      &sourceAddress, 
                                      &sourcePort) > 0)
            {
                transport->ProcessUDP(udpMessage, sourceAddress, sourcePort);
            }
        }
        else
        {
            if(transport->mMulticast.GetSourcePort() > 0 &&
               transport->mMulticast.Recv(udpMessage, 
                                          5000, 
                                          timeoutMs, 
                                          &sourceAddress, 
                                          &sourcePort) > 0)
            {
                transport->ProcessUDP(udpMessage, sourceAddress, sourcePort);
            }
        }
        
        if(transport->mShutdownServiceFlag)
        {
            break;
        }

        if(transport->mDelayTimeMs == 0)
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
            CxUtils::SleepMs(transport->mDelayTimeMs);
        }
    }
    
    //std::cout << "EXIT!\n";
}


/*  End of File */
