////////////////////////////////////////////////////////////////////////////////////
///
///  \file jtcpclient.h
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
#ifndef __JAUS_CORE_TRANSPORT_TCP_CLIENT__H
#define __JAUS_CORE_TRANSPORT_TCP_CLIENT__H

#include "jaus/core/transport/transport.h"
#include "jaus/core/transport/jtcp.h"
#include <cxutils/networking/tcpclient.h>

namespace JAUS
{
    class TransportManager;
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class JTCPClient
    ///   \brief Transport is an interface class for sending/receiving JTCPClient 
    ///          streams.
    ///
    ///          This interface coforms to the SAE-JAUS AS5669 Standard.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL JTCPClient : public Transport,
                                     public JTCP
    {
    public:
        JTCPClient();
        virtual ~JTCPClient();
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename);
        // Initializes the transport with a given ID for a component.
        virtual bool Initialize(const Address& componentID);
        // Initializes the transport with a given ID for a component.
        virtual bool Initialize(const Address& componentID, const IP4Address& host);
        // Returns true if transport has been initialized.
        virtual bool IsInitialized() const;
        // Shutsdown the transport service.
        virtual void Shutdown();
        // Sets maximum packet size.
        void SetMaxPacketSize(const unsigned int maxSizeBytes);
        // Gets the list of connections available (not supported).
        virtual Address::List GetConnections() const;
        // Gets a list of all manually created connections (not supported).
        virtual Address::List GetManualConnections() const { return JAUS::Address::List(); }
        // Returns true if connection is present.
        virtual bool HaveConnection(const Address& id) const;
        // Returns true if connection is present.
        virtual bool HaveConnection(const Address& id,
                                    const unsigned int timeSinceUpdateMs) const;
        // Send a serialized message.
        virtual bool SendPacket(const Packet& packet, 
                                const Header& header, 
                                const int broadcastFlags = NoBroadcast) const;
        // Serialize the JAUS message (add transport headers, serialize payload, etc.) 
        virtual bool SerializeMessage(const Message* message, 
                                      Packet::List& stream,
                                      Header::List& streamHeaders,
                                      const UShort startingSequenceNumber,
                                      const int broadcastFlag) const;
        // Get a copy of the transport header.
        virtual Packet GetTransportHeader() const { return mTransportHeader; }
        // Sets the host interface IP address to connect on.
        void SetInterfaceIP(const IP4Address& networkIP = IP4Address());
        // Turn on Debug Messages.
        virtual void EnableDebugMessages(const bool on = true);
        // Adds a connection to a specific IP address.
        bool AddConnection(const IP4Address& networkIP,
                           const Address& jausID);
    private:
        virtual void ProcessPacket(Packet& packet,
                                   Header& header,
                                   const IP4Address& ipAddress,
                                   const unsigned short sourcePort);
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        unsigned int mMaxPayloadSize;                    ///< Maximum message payload size in bytes.
        CxUtils::IP4Address mHostIP;                     ///< Host IP Address to use.
        std::map<Address, Time::Stamp> mUpdateTimes;     ///< Times when data was received from a JAUS Component.
        Packet mTransportHeader;                         ///< JTCP Transport Header.
        TransportManager* mpTransportManager;            ///< Transport manager if created.
        Time::Stamp mDisconnectTimeMs;                   ///< How long to wait in ms before a connection to a subsystem determined lost.
    };
}

#endif
/*  End of File */
