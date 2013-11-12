////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcplistensocket.cpp
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
#include "cxutils/networking/tcplistensocket.h"
#include "cxutils/networking/tcpserver.h"
#include "cxutils/networking/socket.h"
#include <string.h>
#include <iostream>

using namespace std;

#ifdef WIN32
unsigned int CxUtils::TcpListenSocket::mWinsockInitCount = 0;
#else
#include <netinet/tcp.h>
#include <errno.h>
#endif


using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TcpListenSocket::TcpListenSocket()
{
    mSocket =  0;
    mServiceLength = 0;
    memset(&mService, 0, sizeof(struct sockaddr_in));
    mPort = 0;
    mGoodSocket = false;
    mComputer = 0;
    mpHP = NULL;
    mNetworkInterface = -1;
#ifdef WIN32
    //  In windows you must initialize
    //  the winsock dll
    if(mWinsockInitCount == 0)
    {
        WSADATA wsaData;
        // Initialize Winsock
        WSAStartup(MAKEWORD(2,2), &wsaData);

        // Increase static counter
        mWinsockInitCount++;
    }
    else
    {
        mWinsockInitCount++;
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TcpListenSocket::~TcpListenSocket()
{
    Shutdown();
#ifdef WIN32
    mWinsockInitCount--;
    if(mWinsockInitCount == 0)
    {
        // Deinitialize the ws2_32.dll
        // from winsock libraries
        // this must be called.
        WSACleanup();
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the network interface to use for sending/receiving data
///          on the socket.
///
///   \param num Interface number to use. -1 is any, 0 the first, 1 the second,
///              etc.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TcpListenSocket::SetNetworkInterface(const int num)
{
    mNetworkInterface = num;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method looks up the network connection number and then
///   calls the other SetNetworkInterface method with it.
///
///   \param ipAddress The IP Address of an existing connection to use.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TcpListenSocket::SetNetworkInterface(const IP4Address& ipAddress)
{
    int num = Socket::GetNetworkInterface(ipAddress);
    if(num >= 0)
    {
        return SetNetworkInterface(num);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes a TCP socket for creation of incomming connections
///   from clients.  This creates a socket that can listen for incomming TCP
///   connections.
///
///   This function also establishes how many ports max to assign for listening.
///
///   \param port The port to use for incomming connections.
///   \param queuesize Max number of possible incomming connections.
///   \param stimeout How long to wait in seconds before timeout on a send
///                   operation.  [0, 32767). 0 = INFINITE.
///   \param rtimeout How long to wait in seconds before timeout on a recv
///                   operation.  [0, 32767). 0 = INFINITE.
///   \param nonblocking If 1 than a non-blocking socket is created.
///   \param reusePort If 1, than port can be reused, 0 otherwise. This
///                    value is used with SO_REUSEADDR flag for setsockopt.
///
////////////////////////////////////////////////////////////////////////////////////
int TcpListenSocket::InitializeSocket(const unsigned short port,
                                      const unsigned int queuesize,
                                      const unsigned int stimeout,
                                      const unsigned int rtimeout,
                                      const int nonblocking,
                                      const int reusePort)
{
#ifdef WIN32
    SOCKET listenSocket;
#else
    int listenSocket;
#endif
    int result = 0;

    // Close any previous socket we were listening on.
    Shutdown();

    listenSocket = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    if(listenSocket > 0) 
    {
        //  Set blocking/non-blocking options
        if(nonblocking > 0) 
        {
#ifdef WIN32
            u_long noblock = 1;
            if(ioctlsocket(listenSocket, FIONBIO, &noblock) == SOCKET_ERROR) 
            {
                mGoodSocket = false;
                return 0;
            }
#else
            unsigned int noblock = 1;
            if(ioctl(listenSocket, FIONBIO, &noblock) == -1) 
            {
                cout << "TCP Listen Socket: Non Blocking Failure!\n";
                mGoodSocket = false;
                return 0;
            }
#endif
        }

        /*  Copy the port number and set the
            service information for the socket. */
        mPort = port;
        memset(&this->mService, 0, sizeof(struct sockaddr_in) );
        this->mServiceLength = sizeof(mService);

        this->mService.sin_family = AF_INET;                    //  AF_INET for TCP communication
        this->mService.sin_port = htons(mPort);                 //  Convert port number to network byte order.
        this->mService.sin_addr.s_addr = htonl(INADDR_ANY);     //  Set network address to listen on.

        /*  If we get to this point, set the default send and
            receive timeout options for the socket. */
        int ret = 0;
#ifndef WIN32
        ret = setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR,  (char *)&reusePort, sizeof(int));
        if(ret < 0)
        {
            //std::cout << strerror(errno) << std::endl;
        }
#endif

        // Bind the socket to the port and service.
        if( bind(listenSocket, (struct sockaddr *)& this->mService, sizeof(mService)) >= 0 ) 
        {
            //  Setup incomming connections for listening on the socket.
            if(listen(listenSocket, queuesize) < 0) 
            {
                /*  On a failure close the socket
                    and return false. */
                #ifdef WIN32
                    closesocket(listenSocket);
                #else
                    close(listenSocket);
                #endif
                std::cout << "TCP Listen Socket: Failed to setup Queue Size.\n";
                Shutdown();  //  Ensure defaults are set.
                result = 0;
                return result;
            }
            else
            {
                ret = setsockopt(listenSocket, SOL_SOCKET, SO_RCVTIMEO, (char *)&rtimeout, sizeof(int));
                ret = setsockopt(listenSocket, SOL_SOCKET, SO_SNDTIMEO, (char *)&stimeout, sizeof(int));
                int nodelay = 1;
                ret = setsockopt(listenSocket, IPPROTO_TCP, TCP_NODELAY,  (char *)&nodelay, sizeof(int));

                /*  Everything has been initialized ok, so copy the
                    data from the temporary listenSocket to our
                    final socket destination (mSocket). */
                memcpy(&this->mSocket, &listenSocket, sizeof(int));
                result = 1;
            }
        }
        else
        {
            //std::cout << "TCP Listen Socket: Failed to bind socket\n";
#ifndef WIN32
            //std::cout << strerror(errno) << std::endl;
#endif
        }
    }

#ifdef WIN32
    int value;

    value = WSAGetLastError();
#endif

    /*  If we failed to create
        the socket, then lets close/clear out
        what has been done */
    if(!result) 
    {
        Shutdown();
        mGoodSocket = false;
    }
    else 
    {
        mGoodSocket = true;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes the socket down, and stops any listening.
///
////////////////////////////////////////////////////////////////////////////////////
void TcpListenSocket::Shutdown()
{
#ifdef WIN32
    shutdown(mSocket, 1);
    closesocket(mSocket);
#else
    /*
    if(mSocket > 0)
    {
        if(shutdown(mSocket, 1) == 0)
        {
            mSocket = 0;
        }
    }
    */
    int r1, r2;
    if(mSocket > 0)
    {
        r1 = shutdown(mSocket, SHUT_RDWR);
        r2 = close(mSocket);
        if(r1 != 0 || r2 != 0)
        {
            //std::cout << strerror(errno) << std::endl;
        }
    }
#endif
    mSocket = 0;
    this->mGoodSocket = false;
    mServiceLength = 0;
    memset(&mService, 0, sizeof(struct sockaddr_in));
    mPort = 0;
    mpHP = NULL;
    mComputer = 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function waits/listens for a client to
///   make a connection to the server.
///
///   If the listen socket was created as blocking, this function will block.
///   Otherwise it will wait as long as the maximum receive timeout.
///
///   \param socket The socket to store the received client connection.
///
///   \return 1 if the socket was connected to a client, otherwise 0 (failure).
///
////////////////////////////////////////////////////////////////////////////////////
int TcpListenSocket::AwaitConnection(TcpServer& socket) const
{
    int tSocket;    // Temporary file descriptor

    int tSendAddrLength = sizeof(struct sockaddr_in);
    struct sockaddr tSendAddr;
    memset(&tSendAddr, 0, sizeof(tSendAddr));
    socket.Shutdown();

    /*  Only valid sockets and TCP Listen sockets
        can perform an await connection */
    if( !IsValid() )
        return 0;

    tSocket = -1;

    //  Try an accept an incomming connection attempt.
#ifdef WIN32
    tSocket = (int)accept(mSocket,
                         (struct sockaddr *) &tSendAddr,
                         &tSendAddrLength);
#else
    tSocket = accept(mSocket,
                     (struct sockaddr *) &tSendAddr,
                     (socklen_t *)&tSendAddrLength);
#endif

    if(tSocket == -1)
        return 0;

    int bufferSize = 262144;
    setsockopt(tSocket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
    setsockopt(tSocket, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));

    socket.mGoodSocket = true;
    socket.mSocket = tSocket;
    socket.mService = this->mService;
    socket.mServiceLength = this->mServiceLength;
    socket.mSocketType = Socket::TCPServer;
    struct sockaddr_in inSocket;
    memcpy(&inSocket, &tSendAddr, sizeof(struct sockaddr_in));
    socket.mClientAddress = std::string(inet_ntoa(inSocket.sin_addr));
    socket.mPort = (int)ntohs((((struct sockaddr_in*)&tSendAddr)->sin_port));

    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function waits/listens for a client to
///   make a connection to the server.  If a connection is established
///   then a pointer to a socket is returned.  The socket must be deleted
///   when done with.
///
///   If the listen socket was created as blocking, this function will block.
///   Otherwise it will wait as long as the maximum receive timeout.
///
///   \return Pointer to newly allocated connection if made, otherwise NULL on
///   failure.
///
////////////////////////////////////////////////////////////////////////////////////
TcpServer *TcpListenSocket::AwaitConnection() const
{
    TcpServer* newCon = new TcpServer();
    if(AwaitConnection(*newCon))
    {
        return newCon;
    }

    delete newCon;
    return NULL;
}

/*  End of File */
