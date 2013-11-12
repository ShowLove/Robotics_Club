////////////////////////////////////////////////////////////////////////////////////
///
/// \file statemachine.h
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
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
#ifndef __ZEBULON_SURFACE_STATE_MACHINE__H
#define __ZEBULON_SURFACE_STATE_MACHINE__H


#include <statemachine/statemachine.h>
#include "asvc/surface/globalinfo.h"
#include "asvc/surface/globalcommand.h"

#include "asvc/surface/missions/testmission.h"
#include "asvc/surface/missions/waypointlist.h"
#include "asvc/surface/missions/boatstart.h"
#include "asvc/surface/missions/startgatecamera.h"
#include "asvc/surface/missions/buoynav.h"
#include "asvc/surface/missions/buoynavcamera.h"
#include "asvc/surface/missions/extinguish.h"
#include "asvc/surface/missions/returntochannel.h"
#include "asvc/surface/missions/cartographerbeam.h"


namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class StateMachine
        ///
        ///   State machine used by platform.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class StateMachine : public AI::StateMachine
        {
        public:
            StateMachine();
            virtual ~StateMachine();
            virtual bool AddMissions();
            std::vector<std::string> mMissionsToRun;    ///<  Names of missions to run.
        };
    }
}

#endif
