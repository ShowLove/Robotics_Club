////////////////////////////////////////////////////////////////////////////////////
///
///  \file mutex.h
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
#ifndef __CXUTILS_MUTEX_H
#define __CXUTILS_MUTEX_H

//  Include pthread library for LINUX
#ifdef WIN32

#else
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pthread.h>
#endif

#include <string>
#include "cxbase.h"

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Mutex
    ///   \brief Cross platform mutex structure for thread safe operations.
    ///   This structure has been defined to work in both Windows and Linux operating
    ///   systems.
    ///   
    ///   In windows you can assign a name to the Mutex and make it global (similar
    ///   to a semaphore), however this support is not available in linux.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Mutex
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ScopedPtr
        ///   \brief Template class for making a pointer to some data that that needs
        ///          protection by a mutex.  When the Ptr goes out of scope or is deleted
        ///          the mutex is automatically released.
        ///
        ///   If the provided mutex is already locked, the scoped ptr will not double
        ///   lock, and will also not perform an unlock on release.  However, if on
        ///   creation the forceUnlock flag is true, the pointer will always unlock
        ///   regardless.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        template <class T>
        class ScopedPtr
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Constructor for creation of a ScopedPtr.
            ///
            ///   \param[in] data Pointer to data to be protected by a mutex until program 
            ///                   exits the scope this structure was created in.
            ///   \param[in] mutex Pointer to the mutex to use within the current program scope.
            ///   \param[in] forceUnlock If true (default is false) then when the object
            ///                          goes out of scope, the mutex is unlocked always.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            ScopedPtr(T* data, const Mutex* mutex, const bool forceUnlock = false) : mpData(data),
                                                                                     mpNullSafety(NULL),
                                                                                     mpMutex(mutex),
                                                                                     mUnlockFlag(true)

            {
                if(mpMutex && mpData)
                {
                    if(mpMutex->IsLocked())
                    {
                        if(forceUnlock == false)
                        {
                            mUnlockFlag = false;
                        }
                    }
                    else
                    {
                        mpMutex->Lock();
                    }
                }
            }
            virtual ~ScopedPtr()
            {
                if(mpMutex && mpData)
                {
                    mpData = NULL;
                    if(mUnlockFlag)
                    {
                        mpMutex->Unlock();
                    }
                }
                if(mpNullSafety)
                {
                    delete mpNullSafety;
                    mpNullSafety = NULL;
                }
            }
            // Used to access data portected by Mutex.
            inline T* operator->() { return Data(); }
            inline const T* operator->() const { return Data(); }
            T* Data()
            {
                if(mpMutex && mpData)
                {
                    return mpData;
                }
                else
                {
                    if(NULL == mpNullSafety)
                    {
                        mpNullSafety = new T();
                    }
                    return mpNullSafety;
                }
            }
            const T* Data() const
            {
                if(mpMutex && mpData)
                {
                    return mpData;
                }
                else
                {
                    if(NULL == mpNullSafety)
                    {
                        T** temp = ((T**)(&mpNullSafety));
                        *temp = new T();
                    }
                    return mpNullSafety;
                }
            }
            // Check to see if data is locked.
            bool IsLocked() const
            {
                if(mpMutex)
                {
                    return mpMutex->IsLocked();
                }
                return false;
            }
            // Check to see if the Ptr was created correctly with valid data.
            bool IsValid() const
            {
                if(mpMutex && mpData)
                {
                    return true;
                }
                return false;
            }
            bool operator==(const T* x) const
            {
                return mpData == x;
            }
            bool operator!=(const T* x) const
            {
                return mpData != x;
            }
        private:
            ScopedPtr<T>& operator=(const ScopedPtr<T>& ptr) { return *this; }
            T* mpData;              ///<  Pointer to data being protected. 
            T* mpNullSafety;        ///<  Temp variable used to preven NULL pointer violation.
            const Mutex* mpMutex;   ///<  Mutex to lock with.
            bool mUnlockFlag;       ///<  If true, unlock mutex on destruction.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class CScopedPtr
        ///   \brief Equivalent of the ScopedPtr class, but for constant data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        template <class T>
        class CScopedPtr
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Constructor for creation of a CScopedPtr.
            ///
            ///   \param[in] data Pointer to data to be protected by a mutex until program 
            ///                   exits the scope this structure was created in.
            ///   \param[in] mutex Pointer to the mutex to use within the current program scope.
            ///   \param[in] forceUnlock If true (default is false) then when the object
            ///                          goes out of scope, the mutex is unlocked always.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            CScopedPtr(const T* data, const Mutex* mutex, const bool forceUnlock = false) : mpData(data),
                                                                                            mpNullSafety(NULL),
                                                                                            mpMutex(mutex),
                                                                                            mUnlockFlag(true)

            {
                if(mpMutex && mpData)
                {
                    if(mpMutex->IsLocked())
                    {
                        if(forceUnlock == false)
                        {
                            mUnlockFlag = false;
                        }
                    }
                    else
                    {
                        mpMutex->Lock();
                    }
                }
            }
            virtual ~CScopedPtr()
            {
                if(mpMutex && mpData)
                {
                    mpData = NULL;
                    if(mUnlockFlag)
                    {
                        mpMutex->Unlock();
                    }
                }
                if(mpNullSafety)
                {
                    delete mpNullSafety;
                }
            }
            // Used to access data in const operations.
            inline const T* operator->() const { return Data(); }
            // Used to access data in const operations.
            const T* Data() const
            {
                if(mpMutex && mpData)
                {
                    return mpData;
                }
                else
                {
                    if(NULL == mpNullSafety)
                    {
                        T** temp = ((T**)(&mpNullSafety));
                        *temp = new T();
                    }
                    return mpNullSafety;
                }
            }
            // Check to see if data is locked.
            bool IsLocked() const
            {
                if(mpMutex)
                {
                    return mpMutex->IsLocked();
                }
                return false;
            }
            // Check to see if the Ptr was created correctly with valid data.
            bool IsValid() const
            {
                if(mpMutex && mpData)
                {
                    return true;
                }
                return false;
            }
            bool operator==(const T* x) const
            {
                return mpData == x;
            }
            bool operator!=(const T* x) const
            {
                return mpData != x;
            }
        private:
            const T* mpData;          ///<  Pointer to data being protected.
            const T* mpNullSafety;    ///<  Temp variable used to preven NULL pointer violation.
            const Mutex* mpMutex;     ///<  Mutex to lock with.
            bool mUnlockFlag;         ///<  If true, unlock mutex on destruction.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ScopedLock
        ///   \brief Class for enabling a Mutex within a specific program scope which will
        ///          unlock the mutex when out of scope (when object is destroyed).
        ///
        ///   The ScopedLock mutex is designed to prevent deadlocks.  It will only lock
        ///   the provided mutex in the current thread if it hasn't been locked yet, and
        ///   will only unlock if the ScopedLock instanced called lock.  Unlock is done
        ///   when the object goes out of scope or is deleted.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL ScopedLock
        {
        public:
            ScopedLock(const Mutex* mutex);
            ~ScopedLock();
        private:
            bool mUnlockFlag;        ///<  If true, must unlock on exit.
            const Mutex* mpMutex;    ///<  Pointer to mutex to use within Scoped lock.
        };
        Mutex(const std::string& name = std::string());
        ~Mutex();
        void Create(const std::string& name);
        bool Lock(unsigned long wait = INFINITE) const;
        void Unlock() const;
        bool IsLocked() const;
    protected:
    #ifdef WIN32
        void* mMutex;                   ///<  Mutex handle.    
    #else
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Simple union structure used to create a semaphore (Global Named
        ///          Mutex) in a posix compliant OS (Linux).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        union SemUnion
        {
            int mVal;                   ///< Value for SETVAL
            struct semid_ds *mpBuff;    ///< Buffer for IPC_STAT, IPC_SET
            unsigned short *mpArray;    ///< Array for GETALL, SETALL
            struct seminfo  *mpBuffIPC; ///<  Buffer for IPC info.
        };
        bool mCreateFlag;
        std::string mName;               ///<  Name of the global mutex.
        int mSemID;                      ///<  Shared semaphore file descriptor.
        key_t mSemKey;                   ///<  Key structure used for semaphore.
        union Mutex::SemUnion mSemUnion; ///<  Contains semaphore information.
        pthread_mutex_t mMutex;          ///<  Standard pthread mutex.
    #endif    
#ifdef WIN32
        typedef unsigned long ID;
#else
        typedef pthread_t ID;
#endif
        volatile ID mThreadID;           ///<  ID of the calling thread which locked the mutex.
        static unsigned int mCount;      ///<  The mutex number.

    };

}

#endif
/*  End of File */
