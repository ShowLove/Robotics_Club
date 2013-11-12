////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcpserver.cpp
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
#include "cxutils/networking/tcpserver.h"
#include "cxutils/networking/tcplistensocket.h"
#include <string.h>

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TcpServer::TcpServer()
{
    mPort = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor, shuts down socket.
///
////////////////////////////////////////////////////////////////////////////////////
TcpServer::~TcpServer()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This function waits/listens for a client to
///   make a connection to the server socket using a listen socket.
///
///   If the listen socket was created as blocking, this function will block.  
///   Otherwise it will wait as long as the maximum receive timeout.
///
///   \param socket The socket to listen on.
///
///   \return 1 if the socket was connected to a client, otherwise 0 (failure).
///
////////////////////////////////////////////////////////////////////////////////////
int TcpServer::InitializeSocket(const TcpListenSocket& socket)
{
    return socket.AwaitConnection(*this);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the data in the buffer.
///
///   \param[in] buffer The buffer containing data to send.
///   \param[in] length The length of the buffer (number of bytes to send).
///
///   \return Number of bytes sent, < 0 on failure (socket disconnected).
///
////////////////////////////////////////////////////////////////////////////////////
int TcpServer::SendFromBuffer(const char* buffer, const unsigned int length) const
{
    int result = 0;
    if(!mGoodSocket || mSocketType != TCPServer)
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
int TcpServer::RecvToBuffer(char* buffer, 
                            const unsigned int length, 
                            const long int timeOutMilliseconds,
                            IPAddress* ipAddress,
                            unsigned short* port) const
{
    int result;

    if(!mGoodSocket || mSocketType != TCPServer) 
    {
        return 0;
    }

    if(!Socket::IsIncommingData(this, timeOutMilliseconds))
    {
        return 0;
    }

    result = ::recv(this->mSocket, buffer, ((int)(length)), 0);

    if(result > 0)
    {
        if(ipAddress && dynamic_cast<IP4Address*>(ipAddress))
        {
            *((IP4Address*)(ipAddress)) = mClientAddress;
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
