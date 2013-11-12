////////////////////////////////////////////////////////////////////////////////////
///
///  \file time.h
///  \brief Contains the Time class which is used to store time information in
///  the JAUS library.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 27 September 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#ifndef __JAUS_CORE_TIME__H
#define __JAUS_CORE_TIME__H

#include <cxutils/time.h>
#include "jaus/core/types.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Simple Time structure.  Contains methods to read/write time data
    ///          stored in JAUS messages.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_CORE_DLL Time : public CxUtils::Time
    {
    public:
        typedef CxUtils::Time::Stamp Stamp;
        Time(const bool setCurrentTime = false) : CxUtils::Time(setCurrentTime) {}
        Time(const JAUS::Time& t) : CxUtils::Time() { *this = t; }
        Time(const CxUtils::Time& t) : CxUtils::Time() { *this = t; }
        ~Time() {}
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Creates a UInt of the time information in the format defined by the
        ///          JAUS RA.
        ///
        ///   Bits 0-9: milliseconds, range 0, 999<br>
        ///   Bits 10-15: Seconds, range 0, 59<br>
        ///   Bits 16-21: Minutes, range 0, 59<br>
        ///   Bits 22-26: Hour (24 hour clock), range 0, 23<br>
        ///   Bits 27-31: Day, range 1, 31<br>
        ///
        ///   \return The timestamp data in JAUS RA format.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        UInt ToUInt() const
        {
            UInt tstamp = 0;
            tstamp |= (UInt)(mDay)    << 27;
            tstamp |= (UInt)(mHour)   << 22;
            tstamp |= (UInt)(mMinute) << 16;
            tstamp |= (UInt)(mSecond) << 10;
            tstamp |= mMilliseconds;
            return tstamp;
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Sets the time using a UInt in JAUS Standard Time format.
        ///
        ///   \param tstamp Bits 0-9: milliseconds, range 0, 9999
        ///                 Bits 10-15: Seconds, range 0, 59
        ///                 Bits 16-21: Minutes, range 0, 59
        ///                 Bits 22-26: Hour (24 hour clock), range 0, 23
        ///                 Bits 27-31: Day, range 1, 31
        ///
        ///   \return Current instance of the Time data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        inline Time& SetTime(const UInt tstamp)
        {
            mMilliseconds = (UInt)(tstamp & 0x3FF);
            mSecond = (UInt)((tstamp >> 10) & 0x3F);
            mMinute = (UInt)((tstamp >> 16) & 0x3F);
            mHour = (UInt)((tstamp >> 22) & 0x1F);
            mDay = (UInt)((tstamp >> 27) & 0x1F);
            return *this;
        }
        inline Time& operator=(const Time& time)
        {
            mDay = time.mDay;
            mHour = time.mHour;
            mMinute = time.mMinute;
            mSecond = time.mSecond;
            mMilliseconds = time.mMilliseconds;
            return *this;
        }
        inline Time& operator=(const CxUtils::Time& time)
        {
            mDay = time.mDay;
            mHour = time.mHour;
            mMinute = time.mMinute;
            mSecond = time.mSecond;
            mMilliseconds = time.mMilliseconds;
            return *this;
        }
        inline bool operator==(const Time& time) const
        {
            if(mDay == time.mDay &&
               mHour == time.mHour &&
               mMinute == time.mMinute &&
               mSecond == time.mSecond &&
               mMilliseconds == time.mMilliseconds)
           {
               return true;
           }
           return false;
        }
        inline bool operator!=(const Time& time) const
        {
            if(mDay != time.mDay ||
               mHour != time.mHour ||
               mMinute != time.mMinute ||
               mSecond != time.mSecond ||
               mMilliseconds != time.mMilliseconds)
           {
               return true;
           }
           return false;
        }
        inline bool operator <(const Time& time) const { return ToSeconds() < time.ToSeconds(); }
        inline bool operator <=(const Time& time) const { return ToSeconds() <= time.ToSeconds(); }
        inline bool operator >(const Time& time) const { return ToSeconds() > time.ToSeconds(); }
        inline bool operator >=(const Time& time) const  { return ToSeconds() >= time.ToSeconds(); } 
    };
}

#endif
/*  End of File */
