////////////////////////////////////////////////////////////////////////////////////
///
///  \file mappedmemory.h
///  \brief This file contains software for creating shared/mapped memory
///  locations in both Windows and Linux.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 27 April 2007
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
#ifndef _CXUTILS_IPC_MAPPED_MEMORY_H
#define _CXUTILS_IPC_MAPPED_MEMORY_H

#include <limits.h>
#include <assert.h>

#include "cxutils/cxbase.h"
#include "cxutils/packet.h"
#include "cxutils/mutex.h"
#include "cxutils/time.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class MappedMemory
    ///   \brief The MappedMemory data structure implements shared/mapped memory
    ///   that can be accessed between threads/processes.  It can be used in a similar
    ///   manner to the Packet class.
    ///
    ///   This piece of software is used for sharing messages between threads
    ///   and other processes.  It allocates a mapped memory buffer which includes
    ///   a shared/global mutex and size information by default.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL MappedMemory
    {
    public:
        static const unsigned ReadAccess = 1;
        static const unsigned int ReadWriteAccess = 2;
        static const unsigned int MemoryNameSize = 128;
        typedef unsigned long long int ID;               ///<  ID value for shared memory.
        MappedMemory();
        virtual ~MappedMemory();
        int OpenMappedMemory(const ID id,
                             const unsigned int access = ReadWriteAccess,
                             const unsigned int expectedSize = 0);
        int OpenMappedMemory(const std::string& name,
                             const unsigned int access = ReadWriteAccess,
                             const unsigned int expectedSize = 0);
        int CreateMappedMemory(const ID id,
                               const unsigned int size);
        int CreateMappedMemory(const std::string& name,
                               const unsigned int size);
        int CloseMappedMemory();
        static int DeleteMappedMemory(const std::string& name);
        int Lock(unsigned long wait = INFINITE) const;
        int Unlock() const;
        Packet* operator->();
        const Packet* operator->() const;
        Packet* GetData();
        const Packet* GetData() const;
        int Copy(const unsigned int p1, const unsigned int p2);
        bool IsOpen() const;
        static bool IsMemOpen(const std::string& name);
        unsigned char* GetMemory();
        const unsigned char* GetMemory() const;
        const std::string GetMappedName() const { return std::string(mMappedName); }
        unsigned int Length() const;
    private:
#ifdef WIN32
        void* mFileMapping;                                 ///<  Mapped memory handle.
        void* mFileSizeMapping;                             ///<  Handle to memory map containing memory size.
        unsigned char* mpMemorySizePtr;                     ///<  Pointer to memory size information.
#else
        int mFileMapping;                                   ///<  File descriptor to mapped memory.
        bool mCreatedFlag;                                  ///<  Flag indicating if the memory was created or not.
#endif
        Mutex* mpMemMutex;                                  ///<  Mutex for read/write protection.
        char mMappedName[MemoryNameSize];                   ///<  Name of mapped memory file.
        unsigned int  mWriteAccess;                         ///<  Type of access.
        unsigned char* mpMemory;                            ///<  Pointer to memory data.
        unsigned int mLength;                               ///<  Length of the memory (memory size).
        Packet::Wrapper mPacket;                            ///<  Wrapper packet.
    };
}

#endif

/* End of File */
