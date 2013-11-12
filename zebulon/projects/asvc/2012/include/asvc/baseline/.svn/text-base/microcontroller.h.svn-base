/////////////////////////////////////////////////////////////////////////////////////
///
/// \file microcontroller.h
/// \brief Micorcontroller service used by the Boat
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
#include <mcu/slipinterface.h>
#include <mcu/messagefactory.h>
#include <iostream>
#include <map>

#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <boost/algorithm/string.hpp>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Microcontroller
        ///   \brief This class is the Microcontroller service for reading
        ///          in digital/analog inputs, and outputs digital/analog outputs.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Microcontroller : public JAUS::Microcontroller,
                                public MCU::SlipInterface::Callback
        {
        public:
            enum PortNumber
            {
                DynamixelActual = 0,
                Actuator,
                VoltageMotors = 2,
                VoltageLogic = 3,
                Pump,
                ResetButton = 5,
                RadioTriState,
                EStopButton,
                Autonomous,
                Temperature,
                DynamixelDesired
            };
            const static std::string DYNAMIXELACTUAL;
            const static std::string DYNAMIXELDESIRED;
            const static std::string ACTUATOR;
            const static std::string VOLTAGEMOTORS;
            const static std::string VOLTAGELOGIC;
            const static std::string PUMP;
            const static std::string RESETBUTTON;
            const static std::string RADIOTRISTATE;
            const static std::string ESTOPBUTTON;
            const static std::string AUTONOMOUS;
            const static std::string TEMPERATURE;

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
            bool IsTeleopMode() const { return !IsAutonomousMode(); }
            // Returns true if in autonomous mode, false otherwise.
            bool IsAutonomousMode() const;
            // Sets the state of a device (on = true, false = off) (JAUS command).
            virtual void SetDigitalInput(const PortNumber&, const bool value);
            // Sets the analog value for a pin/device [-100,100]% of max (JAUS command).
            virtual void SetAnalogInput(const PortNumber& portnum, const double value);
            // Get Analog outputs for MCU.
            MCU::SetServoOutputs GetServoOutputs(const PortNumber& portnum) const;
            // Get Digital outputs for MCU.
            MCU::SetDigitalOutputs GetDigitalOutputs(const PortNumber& portnum) const;
            double GetAnalogOutputs(const PortNumber& portnum) const;
            /** Converts a port enum to a string. */
            static std::string ToString(const PortNumber& portnum)
            {
                switch(portnum)
                {
                    case DynamixelActual:
                        return DYNAMIXELACTUAL;
                    case DynamixelDesired:
                        return DYNAMIXELDESIRED;
                    case Actuator:
                        return ACTUATOR;
                    case VoltageMotors:
                        return VOLTAGEMOTORS;
                    case VoltageLogic:
                        return VOLTAGELOGIC;
                    case Pump:
                        return PUMP;
                    case ResetButton:
                        return RESETBUTTON;
                    case RadioTriState:
                        return RADIOTRISTATE;
                    case EStopButton:
                        return ESTOPBUTTON;
                    case Autonomous:
                        return AUTONOMOUS;
                    case Temperature:
                        return TEMPERATURE;
                    default:
                        return "Undefined Port";
                 }
            }
            /** Converts string name of a port to an enum. */
            static PortNumber ToPort(const std::string portname)
            {
                if(boost::iequals(portname,DYNAMIXELACTUAL))
                {
                    return DynamixelActual;
                }
                if(boost::iequals(portname,DYNAMIXELDESIRED))
                {
                    return DynamixelDesired;
                }
                if(boost::iequals(portname,ACTUATOR))
                {
                    return Actuator;
                }
                if(boost::iequals(portname,VOLTAGEMOTORS))
                {
                    return VoltageMotors;
                }
                if(boost::iequals(portname,VOLTAGELOGIC))
                {
                    return VoltageLogic;
                }
                if(boost::iequals(portname,PUMP))
                {
                    return Pump;
                }
                if(boost::iequals(portname,RESETBUTTON))
                {
                    return ResetButton;
                }
                if(boost::iequals(portname,RADIOTRISTATE))
                {
                    return RadioTriState;
                }
                if(boost::iequals(portname,ESTOPBUTTON))
                {
                    return EStopButton;
                }
                if(boost::iequals(portname,TEMPERATURE))
                {
                    return Temperature;
                }
            }
        protected:
            CxUtils::Mutex mMutex;                                              ///<  Mutex for read/write protection.
            std::map<PortNumber,MCU::SetDigitalOutputs> mDesiredDigitalOutput;    ///<  Map holding desired digital out states
            std::map<PortNumber,MCU::SetServoOutputs> mDesiredServoOutput;        ///<  Map holding desired analog out states
        };
    }
}
#endif
