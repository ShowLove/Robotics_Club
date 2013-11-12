/////////////////////////////////////////////////////////////////////////////////////
///
/// \file channel.cpp
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

#include "apl/channel.h"
#include <iostream>

using namespace Zebulon;
using namespace APL;

double Channel::mSampleFrequency = 0.0;
double Channel::mSamplePeriod = 0.0;

CxUtils::Mutex Channel::mMutex;

Channel::Channel()
{
    mVGNA = 0;
    mVGNB = 0;
    mAdder = 0; 
    mVref = 0;
    
    mSignalMax = 0;
    mSignalMin = 0;
    mDCOffset = 0;
    
    mPingTime = 0;
}

Channel::~Channel()
{
    
}

double Channel::DPotToVolts(unsigned char value)
{
    return value / 255.0 * 5.0; //For 5 volts on the Hi and Lo pins of the digital pot
}

double Channel::VoltsToDb(double value)
{
    return value * 20.0; //Per AD605 Data Sheet
}

double Channel::SampleToVolts(unsigned short value)
{
    return value / 4095.0 * 3.3;
}

void Channel::Print()
{
    mMutex.Lock();
    std::cout << "\tVGNa: " << (int) mVGNA << ", " << DPotToVolts(mVGNA) << "V, " << VoltsToDb(DPotToVolts(mVGNA)) << "dB ";
    std::cout << "\tVGNb: " << (int) mVGNB << ", " << DPotToVolts(mVGNB) << "V, " << VoltsToDb(DPotToVolts(mVGNB)) << "dB\n";
    std::cout << "\tAdder: " << (int) mAdder << ", " << DPotToVolts(mAdder) << "V ";
    std::cout << "\tSignal DC: " << (int) mDCOffset << ", " << SampleToVolts(mDCOffset) << "V\n";
    std::cout << "\tSignal Max: " << (int) mSignalMax << ", " << SampleToVolts(mSignalMax) << "V ";
    std::cout << "\tSignal Min: " << (int) mSignalMin << ", " << SampleToVolts(mSignalMin) << "V\n";
    //std::cout << "\tSample Freq: " << mSampleFrequency << " Hz\n";
    //std::cout << "\tSample Period: " << mSamplePeriod << " sec\n";
    mMutex.Unlock();
}

int Channel::GetPingTime()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    return mPingTime;
}

void Channel::DCOffsetReceived(MCU::ReportDCOffset* dc)
{
    mMutex.Lock();
    mDCOffset = dc->mValue;
    mMutex.Unlock();
}

void Channel::DPotValuesReceived(MCU::ReportDPotValues* pot)
{
    mMutex.Lock();
    switch(pot->mChannel%2)
    {
        case 0:
        {
            mVGNB = pot->mValue0;
            mVGNA = pot->mValue1;
            break;
        }
        
        case 1:
        {
            mAdder = pot->mValue0;
            mVref = pot->mValue1;
        }
    }
    mMutex.Unlock();
}

void Channel::PingTimeReceived(MCU::ReportPingTime* pingTime)
{
    mMutex.Lock();
    mPingTime = pingTime->mTime;
    mMutex.Unlock();
}

void Channel::PkToPkReceived(MCU::ReportPkToPk* pk)
{
    mMutex.Lock();
    mSignalMax = pk->mMax;
    mSignalMin = pk->mMin;
    mMutex.Unlock();
}

void Channel::SampleFreqReceived(MCU::ReportSampleFreq* sampleFreq)
{
    mMutex.Lock();
    mSampleFrequency = sampleFreq->mSampleFrequency;
    mMutex.Unlock();
}


void Channel::SamplePeriodReceived(MCU::ReportSamplePeriod* samplePer)
{
    mMutex.Lock();
    mSamplePeriod = samplePer->mSamplePeriod / 1000000000.0;
    mMutex.Unlock();
}


double Channel::GetSampleFrequency()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    return mSampleFrequency;
}


double Channel::GetSamplePeriod()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    return mSamplePeriod;
}


