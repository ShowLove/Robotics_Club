//////////////////////////////////////////////////////////////////////////////////////////
///
/// \file globalcommand.h
/// \brief Class for sending commands to the baseline.
///
/// Author(s): Matt Znoj<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: mattznoj@gmail.com<br>
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
//////////////////////////////////////////////////////////////////////////////////////////
#ifndef __SURFACE_GLOBALCOMMAND_H
#define __SURFACE_GLOBALCOMMAND_H

#include <statemachine/globalcommand.h>
#include <statemachine/globalinfo.h>
#include <jaus/mobility/drivers/setwrencheffort.h>
#include <jaus/extras/mcu/setmicrocontrollerstate.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include <jaus/mobility/drivers/setvelocitycommand.h>
#include <cxutils/math/pidcontroller.h>
#include "asvc/surface/globalinfo.h"


namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GlobalCommand
        ///
        ///   Primary command interface used for control by state machine logic.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalCommand : public AI::GlobalCommand
        {
        public:
            GlobalCommand();
            virtual ~GlobalCommand();
            // Sets the ID of the driving components to use.
            void SetDrivers(const JAUS::Address& primitiveDriver,
                            const JAUS::Address& velocityDriver)
            {
                CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
                mPrimitiveDriverComponentID = primitiveDriver;
                mVelocityStateDriverComponentID = velocityDriver;
            }
            // Sets ID of micricontroller component.
            void SetMicrocontroller(const JAUS::Address& microcontroller)
            {
                CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
                mMicrocontrollerComponentID = microcontroller;
            }
            // Optional method to load settings from an XML file.
            virtual bool LoadFromXML(const std::string& filename) { return true; }
            // Gives a pointer to the info class and starts the thread
            bool Initialize(AI::GlobalInfo* globalInfo, unsigned int delayTimeMs=50);
            // Set forward motor power.    (-100,100) (linar effor x?)
            void SetForwardThrust(double thrust);
            // Set lateral motor power.    (-100,100) (linear effor y?)
            void SetLateralThrust(double thrust);
            // Set turning power.          (-100,100) (rotational effor z?)
            void SetTurningThrust(double thrust);
            // Set Forward Velocity (0, max v)
            void SetForwardVelocity(double v);
            // Set Forward Velocity as a percentage of max V. [-100, 100].
            void SetForwardVelocityPercent(const double percent) { SetForwardVelocity(percent*mMaxVelocity/100.0); }
            // Set Lateral Velocity (0, to max v)
            void SetLateralVelocity(double v);
            // Set Lateral Velocity as a percentage of max V. [-100, 100].
            void SetLateralVelocityPercent(const double percent) { SetLateralVelocity(percent*mMaxVelocity/100.0); }
            void SetMaximumVelocity(const double maxV) { mMaxVelocity = fabs(maxV); }
            void SetYaw(const double yaw);
            void SetYawUtm(const double northing, const double easting);
            void SetYawUtm(const Utm& utm) { SetYawUtm(utm.mNorthing, utm.mEasting); }
            void SetYawWgs(const Wgs& wgs) { SetYawUtm(wgs); }
            void SetYawWgs(const double latitude, const double longitude) { SetYawWgs(Wgs(latitude, longitude, 0)); }
            /** Sets desired heading in degrees [-180,180]. */
            void SetYawDegrees(const double yawDegrees) { SetYaw(CxUtils::CxToRadians(yawDegrees)); }
            /** Sets rotation rate (radians). */
            void SetRotationRate(const double rotationRate);
            /** Sets desired rotatoin rate in degrees/sec. */
            void SetRotationRateDegrees(const double rotationRateDegrees) { SetRotationRate(CxUtils::CxToRadians(rotationRateDegrees)); }
            //Microcontroller
            // Turn the light on/off  (digital state)
            void SetComputerLightOne(bool value);
            // Turn the light on/off   (digital state)
            void SetComputerLightTwo(bool value);
            // Turn the water cannon on/off (0,100) (analog state)
            void SetWaterCannon(double value);
            // Set the tilt for the water cannon (-100, 100) 100 being most up.
            void SetWaterCannonTilt(double value);
            // Arm position [0, 100] 100 put away.
            void SetArmPosition(const double value = 100);
            // Sets the positon (northing,easting) we want to go to
            int SetDesiredWaypoint(double northing, double easting);
            // Gets the current drive mode command is operating in.
            CommandMode GetDriveMode() const;
            ASVC::GlobalInfo* GetGlobalInfo() { return (ASVC::GlobalInfo*)mGlobalInfo; }
            const ASVC::GlobalInfo* GetGlobalInfo() const { return (ASVC::GlobalInfo*)mGlobalInfo; }
        private:
            virtual void CommandThread();                               ///< Does actual commanding
            CxUtils::Mutex mCommandMutex;                               ///< For thread protection of data.
            JAUS::SetWrenchEffort mWrenchEffort;                        ///< Used to send boat motor commands
            JAUS::SetMicrocontrollerState mMicrocontrollerState;        ///< Used to send microcontroller states
            JAUS::SetVelocityCommand mVelocityCommand;                  ///< Used to send velocity commands to boat
            JAUS::SetVelocityVector mVelocityVector;                    ///< Used to send velocity vector commands to boat
            JAUS::Address mPrimitiveDriverComponentID;                  ///< Used to identify the primitive driver component
            JAUS::Address mVelocityStateDriverComponentID;              ///< Used to identify the velocity state driver component
            JAUS::Address mMicrocontrollerComponentID;                  ///< Used to identify the microcontroller service.
            CommandMode mCommandMode;                                   ///< Drive control mode.
            int mCommandLoops;                                          ///< Tracks number of times command loop is run (for blinking blue)
            bool mStandardModeFlag;                                     ///< Flag for velocity command or vector
            double mMaxVelocity;                                        ///< Maximum velocity.
        };
    }
}


#endif // GLOBALCOMMAND_H
