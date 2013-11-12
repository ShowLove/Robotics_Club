/////////////////////////////////////////////////////////////////////////////
///
///  \file dropper.h
///  \brief Dropper mission for AUVC 2011 (aka bombing run, aka weapons, aka xoxoxoxo)
///
///  Author(s): Michael Scherer<br>
///  Created: 2011<br>
///  Copyright (c) 2011<br>
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
#ifndef __ZEBULON_UNDERWATER_DROPPER_H
#define __ZEBULON_UNDERWATER_DROPPER_H

#include "auvc/underwater/mission.h"

#include "dvision/vision.h"

#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class Dropper : public Underwater::Mission
        {
        public:
            static const std::string NextMission;
            
            static const std::string BigX;
            static const std::string SmallX;
            static const std::string BigO;
            static const std::string SmallO;
            
            enum State
            {
                Find,           // Entry state, figure out what to do
                Search,         // Search for the targets
                
                Approach,       // Go near a cluster of targets
                FindActive,     // Find the target we're looking for
                Attack,         // Attack the target!
                
                PathSearch,     // Look for path
                DoPath,         // Do the path stuff
                ExitPath,       // Leave path towards life vests
                
                Exit            // Go onto the next state
            };
            
            Dropper();
            ~Dropper();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
            std::string GetStateName(State state);
            
        private:
            State mState;
            Zebulon::AI::StopWatch mStateTimeout;
            
            Controls mCurrent;                          ///< Current control system state
            Controls mDesired;                          ///< Desired control system state
            
            double mStartYaw;                           ///< Initial yaw of the vehicle
            double mDefaultDepth;                       ///< Default depth of the vehicle
            
            double mLatPixelScale;
            double mDepthPixelScale;
            double mYawPixelScale;
            double mAxialPixelScale;
            
            IplImage* mForwardFrame;                    ///< The forward facing camera's frame
            IplImage* mDownwardFrame;                   ///< The downward facing camera's frame
            IplImage* mFWProcessedImage;                ///< The final frame
            IplImage* mDWProcessedImage;                ///< The final downward frame
            
            Vision::DVision mDVision;
            std::list<Vision::VisualTarget*> mTargets;
            std::list<Vision::VisionReturn> mVisionReturn;
            
            int mFirstTarget;
            int mSecondTarget;
            bool mFirstTargetAttempted;
            
            Zebulon::AI::Debounce mCenteredDebounce;
            Zebulon::AI::Debounce mLostTargetDebounce;
            
            double mCenterIThresh;
            double mCenterJThresh;
            
            PathFinder mPathFinder;
            Zebulon::AI::Debounce mPathDebounce;
            Zebulon::AI::StopWatch mPathTimer;
            double mExitPathThrust;
            double mPathExitTime;
            
            Zebulon::AI::StopWatch mApproachTimer;
            double mApproachTime;
            double mApproachSpeed;
            
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
            
        };
    }
}

#endif // __ZEBULON_UNDERWATER_DROPPER_H
/* End of file */
