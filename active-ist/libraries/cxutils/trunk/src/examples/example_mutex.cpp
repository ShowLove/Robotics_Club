////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_mutex.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Mutex class.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 February 2008
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
#include "cxutils/mutex.h"
#include "cxutils/packet.h"
#include <stdio.h>

using namespace std;
using namespace CxUtils;


int main(int argc, char **argv) 
{
    Mutex m;

    m.Create("456789");

    printf("Entering Mutex...\n");
    m.Lock();
    printf("Success.. blocking other processes.");
    printf("Press ENTER to exit Mutex\n"); getchar();
    m.Unlock();
    printf("Released mutex, other threads can continue.\n");

    Mutex mut2;
    Packet packet;
    Mutex::ScopedPtr<Packet> ptr(&packet, &mut2);
    // packet is within Mutex critical section.
    ptr->Write(std::string("Hello"));
    // When program exits, ptr goes out of scope and
    // automatically releases the mutex.
    
    // Testing ScopedLock 
    {
        Mutex::ScopedLock lock(&m);
        // Should not be able to double lock.
        Mutex::ScopedLock lock2(&m);
        // Do some stuff.
        int x;
        x = 3;
        x *= x;
    }
    // Should be unlocked now.
    if(m.IsLocked())
    {
        printf("Scoped Lock Failed\n");
    }

    // Test constant scoped ptr.
    {
        const int x = 9;
        Mutex m3;
        Mutex::CScopedPtr<int> cptr(&x, &m3);
        int y = *cptr.Data();
    }

    return 0;
}


/*  End of File */
