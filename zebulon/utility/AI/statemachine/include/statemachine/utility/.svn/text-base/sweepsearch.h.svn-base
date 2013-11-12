////////////////////////////////////////////////////////////////////////////////////
///
/// \file sweepsearch.h
/// \brief Interface to calculate speed and heading movement to search in a
///        pattern.
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
#ifndef ZEBULON_AI_UTILITY_SWEEP_SEARCH_H
#define ZEBULON_AI_UTILITY_SWEEP_SEARCH_H

#include <cxutils/timer.h>
#include <cxutils/math/cxmath.h>

#include <iostream>
#include <math.h>
#include "statemachine/libstatemachinedll.h"

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class SweepSearch
        ///  \brief Utility class which given initial settings, generates desired thrust
        ///  and rotational movement to search in a left/right motion while going
        ///  forward.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  SweepSearch
        {
            public:
                SweepSearch();
                ~SweepSearch();
                /** Initialize utility with parameters for generating headings and
                    speeds.

                    \param[in] initHeading Initial compass heading (degrees) [-180,180]
                    \param[in] seepDegrees The total sweep angle in degrees
                    \param[in] sweepSpeed How fast to sweep/rotate degrees/s
                    \param[in] forwardTime How much time to drive forward seconds
                    \param[in] forwardSpeed How fast to move forward
                    \param[in] direction Starting direction, 1 = right, -1 = left.
                */
                void InitSearch(const double initHeading,
                                const double sweepDegrees,
                                const double sweepSpeed,
                                const double forwardTime,
                                const double forwardSpeed,
                                const int direction = 1,
                                const double smoothSpeed = 0.0);

                /** Gets the desired heading and forward speed

                  \param[in] desiredSweepHeading Compass heading (degrees) to implement
                  \param[in] desiredForwardSpeed Desired forward speed

                  \return Current time step.
                */
                void SearchStep(double &desiredSweepHeading, double &desiredForwardSpeed);
                /** Gets the desired heading and forward speed (but doesn't update internal
                    data counts.

                  \param[in] desiredSweepHeading Compass heading (degrees) to implement
                  \param[in] desiredForwardSpeed Desired forward speed

                  \return Current time step.
                */
                void SearchInfo(double &desiredSweepHeading, double &desiredForwardSpeed) const;
            private:
                double mSearchStartTime;        ///<  Search start time.
                double mForwardStartTime;       ///<  Time started moving forward.
                double mInitHeading;            ///<  Initial heading to return to.
                double mSweepDegrees;           ///<  Total sweep size in degrees.
                double mSweepSpeed;             ///<  Sweep speed (deg/s).
                double mForwardTime;            ///<  Time to travel forward.
                double mForwardSpeed;           ///<  Forward speed.
                int mInitSweepDirection;        ///<  Initial sweep direction (right/left first).
                int mSweepDirection;            ///<  Current sweep direction.
                double mDesiredSweepHeading;    ///<  Calculated next heading to go to.
                double mDesiredForwardSpeed;    ///<  Desired forward speed.
                int mSweepCount;                ///<  Number of sweeps.
                int mSweepState;                ///<  Current state in sweep (left/right/forward).
                double mSmoothSpeed;            ///<  Speed to be moving forward while sweeping

                enum SweepStates
                {
                    StartSweep, // Initialization.
                    Sweeping,
                    Forward,
                    FinishSweep
                };

        };
    }
}

#endif
