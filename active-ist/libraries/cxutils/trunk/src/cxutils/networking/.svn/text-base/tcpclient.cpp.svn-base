////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcpclient.cpp
///  \brief This file contains software for creating a TCP client connection.
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
#include "cxutils/networking/tcpclient.h"
#include <string.h>

#ifndef WIN32
#include <sys/socket.h>
#endif

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TcpClient::TcpClient()
{
    mPort = 0;
    mGoodSocket = false;
    mComputer = 0;
    mpHP = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TcpClient::~TcpClient()
{
    Shutdown();
    mPort = 0;
    mGoodSocket = false;
    mComputer = 0;
    mpHP = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes the socket and all sending/receiving.
///
////////////////////////////////////////////////////////////////////////////////////
void TcpClient::Shutdown()
{
    Socket::Shutdown();
    mGoodSocket = false;
    mPort = 0;
    this->mSocketType = Undefined;
    mpHP = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a socket for connecting to a TCP server.
///
///   \param[in] ipAddress The computer host name/IP address to connect to.
///   \param[in] port The port to use.
///   \param[in] stimeout How long to wait in seconds before timeout on a send
///                   operation.  [0, 32767). 0 = INFINITE.
///   \param[in] rtimeout How long to wait in seconds before timeout on a recv
///                   operation.  [0, 32767). 0 = INFINITE.
///
///   \return 1 if a connection was made, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int TcpClient::InitializeSocket(const IP4Address& ipAddress,
                                const unsigned short port,
                                const int stimeout,
                                const int rtimeout)
{
#ifdef WIN32
    SOCKET s;
#else
    int s;
#endif
    sockaddr_in service;
    hostent *hp;
    int slen;
    long computer;

#ifdef WIN32
    s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
    s = socket(PF_INET, SOCK_STREAM, 0);
#endif

    if(s > 0)
    {
        //  Close any previous socket      
        memset(&service, 0, sizeof(struct sockaddr_in));
        slen = sizeof(service);

        hp = gethostbyname(ipAddress.mString.c_str());
        if(hp != NULL)
        {        
            computer = *((long *) hp->h_addr);            
            service.sin_family = AF_INET;
            service.sin_port = htons(port);
            service.sin_addr.s_addr = computer;    

#ifdef WIN32
            setsockopt(s, SOL_SOCKET, SO_RCVTIMEO , (char *)&rtimeout, sizeof(rtimeout));
            setsockopt(s, SOL_SOCKET, SO_SNDTIMEO , (char *)&stimeout, sizeof(stimeout));
            int nodelay = 1;
            setsockopt(s, IPPROTO_TCP, TCP_NODELAY, (char *)&nodelay, sizeof(int));

#else 
            setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &rtimeout, sizeof(rtimeout));
            setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &stimeout, sizeof(stimeout));


#ifdef SO_NOSIGPIPE
            int setNoSIGPIPE = 1;
            setsockopt(s, SOL_SOCKET, MSG_NOSIGNAL, &setNoSIGPIPE, sizeof(setNoSIGPIPE));
#endif
            int nodelay = 1;
            setsockopt(s, IPPROTO_TCP, TCP_NODELAY, &nodelay, sizeof(int));
#endif 
            int bufferSize = 262144;
            setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));

            if(connect(s, (struct sockaddr *)&service, sizeof(struct sockaddr_in)) == 0)
            {
                this->mGoodSocket = true;
                this->mSocket = s;
                this->mService = service;
                this->mServiceLength = slen;
                this->mSocketType = TCPClient;
                this->mServerAddress = ipAddress;
                // Lookup the actual port we connected to in the end.
                struct sockaddr_in sa;
                int sa_len;
                memset(&sa, 0, sizeof(sockaddr_in));
#ifdef WIN32
                getsockname(s, (struct sockaddr*)&sa, &sa_len);
#else
                getsockname(s, (struct sockaddr*)&sa, (socklen_t*)&sa_len);
#endif
                this->mSourceID = inet_ntoa(sa.sin_addr);
                this->mPort = (int)ntohs(sa.sin_port);
                return 1;
            }
        }
        //  Unable to connect, close open socket.
#ifdef WIN32
        int e = WSAGetLastError();
        closesocket(s);
#else
        close(s);
#endif        
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the data in the buffer.
///
///   \param[in] buffer The buffer containing data to send.
///   \param[in] length The length of the buffer.
///
///   \return Number of bytes sent, < 0 on error (socket disconnected).
///
////////////////////////////////////////////////////////////////////////////////////
int TcpClient::SendFromBuffer(const char* buffer, const unsigned int length) const
{
    int result = 0;
    if(!mGoodSocket || mSocketType != TCPClient)
    {
        return 0;
    }
#ifdef MSG_NOSIGNAL
    result = ::send(this->mSocket, buffer, ((int)(length)), MSG_NOSIGNAL);
#else
    result = ::send(this->mSocket, buffer, ((int)(length)), 0);
#endif

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Receives data over a socket and places the result into
///          a character buffer.
///
///   \param[in] buffer Character buffer to put data in.
///   \param[in] length The size off buff/maximum number of bytes to try receive.
///   \param[in] tms How long to wait for incomming data in milliseconds.  A
///          value of 0 is a blocking function call (default).
///   \param[out] ipAddress The IP address of the source of received data.
///   \param[out] port The source port of the sender.
///
///   \return Number of bytes received, < 0 on error (socket disconnected).
///
////////////////////////////////////////////////////////////////////////////////////
int TcpClient::RecvToBuffer(char* buffer, 
                            const unsigned int length, 
                            const long int timeoutMilliseconds, 
                            IPAddress* ipAddress,
                            unsigned short* port) const
{
    int result;

    if(!mGoodSocket || mSocketType != TCPClient) 
    {
        return 0;
    }

    if(!Socket::IsIncommingData(this, timeoutMilliseconds))
    {
        return 0;
    }
#ifdef MSG_NOSIGNAL
    result = ::recv(this->mSocket, buffer, ((int)(length)), MSG_NOSIGNAL);
#else
    result = ::recv(this->mSocket, buffer, ((int)(length)), 0);
#endif

    if(result > 0)
    {
        if(ipAddress && dynamic_cast<IP4Address*>(ipAddress))
        {
            *((IP4Address*)(ipAddress)) = mServerAddress;
        }
        if(port)
        {
            *port = mPort;
        }
        return result;
    }
    else
    {
        // If we got here, then IsIncommingData said data was
        // available, but no data was received.
        return -1;
    }
}

/*  End of File */
