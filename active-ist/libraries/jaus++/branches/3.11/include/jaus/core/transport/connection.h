////////////////////////////////////////////////////////////////////////////////////
///
///  \file connection.h
///  \brief Connection interface for transport services.
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
#ifndef __JAUS_CORE_TRANSPORT_CONNECTION__H
#define __JAUS_CORE_TRANSPORT_CONNECTION__H

#include "jaus/core/message.h"
#include "jaus/core/service.h"
#include <set>
#include <cxutils/networking/ip4address.h>
#include <boost/shared_ptr.hpp>

namespace JAUS
{
    typedef CxUtils::IP4Address IP4Address; ///<  Forward type defintion.
    class NodeManager;  // Forward

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Connection
    ///   \brief Connection defines a common interface for different type of
    ///          transport service connections over JAUS, and represents a
    ///          single connection type.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Connection
    {
        friend class NodeManager;
    public:
        typedef boost::shared_ptr<Connection> Ptr;  ///<  Connection pointer.
        typedef std::map<UInt, Ptr> Map;            ///<  Connection map.
        typedef std::set<Ptr> Set;                  ///<  Set of connections.
        /** Connection type defintions. */
        class JAUS_CORE_DLL Transport
        {
        public:
            static const int JTCP          = 0x0001;
            static const int JUDP          = 0x0002;
            static const int JSerial       = 0x0004;
            static const int JSharedMemory = 0x0008;
            static const int All           = 0xFFFF;
            static std::string ToString(const int type)
            {
                switch(type)
                {
                    case JUDP:
                        return "JUDP";
                        break;  
                    case JSerial:
                        return "JSerial";
                        break;
                    case JSharedMemory:
                        return "SM";
                        break;
                    default:
                        return "JTCP";
                    break;
                }
                return "Unknown";
            }
        };
        /** Contains information about the source of the packet that
            may be specific to a transport mechanism type. */
        class JAUS_CORE_DLL Info
        {
        public:
            Info() : mDestPortNumber(3794), 
                     mSourcePortNumber(3794), 
                     mDestIP("127.0.0.1"), 
                     mSourceIP("127.0.0.1"), 
                     mTransportType(Transport::JTCP) {}
            virtual ~Info() {Destroy(); }
            virtual void Destroy() {}
            Info& operator=(const Info& source)
            {
                mTransportType = source.mTransportType;
                mPortName = source.mPortName;
                mDestIP = source.mDestIP;
                mSourceIP = source.mSourceIP;
                mDestPortNumber = source.mDestPortNumber;
                mSourcePortNumber = source.mSourcePortNumber;
                return *this;
            }
            int mTransportType;               ///<  Connection type information.
            std::string mPortName;            ///<  Portname (e.g. COM1, /dev/ttys0)
            IP4Address mDestIP;               ///<  IP address of source/destination.
            IP4Address mSourceIP;             ///<  Source IP address (almost always localhost).
            unsigned short mDestPortNumber;   ///<  Port number to send to (e.g. 3794).
            unsigned short mSourcePortNumber; ///<  Source port number.
        };
        /** Statistics about a connection on the Node Manager. */
        class JAUS_CORE_DLL Statistics : public Info
        {
        public:
            typedef std::vector<Statistics> List;
            Statistics()
            {
                Clear();
            }
            Statistics(const Statistics& stats) { *this = stats; }
            ~Statistics() {}
            void Clear()
            {
                mConnectionNumber = 0;
                mSourceID = mDestinationID = Address();
                mMessagesReceived = 0;
                mTotalMessagesReceived = 0;
                mMessagesSent = 0;
                mTotalMessagesSent = 0;
                mBytesReceived = 0;
                mTotalBytesReceived = 0;
                mBytesSent = 0;
                mTotalBytesSent = 0;
            }
            Statistics& operator=(const Statistics& stats)
            {
                if(this != &stats)
                {
                    *( (Info*)this) = *( (Info *)&stats);
                    mConnectionNumber = stats.mConnectionNumber;
                    mSourceID = stats.mSourceID;
                    mDestinationID = stats.mDestinationID;
                    mMessagesReceived = stats.mMessagesReceived;
                    mTotalMessagesReceived = stats.mTotalMessagesReceived;
                    mMessagesSent = stats.mMessagesSent;
                    mTotalMessagesSent = stats.mTotalMessagesSent;
                    mBytesReceived = stats.mBytesReceived;
                    mTotalBytesReceived = stats.mTotalBytesReceived;
                    mBytesSent = stats.mBytesSent;
                    mTotalBytesSent = stats.mTotalBytesSent;
                }
                return *this;
            }
            unsigned int mConnectionNumber;         ///<  Connection number.
            Address mSourceID;                      ///<  Source ID.
            Address mDestinationID;                 ///<  Destination ID.
            unsigned int mMessagesReceived;         ///<  Total messages received since last check.
            unsigned int mTotalMessagesReceived;    ///<  Total messages received.
            unsigned int mMessagesSent;             ///<  Total messages sent since last check.
            unsigned int mTotalMessagesSent;        ///<  Total messages sent.
            unsigned int mBytesSent;                ///<  Number of bytes sent since last check.
            unsigned int mTotalBytesSent;           ///<  Total bytes sent.
            unsigned int mBytesReceived;            ///<  Number of bytes received since last check.
            unsigned int mTotalBytesReceived;       ///<  Total bytes received.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief A connection callback is a mechanism to receive any JAUS messages
        ///          received.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Callback
        {
        public:
            typedef std::set<Callback*> Set;
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Method to overload to get packet data received by the
            ///          connection.
            ///
            ///   \param[in] jausPacket JAUS Packet data without the transport header.
            ///   \param[in] jausHeader JAUS General Header information for the packet.
            ///   \param[in] connection Pointer to the connection the data was received by.
            ///   \param[in] sourceInfo Additional information about the source of the packet
            ///                         like the address it came in on.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual void ProcessPacket(const Packet& jausPacket,
                                       const Header& jausHeader,
                                       const Connection* connection,
                                       const Connection::Info* sourceInfo) {}
        };
        /** Defines any parameters used in initialization of the connection. */
        class JAUS_CORE_DLL Parameters : public Info
        {
        public:
            Parameters() { mMaxPacketSizeBytes = 1500; }
            virtual ~Parameters() {}
            virtual bool LoadSettings(const std::string& xmlSettingsFile);
            virtual void CopyBaseData(const Parameters* parentObj)
            {
                if(this != parentObj)
                {
                    *( (Info*)this) = *( (Info *)parentObj);
                    mNetworkInterface = parentObj->mNetworkInterface;
                    mMaxPacketSizeBytes = parentObj->mMaxPacketSizeBytes;
                }
            }
            Parameters& operator=(const Info& info)
            {
                *( (Info*)this) = *( (Info *)&info);
                return *this;
            }
            IP4Address mNetworkInterface;       ///<  Network interface to connecton (for IP based connections).
            unsigned int mMaxPacketSizeBytes;   ///<  Maximum packet size in bytes, should match MTU for network cards, etc.
        };
        /** Thread class for updating connections (i.e. refresh). */
        class JAUS_CORE_DLL Thread : public CxUtils::Thread
        {
        public:
            static const unsigned int MaxConnections;   ///<  Maximum connections per refresher.
            Thread(NodeManager* parent = NULL, 
                   volatile bool* shutdownFlag = NULL,
                   const bool updatParent = false);
            ~Thread();
            void SetParent(NodeManager* parent, 
                           volatile bool* shutdownFlag, 
                           const bool updateParent);
            bool AddConnection(Connection::Ptr connection);
            void RemoveConnection(Connection::Ptr connection);
            void ShutdownConnections();
            unsigned int GetNumConnections() const;
        protected:
            virtual void Execute();
            SharedMutex mConnectionsMutex;      ///<  For thread protection.
            Connection::Set mConnections;       ///<  Connections.
            volatile bool* mpShutdownFlag;      ///<  Shutdown flag.
            NodeManager* mpParent;              ///<  Node Manager pointer.
            bool mUpdateParentFlag;             ///<  If true, updates parent also.
        };
        Connection(const bool singleThread);
        virtual ~Connection();
        /** Initializes connection with given parameters. */
        virtual bool Initialize(const Connection::Parameters* parameters) = 0;
        /** Shutdown the connection. */
        virtual void Shutdown() = 0;
        /** Method to check if connection is valid. */
        virtual bool IsConnected() const = 0;
        /** Gets the connection statistics, and resets current values. */
        virtual Connection::Statistics GetStatistics() = 0;
        /** Method to indicate the interface has broadcasting capabilities. */
        virtual bool CanBroadcast(const int transportType = Transport::All) const = 0;
        /** Send a serialized message. */
        virtual bool SendPacket(const Packet& packet,
                                const Header& packetHeader) const = 0;
        /** Method called to update the connection to do send/recv. operations. */
        virtual void UpdateConnection() = 0;
        /** Method to create a connection to a new source of data. */
        virtual Connection* CreateConnection(const Address& id,
                                             const Info* destination) = 0;
        /** Get a copy of the connections transport header */
        virtual const Packet& GetTransportHeader() const = 0;
        /** Gets the overhead for each packet added by medium in bytes. */
        virtual unsigned int GetTransportOverheadInBytes() const = 0;
        /** Method indicates if the connection is send only (doesn't receive) */
        virtual bool IsSendOnly() const { return false; }
        /** Gets the recommend maximum packet size for the transport. */
        virtual unsigned int GetMaximumPacketSizeInBytes() const { return 1500; }
        /** Creates a parameters object for initialization. */
        virtual Parameters* CreateParameters() const = 0;
        /** Method to check if the connection needs to be updated or modified. */
        virtual bool ConnectionChanged(const Connection::Info* destination)  const { return false; }
        /** Checks if the connection transport type is the same. */
        virtual bool IsSameAs(Connection* connection) const { return mTransportType == connection->mTransportType; }
        /** Gets the connection type. */
        inline int GetConnectionTransportType() const { return mTransportType; }
        /** Gets the JAUS ID of the connection. */
        inline Address GetConnectionID() const { return mID; }
        /** Sets the ID of the connection destination. */
        inline void SetConnectionID(const Address& id) { if(id.IsValid()) {mID = id;} }
        /** Sets the source ID of the connection. */
        inline void SetConnectionSourceID(const Address& id) { if(id.IsValid()) {mSourceID = id;} }
        /** Indicates connections is not running a thread and needs to be
            updated manually via UpdateConnection. */
        inline bool IsSingleThreadMode() const { return mSingleThreadModeFlag; }
        /** Method to register a callback to get data as it arrives. */
        virtual void RegisterCallback(Callback* pointer);
        /** Clears callbacks. */
        void ClearCallbacks();
        /** Method to know if connection is on the local host machine, which is used to
            support broadcasting options. */
        inline bool IsLocalConnection() const { return mLocalConnectionFlag; }
        /** Method to set if the connection was made manually via/code or settings file and
            may not be discoverable dynamically. */
        inline void SetFixedConnection(const bool fixed = true) { mFixedConnectionFlag = fixed; }
        /** Method to know if the connection was made manually via/code or settings file and
            may not be discoverable dynamically. */
        inline bool IsFixedConnection() const { return mFixedConnectionFlag; }
        /** Method to set delay in thread loop updates for receiving data in multi-threaded mode.
            A value of 0 means as fast as possible. */
        void SetConnectionUpdateDelayMs(const unsigned int delay)
        {
            if(mUpdateConnectionThread.IsThreadActive() == false) mConnectionUpdateDelayMs = delay;
        }
        /** Gets the connection update delay values. */
        inline unsigned int GetConnectionUpdateDelayMs() const { return mConnectionUpdateDelayMs; }
        /** Sets the global shutdown parameters. */
        void SetGlobalShutdownFlag(volatile bool* flag) { mpGlobalShutdownFlag = flag; }
        /** Signals all connections in process to begin shutdown process. */
        void SignalGlobalShutdown(const bool value = true)
        {
            if(mpGlobalShutdownFlag)
            {
                *mpGlobalShutdownFlag = value;
            }
        }
        /** Gets global shutdown signal status. */
        bool GetGlobalShutdownSignal() const
        {
            if(mpGlobalShutdownFlag != NULL)
            {
                return *mpGlobalShutdownFlag;
            }
            else
            {
                return false;
            }
        }
        /** Gets the last time data was received via this connection. */
        virtual Time::Stamp GetUpdateTimeUtcMs() const { return mUpdateTimeMs; }
        /** Sets the node manager pointer. */
        void SetNodeManager(NodeManager* nm);
        /** Updates the connection update time. */
        void SignalConnectionUpdate(unsigned int bytesReceived = 0)
        {
            mUpdateTimeMs = Time::GetUtcTimeMs();
            if(bytesReceived > 0)
            {
                WriteLock wLock(mConnectionMutex);
                mStats.mMessagesReceived++;
                mStats.mTotalMessagesReceived++;
                mStats.mBytesReceived += bytesReceived;
                mStats.mTotalBytesReceived += bytesReceived;
            }
        }
    protected:
        static void UpdateConnectionThread(void* args);
        virtual void CopyConnectionData(Connection* connection);
        virtual bool SendToCallbacks(const Packet& jausPacket,
                                     const Header& jausHeader,
                                     const Connection::Info* sourceInfo);
        int mTransportType;                         ///<  Connection type (e.g. JTCP, JUDP, JSerial)
        Address mID;                                ///<  ID of the JAUS component the connection is for.
        Address mSourceID;                          ///<  ID of the JAUS component the connection is from.
        bool mLocalConnectionFlag;                  ///<  Is this a connection on the local machine.
        bool mFixedConnectionFlag;                  ///<  If true, this connection was manually created and may not have discovery capabilities.
        volatile bool* mpGlobalShutdownFlag;        ///<  If true, all connection should terminate.
        const bool mSingleThreadModeFlag;           ///<  If true, library is running in single thread mode.
        SharedMutex mConnectionMutex;               ///<  Mutex for thread protection of data.
        Statistics mStats;                          ///<  Connection stats.
        SharedMutex mSendMutex;                     ///<  Optional mutex for sending operations.
        SharedMutex mRecvMutex;                     ///<  Optional mutex for receive operations.
        JAUS::Thread mUpdateConnectionThread;       ///<  Update thread object.
        unsigned int mConnectionUpdateDelayMs;      ///<  If in multi-threaded mode, this is the delay between updates.
        volatile Time::Stamp mUpdateTimeMs;         ///<  Last time UTC in ms that data was received.
        NodeManager* mpManager;                     ///<  Connection manager.
        unsigned int mConnectionNumber;             ///<  Connection number.
    private:
        static unsigned int ConnectionCounter;  ///<  For generated a unique connection ID.
        Callback::Set mCallbacks;               ///<  Pointer to the callback objects receiving data.
        SharedMutex mCallbacksMutex;            ///<  Thread protection of callback objects.
    };
}

#endif
/*  End of File */
