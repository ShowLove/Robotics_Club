////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpserver.cpp
///  \brief This file contains software for creating a client UDP socket.
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
#include "cxutils/networking/udpserver.h"
#include <string.h>
#include <limits.h>
#include <iostream>

#ifdef WIN32
#else
#include <errno.h>
#endif

using namespace std;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpServer::UdpServer()
{
    mPort = 0;
    mComputer = 0;
    mHP = NULL;
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));
    mSocketType = UDPServer;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpServer::~UdpServer()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes the socket.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpServer::Shutdown()
{
    if(mSocket)
    {
#ifdef WIN32
        closesocket(mSocket);
#else
        close(mSocket);        
#endif
        mSocket = 0;
    }
    Socket::Shutdown();
    mPort = 0;
    mComputer = 0;
    this->mSocketType = UDPServer;
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes as a socket for connecting to a UDP server.
///
///   \param[in] port The port to use.
///
///   \return 1 if socket created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpServer::InitializeSocket(const unsigned short port)
{
    //  Close any previous socket
    Shutdown();
    // Clear out settings.
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));

    // Create a UDP Socket
    mSocket = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    mPort = port;

    if(mSocket > 0)
    {
        mRecvAddr.sin_family = AF_INET;
        mRecvAddr.sin_port = htons(mPort);
        mRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        int bufferSize = 262144;
        int on = 1;

        if( bind(mSocket, (struct sockaddr *) &mRecvAddr, sizeof(mRecvAddr)) >= 0)
        {
            int code = 0;

            if(mNetworkInterface >= 0)
            {
                IP4Address::List myHostnames;
                GetHostAddresses(myHostnames);
                if(mNetworkInterface < (int)(myHostnames.size()))
                {
                    mRecvAddr.sin_addr.s_addr = *((long *)gethostbyname(myHostnames[mNetworkInterface].mString.c_str())->h_addr);
                    mRecvAddress = IP4Address(myHostnames[mNetworkInterface]);
                    setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_IF, (const char*) &mRecvAddr, sizeof(mRecvAddr));
                }
            }

            int options = 0;
            options |= setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));

            if(options != 0)
            {
                cout << "UDP ERROR: Failed to set UDP socket options.\n";
                Shutdown();
                return 0;
            }

            mServiceLength = sizeof(mService);
            //  We have a valid socket
            mGoodSocket = true;
            memcpy(&mService, &mRecvAddr, sizeof(mRecvAddr));
            memcpy(&this->mSendAddr, (struct sockaddr *)(&mService), sizeof(mSendAddr));
        }
        else
        {
            //cout << "UDP ERROR: bind failued on socket. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown();
        }        
    }

    return mGoodSocket;

}
/*
{
    int s;
    sockaddr_in recvaddr;

    //  Close any previous socket
    Shutdown();

    memset((void *)&mService, 0, sizeof(struct sockaddr_in));

    s = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    if(s > 0)
    {
        recvaddr.sin_family = AF_INET;
        recvaddr.sin_port = htons(port);
        recvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
        if(mNetworkInterface >= 0)
        {
            IP4Address::List myHostnames;
            GetHostAddresses(myHostnames);
            if(mNetworkInterface < (int)(myHostnames.size()))
            {
                recvaddr.sin_addr.s_addr = *((long *)gethostbyname(myHostnames[mNetworkInterface].mString.c_str())->h_addr);
                mRecvAddress = IP4Address(myHostnames[mNetworkInterface]);
            }
        }

        int sndsize = 262144;

        //  Don't allow multiple UDP Servers on single host (causes problems with unicast receiving).
        //setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
        setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&sndsize, sizeof(sndsize));

        if( bind(s, (struct sockaddr *) &recvaddr, sizeof(recvaddr)) >= 0)
        {
            mSocket = s;
            mPort = port;
            mServiceLength = sizeof(mService);
            //  We have a valid socket
            mGoodSocket = true;
            memcpy(&mService, &recvaddr, sizeof(recvaddr));
            memcpy(&this->mSendAddr, (struct sockaddr *)(&mService), sizeof(mSendAddr));
            memcpy(&this->mRecvAddr, (struct sockaddr *)(&mService), sizeof(mRecvAddr));
        }
        else
        {
            printf( "UDP ERROR: %s\n", strerror( errno ) );
            Shutdown();
        }
    }

    return mGoodSocket;

}
*/

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes as a socket for connecting to a UDP server.
///
///   \param[in] port The port to use.
///   \param[in] multicastGroup The multicast group to connect to.  This is an 
///             IP address in the range of "224.0.0.0-239.255.255.255"
///   \param[in] allowReuse If true, then multiple udp server sockets can
///                         run on single host.  If you need multiple sockets with
///                         the same port, you should use the UdpSharedServer
///                         instead.  However, if you are only using multicast,
///                         then this is OK to be true.
///
///   \return 1 if socket created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpServer::InitializeMulticastSocket(const unsigned short port,
                                         const IP4Address& multicastGroup,
                                         const bool allowReuse)
{
    //  Close any previous socket
    Shutdown();
    // Clear out settings.
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));

    // Create a UDP Socket
    mSocket = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    mPort = port;

    if(mSocket > 0)
    {
        mRecvAddr.sin_family = AF_INET;
        mRecvAddr.sin_port = htons(mPort);
        mRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);
        
        int bufferSize = 262144;
        int on = 1;

        // Don't allow multiple UDP Servers on single host (causes problems with unicast receiving).
        if(allowReuse == true && setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        {
            cout << "UDP ERROR: setsockopt failed SO_REUSEADDR. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown();
            return 0;
        }

        if(bind(mSocket, (struct sockaddr *) &mRecvAddr, sizeof(mRecvAddr)) >= 0)
        {
            struct ip_mreq mreq;
            int code = 0;

            if(mNetworkInterface >= 0)
            {
                IP4Address::List myHostnames;
                GetHostAddresses(myHostnames);
                if(mNetworkInterface < (int)(myHostnames.size()))
                {
                    // This makes sure we use the approprite NIC
                    //setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_IF, (const char*) &mRecvAddr, sizeof(mRecvAddr));
                    mRecvAddr.sin_addr.s_addr = *((long *)gethostbyname(myHostnames[mNetworkInterface].mString.c_str())->h_addr);
                    mRecvAddress = IP4Address(myHostnames[mNetworkInterface]);
                    mreq.imr_interface.s_addr = inet_addr(mRecvAddress.mString.c_str());
                }
                else
                {
                    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
                }
            }
            else
            {
                mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            }

            mreq.imr_multiaddr.s_addr = inet_addr(multicastGroup.mString.c_str());

            int options = 0;
            options |= setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));

            if(options != 0)
            {
#ifdef WIN32
                int errorCode = WSAGetLastError();
                cout << "UDP ERROR: " << errorCode << "\n";
#else
                cout << "UDP ERROR: Failed to set UDP socket options.\n";
#endif
                Shutdown();
                return 0;
            }

            mServiceLength = sizeof(mService);
            //  We have a valid socket
            mGoodSocket = true;
            memcpy(&mService, &mRecvAddr, sizeof(mRecvAddr));
            memcpy(&this->mSendAddr, (struct sockaddr *)(&mService), sizeof(mSendAddr));
        }
        else
        {
            //cout << "UDP ERROR: bind failued on socket. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown();
        }        
    }

    return mGoodSocket;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Receives data over a socket and places the result into
///          a character buffer.
///
///   \param[in] buffer Character buffer to put data in.
///   \param[in] length The size off buff/maximum number of bytes to try receive.
///   \param[in] timeOutMilliseconds How long to wait for 
///                                  incomming data in milliseconds.  A
///                                  value of 0 is a blocking function 
///                                  call (default).
///   \param[out] ipAddress The IP address of the source of received data.
///   \param[out] port The source port of the sender.
///
///   \return Number of bytes received, < 0 on error.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpServer::RecvToBuffer(char *buffer,
                            const unsigned int length,
                            const long int timeOutMilliseconds,
                            IPAddress* ipAddress,
                            unsigned short* port) const
{
    if(!IsValid())
    {
        return 0;
    }

    if(!Socket::IsIncommingData(this, timeOutMilliseconds))
    {
        return 0;
    }

    int rcvd = 0;

    sockaddr *r, *s;

    r = (sockaddr *)(&mRecvAddr);
    s = (sockaddr *)(&mSendAddr);
    memset(r, 0, sizeof(struct sockaddr));
    int recvlen = sizeof(struct sockaddr);

#ifdef WIN32
    rcvd =  recvfrom(mSocket,
                     buffer, 
                     length, 
                     0,
                     (struct sockaddr *)r, 
                     &recvlen);
#else
    rcvd =  recvfrom(mSocket, 
                     buffer, 
                     length, 
                     0,
                    (struct sockaddr *)r, 
                    (socklen_t *)&recvlen);
#endif

    //  By default set the send address to who
    //  data was last received from.
    if(rcvd > 0)
    {
        memcpy(s, r, sizeof(struct sockaddr));
        if(port || ipAddress)
        {
            struct sockaddr_in in;
            memcpy(&in, &mRecvAddr, sizeof(struct sockaddr_in));
            if(ipAddress && dynamic_cast<IP4Address*>(ipAddress))
            {
                *((IP4Address*)(ipAddress)) = std::string(inet_ntoa(in.sin_addr)); 
            }
            if(port)
            {
                *port = (unsigned short)ntohs(in.sin_port);
            }
        }
    }

    return rcvd;
}


/*  End of File */
