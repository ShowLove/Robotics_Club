////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_sharedmemory.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  shared memory for inter-process communication.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 28 April 2007
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
#include "cxutils/ipc/mappedmemory.h"
#include "cxutils/thread.h"
#include "cxutils/keyboard.h"

using namespace std;
using namespace CxUtils;

//  Number of threads writing to shared memory.
const int NUM_THREADS = 10;

void WriteToMemory(void *arg);

class ThreadData
{
public:
    ThreadData()
    {
        mPos = 0;
    }
	~ThreadData() {}
    string mName;
    unsigned int mPos;
    static bool mLoop;
};

bool ThreadData::mLoop = true;

int main(int argc, char **argv)
{
    char name[128] = "Mem_Example";
    Thread threads[NUM_THREADS];
    MappedMemory mem;
    ThreadData data[NUM_THREADS];

    cout << "Creating Shared Memory...";
    /*  Create a shared memory location, and make it large enough so
        that each thread running can encode an integer. The maximum
        size of the shared memory is fixed and cannot be changed. */
    if(!mem.CreateMappedMemory(name, NUM_THREADS*sizeof(unsigned short))) 
    {
        cout << "Could not create shared/mapped memory: " << name << endl;
        return 0;
    }
    cout << "Success\n";

    // If no additional program arguments passed, create threads
    // that will write to shared memory.  You can also use the
    // cxipc_client.cpp program to write to shared memory also.
    if(argc == 1)
    {
        /*  Create a bunch of threads.  Each thread will open a view
            of the shared memory and write a counter number to
            the encode position told. This will be used to test the
            sharing of data, and read/write safety. */
        for(int i = 0; i < NUM_THREADS; i++) 
        {            
            //  Set encode position for the thread to use.
            data[i].mPos = i*sizeof(unsigned short);
            data[i].mName = name;
            //  Create thread.
            threads[i].CreateThread(WriteToMemory, (void *)(&data[i]));
        }
    }

    while(true) 
    {

        if(GetChar() == 27)
        {
            for(int i = 0; i < NUM_THREADS; i++)
            {
                data[i].mLoop = false;
            }
            break;
        }

        /*  Lock the memory, and then decode
            the counter numbers written by each
            thread writing to memory and display to console. */
        unsigned short count = 0;
        mem->SetReadPos(0);
        mem.Lock();    //  Must lock for read/write safety.
        for(int i = 0; i < NUM_THREADS; i++) 
        {
            count = 0;
            mem->Read(count);
            cout << count << " ";
        }
        mem.Unlock();  //  Must unlock to prevent deadlock.
        cout << "\n";

        SleepMs(1);
    }
    
    for(int i = 0; i < NUM_THREADS; i++)
    {
        data[i].mLoop = false;
        SleepMs(1);
        threads[i].StopThread();
    }

    return 0;
}

void WriteToMemory(void *arg)
{
    string name;
    unsigned int epos = 0;
    unsigned short count = 0;
    MappedMemory mem;
    ThreadData *info = (ThreadData *)(arg);

    if(!arg)
        return;

    name = info->mName;      //  Name of memory.
    epos = info->mPos;       //  Write position.

    SleepMs(50);
    if(mem.OpenMappedMemory(name.c_str()) == 0) 
    {
        cout << "Could not open shared/mapped memory: " << name << endl;
        return;
    }

    while(info->mLoop) 
    {
        /*  Lock memory and encode count value at
            the encode position told. */
        mem.Lock();
        mem->Write(++count, epos);
        mem.Unlock();
        SleepMs(1);
    }
}

/*  End of File */
