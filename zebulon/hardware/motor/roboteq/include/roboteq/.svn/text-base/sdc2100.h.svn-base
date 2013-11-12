/////////////////////////////////////////////////////////////////////////////////////
///
/// \file sdc2100.h
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
#ifndef __ZEBULON_MOTOR_ROBOTEQ_SDC2100__H
#define __ZEBULON_MOTOR_ROBOTEQ_SDC2100__H

#include "libroboteqdll.h"
#include <cxutils/thread.h>
#include <cxutils/serial.h>
#include <string>
#include <list>
#include <set>
#include <string.h>
using namespace std;
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
            ///  \class SDC2100
            ///  \brief The SDC2100 class is an interface for communication with the
            ///  Roboteq SDC2100 motor controller.  It has methods for basic sending and
            ///  receiving, and for main board functionality like setting motor commands.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class ZEB_ROBOTEQ_DLL SDC2100
            {
            public:
                ////////////////////////////////////////////////////////////////////////////////////
                ///
                ///  \class Message
                ///  \brief Simple structure for storing messages received from SDC2100.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                class ZEB_ROBOTEQ_DLL Message
                {
                public:
                    static const string SETACCELERATION;
                    static const string SETDECELERATION;
                    static const string SETESTOP;
                    static const string SETESTOPRELEASE;
                    static const string SETSINGLEMOTOR;
                    static const string SETMULTIPLEMOTOR;

                    static const string QUERYMOTORAMPS;
                    static const string QUERYBATTERYAMPS;
                    static const string QUERYTEMP;
                    static const string QUERYVOLTS;

                    static const string CONFWATCHDOG;

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
                    virtual void ProcessMessage(const SDC2100::Message* message, const unsigned int boardID) = 0;
                };
                SDC2100();
                virtual ~SDC2100();
                int Initialize(const std::string& port, const unsigned int boardID = 0);
                int Shutdown();
                int TransmitMessage(const std::string& msg, SDC2100::Message** response, const unsigned int waitTimeMs = 750, const bool confirm = true);
                int EnterWatchdogMode(const unsigned int waitTimeMs = 1500);
                int LeaveWatchdogMode() { return ResetController(); }
                int ResetController();
                int SendMotorCommand(const char channel,
                                     const double percent,
                                     bool confirm = false,
                                     const unsigned int waitTimeMs = 750);
                int SendDualMotorCommand(const double percentChannelOne,
                                         const double percentChannelTwo,
                                         const bool confirm = false,
                                         const unsigned int waitTimeMs = 750);
                int QueryMainBatteryVoltage(double& mainBattery, double& internalVoltage, double& dsubVoltage, const unsigned int waitTimeMs = 750);
                bool IsConnected() const { return mSerialPort.IsConnected() && mReceiveThread.IsThreadActive(); }
                bool IsInWatchdogMode() const;
                void RegisterCallback(Callback* callback);
            protected:
                void AddToMessageQueue(SDC2100::Message* msg);
                unsigned int ReadMessage(const char *buffer);
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
