////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpserver.h
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
#ifndef __CXUTILS_SOCKET_UDP_SERVER_H
#define __CXUTILS_SOCKET_UDP_SERVER_H

#include "cxutils/networking/socket.h"
#include "cxutils/thread.h"
#include <string>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class UdpServer
    ///   \brief Socket structure for receiving and UDP/IP messages.
    ///
    ///   Just like UdpClient, UdpServer is one way communication and can only be
    ///   used to receive UDP data.  To transmit you must us a UdpClient.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL UdpServer : public Socket
    {
    public:
        UdpServer();
        virtual ~UdpServer();
        int InitializeSocket(const unsigned short port);
        int InitializeMulticastSocket(const unsigned short port,
                                      const IP4Address& multicastGroup /*= "224.0.0.0" */,
                                      const bool allowReuse = false);
        virtual void Shutdown();
    private:
        virtual int SendFromBuffer(const char* buffer, 
                                   const unsigned int length) const { return 0; }
        virtual int RecvToBuffer(char* buffer, 
                                 const unsigned int length, 
                                 const long int timeOutMilliseconds = 0, 
                                 IPAddress* ipAddress = NULL,
                                 unsigned short* port = NULL) const;
        int mPort;                  ///<  Port number to use.
        long mComputer;             ///<  The computer identification number on network.
        struct hostent* mHP;        ///<  Used for converting computer name/ip address to integer value.
        struct sockaddr_in mSendAddr;  ///<  Address to send to on network.
        struct sockaddr_in mRecvAddr;  ///<  Address data received from.
        IP4Address mRecvAddress;       ///<  Network address to receive data on.
    };

    typedef UdpServer UdpReceiver;  ///<  Type defintiion.
}

#endif
/*  End of File */
