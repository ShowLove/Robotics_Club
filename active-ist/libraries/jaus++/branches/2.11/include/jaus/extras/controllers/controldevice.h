////////////////////////////////////////////////////////////////////////////////////
///
///  \file controldevice.h
///  \brief Contains the ControlDevice service implementation.
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
#ifndef __JAUS_EXTRAS_CONTROLLERS_CONTROL_DEVICE__H
#define __JAUS_EXTRAS_CONTROLLERS_CONTROL_DEVICE__H

#include "jaus/extras/jausextrasdll.h"
#include "jaus/extras/mcu/setmicrocontrollerstate.h"
#include "jaus/core/management/management.h"
#include "jaus/mobility/drivers/primitivedriver.h"
#include "jaus/mobility/drivers/setwrencheffort.h"

namespace JAUS
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ControlDevice
    ///   \brief Interface for creating controller interfaces (e.g. Joystick, Wiimote)
    ///          for controlling a Primitive Driver service of an Unmanned System.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL ControlDevice : public Management::Child
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Enumeration of possible button mappings.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum ButtonActions
        {
            RequestDriveControl = 0, ///<  Request drive control.
            ReleaseDriveControl,     ///<  Release drive control.
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Enumeration of different wrench efforts the data can
        ///          be mapped to.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum WrenchEffort
        {
            PropulsiveLinearEffortX = 0,
            PropulsiveLinearEffortY,
            PropulsiveLinearEffortZ,
            PropulsiveRotationalEffortX,
            PropulsiveRotationalEffortY,
            PropulsiveRotationalEffortZ,
            ResistiveLinearEffortX,
            ResistiveLinearEffortY,
            ResistiveLinearEffortZ,
            ResistiveRotationalEffortX,
            ResistiveRotationalEffortY,
            ResistiveRotationalEffortZ,
        };
        const static std::string Name; ///<  String name of the Service.
        // Constructor.
        ControlDevice();
        // Destructor.
        virtual ~ControlDevice();
        // Load settings from a file.
        virtual bool LoadSettings(const std::string& filename);
        // Loads settings for a specific type of robot to control
        virtual bool LoadSettingsForVehicle(const std::string& vehicleIdentificationName,
                                            const std::string& directory);
        // Shutsdown the Service.
        virtual void Shutdown() ;
        // Sets the ID of the subsystem to take control of (when control is to be taken).
        void SetSubsystemToControl(const UShort id);
        // Force take control of a subsystem (enable without button event).
        virtual void TakeDriveControl(const bool enable = true);
        // Method to check if we have control of a Primitive Driver
        virtual bool HaveDriveControl() const;
        // Method called when a button has been pressed.
        virtual void SignalButtonPress(const int buttonNumber);
        // Method called when a button has been released.
        virtual void SignalButtonRelease(const int buttonNumber);
        // Method called when a controller/joystick axis has been updated.
        virtual void UpdateWrench(const int inputID, const double percentEffort);
        // Method called when a controller/joystick axis has been updated.
        virtual void UpdateAnalogOut(const int inputID, const double signalValue);
        // Sends the current wrench efforts.
        virtual void SendWrenchEffort();
        // Sends the SetMicrocontrollerState command.
        virtual void SendMicrocontrollerState();
        // Clears current wrench effort being constructed.
        virtual void ClearWrenchEffort();
        // Clears current microcontroller state message being constructed.
        virtual void ClearMicrocontrollState();
        // Method to map a button on a controller to an actioan.
        virtual void MapButtonToAction(const int buttonNumber, const ButtonActions action);
        // Method to map a button on a controller to an actioan.
        virtual void MapButtonToDigitalOut(const int buttonNumber, 
                                           const std::string& digital,
                                           const bool onPress = true);
        // Method to map a controller axis/input to a Wrench.
        virtual void MapInputToWrench(const int inputID, 
                                      const WrenchEffort wrenchID,
                                      const double deadzone,
                                      const bool invert,
                                      const double limit);
        // Method to map a controller axis/input to a Wrench.
        virtual void MapInputToAnalogOut(const int inputID, 
                                         const std::string& analog,
                                         const bool invert,
                                         const double limit);
        // Method called periodically by Component or other Services, can be used for periodic udpdates.
        virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const { return false; }
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const { return false; }
        virtual void ProcessAcquisitionOfControl(const Address& controlledComponent);
        // VideoSubscriber doesn't need to be discovered.
        virtual bool IsDiscoverable() const { return false; }
        // Processes messages associated with the VideoSubscriber Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the VideoSubscriber Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Method called when transitioning to a ready state.
        virtual bool Resume() { return true; }
        // Method called to transition due to reset.
        virtual bool Reset() { return true; }
        // Method called when transitioning to a standby state.
        virtual bool Standby() { return true; }
        // Method called when transitioning to an emergency state.
        virtual bool SetEmergency() { return true; }
        // Method called when leaving the emergency state.
        virtual bool ClearEmergency() { return true; }
        // Method called when control is released.
        virtual bool ReleaseControl() { return true; }
        // Prints information about the service.
        virtual void PrintStatus() const;
        // Gets the ID of the driving component.
        Address GetDriverID() const { Mutex::ScopedLock lock(&mControlDevice); return mPrimitiveDriverID; }
        // Gets the ID of the microcontroller component.
        Address GetMicrocontrollerID() const { Mutex::ScopedLock lock(&mControlDevice); return mMicrocontrollerID; }
        // Gets the mappings of input IDs to wrench efforts.
        std::map<int, WrenchEffort> GetInputIdToWrenchMapping(){ Mutex::ScopedLock lock(&mControlDevice); return mWrenchMapping; }
        // Enable/Disable auto braking (do Resistive Efforts automatically when no control entered)
        void EnableAutoBrake(const bool enable) { mAutoBrakingFlag = enable; }
    private:
        Mutex mControlDevice;           ///< Mutex for proction of button/input mapping data.
        bool mTakeDriveControlFlag;     ///< If true, joystick should be taking control of desired subsystem.
        volatile bool mAutoBrakingFlag; ///< If true, automatic brake commands sent when force wrench is 0.
        UShort mSubsystemID;        ///< ID of subsystem to control.
        Address mPrimitiveDriverID; ///< ID of the primitive driver component.
        Address mMicrocontrollerID; ///< ID of the microcontroller component.
        std::map<int, WrenchEffort> mWrenchMapping;     ///<  Mapping from input ID to a wrench effort.
        std::map<int, double> mDeadzoneMapping;///<  Mapping of input ID to deadzones for wrenches.
        std::map<int, bool> mInvertMapping;    ///<  Mapping of input ID to invert flags for wrenches.
        std::map<int, ButtonActions> mButtonMapping;    ///<  Mapping from a button ID to an action.
        std::map<int, double> mLimitsMapping;  ///<  Mapping of input ID to limits.
        std::map<int, std::string> mAnalogMapping;           ///<  Mapping from input ID to a analog signal.
        std::map<std::string, bool> mAnalogInvertMapping;    ///<  Invert flags for wrenches.
        std::map<std::string, bool> mDigitalButtonState;     ///<  State when a digital out button is mapped.
        std::map<int, std::string> mButtonDigitalMapping;    ///<  Mapping from a button ID to an digiital out.
        std::map<std::string, double> mAnalogLimitsMapping;  ///<  Mapping of analog values to limits.
        SetWrenchEffort mWrenchEffort;                  ///<  Wrench effort being sent to Primitive Driver service.
        SetMicrocontrollerState mMicrocontrollerState;  ///<  Set Microcontroller State messages.
    };
}

#endif
/* End of File */
