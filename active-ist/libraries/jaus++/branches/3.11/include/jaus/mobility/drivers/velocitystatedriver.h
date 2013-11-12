////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatedriver.h
///  \brief This file contains the definition of the VelocityStateDriver class, used
///         as an interface for platform mobility.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: March 23 2011
///  <br>Copyright (c) 2011
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
#ifndef __JAUS_MOBILITY_VELOCITY_STATE_DRIVER__H
#define __JAUS_MOBILITY_VELOCITY_STATE_DRIVER__H

#include "jaus/core/management/management.h"
#include "jaus/mobility/drivers/queryvelocitycommand.h"
#include "jaus/mobility/drivers/queryaccelerationlimit.h"
#include "jaus/mobility/drivers/reportvelocitycommand.h"
#include "jaus/mobility/drivers/reportaccelerationlimit.h"
#include "jaus/mobility/drivers/setvelocitycommand.h"
#include "jaus/mobility/drivers/setaccelerationlimit.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class VelocityStateDriver
    ///   \brief The Velocity State Driver allows for low level control
    ///          of platform mobility.
    ///
    //    This service does not imply any particular platform type such as
    ///   tracked or wheeled, but describes the mobility in six degrees of
    ///   freedom using velocity commands relative to the vehicle's coordinate
    ///   system.  The function of the Velocity State Driver service is to
    ///   control the desired linear and angular velocity of a mobile
    ///   platform.  The Velocity State Driver takes the desired velocity as
    ///   measured with respect to the vehicle coordinate system defined in
    ///   AS6009 service set.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL VelocityStateDriver : public Management::Child
    {
    public:
        const static std::string Name; ///< String name of the Service.
        VelocityStateDriver();
        virtual ~VelocityStateDriver();
        // Method called when Resume command received, overload for additional behavior.
        virtual bool Resume() { return true; }
        // Method called when Reset command received, overload for additional behavior.
        virtual bool Reset()
        { 
            Mutex::ScopedLock lock(&mVelocityStateDriverMutex); 
            mVelocityCommand.ClearMessage(); 
            mVelocityCommand = mDefaultVelocityCommand;
            mVelocityCommand.SetCommandType(SetVelocityCommand::SetCurrentCommand);
            mVelocityCommandTime.SetCurrentTime();
            return true; 
        }
        // Method called when Standby command received, overload for additional behavior.
        virtual bool Standby() { return Reset(); }
        // Method called when Set Emergency command received, overload for additional behavior.
        virtual bool SetEmergency() { return Reset(); }
        // Method called when Clear Emergency command received, overload for additional behavior.
        virtual bool ClearEmergency() { return true; }
        // Method called when Release Control command received, overload for additional behavior.
        virtual bool ReleaseControl() { return Reset(); }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Method called whenever a SetVelocityCommand is received.
        ///
        ///   Overload this method to be notified when a new command is received. Drive
        ///   commands are saved internally regardless and can be accessed using the
        ///   GetCurrentDriveCommand method.
        ///
        ///   \param[in] command Drive command to implement.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool SetDriveCommand(const SetVelocityCommand* command) {  return true; };
        // Sets the acceleration limit.
        virtual bool SetAccelerationLimit(const JAUS::SetAccelerationLimit* command);
        // Gets the current driver command.
        virtual SetVelocityCommand GetCurrentDriveCommand() const;
        // Get drive command limits.
        SetVelocityCommand GetDriveCommandLimits(const bool max) const;
        // Gets the current limit.
        virtual JAUS::SetAccelerationLimit GetAccelerationLimit(const bool max) const;
        // Gets the time when the last drive command was received.
        virtual Time GetDriveCommandTime() const;
        // Generates service specific events.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Adds support for service specific events.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // By default, this is discoverable to other components (overload to hide).
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;  
        // Prints information about the service.
        virtual void PrintStatus() const;
    protected:
        Mutex mVelocityStateDriverMutex;                    ///<  Mutex for thread protection of data.
    private:
        void CreateReportFromQuery(const QueryVelocityCommand* query, 
                                   ReportVelocityCommand& report) const;
        void CreateReportFromQuery(const QueryAccelerationLimit* query, 
                                   ReportAccelerationLimit& report) const;
        Time mVelocityCommandTime;                          ///<  Time when the last drive command was received.
        JAUS::SetVelocityCommand mVelocityCommand;          ///<  The last drive command received.
        JAUS::SetVelocityCommand mDefaultVelocityCommand;   ///<  The default vector.
        JAUS::SetVelocityCommand mMaxVelocityCommand;       ///<  The max values allowed.
        JAUS::SetVelocityCommand mMinVelocityCommand;       ///<  The min values allowed.
        JAUS::SetAccelerationLimit mMaxAcceleration;        ///<  Current acceleration limit data for service.
        JAUS::SetAccelerationLimit mMinAcceleration;        ///<  Current acceleration limit data for service.
    };
}

#endif
/*  End of File */
