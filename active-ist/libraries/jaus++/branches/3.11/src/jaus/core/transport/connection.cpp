////////////////////////////////////////////////////////////////////////////////////
///
///  \file connection.cpp
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
#include "jaus/core/transport/connection.h"
#include "jaus/core/transport/nodemanager.h"
#include <tinyxml/tinyxml.h>
#include <boost/thread.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time.hpp>

using namespace JAUS;

unsigned int Connection::ConnectionCounter = 0;
const unsigned int Connection::Thread::MaxConnections = 1;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads any transport settings from the file.
///
///   \param[in] xmlSettingsFile Name of the file containing data to load.
///
////////////////////////////////////////////////////////////////////////////////////
bool Connection::Parameters::LoadSettings(const std::string& xmlSettingsFile)
{
    TiXmlDocument xml;

    if(xml.LoadFile(xmlSettingsFile.c_str()) == false)
    {
        return false;
    }
    TiXmlHandle doc(&xml);
    TiXmlNode* node;

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("DefaultPort").FirstChild().ToNode();
    if(node && node->Value() && atoi(node->Value()) >= 0)
    {
        mDestPortNumber = (UShort)atoi(node->Value());
    }

    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("NetAddress").FirstChild().ToNode();
    if(node && node->Value())
    {
        mNetworkInterface.SetAddress(node->Value());
    }
    node = doc.FirstChild("JAUS").FirstChild("Transport").FirstChild("MaxPacketSizeBytes").FirstChild().ToNode();
    if(node && node->Value())
    {
        mMaxPacketSizeBytes = (unsigned int)atoi(node->Value());
    }

    return true;
}


/** Constructor, initialize data. 
    \param[in] parent NodeManager pointer. 
    \param[in] shutdownFlag Pointer to global shutdown flag.
    \param[in] updateParent If true, calls UpdateServiceEvent method on
                            NodeManager pointer to update it. 
*/
Connection::Thread::Thread(NodeManager* parent, 
                           volatile bool* shutdownFlag, 
                           const bool updateParent) : mpParent(parent), mpShutdownFlag(shutdownFlag), mUpdateParentFlag(updateParent)
{
    SetThreadName("JAUS Node UpdateThread");
}

/** Destructor. */
Connection::Thread::~Thread()
{
    StopThread();
}


/** Constructor, initialize data. 
    \param[in] parent NodeManager pointer. 
    \param[in] shutdownFlag Pointer to global shutdown flag.
    \param[in] updateParent If true, calls UpdateServiceEvent method on
                            NodeManager pointer to update it. 
*/
void Connection::Thread::SetParent(NodeManager* parent, 
                                   volatile bool* shutdownFlag, 
                                   const bool updateParent)
{
    mpParent = parent;
    mpShutdownFlag = shutdownFlag;
    mUpdateParentFlag = updateParent;
}


/** Adds the connection to internal data members for updating. */
bool Connection::Thread::AddConnection(Connection::Ptr connection)
{
    bool result = false;
    unsigned int size = 0;

    {
        WriteLock wLock(mConnectionsMutex);

        size = (unsigned int)mConnections.size();
        if(size < MaxConnections ||
           (mpParent && size < mpParent->GetSettings()->GetConnectionsPerThread()))
        {
            mConnections.insert(connection);
            size = (unsigned int)mConnections.size();
            result = true;
        }
    }
    if(size > 0 && IsThreadActive() == false)
    {
        CreateThread();
    }
    return result;
}


/** Removes the connection from the object. */
void Connection::Thread::ShutdownConnections()
{
    // Shutdown connection first
    {
        ReadLock rLock(mConnectionsMutex);
        Connection::Set::iterator con;
        for(con = mConnections.begin();
            con != mConnections.end();
            con++)
        {
            (*con)->SignalGlobalShutdown(true);
            (*con)->ClearCallbacks();
            (*con)->Shutdown();
        }
    }
}


/** Removes the connection from the object. */
void Connection::Thread::RemoveConnection(Connection::Ptr connection)
{
    // Shutdown connection first
    {
        ReadLock rLock(mConnectionsMutex);
        Connection::Set::iterator con;
        if(connection == NULL)
        {
            for(con = mConnections.begin();
                con != mConnections.end();
                con++)
            {
                (*con)->Shutdown();
            }
        }
        else
        {
            con = mConnections.find(connection);
            if(con != mConnections.end())
            {
                (*con)->Shutdown();
            }
        }
    }
    {
        WriteLock wLock(mConnectionsMutex);

        if(connection == NULL)
        {
            mConnections.clear();
            return;
        }
        Connection::Set::iterator con = mConnections.find(connection);
        if(con != mConnections.end())
        {
            mConnections.erase(con);
        }
    }
}


/** Returns the number of connections being maintained. */
unsigned int Connection::Thread::GetNumConnections() const
{
    ReadLock rLock(*((SharedMutex *)&mConnectionsMutex));
    return (unsigned int)mConnections.size();
}


/** Updates the connections within a thread. */
void Connection::Thread::Execute()
{
    Connection::Set connections;
    Connection::Set::iterator con;
    bool haveConnections = true;

    std::vector<Connection*> bad;
    Time::Stamp prevUpdateParentTime = 0;

    while(QuitThreadFlag() == false && false == *mpShutdownFlag)
    {
        haveConnections = false;
        {
            {
                ReadLock rLock(mConnectionsMutex);
                connections = mConnections;
            }

            for(con = connections.begin();
                con != connections.end() && false == *mpShutdownFlag;
                con++)
            {
                haveConnections = true;

                if((*con)->IsConnected())
                {
                    (*con)->UpdateConnection();
                }
                else
                {
                    bad.push_back(con->get());
                }
            }
        }
        // Remove bad connections.
        if(bad.size() > 0)
        {
            {
                WriteLock wLock(mConnectionsMutex);
                
                for(unsigned int i = 0; i < (unsigned int)bad.size(); i++)
                {
                    con = mConnections.begin();
                    while(con != mConnections.end())
                    {
                        if( con->get() == bad[i] )
                        {
                            mConnections.erase(con);
                            break;
                        }
                        con++;
                    }
                }
            }
            bad.clear();
        }

        
        if(mUpdateParentFlag && mpParent && Time::GetUtcTimeMs() - prevUpdateParentTime >= 1000)
        {
            mpParent->UpdateServiceEvent();
            prevUpdateParentTime = Time::GetUtcTimeMs();
        }

        if(haveConnections == false)
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(10)); 
        }

        boost::this_thread::sleep(boost::posix_time::microseconds(1000)); 
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] singleThread If true, operate in single thread mode.
///
////////////////////////////////////////////////////////////////////////////////////
Connection::Connection(const bool singleThread) : mSingleThreadModeFlag(singleThread)
{
    {
        Mutex countLock("JAUS37941");
        Mutex::ScopedLock Lock(&countLock);
        mConnectionNumber = ConnectionCounter++;
    }
    mpManager = NULL;
    mpGlobalShutdownFlag = NULL;
    mTransportType = 0;
    mLocalConnectionFlag = false;
    mFixedConnectionFlag = false;
    mConnectionUpdateDelayMs = 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Connection::~Connection()
{
    mpGlobalShutdownFlag = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback method to receive data from the connection.
///
///   \param[in] pointer Pointer to you callback object.
///
////////////////////////////////////////////////////////////////////////////////////
void Connection::RegisterCallback(Connection::Callback* pointer)
{
    WriteLock wLock(mCallbacksMutex);
    mCallbacks.insert(pointer);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all callbacks (does note delete memory).
///
////////////////////////////////////////////////////////////////////////////////////
void Connection::ClearCallbacks()
{
    WriteLock wLock(mCallbacksMutex);
    mCallbacks.clear();
}


/** Copies callbacks, etc. common to the connections. */
void Connection::CopyConnectionData(Connection* connection)
{
    {
        ReadLock rLock(connection->mCallbacksMutex);
        mCallbacks = connection->mCallbacks;
    }
    {
        ReadLock rLock(mConnectionMutex);
        mConnectionUpdateDelayMs = connection->mConnectionUpdateDelayMs;
        mpGlobalShutdownFlag = connection->mpGlobalShutdownFlag;
        mpManager = connection->mpManager;
    }
}


/** Copies callbacks, etc. common to the connections. */
void Connection::SetNodeManager(NodeManager* nm)
{
    WriteLock wLock(mCallbacksMutex);
    mpManager = nm;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the packet data to all registered callbacks, and then
///          deletes the packet.
///
///   This method also updates the "update time" for the connection to
///   keep track of when data was last received.
///
///   \param[in] jausPacket JAUS Packet data without the transport header.
///   \param[in] jausHeader JAUS General Header information for the packet.
///   \param[in] sourceInfo Additional information about the source of the packet
///                         like the address it came in on.
///
////////////////////////////////////////////////////////////////////////////////////
bool Connection::SendToCallbacks(const Packet& jausPacket,
                                 const Header& jausHeader,
                                 const Info* sourceInfo)
{
    this->mUpdateTimeMs = Time::GetUtcTimeMs();

    bool result = false;
    ReadLock rLock(mCallbacksMutex);
    Connection::Callback::Set::iterator cb;
    for(cb = mCallbacks.begin();
        cb != mCallbacks.end();
        cb++)
    {
        result = true;
        (*cb)->ProcessPacket(jausPacket,
                             jausHeader,
                             this,
                             sourceInfo);
    }
    return result;
}


/** Thread function which calls all connection updates when running
    in multi-threaded mode. */
void Connection::UpdateConnectionThread(void* args)
{
    Connection* connection = (Connection *)args;

    if(connection->IsSingleThreadMode())
    {
        return;
    }

    while(connection &&
          connection->mUpdateConnectionThread.QuitThreadFlag() == false)
    {
        // Update connection.

        connection->UpdateConnection();

        if(connection->GetGlobalShutdownSignal() == true || connection->IsConnected() == false)
        {
            // Connection has closed, exit thread.
            break;
        }

        // Sleep!
        if(connection->mConnectionUpdateDelayMs == 0)
        {
            boost::this_thread::sleep(boost::posix_time::microseconds(1000));
        }
        else
        {
            boost::this_thread::sleep(boost::posix_time::milliseconds(connection->mConnectionUpdateDelayMs)); 
        }
    }
}



/*  End of File */

