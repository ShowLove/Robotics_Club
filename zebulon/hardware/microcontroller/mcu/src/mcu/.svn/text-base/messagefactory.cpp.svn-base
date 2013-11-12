//////////////////////////////////////////////////////////////////////////////////////
///
/// \file messagefactory.cpp
/// \brief Message factory methods for dynamic creation of any type of MCU message.
///
/// Author(s): Daniel Barber<br>
/// Created: 4/19/2009<br>
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
#include "mcu/messagefactory.h"
#include <cxutils/mutex.h>
#include <map>

using namespace Zebulon;
using namespace MCU;

CxUtils::Mutex gMutex;
std::map<Message::Code, Message*> gMessages;

bool MessageFactory::mIsInitialized=false;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds the default messages (Ping, Pong, and QueryReport) to the
///   factory.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageFactory::Initialize()
{
    if(mIsInitialized==false)
    {
        AddToFactory(new Ping());
        AddToFactory(new Pong());
        AddToFactory(new QueryReport());
        AddToFactory(new SetDigitalOutputs());
        AddToFactory(new ReportDigitalInputs());
        AddToFactory(new ReportAnalogInputs());
        AddToFactory(new ReportEncoders());
        AddToFactory(new SetPingerFrequency());
        AddToFactory(new SetServoOutputs());
        AddToFactory(new SetAnalogOutputs());
        AddToFactory(new SetMotorOutput());
        
        AddToFactory(new SetAPL());
        AddToFactory(new ReportDCOffset());
        AddToFactory(new ReportPkToPk());
        AddToFactory(new ReportPingTime());
        AddToFactory(new ReportDPotValues());
        AddToFactory(new ReportSampleFreq());
        AddToFactory(new ReportSamplePeriod());
        AddToFactory(new ReportFFT());
        
        AddToFactory(new ReportMotorStatus());
        AddToFactory(new SetMotorAddress());
        
        AddToFactory(new ReportID());
        
        mIsInitialized=true;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a message based on the code passed.
///
///   \param[in] code The message type to create.
///
///   \return Pointer to newly created message, NULL if it hasn't been 
///   added to the factory.
///
////////////////////////////////////////////////////////////////////////////////////
Message* MessageFactory::CreateMessage(const Message::Code code)
{
    std::map<Message::Code, Message*>::iterator msg;
    Message* newMessage = NULL;
    gMutex.Lock();
    msg = gMessages.find(code);
    if(msg != gMessages.end())
    {
        newMessage = msg->second->Clone();
    }
    gMutex.Unlock();
    
    return newMessage;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a message to the Factory so new ones can be created.
///
///   \param[in] message Message to add to factory (takes ownership of pointer).
///
///   \return 1 on success, 0 if a message with the same code already is added.
///
////////////////////////////////////////////////////////////////////////////////////
int MessageFactory::AddToFactory(Message* message)
{
    std::map<Message::Code, Message*>::iterator msg;
    int result = 0;
    
    gMutex.Lock();
    msg = gMessages.find(message->mMessageCode);
    if(msg == gMessages.end())
    {
        result = 1;
        gMessages[message->mMessageCode] = message;
    }
    gMutex.Unlock();
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all messages in the factory.
///
////////////////////////////////////////////////////////////////////////////////////
void MessageFactory::Clear()
{
    std::map<Message::Code, Message*>::iterator msg;
    
    gMutex.Lock();
    for(msg = gMessages.begin(); msg != gMessages.end(); msg++)
    {
        delete msg->second;
    }
    gMessages.clear();
    gMutex.Unlock();
}


/*  End of File */
