////////////////////////////////////////////////////////////////////////////////////
///
/// \file sweepsearch.cpp
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
#include "statemachine/utility/sweepsearch.h"

using namespace std;
using namespace CxUtils;
using namespace Zebulon;
using namespace AI;


SweepSearch::SweepSearch()
{
    mSearchStartTime = 0.0;
    mInitHeading = 0.0;
    mSweepDegrees = 0.0;
    mSweepSpeed = 0.0;
    mForwardTime = 0.0;
    mForwardSpeed = 0.0;
    mDesiredSweepHeading = 0.0; // next heading to be
    mSmoothSpeed = 10.0;

    mSweepDirection = 0;
    mSweepCount = 0;
    mSweepState = 0;
    mInitSweepDirection = 1;
}

SweepSearch::~SweepSearch()
{

}

void SweepSearch::InitSearch(const double initHeading,
                             const double sweepDegrees,
                             const double sweepSpeed,
                             const double forwardTime,
                             const double forwardSpeed,
                             const int direction,
                             const double smoothSpeed)
{
    mInitHeading = initHeading;
    mSweepDegrees = sweepDegrees;
    mSweepSpeed = sweepSpeed;
    mForwardTime = forwardTime;
    mForwardSpeed = forwardSpeed;
    mInitSweepDirection = direction;
    mSmoothSpeed = smoothSpeed;
    mSweepState = StartSweep;
}

void SweepSearch::SearchStep(double &desiredSweepHeading, double &desiredForwardSpeed)
{
    double time = 0;


    switch(mSweepState)
    {
        case StartSweep:
            mSearchStartTime = CxUtils::Timer::GetTimeSeconds();
            if(mSweepCount%2 == 0)
            {
                mSweepDirection = mInitSweepDirection;
            }
            else
            {
                mSweepDirection = -mInitSweepDirection;
            }

            mDesiredSweepHeading = mInitHeading;
            mDesiredForwardSpeed = 0;
            mSweepState = Sweeping;

            break;
        case Sweeping:
            time = CxUtils::Timer::GetTimeSeconds() - mSearchStartTime;

            if(time < 360.0/mSweepSpeed)
            {
                mDesiredSweepHeading = mSweepDegrees*mSweepDirection*sin(CX_DEG2RAD(mSweepSpeed)*time) + mInitHeading;
                mDesiredForwardSpeed = mSmoothSpeed;
            }
            else
            {
                mForwardStartTime = CxUtils::Timer::GetTimeSeconds();
                mSweepState = Forward;
            }
            /*std::cout << "Sweeping: " << "time: " << time
                << " mInitHeading " << mInitHeading
                << " mDesiredSweepHeading: " << mDesiredSweepHeading
                << " mDesiredForwardSpeed: " << mDesiredForwardSpeed
                << std::endl;*/

            break;
        case Forward:
            time = CxUtils::Timer::GetTimeSeconds() - mForwardStartTime;

            mDesiredSweepHeading = mInitHeading;

            if(time > mForwardTime)
            {
                mDesiredForwardSpeed = 0;
                mSweepState = FinishSweep;
            }
            else
            {
                mDesiredForwardSpeed = mForwardSpeed;
            }
            /*std::cout << "Forward: " << "time: " << time
                << " mInitHeading " << mInitHeading
                << " mDesiredSweepHeading: " << mDesiredSweepHeading
                << " mDesiredForwardSpeed: " << mDesiredForwardSpeed
                << std::endl;*/
            break;
        case FinishSweep:
            mDesiredSweepHeading = mInitHeading;
            mDesiredForwardSpeed = 0;
            mSweepCount++;
            mSweepState = StartSweep;

            break;
    }
    desiredSweepHeading = mDesiredSweepHeading;
    desiredForwardSpeed = mDesiredForwardSpeed;
}

void SweepSearch::SearchInfo(double &desiredSweepHeading, double &desiredForwardSpeed) const
{
    desiredSweepHeading = mDesiredSweepHeading;
    desiredForwardSpeed = mDesiredForwardSpeed;
}
/*  End of File */
