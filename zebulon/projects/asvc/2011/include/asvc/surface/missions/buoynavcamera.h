//////////////////////////////////////////////////////////////////////////////////////
///
/// \file buoynavcamera.h
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
#ifndef _ZEBULON_SURFACE_BUOY_NAVIGATION_CAMERA_H
#define _ZEBULON_SURFACE_BUOY_NAVIGATION_CAMERA_H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"
#include <gvision/segment.h>
#include <gvision/threshhold.h>
#include <gvision/visionutil.h>
#include <gvision/threshratio.h>
#include <statemachine/utility/debounce.h>
#include <statemachine/utility/stopwatch.h>
#include <statemachine/utility/sweepsearch.h>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class BuoyNavCamera
        ///  \brief Currently the 3rd and later returning mission for the RoboBoats
        ///   competition.  This mission performans channel navigation between red and
        ///   green buoys, and attempts to avoid yellow buoys (obstacles).  This
        ///   Implementation only uses machine vision for channel navigation, and
        ///   LIDAR only for obstacle avoidance.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class BuoyNavCamera : public MapBuilder
        {
        public:
            static const std::string Name;       ///<  Name of the mission.
            static const std::string ReturnName; ///<  Name of mission on return.
            enum State
            {
                Find,
                Red,
                Green,
                RedGreen,
                Yellow,
                Search,
                LockHeading,
                ExitHeading
            };
            BuoyNavCamera(const bool returnNavFlag = false);
            ~BuoyNavCamera();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        protected:
            std::string ToString(const State state) const
            {
                std::string str;
                switch(state)
                {
                case Find:
                    str = "Find Buoys";
                    break;
                case Red:
                    str = "Red Buoy Only";
                    break;
                case Green:
                    str = "Green Buoy Only";
                    break;
                case RedGreen:
                    str = "Red and Green Buoys";
                    break;
                case Yellow:
                    str = "Yellow Buoy";
                    break;
                case Search:
                    str = "Searching";
                    break;
                case ExitHeading:
                    str = "Exit Heading";
                    break;
                default:
                    str = "Lock Heading";
                    break;
                }
                return str;
            }

            virtual void UpdateMaps(const bool clearMaps);
            virtual void RunVisionClassifier();
            virtual void LoadSettings();
            // Draws maps and displays them to screen (if display is on)
            virtual void DrawMaps();
        private:
            bool mReturnFlag;                          ///<  If true, we are returning through the channel back to start of mission (false by default).
            std::string mNextMission;                  ///<  The next mission to go to on exit.
            State mCurrentState;                       ///<  Current sub-state
            State mPrevState;                          ///<  Previous state.
            double mTravelSpeed;                       ///<  Default travel speed.
            double mDesiredHeading;
            int mBuoyCount;                            ///<  Number of buoys traversed.
            int mMaxBuoyCount;                         ///<  Maximum buoy count.
            bool mSearchFlag;                          ///<  Search for buoys.
            bool mFirstBuoyFoundFlag;                  ///<  Flag signaling that buoys have been located (start of channel).
            double mNewStateDuration;                  ///<  If we haven't seen buoys in this time, leave mission.
            AI::StopWatch mNewStateTimer;              ///<  Stop watch for new state.
            double mGlobalExitDuration;                ///<  Maximum time to do buoy nav.
            double mGlobalMinDuration;                 ///<  Minimum time (seconds) to do buoy nav.
            AI::StopWatch mGlobalExitTime;             ///<  Global exit timer for mission.

            double mBeamLengthMeters;                  ///<  Beam length in meters for collision detection.
            // Lock heading settings.
            double mLockHeadingDuration;               ///<  How long to lock heading when traveling through buoys.
            AI::StopWatch mLockHeadingTimer;           ///<  Timer to keep track of heading lock.

            // Search settings.
            double mSearchSweepSpanDegrees;            ///<  Span angle for sweep (degrees).
            double mSearchSweepSpeed;                  ///<  How fast to rotate in degrees/s.
            double mSearchForwardTime;                 ///<  Time to move forward while searching.
            double mSearchForwardSpeed;                ///<  Search forward speed (percent).
            AI::SweepSearch mSweepSearch;              ///<  Sweep search utility.
            std::map<unsigned int, int> mSweepDir;     ///<  Sweep direction by zone.
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
            AI::Debounce mGreenDebounce;                ///< Debound for detection of green buoy.
            AI::Debounce mYellowDebounce;               ///< Debound for detection of yellow buoy.
            AI::Debounce mBlueDebounce;                 ///< Debound for detection of buoy buoy.
            double mBlueDebounceLimit;                  ///< Debounce limit for blue buoy.
            double mBlueExitMinSize;                    ///< Minimum size for blue buoy for exit.

            double mMaxPixelArea;                       ///< Maximum pixel size for segments found.
            double mMinPixelArea;                       ///< Minimum pixel size for segments found.
            double mMinRedPixelArea;                    ///< Minimum pixel size for red segments.
            double mMinGreenPixelArea;                  ///< Minimum pixel size for green segements.
            double mMinYellowPixelArea;                 ///< Minimum pixel size for yellow segements.
            double mMinBluePixelArea;                   ///< Minimum pixel area for blue segments.
            unsigned int mVisionDebounceLimit;          ///< Debounce limit for visual detection.

            double mCenterI;                            ///< Point in the middle of the bouys (in frame).
            double mCenterJ;                            ///< Point in the middle of the bouys (in frame).

            double mLeftI;                              ///< Left pixel value for channel nav.
            double mRightI;                             ///< Right pixel value for channel nav.
            double mMaxJ;                               ///< Max value of J before we consider we have passed the gates.

            double mPixelScale;                         ///< Scale for how pixels convert to turn degrees.
            double mPixelScaleNum;                      ///< Scale for how pixels convert to turn degrees (numerator).
            double mPixelScaleDenom;                    ///< Scale for how pixels convert to turn degrees (denomenator).

            double mYellowBuoyCenter;                   ///< Center buoy values.
            double mYellowOffset;                       ///< Amount to offset due to yellow buoy in radians.

            double mExitHeading;                        ///< Heading to go to on exit.
        };
    }
}


#endif //  ZEBULON_SURFACE_BUOY_NAV
/* End of File */
