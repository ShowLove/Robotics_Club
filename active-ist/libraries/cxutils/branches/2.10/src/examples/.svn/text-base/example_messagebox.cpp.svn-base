////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_messagebox.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  MessageBox class.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 March 2009
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
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

bool gQuitFlag = false;
std::string gSharedName = "50";
const unsigned int gWriterThreads = 10;
const unsigned int gReaderThreads = 1;
Mutex gPrintMutex;


// Function run in a thread that keeps trying to
// read messages in the box.
void ReaderThread(void* args)
{
    MappedMessageBox box;
    static unsigned int count = 0;
    unsigned int num = count++;
    
    if(box.OpenMessageBox(gSharedName))
    {
        double startTimeSeconds = Timer::GetTimeSeconds();
        while(!gQuitFlag)
        {
            std::string message;
            if(box.ReadMessage(message))
            {
                if(box.GetNumMessagesRead()%10 == 0)
                {
                    gPrintMutex.Lock();
                    cout << num << " Read " << box.GetNumMessagesRead() << " Messages [" << message << "] - FPS: " << box.GetNumMessagesRead()/(Timer::GetTimeSeconds() - startTimeSeconds) << "\n";
                    gPrintMutex.Unlock();
                }
            }
            SleepMs(1);
        }
    }
}


// Function that is run a in a thread which keeps
// writing new messages to the box.
void WriterThread(void* args)
{
    MappedMessageBox box;
    char buffer[256];
    static unsigned int count = 0;
    static unsigned int writerCount = 0;
    unsigned int num = writerCount++;

    if(box.CreateMessageBox(gSharedName, 1000) ||
       box.OpenMessageBox(gSharedName))
    {
        gPrintMutex.Lock();
        cout << num << " Opened Message Box.\n";
        gPrintMutex.Unlock();
        
        while(!gQuitFlag)
        {
            sprintf(buffer, "%d - Message %4d", num, count++);
            box.WriteMessage(std::string(buffer));
            SleepMs(1);
        }
    }
}

int main(int argc, char **argv) 
{    
    Thread writer[gWriterThreads];
    Thread readers[gReaderThreads];

    // Create threads to continuously write to the
    // message box.
    for(unsigned int i = 0; i < gWriterThreads; i++)
    {
        writer[i].CreateThread(WriterThread, NULL);
        writer[i].SetThreadName("Writer");
        SleepMs(5);
    }

    SleepMs(100);

    // Create a bunch of reader threads to have
    // multiple readers from the message box.
    for(unsigned int i = 0; i < gReaderThreads; i++)
    {
        readers[i].CreateThread(ReaderThread, NULL);
        readers[i].SetThreadName("Reader");
        SleepMs(1);
    }
    // Wait for escape key to exit
    while(GetChar() != 27)
    {
        SleepMs(100);
    }

    // Signal quit to threads.
    gQuitFlag = true;
    SleepMs(500);

    return 0;
}

/*  End of File */
