////////////////////////////////////////////////////////////////////////////////////
///
///  \file messageserver.h
///  \brief This file contains software for creating shared memory Message Server
///  that will distribute received messages to Message Clients.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 April 2009
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
#include "cxutils/ipc/messageserver.h"
#include <iostream>
#include <string.h>
#include <sstream>
#include <cstdio>

using namespace std;
using namespace CxUtils;

unsigned int gInactiveThreshMs = 250;

//#define __CX_DEBUG

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageServer::Registry::Registry()
{
    mServerFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageServer::Registry::~Registry()
{
    CloseRegistry();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Open a view of the registry.  Allows you to register/unregister
///  and get a view of all registered client ID's.
///
///  \param[in] registryID The ID to use for the registry.  In Windows this can
///             be a string of any kind.  This should also with Linux, but you
///             will have more reliable results using a number.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::OpenRegistry(const ID registryID)
{
    std::ostringstream input;
    input << registryID;
    return OpenRegistry(input.str());
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Open a view of the registry.  Allows you to register/unregister
///  and get a view of all registered client ID's.
///
///  \param[in] registryID The ID to use for the registry.  In Windows this can
///             be a string of any kind.  This should also with Linux, but you
///             will have more reliable results using a number.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::OpenRegistry(const std::string& registryID)
{
    int result = 0;
    CloseRegistry();

    if(mRegistry.OpenMappedMemory(registryID))
    {
        mRegistry.Lock();
        // Check for valid header.
        if(memcmp(mRegistry->Ptr(), "REGISTRY", 8) == 0)
        {
            result = 1;
        }
        mRegistry.Unlock();
    }
    if(result == 0)
    {
        mRegistry.CloseMappedMemory();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Closes the view of the registry.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::CloseRegistry()
{
    List registry;
    if(mRegistry.IsOpen())
    {
        std::string name;
        name = mRegistry.GetMappedName();
        mRegistry.Lock();

        // Make sure mapped memory is deleted.
        unsigned int count = 0;
        // Get the size of the registry.
        mRegistry->SetReadPos(CountPos);
        mRegistry->Read(count);
        if(count == 0 && mServerFlag == true)
        {
            // We are the last to close a view of
            // this, so flush contents.
            mRegistry->SetWritePos(0);
            mRegistry->Write( std::string("REGISTRY") );
            mRegistry->Write( ( (unsigned int)0 ) );
            mRegistry->Write( ( (unsigned int)0 ) );
        }

        mRegistry.Unlock();
        mRegistry.CloseMappedMemory();
    }

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds the ID to the registry.
///
///  \param[in] id The client ID to register.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::Register(const ID id)
{
    int result = 0;
    if(mRegistry.IsOpen())
    {
        unsigned int count = 0;
        ID temp = 0;
        mRegistry.Lock();

        // Get the contents of the registry.
        mRegistry->SetReadPos(CountPos);
        mRegistry->Read(count);
        for(unsigned int i = 0; i < count; i++)
        {
            mRegistry->Read(temp);
            if(temp == id)
            {
                result = 1;
                break;
            }
        }
        if(result == 0)
        {
            mRegistry->SetWritePos(HeaderSize + count*sizeof(ID));
            if(mRegistry->Write(id))
            {
                count++;
                mRegistry->SetWritePos(CountPos);
                mRegistry->Write(count);
                result = 1;
            }
        }
        mRegistry.Unlock();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Removes the ID from the registry.
///
///  \param[in] id The client ID to unregister.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::Unregister(const ID id)
{
    int result = 0;
    if(mRegistry.IsOpen())
    {
        unsigned int count = 0;
        ID temp = 0;
        mRegistry.Lock();

        // Get the contents of the registry.
        mRegistry->SetReadPos(CountPos);
        mRegistry->Read(count);
        for(unsigned int i = 0; i < count; i++)
        {
            mRegistry->Read(temp);
            if(temp == id)
            {
                result = 1;
                count--;
                memmove(mRegistry->Ptr() + HeaderSize + i*sizeof(ID),
                        mRegistry->Ptr() + HeaderSize + (i+1)*sizeof(ID),
                        count*sizeof(ID));
                break;
            }
        }
        if(result == 1)
        {
            mRegistry->SetWritePos(CountPos);
            mRegistry->Write(count);
        }
        mRegistry.Unlock();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets a copy of all the client ID's in the registry.
///
///  \param[in] registry List of all items in the Registry.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::GetRegistry(MessageServer::List& registry)
{
    int result = 0;
    registry.clear();

    if(mRegistry.IsOpen())
    {
        result = 1;
        unsigned int count = 0;
        ID temp = 0;
        mRegistry.Lock();

        // Get the contents of the registry.
        mRegistry->SetReadPos(CountPos);
        mRegistry->Read(count);
        for(unsigned int i = 0; i < count; i++)
        {
            mRegistry->Read(temp);
            registry.insert(temp);
        }

        mRegistry.Unlock();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks to see if an ID is in the registry.
///
///  \param[in] id The client ID to check for.
///
///  \return True if registered, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::Registry::IsRegistered(const ID id) const
{
    bool result = false;
    if(mRegistry.IsOpen())
    {
        unsigned int count = 0;
        ID temp = 0;
        mRegistry.Lock();

        // Get the contents of the registry.
        mRegistry->SetReadPos(CountPos);
        mRegistry->Read(count);
        for(unsigned int i = 0; i < count; i++)
        {
            mRegistry->Read(temp);
            if(temp == id)
            {
                result = true;
                break;
            }
        }
        mRegistry.Unlock();
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Check to see if a server is actively checking the server for clients.
///
///  \param[in] thresholdMs How long of a threshold to determine if a server
///                         is actively checking the registry.  For example, if
///                         this value is 500, then the method will return false
///                         if no server has checked the registry in more than
///                         500 ms.
///
///  \return True if registry is being checked by a server, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::Registry::IsActive(const unsigned int thresholdMs) const
{
    bool result = false;

    if(mRegistry.IsOpen())
    {
        mRegistry.Lock();
        // Check to see if the registry is being used
        // by a server.
        mRegistry->SetReadPos(TimePos);
        Time::Stamp updateTimeMs = 0;
        if(mRegistry->Read(updateTimeMs))
        {
            if((unsigned int)(GetTimeMs() - updateTimeMs) <= thresholdMs)
            {
                result = true;
            }
        }
        mRegistry.Unlock();
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a registry.  This option is only available to MessageServer.
///
///  \param[in] registryID The ID to use for the registry.  In Windows this can
///             be a string of any kind.  This should also with Linux, but you
///             will have more reliable results using a number.
///  \param[in] maxClients Maximum number of clients.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::CreateRegistry(const ID registryID,
                                            const unsigned int maxClients)
{
    std::ostringstream input;
    input << registryID;
    return CreateRegistry(input.str(), maxClients);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates a registry.  This option is only available to MessageServer.
///
///  \param[in] registryID The ID to use for the registry.  In Windows this can
///             be a string of any kind.  This should also with Linux, but you
///             will have more reliable results using a number.
///  \param[in] maxClients Maximum number of clients.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Registry::CreateRegistry(const std::string& registryID,
                                            const unsigned int maxClients)
{
    int result = 0;
    unsigned int size = maxClients*sizeof(ID) + HeaderSize;

    CloseRegistry();

    if(mRegistry.OpenMappedMemory(registryID))
    {
        mRegistry.Lock();
        // Check for valid header.
        if(memcmp(mRegistry->Ptr(), "REGISTRY", 8) == 0)
        {
            // Check to see if the registry is already being used
            // by another server.
            mRegistry->SetReadPos(TimePos);
            Time::Stamp updateTimeMs = 0;
            if(mRegistry->Read(updateTimeMs))
            {
                if(GetTimeMs() - updateTimeMs > 500)
                {
                    result = 1;
                    mRegistry->SetWritePos(TimePos);
                    // Set a new write time.
                    mRegistry->Write( GetTimeMs() );
                }
            }
        }
        else
        {
            mRegistry->SetWritePos(0);
            mRegistry->Write( std::string("REGISTRY") );
            mRegistry->Write( GetTimeMs() );
            result = 1;
        }
        mRegistry.Unlock();
    }
    else if(mRegistry.CreateMappedMemory(registryID, size))
    {
        mRegistry.Lock();
        mRegistry->SetWritePos(0);
        mRegistry->Write( std::string("REGISTRY") );
        mRegistry->Write( GetTimeMs() );
        mRegistry->Write( ( (unsigned int)0 ) );
        mRegistry.Unlock();
        result = 1;
    }

    if(result == 0)
    {
        mRegistry.CloseMappedMemory();
    }
    else
    {
        mServerFlag = true;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Updates the last update/check time for the registry owner.
///
///  \param[in] timestampMs The timestamp to write to the header.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::Registry::Touch(const Time::Stamp timestampMs)
{
    if(mRegistry.IsOpen())
    {
        mRegistry.Lock();
        mRegistry->SetWritePos(TimePos);
        // Set a new write time.
        mRegistry->Write( timestampMs );
        mRegistry.Unlock();
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageServer::MessageServer()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MessageServer::~MessageServer()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the MessageServer.
///
///  \param[in] serverID Name of server to use (all clients must use this
///                      name to connect).  For maximum portability between
///                      Windows and Linux, use an integer (in Windows you can
///                      use any string).
///  \param[in] maxClients Maximum number of clients to allow connections to
///                        server (default is 25).
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Initialize(const ID serverID,
                              const unsigned int maxClients)
{
    std::ostringstream input;
    input << serverID;
    return Initialize(input.str(), maxClients);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the MessageServer.
///
///  \param[in] serverID Name of server to use (all clients must use this
///                      name to connect).  For maximum portability between
///                      Windows and Linux, use an integer (in Windows you can
///                      use any string).
///  \param[in] maxClients Maximum number of clients to allow connections to
///                        server (default is 25).
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageServer::Initialize(const std::string& serverID,
                              const unsigned int maxClients)
{
    int result = 0;

    // Close if already running.
    Shutdown();

    if(mClientRegistry.CreateRegistry(serverID, maxClients))
    {
        if(mClientDiscoveryThread.CreateThread(MessageServer::ClientDiscoveryThread, this))
        {
            result = 1;
        }
        else
        {
            cout << "MessageServer::Initialize - Failed to create thread.\n";
        }
    }
    else
    {
#ifdef __CX_DEBUG
        cout << "MessageServer::Initialize - Failed to create registry for clients.\n";
        cout << "Another server may be running, or a previouis instance of an application\nrunning a server didn't close properly (delete memory or reboot system to force release).\n";
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
///  \brief Stops the server and releases all connections to clients and
///         registry.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageServer::Shutdown()
{
    mClientDiscoveryThread.StopThread();

    if(mClientRegistry.IsOpen())
    {
        // Write a 0 value so another server
        // can claim ownership.
        mClientRegistry.Touch(0);
        mClientRegistry.CloseRegistry();
    }
    // Delete allocated memory.
    Clients::iterator client;
    for(client = mClients.begin();
        client != mClients.end();
        client++)
    {
        client->second->CloseMessageBox();
        delete client->second;
    }
    mClients.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Optional method to force a connection attempt to a specific ID number.
///
///  The MessageServer class will automatically find and connect to MessageClients
///  running on the host machine, however if it has not detected the connection
///  yet, it is possible to force one using this interface.
///
///  \param[in] id The ID of the MessageClient to try create a connection to.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::CreateConnection(const ID id)
{
    bool result = false;
    MappedMessageBox* client = new MappedMessageBox();

    if(client->OpenMessageBox(id))
    {
        mClientRegistry.Register(id);
        Clients::iterator c;
        mClientsMutex.Lock();
        c = mClients.find(id);
        if(c != mClients.end())
        {
            c->second->CloseMessageBox();
            delete c->second;
            mClients.erase(c);
        }
        mClients[id] = client;
        client = NULL;
        mClientsMutex.Unlock();
        result = true;
    }

    if(result == false && client)
    {
        delete client;
        client = NULL;
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a message to a specific subscribing client ID.
///
///  \param[in] id The ID of the client to send the message to.
///  \param[in] message The message data to send.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::SendToClient(const ID id, const Packet& message)
{
    bool result = false;
    Clients::iterator client;
    mClientsMutex.Lock();
    client = mClients.find(id);
    if(client != mClients.end())
    {
        result = client->second->WriteMessage(message);
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a message to a specific subscribing client ID.
///
///  \param[in] id The ID of the client to send the message to.
///  \param[in] message The message data to send.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::SendToClient(const ID id, const std::string& message)
{
    bool result = false;
    Clients::iterator client;
    mClientsMutex.Lock();
    client = mClients.find(id);
    if(client != mClients.end())
    {
        result = client->second->WriteMessage(message);
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a message to all subscribing clients.
///
///  \param[in] message The message data to send.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::SendToAllClients(const Packet& message)
{
    bool result = false;
    Clients::iterator client;
    mClientsMutex.Lock();
    for(client = mClients.begin();
        client != mClients.end();
        client++)
    {
        result = client->second->WriteMessage(message);
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a message to all subscribing clients.
///
///  \param[in] message The message data to send.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::SendToAllClients(const std::string& message)
{
    bool result = false;
    Clients::iterator client;
    mClientsMutex.Lock();
    for(client = mClients.begin();
        client != mClients.end();
        client++)
    {
        result = client->second->WriteMessage(message);
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Get a list of all connected client IDs.
///
///  \param[in] clients List of active client connections.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::GetClientList(List& clients) const
{
    bool result = false;
    Clients::const_iterator client;
    clients.clear();
    mClientsMutex.Lock();
    for(client = mClients.begin();
        client != mClients.end();
        client++)
    {
        result = true;
        clients.insert(client->first);
    }
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Number of active client connections.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int MessageServer::GetNumClients() const
{
    unsigned int result = 0;
    mClientsMutex.Lock();
    result = (unsigned int)mClients.size();
    mClientsMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the server is initialized and running, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::IsInitialized() const
{
    if(mClientRegistry.IsOpen() && mClientDiscoveryThread.IsThreadActive())
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method used to check if a server is running on the host machine.
///
///  \param[in] serverID The name/ID of the server to check for.
///
///  \return True if the server is active and running, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool MessageServer::IsServerPresent(const std::string& serverID)
{
    bool result = false;
    MappedMemory registry;
    if(registry.OpenMappedMemory(serverID))
    {
        registry.Lock();
        // Check for valid header.
        if(memcmp(registry->Ptr(), "REGISTRY", 8) == 0)
        {
            // Check the timestamp to see if it is being written to.
            registry->SetReadPos(Registry::TimePos);
            Time::Stamp updateTimeMs = 0;
            if(registry->Read(updateTimeMs))
            {
                if(GetTimeMs() - updateTimeMs <= 500)
                {
                    result = true;
                }
            }
        }
        registry.Unlock();
    }

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief This thread checks the Registry for client connections/disconnects
///         and deletes inactive client connections.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageServer::ClientDiscoveryThread(void* args)
{
    MessageServer* server = (MessageServer *)args;
    MessageServer::List registry;
    MessageServer::List myRegistry;
    MessageServer::List::iterator client;
    Clients::iterator connection;

    while(server->mClientDiscoveryThread.QuitThreadFlag() == false)
    {
        // Get a copy of the current registry.
        if(server->mClientRegistry.IsOpen() && server->mClientRegistry.GetRegistry(registry))
        {
            // Create new client connections.
            for(client = registry.begin();
                client != registry.end();
                client++)
            {
                // Check for new connection.
                if(myRegistry.find( *client ) == myRegistry.end())
                {
                    server->mClientsMutex.Lock();
                    connection = server->mClients.find(*client);
                    if(connection != server->mClients.end())
                    {
                        // Have a connection already.
                        myRegistry.insert(*client);
                    }
                    else
                    {
                        // Try create a new connection.
                        MappedMessageBox* newConnection = new MappedMessageBox();
                        char name[256];
                        sprintf(name, "%llu", *client);
                        if(newConnection->OpenMessageBox(name))
                        {
                            myRegistry.insert(*client);
                            server->mClients[*client] = newConnection;
                        }
                        else
                        {
                            delete newConnection;
                            // Remove from registry since we cannot connect
                            // to them (probably an inactive block of shared memory).
                            server->mClientRegistry.Unregister(*client);
                        }
                    }
                    server->mClientsMutex.Unlock();
                }
            }
            // Check for disconnect event.
            client = myRegistry.begin();
            while(client != myRegistry.end())
            {
                // If not found in current registry, then we
                // need to close our connection.
                if(registry.find(*client) == registry.end())
                {
                    server->mClientsMutex.Lock();
                    connection = server->mClients.find(*client);
                    if(connection != server->mClients.end())
                    {
                        connection->second->CloseMessageBox();
                        delete connection->second;
                        server->mClients.erase(connection);
                    }
                    server->mClientsMutex.Unlock();
                    // Remove from current list.
                    myRegistry.erase(client);
                    client = myRegistry.begin();
                }
                else
                {
                    client++;
                }
            }
        }

        // Check for in-active client connections and delete them.
        server->mClientsMutex.Lock();
        client = myRegistry.begin();
        while(client != myRegistry.end())
        {
            connection = server->mClients.find( *client );
            if(connection == server->mClients.end())
            {
                myRegistry.erase(client);
                client = myRegistry.begin();
            }
            else
            {
                client++;
            }
        }
        connection = server->mClients.begin();
        while(connection != server->mClients.end())
        {
            if(connection->second->IsActive(true, gInactiveThreshMs) == false)
            {
                connection->second->CloseMessageBox();
                delete connection->second;
                server->mClients.erase(connection);
                connection = server->mClients.begin();
            }
            else
            {
                connection++;
            }
        }
        server->mClientsMutex.Unlock();

        // Indicate that we are reading the registry by
        // updating a time stamp in it that only the server
        // can access.
        server->mClientRegistry.Touch(GetTimeMs());

        SleepMs(100);
    }
}

/*  End of File */
