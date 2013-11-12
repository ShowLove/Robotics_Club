/////////////////////////////////////////////////////////////////////////////
///
/// \file testtracking.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Michael Scherer<br>
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
#ifndef __ZEBULON_UNDERWATER_TestTracking_H
#define __ZEBULON_UNDERWATER_TestTracking_H

#include "auvc/underwater/mission.h"
#include "gvision/segment.h"
//#include "gvision/threshhold.h"
#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "auvc/underwater/controls.h"

#include "statemachine/utility/strafesearch.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class TestTracking : public Underwater::Mission
        {
        public:
            enum State
            {
                Turn,
                Lateral,
                Search
            };
            
            TestTracking();
            ~TestTracking();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
            IplImage* BackProject(const IplImage * img, const IplImage * sampleHistImg, const int threshold, const int dilate);
            
        private:
            State mState;
            
            Controls mCurrent;
            Controls mDesired;
            
            IplImage* mHistImage;
            
            IplImage* mForwardFrame;
            IplImage* mSegFrame;                        ///< The Segmented frame.
            IplImage* mThreshFrame;                     ///< The thresholded frame.
            IplImage* mProcessedImage;                  ///< The final frame
            
            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            
            Zebulon::AI::Debounce mDebounce;
            Vision::Return mReturn;
            Vision::Target mTarget;
            
            double mYawPixelScale;
            double mLatPixelScale;
            double mDepthPixelScale;
            double mAxialPixelScale;
            
            double mDefaultDepth;
            
            double mTargetArea;
            
            double mMinArea;
            double mMaxArea;
            
            double mLaserDist;
            double mLaserCloseDist;
            
            Zebulon::AI::StrafeSearch mSearchPattern;
            double mSearchStrafeTime;
            double mSearchStrafeSpeed;
            double mSearchForwardTime;
            double mSearchForwardSpeed;
            double mSearchSmoothSpeed;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TestTracking_H
/* End of file */