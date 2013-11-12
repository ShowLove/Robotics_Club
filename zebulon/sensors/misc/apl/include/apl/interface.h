/////////////////////////////////////////////////////////////////////////////////////
///
/// \file interface.h
/// \brief Acoustic Pinger Locator Interface.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 4/12/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu<br>
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
#ifndef _ZEBULON_APL_INTERFACE__H
#define _ZEBULON_APL_INTERFACE__H

#include <iostream>

#include <mcu/interface.h>
#include <mcu/messagefactory.h>
#include <cxutils/cxutils.h>
#include <display/display.h>
#include "apl/channel.h"
#include <mcu/messages/custom/customcodes.h>


#include <fftw3.h>
#include <filter/filter.h>

#include <set>

namespace Zebulon
{
    namespace APL
    {
        class Callback
        {
            public:
                Callback(){};
                virtual ~Callback(){};
                virtual int LockYaw() = 0;
                virtual int SetChannel(double *Real,double *Imag,double *Freq,double Gain) = 0;
        };
        
        class Interface : public MCU::Interface::Callback
        {
        public:
            Interface();
            ~Interface();
            
            bool Initialize(MCU::Interface* mcu,
                            std::string rawPort="",
                            bool display=false,
                            int rawBaud=57600);
            
            void Shutdown();
            
            static const unsigned int MaxSamples = 512; ///<  Sample size
            static const unsigned int PacketSize = 12;
            static const double CenterValue = 2048.0;
            static const double NormalRange = 5.0;
            
            static const double SampleFreq = 694400;
            
            static const double TargetRange = 1000;
            
            virtual void ProcessMessage(const MCU::Message* message);
            
            void DisplayPingTime(int zone);
            void SetPinger(double Frequency,int SampleMode=1,int PingThreshold=50,int PingCount=10,double Q=64,int DCDecay=1,int PktoPkDecay=1, int ArbiterMode=0,double Min=25,double Max=75);
            
            void Print();

            void RequestDebugData();
            
            void SetCallback(APL::Callback *callback);

        private:
            MCU::Interface* mMCU;    ///<  The MCU Interface for the APL Subsystem
            
            CxUtils::Serial mSerial; ///<  The raw data port for the APL Subsystem
            
            CxUtils::Thread mRawDataThread;           ///<  Manages/creates Raw Data Thread.
            static void RawDataThread(void* arg);     ///<  Responsible for receiving raw data.
            
            CxUtils::Thread mProcessThread;           ///<  Manages/creates Process Data Thread.
            static void ProcessThread(void* arg);     ///<  Responsible for processing data after transferred to double buffer.
            
            CxUtils::Thread mRequestDataThread;       ///<  Thread that constantly requests data from the APL Subsystem.
            static void RequestDataThread(void* arg);
            
            bool mDisplay;                            ///<  Flag to display the graphs.
            
            
            CxUtils::Mutex mDataMutex;                ///<  Mutex for thread protection when dealing with sampled data.
            unsigned int mAD0[MaxSamples];
            unsigned int mAD1[MaxSamples];
            unsigned int mAD2[MaxSamples];
            unsigned int mAD3[MaxSamples];
            unsigned int mAD4[MaxSamples];
            
            //Double Buffer
            unsigned int mDBAD0[MaxSamples];
            unsigned int mDBAD1[MaxSamples];
            unsigned int mDBAD2[MaxSamples];
            unsigned int mDBAD3[MaxSamples];
            unsigned int mDBAD4[MaxSamples];
            
            int mADPos;   ///<  Stores current AD sample position.
            
            unsigned int ExtractMessage(const unsigned char* buffer, const unsigned int length, CxUtils::Packet& message);
            int ExtractDataFromMessage(const unsigned char *Buffer);
            int TransferToBuffer();
            
            int mNumTranfers;              ///<  Stores number of successful complete transfers.
            int mNewData;                  ///<  Flag to indicate new data has been received between the receive raw data thread and process data thread.
            CxUtils::Mutex mProcessMutex;  ///<  Mutex for thread protection when dealing with processing the data.
            
            IplImage *mTimeImage;
            void DisplayTime();
            
            IplImage *mFreqImage;
            void DisplayFreq();
            
            IplImage *mPingTimeImage;
            
            void MakeComplex(unsigned int *mAD,fftw_complex *mADTime);
            
            fftw_complex *mAD0Time;
            fftw_complex *mAD1Time;
            fftw_complex *mAD2Time;
            fftw_complex *mAD3Time;
            fftw_complex *mAD4Time;
            
            //fft domain
            fftw_complex *mAD0Freq;
            fftw_complex *mAD1Freq;
            fftw_complex *mAD2Freq;
            fftw_complex *mAD3Freq;
            fftw_complex *mAD4Freq;

            fftw_plan mFFTPlan0;
            fftw_plan mFFTPlan1;
            fftw_plan mFFTPlan2;
            fftw_plan mFFTPlan3;
            fftw_plan mFFTPlan4;
            
            int mTimes[Channel::NumChannels];

            Utility::Filter *mZoneFilter;
            
            Channel mChannel[Channel::NumChannels];
            
            void PkToPkReceived(MCU::ReportPkToPk* pk);
            
            CxUtils::Mutex mRequestMutex;
            bool mRequestFlag;
            
            void Calc();
            APL::Callback *mCallback;
            
            //for Self correction loop
            double mFrequency;
            double mGain;
            int mMax,mMin;
            double mOldError;
            double mLastTime;
            
            void UpdateGain();
            
        };
    }
}

#endif //_ZEBULON_APL_INTERFACE__H
