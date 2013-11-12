////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_sharedsocket.cpp
///  \brief This is a sample program to test/demonstrate how to use a
///  UdpSharedServer to create multiple UDP Servers capable of receiving both
///  unicast and multicast UDP messages sent to the host machine.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 April 2009
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
#include <iostream>
#include <string.h>

#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

bool gQuitFlag = false;
const unsigned int gServerThreads = 4;
const unsigned int gClientThreads = 1;
unsigned short gPort = 3794;
std::string gMulticastGroup = "239.255.0.1";
std::string gHostDest = "localhost";

class UdpCallback : public UdpSharedServer::Callback
{
public:
    virtual void ProcessUDP(const Packet& packet, const IP4Address& ipAddress)
    {
        // Uses mutex to make sure the cout data
        // doesn't overlap between multiple threads.
        mCount++;
        if(strstr((const char *)packet.Ptr(), "Unicast") != 0)
        {
            mUnicastCount++;
        }
        else
        {
            mMulticastCount++;
        }
    }
    UdpSharedServer* mServer;
    volatile unsigned int mCount;
    volatile unsigned int mUnicastCount;
    volatile unsigned int mMulticastCount;
    double mStartTimeSeconds;
};

UdpCallback gCallbacks[gServerThreads];

// Function run in a thread that keeps trying to
// sending messages.
void ClientThread(void* args)
{
    static unsigned int clientNumber = 0;
    unsigned int instanceNumber = clientNumber++;
    UdpClient unicastClient;
    UdpClient multicastClient;

    char unicastBuff[1024];
    char multicastBuff[1024];
    unsigned int messageCount = 0;

    if(unicastClient.InitializeSocket(gHostDest, gPort) &&
        multicastClient.InitializeMulticastSocket(gMulticastGroup, gPort))
    {
        while(gQuitFlag == false)
        {
            memset(unicastBuff, 0, 1024);
            memset(multicastBuff, 0, 1024);
            sprintf(unicastBuff, "Unicast %d", messageCount);
            sprintf(multicastBuff, "Multicast %d", messageCount);
            messageCount++;
            unicastClient.Send(unicastBuff, (unsigned int)strlen(unicastBuff));
            multicastClient.Send(multicastBuff, (unsigned int)strlen(multicastBuff));
            SleepMs(1);
        }
    }
}


// Function that is run a in a thread runs a
// UDPSharedServer to receive messages.
void ServerThread(void* args)
{
    static unsigned int clientNumber = 0;
    UdpSharedServer server;
    unsigned int instanceNumber = clientNumber++;

    gCallbacks[instanceNumber].mServer = &server;
    gCallbacks[instanceNumber].mCount = 0;
    gCallbacks[instanceNumber].mUnicastCount = 0;
    gCallbacks[instanceNumber].mMulticastCount = 0;
    gCallbacks[instanceNumber].mStartTimeSeconds = CxUtils::Timer::GetTimeSeconds();

    Time::Stamp resetTimeMs = GetTimeMs();

    // Callbacks are used to get data received by
    // shared socket.
    server.RegisterCallback(&gCallbacks[instanceNumber]);

    if(server.InitializeSocket(gPort, gMulticastGroup))
    {
        gCallbacks[instanceNumber].mStartTimeSeconds = CxUtils::Timer::GetTimeSeconds();
        while(gQuitFlag == false)
        {
            if(server.IsPrimary())
            {
                // If this is the primary server (the one with the
                // actual UDP Server socket) then after 2500 ms, shutdown
                // and re-initialize to test if another active server will
                // take over as primary to keep everyone receiving data.
                if(GetTimeMs() - resetTimeMs >= 5000)
                {
                    server.Shutdown();
                    SleepMs(500);
                    resetTimeMs = GetTimeMs();
                }
            }
            else
            {
                resetTimeMs = GetTimeMs();
            }
            if(server.IsValid() == false)
            {
                server.InitializeSocket(gPort, gMulticastGroup);
            }
            SleepMs(100);
        }
    }
}

int main(int argc, char **argv)
{
    Thread server[gServerThreads];
    Thread client[gClientThreads];

    // Create threads to continuously write to the
    // message box.
    for(unsigned int i = 0; i < gServerThreads; i++)
    {
        server[i].CreateThread(ServerThread, NULL);
        server[i].SetThreadName("Server");
        SleepMs(50);
    }

    SleepMs(10);

    // Create a bunch of reader threads to have
    // multiple readers from the message box.
    for(unsigned int i = 0; i < gClientThreads; i++)
    {
        client[i].CreateThread(ClientThread, NULL);
        client[i].SetThreadName("Client");
        SleepMs(5);
    }

    SleepMs(500);

    // Wait for escape key to exit
    while(GetChar() != 27)
    {
        for(unsigned int i = 0; i < gServerThreads; i++)
        {
            if(gCallbacks[i].mServer->IsPrimary())
            {
                cout << "Primary ";
            }
            else
            {
                cout << "        ";
            }
            cout << i << " - Unicast [" << gCallbacks[i].mUnicastCount << "] Multicast [" << gCallbacks[i].mMulticastCount << "] Total - " << gCallbacks[i].mCount << " FPS - " << gCallbacks[i].mCount/(CxUtils::Timer::GetTimeSeconds() - gCallbacks[i].mStartTimeSeconds) << "\n";
        }
        cout << "===========================================================================\n";
        SleepMs(500);
    }

    // Signal quit to threads.
    gQuitFlag = true;
    SleepMs(500);

    return 0;
}

/*  End of File */
