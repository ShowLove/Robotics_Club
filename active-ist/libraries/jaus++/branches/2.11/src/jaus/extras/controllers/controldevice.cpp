////////////////////////////////////////////////////////////////////////////////////
///
///  \file controldevice.cpp
///  \brief Contains the Control Device service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 1 February 2010
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
#include "jaus/extras/controllers/controldevice.h"
#include "jaus/mobility/drivers/primitivedriver.h"
#include "jaus/extras/mcu/microcontroller.h"
#include "jaus/extras/mcu/setmicrocontrollerstate.h"
#include "jaus/extras/controllers/keyboard.h"
#include <cmath>
#include <tinyxml/tinyxml.h>
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <algorithm>

using namespace JAUS;

const std::string ControlDevice::Name = "urn:jaus:jss:jpp:extras:ControlDevice";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default value.
///
////////////////////////////////////////////////////////////////////////////////////
ControlDevice::ControlDevice() : Management::Child(Service::ID(ControlDevice::Name), 
                                                   Service::ID(Management::Name))
{
    mTakeDriveControlFlag = false;
    mAutoBrakingFlag = true;
    mSubsystemID = 0;
    // Initialize limits.
    for(unsigned int i = 0; i < 11; i++)
    {
        mLimitsMapping[(WrenchEffort)i] = 100.0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ControlDevice::~ControlDevice()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads settings for the service from an XML file.
///
///   \param[in] filename Name of the XML file containing data.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ControlDevice::LoadSettings(const std::string& filename)
{
    TiXmlDocument xml;

    if(xml.LoadFile(filename.c_str()) == false)
    {
        return false;
    }

    // Clear all mappings.
    mControlDevice.Lock();
    mWrenchMapping.clear();
    mAnalogMapping.clear();
    mButtonDigitalMapping.clear();
    mDeadzoneMapping.clear();
    mInvertMapping.clear();
    mButtonMapping.clear();
    mLimitsMapping.clear();
    mControlDevice.Unlock();

    TiXmlHandle doc(&xml);
    TiXmlNode* node;
    TiXmlElement* element;
    element = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Wrenches").FirstChild("WrenchEffort").ToElement();
    while(element)
    {
        if(element->Attribute("wrench") && (element->Attribute("input") || element->Attribute("key")))
        {
            WrenchEffort wrench;
            int input;
            double deadzone = 0.0;
            double limit = 100.0;
            bool invert = false;
            wrench = (WrenchEffort)atoi(element->Attribute("wrench"));
            if(element->Attribute("input"))
            {
                input = atoi(element->Attribute("input"));
            }
            if(element->Attribute("key"))
            {
                CxUtils::Keyboard::Key key = CxUtils::Keyboard::FromString(element->Attribute("key"));
                input = (int)key;
                if(key == CxUtils::Keyboard::Any || key == CxUtils::Keyboard::Invalid)
                {
                    element = element->NextSiblingElement();
                    continue;
                }
            }
            if(element->Attribute("deadzone"))
            {
                deadzone = atof(element->Attribute("deadzone"));
            }
            if(element->Attribute("limit"))
            {
                limit = atof(element->Attribute("limit"));
            }
            if(element->Attribute("invert"))
            {
                std::string value = element->Attribute("invert");
                if(value == "true" || value == "false")
                {
                    if(value == "true")
                    {
                        invert = true;
                    }
                }
                else
                {
                    invert = atoi(value.c_str()) > 0 ? true : false;
                }
            }
            MapInputToWrench(input, wrench, deadzone, invert, limit);
        }
        element = element->NextSiblingElement();
    }

    element = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Microcontroller").FirstChild("AnalogOut").ToElement();
    while(element)
    {
        if(element->Attribute("name") && element->Attribute("input"))
        {
            std::string name;
            int input;
            double limit = 100.0;
            bool invert = false;
            name = element->Attribute("name");
           
            input = atoi(element->Attribute("input"));

            if(element->Attribute("limit"))
            {
                limit = atof(element->Attribute("limit"));
            }
            if(element->Attribute("invert"))
            {
                std::string value = element->Attribute("invert");
                if(value == "true" || value == "false")
                {
                    if(value == "true")
                    {
                        invert = true;
                    }
                }
                else
                {
                    invert = atoi(value.c_str()) > 0 ? true : false;
                }
            }
            MapInputToAnalogOut(input, name, invert, limit);
        }
        element = element->NextSiblingElement("AnalogOut");
    }

    element = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Microcontroller").FirstChild("DigitalOut").ToElement();
    while(element)
    {
        if(element->Attribute("name") && element->Attribute("button"))
        {
            std::string name;
            int button;
            bool onPress = true;
            name = element->Attribute("name");
            button = atoi(element->Attribute("button"));
            if(element->Attribute("press"))
            {
                std::string value = element->Attribute("press");
                if(value == "1" || value == "true")
                {
                    onPress = 1;
                }
                else
                {
                    onPress = 0;
                }
            }
            MapButtonToDigitalOut(button, name, onPress);
        }
        element = element->NextSiblingElement("AnalogOut");
    }
     
    node = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Buttons").FirstChild("RequestDriveControlButton").ToNode();
    if(node && node->FirstChild())
    {
        int bvalue = 0;
        if(dynamic_cast<JAUS::Keyboard*>(this))
        {
            bvalue = (int)CxUtils::Keyboard::FromString(node->FirstChild()->Value());
            if(bvalue <= 0)
            {
                bvalue = atoi(node->FirstChild()->Value());
            }
        }
        else
        {
            bvalue = atoi(node->FirstChild()->Value());
        }
        MapButtonToAction(bvalue, RequestDriveControl);
    }
    node = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Buttons").FirstChild("ReleaseDriveControlButton").ToNode();
    if(node && node->FirstChild())
    {
        int bvalue = 0;
        if(dynamic_cast<JAUS::Keyboard*>(this))
        {
            bvalue = (int)CxUtils::Keyboard::FromString(node->FirstChild()->Value());
            if(bvalue <= 0)
            {
                bvalue = atoi(node->FirstChild()->Value());
            }
        }
        else
        {
            bvalue = atoi(node->FirstChild()->Value());
        }
        MapButtonToAction(bvalue, ReleaseDriveControl);
    }
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads settings for the service from an XML file.
///
///   \param[in] vehicleIdentificationName Name of robot to load joystick settings
///                                        for.
///   \param[in] directory Name of the directory containing joystick files.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ControlDevice::LoadSettingsForVehicle(const std::string& vehicleIdentificationName,
                                           const std::string& directory)
{
    bool result = false;
    if(vehicleIdentificationName == "")
    {
        return result;
    }
    std::vector<std::string> filenames;
    if(CxUtils::FileIO::GetFiles(filenames, "*.xml", directory, false, true) > 0)
    {
        std::string ident = vehicleIdentificationName;
        std::transform(ident.begin(), ident.end(), ident.begin(), tolower);
        for(unsigned int i = 0; i < (unsigned int)filenames.size(); i++)
        {
            std::string copy = filenames[i];
            std::transform(copy.begin(), copy.end(), copy.begin(), tolower);
            if(strstr(copy.c_str(), ident.c_str()) > 0)
            {
                result = this->LoadSettings(filenames[i]);
                break;
            }
        }
        if(result == false)
        {
            for(unsigned int i = 0; i < (unsigned int)filenames.size(); i++)
            {
                std::string copy = filenames[i];
                std::transform(copy.begin(), copy.end(), copy.begin(), tolower);
                if(strstr(copy.c_str(), "default") > 0)
                {
                    result = this->LoadSettings(filenames[i]);
                    break;
                }
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the service.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::Shutdown()
{
    TakeDriveControl(false);
    Mutex::ScopedLock lock(&mControlDevice);
    if(mPrimitiveDriverID.IsValid())
    {
        if(GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
        {
            ReleaseComponentControl(mPrimitiveDriverID, true);
        }
    }

    if(mMicrocontrollerID.IsValid())
    {
        if(GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
        {
            ReleaseComponentControl(mMicrocontrollerID, true);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the ID of the subsystem to control with the control device.
///
///   \param[in] id ID number of the subsystem.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::SetSubsystemToControl(const UShort id)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mSubsystemID = id;
    if(mPrimitiveDriverID.IsValid())
    {
        // Release control if we were controlling a component.
        if(mPrimitiveDriverID.mSubsystem != mSubsystemID)
        {
            if(mTakeDriveControlFlag == true &&
               GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
            {
                ReleaseComponentControl(mPrimitiveDriverID, true);
            }
            mPrimitiveDriverID.Clear();
        }
    }
    if(mMicrocontrollerID.IsValid())
    {
        // Release control if we were controlling a component.
        if(mMicrocontrollerID.mSubsystem != mSubsystemID)
        {
            if(mTakeDriveControlFlag == true &&
               GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
            {
                ReleaseComponentControl(mMicrocontrollerID, true);
            }
            mMicrocontrollerID.Clear();
        }
    }
    // Make sure the Discovery service is enabled.
    if(GetComponent()->DiscoveryService()->IsEnabled() == false)
    {
        GetComponent()->DiscoveryService()->EnableService(true);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Takes or releases control of the Primitive Driver service on the
///          subsystem being controlled.
///
///   \param[in] enable If true, take control, false to release.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::TakeDriveControl(const bool enable)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mTakeDriveControlFlag = enable;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if Control Device has control of a Primitive Driver.
///
///   \return True if control of a Primitive Driver has been established.
///
////////////////////////////////////////////////////////////////////////////////////
bool ControlDevice::HaveDriveControl() const
{
    Mutex::ScopedLock lock(&mControlDevice);
    if(mPrimitiveDriverID.IsValid() && 
       GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
    {
        return true;
    }
    return false;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to call when a button is pressed.  Method will convert 
///          button number to a ButtonActions command based on button mappings.
///
///   For digital out mappings, a button press turns a mapped digital out to
///   true.  For example, on button press, turn trigger digital out to 1.  For
///   reset to 0, see SignalButtonRelease.
///
///   \param[in] buttonNumber ID/Number of the button that was pressed.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::SignalButtonPress(const int buttonNumber)
{
    Mutex::ScopedLock lock(&mControlDevice);
    std::map<int, ButtonActions>::iterator b;
    b = mButtonMapping.find(buttonNumber);
    if(b != mButtonMapping.end())
    {
        switch(b->second)
        {
        case RequestDriveControl:
            mTakeDriveControlFlag = true;
            break;
        case ReleaseDriveControl:
            mTakeDriveControlFlag = false;
            if(mPrimitiveDriverID.IsValid())
            {      
                ReleaseComponentControl(mPrimitiveDriverID, true);
            }
            if(mMicrocontrollerID.IsValid())
            {              
                ReleaseComponentControl(mMicrocontrollerID, true);
            }
            break;
        default:
            break;
        }
    }

    std::map<int, std::string>::iterator digital;
    digital = mButtonDigitalMapping.find(buttonNumber);
    if(mMicrocontrollerID.IsValid() && 
       digital != mButtonDigitalMapping.end())
    {
        SetMicrocontrollerState command(mMicrocontrollerID, GetComponentID());
        (*command.GetDigitalStates())[digital->second] = mDigitalButtonState[digital->second];
        Send(&command);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to call when a button is released.  Method will convert 
///          button number to a ButtonActions command based on button mappings.
///
///   For digital out mappings, a button press turns a mapped digital out to
///   false.  For example, on button release, turn trigger digital out to 0.  For
///   reset to 1, see SignalButtonPress.
///
///   \param[in] buttonNumber ID/Number of the button that was released.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::SignalButtonRelease(const int buttonNumber)
{
    Mutex::ScopedLock lock(&mControlDevice);
    std::map<int, ButtonActions>::iterator b;
    b = mButtonMapping.find(buttonNumber);
    if(b != mButtonMapping.end())
    {
        // Currently no off button mappings.
    }

    std::map<int, std::string>::iterator digital;
    digital = mButtonDigitalMapping.begin();
    if(mMicrocontrollerID.IsValid() && 
       digital != mButtonDigitalMapping.end())
    {
        SetMicrocontrollerState command(mMicrocontrollerID, GetComponentID());
        (*command.GetDigitalStates())[digital->second] = !mDigitalButtonState[digital->second];
        Send(&command);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the value of a Wrench for the Set Wrench Effort command
///          being sent to the controlled Primitive Driver.
///
///   Based on input mappings to wrenches, the inputID is converted to a 
///   specfice Wrench Effort (e.g. 1 maps to PropulsiveLinearEffortX).
///
///   This method will also automatically apply deadzones, inversion, and
///   scaling to the percent effort being set.
///
///   \param[in] inputID The ID of the device input axis (e.g. joystick x or y
///                      axis).
///   \param[in] percentEffort Percent effort value to send [-100,100]%.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::UpdateWrench(const int inputID, const double percentEffort)
{
    Mutex::ScopedLock lock(&mControlDevice);
    std::map<int, WrenchEffort>::iterator w;
    w = mWrenchMapping.find(inputID);
    if(w != mWrenchMapping.end())
    {
        double value = 0.0;
        std::map<int, double>::iterator scale = mLimitsMapping.find(inputID);        
        std::map<int, double>::iterator deadzone = mDeadzoneMapping.find(inputID);

        // Apply deadzone filter.
        if(deadzone != mDeadzoneMapping.end())
        {
            if(fabs(percentEffort) > deadzone->second)
            {
                value = (fabs(percentEffort) - deadzone->second)*100.0/(100.0 - deadzone->second);
                if(percentEffort < 0)
                {
                    value *= -1;
                }
            }
            else
            {
                value = 0.0;
            }
        }
        
        // Apply scaling.
        value = value*scale->second/100.0;

        // Apply invert filter
        std::map<int, bool>::iterator invert = mInvertMapping.find(inputID);
        if(invert != mInvertMapping.end())
        {
            if(invert->second)
            {
                value *= -1;
            }
        }
       
        // Set wrench value.
        switch(w->second)
        {
        case PropulsiveLinearEffortX:
            mWrenchEffort.SetPropulsiveLinearEffortX(value);
            if(fabs(value) < 0.001 && mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortX(100.0);
            }
            else if(mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortX(0.0);
            }
            break;
        case PropulsiveLinearEffortY:
            mWrenchEffort.SetPropulsiveLinearEffortY(value);
            if(fabs(value) < 0.001 && mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortY(100.0);
            }
            else if(mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortY(0.0);
            }
            break;
        case PropulsiveLinearEffortZ:
            mWrenchEffort.SetPropulsiveLinearEffortZ(value);
            if(fabs(value) < 0.001 && mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortZ(100.0);
            }
            else if(mAutoBrakingFlag)
            {
                mWrenchEffort.SetResistiveLinearEffortZ(0.0);
            }
            break;
        case PropulsiveRotationalEffortX:
            mWrenchEffort.SetPropulsiveRotationalEffortX(value);
            break;
        case PropulsiveRotationalEffortY:
            mWrenchEffort.SetPropulsiveRotationalEffortY(value);
            break;
        case PropulsiveRotationalEffortZ:
            mWrenchEffort.SetPropulsiveRotationalEffortZ(value);
            break;
        case ResistiveLinearEffortX:
            mWrenchEffort.SetResistiveLinearEffortX(value);
            break;
        case ResistiveLinearEffortY:
            mWrenchEffort.SetResistiveLinearEffortY(value);
            break;
        case ResistiveLinearEffortZ:
            mWrenchEffort.SetResistiveLinearEffortZ(value);
            break;
        case ResistiveRotationalEffortX:
            mWrenchEffort.SetResistiveRotationalEffortX(value);
            break;
        case ResistiveRotationalEffortY:
            mWrenchEffort.SetResistiveRotationalEffortY(value);
            break;
        case ResistiveRotationalEffortZ:
            mWrenchEffort.SetResistiveRotationalEffortZ(value);
            break;
        default:
            break;
        }              
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the value of a Set Microcontroller State message to
///          transmit to any Microcontroller Service of the vehicle we are
///          controlling.
///
///   \param[in] inputID The ID of the device input axis (e.g. joystick x or y
///                      axis).
///   \param[in] signalValue Signal value to send [-100,100]%.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::UpdateAnalogOut(const int inputID, const double signalValue)
{
    Mutex::ScopedLock lock(&mControlDevice);
    std::map<int, std::string>::iterator a;
    a = mAnalogMapping.find(inputID);
    if(a != mAnalogMapping.end())
    {
        double value = 0.0;
        std::map<std::string, double>::iterator scale = mAnalogLimitsMapping.find(a->second);        
        
        // Apply scaling.
        value = signalValue*scale->second/100.0;

        // Apply invert filter
        std::map<std::string, bool>::iterator invert = mAnalogInvertMapping.find(a->second);
        if(invert != mAnalogInvertMapping.end())
        {
            if(invert->second)
            {
                value *= -1;
            }
        }
       
        (*mMicrocontrollerState.GetAnalogStates())[a->second] = value;            
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the current wrench effort built using the UpdateWrench
///          method to the Primitive Driver service being controlled.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::SendWrenchEffort()
{
    Mutex::ScopedLock lock(&mControlDevice);
    if(mPrimitiveDriverID.IsValid() && mTakeDriveControlFlag)
    {
        // Request control.
        if(!GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
        {
            GetComponent()->AccessControlService()->RequestComponentControl(mPrimitiveDriverID, true);
        }
        if(mPrimitiveDriverID.IsValid() &&
           GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
        {
            mWrenchEffort.SetSourceID(GetComponentID());
            mWrenchEffort.SetDestinationID(mPrimitiveDriverID);
            Send(&mWrenchEffort);            
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends the current wrench effort built using the UpdateWrench
///          method to the Primitive Driver service being controlled.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::SendMicrocontrollerState()
{
    Mutex::ScopedLock lock(&mControlDevice);
    if(mMicrocontrollerID.IsValid() && mTakeDriveControlFlag)
    {
        // Request control.
        if(!GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
        {
            GetComponent()->AccessControlService()->RequestComponentControl(mMicrocontrollerID, true);
        }
        if(mMicrocontrollerID.IsValid() &&
           GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
        {
            mMicrocontrollerState.SetSourceID(GetComponentID());
            mMicrocontrollerState.SetDestinationID(mMicrocontrollerID);
            Send(&mMicrocontrollerState);            
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears the current wrench effort.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::ClearWrenchEffort()
{
    Mutex::ScopedLock lock(&mControlDevice);
    mWrenchEffort.ClearMessage();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears the current Set Microcontroller State message.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::ClearMicrocontrollState()
{
    Mutex::ScopedLock lock(&mControlDevice);
    mMicrocontrollerState.ClearMessage();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to map a button ID/number to an button action.
///
///   \param[in] buttonNumber ID/Number of the button to map.
///   \param[in] action The action to map the button to (e.g. TakeDriveControl).
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::MapButtonToAction(const int buttonNumber,
                                      const ButtonActions action)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mButtonMapping[buttonNumber] = action;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to map a button ID/number to a digital state output.
///
///   \param[in] buttonNumber ID/Number of the button to map.
///   \param[in] digital Digital device to toggle on/off.
///   \param[in] onPress Value to send when button is pressed, opposit value
///                      is sent on release.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::MapButtonToDigitalOut(const int buttonNumber,
                                          const std::string& digital,
                                          const bool onPress)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mButtonDigitalMapping[buttonNumber] = digital;
    mDigitalButtonState[digital] = onPress;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to map an control device input ID to a wrench effort ID value.
///
///   An example could be mapping a joystick axis (e.g. X axis = 1) to a wrench
///   effort (e.g. PropulsiveLinearEffortX).
///
///   For example, if you set the limit to 50, and you try to sent a value of
///   100% for the matching wrench, the resulting wrench will be 100% of the limit
///   (e.g. desired*limit/100 = 100*50/100 = 50%).
///
///   \param[in] inputID ID/Number of the input axis/lever, etc.
///   \param[in] wrenchID The wrench to map to.
///   \param[in] deadzone If a desired value is within the deazone range (e.g. 10%)
///                       wrench will be set to 0. Values are scaled/offset by
///                       deadzones.
///   \param[in] invert If true, input value are inverted for this wrench.
///   \param[in] limit The maximum value (positive and negative) that can be
///                    set for a wrench. [1-100).
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::MapInputToWrench(const int inputID,
                                     const WrenchEffort wrenchID,
                                     const double deadzone,
                                     const bool invert,
                                     const double limit)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mWrenchMapping[inputID] = wrenchID;
    if(fabs(deadzone) < 100)
    {
        mDeadzoneMapping[inputID] = fabs(deadzone);
    }
    mInvertMapping[inputID] = invert;
    if(fabs(limit) > 0 && fabs(limit) <= 100.0)
    {
        mLimitsMapping[inputID] = fabs(limit);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to map an control device input ID to analog out.
///
///   \param[in] inputID ID/Number of the input axis/lever, etc.
///   \param[in] analog Name of analog out to map to.
///   \param[in] invert If true, input value are inverted for this wrench.
///   \param[in] limit The maximum value (positive and negative) that can be
///                    set for an analog signal. [1-100).
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::MapInputToAnalogOut(const int inputID,
                                        const std::string& analog,
                                        const bool invert,
                                        const double limit)
{
    Mutex::ScopedLock lock(&mControlDevice);
    mAnalogMapping[inputID] = analog;
    mAnalogInvertMapping[analog] = invert;
    if(fabs(limit) > 0 && fabs(limit) <= 100.0)
    {
        mAnalogLimitsMapping[analog] = fabs(limit);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called periodicly by the Component the service belongs to.  This
///   method attempts to lookup a primitive driver on the subsystem we are
///   trying to control.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    // See if we need to take control of a component.
    Mutex::ScopedLock lock(&mControlDevice);
    if(mTakeDriveControlFlag && mSubsystemID != 0)
    {
        if(mPrimitiveDriverID.IsValid() == false)
        {
            // Lookup a primitive driver on the subsystem.
            GetComponent()->DiscoveryService()->GetSubsystem(mSubsystemID)->HaveService(PrimitiveDriver::Name, 
                                                                                        &mPrimitiveDriverID);
        }
        if(mMicrocontrollerID.IsValid() == false)
        {
            // Lookup a primitive driver on the subsystem.
            GetComponent()->DiscoveryService()->GetSubsystem(mSubsystemID)->HaveService(Microcontroller::Name, 
                                                                                        &mMicrocontrollerID);
        }
    }    
    if(mTakeDriveControlFlag == false)
    {
        if(mPrimitiveDriverID.IsValid() && GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
        {
            ReleaseComponentControl(mPrimitiveDriverID, true);
        }
        if(mMicrocontrollerID.IsValid() && GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
        {
            ReleaseComponentControl(mMicrocontrollerID, true);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called whenever our component is able to acquire
///          control of a component. 
///
///   For a ControlDevice, if the ID matches that of the Primitive Driver 
///   component we are trying to drive, then this method sends a Resume
///   command to it.
///
///   \param[in] controlledComponent The component we took control of.
///
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::ProcessAcquisitionOfControl(const Address& controlledComponent)
{
    Mutex::ScopedLock lock(&mControlDevice);
    // If we just too control, or re-acquired control of a vehicle
    // we are operating, we must put it in a Resume/Ready state.
    if(mTakeDriveControlFlag && controlledComponent == mPrimitiveDriverID)
    {
        GetComponent()->ManagementService()->Resume(mPrimitiveDriverID);
    }
    if(mTakeDriveControlFlag && controlledComponent == mMicrocontrollerID)
    {
        GetComponent()->ManagementService()->Resume(mMicrocontrollerID);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes messages received by the component that are supported
///          by this service.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::Receive(const Message* message)
{
    // Currently service doesn't receive any messages.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Given a message code, the Service attempts to create a Message object
///          that can be used for de-serialization of JAUS packets.
///
///   \param[in] messageCode JAUS Message to try create.
///
///   \return Pointer to created message, NULL if message not supported by
///           any Services attached to the Transport.
///
////////////////////////////////////////////////////////////////////////////////////
Message* ControlDevice::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    // Currently service doesn't receive any messages.
    //switch(messageCode)
    //{
    //default:
    //    message = NULL;
    //    break;
    //};
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints service status.
///
////////////////////////////////////////////////////////////////////////////////////
void ControlDevice::PrintStatus() const
{
    Mutex::ScopedLock lock(&mControlDevice);
    if(mPrimitiveDriverID.IsValid() && GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
    {
        std::cout << "[Control Device] - Controlling Primitive Driver [" << mPrimitiveDriverID.ToString() << "]\n";
        std::cout << "[Control Device] - Current Control Wrench:\n";
        mWrenchEffort.PrintMessageBody();
    }
    
    
    if(mMicrocontrollerID.IsValid() && GetComponent()->AccessControlService()->HaveControl(mMicrocontrollerID))
    {
        std::cout << "[Control Device] - Controlling Microcontroller [" << mMicrocontrollerID.ToString() << "]\n";
        std::cout << "[Control Device] - Current Control MCU States:\n";
        mMicrocontrollerState.PrintMessageBody();
    }
}

/*  End of File */
