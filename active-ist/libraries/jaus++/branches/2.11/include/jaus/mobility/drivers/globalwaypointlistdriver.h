////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalwaypointlistdriver.h
///  \brief This file contains the implementation of the Global Waypoint List
///         Driver service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 April 2010
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
#ifndef __JAUS_MOBILITY_GLOBAL_WAYPOINT_LIST_DRIVER__H
#define __JAUS_MOBILITY_GLOBAL_WAYPOINT_LIST_DRIVER__H

#include "jaus/mobility/drivers/listdriver.h"
#include "jaus/mobility/drivers/globalwaypointdriver.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class GlobalWaypointListDriver
    ///   \brief This class is used to drive a platform to multiple waypoints.
    ///
    ///   This is a basic implementation of a GlobalWaypointListDriver, it just
    ///   processes the messages and makes the available, overload from it or
    ///   use it to drive to waypoints.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL GlobalWaypointListDriver : public ListDriver
    {
    public:
        const static std::string Name;       ///<  Name of this service.
        GlobalWaypointListDriver(const double updateRateHz = 2);
        virtual ~GlobalWaypointListDriver();
        // Method called to begin/continue execution of a list.
        virtual void ExecuteList(const double speedMetersPerSecond);
        // Method called to check if an element type (message payload) is supported.
        virtual bool IsElementSupported(const Message* message) const;
        // Method called whenever a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Generates Global Waypoint Driver related events.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Adds support for Report Global Waypoint events.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Gets the current active waypoint element in list.
        JAUS::SetGlobalWaypoint GetCurrentWaypoint() const;
        // Gets the list of waypoints so far.
        std::vector<JAUS::SetGlobalWaypoint> GetWaypointList() const;
        // Prints status to console.
        virtual void PrintStatus() const;
    };
}

#endif
/*  End of File */
