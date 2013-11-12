//////////////////////////////////////////////////////////////////////////////////////
///
/// \file boatstart.h
/// \brief Mission 1.* Waits for start button to be pushed
///
/// Author(s): Brian Valentino<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:brian_valentino@knights.ucf.edu<br>
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
#ifndef _ZEBULON_SURFACE_BOATSTART_H
#define _ZEBULON_SURFACE_BOATSTART_H

#include "asvc/surface/missions/mission.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class BoatStart
        ///  \brief First mission to always be run.  It waits until the start button
        ///  is pressed, then exits.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class BoatStart : public Mission
        {
        public:
            static const std::string Name; ///<  Name of the mission.
            BoatStart();
            ~BoatStart();
            virtual int Init();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
        private:
            AI::Debounce mDebounce;     ///< Debounce interface to check if a button has been pressed.
            AI::Debounce mRadioDebounce;     ///< Debounce interface to check if a button has been pressed.
            bool mStartButton;          ///< The state of the start button.
            int mRadioState;          ///< The state of the start button.
            CxUtils::Utm mStartPosition;///< The starting position of the vehicle.
            std::string mNextMission;   ///< The next mission to go to on exit.
            double mDriveThrust;            ///<  Drive thrust (not speed).
            double mDriveTimeSeconds;       ///<  How long to drive in seconds.
            AI::StopWatch mDriveStopWatch;  ///<  Stop watch for a short drive right after starting.
            IplImage *mInitFrame;
            bool mDisplayFlag;
        };
    }
}


#endif //BOATSTART_H
/* End of File */
