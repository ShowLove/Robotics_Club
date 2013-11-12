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
#ifndef _ZEBULON_MAESTRO__H
#define _ZEBULON_MAESTRO__H

#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/serial.h>
#include <set>

#ifndef ZEB_MAESTRO_DLL

#ifdef WIN32
#define ZEB_MAESTRO_DLL __declspec(dllimport)
#else
#define ZEB_MAESTRO_DLL
#endif

#endif

namespace Zebulon
{
    namespace Motor
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class Maestro 
        ///  \brief Interface class for talking to a Maestro Servo Board over a 
        ///  serial port.  Also supports automatic mixing for speed/steering type
        ///  commands.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_MAESTRO_DLL Maestro
        {
        public:
            static const unsigned int MaxChannel = 12;
            static const unsigned int MaxMixer = 4;
            static const unsigned int ChannelLimit = 16;
            static const unsigned int MaxRateChange = 180; // Units/Second
            Maestro();
            ~Maestro();
            bool Initialize(const std::string& port,
                            const unsigned int delay = 20,
                            const unsigned int reversed = 0);
            void Shutdown();
            void SetChannelToRate(const unsigned int channel, const bool onFlag = true);
            bool SetValue(const unsigned int channel, const double value);
            double GetValue(const unsigned int channel) const;
            bool AddChannel(const unsigned int channel);
            bool AddMixer(const unsigned int channelX, const unsigned int channelY,
                          const unsigned int channelA, const unsigned int chanelB);
        private:
            bool WriteData(const unsigned int channel, const double value);
            static void WriteThread(void* args);
            CxUtils::Serial mSerial;                    ///<  Interface to serial port.
            CxUtils::Thread mWriteThread;               ///<  Thread object to run thread for writing to serial port.
            CxUtils::Mutex  mMutex;                     ///<  Mutex for thread protection of data.
            unsigned int mChannels[MaxChannel];         ///<  Servo Channels available.
            unsigned int mChannelsLookup[MaxChannel];   ///<  Array for lookup of channels being used.
            double mValues[2*MaxChannel];               ///<  Values to be sent to the different channels.
            double mActualValues[2*MaxChannel];         ///<  Values actually sent to the different channels.
            unsigned int mNumChannels;                  ///<  Number of channels being used.        
            unsigned int mMixChannels[MaxMixer][4];     ///<  Mixer Channels for mixing.
            unsigned int mMixLookup[MaxChannel];        ///<  Array for lookup of Mixer Channels.
            unsigned int mNumMixed;                     ///<  Number of mixer channels.        
            unsigned int mDelayTimeMs;                  ///<  How long to wait between write updates.
            std::set<unsigned int> mRateChannels;       ///<  Channels to use values as servo change rates [-100,100] percent or max rate.
            
            unsigned int mReversed;                     //if the plug is backwards
        };
    }
}

#endif
/*  End of File */
