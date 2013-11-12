//////////////////////////////////////////////////////////////////////////////////////
///
/// \file extinguish.h
/// \brief Mission identifies and shoots at a target (in this case a boat)
///
/// Author(s): Michael Scherer<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:michael.scherer@knights.ucf.edu<br>
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
#ifndef _ZEBULON_SURFACE_EXTINGUISH_H
#define _ZEBULON_SURFACE_EXTINGUISH_H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"

#include <statemachine/utility/debounce.h>
#include <statemachine/utility/stopwatch.h>
#include <statemachine/utility/sweepsearch.h>

#include <cvision/opencvsurf.h>
#include <gvision/visionutil.h>
#include <gvision/threshratio.h>
#include <gvision/segment.h>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class BuoyNav
        ///  \brief Currently the 3rd and later returning mission for the RoboBoats
        ///   competition.  This mission performans channel navigation between red and
        ///   green buoys, and attempts to avoid yellow buoys (obstacles).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Extinguish : public MapBuilder
        {
        public:
            static const std::string Name;       ///<  Name of the mission.
            enum States
            {
                Search,
                ApproachBoat,
                SquirtBoat,
                Leave
            };

            Extinguish(const bool returnNavFlag = false);
            ~Extinguish();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        protected:
            virtual void UpdateMaps(const bool clearMaps);
            virtual void RunVisionClassifier();

            virtual void LoadSettings();

            // Draws maps and displays them to screen (if display is on)
            virtual void DrawMaps();

        private:
            unsigned int mDebounceLimit;               ///<  Debounce limit.
            AI::Debounce mDebounce;                    ///< Debounce interface to check if a button has been pressed.

            unsigned int mHeadingDebounceLimit;        ///<  Debounce limit.
            AI::Debounce mHeadingDebounce;             ///< Check if we've locked into the right heading
            std::string mNextMission;                  ///< The next mission to go to on exit.
            States mCurrentState;                      ///<  Current sub-state

            // Kinematics
            CxUtils::Point3D mOrientation;
            double mTravelSpeedP;


            // Object scanning from lasers.
            double mSearchAngle;                       ///<  Search angle in radians for finding buoys.
            double mSearchDepth;                       ///<  Search depth for finding buoys.

            // Driving and timeout data.
            double mDriveTimeSeconds;                  ///<  How long to spend driving between buoys to next set (seconds).
            double mTravelSpeed;                       ///<  Travel speed (percent of max).

            double mTimeoutTimeSeconds;                ///<  Timeout time in seconds to give up and exit.
            double mDriveThroughDistanceThresh;        ///<  When this distance is reached to gate, we drive through.
            double mSquirtTime;
            AI::StopWatch mDrivingStopWatch;           ///<  How long to wait in time for driving through.
            AI::StopWatch mTimeoutStopWatch;           ///<  How long until timeout in search.
            AI::StopWatch mSquirtStopWatch;

            // Search settings.
            double mSearchSweepSpanDegrees;            ///<  Span angle for sweep (degrees).
            double mSearchSweepSpeed;                  ///<  How fast to rotate in degrees/s.
            double mSearchForwardTime;                 ///<  Time to move forward while searching.
            double mSearchForwardSpeed;                ///<  Search forward speed (percent).
            AI::SweepSearch mSweepSearch;              ///<  Sweep search utility.
            bool mSearchFlag;

            // Image Processing data
            IplImage* mInitFrame;                       ///< Image from main camera.
            IplImage* mFinalFrame;                      ///< The fully processed forward frame.
            IplImage* mGrayFrame;                       ///< Grayscale version of the frame, used for SURF
            IplImage* mSegFrame;
            IplImage* mThreshFrame;

            // Image classification
            // SURF Classifier
            Vision::OpenCvSurf mSURF;                   ///< SURF implementation to classify the boat
            int mSURFMinPairs;                          ///< Configurable value allows SURF to be more strict with greater number
            double mSURFParam1;                         ///< ??? == 50
            double mSURFParam2;                         ///< ??? = 5
            IplImage* mSURFTemplateImage;               ///< Template image of features to be found

            // GVision Classifier
            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            Vision::Return mBlueReturn;                 ///< Return values for blue boat image.
            Vision::Target mBlueTarget;                 ///< Boat target
            double mBlueMinArea;
            double mBlueComboDist;

            double mBoatI;
            double mBoatJ;
            double mHeadingThreshold;                   ///< Threshold to be within to lock heading for squirting
            double mShoreDist;
            double mShoreDistThreshold;                 ///< 1/2 Threshold to be within from the shore before firing

            double mTurnWeight;
            double mLateralWeight;
        };
    }
}


#endif //  _ZEBULON_SURFACE_EXTINGUISH_H
/* End of File */
