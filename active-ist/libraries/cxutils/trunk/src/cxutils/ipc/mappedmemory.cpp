////////////////////////////////////////////////////////////////////////////////////
///
///  \file mappedmemory.cpp
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
#include "cxutils/ipc/mappedmemory.h"
#include "cxutils/mutex.h"
#include "cxutils/fileio.h"
#include <assert.h>
#include <string>
#include <string.h>
#include <stdlib.h>
#include <iostream>
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
#else
    #include <fcntl.h>
    #include <sys/stat.h>
    #include <errno.h>
    #include <sys/mman.h>
    #include <sys/types.h> //shm_open
    #include <stdio.h>     //printf
    #include <stdlib.h>    //exit
    #include <unistd.h>    //close
    #include <string.h>    //strerror

    std::string gPostFix = ".cx_shared_mem";
#endif

using namespace CxUtils;

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

//#define CX_MAPPED_MEMORY_BASE sizeof(unsigned int)
#define CX_MAPPED_MEMORY_BASE 0

#ifdef WIN32
#else
#include <sys/ipc.h>
#include <sys/shm.h>
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMemory::MappedMemory()
{
#ifdef WIN32
    mFileMapping = NULL;
    mFileSizeMapping = NULL;
    mpMemorySizePtr = NULL;
#else
    mFileMapping = 0;
    mCreatedFlag = false;
#endif
    memset(mMappedName, 0, sizeof(char)*MemoryNameSize);
    mpMemory = NULL;
    mLength = 0;
    mpMemMutex = 0;
    mWriteAccess = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MappedMemory::~MappedMemory()
{
    CloseMappedMemory();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Opens an exisiting shared/mapped memory location.
///
///  If memory was opened, but the length is 0, you can set it using the
///  SetLength method.
///
///  \param id The ID Number of the shared memory resource to open.
///  \param access How to access the memory (ReadAccess,
///                or CX_MAPPED_MEMORY_WRITE_ONLY).
///  \param expectedSize The size of shared memory if known.  If not known, set to
///              0.  If you are opening a mapped memory created by another
///              program using CxUtils or Linux, then the size will be discovered
///              automatically.  If not, then the size will be unknown and
///              the Size and Length functions will return 0, even though the
///              memory map is valid.
///
///  \return 0 if the memory doesn't exist,
///          otherwise 1.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::OpenMappedMemory( const ID id,
                                    const unsigned int access,
                                    const unsigned int expectedSize)
{
    std::ostringstream input;
    input << id;
    return OpenMappedMemory(input.str(), access, expectedSize);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Opens an exisiting shared/mapped memory location.
///
///  If memory was opened, but the length is 0, you can set it using the
///  SetLength method.
///
///  \param name The name of the shared/mapped memory to open.<b>To maximize
///              portability between windows and linux, this should be an
///              integer number.</b>
///  \param access How to access the memory (ReadAccess,
///                or CX_MAPPED_MEMORY_WRITE_ONLY).
///  \param expectedSize The size of shared memory if known.  If not known, set to
///              0.  If you are opening a mapped memory created by another
///              program using CxUtils or Linux, then the size will be discovered
///              automatically.  If not, then the size will be unknown and
///              the Size and Length functions will return 0, even though the
///              memory map is valid.
///
///  \return 0 if the memory doesn't exist,
///          otherwise 1.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::OpenMappedMemory( const std::string& name,
                                    const unsigned int access,
                                    const unsigned int expectedSize)
{
    if( name.empty() ||
        ( access != ReadAccess &&
          access != ReadWriteAccess ) ) 
    {
        return 0;
    }

    unsigned int slength = (unsigned int)(strlen(name.c_str()));
    if(slength > MemoryNameSize) 
    {
        return 0;
    }

#ifdef WIN32

    USES_CONVERSION; //  Need for UNICODE conversion.
    /*  Create a mutex for thread protection of data */
    char mutName[MemoryNameSize+6];
    sprintf(mutName, "%s_Mutex", name.c_str());
    if( mpMemMutex )
    {
        delete mpMemMutex;
        mpMemMutex = NULL;
    }
    mpMemMutex = new Mutex(mutName);
    assert(mpMemMutex);

    mpMemMutex->Lock();

    DWORD attributes = 0;
    if(access == ReadAccess)
        attributes = FILE_MAP_READ;
    else
        attributes = FILE_MAP_READ | FILE_MAP_WRITE;

    mFileMapping = OpenFileMapping(attributes, FALSE, A2W(name.c_str()));
    if(mFileMapping == NULL) 
    {
        mpMemMutex->Unlock();
        return 0;
    }

    mpMemory = (unsigned char *)MapViewOfFile(mFileMapping, attributes, 0, 0, 0);
    if(!mpMemory) 
    {
        mpMemMutex->Unlock();
        delete mpMemMutex;
        mpMemMutex = 0;
        CloseMappedMemory();
        return 0;
    }

    std::string memSizeName = name.c_str();
    memSizeName += "__sIzE__InF0";

    mFileSizeMapping = OpenFileMapping(FILE_MAP_READ, FALSE, A2W(memSizeName.c_str()));

    if(mFileSizeMapping == 0)
    {
        mLength = expectedSize;
    }
    else
    {
        mpMemorySizePtr = (unsigned char*)MapViewOfFile(mFileSizeMapping, FILE_MAP_READ, 0, 0, 0);
        memcpy(&mLength, mpMemorySizePtr , sizeof(unsigned int) );
    }
    memcpy(mMappedName, name.c_str(), slength);
    mWriteAccess = access;
    mPacket.Create(mpMemory + CX_MAPPED_MEMORY_BASE, mLength);
    mpMemMutex->Unlock();
 
    return 1;
#else
    /*  Linux Code Here */
    std::string smName = "/tmp/";
    //FILE* fp;

    smName += name.c_str();
    smName += gPostFix;

    //  Create a mutex for thread protection of data
    char mutName[MemoryNameSize+6];
    sprintf(mutName, "%s", name.c_str());
    if( mpMemMutex )
    {
        delete mpMemMutex;
        mpMemMutex = NULL;
    }
    mpMemMutex = new Mutex(mutName);
    assert(mpMemMutex);

    mpMemMutex->Lock();

    key_t key;
    struct shmid_ds shmbuffer;

    key = ftok(smName.c_str(), O_RDWR);
    // Allocate shared memory segment
    mFileMapping = shmget(key, CX_MAPPED_MEMORY_BASE, S_IRUSR | S_IWUSR);

    if(mFileMapping != -1)
    {
        // Attach to the shared segment.
        mpMemory = (unsigned char *)shmat(mFileMapping, 0, 0);
        // Determine the segments size
        shmctl(mFileMapping, IPC_STAT, &shmbuffer);
        if((mpMemory) != (unsigned char *)-1 && shmbuffer.shm_segsz > CX_MAPPED_MEMORY_BASE)
        {
            mLength = (unsigned int)shmbuffer.shm_segsz;
            // Copy name data.
            memcpy(mMappedName, name.c_str(), slength);
            // Save the lenth, and current positions and return success!
            mWriteAccess = ReadWriteAccess;
            mPacket.Create(mpMemory + CX_MAPPED_MEMORY_BASE, mLength);
            mpMemMutex->Unlock();            
            return 1;
        }
    }

    mpMemMutex->Unlock();

    return 0;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a shared/mapped memory location.
///
///  \param id The ID to use for the shared memory. This is a unique number for
///            other processes to use for lookup.
///  \param size The size in bytes to allocate.
///
///  \return 0 if the memory couldn't be created, 1 if created successfully.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::CreateMappedMemory(const ID id,
                                     const unsigned int size)
{
    std::ostringstream input;
    input << id;
    return CreateMappedMemory(input.str(), size);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a shared/mapped memory location.
///
///  \param name The name of the shared/mapped memory to open.  <b>To maximize
///              portability between windows and linux, this should be an
///              integer number.</b>
///  \param size The size in bytes to allocate.
///
///  \return 0 if the memory couldn't be created, 1 if created successfully.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::CreateMappedMemory(const std::string& name,
                                      const unsigned int size)
{
    if( name.empty() ) 
    {
        return 0;
    }

    unsigned int slength = (unsigned int)(strlen(name.c_str()));
    if(slength > MemoryNameSize) 
    {
        return 0;
    }

#ifdef WIN32
    DWORD attributes = 0;
    int memExists;

    USES_CONVERSION; //  Need for UNICODE conversion.

    /*  Create a named mutex for inter-process protection of data */
    char mutName[MemoryNameSize+6];
    sprintf(mutName, "%s_Mutex", name.c_str());
    if( mpMemMutex )
    {
        delete mpMemMutex;
        mpMemMutex = NULL;
    }
    mpMemMutex = new Mutex(mutName);
    assert(mpMemMutex);

    mpMemMutex->Lock();

    mFileMapping = CreateFileMapping( INVALID_HANDLE_VALUE,
                                      NULL,
                                      PAGE_READWRITE,
                                      0,
                                      sizeof(unsigned char)*size + CX_MAPPED_MEMORY_BASE,
                                      A2W(name.c_str()));
    memExists = GetLastError();
    /*  If the memory already exists, try an open
        it and check to see how bit it is, if it
        is the same size we want, we are ok, otherwise
        return failure gracefully */
    if(memExists == ERROR_ALREADY_EXISTS) 
    {
        mpMemMutex->Unlock();
        return 0;
    }
    
    if(mFileMapping == NULL) 
    {
        mpMemMutex->Unlock();
        return 0;
    }

    memcpy(mMappedName, name.c_str(), slength);

    std::string memSizeName = name.c_str();
    memSizeName += "__sIzE__InF0";
    mFileSizeMapping = CreateFileMapping(INVALID_HANDLE_VALUE,
                                         NULL,
                                         PAGE_READWRITE,
                                         0,
                                         sizeof(unsigned int),
                                         A2W(memSizeName.c_str()));
    mpMemorySizePtr = (unsigned char*)MapViewOfFile(mFileSizeMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);

    //  Map as read/write so we can initialize the memory correctly.
    mpMemory = (unsigned char *)MapViewOfFile(mFileMapping, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0);
    if(!mpMemory) 
    {
        mpMemMutex->Unlock();
        delete mpMemMutex;
        mpMemMutex = NULL;
        CloseMappedMemory();
        return 0;
    }

    /*  All MappedMemory has a Length header
        writed in the front, we must write this to the header*/
    mLength = sizeof(unsigned char)*size + CX_MAPPED_MEMORY_BASE;
    memcpy(mpMemorySizePtr, &mLength, sizeof(unsigned int));
    memcpy(mMappedName, name.c_str(), slength);

    mWriteAccess = ReadWriteAccess;
    mPacket.Create(mpMemory + CX_MAPPED_MEMORY_BASE, mLength);

    //  By default mutex is on when created.
    mpMemMutex->Unlock();

    return 1;
#else
    /*  Linux Code Here */
    std::string smName = "/tmp/";
    FILE* fp;

    smName += name.c_str();
    smName += gPostFix;
    // Create a file to use for shared memory.
    fp = fopen(smName.c_str(), "w+b");
    if(fp == NULL)
    {
        return 0;
    }
    fclose(fp);

    //  Create a mutex for thread protection of data
    char mutName[MemoryNameSize+6];
    sprintf(mutName, "%s", name.c_str());
    if( mpMemMutex )
    {
        delete mpMemMutex;
        mpMemMutex = NULL;
    }
    mpMemMutex = new Mutex(mutName);
    assert(mpMemMutex);

    mpMemMutex->Lock();

    key_t key;
    
    struct shmid_ds shmbuffer;

    mLength = CX_MAPPED_MEMORY_BASE + size;

    key = ftok(smName.c_str(), O_RDWR);
    // Allocate shared memory segment
    mFileMapping = shmget(key, mLength, IPC_CREAT | S_IRUSR | S_IWUSR);
    shmctl(mFileMapping, IPC_STAT, &shmbuffer);
    if(mFileMapping >= 0 && shmbuffer.shm_nattch == 0)
    {
        // Delete old shared memory, and re-create if needed.
        if(shmbuffer.shm_segsz != mLength)
        {
            shmctl(mFileMapping, IPC_RMID, NULL);
            mFileMapping = shmget(key, mLength, IPC_CREAT | S_IRUSR | S_IWUSR);
        }
        // Attach to the shared segment.
        mpMemory = (unsigned char *)shmat(mFileMapping, 0, 0);
        // Determine the segments size
        shmctl(mFileMapping, IPC_STAT, &shmbuffer);
        if((mpMemory) != (unsigned char *)(-1) && mLength == (unsigned int)shmbuffer.shm_segsz)
        {            
            // Copy name data.
            memcpy(mMappedName, name.c_str(), slength);
            // Save the lenth, and current positions and return success!
            mPacket.Create(mpMemory + CX_MAPPED_MEMORY_BASE, mLength);
            mWriteAccess = ReadWriteAccess;
            mCreatedFlag = true;
            mpMemMutex->Unlock();

            return 1;
        }
    }
    
    mpMemMutex->Unlock();

    return 0;
#endif
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Closes any open view of mapped memory.  If this is the last
///  process to close the view of the memory location, it is deleted.
///
///  \return 1 if closed, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::CloseMappedMemory()
{
    int result = 1;

    Lock();

    mPacket.Clear();

#ifdef WIN32
    if(mpMemory) 
    {
        UnmapViewOfFile(mpMemory);
        mpMemory = NULL;
    }
    if(mFileMapping) 
    {
        CloseHandle(mFileMapping);
        mFileMapping = NULL;
    }
    if(mpMemorySizePtr)
    {
        UnmapViewOfFile(mpMemorySizePtr);
        mpMemorySizePtr = NULL;
    }
    if(mFileSizeMapping)
    {
        CloseHandle(mFileSizeMapping);
        mFileSizeMapping = NULL;
    }
    result = 1;
#else
    /* Linux Code */
    if(mpMemory)
    {
        // Detach from shared memory segment.
        shmdt(mpMemory);
    }
    if(mFileMapping > 0)
    {
        struct shmid_ds shminfo;
        shmctl(mFileMapping, IPC_STAT, &shminfo);
        // If there are no other process attached to
        // the shared memory segment, then we must 
        // make sure it is deleted.
        if(shminfo.shm_nattch == 0)
        {
            // Deallocate the segement so that it is
            // deleted after the last process deteaches.
            shmctl(mFileMapping, IPC_RMID, 0);
            // Delete file created.
            std::string smName = "/tmp/";
            smName += mMappedName;
            smName += gPostFix;
            FileIO::DeleteFiles(smName);
        }
    }    
    mpMemory = NULL;
    mFileMapping = 0;
    mCreatedFlag = false;
#endif

    Unlock();

    if(mpMemMutex) 
    {
        delete mpMemMutex;
        mpMemMutex = 0;
    }

    memset(mMappedName, 0, sizeof(char)*MemoryNameSize);
    mLength = 0;
    mWriteAccess = 0;
    mFileMapping = 0;
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This method can be used to forcefully delete a shared memory
///  segment based on it's name.  This method is only useful in Linux.
///
///  In Windows, shared memory gets deleted as soon as the last viewing process
///  ends, but not in Linux systems.  Therefore, sometimes it is necessary
///  to manually delete this data if a program does not exit properly and release
///  the shared memory.
///
///  \return 1 on success, 0 on error.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::DeleteMappedMemory(const std::string& name)
{
    #ifdef WIN32
    // Do nothing, in windows memory can only be
    // deleted when the last viewing process detaches or exits
    return 1;
    #else

    //  Look up the key for the shared memory.
    key_t key;
    int fileMapping = 0;
    std::string smName;
    key = ftok(name.c_str(), O_RDWR);
    // Get the shared memory ID.

    fileMapping = shmget(key, 1, S_IRUSR | S_IWUSR);
    if(fileMapping == -1)
    {
        smName = "/tmp/";
        smName += name.c_str();
        smName += gPostFix;
        key = ftok(smName.c_str(), O_RDWR);

        fileMapping = shmget(key, 1, S_IRUSR | S_IWUSR);
    }

    if(fileMapping != -1)
    {
        // Delete it forcefully.
        shmctl(fileMapping, IPC_RMID, 0);
        if(smName.size() > 0)
        {
            // Delete the generated temp file.
            FileIO::DeleteFiles(smName);
        }
        return 1;
    }
    if(smName.size() > 0)
    {
        // Delete the generated temp file.
        FileIO::DeleteFiles(smName);
    }

    #endif

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Locks the mutex for the shared memory.
///
///  By calling this function, you are enabling a global mutex.  It is important
///  to use this function if a shared memory location is being used for I/O by
///  multiple processes.
///
///  \param[in] wait How long to wait in milliseconds for mutex to lock. Default
///                  is INFINITE.
///
///  \return 1 if locked, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::Lock(unsigned long wait) const
{
    if(mpMemMutex) 
    {
        return mpMemMutex->Lock(wait);
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Unlocks the mutex for the shared memory.
///
///  By calling this function, you are leaving a global mutex.  If you use the
///  Lock function, you must call this when done if you do not want to
///  deadlock your program.
///
///  \return 1 if unlocked, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::Unlock() const
{
    if(mpMemMutex) 
    {
        mpMemMutex->Unlock();
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///   \return Pointer to a packet structure that wrapps the shared memory
///           buffer to simply read/write operations.
///
///////////////////////////////////////////////////////////////////////////////////
const Packet* MappedMemory::operator ->() const
{
    return mPacket.GetData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///   \return Pointer to a packet structure that wrapps the shared memory
///           buffer to simply read/write operations.
///
///////////////////////////////////////////////////////////////////////////////////
Packet* MappedMemory::operator ->()
{
    return mPacket.GetData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///   \return Pointer to a packet structure that wrapps the shared memory
///           buffer to simply read/write operations.
///
///////////////////////////////////////////////////////////////////////////////////
const Packet* MappedMemory::GetData() const
{
    return mPacket.GetData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to get a pointer to the wrapper packet for read/write
///          operations.
///
///   \return Pointer to a packet structure that wrapps the shared memory
///           buffer to simply read/write operations.
///
///////////////////////////////////////////////////////////////////////////////////
Packet* MappedMemory::GetData()
{
    return mPacket.GetData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copies the data starting from the memory index location of p1 to the
///  memory index location of p2.
///
///  This essentially does a memcpy of all the data starting at p1 to
///  the location of p2. memcpy(&mem[p2], &mem[p1], CX_BYTE_SIZE*(length - p1)).
///
///  \param p1 The starting position in the mapped memory to move.
///  \param p2 The destination position to move the memory starting from p1 to.
///
///  \return 1 if moved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int MappedMemory::Copy(const unsigned int p1, const unsigned int p2)
{
    if(!mpMemory || !mLength) 
    {
        return 0;
    }

    unsigned int len = mLength - CX_MAPPED_MEMORY_BASE;
    if(p2 >= p1 || p1 >= len || p2 >= len) 
    {
        return 0;
    }

    memmove(&mpMemory[p2 + CX_MAPPED_MEMORY_BASE],
            &mpMemory[p1 + CX_MAPPED_MEMORY_BASE],
            sizeof(unsigned char)*(len - p1));

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if memory is open, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMemory::IsOpen() const
{
    return mpMemory != NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This method returns a pointer to the begining of the mapped memory.
///  Make sure you have Locked shared memory before reading/writing.
///
///  \return Returns a pointer to the start of shared memory.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char *MappedMemory::GetMemory()
{
    if(mpMemory) 
    {
        return mpMemory + CX_MAPPED_MEMORY_BASE;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This method returns a pointer to the begining of the mapped memory.
///  Make sure you have Locked shared memory before reading/writing.
///
///  \return Returns a pointer to the start of shared memory.
///
////////////////////////////////////////////////////////////////////////////////////
const unsigned char *MappedMemory::GetMemory() const
{
    if(mpMemory) 
    {
        return mpMemory + CX_MAPPED_MEMORY_BASE;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Returns the size of the mapped memory file in bytes.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int MappedMemory::Length() const
{
    if(mpMemory && mLength)
    {
        return mLength - CX_MAPPED_MEMORY_BASE;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Use this function to determine if there is a mapped memory
///  resource already created/open.
///
///  \param name Name of mapped memory resource to check for.
///
///  \return True if there is a shared/mapped memory resource with the name
///  provided already created.
///
////////////////////////////////////////////////////////////////////////////////////
bool MappedMemory::IsMemOpen(const std::string& name)
{
    bool result = false;

#ifdef WIN32
    HANDLE fileMapping;
    USES_CONVERSION; //  Need for UNICODE conversion.
    fileMapping = OpenFileMapping(FILE_MAP_READ, FALSE, A2W(name.c_str()));
    if(fileMapping != NULL) 
    {
        result = true;
        CloseHandle(fileMapping);
    }

#else
    MappedMemory mem;
    if(mem.OpenMappedMemory(name))
    {
        result = true;
    }
#endif

    return result;
}

/*  End of File */
