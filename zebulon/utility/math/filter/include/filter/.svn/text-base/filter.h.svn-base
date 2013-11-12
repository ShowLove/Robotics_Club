/////////////////////////////////////////////////////////////////////////////////////
///
/// \file filter.h
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
#ifndef _ZEBULON_UTILITY_FILTER__H
#define _ZEBULON_UTILITY_FILTER__H


#include <cstdio>
#include <cmath>
#include <string.h>
#include <cxutils/mutex.h>

//////////////////////////////////////////////////////////////////////////////////// 
///  
///  \def ZEB_FILTER_UTIL_DLL
///  \brief Pre-processor definition used for exporting/importing DLL for library.
///
////////////////////////////////////////////////////////////////////////////////////
#ifndef ZEB_FILTER_UTIL_DLL
    #ifdef WIN32
        #define ZEB_FILTER_UTIL_DLL __declspec(dllimport)
    #else
        #define ZEB_FILTER_UTIL_DLL
    #endif
#endif

namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Filter
        ///   \brief Defines a filter object.  All filters are derived from this
        ///          interface.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL Filter
        {
        public:
            //  All filters require a size (data values count/buffer size).
            Filter(int size);
            // Destructor.
            virtual ~Filter();
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
            virtual int Setup(double minVal,double maxVal);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Add a new data point to be filtered.
            ///
            ///   \param[in] value Data point to add to filter.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual void Add(double value);
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Gets the output of the filter.
            ///
            ///   \param[in] value Output value of filter.
            ///
            ///   \return 1 if data is valid, 0 otherwise.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual int Output(double &value) const;
        protected:
            // Overload this method.
            virtual int Process();
            CxUtils::Mutex mMutex;; ///<  For thread protection of data.
            int mSize;              ///<  Number of values used in filter.
            int mCount;             ///<  Keeps track of elements in circular array.
            double *mpData;         ///<  Data points collected so far for filter use.
            double mOutput;         ///<  Previous output value generated.
            double mPreWrapOut;     ///<  Previoius wrapping value (for data that wraps around 0 like a compass).
            int mValid;             ///<  1 if data is valid (have enough points for valid output) 0 otherwise.
        private:
            int mWrap;              ///<  Flag set when wrapping data points.
            double mMinValue;       ///<  Minimum value (limit).
            double mMaxValue;       ///<  Maximum value (limit).
            double mHalfRange;      ///<  Half range value.
            double mRange;          ///<  Range of values.
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class AverageFilter
        ///   \brief Averaging filter (averages data).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL AverageFilter : public Filter
        {
        public:
            AverageFilter(int size);
            ~AverageFilter();
        protected:
            virtual int Process();
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class MedianFilter
        ///   \brief Implementation of a median filter (gets median value).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL MedianFilter : public Filter
        {
        public:
            MedianFilter(int size);
            ~MedianFilter();
        protected:
            virtual int Process();
        private:
            double *mpSortedData;   ///<  Sorted data points.
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class KalmanLinearFilter
        ///   \brief Kalman filter for single data axis.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL KalmanLinearFilter : public Filter
        {
        public:
            KalmanLinearFilter(int size,double kalmanQ=1.0,double kalmanR=8.0, double kalmanP = 16.0);
            ~KalmanLinearFilter();
        protected:
            virtual int Process();
            double mKalmanPNext;    ///< Projected error.
            double mKalmanG;        ///< Kalman gain.
            double mState;          ///< State data for filter.
            double mKalmanP;        ///< Co-Variance matrix data.
            double mKalmanQ;        ///< Q parameter for filter (error).
            double mKalmanR;        ///< Residual covariance.

        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class FIRFilter
        ///   \brief Finite Impulse Response (FIR) filter.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL FIRFilter : public Filter
        {
        public:
            FIRFilter(int Size,double CutOff=0.25);
            ~FIRFilter();
        protected:
            virtual int Process();
        private:
            double *mpFirConstants;
            int mM;
            double mCutOff;
        };

    }
}
#endif
