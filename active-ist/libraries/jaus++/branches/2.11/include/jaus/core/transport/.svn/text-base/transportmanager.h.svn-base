////////////////////////////////////////////////////////////////////////////////////
///
///  \file transportmanager.h
///  \brief The "Transport" TransportManager is a program that handles UDP, TCP, and 
///         other transport communcations on a host PC.
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
#ifndef __JAUS_CORE_TRANSPORT_TRANSPORT_TransportManager__H
#define __JAUS_CORE_TRANSPORT_TRANSPORT_TransportManager__H


#include "jaus/core/message.h"
#include "jaus/core/transport/jtcpclient.h"
#include <cxutils/networking/udpclient.h>
#include <cxutils/networking/tcpserver.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TransportManager
    ///   \brief The TransportManager manages all JAUS transport
    ///          layer communications (JUDP/JTCP) so that multiple components
    ///          can communicate on the same host PC without needing to reserve
    ///          the JAUS port.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL TransportManager
    {
    public:
        static const int TCP    = 0;
        static const int UDP    = 1;
        static const int Serial = 2;
        TransportManager();
        virtual ~TransportManager();
        // Check if this TransportManager is active.
        bool IsInitialized() const { return mTcpListenThread.IsThreadActive(); }
        // Loads settings from a JAUS file.
        virtual bool LoadSettings(const std::string& filename);
        // Initializes using loaded/pre-defines settings.
        virtual bool Initialize();
        // Shutsdown all transport comms.
        virtual void Shutdown();
        // Process packet data that needs to be routed
        void ProcessPacket(Packet& packet,
                           Header& header,
                           const IP4Address& ipAddress,
                           const unsigned short sourcePort,
                           const int transport);
        // Turns on debug messages to console window.
        virtual void EnableDebugMessages(const bool on) { mDebugMessagesFlag = true; }
        // Adds a connection to a specific IP address.
        bool AddConnection(const IP4Address& networkIP,
                           const Address& jausID);
        // Gets the number of internal local connections.
        unsigned int GetNumLocalClients() const;
        /** Method to signal shutdown is happening, but does not do a complete shutdown. */
        void SignalShutdown(const bool value = true) { mGlobalShutdownFlag = value; }
    protected:
        virtual void CleanupConnections();
        void CheckForConnection(const Address& header,
                                const IP4Address& sourceAddress,
                                const unsigned short sourcePort,
                                const int transport);   ///<  Checks to see if a connection exists, and if not, creates one.
        static void UpdateThread(void* args);           ///<  Does periodic updates and connection cleanup.
        static void TcpListenThread(void* args);        ///<  Listens for TCP connections.
        static void UdpReceiveThread(void* args);       ///<  Thread for UDP receiving.
        Thread mUpdateThread;                           ///<  Update/Check connections thread.
        bool mUseTcpFlag;                               ///<  If true, TCP is chosen over UDP for making connections.
        IP4Address mHostIP;                             ///<  Host IP Address to use.
        Mutex mDebugMessagesMutex;                      ///<  Mutex to lock console for message display.
        volatile bool mDebugMessagesFlag;               ///<  If true, show debugging messages.
        volatile bool mGlobalShutdownFlag;              ///<  Shutdown flag signal.
        unsigned int mDelayTimeMs;                      ///<  Delay time in polling for network/message data.
        Mutex mConnectionsMutex;                        ///<  Mutex for thread protection of all connection data structures.
        // JUDP Data
        CxUtils::UdpClient  mUdpSocket;                 ///<  UDP server and multicast/broadcast socket.
        unsigned int mMaxPayloadSize;                   ///<  Maximum payload size.
        unsigned char mTimeToLive;                      ///<  Time to Live TTL for UDP.
        CxUtils::IP4Address mMulticastIP;               ///<  IP Address for local multicast group.
        bool mUseBroadcastingFlag;                      ///<  If true, broadcasting is enabled over multicast.
        Time::Stamp mDisconnectTimeMs;                  ///<  How long to wait in ms before a connection is closed.
        CxUtils::Thread mUdpReceiveThread;              ///<  UDP Receving thread.
        std::map<Address, CxUtils::UdpClient*> mUdpConnections; ///<  UDP Socket connections made by TransportManager.
        std::map<Address, Time::Stamp> mUpdateTimes;    ///<  Times when connections were last updated.
        // JTCP Data
        CxUtils::TcpListenSocket mListenSocket;         ///<  TCP Listening socket for incomming connections.
        CxUtils::Thread mTcpListenThread;               ///<  TCP Listening thread.
        std::map<Address, JTCP*> mTcpConnections;       ///<  TCP connections made from external sources.
        std::vector<JTCP*> mUnknownConnections;         ///<  Temporary TCP connections not mapped to a JAUS ID yet.
        // Other information
        std::map<Address, IP4Address> mPermanentConnections; ///<  Permanent connections added manually.
    };
}

#endif
/*  End of File */
