/////////////////////////////////////////////////////////////////////////////
///
///  \file training.h
///  \brief Buoy mission for the AUVC 2012
///
///  Author(s): Cassondra Puklavage<br>
///  Created: 2012<br>
///  Copyright (c) 2012<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: cassondrap@knights.ucf.edu<br>
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
#ifndef __ZEBULON_UNDERWATER_TRAINING_H
#define __ZEBULON_UNDERWATER_TRAINING_H

#include "auvc/underwater/mission.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/sweepsearch.h"
#include "statemachine/utility/strafesearch.h"
#include "statemachine/utility/sinesearch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"
#include "statemachine/utility/objectscan.h"
#include <boost/timer.hpp>

#include "auvc/underwater/vision/bouyobject.h"

#include <vector>

namespace Zebulon
{
    namespace Underwater
    {
        class Training : public Underwater::Mission
        {
        public:
            static const std::string NextMission;

            enum State
            {
                GoToPathDepth,  // Go to pathdepth, leave
                PathSearch,     // Look for path or buoys
                DoPath,         // Do the path stuff
                ExitPath,       // Leave path towards life vests

                GoToBuoyDepth,
                BuoySearch,

                InitAttack,
                Attack,
                LockAttack,

                Unhit,

                UnhitLeave,
                GoToLeaveDepth,
                Leave,

                LeaveMission            // Go onto the next state
            };

            enum Color
            {
                Red     = 0,
                Green   = 1,
                Yellow  = 2
            };

            Training();
            ~Training();

            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();

            std::string GetStateName(State state);

        private:
            double AngleToBuoy(const Zebulon::Vision::Return& buoy);
            State mState;

            //bool GetPaths(std::vector<Path> &paths);
            void InitSearch();
            void SearchStep();

            bool mLeaveMission;

            Zebulon::AI::StopWatch mInitAttackTimer;
            double mInitAttackTime;
            double mInitAxialThrust;

            Zebulon::AI::StopWatch mStateTimeout;

            double mBuoyDepth;
            double mPathDepth;
            double mLeaveDepth;
            double mDepthThresh;

            double mFixedYaw;
            double mStartYaw;
            double mPathYaw;
            bool mPathFoundFlag;

            //Controls mCurrent;
            //Controls mDesired;
            // In Every Mission
            double mCurrentDepth;
            double mCurrentYaw;
            double mCurrentPitch;
            double mCurrentRoll;

            double mDesiredAxialThrust;
            double mDesiredLateralThrust;
            double mDesiredDepth;
            double mDesiredYaw;
            double mDesiredPitch;
            double mDesiredRoll;

            IplImage* mFWFrame;
            IplImage* mDWFrame;
            IplImage* mFWProcFrame;                  ///< The final frame
            IplImage* mDWProcFrame;                ///< The final downward frame
            bool mVTGary;
            bool mVTDavid;
            bool mVTNew;
            bool mInitCameras;


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

            double mYawThresh;

            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search

            int mAttackingBuoy;
            int mBuoyToAttack[2];
            int mBuoyOrder[3];

            // Attack
            double mAttackThrust;
            bool mYawAttack;
            bool mLateralAttack;
            double mAttackLaserDist;

            // Lock attack
            Zebulon::AI::StopWatch mLockAttackTimer;
            double mLockAttackTime;
            double mLockAttackThrust;
            double mLockAttackYaw;
            double mLockAttackDepth;

            // Unhit
            Zebulon::AI::StopWatch mUnhitTimer;
            double mUnhitTime;
            double mUnhitThrust;

            // UnhitLeave
            Zebulon::AI::StopWatch mUnhitLeaveTimer;
            double mUnhitLeaveTime;
            double mUnhitLeaveThrust;

            // GoToLeaveDepth
            Zebulon::AI::StopWatch mLeaveTimer;
            double mLeaveTime;
            double mLeaveThrust;

            // Search
            bool mBuoySearchStrafe;
            bool mBuoySearchSweep;

            Zebulon::AI::SweepSearch mBuoySweepSearch;
            double mBuoySearchSweepDegrees;
            double mBuoySearchSweepSpeed;
            double mBuoySweepForwardTime;
            double mBuoySweepForwardSpeed;
            double mBuoySweepSmoothSpeed;
            int mBuoySweepInitDirection;

            Zebulon::AI::SineSearch mBuoyStrafeSearch;
            double mBuoySearchStrafeTime;
            double mBuoySearchStrafeSpeed;
            double mBuoyStrafeForwardTime;
            double mBuoyStrafeForwardSpeed;
            double mBuoyStrafeSmoothSpeed;
            int mBuoyStrafeInitDirection;

            // Path Search
            Zebulon::AI::SineSearch mPathSearchPattern;
            double mPathSearchStrafeTime;
            double mPathSearchStrafeSpeed;
            double mPathSearchForwardTime;
            double mPathSearchForwardSpeed;
            double mPathSearchSmoothSpeed;
            int mPathSearchInitDirection;

            // Path
            //PathFinder mPathFinder;
            //Zebulon::AI::Debounce mPathDebounce;
            Zebulon::AI::Debounce mPathCenteredDebounce;
            Zebulon::AI::StopWatch mPathTimer;
            double mExitPathThrust;
            double mPathExitTime;
            Zebulon::AI::StopWatch mPathSearchTimer;


            // Leave
            //double mLeaveDepth;
            //double mLeaveDuration;

            double mLaserDist;
            double mLaserCloseDist;

            AI::ObjectScan mObjectScanData;
            CxUtils::Point3D::List mRawLaserData;
            CxUtils::Point3D::List mLaserObjects;
            bool mLaserObjectValid;
            double mMinLaserObjectRadius;
            double mMaxLaserObjectRadius;
            double mCurrLaserDist;
            double mMaxDistance;
            double mMinDistance;
            double mAngleRange;
            double mStartAngle;
            double mEndAngle;
            bool mIsLaserAvailable;
            // Laser display
            //Zebulon::Utility::LaserDisplay mLaserDisplay;
            IplImage* mDisplayLaser;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TRAINING_H
/* End of file */
