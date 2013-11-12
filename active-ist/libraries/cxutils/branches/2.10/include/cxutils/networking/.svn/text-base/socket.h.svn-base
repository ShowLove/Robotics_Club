////////////////////////////////////////////////////////////////////////////////////
///
///  \file socket.h
///  \brief This file contains software for deriving different types
///  of socket structures for network communication.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 June 2007
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
#ifndef __CXUTILS_SOCKET_H
#define __CXUTILS_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>

#ifdef WIN32
#include <winsock.h>
#else
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#endif

#include "cxutils/cxbase.h"
#include "cxutils/packet.h"
#include "cxutils/networking/ip4address.h"
#include "cxutils/networking/ip6address.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Socket
    ///   \brief Basic socket structure for creating network connections.  All 
    ///          specific sockets inherit from this class (UDP, TCP).
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Socket
    {
    public:
        enum Type
        {
            Undefined = 0,
            TCPListen,
            TCPServer,
            TCPClient,
            UDPServer,
            UDPClient
        };

        Socket();
        Socket(const Socket &arg);
        virtual ~Socket();
        virtual void Shutdown();

        virtual int Send(const Packet &packet) const;
        virtual int Send(const char* buffer, 
                         const unsigned int length) const;
        int Recv(Packet &packet, 
                 const unsigned int receiveSize, 
                 const long int timeOutMilliseconds,
                 IPAddress* ipAddress = NULL,
                 unsigned short* port = NULL) const;
        int Recv(char* buffer, const unsigned int length, 
                 const long int timeOutMilliseconds,
                 IPAddress* ipAddress = NULL,
                 unsigned short* port = NULL) const;
        bool IsValid() const;
        bool SetNetworkInterface(const int num);
        bool SetNetworkInterface(const IP4Address& ipAddress);
        int GetSocket() const;
        Type GetType() const;
        int GetNetworkInterface() const;

        static unsigned int GetHostAddresses(IP4Address::List& ipAddresses);
        static bool IsHostAddress(const IP4Address& ipAddress);
        static int GetNetworkInterface(const IP4Address& ipAddress);
        static unsigned int GetNumNetworkConnections();
        static int IsIncommingData(const Socket* sock, const long int timeOutMilliseconds = 0);
        
        Socket &operator=(const Socket &arg);

    protected:
        virtual int SendFromBuffer(const char* buffer, 
                                   const unsigned int length) const = 0;
        virtual int RecvToBuffer(char* buffer, 
                                 const unsigned int length, 
                                 const long int timeOutMilliseconds = 0, 
                                 IPAddress* ipAddress = NULL,
                                 unsigned short* port = NULL) const = 0;
        static void InitializeSockets();
#ifdef WIN32
        SOCKET mSocket;                         ///<  Socket file descripter.
        static unsigned int mWinsockInitCount;  ///<  Initialization flag for winsock library.
#else
        int mSocket;                            ///<  Socket file descripter.
#endif
        Type mSocketType;                       ///<  Type of socket.
        int mServiceLength;                     ///<  Size of service data structure in bytes.
        int mNetworkInterface;                  ///<  Which network interface to use -1 = any, [0, n).
        bool mGoodSocket;                       ///<  Valid socket type.
        sockaddr_in mService;                   ///<  Type of socket and service being used.
    };
}

#endif

/*  End of File */
