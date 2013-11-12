////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalcommand.h
/// \brief Global Command interface for AI program to drive the
///        ground vehicle.
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
#ifndef __ZEBULON_GROUND_GLOBAL_COMMAND__H
#define __ZEBULON_GROUND_GLOBAL_COMMAND__H

#include <statemachine/globalcommand.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/mobility/drivers/velocitystatedriver.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include <cartographer/planning/beamsearch.h>
#include "igvc/ground/globalinfo.h"

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalCommand
        ///  \brief Command class for operating the robotic platform through a
        ///         single interface.  This class can change control modes from
        ///         closed loop to open loop, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalCommand : public AI::GlobalCommand
        {
        public:
            GlobalCommand();
            virtual ~GlobalCommand();
            virtual bool LoadSettings(const std::string& xmlFilename);
            void SetDrivers(const JAUS::Address& primitiveDriver,
                            const JAUS::Address& velocityDriver)
                            {
                                CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
                                mPrimitiveDriverComponentID = primitiveDriver;
                                mVelocityStateDriverComponentID = velocityDriver;
                            }
            void SetThrust(const double thrust);
            void SetSteering(const double steering);
            void SetYaw(const double yaw);
            /** Sets desired heading in degrees [-180,180]. */
            void SetYawDegrees(const double yawDegrees) { SetYaw(CxUtils::CxToRadians(yawDegrees)); }
            void SetVelocity(const double velocity);
            void SetVelocityAsPercent(const double percent) { SetVelocity(percent*mMaxVelocityX/100.0); }
            void SetRotationRate(const double rotationRate);
            /** Sets desired rotatoin rate in degrees/sec. */
            void SetRotationRateDegrees(const double rotationRateDegrees) { SetRotationRate(CxUtils::CxToRadians(rotationRateDegrees)); }
            /** Sets a destination waypoint to drive to.  Use limited obstacle avoidance. */
            void SetDestination(const Wgs& destination, const double thresh = 0.5);
            void SetDestination(const Utm& destination, const double thresh = 0.5) { SetDestination(Wgs(destination), thresh); }
            virtual void PrintStatus() const;
        protected:
            /** Overload to have run in a thread commands. */
            virtual void CommandThread();
            AI::GlobalCommand::CommandMode mCommandMode;        ///<  Command mode.
            CxUtils::Mutex mCommandMutex;                       ///<  For thread protection of data.
            volatile bool mStandardsModeFlag;                   ///<  If true, use SetVelocityCommand, otherwise SetVecolityVector
            JAUS::Address mPrimitiveDriverComponentID;          ///<  ID of Primitive Driver component.
            JAUS::Address mVelocityStateDriverComponentID;      ///<  ID of Velocity State Driver component.
            JAUS::SetWrenchEffort mWrenchEffort;                ///<  JAUS wrench effort command.
            JAUS::SetVelocityCommand mVelocityCommand;          ///<  Velocity command.
            JAUS::SetVelocityVector mVelocityVector;            ///<  Velocity vector command (heading control).
            Wgs mDestinationWaypoint;                           ///<  Destination waypoint.
            double mWaypointThreshold;                          ///<  Waypoint distance threshold.
            double mMaxVelocityX;                                ///<  Maximum vehicle velocity.
        };
    }
}

 #endif
 /*  End of File */
