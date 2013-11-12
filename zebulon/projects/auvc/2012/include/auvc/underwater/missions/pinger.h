/////////////////////////////////////////////////////////////////////////////
///
///  \file pinger.h
///  \brief Pinger mission for AUVC 2011
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
#ifndef __ZEBULON_UNDERWATER_PINGER_H
#define __ZEBULON_UNDERWATER_PINGER_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
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
        class Pinger : public Underwater::Mission
        {
        public:
            static const std::string NextMission;
            
            enum State
            {
                Listen,
                GoToPinger,
                AlignToPayload,
                GrabPayload,
                Surface,
                Exit            ///< Exit from the mission
            };
            
            Pinger();
            ~Pinger();
            
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
            
            double mPingerYaw;
            double mPingerLockYaw;
            double mPreviousPingerYaw;
            double mPingerDistance;
            double mPingerUpdateTime;
            double mPingerUpdateTimeThresh;
            double mPingerPreviousUpdateTime;
            int mPingCount;
            
            double mPingerGain;
            double mPingerGainThresh;
            Zebulon::AI::Debounce mPingerGainDebounce;
            
            double mLockTime;
            Zebulon::AI::StopWatch mLockTimer;
            
            double mIgnoreDuration;
            Zebulon::AI::StopWatch mIgnoreTimer;
            
            double mMinPingerDist;
            double mMaxPingerDist;
            int mDestPinger;
            
            double mTravelSpeed;
            
            IplImage* mDownwardFrame;
            IplImage* mProcessedImage;                  ///< The final frame
            
            Zebulon::AI::Debounce mCenteredDebounce;
            Zebulon::AI::Debounce mPingerDebounce;
            double mGrabDepth;
            
            PathFinder mPathFinder;
            
            Zebulon::AI::StopWatch mFancyTimer;
            int mFancySwitch;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_PINGER_H
/* End of file */
