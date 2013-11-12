//////////////////////////////////////////////////////////////////////////////////////
///
/// \file slipinterface.cpp
/// \brief SlipInterface for Slip (Serial Line IP, RFC 1055) Transmission Protocol
///        compliant device, i.e. microcontroller.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 12/27/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu <br>
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
#include "mcu/slipinterface.h"
#include "mcu/messagefactory.h"
#include <vector>
#include <iostream>

using namespace Zebulon;
using namespace MCU;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
SlipInterface::SlipInterface()
{
    mFrameQueue.reserve(100);
    MessageFactory::Initialize();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SlipInterface::~SlipInterface()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initialize the Slip interface to the port, baud rate and parity.
///
///   \param[in] port The port to connect to on, ie "COM1" or "/dev/ttyUSB0".
///   \param[in] baud The baud rate to connect at, ie 9600, 115200.
///   \param[in] parity If parity of the connecting device is odd, even, no parity.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SlipInterface::Initialize(const std::string& port, const unsigned int baud, const unsigned int parity)
{
    mCallbackThread.CreateThread(SlipInterface::CallbackThread, this);
    CxUtils::SleepMs(10);
    mSerialThread.CreateThread(&SlipInterface::SerialThread, this);
    CxUtils::SleepMs(10);
    if(mSerial.Connect(port, baud, 8, parity))
        return true;
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the interface.
///
////////////////////////////////////////////////////////////////////////////////////
void SlipInterface::Shutdown()
{
    mSerialThread.StopThread();
    mCallbackThread.StopThread();
    mSerial.Disconnect();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Transmits the frame with Slip over the serial port to the device.
///
///   \param[in] frame Character array of frame to transmit.
///   \param[in] length Size of frame.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SlipInterface::Send(const unsigned char* frame, const unsigned int& length)
{
    CxUtils::Packet packet;
    packet.WriteByte(END); //Send to clear receiver buffer for new fram
    for(unsigned int i=0; i<length; i++)
    {
        switch(frame[i])
        {
            case END:
                packet.WriteByte(ESC);
                packet.WriteByte(ESC_END);
                break;
                
            case ESC:
                packet.WriteByte(ESC);
                packet.WriteByte(ESC_ESC);
                break;
                
            default:
                packet.WriteByte(frame[i]);
                break;
        }
    }
    packet.WriteByte(END); //Indicates end of frame
    if(mSerial.IsConnected())
    {
        if(mSerial.Send(packet))
        {
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the message over the serial port to the device.
///
///   \param[in] message Pointer to message structure to transmit.
///   \param[in] sleepMs Adds a sleep statement after the send operation (time in
///                      ms).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SlipInterface::Send(const Message* message, const unsigned int sleepMs)
{
    CxUtils::Packet packet;
    if(message->Write(packet))
    {
        if(Send(packet.Ptr(), packet.Length()))
        {
            if(sleepMs > 0)
                CxUtils::SleepMs(sleepMs);
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to register a callback to be run whenever a complete
///   frame is received.
///
///   \param[in] cb The callback to be run when a complete frame is received.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool SlipInterface::RegisterCallback(Callback* cb)
{
    mCallbackMutex.Lock();
    mCallbacks.insert(cb);
    mCallbackMutex.Unlock();
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to unregister a callback.
///
///   \param[in] cb The callback to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void SlipInterface::RemoveCallback(Callback* cb)
{
    std::set<Callback*>::iterator cb1;
    mCallbackMutex.Lock();
    cb1 = mCallbacks.find(cb);
    if(cb1 != mCallbacks.end())
    {
        mCallbacks.erase(cb1);
    }
    mCallbackMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all registered callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void SlipInterface::ClearCallback()
{
    mCallbackMutex.Lock();
    mCallbacks.clear();
    mCallbackMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread function that runs callbacks on received frames.
///
////////////////////////////////////////////////////////////////////////////////////
void SlipInterface::CallbackThread(void* arg)
{
    SlipInterface* slip = (SlipInterface*)arg;
    CxUtils::Packet frame;
    unsigned int loopCounter = 0;
    frame.Reserve(500);
    while(slip->mCallbackThread.QuitThreadFlag() == false)
    {
        // See if there is any frame data to process.
        bool haveFrame = false;
        frame.Clear();
        slip->mSerialMutex.Lock();
        haveFrame = slip->mFrameQueue.pop_front(&frame);
        slip->mSerialMutex.Unlock();

        if(haveFrame)
        {
            std::set<Callback*>::iterator cb;
            {
                CxUtils::Mutex::ScopedLock lock(&slip->mCallbackMutex);
                for(cb=slip->mCallbacks.begin(); cb != slip->mCallbacks.end(); cb++)
                {
                    (*cb)->ProcessFrame(frame.Ptr(), frame.Length());
                }
            }

            Message::Code messageCode = 0;
            if(Message::GetMessageCode(frame.Ptr(), frame.Length(), messageCode, false))
            {
                Message* message = MessageFactory::CreateMessage(messageCode);
                if(message)
                {
                    message->mUseStaticFormatFlag = false;
                    if(message->Read(frame))
                    {
                        message->mTimeStamp.SetCurrentTime();
                        CxUtils::Mutex::ScopedLock lock(&slip->mCallbackMutex);
                        for(cb=slip->mCallbacks.begin(); cb != slip->mCallbacks.end(); cb++)
                        {
                            (*cb)->ProcessMessage(message);
                        }
                    }
                    if(message)
                    {
                        delete message;
                    }
                }
            }
        }

        //Sleep to play nice with other threads
#ifdef WIN32
        loopCounter++;
        if(loopCounter == 10)
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
///   \brief Thread function that receives serial data and parses the slip frame.
///
////////////////////////////////////////////////////////////////////////////////////
void SlipInterface::SerialThread(void* arg)
{
    SlipInterface* slip = (SlipInterface*)arg;
    
    unsigned char c;
    CxUtils::Packet frame;
    
    while(!slip->mSerialThread.QuitThreadFlag())
    {
        c = slip->GetChar();
        switch(c)
        {
            case END:
                if(frame.Length())
                {
                    //Add frame to queue
                    slip->mSerialMutex.Lock();
                    slip->mFrameQueue.push_back(frame);
                    slip->mSerialMutex.Unlock();
                }
                frame.Clear();
                break;
            
            case ESC:
                c = slip->GetChar();
                switch(c)
                {
                    case ESC_END:
                        c = END;
                        break;
                        
                    case ESC_ESC:
                        c = ESC;
                        break;
                }
            
            default:
                if(frame.Length() < 512) //Data frame too big, data gets truncated
                {
                    frame.Write(c);
                }
                break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief An attempt at making a blocking serial call. Look into that for
///          CxUtils.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned char SlipInterface::GetChar()
{
    char c;
    //Check if at least one byte is available to be read, if not wait
    while(!mSerial.IsConnected() || !mSerial.ReadBytesAvailable())
    {
        //Add delay to play nice with other threads
		CxUtils::SleepMs(1);
    }
    mSerial.Recv(&c, 1);
    
    /*
    //Used for calculating throughput, may clean up in a class later.
    static int num = 0;
    static CxUtils::Time prevTime, currTime;
    num++;
    if(num%1000 == 0)
    {
        currTime.SetCurrentTime();
        double diff = (double) currTime.ToMs() - prevTime.ToMs();
        prevTime = currTime;
        //std::cout << "Receiving " << 1000.0/diff*1000.0 << " Bps\n";
    }
    */
    return (unsigned char)c;
}

/*  End of File */
