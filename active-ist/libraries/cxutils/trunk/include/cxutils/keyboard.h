////////////////////////////////////////////////////////////////////////////////////
///
///  \file keyboard.h
///  \brief Main include file which contains methods for capturing and generating
///  keyboard events.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 09 March 2009
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
#ifndef __CXUTILS_KEYBOARD__H
#define __CXUTILS_KEYBOARD__H

#include "cxutils/cxbase.h"
#include "cxutils/timer.h"
#include <vector>
#include <string>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Keyboard
    ///   \brief Static methods for determining if a key is currently being pressed
    ///   and to generate keyboard/key events.
    ///
    ///   Dependencies for Linux: libx11, libxtst
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Keyboard
    {
    public:
        static const unsigned int MaxKeys = 256; ///<  Current max supported keys.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Key
        ///   \brief Enumeration of different keys available.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Key
        {
            Any = -1,
            Invalid = 0,
            Backspace,
            Tab,
            Clear,
            Return,
            ShiftLock,
            Pause,
            CapsLock,
            Escape,
            Space,
            PageUp,
            PageDown,
            End,
            Home,
            LeftArrow,
            UpArrow,
            RightArrow,
            DownArrow,
            Select,
            Print,
            Execute,
            PrintScreen,
            Insert,
            Delete,
            Help,
            Zero = 0x30,
            One,
            Two,
            Three,
            Four,
            Five,
            Six,
            Seven,
            Eight,
            Nine,
            A = 0x41,
            B,
            C,
            D,
            E,
            F,
            G,
            H,
            I,
            J,
            K,
            L,
            M,
            N,
            O,
            P,
            Q,
            R,
            S,
            T,
            U,
            V,
            W,
            X,
            Y,
            Z,
            Numpad0,
            Numpad1,
            Numpad2,
            Numpad3,
            Numpad4,
            Numpad5,
            Numpad6,
            Numpad7,
            Numpad8,
            Numpad9,
            Multiply,
            Add,
            Separator,
            Subtract,
            Decimal,
            Divide,
            F1,
            F2,
            F3,
            F4,
            F5,
            F6,
            F7,
            F8,
            F9,
            F10,
            F11,
            F12,
            NumLock,
            ScrollLock,
            LeftShift,
            RightShift,
            LeftControl,
            RightControl,
            Comma,
            Period,
            LeftAlt,
            RightAlt
        };
        typedef std::vector<Key> KeyList;
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class to use to get keyboard events.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CX_UTILS_DLL Callback
        {
        public:
            typedef std::set<Callback*> Set;
            Callback() {}
            virtual ~Callback() {}
            virtual void ProcessKeyEvent(const Keyboard::Key key, const bool down) = 0;
        };
        Keyboard();
        ~Keyboard();
        bool RegisterCallback(Callback* cb);
        void RemoveCallback(Callback* cb);
        static bool IsKeyDown(const Key key = Any);
        static Key GetCurrentKey();
        static KeyList GetPressedKeys();
        static bool FakeKeyPress(const Key key, const bool releaseFlag = true, const bool directX = false);
        static bool FakeKeyRelease(const Key key, const bool directX = false);
        static unsigned int LookupHostSystemKey(const Key key);
        static Key LookupFromHostSystemKey(const unsigned int hostKey);
        static std::string ToString(const Key key);
        static Key FromString(const std::string& strKey);
#ifdef WIN32
        static int LookupScanCodeDX(const Key key);
#endif
        // Returns true if callbacks are being generated.
        inline bool IsActive() const { return mTimer.IsActive(); }
    private:
        static void KeyboardTimerEvent(void* args);
        Mutex mCallbacksMutex;          ///<  Thread protection of callbacks.
        Callback::Set mCallbacks;       ///<  Callbacks for generating keyboard events.
        Timer mTimer;                   ///<  Timer used to check for key events.
        std::map<Key, bool> mKeyStates; ///<  States of all the keys.
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \return The current key being pressed (for active console), 0 if none. 
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    int CX_UTILS_DLL GetChar();
}


#endif
/* End of File */
