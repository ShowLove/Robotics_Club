/// ////////////////////////////////////////////////////////////////////////////////////
///
///  \file time.cpp
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
#include "cxutils/time.h"
#include <cassert>
#include <cstdlib>
#include <cstring>
#include "cxutils/networking/udpclient.h"
#include "cxutils/networking/udpserver.h"
#include "cxutils/thread.h"

#ifdef WIN32
#include "cxutils/timer.h"
#include <windows.h>
#include <conio.h>
#include <ctime>
#else
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <assert.h>
#include <cstdio>
#include <ctime>
#endif
#include <iostream>

/**
 * number of seconds from 1 Jan. 1601 00:00 to 1 Jan 1970 00:00 UTC
 */
#define EPOCH_DIFF 11644473600LL

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
void CxUtils::SleepMs(const unsigned int ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    usleep(ms*1000);
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method gets the current system time including Day, Hour, Minute,
///          Seconds, and Milliseconds.  This is not the number of ms since
///          1 Jan 1970.
///
///   \return The current UTC time in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned long long int CxUtils::GetTimeMs()
{
    unsigned int day, hour, min, sec, ms;
    CxUtils::GetSystemTime(day, hour, min, sec, ms);
    return (unsigned long long int)(day*86400000) +
           (unsigned long long int)(ms) +
           (unsigned long long int)(sec*1000)  +
           (unsigned long long int)(min*60000) +
           (unsigned long long int)(hour*3600000);
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the number of milliseconds since 1 Jan 1970.
///
///   \return The UTC time in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned long long int CxUtils::GetTimeMsSince1Jan1970()
{
#ifdef WIN32
    SYSTEMTIME systemTime;
    ::GetSystemTime(&systemTime);

    FILETIME fileTime;
    ::SystemTimeToFileTime( &systemTime, &fileTime );

    ULARGE_INTEGER uli;
    uli.LowPart = fileTime.dwLowDateTime;
    uli.HighPart = fileTime.dwHighDateTime;

    ULONGLONG systemTimeIn_ms( uli.QuadPart/10000 );
    return (unsigned long long int)(systemTimeIn_ms - EPOCH_DIFF);
#else
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (unsigned long long int)(1000.0*tv.tv_sec + tv.tv_usec/1000.0);

    /*
    msecond = (unsigned short)(tv.tv_usec/1000.0);
    struct tm *timeinfo;
    timeinfo = gmtime(&tv.tv_sec);

    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;
    */
#endif
}
/*
{
    unsigned int day, hour, min, sec, ms;
    CxUtils::GetSystemTime(day, hour, min, sec, ms);
    return (unsigned long long int)(day*86400000) +
           (unsigned long long int)(ms) +
           (unsigned long long int)(sec*1000)  +
           (unsigned long long int)(min*60000) +
           (unsigned long long int)(hour*3600000);
}
*/

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the number of seconds since 1 Jan 1970.
///
///   \return The UTC time in seconds.
///
////////////////////////////////////////////////////////////////////////////////////
double CxUtils::GetTimeSecondsSince1Jan1970() { return GetTimeMsSince1Jan1970()/1000.0; }

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method gets the current system time including Day, Hour, Minute,
///          Seconds, and Milliseconds.  This is not the number of ms since
///          1 Jan 1970.
///
///   \return The current UTC time in seconds.
///
////////////////////////////////////////////////////////////////////////////////////
double CxUtils::GetTimeSeconds() { return GetTimeMs()/1000.0; }


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
void CxUtils::GetSystemTime(unsigned int& day,
                            unsigned int& hour,
                            unsigned int& minute,
                            unsigned int& second,
                            unsigned int& msecond)
{
    CxUtils::Time cxtime;

    if(cxtime.HaveExternalTime())
    {
        cxtime = cxtime.GetExternalTime();
        day = cxtime.mDay;
        hour = cxtime.mHour;
        minute = cxtime.mMinute;
        second = cxtime.mSecond;
        msecond = cxtime.mMilliseconds;
        return;
    }
#ifdef WIN32
    SYSTEMTIME t;
    ::GetSystemTime(&t);
    day = t.wDay;
    hour = t.wHour;
    minute = t.wMinute;
    second = t.wSecond;
    msecond = t.wMilliseconds;
#else
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    msecond = (unsigned short)(tv.tv_usec/1000.0);
    timeinfo = gmtime(&tv.tv_sec);

    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;
#endif
}

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
void CxUtils::GetSystemTime(unsigned short& day,
                            unsigned short& hour,
                            unsigned short& minute,
                            unsigned short& second,
                            unsigned short& msecond)
{
#ifdef WIN32
    SYSTEMTIME t;
    ::GetSystemTime(&t);
    day = t.wDay;
    hour = t.wHour;
    minute = t.wMinute;
    second = t.wSecond;
    msecond = t.wMilliseconds;
#else
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    msecond = (unsigned short)(tv.tv_usec/1000.0);
    timeinfo = gmtime(&tv.tv_sec);

    day = timeinfo->tm_mday;
    hour = timeinfo->tm_hour;
    minute = timeinfo->tm_min;
    second = timeinfo->tm_sec;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current system time (UTC).
///
///   \param year Year 1900- 30827.
///   \param month Month [1-12].
///   \param day Day of the month. 1-31.
///   \param hour Hour of the day (24 hour clock) [0-23].
///   \param minute Minutes [0-59].
///   \param second Seconds [0-61].
///   \param msecond Milliseconds [0-999].
///
////////////////////////////////////////////////////////////////////////////////////
bool CxUtils::SetSystemTime(unsigned int year,
                            unsigned int month,
                            unsigned int day,
                            unsigned int hour,
                            unsigned int minute,
                            unsigned int second,
                            unsigned int msecond)
{
#ifdef WIN32
    SYSTEMTIME t;
    ::GetSystemTime(&t);
    t.wYear = year;
    t.wDay = day;
    t.wHour = hour;
    t.wMinute = minute;
    t.wSecond = second;
    t.wMilliseconds = msecond;

    struct tm *timeinfo;
    time_t rawtime;
    time(&rawtime);
    timeinfo = gmtime ( &rawtime );
    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;
    mktime(timeinfo);
    t.wDayOfWeek = timeinfo->tm_wday;

    return ::SetSystemTime(&t) ? true : false;
#else
    struct timeval tv;
    struct tm *timeinfo;
    struct timezone tz;
    time_t rawtime;

    gettimeofday(&tv, &tz);

    time(&rawtime);
    timeinfo = gmtime ( &rawtime );

    timeinfo->tm_year = year - 1900;
    timeinfo->tm_mon = month;
    timeinfo->tm_mday = day;
    timeinfo->tm_hour = hour;
    timeinfo->tm_min = minute;
    timeinfo->tm_sec = second;
    tv.tv_sec = mktime(timeinfo);
    tv.tv_usec = (__suseconds_t)(msecond*1000);
    int result = 0;
    result = settimeofday(&tv, &tz);
    return result == 0 ? true : false;
#endif
}


using namespace CxUtils;

volatile bool Time::mExternalTimeFlag = false;
Time Time::mExternalTime = Time();
Mutex Time::mExternalTimeMutex;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param setCurrentTime If true, initialized with current UTM time.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Time(const bool setCurrentTime)
{
    mDay = 0;
    mHour = 0;
    mMinute = 0;
    mSecond = 0;
    mMilliseconds = 0;

    if(setCurrentTime)
    {
        SetCurrentTime();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Time(const Time& time)
{
    mDay = time.mDay;
    mHour = time.mHour;
    mMinute = time.mMinute;
    mSecond = time.mSecond;
    mMilliseconds = time.mMilliseconds;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Time::~Time()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current UTC time using CxUtils::GetSystemTime method.
///
////////////////////////////////////////////////////////////////////////////////////
void Time::SetCurrentTime()
{
    CxUtils::GetSystemTime((unsigned int &)mDay,
                           (unsigned int &)mHour,
                           (unsigned int &)mMinute,
                           (unsigned int &)mSecond,
                           (unsigned int &)mMilliseconds);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current local time.
///
////////////////////////////////////////////////////////////////////////////////////
void Time::SetLocalTime()
{
#ifdef WIN32
    SYSTEMTIME t;
    ::GetLocalTime(&t);
    mDay = (unsigned int)t.wDay;
    mHour = (unsigned int)t.wHour;
    mMinute = (unsigned int)t.wMinute;
    mSecond = (unsigned int)t.wSecond;
    mMilliseconds = (unsigned int)t.wMilliseconds;
#else
    struct timeval tv;
    struct tm *timeinfo;
    gettimeofday(&tv, NULL);
    mMilliseconds = (unsigned short)(tv.tv_usec/1000.0);
    timeinfo = localtime ( &tv.tv_sec );

    mDay = timeinfo->tm_mday;
    mHour = timeinfo->tm_hour;
    mMinute = timeinfo->tm_min;
    mSecond = timeinfo->tm_sec;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the time data to a value in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Stamp Time::ToMs() const
{
    return (unsigned long long int)(mDay*86400000) +
           (unsigned long long int)(mMilliseconds) +
           (unsigned long long int)(mSecond*1000)  +
           (unsigned long long int)(mMinute*60000) +
           (unsigned long long int)(mHour*3600000);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the time data to a value in seconds.
///
////////////////////////////////////////////////////////////////////////////////////
double Time::ToSeconds() const
{
    return (double)(mDay*86400.0) +
           (double)(mMilliseconds/1000.0) +
           (double)(mSecond)  +
           (double)(mMinute*60) +
           (double)(mHour*3600);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The current UTC time in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Stamp Time::GetUtcTimeMs()
{
    return CxUtils::GetTimeMs();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The current local time in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Stamp Time::GetLocalTimeMs()
{
    Time t;
    t.SetLocalTime();
    return t.ToMs();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts time to a string format.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Time::ToString(const bool day,
                           const bool hour,
                           const bool minute,
                           const bool second,
                           const bool ms) const
{
    char buffer[512];
    std::string ret = "";

    if(day)
    {
        sprintf(buffer, ".%02d", mDay);
        ret += buffer;
    }
    if(hour)
    {
        sprintf(buffer, ".%02d", mHour);
        ret += buffer;
    }
    if(minute)
    {
        sprintf(buffer, ".%02d", mMinute);
        ret += buffer;
    }
    if(second)
    {
        sprintf(buffer, ".%02d", mSecond);
        ret += buffer;
    }
    if(ms)
    {
        sprintf(buffer, ".%03d", mMilliseconds);
        ret += buffer;
    }

    ret.erase(0,1);
    return ret;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts time to a string format.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Time::ToString() const
{
    char buffer[512];
    sprintf(buffer,
            "%02d.%02d.%02d.%02d.%03d",
            mDay, mHour, mMinute, mSecond, mMilliseconds);
    return std::string(buffer);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads time data from a formatted string.
///
////////////////////////////////////////////////////////////////////////////////////
Time& Time::FromString(const std::string& str)
{
    Clear();
    // Newer format
    if(sscanf(str.c_str(),
        "%d.%d.%d.%d.%d",
        &mDay,
        &mHour,
        &mMinute,
        &mSecond,
        &mMilliseconds) == 5)
    {
        return *this;
    }
    Clear();
    if(strchr(str.c_str(), ':'))
    {
        // Format without day.
        if(sscanf(str.c_str(),
            "%d:%d:%d:%d",
            &mHour,
            &mMinute,
            &mSecond,
            &mMilliseconds) == 4)
        {
            return *this;
        }
        Clear();
        // Format without day.
        if(sscanf(str.c_str(),
            "%d:%d:%d:%d:%d",
            &mDay,
            &mHour,
            &mMinute,
            &mSecond,
            &mMilliseconds) == 5)
        {
            return *this;
        }
    }
    Clear();
    // Format without day.
    if(sscanf(str.c_str(),
        "%d.%d.%d.%d",
        &mHour,
        &mMinute,
        &mSecond,
        &mMilliseconds) == 5)
    {
        return *this;
    }
    Clear();
    // Format without hour.
    if(sscanf(str.c_str(),
        "%d.%d.%d",
        &mMinute,
        &mSecond,
        &mMilliseconds) == 3)
    {
        return *this;
    }
    Clear();
    // Old Simulation Time Format (don't ask...).
    if(sscanf(str.c_str(),
        "%d.%d",
        &mMinute,
        &mSecond) == 2)
    {
        // Clear these values in case they got assigned data in the previous
        // call to sscanf.
        mDay = mHour = mMilliseconds = 0;

        // Push overlapping minutes to hours.
        mHour = mMinute/60;
        mMinute -= mHour*60;
        return *this;
    }

    return *this;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Time& Time::operator =(const Time& time)
{
    mDay = time.mDay;
    mHour = time.mHour;
    mMinute = time.mMinute;
    mSecond = time.mSecond;
    mMilliseconds = time.mMilliseconds;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Convers the number of seconds to D.H.M.S format.  If seconds contains
///   month or year data in it, remaining data will just be added to day.
///
///   \param[in] seconds Number of seconds to convert to day, hour, minute, second, 
///              and milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
Time& Time::operator=(const double& seconds)
{
    mDay = mHour = mMinute = mSecond = mMilliseconds = 0;
    mMilliseconds = (unsigned int)((seconds - floor(seconds) + 0.0005)*1000);
    //mMilliseconds = (unsigned int)((seconds - ((unsigned int)seconds) + 0.0005)*1000);
    mSecond = (unsigned int)(fmod(seconds, 60.0));

    mMinute = (unsigned int)(seconds/60.0);
    mHour = mMinute/60;
    mMinute -= mHour*60;

    mDay = mHour/24;
    mHour -= mDay*24;
    
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return The result of t1 + t2 in a Time object.
///
////////////////////////////////////////////////////////////////////////////////////
Time Time::Add(const Time& t1, const Time& t2)
{
    Time result(t1);

    result.mMilliseconds += t2.mMilliseconds;
    result.mSecond += t2.mSecond;
    result.mMinute += t2.mMinute;
    result.mHour += t2.mHour;
    result.mDay += t2.mDay;

    unsigned int extra;

    extra = result.mMilliseconds/1000;
    result.mMilliseconds -= extra*1000;
    result.mSecond += extra;

    extra = result.mSecond/60;
    result.mSecond -= extra*60;
    result.mMinute += extra;

    extra = result.mMinute/60;
    result.mMinute -= extra*60;
    result.mHour += extra;

    extra = result.mHour/24;
    result.mHour -= extra*24;
    result.mDay += extra;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The result of t1 - t2 in a Time object.
///
////////////////////////////////////////////////////////////////////////////////////
Time Time::Subtract(const Time& t1, const Time& t2)
{
    assert(t1 >= t2);
    return Time(t1.ToSeconds() - t2.ToSeconds());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time data from any external time source (if available).
///
////////////////////////////////////////////////////////////////////////////////////
Time Time::GetExternalTime()
{
    Time ext;
    //mExternalTimeMutex.Lock();
    ext = mExternalTime;
    //mExternalTimeMutex.Unlock();
    return ext;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Synchronizer::Synchronizer()
{
    mpThreadObject = new Thread();
    mpSocket = NULL;
    mDelayUs = 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Time::Synchronizer::~Synchronizer()
{
    Stop();
    if(mpThreadObject)
    if(((Thread *)mpThreadObject) != NULL)
    {
        delete ((Thread *)mpThreadObject);
        mpThreadObject = NULL;
    }
    if(mpSocket)
    {
        delete mpSocket;
        mpSocket = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a time synchronizer in either server mode or client.
///
///   \param[in] server If true, creates a thread which multicasts UTC time data.
///                     If false, thread tries to receive synchronized time being
///                     transmitted.
///   \param[in] port Port to use.
///   \param[in] multicastGroup Multicast group to use.
///   \param[in] udelay By default, there is a minimum of a 1 micro-second
///                     delay (Linux only) between reading of system time
///                     to prevent over-use of CPU.  However, on some systems
///                     you may want to turn this off if you don't care or
///                     they system is slow and cannot process quickly enough
///                     with a delay. To turn off, set to 0.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Time::Synchronizer::Start(const bool server,
                               const unsigned short port,
                               const IP4Address& multicastGroup,
                               const unsigned int udelay)
{
    Stop();
    Thread* thread = (Thread *)mpThreadObject;
    if(server)
    {
        UdpClient* client = new UdpClient();
        mpSocket = client;
        mDelayUs = udelay;
        if(client->InitializeMulticastSocket(multicastGroup, port) &&
            thread->CreateThread(Time::Synchronizer::ServerThread, this))
        {
            thread->SetThreadPriority(50);
            thread->SetThreadName("TimeSynchronization");
            return true;
        }
    }
    else
    {
        UdpServer* server = new UdpServer();
        mpSocket = server;
        if(server->InitializeMulticastSocket(port, multicastGroup, true) &&
            thread->CreateThread(Time::Synchronizer::ServerThread, this))
        {
            thread->SetThreadPriority(50);
            thread->SetThreadName("TimeSynchronization");
            return true;
        }
    }

    Stop();
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Stops the synchronizer.
///
////////////////////////////////////////////////////////////////////////////////////
void Time::Synchronizer::Stop()
{
    ((Thread *)mpThreadObject)->StopThread();
    if(mpSocket)
    {
        delete mpSocket;
        mpSocket = NULL;
    }
    mpSocket = NULL;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread for sending or receiving synchronized time data.
///
////////////////////////////////////////////////////////////////////////////////////
void Time::Synchronizer::ServerThread(void *args)
{
    Synchronizer* sync = (Synchronizer*)args;
    UdpClient* client = dynamic_cast<UdpClient*>(sync->mpSocket);
    UdpServer* server = dynamic_cast<UdpServer*>(sync->mpSocket);
    Packet packet;
    packet.Reserve(32);
#ifdef WIN32
    unsigned int loopCounter = 0;
#endif

//#define TEST_TIME_SYNC

#ifdef TEST_TIME_SYNC
    Time::Stamp buff[3000];
    unsigned int size = 0;
    unsigned int limit = 3000;
#endif

    if(client)
    {
        unsigned int tstamp = 0;

        Time cur, prev, startUtc;
        packet.WriteByte('T');
        packet.Write(tstamp);

#ifdef WIN32
        double startMs = 0.0, curMs = 0.0;
        Time staticTime;
#endif

        while(sync && ((Thread *)sync->mpThreadObject)->QuitThreadFlag() == false)
        {
            // Get the current system time.
        #ifdef WIN32
            if(startMs <= 0.00001)
            {
                startMs = Timer::GetTimeMs();
                startUtc.SetCurrentTime();
            }
            else
            {
                curMs = Timer::GetTimeMs();
                unsigned int diffMs = (unsigned int)(curMs - startMs);

                cur = startUtc;
                cur.mMilliseconds += diffMs;

                unsigned int extra;

                extra = cur.mMilliseconds/1000;
                cur.mMilliseconds -= extra*1000;
                cur.mSecond += extra;

                extra = cur.mSecond/60;
                cur.mSecond -= extra*60;
                cur.mMinute += extra;

                extra = cur.mMinute/60;
                cur.mMinute -= extra*60;
                cur.mHour += extra;

                extra = cur.mHour/24;
                cur.mHour -= extra*24;
                cur.mDay += extra;
            }
            /*
            SYSTEMTIME t;
            ::GetSystemTime(&t);
            cur.mDay = t.wDay;
            cur.mHour = t.wHour;
            cur.mMinute = t.wMinute;
            cur.mSecond = t.wSecond;
            cur.mMilliseconds = t.wMilliseconds;
            */
        #else
            struct timeval tv;
            struct tm *timeinfo;
            gettimeofday(&tv, NULL);
            cur.mMilliseconds = (unsigned int)(tv.tv_usec/1000.0);
            timeinfo = gmtime(&tv.tv_sec);
            cur.mDay = timeinfo->tm_mday;
            cur.mHour = timeinfo->tm_hour;
            cur.mMinute = timeinfo->tm_min;
            cur.mSecond = timeinfo->tm_sec;
        #endif
            // Build 4 Byte time Stamp.
            tstamp = 0;
            tstamp |= (cur.mDay    << 27);
            tstamp |= (cur.mHour   << 22);
            tstamp |= (cur.mMinute << 16);
            tstamp |= (cur.mSecond << 10);
            tstamp |= cur.mMilliseconds;

            // Only transmit if the value changes.
            if(cur > prev)
            {
                packet.Write(tstamp, 1);
                client->Send(packet);
                prev = cur;
                #ifdef WIN32
                staticTime.mExternalTimeFlag = true;
                staticTime.mExternalTime = cur;
                #endif
            }
            #ifdef WIN32
            if(sync->mDelayUs > 0)
            {
                // Only sleep every N loops
                if( loopCounter++ == 10)
                {
                    loopCounter = 0;
                    Sleep(1);
                }
            }
            #else
            if(sync->mDelayUs > 0)
            {
                usleep(sync->mDelayUs);
            }
        #endif
        }
        #ifdef WIN32
        staticTime.mExternalTimeFlag = false;
        #endif
    }
    else if(server)
    {
        unsigned int tstamp = 0;
        unsigned int day = 0, hour = 0, minute = 0, second = 0, msecond = 0;
        Time cur, prev;
        Time staticTime;
        while(sync && ((Thread *)sync->mpThreadObject)->QuitThreadFlag() == false)
        {
            if(server->Recv(packet, 5, 500) > 0)
            {
                char h = 0;
                if(packet.Read(h) && h == 'T' &&
                   packet.Read(tstamp))
                {
                    msecond = tstamp & 0x3FF;
                    second = ((tstamp >> 10) & 0x3F);
                    minute = ((tstamp >> 16) & 0x3F);
                    hour = ((tstamp >> 22) & 0x1F);
                    day = ((tstamp >> 27) & 0x1F);

                    cur.mDay = day;
                    cur.mHour = hour;
                    cur.mMinute = minute;
                    cur.mSecond = second;
                    cur.mMilliseconds = msecond;
                    // Make sure we handle out of order UDP packets.
                    if(cur > prev)
                    {
                        // Lock and update data.
                        //staticTime.mExternalTimeMutex.Lock();
                        staticTime.mExternalTimeFlag = true;
                        staticTime.mExternalTime = cur;
                        //staticTime.mExternalTimeMutex.Unlock();

                        #ifdef TEST_TIME_SYNC
                        buff[size++] = cur.ToMs();
                        if(size == limit)
                        {
                            for(unsigned int i = 0; i < limit - 1; i++)
                            {
                                Time::Stamp diff = buff[i + 1] - buff[i];
                                if(diff > 1)
                                {
                                    int x;
                                    x = 3;
                                }
                            }
                            size = 0;
                        }
                        #endif

                        prev = cur;
                    }
                }
            }
            else
            {
                cur.Clear();
                prev.Clear();
                // Lock and update data.
                //staticTime.mExternalTimeMutex.Lock();
                staticTime.mExternalTimeFlag = false;
                //staticTime.mExternalTimeMutex.Unlock();
            }
        }

        // Lock and update data.
        //staticTime.mExternalTimeMutex.Lock();
        staticTime.mExternalTimeFlag = false;
        //staticTime.mExternalTimeMutex.Unlock();
    }
}

/*  End of File */
