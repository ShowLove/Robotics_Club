/////////////////////////////////////////////////////////////////////////////////////
///
/// \file microcontroller.cpp
/// \brief Micorcontroller service used by the Surface vehicle.
///
///  <br>Author(s): Brian Valentino<br>
///  <br>Created: 3/05/2010<br>
///  <br>Copyright (c) 2010<br>
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bmvalentino@gmail.com
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

using namespace Zebulon;
using namespace ASVC;

const std::string Microcontroller::PinNames::ComputerLightOne = "Computer Light One";
const std::string Microcontroller::PinNames::ComputerLightTwo = "Computer Light Two";
const std::string Microcontroller::PinNames::LateralRotate = "Rotate Lateral";
const std::string Microcontroller::PinNames::WaterCannon = "Water Cannon";
const std::string Microcontroller::PinNames::WaterCannonTilt = "Water Cannon Tilt";
const std::string Microcontroller::PinNames::StartButton = "Start Button";
const std::string Microcontroller::PinNames::TeleopControl = "Teleop Control";
const std::string Microcontroller::PinNames::EmergencyStop = "E-Stop";
const std::string Microcontroller::PinNames::LeftServo = "Left Servo";
const std::string Microcontroller::PinNames::RightServo = "Right Servo";
const std::string Microcontroller::PinNames::LateralServo = "Lateral Servo";
const std::string Microcontroller::PinNames::ArmServo = "Arm Servo";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller::Microcontroller()
{
    // Initialize digital outputs values here to defaults
    mDigitalOutputPins[PinNames::ComputerLightOne].mPort = PinNumbers::ComputerLightOne;
    mDigitalOutputPins[PinNames::ComputerLightOne].mValue = false;
    
    mDigitalOutputPins[PinNames::ComputerLightTwo].mPort = PinNumbers::ComputerLightTwo;
    mDigitalOutputPins[PinNames::ComputerLightTwo].mValue = false;

    mDigitalOutputPins[PinNames::LateralRotate].mPort = PinNumbers::LateralRotate;
    mDigitalOutputPins[PinNames::LateralRotate].mValue = false;
    
    mServoOutputPins[PinNames::WaterCannon].mChannel = PinNumbers::WaterCannon;
    mServoOutputPins[PinNames::WaterCannon].mValue = 0.0;

    mServoOutputPins[PinNames::WaterCannonTilt].mChannel = PinNumbers::WaterCannonTilt;
    mServoOutputPins[PinNames::WaterCannonTilt].mValue = 0.0;

    mServoOutputPins[PinNames::ArmServo].mChannel = PinNumbers::ArmServo;
    mServoOutputPins[PinNames::ArmServo].mValue = 255;
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
    std::map<std::string,MCU::SetDigitalOutputs>::iterator it;
    mMutex.Lock();
    it = mDigitalOutputPins.find(name);
    if ( it == mDigitalOutputPins.end() )
    {
        std::cout << name << " is not a valid Digital output";
    }
    else
    {
        mDigitalOutputPins[name].mValue = value;
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
    std::map<std::string,MCU::SetServoOutputs>::iterator it;
    it = mServoOutputPins.find(name);
    if ( it == mServoOutputPins.end() )
    {
        std::cout << name << " is not a valid Servo output\n";
    }
    else
    {
        if(name == PinNames::WaterCannonTilt)
        {
            double final = value*-1;
            final = (final + 100)/2.0;
            if(final > WaterCannonTiltMax)
            {
                final = WaterCannonTiltMax;
            }
            if(final < WaterCannonTiltMin)
            {
                final = WaterCannonTiltMin;
            }
            mServoOutputPins[name].mValue = (unsigned char)final;
        }
        else if(name == PinNames::WaterCannon)
        {
            double final = fabs(value);
            if(final >= 45)
                final = 45.0;

            mServoOutputPins[name].mValue = (unsigned char)(127 + ((final/100.0)*127));

            //mServoOutputPins[name].mValue = (unsigned char)(final*255.0/100.0);
        }
        else if(name == PinNames::ArmServo)
        {
            mServoOutputPins[name].mValue = (unsigned char)(fabs(value)*(0xE4)/100.0);
        }
        else
        {
            mServoOutputPins[name].mValue = (unsigned char)(127 + ((value/100.0)*127));
        }
    }
    mMutex.Unlock();
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
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    
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
            
            switch(digital->mPort)
            {
                case 0x00:
                    SetDigitalInput(PinNames::StartButton, !JAUS::BitVector::IsBitSet(digital->mValue, PinNumbers::StartButton));
                    SetDigitalInput(PinNames::TeleopControl, JAUS::BitVector::IsBitSet(digital->mValue, PinNumbers::TeleopControl));
                    SetDigitalInput(PinNames::EmergencyStop, JAUS::BitVector::IsBitSet(digital->mValue, PinNumbers::EmergencyStop));
                    /*
                    for(unsigned int i = 0; i < 8; i++)
                    {
                        std::cout << JAUS::BitVector::IsBitSet(digital->mValue, i) << " ";
                    }
                    std::cout << std::endl;
                    */
                    if(mDebugMessagesFlag)
                    {
                        JAUS::Mutex::ScopedLock lock(&mDebugMessagesMutex);
                        digital->Print();
                        std::cout << "Start Button: " << GetDigitalState(PinNames::StartButton) << std::endl;
                        std::cout << "Teleop State: " << GetDigitalState(PinNames::TeleopControl) << std::endl;
                        std::cout << "Estop State: " << GetDigitalState(PinNames::EmergencyStop) << std::endl;
                    }
                default:
                    //Invalid Port
                    break;
            }
            break;
        }
        case MCU::Message::ReportAnalogInputs:
        {
            const MCU::ReportAnalogInputs* analog = dynamic_cast<const MCU::ReportAnalogInputs*>(message);

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
MCU::SetServoOutputs Microcontroller::GetServoOutputs(const std::string& servoName) const
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    std::map<std::string,MCU::SetServoOutputs>::const_iterator m;
    m  = mServoOutputPins.find(servoName);
    if(m != mServoOutputPins.end())
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
MCU::SetDigitalOutputs Microcontroller::GetDigitalOutputs(const std::string& digitalName) const
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    std::map<std::string,MCU::SetDigitalOutputs>::const_iterator m;
    m  = mDigitalOutputPins.find(digitalName);
    if(m != mDigitalOutputPins.end())
    {
        return m->second;
    }
    return MCU::SetDigitalOutputs();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void Microcontroller::PrintStatus() const
{
    JAUS::Microcontroller::PrintStatus();
    mMutex.Lock();
    std::map<std::string,MCU::SetDigitalOutputs> digitalStates = mDigitalOutputPins;
    std::map<std::string,MCU::SetServoOutputs> analogStates = mServoOutputPins;
    mMutex.Unlock();
    std::map<std::string,MCU::SetDigitalOutputs>::const_iterator digital;
    for(digital = digitalStates.begin();
        digital != digitalStates.end();
        digital++)
    {
        std::cout << "Digital Out - " << digital->first << " - " << (int)digital->second.mValue << std::endl;
    }
    std::map<std::string,MCU::SetServoOutputs>::const_iterator analog;
    for(analog = analogStates.begin();
        analog != analogStates.end();
        analog++)
    {
        std::cout << "Analog Out - " << analog->first << " - " << (int)analog->second.mValue<< std::endl;
    }
}

/*  End of File */
