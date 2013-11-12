////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatesensor.h
///  \brief This file contains the definition of the VelocityStateSensor class,
///         used as an interface for reporting instantaneous velocity.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 February 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#ifndef __JAUS_MOBILITY_VELOCITY_STATE_SENSOR__H
#define __JAUS_MOBILITY_VELOCITY_STATE_SENSOR__H

#include "jaus/core/sensor.h"
#include "jaus/mobility/sensors/queryvelocitystate.h"
#include "jaus/mobility/sensors/reportvelocitystate.h"

namespace JAUS
{
    typedef JAUS::ReportVelocityState VelocityState;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class VelocityStateSensor
    ///   \brief The Velocity State Sensor allows for reporting of instantaneous
    ///          platform velocity.
    ///
    //    The Velocity Sensor has the responsibility of reporting the instantaneous
    ///   velocity of the platform.  The velocity state of a rigid body is defined as
    ///   the set of parameters that are necessary to calculate the velocity of any
    ///   point in that rigid body.  Six parameters are required to specify a velocity
    ///   state of a rigid body in terms of some fixed reference coordinate system.
    ///   The first three parameters reporesent the velocity components of a point in
    ///   the rigid body that is coincident with the origin of the fixed reference.
    ///   The second three components represent the instantaneous angular velocity
    ///   components.  It is possible to represent the six velocity state parameters as
    ///   a screw, about which the rigid body is rotating and translating along at that
    ///   instant.
    ///
    ///   The reference frame of the velocity state sensor component is selected as a 
    ///   fixed coodinate system that at this instant is co-located with and aligned
    ///   with the vehicle or system coordinate system.  Thus the message data
    ///   'velocity x', 'velocity y', and 'velocity z' represents the current velocity
    ///   of the subsystem's control point at this instant.  For example if 'velocity x'
    ///   has a value of 3 m/sec and 'velocity 'y' and 'velocity z' are zero, then the
    ///   vehicle is moving in the forward direction at a velocity of 3 m/sec.  The
    ///   message data 'omega x', 'omega y', and 'omega z' represent the actual rate of
    ///   change of orientation or angular velocity of the vehicle about its
    ///   corrdinate axes.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL VelocityStateSensor : public Sensor, public Events::Child
    {
    public:
        const static std::string Name;  ///< String name of the Service.
        VelocityStateSensor(const double updateRate = 10);
        virtual ~VelocityStateSensor();
        // Sensors can always be discovered (overload if you don't want this).
        virtual bool IsDiscoverable() const { return true; }
        // Called by the platform to update this sensor.
        void SetVelocityState(const VelocityState& state);
        // Sets maximum sensor update rate.
        bool SetSensorUpdateRate(const double rate);
        /// Gets the maximum sensor update rate.
        double GetSensorUpdateRate() const { return mMaxUpdateRate; }
        // Gets the velocity state stored by the sensor.
        VelocityState GetVelocityState() const;
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Prints status to console.
        virtual void PrintStatus() const;
    private:
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        // Creates a ReportVelocityState from QueryVelocityState.
        void CreateReportFromQuery(const QueryVelocityState* query, ReportVelocityState& report) const;
        double mMaxUpdateRate;                  ///<  Update rate of the sensor.
        VelocityState mVelocityState;           ///<  Current velocity state data. 
        CxUtils::Mutex mVelocityStateMutex;     ///<  Mutex for thread protection.
    };
}

#endif
/*  End of File */
