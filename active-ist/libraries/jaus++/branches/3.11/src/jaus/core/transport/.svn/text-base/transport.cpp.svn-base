////////////////////////////////////////////////////////////////////////////////////
///
///  \file transport.cpp
///  \brief This file contains the implementation for creating Transport Services
///  in JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 11 October 2009
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
#include "jaus/core/transport/transport.h"
#include "jaus/core/transport/tcp.h"
#include "jaus/core/events/event.h"
#include "jaus/core/discovery/queryidentification.h"
#include "jaus/core/time/reporttime.h"
#include "jaus/core/transport/udp.h"
#include "jaus/core/transport/sharedmemory.h"
#include "jaus/core/transport/nodemanager.h"

#include <queue>
#include <map>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <fstream>

#include <boost/scoped_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

#include <cxutils/fileio.h>
#include <cxutils/networking/udpclient.h>
#include <cxutils/circulararray.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Receipt
    ///   \brief Data structure used to store information about messages waiting
    ///          for incomming responses.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Receipt
    {
    public:
        typedef std::set<Receipt*> Set; ///< List of receipt data.
        Receipt();
        ~Receipt();
        volatile bool mPendingFlag;     ///<  True if pending.
        const Message* mpMessage;       ///<  Message sent.
        Message* mpResponse;            ///<  Message response data (if only 1 possible response).
        Message::List* mpResponses;     ///<  Message response data.
        boost::mutex mConditionMutex;               ///<  Receipt mutex.
        boost::condition_variable mWaitCondition;   ///<  Used to notify when data is ready.
    };
}

//#define USE_MESSAGE_QUEUE

using namespace JAUS;

static const unsigned int PACKET_QUEUE_SIZE = 1024;

const std::string Transport::Name = "urn:jaus:jss:core:Transport";


/** Stores all data structures required by the Transport Layer. */
class Data
{
public:
    Data(const bool singleThreadModeFlag) : mpSharedMemory(new SharedMemory(singleThreadModeFlag))
    {
        mProcessingThreadsLimit = 1;
        mStopMessageProcessingFlag = false;
        mSequenceNumber = 0;
        mSinglePacketQueue.reserve(PACKET_QUEUE_SIZE);
        mMultiPacketQueue.reserve(PACKET_QUEUE_SIZE*2);
        mSinglePacketCache.Reserve(Header::MaxPacketSize);
        mMultiPacketCache.Reserve(Header::MaxPacketSize);
        mLastNodeManagerCheckTimeMs = 0;
    }
    ~Data() {}

    boost::scoped_ptr<SharedMemory> mpSharedMemory;         ///<  Shared memory for communcation.

    NodeManager mNodeManager;                               ///<  Manages connection on the computing node.

    typedef CxUtils::CircularArray<Packet> PacketQueue;     ///<  Type definition for a packet queue.
#ifdef USE_MESSAGE_QUEUE
    std::queue<Message*> mMessageQueue;                     ///<  Message queue.
    Mutex mMessageQueueMutex;                               ///<  Mutex for thread protection of queue.
#endif
    SharedMutex mSinglePacketQueueMutex;                    ///<  Mutex for thread protection of single packet queue.
    PacketQueue mSinglePacketQueue;                         ///<  Queue of message packets that are not part of a LDS.
    SharedMutex mMultiPacketQueueMutex;                     ///<  Mutex for thread protection of multi-packet queue.
    PacketQueue mMultiPacketQueue;                          ///<  Queue for multi-packet stream packets (LDS).
    Packet  mSinglePacketCache;                             ///<  Cached copy of a single packet for de-serialization.
    Packet  mMultiPacketCache;                              ///<  Cached copy of a multi-packet for de-serialization.
    unsigned int mProcessingThreadsLimit;                   ///<  How many threads to use for message processing, default is 1.
    std::vector<Thread*> mProcessingThreads;                ///<  Additional processing threads.

    volatile bool mStopMessageProcessingFlag;               ///<  If true, all message processing should stop.
    Time::Stamp mLastNodeManagerCheckTimeMs;                ///<  Last time we check to see if node manager was still running.

    Mutex mJausMessageQueueMutex;                           ///<  Mutex for thread protection of message queue.
    CxUtils::CircularArray<Packet> mJausMessageQueue;       ///<  Array of messages received by the Transport Service.

    SharedMutex mMessageTemplatesMutex;                     ///<  Mutex for thread protection.
    std::map<UShort, Message*> mMessageTemplates;           ///<  Custom message templates.
    SharedMutex mMessageCacheMutex;                         ///<  Mutex for thread protection of message cache.
    std::map<UShort, Message*> mMessageCache;               ///<  Pre-allocated memory for message decoding.

    Packet mLargeDataSetCache;                              ///<  Cache packet memory for merged large data sets.
    LargeDataSet::Map mLargeDataSets;                       ///<  Large data sets.

    SharedMutex mPendingReceiptsMutex;                      ///<  Mutex for thread protection of receipts.
    Receipt::Set mPendingReceipts;                          ///<  List of blocking send calls waiting for responses.

    Transport::Callback::Map mMessageCallbacks;             ///<  Map of message callbacks.
    SharedMutex mSequenceNumberMutex;                       ///<  Mutex for thread protection of sequence number.
    UShort mSequenceNumber;                                 ///<  Message sequence number.
};


/** This is a definition for typecasting a void pointer to data used by the Transport Service. */
#define MEMBER ((Data *)mpData)


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Receipt::Receipt()
{
    mPendingFlag = true;
    mpMessage = NULL;
    mpResponse = NULL;
    mpResponses = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Receipt::~Receipt()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::Transport() : Service(Service::ID(Transport::Name, 1.0), Service::ID())
                         
{
    mpData = (void*)new Data(mSingleThreadModeFlag);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::~Transport()
{
    Shutdown();

    // Delete memory
    std::map<UShort, Message*>::iterator msg;
    {
        WriteLock wLock(MEMBER->mMessageTemplatesMutex);
        for(msg = MEMBER->mMessageTemplates.begin();
            msg != MEMBER->mMessageTemplates.end();
            msg++)
        {
            delete msg->second;
        }
        MEMBER->mMessageTemplates.clear();
    }
    {
        WriteLock wLock(MEMBER->mMessageTemplatesMutex);
        for(msg = MEMBER->mMessageCache.begin();
            msg != MEMBER->mMessageCache.end();
            msg++)
        {
            delete msg->second;
        }
        MEMBER->mMessageCache.clear();
    }

    delete (Data *)mpData;
}


/** Transport service is not discoverable. */
bool Transport::IsDiscoverable() const { return false; }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the Transport Service based on configuration values
///          set.
///
///   \param[in] componentID ID of the component using the Transport Service.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::Initialize(const Address& componentID)
{
    bool result = false;

    MEMBER->mStopMessageProcessingFlag = false;

    // Try to initialize a node manager, there must be one
    // running on the system for any communication to take place.
    MEMBER->mNodeManager.Initialize();

    if(componentID.IsValid() == false || componentID.IsBroadcast())
    {
        WriteLock printLock(mDebugMessagesMutex);
        std::cout << "Transport::Error- Invalid component ID [" << componentID.ToString() << "]\n";
        return false;
    }

    SharedMemory::Parameters smParams;

    // Wonderful, now we must try and initialize our own shared memory, 
    // but first, let's check to see if this component ID is in use.
    smParams.mComponentID = componentID;
    smParams.mDestPortNumber = smParams.mSourcePortNumber = MEMBER->mNodeManager.GetSettings()->GetDefaultPortNumber();
    smParams.mSharedConnectionType = SharedMemory::Parameters::Host;

    // Subscribe to messages received by inbox.
    MEMBER->mpSharedMemory->RegisterCallback(this);

    if(MEMBER->mpSharedMemory->Initialize(&smParams))
    {
        result = true;
        SetComponentID(componentID);

        if(false == mSingleThreadModeFlag && this->mServiceUpdateThread.IsThreadActive() == false)
        {
            for(unsigned int i = 0; i < MEMBER->mProcessingThreadsLimit - 1; i++)
            {
                Thread* thread = new Thread();
                MEMBER->mProcessingThreads.push_back(thread);
            }
            for(unsigned int i = 0; i < (int)MEMBER->mProcessingThreads.size(); i++)
            {
                MEMBER->mProcessingThreads[i]->CreateThread(Service::ServiceUpdateThreadFunc, this);
            }
            this->StartServiceUpdateEventThread();
            boost::this_thread::sleep(boost::posix_time::milliseconds(100)); 
        }

        
        // Send a few messages to server so that the Node Manager will find us.
        NotifyNodeManager();
    }

    if(false == result)
    {
        MEMBER->mpSharedMemory->Shutdown();
    }

    return result;
}


/** Returns true if service is initialized and active, otherwise false. */
bool Transport::IsInitialized() const
{
    if(MEMBER->mpSharedMemory->IsConnected())
    {
        return true;
    }
    return false;
}


/** Shuts down the transport service. */
void Transport::Shutdown()
{
    MEMBER->mStopMessageProcessingFlag = true;
    StopServiceUpdateEventThread();
    for(unsigned int i = 0; i < (unsigned int)MEMBER->mProcessingThreads.size(); i++)
    {
        MEMBER->mProcessingThreads[i]->StopThread();
        delete MEMBER->mProcessingThreads[i];
    }
    MEMBER->mProcessingThreads.clear();

    MEMBER->mpSharedMemory->ClearCallbacks();

    MEMBER->mpSharedMemory->Shutdown();

    // Shutdown the Node Manager if running.
    MEMBER->mNodeManager.Shutdown();

    MEMBER->mStopMessageProcessingFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads settings for transport from XML file.
///
///   \param[in] filename Name of the file to load from.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::LoadSettings(const std::string& filename)
{
    return MEMBER->mNodeManager.LoadSettings(filename); 
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Tries to create/add a new fixed/permanet connection to a component
///          ID.
///
///   If component/transport is initialized, and method returns false, then
///   the connection attemp failed.  If not initialized, method will always
///   return false, however when initialization takes place, connections will
///   be created then.
///
///   \param[in] id ID of JAUS component.
///   \param[in] info Connection type information.
///
///   \return True on success, false on failure.  See detailed function notes.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::AddConnection(const Address& id,
                              const Connection::Info& info)
{
    return MEMBER->mNodeManager.CreateConnection(id, info);
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when data is received by the service.  This method
///   shares it will all Services on on the component.
///
///   \param[in] message Message to process.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::Receive(const Message* message)
{
    PushMessageToChildren(message);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a serialized JAUS packet that has the correct JTCP header
///          setup.  If the destination is a local broadcast, then it is sent
///          using multicast, if destination is global, then it is sent using
///          a broadcast socket.
///
///   \param[in] packet JTCP Packet to send.
///   \param[in] header JAUS Transport header information.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::SendPacket(const Packet& packet,
                           const Header& header) const
{
    bool result = false;
    
    if(MEMBER->mpSharedMemory->SendPacket(packet, header))
    {
        result = true;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the message data into packets for transmission over
///          the protocol in use.
///
///   \param[in] message Message to convert/serialize.
///   \param[out] stream Packets to send.
///   \param[out] streamHeaders Header info for packets.
///   \param[in] startingSequenceNumber Sequence number to use for packets.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g.
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::SerializeMessage(const Message* message,
                                 Packet::List& stream,
                                 Header::List& streamHeaders,
                                 const UShort startingSequenceNumber,
                                 const int broadcastFlags) const
{
    Packet packet;
    Header header;

    // Pre-allocate memory. (use TCP overhead because it is the largest).
    packet.Reserve(JAUS::Header::MaxPacketSize + TCP::OverheadSizeBytes + USHORT_SIZE);

    // Clear stream/headers.
    stream.clear();
    streamHeaders.clear();
    unsigned int bestPacketSize = 1500 - /*MEMBER->mpSharedMemory->GetMaximumPacketSizeInBytes()*/ - TCP::OverheadSizeBytes - USHORT_SIZE;
    // If the message is a large data set, create a multi-packet stream.
    if(message->IsLargeDataSet(bestPacketSize))
    {
        return message->WriteLargeDataSet(stream,
                                          streamHeaders,
                                          bestPacketSize,
                                          &(MEMBER->mpSharedMemory->GetTransportHeader()),
                                          startingSequenceNumber) > 0;
    }
    // Single packet.
    else if(message->Write(packet, header, &(MEMBER->mpSharedMemory->GetTransportHeader()), true, startingSequenceNumber, (Byte)broadcastFlags) > 0)
    {
        stream.push_back(packet);
        streamHeaders.push_back(header);
        return true;
    }
    return false;
}


/** Updates all processes within the Transport service. */
void Transport::UpdateServiceEvent()
{
    // Check to see if we need to initialize a new Node Manager
    static JAUS::Time::Stamp lastTimeRecvActive = Time::GetUtcTimeMs();
    
    if(Time::GetUtcTimeMs() - lastTimeRecvActive >= 1000)
    {
        bool recvActive = MEMBER->mpSharedMemory->HasSubscriber(2000);

        // Check to see if we have lost our connection to a Transport Manager
        if(recvActive == false)
        {
            // Try create a node manager in case one
            // is not running.  Will fail immediately if the
            // main port is still in use.
            MEMBER->mNodeManager.Initialize();

            NotifyNodeManager();
        }
        lastTimeRecvActive = Time::GetUtcTimeMs();
    }
    
    // Help Node Manager Do any Cleanup Needed (single thread mode only)
    if(this->mSingleThreadModeFlag)
    {
        MEMBER->mNodeManager.UpdateServiceEvent();
    }
#ifndef USE_MESSAGE_QUEUE
    ProcessSinglePackets();
    ProcessMultiPackets();
#else
    Message* messageToShare = NULL;
    MEMBER->mMessageQueueMutex.Lock();
    if(MEMBER->mMessageQueue.size() > 0)
    {
        messageToShare = MEMBER->mMessageQueue.front();
        MEMBER->mMessageQueue.pop();
    }
    MEMBER->mMessageQueueMutex.Unlock();

    if(messageToShare)
    {
        PushMessageToChildren(messageToShare);
        delete messageToShare;
    }
#endif

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Given a message code, the Service attempts to create a Message object
///          that can be used for de-serialization of JAUS packets.
///
///   \param[in] messageCode JAUS Message to try create.
///
///   \return Pointer to created message, NULL if message not supported by
///           any Services attached to the Transport.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Transport::CreateMessage(const UShort messageCode) const
{
    // Try to create the message recursively.
    Message* message = CreateMessageFromService(messageCode, this);
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message.
///
///   \param[in] message JAUS Message to send.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g.
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::Send(const Message* message,
                     const int broadcastFlags) const
{
    if(MEMBER->mpSharedMemory->IsConnected() == false)
    {
        //throw Exception("JAUS::Transport::Send::ERROR: Not initialized");
        std::cout << "JAUS::Transport::Send::ERROR: Not initialized!\n";
        return false;
    }
    Packet sendPacket;
    Packet::List stream;
    Header::List streamHeaders;
    Packet::List::iterator packet;
    Header::List::iterator header;
    Header jausHeader;

    if(message->GetDestinationID() == mComponentID)
    {
        return false;
    }
    // Automatically set the source ID.
    ( (Message * ) message )->SetSourceID(mComponentID);
    unsigned int bestPacketSize = 1500 /*MEMBER->mpSharedMemory->GetMaximumPacketSizeInBytes()*/ - TCP::OverheadSizeBytes - USHORT_SIZE;

    SharedMutex* seqMutex = (SharedMutex*)&MEMBER->mSequenceNumberMutex;
    UShort sequenceNumber = 0;
    
    if(message->IsLargeDataSet(bestPacketSize))
    {
        if(SerializeMessage(message, stream, streamHeaders, 0, (Byte)broadcastFlags) == false)
        {
            return false;
        }
        // Get sequence number
        {
            WriteLock wLock(*seqMutex);
            sequenceNumber = MEMBER->mSequenceNumber;
            (*((UShort *)(&MEMBER->mSequenceNumber))) += (UShort)stream.size();
        }
        for(packet = stream.begin(), header = streamHeaders.begin();
            packet != stream.end() && header != streamHeaders.end();
            packet++, header++)
        {
            header->mSequenceNumber = sequenceNumber++;
            // Sequence number goes at the end of the packet (this is retarded).
            packet->Write(sequenceNumber, packet->Length() - USHORT_SIZE);
            if(SendPacket(*packet, *header) == false)
            {
                return false;
            }
        }
        return true;
    }
    // Small message (single packet).
    {
        // Get sequence number
        WriteLock wLock(*seqMutex);
        sequenceNumber = MEMBER->mSequenceNumber;
        (*((UShort *)(&MEMBER->mSequenceNumber)))++;
    }
    if(message->Write(*((Packet *)&sendPacket), jausHeader, &(MEMBER->mpSharedMemory->GetTransportHeader()), true, sequenceNumber, (Byte)broadcastFlags))
    {
        bool result =  SendPacket(sendPacket, jausHeader);

        return result;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message to multiple destinations, reducing the 
///   number of times the data must be re-serialized.
///
///   This method is good for when you needs to send a large chunk of the same
///   data to multiple places (like range data or video).
///
///   \param[in] destinations All destinations the message must go to.
///   \param[in] message JAUS Message to send.
///   \param[in] broadcastFlags Values to use to signify if message should be
///                             sent using any broadcast options (e.g.
///                             multicast). 0 = no options, 1 = local broadcast,
///                             2 = global broadcast.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::SendToList(const Address::Set& destinations,
                           const Message* message,
                           const int broadcastFlags) const
{
    if(MEMBER->mpSharedMemory->IsConnected() == false)
    {
        std::cout << "JAUS::Transport::Send::ERROR: Not initialized!\n";
        return false;
    }

    Packet sendPacket;
    Packet::List stream;
    Header::List streamHeaders;
    Packet::List::iterator packet;
    Header::List::iterator header;
    Header jausHeader;

    if(message->GetDestinationID() == mComponentID)
    {
        return false;
    }
    // Automatically set the source ID.
    ( (Message * ) message )->SetSourceID(mComponentID);
    unsigned int bestPacketSize = 1500 /*MEMBER->mpSharedMemory->GetMaximumPacketSizeInBytes()*/ - TCP::OverheadSizeBytes - USHORT_SIZE;

    Address::Set::const_iterator dest;
    SharedMutex* seqMutex = (SharedMutex*)&MEMBER->mSequenceNumberMutex;
    unsigned int transportHeaderSize = MEMBER->mpSharedMemory->GetTransportHeader().Length();

    if(message->IsLargeDataSet(bestPacketSize))
    {
        if(SerializeMessage(message, stream, streamHeaders, 0, (Byte)broadcastFlags) == false)
        {
            return false;
        }
        for(dest = destinations.begin();
            dest != destinations.end();
            dest++)
        {
            // Get sequence numbers for this message sequence.
            UShort sequenceNumber = 0;
            {
                WriteLock wsLock(*seqMutex);
                sequenceNumber = MEMBER->mSequenceNumber;
                (*((UShort *)(&MEMBER->mSequenceNumber))) += (UShort)stream.size();;
            }
            for(packet = stream.begin(), header = streamHeaders.begin();
                packet != stream.end() && header != streamHeaders.end();
                packet++, header++)
            {
                header->mDestinationID = *dest;
                header->mSequenceNumber = sequenceNumber++;
                packet->SetWritePos(transportHeaderSize);
                // Update header
                header->Write(*packet);
                // Send the data.
                if(SendPacket(*packet, *header) == false)
                {
                    return false;
                }
            }
        }
        return true;
    }
    else
    {
        bool result = false;
        
        message->Write(sendPacket, jausHeader, &(MEMBER->mpSharedMemory->GetTransportHeader()), true, 0, (Byte)broadcastFlags);
        for(dest = destinations.begin();
            dest != destinations.end() && sendPacket.Length() > 0;
            dest++)
        {
            // Update JAUS Header data in packet.
            jausHeader.mDestinationID = *dest;
            // Now get a new sequence number
            {
                WriteLock wsLock(*seqMutex);
                jausHeader.mSequenceNumber = MEMBER->mSequenceNumber;
                (*((UShort *)(&MEMBER->mSequenceNumber)))++;
            }
            // Replace sequence number in packet.
            sendPacket.SetWritePos(transportHeaderSize);
            jausHeader.Write(sendPacket);
            // Send the packet
            result = SendPacket(sendPacket, jausHeader);
        }
        return result;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message, and blocks until a response is received.
///
///   \param[in] message JAUS Message to send.
///   \param[out] response Pointer to message that is the response to the
///                        message sent which will read received data.
///   \param[in] waitTimeMs How long to wait in ms for a response to be
///                         received. A value of 0 is INFINITE.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::Send(const Message* message,
                     Message* response,
                     const unsigned int waitTimeMs) const
{
    Message::List possibleResponses;
    possibleResponses.push_back(response);
    return Send(message, possibleResponses, waitTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a JAUS message, and blocks until a response is received.  You
///          must pass pointers to the response message object by adding them
///          to the list passed.
///
///   On a succesful return, the possibleResponses list is modified to
///   contain only the message containing the response.  Make sure you manage
///   the memory of the messages put in this list to prevent memory leaks.
///
///   \param[in] message JAUS Message to send.
///   \param[out] possibleResponses Lists of messages that are possible
///                                 responses to the message sent. For example
///                                 Create Event can be responded with
///                                 Confirm or Reject Event Request.  Method
///                                 does not delete or modify pointers.
///   \param[in] waitTimeMs How long to wait in ms for a response to be
///                         received, 0 is INFINITE.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::Send(const Message* message,
                     Message::List& possibleResponses,
                     const unsigned int waitTimeMs) const
{
    Receipt receipt;
    Receipt::Set::iterator ritr;

    if(message == NULL || possibleResponses.size() == 0 || message->GetDestinationID().IsBroadcast())
    {
        return false;
    }

    receipt.mPendingFlag = true;
    receipt.mpMessage = message;
    if(possibleResponses.size() == 1)
    {
        receipt.mpResponse = possibleResponses.front();
    }
    else
    {
        receipt.mpResponses = &possibleResponses;
    }

    Receipt::Set* pending = NULL;
    {
        WriteLock wLock(*((SharedMutex *)&MEMBER->mPendingReceiptsMutex));
        pending = ((Receipt::Set *)&MEMBER->mPendingReceipts);
        pending->insert(&receipt);
    }

    {
        if(Send(message))
        {

            boost::unique_lock<boost::mutex> wLock(receipt.mConditionMutex);
            
            if(waitTimeMs == 0)
            {
                receipt.mWaitCondition.wait(wLock);
            }
            else
            {
                receipt.mWaitCondition.timed_wait(wLock, boost::posix_time::milliseconds(waitTimeMs));
            }
        }
    }
    
    {
        WriteLock wLock(*((SharedMutex *)&MEMBER->mPendingReceiptsMutex));
        ritr = pending->find(&receipt);
        if(ritr != pending->end())
        {
            pending->erase(ritr);
        }
    }

    return !receipt.mPendingFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Allows external programs to get notified when specific types of
///          messages are received by the Transport Service.
///
///   Messages are always passed to the inheriting Services, only copies of those
///   messages are sent to registered callbacks (after Service receives them).
///
///   This method only works if called before component/transport initialization.
///
///   \param[in] messageCode The type of message to register the callback for.
///   \param[in] callback The callback to use.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::RegisterCallback(const UShort messageCode, Transport::Callback* callback)
{
    // Can only register before initialization
    if(IsInitialized() == false)
    {
        MEMBER->mMessageCallbacks[messageCode].insert(callback);
    }
    else
    {
        std::cout << "Transport::RegisterCallback::ERROR - Must register before initialization!\n";
        assert(0 && "Transport::RegisterCallback::ERROR - Must register before initialization!\n");
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Given a message code, the Service attempts to create a Message object
///          that can be used for de-serialization of JAUS packets.
///
///   \param[in] messageCode JAUS Message to try create.
///   \param[in] service The Service to use to create a message.
///
///   \return Pointer to created message, NULL if message not supported by
///           or attached to the given Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Transport::CreateMessageFromService(const UShort messageCode, const Service *service) const
{
    Message* message = NULL;

    if(MEMBER->mStopMessageProcessingFlag)
    {
        return message;
    }

    const Transport* transport = dynamic_cast<const Transport*>(service);
    if(transport)
    {
        message =  GetMessageFromTemplate(messageCode);
    }
    // Make sure we do not do an infinite recursive call on ourselves!
    if(message == NULL && this != service)
    {
        message = service->CreateMessage(messageCode);
    }
    if(message == NULL)
    {
        Service::Map::const_iterator child;
        for(child = service->mJausChildServices.begin();
            child != service->mJausChildServices.end();
            child++)
        {
            if( (message = CreateMessageFromService(messageCode, child->second)) != NULL )
            {
                break;
            }
        }
    }

    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a packet containing a message type (UShort) and payload to
///          a Message structure if supported by the child services.
///
///   \param[in] packet Packet of payload data with the frist 2 bytes being
///                     the message code.  This packet should not contain
///                     the general transport header.
///
///   \return Pointer to created message, NULL if message not supported by
///           or attached to the given Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Transport::CreateMessageFromPacket(const Packet& packet) const
{
    // Read the header data from the packet.
    packet.SetReadPos(0);
    UShort messageCode = 0;
    packet.Read(messageCode);
    Message* message = CreateMessageFromService(messageCode, this);
    if(message)
    {
        Packet::Wrapper wrapper((unsigned char *)(packet.Ptr() + USHORT_SIZE), packet.Length() - USHORT_SIZE);
        if(message->ReadMessageBody(*wrapper.GetData()) < 0)
        {
            delete message;
            message = NULL;
            if(mDebugMessagesFlag)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Malformed Packet [0x" << std::setbase(16) << messageCode << std::setbase(10) << "]\n";
            }
        }
    }
    // Reset packet data.
    packet.SetReadPos(0);
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Triggers any callbacks registered for the given message type.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::TriggerMessageCallbacks(const Message* message)
{
    Callback::Map::iterator cb;
    cb = MEMBER->mMessageCallbacks.find(message->GetMessageCode());
    if(cb != MEMBER->mMessageCallbacks.end())
    {
        Callback::Set::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            (*cb2)->ProcessMessage(message);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Add a message class that the Transport service will save and use
///          to deserialize custom messages no created by any known Service.
///
///   \param[in] message Pointer to message template.  Pointer ownership will
///                      be taken and any memory deleted by service.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::AddMessageTemplate(Message* message)
{
    WriteLock wLock(MEMBER->mMessageTemplatesMutex);
    if(MEMBER->mMessageTemplates.find(message->GetMessageCode()) != MEMBER->mMessageTemplates.end())
    {
        delete MEMBER->mMessageTemplates[message->GetMessageCode()];
    }
    MEMBER->mMessageTemplates[message->GetMessageCode()] = message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets how long to wait before dropping connections.
///
///   \param[in] timeMs How long to wait in ms.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::SetDisconnectTimeMs(const unsigned int timeMs)
{ 
    return MEMBER->mNodeManager.GetSettings()->SetDropConnectionTimeMs(timeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return How long a connection will be inactive before it is considered no
///           longer alive and can be dropped (ms).
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Transport::GetDisconnectTimeMs() const { return MEMBER->mNodeManager.GetSettings()->GetDropConnectionTimeMs(); }


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the maximum number of threads to use for processing messages
///          received via the transport.
///
///   \param[in] limit Thread limit (default is 1).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::SetMaxMessageProcessingThreads(unsigned int limit)
{
    if(IsInitialized() == false && limit >= 1)
    {
        MEMBER->mProcessingThreadsLimit = limit;
        return true;
    }
    return false;
}


/*  Gets a pointer to the node manager, don't use if initialized. */
NodeManager* Transport::GetNodeManager() 
{
    if(IsInitialized() == false)
    {
        return &MEMBER->mNodeManager; 
    }
    return NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates the desired message from templates.
///
///   \param[in] messageCode Message type.
///
///   \return Pointer to message structure (you must delete) on success, NULL
///           if message type not found.
///
////////////////////////////////////////////////////////////////////////////////////
Message*  Transport::GetMessageFromTemplate(const UShort messageCode) const
{
    Message* message = NULL;
    {
        ReadLock rLock(*((SharedMutex*)&MEMBER->mMessageTemplatesMutex));
        std::map<UShort, Message*>::const_iterator m;
        m = MEMBER->mMessageTemplates.find(messageCode);
        if(m != MEMBER->mMessageTemplates.end())
        {
            message = m->second->Clone();
        }
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to a Message structure that has been previoulsy
///          created to reduce constant re-allocation of memory for
///          de-serializing message data.  If the message has not been
///          created previously, it get's made.
///
///   \param[in] messageCode Message type.
///
///   \return Pointer to message structure (you must delete) on success, NULL
///           if message type not found.
///
////////////////////////////////////////////////////////////////////////////////////
Message*  Transport::GetCachedMessage(const UShort messageCode)
{
    Message* message = NULL;
    {
        ReadLock rLock(MEMBER->mMessageCacheMutex);
        std::map<UShort, Message*>::iterator m;
        m = MEMBER->mMessageCache.find(messageCode);
        if(m == MEMBER->mMessageCache.end())
        {
            message = CreateMessage(messageCode);
            if(message)
            {
                MEMBER->mMessageCache[messageCode] = message;
            }
        }
        else
        {
            message = m->second;
        }
        if(message)
        {
            message->ClearMessage();
        }
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies message callbacks and templates from transport service.
///
///   This method is needed to preserve registered events, etc. when the
///   Transport method is changed within a Component.
///
///   \param[in] transport Pointer to transport service to get data from.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::CopyRegisteredItems(Transport* transport)
{
    {
        std::map<UShort, Message*>::iterator msg;
        ReadLock lock(( (Data *)transport->mpData)->mMessageTemplatesMutex);
        for(msg = ( (Data *)transport->mpData)->mMessageTemplates.begin();
            msg != ( (Data *)transport->mpData)->mMessageTemplates.end();
            msg++)
        {
            AddMessageTemplate(msg->second->Clone());
        }
    }
    {
        Callback::Map::iterator cb;
        for(cb = ( (Data *)transport->mpData)->mMessageCallbacks.begin();
            cb != ( (Data *)transport->mpData)->mMessageCallbacks.end();
            cb++)
        {
            Callback::Set::iterator cb2;
            for(cb2 = cb->second.begin();
                cb2 != cb->second.end();
                cb2++)
            {
                RegisterCallback(cb->first, *cb2);
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Helper method to match packets to pending receipts that are
///          from blocking send methods.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::CheckPendingReceipts(const Header& header,
                                     const UShort messageCode,
                                     const Packet& packet)
{
    bool foundReceipt = false;
    Receipt::Set::iterator receipt;

    ReadLock rLock(MEMBER->mPendingReceiptsMutex);

    for(receipt = MEMBER->mPendingReceipts.begin();
        receipt != MEMBER->mPendingReceipts.end() && false == foundReceipt;
        receipt++)
    {
        boost::lock_guard<boost::mutex> lock((*receipt)->mConditionMutex);
        if((*receipt)->mpResponse != NULL)
        {
            if((*receipt)->mpResponse ->GetMessageCode() == messageCode &&
                   (*receipt)->mpMessage->GetDestinationID() == header.mSourceID)
                {
                    if( (*receipt)->mpResponse ->Read(packet) )
                    {
                        foundReceipt = true;
                        (*receipt)->mPendingFlag = false;
                        (*receipt)->mWaitCondition.notify_one();
                        return true;
                    }
                }
        }
        else
        {
            Message::List::iterator responses;
            for(responses = (*receipt)->mpResponses->begin();
                responses != (*receipt)->mpResponses->end();
                responses++)
            {
                if((*responses)->GetMessageCode() == messageCode &&
                   (*receipt)->mpMessage->GetDestinationID() == header.mSourceID)
                {
                    if( (*responses)->Read(packet) )
                    {
                        Message* final = (*responses);
                        // Remove possible choices so we know which one
                        // has valid data.
                        responses = (*receipt)->mpResponses->begin();
                        while(responses != (*receipt)->mpResponses->end())
                        {
                            if( (*responses) != final )
                            {
                                responses = (*receipt)->mpResponses->erase(responses); continue;
                            }
                            responses++;
                        }

                        foundReceipt = true;
                        (*receipt)->mPendingFlag = false;
                        (*receipt)->mWaitCondition.notify_one();
                        return true;
                    }
                }
            }
        }
        if(foundReceipt)
            break;
    }

    return foundReceipt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Process all data as it arrives and assignes to packet handling
///          queues for processing by other methods.
///
///   \param[in] jausPacket JAUS Packet data without the transport header.
///   \param[in] jausHeader JAUS General Header information for the packet.
///   \param[in] connection Pointer to the connection the data was received by.
///   \param[in] sourceInfo Additional information about the source of the packet
///                         like the address it came in on.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::ProcessPacket(const Packet& jausPacket,
                              const Header& jausHeader,
                              const Connection* connection,
                              const Connection::Info* sourceInfo)
{
    // Ignore message not destined for this component.
    if(Address::DestinationMatch(jausHeader.mDestinationID, mComponentID) == false)
    {
        return;
    }

    UShort messageCode = 0;

    // Read the message code.
    jausPacket.SetReadPos(Header::PayloadOffset);
    jausPacket.Read(messageCode);
    jausPacket.SetReadPos(0);   // Reset the read position.

    // Send Acknowledge if requested.
    if(jausHeader.mAckNackFlag == Header::AckNack::Request)
    {
        Header ackHeader = jausHeader;
        ackHeader.mSourceID = mComponentID;
        ackHeader.mDestinationID = jausHeader.mSourceID;
        ackHeader.mSize = Header::MinSize;
        ackHeader.mAckNackFlag = Header::AckNack::Ack;
        ackHeader.mSequenceNumber = jausHeader.mSequenceNumber;
        Packet ackPacket;
        if(ackHeader.Write(ackPacket))
        {
            SendPacket(ackPacket, ackHeader);
        }
    }

    // Check for multi-packet or single stand alone packet
    if(jausHeader.mControlFlag == Header::DataControl::Single)
    {
#ifdef USE_MESSAGE_QUEUE
        this->ProcessSinglePackets((Packet *)&jausPacket);
#else
        WriteLock wLock(MEMBER->mSinglePacketQueueMutex);

        // Check for need to resize.
        if(MEMBER->mSinglePacketQueue.size() >= MEMBER->mSinglePacketQueue.capacity())
        {
            MEMBER->mSinglePacketQueue.resize(MEMBER->mSinglePacketQueue.capacity()*2);
        }
        MEMBER->mSinglePacketQueue.push_back(jausPacket);
#endif
    }
    else
    {
#ifdef USE_MESSAGE_QUEUE
        this->ProcessMultiPackets((Packet *)&jausPacket);
#else
        WriteLock wLock(MEMBER->mMultiPacketQueueMutex);

        // Check for need to resize.
        if(MEMBER->mMultiPacketQueue.size() >= MEMBER->mMultiPacketQueue.capacity())
        {
            MEMBER->mMultiPacketQueue.resize(MEMBER->mMultiPacketQueue.capacity()*2);
        }
        MEMBER->mMultiPacketQueue.push_back(jausPacket);
#endif
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Goes the through the single packets queue and de-serializes the data
///          and passes it to any neededing services.
///
///   \param[in] packet Optional parameter, if not NULL, packet to process, if
///                     NULL, function checks for packet buffer for data to
///                     process.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::ProcessSinglePackets(Packet* packet)
{
    bool dataToProcess = false;
    bool multithreaded = false;
    Packet mtPacket;

    MEMBER->mSinglePacketCache.Clear();

    Packet* packetPtr = packet;
    // If not given a packet, get one to read
    if(packetPtr == NULL)
    {
        packetPtr = &MEMBER->mSinglePacketCache;

        if(MEMBER->mProcessingThreads.size() > 0)
        {
            // We have more than one thread, can't use
            // the single cache.
            packetPtr = &mtPacket;
            multithreaded = true;
        }

        if(packet == NULL)
        {
            {

                WriteLock wLock(MEMBER->mSinglePacketQueueMutex);
                dataToProcess = MEMBER->mSinglePacketQueue.pop_front(&(*packetPtr));
            }
        }

        if((packet == NULL && false == dataToProcess) || MEMBER->mStopMessageProcessingFlag)
        {
            return;
        }
    }

    // De-serialize the data
    Header header;
    UShort messageCode = 0;
    Message* message = NULL;
    // Read the message type.
    packetPtr->SetReadPos(0);
    if(header.Read(*packetPtr) > 0 &&
        packetPtr->Read(messageCode) > 0)
    {
        bool foundReceipt = false;
        packetPtr->SetReadPos(0);

        // See if there is a receipt already waiting for this packet
        // as a response
        if(CheckPendingReceipts(header, messageCode, *packetPtr) == false)
        {
#ifndef USE_MESSAGE_QUEUE
            // If no pending match, then we must de-serialize and
            // share the data.

            // Create message.
            if(false == multithreaded)
            {
                message = GetCachedMessage(messageCode);
            }
            else
            {
                message = CreateMessage(messageCode);
            }
            // If supported, de-serialize data and receive.
            if(message && message->Read(*packetPtr) > 0)
            {
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Processing " << message->GetMessageName() << " Message\n";
                }
                PushMessageToChildren(message);
            }
            else if(message == NULL)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Unsupported Message Type [0x" << std::setbase(16) << messageCode << std::setbase(10) << "]\n";
            }
            
            if(multithreaded && message)
            {
                delete message;
            }
#else
            message = CreateMessage(messageCode);
            // If supported, de-serialize data and receive.
            if(message && message->Read(*packetPtr) > 0)
            {
                if(mDebugMessagesFlag)
                {
                    WriteLock printLock(mDebugMessagesMutex);
                    std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Processing " << message->GetMessageName() << " Message\n";
                }
                MEMBER->mMessageQueueMutex.Lock();
                MEMBER->mMessageQueue.push(message);
                MEMBER->mMessageQueueMutex.Unlock();
                message = NULL;
            }
            else if(message == NULL)
            {
                WriteLock printLock(mDebugMessagesMutex);
                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Unsupported Message Type [0x" << std::setbase(16) << messageCode << std::setbase(10) << "]\n";
            }
            if(message)
            {
                delete message;
            }
#endif
        }
    }
}


/** Goes the through the multi packets queue and de-serializes the data
    and passes it to any neededing services. */
void Transport::ProcessMultiPackets(Packet* packet)
{
    bool dataToProcess = false;
    bool multithreaded = false;

    MEMBER->mMultiPacketCache.Clear();

    Packet mtPacket;

    Packet* packetPtr = packet;

    // If not provided a packet, get one to process.
    if(packetPtr == NULL)
    {
        packetPtr = &MEMBER->mMultiPacketCache;

        if(MEMBER->mProcessingThreads.size() > 0)
        {
            packetPtr = &mtPacket;
            multithreaded = true;
        }

        {
            WriteLock wLock(MEMBER->mMultiPacketQueueMutex);
            dataToProcess = MEMBER->mMultiPacketQueue.pop_front(&(*packetPtr));
        }

        if(false == dataToProcess || MEMBER->mStopMessageProcessingFlag)
        {
            return;
        }
    }

    // De-serialize the data
    Header header;
    UShort messageCode = 0;
    Message* message = NULL;
    // Read the message type.
    packetPtr->SetReadPos(0);
    if(header.Read((*packetPtr)) > 0 &&
        packetPtr->Read(messageCode) > 0)
    {
        packetPtr->SetReadPos(0);

        // Multi-packet stream handling.
        UInt presenceVector = 0;
        LargeDataSet::Key key(header.mSourceID,
                              messageCode,
                              presenceVector);

        bool added = false;
        LargeDataSet::Map::iterator ld;

        for(ld = MEMBER->mLargeDataSets.begin();
            ld != MEMBER->mLargeDataSets.end() && added == false;
            ld++)
        {
            if(ld->second->mCompleteFlag == false &&
               ld->first.mMessageCode == key.mMessageCode &&
               ld->first.mSourceID == key.mSourceID)
            {
                // If the first in the sequendce, flush out the old data
                // and start over.
                if(header.mControlFlag == Header::DataControl::First)
                {
                    delete ld->second;
                    MEMBER->mLargeDataSets.erase(ld);
                }
                else if(ld->second->AddPacket(header, messageCode, (*packetPtr)))
                {
                    added = true;
                }
                break;
            }
        }

        if(added == false)
        {
            LargeDataSet* newStream = new LargeDataSet();
            if(newStream->AddPacket(header, messageCode, (*packetPtr)))
            {
                MEMBER->mLargeDataSets[key] = newStream;
            }
            else
            {
                delete newStream;
            }
            newStream = NULL;
        }

        // Check for pending receipts/and or completed data.
        ld = MEMBER->mLargeDataSets.begin();
        while(ld != MEMBER->mLargeDataSets.end())
        {
            if(ld->second->mCompleteFlag == true)
            {
                LargeDataSet* stream = ld->second;
                Message* message = NULL;

                // Remove from map
                MEMBER->mLargeDataSets.erase(ld);
                ld = MEMBER->mLargeDataSets.begin();

                    if(multithreaded)
                    {
                         packetPtr = &mtPacket;
                    }
                    else
                    {
                         packetPtr = &MEMBER->mLargeDataSetCache;
                         // Reset values in cache packet.
                         MEMBER->mLargeDataSetCache.Clear();
                    }
                // Try merge the stream into a single packet.
                if(LargeDataSet::MergeLargeDataSet(stream->mHeader,
                                                   stream->mMessageCode,
                                                   *packetPtr,
                                                   stream->mStream,
                                                   NULL))
                {
                    // If we merged the packet, then use the same methods as if
                    // this was a single packet.

                    // See if there is a receipt already waiting for this packet
                    // as a response
                    if(CheckPendingReceipts(stream->mHeader, stream->mMessageCode, *packetPtr) == false)
                    {
#ifndef USE_MESSAGE_QUEUE
                        // If no pending match, then we must de-serialize and
                        // share the data.

                        // Create message.
                        if(false == multithreaded)
                        {
                            message = GetCachedMessage(messageCode);
                        }
                        else
                        {
                            message = CreateMessage(messageCode);
                        }
                        // If supported, de-serialize data and receive.
                        if(message && message->Read(*packetPtr) > 0)
                        {
                            if(mDebugMessagesFlag)
                            {
                                WriteLock printLock(mDebugMessagesMutex);
                                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Processing " << message->GetMessageName() << " Message\n";
                            }
                            PushMessageToChildren(message);
                        }
                        else if(message == NULL)
                        {
                            WriteLock printLock(mDebugMessagesMutex);
                            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Unsupported Message Type [0x" << std::setbase(16) << stream->mMessageCode << std::setbase(10) << "]\n";
                        }

                        if(multithreaded && message)
                        {
                            delete message;
                        }
#else
                        message = CreateMessage(messageCode);
                        // If supported, de-serialize data and receive.
                        if(message && message->Read(*packetPtr) > 0)
                        {
                            if(mDebugMessagesFlag)
                            {
                                WriteLock printLock(mDebugMessagesMutex);
                                std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Processing " << message->GetMessageName() << " Message\n";
                            }
                            MEMBER->mMessageQueueMutex.Lock();
                            MEMBER->mMessageQueue.push(message);
                            MEMBER->mMessageQueueMutex.Unlock();
                            message = NULL;
                        }
                        else if(message == NULL)
                        {
                            WriteLock printLock(mDebugMessagesMutex);
                            std::cout << "[" << GetServiceID().ToString() << "-" << mComponentID.ToString() << "] - Received Unsupported Message Type [0x" << std::setbase(16) << stream->mMessageCode << std::setbase(10) << "]\n";
                        }

                        if(message)
                        {
                            delete message;
                        }
#endif
                    }
                }

                delete stream;
                stream = NULL;
            }
            else if(Time::GetUtcTimeMs() - ld->second->mUpdateTimeMs > 500)
            {
                delete ld->second;
                MEMBER->mLargeDataSets.erase(ld);
                ld = MEMBER->mLargeDataSets.begin();
            }
            else
            {
                ld++;
            }
        }
    }
}


/** Sends a  UDP message to the Node Manager to notify it that
    the component exists, so that it can find us. */
void Transport::NotifyNodeManager(const unsigned int waitForResponseTimeMs)
{
    ReportTime reportTime(GetComponentID(), GetComponentID());
    reportTime.SetTimeStamp(MEMBER->mpSharedMemory->GetParameters().mCreationTime);

    Header sendHeader;
    Packet sendPacket;
    Packet udpHeader;

    udpHeader.Write(UDP::Version);
    reportTime.Write(sendPacket, sendHeader, &udpHeader, true, 1);
    //reportTime.Read(sendPacket);
    CxUtils::UdpClient udp;

    udp.InitializeSocket(std::string("localhost"),
                         MEMBER->mNodeManager.GetSettings()->GetDefaultPortNumber());

    if(udp.Send(sendPacket) > 0)
    {
        // Now wait for someone to start reading from our shared memory connection.
        for(unsigned int i = 0; i < waitForResponseTimeMs; i++)
        {
            // If single thread, we must manually retreive data
            if(mSingleThreadModeFlag)
            {
                MEMBER->mpSharedMemory->ReceiveIncommingData();
            }
            if(MEMBER->mpSharedMemory->HasSubscriber())
            {
                break;
            }
            boost::this_thread::sleep(boost::posix_time::milliseconds(1)); 
        }
    }

    udp.Shutdown();

}

/*  End of File */
