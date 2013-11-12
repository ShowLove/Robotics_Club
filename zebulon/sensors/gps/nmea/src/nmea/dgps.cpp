/////////////////////////////////////////////////////////////////////////////////////
///
/// \file dgps.cpp
/// \brief A simple GPS class capable of receiving data from any NMEA
/// compliant GPS using a serial connection.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/28/2008<br>
/// Last Modified: 2/29/2008<br>
/// Copyright (c) 2007<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
/////////////////////////////////////////////////////////////////////////////////////
#include "nmea/dgps.h"
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include <tinyxml/tinyxml.h>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace GPS;
using namespace NMEA;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
DGPS::DGPS() :
               mHaveSignalFlag(false),
               mRunningFlag(false)
{
    mListSize = 10;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
DGPS::~DGPS()
{
    Shutdown();
    mMessages.clear();
    mCallbacks.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initialize the DGPS interface using an XML settings file.
///
///  This method loads an XML file with port, baud, timeout, and message type
///  information and then tries to initialize the connection to the device.
///
///  See the mini_max.xml example file in the executeable directory.
///
///  \param xml Path and name of XML file with configuration for interface.
///
///  \return 1 on successful connection, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int DGPS::Initialize(const std::string& xml)
{
    int result = 0;
    TiXmlDocument file;

    if(file.LoadFile(xml.c_str()))
    {
        TiXmlHandle doc(&file);
        TiXmlNode* node = doc.FirstChild("Zebulon").FirstChild("DGPS").ToNode();
        if(node)
        {
            if(node->FirstChild("Port") &&
               node->FirstChild("Baud"))
            {
                std::string port = node->FirstChild("Port")->FirstChild()->Value();
                unsigned int baud = atoi(node->FirstChild("Baud")->FirstChild()->Value());
                unsigned int timeout = 1000;
                if(node->FirstChild("TimeOut"))
                {
                    timeout = atoi(node->FirstChild("TimeOut")->FirstChild()->Value());
                }
                if(node->FirstChild("MessageTypes"))
                {
                    TiXmlNode* types = node->FirstChild("MessageTypes")->FirstChild("Type");
                    DGPS::TypeSet tset;
                    while(types)
                    {
                        tset.insert( NMEA::MessageFactory::GetType(types->FirstChild()->Value()) );
                        types = types->NextSibling();
                    }
                    // Only erase current message set if
                    // we were able to read data from XML
                    if(tset.size())
                    {
                        SetMessageTypes(tset);
                    }
                }

                // Now try to initialize DGPS
                //broken unless further notice -gary
                //result = Initialize(port, baud, false, timeout);
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initialize receiving of data from an NMEA compatible GPS over a 
///         serial connection.
///
///  This method connects to a serial port at the desired initial baud.  It then
///  waits up to timeout time to try and receive valid NMEA formatted data. If
///  it is not able to get data from the device at the initial baudrate (ibaud)
///  it will try again start at a baud of 9600, up to 115200.
///
///  \param port The serial port to connect to.
///  \param ibaud The initial baud to try connect on.
///  \param tryOtherBauds If true, method will try to connect on other BAUD
///                       rates.  If false, will only use ibaud.
///  \param timeout How long to wait for discovery of valid incomming NMEA
///                 messages.
///
///  \return 1 on successful connection, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
#if 0
int DGPS::Initialize(const std::string& port,
                     const unsigned int ibaud,
                     const bool tryOtherBauds,
                     const unsigned int timeout)
{
    Shutdown();
    int result = 0;
    unsigned int baud = 9600;
    unsigned int start = 0;
    
    // Start the serial thread which will
    // be used to receive all incomming messages.
    if(mSerialThread.CreateThread(&DGPS::SerialThread, this))
    {
#ifdef _DEBUG
        std::cout << "Establishing connection to DGPS device on " << port << "...\n";
        std::cout << "Trying baud " << ibaud << "...";
#endif
        //  Now connect at the initial baud rate
        if(mSerial.Connect(port.c_str(), ibaud))
        {
            // Wait and see if we received any messages.
            start = CxUtils::GetTimeMs();
            while(CxUtils::GetTimeMs() - start < timeout && result == 0)
            {
                if(mValidDataFlag)
                {
#ifdef _DEBUG
                    std::cout << "Success!\n";
#endif
                    result = 1;
                }
                CxUtils::SleepMs(1);
            }

            if( result == 0 )
            {
                mSerial.Flush();
                mSerial.Shutdown();
#ifdef _DEBUG
                std::cout << "Failure.\n"; 
#endif
                while(tryOtherBauds && baud <= 115200 && result == 0)
                {
                    // Don't retry the default baud
                    if(baud != ibaud)
                    {
#ifdef _DEBUG
                        std::cout << "Trying baud " << baud << "...";
#endif
                        if(mSerial.Connect(port.c_str(), baud))
                        {
                            // Wait and see if we received any messages.
                            start = CxUtils::GetTimeMs();
                            while( CxUtils::GetTimeMs() - start < timeout && result == 0)
                            {
                                if(mValidDataFlag)
                                {
#ifdef _DEBUG
                                    std::cout << "Success!\n";
#endif
                                    result = 1;
                                }
                                CxUtils::SleepMs(1);
                            }
                        }
                        if(result == 0)
                        {
#ifdef _DEBUG
                            std::cout << "Failure.\n"; 
#endif
                            // If no connection made, flush and
                            // shutdown the connection.
                            mSerial.Flush();
                            mSerial.Shutdown();
                        }
                    }

                    //  Select a different baudrate.
                    switch(baud)
                    {
                    case 9600:
                        baud = 19200;
                        break;
                    case 19200:
                        baud = 38400;
                        break;
                    case 38400:
                        baud = 57600;
                        break;
                    case 57600:
                        baud = 115200;
                        break;
                    default:
                        baud = 115201;
                        break;
                    }// Switch to different Baud.
                }// While not connected and baud <= 115200.
            }// If not connected yet.
        }// If connection to serial port was made.
        else
        {
            std::cout << "Failure. Could not connect to " << port << "\n";
        }
    }// If thread was started.

    if(result == 0)
    {
        // Flush everything and shutdown.
        Shutdown();
    }
    return result;
}
#endif

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown receiving of data from the device, and clears any
///         messages in the queue.  This method does not clear registered
///         callbacks.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
void DGPS::Shutdown()
{
    GenericGPS::Shutdown();
    mRunningFlag = false;
    mMutex.Lock();
    MessageList::iterator msg;
    for(msg = mMessages.begin();
        msg != mMessages.end();
        msg++)
    {
        delete (*msg);
    }
    mMessages.clear();
    mMutex.Unlock();
 }


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Set a list of the types of messages to receive (all others are 
///         ignored).
///
///  \param types Set of types to listen for.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int DGPS::SetMessageTypes(const DGPS::TypeSet types)
{
    if(types.size() > 0)
    {
        mMutex.Lock();
        mMessageTypes = types;
        mMutex.Unlock();
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Add a message type to listen for.
///
///  \param type Type of message to listen for.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int DGPS::AddMessageType(const NMEA::Message::Type type)
{
    mMutex.Lock();
    mMessageTypes.insert(type);
    mMutex.Unlock();
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Remove a type from the set of types to listen for.
///
///  \param type The message type to remove/ignore from incomming data stream.
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int DGPS::RemoveMessageType(const NMEA::Message::Type type)
{
    int result = 0;
    mMutex.Lock();
    if( mMessageTypes.find(type) != mMessageTypes.end())
    {
        mMessageTypes.erase(mMessageTypes.find(type));
        result = 1;
    }
    mMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the maximum number of received messages to store in an internal
///         message queue.
///
///  \param size Size of the queue [1, MAX_UINT].
///
///  \return 1 on success, 0 on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int DGPS::SetMaximumListSize(const unsigned int size)
{
    if(size >= 1)
    {
        mListSize = size;
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the last received message in the queue.
///
///  \return Pointer to a copy of the last message received (you must delete it), 
///          and NULL if no messages in queue.
///
////////////////////////////////////////////////////////////////////////////////////
NMEA::Message* DGPS::GetLastMessage() const
{
    NMEA::Message* msg = NULL;
    mMutex.Lock();
    if(mMessages.size() > 0)
    {
        msg = mMessages.back()->Clone();
    }
    mMutex.Unlock();

    return msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets copies of the last "num" messages from message queue.
///
///  \param num The number of messages (starting from the latest) to put in
///             the vector returned.
///
///  \return Vector of messages from queue (you must delete them).
///
////////////////////////////////////////////////////////////////////////////////////
DGPS::MessageVector DGPS::GetMessages(const unsigned int num) const
{
    DGPS::MessageVector vect;
    unsigned int count = 0;
    DGPS::MessageList::const_reverse_iterator itr;
    
    mMutex.Lock();
    itr = mMessages.rbegin();
    while(itr != mMessages.rend() && count < num)
    {
        vect.push_back( (*itr)->Clone() );
        count++;
        itr++;
    }

    mMutex.Unlock();

    return vect;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Main operating thread for the class.  This method continuously 
///         recieves data from the connected serial port, parses the stream,
///         and puts the read NMEA messages in a queue.
///
///         This method also sets flags for validity of data coming in.  This is
///         done by checking UTC timestamps on supported messages, and if they
///         are not updating, then this indicates loss of signal, and the 
///         lost signal flag is set.
///
////////////////////////////////////////////////////////////////////////////////////
void DGPS::ProcessingFunction()
{
    CxUtils::Packet buffer, packet;
    CxUtils::Time::Stamp tstamp;
    unsigned int day = 0;
    unsigned int failCount = 0;
    double prevUTC, curUTC;
    std::string header;
    int spos;                   // position of last '$' found in buffer.
    unsigned int recvSize = 50; // Number of bytes to receive at a time.
    unsigned char* ptr = NULL;  // Pointer to buffered data received.
    unsigned int cpos = 0;      // current position variable.
    
    prevUTC = curUTC = 0;
    // Reserve some memory for receiving data.
    buffer.Reserve(512);
    packet.Reserve(512);
    // Loop until quit detected.
    while(!mSerialThread.QuitThreadFlag())
    {
        mRunningFlag = true;
        if(mSerial.IsConnected())
        {
            // Receive data from serial port. (receive max packet size because
            // these messages contains carriage returns and line feeds which will
            // break the receive at the end of the message.
            if(mSerial.Recv(packet, recvSize) > 0)
            {
                failCount = 0; // Got data, clear failure count.

                // Add data to the end of our buffer
                buffer += packet;
                // Now search the buffer for any NMEA messages.
                spos = -1;
                ptr = (unsigned char *)buffer.Ptr();
                for(cpos = 0; cpos < buffer.Length();)
                {
                    // Check for start of a 
                    if( (ptr[cpos]) == '$' )
                    {
                        // Mark position of message header discovery.
                        spos = (int)cpos;
                        // Try an pull out an NMEA formatted message from the buffer.
                        if( NMEA::Message::ExtractMessage(&ptr[cpos], 
                                                          buffer.Length() - cpos, 
                                                          packet,
                                                          &header) )
                        {
                            cpos += packet.Length();
                            NMEA::Message* msg = NMEA::MessageFactory::CreateMessage(header);
                            if(msg && msg->Read(packet))
                            {
                                // Write message timestamp (UTC).
                                tstamp=CxUtils::Time::GetUtcTimeMs();
                                msg->SetMessageTimeStamp(tstamp);

                                // If message contains UTC information, check it
                                // to see that the values are changing.  If values are
                                // not changing, then we probabaly lost signal from
                                // satellites.
                                if(msg->GetUTC(curUTC))
                                {
                                    if(fabs(curUTC - prevUTC) < .001 )
                                    {
                                        mHaveSignalFlag = false;
                                    }
                                    else
                                    {
                                        mHaveSignalFlag = true;
                                    }

                                    prevUTC = curUTC;
                                }
                                
                                // Signal that we are connected to an NMEA device
                                // since we are receiving supported messages.
                                mValidDataFlag = true;

                                // Enter mutex to insert in message list and
                                // to run callbacks.
                                mMutex.Lock();
                
                                mMessages.push_back(msg->Clone());
                                
                                // If we have hit our message limit size, then
                                // we must pop from the front.
                                if(mMessages.size() > mListSize)
                                {
                                    delete *(mMessages.begin());
                                    mMessages.pop_front();
                                }
                                // Leave mutex.
                                mMutex.Unlock();
                                
                                if(msg->GetType()==Message::GPGGA)
                                {
                                    const NMEA::GPGGA* report = 
                                        dynamic_cast<const NMEA::GPGGA*>(msg);
                                    if(report)
                                    {
                                        //std::cout << "NMEA: Data Signal: " << (int)mHaveSignalFlag << "\n";
                                        if(mHaveSignalFlag)
                                        {
                                            if(report->mNumSatellites > 3)
                                            {
                                                CxUtils::Wgs pos;
                                                report->GetPosition(pos);
                                                mLatitude=pos.mLatitude;
                                                mLongitude=pos.mLongitude;
                                                mAltitude=pos.mElevation;
                                                RunDefaultCallback();
                                            }
                                            else
                                            {
                                                std::cout << "NMEA: Not enough satillites: " << report->mNumSatellites << std::endl;
                                            }
                                        }
                                    }
                                }
                                //Do callbacks here
                                RunCallback(msg);
                            }                            
                            if(msg)
                            {
                                delete msg;
                                msg = NULL;
                            }
                            continue; // Continue search for messages.
                        }
                    }// If '$' found.
                    cpos++; // No message found, increment position.
                }// For each element in buffer.
                // If no '$' found, then we need to indicate failure to
                // receive a message.
                if(spos >= 0)
                {
                    // Delete all data up to the last '$' found
                    // in the buffer.
                    buffer.Delete((unsigned int)spos, 0);
                }
                if(buffer.Length() >= buffer.Reserved() - recvSize)
                {
                    buffer.Clear();
                }
            }// If received data
            else
            {
                failCount++;
            }
            
            // If we were connected, but have failed to receive
            // data a lot, then reset flag.
            if( mValidDataFlag && failCount > 50 )
            {
                mValidDataFlag = false;
            }
        }// If connected.
        CxUtils::SleepMs(1);
    }

    mRunningFlag = false;
}

bool DGPS::SetContinuousMode()
{
    mMessageTypes.insert(NMEA::Message::GPGGA);
    return true;
}

bool DGPS::RunCallback(const Message* message)
{
    std::map<unsigned char,std::set<GPS::Callback*> >::iterator cb;
    mMutex.Lock();
    cb = mCallbacks.find(message->GetType());
    if(cb != mCallbacks.end())
    {
        std::set<GPS::Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            ((NMEA::Callback *)*cb2)->ProcessMessage(message);
        }
    }
    mMutex.Unlock();
    return true;
}

/*  End of File */
