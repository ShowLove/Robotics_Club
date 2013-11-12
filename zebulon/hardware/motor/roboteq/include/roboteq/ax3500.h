/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ax3500.h
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
#ifndef __ZEBULON_MOTOR_ROBOTEQ_AX3500__H
#define __ZEBULON_MOTOR_ROBOTEQ_AX3500__H

#include "libroboteqdll.h"
#include <cxutils/thread.h>
#include <cxutils/serial.h>
#include <string>
#include <list>
#include <set>
#include <string.h>

namespace Zebulon
{
    namespace Motor
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \namespace Roboteq
        ///  \brief Namespace for all Roboteq related software in library.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        namespace Roboteq
        {
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class AX3500
            ///  \brief The AX3500 class is an interface for communication with the
            ///  Roboteq AX3500 motor controller.  It has methods for basic sending and
            ///  receiving, and for main board functionality like setting motor commands.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_ROBOTEQ_DLL AX3500
            {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \class Message
                ///  \brief Simple structure for storing messages received from AX3500.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                class ZEB_ROBOTEQ_DLL Message
                {
                public:
                    typedef std::list<Message*> List;
                    Message();
                    Message(const Message& msg);
                    ~Message();
                    bool IsQueryMessage() const;
                    bool IsCommandMessage() const;
                    bool IsDataLoggingMessage() const;
                    bool IsWatchDogModeMessage() const;
                    Message& operator=(const Message& msg);

                    std::string  mString;              ///<  The message data.
                    CxUtils::Time::Stamp mTimeStampMs; ///<  Time Stamp in Milliseconds.
                };
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \class Callback
                ///  \brief Callback structure to get messages received by AX3500 board.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                class ZEB_ROBOTEQ_DLL Callback
                {
                public:
                    Callback() {}
                    virtual ~Callback() {}
                    virtual void ProcessMessage(const AX3500::Message* message, const unsigned int boardID) = 0;
                };
                AX3500();
                virtual ~AX3500();
                int Initialize(const std::string& port, const unsigned int boardID = 0);
                int Shutdown();
                int TransmitMessage(const std::string& msg, AX3500::Message** response, const unsigned int waitTimeMs = 750, const bool confirm = true);
                int EnterWatchdogMode(const unsigned int waitTimeMs = 1500);
                int LeaveWatchdogMode() { return ResetController(); }
                int ResetController();
                int SendMotorCommand(const char channel, 
                                     const char value, 
                                     bool confirm = false,
                                     const unsigned int waitTimeMs = 750);
                int SendDualMotorCommand(const double percentChannelOne, 
                                         const double percentChannelTwo,
                                         const bool confirm = false, 
                                         const unsigned int waitTimeMs = 750);
                int QueryMainBatteryVoltage(double& mainBattery, double& internalVoltage, const unsigned int waitTimeMs = 750);
                int QueryEncoder(const int encoder, const bool relative, const unsigned int waitTimeMs);
                int QueryEncoder(const int encoder, int& value, const bool relative, const unsigned int waitTimeMs);
                int QueryEncoders(int& enc1, int& enc2, const bool relative, const unsigned int waitTimeMs = 750);
                bool IsConnected() const { return mSerialPort.IsConnected() && mReceiveThread.IsThreadActive(); }
                bool IsInWatchdogMode() const;
                void RegisterCallback(Callback* callback);
                static unsigned char HexToByte(const char hex);
                static int HexToInt(const char hex);
                static unsigned char HexToUnsignedByte(const char upper, const char lower); 
                static char HexToSignedByte(const char upper, const char lower); 
                static void UnsignedByteToHex(const unsigned char byte, char &upper, char &lower);
                static void SignedByteToHex(const char byte, char &upper, char &lower);
                static bool ExtractEncoderData(const std::string& msgData, int& encoderNumber, int& encoderValue);
            protected:
                void AddToMessageQueue(AX3500::Message* msg);
                unsigned int ReadQueryMessage(const char *buffer, const unsigned int len);
                static void ReceiveThreadFunction(void *args);
                unsigned int mBoardID;                  ///< Board ID (in case of more than 1 board).
                CxUtils::Mutex  mMessageQueueMutex;     ///< Mutex for message queue. 
                CxUtils::Mutex  mTransmittingMutex;     ///< Mutex for transmitting messages.
                CxUtils::Thread mReceiveThread;         ///< Thread for continuous message receiving.
                CxUtils::Serial mSerialPort;            ///< Serial port interface for board.
                CxUtils::Mutex  mCallbacksMutex;        ///< Mutex for thread protection of callbacks.
                Message::List mMessageQueue;            ///< Stores list of all messages received from board.
                volatile bool mWatchDogModeFlag;        ///< Indicates the board is in watchdog mode.
                volatile bool mEnterWatchDogModeFlag;   ///< If true, interface always attempts to maintain wathdog mode.
                unsigned int mMaxQueueSize;             ///< Sets the maximum size of queue for received messages.
                std::set<Callback*> mCallbacks;         ///< Callbacks.
            };
        }
    }
}

#endif
/* End of File */
