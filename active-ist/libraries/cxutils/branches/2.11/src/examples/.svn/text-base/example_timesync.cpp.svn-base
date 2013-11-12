////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_timesync.cpp
///  \brief Demonstrates how to synchronize time between computers.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2 February 2010
///  <br>Copyright (c) 2010
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

using namespace std;
using namespace CxUtils;

unsigned short gPort = 20000;
IP4Address gMulticastGroup = "225.0.0.0";

int main(int argc, char **argv) 
{
    Time::Synchronizer timeSync;
    bool client = true;
    cout << "Program Usage: " << argv[0] << " <server 1=yes, 0=no>\n";
    if(argc > 1)
    {
        int a = (int)atoi(argv[1]);
        if(a == 1)
        {
            cout << "Running Server...\n";
            client = false;
            unsigned int udelay = 1;
            if(argc > 2)
            {
                 udelay = (unsigned int)atoi(argv[2]);
            }
            timeSync.Start(true, gPort, gMulticastGroup, udelay);
        }
        else
        {
            cout << "Running Client...\n";
            timeSync.Start(false, gPort, gMulticastGroup);
        }
    }
    else
    {
        cout << "Running Client...\n";
        timeSync.Start(false, gPort, gMulticastGroup);
    }
    Time cur, prev;
    Time::List samples;
    unsigned int numSamples = 3000;
    samples.reserve(numSamples);
    bool flag = false;
    
    while(GetChar() != 27)
    {
        if(Time::HaveExternalTime())
        {
            if(flag == false)
            {
                std::cout << std::endl;
                flag = true;
            }
            std::cout << "External Time: " << Time::GetUtcTime().ToString() << "\r";
        }
        else
        {
            if(flag)
            {
                std::cout << std::endl;
                flag = false;
            }
            std::cout << Time::GetUtcTime().ToString() << "\r";
        }
        SleepMs(50);
    }

    return 0;
}


/*  End of File */
