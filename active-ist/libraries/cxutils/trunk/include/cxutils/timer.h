////////////////////////////////////////////////////////////////////////////////////
///
///  \file timer.h
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
#ifndef __CXUTILS_TIMER_H
#define __CXUTILS_TIMER_H

#ifdef WIN32

#else
#include <unistd.h>
#endif

#include <set>
#include <vector>
#include "cxutils/thread.h"

namespace CxUtils
{    
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///  \class Timer
    ///  \brief Simple cross-platform Timer interface.  Timer can be used for
    ///  more accurate delay/sleep statement (1 ms accuracy), and for creating
    ///  a periodic timed event with high accuracy.
    ///
    ///  The Timer class has some static method for getting the current system time.
    ///  This time is relative to when the system turned on in Windows, and is UTC
    ///  time in Linux.  Therefore, only use the GetTime methods in this class when
    ///  you require 1ms timing information in multiple environments.
    ///
    ///  The Timer class can start a periodic event with 1ms accuracy using callback
    ///  interfaces or function callbacks.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Timer
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class Callback
        ///  \brief Callback interface for registering for Timer Events.  Overload the
        ///         ProcessTimerEvent method, and register with a Timer.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Callback
        {
        public:
            typedef std::set<Callback*> Set;     ///<  Set of callbacks.
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///  \class Function
            ///  \brief Function object data used by the Timer class for storing Function 
            ///  Callbacks for Timer Events..
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CX_UTILS_DLL Function
            {
            public:
                typedef std::vector<Function> List; ///<  List of functions.
                Function(void (*func)(void *args), void* fargs = NULL);
                ~Function();
                void Run();
                void (*mpFunctionPtr)(void* args);   ///<  Function pointer for Timer Events.
                void *mpFunctionArgs;                ///<  Pointer to function arguments.
            };
            virtual void ProcessTimerEvent() = 0;
        };
        Timer();
        ~Timer();
        int Start(const double frequencyHz, const double hptThresholdHz = 50);
        void Stop();
        bool RegisterTimerEvent(Callback* timerCallback);
        bool RegisterTimerEvent(void (*func)(void *args), void* fargs);
        void RemoveTimerEvent(Callback* timerCallback);
        void RemoveTimerEvent(void (*func)(void *args), void* fargs);
        static double GetTimeMs();
        static double GetTimeSeconds();
        static void Pause(const unsigned int ms);
        //  Is a periodic timer running?
        inline bool IsActive() const { return mTimerActive; }
        // Is periodic timer trying to stop.
        inline bool IsShuttingDown() const { return mShutdownFlag; }
        void TriggerEvents();
        double GetFrequency() const;
        bool ChangeFrequency(const double frequencyHz, const double hptThresholdHz = 50);
        void SetName(const std::string& name) { mTimerEventThread.SetThreadName(name); }
    private:
#ifdef WIN32
        class Data;
        Data* mpTimerData; ///< Pointer to windows specific data.
#endif
        static void TimerEventThread(void *args);
        
        Thread mTimerEventThread;         ///<  Performs function callbacks on timing interval.
        Mutex mCallbacksMutex;            ///<  Mutex for callback event methods.
        Mutex mSleepMutex;                ///<  Mutex for protection of mDelayTimeMs.
        Callback::Set mCallbackObjects;   ///<  Set of pointers to callback objects.
        Callback::Function::List mCallbackFunctions;    ///<  Vector of callback functions.
        volatile unsigned int mDelayTimeMs; ///<  How long to delay between events.
        volatile bool mTimerActive;         ///<  Timer active flag.
        volatile bool mShutdownFlag;        ///<  Flag to indicate shutdown signal.
    };

    typedef Timer::Callback TimerCallback;///< Typedef for backwards compatibility.
} 

#endif 
/*  End of File */
