////////////////////////////////////////////////////////////////////////////////////
///
///  \file keyboard.h
///  \brief Contains a ControlDevice service implementation with a keyboard
///         as input.
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
#ifndef __JAUS_EXTRAS_CONTROLLERS_KEYBOARD__H
#define __JAUS_EXTRAS_CONTROLLERS_KEYBOARD__H

#include "jaus/extras/controllers/controldevice.h"
#include <cxutils/keyboard.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Keyboard
    ///   \brief Interface for using a keyboard to control a Primitive Driver
    ///          service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL Keyboard : public ControlDevice
    {
    public:
        // Constructor.
        Keyboard();
        // Destructor.
        virtual ~Keyboard();
        // Shutsdown the ControlDevice.
        virtual void Shutdown() { ControlDevice::Shutdown();}
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename);
        // Method called periodically by Component or other Services, can be used for periodic udpdates.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
    protected:
        CxUtils::Keyboard mKeyboard;        ///< Interface to the keyboard.
    };
}

#endif //__JAUS_EXTRAS_CONTROLLERS_KEYBOARD__H
/* End of File */
