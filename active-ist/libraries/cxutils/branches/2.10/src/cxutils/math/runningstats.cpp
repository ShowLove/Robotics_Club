////////////////////////////////////////////////////////////////////////////////////
///
///  \file runningstats.cpp
///  \brief Simple class for measuring certain statistics dynamically that is
///         numerically efficient.
///
///  This code is based off of similar class written by John D. Cook
///  http://www.johndcook.com/standard_deviation.html
///
///  Which is based on the following references:
///  Chan, Tony F.; Golub, Gene H.; LeVeque, Randall J. (1983). 
///  Algorithms for Computing the Sample Variance: Analysis and 
///  Recommendations. The American Statistician 37, 242-247.
///
///  Ling, Robert F. (1974). Comparison of Several Algorithms for 
///  Computing Sample Means and Variances. Journal of the American 
///  Statistical Association, Vol. 69, No. 348, 859-866.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 Januar 2010
///  <br>Copyright (c) 2010
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
#include "cxutils/math/runningstats.h"
#include <cmath>
#include <algorithm>

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
RunningStats::RunningStats()
{
    Clear();
    mDirtyFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor
///
////////////////////////////////////////////////////////////////////////////////////
RunningStats::RunningStats(const RunningStats& stats)
{
    Clear();
    *this = stats;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
RunningStats::~RunningStats()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets the running statistics.
///
////////////////////////////////////////////////////////////////////////////////////
void RunningStats::Clear()
{
    mCount = 0;
    mOldMean = 0;
    mNewMean = 0;
    mOldStd = 0;
    mNewStd = 0;
    mValues.clear();
    mDirtyFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a new metric to the running statistics.
///
///   \param[in] value Data value to add to the running statistics.
///
////////////////////////////////////////////////////////////////////////////////////
void RunningStats::Push(const double value)
{
    mCount++;
    // See Knuth TAOCP vol 2, 3rd edition, page 232
    if(mCount == 1)
    {
        mOldMean = mNewMean = value;
        mOldStd = mNewStd = 0.0;
    }
    else
    {
        mNewMean = mOldMean + (value - mOldMean)/mCount;
        mNewStd = mOldStd + (value - mOldMean)*(value - mNewMean);
        
        // Set up for next iteration.
        mOldMean = mNewMean;
        mOldStd = mNewStd;
    }
    mValues.push_back(value);
    mDirtyFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets the standard deviation.
///
////////////////////////////////////////////////////////////////////////////////////
void RunningStats::SetStandardDeviation(const double std)
{
    if(mCount <= 1)
    {
        mOldStd = mNewStd = 0;
    }
    else
    {
        mOldStd = mNewStd = (std*std)*mCount - 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The mean of the data points.
///
////////////////////////////////////////////////////////////////////////////////////
double RunningStats::Mean() const
{
    return (mCount > 0) ? mNewMean : 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The median of the data points.
///
////////////////////////////////////////////////////////////////////////////////////
double RunningStats::Median() const
{
    double m = 0.0;
    // Sort
    if(mDirtyFlag)
    {
        std::vector<double> *ptr = (std::vector<double> *)&mValues;
        std::sort(ptr->begin(), ptr->end());
        *((bool *)&mDirtyFlag) = false;
    }
    if(mValues.size() > 0)
    {
        if(mValues.size() % 2 == 0) // Even number
        {
            if(mValues.size() == 2)
            {
                m = (mValues.front() + mValues.back())/2.0;
            }
            else
            {
                unsigned int middle = (unsigned int)(mValues.size()/2);
                m = (mValues[middle - 1] + mValues[middle])/2.0;
            }
        }
        else    // Odd
        {
            if(mValues.size() == 1)
            {
                m = mValues.front();
            }
            else
            {
                m = mValues[mValues.size()/2];
            }
        }
    }
    return m;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The variance of the data points.
///
////////////////////////////////////////////////////////////////////////////////////
double RunningStats::Variance() const
{
    return (mCount > 1) ? mNewStd/(mCount - 1) : 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The standard deviation of the data points.
///
////////////////////////////////////////////////////////////////////////////////////
double RunningStats::StandardDeviation() const
{
    return sqrt(Variance());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
RunningStats& RunningStats::operator =(const RunningStats& stats)
{
    if(this != &stats)
    {
        mCount = stats.mCount;
        mOldMean = stats.mOldMean;
        mNewMean = stats.mNewMean;
        mOldStd = stats.mOldStd;
        mNewStd = stats.mNewStd;
    }
    return *this;
}

/*  End of File */
