////////////////////////////////////////////////////////////////////////////////////
///
///  \file keyboard.cpp
///  \brief Contains a Keyboard service implementation.
///
///  <br>Author(s): Jonathan Mohlenhoff
///  Created: 04 November 2010
///  Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: jmohlenh@ist.ucf.edu
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
#include "jaus/extras/controllers/keyboard.h"
#include <cmath>
#include <tinyxml/tinyxml.h>

using namespace JAUS;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default value.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::Keyboard()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::~Keyboard()
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
bool Keyboard::LoadSettings(const std::string& filename)
{
    return ControlDevice::LoadSettings(filename);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called periodicly by the Component the service belongs to.  This
///   method attempts to lookup a primitive driver on the subsystem we are
///   trying to control.
///
////////////////////////////////////////////////////////////////////////////////////
void Keyboard::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    ControlDevice::CheckServiceStatus(timeSinceLastCheckMs);
    ClearWrenchEffort();
    
    //Reset all possible input wrench efforts to zero, so if buttons have
    // been released the wrench effort message will have presence vectors set
    // and a thrust = 0.
    std::map<int, WrenchEffort> wrenchMap = GetInputIdToWrenchMapping();
    for(std::map<int, WrenchEffort>::iterator it = wrenchMap.begin();
        it != wrenchMap.end();
        it++)
    {
        UpdateWrench(it->first, 0.0);
    }

    CxUtils::Keyboard::KeyList keys = CxUtils::Keyboard::GetPressedKeys();
    for(CxUtils::Keyboard::KeyList::iterator k = keys.begin(); k != keys.end(); k++)
    {
        SignalButtonPress((int)(*k));
        UpdateWrench((int)(*k), 100.0);
    }

    SendWrenchEffort();
    // Send any MCU command
    SendMicrocontrollerState();
}


/*  End of File */
