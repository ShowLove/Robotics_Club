////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpclient.cpp
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
#include "cxutils/networking/udpclient.h"
#include <string.h>
#include <iostream>

using namespace std;
using namespace CxUtils;

#ifndef WIN32
#include <errno.h>
#endif

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpClient::UdpClient()
{
    mComputer = 0;
    mHP = NULL;
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));
    mSocketType = UDPClient;
    mDestinationPort = 0;
    mSourcePort = 0;
    mDuplicateSocket = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpClient::~UdpClient()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes the socket.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpClient::Shutdown()
{
    if(mDuplicateSocket == false)
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
    }
    else
    {
        mSocket = 0;
        mServiceLength = 0;
        mGoodSocket = false;
        memset(&mService, 0, sizeof(struct sockaddr_in));
    }
    mDestinationPort = 0;
    mSourcePort = 0;
    mComputer = 0;
    mSocketType = UDPClient;
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));
    mConnectionID.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes as a socket for connecting to a UDP server.
///
///   \param[in] ipAddress The IP Address information of the destination host you
///                    want to send data to.
///   \param[in] destinationPort The port to send to.
///   \param[in] sourcePort The port origin port the data will be sent from.  If
///                         the value is 0, then any available port is chosen.
///                         This is also the port that the Rev methods will 
///                         listen on.
///
///   \return 1 if socket created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpClient::InitializeSocket(const IP4Address& ipAddress,
                                const unsigned short destinationPort,
                                const unsigned short sourcePort)
{
    //  Close any previous socket
    Shutdown();
    // Clear out settings.
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));

    // Create a UDP Socket
    mSocket = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    mDestinationPort = destinationPort;

    if(mSocket > 0)
    {
        // Setup receive options for binding.
        mRecvAddr.sin_family = AF_INET;                 // IP Family
        mRecvAddr.sin_port = htons(sourcePort);         // Receive on this port (0) = any
        mRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Receive on any address by default.
        // Now try to bind the socket.
        if(bind(mSocket, (struct sockaddr *) &mRecvAddr, sizeof(mRecvAddr)) >= 0)
        {
            int bufferSize = 262144;
            int on = 1;
            int options = 0;

            // Now setup send options.
            mSendAddr.sin_family = AF_INET;
            mSendAddr.sin_port = htons(mDestinationPort);
            if( strstr(ipAddress.mString.c_str(), "255") != 0 )
            {
                mSendAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
                if(setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on)) < 0)
                {
                    cout << "UDP ERROR: setsockopt failed SO_BROADCAST. ERRNO: " << errno << "-" << strerror(errno) << "\n";
                    Shutdown();
                    return 0;
                }
            }
            else
            {
                mHP = gethostbyname(ipAddress.mString.c_str());
                if(mHP == NULL)
                {
                    // Failed to lookup host name.
                    cout << "UDP ERROR: hostname lookup failure. ERRNO: " << errno << "-" << strerror(errno) << "\n";
                    Shutdown();
                    return 0;
                }
                mComputer = *((long *) mHP->h_addr);
                mSendAddr.sin_addr.s_addr =  mComputer;

            }

            options |= setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            if(options != 0)
            {
                cout << "UDP ERROR: Failed to set UDP socket options.\n";
                Shutdown();
                return 0;
            }

            // Lookup port we bound to.
            #ifdef WIN32
            int length = sizeof(mRecvAddr);
            #else
            socklen_t length = sizeof(mRecvAddr);
            #endif
            
            if(getsockname(mSocket, (struct sockaddr*)&mRecvAddr, &length) != 0)
            {
#ifdef WIN32
                int errorCode = WSAGetLastError();
                cout << "UDP ERROR: getsockname failed.  " << errorCode << "\n";
#endif
                Shutdown();
                return 0;
            }
            mSourcePort = (unsigned short)ntohs(mRecvAddr.sin_port);

            //  We have a valid socket
            mGoodSocket = true;
            mConnectionID = ipAddress;
            memcpy(&mService, &mRecvAddr, sizeof(mRecvAddr));
            mServiceLength = sizeof(mService);
        }
    }

    return mGoodSocket;

}

/*
{
    int s = 0;  // socket file descriptor.
    int on = 1;
    int bsize = 262144;

    // Clear previous connection.
    Shutdown();

    s = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( s > 0 )
    {
        //cout << "Socket initialized.\n";
        mService.sin_family = AF_INET;
        mService.sin_port = htons(port);
        if( strstr(ipAddress.mString.c_str(), "255") != 0 )
        {
            mService.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            if(setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on)) < 0)
            {
                cout << "ERROR: setsockopt failed SO_BROADCAST. ERRNO: " << errno << "-" << strerror(errno) << "\n";
                Shutdown();
                return 0;
            }
        }
        else
        {
            mHP = gethostbyname(ipAddress.mString.c_str());
            if(mHP == NULL)
            {
                // Failed to lookup host name.
                cout << "ERROR: hostname lookup failure. ERRNO: " << errno << "-" << strerror(errno) << "\n";
                Shutdown();
                return 0;
            }
            mComputer = *((long *) mHP->h_addr);
            mService.sin_addr.s_addr =  mComputer;
        }

        if(setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&bsize, (int)sizeof(bsize)) < 0)
        {
            cout << "ERROR: setsockopt failed SO_RCVBUF. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown(); return 0;
        }
        if(setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&bsize, (int)sizeof(bsize)) < 0)
        {
            cout << "ERROR: setsockopt failed SO_SNDBUF. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown(); return 0;
        }
        mConnectionID = ipAddress;
        mSocket = s;
        mGoodSocket = true;
        mDestinationPort = port;
        mServiceLength = sizeof(struct sockaddr_in);
        memcpy(&this->mSendAddr, (struct sockaddr *)(&mService), sizeof(struct sockaddr));
        memcpy(&this->mRecvAddr, (struct sockaddr *)(&mService), sizeof(struct sockaddr));
    }
    return mGoodSocket;
}
*/

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes as a socket for sending to a multicast address.
///
///   \param[in] multicastGroup The multicast group to connect to.  This is an IP 
///              address in the range of "224.0.0.0-239.255.255.255"
///   \param[in] destinationPort The port to send to.
///   \param[in] ttl Time To Live (TTL).  Default value is 1, meaning the data only
///              goes as far as the local network, increase to go further out.
///   \param[in] broadcastFlag If true, create a Broadcast Socket which is global.
///   \param[in] sourcePort The port origin port the data will be sent from.  If
///                         the value is 0, then any available port is chosen.
///                         This is also the port that the Rev methods will 
///                         listen on.
///   \param[in] allowReuse If true, then multiple udp server sockets can
///                         run on single host.  If you need multiple sockets with
///                         the same port, you should use the UdpSharedServer
///                         instead.  However, if you are only using multicast,
///                         then this is OK to be true.
///
///   \return 1 if socket created, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpClient::InitializeMulticastSocket(const IP4Address& multicastGroup,
                                         const unsigned short destinationPort,
                                         const unsigned char ttl,
                                         const bool broadcastFlag,
                                         const unsigned short sourcePort,
                                         const bool allowReuse)
{
    //  Close any previous socket
    Shutdown();
    // Clear out settings.
    memset((void *)&mService, 0, sizeof(struct sockaddr_in));
    memset((void *)&mSendAddr, 0, sizeof(struct sockaddr_in));
    memset((void *)&mRecvAddr, 0, sizeof(struct sockaddr_in));

    // Create a UDP Socket
    mSocket = (int)socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
    mDestinationPort = destinationPort;

    if(mSocket > 0)
    {
        int bufferSize = 262144;
        int on = 1;
        struct ip_mreq mreq;
        int code = 0;

        // Setup receive options for binding.
        mRecvAddr.sin_family = AF_INET;                 // IP Family
        mRecvAddr.sin_port = htons(sourcePort);         // Receive on this port (0) = any
        mRecvAddr.sin_addr.s_addr = htonl(INADDR_ANY);  // Receive on any address by default.

        // Don't allow multiple UDP Servers on single host (causes problems with unicast receiving).
        if(allowReuse == true && setsockopt(mSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on)) < 0)
        {
            cout << "UDP ERROR: setsockopt failed SO_REUSEADDR. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            Shutdown();
            return 0;
        }

        // Now try to bind the socket.
        if(bind(mSocket, (struct sockaddr *) &mRecvAddr, sizeof(mRecvAddr)) >= 0)
        {
            // Now setup send options.
            mSendAddr.sin_family = AF_INET;
            mSendAddr.sin_port = htons(mDestinationPort);

            if(mNetworkInterface >= 0)
            {
                IP4Address::List myHostnames;
                GetHostAddresses(myHostnames);
                if(mNetworkInterface < (int)(myHostnames.size()))
                {
                    mRecvAddr.sin_addr.s_addr = *((long *)gethostbyname(myHostnames[mNetworkInterface].mString.c_str())->h_addr);
                    mreq.imr_interface.s_addr = inet_addr(IP4Address(myHostnames[mNetworkInterface]).mString.c_str());
                    mSourceID = myHostnames[mNetworkInterface];
                }
                else
                {
                    mreq.imr_interface.s_addr = htonl(INADDR_ANY);
                    mSourceID.Clear(); // Any IP address.
                }
            }
            else
            {
                mreq.imr_interface.s_addr = htonl(INADDR_ANY);
            }

            mreq.imr_multiaddr.s_addr = inet_addr(multicastGroup.mString.c_str());

            if(broadcastFlag)
            {
                mSendAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
                setsockopt(mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on));
            }
            else
            {
                mHP = gethostbyname(multicastGroup.mString.c_str());
                if(mHP == NULL)
                {
                    // Failed to lookup group name.
                    cout << "Multicast Groupname Lookup Failure!\n";
                    Shutdown();
                    return 0;
                }
                mComputer = *((long *) mHP->h_addr);
                mSendAddr.sin_addr.s_addr =  mComputer;
            }
            int options = 0;
            options |= setsockopt(mSocket, SOL_SOCKET, SO_RCVBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, SOL_SOCKET, SO_SNDBUF, (char *)&bufferSize, (int)sizeof(bufferSize));
            options |= setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, (int)sizeof(ttl));
            if(options == 0)
            {
                //std::cout << "UDP ERROR: Failed to set Multicast TTL\n";
            }
            on = 1;
            options |= setsockopt(mSocket, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&on, (int)sizeof(on));
            if(options == 0)
            {
                //std::cout << "UDP ERROR: Failed to set Multicast Loopback\n";
            }
            if(options != 0)
            {
                cout << "UDP ERROR: Failed to set UDP socket options.\n";
                Shutdown();
                return 0;
            }
            options |= setsockopt(mSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&mreq, sizeof(mreq));
            if(options != 0)
            {
                cout << "UDP ERROR: Failed to set UDP Multicast Add Membership.\n";
                std::cout << strerror(errno) << std::endl;
            }
            
            #ifdef WIN32
            int length = sizeof(mRecvAddr);
            #else
            socklen_t length = sizeof(mRecvAddr);
            #endif
            
            if(getsockname(mSocket, (struct sockaddr*)&mRecvAddr, &length) != 0)
            {
#ifdef WIN32
                int errorCode = WSAGetLastError();
                cout << "UDP ERROR: getsockname failed.  " << errorCode << "\n";
#endif
                Shutdown();
                return 0;
            }
            mSourcePort = (unsigned short)ntohs(mRecvAddr.sin_port);

            //  We have a valid socket
            mGoodSocket = true;
            mConnectionID = multicastGroup;
            memcpy(&mService, &mRecvAddr, sizeof(mRecvAddr));
            mServiceLength = sizeof(mService);
        }
    }

    return mGoodSocket;

}
/*
{
    int s = 0;  // socket file descriptor.
    int on = 1;
    int bsize = 262144;
    unsigned char one = 1;

    // Clear previous connection.
    Shutdown();

    s = (int)socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if( s > 0 )
    {
        //cout << "Socket initialized.\n";
        mService.sin_family = AF_INET;
        mService.sin_port = htons(port);
        if( broadcastFlag )
        {
            mService.sin_addr.s_addr = htonl(INADDR_BROADCAST);
            setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on));
        }
        else
        {
            mHP = gethostbyname(multicastGroup.mString.c_str());
            if(mHP == NULL)
            {
                // Failed to lookup group name.
                cout << "groupname Lookup Failure!\n";
                Shutdown();
                return 0;
            }
            mComputer = *((long *) mHP->h_addr);
            mService.sin_addr.s_addr =  mComputer;
        }

        setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char *)&bsize, (int)sizeof(bsize));
        setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char *)&bsize, (int)sizeof(bsize));
        setsockopt(s, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&ttl, (int)sizeof(unsigned char));
        setsockopt(s, IPPROTO_IP, IP_MULTICAST_LOOP, (char *)&one, (int)sizeof(unsigned char));

        mConnectionID = multicastGroup;
        mSocket = s;
        mGoodSocket = true;
        mDestinationPort = port;
        mServiceLength = sizeof(struct sockaddr_in);
        memcpy(&this->mSendAddr, (struct sockaddr *)(&mService), sizeof(struct sockaddr));
        memcpy(&this->mRecvAddr, (struct sockaddr *)(&mService), sizeof(struct sockaddr));
        struct sockaddr_in in;
        memcpy(&in, &mService, sizeof(struct sockaddr_in));
        mSourcePort = (unsigned short)in.sin_port;
    }
    return mGoodSocket;
}
*/


/** Creates a connection to another destination from the currently bound
    socket source port to a new destination port and IP address. */
UdpClient* UdpClient::CreateNewDestination(const IP4Address& ipAddress,
                                           const unsigned short destinationPort) const
{
    if(IsValid() == false)
    {
        return NULL;
    }

    UdpClient* client = new UdpClient();
    // Copy data.
    client->mSocket = mSocket;
    client->mServiceLength = mServiceLength;
    client->mNetworkInterface = mNetworkInterface;
    client->mGoodSocket = true;
    client->mSourcePort = mSourcePort;
    client->mDestinationPort = mDestinationPort;
    memcpy(&client->mRecvAddr, &mRecvAddr, sizeof(sockaddr_in));
    memcpy(&client->mService, &mService, sizeof(mService));
    memset((void *)&client->mSendAddr, 0, sizeof(struct sockaddr_in));

    // Now setup send options.
    client->mSendAddr.sin_family = AF_INET;
    if(destinationPort == 0)
    {
        client->mDestinationPort = mDestinationPort;
    }
    else
    {
        client->mDestinationPort = destinationPort;
    }
    client->mSendAddr.sin_port = htons(client->mDestinationPort);
    int on = 1;
    if( strstr(ipAddress.mString.c_str(), "255") != 0 )
    {
        client->mSendAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
        if(setsockopt(client->mSocket, SOL_SOCKET, SO_BROADCAST, (char *)&on, sizeof(on)) < 0)
        {
            cout << "UDP ERROR: setsockopt failed SO_BROADCAST. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            delete client;
            return NULL;
        }
    }
    else
    {
        client->mHP = gethostbyname(ipAddress.mString.c_str());
        if(client->mHP == NULL)
        {
            // Failed to lookup host name.
            cout << "UDP ERROR: hostname lookup failure. ERRNO: " << errno << "-" << strerror(errno) << "\n";
            delete client;
            return NULL;
        }
        client->mComputer = *((long *) client->mHP->h_addr);
        client->mSendAddr.sin_addr.s_addr =  client->mComputer;
    }
    client->mConnectionID = ipAddress;
    client->mDuplicateSocket = true;
    client->mGoodSocket = true;

    return client;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends information in the buffer over the socket.
///
///   \param[in] buffer Character buffer with data to send.
///   \param[in] length The size off buff/maximum number of bytes to try send.
///
///   \return The number of bytes sent, < 0 on error.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpClient::SendFromBuffer(const char *buffer,
                              const unsigned int length) const
{
    int result = 0;

    if(!IsValid())
    {
        return 0;
    }

    result = ::sendto(mSocket,
                      buffer,
                      (int)(length),
                      0,
                      (struct sockaddr *) &mSendAddr,
                      sizeof(struct sockaddr));
    return result;
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
int UdpClient::RecvToBuffer(char *buffer,
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

    sockaddr *r;

    r = (sockaddr *)(&mRecvAddr);
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
