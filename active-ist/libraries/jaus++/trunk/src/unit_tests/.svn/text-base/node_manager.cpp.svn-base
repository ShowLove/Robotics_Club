////////////////////////////////////////////////////////////////////////////////////
///
///  \file transport_services.cpp
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
#include "jaus/core/liveness/liveness.h"
#include "jaus/core/transport/nodemanager.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef VLD_ENABLED
#include <vld.h>
#endif


int main(int argc, char* argv[])
{
    JAUS::Component left, right;
    JAUS::NodeManager *leftNode, *rightNode;

    leftNode = left.TransportService()->GetNodeManager();
    rightNode = right.TransportService()->GetNodeManager();

    leftNode->GetSettings()->SetDefaultPortNumber(3794);
    rightNode->GetSettings()->SetDefaultPortNumber(20000);

    JAUS::Connection::Info rightConnectionInfo;
    rightConnectionInfo.mTransportType = JAUS::Connection::Transport::JUDP;
    rightConnectionInfo.mDestIP = std::string("localhost");
    rightConnectionInfo.mDestPortNumber = rightNode->GetSettings()->GetDefaultPortNumber();
    rightConnectionInfo.mSourcePortNumber = leftNode->GetSettings()->GetDefaultPortNumber();
    leftNode->GetSettings()->AddCustomConnection(JAUS::Address(2, 1, 1), rightConnectionInfo);

    JAUS::Connection::Info leftConnectionInfo;
    leftConnectionInfo.mTransportType = JAUS::Connection::Transport::JUDP;
    leftConnectionInfo.mDestIP = std::string("localhost");
    leftConnectionInfo.mDestPortNumber = leftNode->GetSettings()->GetDefaultPortNumber();
    leftConnectionInfo.mSourcePortNumber = rightNode->GetSettings()->GetDefaultPortNumber();
    rightNode->GetSettings()->AddCustomConnection(JAUS::Address(1, 1, 1), leftConnectionInfo);

    left.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "LEFT");
    right.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle, "RIGHT");

    if(left.Initialize(JAUS::Address(1, 1, 1)) == false ||
       right.Initialize(JAUS::Address(2, 1, 1)) == false)
    {
        return 0;
    }

    while(CxUtils::GetChar() != 27)
    {
        left.DiscoveryService()->PrintSystemConfiguration();

        CxUtils::SleepMs(1000);
    }

    leftNode->Shutdown();
    rightNode->Shutdown();

    return 0;
}


/* End of File */
