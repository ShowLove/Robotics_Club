/////////////////////////////////////////////////////////////////////////////
///
///  \file buoysdavid.h
///  \brief Buoy mission for the AUVC 2011
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
#ifndef __ZEBULON_UNDERWATER_BUOYSCLEAN_H
#define __ZEBULON_UNDERWATER_BUOYSCLEAN_H

#include "auvc/underwater/mission.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/sweepsearch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"
#include "statemachine/utility/objectscan.h"

#include "auvc/underwater/vision/bouyobject.h"

#include <vector>

namespace Zebulon
{
    namespace Underwater
    { 
        class BuoysClean : public Underwater::Mission
        {
        public:
            static const std::string NextMission;
            
            enum State
            {
                Search,
                
                Attack,
                Unhit,
                LockAttack,
                Leave,
                
                PathSearch,     // Look for path or buoys
                DoPath,         // Do the path stuff
                ExitPath,       // Leave path towards life vests
                
                Exit            // Go onto the next state
            };
            
            enum Color
            {
                Red     = 0,
                Green   = 1,
                Yellow  = 2
            };
            
            BuoysClean();
            ~BuoysClean();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
            std::string GetStateName(State state);
            
        private:
            State mState;
            Zebulon::AI::StopWatch mStateTimeout;
            
            double mDefaultDepth;
            double mYawFix;
            
            Controls mCurrent;
            Controls mDesired;
            
            IplImage* mForwardFrame;
            IplImage* mDownwardFrame;
            IplImage* mFWProcessedImage;                  ///< The final frame
            IplImage* mDWProcessedImage;                ///< The final downward frame
            
            BouyObject mBuoyObject[3];
            Zebulon::AI::Debounce mDebounce[3];
            Vision::Return mReturn[3];
            
            Zebulon::AI::Debounce mCenteredDebounce;
            double mIThresh;
            double mJThresh;
            double mCenterI;
            double mCenterJ;
            
            double mArea;
            double mAreaCloseThresh;
            double mTargetArea;
            
            double mLatPixelScale;
            double mDepthPixelScale;
            double mAxialPixelScale;
            double mYawPixelScale;
            
            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search
            
            int mAttackingBuoy;
            int mBuoyToAttack[2];
            int mBuoyOrder[3];
            
            // Lock attack
            Zebulon::AI::StopWatch mLockAttackTimer;
            double mLockAttackDuration;
            double mLockAttackThrust;
            double mLockAttackYaw;
            double mLockAttackDepth;
            
            // Search
            Zebulon::AI::SweepSearch mSearchPattern;
            double mSearchSweepDegrees;
            double mSearchSweepSpeed;
            double mSearchForwardTime;
            double mSearchForwardSpeed;
            double mSearchSmoothSpeed;
            int mSearchInitDirection;
            
            // Path Search
            Zebulon::AI::StrafeSearch mPathSearchPattern;
            double mPathSearchStrafeTime;
            double mPathSearchStrafeSpeed;
            double mPathSearchForwardTime;
            double mPathSearchForwardSpeed;
            double mPathSearchSmoothSpeed;
            int mPathSearchInitDirection;
            
            // Path
            PathFinder mPathFinder;
            Zebulon::AI::Debounce mPathDebounce;
            Zebulon::AI::StopWatch mPathTimer;
            double mExitPathThrust;
            double mPathExitTime;
            
            // Leave
            double mLeaveDepth;
            double mLeaveDuration;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_BUOYSCLEAN_H
/* End of file */
