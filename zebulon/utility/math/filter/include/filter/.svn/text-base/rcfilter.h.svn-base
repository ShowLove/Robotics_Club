/////////////////////////////////////////////////////////////////////////////
///
/// \file rcfilter.h
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
#ifndef _ZEBULON_UTILITY_RC_FILTER__H
#define _ZEBULON_UTILITY_RC_FILTER__H

#include <filter/filter.h>
#include <stdlib.h>

namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class SeriesFilter
        ///   \brief This kind of filter is a set of filters which get execunted on
        ///          data in the order that they were added.  The output of one goes to
        ///          the input of the next and so on.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL SeriesFilter : public Filter
        {
        public:
            SeriesFilter(int size);
            ~SeriesFilter();
            virtual void Add(double value);
            int AddFilter(Filter* f);
        private:
            Filter** mFilters;  ///<  Filters in series.
            int mNumFilters;    ///<  Max number of filters (max).
            int mFilterIndex;   ///<  Filter index (for array).
        };
        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class LowPassFilter
        ///   \brief Low pass filter (only low values/changes pass through).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL LowPassFilter : public Filter
        {
        public:
            LowPassFilter(double dt, double rc);
            virtual ~LowPassFilter();
            virtual int Process();
        private:
            double mAlpha;  ///<  Ratio of sampling time (dt) to time constant (rc). 
        };
        
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class HighPassFilter
        ///   \brief High pass filter (only high values/changes pass through).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_FILTER_UTIL_DLL HighPassFilter : public Filter
        {
        public:
            HighPassFilter(double dt, double rc);
            virtual ~HighPassFilter();
            virtual int Process();
        private:
            double mAlpha;  ///<  Ratio of sampling time (dt) to time constant (rc). 
        };
    }
}

#endif
/* End of file */