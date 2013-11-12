////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpclient.h
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
#ifndef __CXUTILS_SOCKET_UDP_CLIENT_H
#define __CXUTILS_SOCKET_UDP_CLIENT_H

#include "cxutils/cxbase.h"
#include "cxutils/networking/socket.h"
#include "cxutils/mutex.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class UdpClient
    ///   \brief Socket structure sending UDP messages.
    ///
    ///   UdpClient is a one way socket which can only be used for transmision of 
    ///   packet data over UDP/IP.  It does not bind to the socket, therefore it 
    ///   cannot receive any data.  To receive UDP data, use the UdpServer class.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL UdpClient : public Socket
    {
        friend class UdpServer;
    public:
        UdpClient();
        virtual ~UdpClient();
        int InitializeSocket(const IP4Address& ipAddress, 
                             const unsigned short destinationPort,
                             const unsigned short sourcePort = 0);
        int InitializeMulticastSocket(const IP4Address& multicastGroup,
                                      const unsigned short destinationPort,
                                      const unsigned char ttl = 16,
                                      const bool broadcastFlag = false,
                                      const unsigned short sourcePort = 0,
                                      const bool allowReuse = true);
        virtual void Shutdown();      
        inline IP4Address GetConnectionAddress() const { return mConnectionID; }
        inline unsigned short GetDestinationPort() const { return mDestinationPort; }
        inline unsigned short GetSourcePort() const { return mSourcePort; }
        UdpClient* CreateNewDestination(const IP4Address& ipAddress,
                                        const unsigned short destinationPort = 0) const;
    private:
        virtual int SendFromBuffer(const char* buffer, 
                                   const unsigned int length) const;
        virtual int RecvToBuffer(char* buffer, 
                                 const unsigned int length, 
                                 const long int timeOutMilliseconds = 0, 
                                 IPAddress* ipAddress = NULL,
                                 unsigned short* port = NULL) const;
        int mDestinationPort;       ///<  Destination port number to use for sending.
        int mSourcePort;            ///<  Source port for sending.
        long mComputer;             ///<  The computer identification number on network.
        struct hostent* mHP;        ///<  Used for converting computer name/ip address to integer value.
        struct sockaddr_in mSendAddr;  ///<  Address to send to on network.
        struct sockaddr_in mRecvAddr;  ///<  Address data received from.
        IP4Address mConnectionID;      ///<  ID of the IP address we are connected to.
        bool mDuplicateSocket;         ///<  Duplicate socket ID.
    };
    typedef UdpClient UdpSender;       ///<  Type defintiion.
}

#endif
/*  End of File */
