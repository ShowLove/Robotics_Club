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
#ifndef __JAUS_CORE_TRANSPORT_SHARED_MEMORY_CONNECTION__H
#define __JAUS_CORE_TRANSPORT_SHARED_MEMORY_CONNECTION__H

#include "jaus/core/transport/connection.h"
#include <map>
#include <boost/interprocess/sync/interprocess_mutex.hpp>


namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class SharedMemory
    ///   \brief Defines a JSharedMemory Connection.  It is what is used for all
    ///   interprocess communication on the local host.
    ///
    ///   This implementation of shared memory for JAUS++ only, uses the same
    ///   transport header has JTCP and JUDP so that no data modifications are needed
    ///   when converting from shared memory to those transport.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL SharedMemory : public Connection
    {
    public:
        typedef std::map<Address, Connection::Ptr > Map;
        static const unsigned int DefaultMemorySize = JAUS_USHORT_MAX*10;
        /** SharedMemory Parameters. */
        class JAUS_CORE_DLL Parameters : public Connection::Parameters
        {
        public:
            enum Type
            {
                Client = 0, ///<  Opens a client view of shared memory (doesn't own memory).
                Host,       ///<  Hosts shared memory (owns it).
            };
            Parameters();
            virtual ~Parameters();
            virtual bool LoadSettings(const std::string& xmlSettingsFile);
            SharedMemory::Parameters& operator=(const Info& info)
            {
                *( (Info*)this) = *( (Info *)&info);
                return *this;
            }
            SharedMemory::Parameters& operator=(const SharedMemory::Parameters& params);
            Type mSharedConnectionType;     ///<  Type of shared memory "connection" to create.
            Address mComponentID;           ///<  ID of component to host or open shared memory for.
            Time mCreationTime;             ///<  Time stamp for creation of shared memory.
        };
        SharedMemory(const bool singleThread = true);
        virtual ~SharedMemory();
        virtual bool Initialize(const Connection::Parameters* parameters);
        virtual void Shutdown();
        virtual bool IsConnected() const;
        virtual Connection::Statistics GetStatistics();
        virtual bool CanBroadcast(const int transportType = Transport::All) const { return false; }
        virtual bool SendPacket(const Packet& packet, 
                                const Header& packetHeader) const;
        virtual void UpdateConnection();
        virtual Connection* CreateConnection(const Address& id,
                                             const Connection::Info* destination);
        virtual const Packet& GetTransportHeader() const { return mTransportHeader; }
        /** Gets the recommend maximum packet size for the transport. */
        virtual unsigned int GetMaximumPacketSizeInBytes() const { return mParameters.mMaxPacketSizeBytes; }
        /** Gets the overhead for each packet added by medium in bytes. */
        inline virtual unsigned int GetTransportOverheadInBytes() const { return mTransportHeader.Length(); }
        virtual Parameters* CreateParameters() const { return new SharedMemory::Parameters(); }
        /** Pulls any data from shared memory and shares it. */
        void ReceiveIncommingData();
        /** Gets the last time data was read via this connection from a process. */
        virtual Time::Stamp GetUpdateTimeUtcMs() const;
        /** Method indicates if the connection is send only (doesn't receive) */
        virtual bool IsSendOnly() const { return false; }
        /** Method to check if someone is listing to the connection. */
        bool HasSubscriber(const unsigned int readInterval = 500);
        SharedMemory::Parameters GetParameters() const { return mParameters; }
    protected:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Table
        ///   \brief Class for storing header information that is stored at
        ///   the beginning of shared memory.  It is used to impelement a 
        ///   cicrular buffer type system within shared memory.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Table
        {
        public:
            static const unsigned int HeaderSize = 29; ///<  Size of message box header in bytes.
            Table();
            Table(const Table& header);
            ~Table();
            bool IsValid() const { return (mValidFlag > 0 || mUpdateTimeMs != 0); }
            Table& operator=(const Table& header);
            unsigned char mValidFlag;       ///<  If 1, memory is valid, if 0, host is trying to close.
            Time::Stamp mUpdateTimeMs;      ///<  The last time a message was written to box in ms.
            unsigned int mCount;            ///<  The current number of messages stored in the box in ms.
            unsigned int mStartBytePos;     ///<  The byte index of the first message in the box (from memory index 0).
            unsigned int mEndBytePos;       ///<  The ending byte index of the last message (from memory index 0).
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Box
        ///   \brief Data structure for contents of shared memory, including
        ///          interprocess mutex for interprocess synchronization.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Box
        {
        public:
            Box() {}
            ~Box() {}
            boost::interprocess::interprocess_mutex mReadMutex;     ///<  For synchronization
            boost::interprocess::interprocess_mutex mWriteMutex;    ///<  For synchronization
            Table mReadTable;             ///<  Contents of read region.
            Table mWriteTable;            ///<  Contents of write region.
            unsigned char mpWriteRegion[DefaultMemorySize]; ///<  Write region (where to write outgoing data to).
            unsigned char mpReadRegion[DefaultMemorySize];  ///<  Read region (where to read data from).
        };
        Packet mTransportHeader;            ///<  Transport header (blank).
        Parameters mParameters;             ///<  Parameters.
        volatile bool mConnectedFlag;       ///<  Signals connected or not.
        std::string mSharedMemoryName;      ///<  Name of shared memory object.
        void* mpSharedObject;               ///<  Shared object/structure in shared memory.
        void* mpMappedObjectRegion;         ///<  Mapped region of memory for data structure.
        SharedMemory::Box* mpBox;           ///<  Mapped memory data.
    };
}

#endif
/*  End of File */
