////////////////////////////////////////////////////////////////////////////////////
///
///  \file accelerationstatesensor.h
///  \brief This file contains the definition of the AccelerationStateSensor class,
///         used as an interface for reporting acceleration state of the platform.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 9 February 2010
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
#ifndef __JAUS_MOBILITY_ACCELERATION_STATE_SENSOR__H
#define __JAUS_MOBILITY_ACCELERATION_STATE_SENSOR__H

#include "jaus/core/sensor.h"
#include "jaus/mobility/sensors/queryaccelerationstate.h"
#include "jaus/mobility/sensors/reportaccelerationstate.h"

namespace JAUS
{
    typedef JAUS::ReportAccelerationState AccelerationState;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class AccelerationStateSensor
    ///   \brief The Velocity State Sensor allows for reporting of the state of the
    ///          platform's acceleration.
    ///
    ///   This service reports the acceleration state of the unmanned platform.  The
    ///   Acceleration State Sensor reports the acceleration state that is the first
    ///   derivative (the rate of change) of the velocity state reported by the
    ///   Velocity State Sensor.  Refer to the description on Velocity State Sensor
    ///   for coordinate details.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_MOBILITY_DLL AccelerationStateSensor : public Sensor, public Events::Child
    {
    public:
        const static std::string Name;  ///< String name of the Service.
        AccelerationStateSensor(const double updateRate = 10);
        virtual ~AccelerationStateSensor();
        // Called by the platform to update this sensor.
        void SetAccelerationState(const AccelerationState& state);
        // Sets maximum sensor update rate.
        bool SetSensorUpdateRate(const double rate);
        /// Gets the maximum sensor update rate.
        double GetSensorUpdateRate() const { return mMaxUpdateRate; }
        // Gets the velocity state stored by the sensor.
        AccelerationState GetAccelerationState() const;
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        /// By default, the Acceleration State Sensor is discoverable to other components (overload to hide).
        virtual bool IsDiscoverable() const { return true; }
        // Method called whenver a message is received.
        virtual void Receive(const Message* message);
        // Prints status to console.
        virtual void PrintStatus() const;
        // Creates messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;  
    private:
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        // Creates a ReportAccelerationState from QueryAccelerationState.
        void CreateReportFromQuery(const QueryAccelerationState* query, AccelerationState& report) const;
        double mMaxUpdateRate;                          ///<  Update rate of the sensor.
        AccelerationState mAccelerationState;     ///<  Current velocity state data. 
        CxUtils::Mutex mAccelerationStateMutex;         ///<  Mutex for thread protection.
    };
}

#endif
/*  End of File */
