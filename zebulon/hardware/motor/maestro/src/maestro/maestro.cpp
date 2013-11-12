/////////////////////////////////////////////////////////////////////////////////////
///
///  \file maestro.h
///  \brief Interface class for talking to a Maestro Server Controller 
///  board over a serial port or USB.
///
///  Author(s): Gary Stein<br>
///  Created: 2011<br>
///  Copyright (c) 2011<br>
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
#include "maestro/maestro.h"
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
Maestro::Maestro()
{
    memset(mChannels, 0, MaxChannel*sizeof(unsigned int));
    memset(mChannelsLookup, 0, MaxChannel*sizeof(unsigned int));
    memset(mValues, 0, 2*MaxChannel*sizeof(double));
    memset(mActualValues, 0, 2*MaxChannel*sizeof(double));
    memset(mMixLookup, 0, MaxChannel*sizeof(unsigned int));
    for(unsigned int i = 0; i < MaxMixer; i++)
    {
        for(unsigned int j = 0; j < 4; j++)
        {
            mMixChannels[i][j] = 0;
        }
    }
    mNumChannels = 0;
    mNumMixed = 0;
    mDelayTimeMs = 20;
    mReversed = 0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Maestro::~Maestro()
{
    Shutdown();
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes serial connection to Mini SSC and creates thread that
///         continuously writes to serial port.
///
///  \param[in] port Name of serial port to connect to. Example "/dev/ACM0"
///  \param[in] delay How long to wait between writes to serial port in milliseconds.
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Maestro::Initialize(const std::string& port,
                         const unsigned int delay,
                         const unsigned int reversed)
{
    bool result = false;
    
    mReversed=reversed;
    
    Shutdown();
    
    //mSerial.EnableDTR(true);
    //mSerial.EnableRTS(true);
    
    if(mSerial.Connect(port.c_str(), 9600))
    {
        mDelayTimeMs = delay;
        if(mWriteThread.CreateThread(&Maestro::WriteThread, this))
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
void Maestro::Shutdown()
{
    mSerial.Shutdown();
    mWriteThread.StopThread();
    memset(mValues, 0, 2*MaxChannel*sizeof(double));
    memset(mActualValues, 0, 2*MaxChannel*sizeof(double));
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Normally when you set the value for a channel it will output
///         that value directy (like a position).  Use this method if you want
///         this interface to use the value as a continuous rate change.
///
///  For example, if your channel is set to a rate change you set a value of
///  50%, then every second the position/servo channel value will increase
///  by 50% of MaxRateChange.
///
///  \param[in] channel Channel that is outputing rate channels.
///  \param[in] onFlag If true, channel is changed to rate change, false for
///             fixed values.
///
////////////////////////////////////////////////////////////////////////////////////
void Maestro::SetChannelToRate(const unsigned int channel, const bool onFlag)
{
    std::set<unsigned int>::iterator c;
    mMutex.Lock();
    
    if(onFlag)
    {
        mRateChannels.insert(channel);
    }
    else
    {
        c = mRateChannels.find(channel);
        if(c != mRateChannels.end())
        {
            mRateChannels.erase(c);
        }
    }
    
    mMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the value of an output channel. 
///
///  \param[in] channel Channel that is being sent a value. Values [0,7] mapped to 
///             specific output channels, values [8,15] are virtual channels used
///             for mixing.
///  \param[in] value Value to be sent to the channel[-100,100].
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Maestro::SetValue(const unsigned int channel, const double value)
{
    if(channel < ChannelLimit)
    {
        mMutex.Lock();
        mValues[channel]=value;
        //if Channel>MaxChannel then it is a mixer channel
        //could be a time step behind, important?
        if(channel >= MaxChannel)
        {
            int mixer = mMixLookup[channel-MaxChannel];
            mValues[mMixChannels[mixer][2]] = mValues[mMixChannels[mixer][1]]/2.0 - mValues[mMixChannels[mixer][0]]/2.0;
            mValues[mMixChannels[mixer][3]] = mValues[mMixChannels[mixer][1]]/2.0 + mValues[mMixChannels[mixer][0]]/2.0;
        }
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
double Maestro::GetValue(const unsigned int channel) const
{
    double value = 0;
    if(channel < ChannelLimit)
    {
        mMutex.Lock();
        value = mActualValues[channel];
        mMutex.Unlock();
    }
    return value;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds channels that you want to use.
///
///  \param[in] channel Channel to get the value for. Values [0,7] mapped to 
///             specific output channels, values [8,15] are virtual channels used
///             for mixing.
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Maestro::AddChannel(const unsigned int channel)
{
    if(channel < ChannelLimit)
    {
        // Single channel
        if(channel < MaxChannel)
        {
            mMutex.Lock();
            mChannels[mNumChannels] = channel;
            mChannelsLookup[channel] = mNumChannels;
            mValues[mNumChannels] = 0.0;
            mNumChannels++;
            if(mNumChannels == MaxChannel)
            {
                cout << "Mini SSC Max Channels Reached!\n";
            }
            mMutex.Unlock();
        }
        // Mixing
        else
        {
            mMutex.Lock();
            bool notFound = true;
            for(unsigned int i = 0; i < mNumMixed; i++)
            {
                if(channel == mMixChannels[i][0] || channel == mMixChannels[i][1])
                {
                    notFound = false;
                    break;
                }
            }
            if(notFound == true)
            {
                cout << "Mini SSC Mixer Not Configured: " << channel << endl;
            }
            mMutex.Unlock();
        }
        
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates two virtual channels that can be mixed to greate ouputs to two 
///         physical channels.Example virtual channel 8 (speed) and virtual channel
///         9 (turning) output to channel 0 (left motor) and 1 (right motor).
///
///  \param[in] channelX Virtual channel to create [8-15].
///  \param[in] channelY Virtual channel to create [8-15]. 
///  \param[in] channelA Physical channel to map to [0-7]. 
///  \param[in] channelB Physical channel to map to [0-7].
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Maestro::AddMixer(const unsigned int channelX, const unsigned int channelY, const unsigned int channelA, const unsigned int channelB)
{
    mMutex.Lock();
    
    if(mNumMixed >= MaxMixer)
    {
        mMutex.Unlock();
        return false;
    }
    
    mMixChannels[mNumMixed][0] = channelX;
    mMixChannels[mNumMixed][1] = channelY;
    mMixChannels[mNumMixed][2] = channelA;
    mMixChannels[mNumMixed][3] = channelB;
    mNumMixed++;
    
    if(mNumMixed == MaxMixer)
    {
        cout << "Mini SSC Max Mixed Reached\n";
    }
    mMutex.Unlock();
    
    if(AddChannel(channelA) && AddChannel(channelB))
    {
        return true;
    }
    return false;
    
    
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts channel number and value to Mini SSC serial message and sends
///         it.
///
///  \param[in] channel Channel that is being sent a value. Values [0,7] mapped to 
///             specific output channels, values [8,15] are virtual channels used
///             for mixing.
///  \param[in] value Value to be sent to the channel[-100,100].
///
///  \return True on success, False on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Maestro::WriteData(const unsigned int channel, const double value)
{
    unsigned char output[6];
    
    output[0]=0xAA;
    output[1]=0x0C;
    output[2]=0x04;
    
    //Ross check if plug is backwards
    if(mReversed == 1)
    {
        output[3] = (char)(MaxChannel-channel-1);
    }
    else
    {
        output[3] = (char)(channel);
    }
    //Conversion for weird 2 byte value
    int tmp=floor((value/100.0*500+1500)*4.0);
    output[4]=tmp & 0x7F;
    output[5]=(tmp >> 7) & 0x7F;
 
    if(mSerial.Send((char *)output, 6))
    {
        return true;
    }
    else
    {
        return false;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Function that is run inside of a thread that continuously calls the 
///         WriteData method of a Maestro class for all channels.
///
///  \param[in] args Pointer to an instance of Maestro.
///
////////////////////////////////////////////////////////////////////////////////////
void Maestro::WriteThread(void* args)
{
    Maestro* maestro = (Maestro*)args;
    double updateTimeSeconds = CxUtils::Timer::GetTimeSeconds();
    while(maestro->mWriteThread.QuitThreadFlag() == false)
    {
        maestro->mMutex.Lock();
        for(unsigned int i = 0; i < maestro->mNumChannels; i++)
        {
            // See if we are increasing at a rate.
            if(maestro->mRateChannels.find(maestro->mChannels[i]) != maestro->mRateChannels.end())
            {
                double valueChange = 0;
                valueChange = (maestro->mValues[maestro->mChannels[i]]/100.0)*MaxRateChange*maestro->mDelayTimeMs/1000.0;
                maestro->mActualValues[maestro->mChannels[i]] += valueChange;
                if(maestro->mActualValues[maestro->mChannels[i]]  < -100.0)
                {
                    maestro->mActualValues[maestro->mChannels[i]] = -100;
                }
                else if(maestro->mActualValues[maestro->mChannels[i]]  > 100.0)
                {
                    maestro->mActualValues[maestro->mChannels[i]] = 100;
                }
                // Output fixed value.
                maestro->WriteData(maestro->mChannels[i], maestro->mActualValues[maestro->mChannels[i]]);
            }
            else
            {
                // Output fixed value.
                maestro->WriteData(maestro->mChannels[i], maestro->mValues[maestro->mChannels[i]]);
                maestro->mActualValues[maestro->mChannels[i]] = maestro->mValues[maestro->mChannels[i]];
            }
        }
        maestro->mMutex.Unlock();
        updateTimeSeconds = CxUtils::Timer::GetTimeSeconds();
        CxUtils::SleepMs(maestro->mDelayTimeMs);
    }
}

/*  End of File */
