/////////////////////////////////////////////////////////////////////////////
///
/// \file path.h
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

#ifndef __ZEBULON_UNDERWATER_PATH_H
#define __ZEBULON_UNDERWATER_PATH_H

#include <opencv/cv.h>
#include <vector>

#include <dvision/vision.h>

#include <auvc/underwater/mission.h>
#include <auvc/underwater/controls.h>

#include "gvision/visionutil.h"
#include "gvision/threshratio.h"
#include "gvision/segment.h"
#include "gvision/return.h"
#include "gvision/target.h"
#include "auvc/underwater/vision/pathobject.h"

#include <cxutils/mutex.h>

namespace Zebulon
{
    namespace Underwater
    { 
        struct Path
        {
        public:
            Path() {};
            Path(const Path& path)
            {
                *this = path;
            }
            Path& operator=(const Path& path)
            {
                if (this != &path)
                {
                    mI = path.mI;
                    mJ = path.mJ;
                    
                    mAngle = path.mAngle;
                    
                    mArea = path.mArea;
                    mRatio = path.mRatio;
                    mConfidence = path.mConfidence;
                }
                return *this;
            }
            ~Path() {};
            
            double mI;
            double mJ;
            
            double mAngle;          // should be with respect to the frame of the robot
            
            double mArea;
            double mRatio;
            double mConfidence;
        };
        
        class PathFinder
        {
        public:
            static double ImageWidth;
            static double ImageHeight;
            
            PathFinder();
            ~PathFinder();
            
            void Initialize();
            
            std::vector<Path> GetPaths(const Controls current, IplImage* image, IplImage * output=NULL);
            std::vector<Path> GetPathsBox(IplImage* image, IplImage * output=NULL);
            std::vector<Path> GetPathsDavid(const Controls current, IplImage* image, IplImage * output=NULL);
            
            bool StepPath(const Path* path, const Controls input, Controls &output);
        
        private:
            CxUtils::Mutex mMutex;
            
            IplImage* mSegFrame;                        ///< The Segmented frame.
            IplImage* mThreshFrame;                     ///< The thresholded frame.
            IplImage* mProcessedImage;                  ///< The final frame
            IplImage* mProcessedImageB;                  ///< The final frame (back)
            
            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            //Vision::Threshhold *mThreshold;            ///< Thresholding for segmented frame.
            
            Vision::Return mOrangeReturn;
            Vision::Target mOrangePath;
            
            double mTargetRatio;
            double mTargetRatioSigma;
            
            double mTargetMinArea;
            double mTargetMaxArea;
            
            double mMaxPaths;
            
            double mDefaultDepth;
            
            double mBoxMinSize;
            double mBoxMaxSize;
            int mBoxCannyThresh;
            
            double mCenterIThresh;
            double mCenterJThresh;
            
            double mAngleCloseThresh;
            double mPathAreaDeviationThresh;
            
            double mLatPixelScale;
            double mAxialPixelScale;
            
            double mFrontPathOffsetJ;
            
            PathObject mPathObject;
        };
        
        
    }
}

#endif // __ZEBULON_UNDERWATER_PATH_H