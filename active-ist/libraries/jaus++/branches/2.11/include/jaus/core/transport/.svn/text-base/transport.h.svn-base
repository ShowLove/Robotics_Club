////////////////////////////////////////////////////////////////////////////////////
///
///  \file transport.h
///  \brief This file contains the definition for creating Transport Services
///  in JAUS++.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 September 2009
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
#ifndef __JAUS_CORE_TRANSPORT__H
#define __JAUS_CORE_TRANSPORT__H

#include "jaus/core/service.h"
#include "jaus/core/time.h"
#include <set>
#include <map>
#include <cxutils/circulararray.h>
#include <fstream>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Transport
    ///   \brief Transport is an interface class for creating Transport Services
    ///          defined by the SAE-JAUS standard.  All Transport Service 
    ///          implementations are based from this class.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Transport : public Service
    {
    public:        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used to register to get a copy of a message
        ///          received by the Transport Service.  Messages received through 
        ///          the Callback are still received by the Component Services.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_CORE_DLL Callback
        {
        public:
            Callback() {}
            virtual ~Callback() {}
            typedef std::set<Callback* > Set;
            typedef std::map<UShort, Set > Map;
            virtual void ProcessMessage(const Message* message) {};
        };
        static const std::string Name;                  ///<  Name of the service.
        static const unsigned int MaxQueueSize = 1024;  ///<  Maximum packet queue size.
        Transport();
        virtual ~Transport();
        // Closes a connection to a given component ID.
        virtual bool CloseConnection(const Address& id) { return false; }
        // Transport Service does not response to any messages.
        virtual bool IsDiscoverable() const { return false; }
        // Sets the maximum number of threads for message processing.
        void SetMaxMessageProcessingThreads(const unsigned int maxThreads = 2);
        // Add a message type to be processed within its own thread.
        void AddPriorityMessage(const UShort messageCode);
        // Sets the delay time in ms for message polling loop.
        virtual void SetMessagePollingDelayMs(const unsigned int delayTimeMs = 1);
        // Initializes the transport with a given ID for a component.
        virtual bool Initialize(const Address& componentID) = 0;
        // Returns true if transport has been initialized.
        virtual bool IsInitialized() const = 0;
        // Shutsdown the transport service.
        virtual void Shutdown() = 0;
        // Gets the list of connections available.
        virtual Address::List GetConnections() const = 0;
        // Returns true if connection is present.
        virtual bool HaveConnection(const Address& id) const = 0;
        // Sends to all children.
        virtual void Receive(const Message* message) { PushMessageToChildren(message); }
        // Send a serialized message.
        virtual bool SendPacket(const Packet& packet, 
                                const Header& header, 
                                const int broadcastFlags = NoBroadcast) const = 0;
        // Serialize the JAUS message (add transport headers, serialize payload, etc.) 
        virtual bool SerializeMessage(const Message* message, 
                                      Packet::List& stream,
                                      Header::List& streamHeaders,
                                      const UShort startingSequenceNumber,
                                      const int broadcastFlags) const = 0;
        // Get a copy of the transport header.
        virtual Packet GetTransportHeader() const = 0;
        // Gets a list of all manually created connections.
        virtual Address::List GetManualConnections() const = 0;
        // Searches inheriting Services (child Services) to create a message for processing.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Sends a message.
        virtual bool Send(const Message* message, const int broadcastFlags = Service::NoBroadcast) const;
        // Sends a message, then waits for a response.
        virtual bool Send(const Message* message, 
                          Message* response, 
                          const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Sends a message and waits for one of multiple responses.
        virtual bool Send(const Message* message,
                          Message::List& possibleResponses,
                          const unsigned int waitTimeMs = Service::DefaultWaitMs) const;
        // Register to receive copies of messages when received by Transport.
        void RegisterCallback(const UShort messageCode, Callback* callback);
        // Turns logging on/off.
        void EnableLogging(const bool flag = true);
        // Recursively try to create the message.
        Message* CreateMessageFromService(const UShort messageCode, const Service* service) const;
        // Reads the payload data which includes a message code, and converts to Message structure.
        Message* CreateMessageFromPacket(const Packet& packet) const;
        // Triggers any callbacks for a message.
        void TriggerMessageCallbacks(const Message* message);
        // Adds new message template.
        void AddMessageTemplate(Message* message);
        // Create a message using templates.
        Message* GetMessageFromTemplate(const UShort messageCode) const;
        // Copies message template and callbacks.
        void CopyRegisteredItems(Transport* transport);
    protected:
        virtual void ProcessPacket(const Packet& packet, const Header& header);
    private:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Receipt
        ///   \brief Data structure used to store information about messages waiting
        ///          for incomming responses.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Receipt
        {
        public:
            typedef std::set<Receipt*> Set; ///< List of receipt data.
            Receipt();
            ~Receipt();
            volatile bool mPendingFlag;     ///<  True if pending.
            const Message* mpMessage;       ///<  Message sent.
            Message::List* mpResponses;     ///<  Message response data.
            Time::Stamp mSendTimeMs;        ///<  Time when the message was sent.
            Time::Stamp mUpdateTimeMs;      ///<  Time when the receipt was updated (e.g. ACK/NACK Received).
        };       
        bool CheckPendingReceipts(const Header& header, const UShort messageCode, const Packet& packet);
        bool CheckPendingReceipts(const LargeDataSet* stream);
        static void MessageProcessingThread(void* args);
        static void PriorityMessageProcessingThread(void* args);
        void WriteToLog(const Packet& packet, const Header& header);
        bool IsPriority(Message** message);
        volatile bool mLoggingFlag;                             ///<  If true, logging is enabled.
        Mutex mLogFileMutex;                                    ///<  Mutex for log file.
        std::fstream mLogFile;                                  ///<  Log file.
        std::map<UShort, std::string> mMessageNames;            ///<  Lookup table of message names.
        volatile bool mStopMessageProcessingFlag;               ///<  If true, message processing should stop.
        Thread::Manager mMessageProcessingThreads;              ///<  Thread manager for handling message processing threads.
        Mutex mJausMessageQueueMutex;                           ///<  Mutex for thread protection of message queue.
        CxUtils::CircularArray<Packet> mJausMessageQueue;       ///<  Array of messages received by the Transport Service.
        Mutex mPriorityJausMessageMutex;                        ///<  Mutex for thread protection of priority data.
        std::set<UShort> mPriorityJausMessageTypes;             ///<  Types of priority messages.
        std::map<Address, Message::Map > mPriorityJausMessages; ///<  Priority JAUS messages that must be processed seperately.
        Mutex mMessageTemplatesMutex;                           ///<  Mutex for thread protection.
        std::map<UShort, Message*> mMessageTemplates;           ///<  Custom message templates.
        Thread mPriorityJausMessagesThread;                     ///<  Thread for priority messages.
        Mutex mLargeDataSetsMutex;                              ///<  Mutex for thread protection of large data set info.
        LargeDataSet::Map mLargeDataSets;                       ///<  Large data sets.
        unsigned int mNumberOfMessageProcessingThreads;         ///<  Number of message processing threads to use.
        volatile unsigned int mMessageProcessingDelayMs;        ///<  Maximum time to wait between message updates in ms to wait.
        Mutex mPendingReceiptsMutex;                            ///<  Mutex for thread protection of receipts.
        Receipt::Set mPendingReceipts;                          ///<  List of blocking send calls waiting for responses.
        Mutex mCallbacksMutex;                                  ///<  Mutex for thread protection of mutexes.
        Callback::Map mMessageCallbacks;                        ///<  Map of message callbacks.
        Mutex mSequenceNumberMutex;                             ///<  Mutex for thread protection of sequence number.
        UShort mSequenceNumber;                                 ///<  Message sequence number.
    };
}

#endif
/*  End of File */
