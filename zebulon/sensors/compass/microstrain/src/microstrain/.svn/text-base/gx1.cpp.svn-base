/////////////////////////////////////////////////////////////////////////////////////
///
/// \file GX1.cpp
/// \brief RS232 Interface class for the MicroStrain 3DM-GX1 sensor.
///
/// Author(s): Daniel Barber<br>
/// Created: 5/28/2008<br>
/// Last Modified: 5/28/2008<br>
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
#include "microstrain/gx1.h"
#include <iostream>
#include <limits.h>
#include <vector>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Compass;
using namespace MicroStrain;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
GX1::GX1(const unsigned int maxQueueSize) : mMaxQueueSize(maxQueueSize)
{
    mName="3DM-GX1";
    mContinuousModeType = Message::Null;
#ifdef GX1_IMU
    mImuSupported=true;
#endif
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GX1::~GX1()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown the connection to the device.
///
///  \return 1 on success, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
void GX1::Shutdown()
{
    GenericCompass::Shutdown();
    mMessageQueueMutex.Lock();
    Message::List::iterator msg;
    for(msg = mMessageQueue.begin();
        msg != mMessageQueue.end();
        msg++)
    {
        delete (*msg);
    }
    mMessageQueue.clear();
    mMessageQueueMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends the Set Continuous Mode command for a type of message.
///
///  \param type The type of data to send continuously.
///
///  \return True on success, otherwise failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GX1::SetContinuousMode(const Message::Type type)
{
    if(type != Message::SetContinuousMode && type != Message::Null)
    {
        CxUtils::Packet command;
        Message *response = NULL;
        command.SetByteOrder(CxUtils::CX_PACKET_BIG_ENDIAN);
        command.WriteByte((unsigned char)Message::SetContinuousMode);
        command.WriteByte(0x00);
        command.WriteByte((unsigned char)type);
        if(TransmitMessage(command, &response))
        {
            mContinuousModeType = type;
            delete response;
            return true;
        }
    }

    return false;
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sends a message, and waits for response.
///
///  \param message Command message to send.
///  \param response Double pointer to copy pointer to response to.  You must 
///                  delete this data.
///  \param timeout How long to wait for response.
///
///  \return True on success, otherwise failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GX1::TransmitMessage(const CxUtils::Packet message, 
                             MicroStrain::Message** response,
                             unsigned int timeout)
{
    unsigned char commandByte = 0;
    bool done = false;
    message.Read(commandByte, 0);
    CxUtils::Time::Stamp startTimeMs = CxUtils::GetTimeMs();
    mTransmittingMutex.Lock();
    if(response && mSerial.Send(message))
    {
        Message::List::iterator queueMessage;
        while(CxUtils::GetTimeMs() - startTimeMs < timeout && !done)
        {
            mMessageQueueMutex.Lock();
            for(queueMessage = mMessageQueue.begin();
                !done && queueMessage != mMessageQueue.end();
                )
            {
                if((*queueMessage)->GetType() == commandByte)
                {
                    (*response) = (*queueMessage);
                    queueMessage = mMessageQueue.erase(queueMessage);
                    done = true;
                }
                else
                {
                    queueMessage++;
                }
            }
            mMessageQueueMutex.Unlock();
            CxUtils::SleepMs(1);
        }        
    }

    mTransmittingMutex.Unlock();
    if(done && *response)
    {
        return true;
    }
    return false;
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread for continuously receiving data from the sensor.
///
////////////////////////////////////////////////////////////////////////////////////
void GX1::ProcessingFunction()
{
    unsigned int count = 0;
    Message* newMessage = NULL;
    unsigned int cpos = 0;
    unsigned char* ptr = NULL;
    unsigned int readCount = 0;
    unsigned int readFailure = 0;
    unsigned int tickFailure = 0;
    int endPos = -1;
    unsigned int continueCount = 0;
    unsigned int prevTimerTicks = 0;
    CxUtils::Packet buffer, packet;
    std::vector<unsigned short> tickTimers;
    buffer.Reserve(512);
    CxUtils::Time::Stamp updateTimeMs = 0;

    while(!mSerialThread.QuitThreadFlag())
    {
        if(mSerial.IsConnected())
        {
            //packet.Clear();
            // Receive data from the serial port. 
            //if(mSerial.ReadBytesAvailable() > 11 &&
            //    mSerial.Recv(packet, mSerial.ReadBytesAvailable()) > 0)
            //{
            if(mSerial.Recv(packet, 4) > 0)
            {

                readFailure = 0;
                // Add data to the end of our buffer
                buffer += packet;
                // Now search the buffer for any messages.
                ptr = (unsigned char *)buffer.Ptr();
                endPos = -1;
                
                for(cpos = 0; cpos < buffer.Length();)
                {
                    CxUtils::Time::Stamp time;
                    time=CxUtils::Time::GetUtcTimeMs();
                    // See if we recognize a command byte
                    switch(ptr[cpos])
                    {
                        case Message::SendGyroStabilizedEulerAngles:
                            {
                                //std::cout << "Send Euler\n";
                                newMessage = new SendGyroStabilizedEulerAngles();
                            }
                            break;
                        case Message::SetContinuousMode:
                            {
                                //std::cout << "Send Continous\n";
                                newMessage = new MicroStrain::SetContinuousMode();
                            }
                            break;
                        case Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector:
                            {
                                //std::cout << "Send IMU\n";
                                newMessage = new SendGyroStabilizedIMUVector();
                            }
                            break;
                        default:
                            //std::cout << "Here: " << (int)ptr[cpos] << std::endl;
                            break;
                    }

                    if(newMessage)
                    {
                        updateTimeMs = CxUtils::GetTimeMs();
                        newMessage->SetMessageTimeStamp(time);
                        
                        
                        if((readCount = newMessage->Read(&ptr[cpos], buffer.Length() - cpos)) > 0)
                        {
                            //std::cout << "New Message: GX1\n";
                            cpos += readCount;
                            endPos = (int)(cpos - 1);
                            //mValidDataFlag = true;
                        }
                        else
                        {
                            delete newMessage;
                            newMessage = NULL;
                            cpos++;
                        }
                    }

                    if(newMessage)
                    {         
                        //std::cout << "Message Type: " << std::hex << newMessage->GetType() << std::endl;
#ifdef GX1_IMU
                        if(newMessage->GetType()==Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector)
#else
                        if(newMessage->GetType()==Message::SendGyroStabilizedEulerAngles)
#endif
                        
                        {
                            mValidDataFlag = true;
#ifdef GX1_IMU
                            const MicroStrain::SendGyroStabilizedIMUVector* report = 
                                dynamic_cast<const MicroStrain::SendGyroStabilizedIMUVector*>(newMessage);
#else
                            //Old Way
                            const MicroStrain::SendGyroStabilizedEulerAngles* report = 
                                dynamic_cast<const MicroStrain::SendGyroStabilizedEulerAngles*>(newMessage);
#endif
                            if(report)
                            {

#ifdef GX1_IMU
                                mRoll=report->mRoll;
                                mPitch=report->mPitch;
                                mYaw=report->mYaw;

                                
                                
                                mAttitude.mX=report->mRoll;
                                mAttitude.mY=report->mPitch;
                                mAttitude.mZ=report->mYaw;

                                this->FixDeclination();

                                RunDefaultCallback();
                                
                                mGyroscope.mX=report->mDRoll;
                                mGyroscope.mY=report->mDPitch;
                                mGyroscope.mZ=report->mDYaw;
                                
                                mAcceleration.mX=report->mDDX;
                                mAcceleration.mY=report->mDDY;
                                mAcceleration.mZ=report->mDDZ;
                                RunIMUCallback();
#else
                                mRoll=report->mRoll;
                                mPitch=report->mPitch;
                                mYaw=report->mYaw;
                                RunDefaultCallback();
#endif
                            }
                        }
                        RunCallback(newMessage);

                        mMessageQueueMutex.Lock();
                        if(mMessageQueue.size() > mMaxQueueSize)
                        {
                            delete *(mMessageQueue.begin());
                            mMessageQueue.pop_front();
                        }
                        mMessageQueue.push_back(newMessage);
                        mMessageQueueMutex.Unlock();
                        newMessage = NULL;
                    } // If message read from buffer.
                    else
                    {
                        cpos++;
                    }
                } // Looping through buffered data.

                if(endPos > 0)
                {
                    // Delete all data up to the end of the last message read.
                    buffer.Delete((unsigned int)endPos, 0);
                }
                // If buffer is full, flush it.
                if(buffer.Length() >= 100)
                {
                    //std::cout << "Buffer Full\n";
                    std::cout.flush();
                    buffer.Clear();
                }
            }
            /*else
            {
                readFailure++;
                if(readFailure > 1000)
                {
                    std::cout << readFailure << std::endl;
                    prevTimerTicks = 0;
                    mValidDataFlag = false;
                }
            }*/
            /*
            if((CxUtils::GetTimeMs() - updateTimeMs) > 100 && mContinuousModeType != Message::Null)
            {
                SetContinuousMode(mContinuousModeType);
            }
            */
        }
        else
        {
            // Not connected on serial 
            // port or anything.
            mValidDataFlag = false;
        }

        CxUtils::SleepMs(1);
    }

    if(newMessage)
    {
        delete newMessage;
        newMessage = NULL;
    }
}

bool GX1::SetContinuousMode()
{
#ifdef GX1_IMU
    return SetContinuousMode(Message::SendGyroStabilizedEulerAnglesAndAccelAndRateVector);
#else
    return SetContinuousMode(Message::SendGyroStabilizedEulerAngles);
#endif
}

bool GX1::RunCallback(const Message* message)
{
    std::map<unsigned char,std::set<Compass::Callback*> >::iterator cb;
    mMutex.Lock();
    cb = mCallbacks.find(message->GetType());
    if(cb != mCallbacks.end())
    {
        std::set<Compass::Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            ((MicroStrain::Callback *)*cb2)->ProcessMessage(message);
        }
    }
    mMutex.Unlock();
    return true;
}

/*  End of File */
