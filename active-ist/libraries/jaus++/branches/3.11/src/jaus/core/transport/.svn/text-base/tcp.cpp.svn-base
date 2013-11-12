////////////////////////////////////////////////////////////////////////////////////
///
///  \file tcp.h
///  \brief TCP interface for transport services.
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
#include "jaus/core/transport/tcp.h"
#include "jaus/core/transport/nodemanager.h"
#include <cxutils/networking/tcplistensocket.h>
#include <cxutils/networking/tcpclient.h>
#include <cxutils/networking/tcpserver.h>
#include <tinyxml/tinyxml.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TCP::Parameters::Parameters()
{
    this->mDestIP = std::string("127.0.0.1");
    this->mDestPortNumber = TCP::Port;
    mClientFlag = true;
    this->mTransportType = Connection::Transport::JTCP;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TCP::Parameters::~Parameters()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads any transport settings from the file.
///
///   \param[in] xmlSettingsFile Name of the file containing data to load.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCP::Parameters::LoadSettings(const std::string& xmlSettingsFile)
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
TCP::Parameters& TCP::Parameters::operator=(const TCP::Parameters& params)
{
    if(this != &params)
    {
        CopyBaseData(&params);
        mClientFlag = params.mClientFlag;
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
TCP::TCP(const bool singleThread) : Connection(singleThread)
{
    mpSocket = NULL;
    mListenerFlag = false;
    mTransportType = Connection::Transport::JTCP;
    mpParent = NULL;
    mTransportHeader.Write(Version);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TCP::~TCP()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the connection with given parameter data.
///
///   \param[in] parameters Parameter data for TCP connection.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCP::Initialize(const Connection::Parameters* parameters)
{
    bool result = false;

    WriteLock wLock(mConnectionMutex);

    mStats.Clear();
    mStats.mTransportType = Connection::Transport::JTCP;

    if(parameters)
    {
        mParameters = *(TCP::Parameters*)parameters;
    }

    if(mpSocket && ((CxUtils::Socket*)mpSocket)->IsValid() == false)
    {
        DeleteSocket();
    }

    // Server/Incomming Socket
    if(mpSocket)
    {
        // If we already have a working socket.
        result = true;
        CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>( ((CxUtils::Socket*)mpSocket) );
        mStats.mDestIP = server->GetSourceAddress();
        mStats.mSourceIP = server->GetDestinationAddress();
        mStats.mDestPortNumber = mStats.mSourcePortNumber = server->GetPort();
        mStats.mDestinationID = mID;
        mStats.mSourceID = this->mSourceID;
    }
    else if(mParameters.mClientFlag)
    {
        CxUtils::TcpClient* tcp = new CxUtils::TcpClient();

        if(tcp->InitializeSocket(mParameters.mDestIP,
            mParameters.mDestPortNumber))
        {
            mpSocket = tcp;
            if(CxUtils::Socket::IsHostAddress(mParameters.mDestIP))
            {
                this->mLocalConnectionFlag = true;
            }

            mStats.mSourceIP = tcp->GetSourceAddress();
            mStats.mDestIP = tcp->GetDestinationAddress();
            mStats.mDestPortNumber = mStats.mSourcePortNumber = tcp->GetPort();
            mStats.mDestinationID = mID;

            this->mUpdateTimeMs = Time::GetUtcTimeMs();

            result = true;
        }
        else
        {
            delete tcp;
            tcp = NULL;
        }
    }
    // Listen socket
    else if(false == mParameters.mClientFlag )
    {
        // Try match to a defined IP address for a host
        // interface.
        IP4Address hostInterface;
        IP4Address::List hostnames;
        IP4Address::List::iterator eth0;
        CxUtils::Socket::GetHostAddresses(hostnames);

        for(eth0 = hostnames.begin();
            eth0 != hostnames.end();
            eth0++)
        {
            // Find matching network interface
            if(mParameters.mNetworkInterface.mString == "0.0.0.0" ||
               mParameters.mNetworkInterface.mString == "127.0.0.1" ||
               mParameters.mNetworkInterface.mString.empty() ||
               mParameters.mNetworkInterface == *eth0)
            {
                hostInterface = mParameters.mNetworkInterface;
                break;
            }
        }
        int nonblocking = 1;
#ifdef WIN32
        if(mSingleThreadModeFlag == false)
        {
            nonblocking = 0; // Making blocking for better performance
        }
#endif
        CxUtils::TcpListenSocket* listenSocket = new CxUtils::TcpListenSocket();
        listenSocket->SetNetworkInterface(hostInterface);
        if(listenSocket->InitializeSocket(mParameters.mSourcePortNumber, // JAUS Port typically.
                                          500,       // Pending listen queue size
                                          0,         // Infinite wait on sending
                                          0,         // Infinite ms max wait on receive
                                          nonblocking,  // Non-Blocking socket
                                          1))
        {
            mStats.mSourcePortNumber = mStats.mDestPortNumber = mParameters.mDestPortNumber;

            result = true;
            mListenerFlag = true;
            mpSocket = listenSocket;
            this->mUpdateTimeMs = Time::GetUtcTimeMs();
        }
        else
        {
            delete listenSocket;
            result = false;
        }
    }


    if(true == result &&
       false == this->mSingleThreadModeFlag &&
       false == mUpdateConnectionThread.IsThreadActive())
    {
        
        if(mListenerFlag)
        {
            mUpdateConnectionThread.SetThreadName("JTCP-Listener");
        }
        else
        {
            if(mID.IsValid())
            {
                mUpdateConnectionThread.SetThreadName("JTCP:" + mSourceID.ToString() + " -> " + mID.ToString());
            }
            else
            {
                if(mSourceID.IsValid())
                {
                    mUpdateConnectionThread.SetThreadName("JTCP:" + mSourceID.ToString() + " -> " + mID.ToString());
                }
                else
                {
                    mUpdateConnectionThread.SetThreadName("JTCP-Inbound");
                }
            }

        }
        result = mUpdateConnectionThread.CreateThread(UpdateConnectionThread, this) > 0 ? true : false;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the connection.
///
////////////////////////////////////////////////////////////////////////////////////
void TCP::Shutdown()
{
    CloseSocket();

    mUpdateConnectionThread.StopThread();

    DeleteSocket();

    WriteLock (mConnectionMutex);

    std::set<TCP*>::iterator undefined;
    for(undefined = mNewConnections.begin();
        undefined != mNewConnections.end();
        undefined++)
    {
        delete *undefined;
    }
    mNewConnections.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if connected/valid otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCP::IsConnected() const
{
    if(this->mListenerFlag)
    {
        return mpSocket == NULL ? false : ((CxUtils::TcpListenSocket*)mpSocket)->IsValid();
    }
    else
    {
        return mpSocket == NULL ? false : ((CxUtils::Socket*)mpSocket)->IsValid();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current stats of connection.
///
////////////////////////////////////////////////////////////////////////////////////
Connection::Statistics TCP::GetStatistics()
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
///   \return True if it can broadcast on the given transport mechanism.
///
////////////////////////////////////////////////////////////////////////////////////
bool TCP::CanBroadcast(const int transportType) const
{
    if(transportType == Connection::Transport::JTCP)
    {
        return true;
    }
    return false;
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
bool TCP::SendPacket(const Packet& packet,
                     const Header& packetHeader) const
{
    bool result = false;

    // Send TCP Header, then packet
    CxUtils::Socket* socket = (CxUtils::Socket*)mpSocket;

    Packet temp;

    if(socket == NULL)
    {
        return result;
    }

    int size = 0;
    {
        SharedMutex* m = (SharedMutex*)&mSendMutex;
        WriteLock wLock(*m);
        size += socket->Send(mTransportHeader);
        size += socket->Send(packet);
    }
    if(size >= (int)(packet.Length()))
    {
        // Update stats.
        WriteLock wLock(*((SharedMutex *)&mConnectionMutex));
        Connection::Statistics* stats = (Connection::Statistics*)&mStats;
        stats->mMessagesSent++;
        stats->mTotalMessagesSent++;
        stats->mBytesSent += size;
        stats->mTotalBytesSent += size;
        result = true;
    }
    return result;
}


/** Updates the current state of the connection. */
void TCP::UpdateConnection()
{
    if(GetGlobalShutdownSignal())
    {
        return;
    }
    ListenForConnections();
    ReceiveIncommingData();
    if(mSingleThreadModeFlag)
    {
        std::set<TCP*> newConnections = mNewConnections;
        std::set<TCP*>::iterator newCon;
        for(newCon = newConnections.begin();
            newCon != newConnections.end() && GetGlobalShutdownSignal() == false;
            newCon++)
        {
            (*newCon)->UpdateConnection();
        }
    }
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
Connection* TCP::CreateConnection(const Address& id,
                                  const Info* destination)
{
    TCP* newTCP = new TCP(mSingleThreadModeFlag);

    newTCP->CopyConnectionData(this);
    newTCP->mID = id;

    TCP::Parameters parameters = mParameters;
    parameters = *destination;
    parameters.mClientFlag = true;

    if(newTCP->Initialize(&parameters))
    {
        return newTCP;
    }
    else
    {
        delete newTCP;
    }

    return NULL;
}


/** Returns reference to transport header data. */
const Packet& TCP::GetTransportHeader() const
{
    return mTransportHeader;
}


/** Is this a client connection we created ? */
bool TCP::IsClient() const
{
    if(mpSocket)
    {
        CxUtils::TcpClient* client = dynamic_cast<CxUtils::TcpClient*>( ((CxUtils::Socket*)mpSocket) );
        return client > 0 ? true : false;
    }
    return false;
}

/** Closes the current socket safely. */
void TCP::CloseSocket()
{
    if(mpSocket)
    {
        if(this->mListenerFlag)
        {
            CxUtils::TcpListenSocket* listen = dynamic_cast<CxUtils::TcpListenSocket*>( ((CxUtils::TcpListenSocket*)mpSocket) );
            if(listen)
            {
                listen->Shutdown();
            }
        }
        else
        {
            CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>( ((CxUtils::Socket*)mpSocket) );
            CxUtils::TcpClient* client = dynamic_cast<CxUtils::TcpClient*>( ((CxUtils::Socket*)mpSocket) );

            if(server)
            {
                server->Shutdown();
            }
            if(client)
            {
                client->Shutdown();
            }
        }

        mpSocket = NULL;
    }
}


/** Deletes the current socket safely. */
void TCP::DeleteSocket()
{
    if(mpSocket)
    {
        if(this->mListenerFlag)
        {
            CxUtils::TcpListenSocket* listen = dynamic_cast<CxUtils::TcpListenSocket*>( ((CxUtils::TcpListenSocket*)mpSocket) );
            if(listen)
            {
                listen->Shutdown();
                delete listen;
            }
        }
        else
        {
            CxUtils::TcpServer* server = dynamic_cast<CxUtils::TcpServer*>( ((CxUtils::Socket*)mpSocket) );
            CxUtils::TcpClient* client = dynamic_cast<CxUtils::TcpClient*>( ((CxUtils::Socket*)mpSocket) );

            if(server)
            {
                server->Shutdown();
                delete server;
            }
            if(client)
            {
                client->Shutdown();
                delete client;
            }
        }

        mpSocket = NULL;
    }
}


/** Removes a TCP connection from the list of connections that
    do not know what JAUS component they are tied to. */
void TCP::RemoveConnection(TCP* tcp)
{
    {
        WriteLock wLock(mConnectionMutex);
        if(mNewConnections.find(tcp) != mNewConnections.end())
        {
            tcp->mStats.mDestinationID.Clear();
            tcp->mStats.mSourceID = tcp->mSourceID;
            mNewConnections.erase(mNewConnections.find(tcp));
        }
    }
    tcp->mpParent = NULL;
    
    if(tcp->IsLocalConnection())
    {
        if(tcp->mSourceID == tcp->mID)
        {
            tcp->mUpdateConnectionThread.SetThreadName("JTCP:" + tcp->mSourceID.ToString() + " <- " + mID.ToString());
        }
        else
        {
            tcp->mUpdateConnectionThread.SetThreadName("JTCP:" + tcp->mSourceID.ToString() + " -> " + tcp->mID.ToString());
        }
    }
    mpManager->AddConnection(tcp);
}



/** If in server mode, this method listens for
    and creates incomming TCP connections. */
void TCP::ListenForConnections()
{
    static unsigned int totalCount = 0;

    CxUtils::TcpListenSocket* listenSocket = (CxUtils::TcpListenSocket*)mpSocket;
    if(mListenerFlag && GetGlobalShutdownSignal() == false)
    {
        CxUtils::TcpServer* sock = NULL;

        // JAUS++ uses non-blocking receive calls to allow for
        // single thread operation.
        sock = listenSocket->AwaitConnection();

        if(sock && false == GetGlobalShutdownSignal() && mpManager)
        {
            totalCount++;
            // Create a new connection JTCP connection.
            TCP* newTCP = new TCP(this->mSingleThreadModeFlag);

            // Push onto stack of new connections.  For these
            // connections we do not yet know what JAUS ID is
            // associated with the connection.
            {
                WriteLock wLock(mConnectionMutex);
                mNewConnections.insert(newTCP);
            }

            newTCP->CopyConnectionData(this);
            newTCP->mpSocket = sock;
            newTCP->mListenerFlag = false;
            newTCP->mUpdateTimeMs = Time::GetUtcTimeMs();
            newTCP->mpParent = this;

            if(CxUtils::Socket::IsHostAddress(sock->GetClientAddress()))
            {
                newTCP->mLocalConnectionFlag = true;
            }
            newTCP->Initialize(&mParameters);
        }
        CxUtils::SleepMs(1);
    }
}


/** Method to listen for new incomming data over the
    current socket connection. */
void TCP::ReceiveIncommingData()
{
    if(mListenerFlag)
    {
        return;
    }

    if(mLargeBuffer.Reserved() == 0 || mSmallBuffer.Reserved() == 0)
    {
        mLargeBuffer.Reserve(10*(JAUS::Header::MaxPacketSize + OverheadSizeBytes));
        mSmallBuffer.Reserve(JAUS::Header::MaxPacketSize + OverheadSizeBytes);
    }

    long int timeoutMs = 0;
    if(mSingleThreadModeFlag)
    {
        timeoutMs = 1;
    }
    else
    {
#ifdef WIN32
        timeoutMs = 0;
#else
        timeoutMs = 500;
#endif
    }
    CxUtils::Socket* socket = (CxUtils::Socket*)mpSocket;

    // Try receive some data.
    mSmallBuffer.Clear();

    Info sourceInfo;
    sourceInfo.mTransportType = Connection::Transport::JTCP;
    sourceInfo.mSourcePortNumber = this->mStats.mSourcePortNumber;
    sourceInfo.mDestPortNumber = this->mStats.mDestPortNumber;

    int rt = 0;
    mSmallBuffer.Clear();

    if( socket &&
        (rt = socket->Recv(mSmallBuffer,
                           5000,
                           timeoutMs,
                           &sourceInfo.mDestIP,
                           &sourceInfo.mDestPortNumber)) > 0)
    {
        mLargeBuffer += mSmallBuffer;
    }

    if(rt < 0)
    {
        if(socket)
        {
            socket->Shutdown();
        }
        mLargeBuffer.Clear();
        return;
    }

    // Try parse out a JAUS message.
    unsigned char* ptr = mLargeBuffer.Ptr();
    unsigned int pos = 0;
    unsigned int endPos = 0;
    while(mLargeBuffer.Length() >= JAUS::Header::MinSize && pos < mLargeBuffer.Length())
    {
        // Check for JTCP Transport Header
        if(ptr[pos] == Version)
        {
            unsigned int offset = pos + sizeof(Version);
            while(offset < mLargeBuffer.Length())
            {
                // Try read message data.  there may be more than 1 JAUS message per transport header
                JAUS::Header jausHeader;
                Packet::Wrapper subPacket(&ptr[offset], mLargeBuffer.Length() - offset);
                std::string errorMessage;
                int bytesRead = 0;
                // Try read JAUS General Header data to check if there is a valid
                // JAUS packet here.
                if( (bytesRead = jausHeader.Read(*subPacket.GetData())) > 0 &&
                    jausHeader.IsValid(&errorMessage) &&
                    subPacket->Length() >= jausHeader.mSize)
                {
                    // Wrap the JAUS packet with general header.
                    Packet::Wrapper jausPacket(&ptr[offset],
                                                (unsigned int)jausHeader.mSize);

                    // Assign the current source of messages on this
                    // connection.
                    mID = jausHeader.mDestinationID;
                    mSourceID = jausHeader.mSourceID;

                    // If we are still tied to a parent listener,
                    // remove ourselves because the Node Manager
                    // should handle things.
                    if(mpParent && mpManager && mID.IsValid() && mID.IsBroadcast() == false)
                    {
                        mpParent->RemoveConnection(this);
                    }

                    // Update stats.
                    {
                        WriteLock wLock(mConnectionMutex);
                        Connection::Statistics* stats = (Connection::Statistics*)&mStats;
                        stats->mMessagesReceived++;
                        stats->mTotalMessagesReceived++;
                        stats->mBytesReceived += jausPacket->Length();
                        stats->mTotalBytesReceived += jausPacket->Length();
                    }

                    // Process data
                    SendToCallbacks(*jausPacket.GetData(),
                                    jausHeader,
                                    &sourceInfo);

                    // Advance position in stream buffer.
                    offset += jausPacket->Length();
                    pos = endPos = offset;
                }
                // Failed to reader valid JAUS General Header
                else
                {
                    if(errorMessage.empty() == false)
                    {
                        //std::cout << "JTCP::Error [Header] - " << errorMessage << std::endl;
                    }
                    // Exit loop
                    break;
                }
            }
        }
        else
        {
            pos++;
        }
    }

    if(endPos > 0)
    {
        // Delete data we have read from the buffer.
        mLargeBuffer.Delete(endPos, 0);
    }

    if(mLargeBuffer.Size() >= mLargeBuffer.Reserved() - JAUS::Header::MinSize + 1)
    {
        // Getting bad data, flush out buffers.
        // Hopefully we never get to this point in code.
        std::cout << "JTCP::ERROR - Buffer Overflow.\n";
        mLargeBuffer.Clear();
        mSmallBuffer.Clear();
    }
}


/*  End of File */
