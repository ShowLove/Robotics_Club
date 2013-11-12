////////////////////////////////////////////////////////////////////////////////////
///
///  \file thread.h
///  \brief Contains software for creating cross-platform threads.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 24 April 2007
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
#ifndef __CXUTILS_THREAD_H
#define __CXUTILS_THREAD_H

#ifdef WIN32

#else
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#endif

#include <map>
#include "cxbase.h"
#include "mutex.h"

////////////////////////////////////////////////////////////////////////////////////
///
///  \def INFINITE
///  \brief Defines the value for INFINITE if it has not already been defined.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef INFINITE
#  define INFINITE 0xFFFFFFFF
#endif

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Thread
    ///   \brief Cross-platform class for creating threads for functions or adding
    ///   thread capabilities to a class using inheritence.
    ///
    ///   Thread is used to create a threaded function within a class on both
    ///   Windows and Linux based systems.  This is extremely useful and simple to use
    ///   and makes it easy to have threading on multiple platforms.
    ///
    ///   To make use of the Thread class just use inheritance and implement a virtual
    ///   function.  The Thread function is where you place code to
    ///   be performed in the spawned thread.  Functions for checking the status of
    ///   thread and killing it exist.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Thread
    {
    public:
#ifdef WIN32
        typedef unsigned long ID;
#else
        typedef pthread_t ID;
#endif
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Manager
        ///   \brief Thread Manager class which can be used to keep track of multiple
        ///   running threads.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Manager
        {
        public:
            Manager();
            ~Manager();
            bool CreateThread(const std::string& name, void (*func)(void* ), void* args, const int priority = 0);
            void StopThread(const std::string& name, const int ms = 500);
            void StopAllThreads();
            bool IsThreadActive(const std::string& name) const;
            bool QuitThreadFlag(const std::string& name) const;
            unsigned int GetNumThreads() const { Mutex::ScopedLock lock(&mMutex); return (unsigned int)mThreads.size(); }
        private:
            static unsigned int mManagerCount;      ///<  Number of managers created.
            Mutex mMutex;                           ///<  Mutex protecting creation/deletion of threads.
            std::map<std::string, Thread*> mThreads;///<  Threads being managed.
        };
        Thread();                                   ///<  Constructor.
        Thread(const std::string& name);            ///<  Constructor with optional name for VC++ debugger.
        virtual ~Thread();                          ///<  Destructor.
        int CreateThread();                         ///<  Creates the thread.
        int CreateThread(void (*func)(void* ), 
                         void* args);               ///<  Creates a thread based on a function pointer.
        int StopThread(const int ms = 500);         ///<  Stop a thread, but wait for it to exit.
        int SetThreadPriority(const int priority);  ///<  Call after creating the thread
        int KillThread();                           ///<  Kills the thread forcefully.
        bool QuitThreadFlag() const;                ///<  If true thread should exit.
        bool IsThreadActive() const;                ///<  Is the thread still active?
        void SetThreadName(const std::string& name);///<  Sets the name of the thread in the VC++ debugger.
        static Thread::ID GetCurrentThreadID();     ///<  Gets the ID of the current operating thread.
        std::string GetThreadName() const;          ///<  Get the name of the thread.
        Thread::ID GetThreadID() const;             ///<  Gets the thread ID.
    protected:    
        virtual void Execute();                     ///<  The actual thread function.
    private:
        volatile bool mActive;                      ///<  Is the thread active?
        volatile bool mQuitThreadFlag;              ///<  Tells the thread to quit.
    
        void InitializeThread();                    ///<  Common constructor.
        void (*mThreadFunc)(void*);                 ///<  Function pointer for thread objects.
        void* mThreadArgs;                          ///<  Arguments for function pointer.
        std::string mThreadName;                    ///<  Name of the thread.
        Mutex mFuncCbMutex;                         ///<  Function callback mutex.    
    #ifdef WIN32    
        static unsigned int mThreadNum;             ///<  Thread number.
        void* mThread;                              ///<  File descriptor information for thread.
        static unsigned long __stdcall CxThreadProc(void* arg); 
    #else
        static void*  CxThreadProc(void* arg);
    #endif
        ID mThreadId;                               ///<  Thread ID/File descriptor.
    };
}

#endif

/*  End of File */
