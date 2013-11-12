////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcplistensocket.h
///  \brief This file contains software for creating TCP Listen Socket.  A Listen
///  socket waits for incomming TCP client connections.  Once a connection is
///  made, a pointer to a new TcpServer is returned.
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
#ifndef __CXUTILS_TCP_LISTEN_SOCKET_H
#define __CXUTILS_TCP_LISTEN_SOCKET_H

#include "socket.h"

namespace CxUtils
{
    class TcpServer; //  Forward declaration.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TcpListenSocket
    ///   \brief Socket which listens for incomming TCP client connections, and
    ///   creates TcpServerSockets.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL TcpListenSocket
    {
    public:
        TcpListenSocket();
        virtual ~TcpListenSocket();
        int InitializeSocket(const unsigned short port,
                             const unsigned int queuesize = 5,
                             const unsigned int stimeout = 0,
                             const unsigned int rtimeout = 0,
                             const int nonblocking = 0,
                             const int reusePort = 1);
        void Shutdown();
        int AwaitConnection(TcpServer& socket) const;
        bool SetNetworkInterface(const int num);
        bool SetNetworkInterface(const IP4Address& ipAddress);
        inline bool IsValid() const { return mGoodSocket; }
        TcpServer* AwaitConnection() const;
    private:
#ifdef WIN32
        SOCKET mSocket;                         ///<  Socket file descripter.
        static unsigned int mWinsockInitCount;  ///<  Initialization flag for winsock library.
#else
        int mSocket;                            ///<  Socket file descripter.
#endif
        int mNetworkInterface;                  ///<  Which network interface to use -1 = any, [0, n).
        int mPort;                              ///<  Port number used for TCP communication.
        long mComputer;                         ///<  Computer ID.
        struct hostent* mpHP;                   ///<  Host address.
        int mServiceLength;                     ///<  Size of service data structure in bytes.
        bool mGoodSocket;                       ///<  Valid socket type.
        sockaddr_in mService;                   ///<  Type of socket and service being used.
    };
}

#endif
/*  End of File */
