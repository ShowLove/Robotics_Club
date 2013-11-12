/////////////////////////////////////////////////////////////////////////////
///
///  \file torpedo.h
///  \brief Torpedo (Cupid) mission for AUVC 2011
///
///  Author(s): Michael Scherer<br>
///  Created: 2010<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: michael.scherer@knights.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#ifndef __ZEBULON_UNDERWATER_TORPEDO_H
#define __ZEBULON_UNDERWATER_TORPEDO_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/path.h"
#include "auvc/underwater/controls.h"
#include "statemachine/utility/objectscan.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class Torpedo : public Underwater::Mission
        {
        public:
            static const std::string NextMission;
            
            enum State
            {
                Find,           ///< Figure out what to do
                Search,         ///< Search for the heart
                
                GetDepth,       ///< Find the depth of the heart
                Advance,        ///< Go to the depth offset and advance
                Fire,           ///< Shoot through the heart, the robot's to blame
                
                PathSearch,     ///< Look for path or heart
                DoLineup,       ///< We saw the heart but not the path, just try to align yaw
                DoPath,         ///< Do the path stuff
                ExitPath,       ///< Leave path towards heart
                
                Exit            ///< Exit from the mission
            };
            
            Torpedo();
            ~Torpedo();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
            std::string GetStateName(State state);
            
        private:
            State mState;                               ///< The current state
            Zebulon::AI::StopWatch mStateTimeout;       ///< Global timeout for the state
            
            Controls mCurrent;                          ///< Current controls state (speeds, depth, etc)
            Controls mDesired;                          ///< Desired controls state
            
            double mStartYaw;                           ///< The initial yaw of the vehicle (aligned with path or buoys)
            double mDefaultDepth;                       ///< Default depth to go to when no other alignment given
            
            IplImage* mForwardFrame;                    ///< The forward camera's frame
            IplImage* mDownwardFrame;                   ///< The downward camera's frame
            IplImage* mSegFrame;                        ///< The Segmented frame.
            IplImage* mThreshFrame;                     ///< The thresholded frame.
            IplImage* mProcessedImage;                  ///< The final frame
            
            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            
            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search
            
            Zebulon::AI::Debounce mTargetDebounce;      ///< Debounce the loss of a target from view
            Vision::Return mTempReturn;                 ///< Return from the color finder/classifier
            Vision::Return mHorizontileReturn;
            Vision::Target mTarget;                     ///< What the classifier should look for
            
            double mLatPixelScale;
            double mDepthPixelScale;
            double mYawPixelScale;
            double mAxialPixelScale;
            
            double mMinArea;
            double mMaxArea;
            double mArea;
            double mTargetArea;
            double mAreaThresh;
            
            Zebulon::AI::Debounce mCenteredDebounce;
            
            double mCenterI;
            double mCenterJ;
            double mCenterOffsetJ;
            
            double mIThresh;
            double mJThresh;
            double mDepthThresh;
            
            double mICloseThresh;
            double mLeftBound;
            
            double mHedgePassedJ;
            
            double mDepthOffset;
            
            double mAdvanceThrust;
            double mLockHeadingThrust;
            double mLockHeadingDuration;
            Zebulon::AI::StopWatch mLockHeadingTimer;
            
            PathFinder mPathFinder;
            Zebulon::AI::Debounce mPathDebounce;
            Zebulon::AI::StopWatch mPathTimer;
            double mExitPathThrust;
            double mPathExitTime;
            
            double mSearchStrafeTime;
            double mSearchStrafeSpeed;
            double mSearchForwardTime;
            double mSearchForwardSpeed;
            double mSearchSmoothSpeed;
            Zebulon::AI::StrafeSearch mSearchPattern;
            
            Zebulon::AI::StrafeSearch mPathSearchPattern;
            double mPathSearchStrafeTime;
            double mPathSearchStrafeSpeed;
            double mPathSearchForwardTime;
            double mPathSearchForwardSpeed;
            double mPathSearchSmoothSpeed;
            Zebulon::AI::StopWatch mPathSearchTimeout;
            double mPathSearchDuration;
            
            double mVerticalCenter;
            double mVerticalOffset;
            
            AI::ObjectScan mObjectScanData;
            CxUtils::Point3D::List mRawLaserData;
            CxUtils::Point3D::List mLaserObjects;
            bool mLaserObjectValid;
            double mMinLaserObjectRadius;
            double mMaxLaserObjectRadius;
            double mCurrLaserDist;
            
            // Laser display
            //Zebulon::Utility::LaserDisplay mLaserDisplay;
            IplImage* mDisplayLaser;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TORPEDO_H
/* End of file */
