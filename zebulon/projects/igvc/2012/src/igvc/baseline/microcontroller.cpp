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
#include "igvc/baseline/microcontroller.h"
//#include <bitvector/bitvector.h>

using namespace Zebulon;
using namespace IGVC;

const std::string Microcontroller::PinNames::TeleopControl = "Teleop Control";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Microcontroller::Microcontroller()
{
    // Initialize digital outputs values here to defaults
    /*
    mDigitalOutputPins[PinNames::GreenLight].mPort = PinNumbers::GreenLight;
    mDigitalOutputPins[PinNames::GreenLight].mValue = false;

    mDigitalOutputPins[PinNames::YellowLight].mPort = PinNumbers::YellowLight;
    mDigitalOutputPins[PinNames::YellowLight].mValue = false;

    mDigitalOutputPins[PinNames::EmergencyStop].mPort = PinNumbers::EmergencyStop;
    mDigitalOutputPins[PinNames::EmergencyStop].mValue = false;

    mServoOutputPins[PinNames::WaterCannon].mChannel = PinNumbers::WaterCannon;
    mServoOutputPins[PinNames::WaterCannon].mValue = 0.0;
    */
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
        mServoOutputPins[name].mValue = (unsigned char)(127 + ((value/100.0)*127));
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
                    SetDigitalInput(PinNames::TeleopControl, JAUS::BitVector::IsBitSet(digital->mValue, PinNumbers::TeleopControl));

                    if(mDebugMessagesFlag)
                    {
                        //JAUS::Mutex::ScopedLock lock(&mDebugMessagesMutex);
                        JAUS::ReadLock rLock(mDebugMessagesMutex);
                        digital->Print();
                        std::cout << "Teleop State: " << GetDigitalState(PinNames::TeleopControl) << std::endl;
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

/*  End of File */
