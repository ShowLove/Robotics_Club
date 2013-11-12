/////////////////////////////////////////////////////////////////////////////
///
/// \file VisionValidationGate2.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_UNDERWATER_VisionValidationGate2_H
#define __ZEBULON_UNDERWATER_VisionValidationGate2_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class VisionValidationGate2 : public Underwater::Mission
        {
        public:
            enum State
            {
                GoToDepth,
                InitialTravel,
                LineUp,
                ThroughGate,
                NextMission
            };
            
            VisionValidationGate2();
            ~VisionValidationGate2();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            State mState;
            
            Controls mCurrent;
            Controls mDesired;
            
            IplImage* mForwardFrame;
            IplImage* mFWProcessedImage;
            
            IplImage* mDownwardFrame;
            IplImage* mDWProcessedImage;
            
            Zebulon::AI::Debounce mLeftLegDebounce;
            Zebulon::AI::Debounce mRightLegDebounce;
            Zebulon::AI::Debounce mGateDebounce;
            
            Zebulon::AI::Debounce mCenterGateDebounce;
            
            IplImage* mGateSegImage;
            IplImage* mGateThreshFrame;
            Vision::Target mTarget;
            Vision::Return mReturn[4];
            Vision::Segment *mSegment;
            Vision::ThreshRatio *mThreshold;
            double mLeftGateI;
            double mLeftGateJ;
            double mRightGateI;
            double mRightGateJ;
            double mGateAngleThresh;
            double mGateCenterThresh;
            double mGateValid[2];// = mReturn[i].mValid;
            double mGateI[2]; 
            double mCurrentGate;
            double mLineUpSpeed;
            double mLatPixelScale;
            //double mGate
            
            bool mLeftGateFlag;
            bool mRightGateFlag;
            
            Zebulon::AI::Debounce mPathDebounce;
            PathFinder mPathFinder;
            
            double mDepthThresh;
            double mYawThresh;
            
            double mCurrentDepth;
            double mCurrentYaw;
            
            double mDesiredThrust;
            double mDesiredDepth;
            double mDesiredYaw;
            double mDesiredPitch;
            double mDesiredRoll;
            
            double mStartDepth;
            double mStartYaw;
            
            double mGateSpeed;
            
            Zebulon::AI::StopWatch mStopWatchInitTravel;
            Zebulon::AI::StopWatch mStopWatchLineUp;
            Zebulon::AI::StopWatch mStopWatchThroughGate;
            double mMinDuration;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_Vision2ValidationGate_H
/* End of file */
