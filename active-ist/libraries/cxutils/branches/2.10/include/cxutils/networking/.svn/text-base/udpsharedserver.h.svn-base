////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpsharedserver.h
///  \brief This file contains software for creating a shared UDP Server service.
///  This type of socket should only be used if you are creating software that
///  requires two applications running on the same host machine to receive
///  Unicast or Unicast and Multicast data simultaneously.  If you don't use
///  this class, and you have two applications running at the same time on a
///  single machine and both applications are receiving UDP data on a port, only
///  one of the applications will receive unicast messages sent to your host
///  machine.  With this class, you can avoid this issue as long as you don't
///  have other software running that is not using this class to receive UDP
///  data on a specific port.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 March 2009
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
#ifndef __CXUTILS_UDP_SHARED_SERVER__H
#define __CXUTILS_UDP_SHARED_SERVER__H

#include "cxutils/networking/udpserver.h"
#include "cxutils/ipc/messageserver.h"
#include "cxutils/ipc/messageclient.h"
#include "cxutils/thread.h"
#include "cxutils/circulararray.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class UdpSharedServer
    ///   \brief UDP Server software that can be used to run multiple applications
    ///   that can receive unicast UDP messages simultaneously.
    ///
    ///   This class/software was developed to deal with an issue in UDP communication.
    ///   Normally, if you have to applications running on a single computer, and 
    ///   both are running a UDP Server on the same port number, only one of the 
    ///   processes will receive unicast messages sent to the computer.  This software
    ///   removes this restriction using a shared memory to communicate between
    ///   this applications and share UDP traffic received.  Therefore, if you use this
    ///   code in your program, you can run your application multiple times on a single
    ///   machine and all applications will receive unicast and multicast/broadcast
    ///   UDP data sent to your computer.
    ///
    ///   If you use any other software that receives UDP data on the port your are
    ///   using then this software will not function as expected.  In order to work,
    ///   all of your applications receiving on the same UDP port must use this
    ///   code. Therefore, this software should only be used in very specific 
    ///   instances and custom applications/environments.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL UdpSharedServer
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Info
        ///   \brief Data structure to store socket information.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Info
        {
        public:
            static const unsigned int Size = 18;    ///<  Size of info header in memory.
            Info();
            Info(const Info& info);
            ~Info();
            Info& operator=(const Info& info);
            int mNetworkInterface;                  ///<  Network interface number to use (eth0, wifi, etc.)
            IP4Address mMulticastGroup;             ///<  Multicast group to listen on. [224.0.0.0-239.255.255.255].
            unsigned short mPort;                   ///<  Port number to use.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used for getting UDP messages received by the
        ///    shared server socket.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Callback
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Data
            ///   \brief Small structure for storing function callback data.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CX_UTILS_DLL Data
            {
            public:
                Data();
                Data(void (*callback)(const Packet& packet, 
                                      const IP4Address& ipAddress, 
                                      void* args), 
                     void* cbArgs = NULL);
                Data(const Data& data);
                ~Data();
                void Run(const Packet& packet, const IP4Address& ipAddress);
                Data& operator=(const Data& data);
                void (*mpCallback)(const Packet& packet, const IP4Address& ipAddress, void* args);
                void *mpCallbackArgs;
            };
            virtual void ProcessUDP(const Packet& packet, 
                                    const IP4Address& ipAddress) {};
            virtual void ProcessUDP(const Packet& packet, 
                                    const IP4Address& ipAddress,
                                    const unsigned short sourcePort) {}
        };
        UdpSharedServer();
        ~UdpSharedServer(); 
        int InitializeSocket(const unsigned short port,
                             const IP4Address& multicastGroup = "224.0.0.1",
                             const int netInterface = -1);
        void Shutdown();
        int GetNetworkInterface() const;
        bool RegisterCallback(Callback* callback);
        bool RegisterCallback(const Callback::Data& callback);
        void RemoveCallback(Callback* callback);
        void RemoveCallback(const Callback::Data& callback);
        bool IsValid() const;
        bool IsPrimary() const;
        bool IsConnected() const { return mConnectedFlag; }
        bool Recv(Packet& packet, IP4Address& ipAddress);  
        void SetUpdateDelayMs(const unsigned int delayTimeMs = 0);
    private:
        static void UdpStatusThread(void* udpSharedServer);
        static void UdpSocketThread(void* udpSharedServer);        
        static void ProcessUdpMessage(Packet& udpMessage, void* udpSharedServer);
        volatile bool mPrimaryFlag;             ///<  If true, than this instance is the active UDP Server.
        volatile bool mConnectedFlag;           ///<  Are we connected on a socket or to a server?
        volatile unsigned int mUpdateDelayMs;   ///<  Update delay value for thread in milliseconds.
        Thread mUdpStatusThread;        ///<  Thread object for running the UdpStatusThread method.
        Thread mUdpSocketThread;        ///<  Thread object for running UDP socket receiving (UdpServerThread method).
        UdpServer* mpSocket;            ///<  Pointer to UDP Socket if in use.
        MessageServer* mpMessageServer; ///<  Message server to distribute received UDP messages.
        MessageClient* mpMessageClient; ///<  Message client to get messages from the active socket process.
        Info mSocketInfo;               ///<  Socket settings.
        Mutex mCallbacksMutex;          ///<  Mutex for callacks.
        Mutex mDelayMutex;              ///<  Mutex for thread protection of sleep value.
        std::set<Callback*> mCallbacks; ///<  Callbacks for received messages.
        std::vector<Callback::Data> mFunctionCallbacks; ///<  Function callback data.
        volatile bool mQuitServerFlag;  ///<  And extra flag to signal need to exit.
        Mutex mQueueMutex;              ///<  Mutex for thread protection of queue.
        CircularArray<IP4Address> mMessageAddressQueue;   ///<  Queue of IP addresses for messages received.
        CircularArray<Packet> mPacketQueue;               ///<  Queue of last few messages received.
    };
}


#endif
/* End of File */
