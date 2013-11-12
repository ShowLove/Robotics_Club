////////////////////////////////////////////////////////////////////////////////////
///
///  \file waypoint.cpp
///  \brief This file contains the implementation of a waypoint structure
///         template.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 22 September 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/mobility/drivers/waypoint.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Waypoint::Waypoint() : mActiveFlag(false), mFinishedFlag(false)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Waypoint::~Waypoint() {}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to set this waypoint as the current/active waypoint being
///          navigated to by the unmanned system.
///
///   \param[in] onFlag If true, sets as active, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
void Waypoint::SetCurrentWaypoint(const bool onFlag)
{
    mActiveFlag = onFlag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to mark a waypoint as finished.
///
///   This is just additional data that can be optionally used by a
///   list driver or waypoint driver to mark that status of a waypoint.  This
///   data is not part of any current JAUS message.
///
///   \param[in] flag If true, sets as finished, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
void Waypoint::SetWaypointAcheived(const bool flag)
{
    mFinishedFlag = flag;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   return True if the waypoint is currently being navigated to, false 
///          otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Waypoint::IsCurrentWaypoint() const { return mActiveFlag; }

////////////////////////////////////////////////////////////////////////////////////
///
///   return True if the waypoint has been marked as completed at some point in
///          time, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Waypoint::IsWaypointAcheived() const { return mFinishedFlag; }


/*  End of File */
