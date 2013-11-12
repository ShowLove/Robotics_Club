/////////////////////////////////////////////////////////////////////////////////////
///
///  \file sabertooth.cpp
///  \brief Interface class for Sabertooth 5 and 10.
///
///  Author(s): Gary Stein<br>
///  Created: 2010<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#include "sabertooth/sabertooth.h"
#include <string.h>
#include <iostream>
#include <cxutils/timer.h>

using namespace std;
using namespace Zebulon;
using namespace Motor;
using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Sabertooth::Sabertooth()
{
    //two channels, set to zero
    mValues[0]=0.0;
    mValues[1]=0.0;
    mDelayTimeMs = 20;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Sabertooth::~Sabertooth()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes serial connection to Sabertooth and creates thread that
///         continuously writes to serial port.
///
///  \param[in] port Name of serial port to connect to. Example "/dev/ttyS1"
///  \param[in] delay How long to wait between writes to serial port in milliseconds.
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sabertooth::Initialize(const std::string& port,
                         const unsigned int delay,
                         const unsigned int reversed)
{
    bool result = false;
    
    Shutdown();
    
    if(mSerial.Connect(port.c_str(), 9600))
    {
        mDelayTimeMs = delay;
        if(mWriteThread.CreateThread(&Sabertooth::WriteThread, this))
        {            
            result = true;
        }
    }
    
    if(result == false)
    {
        Shutdown();
    }
    
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shuts down interface.
///
////////////////////////////////////////////////////////////////////////////////////
void Sabertooth::Shutdown()
{
    mSerial.Shutdown();
    mWriteThread.StopThread();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of an output channel. 
///
///  \param[in] channel Channel that is being sent a value. Values [0,1] mapped to 
///             specific output channels
///
///  \param[in] value Value to be sent to the channel[-100,100].
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sabertooth::SetValue(const unsigned int channel, const double value)
{
    if(channel < MaxChannel)
    {
        mMutex.Lock();
        mValues[channel]=value;
        mMutex.Unlock();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the value being sent to a specific output channel.
///
///  \param[in] channel Channel to get the value for. Values [0,7] mapped to 
///             specific output channels, values [8,15] are virtual channels used
///             for mixing.
///
///  \return Value being sent out on a channel.
///
////////////////////////////////////////////////////////////////////////////////////
double Sabertooth::GetValue(const unsigned int channel) const
{
    double value = 0;
    if(channel < MaxChannel)
    {
        mMutex.Lock();
        value = mValues[channel];
        mMutex.Unlock();
    }
    return value;
}

unsigned char Sabertooth::MakeByte(double value)
{
    double tmp;
    unsigned char out;
    //Conversion for byte
    tmp = fabs(value)/100.0*127.0;
    if(tmp > 127)
    {
        tmp = 127;
    }
    else if(tmp < 0)
    {
        tmp = 0;
    }
    
    //always worried that the unsigned
    //to signed char will break here
    out = (unsigned char)tmp;
    return out;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts channel number and value to Sabertooth serial message and sends
///         it.  Only set up for Packet Serial Mode
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sabertooth::WriteData()
{
    bool retVal;

    //construct packet here
    unsigned char output[4];
    
    //Address, should be setable
    output[0]=128;
    
    //forward, motor 1
    if(mValues[0]>=0)
    {
        output[1]=0;
    }
    //backward, motor 1
    else
    {
        output[1]=1;
    }
    output[2]=MakeByte(mValues[0]);
    output[3]=(output[0]+output[1]+output[2]) & 0x7F;

    if(mSerial.Send((char *)output, 4))
    {
        retVal = true;
    }
    else
    {
        retVal = false;
    }
    
    CxUtils::SleepMs(20);
    
    //forward, motor 2
    if(mValues[1]>=0)
    {
        output[1]=4;
    }
    //backward, motor 2
    else
    {
        output[1]=5;
    }
    output[2]=MakeByte(mValues[1]);
    output[3]=(output[0]+output[1]+output[2]) & 0x7F;

    if(mSerial.Send((char *)output, 4))
    {
        retVal &= true;
    }
    else
    {
        retVal &= false;
    }    
    
    return retVal;

}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Function that is run inside of a thread that continuously calls the 
///         WriteData method of a Sabertooth class for all channels.
///
///  \param[in] args Pointer to an instance of Sabertooth.
///
////////////////////////////////////////////////////////////////////////////////////
void Sabertooth::WriteThread(void* args)
{
    Sabertooth* sabertooth = (Sabertooth*)args;
    double updateTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    while(sabertooth->mWriteThread.QuitThreadFlag() == false)
    {
        sabertooth->mMutex.Lock();
        
        // Output fixed value.
        sabertooth->WriteData();
        
        sabertooth->mMutex.Unlock();
        updateTimeSeconds = CxUtils::Timer::GetTimeSeconds();
        CxUtils::SleepMs(sabertooth->mDelayTimeMs);
    }
}

/*  End of File */
