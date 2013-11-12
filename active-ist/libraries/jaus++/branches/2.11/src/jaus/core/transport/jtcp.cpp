////////////////////////////////////////////////////////////////////////////////////
///
///  \file jtcp.cpp
///  \brief Handles a single JTCP connection to the host application.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 October 2010
///  <br>Copyright (c) 2010
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
#include "jaus/core/transport/jtcp.h"
#include "jaus/core/transport/transportmanager.h"

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
JTCP::JTCP() : mDelayTimeMs(0), 
               mpSocket(0),
               mpManager(0),
               mReceivedValidDataFlag(false),
               mLocalConnectionFlag(true)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
JTCP::~JTCP()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the object using an existing socket and pointer
///          to transport manager for sharing data.
///
///   \param[in] manager Pointer to manager which will process received message
///                      data.
///   \param[in] socket Pointer to a TCPServer or TCPClient socket.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool JTCP::Initialize(TransportManager* manager,
                      CxUtils::Socket* socket)
{
    Shutdown();
    if(manager && 
       socket && 
       socket->IsValid() && 
       (socket->GetType() == CxUtils::Socket::TCPServer || socket->GetType() == CxUtils::Socket::TCPClient))
    {
        mpManager = manager;
        mpSocket = socket;
        mUpdateTime.SetCurrentTime();
        if(socket->GetType() == CxUtils::Socket::TCPServer)
        {
            CxUtils::TcpServer* tcp = (CxUtils::TcpServer*)socket;
            if(socket->IsHostAddress(tcp->GetClientAddress()))
            {
                mLocalConnectionFlag = true;
            }
            else
            {
                mLocalConnectionFlag = false;
            }
        }
        if(socket->GetType() == CxUtils::Socket::TCPClient)
        {
            CxUtils::TcpClient* tcp = (CxUtils::TcpClient*)socket;
            if(socket->IsHostAddress(tcp->GetServerAddress()))
            {
                mLocalConnectionFlag = true;
            }
            else
            {
                mLocalConnectionFlag = false;
            }
        }
        return mRecvThread.CreateThread(JTCP::ReceiveThread, this) > 0 ? true : false;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the socket and receiving thread.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCP::Shutdown()
{
    mRecvThread.StopThread(1000);

    if(mpSocket)
    {
        mpSocket->Shutdown();
        delete mpSocket;
    }
    /*
    if(mpSocket)
    {

    }
    */
    mpSocket = NULL;
    mpManager = NULL;
    mUpdateTime.Clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the delay time between checks for new packets to process by
///          each message processing thread.  
///
///   The default value is 1 ms, but can be set to 0 for no delay, or larger
///   for less frequent checking of messages (use larger numbers on slower 
///   hardware).
///
///   \param[in] delayTimeMs Delay time between checks in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCP::SetPacketPollingDelayMs(const unsigned int delayTimeMs)
{
    if(mRecvThread.IsThreadActive() == false)
    {
        mDelayTimeMs = delayTimeMs;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the ID of the source of the last message received by the
///          TCP connection.
///
////////////////////////////////////////////////////////////////////////////////////
Address JTCP::GetLastSourceID() const
{
    Mutex::ScopedLock lock(&mDataMutex);
    return mLastSourceID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the last time data was received from the socket.
///
////////////////////////////////////////////////////////////////////////////////////
Time JTCP::GetUpdateTime() const
{
    Mutex::ScopedLock lock(&mDataMutex);
    return mUpdateTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called whenever a JTCP stream packet has been received.
///
///   This implementation attempts to pass the data to the Transport Manager.
///
///   \param[in] packet JTCP packet with JTCP header, JAUS general header, and
///                     payload data.
///   \param[in] header Extracted JAUS general header.
///   \param[in] ipAddress IP Address of data source.
///   \param[in] sourcePort Source port the data came from on the stream.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCP::ProcessPacket(Packet& packet,
                         Header& header,
                         const IP4Address& ipAddress,
                         const unsigned short sourcePort)
{
    if(mpManager)
    {
        mpManager->ProcessPacket(packet, header, ipAddress, sourcePort, TransportManager::TCP);
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread for receiving and processing TCP data received.
///
////////////////////////////////////////////////////////////////////////////////////
void JTCP::ReceiveThread(void* args)
{
    JTCP* transport = (JTCP*)args;
    Packet smallBuffer;
    Packet largeBuffer;
    long int timeoutMs = 100;

    largeBuffer.Reserve(4*(JAUS::Header::MaxPacketSize + OverheadSizeBytes));
    smallBuffer.Reserve(JAUS::Header::MaxPacketSize + OverheadSizeBytes);

    CxUtils::SleepMs(500);

#ifdef WIN32
    int loopCounter = 0;
#endif
    unsigned int failureCount = 0;
    while(transport &&
          transport->mRecvThread.QuitThreadFlag() == false &&
          transport->ExitThread() == false)
    {
        // Detect shutdown of socket from server.
        if(transport->mpSocket->IsValid() == false || failureCount == 10)
        {
            //  Socket closed on server side, exit.
            smallBuffer.Destroy();
            largeBuffer.Destroy();
            transport->mpSocket->Shutdown();
            break;
        }
        // Try receive some data.
        smallBuffer.Clear();
        Time::Stamp startTimeMs = Time::GetUtcTimeMs();
        IP4Address ipAddress;
        unsigned short sourcePort = 0;
        int rt = 0;
        if( (rt = transport->mpSocket->Recv(smallBuffer,
                                            5000,
                                            timeoutMs,
                                            &ipAddress,
                                            &sourcePort)) > 0)
        {
            failureCount = 0;
            largeBuffer += smallBuffer;

            // Try parse out a JAUS message.
            unsigned char* ptr = largeBuffer.Ptr();
            unsigned int pos = 0;
            unsigned int endPos = 0;
            while(largeBuffer.Length() >= JAUS::Header::MinSize && pos < largeBuffer.Length() - JAUS::Header::MinSize)
            {
                // Check for JTCP Header
                if(ptr[pos] == Version)
                {
                    // Try read message data.
                    JAUS::Header generalHeader;
                    Packet::Wrapper subPacket(&ptr[pos + sizeof(Version)], largeBuffer.Length() - pos);
                    std::string errorMessage;
                    int bytesRead = 0;
                    if( (bytesRead = generalHeader.Read(*subPacket.GetData())) > 0 &&
                        generalHeader.IsValid(&errorMessage) &&
                        subPacket->Length() >= generalHeader.mSize)
                    {
                        // Read header data, now pull payload data.
                        Packet::Wrapper finalPacket(&ptr[pos], 
                                                    (unsigned int)generalHeader.mSize + sizeof(Version));
                        // Process data.
                        transport->ProcessPacket(*finalPacket.GetData(), generalHeader, ipAddress, sourcePort);
                        {
                            Mutex::ScopedLock lock(&transport->mDataMutex);
                            transport->mUpdateTime.SetCurrentTime();
                            transport->mReceivedValidDataFlag = true;
                            transport->mLastSourceID = generalHeader.mSourceID;
                        }
                        // Advance position in stream buffer.
                        pos += finalPacket->Length();
                        endPos = pos;
                        continue;
                    }
                    if(errorMessage.empty() == false)
                    {
                        //std::cout << "JTCP::Error [Header] - " << errorMessage << std::endl;
                    }
                }
                pos++;
            }

            if(endPos > 0)
            {
                // Delete data we have read from the buffer.
                largeBuffer.Delete(endPos, 0);
            }

            if(largeBuffer.Size() >= largeBuffer.Reserved() - JAUS::Header::MinSize + 1)
            {
                // Getting bad data, flush out buffers.
                // Hopefully we never get to this point in code.
                std::cout << "JTCP::ERROR - Buffer Overflow.\n";
                largeBuffer.Clear();
                smallBuffer.Clear();
            }
        }
        else if(Time::GetUtcTimeMs() - startTimeMs <= (Time::Stamp)(timeoutMs/2.0))
        {
            failureCount++;
        }
        if(rt < 0)
        {
            failureCount = 10;
        }

        if(transport->mDelayTimeMs == 0)
        {
#ifdef WIN32
            // Only sleep every N loops
            if( loopCounter++ == 250)
            {
                loopCounter = 0;
                CxUtils::SleepMs(1);
            }
#else
            usleep(500);
#endif
        }
        else
        {
            CxUtils::SleepMs(transport->mDelayTimeMs);
        }
    }
}

/*  End of File */
