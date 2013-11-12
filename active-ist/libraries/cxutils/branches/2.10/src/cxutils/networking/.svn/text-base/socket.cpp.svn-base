////////////////////////////////////////////////////////////////////////////////////
///
///  \file socket.cpp
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
#include "cxutils/networking/socket.h"
#include <string.h>
#include <iostream>

#ifdef WIN32
#else
#include <stdio.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

using namespace CxUtils;

#ifdef WIN32
unsigned int Socket::mWinsockInitCount = 0;
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Default constructor, initializes any shared libraries such
///   as winsock in Windows.
///
////////////////////////////////////////////////////////////////////////////////////
Socket::Socket()
{
    mSocket =  0;
    mNetworkInterface = -1;
    mSocketType = Undefined;
    mServiceLength = 0;
    memset(&mService, 0, sizeof(struct sockaddr_in));
    mGoodSocket = false;

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
///   \brief Copy constructor, initializes any shared libraries such
///   as winsock in Windows.
///
////////////////////////////////////////////////////////////////////////////////////
Socket::Socket(const Socket& arg)
{
    mSocket =  0;
    mNetworkInterface = -1;
    mSocketType = Undefined;
    mServiceLength = 0;
    memset(&mService, 0, sizeof(struct sockaddr_in));
    mGoodSocket = false;

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

    *this = arg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor, checks for the deinitialization of winsock dll in Windows.
///
////////////////////////////////////////////////////////////////////////////////////
Socket::~Socket()
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
///   \brief Closes the socket, and clears service information.
///
////////////////////////////////////////////////////////////////////////////////////
void Socket::Shutdown()
{

#ifdef WIN32
    shutdown(mSocket, 1);
    closesocket(mSocket);
#else
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
    mServiceLength = 0;
    memset(&mService, 0, sizeof(struct sockaddr_in));
    this->mGoodSocket = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the data in the packet structure over the socket.
///
///   \param[in] packet The packet data to send.
///
///   \return Number of bytes sent, < 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::Send(const Packet &packet) const
{
    return SendFromBuffer(((const char *)(packet.Ptr())), packet.Size());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the data in the byte array over the socket.
///
///   \param[in] buffer Pointer to buffered data to transmit.
///   \param[in] length The length of the buffer in bytes.
///
///   \return Number of bytes sent, < 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::Send(const char* buffer, const unsigned int length) const
{
    return SendFromBuffer(buffer, length);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Receives data and puts it in the packet.
///
///   \param[in] packet The packet data to put received data into.
///   \param[in] receiveSize The number of bytes to try and receive.
///   \param[in] timeOutMilliseconds The timeout in milliseconds, 0 = INFINITE.
///   \param[out] ipAddress If the socket you are using supports retriving the IP
///               address of the sender (UDP), then that value will be saved
///               here if not NULL.
///   \param[out] port The senders source port the sent from.  Optional
///                    parameter set if not NULL, only works with UDP sockets.
///
///   \return Number of bytes received,  < 0 on error.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::Recv(Packet &packet, 
                 const unsigned int receiveSize, 
                 const long int timeOutMilliseconds,
                 IPAddress* ipAddress,
                 unsigned short* port) const
{
    int results = 0;
    packet.Clear(false);
    packet.Reserve(receiveSize);
    if( (results = RecvToBuffer((char *)(packet.Ptr()), 
                                receiveSize, 
                                timeOutMilliseconds,
                                ipAddress, 
                                port)) > 0) 
    {
        packet.SetLength((unsigned int)results);
        packet.SetWritePos(0);
        packet.SetReadPos(0);
        return results;
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Receives data and puts it in the buffer.
///
///   \param[in] buffer Buffer to put received data in.
///   \param[in] length Maximum amount of data to receive in bytes.
///   \param[in] timeOutMilliseconds The timeout in milliseconds, 0 = INFINITE.
///   \param[out] ipAddress If the socket you are using supports retriving the IP
///               address of the sender (UDP), then that value will be saved
///               here if not NULL.
///   \param[out] port The senders source port the sent from.  Optional
///                    parameter set if not NULL, only works with UDP sockets.
///
///   \return Number of bytes received,  <  0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::Recv(char* buffer, const unsigned int length, 
                 const long int timeOutMilliseconds,
                 IPAddress* ipAddress,
                 unsigned short* port) const
{
    return RecvToBuffer(buffer, length, timeOutMilliseconds, ipAddress, port);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns true if a valid socket has been created.
///
////////////////////////////////////////////////////////////////////////////////////
bool Socket::IsValid() const
{
    return mGoodSocket;
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
bool Socket::SetNetworkInterface(const int num)
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
bool Socket::SetNetworkInterface(const IP4Address& ipAddress)
{
    int num = GetNetworkInterface(ipAddress);
    if(num >= 0)
    {
        return SetNetworkInterface(num);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method looks up the network connection number based on the IP
///   address.
///
///   \param ipAddress The IP Address of an existing connection to get number
///                    of.
///
///   \return -1 on failure, 0,N where N is the Nth network connection on the host.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::GetNetworkInterface(const IP4Address& ipAddress)
{
    IP4Address::List hostnames;
    if(GetHostAddresses(hostnames))
    {
        IP4Address::List::iterator host;
        int num = 0;
        for(host = hostnames.begin(); host != hostnames.end(); host++)
        {
            if(*host == ipAddress)
            {
                return num;
            }
            num++;
        }
    }

    return -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the socket file descriptor data.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::GetSocket() const
{
#ifdef WIN32
    return (int)mSocket;
#else
    return mSocket;
#endif
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns what type of socket the file descripter is.
///
///   \return Undefined,
///           TCPClient,
///           TCPListen,
///           TCPServer,
///           UDPClient,
///           UDPServer.
///
////////////////////////////////////////////////////////////////////////////////////
Socket::Type Socket::GetType() const
{
    return mSocketType;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the interface number to use. -1 means any interface.
///          Interface numbers are indexed with 0 being the first, 1 the second,
///          etc.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::GetNetworkInterface() const { return mNetworkInterface; }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Discovers the number of IP address for the different network
///   connections on the host machine.
///
///   \param[out] ipAddresses List of IP4 Addresses detected.
///
///   \return Number of address found, 0 if no network interface present.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Socket::GetHostAddresses(IP4Address::List& ipAddresses)
{
    ipAddresses.clear();

#ifdef WIN32
    bool result = false;
    char ac[512];
    if(Socket::mWinsockInitCount == 0)
    {
        Socket::InitializeSockets();
    }
    memset(ac, 0, 512);
    if(gethostname(ac, 512) == 0) 
    {
        struct hostent *phe = gethostbyname(ac);
        if(phe != 0) 
        {
            struct in_addr addr;
            for(int i = 0; phe->h_addr_list[i] != 0; i++) 
            {
                memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr));
                ipAddresses.push_back( IP4Address(std::string(inet_ntoa(addr))));
            }
        }
    }
#else
    struct ifreq   buffer[32];
    struct ifconf  intfc;
    struct ifreq  *pIntfc;
    int            i, fd, num_intfc;

    intfc.ifc_len = sizeof(buffer);
    intfc.ifc_buf = (char*) buffer;

    if ((fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        perror("socket() failed");
        return false;
    }

    if (ioctl(fd, SIOCGIFCONF, &intfc) < 0)
    {
        perror("ioctl SIOCGIFCONF failed");
        return false;
    }

    pIntfc    = intfc.ifc_req;
    num_intfc = intfc.ifc_len / sizeof(struct ifreq);

    for (i = 0; i < num_intfc; i++)
    {
        struct ifreq *item = &(pIntfc[i]);
        ipAddresses.push_back( IP4Address(std::string(inet_ntoa(((struct sockaddr_in *)&item->ifr_addr)->sin_addr))) );
    }
    close(fd);
#endif

    return (unsigned int)ipAddresses.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Discovers the number of physical device interfaces on host
///   machine (number of NIC connections).
///
///   \return Number of address found.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Socket::GetNumNetworkConnections()
{
    IP4Address::List ipAddresses;
    return GetHostAddresses(ipAddresses);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if an IP address is on the local machine.
///
///   \param[in] ipAddress IP Address to check against list of local addresses.
///
///   \return True if the IP address is part of the local host machine, otherwise
///   false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Socket::IsHostAddress(const IP4Address& ipAddress)
{
    IP4Address::List ipAddresses;
    if(ipAddress.mData[0] == 127 && ipAddress.mData[1] == 0 && ipAddress.mData[2] == 0)
    {
        return true;
    }
    if(GetHostAddresses(ipAddresses))
    {
        for(unsigned int i = 0; i < (unsigned int)ipAddresses.size(); i++)
        {
            if(ipAddresses[i] == ipAddress)
            {
                return true;
            }
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks the socket to see if there is any incomming data.
///
///   \param sock The socket to check for incomming data. 
///   \param tms Time to wait in milliseconds.  If 0, then method
///              always returns 1 to support blocked receiving.
///
///   \return 0 if no data incomming, 1 if data is ready for reading.
///
////////////////////////////////////////////////////////////////////////////////////
int Socket::IsIncommingData(const Socket *sock, const long int tms)
{
    if(!sock || !sock->mGoodSocket)
        return 0;

    if(tms == 0)
        return 1;

#ifdef WIN32
    fd_set fds;
    timeval tval;

    //  Populate a timeval structure
    tval.tv_sec = 0;
    tval.tv_usec = tms*1000;

    fds.fd_array[0] = ((SOCKET)(sock->mSocket));
    fds.fd_count = 1;

    ::select(0, &fds, NULL, NULL, &tval);
    if(fds.fd_count != 1)
        return 0;

    return 1;
#else
    fd_set fds;
    int s;
    timeval tval;
    s = sock->mSocket;

    FD_ZERO(&fds);
    FD_SET(s, &fds);

    //  Populate a timeval structure
    tval.tv_sec = 0;
    tval.tv_usec = tms*1000;

    select(s + 1, &fds, (fd_set *)0, (fd_set *)0,
           (struct timeval *)&tval);
    if(FD_ISSET(s, &fds) != 0)   //  Is the file descripter set for s?
        return 1;

    return 0;
#endif

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Operator overloading for operator =
///
////////////////////////////////////////////////////////////////////////////////////
Socket &Socket::operator = (const Socket &arg)
{
    this->mSocket = arg.mSocket;
    this->mServiceLength = arg.mServiceLength;
    this->mSocketType = arg.mSocketType;
    memcpy(&this->mService, &arg.mService, sizeof(struct sockaddr_in));

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Makes sure any socket shared libraries have been loaded.
///
////////////////////////////////////////////////////////////////////////////////////
void Socket::InitializeSockets()
{
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


/*  End of File */
