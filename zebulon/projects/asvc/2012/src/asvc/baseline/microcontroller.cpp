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
#include "asvc/baseline/microcontroller.h"
//#include <bitvector/bitvector.h>
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace ASVC;

const std::string Microcontroller::DYNAMIXELACTUAL = "DynamixelActual";
const std::string Microcontroller::DYNAMIXELDESIRED = "DynamixelDesired";
const std::string Microcontroller::ACTUATOR = "Actuator";
const std::string Microcontroller::VOLTAGEMOTORS = "VoltageMotors";
const std::string Microcontroller::VOLTAGELOGIC = "VoltageLogic";
const std::string Microcontroller::PUMP = "Pump";
const std::string Microcontroller::RESETBUTTON = "ResetButton";
const std::string Microcontroller::RADIOTRISTATE = "RadioTriState";
const std::string Microcontroller::ESTOPBUTTON = "EStopButton";
const std::string Microcontroller::AUTONOMOUS = "Autonomous";
const std::string Microcontroller::TEMPERATURE = "Temperature";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller::Microcontroller()
{
    // Initialize digital outputs values here to defaults
    //    Dyamixel = 0,
    //    Actuator,
    //    Pump,
    //    VoltageElectronics = 3,
    //    RFID,
    //    EStopButton,
    //    ResetButton,
    //    Autonomous,
    //    VoltageMotors,
    //    Temperature

    mDesiredDigitalOutput[Pump].mPort = Pump;
    mDesiredDigitalOutput[Pump].mValue = false;

    mDesiredDigitalOutput[EStopButton].mPort = EStopButton;
    mDesiredDigitalOutput[EStopButton].mValue = false;

    mDesiredDigitalOutput[ResetButton].mPort = ResetButton;
    mDesiredDigitalOutput[ResetButton].mValue = false;

    mDesiredServoOutput[Actuator].mChannel = Actuator;
    mDesiredServoOutput[Actuator].mValue = 0.0;

    mDesiredServoOutput[VoltageMotors].mChannel = VoltageMotors;
    mDesiredServoOutput[VoltageMotors].mValue = 0.0;

    mDesiredServoOutput[VoltageLogic].mChannel = VoltageLogic;
    mDesiredServoOutput[VoltageLogic].mValue = 0.0;

    mDesiredServoOutput[DynamixelActual].mChannel = DynamixelActual;
    mDesiredServoOutput[DynamixelActual].mValue = CxUtils::CxToRadians(120);

    mDesiredServoOutput[DynamixelDesired].mChannel = DynamixelDesired;
    mDesiredServoOutput[DynamixelDesired].mValue = CxUtils::CxToRadians(120);

    mDesiredServoOutput[RadioTriState].mChannel = RadioTriState;
    mDesiredServoOutput[RadioTriState].mValue = 1;

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller:: ~Microcontroller()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method is called when a JAUS Set Digital Out value is received
///          through a command message (like from an AI program).
///
///   Save values internally so they can be sent to actual device later.
///
///   \param[in] name Pin name
///   \param[in] value The value to set.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::SetDigitalOut(const std::string& name, const bool value)
{
    std::map<PortNumber,MCU::SetDigitalOutputs>::iterator it;
    PortNumber port = ToPort(name);
    mMutex.Lock();
    it = mDesiredDigitalOutput.find(port);
    if ( it == mDesiredDigitalOutput.end() )
    {
        std::cout << name << " is not a valid Digital output" << std::endl;
    }
    else
    {
        mDesiredDigitalOutput[port].mValue = value;
    }
    mMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called to when a set analog output command is received.
///
///   \param[in] name Pin name for anolog output.
///   \param[in] value The value to set.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::SetAnalogOut(const std::string& name, const double value)
{
    mMutex.Lock();
    PortNumber port = ToPort(name);
    std::map<PortNumber,MCU::SetServoOutputs>::iterator it;
    it = mDesiredServoOutput.find(port);
    if ( it == mDesiredServoOutput.end() )
    {
        std::cout << name << " is not a valid Servo output"<< std::endl;
    }
    else
    {
        mDesiredServoOutput[port].mValue = value;
    }
    mMutex.Unlock();
}


/** Sets the digital input value referencing an enum value. */
void Microcontroller::SetDigitalInput(const PortNumber& port, const bool value)
{
    JAUS::Microcontroller::SetDigitalInput(Microcontroller::ToString(port), value);
}


/** Sets the analog value for a pin/device [-100,100]% of max (JAUS command). */
void Microcontroller::SetAnalogInput(const PortNumber& port, const double value)
{
    std::string name = Microcontroller::ToString(port);
    JAUS::Microcontroller::SetAnalogInput(name, value);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the state of autonomous/teleop mode based on information
///          from mcu.
///   \return True if vehicle set to autonomous, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Microcontroller::IsAutonomousMode() const
{
    return GetDigitalState(ToString(Zebulon::ASVC::Microcontroller::Autonomous)) ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Message callback for data received from MCU interface.
///
///   \param[in] message Message data to process.  Usually contains pin values
///              for digital and analog.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::ProcessMessage(const MCU::Message* message)
{
    switch(message->mMessageCode)
    {
        case MCU::Message::Pong:
        {
            const MCU::Pong* pong = (const MCU::Pong*)message;
            pong->Print();
            break;
        }
        case MCU::Message::ReportDigitalInputs:
        {
            const MCU::ReportDigitalInputs* digital = dynamic_cast<const MCU::ReportDigitalInputs*>(message);
            SetDigitalInput((PortNumber)digital->mPort, digital->mValue);
            break;
        }
        case MCU::Message::ReportAnalogInputs:
        {
            const MCU::ReportAnalogInputs* analog = dynamic_cast<const MCU::ReportAnalogInputs*>(message);
            if(analog->mPort == VoltageMotors || analog->mPort == VoltageLogic)
            {
                SetAnalogInput((PortNumber)analog->mPort,(double)(analog->mValue)/1000.0);
            }
            else
            {
                SetAnalogInput((PortNumber)analog->mPort,analog->mValue);
            }
            break;
        }
        default:
        {
            std::cout << "Microcontroller::Error - Unsupported message received\n";
            break;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Call this method to get the MCU message to send for a specific servo
///          output.
///
///   \param[in] servoName The name of the analog output pin to get message for.
///
///   \return Message data to send.
///
////////////////////////////////////////////////////////////////////////////////////
MCU::SetServoOutputs Microcontroller::GetServoOutputs(const PortNumber& port) const
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    std::map<PortNumber,MCU::SetServoOutputs>::const_iterator m;
    m  = mDesiredServoOutput.find(port);
    if(m != mDesiredServoOutput.end())
    {
        return m->second;
    }
    return MCU::SetServoOutputs();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Call this method to get the MCU message to send for a specific digital
///          output.
///
///   \param[in] digitalName The name of the digital output pin to get message for.
///
///   \return Message data to send.
///
////////////////////////////////////////////////////////////////////////////////////
MCU::SetDigitalOutputs Microcontroller::GetDigitalOutputs(const PortNumber& port) const
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    std::map<PortNumber,MCU::SetDigitalOutputs>::const_iterator m;
    m  = mDesiredDigitalOutput.find(port);
    if(m != mDesiredDigitalOutput.end())
    {
        return m->second;
    }
    return MCU::SetDigitalOutputs();
}


/** Gets analog value using an enumeration instead of a string. */
double Microcontroller::GetAnalogOutputs(const PortNumber& portnum) const
{

    return GetAnalogState(ToString(portnum));
}

/*  End of File */
