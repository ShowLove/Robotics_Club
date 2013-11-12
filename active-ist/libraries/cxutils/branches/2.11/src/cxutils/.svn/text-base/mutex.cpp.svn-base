////////////////////////////////////////////////////////////////////////////////////
///
///  \file mutex.cpp
///  \brief This file contains a structure for creating a cross-platform
///  Mutex.
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
#include "cxutils/mutex.h"
#include "cxutils/time.h"
#include "cxutils/thread.h"
#include <assert.h>
#include <string>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <sstream>

#ifdef WIN32
    #include <windows.h>
    #ifndef MINGW
        #include <atlbase.h>
    #else
        #define USES_CONVERSION
        #define A2W(x) (x)
        #define W2A(x) (x)
    #endif
#endif

using namespace CxUtils;

unsigned int Mutex::mCount = 0;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.  This initializes a ScopedLock by locking the mutex. Once
///         the ScopedLock object goes out of scope (or is deleted), the Mutex 
///         is automatically unlocked by the destructor.
///
///  \param[in] mutex Pointer to Mutex to lock within a specific scope.
///
////////////////////////////////////////////////////////////////////////////////////
Mutex::ScopedLock::ScopedLock(const Mutex* mutex) : mUnlockFlag(true), mpMutex(mutex)
{
    if(mpMutex)
    {
        if(mpMutex->IsLocked())
        {
            mUnlockFlag = false;
        }
        else
        {
            mpMutex->Lock();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.  On destruction, the Mutex is unlocked automatically.
///
////////////////////////////////////////////////////////////////////////////////////
Mutex::ScopedLock::~ScopedLock() 
{
    if(mpMutex && mUnlockFlag)
    {
        mpMutex->Unlock();
    }
}

/*
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If your program is not causing a ScopedLock object to get deleted
///         (go out of scope) properly, you can use this method to force an
///         unlocking (or desctruction) of the lock.
///
////////////////////////////////////////////////////////////////////////////////////
void Mutex::ScopedLock::ForceUnlock() 
{
    if(mpMutex && mUnlockFlag)
    {
        mpMutex->Unlock();
        mUnlockFlag = false;
    }
}
*/

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.  This initializes the mutex file descripters for use.
///
///  \param[in] name Name of mutex (GLOBAL NAMESPACE, should be an integer for best
///              portability between Windows and Linux.
///
////////////////////////////////////////////////////////////////////////////////////
Mutex::Mutex(const std::string& name)
{
    mThreadID = 0;
#ifdef WIN32
    mMutex = 0;
#else
    pthread_mutex_init(&mMutex, NULL);
    mSemUnion.mVal = 0;
    mSemUnion.mpArray = NULL;
    mSemUnion.mpBuff = NULL;
    mSemUnion.mpBuffIPC = NULL;
#endif
    Create(name);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.  This closes any mutex that may be open.
///
////////////////////////////////////////////////////////////////////////////////////
Mutex::~Mutex()
{
#ifdef WIN32
    if (mMutex)
    {
        ReleaseMutex(mMutex);
        CloseHandle(mMutex);
        mMutex = 0;
    }
#else
    if(IsLocked())
    {
        Unlock();
    }
    
    if( !mName.empty() )
    {
        mSemUnion.mVal = 0;
        mSemUnion.mpArray = NULL;
        mSemUnion.mpBuff = NULL;
        mSemUnion.mpBuffIPC = NULL;
        mName.clear();
    }
    else
    {
        //  Close the mutex
        pthread_mutex_destroy(&mMutex);
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates mutex using a global name.
///
///  \param[in] name Name of mutex (GLOBAL NAMESPACE, should be an integer for best
///              portability between Windows and Linux.  If empty string, then
///              a regulur mutex within this process is created.
///
////////////////////////////////////////////////////////////////////////////////////
void Mutex::Create(const std::string& name)
{
#ifdef WIN32
    if (mMutex)
    {
        ReleaseMutex(mMutex);
        CloseHandle(mMutex);
        mMutex = 0;
    }
#else
    // Make sure we are unlocked.
    Unlock();
    //  Close the mutex
    pthread_mutex_destroy(&mMutex);
    mName.clear();
#endif

#if defined(WIN32)
    mMutex = 0;
    if(name.empty()) 
    {
        mCount++;
        mMutex = CreateMutex(NULL, FALSE, NULL);
        assert(mMutex);
    }
    else 
    {
        USES_CONVERSION; //  Need for UNICODE conversion.
        mMutex = CreateMutex(NULL, FALSE, A2W(name.c_str()));
        assert(mMutex);
    }
#else
    if( !name.empty() )
    {
        mName = name;
        int flag = 0;

        flag = IPC_CREAT;
        
        if( (mSemKey = (key_t)atol( mName.c_str())) == 0)
        {
            // Convert letters in string to a unique number.
            std::stringstream str;
            std::string final;
            for(unsigned int i = 0; i < (unsigned int)mName.size(); i++)
            {
                str << (int)mName.c_str()[i];
                final = str.str();
            }

            if( (mSemKey = (key_t)atol( final.c_str() )) == 0)
            {
                mName.clear();
                printf("ERROR: Failed to Create Global Mutex\n");
                assert("Failed to Create Mutex");
                exit(0);
            }
        }

        // See if the mutex has already been created.
        flag = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
        // Try get access to semaphore if it already exists
        // but with read/write access for owner and group.
        mSemID = (int)semget( mSemKey, 1, flag );

        // If == -1 then the semaphore doesn't exist and
        // we need to create it.
        if( mSemID < 0)
        {
            mSemID = (int)semget( mSemKey, 1, flag | IPC_CREAT);
            if(mSemID < 0)
            {
                mName.clear();
                printf("ERROR: Failed to Create Global Mutex\n");
                exit(0);
            }
            // Set the initial state of the semaphore
            mSemUnion.mVal = 1;
            if( semctl( mSemID, 0, SETVAL, mSemUnion) == -1)
            {
                mName.clear();
                printf("ERROR: Failed to Create Global Mutex\n");
                exit(0);
            }
        }        
        //printf( "Mutex Created!\n");
    }
    else
    {
        pthread_mutex_init(&mMutex, NULL);
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Call this function to enter the critical area that the mutex is in.
///
///  \param[in] wait How long to wait for an event to occur allowing access to
///                  the critical section of code you want to protect with a
///                  mutex.  Value is in milliseconds.
///
///  \return True if mutex was enabled, and you have entered the critical 
///          section of memory, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mutex::Lock(unsigned long wait /*= INFINITE */) const
{
    if(mThreadID != 0)
    {
        Thread::ID callingID = Thread::GetCurrentThreadID();
        if(mThreadID == callingID)
        {
            assert("Created Mutex Deadlock!" && 0);
            return true;
        }
    }
#ifdef WIN32
    if (mMutex) 
    {
        DWORD result = WaitForSingleObject(mMutex, wait);
        if(result == WAIT_TIMEOUT)
        {
            return false;
        }
        *((ID *)(&mThreadID)) = Thread::GetCurrentThreadID();
        return true;
    }
    return false;
#else
    if( !mName.empty() )
    {
        struct sembuf buff;          ///<  Semaphore buffer.
        unsigned int startTimeMs = GetTimeMs();
        // Decrement the operation signal to
        // mark entering mutex.
        buff.sem_num = 0;
        buff.sem_op = -1;  // Block/Wait
        buff.sem_flg = SEM_UNDO;
        
        struct timespec delay;
        delay.tv_sec = 0;
        delay.tv_nsec = 1000;
        while(GetTimeMs() - startTimeMs < wait || wait == INFINITE)
        {
            if( semop( mSemID, &buff, 1) >= 0 )
            {
                //printf("Mutex Locked!\n");
                *((ID *)(&mThreadID))  = Thread::GetCurrentThreadID();
                return true;
            }
            nanosleep(&delay, NULL);
        } 
        //printf("ERROR: Failed to Lock Global Mutex!\n");
        //assert("Failed to Lock Global Mutex" && 0);
        return false;
    }
    else
    {
        pthread_mutex_t *mut = (pthread_mutex_t *)(&mMutex);
        signal(SIGHUP, SIG_IGN);
        pthread_mutex_lock(mut);
    }

    *((ID *)(&mThreadID))  = Thread::GetCurrentThreadID();

    return true;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Call this function to leave the critical area that the mutex is in.
///
////////////////////////////////////////////////////////////////////////////////////
void Mutex::Unlock() const
{
    if(mThreadID != 0 && Thread::GetCurrentThreadID() == mThreadID)
    {
        *((ID *)(&mThreadID)) = 0;
    }
#ifdef WIN32
    if(mMutex)
    {
        ReleaseMutex(mMutex);        
    }
#else
    if( !mName.empty() )
    {
        struct sembuf buff;          ///<  Semaphore buffer.

        // Increment the operation to signal leaving
        // the critical section.
        buff.sem_num = 0;
        buff.sem_op = 1;
        buff.sem_flg = SEM_UNDO;

        if( semop( mSemID, &buff, 1) != 0)
        {
            //printf("Failed to Unlock Mutex!\n");
        }
        //printf( "Mutex Unlocked!\n");
    }
    else
    {
        pthread_mutex_t *mut = (pthread_mutex_t *)(&mMutex);
        pthread_mutex_unlock(mut);
    }
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the mutex has been locked in the current thread/scope, 
///          otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mutex::IsLocked() const
{
    if(mThreadID == Thread::GetCurrentThreadID())
    {
        return true;
    }
    return false;
}

/*  End of File */
