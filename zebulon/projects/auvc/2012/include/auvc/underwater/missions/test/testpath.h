/////////////////////////////////////////////////////////////////////////////
///
///  \file TestPath.h
///  \brief Life Vest mission for the AUVC 2010
///
///  Author(s): Michael Scherer<br>
///  Created: 2010<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: michael.scherer@ieee.org<br>
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
#ifndef __ZEBULON_UNDERWATER_TestPath_H
#define __ZEBULON_UNDERWATER_TestPath_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
//#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"

#include <vector>

namespace Zebulon
{
    namespace Underwater
    { 
        class TestPath : public Underwater::Mission
        {
        public:
            enum State
            {
                Find,           // Entry state, figure out what to do
                PathSearch,     // Look for path or buoys
                DoPath,         // Do the path stuff
                ExitPath,       // Leave path towards life vests
            };
            
            TestPath();
            ~TestPath();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            State mState;
            
            //Controls mCurrent;
            //Controls mDesired;

            double mDesiredDepth;
            double mDesiredYaw;
            double mDesiredPitch;
            double mDesiredRoll;
            double mDesiredAxialThrust;
            double mDesiredLateralThrust;

            double mCurrentDepth;
            double mCurrentYaw;
            double mCurrentPitch;
            double mCurrentRoll;
            
            IplImage* mForwardFrame;
            IplImage* mDownwardFrame;
            IplImage* mDownwardFrameF;
            IplImage* mSegFrame;                        ///< The Segmented frame.
            IplImage* mThreshFrame;                     ///< The thresholded frame.
            IplImage* mProcessedImage;                  ///< The final frame
            IplImage* mProcessedDWImage;                  ///< The final downward frame
            
            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            
            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search
            
            Zebulon::AI::Debounce mDebounce[3];
            Vision::Return mReturn[3];
            Vision::Target mTarget[3];
            
            Zebulon::AI::Debounce mCenteredDebounce;
            double mIThresh;
            double mJThresh;
            double mCenterI;
            double mCenterJ;
            double mWidth;
            double mTargetArea;
            double mTargetWidth;
            //double area;
            
            double mYawFix;
            
            double mLatPixelScale;
            double mDepthPixelScale;
            double mAxialPixelScale;
            double mYawPixelScale;
            
            double mMinArea;
            double mMaxArea;
            double mAreaCloseThresh;
            
            double mLaserDist;
            double mLaserCloseDist;
            
            Zebulon::AI::StopWatch mNewStateTimer;
            
            double mLockAttackDuration;
            double mAttackThrust;
            int mAttackingBuoy;
            int mBuoyToAttack[2];
            int mBuoyOrder[3];
            Zebulon::AI::StopWatch mLockAttackTimer;
            
            double mFindActiveLateralThrust;
            Zebulon::AI::StopWatch mFindActiveTimer;
            double mFindActiveDuration;
            bool mFindActiveTimerActive;
            int mFindActiveModifier;
            bool mFindActiveFirstTime;
            
            double mLockUnhitDuration;
            double mUnhitThrust;
            
            double mApproachThrust;
            
            Zebulon::AI::StrafeSearch mSearchPattern;
            double mSearchStrafeTime;
            double mSearchStrafeSpeed;
            double mSearchForwardTime;
            double mSearchForwardSpeed;
            double mSearchSmoothSpeed;
            
            Zebulon::AI::StrafeSearch mPathSearchPattern;
            double mPathSearchStrafeTime;
            double mPathSearchStrafeSpeed;
            double mPathSearchForwardTime;
            double mPathSearchForwardSpeed;
            double mPathSearchSmoothSpeed;
            Zebulon::AI::StopWatch mPathSearchTimeout;
            double mPathSearchDuration;
            
            //PathFinder mPathFinder;
            //Zebulon::AI::Debounce mPathDebounce;
            Zebulon::AI::StopWatch mPathTimer;
            double mExitPathThrust;
            double mPathExitTime;
            
            double mEvadeLatThrust;
            double mEvadeLockTime;
            double mEvadeLockThrust;
            double mEvadeTime;
            Zebulon::AI::StopWatch mEvadeTimer;
            
            double mDefaultDepth;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TestPath_H
/* End of file */
