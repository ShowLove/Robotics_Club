////////////////////////////////////////////////////////////////////////////////////
///
///  \file thread.cpp
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
#include "cxutils/thread.h"

#include <stdio.h>
#include <string.h>

#ifdef WIN32
#include <windows.h>
#endif

using namespace CxUtils;

unsigned int Thread::Manager::mManagerCount = 0;


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////
Thread::Manager::Manager()
{
    mManagerCount++;
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////
Thread::Manager::~Manager()
{
    StopAllThreads();
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a new thread.
///
///   \param[in] name Name to reference thread by.
///   \param[in] func Pointer to function to execute in thread.
///   \param[in] args Pointer to arguments to pass to executing function.
///   \param[in] priority Thread priority [-50, 50].
///
///   \return True on success, otherwise false on failure to create thread.
///
////////////////////////////////////////////////////////////////////////////////
bool Thread::Manager::CreateThread(const std::string& name, void (*func)(void *), void* args, const int priority)
{
    Mutex::ScopedLock lock(&mMutex);
    if(mThreads.find(name) == mThreads.end())
    {
        mThreads[name] = new Thread();
        mThreads[name]->CreateThread(func, args);
        mThreads[name]->SetThreadName(name);
        mThreads[name]->SetThreadPriority(priority);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops and deletes a thread.
///
///   \param[in] name Name to reference thread by.
///   \param[in] ms Number of milliseconds to wait for thread to quit.
///
////////////////////////////////////////////////////////////////////////////////
void Thread::Manager::StopThread(const std::string& name, const int ms)
{
    Mutex::ScopedLock lock(&mMutex);
    std::map<std::string, Thread*>::iterator t;
    t = mThreads.find(name);
    if(t != mThreads.end())
    {
        t->second->StopThread(ms);
        delete t->second;
    }
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops and deletes all threads.
///
////////////////////////////////////////////////////////////////////////////////
void Thread::Manager::StopAllThreads()
{
    Mutex::ScopedLock lock(&mMutex);
    std::map<std::string, Thread*>::iterator t;
    for(t = mThreads.begin(); t != mThreads.end(); t++)
    {
        t->second->StopThread();
        delete t->second;
    }
    mThreads.clear();
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a thread is actively running.
///
///   \param[in] name Name of the thread to check status for.
///
///   \return True if running, false otherwise (or doesn't exist).
///
////////////////////////////////////////////////////////////////////////////////
bool Thread::Manager::IsThreadActive(const std::string& name) const
{
    Mutex::ScopedLock lock(&mMutex);
    std::map<std::string, Thread*>::const_iterator t;
    t = mThreads.find(name);
    if(t != mThreads.end())
    {
        return t->second->IsThreadActive();
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a thread should exit.
///
///   \param[in] name Name of the thread to check status for.
///
///   \return True if thread should exist (StopThread called), false if it
///           should keep running.
///
////////////////////////////////////////////////////////////////////////////////
bool Thread::Manager::QuitThreadFlag(const std::string& name) const
{
    Mutex::ScopedLock lock(&mMutex);
    std::map<std::string, Thread*>::const_iterator t;
    t = mThreads.find(name);
    if(t != mThreads.end())
    {
        return t->second->QuitThreadFlag();
    }
    return true;
}

#ifdef WIN32
#include <time.h>
#include <windows.h>
unsigned int Thread::mThreadNum = 0;
//
// Usage: SetThreadName (-1, "MainThread");
//
typedef struct TagTHREADNAME_INFO
{
    DWORD dwType;     ///< must be 0x1000
    LPCSTR szName;    ///< pointer to name (in user addr space)
    DWORD dwThreadID; ///< thread ID (-1=caller thread)
    DWORD dwFlags;    ///< reserved for future use, must be zero
} THREADNAME_INFO;

////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the thread name for Visual Studio C++ 6 and above
///
///   \param dwThreadID ID of the thrad
///   \param szThreadName Name of the thread
///
////////////////////////////////////////////////////////////////////////////////
void CxSetThreadName(DWORD dwThreadID, LPCSTR szThreadName)
{
#ifndef MINGW
    THREADNAME_INFO info;
    info.dwType = 0x1000;
    info.szName = szThreadName;
    info.dwThreadID = dwThreadID;
    info.dwFlags = 0;

    __try
    {
        RaiseException(0x406D1388, 0, sizeof(info)/sizeof(DWORD), (ULONG_PTR*)&info);
    }
    __except(EXCEPTION_CONTINUE_EXECUTION)
    {
    }
#endif
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief The actual function that gets called by the new thread within
///  the wrapper class.
///
///  This function uses a void pointer to a class that is based off of the
///  Thread class and calls the virtual functions execute and cleanup
///  that are in the Thread class.  It also sets the active status flag.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned long __stdcall Thread::CxThreadProc(void* arg)
{
    Thread *pThread = (Thread*)arg;
    pThread->mActive = true;

    if (pThread->mThreadName.empty() == false)
        ::CxSetThreadName(GetCurrentThreadId(), pThread->mThreadName.c_str());

    pThread->mFuncCbMutex.Lock();
    pThread->Execute();
    pThread->mFuncCbMutex.Unlock();
    pThread->mActive = false;
    return 0;
}

#else
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief The actual function that gets called by the new thread within
///  the wrapper class.
///
///  This function uses a void pointer to a class that is based off of the
///  Thread class and calls the virtual functions execute and cleanup
///  that are in the Thread class.  It also sets the active status flag.
///
////////////////////////////////////////////////////////////////////////////////////
void *Thread::CxThreadProc(void *arg)
{
    //  Typecast
    Thread *pThread = (Thread*)arg;
    pThread->mActive = true;
    pThread->mFuncCbMutex.Lock();
    pThread->Execute();
    // Signal exit.        
    pThread->mFuncCbMutex.Unlock();
    pThread->mActive = false;    
    pthread_exit(NULL);
    return 0;
}
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, sets defaults.
///
////////////////////////////////////////////////////////////////////////////////////
Thread::Thread() : mThreadFunc(0)
{
    InitializeThread();
#ifdef WIN32
    SetThreadName("CX_Thread");
#endif

}

////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates the thread object and then sets the name
///
///   \param name Name of the thread to display in the VC++ debugger
///
////////////////////////////////////////////////////////////////////////////////
Thread::Thread(const std::string& name)
{
    InitializeThread();
    this->SetThreadName(name);
}

////////////////////////////////////////////////////////////////////////////////
///
///   \return Thread name.
///
////////////////////////////////////////////////////////////////////////////////
std::string Thread::GetThreadName() const { return mThreadName; }

////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the name of the thread in the VC++ debugger window
///
///   \param name Name of the thread (9 char limitaiton in VC++ 6)
///
////////////////////////////////////////////////////////////////////////////////
void Thread::SetThreadName(const std::string& name)
{
    mThreadName = name;
#ifdef WIN32
    if (IsThreadActive())
    {
        ::CxSetThreadName(mThreadId, mThreadName.c_str());
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the ID of the current calling thread.
///
///   \return Gets the ID of the current calling thread.
///
////////////////////////////////////////////////////////////////////////////////
Thread::ID Thread::GetCurrentThreadID()
{
#ifdef WIN32
    return GetCurrentThreadId();
#else
    return pthread_self();
#endif
}


////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the defaults on all the thread members 
///
////////////////////////////////////////////////////////////////////////////////
void Thread::InitializeThread()
{
    mActive = false;
    mQuitThreadFlag = true;
    mThreadId = 0;
    mThreadArgs = NULL;
    mThreadName = "CX_THREAD";
#ifdef WIN32
    mThread = 0;    
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor, terminates any running threads by force.
///
////////////////////////////////////////////////////////////////////////////////////
Thread::~Thread()
{
    this->StopThread();
    KillThread();
#ifdef WIN32
    if(mThread)
    {
        CloseHandle(mThread);
        mThread = NULL;
    }    
#else
    //pthread_exit(NULL);
#endif
    mThreadName = "CX_THREAD";
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates the thread that calls the Thread::Thread function 
///  implemented.
///
///  Returns 1 if successful, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Thread::CreateThread()
{
    //  Close any previous open threads
    KillThread();
#ifdef WIN32
    mThread = ::CreateThread(NULL,
                            0,
                            (LPTHREAD_START_ROUTINE)Thread::CxThreadProc,
                            (LPVOID) this,
                            0,
                            (LPDWORD)&mThreadId);

    if(mThread == NULL)
    {
        int errorCode;
        errorCode = ::GetLastError();
        return 0;
    }
    mActive = true;
    mQuitThreadFlag = false;
    
    Sleep(10);

    return 1;
#else
    int rc;
    rc = pthread_create(&mThreadId, NULL, Thread::CxThreadProc, (void *)this);
    
    if(rc)
    {
        printf("ERROR:Failed to create thread, error code %d\n", rc);
        printf("Thread::CreateThread()\n");
        return 0;
    }
    else
    {
        mActive = true;
        mQuitThreadFlag = false;
        usleep(10000);
        return 1;
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a thread based on the function and it's arguments.
///
///  Uses either windows or pthread function calls to spawn a thread that uses
///  the function passed.
///
///  \param func Function pointer for thread operation.
///  \param args Arguments for the thread.
///
///  Returns true if successful, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int Thread::CreateThread(void (*func)(void *), void *args)
{
    if(!func)
        return false;

    StopThread();
    mFuncCbMutex.Lock();
    this->mThreadFunc = func;
    this->mThreadArgs = args;
    mFuncCbMutex.Unlock();
    return this->CreateThread();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If the thread is in a looping state, this function will stop it.  If
///  the thread takes longer than ms milliseconds to exit, then the thread is
///  killed.
///
///  \param ms How long to wait for the thread to quit in milliseconds.
///
///  \return True if thread stopped.
///
////////////////////////////////////////////////////////////////////////////////////
int Thread::StopThread(const int ms)
{

    int result = 0;
    
    if(!IsThreadActive())
        return result;

    mQuitThreadFlag = true;

    if(mThreadId == Thread::GetCurrentThreadID())
    {
        return false;
    }

    for(int i = 0; i < ms; i++)
    {
        if(!IsThreadActive())
        {
            result = 1;
            break;
        }
#ifdef WIN32
        Sleep(1);
#else
        usleep(1000);
#endif
    }

    if(IsThreadActive())
        return KillThread();

    mFuncCbMutex.Lock();
    #ifndef WIN32
    //pthread_kill(mThreadId, 1);
    //pthread_cancel(mThreadId);
    pthread_join(mThreadId, NULL);
    #else
    mThread = 0;
    #endif
    mThreadId = 0;
    mThreadFunc = NULL;
    mThreadArgs = NULL;
    mFuncCbMutex.Unlock();
    mThreadName = "CX_THREAD";

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Returns true if a thread is active, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Thread::IsThreadActive() const
{
    return mActive;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Kills the thread forefully.
///
///  \return 1 if killed, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Thread::KillThread()
{
    mQuitThreadFlag = false;
#ifdef WIN32
    int  i;
    int x;
    if(mActive)
    {
        i = TerminateThread(mThread, 1);
        if(i == 0)
            x = 0;
        else
        {
            CloseHandle(mThread);
            mThread = NULL;
            mActive = false;
            x = 1;
        }
    }
    else
    {
        if(mThread != NULL)
        {
            CloseHandle(mThread);
            mThread = NULL;
        }
        x = 1;
    }
       
    return x;
#else
    if(mActive)
    {
        if(pthread_kill(mThreadId, 1))
        {
            return 0;
        }
        else
        {
            mThreadId = 0;
            mActive = false;
            return 1;
        }
    }
    else
    {
        mThreadId = 0;
        return 1;
    }
#endif
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Returns the identifier of the thread.
///
////////////////////////////////////////////////////////////////////////////////////
Thread::ID Thread::GetThreadID() const
{
    return mThreadId;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the priority of the thread
///
///  \param priority Value ranges form -50, 50, with 0 the default.
///
///  \return 1 if successfull, CX_FALSE otherwise
///
////////////////////////////////////////////////////////////////////////////////////
int Thread::SetThreadPriority(const int priority)
{
#if WIN32
    if (IsThreadActive())
    {
        int winPriority = 0;
        if(priority > 0 && priority <= 16)
        {
            winPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        }
        else if(priority > 16 && priority < 33)
        {
            winPriority = THREAD_PRIORITY_HIGHEST;
        }
        else if(priority >= 33)
        {
            winPriority = THREAD_PRIORITY_TIME_CRITICAL;
        }
        else if(priority < 0 && priority >= -16)
        {
            winPriority = THREAD_PRIORITY_BELOW_NORMAL;
        }
        else if(priority < -16 && priority > -33)
        {
            winPriority = THREAD_PRIORITY_LOWEST;
        }
        else if(priority <= -33)
        {
            winPriority = THREAD_PRIORITY_IDLE;
        }
        if(::SetThreadPriority(mThread, winPriority))
            return 1;
        else
            return 0;
    }
#else
    if(IsThreadActive())
    {
        int ret = 0;
        int linPriority = abs((int)(priority + 49.0)/100.0);
        if(linPriority > 99)
            linPriority = 99;
        struct sched_param param;
        param.sched_priority = linPriority;
        // Scheduling parameters of target thread 
        ret = pthread_setschedparam(mThreadId, SCHED_OTHER, &param);
        if(ret == 0)
            return 1;
    }
#endif
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the the thread has been told to quit, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Thread::QuitThreadFlag() const { return mQuitThreadFlag; } 


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief The main execution function called within the thread.
///
///  This virtual function is what gets called within the thread created by
///  the createThread function. This is an abstract function, meaning if you
///  inherit Thread, you must implement this function.
///
////////////////////////////////////////////////////////////////////////////////////
void Thread::Execute()
{
    if(this->mThreadFunc)
        mThreadFunc(mThreadArgs);
}

/*  End of File */
