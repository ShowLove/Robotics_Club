////////////////////////////////////////////////////////////////////////////////////
///
///  \file jtcp.h
///  \brief Handles a single JTCP connection to the host application.
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
#ifndef __JAUS_CORE_TRANSPORT_TCP_SERVER__H
#define __JAUS_CORE_TRANSPORT_TCP_SERVER__H

#include "jaus/core/transport/transport.h"
#include <cxutils/networking/tcpclient.h>
#include <cxutils/networking/tcpserver.h>

namespace JAUS
{
    class TransportManager;
    typedef CxUtils::IP4Address IP4Address; ///< Typedef for shortcut in typing.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class JTCP
    ///   \brief Transport is an interface class for sending/receiving JTCP
    ///          streams.
    ///
    ///          This interface coforms to the SAE-JAUS AS5669 Standard.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL JTCP
    {
    public:
        const static unsigned short Port = 3794;         ///< JAUS UDP/TCP Port Number == "jaus".
        const static unsigned int OverheadSizeBytes = 61;///< JTCP Overhead in bytes.
        const static Byte Version = 0x02;                ///< JTCP Header Version.
        JTCP();
        virtual ~JTCP();
        // Initializes, creating a thread for receiving TCP data.
        virtual bool Initialize(TransportManager* manager, CxUtils::Socket* socket);
        // Shutsdown the transport service.
        virtual void Shutdown();
        // Sets the delay time in ms for stream polling loop.
        virtual void SetPacketPollingDelayMs(const unsigned int delayTimeMs = 0);
        // Gets a pointer to the socket.
        CxUtils::Socket* GetSocket() { return mpSocket; }
        // Gets a pointer to the socket.
        const CxUtils::Socket* GetSocket() const { return mpSocket; }
        // Sends the data using the socket.
        virtual int SendTCP(const Packet& packet) const { return mpSocket ? mpSocket->Send(packet) : 0; }
        // Gets the IP Address of the connection
        IP4Address GetIP() const 
        {
            if(mpSocket)
            {
                CxUtils::TcpClient* client = dynamic_cast<CxUtils::TcpClient*>(mpSocket);
                CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>(mpSocket);
                return client ? client->GetServerAddress() : server->GetClientAddress();
            }
            return IP4Address();
        }
        // Gets the IP Address of the connection
        int GetPort() const 
        {
            if(mpSocket)
            {
                CxUtils::TcpClient* client = dynamic_cast<CxUtils::TcpClient*>(mpSocket);
                CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>(mpSocket);
                return client ? client->GetPort() : server->GetPort();
            }
            return 0;
        }
        // Gets the update time (when last message was received).
        JAUS::Time GetUpdateTime() const;
        // Gets the address of the component that last sent a message.
        JAUS::Address GetLastSourceID() const;
        // Flag set if connection is on the current machine.
        bool IsLocalConnection() const { return mLocalConnectionFlag; }
    protected:
        virtual void ProcessPacket(Packet& packet,
                                   Header& header,
                                   const IP4Address& ipAddress,
                                   const unsigned short sourcePort);
        virtual bool ExitThread() const { return false; }
        static void ReceiveThread(void* args);
        Mutex mDataMutex;                         ///<  Mutex for thread protection of data.
        Thread mRecvThread;                       ///<  Thread for receiving UDP data.
        unsigned int mDelayTimeMs;                ///<  Polling delay time in ms.
        CxUtils::Socket* mpSocket;                ///<  TCP Socket.
        TransportManager* mpManager;              ///<  Pointer to transport manager.
        Time mUpdateTime;                         ///<  Times when data was received on stream.
        volatile bool mReceivedValidDataFlag;     ///< If true, message data has been received.
        Address mLastSourceID;                    ///<  ID of the source ID of last message received.
        bool mLocalConnectionFlag;                ///<  True if local connection.
    };
}

#endif
/*  End of File */
