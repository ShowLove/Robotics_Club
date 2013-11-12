/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ax3500.cpp
/// \brief Interface class for the Roboteq AX3500 Motor Controller board.
///
/// Author(s): Daniel Barber<br>
/// Created: 5/3/2008<br>
/// Last Modified: 5/3/2008<br>
/// Copyright (c) 2008<br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "roboteq/ax3500.h"
#include <iostream>
#include <math.h>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Motor;
using namespace Roboteq;

#define DATA_LOGGING_MSG_LENGTH 33
//#define ROBOTEQ_DEBUG // Show debugging messages.


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
AX3500::Message::Message() : mTimeStampMs(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
AX3500::Message::Message(const Message& msg)
{
    *this = msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
AX3500::Message::~Message()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the message is a query response, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool AX3500::Message::IsQueryMessage() const
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
bool AX3500::Message::IsCommandMessage() const
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
bool AX3500::Message::IsDataLoggingMessage() const
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
bool AX3500::Message::IsWatchDogModeMessage() const
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
AX3500::Message& AX3500::Message::operator=(const Message& msg)
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
AX3500::AX3500() : mBoardID(0),
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
AX3500::~AX3500()
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
int AX3500::Initialize(const std::string& port, const unsigned int boardID)
{
    int result = 0;
    Shutdown();
    if(mSerialPort.Connect(port.c_str(), 9600, 7, 1, 1) == 1)
    {
        // Create thread to receiving
        if(mReceiveThread.CreateThread(&AX3500::ReceiveThreadFunction, this) == 1)
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
int AX3500::Shutdown()
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
///  \param waitTimeMs How long to wait for response from AX3500.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::TransmitMessage(const std::string& msg, AX3500::Message** response, const unsigned int waitTimeMs, const bool confirm)
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
///  \brief If the AX3500 is setup for serial communication, this will tell
///         it to enter Watch Dog Mode so it can receive queries and commands.
///
///  \param waitTimeMs How long to wait for watching dog mode success.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::EnterWatchdogMode(const unsigned int waitTimeMs)
{
    CxUtils::Time::Stamp startTimeMs = 0;
    
    mEnterWatchDogModeFlag = true;
    
    startTimeMs = CxUtils::GetTimeMs();
    // Wait for watch dog mode
    while((unsigned int)(CxUtils::GetTimeMs() - startTimeMs) < waitTimeMs)
    {
        if(mWatchDogModeFlag)
        {
            break;
        }
        CxUtils::SleepMs(1);
    }

    return mWatchDogModeFlag ? 1 : 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Resets the controller.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::ResetController()
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
///  \param channel 0 for channel 1, 1 for channel 2.
///  \param value The value to send to the motor.
///  \param confirm If true, method waits for confirmation response from board.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::SendMotorCommand(const char channel, const char value, const bool confirm, const unsigned int waitTimeMs)
{
    char message[128];
    char upper, lower, byte;
    AX3500::Message* response = NULL;
    int result = 0;

    if(value < 0)
    {
        byte = value*-1;
    }
    else
    {
        byte = value;
    }
    SignedByteToHex(byte, upper, lower);
    if(channel == 0)
    {
        if(value < 0)
        {
            sprintf(message, "!a%c%c\r", upper, lower);
        }
        else
        {
            sprintf(message, "!A%c%c\r", upper, lower);
        }
    }
    else
    {
        if(value < 0)
        {
            sprintf(message, "!b%c%c\r", upper, lower);
        }
        else
        {
            sprintf(message, "!B%c%c\r", upper, lower);
        }
    }
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
int AX3500::SendDualMotorCommand(const double percentChannelOne, 
                                 const double percentChannelTwo,
                                 const bool confirm,
                                 const unsigned int waitTimeMs)
{
    char value1, value2;

    value1 = (char)(percentChannelOne*127.0/100.0);
    value2 = (char)(percentChannelTwo*127.0/100.0);
    int result1, result2;
    result1 = SendMotorCommand(0, value1, confirm, waitTimeMs);
    result2 = SendMotorCommand(1, value2, confirm, waitTimeMs);
    if(result1 && result2)
    {
        return 1;
    }
    else
    {
        return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the battery voltage levels.
///
///  \param mainBattery Main battery voltage.
///  \param internalVoltage Internal power supply voltage level.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::QueryMainBatteryVoltage(double& mainBattery, double& internalVoltage, const unsigned int waitTimeMs)
{
    int result = 0;
    AX3500::Message* response = NULL;

    mainBattery = internalVoltage = 0;
    if(mWatchDogModeFlag)
    {
        std::string message("?e\r");
        
        if(TransmitMessage(message, &response, waitTimeMs) == 1 && response != NULL)
        {
            // Parse the data.
            char lower1, lower2, upper1, upper2;
            if(sscanf(response->mString.c_str(), 
                      "?e\r%c%c\r%c%c\r", 
                      &upper1, &lower1, &upper2, &lower2))
            {
                mainBattery = 55.0*(HexToUnsignedByte(upper1, lower1))/256.0;
                internalVoltage = 28.5*(HexToUnsignedByte(upper2, lower2))/256.0;
                result = 1;
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
///  \brief Gets the reading from both encoders.
///
///  \param enc1 Reading from encoder 1.
///  \param enc2 Reading from encoder 2.
///  \param relative Read relative encoder position or absolute.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::QueryEncoders(int& enc1, int& enc2, const bool relative, const unsigned int waitTimeMs)
{
    enc1 = enc2 = 0;

    if(QueryEncoder(0, enc1, relative, waitTimeMs) &&
       QueryEncoder(1, enc2, relative, waitTimeMs) )
    {
        return 1;
    }
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if in watchdog mode, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool AX3500::IsInWatchdogMode() const
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
void AX3500::RegisterCallback(Callback* callback)
{
    mCallbacksMutex.Lock();
    mCallbacks.insert(callback);
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends message to query for encoder, but doesn't wait for response.
///
///  \param encoder Encoder number 0 = 1, 1 = 2.
///  \param relative Read relative encoder position or absolute.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::QueryEncoder(const int encoder, const bool relative, const unsigned int waitTimeMs)
{
    int result = 0;
    AX3500::Message* response = NULL;
    std::string message;
    
    if(relative)
    {
        if(encoder == 0)
        {
            message = "?Q4\r";
        }
        else
        {
            message = "?Q5\r";
        }
    }
    else
    {
        if(encoder == 0)
        {
            message = "?Q0\r";
        }
        else
        {
            message = "?Q1\r";
        }
    }

    if(mWatchDogModeFlag)
    {
        if(TransmitMessage(message, NULL, waitTimeMs) == 1)
        {
            result = 1;
        }
    }

    return result;
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the reading from an encoder.
///
///  \param encoder Encoder number 0 = 1, 1 = 2.
///  \param value The value read.
///  \param relative Read relative encoder position or absolute.
///  \param waitTimeMs How long to wait for response/confirmation.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::QueryEncoder(const int encoder, int &value, const bool relative, const unsigned int waitTimeMs)
{
    int result = 0;
    AX3500::Message* response = NULL;
    std::string message;
    
    if(relative)
    {
        if(encoder == 0)
        {
            message = "?Q4\r";
        }
        else
        {
            message = "?Q5\r";
        }
    }
    else
    {
        if(encoder == 0)
        {
            message = "?Q0\r";
        }
        else
        {
            message = "?Q1\r";
        }
    }

    if(mWatchDogModeFlag)
    {
        if(TransmitMessage(message, &response, waitTimeMs) == 1 && response != NULL)
        {
            // Parse the data.
            int enumber = 0;
            char hexString[32];
            char *hexPtr = NULL;
            char *endPtr = NULL;
            bool negative = false;
            memset(hexString, 0, 32);
            if(sscanf(response->mString.c_str(), 
                      "?Q%d\r%s\r", 
                      &enumber, hexString))
            {                
                
                // Check for negative number
                if(HexToInt(hexString[0]) > 7)
                {
                    negative = true;
                }
                else
                {
                    negative = false;
                }

                hexPtr = hexString;      
                
                if(strlen(hexString) > 1)
                {
                    // If we have a 0 followed by a character greater
                    // than '7' we have a positive number
                    if(hexPtr[0] == '0' && HexToInt(hexPtr[1]) > 7)
                    {
                        hexPtr++;
                        negative = false;
                    }
                    // If we have an F followed by a number less
                    // than or equal to '7', then we have a
                    // negative number
                    else if(hexPtr[0] == 'F' && HexToInt(hexPtr[1]) <= 7)
                    {
                        hexPtr++;
                        negative = true;
                    }
                }
                sscanf(hexPtr, "%x", &value);
                if(negative)
                {
                    value = -((int)pow(16.0f, (float)(strlen(hexPtr))) - value); 
                }
                
                result = 1;
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
///  \brief Converts a hex character to a byte value.
///
///  \param hex Hexadecimal character to convert.
///
///  \return Byte value of hex character.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char AX3500::HexToByte(const char hex)
{
    return isdigit(hex) ? (hex - '0') : (hex - 'A' + 10);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts a hex character to a integer value.
///
///  \param hex Hexadecimal character to convert.
///
///  \return Integer value of hex character.
///
////////////////////////////////////////////////////////////////////////////////////
int AX3500::HexToInt(const char hex)
{
    return isdigit(hex) ? (hex - '0') : (hex - 'A' + 10);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts hex characters to byte value.
///
///  \param upper Most significant  hex character of byte value.
///  \param lower Least significant hex character of byte value.
///
///  \return Byte value represented by hex characters.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char AX3500::HexToUnsignedByte(const char upper, const char lower)
{
    unsigned char byte = 0;
    byte = isdigit(lower) ? (lower - '0') : (lower - 'A' + 10);
    byte |= (isdigit(upper) ? (upper - '0') : (upper - 'A' + 10)) << 4;
    return byte;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts hex characters to byte value.
///
///  \param upper Most significant  hex character of byte value.
///  \param lower Least significant hex character of byte value.
///
///  \return Byte value represented by hex characters.
///
////////////////////////////////////////////////////////////////////////////////////
char AX3500::HexToSignedByte(const char upper, const char lower)
{
    char byte = 0;
    byte = isdigit(lower) ? (lower - '0') : (lower - 'A' + 10);
    byte |= (isdigit(upper) ? (upper - '0') : (upper - 'A' + 10)) << 4;
    return byte;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts hex characters to byte value.
///
///  \param upper Most significant  hex character of byte value.
///  \param lower Least significant hex character of byte value.
///
///  \return Byte value represented by hex characters.
///
////////////////////////////////////////////////////////////////////////////////////
void AX3500::UnsignedByteToHex(const unsigned char byte, char &upper, char &lower)
{
    upper = (byte&0xF0) >> 4;
    lower = (byte&0x0F);
    upper = upper < 10 ? upper + '0' : upper + 'A' - 10; 
    lower = lower < 10 ? lower + '0' : lower + 'A' - 10; 
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts hex characters to byte value.
///
///  \param upper Most significant  hex character of byte value.
///  \param lower Least significant hex character of byte value.
///
///  \return Byte value represented by hex characters.
///
////////////////////////////////////////////////////////////////////////////////////
void AX3500::SignedByteToHex(const char byte, char &upper, char &lower)
{
    upper = (byte&0xF0) >> 4;
    lower = (byte&0x0F);
    upper = upper < 10 ? upper + '0' : upper + 'A' - 10; 
    lower = lower < 10 ? lower + '0' : lower + 'A' - 10; 
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Parse encoder data from a string returned from the roboteq.
///
///  \param The string that contains encoder data.
///  \param encoderNumber The encoder number is read from data, shows which encoder
///                       message originates from.
///  \param The value to save the data to.
///
///  \return True if data was found, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool AX3500::ExtractEncoderData(const std::string& msgData, int& encoderNumber, int& encoderValue)
{

    bool result = false;
    // Parse the data.
    char hexString[32];
    char *hexPtr = NULL;
    char *endPtr = NULL;
    bool negative = false;
    memset(hexString, 0, 32);
    if(sscanf(msgData.c_str(), 
              "?Q%d\r%s\r", 
              &encoderNumber, hexString))
    {                
        result = true;
        // Check for negative number
        if(HexToInt(hexString[0]) > 7)
        {
            negative = true;
        }
        else
        {
            negative = false;
        }

        hexPtr = hexString;      
        
        if(strlen(hexString) > 1)
        {
            // If we have a 0 followed by a character greater
            // than '7' we have a positive number
            if(hexPtr[0] == '0' && HexToInt(hexPtr[1]) > 7)
            {
                hexPtr++;
                negative = false;
            }
            // If we have an F followed by a number less
            // than or equal to '7', then we have a
            // negative number
            else if(hexPtr[0] == 'F' && HexToInt(hexPtr[1]) <= 7)
            {
                hexPtr++;
                negative = true;
            }
        }
        sscanf(hexPtr, "%x", &encoderValue);
        if(negative)
        {
            encoderValue = -((int)pow(16.0f, (float)(strlen(hexPtr))) - encoderValue); 
        }
    }
    return result;
        
}
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds message to internal queue.
///
///  \param message Message to ad to queue.
///
////////////////////////////////////////////////////////////////////////////////////
void AX3500::AddToMessageQueue(AX3500::Message* message)
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
unsigned int AX3500::ReadQueryMessage(const char *buffer, const unsigned int len)
{
    unsigned int bytesRead = 0;
    bool complete = false;
    AX3500::Message* message = new AX3500::Message();
    message->mTimeStampMs = CxUtils::GetTimeMs();

    if(*buffer != '?')
        return bytesRead;

    switch(tolower(buffer[1]))
    {
    case 'e':
        {
            unsigned int returns = 0;

            for(unsigned int i = 0; i < len && i < 10; i++)
            {
                message->mString.push_back(buffer[i]);
                if(buffer[i] == '\r')
                {
                    returns++;
                }
                if(returns == 3)
                {
                    complete = true;
                    break;
                }
            }            
        }
        break;
    case 'q':
        {
            unsigned int returns = 0;

            for(unsigned int i = 0; i < len && i < 10; i++)
            {
                if(i > 0 && buffer[i] == '?')
                {
                    complete = true;
                    break;
                }
                message->mString.push_back(buffer[i]);
                if(buffer[i] == '\r')
                {
                    returns++;
                }
                if(returns == 2)
                {
                    complete = true;
                    break;
                }
            }            
        }
        break;
    default:
        bytesRead = 0;
        break;
    };

    if(complete)
    {
        bytesRead = (unsigned int)message->mString.size();
#ifdef ROBOTEQ_DEBUG
        const char *strPtr = message->mString.c_str();
        for(unsigned int i = 0; i < message->mString.size(); i++)
        {
            if(strPtr[i] == '\r')
            {
                std::cout << " ";
            }
            else
            {
                std::cout << strPtr[i];
            }
        }
        std::cout << std::endl;
#endif
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
void AX3500::ReceiveThreadFunction(void *args)
{
    AX3500* board = (AX3500*)(args);
    CxUtils::Packet buffer, packet;
    unsigned int readBytesAvailable = 0;
    unsigned char *ptr = NULL;
    unsigned int pos = 0;
    unsigned int erasePos = 0;
    AX3500::Message* message = NULL;

    while(board && 
          board->mReceiveThread.QuitThreadFlag() == false &&
          board->mSerialPort.IsConnected())
    {
        if(board->mEnterWatchDogModeFlag && 
            board->mWatchDogModeFlag == false)
        {
            packet.Clear();
            packet.WriteByte(13);
            for(unsigned int i = 0; i < 10; i++)
            {
                board->mSerialPort.Send(packet);
                CxUtils::SleepMs(50);
            }
        }
        // Get the number of bytes in the receive buffer
        readBytesAvailable = board->mSerialPort.ReadBytesAvailable();

        if(readBytesAvailable &&
            board->mSerialPort.Recv(packet, readBytesAvailable))
        {
            // Add the recently read data to
            // our total message buffer.
            buffer += packet;

            // Go through the buffer and try parse out messages.
            ptr = (unsigned char *)(buffer.Ptr());
            pos = 0;
            erasePos = 0;
            while(pos < buffer.Length())
            {
                // Check for the start of different kinds of messages
                if(ptr[pos] == ':') // Analog and R/C Data Logging String Format
                {
                    board->mWatchDogModeFlag = false;
                    // See if the whole string is available
                    if(pos + DATA_LOGGING_MSG_LENGTH < buffer.Length())
                    {
                        // Get the whole string.
                        if(ptr[pos + DATA_LOGGING_MSG_LENGTH] == '\r')
                        {
                            message = new AX3500::Message();
                            message->mTimeStampMs = CxUtils::GetTimeMs();
                            message->mString = (char *)&ptr[pos];
#ifdef ROBOTEQ_DEBUG
                            std::cout << message->mString << std::endl;
#endif
                            board->AddToMessageQueue(message);
                            message = NULL;
                            pos += DATA_LOGGING_MSG_LENGTH + 1;
                            erasePos = pos;
                            continue;
                        }
                    }
                }
                else if(pos + 1 < buffer.Length() && 
                        ptr[pos] == 'O' && ptr[pos + 1] == 'K')
                {
                    board->mWatchDogModeFlag = true;
                    message = new AX3500::Message();
                    message->mTimeStampMs = CxUtils::GetTimeMs();
                    message->mString = "OK";                    
#ifdef ROBOTEQ_DEBUG
                    std::cout << message->mString << std::endl;
#endif
                    board->AddToMessageQueue(message);
                    message = NULL;
                    pos += 2;
                    erasePos = pos;
                }
                else if(ptr[pos] == 'W') // Watchdog Mode
                {
                    board->mWatchDogModeFlag = true;
                    message = new AX3500::Message();
                    message->mTimeStampMs = CxUtils::GetTimeMs(); 
                    message->mString.push_back(ptr[pos]);
#ifdef ROBOTEQ_DEBUG
                    std::cout << message->mString << std::endl;
#endif
                    board->AddToMessageQueue(message);
                    message = NULL;
                    pos += 1;
                    erasePos = pos;                    
                    continue;
                }
                else if(ptr[pos] == '?') // Query message response.
                {
                    unsigned int read = board->ReadQueryMessage((const char *)(&ptr[pos]), buffer.Length() - pos);
                    if(read > 0)
                    {
                        pos += read;
                        erasePos = pos;
                        continue;
                    }
                }
                else if(ptr[pos] == '!') // Command message response.
                {
                    std::string str;
                    bool complete = false;
                    for(unsigned int i = pos; i < buffer.Length(); i++)
                    {
                        str.push_back(ptr[i]);
                        if(ptr[i] == '\r')
                        {
                            if(i + 1 < buffer.Length() &&
                                ptr[i+1] == '+' || ptr[i+1] == '-')
                            {
                                str.push_back(ptr[i+1]);
                                complete = true;
                            }
                            break;
                        }
                    }
                    if(complete)
                    {
                        message = new AX3500::Message();
                        message->mTimeStampMs = CxUtils::GetTimeMs();
                        message->mString = str;
#ifdef ROBOTEQ_DEBUG
                        std::cout << message->mString << std::endl;
#endif
                        board->AddToMessageQueue(message);

                        message = NULL;
                        pos += (unsigned int)str.size();
                        erasePos = pos;  
                    }
                }

                pos++;

            } // While parsing buffer

            // Delete all old data in buffer up to
            // where we finished parsing.
            buffer.Delete(erasePos, 0);
            // Flush large buffer of junk.
            if(erasePos == 0 && buffer.Length() > 1000)
            {
                buffer.Clear();
            }
        }

        CxUtils::SleepMs(1);
    }
}


/*  End of File */
