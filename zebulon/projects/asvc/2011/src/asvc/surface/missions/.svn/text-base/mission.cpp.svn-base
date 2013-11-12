/////////////////////////////////////////////////////////////////////////////////////
///
/// \file mission.cpp
/// \brief Implementation of Mission Interface.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
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
#include "asvc/surface/missions/mission.h"

using namespace Zebulon;
using namespace ASVC;

ASVC::GlobalInfo* Mission::mpGlobalInfo = NULL;
ASVC::GlobalCommand* Mission::mpGlobalCommand = NULL;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Mission::Mission()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Mission::~Mission()
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global (static) pointer to the command interface for
///   driving the platform and moving actuators.
///
////////////////////////////////////////////////////////////////////////////////////
void Mission::SetGlobalCommand(AI::GlobalCommand* globalCommand)
{
    mpGlobalCommand = (ASVC::GlobalCommand*)globalCommand;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global (static) pointer to the information store used
///   by state machine logic.
///
////////////////////////////////////////////////////////////////////////////////////
void Mission::SetGlobalInfo(AI::GlobalInfo* globalInfo)
{
    mpGlobalInfo = (ASVC::GlobalInfo*)globalInfo;
}

/* End of File */

