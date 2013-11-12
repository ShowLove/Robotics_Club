////////////////////////////////////////////////////////////////////////////////////
///
/// \file statemachine.cpp
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
///
/// Author(s): Michael Scherer, Daniel Barber<br>
/// Created: 2010<br>
/// Copyright (c) 2011<br>
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
#include "asvc/surface/statemachine.h"

using namespace Zebulon;
using namespace ASVC;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
StateMachine::StateMachine()
{
    // Add all possible missions to the factory.
    AddMissionToFactory(TestMission::Name, new TestMission());
    AddMissionToFactory(WayPointList::Name, new WayPointList());
    AddMissionToFactory(BoatStart::Name, new BoatStart());
    AddMissionToFactory(StartGateCamera::Name, new StartGateCamera());
    AddMissionToFactory(BuoyNav::Name, new BuoyNav());
    AddMissionToFactory(BuoyNavCamera::Name, new BuoyNavCamera());
    AddMissionToFactory(Extinguish::Name, new Extinguish());
    // Return to dock missions.
    AddMissionToFactory(ReturnToChannel::Name, new ReturnToChannel());
    AddMissionToFactory(BuoyNavCamera::ReturnName, new BuoyNavCamera(true));
    AddMissionToFactory(StartGateCamera::ReturnName, new StartGateCamera(true));
    AddMissionToFactory(CartographerBeam::Name, new CartographerBeam());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
StateMachine::~StateMachine()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Overloaded method which adds all missions to the state machine.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool StateMachine::AddMissions()
{
    // Must set in XML.
    if(mMissionsToRun.size() > 0)
    {
        return AddMissionsFromFactory(mMissionsToRun);
    }
    return false;
}

/*  End of File */

