////////////////////////////////////////////////////////////////////////////////////
///
/// \file stopwatch.h
/// \brief Stopwatch object to keep track of time.
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef ZEBULON_AI_UTILITY_STOPWATCH_H
#define ZEBULON_AI_UTILITY_STOPWATCH_H

#include <cxutils/time.h>
#include "statemachine/libstatemachinedll.h"

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class StopWatch
        ///  \brief Stop Watch object to keep track of an elapsed time.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  StopWatch
        {
            public:
                StopWatch();
                ~StopWatch();
                /** Initialize stop watch with how long to run in seconds. */
                bool Initialize(double RunTime = 0.0);
                /** Start the stop watch. */
                bool Start();
                /** Reset the stop watch. */
                bool Reset() { return Start(); };
                /** Returns true if time has elapsed (finished). */
                bool IsFinished();
                /** Returns the time elapsed. */
                double TimeElapsed();
                /** Returns true if intialized, false otherwise. */
                inline bool IsInitialized() const { return mInitializedFlag; }
                /** Returns true if start has been called.  Resets on finished. */
                inline bool IsStarted() const { return mStartedFlag; }

                inline double GetRunTime() const { return mRunTime; }
            private:
                double mStartTime;  ///<  Start time in seconds.
                double mRunTime;    ///<  How long to run in seconds.
                bool mInitializedFlag; ///<  Initialized yet?
                bool mStartedFlag;     ///<  Started yet?
        };
    }
}

#endif // STOPWATCH_H
