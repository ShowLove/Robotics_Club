////////////////////////////////////////////////////////////////////////////////////
///
///  \file udpsharedserver.cpp
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
#include "cxutils/networking/udpsharedserver.h"
#include "cxutils/timer.h"
#include "cxutils/time.h"
#include <iostream>

using namespace std;
using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Info::Info() 
{
    mNetworkInterface = -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Info::Info(const UdpSharedServer::Info& info) 
{
    *this = info;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Info::~Info() 
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Info& UdpSharedServer::Info::operator=(const UdpSharedServer::Info& info) 
{
    mNetworkInterface = info.mNetworkInterface;
    mPort = info.mPort;
    mMulticastGroup = info.mMulticastGroup;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Callback::Data::Data()
{
    mpCallback = NULL;
    mpCallbackArgs = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes callback functions and arguments.
///
///   \param[in] callback Function pointer to callback to be run when UDP messages
///                       are received.
///   \param[in] cbArgs Pointer to additional callack arguments.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Callback::Data::Data(void (*callback)(const Packet& packet,
                                                       const IP4Address& ipAddress,
                                                       void* args),
                                      void* cbArgs)
{
    mpCallback = callback;
    mpCallbackArgs = cbArgs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Callback::Data::Data(const UdpSharedServer::Callback::Data& data)
{
    *this = data;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Callback::Data::~Data()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs the Function Callback.
///
///   \param[in] packet UDP message to process in callback.
///   \param[in] ipAddress IP Address of sender of UDP message.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::Callback::Data::Run(const Packet& packet, const IP4Address& ipAddress)
{
    if(mpCallback)
    {
        mpCallback(packet, ipAddress, mpCallbackArgs);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::Callback::Data& UdpSharedServer::Callback::Data::operator=(const UdpSharedServer::Callback::Data& data)
{
    mpCallback = data.mpCallback;
    mpCallbackArgs = data.mpCallbackArgs;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::UdpSharedServer() : mPrimaryFlag(false),
                                     mConnectedFlag(false),
                                     mUpdateDelayMs(0),
                                     mpSocket(NULL),
                                     mpMessageServer(NULL),
                                     mpMessageClient(NULL),
                                     mQuitServerFlag(false)
{
    mPacketQueue.reserve(15);
    mMessageAddressQueue.reserve(15);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UdpSharedServer::~UdpSharedServer()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes a shared UDP socket on a port with a specific
///          network interface.  This method also will join a multicast group
///          to get multicast messages.
///
///   If an existing UDP Shared Server is running on your system with
///   the same port, but a different multicastGroup (or is unicast) then
///   this method will fail.
///
///   \param[in] port The port to receive on.
///   \param[in] multicastGroup The multicast group to join.  Value 
///                             values are "224.0.0.1-239.255.255.255."
///   \param[in] netInterface The network interface to receive on (which
///                           network card, etc.)  A value of -1 indicates
///                           any interface (default).
///
///   \return 1 on success, 0 on failure.
///
/////////////////////////////////////////////////////////////////////////////////
int UdpSharedServer::InitializeSocket(const unsigned short port, 
                                      const IP4Address& multicastGroup,
                                      const int netInterface)
{
    int result = 0;

    Shutdown();
    mConnectedFlag = false;
    mQuitServerFlag = false;

    mSocketInfo.mPort = port;
    mSocketInfo.mNetworkInterface = netInterface;
    mSocketInfo.mMulticastGroup = multicastGroup;

    // Create thread to initialize appropriate connections.
    if(mUdpStatusThread.CreateThread(UdpSharedServer::UdpStatusThread, this))
    {
        mUdpStatusThread.SetThreadName("UdpSharedServer");
        result = 1;
    }

    // Do cleanup on failure.
    if(result == 0)
    {
        Shutdown();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the UDP Server.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::Shutdown()
{
    mQuitServerFlag = true;  
    mUdpStatusThread.StopThread();
    mUdpSocketThread.StopThread();
 
    if(mpMessageServer)
    {
        mpMessageServer->Shutdown();
        delete mpMessageServer;
        mpMessageServer = NULL;
    }
    
    if(mpMessageClient)
    {
        mpMessageClient->Shutdown();
        delete mpMessageClient;
        mpMessageClient = NULL;
    }

    mPrimaryFlag = false;
    mSocketInfo.mMulticastGroup.SetAddress("0.0.0.0");
    mSocketInfo.mNetworkInterface = -1;
    mSocketInfo.mPort = 0;
    
    mQuitServerFlag = false;
    
    if(mpSocket)
    {
        mpSocket->Shutdown();
        delete mpSocket;
        mpSocket = NULL;
    }
    mConnectedFlag = false;
    mPacketQueue.clear();
    mMessageAddressQueue.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The network interface being used for UDP receiving. -1 is any.
///
////////////////////////////////////////////////////////////////////////////////////
int UdpSharedServer::GetNetworkInterface() const
{
    return mSocketInfo.mNetworkInterface;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to be run whenever a new message is
///          received.
///
///   \param[in] callback Pointer to callback to be run.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UdpSharedServer::RegisterCallback(Callback* callback)
{
    bool result = false;
    std::set<Callback*>::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbacks.find(callback);
    if(cb == mCallbacks.end())
    {
        result = true;
        mCallbacks.insert(callback);
    }
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a function callback to be run whenever a new message is
///          received.
///
///   \param[in] callback Function callback data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UdpSharedServer::RegisterCallback(const Callback::Data& callback)
{
    bool result = false;
    std::vector<Callback::Data>::iterator cb;
    mCallbacksMutex.Lock();
    for(cb = mFunctionCallbacks.begin();
        cb != mFunctionCallbacks.end();
        cb++)
    {
        if(cb->mpCallback == callback.mpCallback &&
           cb->mpCallbackArgs == callback.mpCallbackArgs)
           {
               break;
           }
    }
    if(cb == mFunctionCallbacks.end())
    {
        result = true;
        mFunctionCallbacks.push_back(callback);
    }
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a registered callback.
///
///   \param[in] callback Callback data to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::RemoveCallback(Callback* callback)
{
    std::set<Callback*>::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbacks.find(callback);
    if(cb != mCallbacks.end())
    {
        mCallbacks.erase(callback);
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a registered callback.
///
///   \param[in] callback Callback data to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::RemoveCallback(const Callback::Data& callback)
{
    std::vector<Callback::Data>::iterator cb;
    mCallbacksMutex.Lock();
    for(cb = mFunctionCallbacks.begin();
        cb != mFunctionCallbacks.end();
        cb++)
    {
        if(cb->mpCallback == callback.mpCallback &&
           cb->mpCallbackArgs == callback.mpCallbackArgs)
           {
               mFunctionCallbacks.erase(cb);
               break;
           }
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if server is running and valid.
///
////////////////////////////////////////////////////////////////////////////////////
bool UdpSharedServer::IsValid() const
{
    if(mUdpStatusThread.IsThreadActive())
        return true;
        
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Return true if this instance is the active UDP socket, otherwise
///          false.
///
////////////////////////////////////////////////////////////////////////////////////
bool UdpSharedServer::IsPrimary() const
{
    return mPrimaryFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Whenever a message is recieved it is added to a queue or shared
///   through a callback.  This function pulls a message from the queue in FIFO
///   order.
///
///   \param[out] packet The message data received.
///   \param[out] ipAddress IP address of data sender.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UdpSharedServer::Recv(Packet& packet, IP4Address& ipAddress)
{
    bool result = false;

    mQueueMutex.Lock();
    if(mPacketQueue.size() > 0)
    {
        packet = *mPacketQueue.begin();
        ipAddress = *mMessageAddressQueue.begin();
        mPacketQueue.pop_front(NULL);
        mMessageAddressQueue.pop_front(NULL);
        result = true;
    }
    mQueueMutex.Unlock();

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets delay in polling for incomming messages loop.
///
///   \param[in] delayTimeMs Sets the time between checking for messages.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::SetUpdateDelayMs(const unsigned int delayTimeMs)
{
    Mutex::ScopedLock lock(&mDelayMutex);
    mUpdateDelayMs = delayTimeMs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread used to determine if this Server should become active by
///          creating a new UDPServer socket, and also makes sure that the
///          server has an active shared memory client connection to receive 
///          data from the active server.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::UdpStatusThread(void* udpSharedServer)
{
    UdpSharedServer* server = (UdpSharedServer*)udpSharedServer;
    char messageServerName[512];
    
    sprintf(messageServerName, "%d", server->mSocketInfo.mPort);
            
    while(server && 
          (server->mUdpStatusThread.QuitThreadFlag() == false ||
           server->mQuitServerFlag == false))
    {
        // Check to see if the server is active.
        if(server->mPrimaryFlag == false && server->mpSocket == NULL && MessageServer::IsServerPresent(messageServerName) == false)
        {
            // Try to initialize a UDP Server Socket, if another is present, then this will fail
            server->mpSocket = new UdpServer();
            server->mpSocket->SetNetworkInterface(server->mSocketInfo.mNetworkInterface);
            // Check if this is a multicast socket or not.
            if(server->mSocketInfo.mMulticastGroup.mString >= "224.0.0.0" && 
               server->mSocketInfo.mMulticastGroup.mString <= "239.255.255.255")
            {
                server->mpSocket->InitializeMulticastSocket(server->mSocketInfo.mPort,
                                                            server->mSocketInfo.mMulticastGroup);
            }
            else
            {
                server->mpSocket->InitializeSocket(server->mSocketInfo.mPort);
            }
            
            if(server->mpSocket->IsValid())
            {
                // See if we can take ownership as the primary
                server->mpMessageServer = new MessageServer();
                // This will only succeed if there is no active other server.
                if(server->mpMessageServer->Initialize(messageServerName, MessageServer::DefaultMaxClients))
                {
                    server->mPrimaryFlag = true; // SUCCESS!
                    // If we were a client, shutdown interface
                    if(server->mpMessageClient)
                    {
                        delete server->mpMessageClient;
                        server->mpMessageClient = NULL;
                        
                    }
                    // Create a thread to receive data from the socket.
                    server->mUdpSocketThread.CreateThread(UdpSharedServer::UdpSocketThread, server);
                    //server->mUdpSocketThread.SetThreadPriority(50);
                    server->mUdpSocketThread.SetThreadName("UdpSocketServer");      
                }
            }

            if(server->mPrimaryFlag == false)
            {
                // Shutdown and delete servers
                if(server->mpSocket)
                {
                    delete server->mpSocket;
                }
                if(server->mpMessageServer)
                {
                    delete server->mpMessageServer;
                }
                server->mpSocket = NULL;
                server->mpMessageServer = NULL;
            }
        }
        // Make sure we have an active shared memory client interface running
        // otherwise we won't receive any messages!
        if(server->mPrimaryFlag == false && server->mpMessageClient == NULL)
        {
            server->mpMessageClient = new MessageClient();
            server->mpMessageClient->SetUpdateDelayMs(server->mUpdateDelayMs);
            if(server->mpMessageClient->Initialize(messageServerName, MessageClient::AnyID) == 0)
            {
                delete server->mpMessageClient;
                server->mpMessageClient = NULL;
            }
            else
            {
                // Register callback to get messages as they arrive
                server->mpMessageClient->RegisterCallback(MessageClient::Callback::Function(UdpSharedServer::ProcessUdpMessage, server));
            }
        }

        SleepMs(250);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread used by the primary server (the active socket) to receive
///   UDP datagrams and share to clients.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::UdpSocketThread(void* udpSharedServer)
{
    UdpSharedServer* server = (UdpSharedServer*)udpSharedServer;
    Packet udpMessage;
    IP4Address sourceAddress;
    unsigned short sourcePort = 0;
#ifdef WIN32
    unsigned int loopCounter = 0;
#endif

    while(server && 
          (server->mUdpSocketThread.QuitThreadFlag() == false ||
           server->mQuitServerFlag == false))
    {
        // Try and read a new incomming message.
        udpMessage.Clear(false);
        sourcePort = 0;
        if(server->mpSocket->Recv(udpMessage, 5000, 100, &sourceAddress, &sourcePort) > 0)
        {
            // Add the IP address to the end of the message for shared memory clients.
            udpMessage.Write(sourceAddress.mData, 4, udpMessage.Length());
            udpMessage.Write(sourcePort, udpMessage.Length());
            // Send to non-primary UdpSharedServer instances.
            server->mpMessageServer->SendToAllClients(udpMessage);
            // Now process the data internally.  This will add to the
            // queue of received messages, and generate callbacks.
            UdpSharedServer::ProcessUdpMessage(udpMessage, server);
        }
        server->mDelayMutex.Lock();
        if(server->mUpdateDelayMs == 0)
        {
#ifdef WIN32
            // Only sleep every N loops
            if( loopCounter++ == 250)
            {
                loopCounter = 0;
                SleepMs(1);
            }
#else
            usleep(500);
#endif
        }
        else
        {
            CxUtils::SleepMs(server->mUpdateDelayMs);
        }
        server->mDelayMutex.Unlock();
    }
    server->mConnectedFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback function that is run whenever a new message is
///          received from the UDP Socket or through the MessageClient 
///          interface.
///
///   \param[in] udpMessage Packet data containing the received message with the
///                         IP address of the source of the message appended
///                         at the end.
///   \param[in] udpSharedServer Pointer the the UdpSharedServer object to use
///                              for processing the message.
///
////////////////////////////////////////////////////////////////////////////////////
void UdpSharedServer::ProcessUdpMessage(Packet& udpMessage, void* udpSharedServer)
{
    UdpSharedServer* server = (UdpSharedServer*)udpSharedServer;
    IP4Address sourceAddress;
    unsigned short sourcePort = 0;
    // Read out the ipAddress data from the packet.
    const unsigned char * ptr = udpMessage.Ptr() + udpMessage.Length() - 6;
    sourceAddress.SetAddress(*ptr, *(ptr + 1), *(ptr + 2), *(ptr + 3));
    udpMessage.Read(sourcePort, udpMessage.Length() - 2);
    udpMessage.Write((unsigned char)0, (unsigned int)udpMessage.Length() - 6);
    udpMessage.SetLength(udpMessage.Length() - 6);
    
    server->mConnectedFlag = true;

    // Generate callbacks.
    std::set<Callback*>::iterator cb;
    server->mCallbacksMutex.Lock();
    for(cb = server->mCallbacks.begin();
        cb != server->mCallbacks.end();
        cb++)
    {
        (*cb)->ProcessUDP(udpMessage, sourceAddress);
        (*cb)->ProcessUDP(udpMessage, sourceAddress, sourcePort);
    }
    
    std::vector<Callback::Data>::iterator cbf;
    for(cbf = server->mFunctionCallbacks.begin();
        cbf != server->mFunctionCallbacks.end();
        cbf++)
    {
        cbf->Run(udpMessage, sourceAddress);
    }
    
    server->mCallbacksMutex.Unlock();

    // Add to queue
    server->mQueueMutex.Lock();
    server->mPacketQueue.push_back(udpMessage);
    server->mMessageAddressQueue.push_back(sourceAddress);
    server->mQueueMutex.Unlock();
    // Done!
}

/*  End of File */
