////////////////////////////////////////////////////////////////////////////////////
///
///  \file runningstats.h
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
#ifndef __CXUTILS_MATH_RUNNING_STATS__H
#define __CXUTILS_MATH_RUNNING_STATS__H

#include "cxutils/cxbase.h"
#include <vector>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RunningStats
    ///   \brief A simple class for dynamically calculating certain statistics such as
    ///          Mean, Variance, Standard Deviation, and Average.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL RunningStats
    {
    public:
        typedef std::vector<RunningStats> List;
        RunningStats();
        RunningStats(const RunningStats& stats);
        ~RunningStats();
        void Clear();
        void SetCount(const unsigned int count) { mCount = count; }
        void SetMean(const double mean) { mOldMean = mNewMean = mean; } 
        void SetStandardDeviation(const double std);
        void Push(const double value);
        inline unsigned int GetNumberOfValues() const { return mCount; }
        double Mean() const;
        double Median() const;
        double Variance() const;
        double StandardDeviation() const;       
        RunningStats& operator=(const RunningStats& stats);
    private:
        unsigned int mCount;    ///<  Number of data values.
        double mOldMean;        ///<  Previous mean.
        double mNewMean;        ///<  Current mean.
        double mOldStd;         ///<  Old Standard Deviation.
        double mNewStd;         ///<  New Standard Deviation.
        std::vector<double> mValues; ///<  Running values for median calculations.
        bool mDirtyFlag;             ///<  Dirty if vector needs sorting.
    };
}


#endif
/*  End of File */
