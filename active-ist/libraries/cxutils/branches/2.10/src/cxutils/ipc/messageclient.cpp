////////////////////////////////////////////////////////////////////////////////////
///
///  \file messageclient.cpp
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
#include "cxutils/ipc/messageclient.h"
#include "cxutils/time.h"
#include <iostream>
#include <sstream>

using namespace std;
using namespace CxUtils;

unsigned int gMaxQueueSize = 500;

//#define __CX_DEBUG

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::Callback::Function::Function()
{
    mpCallback = NULL;
    mpCallbackArgs = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes callback functions and arguments.
///
///   \param[in] callback Function pointer to callback to be run when messages
///                       are received.
///   \param[in] cbArgs Pointer to additional callack arguments.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::Callback::Function::Function(void (*callback)(Packet& packet, void* args),
                                            void* cbArgs)
{
    mpCallback = callback;
    mpCallbackArgs = cbArgs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::Callback::Function::Function(const MessageClient::Callback::Function& data)
{
    *this = data;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::Callback::Function::~Function()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs the Function Callback.
///
///   \param[in] packet UDP message to process in callback.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::Callback::Function::Run(Packet& packet)
{
    if(mpCallback)
    {
        mpCallback(packet, mpCallbackArgs);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::Callback::Function& MessageClient::Callback::Function::operator=(const MessageClient::Callback::Function& data)
{
    mpCallback = data.mpCallback;
    mpCallbackArgs = data.mpCallbackArgs;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::MessageClient()
{
    mConnectToServerFlag = false;
    mDesiredBoxSize = 0;
    mUpdateDelayMs = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageClient::~MessageClient()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the client by making a connection to a message server.
///
///   \param[in] serverID The ID of the server to try and connect to.
///   \param[in] clientID The ID to assign to your client, set to AnyID if you
///                       don't care.
///   \param[in] messageBoxSize Size in bytes to make your internal receive
///                             buffer in shared memory.  Make this value larger
///                             or smaller depending on the volume and size
///                             of messages you expect to receive.
///   \param[in] mustConnect If true, then we must successfully open a connection
///                          to a MessageServer.  If false, then the client
///                          will try to connect on it's own as soon as the
///                          server is available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageClient::Initialize(const ID serverID, 
                              const ID clientID,
                              const unsigned int messageBoxSize,
                              const bool mustConnect)
{
    std::ostringstream input;
    input << serverID;
    return Initialize(input.str(), clientID, messageBoxSize, mustConnect);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the client by making a connection to a message server.
///
///   \param[in] serverID The ID of the server to try and connect to.
///   \param[in] clientID The ID to assign to your client, set to AnyID if you
///                       don't care.
///   \param[in] messageBoxSize Size in bytes to make your internal receive
///                             buffer in shared memory.  Make this value larger
///                             or smaller depending on the volume and size
///                             of messages you expect to receive.
///   \param[in] mustConnect If true, then we must successfully open a connection
///                          to a MessageServer.  If false, then the client
///                          will try to connect on it's own as soon as the
///                          server is available.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageClient::Initialize(const std::string& serverID, 
                              const ID clientID,
                              const unsigned int messageBoxSize,
                              const bool mustConnect)
{
    int result = 0;
    Shutdown();
    
    // Connect to the server.
    if(mRegistry.OpenRegistry(serverID) || mustConnect == false)
    {
        mServerID = serverID;
        mConnectToServerFlag = true;
        mDesiredBoxSize = messageBoxSize;
        // Try to create a message box.
        ID id = clientID;
        if(id == AnyID && mRegistry.IsOpen())
        {
            // Try any available ID.
            for(id = 1; id < 10000 && result == 0; id++)
            {
                if(mRegistry.IsRegistered(id) == false)
                {
                    if(mMessageBox.CreateMessageBox(id, messageBoxSize) || mustConnect == false)
                    {
                        mID = id;
                        if(mReceiveThread.CreateThread(MessageClient::ReceiveThread, this))
                        {
                            //mReceiveThread.SetThreadPriority(50);
                            mReceiveThread.SetThreadName("MessageClientThread");
                            result = 1;
                            break;
                        }
                        else
                        {
                            #ifdef __CX_DEBUG
                            cout << "MessageClient::Initialize - Unable to create thread.\n";
                            #endif
                        }
                    }
                    else
                    {
                        #ifdef __CX_DEBUG
                        cout << "MessageClient::Initialize - Unable to create a Message Box [ID = " << id << "].\n";
                        #endif
                    }
                }
                SleepMs(1);
            }
        }        
        else if(mMessageBox.CreateMessageBox(id, messageBoxSize) || mustConnect == false)
        {
            mID = id;
            if(mReceiveThread.CreateThread(MessageClient::ReceiveThread, this))
            {
                //mReceiveThread.SetThreadPriority(50);
                mReceiveThread.SetThreadName("MessageClientThread");
                result = 1;
            }
            else
            {
                #ifdef __CX_DEBUG
                cout << "MessageClient::Initialize - Unable to create thread.\n";
                #endif
            }
        }
        else
        {
            #ifdef __CX_DEBUG
            cout << "MessageClient::Initialize - Unable to create a Message Box [ID = " << id << "].\n";
            #endif
        }        
    }
    else
    {
        //cout << "MessageClient::Initialize - Unable to open Message Server Registry.\n";
    }
        
    
    
    if(result == 0)
    {
        Shutdown();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes a MessageBox that does not connect to a server.  This
///   is useful because this method will create a MessageBox with a thread
///   continuously reading from it.
///
///   \param[in] clientID The ID to use for the creation of a MessageBox.
///   \param[in] messageBoxSize Size in bytes to make your internal receive
///                             buffer in shared memory.  Make this value larger
///                             or smaller depending on the volume and size
///                             of messages you expect to receive.
///
///   \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageClient::Initialize(const ID clientID,
                              const unsigned int messageBoxSize)
{
    int result = 0;
    Shutdown();

    if(mMessageBox.CreateMessageBox(clientID, messageBoxSize))
    {
        mID = clientID;
        if(mReceiveThread.CreateThread(MessageClient::ReceiveThread, this))
        {
            //mReceiveThread.SetThreadPriority(50);
            result = 1;
        }
        else
        {
#ifdef __CX_DEBUG
            cout << "MessageClient::Initialize - Unable to create thread.\n";
#endif
        }
    }
    else
    {
#ifdef __CX_DEBUG
        cout << "MessageClient::Initialize - Unable to create a Message Box [ID = " << id << "].\n";
#endif
    }              
    
    if(result == 0)
    {
        Shutdown();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Closes client connection and removes from Message Server registry.
///
///   Callbacks are not cleared by this method.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::Shutdown()
{
    mReceiveThread.StopThread();
    mRegistry.Unregister(mID);
    mRegistry.CloseRegistry();
    mMessageBox.CloseMessageBox();
    mReceivedDataMutex.Lock();
    mReceivedMessages.clear();
    mReceivedDataMutex.Unlock();
    mConnectToServerFlag = false;
    mID = 0;    
    mDesiredBoxSize = 0;
    mServerID.clear();
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if initialized, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageClient::IsInitialized() const
{
    return mReceiveThread.IsThreadActive();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the next message received by the client.
///
///   \param[in] message The message contents read.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageClient::ReadMessage(Packet& message) const
{
    message.Clear();
    Packet::Queue* buffer = ( (Packet::Queue *)(&mReceivedMessages) );
    mReceivedDataMutex.Lock();
    if(buffer->size() > 0)
    {
        message = buffer->front();
        buffer->pop_front();
    }
    mReceivedDataMutex.Unlock();
    return message.Length() > 0 ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads the next message received by the client.
///
///   \param[in] message The message contents read.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageClient::ReadMessage(std::string& message) const
{
    message.clear();
    Packet::Queue* buffer = ( (Packet::Queue *)(&mReceivedMessages) );
    mReceivedDataMutex.Lock();
    if(buffer->size() > 0)
    {
        message = (char *)buffer->front().Ptr();
        buffer->pop_front();
    }
    mReceivedDataMutex.Unlock();
    return message.size() > 0 ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to be run whenever a new message is
///          received.
///
///   \param[in] callback Pointer to callback to be run.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageClient::RegisterCallback(Callback* callback)
{
    bool result = false;
    std::set<Callback*>::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbacks.find(callback);
    if(cb == mCallbacks.end())
    {
        result = true;
        mCallbacks.insert(callback);
    }
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a function callback to be run whenever a new message is
///          received.
///
///   \param[in] callback Function callback data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageClient::RegisterCallback(const Callback::Function& callback)
{
    bool result = false;
    std::vector<Callback::Function>::iterator cb;
    mCallbacksMutex.Lock();
    for(cb = mFunctionCallbacks.begin();
        cb != mFunctionCallbacks.end();
        cb++)
    {
        if(cb->mpCallback == callback.mpCallback &&
           cb->mpCallbackArgs == callback.mpCallbackArgs)
           {
               break;
           }
    }
    if(cb == mFunctionCallbacks.end())
    {
        result = true;
        mFunctionCallbacks.push_back(callback);
    }
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a registered callback.
///
///   \param[in] callback Callback data to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::RemoveCallback(Callback* callback)
{
    std::set<Callback*>::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbacks.find(callback);
    if(cb != mCallbacks.end())
    {
        mCallbacks.erase(callback);
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a registered callback.
///
///   \param[in] callback Callback data to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::RemoveCallback(const Callback::Function& callback)
{
    std::vector<Callback::Function>::iterator cb;
    mCallbacksMutex.Lock();
    for(cb = mFunctionCallbacks.begin();
        cb != mFunctionCallbacks.end();
        cb++)
    {
        if(cb->mpCallback == callback.mpCallback &&
           cb->mpCallbackArgs == callback.mpCallbackArgs)
           {
               mFunctionCallbacks.erase(cb);
               break;
           }
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the polling frequency for Interprocess Communication. 
///
///   \param[in] delayTimeMs How long to wait in ms between checks for
///                          new data.  Set to 0 for no wait.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::SetUpdateDelayMs(const unsigned int delayTimeMs)
{
    Mutex::ScopedLock lock(&mDelayMutex);
    mUpdateDelayMs = delayTimeMs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method that continuously pulls message received from shared memory
///   and places them inside a queue for processing.  
///
///   This method also runs any callbacks associated with a message received
///   event, and makes sure the client is registered with the server.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageClient::ReceiveThread(void* args)
{
    MessageClient* client = (MessageClient *)args;
    #ifdef WIN32
    unsigned int loopCounter = 0;
    #endif
    
    Time::Stamp registryCheckTimeMs = 0;
    
    while(client->mReceiveThread.QuitThreadFlag() == false)
    {
        // Make sure we are registered with the server.
        if(GetTimeMs() - registryCheckTimeMs > 250)
        {
            if(client->mRegistry.IsOpen())
            {
                client->mRegistry.Register(client->mID);
            }
            else if(client->mRegistry.OpenRegistry(client->mServerID))
            {
                ID id = client->mID;
                if(client->mID == AnyID)
                {         
                    id = 1;
                    while(client->mRegistry.IsRegistered(id))
                    {
                        id++;
                        // Don't go forever.
                        if(id - client->mID >= 10000)
                        {
                            break;
                        }
                        //SleepMs(1);
                    }
                }
                if(client->mMessageBox.CreateMessageBox(id, client->mDesiredBoxSize))
                {
                    client->mRegistry.Register(id);
                    client->mID = id;
                }
            }
            registryCheckTimeMs = GetTimeMs();
        }
        
        // Try read a message from the in-box.
        if(client->mMessageBox.ReadMessage(client->mTempPacket))
        {
            // Add to queue, then run callbacks.
            client->mReceivedDataMutex.Lock();
            client->mReceivedMessages.push_back(client->mTempPacket);
            if(client->mReceivedMessages.size() > gMaxQueueSize)
            {
                client->mReceivedMessages.erase(client->mReceivedMessages.begin());
            }
            client->mReceivedDataMutex.Unlock();
            
            // Now run callbacks if we have them.
            client->mCallbacksMutex.Lock();
            
            std::set<Callback*>::iterator cb;
            for(cb = client->mCallbacks.begin();
                cb != client->mCallbacks.end();
                cb++)
            {
                (*cb)->ProcessMessage(client->mTempPacket);
            }
            std::vector<Callback::Function>::iterator cbf;
            for(cbf = client->mFunctionCallbacks.begin();
                cbf != client->mFunctionCallbacks.end();
                cbf++)
            {
                cbf->Run(client->mTempPacket);
            }
            
            client->mCallbacksMutex.Unlock();
        }
        client->mDelayMutex.Lock();
        if(client->mUpdateDelayMs == 0)
        {
            #ifdef WIN32
            if(loopCounter++ == 250)
            {
                loopCounter = 0;
                SleepMs(1);
            }
            #else
            usleep(500);
            #endif
        }
        else
        {
            SleepMs(client->mUpdateDelayMs);
        }
        client->mDelayMutex.Unlock();
    }
    
    // Unregister.
    client->mRegistry.Unregister(client->mID);
}

/*  End of File */
