/////////////////////////////////////////////////////////////////////////////////////
///
/// \file sdc2100.cpp
/// \brief Interface class for the Roboteq sdc2100 Motor Controller board.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Last Modified: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "roboteq/sdc2100.h"
#include <iostream>
#include <math.h>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Motor;
using namespace Roboteq;

#define DATA_LOGGING_MSG_LENGTH 33
//#define ROBOTEQ_DEBUG // Show debugging messages.


const string SDC2100::Message::SETACCELERATION = "!AC";
const string SDC2100::Message::SETDECELERATION = "!DC";
const string SDC2100::Message::SETESTOP = "!EX";
const string SDC2100::Message::SETESTOPRELEASE = "!MG";
const string SDC2100::Message::SETSINGLEMOTOR = "!G";
const string SDC2100::Message::SETMULTIPLEMOTOR = "!M";

const string SDC2100::Message::QUERYMOTORAMPS = "?A";
const string SDC2100::Message::QUERYBATTERYAMPS = "?BA";
const string SDC2100::Message::QUERYTEMP = "?T";
const string SDC2100::Message::QUERYVOLTS = "?V";
const string SDC2100::Message::CONFWATCHDOG = "^RWD";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::Message::Message() : mTimeStampMs(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::Message::Message(const Message& msg)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::Message::~Message()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the message is a query response, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SDC2100::Message::IsQueryMessage() const
{
    if(mString.empty() == false &&
            mString.c_str()[0] == '?')
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the message is a command response, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SDC2100::Message::IsCommandMessage() const
{
    if(mString.empty() == false &&
            mString.c_str()[0] == '!')
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the message is a data loging message (when not in watchdog
///          mode).
///
////////////////////////////////////////////////////////////////////////////////////
bool SDC2100::Message::IsDataLoggingMessage() const
{
    if(mString.empty() == false &&
            mString.c_str()[0] == ':')
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if this is a watch dog mode message (only has 'W' in it).
///
////////////////////////////////////////////////////////////////////////////////////
bool SDC2100::Message::IsWatchDogModeMessage() const
{
    if(mString.size() == 2 &&
            mString.c_str()[0] == 'W')
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::Message& SDC2100::Message::operator=(const Message& msg)
{
    mString = msg.mString;
    mTimeStampMs = msg.mTimeStampMs;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::SDC2100() : mBoardID(0),
    mMaxQueueSize(10),
    mWatchDogModeFlag(false),
    mEnterWatchDogModeFlag(false)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SDC2100::~SDC2100()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the interface, connecting to board.
///
///  \param port The serial port to connect to.
///
///  \return 1 if success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::Initialize(const std::string& port, const unsigned int boardID)
{
    int result = 0;
    Shutdown();
    if(mSerialPort.Connect(port.c_str(), 9600, 7, 1, 1) == 1)
    {
        // Create thread to receiving
        if(mReceiveThread.CreateThread(&SDC2100::ReceiveThreadFunction, this) == 1)
        {
            // See if we got any valid data from serial port.
            CxUtils::Time::Stamp startTimeMs = CxUtils::GetTimeMs();
            while(CxUtils::GetTimeMs() - startTimeMs < 2000 && result == 0)
            {
                //mMessageQueueMutex.Lock();
                //if(mMessageQueue.size() > 0)
                //{
                result = 1;
                mWatchDogModeFlag = true;
                //}
                //mMessageQueueMutex.Unlock();
                CxUtils::SleepMs(1);
            }
        }
    }

    // Do shutdown again just for cleanup on failure.
    if(result == 0)
    {
        Shutdown();
    }

    mBoardID = boardID;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown the interface, and resets the controller to default state.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::Shutdown()
{
    ResetController();
    mSerialPort.Disconnect();
    mReceiveThread.StopThread();

    Message::List::iterator msg;
    mMessageQueueMutex.Lock();
    for(msg = mMessageQueue.begin();
        msg != mMessageQueue.end();
        msg++)
    {
        delete (*msg);
    }
    mMessageQueue.clear();
    mMessageQueueMutex.Unlock();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the message, and waits for a response if expected.
///
///  \param msg Command or query to send to the board.
///  \param response If response message is generated, the pointer to it is copied.
///                  You must delete it!  Set to NULL if you do not want to
///                  get confirmation response.
///  \param waitTimeMs How long to wait for response from SDC2100.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::TransmitMessage(const std::string& msg, SDC2100::Message** response, const unsigned int waitTimeMs, const bool confirm)
{
    int result = 0;
    Message::List::iterator item;
    if(response)
    {
        *response = NULL;
    }

    if(mWatchDogModeFlag)
    {
        // Use mutex to make sure only one command/query can
        // be sent at a time.
        int sentBytes =  0;
        {
            CxUtils::Mutex::ScopedLock lock(&mTransmittingMutex);
            sentBytes = mSerialPort.Send(msg.c_str(), (unsigned int)msg.length());
        }
        if(sentBytes > 0)
        {
            //mSerialPort.FlushWriteBuffer();
            if(confirm && response && (msg.c_str()[0] == '?' || msg.c_str()[0] == '!'))
            {
                CxUtils::Time::Stamp startTimeMs = CxUtils::GetTimeMs();
                // Wait for a response (but only for XX milliseconds).
                while((unsigned int)(CxUtils::GetTimeMs() - startTimeMs) < waitTimeMs)
                {
                    // Look for response.
                    {
                        CxUtils::Mutex::ScopedLock queueLock(&mMessageQueueMutex);
                        for(item = mMessageQueue.begin();
                            item != mMessageQueue.end();
                            item++)
                        {
                            // All commands and queries echo back what was
                            // transmitted, which is then followed by data body.  Find
                            // the matching message item with same header.
                            if( *item != NULL && strstr((*item)->mString.c_str(), msg.c_str()) != NULL )
                            {
                                if(response)
                                {
                                    *response = (*item);
                                }
                                else
                                {
                                    // Free memory.
                                    delete (*item);
                                }
                                (*item) = NULL;
                                mMessageQueue.erase(item);
                                result = 1;
                                break;
                            }
                        }
                    }
                    //CxUtils::SleepMs(1);
                } // While not timed-out
            }
            else
            {
                // If not query or command, then
                // no response is generated or no response
                // wanted.
                result = 1;
            }
        } // If data sent.
    } // If board is ready to receive RS232 data.

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief If the SDC2100 is setup for serial communication, this will tell
///         it to enter Watch Dog Mode so it can receive queries and commands.
///
///  \param waitTimeMs How long to wait for watching dog mode success.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::EnterWatchdogMode(const unsigned int waitTimeMs)
{
    //    CxUtils::Time::Stamp startTimeMs = 0;

    //    mEnterWatchDogModeFlag = true;

    //    startTimeMs = CxUtils::GetTimeMs();
    //    // Wait for watch dog mode
    //    while((unsigned int)(CxUtils::GetTimeMs() - startTimeMs) < waitTimeMs)
    //    {
    //        if(mWatchDogModeFlag)
    //        {
    //            break;
    //        }
    //        CxUtils::SleepMs(1);
    //    }

    //    return mWatchDogModeFlag ? 1 : 0;
    char message[128];
    SDC2100::Message* response = NULL;
    int result = 0;
    sprintf(message,"%s %c",SDC2100::Message::CONFWATCHDOG.c_str(),waitTimeMs);

    if(TransmitMessage(std::string(message), &response, waitTimeMs) == 1 && response)
    {
        // Check for positive response.
        if(strchr(response->mString.c_str(), '+'))
        {
            result = 1;
        }
    }
    if(response)
    {
        delete response;
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Resets the controller.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::ResetController()
{
    std::string message("%rrrrrr");
    mEnterWatchDogModeFlag = false;
    {
        CxUtils::Mutex::ScopedLock lock(&mTransmittingMutex);
        if(mSerialPort.Send(message.c_str(), (unsigned int)message.size()))
        {
            // There is no response message from controller.
            return 1;
        }
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a speed or position command to a motor channel.
///
///  \param channel 1 for channel 1, 2 for channel 2.
///  \param value The value to send to the motor [-100,100].
///  \param confirm If true, method waits for confirmation response from board.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::SendMotorCommand(const char channel, const double percent, const bool confirm, const unsigned int waitTimeMs)
{
    char message[128];
    SDC2100::Message* response = NULL;
    int result = 0;
    int value = percent * 10.0;
    sprintf(message,"%s %d %d\r",SDC2100::Message::SETSINGLEMOTOR.c_str(),channel,value);
    if(confirm)
    {
        if(TransmitMessage(std::string(message), &response, waitTimeMs) == 1 && response)
        {
            // Check for positive response.
            if(strchr(response->mString.c_str(), '+'))
            {
                result = 1;
            }
        }
        if(response)
        {
            delete response;
        }
    }
    else
    {
        result = TransmitMessage(std::string(message), NULL, waitTimeMs, false);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends an effort percentage to both motor channels.  If in single motor
///  control mode, it will set channel 1 and 2 seperately, if in mixed modes this
///  will adjust speed and steering.
///
///  \param percentChannelOne Wrench effort for channel 1 [-100,100].
///  \param percentChannelTwo Wrench effort for channel 2 [-100,100].
///  \param confirm If true, waits for confirmation response message. Otherwise
///         just transmits data.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::SendDualMotorCommand(const double percentChannelOne,
                                  const double percentChannelTwo,
                                  const bool confirm,
                                  const unsigned int waitTimeMs)
{
    char message[128];
    SDC2100::Message* response = NULL;
    int result = 0;
    int val1 = percentChannelOne * 10.;
    int val2 = percentChannelTwo * 10.;
    sprintf(message,"%s %d %d\r",SDC2100::Message::SETMULTIPLEMOTOR.c_str(),val1,val2);
    if(confirm)
    {
        if(TransmitMessage(std::string(message), &response, waitTimeMs) == 1 && response)
        {
            // Check for positive response.
            if(strchr(response->mString.c_str(), '+'))
            {
                result = 1;
            }
        }
        if(response)
        {
            delete response;
        }
    }
    else
    {
        result = TransmitMessage(std::string(message), NULL, waitTimeMs, false);
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the battery voltage levels.
///
///  \param mainBattery Main battery voltage.
///  \param internalVoltage Internal power supply voltage level.
///  \param dsubVoltage 5V ouptut on the DSUB 15 or 25 front connector.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int SDC2100::QueryMainBatteryVoltage(double& mainBattery, double& internalVoltage, double& dsubVoltage, const unsigned int waitTimeMs)
{
    int result = 0;
    SDC2100::Message* response = NULL;

    mainBattery = internalVoltage = 0;
    if(mWatchDogModeFlag)
    {

        if(TransmitMessage(SDC2100::Message::QUERYVOLTS, &response, waitTimeMs) == 1 && response != NULL)
        {

            if(sscanf(response->mString.c_str(),
                      "V=%lf:%lf:%lf",
                      &mainBattery, &internalVoltage, &dsubVoltage))
            {
                result = 1;
                mainBattery /= 10.;
                internalVoltage /= 10.;
                dsubVoltage /= 100;
            }
        }
    }

    if(response)
    {
        delete response;
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if in watchdog mode, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool SDC2100::IsInWatchdogMode() const
{
    return mWatchDogModeFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Register a method to be called when messages are received.
///
///  \param[in] callback Callback object.
///
////////////////////////////////////////////////////////////////////////////////////
void SDC2100::RegisterCallback(Callback* callback)
{
    mCallbacksMutex.Lock();
    mCallbacks.insert(callback);
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds message to internal queue.
///
///  \param message Message to ad to queue.
///
////////////////////////////////////////////////////////////////////////////////////
void SDC2100::AddToMessageQueue(SDC2100::Message* message)
{
    if(message)
    {
        {
            CxUtils::Mutex::ScopedLock cbLock(&mCallbacksMutex);
            std::set<Callback*>::iterator cb;
            for(cb = mCallbacks.begin();
                cb != mCallbacks.end();
                cb++)
            {
                (*cb)->ProcessMessage(message, mBoardID);
            }
        }
        {
            CxUtils::Mutex::ScopedLock queueLock(&mMessageQueueMutex);
            // Check to see if we have hit max queue size and
            // remove the data.
            if(mMessageQueue.size() > mMaxQueueSize)
            {
                delete mMessageQueue.front();
                mMessageQueue.pop_front();
            }
            mMessageQueue.push_back(message);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a single query message from the start of the buffer.
///
///  \param buffer Pointer to character buffer to read from.
///  \param len Length of the buffer in bytes.
///
///  \return Number of bytes read.
///
////////////////////////////////////////////////////////////////////////////////////
//unsigned int SDC2100::ReadQueryMessage(const char *buffer, const unsigned int len)
//{
//    unsigned int bytesRead = 0;
//    bool complete = false;
//    SDC2100::Message* message = new SDC2100::Message();
//    message->mTimeStampMs = CxUtils::GetTimeMs();

//    if(*buffer != '?')
//        return bytesRead;

//    for(unsigned int i = 0; i < len && i < 30; i++)
//    {
//        message->mString.push_back(buffer[i]);
//        if(buffer[i] == '\r')
//        {
//            complete = true;
//            break;
//        }
//    }

//    if(complete)
//    {
//        bytesRead = (unsigned int)message->mString.size();
//#ifdef ROBOTEQ_DEBUG
//        const char *strPtr = message->mString.c_str();
//        for(unsigned int i = 0; i < message->mString.size(); i++)
//        {
//            if(strPtr[i] == '\r')
//            {
//                std::cout << " ";
//            }
//            else
//            {
//                std::cout << strPtr[i];
//            }
//        }
//        std::cout << std::endl;
//#endif
//        AddToMessageQueue(message);
//        message = NULL;
//    }
//    else
//    {
//        delete message;
//        message = NULL;
//    }

//    return bytesRead;
//}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Reads a single query message from the start of the buffer.
///
///  \param buffer Pointer to character buffer to read from.
///
///  \return Number of bytes read.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int SDC2100::ReadMessage(const char *buffer)
{
    unsigned int bytesRead = 0;
    bool complete = false;
    SDC2100::Message* message = new SDC2100::Message();
    message->mTimeStampMs = CxUtils::GetTimeMs();

    if(buffer == NULL || *buffer != '?' || *buffer != '!')
        return bytesRead;

    for(unsigned int i = 1; i < 30; i++)
    {
        message->mString.push_back(buffer[i]);
        if(buffer[i] == '\r')
        {
            complete = true;
            break;
        }
    }

    if(complete)
    {
        bytesRead = (unsigned int)message->mString.size();
        AddToMessageQueue(message);
        message = NULL;
    }
    else
    {
        delete message;
        message = NULL;
    }

    return bytesRead;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread function which keeps reading data from serial port, adding
///  messages to queue, and will even maintain Watch Dog Mode if required.
///
////////////////////////////////////////////////////////////////////////////////////
void SDC2100::ReceiveThreadFunction(void *args)
{
    SDC2100* board = (SDC2100*)(args);
    CxUtils::Packet buffer, packet;
    unsigned int readBytesAvailable = 0;
    char *ptr = NULL;
    unsigned int pos = 0;
    unsigned int erasePos = 0;
    SDC2100::Message* message = NULL;

    while(board &&
          board->mReceiveThread.QuitThreadFlag() == false &&
          board->mSerialPort.IsConnected())
    {

        CxUtils::SleepMs(1);
        // Get the number of bytes in the receive buffer
        readBytesAvailable = board->mSerialPort.ReadBytesAvailable();

        if(readBytesAvailable &&
                board->mSerialPort.Recv(packet, readBytesAvailable))
        {
            // Add the recently read data to
            // our total message buffer.
            buffer += packet;
            // Go through the buffer and try parse out messages.
            ptr = (char *)(buffer.Ptr());
            pos = 0;
            erasePos = 0;
            while(pos < buffer.Length())
            {
                if(ptr[pos] == '?' || ptr[pos] == '!')
                {
                    pos += board->ReadMessage(ptr+pos);

                }
                else
                {
                    pos++;
                }
            }
            // Delete all old data in buffer up to
            // where we finished parsing.
            buffer.Delete(pos, 0);
            // Flush large buffer of junk.
            if(pos == 0 && buffer.Length() > 1000)
            {
                buffer.Clear();
            }
        }
    }
    //        if(board->mEnterWatchDogModeFlag &&
    //            board->mWatchDogModeFlag == false)
    //        {
    //            packet.Clear();
    //            packet.WriteByte(13);
    //            for(unsigned int i = 0; i < 10; i++)
    //            {
    //                board->mSerialPort.Send(packet);
    //                CxUtils::SleepMs(50);
    //            }
    //        }
    //        // Get the number of bytes in the receive buffer
    //        readBytesAvailable = board->mSerialPort.ReadBytesAvailable();

    //        if(readBytesAvailable &&
    //            board->mSerialPort.Recv(packet, readBytesAvailable))
    //        {
    //            // Add the recently read data to
    //            // our total message buffer.
    //            buffer += packet;

    //            // Go through the buffer and try parse out messages.
    //            ptr = (unsigned char *)(buffer.Ptr());
    //            pos = 0;
    //            erasePos = 0;
    //            while(pos < buffer.Length())
    //            {
    //                // Check for the start of different kinds of messages
    //                if(ptr[pos] == ':') // Analog and R/C Data Logging String Format
    //                {
    //                    board->mWatchDogModeFlag = false;
    //                    // See if the whole string is available
    //                    if(pos + DATA_LOGGING_MSG_LENGTH < buffer.Length())
    //                    {
    //                        // Get the whole string.
    //                        if(ptr[pos + DATA_LOGGING_MSG_LENGTH] == '\r')
    //                        {
    //                            message = new SDC2100::Message();
    //                            message->mTimeStampMs = CxUtils::GetTimeMs();
    //                            message->mString = (char *)&ptr[pos];
    //#ifdef ROBOTEQ_DEBUG
    //                            std::cout << message->mString << std::endl;
    //#endif
    //                            board->AddToMessageQueue(message);
    //                            message = NULL;
    //                            pos += DATA_LOGGING_MSG_LENGTH + 1;
    //                            erasePos = pos;
    //                            continue;
    //                        }
    //                    }
    //                }
    //                else if(pos + 1 < buffer.Length() &&
    //                        ptr[pos] == 'O' && ptr[pos + 1] == 'K')
    //                {
    //                    board->mWatchDogModeFlag = true;
    //                    message = new SDC2100::Message();
    //                    message->mTimeStampMs = CxUtils::GetTimeMs();
    //                    message->mString = "OK";
    //#ifdef ROBOTEQ_DEBUG
    //                    std::cout << message->mString << std::endl;
    //#endif
    //                    board->AddToMessageQueue(message);
    //                    message = NULL;
    //                    pos += 2;
    //                    erasePos = pos;
    //                }
    //                else if(ptr[pos] == 'W') // Watchdog Mode
    //                {
    //                    board->mWatchDogModeFlag = true;
    //                    message = new SDC2100::Message();
    //                    message->mTimeStampMs = CxUtils::GetTimeMs();
    //                    message->mString.push_back(ptr[pos]);
    //#ifdef ROBOTEQ_DEBUG
    //                    std::cout << message->mString << std::endl;
    //#endif
    //                    board->AddToMessageQueue(message);
    //                    message = NULL;
    //                    pos += 1;
    //                    erasePos = pos;
    //                    continue;
    //                }
    //                else if(ptr[pos] == '?') // Query message response.
    //                {
    //                    unsigned int read = board->ReadQueryMessage((const char *)(&ptr[pos]), buffer.Length() - pos);
    //                    if(read > 0)
    //                    {
    //                        pos += read;
    //                        erasePos = pos;
    //                        continue;
    //                    }
    //                }
    //                else if(ptr[pos] == '!') // Command message response.
    //                {
    //                    std::string str;
    //                    bool complete = false;
    //                    for(unsigned int i = pos; i < buffer.Length(); i++)
    //                    {
    //                        str.push_back(ptr[i]);
    //                        if(ptr[i] == '\r')
    //                        {
    //                            if(i + 1 < buffer.Length() &&
    //                                ptr[i+1] == '+' || ptr[i+1] == '-')
    //                            {
    //                                str.push_back(ptr[i+1]);
    //                                complete = true;
    //                            }
    //                            break;
    //                        }
    //                    }
    //                    if(complete)
    //                    {
    //                        message = new SDC2100::Message();
    //                        message->mTimeStampMs = CxUtils::GetTimeMs();
    //                        message->mString = str;
    //#ifdef ROBOTEQ_DEBUG
    //                        std::cout << message->mString << std::endl;
    //#endif
    //                        board->AddToMessageQueue(message);

    //                        message = NULL;
    //                        pos += (unsigned int)str.size();
    //                        erasePos = pos;
    //                    }
    //                }

    //                pos++;

    //            } // While parsing buffer

//    // Delete all old data in buffer up to
//    // where we finished parsing.
//    buffer.Delete(erasePos, 0);
//    // Flush large buffer of junk.
//    if(erasePos == 0 && buffer.Length() > 1000)
//    {
//        buffer.Clear();
//    }
}


/*  End of File */
