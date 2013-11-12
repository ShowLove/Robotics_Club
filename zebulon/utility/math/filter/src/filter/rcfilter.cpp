/////////////////////////////////////////////////////////////////////////////
///
/// \file rcfilter.cpp
/// \brief Low Pass and High Pass filters.
///
/// Author(s): Micheal Scherer<br>
/// Created: 5/18/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org <br>
/// Web: http://robotics.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////
#include "filter/rcfilter.h"

using namespace Zebulon;
using namespace Utility;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] size Maximum number of filters in series.
///
////////////////////////////////////////////////////////////////////////////////////
SeriesFilter::SeriesFilter(int size) : Filter(1)
{
    mFilters = (Filter**)malloc(size*sizeof(Filter*));
    mNumFilters = size;
    mFilterIndex = -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SeriesFilter::~SeriesFilter()
{
    for (int i = 0; i <= mFilterIndex; i++)
        free(mFilters[i]);
    free(mFilters);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Add a filter to the series.  You can't add any more than
///   you've initialized the series filter to support.
///
///   \param[in] f Filter to add.
///
///   \return 1 on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
int SeriesFilter::AddFilter(Filter* f)
{
    // You've filled up the buffer, and I'm not
    // going to even bother building in a mechanism
    // for resizing, because honestly, you shouldn't
    // be making this kind of mistake. This should
    // be initialized once at the top and that's that.
    if (mFilterIndex >= mNumFilters-1)
        return 0;
    mFilterIndex++;
    mFilters[mFilterIndex] = f;
    
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds data to the filter for processing.
///
///   \param[in] value Data to add.
///
////////////////////////////////////////////////////////////////////////////////////
void SeriesFilter::Add(double value)
{
    mMutex.Lock();

    int valid = true;
    double out = value;

    for (int i = 0; i <= mFilterIndex; i++)
    {
        mFilters[i]->Add(out);
        valid &= mFilters[i]->Output(out);
    }

    mOutput = out;
    mValid = valid;

    mMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] dt Time between values in seconds (dt).
///   \param[in] rc Time constant seconds.  
///                 (like a capacitor, will smooth data the larger).
///
////////////////////////////////////////////////////////////////////////////////////
LowPassFilter::LowPassFilter(double dt, double rc)
    : Filter(2)
{
    this->mAlpha = dt / (dt + rc);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LowPassFilter::~LowPassFilter()
{
    
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int LowPassFilter::Process()
{
    if(mCount < mSize)
    {
        mOutput = 0.0;
        return 0;
    }
    else
    {
        double mValue = mpData[(mCount-1)%mSize];
        mOutput = mOutput + this->mAlpha * (mValue - mOutput);
        return 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] dt Time between values in seconds (dt).
///   \param[in] rc Time constant seconds.  
///                 (like a capacitor, will smooth data the larger).
///
////////////////////////////////////////////////////////////////////////////////////
HighPassFilter::HighPassFilter(double dt, double rc)
    : Filter(2)
{
    this->mAlpha = rc / (dt + rc);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
HighPassFilter::~HighPassFilter()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int HighPassFilter::Process()
{
    if(mCount < mSize)
    {
        mOutput = 0.0;
        return 0;
    }
    else
    {
        double mValue = mpData[(mCount-1)%mSize];
        double prev = mpData[(mCount-2)%mSize];
        mOutput = this->mAlpha * (mOutput + mValue - prev);
        return 1;
    }
}

/*  End of File */
