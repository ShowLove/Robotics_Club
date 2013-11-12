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
#include "jaus/core/events/event.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <iomanip>
#include <cxutils/fileio.h>

//  Include pthread library for LINUX
#ifdef WIN32

#else
#include <sys/sem.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

using namespace JAUS;

const std::string Transport::Name = "urn:jaus:jss:core:Transport";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::Receipt::Receipt()
{
    mPendingFlag = true;
    mpMessage = NULL;
    mSendTimeMs = mUpdateTimeMs = 0;
    mpResponses = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::Receipt::~Receipt()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::Transport() : Service(Service::ID(Transport::Name, 1.0), Service::ID())
{
    mLoggingFlag = false;
    mStopMessageProcessingFlag = false;
    mMessageProcessingDelayMs = 1;
    mNumberOfMessageProcessingThreads = 1;
    mJausMessageQueue.Reserve(MaxQueueSize);
    mSequenceNumber = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Transport::~Transport()
{
    mStopMessageProcessingFlag = true;
    CxUtils::SleepMs(50);
    mMessageProcessingThreads.StopAllThreads();
    mPriorityJausMessagesThread.StopThread();
    std::map<UShort, Message*>::iterator m;
    for(m = mMessageTemplates.begin(); m != mMessageTemplates.end(); m++)
    {
        delete m->second;
    }
    mMessageTemplates.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets how many threads should be created to handle processing of
///          messages received by the Transport Service.  Only works if
///          IsInitialized is false (i.e. before calling Initialize).
///
///   \param[in] maxThreads Number of threads (maxThreads >= 1).
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::SetMaxMessageProcessingThreads(const unsigned int maxThreads)
{
    if(IsInitialized() == false && maxThreads >= 1)
    {
        mNumberOfMessageProcessingThreads = maxThreads;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief A priority message is processed within it's own message
///          processing thread.  Priority messages are not queued, and any
///          old messages will be overwritten with new ones.
///
///   This feature is useful if you have a message type that arrives
///   frequently from a given data source, and you do not want it queued
///   because it will cause delays in your system.
///
///   \param[in] messageCode Message type to process seperately from other
///                          messages.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::AddPriorityMessage(const UShort messageCode)
{
    if(IsInitialized() == false)
    {
        mPriorityJausMessageTypes.insert(messageCode);
        if(mPriorityJausMessagesThread.IsThreadActive() == false)
        {
            mPriorityJausMessagesThread.CreateThread(PriorityMessageProcessingThread, this);
            char buffer[512];
            sprintf(buffer, "%s_PRIORITY_THREAD_%d", mComponentID.ToString().c_str(), mMessageProcessingThreads.GetNumThreads() + 1);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the delay time between checks for new messages to process by
///          each message processing thread.  
///
///   The default value is 1 ms, but can be set to 0 for no delay, or larger
///   for less frequent checking of messages (use larger numbers on slower 
///   hardware).
///
///   \param[in] delayTimeMs Delay time between checks in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::SetMessagePollingDelayMs(const unsigned int delayTimeMs)
{
    mMessageProcessingDelayMs = delayTimeMs;
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
    if(message == NULL)
    {
        message = GetMessageFromTemplate(messageCode);
    }
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
    Packet::List stream;
    Header::List streamHeaders;
    Packet::List::iterator packet;
    Header::List::iterator header;

    if(message->GetDestinationID() == mComponentID)
    {
        return false;
    }
    // Automatically set the source ID.
    ( (Message * ) message )->SetSourceID(mComponentID);
    mSequenceNumberMutex.Lock();
    if(SerializeMessage(message, stream, streamHeaders, mSequenceNumber, (Byte)broadcastFlags))
    {
        *((UShort *)(&mSequenceNumber)) += (UShort)stream.size();
        mSequenceNumberMutex.Unlock();
        for(packet = stream.begin(), header = streamHeaders.begin();
            packet != stream.end() && header != streamHeaders.end();
            packet++, header++)
        {
            if(SendPacket(*packet, *header, broadcastFlags) == false)
            {
                return false;
            }
        }
        return true;
    }
    mSequenceNumberMutex.Unlock();
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
///                         received.
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
///                         received.
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

    if(message == NULL || possibleResponses.size() == 0 || waitTimeMs == 0 || message->GetDestinationID().IsBroadcast())
    {
        return false;
    }

    mPendingReceiptsMutex.Lock();
    receipt.mPendingFlag = true;
    receipt.mpMessage = message;
    receipt.mpResponses = &possibleResponses;
    receipt.mSendTimeMs = receipt.mUpdateTimeMs = Time::GetUtcTimeMs();
    Receipt::Set* pending = ((Receipt::Set *)&mPendingReceipts);
    pending->insert(&receipt);
    mPendingReceiptsMutex.Unlock();

    if(Send(message))
    {
        Time::Stamp timeMs = Time::GetUtcTimeMs();
        while(timeMs - receipt.mSendTimeMs < waitTimeMs && receipt.mPendingFlag == true)
        {
            timeMs = Time::GetUtcTimeMs();
            // Small sleep statement to not kill processor.
#ifdef WIN32
            CxUtils::SleepMs(1);
#else
            usleep(1);
#endif
        }
    }

    // Regardless of result
    // remove the receipt from the pending set.
    mPendingReceiptsMutex.Lock();
    ritr = pending->find(&receipt);
    if(ritr != pending->end())
    {
        pending->erase(ritr);
    }
    mPendingReceiptsMutex.Unlock();
    
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
///   \param[in] messageCode The type of message to register the callback for.
///   \param[in] callback The callback to use.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::RegisterCallback(const UShort messageCode, Transport::Callback* callback)
{
    Mutex::ScopedLock lock(&mCallbacksMutex);
    mMessageCallbacks[messageCode].insert(callback);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Turns on logging of data sent to/from the component using the
///          transport service.
///
///   \param[in] flag If true, turns logging on, if false, turns off.  Logging
///                   is off by default.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::EnableLogging(const bool flag)
{
    Mutex::ScopedLock lock(&mLogFileMutex);
    mLoggingFlag = flag;
    if(flag)
    {
        if(mLogFile.is_open() == false)
        {
            // Generate a file name, and create log file.
            CxUtils::FileIO::CreateDir("logs");
            std::string fname = "logs/JAUS++ Transport Receive Log - ";
            fname += JAUS::Time::GetUtcTime().ToString();
            fname += ".csv";
            mLogFile.open(fname.c_str(), std::ios_base::out);
            if(mLogFile.is_open())
            {
                mLogFile << "Time (UTC), Message Code, Message Name, Source ID, Destination ID, Size (Bytes), Sequence Number, Message Body\n";
                mLogFile.flush();
            }
        }
    }
    else
    {
        if(mLogFile.is_open())
        {
            mLogFile.close();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called whenever a JAUS packet has been received by
///          the transport layer.
///
///   \param[in] packet JAUS formatted packet to process.  Any transport
///                     headers excluding the transport header
///                     must be removed.
///   \param[in] header General Transport Header for the packet.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::ProcessPacket(const Packet& packet, const Header& header)
{
    // Ignore message not destined for this component.
    if(Address::DestinationMatch(header.mDestinationID, mComponentID) == false)
    {
        return;
    }

    if(mLoggingFlag)
    {
        WriteToLog(packet, header);
    }

    if(mMessageProcessingThreads.GetNumThreads() < mNumberOfMessageProcessingThreads)
    {
        char buffer[512];
        sprintf(buffer, "%s_MP_THREAD_%d", mComponentID.ToString().c_str(), mMessageProcessingThreads.GetNumThreads() + 1);
        mMessageProcessingThreads.CreateThread(buffer, Transport::MessageProcessingThread, this);
    }

    UShort messageCode = 0;
    // Read the message code.
    packet.SetReadPos(Header::PayloadOffset);
    packet.Read(messageCode);
    packet.SetReadPos(0);   // Reset the read position.
    
    // Send Acknowledge if requested.
    if(header.mAckNackFlag == Header::AckNack::Request)
    {
        Header ackHeader = header;
        ackHeader.mSourceID = mComponentID;
        ackHeader.mDestinationID = header.mSourceID;
        ackHeader.mSize = Header::MinSize;
        ackHeader.mAckNackFlag = Header::AckNack::Ack;
        ackHeader.mSequenceNumber = header.mSequenceNumber;
        Packet ackPacket;
        if(ackHeader.Write(ackPacket))
        {
            SendPacket(ackPacket, ackHeader);
            if(mLoggingFlag)
            {
                WriteToLog(ackPacket, ackHeader);
            }
        }
    }
    // Check for multi-packet stream
    if(header.mControlFlag == Header::DataControl::Single)
    {
        // Check for blocking receive
        if(CheckPendingReceipts(header, messageCode, packet) == false)
        {
            // Add to queue for processing in message thread.
            mPendingReceiptsMutex.Unlock();
            Mutex::ScopedLock lock(&mJausMessageQueueMutex);
            if(mJausMessageQueue.Size() >= mJausMessageQueue.Reserved())
            {
                mJausMessageQueue.Resize(mJausMessageQueue.Reserved()*2);
            }
            mJausMessageQueue.PushBack(packet);
        }
    }
    else
    {
        // Multi-packet stream handling.
        UInt presenceVector = 0;
        LargeDataSet::Key key(header.mSourceID,
                              messageCode,
                              presenceVector);

        // Try to find a data set to add to.
        Mutex::ScopedLock lockLargeData(&mLargeDataSetsMutex);

        bool added = false;
        LargeDataSet::Map::iterator ld;

        for(ld = mLargeDataSets.begin();
            ld != mLargeDataSets.end() && added == false;
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
                    mLargeDataSets.erase(ld);
                }                
                else if(ld->second->AddPacket(header, messageCode, packet))
                {
                    added = true;
                }
                break;
            }                   
        }

        if(added == false)
        {
            LargeDataSet* newStream = new LargeDataSet();
            if(newStream->AddPacket(header, messageCode, packet))
            {
                mLargeDataSets[key] = newStream;
            }
            else
            {
                delete newStream;
            }
            newStream = NULL;
        }

        // Check for pending receipts.
        ld = mLargeDataSets.begin();
        while(ld != mLargeDataSets.end())
        {
            if(ld->second->mCompleteFlag == true)
            {
                if(CheckPendingReceipts(ld->second))
                {
                    delete ld->second;
                    ld = mLargeDataSets.begin();
                    continue;
                }
            }
            ld++;
        }
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

    if(mStopMessageProcessingFlag)
    {
        return message;
    }

    const Transport* transport = dynamic_cast<const Transport*>(service);
    if(transport)
    {
        message =  GetMessageFromTemplate(messageCode);
    }

    if(message == NULL && this != service)
    {
        message = service->CreateMessage(messageCode);
    }
    if(message == NULL)
    {
        Service::Map children;
        // Copy the child services.
        {
            Mutex::ScopedLock lock(&service->mJausServiceMutex);
            children = service->mJausChildServices;
        }
        Service::Map::const_iterator child;
        for(child = children.begin();
            child != children.end();
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
                Mutex::ScopedLock lock(&mDebugMessagesMutex);
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
    Mutex::ScopedLock lock(&mCallbacksMutex);
    Callback::Map::iterator cb;
    cb = mMessageCallbacks.find(message->GetMessageCode());
    if(cb != mMessageCallbacks.end())
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
    Mutex::ScopedLock lock(&mMessageTemplatesMutex);
    if(mMessageTemplates.find(message->GetMessageCode()) != mMessageTemplates.end())
    {
        delete mMessageTemplates[message->GetMessageCode()];
    }
    mMessageTemplates[message->GetMessageCode()] = message;
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
        Mutex::ScopedLock lock(&mMessageTemplatesMutex);
        std::map<UShort, Message*>::const_iterator m;
        m = mMessageTemplates.find(messageCode);
        if(m != mMessageTemplates.end())
        {
            message = m->second->Clone();
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
        Mutex::ScopedLock lock(&transport->mMessageTemplatesMutex);
        for(msg = transport->mMessageTemplates.begin();
            msg != transport->mMessageTemplates.end();
            msg++)
        {
            AddMessageTemplate(msg->second->Clone());
        }
    }
    {
        Mutex::ScopedLock lock(&transport->mCallbacksMutex);
        Callback::Map::iterator cb;
        for(cb = transport->mMessageCallbacks.begin();
            cb != transport->mMessageCallbacks.end();
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
    mPendingReceiptsMutex.Lock();
    for(receipt = mPendingReceipts.begin();
        receipt != mPendingReceipts.end() && false == foundReceipt;
        receipt++)
    {
        Message::List::iterator responses;
        for(responses = (*receipt)->mpResponses->begin();
            responses != (*receipt)->mpResponses->end();
            responses++)
        {              
            if((*responses)->GetMessageCode() == messageCode &&
               (*receipt)->mpMessage->GetDestinationID() == header.mSourceID)
            {
                if( (*responses)->Read(packet) && (*responses)->IsResponseToMessage((*receipt)->mpMessage))
                {
                    foundReceipt = true;
                    (*receipt)->mPendingFlag = false;
                    (*receipt)->mUpdateTimeMs = Time::GetUtcTimeMs();                        
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
                    mPendingReceipts.erase(receipt);
                    break;
                }
            }
        }
        if(foundReceipt)
            break;
    }
    mPendingReceiptsMutex.Unlock();

    return foundReceipt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Helper method to match packets to pending receipts that are
///          from blocking send methods.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::CheckPendingReceipts(const LargeDataSet* stream)
{
    bool foundReceipt = false;
    Receipt::Set::iterator receipt;
    mPendingReceiptsMutex.Lock();
    for(receipt = mPendingReceipts.begin();
        receipt != mPendingReceipts.end() && false == foundReceipt;
        receipt++)
    {
        Message::List::iterator responses;
        for(responses = (*receipt)->mpResponses->begin();
            responses != (*receipt)->mpResponses->end();
            responses++)
        {              
            if((*responses)->GetMessageCode() == stream->mMessageCode &&
               (*receipt)->mpMessage->GetDestinationID() == stream->mHeader.mSourceID)
            {
                if( (*responses)->ReadLargeDataSet(stream->mStream) && (*responses)->IsResponseToMessage((*receipt)->mpMessage))
                {
                    foundReceipt = true;
                    (*receipt)->mPendingFlag = false;
                    (*receipt)->mUpdateTimeMs = Time::GetUtcTimeMs();                        
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
                    mPendingReceipts.erase(receipt);
                    break;
                }
            }
        }
        if(foundReceipt)
            break;
    }
    mPendingReceiptsMutex.Unlock();

    return foundReceipt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method performed in threads for processing messages.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::MessageProcessingThread(void* args)
{
    Transport* transport = (Transport *)args;
    Packet packetToProcess;
    bool dataToProcess = false;
    unsigned int loopCounter = 0;
    
    while(transport && transport->mStopMessageProcessingFlag == false)
    {
        dataToProcess = false;
        packetToProcess.Clear();

        // Grab the next available message.
        // This will block until a message is received.
        transport->mJausMessageQueueMutex.Lock();
        dataToProcess = transport->mJausMessageQueue.PopFront(&packetToProcess);
        transport->mJausMessageQueueMutex.Unlock();
        
        if(dataToProcess && transport->mStopMessageProcessingFlag == false)
        {
            Header header;
            UShort messageCode = 0;
            Message* message = NULL;
            // Read the message type.
            packetToProcess.SetReadPos(0);            
            if(header.Read(packetToProcess) > 0 && 
               packetToProcess.Read(messageCode) > 0)
            {  
                bool foundReceipt = false;
                packetToProcess.SetReadPos(0);                
                // Create message.
                message = transport->CreateMessageFromService(messageCode, transport);
                // If supported, de-serialize data and receive.
                if(message && message->Read(packetToProcess) > 0)
                {
                    if(transport->mDebugMessagesFlag)
                    {
                        Mutex::ScopedLock lock(&transport->mDebugMessagesMutex);
                        std::cout << "[" << transport->GetServiceID().ToString() << "-" << transport->mComponentID.ToString() << "] - Processing " << message->GetMessageName() << " Message\n";
                    }
                    if(transport->mStopMessageProcessingFlag == false)
                    {
                        if(transport->IsPriority(&message) == false)
                        {
                            transport->PushMessageToChildren(message);
                        }
                    }
                }
                else if(message == NULL && transport->mDebugMessagesFlag)
                {
                    Mutex::ScopedLock lock(&transport->mDebugMessagesMutex);
                    std::cout << "[" << transport->GetServiceID().ToString() << "-" << transport->mComponentID.ToString() << "] - Received Unsupported Message Type [0x" << std::setbase(16) << messageCode << std::setbase(10) << "]\n";
                }
            }
            
            // Clean up memory.
            if(message)
            {
                delete message;
            }
        }
        // Check for exit.
        if(transport->mStopMessageProcessingFlag == true)
        {
            break;
        }

        // Check for completed large data sets for processing.
        LargeDataSet* stream = NULL;
        LargeDataSet::Map::iterator lds;        
        transport->mLargeDataSetsMutex.Lock();
        lds = transport->mLargeDataSets.begin();
        while(lds != transport->mLargeDataSets.end())
        {
            // Get completed streams for processing.
            if(lds->second->mCompleteFlag)
            {
                stream = lds->second;
                transport->mLargeDataSets.erase(lds);
                break;
            }
            // Delete streams that have not updated recently.
            else if(Time::GetUtcTimeMs() - lds->second->mUpdateTimeMs > 500)
            {
                delete lds->second;
                transport->mLargeDataSets.erase(lds);
                lds = transport->mLargeDataSets.begin();
                continue;
            }
            lds++;
        }
        transport->mLargeDataSetsMutex.Unlock();

        // Process Multi-Packet Stream.
        if(stream)
        {
            Message* message = NULL;
            message = transport->CreateMessageFromService(stream->mMessageCode, transport);
            if(message && message->ReadLargeDataSet(stream->mStream) && transport->mStopMessageProcessingFlag == false)
            {
                if(transport->IsPriority(&message) == false)
                {
                    transport->PushMessageToChildren(message);
                }
            }
            // Clean up memory.
            if(message)
            {
                delete message;
            }
            delete stream;
            stream = NULL;
        }
        
        if(transport->mMessageProcessingDelayMs == 0)
        {
            // Use a small sleep statement so that we don't
            // max out the CPU, but don't do it all the time.
            #ifdef WIN32
            if( ++loopCounter == 10 )
            {
                loopCounter = 0;
                CxUtils::SleepMs(1);
            }
            #else
            loopCounter = 0;
            usleep(500);
            #endif
        }
        else
        {
            CxUtils::SleepMs(transport->mMessageProcessingDelayMs);
        }
    }

    packetToProcess.Destroy();
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method performed in threads for processing messages.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::PriorityMessageProcessingThread(void* args)
{
    Transport* transport = (Transport *)args;
    Message::List toProcess;
    Message::List::iterator message;

    unsigned int loopCounter = 0;
    
    while(transport && transport->mStopMessageProcessingFlag == false)
    {
        std::map<Address, Message::Map>::iterator src;
        Message::Map::iterator m;
        transport->mPriorityJausMessageMutex.Lock();
        for(src = transport->mPriorityJausMessages.begin();
            src != transport->mPriorityJausMessages.end();
            src++)
        {
            for(m = src->second.begin();
                m != src->second.end();
                m++)
            {
                toProcess.push_back(m->second);
            }
            src->second.clear();
        }
        transport->mPriorityJausMessageMutex.Unlock();

        for(message = toProcess.begin();
            message != toProcess.end();
            message++)
        {
            transport->PushMessageToChildren(*message);
            delete (*message);
        }
        toProcess.clear();

        if(transport->mMessageProcessingDelayMs == 0)
        {
            // Use a small sleep statement so that we don't
            // max out the CPU, but don't do it all the time.
            #ifdef WIN32
            if( ++loopCounter == 10 )
            {
                loopCounter = 0;
                CxUtils::SleepMs(1);
            }
            #else
            loopCounter = 0;
            usleep(500);
            #endif
        }
        else
        {
            CxUtils::SleepMs(transport->mMessageProcessingDelayMs);
        }
    }

    std::map<Address, Message::Map>::iterator src;
    Message::Map::iterator m;
    transport->mPriorityJausMessageMutex.Lock();
    for(src = transport->mPriorityJausMessages.begin();
        src != transport->mPriorityJausMessages.end();
        src++)
    {
        for(m = src->second.begin();
            m != src->second.end();
            m++)
        {
           delete m->second;
        }
    }
    transport->mPriorityJausMessages.clear();
    transport->mPriorityJausMessageMutex.Unlock();

    for(message = toProcess.begin();
        message != toProcess.end();
        message++)
    {
        delete (*message);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes data to log file.
///
////////////////////////////////////////////////////////////////////////////////////
void Transport::WriteToLog(const Packet& packet,
                           const Header& header)
{
    if(mLoggingFlag)
    {
        Mutex::ScopedLock lock(&mLogFileMutex);
        if(mLogFile.is_open() == false) { return; }

        std::string messageName = "Unknown";
        mLogFile << Time::GetUtcTime().ToString() << ", ";
        UShort messageCode = 0;
        packet.Read(messageCode, Header::PayloadOffset);
        mLogFile << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(4) << messageCode << ", ";
        
        // Lookup message name.
        std::map<UShort, std::string>::iterator n;
        n = mMessageNames.find(messageCode);
        if(n == mMessageNames.end())
        {
            Message* message = CreateMessageFromService(messageCode, this);
            if(message)
            {
                messageName = mMessageNames[messageCode] = message->GetMessageName();
                delete message;
            }
            else
            {
                messageName = "Unknown";
            }
        }
        else
        {
            messageName = n->second;
        }
        mLogFile << messageName << ", ";

        mLogFile << header.mSourceID.ToString() << ", " << header.mDestinationID.ToString() << ", ";
        mLogFile << std::dec << header.mSize << ", " << header.mSequenceNumber << ", ";
        unsigned char* ptr = (unsigned char *)(packet.Ptr() + Header::PayloadOffset + USHORT_SIZE);
        for(unsigned int i = 0; i < packet.Size() - (Header::PayloadOffset + USHORT_SIZE); i++)
        {
            mLogFile << "0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(2) << (unsigned short)*ptr << ", ";
            ptr++;
        }
        mLogFile << std::endl;
        mLogFile.flush();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the message was high priority and passed to a high
///          priority processing thread.
///
///   \param[in] message Message pointer, set to NULL if ownership taken.
///
///   \return True if high priority and ownership of pointer taken, false 
///           otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Transport::IsPriority(Message** message)
{
    bool result = false;

    JAUS::Event* e = dynamic_cast<JAUS::Event*>(*message);
    UShort messageCode = (*message)->GetMessageCode();
    if(e)
    {
        messageCode = e->GetReportMessageCode();
    }

    mPriorityJausMessageMutex.Lock();
    
    if(mPriorityJausMessageTypes.find(messageCode) != mPriorityJausMessageTypes.end())
    {
        result = true;
        std::map<Address, Message::Map >::iterator src;
        src = mPriorityJausMessages.find((*message)->GetSourceID());
        if(src == mPriorityJausMessages.end())
        {
            mPriorityJausMessages[(*message)->GetSourceID()][(*message)->GetMessageCode()] = (*message);
        }
        else
        {
            Message::Map::iterator msg;
            msg = src->second.find((*message)->GetMessageCode());
            if(msg == src->second.end())
            {
                src->second[(*message)->GetMessageCode()] = (*message);
            }
            else
            {
                delete msg->second;
                msg->second = (*message);
            }
        }
        // De-reference.
        (*message) = NULL;
    }
    mPriorityJausMessageMutex.Unlock();
    return result;
}


/*  End of File */
