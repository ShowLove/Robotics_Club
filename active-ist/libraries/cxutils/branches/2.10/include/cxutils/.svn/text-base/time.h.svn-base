////////////////////////////////////////////////////////////////////////////////////
///
///  \file time.h
///  \brief Contains the Time class and methods to get the current system time.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 April 2009
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
#ifndef __CXUTILS_TIME_H
#define __CXUTILS_TIME_H

#include "cxutils/cxbase.h"
#include <string>
#include <cmath>
#include <vector>

namespace CxUtils
{
    class IP4Address;
    class Socket;
    class Mutex;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Time
    ///   \brief A simple time structure.  All time values are UTC.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Time
    {
        friend class Synchronizer;
    public: 
        typedef unsigned long long int Stamp; ///<  Typedef to store Time Stamp.
        typedef std::vector<Time> List;       ///<  Typedef for a list of Time Stamp.
        Time(const bool setCurrentTime = false);
        Time(const Time& time);
        Time(const double seconds)
        {
            *this = seconds;
        }
        virtual ~Time();
        void SetCurrentTime();
        void SetLocalTime();
        inline void Clear()
        {
            mDay = mHour = mMinute = mSecond = mMilliseconds = 0;
        }
        Stamp ToMs() const;                     
        double ToSeconds() const;                          
        static double DifferenceInSeconds(const Time& a, const Time& b) { return a - b; }
        static double DifferenceInMs(const Time& a, const Time& b) { return DifferenceInSeconds(a, b)*1000.0; }
        static Time GetUtcTime()
        {
            Time t;
            t.SetCurrentTime();
            return t;
        }
        static Time GetLocalTime()
        {
            Time t;
            t.SetLocalTime();
            return t;
        }
        static Stamp GetUtcTimeMs();
        static Stamp GetLocalTimeMs();
        std::string ToString() const;
        std::string ToString(const bool day,
                             const bool hour,
                             const bool minute,
                             const bool second,
                             const bool ms) const;
        Time& FromString(const std::string& str);
        bool IsValid() const
        {
            return mMilliseconds < 1000
                   && mSecond < 60
                   && mMinute < 60
                   && mHour < 24;

        }
        Time& operator = (const Time& time);
        // Converts a time in seconds to D:H:M:S:MS format.
        Time& operator=(const double& seconds)
        {
            mDay = mHour = mMinute = mSecond = mMilliseconds = 0;
            mMilliseconds = (unsigned int)((seconds - ((unsigned int)seconds))*1000);
            mSecond = (unsigned int)(fmod(seconds, 60.0));

            mMinute = (unsigned int)(seconds/60.0);
            mHour = mMinute/60;
            mMinute -= mHour*60;

            mDay = mHour/24;
            mHour -= mDay*24;

            return *this;
        }
        static Time Add(const Time& t1, const Time& t2);
        static Time Subtract(const Time& t1, const Time& t2);
        inline Time Add(const Time& time) const { return Add(*this, time); }
        inline Time Subtract(const Time& time) const { return Subtract(*this, time); }
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
        inline bool operator==(const double seconds) const
        {
           return fabs(ToSeconds() - seconds) < 0.001;
        }
        inline bool operator==(const Stamp milliseconds) const
        {
           return  ToMs() == milliseconds;
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
        inline bool operator!=(const double seconds) const
        {
           return fabs(ToSeconds() - seconds) > 0.001;
        }
        inline bool operator!=(const Stamp milliseconds) const
        {
           return  ToMs() != milliseconds;
        }
        inline bool operator <(const Time& time) const { return ToMs() < time.ToMs(); }
        inline bool operator <=(const Time& time) const { return ToMs() <= time.ToMs(); }
        inline bool operator >(const Time& time) const { return ToMs() > time.ToMs(); }
        inline bool operator >=(const Time& time) const  { return ToMs() >= time.ToMs(); }
        ///
        /// \return Difference in time in seconds.
        ///
        inline double operator-(const Time& time) const { return ToSeconds() - time.ToSeconds(); }
        static bool HaveExternalTime() { return mExternalTimeFlag; }
        static Time GetExternalTime();
        volatile unsigned int mDay;            ///<  Day of the month.
        volatile unsigned int mHour;           ///<  Hour of the day [0,11].
        volatile unsigned int mMinute;         ///<  Minute [0,59].
        volatile unsigned int mSecond;         ///<  Second [0,59].
        volatile unsigned int mMilliseconds;   ///<  Milliseconds [0,999].
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Synchronizer
        ///   \brief A Time Synchronizer acts as either a timer server or a time client.
        ///
        ///   If created in server mode, the Synchronizer will continuosly transmit at
        ///   UTC time at high speed over the network to a multicast address so that
        ///   clients on other computers can use that value is the system time for
        ///   applications using this library (CxUtils).
        ///
        ///   If initialized in client mode, the client attempts to receive time data
        ///   being transmitted by a Synchronizer server.  If data is available, the
        ///   Time methods in this fill will return the time values received by the
        ///   server, synchronizing time between multiple computers.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Synchronizer
        {
        public:
            Synchronizer();
            virtual ~Synchronizer();
            bool Start(const bool server,
                       const unsigned short port,
                       const IP4Address& multicastGroup/* = "225.0.0.0"*/,
                       const unsigned int udelay = 1);
            void Stop();
        protected:
            static void ServerThread(void* args);
            void* mpThreadObject;       ///< Thread data object.
            Socket* mpSocket;           ///< Socket data.
            unsigned int mDelayUs;      ///< Delay in ms. Changes based on hardware capabilities.
        };
    private:
        static volatile bool mExternalTimeFlag;///<  If true, external time data is available.
        static Time mExternalTime;             ///<  If a External Time source is available, this is the time.
        static Mutex mExternalTimeMutex;       ///<  Mutex for synchronization.
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Method for performing a sleep statement.
    ///
    ///   This function is very useful during endless loops in code to prevent
    ///   maxing out of the computers CPU by putting a 1ms sleep statement.
    ///
    ///   \param ms How long to sleep in milliseconds.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void SleepMs(const unsigned int ms);

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \return The current UTC time in milliseconds.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL unsigned long long int GetTimeMs();

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \return The current UTC time in milliseconds.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL double GetTimeSeconds();

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Gets the current system time (UTC).
    ///
    ///   \param day Day of the month. 1-31.
    ///   \param hour Hour of the day (24 hour clock) [0-23].
    ///   \param minute Minutes [0-59].
    ///   \param second Seconds [0-61].
    ///   \param msecond Milliseconds [0-999].
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void GetSystemTime(unsigned int& day,
                                    unsigned int& hour,
                                    unsigned int& minute,
                                    unsigned int& second,
                                    unsigned int& msecond);

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Gets the current system time (UTC).
    ///
    ///   \param day Day of the month. 1-31.
    ///   \param hour Hour of the day (24 hour clock) [0-23].
    ///   \param minute Minutes [0-59].
    ///   \param second Seconds [0-61].
    ///   \param msecond Milliseconds [0-999].
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void GetSystemTime(unsigned short& day,
                                    unsigned short& hour,
                                    unsigned short& minute,
                                    unsigned short& second,
                                    unsigned short& msecond);

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Sets the current system time (UTC).  You will need system
    ///          privalages to make this function call.  Advise not to use.
    ///
    ///   \param year Year 1601 - 30827.
    ///   \param month Month [1-12].
    ///   \param day Day of the month. 1-31.
    ///   \param hour Hour of the day (24 hour clock) [0-23].
    ///   \param minute Minutes [0-59].
    ///   \param second Seconds [0-61].
    ///   \param msecond Milliseconds [0-999].
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL bool SetSystemTime(unsigned int year,
                                    unsigned int month,
                                    unsigned int day,
                                    unsigned int hour,
                                    unsigned int minute,
                                    unsigned int second,
                                    unsigned int msecond);

}


#endif
/*  End of File */
