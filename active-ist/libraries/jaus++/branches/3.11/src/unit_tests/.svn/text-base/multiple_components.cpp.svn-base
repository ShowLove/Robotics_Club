////////////////////////////////////////////////////////////////////////////////////
///
///  \file multiple_components.cpp
///  \brief This file is a unit test program to verify communication over
///          various JAUS transport layers.
///
///  <br>Author(s): Daniel Barber
///  <br>Copyright (c) 2011
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
#include "jaus/core/component.h"
#include "jaus/core/transport/nodemanager.h"
#include "jaus/core/control/reportauthority.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef VLD_ENABLED
#include <vld.h>
#endif

JAUS::UShort gSubsystemStart = 1;


class UnitTestCallback : public JAUS::Connection::Callback
{
public:
    UnitTestCallback()
    {
    }
    ~UnitTestCallback() {}
    virtual void ProcessPacket(const JAUS::Packet& transportPacket,
                               const JAUS::Packet& jausPacket,
                               const JAUS::Header& jausHeader,
                               const JAUS::Connection* connection,
                               const JAUS::Connection::Info* sourceInfo) 
    {
        jausPacket.Print();
    }
};


int main(int argc, char* argv[])
{
    std::map<JAUS::UShort, JAUS::Component*> components;
    UnitTestCallback callback;

    #ifndef WIN32
    gSubsystemStart = 100;
    #endif


    JAUS::UShort limit = 3;
    if(argc > 1)
    {
        limit = (JAUS::UShort)atoi(argv[1]);
    }
    if(argc > 2)
    {
        gSubsystemStart = (JAUS::UShort)atoi(argv[2]);
    }


    for(JAUS::UShort s = gSubsystemStart; s < gSubsystemStart + limit; s++)
    {
        components[s] = new JAUS::Component();
        components[s]->LoadSettings("settings/services.xml");
        components[s]->DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "DISCOVERY_UNIT_TEST");
        if(components[s]->Initialize(JAUS::Address(s, 1, 1)) == false)
        {
            std::cout << "INITIALIZATION ERROR!\n";
            return 0;
        }
    }

    while(CxUtils::GetChar() != 27)
    {
        components[gSubsystemStart]->DiscoveryService()->PrintSystemConfiguration();

        CxUtils::SleepMs(1000);
    }

    std::map<JAUS::UShort, JAUS::Component*>::iterator c;
    for(c = components.begin();
        c != components.end();
        c++)
    {
        c->second->Shutdown();
        delete c->second;
    }
    
    return 0;
}


/* End of File */
