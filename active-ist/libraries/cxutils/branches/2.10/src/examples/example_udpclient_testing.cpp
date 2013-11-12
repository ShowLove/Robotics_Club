////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_udpclient_testing.cpp
///  \brief This is a test program to check the throughput of UDP traffic.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 28 April 2009
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
unsigned short gPort = 20000;
std::string gMulticastGroup = "224.0.0.1";
std::string gHostDest = "localhost";


int main(int argc, char **argv) 
{
    Joystick joystick;
    UdpClient client;
    
    joystick.Initialize(Joystick::AnyJoystick);
    char x = 0, y = 0;
    if(client.InitializeSocket(gHostDest, gPort))
    {
        char buffer[512];
        while(true)
        {   
            if(joystick.IsConnected())
            {
                sprintf(buffer, "Joystick - X:%d, Y:%d", (int)joystick.GetAxisPercentage(Joystick::X), (int)joystick.GetAxisPercentage(Joystick::Y));
            }
            else
            {
                sprintf(buffer, "Joystick - X:%d, Y:%d", (int)x++, (int)y--);
            }
            client.Send(buffer, ((unsigned int)strlen(buffer)));
            if(GetChar() == 27)
            {
                break;
            }
            SleepMs(1);
        }
    }
    return 0;
}

/*  End of File */
