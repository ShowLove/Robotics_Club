////////////////////////////////////////////////////////////////////////////////////
///
/// \file strafesearch.cpp
/// \brief Interface to calculate speed and lateral movement to search in a
///        strafing pattern.
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
#include "statemachine/utility/strafesearch.h"

using namespace std;
using namespace CxUtils;
using namespace Zebulon;
using namespace AI;

StrafeSearch::StrafeSearch()
{
    mSearchStartTime = 0.0;
    mForwardStartTime = 0.0;
    mStrafeTime = 0.0;
    mStrafeSpeed = 0.0;
    mForwardTime = 0.0;
    mForwardSpeed = 0.0;
    mSmoothSpeed = 0.0;
    mInitSearchDirection = 1;
    mAngSpeed = 0.0;

    mSearchDirection = 0;
    mDesiredStrafeSpeed = 0.0;
    mDesiredForwardSpeed = 0.0;
    mSearchCount = 0;
    mStrafeState = 0;

    mFirstRun = true;
}

StrafeSearch::~StrafeSearch()
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
void StrafeSearch::InitSearch(double strafeTime, double strafeSpeed, double forwardTime, double forwardSpeed, double smoothSpeed, int initSearchDirection)
{
    mStrafeTime = strafeTime;
    mStrafeSpeed = strafeSpeed;
    mForwardTime = forwardTime;
    mForwardSpeed = forwardSpeed;
    mSmoothSpeed = smoothSpeed;
    mAngSpeed = CxUtils::CX_TWO_PI/strafeTime; // Radians per second for sine wave
    mInitSearchDirection = initSearchDirection;

    mSearchCount = 0;

    mFirstRun = true;
}

int StrafeSearch::SearchStep(double& desiredStrafeSpeed, double& desiredForwardSpeed)
{
    double time = 0;

    switch(mStrafeState)
    {
        case StartSearch:
            mSearchStartTime = CxUtils::Timer::GetTimeSeconds();
            if(mSearchCount%2 == 0)
            {
                mSearchDirection = mInitSearchDirection;
            }
            else
            {
                mSearchDirection = -mInitSearchDirection;
            }
            
            mDesiredForwardSpeed = 0;
            mStrafeState = Searching;

            break;
        
        case Searching:
            time = CxUtils::Timer::GetTimeSeconds() - mSearchStartTime;

            if(time < mStrafeTime)
            {
                mDesiredStrafeSpeed = mStrafeSpeed * mSearchDirection * sin(mAngSpeed * time);
                mDesiredForwardSpeed = mSmoothSpeed;
            }
            else
            {
                mForwardStartTime = CxUtils::Timer::GetTimeSeconds();
                mStrafeState = Forward;
            }

            break;
        
        case Forward:
            time = CxUtils::Timer::GetTimeSeconds() - mForwardStartTime;
            
            mDesiredStrafeSpeed = 0;
            
            if(time > mForwardTime)
            {
                mDesiredForwardSpeed = 0;
                mStrafeState = FinishSearch;
            }
            else
            {
                mDesiredForwardSpeed = mForwardSpeed;
            }
            break;
        
        case FinishSearch:
            mDesiredStrafeSpeed = 0;
            mDesiredForwardSpeed = 0;
            mSearchCount++;
            mStrafeState = StartSearch;

            break;
    }

    desiredForwardSpeed = mDesiredForwardSpeed;
    desiredStrafeSpeed = mDesiredStrafeSpeed;
    return mSearchCount;
}


/*  End of File */
