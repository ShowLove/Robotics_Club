////////////////////////////////////////////////////////////////////////////////////
///
/// \file waypointlist.h
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
///
/// Author(s): Matt Znoj<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: matthew.znoj@knights.ucf.edu<br>
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
////////////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_SURFACE_WAYPOINTLIST__H
#define __ZEBULON_SURFACE_WAYPOINTLIST__H

#include "asvc/surface/missions/mission.h"

#include <vector>

namespace Zebulon
{
    namespace ASVC
    { 
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class WayPointList
        ///  \brief Simple mission which will drive to a set of GPS waypoints.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class WayPointList : public ASVC::Mission
        {
        public:
            const static std::string Name;  ///<  Name of the mission.
            enum State
            {
                GoToWaypoint,
                NextWaypoint,
                Finish
            };

            WayPointList();
            ~WayPointList();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            State mState;

            Utm mCurrentPosition;
            std::list<Utm> mDesiredPositionList;
            std::list<Utm>::iterator mDesiredPositionIterator;

            Orientation mCurrentOrientation;
            Orientation mDesiredOrientation;

            double mCurrentVelocity;
            double mDesiredVelocity;
            double mMaxVelocity;

            double mMaxDistanceThresh;
            double mDistanceThresh;
            double mForceWeight;
            double mSinkWeight;

            CxUtils::Point3D::List mRangeData;
        };
    }
}

#endif // __ZEBULON_SURFACE_WAYPOINTLIST__H
/* End of file */
