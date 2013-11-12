//////////////////////////////////////////////////////////////////////////////////////
///
/// \file cheatershand.h
/// \brief Mission identifies and shoots at a target (in this case a cheater's hand)
///
/// Author(s): Tuyen DO<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:nintwindo@knights.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
//////////////////////////////////////////////////////////////////////////////////////
#ifndef _ZEBULON_SURFACE_CHEATERSHAND_H
#define _ZEBULON_SURFACE_CHEATERSHAND_H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"

#include <statemachine/utility/debounce.h>
#include <statemachine/utility/stopwatch.h>
#include <statemachine/utility/sweepsearch.h>

#include <cvision/opencvsurf.h>
#include <gvision/visionutil.h>
#include <gvision/threshratio.h>
#include <gvision/segment.h>
#include <filter/rcfilter.h>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "dvision/visionutils.h"
#include "dvision/visionobject.h"
#include <opencv2/legacy/legacy.hpp>
#include "surface/vision/bouyobject.h"
namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class CheatersHand
        ///  \brief Cheaters Hand mission of Roboboat 2012.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CheatersHand : public MapBuilder
        {
        public:
            static const std::string Name;       ///<  Name of the mission.
            enum States
            {
                ApproachTarget,
                FindTarget,
                TargetHotSuit,
                TargetCheatersHand,
                TargetButtonPress,
                LeaveShore
            };

            CheatersHand(const bool returnNavFlag = false);
            ~CheatersHand();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        protected:
            std::list<CvBox2D> FindSquares(const IplImage* img, IplImage *dest, const double minsize, const double maxsize, const int cannyThreshold, const bool angle3d);
            IplImage* SegmentationMask(const IplImage * imgIn);
            virtual void UpdateMaps(const bool clearMaps);
            std::vector<Zebulon::Vision::Return> GetBoxes( IplImage* camimage,  IplImage* depthimage,  IplImage* depthmap);

            virtual void LoadSettings();

            // Draws maps and displays them to screen (if display is on)
            virtual void DrawMaps();
            virtual void DrawDesiredVectors();
            CxUtils::Point3D GetCentroid(CxUtils::Point3D::List pts);
            void AimToShore();
            bool LinedUpWithShore(double distanceThreshold = 0.4, double angleThreshold = 20);

        private:
            std::string mNextMission;                  ///<  The next mission to go to on exit.
            AI::StopWatch mTimeoutStopWatch;           ///<  How long until timeout in search.
            double mTimeoutTimeSeconds;                ///<  Timeout time in seconds to give up and exit.
            IplImage* mpMainCamera;                    ///<  Main camera image.
            IplImage* mpDepthData;
            IplImage* mpDepthImage;


            CxUtils::Point3D::List mShoreline;
            double mShorelineAverage;
            double mShorelineAngleDeg;

            Utm mDestinationWaypoint;

            States mCurrentState;

            double mCourseATargetHeading;              ///< Heading in degrees
            double mCourseBTargetHeading;              ///< Heading in degrees

            double mDesiredHeadingDeg;
            double mDesiredThrust;
            double mDesiredLateral;

            int mNumberOfShorelinePts;
            int mMinPts;
            int mMinPtsForShore;
            double mShorelinePerpAngleDeg;
            double mDistanceToShoreline;
            AI::StopWatch mLateralStopWatch;
            AI::StopWatch mLeaveShoreStopWatch;
            double mDesiredLateralDir;
            double mScanLength;
            double mPrevGoodAngleDeg;
            double mSearchHeadingDir;
            double mSearchHeading;

            CxUtils::Point3D mStartingPosition;
            bool mSeesShoreFlag;
            double mCoverageDistance;
            CxUtils::Point3D mTargetPose;
            double mTargetThreshold;
            double mDesiredDistanceToShore;
            int mNumSweeps;

            BouyObject mWhite;

            // Network Messaging
            CxUtils::TcpClient mTcp;

            // Suit values 0 = club, 1 = spade, 2 = diamon, 3 = heart.
            void SendHotSuit(const CxUtils::Wgs& position,
                             int suit = 0);

            //Utility::LowPassFilter mShorelineAngleFilter;
            //Utility::LowPassFilter mShorelineDistanceFilter;
        };
    }
}


#endif //  _ZEBULON_SURFACE_CHEATERSHAND_H
/* End of File */
