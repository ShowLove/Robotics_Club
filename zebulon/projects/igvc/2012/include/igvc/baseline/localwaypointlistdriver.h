////////////////////////////////////////////////////////////////////////////////////
///
/// \file localwaypointlistdriver.h
/// \brief Class which implements the Local Waypoint List driver service.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#ifndef _ZEBULON_IGVC_LOCAL_WAYPOINT_LIST_DRIVER__H
#define _ZEBULON_IGVC_LOCAL_WAYPOINT_LIST_DRIVER__H

#include <jaus/mobility/drivers/localwaypointlistdriver.h>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class LocalWaypointListDriver
        ///   \brief Implementation of a JAUS Local Waypoint List Driver service.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class LocalWaypointListDriver : public JAUS::LocalWaypointListDriver
        {
        public:
            LocalWaypointListDriver();
            virtual ~LocalWaypointListDriver();
            void SetDriverID(const JAUS::Address& id) { mDriverID = id; }
            virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        protected:
            static void DriveThread(void* ptr);
            virtual void Execute();
            JAUS::Address mDriverID;        ///<  ID of the closed loop driver.
            double mMaxDriveVelocity;       ///<  Max drive velocity.
            JAUS::Thread mDriverThread;     ///<  Thread used for driving to waypoints.
        };
    }
}

#endif // _ZEBULON_IGVC_LOCAL_WAYPOINT_LIST_DRIVER__H

/*  End of File */