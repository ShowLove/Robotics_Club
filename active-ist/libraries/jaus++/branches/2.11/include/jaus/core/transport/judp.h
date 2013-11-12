////////////////////////////////////////////////////////////////////////////////////
///
///  \file judp.h
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
#ifndef __JAUS_CORE_TRANSPORT_UDP__H
#define __JAUS_CORE_TRANSPORT_UDP__H

#include "jaus/core/transport/transport.h"
#include <cxutils/networking/udpclient.h>
#include <cxutils/networking/udpsharedserver.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class JUDP
    ///   \brief Transport is an interface class for sending/receiving UDP packets.
    ///          This interface coforms to the SAE-JAUS AS5669 Standard.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL JUDP : public Transport, 
                               public CxUtils::UdpSharedServer::Callback
    {
    public:
        const static unsigned short Port = 3794;         ///< JAUS UDP/TCP Port Number == "jaus".
        const static unsigned int OverheadSizeBytes = 61;///< JUDP Overhead in bytes.
        const static Byte Version = 0x02;                ///< JUDP Header Version.
        JUDP();
        ~JUDP();
        // Sets the delay time in ms for UDP polling loop.
        virtual void SetPacketPollingDelayMs(const unsigned int delayTimeMs = 0);
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename);
        // Initializes the transport with a given ID for a component.
        virtual bool Initialize(const Address& componentID);
        // Returns true if transport has been initialized.
        virtual bool IsInitialized() const;
        // Shutsdown the transport service.
        virtual void Shutdown();
        // Gets the list of connections available.
        virtual Address::List GetConnections() const;
        // Returns true if connection is present.
        virtual bool HaveConnection(const Address& id) const;
        // Send a serialized message.
        virtual bool SendPacket(const Packet& packet, 
                                const Header& header,
                                const int broadcastFlags = Service::NoBroadcast) const;
        // Serialize the JAUS message (add transport headers, serialize payload, etc.) 
        virtual bool SerializeMessage(const Message* message, 
                                      Packet::List& stream,
                                      Header::List& streamHeaders,
                                      const UShort startingSequenceNumber,
                                      const int broadcastFlags) const;
        // Get a copy of the transport header.
        virtual Packet GetTransportHeader() const { return mTransportHeader; }
        // Gets a list of all manually created connections.
        virtual Address::List GetManualConnections() const ;
        // Sets the maximum packet size.
        void SetMaxPacketSize(const unsigned int maxSizeBytes = 1500);
        // Sets how long to wait until a connection is considered invalid.
        void SetDisconnectTimeMs(const unsigned int timeMs);
        // Sets the default Time-to-Live (TTL) for multicast.
        void SetTTL(const unsigned char ttl = 16);
        // Sets the mutlicast group IP address.
        void SetMulticastIP(const CxUtils::IP4Address& multicastIP);
        // Use broadcast instead of multicast.
        void EnableBroadcasting(const bool enabled);
        // Gets  the mutlicast group IP address.
        CxUtils::IP4Address GetMulticastIP() const { return mMulticastIP; }
        // Sets the host interface IP address to connect on.
        void SetInterfaceIP(const CxUtils::IP4Address& networkIP = CxUtils::IP4Address());
        // Adds a connection to a specific IP address.
        bool AddConnection(const CxUtils::IP4Address& networkIP,
                           const Address& jausID,
                           const unsigned short port = Port);
        virtual bool CloseConnection(const Address& jausID);
        // Method to listen/discover broadcasting subsystem on the network.
        static bool ListenForSubsystems(Address::Set& discovered,
                                        const unsigned int waitTimeMs = 5000,
                                        const CxUtils::IP4Address& multicastIP = "239.255.0.1");
    private:
        static void ReceiveThread(void* args);
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);        
        virtual void ProcessUDP(const Packet& packet, 
                                const CxUtils::IP4Address& ipAddress,
                                const unsigned short sourcePort);
        int UpdateClientConnections(const Address& id,
                                    const CxUtils::IP4Address& ipAddress,
                                    const unsigned short sourcePort);
        Time::Stamp mDisconnectTimeMs;            ///<  How long to wait in ms before a connection is closed.
        unsigned int mMaxPayloadSize;             ///<  Maximum payload size.
        unsigned char mTimeToLive;                ///<  Time to Live TTL for UDP.
        CxUtils::IP4Address mHostIP;              ///<  Host IP Address to use.
        CxUtils::IP4Address mMulticastIP;         ///<  IP Address for local multicast group.
        unsigned short mInternalDiscoveryPort;    ///<  Port to use for internal discovery of connections on localhost.
        bool mUseBroadcastingFlag;                ///<  If true, broadcasting is enabled.
        Mutex mClientsMutex;                      ///<  Mutex for thread protection of client data.
        Thread mPrimaryThread;                    ///<  Thread for receiving UDP data.
        Thread mSecondaryThread;                  ///<  Secondary thread for receiving UDP data.
        unsigned int mDelayTimeMs;                ///<  Polling delay time in ms.
        CxUtils::UdpServer mInput;                ///<  UDP Input.
        CxUtils::UdpClient mMulticast;            ///<  Local UDP Mutlicast Socket.
        std::map<Address, Time::Stamp> mUpdateTimes;     ///< Times when data was received from a JAUS Component.
        std::map<Address, CxUtils::UdpClient*> mClients; ///< Unicast UDP Sockets to JAUS Components.
        std::map<Address, bool> mPermanentConnections;   ///< Permanent connections added manually.
        Packet mTransportHeader;                         ///< JUDP Transport Header.
        volatile bool mPrimaryThreadCreatedFlag;         ///< True if primary thread has been initialized.
    };
}

#endif
/*  End of File */
