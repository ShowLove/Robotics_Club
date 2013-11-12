//////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.cpp
/// \brief Interface to the MCU motor control
///
/// Author(s): Daniel Barber<br>
/// Created: 1/17/2009<br>
/// Copyright (c) 2009<br>
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
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include "mcu/interface.h"

//Default messages
/*
#include "mcu/messages/setdigitaloutputs.h"
#include "mcu/messages/ping.h"
#include "mcu/messages/pong.h"
#include "mcu/messages/queryreport.h"
#include "mcu/messages/setdigitaloutputs.h"
#include "mcu/messages/reportdigitalinputs.h"
#include "mcu/messages/reportanaloginputs.h"
*/

#include <vector>

using namespace Zebulon;
using namespace MCU;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::Interface()
{
    mPacketQueue.reserve(100);
    MessageFactory::Initialize();
    //Assume to not use extra lines
    EnableRTSDTR(false);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::~Interface()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the extra lines for control for serial
///
///   \param[in] port The port name to connect on.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::EnableRTSDTR(bool value)
{
    mSerial.EnableDTR(value);
    mSerial.EnableRTS(value);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Establishes a connection to the device by searching different
///   BAUDS on a port.
///
///   \param[in] port The port name to connect on.
///   \param[in] baud The initial/first BAUD to try.
///   \param[in] parity If parity needs to be define, when using FPGA set to even.
///
///   \return True if connection established, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::Initialize(const std::string& port, const unsigned int baud, const bool tryOtherBauds, const unsigned int parity)
{
    bool result = false;
    
    Shutdown();
#ifndef NO_CB_THREAD
    mCallbackThread.CreateThread(Interface::CallbackThread, this);
    CxUtils::SleepMs(10);
#endif

    // Setup the possible baud rates we can talk to
    // Coral with, starting with initial baud rate passed to method.
    if(tryOtherBauds==true)
    {
        std::vector<unsigned short> baudsToTry;
        baudsToTry.push_back(baud);
        for(unsigned int b = 9600; b <= 115200; b*= 2)
        {
            if(b != baud)
            {
                baudsToTry.push_back(b);
            }
        }
    
        mSerialThread.CreateThread(&Interface::SerialThread, this);
        CxUtils::SleepMs(50);
        for(unsigned int i = 0; i < (unsigned int)baudsToTry.size(); i++)
        {
            mSerial.Disconnect();
            if(mSerial.Connect(port, baud, 8, parity)) // Connects to serial port at specified baud.
            {

            }
            if(mSerial.IsConnected())
            {
                result = true;
                break;
            }
        }
    }
    //use fixed baud
    else
    {
        mSerialThread.CreateThread(&Interface::SerialThread, this);
        CxUtils::SleepMs(100);
        mSerial.Disconnect();
        if(mSerial.Connect(port, baud, 8, parity)) // Connects to serial port at specified baud.
        {

        }
        if(mSerial.IsConnected())
        {
            result = true;
        }
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::Shutdown()
{
    mSerialThread.StopThread();
#ifndef NO_CB_THREAD
    mCallbackThread.StopThread();
#endif
    mSerial.Disconnect();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the message over the serial port to the device.
///
///   \param[in] message Pointer to message structure to transmit.
///   \param[in] postSendDelayMs Number of milliseconds to sleep/block after
///                              a send operation has occured.  This is done
///                              to prevent oversending data to the MCU.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::Send(const Message* message, const unsigned int postSendDelayMs)
{
    CxUtils::Packet packet;
    if(message->Write(packet))
    {
        if(mSerial.Send(packet))
        {
            //Make sure it sends
            mSerial.Flush();
            //Delay for a short period of time
            CxUtils::SleepMs(postSendDelayMs);
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to register a callback to be run whenever a specific
///   type of message is received.
///
///   \param[in] type The type of message to subscribe to.
///   \param[in] cb The callback to be run when the message is received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::RegisterCallback(const Message::Code type, Callback* cb)
{
    mCallbackMutex.Lock();
    mCallbacks[type].insert(cb);
    mCallbackMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to unregister a callback.
///
///   \param[in] type The type of message to subscribe to.
///   \param[in] cb The callback to be run when the message is received.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::RemoveCallback(const Message::Code type, Callback* cb)
{
    std::map<const Message::Code,std::set<Callback*> >::iterator cb1;
    mCallbackMutex.Lock();
    cb1 = mCallbacks.find(type);
    if(cb1 != mCallbacks.end())
    {
        std::set<Callback*>::iterator cb2;
        cb2 = cb1->second.find(cb);
        if(cb2 != cb1->second.end())
        {
            cb1->second.erase(cb2);
        }
    }
    mCallbackMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all registered callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::ClearCallback()
{
    mCallbackMutex.Lock();
    mCallbacks.clear();
    mCallbackMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to create an instance of any type of
///   MCU message.
///
///   \param[in] type The type of message to create.
///
///   \return Pointer to created message (you must delete it), NULL if type is
///   unknown.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Interface::CreateMessage(const Message::Code type)
{
    Message* msg = NULL;

    msg = MessageFactory::CreateMessage(type);
    
    if(msg == NULL)
    {
        std::cout << "MCU Interface: Unknown Message: " << (int)type << std::endl;
    }

    return msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread function that converts packet data received in serial
///          thread to message data and triggers callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::CallbackThread(void* arg)
{
    Interface* mcu = (Interface*)arg;
    CxUtils::Packet packet;
    unsigned int loopCounter = 0;
    packet.Reserve(500);
    while(mcu->mCallbackThread.QuitThreadFlag() == false)
    {
        // See if there is any packet data to process.
        bool havePacket = false;
        packet.Clear();
        mcu->mSerialMutex.Lock();
        havePacket = mcu->mPacketQueue.pop_front(&packet);
        mcu->mSerialMutex.Unlock();

        if(havePacket && packet.Length() > Message::StartOfStaticCommandType + sizeof(Message::Code))
        {
            Message::Code messageType = 0;
            messageType = (Message::Code)packet.Ptr()[Message::StartOfStaticCommandType];

            Message* newMessage = Interface::CreateMessage(messageType);
            if(newMessage)
            {
                if(newMessage->Read(packet))
                {
                    newMessage->ReadMessageBody(packet);

                    std::map<const Message::Code,std::set<Callback*> >::iterator cb;

                    mcu->mCallbackMutex.Lock();
                    
                    cb = mcu->mCallbacks.find(messageType);
                    if(cb != mcu->mCallbacks.end())
                    {
                        std::set<Callback*>::iterator cb2;
                        for(cb2 = cb->second.begin();
                            cb2 != cb->second.end();
                            cb2++)
                        {
                            (*cb2)->ProcessMessage(newMessage);
                        }
                    }

                    messageType = Message::All;
                    // Run callback for people registered for any type of message
                    cb = mcu->mCallbacks.find(messageType);
                    if(cb != mcu->mCallbacks.end())
                    {
                        std::set<Callback*>::iterator cb2;
                        for(cb2 = cb->second.begin();
                            cb2 != cb->second.end();
                            cb2++)
                        {
                            (*cb2)->ProcessMessage(newMessage);
                        }
                    }

                    mcu->mCallbackMutex.Unlock();
                }

                delete newMessage;
                newMessage = NULL;

            }
        }

#ifdef WIN32
        loopCounter++;
        if(loopCounter == 250)
        {
            CxUtils::SleepMs(1);
            loopCounter = 0;
        }
#else
        usleep(250);
#endif
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a thread which continuously receives and parsed data from
///   serial port.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::SerialThread(void* arg)
{
    Interface* mcu = (Interface*)arg;
    unsigned int readBytes = 0;
    unsigned int loopCounter = 0;
    while(mcu->mSerialThread.QuitThreadFlag() == false)
    {
        //std::cout << "In Thread\n" << std::endl;
        if(mcu->mSerial.IsConnected())
        {
            //std::cout << "Is Connected\n" << std::endl;
            //unsigned int available=mcu->mSerial.ReadBytesAvailable();
            //Crashing in linux somehow
            //if(available >= 3 && mcu->mSerial.Recv(mcu->mSmallBuffer, available) > 0)
#ifdef WIN32
            unsigned int available = mcu->mSerial.ReadBytesAvailable();
            if(available >= 3 && mcu->mSerial.Recv(mcu->mSmallBuffer, available) > 0)
#else
            if(mcu->mSerial.Recv(mcu->mSmallBuffer, readSize) > 0 || mcu->mLargeBuffer.Length() > 7)
#endif
            {
                //std::cout << "Has Received\n" << std::endl;
                // Add data to larger buffer.
                if(mcu->mSmallBuffer.Length() > 0)
                {
                    mcu->mLargeBuffer += mcu->mSmallBuffer;
                }
                
                // Look for a message within the buffered data.
                unsigned int endPos = 0;
                unsigned char* buffer = (unsigned char*)mcu->mLargeBuffer.Ptr();
                for(unsigned int pos = 0; pos + 1 < mcu->mLargeBuffer.Length(); pos++)
                {
                    if(buffer[pos] == Message::StaticHeader1 && buffer[pos + 1] == Message::StaticHeader2)
                    {
                        CxUtils::Packet extractedMessage;
                        Message::Code messageType;
                        if(Message::ExtractMessage(&buffer[pos], mcu->mLargeBuffer.Length() - pos, messageType, extractedMessage, true) > 0)
                        {
                            pos += extractedMessage.Length() - 1;
                            endPos = pos + 1;
                            
#ifdef NO_CB_THREAD
                            // Do something with extracted message data.
                            Message* newMessage = Interface::CreateMessage(messageType);
                            if(newMessage)
                            {
                                if(newMessage->Read(extractedMessage))
                                {
                                    newMessage->ReadMessageBody(extractedMessage);

                                    std::map<const Message::Code,std::set<Callback*> >::iterator cb;
                                    mcu->mCallbackMutex.Lock();
                                    cb = mcu->mCallbacks.find(messageType);
                                    if(cb != mcu->mCallbacks.end())
                                    {
                                        std::set<Callback*>::iterator cb2;
                                        for(cb2 = cb->second.begin();
                                            cb2 != cb->second.end();
                                            cb2++)
                                        {
                                            (*cb2)->ProcessMessage(newMessage);
                                        }
                                    }
                                    
                                    messageType = Message::All;
                                    //run callback for people registered for any type of message
                                    cb = mcu->mCallbacks.find(messageType);
                                    if(cb != mcu->mCallbacks.end())
                                    {
                                        std::set<Callback*>::iterator cb2;
                                        for(cb2 = cb->second.begin();
                                            cb2 != cb->second.end();
                                            cb2++)
                                        {
                                            (*cb2)->ProcessMessage(newMessage);
                                        }
                                    }

                                    mcu->mCallbackMutex.Unlock();

                                }
                                else
                                {
                                    delete newMessage;
                                    newMessage = NULL;
                                }
                            }
#else
                            mcu->mSerialMutex.Lock();
                            mcu->mPacketQueue.PushBack(extractedMessage);
                            mcu->mSerialMutex.Unlock();
#endif
                        }                        
                    }
                }
                
                // Clear out all data in buffer up until after the last message read.
                if(endPos > 0)
                {
                    mcu->mLargeBuffer.Delete(endPos, 0);
                }
                // Clear bad data.
                if(mcu->mLargeBuffer.Length() >= 500)
                {
                    mcu->mLargeBuffer.Clear();
                }
            }
        }
        //usleep(1);
        //CxUtils::SleepMs(1);
#ifdef WIN32
        loopCounter++;
        if(loopCounter == 250)
        {
            CxUtils::SleepMs(1);
            loopCounter = 0;
        }
#else
        usleep(250);
#endif
    }
}

/*  End of File */
