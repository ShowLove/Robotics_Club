/////////////////////////////////////////////////////////////////////////////////////
///
/// \file startgatecamera.h
/// \brief Mission code to navigate through start gates.
///
/// <br>Author(s): Gary Stein, Jonathan Mohlenhoff, Daniel Barber
/// <br>Created: 05/31/2011<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: jmohlenh@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
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
#ifndef ZEBULON_ASVC_START_GATE_CAMERA__H
#define ZEBULON_ASVC_START_GATE_CAMERA__H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"
#include <gvision/segment.h>
#include <gvision/threshhold.h>
#include <gvision/threshratio.h>
#include <gvision/visionutil.h>
#include <statemachine/utility/stopwatch.h>
#include <statemachine/utility/debounce.h>
#include <statemachine/utility/sweepsearch.h>
#include <statemachine/utility/utility.h>

#include <asvc/surface/vision/bouyobject.h>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class StartGateCamera
        ///
        ///   \brief Mission to navigate between 2 "speed gates" at the start
        ///   of the event.  It generates multiple waypoints based off of the
        ///   starting heading to take into account drift from wind/current so
        ///   that the boat goes strait.  Uses vision to keep from running into
        ///   the gates.
        ///
        ///////////////////////////////////////////////////////////////////////////////////
        class StartGateCamera : public MapBuilder
        {
        public:
            const static std::string Name;          ///<  Name of the mission.
            const static std::string ReturnName;    ///<  Name of mission on return.
            enum State
            {
                PreStart,
                PreDrive,
                Start,
                DriveToWaypoint,
                FoundRed,
                FoundGreen,
                FoundBoth,
                CheckGate,
                FoundGreenRed //reversed gates
            };
            StartGateCamera(const bool returnNavFlag = false);
            ~StartGateCamera();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
        protected:
            double AngleToGate(const Zebulon::Vision::Return& buoy);
            std::string ToString(const State state)
            {
                std::string str;
                switch(state)
                {
                case PreStart:
                    str = "Pre-Start";
                    break;
                case PreDrive:
                    str = "Pre-Drive";
                    break;
                case Start:
                    str = "Start";
                    break;
                case DriveToWaypoint:
                    str = "Drive to Waypoint";
                    break;
                case FoundRed:
                    str = "Found Red Gate";
                    break;
                case FoundGreen:
                    str = "Found Green Gate";
                    break;
                case FoundGreenRed:
                    str = "Found Reversed Gates";
                    break;
                case FoundBoth:
                    str = "Found Both Gates";
                    break;
                case CheckGate:
                    str = "Check Gate (Drive Through)";
                    break;
                default:
                    str = "Undefined";
                    break;
                }
                return str;
            }

            virtual void RunVisionClassifier();
            virtual void RunDvisionClassifier();
            virtual void LoadSettings();
            virtual void DrawMaps();
        private:
            std::string mNextMission;                   ///<  Next mission in the list.
            bool mReturnFlag;                           ///<  Return flag (true if returning to dock).
            double mPreDriveTimeSeconds;                ///<  Pre-drive time to get away from dock.
            double mStartHeading;                       ///<  Heading when the mission starts.
            CxUtils::Utm mStartPosition;                ///<  Starting position.
            CxUtils::Utm::List mWaypoints;              ///<  Projected waypoints to drive to.
            int mWaypointNumber;                        ///<  Current waypoint number.
            double mCurrentHeading;                     ///<  Current Heading degrees.
            CxUtils::Utm mCurrentPosition;              ///<  Current position.
            double mForwardThrust;
            int mNumWaypoints;                          ///<  Number of waypoints to generate.
            double mDesiredHeading;                     ///<  The heading we want to go to.
            Vision::Target mGreenBuoy;                  ///<  Green buoy color values.
            Vision::Target mRedBuoy;                    ///<  Red buoy color values.
            double mCenterI;                            ///<  point in the middle of the bouys.
            double mCenterJ;                            ///<  point in the middle of the bouys.
            double mLeftI;                              ///<  Left pixel value for gate nav.
            double mRightI;                             ///<  Right pixel value for gate nav.
            double mMaxJ;                               ///<  Max value of J before we consider we have passed the gates.
            double mPixelScale;                         ///<  value to scale pixels for camera
            double mPixelScaleNum;                      ///<  Pixel scale numerator for converting from pixel change to angle change in world.
            double mPixelScaleDenom;                    ///<  Pixel scale denominator for converting from pixel change to angle change in world.
            double mWaypointDistance;                   ///<  Distace to next waypoint.
            double mWaypointThresh;                     ///<  Threshold for waypoints.
            int mWaypointCount;                         ///<  Number of waypoints the boat has traveled.
            bool mStartGate;                            ///<  If we are driving to the start gate(true) or speed gate(false).
            bool mReachedWaypoint;                      ///<  If the boat has reached a point close enough to waypoint to consider being there.
            State mState;                               ///<  The state of the mission.

            IplImage* mInitFrame;                       ///<  The current forward camera frame.
            IplImage* mSegFrame;                        ///<  The Segmented forward frame.
            IplImage* mThreshFrame;                     ///<  The thresholded forward frame.
            IplImage* mFinalFrame;                      ///<  The fully processed forward frame.
            Vision::Segment *mSegment;                  ///<  Segmentation for the camera frame.
            Vision::ThreshRatio *mThreshold;            ///<  Thresholding for segmented frame.
            Vision::Return mGreen;                      ///<  Return values for green image.
            Vision::Return mRed;                        ///<  Return values for red image.

            AI::StopWatch mStopWatch;                   ///<  Stop watch for mission.
            AI::SweepSearch mSearch;                    ///<  Method for sweep search
            bool mSearchFlag;                           ///<  Search flag.
            double mLockHeading;                        ///<  Heading to lock in place and travel.

            bool mFromVisionState;                      ///<  Was the previous sub-state doing vision?
            double mDistOffset;                         ///<  Distance offset.
            double mDistanceFromStart;                  ///<  Travel distance from start.
            double mTempDistCalc;                       ///<  Temporary variable for distance calculations.

            AI::Debounce mRedDebounce;                  ///<  Debounce for red buoys.
            AI::Debounce mGreenDebounce;                ///<  Debounce for green buoys.
            AI::Debounce mBothDebounce;                 ///<  Debouncing for both buoys.
            int mMinGreenArea;                          ///<  Minimum pixel area for accepting green buoy segment.
            int mMinRedArea;                            ///<  Minimum pixel area for accepting red buoy segment.

            int mSecondGateDistance;                    ///<  Distance to second gate.

            int mCheckGateSpeed;                        ///<  Speed to travel while checking for gates.
            int mGreenBuoySpeed;                        ///<  Travel speed when we only see a green buoy
            int mRedBuoySpeed;                          ///<  Travel speed when we only see a red buoy
            int mBothBuoySpeed;                         ///<  Travel speed when we see both buoys
            int mWaypointSpeed;                         ///<  Travel speed when driving blind to waypoints

            BouyObject mRedGateClassifier;
            BouyObject mGreenGateClassifier;

            int mGatesCounted;
            std::string mTrailName;
            GlobalInfo::SimplePose mGuessPose;

        };
    }
}


#endif //StartGateCamera_H
