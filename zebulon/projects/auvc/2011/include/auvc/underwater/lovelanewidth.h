/////////////////////////////////////////////////////////////////////////////
///
///  \file lovelanewidth.h
///  \brief Love Lane mission for AUVC 2011
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
#ifndef __ZEBULON_UNDERWATER_LOVELANEWIDTH_H
#define __ZEBULON_UNDERWATER_LOVELANEWIDTH_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "auvc/underwater/vision/lovelaneobject2.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/path.h"
#include "auvc/underwater/controls.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class LoveLaneWidth : public Underwater::Mission
        {
        public:
            static const std::string NextMission;
            
            enum State
            {
                Find,           ///< Figure out what to do
                Search,         ///< Search for the hedge
                
                GetDepth,       ///< Find the depth of the hedge
                Advance,        ///< Go to the depth offset and advance
                LockHeading,    ///< Drive through the hedge
                
                PathSearch,     ///< Look for path or buoys
                DoLineup,       ///< We saw the buoys but not the path, just try to align yaw
                DoPath,         ///< Do the path stuff
                ExitPath,       ///< Leave path towards hedge
                
                Exit            ///< Exit from the mission
            };
            
            LoveLaneWidth();
            ~LoveLaneWidth();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            State mState;
            Zebulon::AI::StopWatch mStateTimeout;
            
            Controls mCurrent;
            Controls mDesired;
            
            double mStartYaw;
            double mDefaultDepth;
            
            IplImage* mForwardFrame;
            IplImage* mDownwardFrame;
            IplImage* mProcessedImage;                  ///< The final frame
            
            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search
            
            Zebulon::AI::Debounce mHorizontileDebounce;
            Vision::Return mTempReturn;
            Vision::Return mHorizontileReturn;
            Vision::Target mTarget;
            
            LoveLaneObject2 mLoveLaneObject;
            
            double mLatPixelScale;
            double mDepthPixelScale;
            double mYawPixelScale;
            
            double mTargetArea;
            
            double mTargetWidth;
            double mMaxWidth;
            
            Zebulon::AI::Debounce mCenteredDebounce;
            
            double mCenterI;
            double mCenterJ;
            
            double mCenterIOffset;
            int mLaneOrientation;                           ///< 1 == L, -1 == J
            
            double mIThresh;
            double mJThresh;
            double mDepthThresh;
            double mAngleThresh;
            
            double mICloseThresh;
            
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
            
            double mVerticalOffset;
            double mYawThresh;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_LOVELANEWIDTH_H
/* End of file */
