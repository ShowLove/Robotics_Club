////////////////////////////////////////////////////////////////////////////////////
///
///  \file microcontroller.h
///  \brief Contains the Microcontroller service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 9 February 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_MICROCONTROLLER__H
#define __JAUS_EXTRAS_MICROCONTROLLER__H

#include "jaus/extras/jausextrasdll.h"
#include "jaus/core/management/management.h"
#include "jaus/core/sensor.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Microcontroller
    ///   \brief Service so that controlling components can interact with 
    ///          Microcontrollers on a subsystem.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL Microcontroller : public Management::Child, public Sensor
    {
    public:
        const static std::string Name;       ///<  Name of service (string).
        typedef std::map<std::string, bool> DigitalStates;
        typedef std::map<std::string, double> AnalogStates;
        Microcontroller();
        virtual ~Microcontroller();
        // Service is discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Sets the state of a device (on = true, false = off) (overload to send command to MCU).
        virtual void SetDigitalOut(const std::string& name,
                                   const bool value) = 0;
        // Sets the analog value for a pin/device [-100,100]% of max (overload to send command to MCU).
        virtual void SetAnalogOut(const std::string& name,
                                  const double value) = 0;
        // Sets the state of a digital device input (on = true, false = off) (from data recrived from MCU).
        virtual void SetDigitalInput(const std::string& name,
                                     const bool value);
        // Sets the analog value for a pin/device [-100,100]% of max (from data recrived from MCU).
        virtual void SetAnalogInput(const std::string& name,
                                    const double value);        
        // Gets the state of a device by name (values set by the SetDigitalInput method).
        virtual bool GetDigitalState(const std::string& name) const;
        // Gets the current analog state of a pin/device by name (values set by SetAnalogState method).
        virtual double GetAnalogState(const std::string& name) const;
        // Gets the digital states of all pins set.
        DigitalStates GetDigitalStates() const { Mutex::ScopedLock lock(&mMcuMutex); return mDigitalStates; }
        // Gets the analog states of all pins set.
        AnalogStates GetAnalogStates() const { Mutex::ScopedLock lock(&mMcuMutex); return mAnalogStates; }
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // Creates any messages associated with this service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Processes JAUS messages associated with this Service.
        virtual void Receive(const Message* message);
        // Method called when transitioning to a ready state.
        virtual bool Resume() { return true; }
        // Method called to transition due to reset.
        virtual bool Reset() { return true; }
        // Method called when transitioning to a standby state.
        virtual bool Standby() { return true; }
        // Method called when transitioning to an emergency state.
        virtual bool SetEmergency() { return true; }
        // Method called when leaving the emergency state.
        virtual bool ClearEmergency() { return true; }
        // Method called when control is released.
        virtual bool ReleaseControl() { return true; }
        // Prints data to console.
        virtual void PrintStatus() const;
    protected:
        virtual void CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs);
        // Signal an event.
        virtual void SignalEvent(const bool digital,
                                 const std::string& name);
        Mutex mMcuMutex;                ///<  Mutex for thread protection of data.
        DigitalStates   mDigitalStates; ///<  Digital State of pins.
        AnalogStates    mAnalogStates;  ///<  Analog state of pins.
    };
}

#endif
/*  End of File */
