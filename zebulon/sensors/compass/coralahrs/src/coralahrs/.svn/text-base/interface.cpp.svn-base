//////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.cpp
/// \brief Interface to Coral AHRS Digital Compass.
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
#include "coralahrs/interface.h"
#include <vector>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Compass;
using namespace CoralAHRS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::Interface()
{
    mName = "CoralAHRS";
    mImuSupported=true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Interface::~Interface()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the compass to continuous output mode.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::SetContinuousMode()
{
    CoralAHRS::SetOutputMode outputMode;

    outputMode.mMode = (unsigned char) (CoralAHRS::SetOutputMode::DataEulerAndSensors | CoralAHRS::SetOutputMode::Calibrated);
    Send(&outputMode);

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the message over the serial port to the device.
///
///   \param[in] message Pointer to message structure to transmit.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::Send(const Message* message)
{
    CxUtils::Packet packet;
    if(message->Write(packet))
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
///   \brief This method is used to create an instance of any type of
///   Coral AHRS message.
///
///   \param[in] type The type of message to create.
///
///   \return Pointer to created message (you must delete it), NULL if type is
///   unknown.
///
////////////////////////////////////////////////////////////////////////////////////
Message* Interface::CreateMessage(const unsigned char type)
{
    Message* msg = NULL;

    switch(type)
    {
    case Message::DataQuat:
        msg = new DataQuat();
        break;
    case Message::DataEuler:
        msg = new DataEuler();
        break;
    case Message::DataEulerAndSensors:
        msg = new DataEulerAndSensors();
        break;
    case Message::Configuration:
        msg = new Configuration();
        break;
    case Message::Pong:
        msg = new Pong();
        break;
    case Message::SetOutputMode:
        msg = new SetOutputMode;
        break;
    case Message::RequestConfiguration:
        msg = new RequestConfiguration;
        break;
    case Message::Ping:
        msg = new Ping();
        break;    
    default:
        msg = NULL;
        break;
    }

    return msg;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs a thread which continuously receives and parsed data from
///   serial port.
///
////////////////////////////////////////////////////////////////////////////////////
void Interface::ProcessingFunction()
{
    while(mSerialThread.QuitThreadFlag() == false)
    {
        if(mSerial.IsConnected())
        {
            if(mSerial.Recv(mSmallBuffer, 4) > 0)
            {
                // Add data to larger buffer.
                mLargeBuffer += mSmallBuffer;
                // Look for a message within the buffered data.
                unsigned int endPos = 0;
                unsigned char* buffer = (unsigned char*)mLargeBuffer.Ptr();
                for(unsigned int pos = 0; pos < mLargeBuffer.Length(); pos++)
                {
                    if(buffer[pos] == CoralMessageHeader)
                    {
                        CxUtils::Packet extractedMessage;
                        unsigned char messageType;
                        if(Message::ExtractMessage(&buffer[pos],mLargeBuffer.Length() - pos, messageType, extractedMessage) > 0)
                        {
                            //std::cout << "Test\n" << std::hex << (int)messageType << std::dec << " " << extractedMessage.Length() <<std::endl;
                            pos += extractedMessage.Length() - 1;
                            endPos = pos + 1;
                            
                            // Do something with extracted message data.
                            Message* newMessage = Interface::CreateMessage(messageType);
                            if(newMessage)
                            {
                                if(newMessage->Read(extractedMessage))
                                {
                                    //add special type for generic callback
                                    if(newMessage->mMessageType == CoralAHRS::Message::DataEulerAndSensors)
                                    {
                                        mValidDataFlag = true;
                                        const CoralAHRS::DataEulerAndSensors* data = dynamic_cast<const CoralAHRS::DataEulerAndSensors*>(newMessage);
                                        if(data)
                                        {
                                            mMutex.Lock();
                                            mYaw=data->mEulerAngles.mZ;
                                            mPitch=data->mEulerAngles.mY;
                                            mRoll=data->mEulerAngles.mX;
                                            mMutex.Unlock();
                                            RunDefaultCallback();
                                            
                                            mMutex.Lock();
                                            mAttitude=data->mEulerAngles;
                                            mGyroscope=data->mGyros;
                                            mAcceleration=data->mAccelerometers;
                                            mMutex.Unlock();
                                            RunIMUCallback();
                                        }
                                    }
                                    // Generic callback.
                                    RunCallback(newMessage);
                                }
                                else
                                {
                                    delete newMessage;
                                    newMessage = NULL;
                                }
                            }
                        }
                    }
                }
                
                // Clear out all data in buffer up until after the last message read.
                if(endPos > 0)
                {
                    mLargeBuffer.Delete(endPos, 0);
                }
                // Clear bad data.
                if(mLargeBuffer.Length() >= 500)
                {
                    mLargeBuffer.Clear();
                }
            }
        }
        CxUtils::SleepMs(1);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Runs all callbacks registered.
///
////////////////////////////////////////////////////////////////////////////////////
bool Interface::RunCallback(const Message* message)
{
    std::map<unsigned char,std::set<Compass::Callback*> >::iterator cb;
    mMutex.Lock();
    cb = mCallbacks.find(message->mMessageType);
    if(cb != mCallbacks.end())
    {
        std::set<Compass::Callback*>::iterator cb2;
        for(cb2 = cb->second.begin();
            cb2 != cb->second.end();
            cb2++)
        {
            ((CoralAHRS::Callback *)*cb2)->ProcessMessage(message);
        }
    }
    mMutex.Unlock();
    return true;
}

/*  End of File */
