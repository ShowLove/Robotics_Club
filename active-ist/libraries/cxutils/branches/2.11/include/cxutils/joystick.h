////////////////////////////////////////////////////////////////////////////////////
///
///  \file joystick.h
///  \brief Simple Joystick class for interfacing to a joystick.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 21 May 2007
///  <br>Copyright (c) 2009
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
#ifndef __CX_JOYSTICK_H
#define __CX_JOYSTICK_H

#ifdef WIN32
#pragma warning(disable : 4251) // Disables an annoying template DLL warning.
#endif

#include <limits.h>
#include <vector>
#include <string>

#include "thread.h"
#include "mutex.h"
#include "timer.h"
#include "cxutils/math/cxmath.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Joystick
    ///   \brief Simple Joystick class for interfacing to a joystick.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Joystick
    {
    public:
        static const short int MinAxisValue        = -32767;     ///<  Minimum joystick axis value.
        static const short int MaxAxisValue        =  32767;     ///<  Maximum joystick axis value.
        static const short int MaxAxis             =  9;         ///<  Total number of supported joystick axes.
        static const unsigned int MaxButtons       = 32;         ///<  Maximum number of joystick buttons.
        static const unsigned short int PovOff     = 65535;      ///<  POV value when POV centered.
        static const unsigned int AnyJoystick      = UINT_MAX;   ///<  Constant for selection of any joystick.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Axes
        ///   \brief Enumerations for the different axes of a joystick/gamepad.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Axes
        {
            X = 0,
            Y,
            Z,
            R,
            U,
            V,
            POV,
            PovX,
            PovY
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Event
        ///   \brief Enumerations for the different types of Joystick events.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Event
        {
            None = 0,
            ButtonDown,
            ButtonUp
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Simple Joystick Callback class which can be used to receive
        ///   automatic joystick updates.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Callback
        {
        public:
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Data
            ///   \brief Data structure to store pointers to callback data registered with
            ///   the Joystick interface.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            class CX_UTILS_DLL Data
            {
            public:    
                typedef std::vector<Data> List;
                Data() : mpJoyCallback(0), mpJoyFunc(0), mpJoyButtonFunc(0), mpJoyFuncArgs(0)
                {
                }
                Data(const Data& data) : mpJoyCallback(0), mpJoyFunc(0), mpJoyFuncArgs(0)
                {
                    *this = data;
                }
                ~Data() {}
                Data& operator=(const Data& data)
                {
                    mpJoyCallback = data.mpJoyCallback;
                    mpJoyFunc = data.mpJoyFunc;
                    mpJoyButtonFunc = data.mpJoyButtonFunc;
                    mpJoyFuncArgs = data.mpJoyFuncArgs;
                    return *this;
                }
                Joystick::Callback* mpJoyCallback;
                void (*mpJoyFunc)(const Joystick& jstick, void* args);
                void (*mpJoyButtonFunc)(const unsigned int buttonNumber, const Joystick::Event type, void* args);
                void *mpJoyFuncArgs;
            };
            virtual void ProcessJoystickEvent(const Joystick& jstick) = 0;
            virtual void ProcessButtonEvent(const unsigned int buttonNumber, const Joystick::Event type) {}
        };
        Joystick();
        ~Joystick();
        int Initialize(const unsigned int num = AnyJoystick, 
                       const unsigned int freq = 25,
                       const std::string& calibrationFile = std::string());
        int Shutdown();
        int GetAxis(const short int axis) const;
        int GetPOV() const;
        int GetButtonNumber() const;
        int GetID() const { return mID; }
        int RegisterCallback(Joystick::Callback* cb);
        int RegisterCallback(void (*func)(const Joystick& jstick, void* args), void* args);
        int RegisterCallback(void (*func)(const unsigned int buttonNumber, const Joystick::Event type, void* args), 
                             void* args);
        void ClearCallbacks();
        void PrintJoystickInfo() const;
        bool IsButtonPressed(const unsigned int num = 0) const;
        bool IsConnected() const;
        bool IsInitialized() const { return mJoystickInitializedFlag; }
        static short int GetAxisFromLetter(const char ch);
        static char GetLetterFromAxis(const short int axis);
        double GetAxisPercentage(const short int axis, const short int deadzone = 0) const;
        unsigned int GetUpdateFrequencey() const { return mUpdateFrequency; }
    protected:        
        static void JoystickThreadFunction(void* args);
#ifndef WIN32
        static void CallbackThreadFunction(void* args);
        Thread mCallbackThread;
#endif
        Mutex mJoystickMutex;               ///<  I/O protection.
        Mutex mJoyCallbackMutex;            ///<  Mutex for callbacks.
        Thread mJoystickThread;             ///<  Thread where data is read from joystick.
        Callback::Data::List mCallbacks;    ///<  Callbacks to run when joystick updates.
        int mID;                            ///<  Joystick ID number.
        int mButtons;                       ///<  Button values.
        int *mpAxis;                        ///<  Axis data.
        unsigned int mUpdateFrequency;      ///<  Update frequency.
        bool mJoystickConnectedFlag;        ///<  Connection flag.
        void* mpJoyInterface;               ///<  Interface to Joystick.
        bool mJoystickInitializedFlag;      ///<  True if interface was initialized.
    };

    typedef Joystick::Callback JoystickCallback; // Backwards compatibility with previous version of CxUtils.
}

#endif

/*  End of File */
