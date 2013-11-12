/////////////////////////////////////////////////////////////////////////////
///
///  \file wreath.h
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
#ifndef __ZEBULON_UNDERWATER_WREATH_H
#define __ZEBULON_UNDERWATER_WREATH_H

#include "auvc/underwater/mission.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "statemachine/utility/sweepsearch.h"
#include "statemachine/utility/strafesearch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"
#include "statemachine/utility/objectscan.h"
#include <boost/timer.hpp>

#include "auvc/underwater/vision/bouyobject.h"

#include <vector>

#define PINGERYAW (0)
#define BLINDDRIVETIME (90)
// -32

namespace Zebulon
{
    namespace Underwater
    {
        class Wreath : public Underwater::Mission
        {
        public:
            static const std::string NextMission;

            enum State
            {
                BackUp, // back up from current heading and depth (go up?)
                TurnNorth,
                PointAtPinger, // Drive and go to depth

                BlindDrive,
                LookForWreath,

                Surface,

                EndSurface
            };

            Wreath();
            ~Wreath();

            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();

            std::string GetStateName(State state);

        private:
            State mState;

            Zebulon::AI::StopWatch mBackUpTimer;
            Zebulon::AI::StopWatch mSurfaceTimer;
            double mBackUpTime;
            double mBackUpThrust;

            double mBackUpDepth;
            double mPingerDepth;
            double mSurfaceDepth;
            double mDepthThresh;

            double mBackUpYaw;
            double mNorthYaw;
            double mPingerYaw;
            double mPingerDriveTime;

            double mLatPixelScale;
            double mAxialPixelScale;

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

            Zebulon::AI::Debounce mDebounceWreath;

            Zebulon::AI::Debounce mCenteredDebounce;
            double mIThresh;
            double mJThresh;
            double mCenterI;
            double mCenterJ;

            double mYawThresh;

            Zebulon::AI::Debounce mLostTargetDebounce;  ///< If I haven't seen target in a while, you need to search search search


            // Attack
            bool mNorthThrust;
            double mWreathThrust;

            // UnhitLeave
            Zebulon::AI::StopWatch mBlindDriveTimer;
            double mBlindDriveTime;
            double mBlindDriveThrust;
            PathObject mEndobject;
            std::vector<Vision::Return> mObjectReturn;

        };
    }
}

#endif // __ZEBULON_UNDERWATER_WREATH_H
/* End of file */
