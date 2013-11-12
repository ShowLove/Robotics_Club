////////////////////////////////////////////////////////////////////////////////////
///
/// \file sinesearch.cpp
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
#include "statemachine/utility/sinesearch.h"

using namespace std;
using namespace CxUtils;
using namespace Zebulon;
using namespace AI;

SineSearch::SineSearch()
{
    mSearchStartTime = 0.0;
    mForwardStartTime = 0.0;
    mSineTime = 0.0;
    mSineSpeed = 0.0;
    mForwardTime = 0.0;
    mForwardSpeed = 0.0;
    mSmoothSpeed = 0.0;
    mInitSearchDirection = 1;
    mAngSpeed = 0.0;

    mSearchDirection = 0;
    mDesiredSineSpeed = 0.0;
    mDesiredForwardSpeed = 0.0;
    mSearchCount = 0;
    mSineState = StartSearch;

    mFirstRun = true;
}

SineSearch::~SineSearch()
{
}

/**
 * \param[in] strafeTime            seconds
 * \param[in] strafeSpeed           meters/second
 * \param[in] forwardTime           seconds
 * \param[in] forwardSpeed          meters/second
 * \param[in] smoothSpeed           meters/second
 * \param[in] initSearchDirection   +/- 1
 */
void SineSearch::InitSearch(double sineTime, double sineSpeed, double forwardTime, double forwardSpeed, double smoothSpeed, int initSearchDirection)
{
    mSineTime = sineTime;
    mSineSpeed = sineSpeed;
    mForwardTime = forwardTime;
    mForwardSpeed = forwardSpeed;
    mSmoothSpeed = smoothSpeed;
    mAngSpeed = CxUtils::CX_TWO_PI/sineTime; // Radians per second for sine wave
    mInitSearchDirection = initSearchDirection;
    if(mInitSearchDirection==0)
    {
        mInitSearchDirection=1;
    }

    //printf("Init:\n");
    //printf("%lf %lf %lf %lf %lf %lf %d\n",mSineTime,mSineSpeed,mForwardTime,mForwardSpeed,mSmoothSpeed,mAngSpeed,mInitSearchDirection);

    mSearchCount = 0;
    mSineState = StartSearch;

    mFirstRun = true;
}

int SineSearch::SearchStep(double& desiredSineSpeed, double& desiredForwardSpeed)
{
    double time = 0;
    fflush(stdout);
    mDesiredSineSpeed = 0;
    mDesiredForwardSpeed = 0;

    switch(mSineState)
    {
        case StartSearch:
            //printf("Search: Start\n");
            mSearchStartTime = CxUtils::Timer::GetTimeSeconds();
            if(mFirstRun)
            {
                mSearchDirection = mInitSearchDirection;
                mFirstRun=false;
            }
            else
            {
                mSearchDirection = -mSearchDirection;
            }

            mDesiredForwardSpeed = 0;
            mDesiredSineSpeed=0;
            mSineState = HalfOut;

            break;

        case HalfOut:
            //printf("Search: HalfOut\n");
            time = CxUtils::Timer::GetTimeSeconds() - mSearchStartTime;
            if(time < mSineTime/4.0)
            {
                //mDesiredSineSpeed = mSineSpeed * mSearchDirection * sin(2.0*mAngSpeed * time);
                if(mSearchDirection==-1)
                {
                    mDesiredSineSpeed=mSineSpeed * mSearchDirection*0.75;
                }
                else
                {
                    mDesiredSineSpeed=mSineSpeed * mSearchDirection;

                }
                mDesiredForwardSpeed = mSmoothSpeed;
            }
            else
            {
                mForwardStartTime = CxUtils::Timer::GetTimeSeconds();
                mSineState = Forward;
            }
            break;

        case FullMiddle:
            //printf("Search: FullMiddle\n");
            time = CxUtils::Timer::GetTimeSeconds() - mSearchStartTime;

            if(time < mSineTime)
            {
                //mDesiredSineSpeed = mSineSpeed * mSearchDirection * sin(mAngSpeed * time);
                if(mSearchDirection==-1)
                {
                    mDesiredSineSpeed=mSineSpeed * mSearchDirection*0.75;
                }
                else
                {
                    mDesiredSineSpeed=mSineSpeed * mSearchDirection*1.25;

                }

                mDesiredForwardSpeed = mSmoothSpeed;
            }
            else
            {
                mForwardStartTime = CxUtils::Timer::GetTimeSeconds();
                mSineState = Forward;
            }
            break;
        case Forward:
            //printf("Search: Forward\n");
            time = CxUtils::Timer::GetTimeSeconds() - mForwardStartTime;

            mDesiredSineSpeed = 0;

            if(time > mForwardTime)
            {
                mDesiredForwardSpeed = 0;
                mSineState = FullMiddle;
                mSearchDirection = -mSearchDirection;
                mSearchStartTime = CxUtils::Timer::GetTimeSeconds();
            }
            else
            {
                mDesiredForwardSpeed = mForwardSpeed;
            }
            break;

        case FinishSearch:
            mDesiredSineSpeed = 0;
            mDesiredForwardSpeed = 0;
            mSearchCount++;
            mSineState = StartSearch;

            break;
        default:
            std::cout << "Error: Not in a sine state" << std::endl;
                mSineState = StartSearch;
    }

    desiredForwardSpeed = mDesiredForwardSpeed;
    desiredSineSpeed = mDesiredSineSpeed;
    return mSearchCount;
}


/*  End of File */
