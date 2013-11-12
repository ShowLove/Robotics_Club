/////////////////////////////////////////////////////////////////////////////////////
///
/// \file filter.cpp
/// \brief Defines some basic filters and a filter interfaces for mpData
///        processing.
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////
#include "filter/filter.h"
#include <cmath>
#include <limits>
#ifdef WIN32
#include <cxutils/math/cxmath.h>
#endif

using namespace Zebulon;
using namespace Utility;

#ifdef WIN32
    #define M_PI CxUtils::CX_PI
#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] size Number of data points to use within filter.
///
////////////////////////////////////////////////////////////////////////////////////
Filter::Filter(int size)
{
    mSize = size;
    mpData = new double[mSize];
    mCount = 0;
    mOutput = 0.0;
    mPreWrapOut = 0.0;
    //  Assume not wrapped originally
    mWrap = 0;
    mValid = 0;
    
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Filter::~Filter()
{
    delete [] mpData;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to setup limits for data.  This method only really needs to
///   to be used if you have data that wraps around a point.  For example, a compass
///   has values pass 0,360 or -180, 180, data will then be wrapped then
///   unwrapped.
///
///   \param[in] minVal Minimum value.
///   \param[in] maxVal Maximum value.
///
///   \return 0 on error, 1 on success.
///
////////////////////////////////////////////////////////////////////////////////////
int Filter::Setup(double minVal,double maxVal)
{
    //wrapping is happening if called
    mWrap=1;
    mMinValue = minVal;
    mMaxValue = maxVal;
    mRange = (maxVal - minVal);
    mHalfRange = mRange/2.0;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Add a new data point to be filtered.
///
///   \param[in] value Data point to add to filter.
///
////////////////////////////////////////////////////////////////////////////////////
void Filter::Add(double value)
{
    mMutex.Lock();
    
    // Value wrapped to be like output
    if(mWrap == 1)
    {
        while(value >= mPreWrapOut+mHalfRange)
        {
            value -= mRange;
        }
        while(value < mPreWrapOut - mHalfRange)
        {
            value += mRange;
        }
    }
    // Add to end of circular list
    mpData[mCount%mSize] = value;
    mCount++;
    
    //does real process, puts answer in mOutput
    mValid = Process();

    //if mWrap, fix on mOutput level to inside mRange
    if(mWrap == 1)
    {
        mPreWrapOut = mOutput;
        while(mOutput >= mMaxValue)
        {
            mOutput -= mRange;
        }
        while(mOutput < mMinValue)
        {
            mOutput += mRange;
        }
    }
    
    mMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the output of the filter.
///
///   \param[in] value Output value of filter.
///
///   \return 1 if data is valid, 0 otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
int Filter::Output(double &value) const
{
    int tmp;
    mMutex.Lock();
    value = mOutput;
    tmp = mValid;
    mMutex.Unlock();
    return tmp;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Virtual method to overload which processes data.
///
////////////////////////////////////////////////////////////////////////////////////
int Filter::Process()
{
    // Default return current mValue
    if(mCount > 0)
    {
        mOutput = mpData[(mCount-1)%mSize];
        return 1;
    }
    else
    {
        mOutput = 0.0;
        return 0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
AverageFilter::AverageFilter(int size) : Filter(size)
{
    // nothing extra
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
AverageFilter::~AverageFilter()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int AverageFilter::Process()
{
    if(mCount < mSize)
    {
        mOutput = 0.0;
        return 0;
    }
    else
    {
        mOutput = 0.0;
        // Order does not matter
        for(int i = 0; i < mSize; i++)
        {
            mOutput += mpData[i];
        }
        mOutput /= mSize;
        return 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MedianFilter::MedianFilter(int size) : Filter(size)
{
    // Store list of sorted values for median
    mpSortedData = new double[mSize];
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MedianFilter::~MedianFilter()
{
    delete [] mpSortedData;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int MedianFilter::Process()
{
    if(mCount < mSize)
    {
        mOutput = 0.0;
        return 0;
    }
    else
    {
        //easy way to do median filter is to sort
        //then grab the middle, for sake of easy, do bubble
        //make a copy
        int inversion=1;
        double tmpval;
        memcpy(mpSortedData, mpData, mSize*sizeof(double));
        //bubble sort
        //if no inversions happen, exit early
        for(int j = 0; j < mSize-1 && inversion == 1; j++)
        {
            inversion = 0;
            for(int i = mSize-1; i > j; i--)
            {
                //check for an inversion
                //if true, swap
                if(mpSortedData[i-1]>mpSortedData[i])
                {
                    inversion=1;
                    tmpval=mpSortedData[i-1];
                    mpSortedData[i-1]=mpSortedData[i];
                    mpSortedData[i]=tmpval;
                }
            }
        }
        //if even, average the two middle
        if(mSize%2==0)
        {
            mOutput=(mpSortedData[mSize/2-1]+mpSortedData[mSize/2])/2.0;
        }
        //odd pick middle
        else
        {
            mOutput=mpSortedData[mSize/2];
        }
        return 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
KalmanLinearFilter::KalmanLinearFilter(int size,
                                       double kalmanQ,
                                       double kalmanR,
                                       double kalmanP) : Filter(size)
{
    //default values
    mKalmanP = kalmanP;
    //mKalmanQ=1.0;
    //mKalmanR=8.0;
    mKalmanQ = kalmanQ;
    mKalmanR = kalmanR;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
KalmanLinearFilter::~KalmanLinearFilter()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int KalmanLinearFilter::Process()
{
    if(mCount < mSize)
    {
        mState = mpData[(mCount-1)%mSize];
        mOutput = 0.0;
        return 0;
    }
    else
    {
        //added projected error
        mKalmanPNext = mKalmanP+mKalmanQ;
        //calculate Kalman gain
        mKalmanG = mKalmanPNext/(mKalmanPNext+mKalmanR);
        //take projected and combined measured
        mState = mState+mKalmanG*(mpData[(mCount-1)%mSize]-mState);
        //update co-varience matrix
        mKalmanP = (1-mKalmanG)*mKalmanPNext;
        //printf("%lf %lf %lf %lf\n",mKalmanP,mKalmanG,mState,mpData[(mCount-1)%mSize]);
        mOutput = mState;
        return 1;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
FIRFilter::FIRFilter(int size,double cutOff) : Filter(size)
{
    //really not Size=N because of odd/even issues
    //and would need N+1 values for an N filter
    mpFirConstants=new double[size];
    mCutOff=cutOff;
    mM= mSize-1;
    for(int n = 0; n < mSize;n++)
    {
        if(n==mM/2)
        {
            mpFirConstants[n] = 2*mCutOff;
        }
        else
        {
            mpFirConstants[n] = sin(2*M_PI*mCutOff*(n-mM/2))/(M_PI*(n-mM/2));
        }
        //printf("Filter[%d]=%lf\n",n,mpFirConstants[n]);
    }
    double normal=0.0;
    for(int n = 0; n < mSize; n++)
    {
        //using a Hann Window
        //mpFirConstants[n]*=(0.5-0.5*cos(2*M_PI*n/mM));
        //using a Hamming Window
        mpFirConstants[n]*=(0.54-0.46*cos(2*M_PI*n/mM));
        //printf("Filter[%d]=%lf\n",n,mpFirConstants[n]);
        normal+=mpFirConstants[n];
    }
    //printf("Sum: %lf\n",normal);
    //normalize
    for(int n = 0 ; n< mSize; n++)
    {
        mpFirConstants[n] /= normal;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
FIRFilter::~FIRFilter()
{
    delete mpFirConstants;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Applies the filter.
///
////////////////////////////////////////////////////////////////////////////////////
int FIRFilter::Process()
{
    if(mCount<mSize)
    {
        mOutput=0.0;
        return 0;
    }
    else
    {
        double sum=0.0;
        for(int i = 0;i < mSize; i++)
        {
            sum += mpData[(mCount-1-i)%mSize]*mpFirConstants[i];
        }
        mOutput=sum;
        return 1;
    }
}

/*  End of File */
