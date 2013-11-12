////////////////////////////////////////////////////////////////////////////////////
///
///  \file jtcpclient.cpp
///  \brief Contains the definition for the JTCPClient Transport Service for JAUS++.  
///         The JTCPClient client is used to create connections to a TCP server
///         supporting JAUS.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2 October 2010
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
#include "jaus/core/transport/jtcpclient.h"
#include "jaus/core/discovery/queryidentification.h"
#include "jaus/core/transport/transportmanager.h"
#include <tinyxml/tinyxml.h>
#include <iostream>
#include <iomanip>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
JTCPClient::JTCPClient()
{
    mpTransportManager = new TransportManager();
    mpSocket = new CxUtils::TcpClient();
    mTransportHeader.Write(Version);
    mMaxPayloadSize = 1500 - JTCP::OverheadSizeBytes - USHORT_SIZE;
    mDisconnectTimeMs = 3600000;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
JTCPClient::~JTCPClient()
{
    Shutdown();
    if(mpTransportManager)
    {
        delete mpTransportManager;
    }
    mpTransportManager = NULL;

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
bool JTCPClient::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);
    TiXmlNode* node;

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("NetAddress").FirstChild().ToNode();
    if(node && node->Value())
    {
        mHostIP.SetAddress(node->Value());
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
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("MaxPacketSizeBytes").FirstChild().ToNode();
    if(node && node->Value())
    {
        mMaxPayloadSize = (unsigned int)atoi(node->Value());
    }

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DisconnectTimeMs").FirstChild().ToNode();
    if(node && node->Value())
    {
        mDisconnectTimeMs = (Time::Stamp)atoi(node->Value());
    }

    mpTransportManager->LoadSettings(filename);

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
bool JTCPClient::Initialize(const Address& componentID)
{
    return Initialize(componentID, "127.0.0.1");
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Transport Service based on configuration values
///          set.
///
///   \param[in] componentID ID of the component using the Transport Service.
///   \param[in] host IP of host sever to connect to (this is the localhost
///                   or another node/server on the network.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCPClient::Initialize(const Address& componentID,
                            const IP4Address& host)
{
    bool result = false;

    JTCPClient::Shutdown();

    if(mpTransportManager)
    {
        mpTransportManager->Initialize();
    }

    mpSocket = new CxUtils::TcpClient();

    if(componentID.IsValid() == false || componentID.IsBroadcast())
    {
        Mutex::ScopedLock lock(&mDebugMessagesMutex);
        std::cout << "JTCP- Invalid component ID [" << componentID.ToString() << "]\n";
        return false;
    }
    mComponentID = componentID;
    IP4Address::List hostnames;
    IP4Address::List::iterator eth0;
    CxUtils::Socket::GetHostAddresses(hostnames);
    if(hostnames.size() == 0)
    {
        Mutex::ScopedLock lock(&mDebugMessagesMutex);
        std::cout << "[JTCP-" << mComponentID.ToString() << "] - No Network Interface Found.\n";
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
            mpSocket->SetNetworkInterface(*eth0);
            for(int i = 0; i < 5; i++)
            {
                if(((CxUtils::TcpClient *)mpSocket)->InitializeSocket(host, Port, 0, 1000))
                {
                    // Connected to TCP Server (yay!);
                    SetComponentID(componentID);
                    std::string threadName = std::string("JTCP ") + componentID.ToString();
                    mRecvThread.SetThreadName(threadName);
                    mRecvThread.CreateThread(ReceiveThread, (JTCP*)this);
                    CxUtils::SleepMs(50);
                    // Send a few messages to server to make sure we
                    // do not get shutdown for being a duplicated JAUD ID
                    // on the network
                    JAUS::QueryIdentification queryIdentification(Address(Address::GlobalBroadcast, 255, 255),
                                                                  componentID);
                    for(unsigned int i = 0; i < 3; i++)
                    {
                        Send(&queryIdentification);
                        CxUtils::SleepMs(25);
                        if(mpSocket->IsValid() == false || mReceivedValidDataFlag == true)
                        {
                            break;
                        }
                    }

                    if(mpSocket->IsValid())
                    {
                        result = true;
                        CxUtils::SleepMs(100);
                    }
                    else
                    {
                        std::cout << "[JTCP-" << componentID.ToString() << "] - Component ID already in use.\n";
                    }
                }
                CxUtils::SleepMs(10);
            }
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
///   \return True if Transport is initialized, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCPClient::IsInitialized() const
{
    return mpSocket ? mpSocket->IsValid() : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the Transport.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::Shutdown()
{
    mpTransportManager->SignalShutdown(true);

    JTCP::Shutdown();

    if(mpTransportManager)
    {
        mpTransportManager->Shutdown();
    }

    mReceivedValidDataFlag = false;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the maximum packet size value to use for TCP packets. This
///          value should match the MTU of your network interface.  The
///          default value is 1500.
///
///   \param[in] maxSizeBytes The maximum packet size for sending over
///                           TCP.  This should match the MTU of your network
///                           interface.  Must be greater than OverheadSizeBytes.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::SetMaxPacketSize(const unsigned int maxSizeBytes)
{
    if(IsInitialized() == false && maxSizeBytes > (JTCP::OverheadSizeBytes + USHORT_SIZE))
    {
        mMaxPayloadSize = maxSizeBytes - OverheadSizeBytes - USHORT_SIZE;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a list of connections to other components.
///
////////////////////////////////////////////////////////////////////////////////////
Address::List JTCPClient::GetConnections() const
{
    Mutex::ScopedLock lock(&mDataMutex);
    Address::List results;
    std::map<Address, Time::Stamp>::const_iterator client;
    for(client = mUpdateTimes.begin();
        client != mUpdateTimes.end();
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
bool JTCPClient::HaveConnection(const Address& id) const
{
    return HaveConnection(id, (unsigned int)mDisconnectTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param[in] id Component ID to check connection for.
///   \param[in] timeSinceUpdateMs The time in ms, since a message was
///                                was last received.  If no message received
///                                within this timeframe, method will return false.
///
///   \return True if a connection to the component exists.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCPClient::HaveConnection(const Address& id,
                                const unsigned int timeSinceUpdateMs) const
{
    Mutex::ScopedLock lock(&mDataMutex);
    std::map<Address, Time::Stamp>::const_iterator client;
    client = mUpdateTimes.find(id);
    if(client != mUpdateTimes.end())
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a serialized JAUS packet that has the correct JTCP header
///          setup.  If the destination is a local broadcast, then it is sent
///          using multicast, if destination is global, then it is sent using
///          a broadcast socket.
///
///   \param[in] packet JTCP Packet to send.
///   \param[in] header JAUS Transport header information.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g. 
///                             multicast). 0 = no options, 1/2 = multicast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCPClient::SendPacket(const Packet& packet, 
                            const Header& header,
                            const int broadcastFlags) const
{
    bool result = false;

    if(mpSocket && mpSocket->Send(packet))
    {
        result = true;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the message data into JTCP packets for transmission.
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
bool JTCPClient::SerializeMessage(const Message* message,
                                  Packet::List& stream,
                                  Header::List& streamHeaders,
                                  const UShort startingSequenceNumber,
                                  const int broadcastFlags) const
{
    Packet packet;
    Header header;

    // Pre-allocate memory.
    packet.Reserve(JAUS::Header::MaxPacketSize + OverheadSizeBytes);

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
    else if(message->Write(packet, header, &mTransportHeader, true, startingSequenceNumber, (Byte)broadcastFlags) > 0)
    {
        stream.push_back(packet);
        streamHeaders.push_back(header);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the IP address of the network interface on the host machine to
///          use.  Set to blank if any address is fine.
///
///   \param[in] networkIP The broadcast group to connect to.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::SetInterfaceIP(const IP4Address& networkIP)
{
    if(IsInitialized() == false)
    {
        mHostIP = networkIP;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Enables debug messages.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::EnableDebugMessages(const bool on)
{
    if(mpTransportManager)
    {
        mpTransportManager->EnableDebugMessages(on);
    }
    Transport::EnableDebugMessages(on);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds to the list of JAUS and network address to try and create
///          connections to using TCP.  Method will only have an affect if 
///          this transport manager is active on this component.
///
///   \param[in] networkIP Network IP address.
///   \param[in] jausID JAUS ID.
///
///   \return True if added to list, false otherwise.  True does not mean the
///           mean the connection has actually been made.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCPClient::AddConnection(const IP4Address& networkIP,
                               const Address& jausID)
{
    if(mpTransportManager)
    {
        return mpTransportManager->AddConnection(networkIP, jausID);
    }
    else
    {
        std::cout << "JTCPClient::ERROR - Component doesn't have access to JTCP Manager.\n";
        return false;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if any connections need to be closed out.
///
///   \param[in] timeSinceLastCheckMs How many milliseconds that have past since
///              the last time the CheckServiceStatus method was called.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    static JAUS::Time lastTimeRecvActive;
    bool recvActive = mRecvThread.IsThreadActive();
    // Check to see if we have lost our connection to a Transport Manager
    if(recvActive == false && (JAUS::Time(true) - lastTimeRecvActive) > 1)
    {
        // Re-connect.
        Initialize(mComponentID, "127.0.0.1");
        lastTimeRecvActive.SetCurrentTime();
    }
    else if(recvActive)
    {
        lastTimeRecvActive.SetCurrentTime();
    }
    Mutex::ScopedLock lock(&mDataMutex);
    std::map<Address, Time::Stamp>::iterator utime;
    utime = mUpdateTimes.begin();
    while(utime != mUpdateTimes.end() && mDisconnectTimeMs > 0)
    {
        CxUtils::Time::Stamp diff = Time::GetUtcTimeMs() - utime->second;
        if(diff >= mDisconnectTimeMs)
        {
            mUpdateTimes.erase(utime);
            utime = mUpdateTimes.begin();
            continue;
        }
        utime++;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called whenever a JTCP stream packet has been received.
///
///   This implementation attempts to pass the data to the Transport Manager.
///
///   \param[in] packet JTCP packet with JTCP header, JAUS general header, and
///                     payload data.
///   \param[in] header Extracted JAUS general header.
///   \param[in] ipAddress IP Address of data source.
///   \param[in] sourcePort Source port the data came from on the stream.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCPClient::ProcessPacket(Packet& packet,
                               Header& header,
                               const IP4Address& ipAddress,
                               const unsigned short sourcePort)
{
    {
        Mutex::ScopedLock lock(&mDataMutex);
        mUpdateTimes[header.mSourceID] = Time::GetUtcTimeMs();
    }
    // Strip transport header data and process.
    unsigned int overhead = BYTE_SIZE;
    Packet::Wrapper payloadPacket(packet.Ptr() + overhead, packet.Length() - overhead);
    Transport::ProcessPacket(*payloadPacket.GetData(), header);
}


/*  End of File */
