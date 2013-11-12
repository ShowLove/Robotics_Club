/////////////////////////////////////////////////////////////////////////////
///
/// \file testlasertracking.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
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
#ifndef __ZEBULON_UNDERWATER_TestLaserTracking_H
#define __ZEBULON_UNDERWATER_TestLaserTracking_H

#include "auvc/underwater/mission.h"
//#include "gvision/segment.h"
//#include "gvision/threshhold.h"
//#include "gvision/visionutil.h"
//#include "gvision/threshratio.h"
#include "statemachine/utility/objectscan.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "auvc/underwater/controls.h"

#include "statemachine/utility/strafesearch.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class TestLaserTracking : public Underwater::Mission
        {
        public:
            enum State
            {
                Turn,
                Lateral,
                Search
            };
            
            TestLaserTracking();
            ~TestLaserTracking();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
            //IplImage* BackProject(const IplImage * img, const IplImage * sampleHistImg, const int threshold, const int dilate);
            
        private:
            State mState;
            
            Controls mCurrent;
            Controls mDesired;
            
            AI::ObjectScan mObjectScanData;
            CxUtils::Point3D::List mRawLaserData;
            CxUtils::Point3D::List mLaserObjects;
            
            IplImage* mDisplayLaser;
            
            Zebulon::AI::Debounce mDebounce;
            
            double mYawDistScale;
            double mLatDistScale;
            double mDepthDistScale;
            double mAxialDistScale;
            
            double mDefaultDepth;
            
            double mTargetArea;
            
            double mMinArea;
            double mMaxArea;
            
            //double mLaserDist;
            //double mLaserCloseDist;
            
            Zebulon::AI::StrafeSearch mSearchPattern;
            double mSearchStrafeTime;
            double mSearchStrafeSpeed;
            double mSearchForwardTime;
            double mSearchForwardSpeed;
            double mSearchSmoothSpeed;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_TestLaserTracking_H
/* End of file */