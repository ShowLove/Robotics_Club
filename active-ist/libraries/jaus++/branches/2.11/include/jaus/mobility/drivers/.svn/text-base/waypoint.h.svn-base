////////////////////////////////////////////////////////////////////////////////////
///
///  \file waypoint.h
///  \brief This file contains the implementation of a waypoint structure
///         template.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 September 2010
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
#ifndef __JAUS_MOBILITY_DATA_WAYPOINT__H
#define __JAUS_MOBILITY_DATA_WAYPOINT__H

#include "jaus/core/types.h"
#include "jaus/mobility/jausmobilitydll.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Waypoint
    ///   \brief A simple data structure for deriving Local or Global waypoints.
    ///
    ///   This structure contains data common to all waypoints, but not necessarily
    ///   data included in any message structures.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL Waypoint
    {
    public:
        Waypoint();
        virtual ~Waypoint();
        // Method to mark this as the current waypoint being navigated to.
        void SetCurrentWaypoint(const bool onFlag = true);
        // Method to mark that the waypoint has been completed.
        void SetWaypointAcheived(const bool flag = true);
        // Is this the current waypoint being driven to?
        bool IsCurrentWaypoint() const;
        // Has this waypoint been achieved yet.
        bool IsWaypointAcheived() const;
        // Is this the same waypoint, with some allowed error?
        virtual bool IsSameAs(const Waypoint* waypoint, const double errorInMeters = 0.25) const = 0;
    protected:
        bool mActiveFlag;   ///<  Is this the current active waypoint we are driving to?
        bool mFinishedFlag; ///<  Flag to mark if the waypoint is completed (for internal use only).
    };
}

#endif
/*  End of File */
