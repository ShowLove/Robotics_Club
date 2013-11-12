////////////////////////////////////////////////////////////////////////////////////
///
///  \file joystick.h
///  \brief Contains a ControlDevice service implementation.
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
#ifndef __JAUS_EXTRAS_CONTROLLERS_JOYSTICK__H
#define __JAUS_EXTRAS_CONTROLLERS_JOYSTICK__H

#include "jaus/extras/controllers/controldevice.h"
#include <cxutils/joystick.h>
#include <cxutils/time.h>

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Joystick
    ///   \brief Interface for using a joystick to control a Primitive Driver
    ///          service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL Joystick : public ControlDevice,
                                     public CxUtils::Joystick::Callback
    {
    public:
        // Constructor.
        Joystick();
        // Destructor.
        virtual ~Joystick();
        // Method to enable/disable connection to a joystick.
        void EnableJoystick(const bool enable = true) { mEnableJoystickFlag = enable; }
        // Method to check if joystick is enabled for service.
        bool IsJoystickEnabled() const { return mEnableJoystickFlag; }
        // Shutsdown the joystick.
        virtual void Shutdown() { ControlDevice::Shutdown(); mJoystick.Shutdown(); }
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename);
        // Process joystick event callback.
        virtual void ProcessJoystickEvent(const CxUtils::Joystick& jstick);
        // Process joystick event callback.
        virtual void ProcessButtonEvent(const unsigned int buttonNumber, const CxUtils::Joystick::Event type);
        // Method called periodically by Component or other Services, can be used for periodic udpdates.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Get point to physical joystick.
        const CxUtils::Joystick* GetJoystick() const { return &mJoystick; }
    protected:
        CxUtils::Joystick mJoystick;        ///< Interface to the joystick.
        unsigned int mID;                   ///< Joystick ID to connect to.
        unsigned int mFrequency;            ///< Desired joystick update frequency.
        std::string mCalibrationFile;       ///< Joystick calibration file data.
        volatile bool mEnableJoystickFlag;  ///< If true, connect to joystick (default).
    };
}

#endif
/* End of File */
