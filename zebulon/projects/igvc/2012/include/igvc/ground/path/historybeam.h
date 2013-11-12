////////////////////////////////////////////////////////////////////////////////////
///
/// \file potentialfields.h
/// \brief Class extends Cartographer::Navigator, generates paths using a
///         potential fields algorithm
///
/// Author(s): Michael Scherer<br>
/// Created: 2011<br>
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

#ifndef __ZEBULON_GROUND_HISTORYBEAM_H__
#define __ZEBULON_GROUND_HISTORYBEAM_H__

#include <cartographer/planning/navigator.h>
#include <limits>

namespace Zebulon
{
    namespace IGVC
    {
        /**
          * I'm just playing around with this to test out cartographer stuff...
          *
          */
        class HistoryBeam : public Cartographer::Navigator
        {
        public:
            enum ParameterLabels
            {
                PathWidthMeters = 0,
                AngleIncrementDegrees,
                SweepAngleStartDegrees,
                SweepAngleEndDegrees,

                SegmentLengthMeters,

                HistoryDepth,
                SearchDepth,

                StraightnessWeight,
                DistanceWeight,
                SimplicityWeight,
                AngleWeight,
                HeadingWeight,
                LinePotentialWeight
            };

            HistoryBeam();
            HistoryBeam(Cartographer::Navigator::Settings* settings);
            ~HistoryBeam() {}

            virtual int GetPath(const Cartographer::Pose& start,
                                const Cartographer::Pose& end,
                                Cartographer::Path& pathFound,
                                Cartographer::Path::List * allPaths = NULL,
                                const std::string& mapName = "Default");

            virtual double CalculateFitness(const Cartographer::Path& path, const Navigator* navinfo = NULL);

            virtual void ResetHistory();

            void SetLaneHeading(double heading)
            {
                mMutex.Lock();
                mLaneHeading = heading;
                mLaneHeadingSet = true;
                mMutex.Unlock();
            }

            double GetLaneHeading()
            {
                double ret;
                mMutex.Lock();
                if (mLaneHeadingSet)
                    ret = mLaneHeading;
                else
                    ret = std::numeric_limits<double>::quiet_NaN();
                mMutex.Unlock();
                return ret;
            }

            void ResetLaneHeading()
            {
                mMutex.Lock();
                mLaneHeadingSet = false;
                mMutex.Unlock();
            }
            bool IsLaneHeadingSet() const { return mLaneHeadingSet; }
        protected:
            bool CreatePaths(const Cartographer::Path& seed,
                             Cartographer::Path::List& pathList,
                             const int depth);

            Cartographer::Path mDriveHistory;
            Cartographer::Path mLastDesiredPath;

            Cartographer::Pose mStart;
            Cartographer::Pose mDestination;
            const Cartographer::ObjectMap* mMap;

            CxUtils::Mutex mMutex;

        private:
            double mLaneHeading;
            bool mLaneHeadingSet;

            double mWidth;
            double mLength;

            double mStartAngle;
            double mEndAngle;
            double mIncrement;

            int mSearchDepth;

            double mStraightnessWeight;
            double mDistanceWeight;
            double mSimplicityWeight;
            double mAngleWeight;
            double mHeadingWeight;
            double mLinePotentialWeight;
        };
    }
}

#endif // __ZEBULON_GROUND_HISTORYBEAM_H__
