/////////////////////////////////////////////////////////////////////////////
///
/// \file gpsvelocity.h
/// \brief Creates a virtual velocity sensor from the IMU
///
/// Author(s): Michael Scherer, Gary Stein<br>
/// Created: 5/25/2010<br>
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

#ifndef _ZEBULON_UTILITY_MATH_GPSVELOCITY__H
#define _ZEBULON_UTILITY_MATH_GPSVELOCITY__H

#include <math.h>
#include <string.h>
#include <cxutils/mutex.h>
#include <filter/filter.h>

namespace Zebulon
{
    namespace Utility
    {
        class GPSVelocity
        {
            public:
                GPSVelocity();
                virtual ~GPSVelocity();
            
                void SetValues(double time,double easting,double northing,double heading);
                
                static const double MinDistance=0.25;
                //too far to be reality
                static const double MaxDistance=5.0;
                
                static const double MPS2MPH = 2.23693629;
                
                double GetAxialVelocity();
                double GetLateralVelocity();
                
                double GetEasting();
                double GetNorthing();
                
                int IsReady();
                
            private:
                //Does actual calculation (on timer later?)
                int Calc();
                double CalcDistance(double e1,double n1,double e2,double n2);
                double CalcAngle(double e1,double n1,double e2,double n2);
                
                double mEasting;
                double mNorthing;
                double mHeading;
                double mTime;

                double mOldEasting;
                double mOldNorthing;
                double mOldTime;
                int mFoundOld;

                double mAxialVelocity;
                double mLateralVelocity;
                int mCount;
                int mReady;
                
                const static int MaxList = 1024;
                
                double mEastingList[MaxList];
                double mNorthingList[MaxList];
                double mTimeList[MaxList];
                int mListPos;
                
                
                Filter *mFilterX;
                Filter *mFilterY;
                Filter *mFilterH;
                
                Filter *mFilterC;
                
                Filter *mFilterA;
                Filter *mFilterL;
        };
    }
}

#endif // _ZEBULON_UTILITY_MATH_GPSVELOCITY__H
/* End of file */