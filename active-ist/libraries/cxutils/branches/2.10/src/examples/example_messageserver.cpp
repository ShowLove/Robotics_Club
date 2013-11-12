////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_messageserver.cpp
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
const unsigned int gServerThreads = 1;
const unsigned int gClientThreads = 8;
const std::string gServerID = "20000";
volatile unsigned int gTotalMessages = 0;

Mutex gPrintMutex;

// Function that generates messages to clients.
void ServerThread(void* args)
{
    MessageServer server;
    char message[512];
    unsigned int loop = 0;
    if(server.Initialize(gServerID))
    {
        while(gQuitFlag == false)
        {
            // Wait until all clients are connected, then
            // start sending messages so we can get a better
            // performance measurement.
            if(server.GetNumClients() == gClientThreads)
            {
                sprintf(message, "Message #%d", loop++);
                if(server.SendToAllClients(message))
                {
                    gTotalMessages++;
                }
            }
            SleepMs(1);
        }
    }
}

// Function that generates clients to receive messages.
void ClientThread(void* args)
{
    MessageClient client;
    std::string message;
    unsigned int count = 0;
    unsigned int total = 0;
    if(client.Initialize(gServerID, MessageClient::AnyID, MappedMessageBox::DefaultSize, false))
    {
        while(gQuitFlag == false)
        {
            // See if a message has been received by this
            // client process.  A better method would be to 
            // use a callback so you don't have to poll.
            if(client.ReadMessage(message))
            {
                ++total;
                // Only print to the screen every N messages, failure to do 
                // so will skew performance results because it takes time to
                // write to the console window.
                if(++count == 100)
                {
                    count = 0;
                    gPrintMutex.Lock();
                    // Display the percent of messages generated that we've been
                    // able to receive.
                    cout << client.GetID() << " Received " << total*100.0/gTotalMessages << " Percent" << endl;
                    gPrintMutex.Unlock();
                }
            }
            SleepMs(1);
        }
    }
}

void TestRegistry();

int main(int argc, char **argv) 
{   
    Thread server[gServerThreads];
    Thread client[gClientThreads];

    // Create threads to continuously write to the
    // message box.
    for(unsigned int i = 0; i < gClientThreads; i++)
    {
        client[i].CreateThread(ClientThread, NULL);
        client[i].SetThreadName("Client");
        client[i].SetThreadPriority(50);
        SleepMs(5);
    }

    SleepMs(500);

    // Create threads to continuously write to the
    // message box.
    for(unsigned int i = 0; i < gServerThreads; i++)
    {
        server[i].CreateThread(ServerThread, NULL);
        server[i].SetThreadName("Server");
        SleepMs(5);
    }

    gTotalMessages = 0;

    // Wait for escape key to exit
    while(GetChar() != 27)
    {
        SleepMs(500);
    }

    // Signal quit to threads.
    gQuitFlag = true;
    SleepMs(500);

    return 0;
}

void TestRegistry()
{
    MessageServer::Registry rosters[4];
    
    for(int i = 0; i < 4; i++)
    {
        if(i == 0)
        {
            rosters[i].CreateRegistry(gServerID);
        }
        else
        {
            rosters[i].OpenRegistry(gServerID);
        }
    }
    for(int i = 0; i < 4; i++)
    {
        if(rosters[i].IsOpen() == false)
        {
            cout << "Failed to open registry.\n";
        }        
    }
    
    rosters[0].CloseRegistry();
    
    SleepMs(500);
    
    if(rosters[0].CreateRegistry(gServerID) == false)
    {
        cout << "Failed to re-create Registry.\n";
    }
    rosters[0].Register(1);
    rosters[0].Register(2);

    MessageServer::List registry;
    for(int i = 1; i < 4; i++)
    {
        if(rosters[i].GetRegistry(registry) == false || registry.size() != 2)
        {
            cout << "Error Reading Registry.\n";
        }
    }
    
    for(int i = 0; i < 4; i++)
    {
        rosters[i].CloseRegistry();
    }
    
}

/*  End of File */
