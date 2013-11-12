////////////////////////////////////////////////////////////////////////////////////
///
/// \file sinesearch.h
/// \brief Interface to calculate speed and lateral movement to search in a
///        sinusoidal pattern.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: cassondrap@knights.ucf.edu<br>
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
#ifndef ZEBULON_AI_UTILITY_SINE_SEARCH_H
#define ZEBULON_AI_UTILITY_SINE_SEARCH_H

#include <cxutils/timer.h>
#include <cxutils/math/cxmath.h>

#include <iostream>
#include <math.h>
#include "statemachine/libstatemachinedll.h"
#include<stdio.h>

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class StrafeSearch
        ///  \brief Utility class which given initial settings, generates desired thrust
        ///  and lateral movement to implement a search in a strafing pattern. For example
        ///  move right, then forward, then left, then forward, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  SineSearch
        {
            public:
                SineSearch();
                ~SineSearch();

                /**  Initializes the settings for generation of thrust values.

                  Units are in seconds and in thrust or speed.

                  \param[in] strafeTime Time to strafe.
                  \param[in] starfeSpeed Desired overall speed when strafing.
                  \param[in] forwardTime Time to move forward.
                  \param[in] forwardSpeed Speed to travel when going forward.
                  \param[in] smoothSpeed Smooths speed on transitions
                  \param[in] initStrafeFrac Initial fraction for first time step.
                */
                void InitSearch(double sineTime,
                                double sineSpeed,
                                double forwardTime,
                                double forwardSpeed,
                                double smoothSpeed,
                                int initSearchDirection=1);

                /** Gets the desired strafe and forward speed at a time step.

                  \param[in] desiredStrafeSpeed Desired strafing speed
                  \param[in] desiredForwardSpeed Desired forward speed

                  \return Current time step.
                */
                int SearchStep(double &desiredSineSpeed, double &desiredForwardSpeed);

            private:
                double mSearchStartTime;
                double mForwardStartTime;
                double mSineTime;
                double mSineSpeed;
                double mForwardTime;
                double mForwardSpeed;
                double mSmoothSpeed;
                double mAngSpeed;
                int mInitSearchDirection;

                int mSearchDirection;
                double mDesiredSineSpeed;
                double mDesiredForwardSpeed;
                int mSearchCount;
                int mSineState;

                bool mFirstRun;

                enum SineStates
                {
                    StartSearch, // initialize time
                    HalfOut,
                    FullMiddle,
                    HalfBack,
                    Forward,
                    FinishSearch
                };

        };
    }
}

#endif
/*  End of File */
