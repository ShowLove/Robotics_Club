////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_networking.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  sockets for networking.
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
#include <iostream>
#include <vector>
#include "cxutils/cxutils.h"
#include <sstream>
#include <iomanip>

#ifdef WIN32
//#include "vld.h"                //  memory leak detector in VS2005
#endif

using namespace std;
using namespace CxUtils;


#define PORT      3794
#define HOSTNAME "localhost"
//#define HOSTNAME "dbarber.net.ist.ucf.edu"
//#define HOSTNAME "127.0.0.1"

#define MULTICAST_GROUP "224.1.0.1"

void ClientUDP();
void ClientTCP();
void ServerUDP();
void ServerTCP();
void MulticastClientUDP();
void FakeGPS();
void UDP();
void UDP2();

int main(int argc, char **argv)
{
    if(argc <= 1) 
    {
        cout << "usage: " << *argv << " <type>\n";
        cout << "Network options <type>: 1 - UDP CLIENT\n";
        cout << "                        2 - UDP SERVER\n";
        cout << "                        3 - TCP CLIENT\n";
        cout << "                        4 - TCP SERVER\n";
        cout << "                        5 - UDP MULTICAST CLIENT\n";
        cout << "                        6 - FAKE GPS OVER TCP\n";
        return 0;
    }

    switch(atoi(argv[1]))
    {
    case 1:
        ClientUDP();
        break;
    case 2:
        ServerUDP();
        break;
    case 3:
        ClientTCP();
        break;
    case 4:
        ServerTCP();
        break;
    case 5:
        MulticastClientUDP();
        break;
    case 6:
        FakeGPS();
        break;
    default:
        cout << "usage: " << *argv << " <type>\n";
        cout << "Network options <type>: 1 - UDP CLIENT\n";
        cout << "                        2 - UDP SERVER\n";
        cout << "                        3 - TCP CLIENT\n";
        cout << "                        4 - TCP SERVER\n";
        cout << "                        5 - UDP MULTICAST CLIENT\n";
        break;
    };

    return 0;
}

void ClientUDP()
{

    UdpClient client;
    IP4Address::List hosts;
    client.GetHostAddresses(hosts);
    cout << "IP Address for Host Machine\n";
    for(int i = 0; i < (int)hosts.size(); i++ ) 
    {
        cout << hosts[i].mString << endl;
    }

    Packet data;
    data.SetByteOrder(CX_PACKET_BIG_ENDIAN);
    if(!client.InitializeSocket(HOSTNAME, PORT)) 
    {
        cout << "Unable to connect to host " << HOSTNAME << endl;
        return;
    }
    cout << "Initialized UDP Client Socket on port " << PORT << endl;
    char key = 0;
    while(key != 27) 
    {
        if((key = GetChar()) > 0) 
        {
            data.Write(key);
            cout << key;
            client.Send(data);  //  Send data (could send character array also if desired
            data.Clear();       //  Clear packet and encode data.
            /*  Alternative way to send:
                client.Send(&key, 1);    //  Send buffer/array.
            */
        }
    }
}

void MulticastClientUDP()
{
    UdpClient client;
    IP4Address::List hosts;
    client.GetHostAddresses(hosts);
    cout << "IP Address for Host Machine\n";
    for(int i = 0; i < (int)hosts.size(); i++ ) 
    {
        cout << hosts[i].mString << endl;
    }

    Packet data;
    data.SetByteOrder(CX_PACKET_BIG_ENDIAN);
    if(!client.InitializeMulticastSocket(MULTICAST_GROUP, PORT, 1)) 
    {
        cout << "Unable to connect to host " << HOSTNAME << endl;
        return;
    }
    cout << "Initialized UDP Client Socket on port " << PORT << endl;
    char key = 0;
    while(key != 27) 
    {
        if((key = GetChar()) > 0) 
        {
            data.Write(key);
            cout << key;
            client.Send(data);  //  Send data (could send character array also if desired
            data.Clear();       //  Clear packet and encode data.
            /*  Alternative way to send:
                client.Send(&key, 1);    //  Send buffer/array.
            */
        }
    }
}

void ServerUDP()
{
    UdpServer server;
    Packet data;
    IP4Address source;
    //server.SetNetworkInterface(0);  // If you wanted to specifiy a network interface to receive on.
    data.SetByteOrder(CX_PACKET_BIG_ENDIAN);
    if(!server.InitializeMulticastSocket(PORT, MULTICAST_GROUP)) 
    {
        cout << "Unable to initialize on port " << PORT << endl;
        return;
    }
    cout << "Initialized UDP Server Socket on port " << PORT << endl;
    char key = 0;
    while(key != 27) 
    {

        //  Do non-blocking receive for data (wait 100 ms).
        if(server.Recv(data, 500, 100, &source) > 0) //  Receive up to 500 bytes to packet.
        { 
            cout << source.mString << " Sent (" << data.Size() << " bytes): ";
            char val;
            while(data.Read(val))
                cout << val;
            cout << endl;
        }
        key = GetChar();     //  Get key.
    }
}

void ClientTCP()
{
    TcpClient client;
    Packet data;
    data.SetByteOrder(CX_PACKET_BIG_ENDIAN);
    if(!client.InitializeSocket(HOSTNAME, PORT)) 
    {
        cout << "Unable to connect to host " << HOSTNAME << endl;
        return;
    }
    cout << "Initialized TCP Client Socket\n";
    char key = 0;
    while(key != 27) 
    {

        if((key = GetChar()) > 0) 
        {
            data.Clear();       //  Clear packet and encode data.
            data.Write(key);
            client.Send(data);  //  Send data (could send character array also if desired
            /* Alternative way to send:
               client.Send(&key, 1);    //  Send buffer/array.
            */
            data.Clear();
            if(client.Recv(data, 50, 0))
            {
                cout << *data.Ptr() << endl;
            }
            //cout << key;
        }
    }
}

void ServerTCP()
{
    TcpListenSocket listen;
    TcpServer server;
    Packet data;
    string source;
    data.SetByteOrder(CX_PACKET_BIG_ENDIAN);
    // Tests non-blocking TCP connections.  If listen socket is non-blocking
    // then all TCP connections generated will be non-blocking.
    if(!listen.InitializeSocket(PORT, 5U, 0U, 0U, 1, 1)) 
    {
        cout << "Unable to initialize listen socket on port " << PORT << endl;
        return;
    }
    cout << "Initialized TCP Listen Socket\n";
    cout << "Waiting for a connection.... ";
    while( true ) 
    {
        if(listen.AwaitConnection(server))
        {
            cout << "Connection Made!\n";

            while(true)
            {
                if(server.Recv(data, 50, 0) > 0) //  Receive up to 50 bytes to packet.
                { 
                    cout << "Received: ";
                    char val;
                    while(data.Read(val))
                    {
                        cout << val;
                        server.Send(data);
                    }
                    cout << endl;
                }
                CxUtils::SleepMs(1);
            }
            cout << "Connection Closed.\n";
        }
        SleepMs(1);
    }
}


void FakeGPS()
{
    TcpListenSocket listen;
    TcpServer connection;
    Packet data;
    string source;
    if(!listen.InitializeSocket(PORT)) 
    {
        cout << "Unable to initialize listen socket on port " << PORT << endl;
        return;
    }
    cout << "Initialized TCP Listen Socket\n";
    cout << "Waiting for a connection.... ";
    bool quitFlag = false;
    while( listen.AwaitConnection(connection) && !quitFlag) 
    {
        cout << "Connection Made!\n";
        std::stringstream str;
        Time t;
        while(!quitFlag)
        {
            // Clear string.
            str.clear();
            str.str(std::string());
            // Set position data and message type.
            str << "$GPGLL,4916.45,N,12311.12,W,";
            // Get the current time.
            t.SetCurrentTime();
            str << t.mHour << t.mMinute << "." << t.mSecond;
            str << ",A";
            // Now calculate the checksum.
            unsigned char checkSum = 0;
            // Don't include the '$' character, but go to end.
            for(unsigned int i = 1; i < (unsigned int)str.str().size(); i++)
            {
                if(i == 1) 
                {
                    checkSum = str.str().c_str()[i];
                }
                else
                {
                    checkSum ^= str.str().c_str()[i];
                }
            }
            // Now add the checksum to the string.
            str << "*" << std::hex << std::uppercase << (int) checkSum;
            // Print to screen.
            std::cout << str.str() << std::endl;
            // Now send the data.
            if(connection.Send(str.str().c_str(), (unsigned int)str.str().length()) <= 0)
            {
                // Failed to send, connection must be closed.
                break;
            }
            // Check for exit by user.
            if(GetChar() == 27)
            {
                quitFlag = true;
            }
            SleepMs(250);
        }

        cout << "Connection Closed.\n";
        connection.Shutdown();
    }
}


void UDP()
{
    UdpClient a, b, c, d;
    UdpServer e, f;
    IP4Address sourceAddress;
    unsigned short sourcePort;

    e.InitializeMulticastSocket(3794, "239.255.0.1", true);
    f.InitializeMulticastSocket(3794, "239.255.0.1", true);

    a.InitializeMulticastSocket("239.255.0.1", 3794, 16, true);
    b.InitializeMulticastSocket("239.255.0.1", 3794, 16, true);
    c.InitializeSocket("localhost", a.GetSourcePort());
    d.InitializeSocket("localhost", b.GetSourcePort());
    

    while(CxUtils::GetChar() != 27)
    {
        std::string message;
        message = "A";
        a.Send((const char *)message.c_str(), (unsigned int)message.size());
        message = "B";
        b.Send((const char *)message.c_str(), (unsigned int)message.size());
        message = "C";
        c.Send((const char *)message.c_str(), (unsigned int)message.size());
        message = "D";
        d.Send((const char *)message.c_str(), (unsigned int)message.size());

        Packet packet;
        while(a.Recv(packet, 500, 1))
        {
            cout << "A - ";
            char val;
            while(packet.Read(val))
            {
                cout << val;
            }
            cout << endl;
        }
        while(b.Recv(packet, 500, 1))
        {
            cout << "B - ";
            char val;
            while(packet.Read(val))
            {
                cout << val;
            }
            cout << endl;
        }

        while(e.Recv(packet, 500, 1, &sourceAddress, &sourcePort))
        {
            cout << "E -  ";
            char val;
            while(packet.Read(val))
            {
                cout << val;
                if(val == 'A' && c.IsValid() == false)
                {
                    c.InitializeSocket(sourceAddress, sourcePort);
                }
            }
            cout << endl;
        }
        while(f.Recv(packet, 500, 1, &sourceAddress, &sourcePort))
        {
            cout << "F -  ";
            char val;
            while(packet.Read(val))
            {
                cout << val;
                if(val == 'B' && d.IsValid() == false)
                {
                    d.InitializeSocket(sourceAddress, sourcePort);
                }
            }
            cout << endl;
        }
        SleepMs(1000);
    }
}


/*  End of File */
