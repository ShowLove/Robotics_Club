/////////////////////////////////////////////////////////////////////////////
///
///  \file debounce.cpp
///  \brief Interface for debouncing data points.
///
///  Author(s): Gary Stein<br>
///  Copyright (c) 2011<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu, dbarber@ist.ucf.edu<br>
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
#include "statemachine/utility/debounce.h"

using namespace Zebulon;
using namespace AI;


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, sets defaults.
///
////////////////////////////////////////////////////////////////////////////////////
Debounce::Debounce()
{
    mLimit = 0;
    mResetOnMiss = false;
    mCount = 0;
    mTriggered = false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Debounce::~Debounce()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initialize the debounce object.
///
///  \param[in] limit Limit to hit for debounce (threshold).
///  \param[in] resetOnMiss If a miss happens, reset count.
///
////////////////////////////////////////////////////////////////////////////////////
void Debounce::Initialize(int limit, bool resetOnMiss)
{
    mLimit = limit;
    mResetOnMiss = resetOnMiss;
    mCount = 0;
    mTriggered = false;
}

void Debounce::ResetDebounce()
{
    mCount = 0;
    mTriggered = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Bounce/count a hit.
///
///  \param[in] targetHit True if a hit, false causes a miss.
///
///  \return True if triggered, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Debounce::Bounce(bool targetHit)
{
    bool ret = false;
    if (targetHit)
    {
        ret = Hit();
    }
    else
    {
        ret = Miss();
    }
    return ret;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if triggered, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Debounce::GetDebounced()
{
    return mTriggered;
}

int Debounce::GetCount()
{
    return mCount;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Does a hit, incrementing the count.  If above limit, trigger happens.
///
///  \return True if triggered, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Debounce::Hit()
{
    if (++mCount >= mLimit)
    {
        mCount = mLimit;
        mTriggered = true;
    }
    return mTriggered;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Does a miss.
///
///  \return True if triggered, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Debounce::Miss()
{
    if (mResetOnMiss)
    {
        mCount = 0;
        mTriggered = false;
    }
    else 
    {
        if (--mCount <= 0)
        {
            mCount = 0;
            mTriggered = false;
        }
    }
    return mTriggered;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Trigger limit/threshold.
///
////////////////////////////////////////////////////////////////////////////////////
int Debounce::GetLimit()
{
    return mLimit;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if resets happen on a miss.
///
////////////////////////////////////////////////////////////////////////////////////
bool Debounce::GetResetOnMiss()
{
    return mResetOnMiss;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the limit/threshold for trigger.
///
////////////////////////////////////////////////////////////////////////////////////
void Debounce::SetLimit(int limit)
{
    mLimit = limit;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the reset debounce on a miss flag.
///
////////////////////////////////////////////////////////////////////////////////////
void Debounce::SetResetOnMiss(bool resetOnMiss)
{
    mResetOnMiss = resetOnMiss;
}
/* End of File */

