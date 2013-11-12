////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_networksetuptest.cpp
///  \brief This is a program used to test UDP traffice over a network connection.
///  It sends out multicast and unicast messages out over UDP so you can make sure
///  your computers can talk to each other over multiple methods.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 10 December 2008
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
#include <string>
#include <set>
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

int main(int argc, char **argv)
{
    int networkInterface = -1;
    unsigned short port = 3794;
    CxUtils::UdpServer udpServer;
    CxUtils::UdpClient udpMulticastClient;
    CxUtils::UdpClient udpUnicastClient;
    CxUtils::Packet recvPacket;
    IP4Address::Set toPing;
    IP4Address sourceAddress;


    // First argument passed is what port number
    // to use, default is 3794 for JAUS.
    if(argc > 1)
    {
        port = atoi(argv[1]);
    }
    // Second argument is network interface to use.
    if(argc > 2)
    {
        networkInterface = atoi(argv[2]);
    }

    IP4Address::List hostAddresses;
    CxUtils::Socket::GetHostAddresses(hostAddresses);
    for(unsigned int i = 0; i < (unsigned int)hostAddresses.size(); i++)
    {
        cout << "Interface: " << i << " - " << hostAddresses[i].mString << endl;
    }

    if(networkInterface <= (int)hostAddresses.size())
    {
        udpServer.SetNetworkInterface(networkInterface);
        udpMulticastClient.SetNetworkInterface(networkInterface);
    }

    if(udpServer.InitializeMulticastSocket(port, "239.255.0.1") &&
       udpMulticastClient.InitializeMulticastSocket("239.255.0.1", port, 3))
    {
        Time::Stamp pingTimeMs = 0;
        while(CxUtils::GetChar() != 27)
        {
            recvPacket.Clear();
            // Try receive some data
            if(udpServer.Recv(recvPacket, 512, 100, &sourceAddress) > 0)
            {
                // Only ping back to message that came from another computer.
                if(!CxUtils::Socket::IsHostAddress(sourceAddress))
                {
                    cout << "Received: " << (char *)recvPacket.Ptr() << ", From: " << sourceAddress.mString << endl;
                    toPing.insert(sourceAddress);
                }
            }

            if(CxUtils::GetTimeMs() - pingTimeMs > 1000)
            {
                udpMulticastClient.Send("Multicast Ping", 14);
                IP4Address::Set::iterator dest;
                for(dest = toPing.begin();
                    dest != toPing.end();
                    dest++)
                {
                    // Send back a unicast packet
                    udpUnicastClient.InitializeSocket(*dest, port);
                    if(udpUnicastClient.Send("Unicast Ping", 12) <= 0)
                    {
                        cout << "Failed to send Unicast Ping!\n";
                    }
                }
                toPing.clear();
                pingTimeMs = CxUtils::GetTimeMs();
                pingTimeMs = CxUtils::GetTimeMs();
            }
            CxUtils::SleepMs(1);
        }
    }

    return 0;

}
/*  End of File */
