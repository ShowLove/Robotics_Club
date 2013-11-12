/////////////////////////////////////////////////////////////////////////////////////
///
/// \file microcontroller.h
/// \brief Micorcontroller service used by the Ground vehicle.
///
///  <br>Author(s): Daniel Barber<br>
///  <br>Created: 2011<br>
///  <br>Copyright (c) 2011<br>
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://robotics.ucf.edu
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
#ifndef __ZEBULON_GROUND_MICROCONTROLLER__H
#define __ZEBULON_GROUND_MICROCONTROLLER__H

#include <jaus/extras/mcu/microcontroller.h>
#include <mcu/interface.h>
#include <mcu/messagefactory.h>
#include <iostream>
#include <map>

#include <cxutils/thread.h>
#include <cxutils/mutex.h>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Microcontroller
        ///   \brief This class is the Microcontroller service for reading
        ///          in digital/analog inputs, and outputs digital/analog outputs.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Microcontroller : public JAUS::Microcontroller,
                                public MCU::Interface::Callback
        {
        public:
                /**  Stores constants for pin names for microcontroller */
                class PinNames
                {
                public:
                    const static std::string TeleopControl;         ///<  String name for teleop control pin.
                };
                /**  Stores constants for pin numbers for microcontroller */
                class PinNumbers
                {
                public:
                    const static unsigned char TeleopControl = 0;   ///<  String name for teleop pin.
                };
                Microcontroller();
                virtual ~Microcontroller();
                // Sets the state of a device (on = true, false = off) (JAUS command).
                virtual void SetDigitalOut(const std::string& name, const bool value);
                // Sets the analog value for a pin/device [-100,100]% of max (JAUS command).
                virtual void SetAnalogOut(const std::string& name, const double value);
                // Takes in messages from the MCU and sets the appropriate member variable (digita/analog).
                virtual void ProcessMessage(const MCU::Message* message);
                // Places the MCU in the Ready state
                virtual bool Resume() { return true;}
                // Places the MCU in the Standby state
                virtual bool Standby() { return true;}
                // Resets the MCU
                virtual bool Reset() { return true;}
                // Returns true if in teleop mode, false if autonomous.
                bool IsTeleopMode() const { return GetDigitalState(PinNames::TeleopControl); }
                // Returns true if in autonomous mode, false otherwise.
                bool IsAutonomousMode() const { return !IsTeleopMode(); }
                // Get Analog outputs for MCU.
                MCU::SetServoOutputs GetServoOutputs(const std::string& servoName) const;
                // Get Digital outputs for MCU.
                MCU::SetDigitalOutputs GetDigitalOutputs(const std::string& digitalName) const;
        protected:
                CxUtils::Mutex mMutex;                                              ///<  Mutex for read/write protection.
                std::map<std::string,MCU::SetDigitalOutputs> mDigitalOutputPins;    ///<  Map holding the current values of the digital outputs.
                std::map<std::string,MCU::SetServoOutputs> mServoOutputPins;        ///<  Map holding the current values of the analog outputs.
        };
    }
}
#endif
