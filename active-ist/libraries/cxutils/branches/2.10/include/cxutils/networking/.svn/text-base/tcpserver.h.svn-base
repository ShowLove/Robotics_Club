////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcpserver.h
///  \brief This file contains software for the TcpServer.  These
///  sockets are created by the TcpListenSocket and are used to handle
///  TCP connections from clients (TcpClientSocket).
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 22 June 2007
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
#ifndef __CXUTILS_TCP_SERVER_SOCKET_H
#define __CXUTILS_TCP_SERVER_SOCKET_H

#include "cxutils/networking/socket.h"
#include "cxutils/networking/tcplistensocket.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TcpServer
    ///   \brief Socket created by TcpListenSocket which maintains a connection to
    ///   a TCP client.  Capable of sending/receiving packet data from/to client.
    ///
    ///   TCP Connections are bi-directional (you can send and receive on the socket).
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL TcpServer : public Socket
    {
        friend class TcpListenSocket;
    public:
        TcpServer();
        virtual ~TcpServer();
        int InitializeSocket(const TcpListenSocket& socket);
        inline IP4Address GetClientAddress() const { return mClientAddress; }
        inline int GetPort() const { return mPort; }
    private:
        virtual int SendFromBuffer(const char* buffer, 
                                   const unsigned int length) const;
        virtual int RecvToBuffer(char* buffer, 
                                 const unsigned int length, 
                                 const long int timeOutMilliseconds = 0, 
                                 IPAddress* ipAddress = NULL,
                                 unsigned short* port = NULL) const;
        IP4Address mClientAddress; ///<  IP address of connected client.
        int mPort;                 ///<  Port the connection is on.
    };
}

#endif
/*  End of File */
