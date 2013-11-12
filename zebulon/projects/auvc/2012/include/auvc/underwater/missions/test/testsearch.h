/////////////////////////////////////////////////////////////////////////////
///
///  \file testsearch.h
///  \brief test different search algorithms
///
///  Author(s): Cassondra Puklavage<br>
///  Created: 2012<br>
///  Copyright (c) 2012<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: cassondrap@knights.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#ifndef __ZEBULON_UNDERWATER_TESTSEARCH_H
#define __ZEBULON_UNDERWATER_TESTSEARCH_H

#include "auvc/underwater/mission.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/sweepsearch.h"
#include "statemachine/utility/strafesearch.h"
#include "statemachine/utility/sinesearch.h"


namespace Zebulon
{
    namespace Underwater
    {
        class TestSearch : public Underwater::Mission
        {
        public:
            static const std::string NextMission;

            TestSearch();
            ~TestSearch();

            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();


        private:
            enum States
            {
                DEPTH,
                INIT,
                SEARCH
            };

            States mState;

            void InitSearch();
            void SearchStep();

            //Currents
            double mCurrentYaw;
            double mCurrentPitch;
            double mCurrentRoll;
            double mCurrentDepth;

            //Start Direction for search
            double mStartYaw;

            //What to do
            double mDesiredYaw;
            double mDesiredPitch;
            double mDesiredRoll;
            double mDesiredDepth;
            double mDesiredAxialThrust;
            double mDesiredLateralThrust;


            // Search
            bool mBuoySearchStrafe;
            bool mBuoySearchSweep;
            bool mBuoySearchSine;

            Zebulon::AI::SweepSearch mBuoySweepSearch;
            double mBuoySearchSweepDegrees;
            double mBuoySearchSweepSpeed;
            double mBuoySweepForwardTime;
            double mBuoySweepForwardSpeed;
            double mBuoySweepSmoothSpeed;
            int mBuoySweepInitDirection;

            Zebulon::AI::StrafeSearch mBuoyStrafeSearch;
            double mBuoySearchStrafeTime;
            double mBuoySearchStrafeSpeed;
            double mBuoyStrafeForwardTime;
            double mBuoyStrafeForwardSpeed;
            double mBuoyStrafeSmoothSpeed;
            int mBuoyStrafeInitDirection;

            Zebulon::AI::SineSearch mBuoySineSearch;
            double mBuoySearchSineTime;
            double mBuoySearchSineSpeed;
            double mBuoySineForwardTime;
            double mBuoySineForwardSpeed;
            double mBuoySineSmoothSpeed;
            int mBuoySineInitDirection;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TESTSEARCH_H
/* End of file */
