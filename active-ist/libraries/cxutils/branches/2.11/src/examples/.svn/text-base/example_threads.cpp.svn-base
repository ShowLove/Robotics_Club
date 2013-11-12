////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_threads.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Thread and Mutex structures.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 25 April 2007
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

using namespace std;
using namespace CxUtils;

Mutex gGlobalMutex;
vector<int> gTestVector;

void ThreadFunction(void *args);

class TestThreadClass : public Thread
{
public:
    TestThreadClass();
    virtual void Execute();
};

int main(int argc, char **argv) 
{
    Thread thread;
    TestThreadClass threadClass;

    bool quit = false;
    /*  Create thread and pass it a parameter so that
        the function knows when to stop. */
    if(thread.CreateThread(ThreadFunction, (void *)(&quit)) == 0) 
    {
        cout << "Error could not create thread.\n";
        return 0;
    }
    cout << "Created thread, populating vector...\n";

    if(!threadClass.CreateThread()) 
    {
        cout << "Error could not create thread.\n";
        return 0;
    }


    for(int i = 0; i < 100; i++) 
    {
        /*  Lock  (enter critical section) mutex for thread 
            protection since vector is not thread safe. */
        gGlobalMutex.Lock();
        //  Add data to the vector
        gTestVector.push_back(i);
        // Unlock mutex (leave critical section).
        gGlobalMutex.Unlock();

        SleepMs(10);   //  Pause for 100 ms
    }

    quit = true; //  Tell thread to exit using variable.

    /* Thread Destructor will stop thread, however it
       is good practice to close it yourself. */
    if(thread.IsThreadActive()) 
        thread.StopThread(100);  //  Wait up to 100 ms to exit

    /*  Tell the thread class to stop, and wait up to 50 ms to exit. */
    if(threadClass.IsThreadActive())
        threadClass.StopThread(50);

    return 0;
}

void ThreadFunction(void *args)
{
    int total = 0, prev = 0;
    bool* quit = (bool *)(args);
    while(total < 99 && !(*quit))
    {
        /*  Lock  (enter critical section) mutex for thread 
            protection since vector is not thread safe. */
        gGlobalMutex.Lock();
        if(gTestVector.size() > 0) 
        {
            total = gTestVector[gTestVector.size() - 1];
            if(prev != total) 
            {
                cout << "Pulled " << total << " from vector.\n";
            }
            total = prev;
        }
        // Unlock mutex (leave critical section).
        gGlobalMutex.Unlock();

        SleepMs(1); //  Don't overload CPU
    }

    cout << "Done with thread!\n";
}


TestThreadClass::TestThreadClass()
{
}


void TestThreadClass::Execute()
{
    int i = 0;
    while(!Thread::QuitThreadFlag()) 
    {
        /*  Do something */
        cout << "Loop: " << i++ << "\n";
        SleepMs(1);
    }
}

/*  End of File */
