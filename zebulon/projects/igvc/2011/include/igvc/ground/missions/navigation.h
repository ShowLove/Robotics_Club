////////////////////////////////////////////////////////////////////////////////////
///
/// \file autonomous.h
/// \brief Main mission for the autonomous challenge
///
/// Author(s): Michael Scherer<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu<br>
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

#ifndef __ZEBULON_GROUND_NAVIGATION_H
#define __ZEBULON_GROUND_NAVIGATION_H

#include "igvc/ground/missions/mission.h"

#include "igvc/ground/vision/groundvision.h"

#include "igvc/ground/path/quadtreemap.h"
#include "igvc/ground/path/laneline.h"
#include "igvc/ground/path/historybeam.h"

#include "filter/filter.h"
#include "filter/rcfilter.h"
#include "statemachine/utility/stopwatch.h"

#include "statemachine/utility/debounce.h"

#include <cartographer/maps/worldmap.h>
#include <cartographer/objects/sphere.h>
#include <cartographer/objects/line.h>
#include <cartographer/planning/beamsearch.h>

#include <display/display.h>

#include <vector>
#include <stdio.h>

namespace Zebulon
{
    namespace IGVC
    {
        class Navigation : public IGVC::Mission
        {
        public:
            enum State
            {
                Standby,
                Start,
                DriveOut,
                GoToWaypoint,
                NextWaypoint,
                Finish
            };

            Navigation();
            ~Navigation();

            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();

        private:
            State mState;
            int mLoop;

            Utm mCurrentPosition;
            std::vector<Utm> mDesiredPositionList;
            std::vector<Utm>::iterator mDesiredPositionIterator;
            Utm mDesiredPosition;

            std::vector<Utm> mBorderWaypointsVector;
            bool mLastWaypoint;

            Utm mStartPosition;
            Utm mStartSecondaryPosition;

            Orientation mCurrentOrientation;
            Orientation mDesiredOrientation;

            double mCurrentVelocity;
            double mDesiredVelocity;
            double mMaxVelocity;

            double mDistanceThresh;

            // Camera
            IplImage* mForwardCamera;

            // LIDAR
            CxUtils::Point3D::List mRangeData;

            // Cartographer Stuff
            Cartographer::ObjectMap* mMap;
            //Cartographer::BeamSearch mNavigator;
            HistoryBeam mNavigator;
            Cartographer::Path mWaypoints;
            AI::Debounce mWaypointDebounce;

            static const unsigned int VisionSourceID;
            static const unsigned int LaserSourceID;
            static const unsigned int BorderSourceID;

            // Map Parameters
            double mLaserConsumeThreshold;
            double mVisionConsumeThreshold;
            double mLaserTimeToLive;                        ///< time to live of objects found using the LIDAR
            double mVisionTimeToLive;                       ///< time to live of objects found using the vision system

            // History Beam Parameters
            double mPathWidthMeters;
            double mAngleIncrementDegrees;
            double mSweepAngleStartDegrees;
            double mSweepAngleEndDegrees;
            double mSegmentLengthMeters;
            double mHistoryDepth;
            double mSearchDepth;

            double mStraightnessWeight;
            double mDistanceWeight;
            double mSimplicityWeight;
            double mAngleWeight;
            double mHeadingWeight;
            double mLinePotentialWeight;
            static const std::string MapName;

            // Just for drawing
            double mPixelsPerMeter;

            // For driving out
            AI::StopWatch mStopWatch;
        };
    }
}
#endif // __ZEBULON_GROUND_NAVIGATION_H
