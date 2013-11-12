//////////////////////////////////////////////////////////////////////////////////////
///
/// \file buoynav.h
/// \brief Mission 3, goes through buoy channel.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:dbarber@ist.ucf.edu<br>
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
#ifndef _ZEBULON_SURFACE_BUOY_NAVIGATION_H
#define _ZEBULON_SURFACE_BUOY_NAVIGATION_H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"
#include "asvc/surface/buoy.h"
#include <gvision/segment.h>
#include <gvision/threshhold.h>
#include <gvision/visionutil.h>
#include <gvision/threshratio.h>
#include <homography/homography.h>
#include <statemachine/utility/debounce.h>
#include <statemachine/utility/stopwatch.h>
#include <statemachine/utility/sweepsearch.h>

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
        class BuoyNav : public MapBuilder
        {
        public:
            static const std::string Name;       ///<  Name of the mission.
            static const std::string ReturnName; ///<  Name of mission on return.
            enum States
            {
                InitState= 0,
                SearchBuoys,        // Looking for buoys
                FoundBuoys,         // Found buoys, traveling towards
                DriveThroughBuoys   // Driving through buoys
            };
            enum BuoyType
            {
                Left = 0,
                Right,
                Bad
            };
            BuoyNav(const bool returnNavFlag = false);
            ~BuoyNav();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        protected:
            virtual void UpdateMaps(const bool clearMaps);
            virtual void RunVisionClassifier();
            virtual void FindClosestBuoys();
            virtual void MatchBuoy(const Vision::Return& buoy,
                                   const Buoy::Color color,
                                   Buoy::List& buoysFound);
            virtual void LoadSettings();
            virtual void SetBuoyDrawColors();
            // Draws maps and displays them to screen (if display is on)
            virtual void DrawMaps();

            inline BuoyType GetTypeFromColor(const Buoy::Color color)
            {
                BuoyType type = Bad;
                if(mReturnFlag)
                {
                    switch(color)
                    {
                    case Buoy::Red:
                        type = Right;
                        break;
                    case Buoy::Green:
                        type = Left;
                        break;
                    default:
                        type = Bad;
                        break;
                    }
                }
                else
                {
                    switch(color)
                    {
                    case Buoy::Red:
                        type = Left;
                        break;
                    case Buoy::Green:
                        type = Right;
                        break;
                    default:
                        type = Bad;
                        break;
                    }
                }
                return type;
            }
        private:
            bool mReturnFlag;                          ///< If true, we are returning through the channel back to start of mission (false by default).
            unsigned int mDebounceLimit;               ///<  Debounce limit.
            AI::Debounce mDebounce;                    ///< Debounce interface to check if a button has been pressed.
            std::string mNextMission;                  ///< The next mission to go to on exit.
            States mCurrentState;                      ///<  Current sub-state
            States mPrevState;                         ///<  Previous state.

            // Object scanning from lasers.
            double mSearchAngle;                       ///<  Search angle in radians for finding buoys.
            double mSearchDepth;                       ///<  Search depth for finding buoys.

            // Driving and timeout data.
            double mDriveTimeSeconds;                  ///<  How long to spend driving between buoys to next set (seconds).
            double mTravelSpeed;                       ///<  Travel speed (percent of max).

            double mTimeoutTimeSeconds;                ///<  Timeout time in seconds to give up and exit.
            double mDriveThroughDistanceThresh;        ///<  When this distance is reached to gate, we drive through.
            AI::StopWatch mDrivingStopWatch;           ///<  How long to wait in time for driving through.
            AI::StopWatch mTimeoutStopWatch;           ///<  How long until timeout in search.

            // Search settings.
            double mSearchSweepSpanDegrees;            ///<  Span angle for sweep (degrees).
            double mSearchSweepSpeed;                  ///<  How fast to rotate in degrees/s.
            double mSearchForwardTime;                 ///<  Time to move forward while searching.
            double mSearchForwardSpeed;                ///<  Search forward speed (percent).
            AI::SweepSearch mSweepSearch;              ///<  Sweep search utility.

            // Image Processing data
            IplImage* mInitFrame;                       ///< Image from main camera.
            IplImage* mSegFrame;                        ///< The Segmented forward frame.
            IplImage* mThreshFrame;                     ///< The thresholded forward frame.
            IplImage* mFinalFrame;                      ///< The fully processed forward frame.

            Vision::Segment *mSegment;                  ///< Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///< Thresholding for segmented frame.
            Vision::Return mGreen;                      ///< Return values for green image.
            Vision::Return mRed;                        ///< Return values for red image.
            Vision::Return mYellow;                     ///< Return values for yellow image.
            Vision::Return mBlue;                       ///< Return values for blue image.

            Vision::Target mGreenBuoy;                  ///< Green buoy targets data.
            Vision::Target mRedBuoy;                    ///< Red buoy targets data.
            Vision::Target mYellowBuoy;                 ///< Yelow buoy targets data.
            Vision::Target mBlueBuoy;                   ///< Blue buoy targets data.

            AI::Debounce mRedDebounce;                  ///< Debound for detection of red buoy.
            AI::Debounce mGreenDebounce;                ///< Debound for detection of red buoy.
            AI::Debounce mYellowDebounce;               ///< Debound for detection of red buoy.
            AI::Debounce mBlueDebounce;                 ///< Debound for detection of red buoy.

            double mMaxPixelArea;                       ///< Maximum pixel size for segments found.
            double mMinPixelArea;                       ///< Minimum pixel size for segments found.
            unsigned int mVisionDebounceLimit;          ///< Debounce limit for visual detection.
            Vision::Homography mHomography;             ///< Transforms data to the ground plane.
            double mBuoyMatchDistance;                  ///< Distance threshold for matching vision objects to LIDAR.

            // Final Buoy Data
            unsigned int mBuoyCount;                   ///<  Number of buoys we've travelled through
            unsigned int mMaxBuoys;                    ///<  Maximum number of buoys in channel.
            bool mHaveBuoyVectorFlag;                  ///<  Have vector through buoys.
            CxUtils::Point3D mBuoyMidpoint;            ///<  Buoy midpoint.
            std::vector<bool> mBuoyFlags;              ///<  Vector of flags for found buoys.
            std::vector<Buoy> mBuoys;                  ///<  Vector of buoy states.
            Buoy::List mBuoysFound;                    ///<  Vector of all "buoys" found.

            double mExitHeading;                       ///< Heading to go to on exit.
        };
    }
}


#endif //  ZEBULON_SURFACE_BUOY_NAV
/* End of File */
