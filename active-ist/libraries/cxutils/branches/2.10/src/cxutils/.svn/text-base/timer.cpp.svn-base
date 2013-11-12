////////////////////////////////////////////////////////////////////////////////////
///
///  \file timer.cpp
///  \brief Contains software for more precise timing.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 27 June 2007
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
#include "cxutils/timer.h"
#include "cxutils/time.h"
#include <cxutils/math/cxmath.h>
#include <assert.h>

#ifdef WIN32
#include <windows.h>
#include <mmsystem.h>
#else
#include <sys/time.h>
#endif

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor for Function Callback data.
///
///  \param[in] func Pointer to function to call when event is triggered.
///  \param[in] fargs Pointer to arguments to pass to function callback.
///
////////////////////////////////////////////////////////////////////////////////////
Timer::Callback::Function::Function(void (*func)(void* args), void* fargs)
{
    mpFunctionPtr = func;
    mpFunctionArgs = fargs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Timer::Callback::Function::~Function()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs the function callback.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::Callback::Function::Run()
{
    if(mpFunctionPtr)
    {
        mpFunctionPtr(mpFunctionArgs);
    }
}


#ifdef WIN32
class Timer::Data
{
public:
    Data()
    {
        mResult = 0;
        mLeaveFlag = true;
        mEnteredFlag = false;
        mStopCounter = -1;
        mKillTimer = false;
        InitializeCriticalSection(&mCritSection);
        InitializeCriticalSection(&mDelayCritSection);
    }
    ~Data()
    {
        if(mResult)
            mResult = timeKillEvent(mResult);
        DeleteCriticalSection(&mCritSection);
        DeleteCriticalSection(&mDelayCritSection);
    }
    Timer* mpTimer;
    static void CALLBACK PauseTimerProcedure(UINT id, UINT msg, DWORD_PTR user, DWORD_PTR one, DWORD_PTR two);
    static void CALLBACK TimerProcedure(UINT id, UINT msg, DWORD_PTR user, DWORD_PTR one, DWORD_PTR two);
    MMRESULT          mResult;              ///<  Result from multi-media code/timer software.
    CRITICAL_SECTION  mCritSection;         ///<  Semaphore for blocking.
    CRITICAL_SECTION  mDelayCritSection;    ///<  Critical section to prevent delay from being called twice in separate threads.
    volatile bool     mEnteredFlag;         ///<  Flag to determine first time in critical section.
    volatile bool     mLeaveFlag;           ///<  Flag which signals leaving of critical section.
    volatile int      mStopCounter;         ///<  Counter used to determine leaving critical section.
    volatile bool     mKillTimer;           ///<  Kill the timer.
};
#endif



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Timer::Timer()
{
#ifdef WIN32
    mpTimerData = new Timer::Data();
    ((Timer::Data*)(mpTimerData))->mpTimer = this;
#endif
    mTimerActive = false;
    mDelayTimeMs = 0;
    mShutdownFlag = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Timer::~Timer()
{
    Stop();
#ifdef WIN32
    delete mpTimerData;
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Starts periodic timing event/function call.
///
///  The threshold parameter is used to indicate if the Windows High Performance
///  Timer (HPT) is used in Windows.  In Linux, the parameter does nothing. 
///  The HPT has 1ms accuracy, but can be computationally expensive for your 
///  program.  If not used, then a thread is used to generated periodic events 
///  which has <= 15ms of accuracy, but has little CPU cost.  If frequency is  
///  greater than hptThresholdHz, the HPT is used. The default value for the
///  hptThresholdHz is 50 Hz.
///
///  \param[in] frequencyHz How often to generate periodic events in Hz.
///  \param[in] hptThresholdHz This parameter has no affect in Linux.  However in
///                          Windows, it will determine whether or not to use the
///                          Windows High Performance Timer (HPT).  
///
///  \return 1 if timer started, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Timer::Start(const double frequencyHz,
                 const double hptThresholdHz)
{
    Stop(); // Stops the active timer if running.
    
    mShutdownFlag = false;
    
    // Calculate delay time (avoid divide by 0).
    mDelayTimeMs = (unsigned int)(1000.0/(frequencyHz + CX_EPSILON));
    if(mDelayTimeMs == 0)
    {
        mDelayTimeMs = 1;
    }
#ifdef WIN32
    if(frequencyHz > hptThresholdHz)
    {
        ((Timer::Data *)(mpTimerData))->mKillTimer = false;
        ((Timer::Data *)(mpTimerData))->mResult = timeSetEvent(mDelayTimeMs, 
                                                             0, 
                                                             &Timer::Data::TimerProcedure, 
                                                             (DWORD_PTR)this->mpTimerData,
                                                             TIME_PERIODIC);
        if( ((Timer::Data *)(mpTimerData))->mResult != NULL ) 
        {
            mTimerActive = true;
            return 1;
        }
    }
    else
    {
        if(mTimerEventThread.CreateThread(&TimerEventThread, this)) 
        {
            mTimerEventThread.SetThreadPriority(50);
            mTimerActive = true;
            return 1;
        }
    }
#else
    if(mTimerEventThread.CreateThread(&TimerEventThread, this)) 
    {
        mTimerEventThread.SetThreadPriority(50);
        mTimerActive = true;
        return 1;
    }
#endif

    return 0;
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Stops periodic timer events.
///
///  \return 1 if a timer was stopped, 0 if nothing to stop.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::Stop()
{
    mShutdownFlag = true;
#ifdef WIN32
    if(((Timer::Data *)(mpTimerData))->mResult) 
    {
        ((Timer::Data *)(mpTimerData))->mKillTimer = true;
        ((Timer::Data *)(mpTimerData))->mResult = timeKillEvent(((Timer::Data *)(mpTimerData))->mResult);
        return;
    }
    ((Timer::Data *)(mpTimerData))->mKillTimer = false;    
#endif
    mTimerEventThread.StopThread(mDelayTimeMs + 500);

    mTimerActive = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Add a pointer to a Timer::Callback object to use for generating
///  Timer events.
///
///  \param[in] timerCallback Callback pointer to run based on Timer frequency.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Timer::RegisterTimerEvent(Timer::Callback* timerCallback)
{
    bool result = false;
    Callback::Set::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbackObjects.find(timerCallback);
    if(cb == mCallbackObjects.end())
    {
        mCallbackObjects.insert(timerCallback);
        result = true;
    }
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Add a pointer to a function to use for generating
///  Timer events.
///
///  \param[in] func Function callback to used for timer events.
///  \param[in] fargs Pointer to arguments to pass to function callback.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Timer::RegisterTimerEvent(void (*func)(void *args), void* fargs)
{
    bool result = true;
    mCallbacksMutex.Lock();
    mCallbackFunctions.push_back(Timer::Callback::Function(func, fargs));
    mCallbacksMutex.Unlock();
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Remove a Timer::Callback object to used for generating
///  Timer events.
///
///  \param[in] timerCallback Callback pointer to run based on Timer frequency.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::RemoveTimerEvent(Timer::Callback* timerCallback)
{
    Callback::Set::iterator cb;
    mCallbacksMutex.Lock();
    cb = mCallbackObjects.find(timerCallback);
    if(cb != mCallbackObjects.end())
    {
        mCallbackObjects.erase(cb);
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Remove pointer to a function to use for generating
///  Timer events.
///
///  \param[in] func Function callback to used for timer events.
///  \param[in] fargs Pointer to arguments to pass to function callback.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::RemoveTimerEvent(void (*func)(void *args), void* fargs)
{
    Callback::Function::List::iterator cb;
    mCallbacksMutex.Lock();
    for(cb = mCallbackFunctions.begin();
        cb != mCallbackFunctions.end();
        cb++)
    {
        if(cb->mpFunctionPtr == func && cb->mpFunctionArgs == fargs)
        {
            mCallbackFunctions.erase(cb);
            break;
        }
    }
    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Performs a blocking-pause for the set period of time in
///  milliseconds.  This method has 1ms accuracy (but is more computationally
///  expensive in Windows).
///
///  \param[in] ms The time to pause in milliseconds.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::Pause(const unsigned int ms)
{
    if(ms > 0) 
    { 
#ifdef WIN32
        Timer timer;
        EnterCriticalSection( &((Timer::Data *)(timer.mpTimerData))->mDelayCritSection );

        if (((Timer::Data *)(timer.mpTimerData))->mResult == NULL) 
        {
            ((Timer::Data *)(timer.mpTimerData))->mResult = timeSetEvent(1, 
                                                                       0, 
                                                                       &Timer::Data::PauseTimerProcedure, 
                                                                       (DWORD_PTR)timer.mpTimerData,
                                                                       TIME_PERIODIC);
            assert(((Timer::Data *)(timer.mpTimerData))->mResult);
        }

        ((Timer::Data *)(timer.mpTimerData))->mStopCounter = (int)ms;
        ((Timer::Data *)(timer.mpTimerData))->mLeaveFlag = false;
        ((Timer::Data *)(timer.mpTimerData))->mEnteredFlag = false;
        while(!((Timer::Data *)(timer.mpTimerData))->mEnteredFlag) {}
        //Perform the delay using Critical Sections.
        EnterCriticalSection(&((Timer::Data *)(timer.mpTimerData))->mCritSection);
        LeaveCriticalSection(&((Timer::Data *)(timer.mpTimerData))->mCritSection);
        LeaveCriticalSection(&((Timer::Data *)(timer.mpTimerData))->mDelayCritSection );
#else
        usleep(ms*1000);
#endif
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return The current time since the system booted in milliseconds with
///  an accuracy of 1ms.   0 is returned if precision timer is not supported
///  in hardware.
///
////////////////////////////////////////////////////////////////////////////////////
double Timer::GetTimeMs()
{
#ifdef WIN32
    LARGE_INTEGER count;
    static LARGE_INTEGER freq;
    
    if(freq.QuadPart == 0) 
    {
        if(QueryPerformanceFrequency(&freq))
        {
            freq.QuadPart /= 1000;
        }
        else
        {
            // High performance timer not available on system.
            freq.QuadPart = 0;
            return (double)CxUtils::GetTimeMs();
        }
    }
    if(freq.QuadPart != 0)
    {
        if(QueryPerformanceCounter(&count)) 
        {
            return (double)count.QuadPart/(double)freq.QuadPart;
        }
        else
        {
            return (double)CxUtils::GetTimeMs();
        }
    }  
    return (double)CxUtils::GetTimeMs();
#else
    return (double)CxUtils::GetTimeMs();
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return The current time since the system booted in seconds with
///  an accuracy of 1ms.   In Windows, if the HPT is not supported by hardware
///  than UTC time is returned which has an accuracy of <= 15ms.
///
////////////////////////////////////////////////////////////////////////////////////
double Timer::GetTimeSeconds() { return GetTimeMs()/1000.0; }

#ifdef WIN32


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Timer callback function used in windows.  This function is called
///  periodically every 1ms and either enters or leaves a critical section of
///  memory to cause a block in the Delay function of Timer.
///
////////////////////////////////////////////////////////////////////////////////////
void CALLBACK  Timer::Data::PauseTimerProcedure(UINT id, 
                                   UINT msg, 
                                   DWORD_PTR user, 
                                   DWORD_PTR one, 
                                   DWORD_PTR two)
{
    Timer::Data* pData = (Timer::Data*)user;
    if(pData) 
    {
        //  Block using the critical section. 
        if (!pData->mEnteredFlag && !pData->mLeaveFlag) 
        {
            EnterCriticalSection(&pData->mCritSection);
            pData->mEnteredFlag = true;   
            --pData->mStopCounter;
        }        
        //  Otherwise decrement the stop counter. 
        else if  (pData->mEnteredFlag && !pData->mLeaveFlag && pData->mStopCounter > 0) 
        {
            --pData->mStopCounter;
            // Unlock the critical section if the counter has
            // expired. 
            if (pData->mStopCounter == 0 || pData->mKillTimer) 
            {                 
                LeaveCriticalSection(&pData->mCritSection);
                pData->mLeaveFlag = true;
                pData->mKillTimer = false;
            }
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Timer callback function used in windows.  This function is called
///  periodically based on the parameters used for the StartPeriodicTimer
///  function.
///
////////////////////////////////////////////////////////////////////////////////////
void CALLBACK Timer::Data::TimerProcedure(UINT id, 
                             UINT msg, 
                             DWORD_PTR user, 
                             DWORD_PTR one, 
                             DWORD_PTR two)
{
    Timer::Data* pData = (Timer::Data*)user;
    if (pData && !pData->mKillTimer) 
    {
        // Run Callbacks.
        pData->mpTimer->TriggerEvents();
    }
}

#endif

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Performs the events after a specified delay time within the
///  mTimerEventThread object.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::TimerEventThread(void *args)
{
    Timer* pThis = (Timer*)args;
#ifdef WIN32
    double eventTimeMs = 0;
#else
    unsigned int delayMs = (unsigned int)(pThis->mDelayTimeMs*1000);
#endif

    while(!pThis->mTimerEventThread.QuitThreadFlag()) 
    {
#ifdef WIN32
        if(Timer::GetTimeMs() - eventTimeMs >= pThis->mDelayTimeMs)
        {
            // Run event callbacks.
            pThis->TriggerEvents();
            eventTimeMs = Timer::GetTimeMs();
        }
        Sleep(1);
#else
        static Time::Stamp checkTime = 0;
        if(Time::GetUtcTimeMs() - checkTime > 250)
        {
            CxUtils::Mutex::ScopedLock lock(&(pThis->mSleepMutex));
            delayMs = (unsigned int)(pThis->mDelayTimeMs*1000);
        }
        usleep(delayMs);
        // Run event callbacks.
        pThis->TriggerEvents();
#endif
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief When called this method generates timer event callbacks.
///
////////////////////////////////////////////////////////////////////////////////////
void Timer::TriggerEvents()
{
    Callback::Set::iterator cbObject;
    Callback::Function::List::iterator cbFunction;

    mCallbacksMutex.Lock();
    for(cbObject = mCallbackObjects.begin();
        cbObject != mCallbackObjects.end() && mShutdownFlag == false;
        cbObject++)
    {
        (*cbObject)->ProcessTimerEvent();
    }

    for(cbFunction = mCallbackFunctions.begin();
        cbFunction != mCallbackFunctions.end() && mShutdownFlag == false;
        cbFunction++)
    {
        cbFunction->Run();
    }

    mCallbacksMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Timer frequency in Hz.
///
////////////////////////////////////////////////////////////////////////////////////
double Timer::GetFrequency() const 
{ 
    return 1.0/(CX_EPSILON + mDelayTimeMs/1000.0); 
}



////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Method used to try change timer frequency as on the fly. 
///
///  In Windows, if the new frequency is greather >= to hptThresholdHz, than
///  the timer will be stopped so a new HPT can be created.  In all other cases
///  the update time is changed on the fly.
///
///  \param[in] frequencyHz How often to generate periodic events in Hz.
///  \param[in] hptThresholdHz This parameter has no affect in Linux.  However in
///                          Windows, it will determine whether or not to use the
///                          Windows High Performance Timer (HPT). 
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Timer::ChangeFrequency(const double frequencyHz, const double hptThresholdHz)
{
    if(mTimerActive)
    {
#ifdef WIN32
        // Check for use/need for HPT.  If we are using the HPT we
        // must stop it and restart it.  Or if the desired freq is
        // >= hptThresholHz, we start over again.
        if(frequencyHz >= hptThresholdHz || mTimerEventThread.IsThreadActive() == false)
        {
            Stop();
            return Start(frequencyHz, hptThresholdHz) > 0 ? true : false;
        }
#endif
        Mutex::ScopedLock lock(&mSleepMutex);
        // Calculate delay time (avoid divide by 0).
        mDelayTimeMs = (unsigned int)(1000.0/(frequencyHz + CX_EPSILON));
        if(mDelayTimeMs == 0)
        {
            mDelayTimeMs = 1;
        }
        return true;
    }
    return false;
}


/*  End of File */
