////////////////////////////////////////////////////////////////////////////////////
///
/// \file cartographerbeam.h
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
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

#ifndef __ZEBULON_GROUND_CARTOGRAPHERBEAM_H
#define __ZEBULON_GROUND_CARTOGRAPHERBEAM_H

#include "igvc/ground/missions/mission.h"

#include "igvc/ground/vision/groundvision.h"

#include <cartographer/maps/worldmap.h>
#include <cartographer/objects/sphere.h>
#include <cartographer/planning/beamsearch.h>

#include <display/display.h>

#include <vector>
#include <stdio.h>

namespace Zebulon
{
    namespace IGVC
    {
        class CartographerBeam : public IGVC::Mission
        {
        public:
            enum State
            {
                GoToWaypoint,
                NextWaypoint,
                Finish
            };

            CartographerBeam();
            ~CartographerBeam();

            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();

        private:
            State mState;

            Utm mCurrentPosition;
            std::list<Utm> mDesiredPositionList;
            std::list<Utm>::iterator mDesiredPositionIterator;
            Utm mDesiredPosition;

            Orientation mCurrentOrientation;
            Orientation mDesiredOrientation;

            double mCurrentVelocity;
            double mDesiredVelocity;
            double mMaxVelocity;
            
            double mDistanceThresh;

            // Camera
            IplImage* mForwardCamera;
            std::vector<CxUtils::Segment3D> mLaneLines;

            // LIDAR
            CxUtils::Point3D::List mRangeData;

            // Cartographer Stuff
            Cartographer::ObjectMap* mMap;
            Cartographer::BeamSearch mNavigator;
            Cartographer::Path mWaypoints;

            // Map Parameters
            double mConsumeThreshold;

            // Beam Search Parameters
            double mPathWidth;
            int mSearchDepth;
            double mSegmentLengthMeters;
            double mScansPerSweep;
            double mAngleScannedRads;
            int mBranchesPerDepth;
            double mAngleSweepIncrementRads;
            double mEndpointThreshold;

            static const std::string MapName;

            // Just for drawing
            double mPixelsPerMeter;
        };
    }
}
#endif // __ZEBULON_GROUND_CARTOGRAPHERBEAM_H
