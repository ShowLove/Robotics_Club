////////////////////////////////////////////////////////////////////////////////////
///
///  \file udp.h
///  \brief UDP interface for transport services.
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
#include "jaus/core/transport/udp.h"
#include "jaus/core/transport/sharedmemory.h"
#include "jaus/core/transport/nodemanager.h"
#include "jaus/core/time/reporttime.h"
#include <cxutils/networking/udpclient.h>
#include <cxutils/networking/udpserver.h>
#include <tinyxml/tinyxml.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
UDP::Parameters::Parameters()
{
    this->mDestIP = std::string("127.0.0.1");
    this->mDestPortNumber = UDP::Port;
    mClientFlag = true;
    mMulticastIP = std::string("239.255.0.1");
    mTimeToLive = 16;
    mUseBroadcastingFlag = false;
    this->mTransportType = Connection::Transport::JUDP;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UDP::Parameters::~Parameters()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads any transport settings from the file.
///
///   \param[in] xmlSettingsFile Name of the file containing data to load.
///
////////////////////////////////////////////////////////////////////////////////////
bool UDP::Parameters::LoadSettings(const std::string& xmlSettingsFile)
{
    bool result = true;

    result |= Connection::Parameters::LoadSettings(xmlSettingsFile);
    if(result)
    {
        TiXmlDocument xml;

        if(xml.LoadFile(xmlSettingsFile.c_str()) == false)
        {
            return false;
        }

        TiXmlHandle doc(&xml);

        TiXmlNode* node;
        node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("UseBroadcasting").FirstChild().ToNode();
        if(node && node->Value())
        {
            mUseBroadcastingFlag = atoi(node->Value()) > 0 ? true : false;
        }
        node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("MulticastIP").FirstChild().ToNode();
        if(node && node->Value())
        {
            mMulticastIP.SetAddress(node->Value());
        }
        node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("TTL").FirstChild().ToNode();
        if(node && node->Value())
        {
            mTimeToLive = (unsigned char )atoi(node->Value());
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
///   \param[in] params Parameters to be equal to.
///
////////////////////////////////////////////////////////////////////////////////////
UDP::Parameters& UDP::Parameters::operator=(const UDP::Parameters& params)
{
    if(this != &params)
    {
        CopyBaseData(&params);
        mClientFlag = params.mClientFlag;
        mUseBroadcastingFlag = params.mUseBroadcastingFlag;
        mMulticastIP = params.mMulticastIP;
        mTimeToLive = params.mTimeToLive;
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
UDP::UDP(const bool singleThread) : Connection(singleThread)
{
    mpSocket = NULL;
    mTransportType = Connection::Transport::JUDP;
    mTransportHeader.Write(Version);
    mSendCache.Write(Version);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
UDP::~UDP()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the connection with given parameter data.
///
///   \param[in] parameters Parameter data for UDP connection.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool UDP::Initialize(const Connection::Parameters* parameters)
{
    bool result = false;

    WriteLock wLock(mConnectionMutex);

    mStats.Clear();

    if(parameters)
    {
        mParameters = *(UDP::Parameters*)parameters;
    }

    if(mpSocket && ((CxUtils::Socket*)mpSocket)->IsValid() == false)
    {
        DeleteSocket();
    }

    mStats.mTransportType = Connection::Transport::JUDP;

    if(mpSocket)
    {
        // If we already have a working socket.
        CxUtils::UdpSocket* sock = (CxUtils::UdpSocket *)this->mpSocket;
        mStats.mDestPortNumber = sock->GetDestinationPort();
        mStats.mSourcePortNumber = sock->GetSourcePort();
        mStats.mDestIP = sock->GetDestinationAddress();
        mStats.mSourceIP = sock->GetSourceAddress();
        mStats.mDestinationID = this->mID;

        this->mUpdateTimeMs = Time::GetUtcTimeMs();

        result = true;
    }
    else if(mParameters.mClientFlag)
    {
        // We should never get here because we always want to use
        // the same source port.
        CxUtils::UdpSocket* sock = (CxUtils::UdpSocket *)this->mpSocket;
        mStats.mDestPortNumber = sock->GetDestinationPort();
        mStats.mSourcePortNumber = sock->GetSourcePort();
        mStats.mDestIP = sock->GetDestinationAddress();
        mStats.mSourceIP = sock->GetSourceAddress();
        mStats.mDestinationID = this->mID;

        this->mUpdateTimeMs = Time::GetUtcTimeMs();

        result = true;
    }
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
        CxUtils::UdpSocket* udp = new CxUtils::UdpSocket();
        udp->SetNetworkInterface(hostInterface);
        if(mParameters.mMulticastIP != "0.0.0.0")
        {
            if(udp->InitializeMulticastSocket(mParameters.mMulticastIP,
                                              mParameters.mSourcePortNumber,
                                              mParameters.mTimeToLive,
                                              mParameters.mUseBroadcastingFlag,
                                              mParameters.mDestPortNumber,
                                              false))
            {
                mStats.mSourcePortNumber = mStats.mDestPortNumber = mParameters.mDestPortNumber;
                mStats.mSourceIP = udp->GetSourceAddress();
                mStats.mDestIP = mParameters.mMulticastIP;

                this->mUpdateTimeMs = Time::GetUtcTimeMs();

                result = true;
                mpSocket = udp;
            }
            else
            {
                result = false;
                delete udp;
            }
        }
        else
        {
            if(udp->InitializeSocket(mParameters.mDestIP,
                                     mParameters.mDestPortNumber,
                                     mParameters.mSourcePortNumber))
            {
                mStats.mSourcePortNumber = mStats.mDestPortNumber = mParameters.mDestPortNumber;
                mStats.mSourceIP = udp->GetSourceAddress();
                mStats.mDestIP = mParameters.mDestIP;

                this->mUpdateTimeMs = Time::GetUtcTimeMs();

                result = true;
                mpSocket = udp;
            }
            else
            {
                result = false;
                delete udp;
            }
        }
    }

    if(true == result &&
       false == this->mSingleThreadModeFlag &&
       false == mUpdateConnectionThread.IsThreadActive())
    {
        mUpdateConnectionThread.CreateThread(UpdateConnectionThread, this);
        if(false == mParameters.mClientFlag )
        {
            mUpdateConnectionThread.SetThreadName("JUDP");
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the connection.
///
////////////////////////////////////////////////////////////////////////////////////
void UDP::Shutdown()
{
    CloseSocket();
    mUpdateConnectionThread.StopThread();
    DeleteSocket();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if connected/valid otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool UDP::IsConnected() const
{
    return mpSocket == NULL ? false : ((CxUtils::Socket*)mpSocket)->IsValid();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current stats of connection.
///
////////////////////////////////////////////////////////////////////////////////////
Connection::Statistics UDP::GetStatistics()
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
bool UDP::CanBroadcast(const int transportType) const
{
    if(false == mParameters.mClientFlag &&
        (transportType == Connection::Transport::All || transportType == Connection::Transport::JUDP))
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
bool UDP::SendPacket(const Packet& packet,
                     const Header& packetHeader) const
{
    bool result = false;

    Packet* ptr;
    int size = 0;
    ptr = (Packet *)&mSendCache;
    {
        SharedMutex* m = (SharedMutex*)&mSendMutex;
        WriteLock wLock(*m);

        // Send JUDP Header and packet
        ptr->SetWritePos(sizeof(Version));
        ptr->Write(packet);

        CxUtils::Socket* socket = (CxUtils::Socket*)mpSocket;

        if(socket == NULL)
        {
            return result;
        }

        size = socket->Send(*ptr);
    }
    if(size > 0)
    {
        // Update stats.
        WriteLock wLock(*((SharedMutex *)&mConnectionMutex));
        Connection::Statistics* stats = (Connection::Statistics*)&mStats;
        stats->mMessagesSent++;
        stats->mTotalMessagesSent++;
        stats->mBytesSent += (unsigned int)size;
        stats->mTotalBytesSent += (unsigned int)size;

        result = true;
    }

    return result;
}


/** Updates the current state of the connection. */
void UDP::UpdateConnection()
{
    if(GetGlobalShutdownSignal())
    {
        return;
    }
    ReceiveIncommingData();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if connection data is different than the current connection to
///           the same source.
///
////////////////////////////////////////////////////////////////////////////////////
bool UDP::ConnectionChanged(const Connection::Info* destination) const
{
    CxUtils::UdpSocket* sock = (CxUtils::UdpSocket*)mpSocket;
    if(sock == NULL)
    {
        return true;
    }
    if(destination->mDestPortNumber != sock->GetDestinationPort() ||
       destination->mDestIP != sock->GetConnectionAddress())
    {
        return true;
    }
    return false;
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
Connection* UDP::CreateConnection(const Address& id,
                                  const Info* destination)
{
    CxUtils::UdpSocket* primary = dynamic_cast<CxUtils::UdpSocket*>( ( (CxUtils::Socket*)mpSocket) );
    // Only port that has reserved the JUDP port can create UDP connections.
    if(primary == NULL || primary->GetSourcePort() != this->mParameters.mDestPortNumber)
    {
        return NULL;
    }

    UDP* newUDP = new UDP(mSingleThreadModeFlag);
    newUDP->mID = id;
    newUDP->CopyConnectionData(this);
    newUDP->mParameters = mParameters;
    newUDP->mParameters = *destination;
    newUDP->mParameters.mClientFlag = true;

    newUDP->mpSocket = primary->CreateNewDestination(destination->mDestIP, destination->mDestPortNumber);

    if(newUDP->Initialize(&newUDP->mParameters))
    {
        return newUDP;
    }
    else
    {
        delete newUDP;
    }
    return NULL;
}


/** Returns reference to transport header data. */
const Packet& UDP::GetTransportHeader() const
{
    return mTransportHeader;
}


/** Closes the current socket safely. */
void UDP::CloseSocket()
{
    if(mpSocket)
    {
        CxUtils::UdpSocket* sock = dynamic_cast<CxUtils::UdpSocket*>( ((CxUtils::UdpSocket*)mpSocket) );
        if(sock)
        {
            sock->Shutdown();
        }

        mpSocket = NULL;
    }
}


/** Deletes the current socket safely. */
void UDP::DeleteSocket()
{
    if(mpSocket)
    {
        CxUtils::UdpSocket* sock = dynamic_cast<CxUtils::UdpSocket*>( ((CxUtils::UdpSocket*)mpSocket) );
        if(sock)
        {
            sock->Shutdown();
            delete sock;
        }

        mpSocket = NULL;
    }
}


/** Method to listen for new incomming data over the
    current socket connection. */
void UDP::ReceiveIncommingData()
{
    // Client only do not receive data.
    if(mParameters.mClientFlag)
    {
        return;
    }
    if(mRecvBuffer.Reserved() == 0)
    {
        mRecvBuffer.Reserve(JAUS_USHORT_MAX);
    }

    long int timeoutMs = 0;
    if(mSingleThreadModeFlag)
    {
        timeoutMs = 50;
    }
    else
    {
#ifdef WIN32
        timeoutMs = 0;
#else
        timeoutMs = 500;
#endif
    }
    CxUtils::UdpSocket* socket = (CxUtils::UdpSocket*)mpSocket;

    // Try receive some data.

    Info sourceInfo;
    sourceInfo.mTransportType = Connection::Transport::JUDP;
    sourceInfo.mSourcePortNumber = socket->GetSourcePort();
    mRecvBuffer.Clear();

    if( socket &&
        socket->Recv(mRecvBuffer,
                     JAUS_USHORT_MAX,
                     timeoutMs,
                     &sourceInfo.mDestIP,
                     &sourceInfo.mDestPortNumber) > 0)
    {
        unsigned char* ptr = mRecvBuffer.Ptr();

        // Check for JUDP Version header and make sure that this is not
        // a UDP message sent from this Node Manager (loop back of multicast/broadcast data).
        if((CxUtils::Socket::IsHostAddress(sourceInfo.mDestIP) == false || sourceInfo.mSourcePortNumber != sourceInfo.mDestPortNumber)
            &&
            mRecvBuffer.Length() > Header::MinSize + BYTE_SIZE && ptr[0] == Version)
        {
            // There may be multiple JAUS messages within each UDP packet
            unsigned int position = sizeof(Version);
            while(position < mRecvBuffer.Length())
            {
                // Try read message data.
                JAUS::Header jausHeader;
                Packet::Wrapper subPacket(&ptr[position], mRecvBuffer.Length() - position);
                std::string errorMessage;
                int bytesRead = 0;
                // Try read JAUS General Header data to check if there is a valid
                // JAUS packet here.
                if( (bytesRead = jausHeader.Read(*subPacket.GetData())) > 0 &&
                    jausHeader.IsValid(&errorMessage) &&
                    subPacket->Length() >= jausHeader.mSize)
                {
                    // Wrap the extracted packet
                    Packet::Wrapper jausPacket(&ptr[position],
                                                (unsigned int)jausHeader.mSize);

                    mID = jausHeader.mSourceID;

                    // Update stats.
                    {
                        WriteLock wLock(mConnectionMutex);
                        Connection::Statistics* stats = (Connection::Statistics*)&mStats;
                        stats->mMessagesReceived++;
                        stats->mTotalMessagesReceived++;
                        stats->mBytesReceived += jausPacket->Length();
                        stats->mTotalBytesReceived += jausPacket->Length();
                    }

                    // Check for internal discovery message.
                    if(jausHeader.mSourceID == jausHeader.mDestinationID)
                    {
                        ReportTime reportTime;
                        jausPacket.GetData()->SetReadPos(0);
                        if(reportTime.Read(*jausPacket.GetData()))
                        {
                            SharedMemory::Parameters smParams;
                            smParams.mComponentID = jausHeader.mSourceID;
                            smParams.mSharedConnectionType = SharedMemory::Parameters::Client;
                            smParams.mCreationTime = reportTime.GetTimeStamp();
                            smParams.mDestPortNumber = smParams.mSourcePortNumber = mParameters.mSourcePortNumber;
                            mpManager->CreateConnection(jausHeader.mSourceID, smParams, false);
                        }
                    }
                    else
                    {
                        // Process data
                        SendToCallbacks(*jausPacket.GetData(),
                                        jausHeader,
                                        &sourceInfo);
                    }
                    position += jausPacket->Length();
                }
                else
                {
                    break;
                }
            }
        }
    }
}


/*  End of File */
