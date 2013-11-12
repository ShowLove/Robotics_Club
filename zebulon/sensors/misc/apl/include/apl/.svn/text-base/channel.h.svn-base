/////////////////////////////////////////////////////////////////////////////////////
///
/// \file channle.h
/// \brief Analog Channel Class for APL.
///
/// Author(s): Jonathan Mohlenhoff<br>
/// Created: 4/20/2010<br>
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
#ifndef _ZEBULON_APL_CHANNEL__H
#define _ZEBULON_APL_CHANNEL__H

#include <mcu/messagefactory.h>
#include <cxutils/cxutils.h>

namespace Zebulon
{
    namespace APL
    {
        class Channel
        {
        public:
            Channel();
            ~Channel();
            
            static const unsigned int MaxSamples = 512;
            static const unsigned int NumChannels = 5;
            
            double DPotToVolts(unsigned char value);
            double VoltsToDb(double value);
            double SampleToVolts(unsigned short value);
            
            void Print();
            
            int GetPingTime();
            static double GetSampleFrequency();
            static double GetSamplePeriod();

            void DCOffsetReceived(MCU::ReportDCOffset* dc);
            void DPotValuesReceived(MCU::ReportDPotValues* pot);
            void PingTimeReceived(MCU::ReportPingTime* pingTime);
            void PkToPkReceived(MCU::ReportPkToPk* pk);
            void SampleFreqReceived(MCU::ReportSampleFreq* sampleFreq);
            void SamplePeriodReceived(MCU::ReportSamplePeriod* samplePer);
            
        private:
            static CxUtils::Mutex mMutex;
            
            unsigned char mVGNA;    ///< Value of the pot that sets VGNA, range [0,255] maps to [0V,5V]
            unsigned char mVGNB;    ///< Value of the pot that sets VGNB, range [0,255] maps to [0V,5V]
            unsigned char mAdder;   ///< Value of the pot that sets Adder, range [0,255] maps to [0V,5V]
            unsigned char mVref;    ///< Value of the pot that sets Vref, range [0,255] maps to [0V,5V]
            
            unsigned short mSignalMax;
            unsigned short mSignalMin;
            unsigned short mDCOffset;
            int mPingTime;
            
            static double mSampleFrequency;  ///< Sample Frequency of ADC in Hz
            static double mSamplePeriod;     ///< Sample Perioud of ADC in sec
            
            unsigned int mSample[MaxSamples];
            unsigned int mSampleBuffer[MaxSamples];
        };
    }
}

#endif //_ZEBULON_APL_CHANNEL__H
