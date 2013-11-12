////////////////////////////////////////////////////////////////////////////////////
///
///  \file messageclient.h
///  \brief This file contains software for creating shared memory Message Client
///         that connects to a Message Server to receive messages through
///         shared memory.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 3 May 2009
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
#ifndef _CXUTILS_MAPPED_MESSAGE_CLIENT__H
#define _CXUTILS_MAPPED_MESSAGE_CLIENT__H

#include "cxutils/ipc/mappedmessagebox.h"
#include "cxutils/ipc/messageserver.h"
#include <set>
#include <vector>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class MessageClient
    ///   \brief Client interface to subscribe to/get messages from the MessageServer
    ///          class.  All messages are received through shared memory and can
    ///          work between processes.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL MessageClient
    {
    public:
        typedef MappedMemory::ID ID;                 ///<  ID Field.
        static const ID AnyID = 0;                   ///<  Use any available ID.

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used for getting messages received by the
        ///    client.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Callback
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Function
            ///   \brief Small structure for storing function callback data.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CX_UTILS_DLL Function
            {
            public:
                Function();
                Function(void (*callback)(Packet& packet, 
                                      void* args), 
                         void* cbArgs = NULL);
                Function(const Function& data);
                ~Function();
                void Run(Packet& packet);
                Function& operator=(const Function& data);
                void (*mpCallback)(Packet& packet, void* args);
                void *mpCallbackArgs;
            };
            virtual void ProcessMessage(const Packet& packet) = 0;
        };

        MessageClient();
        ~MessageClient();
        int Initialize(const std::string& serverID,
                       const ID clientID = AnyID,
                       const unsigned int messageBoxSize = MappedMessageBox::DefaultSize,
                       const bool mustConnect = true);
        int Initialize(const ID serverID,
                       const ID clientID,
                       const unsigned int messageBoxSize = MappedMessageBox::DefaultSize,
                       const bool mustConnect = true);
        int Initialize(const ID clientID,
                       const unsigned int messageBoxSize = MappedMessageBox::DefaultSize);
        void Shutdown();
        bool IsInitialized() const;
        bool ReadMessage(Packet& message) const;
        bool ReadMessage(std::string& message) const;
        bool RegisterCallback(Callback* callback);
        bool RegisterCallback(const Callback::Function& callback);
        void RemoveCallback(Callback* callback);
        void RemoveCallback(const Callback::Function& callback);
        bool IsConnected(const unsigned int thresholdMs = 500) const { return mRegistry.IsActive(thresholdMs); } ///<  Check if we are connected to an active server.
        void SetUpdateDelayMs(const unsigned int delayTimeMs = 0); ///<  Sets how quickly to check for data (0) is default.
        ID GetID() const { return mID; }
    private:
        static void ReceiveThread(void* messageClient);
        ID mID;                              ///<  Client ID.
        volatile bool mConnectToServerFlag;  ///<  If true, maintain a connection to a server, false otherwise.
        unsigned int mDesiredBoxSize;        ///<  Desired message box size.
        volatile unsigned int mUpdateDelayMs;///<  Update delay value for thread in milliseconds.
        MappedMessageBox mMessageBox;        ///<  Used to receive messages from server.
        MessageServer::Registry mRegistry;   ///<  Used to register with server.
        Thread mReceiveThread;               ///<  Manages message receiving thread.
        Mutex  mReceivedDataMutex;           ///<  Mutex for thread protection of received data.
        Mutex mDelayMutex;                   ///<  Mutex for thread protection of sleep value.
        Mutex  mCallbacksMutex;              ///<  Mutex for protection of registered callbacks.
        Packet::Queue mReceivedMessages;     ///<  Received messages.
        std::string mServerID;               ///<  ID of the message server generating messages.
        std::set<Callback*> mCallbacks;      ///<  Callbacks for received messages.
        std::vector<Callback::Function> mFunctionCallbacks; ///<  Function callback data for received messages.
        Packet mTempPacket;                  ///<  Temporary packet buffer.
    };
}

#endif
/* End of File */
