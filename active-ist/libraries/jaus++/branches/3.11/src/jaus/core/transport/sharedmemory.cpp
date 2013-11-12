////////////////////////////////////////////////////////////////////////////////////
///
///  \file sharedmemory.h
///  \brief Shared Memory interface transport services.
///
///  <br>Author(s): Daniel Barber
///  <br>Copyright (c) 2011
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
#include "jaus/core/transport/sharedmemory.h"
#include <tinyxml/tinyxml.h>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>
#include <boost/interprocess/permissions.hpp>
#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/mapped_region.hpp>
#include <boost/interprocess/sync/scoped_lock.hpp>
#include <boost/interprocess/sync/named_mutex.hpp>

using namespace JAUS;
using namespace boost::interprocess;

#define JAUS_NM_SHARED_NAME "JAUS++NodeManagerSM"
#define JAUS_NM_MUTEX_NAME  "JAUS++NodeManagerMutex"

typedef boost::interprocess::scoped_lock<boost::interprocess::named_mutex> NamedLock;
typedef boost::interprocess::scoped_lock<boost::interprocess::interprocess_mutex> InterprocessLock;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Parameters::Parameters()
{
    mSharedConnectionType = Client;
    this->mMaxPacketSizeBytes = JAUS_USHORT_MAX;
    mCreationTime.SetCurrentTime();
    this->mTransportType = Connection::Transport::JSharedMemory;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Parameters::~Parameters()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads any transport settings from the file.
///
///   \param[in] xmlSettingsFile Name of the file containing data to load.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedMemory::Parameters::LoadSettings(const std::string& xmlSettingsFile)
{
    bool result = true;

    result |= Connection::Parameters::LoadSettings(xmlSettingsFile);

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
///   \param[in] params Parameters to be equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Parameters& SharedMemory::Parameters::operator=(const SharedMemory::Parameters& params)
{
    if(this != &params)
    {
        CopyBaseData(&params);
        mSharedConnectionType = params.mSharedConnectionType;
        mComponentID = params.mComponentID;
        mCreationTime = params.mCreationTime;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] singleThread If true, operate in single thread mode.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::SharedMemory(const bool singleThread) : Connection(singleThread)
{
    mTransportType = Connection::Transport::JSharedMemory;
    mConnectedFlag = false;

    this->mLocalConnectionFlag = true;

    mpSharedObject = NULL;
    mpMappedObjectRegion = NULL;
    mpBox = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::~SharedMemory()
{
    //Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the connection with given parameter data.
///
///   \param[in] parameters Parameter data for SharedMemory connection.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedMemory::Initialize(const Connection::Parameters* parameters)
{
    bool result = false;

    WriteLock wLock(mConnectionMutex);

    if(dynamic_cast<const SharedMemory::Parameters*>(parameters) == NULL)
    {
        return result;
    }

    mParameters = *dynamic_cast<const SharedMemory::Parameters*>(parameters);
    mStats.Clear();
    mStats.mTransportType = Connection::Transport::JSharedMemory;

    std::stringstream portName;
    portName << mParameters.mSourcePortNumber;

    std::string sharedObjectName;
    if(mParameters.mComponentID.IsValid() == false)
    {
        // Node Manager
        sharedObjectName = std::string(JAUS_NM_SHARED_NAME) + ":" + portName.str();
    }
    else
    {
        sharedObjectName = mParameters.mComponentID.ToString() + ":" + mParameters.mCreationTime.ToString();
    }

    mSharedMemoryName = sharedObjectName;

    // If not client, then we are hosting the memory segment.
    if(mParameters.mSharedConnectionType == Parameters::Host)
    {
        try
        {
            mID = mParameters.mComponentID;

            mStats.mPortName = mParameters.mComponentID.ToString();
            mStats.mSourcePortNumber = mStats.mDestPortNumber = mParameters.mSourcePortNumber;
            mStats.mSourceIP = mStats.mDestIP = IP4Address();
            mConnectedFlag = true;

            // Create memory

            // Erase previous shared memory in case it exists
            boost::interprocess::shared_memory_object::remove(sharedObjectName.c_str());

            mpSharedObject = new boost::interprocess::shared_memory_object(create_only,
                                                                            sharedObjectName.c_str(),
                                                                            read_write);
            unsigned int sz = sizeof(SharedMemory::Box);
            ((boost::interprocess::shared_memory_object*)mpSharedObject)->truncate(sizeof(SharedMemory::Box));
            mpMappedObjectRegion = new boost::interprocess::mapped_region(*((boost::interprocess::shared_memory_object*)mpSharedObject),
                                                                            read_write);

            mpBox = new (((boost::interprocess::mapped_region*)mpMappedObjectRegion)->get_address())SharedMemory::Box();

            {
                InterprocessLock iLock(mpBox->mWriteMutex);
                mpBox->mWriteTable.mStartBytePos = 0;
                mpBox->mWriteTable.mEndBytePos = 0;
                mpBox->mWriteTable.mCount = 0;

                mpBox->mWriteTable.mUpdateTimeMs = Time::GetUtcTimeMs();
                mpBox->mWriteTable.mValidFlag = true;
            }
            {
                InterprocessLock iLock(mpBox->mReadMutex);
                mpBox->mReadTable.mStartBytePos = 0;
                mpBox->mReadTable.mEndBytePos = 0;
                mpBox->mReadTable.mCount = 0;

                mpBox->mReadTable.mUpdateTimeMs = Time::GetUtcTimeMs();
                mpBox->mReadTable.mValidFlag = true;
            }

            mConnectedFlag = true;
            result = true;
        }
        catch(boost::interprocess::interprocess_exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    else
    {
        // Shared memory regions are reversed for
        // clients (client sends to host inbox, receives from host outbox).

        try
        {
            mID = mParameters.mComponentID;

            mStats.mPortName = mParameters.mComponentID.ToString();
            mStats.mSourcePortNumber = mStats.mDestPortNumber = mStats.mSourcePortNumber;
            mStats.mSourceIP = mStats.mDestIP = IP4Address();
            mStats.mDestinationID = mParameters.mComponentID;
            mConnectedFlag = true;

            // Open memory

            mpSharedObject = new boost::interprocess::shared_memory_object(open_only,
                                                                            sharedObjectName.c_str(),
                                                                            read_write);
            mpMappedObjectRegion = new boost::interprocess::mapped_region(*((boost::interprocess::shared_memory_object*)mpSharedObject),
                                                                          read_write);

            mpBox = static_cast<SharedMemory::Box*>(((boost::interprocess::mapped_region*)mpMappedObjectRegion)->get_address());

            result = true;
        }
        catch(boost::interprocess::interprocess_exception& ex)
        {
            std::cout << ex.what() << std::endl;
        }
    }
    if(result == false)
    {
        WriteLock wLock(mConnectionMutex);

        if(mpSharedObject)
        {
            delete ((boost::interprocess::shared_memory_object*)mpSharedObject);
        }
        if(mpMappedObjectRegion)
        {
            delete ((boost::interprocess::mapped_region*)mpMappedObjectRegion);
        }
        mpSharedObject = NULL;
        mpMappedObjectRegion = NULL;
        mpBox = NULL;
    }

    // Create a thread if required to pull data from shared memory automatically and consume it.
    else if(mSingleThreadModeFlag == false)
    {
        this->mUpdateTimeMs = Time::GetUtcTimeMs();
        this->mUpdateConnectionThread.SetThreadName(mSharedMemoryName);
        result = this->mUpdateConnectionThread.CreateThread(Connection::UpdateConnectionThread, this) > 0 ? true : false;
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the connection.
///
////////////////////////////////////////////////////////////////////////////////////
void SharedMemory::Shutdown()
{
    mConnectedFlag = false;

    this->mUpdateConnectionThread.StopThread();

    // If we are hosting the shared memory, than we must mark it
    // has closed
    if(mParameters.mSharedConnectionType == Parameters::Host)
    {
        WriteLock wLock(mConnectionMutex);
        if(mpBox)
        {
            {
                InterprocessLock iLock(mpBox->mWriteMutex);
                mpBox->mWriteTable.mValidFlag = false;
                mpBox->mWriteTable.mUpdateTimeMs = 0;
            }
            {
                InterprocessLock iLock(mpBox->mWriteMutex);
                mpBox->mReadTable.mValidFlag = false;
                mpBox->mReadTable.mUpdateTimeMs = 0;
            }
        }
    }

    {
        WriteLock wLock(mConnectionMutex);

        if(mpSharedObject)
        {
            delete ((boost::interprocess::shared_memory_object*)mpSharedObject);
        }
        if(mpMappedObjectRegion)
        {
            delete ((boost::interprocess::mapped_region*)mpMappedObjectRegion);
        }
        mpSharedObject = NULL;
        mpMappedObjectRegion = NULL;
    }
    mpBox = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if a client is connected/valid otherwise false.  This is 
///           not the same as if the connection is initialized.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedMemory::IsConnected() const
{
    return mConnectedFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current stats of connection.
///
////////////////////////////////////////////////////////////////////////////////////
Connection::Statistics SharedMemory::GetStatistics()
{
    Connection::Statistics stats;
    {
        ReadLock rLock(mConnectionMutex);
        stats = mStats;
        mStats.mConnectionNumber = stats.mConnectionNumber = mConnectionNumber;
        mStats.mBytesReceived = mStats.mBytesSent = mStats.mMessagesReceived = mStats.mMessagesSent = 0;
    }
    return stats;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the packet over the connection, adding any transport
///          information as needed.
///
///   \param[in] packet JAUS packet with no additional transport overhead.
///   \param[in] packetHeader JAUS general header data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SharedMemory::SendPacket(const Packet& packet,
                              const Header& packetHeader) const
{
    bool result = false;

    if(mConnectedFlag == false)
    {
        return result;
    }

    try
    {
        // Lock this structure for current process.
        ReadLock rLock(*((SharedMutex*)&mConnectionMutex));
        if(mpBox == NULL)
        {
            return result;
        }

        char* mem = NULL;
        boost::interprocess::interprocess_mutex* mutex;
        Table* table = NULL;
        unsigned int regionSize = DefaultMemorySize;

        // If hosting memory, we send to our write region
        if(mParameters.mSharedConnectionType == SharedMemory::Parameters::Host)
        {
            mem = (char *)mpBox->mpWriteRegion;
            table = &(mpBox->mWriteTable);
            mutex = (boost::interprocess::interprocess_mutex*)&(mpBox->mWriteMutex);
        }
        // If we are a client, we sent to the hosts read region
        else
        {
            mem = (char *)mpBox->mpReadRegion;
            table = &(mpBox->mReadTable);
            mutex = (boost::interprocess::interprocess_mutex*)&(mpBox->mReadMutex);
        }

        // Lock between programs
        InterprocessLock iLock(*mutex);

        // See if we can fit the data
        unsigned int availableBytes = regionSize;
        unsigned int messageSize = packet.Length() + sizeof(UInt);

        if(messageSize < availableBytes)
        {
            // OK, so we know the message can fit in this box, but first
            // lets see if we need to "delete" old data.
            if(messageSize + table->mEndBytePos >= regionSize && table->mStartBytePos > 0)
            {
                unsigned int dataSize = table->mEndBytePos - table->mStartBytePos;
                memmove(mem, 
                        mem + table->mStartBytePos, 
                        dataSize);
                table->mEndBytePos = dataSize;
                table->mStartBytePos = 0;
                table->mUpdateTimeMs = Time::GetUtcTimeMs();
            }

            if(messageSize + table->mEndBytePos > (unsigned int)(regionSize - 1))
            {
                // Box is full, just clear it out.
                table->mStartBytePos = table->mStartBytePos = 0;
                table->mCount = 0;
            }
            if(messageSize + table->mEndBytePos < (unsigned int)(regionSize - 1))
            {
                unsigned int pos = table->mEndBytePos;
                UInt length = packet.Length();
                std::memcpy(mem + pos, &length, sizeof(UInt)); pos += sizeof(UInt);
                std::memcpy(mem + pos, packet.Ptr(), length);
                table->mCount++;
                table->mEndBytePos += length + sizeof(UInt);
                table->mUpdateTimeMs = Time::GetUtcTimeMs();
                result = true;
            }
        }
        else
        {
            throw boost::interprocess::interprocess_exception("Shared Memory Full");
        }
    }
    catch(boost::interprocess::interprocess_exception &ex)
    {
        std::cout << ex.what() << std::endl;
        return false;
    }

    if(result)
    {
        // Update stats.
        WriteLock wLock(*((SharedMutex *)&mConnectionMutex));
        Connection::Statistics* stats = (Connection::Statistics*)&mStats;
        stats->mMessagesSent++;
        stats->mTotalMessagesSent++;
        stats->mBytesSent += packet.Length();
        stats->mTotalBytesSent += packet.Length();
    }
    return result;
}


/** Updates the current state of the connection. */
void SharedMemory::UpdateConnection()
{
    if(GetGlobalShutdownSignal())
    {
        return;
    }
    ReceiveIncommingData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a connection to the JAUS component given the provided data
///   if possible.
///
///   \param[in] id JAUS component ID.
///   \param[in] destination Destination information for connection creation.
///
///   \return Pointer to new connection on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
Connection* SharedMemory::CreateConnection(const Address& id,
                                           const Info* destination)
{
    SharedMemory* newSharedMemory = new SharedMemory(mSingleThreadModeFlag);

    newSharedMemory->mID = id;
    newSharedMemory->CopyConnectionData(this);

    Parameters params;
    params.mSharedConnectionType = Parameters::Client;
    params.mComponentID = id;
    if(newSharedMemory->Initialize(&params))
    {
        return newSharedMemory;
    }
    else
    {
        delete newSharedMemory;
    }

    return NULL;
}

//#define AVERAGE_STATS

#ifdef AVERAGE_STATS
    #include <cxutils/timer.h>
    #include <cxutils/math/runningstats.h>
#endif

/** Method to listen for new incomming data over the
    current socket connection. */
void SharedMemory::ReceiveIncommingData()
{

    if(mConnectedFlag == false || this->GetGlobalShutdownSignal())
    {
        return;
    }
    
    Packet packet;
    packet.Reserve(Header::MinSize*2);

    bool result = false;
#ifdef AVERAGE_STATS
    static CxUtils::RunningStats stats;
    double average = 0;
#endif
    try
    {
#ifdef AVERAGE_STATS
        double start = CxUtils::Timer::GetTimeSeconds();
#endif
        ReadLock rLock(mConnectionMutex);

        char* mem = NULL;
        boost::interprocess::interprocess_mutex* mutex = NULL;
        Table* table = NULL;
        unsigned int regionSize = DefaultMemorySize;

        // If hosting memory, we read from our write region
        if(mParameters.mSharedConnectionType == SharedMemory::Parameters::Host)
        {
            mem = (char *)mpBox->mpReadRegion;
            table = &(mpBox->mReadTable);
            mutex = &(mpBox->mReadMutex);
        }
        // If we are a client, we read from the hosts read region
        else
        {
            mem = (char *)mpBox->mpWriteRegion;
            table = &(mpBox->mWriteTable);
            mutex = &(mpBox->mWriteMutex);
        }

        if(mutex)
        {
            // Lock
            InterprocessLock iLock(*mutex);

            if(table->mValidFlag)
            {
                // If message in memory
                if(table->mCount > 0 && table->mStartBytePos < table->mEndBytePos)
                {
                    // Get the message size
                    UInt length;
                    unsigned int pos = table->mStartBytePos;
                    std::memcpy(&length, mem + pos, sizeof(UInt)); 
                    pos += sizeof(UInt);
                    if(length > 0)
                    {
                        packet.Reserve(length + 1);
                        packet.Clear(false);
                        std::memcpy(packet.Ptr(), mem + pos, length);
                        packet.SetLength(length);
                        pos += length;
                    }

                    table->mCount--;

                    // If empty, reset positions in buffer
                    if(table->mCount == 0)
                    {
                        table->mStartBytePos = table->mEndBytePos = 0;
                    }
                    else
                    {
                        table->mStartBytePos = pos;
                    }

                    mConnectedFlag = true;

                    result = true;
                }
                // Update the read time.
                table->mUpdateTimeMs = Time::GetUtcTimeMs();
            }
        }
        // Check for signal that data is bad.
        if(table->mUpdateTimeMs == 0 || table->mValidFlag == 0)
        {
            mConnectedFlag = false;
        }

#ifdef AVERAGE_STATS
        double end = CxUtils::Timer::GetTimeSeconds();
        if(result)
        {
            stats.Push(end - start);
            average = stats.Mean();
        }
#endif
    }
    catch(boost::interprocess::interprocess_exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }

    if(result)
    {
        {
            // Update stats.
            WriteLock wLock(*((SharedMutex *)&mConnectionMutex));
            Connection::Statistics* stats = (Connection::Statistics*)&mStats;
            stats->mMessagesReceived++;
            stats->mTotalMessagesReceived++;
            stats->mBytesReceived += packet.Length();
            stats->mTotalBytesReceived += packet.Length();
        }
        // Process data
        Packet::Wrapper jausPacket(packet.Ptr(), 
                                   packet.Length() );
        Header jausHeader;
        if(jausHeader.Read(*jausPacket.GetData()))
        {
            Connection::Info sourceInfo;
            sourceInfo.mTransportType = this->mTransportType;
            sourceInfo.mPortName = mSharedMemoryName;
            sourceInfo.mDestIP.Clear();
            sourceInfo.mDestPortNumber = mParameters.mDestPortNumber;
            sourceInfo.mSourcePortNumber = mParameters.mSourcePortNumber;
            // Now share!
            SendToCallbacks(*jausPacket.GetData(),
                            jausHeader,
                            &sourceInfo);
        }
    }
}


/** Methood to get the last time shared memory was read from. */
Time::Stamp SharedMemory::GetUpdateTimeUtcMs() const
{
    bool result = false;
    Table table;

    if(mConnectedFlag == false || this->GetGlobalShutdownSignal())
    {
        return 0;
    }

    try
    {
        ReadLock rLock(*((SharedMutex*)&mConnectionMutex));
        if(mpBox)
        {
            if(mParameters.mSharedConnectionType == SharedMemory::Parameters::Host)
            {
                InterprocessLock iLock(mpBox->mWriteMutex);
                table = mpBox->mWriteTable;
            }
            else
            {
                InterprocessLock iLock(mpBox->mReadMutex);
                table = mpBox->mReadTable;
            }
            result = true;
        }
    }
    catch(boost::interprocess::interprocess_exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    
    if(result)
    {
        // The read time represents when the owner
        // of the data last checked the memory segment
        // for any information, and is what we can
        // use to check for liveness.
        return table.mUpdateTimeMs;
    }
    else
    {
        return 0;
    }
}


/** Method to check if someone is listing to the connection.
   
   \param readInterval How long you expect between reads (for example
                       if 100 ms, and it has been more than 100 ms since
                       a read operation, then there is no subscriber. 
*/
bool SharedMemory::HasSubscriber(const unsigned int readInterval)
{
    bool result = false;
    Table table;
    if(mConnectedFlag)
    {
        try
        {
            ReadLock rLock(*((SharedMutex*)&mConnectionMutex));
            InterprocessLock iLock(mpBox->mWriteMutex);
            table = mpBox->mWriteTable;
            /*
            // Lock
            NamedLock wLock(*mpWriteSharedMutex);

            if(table.Read(mpWriteRegion))
            {
                result = true;
            }
            */
        }
        catch(boost::interprocess::interprocess_exception &ex)
        {
            std::cout << ex.what() << std::endl;
            return false;
        }
        Time::Stamp currentTimeMs = Time::GetUtcTimeMs();
        if(currentTimeMs - table.mUpdateTimeMs < (Time::Stamp)readInterval)
        {
            result = true;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes all values to 0.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Table::Table() : mValidFlag(1),
                               mUpdateTimeMs(0),
                               mCount(0),
                               mStartBytePos(Table::HeaderSize),
                               mEndBytePos(Table::HeaderSize)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Table::Table(const SharedMemory::Table& header) :  mUpdateTimeMs(0),
                                                                 mCount(0),
                                                                 mStartBytePos(0),
                                                                 mEndBytePos(0)
{
    *this = header;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Table::~Table()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SharedMemory::Table& SharedMemory::Table::operator =(const SharedMemory::Table& header)
{
    if(this != &header)
    {
        mValidFlag = header.mValidFlag;
        mUpdateTimeMs = header.mUpdateTimeMs;
        mCount = header.mCount;
        mStartBytePos = header.mStartBytePos;
        mEndBytePos = header.mEndBytePos;
    }
    return *this;
}


/*  End of File */
