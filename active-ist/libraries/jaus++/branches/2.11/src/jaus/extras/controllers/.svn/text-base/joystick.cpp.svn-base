////////////////////////////////////////////////////////////////////////////////////
///
///  \file joystick.cpp
///  \brief Contains a Joystick service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 3 February 2010
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
#include "jaus/extras/controllers/joystick.h"
#include <cmath>
#include <tinyxml/tinyxml.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default value.
///
////////////////////////////////////////////////////////////////////////////////////
Joystick::Joystick()
{
    mID = CxUtils::Joystick::AnyJoystick;
    mFrequency = 25;
    mCalibrationFile = "/home/developer/logitech_extreme_3d";
    mEnableJoystickFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Joystick::~Joystick()
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
bool Joystick::LoadSettings(const std::string& filename)
{
    if(ControlDevice::LoadSettings(filename))
    {
        TiXmlDocument xml;

        if(xml.LoadFile(filename.c_str()) == false)
        {
            return false;
        }
        TiXmlHandle doc(&xml);
        TiXmlNode* node;

        node = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Joystick").FirstChild("ID").ToNode();
        if(node && node->FirstChild())
        {
            int id = atoi(node->FirstChild()->Value());
            if(id < 0)
            {
                mID = CxUtils::Joystick::AnyJoystick;
            }
            else
            {
                mID = (unsigned int)id;
            }
            //mID = (unsigned int)atoi(node->FirstChild()->Value());
        }
        node = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Joystick").FirstChild("Frequency").ToNode();
        if(node && node->FirstChild())
        {
            mFrequency = (unsigned int)atoi(node->FirstChild()->Value());
        }
        node = doc.FirstChild("JAUS").FirstChild("ControlDevice").FirstChild("Joystick").FirstChild("CalibrationFile").ToNode();
        if(node && node->FirstChild())
        {
            mCalibrationFile = node->FirstChild()->Value();
        }

        if(mJoystick.GetID() != mID)
        {
            mJoystick.Shutdown();
        }

        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Process joystick update and generates a wrench effort.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::ProcessJoystickEvent(const CxUtils::Joystick& jstick)
{
    // Clear the current wrench effort data, so we can
    // populate a new message.
    ClearWrenchEffort();
    // Use methods to populate Wrench Command so that it
    // correctly remaps axis/joystick data to wrenches automatically.
    for(int i = 0; i < CxUtils::Joystick::MaxAxis; i++)
    {
        UpdateWrench(i, jstick.GetAxisPercentage((short)i, 0));
        UpdateAnalogOut(i, jstick.GetAxisPercentage((short)i, 0));
    }

    if(mEnableJoystickFlag == false)
    {
        return;
    }
    // Send the wrench effort.
    SendWrenchEffort();
    // Send any MCU command
    SendMicrocontrollerState();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Process button press events.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::ProcessButtonEvent(const unsigned int buttonNumber, const CxUtils::Joystick::Event type)
{
    if(mEnableJoystickFlag == false)
    {
        return;
    }
    if(type == CxUtils::Joystick::ButtonDown)
    {
        SignalButtonPress(buttonNumber);
    }
    else if(type == CxUtils::Joystick::ButtonUp)
    {
        SignalButtonRelease(buttonNumber);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called periodicly by the Component the service belongs to.  This
///   method attempts to lookup a primitive driver on the subsystem we are
///   trying to control.
///
////////////////////////////////////////////////////////////////////////////////////
void Joystick::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    static Time::Stamp registerTime = 0;
    ControlDevice::CheckServiceStatus(timeSinceLastCheckMs);
    if(mJoystick.IsInitialized() && !mJoystick.IsConnected())
    {
        mJoystick.Shutdown();
        TakeDriveControl(false);
    }
    if(mEnableJoystickFlag && mJoystick.IsConnected() == false)
    {
        if(mJoystick.Initialize(mID, mFrequency, mCalibrationFile) == 0)
        {
            mJoystick.Shutdown();
        }
    }
    if(Time::GetUtcTimeMs() - registerTime >= 500)
    {
        mJoystick.RegisterCallback(this);
        registerTime = Time::GetUtcTimeMs();
    }
}


/*  End of File */
