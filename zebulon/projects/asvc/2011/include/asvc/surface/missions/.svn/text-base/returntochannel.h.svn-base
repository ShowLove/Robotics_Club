//////////////////////////////////////////////////////////////////////////////////////
///
/// \file returntochannel.h
/// \brief Returns to the location and heading of the marked end of Buoy Channel
///        for returning to dock.
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
#ifndef _ZEBULON_SURFACE_RETURN_TO_CHANNEL__H
#define _ZEBULON_SURFACE_RETURN_TO_CHANNEL__H

#include "asvc/surface/missions/mission.h"
#include "asvc/surface/mapbuilder.h"

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class ReturnToChannel
        ///  \brief Uses channel end position and heading saved to Global Info by a
        ///         previous run to get back to channel using some obstacle avoidance.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ReturnToChannel : public MapBuilder
        {
        public:
            enum State
            {
                Waypoint = 0,
                Heading
            };
            static const std::string Name; ///<  Name of the mission.
            ReturnToChannel();
            ~ReturnToChannel();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        protected:
            virtual void DrawMaps();
            State mCurrentState;                ///<  Current state.
            std::string mNextMission;           ///<  Next mission to return.
            CxUtils::Utm mChannelStartUtm;      ///<  Location of channel start.
            CxUtils::Utm mCurrentPosition;      ///<  Current position UTM.
            double mChannelStartHeading;        ///<  Heading to face when we reach channel.
            double mDistanceThresh;             ///<  How close to get to waypoint.
            double mTravelSpeed;                ///<  Default travel speed.
        };
    }
}


#endif // RETURN_TO_CHANNEL__H

/* End of File */
