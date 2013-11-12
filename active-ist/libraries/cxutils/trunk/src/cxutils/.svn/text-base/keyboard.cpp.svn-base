////////////////////////////////////////////////////////////////////////////////////
///
///  \file keyboard.cpp
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
#include "cxutils/keyboard.h"
#include <iostream>

#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <windows.h>
#include <conio.h>
#else
#include <X11/extensions/XTest.h>
#include <X11/keysym.h>
#include <X11/XKBlib.h>
#include <X11/cursorfont.h>
#include <X11/Xutil.h>
#include <unistd.h>
#include <sys/time.h>
#include <termios.h>
#include <assert.h>
#endif

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \return The current key being pressed (for active console), 0 if none. 
///
////////////////////////////////////////////////////////////////////////////////////
int CxUtils::GetChar()
{
    int ch = 0;
#ifdef WIN32
    if(_kbhit())
    {
        ch = _getch();
    }
#else
    struct termios initial_settings;
    struct termios new_settings;
    int nread;

    // Get initial settings
    tcgetattr(0, &initial_settings);
    new_settings = initial_settings;
    new_settings.c_lflag &= ~ICANON;
    new_settings.c_lflag &= ~ECHO;
    new_settings.c_lflag &= ~ISIG;
    new_settings.c_cc[VMIN] = 1;
    new_settings.c_cc[VTIME] = 0;
    // Set the new settings.
    tcsetattr(0, TCSANOW, &new_settings);

    // Check if a key is pressed.
    new_settings.c_cc[VMIN] = 0;
    tcsetattr(0, TCSANOW, &new_settings);
    nread = read(0, &ch, 1);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0, TCSANOW, &new_settings);

    if(nread != 1)
    {
        ch = 0;
    }

    // Reset settings
    tcsetattr(0, TCSANOW, &initial_settings);


#endif

    return ch;
}


#ifndef WIN32

unsigned int GetBit(char* keyboard, unsigned int index)
{
    return keyboard[index/8] & (1 << (index%8));
}

#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::Keyboard()
{
    mTimer.RegisterTimerEvent(Keyboard::KeyboardTimerEvent, this);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::~Keyboard()
{
    mTimer.Stop();
    mKeyStates.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Register a callback to receive keyboard events.
///
///   \param[in] cb Pointer to callback object to run when keys are pressed
///                 or released.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Keyboard::RegisterCallback(CxUtils::Keyboard::Callback *cb)
{
    mCallbacksMutex.Lock();
    mCallbacks.insert(cb);
    mCallbacksMutex.Unlock();
    // Initialize key board state and start timer.
    if(mTimer.IsActive() == false)
    {        
        for(unsigned int i = 0; i < MaxKeys; i++)
        {
            mKeyStates[(Key)i] = IsKeyDown((Key)i);
        }
        mTimer.Start(25);
    }
    return mTimer.IsActive();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Remove a callback to receive keyboard events.
///
///   \param[in] cb Pointer to callback object to remove.
///
////////////////////////////////////////////////////////////////////////////////////
void Keyboard::RemoveCallback(CxUtils::Keyboard::Callback *cb)
{
    mCallbacksMutex.Lock();
    if(mCallbacks.find(cb) != mCallbacks.end())
    {
        mCallbacks.erase(mCallbacks.find(cb));
    }
    if(mCallbacks.size() == 0)
    {
        mTimer.Stop();
    }
    mCallbacksMutex.Unlock();    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called on timer events. Checks for changes in key states.
///
////////////////////////////////////////////////////////////////////////////////////
void Keyboard::KeyboardTimerEvent(void *args)
{
    std::vector<Key> downKeys;
    std::vector<Key> upKeys;
    std::vector<Key>::iterator keyEvent;

    std::map<Key, bool>::iterator k;
    Keyboard::Callback::Set::iterator cb;

    Keyboard* keyboard = (Keyboard*)args;

    for(k = keyboard->mKeyStates.begin(); k != keyboard->mKeyStates.end(); k++)
    {
        bool down = IsKeyDown(k->first);
        if(down && k->second == false)
        {
            downKeys.push_back(k->first);
        }
        else if(down == false && k->second == true)
        {
            upKeys.push_back(k->first); 
        }
        k->second = down;
    } 

    keyboard->mCallbacksMutex.Lock();
    for(keyEvent = downKeys.begin();
        keyEvent != downKeys.end();
        keyEvent++)
    {
        for(cb = keyboard->mCallbacks.begin();
            cb != keyboard->mCallbacks.end();
            cb++)
        {
            (*cb)->ProcessKeyEvent(*keyEvent, true);
        }
    }

    for(keyEvent = upKeys.begin();
        keyEvent != upKeys.end();
        keyEvent++)
    {
        for(cb = keyboard->mCallbacks.begin();
            cb != keyboard->mCallbacks.end();
            cb++)
        {
            (*cb)->ProcessKeyEvent(*keyEvent, false);
        }
    }

    keyboard->mCallbacksMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if a key on the keyboard in being pressed.
///
///   \param[in] key The key to check for being pressed. If the value is Any, 
///                  default, then the method will check for any key that is
///                  pressed.  The console or program does not have to be 
///                  active (this is a passive method).
///
///   \return True if the key is currently down.
///
////////////////////////////////////////////////////////////////////////////////////
bool Keyboard::IsKeyDown(const Key key)
{
    bool result = false;
#ifdef WIN32
    SHORT _key = 0;
    if(key == Any)
    {
        for(int k = 1; k < MaxKeys; k++)
        {
            if(IsKeyDown((Key)k))
            {
                result = true;
                break;
            }
        }
    }    
    else
    {
        _key  = ::GetAsyncKeyState((int)LookupHostSystemKey(key));
        if( (_key & 0x8000) != 0 )
        {
            result = true;
        }
    }
#else
    Display* display = XOpenDisplay(NULL);
    char keyboard[32];

    XSynchronize(display, true);
    if(XQueryKeymap(display, keyboard))
    {
        if(key == Any)
        {
            for(unsigned int i = 0; i < 32; i++)
            {
                char c = keyboard[i];
                for(unsigned int b = 0; b < 8; b++)
                {
                    if( (c & 0x1) )
                    {
                        result = true;
                        break;
                    }
                    c = c >> 1;
                }
            }
        }
        else
        {
            KeySym keySym = LookupHostSystemKey(key);
            KeyCode keyCode = XKeysymToKeycode(display, keySym);
            if(IsModifierKey( keySym ) )
            {
                XModifierKeymap* map = XGetModifierMapping(display);
                int iKeyMask = 0;
                unsigned int iMask = 0;
                Window wDummy1, wDummy2;
                int iDummy3, iDummy4, iDummy5, iDummy6;
        
                for(int i = 0; i < 8; ++i)
                {
                    if(map->modifiermap[map->max_keypermod*i] == keyCode)
                    {
                        iKeyMask = 1 << i;
                    }
                }
                XQueryPointer(display, DefaultRootWindow(display), &wDummy1, &wDummy2, 
                              &iDummy3, &iDummy4, &iDummy5, &iDummy6, &iMask);
                XFreeModifiermap(map);
                result = (iMask & iKeyMask) != 0;
            }
            else
            {
                if( (keyboard[keyCode >> 3] & (1 << (keyCode & 7))) > 0)
                {
                    result = true;
                }
            }
        }
    }

    XCloseDisplay(display);
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current key being pressed.
///
///   \return The value of the current key that is down.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::Key Keyboard::GetCurrentKey()
{
    Key pressed = (Key)Invalid;
    
#ifdef WIN32
    for( int k = 1; k < MaxKeys; k++)
    {
        if(IsKeyDown( (Key)k ) )
        {
            pressed = (Key)k;
            break;
        }
    }
#else
    Display* display = XOpenDisplay(NULL);
    char keyboard[32];

    XSynchronize(display, true);
    bool done = false;
    if(XQueryKeymap(display, keyboard))
    {
        for(unsigned int i = 0; i < 32 && !done; i++)
        {
            char c = keyboard[i];
            for(unsigned int b = 0; b < 8; b++)
            {
                if( (c & 0x1) )
                {
                    KeySym keySym = XkbKeycodeToKeysym(display, 8*i + b, true, 0);
                    pressed = LookupFromHostSystemKey(keySym);
                    if(pressed == Invalid)
                    {
                        keySym = XkbKeycodeToKeysym(display, 8*i + b, false, 0);
                        pressed = LookupFromHostSystemKey(keySym);
                    }
                    done = true;
                    break;
                }
                c = c >> 1;
            }
        }
    }
    XCloseDisplay(display);
#endif

    return pressed;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return List of all the keys currently being pressed.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::KeyList Keyboard::GetPressedKeys()
{
    KeyList pressed;
    for(unsigned int k = 1; k < MaxKeys; k++)
    {
        if(IsKeyDown( (Key)k ) )
        {
            pressed.push_back( (Key)k );
        }
    }
    return pressed;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to generate a fake keyboard event.
///
///   \param[in] key The key to generate a fake keyboard event for.
///   \param[in] releaseFlag If true, the method will also generate a 
///                          key up command after the key down event.
///   \param[in] directX Flag only used in Windows.  If you are tryign to fake a
///                      key press for a DirectX application (video games) then
///                      you must set this to true because direct X uses
///                      hardware scan codes, which other applications do not.
///
///   \return True on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Keyboard::FakeKeyPress(const Keyboard::Key key,
                            const bool releaseFlag,
                            const bool directX)
{
    bool result = false;
    if(key != Invalid && key != Any)
    {
#ifdef WIN32
        ::INPUT k;
        k.type = INPUT_KEYBOARD;
        if(directX == false)
        {
            k.ki.wVk = (int)LookupHostSystemKey(key);
            k.ki.dwFlags = KEYEVENTF_UNICODE;
            k.ki.time = 0;
            k.ki.wScan = 0;
            k.ki.dwExtraInfo = 0;
        }
        else
        {
            k.ki.wVk = 0;
            k.ki.dwFlags = KEYEVENTF_SCANCODE;
            k.ki.time = 0;
            k.ki.wScan =  LookupScanCodeDX(key);
            k.ki.dwExtraInfo = 0;
        }

        if(::SendInput(1, &k, sizeof(::INPUT)) > 0)
        {
            result = true;     
            if(releaseFlag)
            {
                FakeKeyRelease(key, directX);
            }
        }        
#else
        Display* display = XOpenDisplay(NULL);
        if(display)
        {
            if(XTestFakeKeyEvent(display, XKeysymToKeycode( display, LookupHostSystemKey(key)), true, 0) != 0)
            {
                result = true;
                if(releaseFlag)
                {
                    XTestFakeKeyEvent(display, XKeysymToKeycode( display, LookupHostSystemKey(key)), false, 0);
                }
            }
            XCloseDisplay(display);
        }
#endif
    }
    
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to generate a fake keyboard event.
///
///   \param[in] key The key to generate a fake keyboard event for.
///   \param[in] directX Flag only used in Windows.  If you are tryign to fake a
///                      key press for a DirectX application (video games) then
///                      you must set this to true because direct X uses
///                      hardware scan codes, which other applications do not.
///
///   \return True on success, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Keyboard::FakeKeyRelease(const Keyboard::Key key, const bool directX)
{
    bool result = false;
    if(key != Invalid && key != Any)
    {
#ifdef WIN32
        ::INPUT k;
        k.type = INPUT_KEYBOARD;
        if(directX == false)
        {
            k.ki.wVk = (int)LookupHostSystemKey(key);
            k.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_UNICODE;
            k.ki.time = 0;
            k.ki.wScan = 0;
            k.ki.dwExtraInfo = 0;
        }
        else
        {
            k.ki.wVk = 0;
            k.ki.dwFlags = KEYEVENTF_KEYUP | KEYEVENTF_SCANCODE;
            k.ki.time = 0;
            k.ki.wScan =  LookupScanCodeDX(key);
            k.ki.dwExtraInfo = 0;
        }
        if(::SendInput(1, &k, sizeof(::INPUT)) > 0)
        {
            result = true;
        }
#else
        Display* display = XOpenDisplay(NULL);
        if(display)
        {
            if(XTestFakeKeyEvent(display, XKeysymToKeycode( display, LookupHostSystemKey(key)), false, 0) != 0)
            {
                result = true;
            }
            XCloseDisplay(display);
        }
#endif
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to convert from generic key enumeration to OS specific
///          value.
///
///   \param[in] key The key to perform lookup for.
///
///   \return 0 on failure, otherwise the host system key value match.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Keyboard::LookupHostSystemKey(const Key key)
{
    unsigned int value = 0;
#ifdef WIN32
    switch(key)
    {
        case Backspace:
            value = VK_BACK;
            break;
        case Tab:
            value = VK_TAB;
            break;
        case Clear:
            value = VK_CLEAR;
            break;
        case Return:
            value = VK_RETURN;
            break;
        case ShiftLock:
            value = VK_CAPITAL;
            break;
        case Pause:
            value = VK_PAUSE;
            break;
        case CapsLock:
            value = VK_CAPITAL;
            break;
        case Escape:
            value = VK_ESCAPE;
            break;
        case Space:
            value = VK_SPACE;
            break;
        case PageUp:
            value = VK_PRIOR;
            break;
        case PageDown:
            value = VK_NEXT;
            break;
        case End:
            value = VK_END;
            break;
        case Home:
            value = VK_HOME;
            break;
        case LeftArrow:
            value = VK_LEFT;
            break;
        case UpArrow:
            value = VK_UP;
            break;
        case RightArrow:
            value = VK_RIGHT;
            break;
        case DownArrow:
            value = VK_DOWN;
            break;
        case Select:
            value = VK_SELECT;
            break;
        case Print:
            value = VK_SNAPSHOT;
            break;
        case Execute:
            value = VK_EXECUTE;
            break;
        case PrintScreen:
            value = VK_SNAPSHOT;
            break;
        case Insert:
            value = VK_INSERT;
            break;
        case Delete:
            value = VK_DELETE;
            break;
        case Help:
            value = VK_HELP;
            break;
        case Zero:
            value = 0x30;
            break;
        case One:
            value = 0x31;
            break;
        case Two:
            value = 0x32;
            break;
        case Three:
            value = 0x33;
            break;
        case Four:
            value = 0x34;
            break;
        case Five:
            value = 0x35;
            break;
        case Six:
            value = 0x36;
            break;
        case Seven:
            value = 0x37;
            break;
        case Eight:
            value = 0x38;
            break;
        case Nine:
            value = 0x39;
            break;        
        case A:
            value = 0x41;
            break;
        case B:
            value = 0x42;
            break;
        case C:
            value = 0x43;
            break;
        case D:
            value = 0x44;
            break;
        case E:
            value = 0x45;
            break;
        case F:
            value = 0x46;
            break;
        case G:
            value = 0x47;
            break;
        case H:
            value = 0x48;
            break;
        case I:
            value = 0x49;
            break;
        case J:
            value = 0x4A;
            break;
        case K:
            value = 0x4B;
            break;
        case L:
            value = 0x4C;
            break;
        case M:
            value = 0x4D;
            break;
        case N:
            value = 0x4E;
            break;
        case O:
            value = 0x4F;
            break;
        case P:
            value = 0x50;
            break;
        case Q:
            value = 0x51;
            break;
        case R:
            value = 0x52;
            break;
        case S:
            value = 0x53;
            break;
        case T:
            value = 0x54;
            break;
        case U:
            value = 0x55;
            break;
        case V:
            value = 0x56;
            break;
        case W:
            value = 0x11; //0x57;
            break;
        case X:
            value = 0x58;
            break;
        case Y:
            value = 0x59;
            break;
        case Z:
            value = 0x5A;
            break;
        case Numpad0:
            value = VK_NUMPAD0;
            break;
        case Numpad1:
            value = VK_NUMPAD1;
            break;
        case Numpad2:
            value = VK_NUMPAD2;
            break;
        case Numpad3:
            value = VK_NUMPAD3;
            break;
        case Numpad4:
            value = VK_NUMPAD4;
            break;
        case Numpad5:
            value = VK_NUMPAD5;
            break;
        case Numpad6:
            value = VK_NUMPAD6;
            break;
        case Numpad7:
            value = VK_NUMPAD7;
            break;
        case Numpad8:
            value = VK_NUMPAD8;
            break;
        case Numpad9:
            value = VK_NUMPAD9;
            break;
        case Multiply:
            value = VK_MULTIPLY;
            break;
        case Add:
            value = VK_ADD;
            break;
        case Separator:
            value = VK_SEPARATOR;
            break;
        case Subtract:
            value = VK_SUBTRACT;
            break;
        case Decimal:
            value = VK_DECIMAL;
            break;
        case Divide:
            value = VK_DIVIDE;
            break;
        case F1:
            value = VK_F1;
            break;
        case F2:
            value = VK_F2;
            break;
        case F3:
            value = VK_F3;
            break;
        case F4:
            value = VK_F4;
            break;
        case F5:
            value = VK_F5;
            break;
        case F6:
            value = VK_F6;
            break;
        case F7:
            value = VK_F7;
            break;
        case F8:
            value = VK_F8;
            break;
        case F9:
            value = VK_F9;
            break;
        case F10:
            value = VK_F10;
            break;
        case F11:
            value = VK_F11;
            break;
        case F12:
            value = VK_F12;
            break;
        case NumLock:
            value = VK_NUMLOCK;
            break;
        case ScrollLock:
            value = VK_SCROLL;
            break;
        case LeftShift:
            value = VK_LSHIFT;
            break;
        case RightShift:
            value = VK_RSHIFT;
            break;
        case LeftControl:
            value = VK_LCONTROL;
            break;
        case RightControl:
            value = VK_RCONTROL;
            break;
        case Comma:
            value = VK_OEM_COMMA;
            break;
        case Period:
            value = VK_OEM_PERIOD;
            break;
        case LeftAlt:
            value = VK_MENU;
            break;
        case RightAlt:
            value = VK_MENU;
            break;
        default:
            value = 0;
            break;
    }
#else
    switch(key)
    {
        case Backspace:
            value = XK_BackSpace;
            break;
        case Tab:
            value = XK_Tab;
            break;
        case Clear:
            value = XK_Clear;
            break;
        case Return:
            value = XK_Return;
            break;
        case ShiftLock:
            value = XK_Shift_Lock;
            break;
        case Pause:
            value = XK_Pause;
            break;
        case CapsLock:
            value = XK_Caps_Lock;
            break;
        case Escape:
            value = XK_Escape;
            break;
        case Space:
            value = ' ';
            break;
        case PageUp:
            value = XK_Page_Up;
            break;
        case PageDown:
            value = XK_Page_Down;
            break;
        case End:
            value = XK_End;
            break;
        case Home:
            value = XK_Home;
            break;
        case LeftArrow:
            value = XK_Left;
            break;
        case UpArrow:
            value = XK_Up;
            break;
        case RightArrow:
            value = XK_Right;
            break;
        case DownArrow:
            value = XK_Down;
            break;
        case Select:
            value = XK_Select;
            break;
        case Print:
            value = XK_Print;
            break;
        case Execute:
            value = XK_Execute;
            break;
        case PrintScreen:
            value = XK_Print;
            break;
        case Insert:
            value = XK_Insert;
            break;
        case Delete:
            value = XK_Delete;
            break;
        case Help:
            value = XK_Help;
            break;
        case Zero:
            value = XK_0;
            break;
        case One:
            value = XK_1;
            break;
        case Two:
            value = XK_2;
            break;
        case Three:
            value = XK_3;
            break;
        case Four:
            value = XK_4;
            break;
        case Five:
            value = XK_5;
            break;
        case Six:
            value = XK_6;
            break;
        case Seven:
            value = XK_7;
            break;
        case Eight:
            value = XK_8;
            break;
        case Nine:
            value = XK_9;
            break;        
        case A:
            value = XK_A;
            break;
        case B:
            value = XK_B;
            break;
        case C:
            value = XK_C;
            break;
        case D:
            value = XK_D;
            break;
        case E:
            value = XK_E;
            break;
        case F:
            value = XK_F;
            break;
        case G:
            value = XK_G;
            break;
        case H:
            value = XK_H;
            break;
        case I:
            value = XK_I;
            break;
        case J:
            value = XK_J;
            break;
        case K:
            value = XK_K;
            break;
        case L:
            value = XK_L;
            break;
        case M:
            value = XK_M;
            break;
        case N:
            value = XK_N;
            break;
        case O:
            value = XK_O;
            break;
        case P:
            value = XK_P;
            break;
        case Q:
            value = XK_Q;
            break;
        case R:
            value = XK_R;
            break;
        case S:
            value = XK_S;
            break;
        case T:
            value = XK_T;
            break;
        case U:
            value = XK_U;
            break;
        case V:
            value = XK_V;
            break;
        case W:
            value = XK_W;
            break;
        case X:
            value = XK_X;
            break;
        case Y:
            value = XK_Y;
            break;
        case Z:
            value = XK_Z;
            break;
        case Numpad0:
            value = XK_KP_0;
            break;
        case Numpad1:
            value = XK_KP_1;
            break;
        case Numpad2:
            value = XK_KP_2;
            break;
        case Numpad3:
            value = XK_KP_3;
            break;
        case Numpad4:
            value = XK_KP_4;
            break;
        case Numpad5:
            value = XK_KP_5;
            break;
        case Numpad6:
            value = XK_KP_6;
            break;
        case Numpad7:
            value = XK_KP_7;
            break;
        case Numpad8:
            value = XK_KP_8;
            break;
        case Numpad9:
            value = XK_KP_9;
            break;
        case Multiply:
            value = XK_KP_Multiply;
            break;
        case Add:
            value = XK_KP_Add;
            break;
        case Separator:
            value = XK_KP_Separator;
            break;
        case Subtract:
            value = XK_KP_Subtract;
            break;
        case Decimal:
            value = XK_KP_Decimal;
            break;
       case Divide:
            value = XK_KP_Divide;
            break;
        case F1:
            value = XK_F1;
            break;
        case F2:
            value = XK_F2;
            break;
        case F3:
            value = XK_F3;
            break;
        case F4:
            value = XK_F4;
            break;
        case F5:
            value = XK_F5;
            break;
        case F6:
            value = XK_F6;
            break;
        case F7:
            value = XK_F7;
            break;
        case F8:
            value = XK_F8;
            break;
        case F9:
            value = XK_F9;
            break;
        case F10:
            value = XK_F10;
            break;
        case F11:
            value = XK_F11;
            break;
        case F12:
            value = XK_F12;
            break;
        case NumLock:
            value = XK_Num_Lock;
            break;
        case ScrollLock:
            value = XK_Scroll_Lock;
            break;
        case LeftShift:
            value = XK_Shift_L;
            break;
        case RightShift:
            value = XK_Shift_R;
            break;
        case LeftControl:
            value = XK_Control_L;
            break;
        case RightControl:
            value = XK_Control_R;
            break;
        case Comma:
            value = XK_comma;
            break;
        case Period:
            value = XK_period;
            break;
        case LeftAlt:
            value = XK_Alt_L;
            break;
        case RightAlt:
            value = XK_Alt_R;
            break;
        default:
            value = 0;
            break;
    }
#endif
    return value;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to convert from the host system key value to
///         a CxUtils Keyboard::Key enumeration value.
///
///   \param[in] hostKey the host system key to convert from.
///
///   \return None on failure, otherwise correct key mapping.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::Key Keyboard::LookupFromHostSystemKey(const unsigned int hostKey)
{
    Keyboard::Key value;
    
    #ifdef WIN32
    if(hostKey >= 0x30 && hostKey <= 0x39)
    {
        return (Keyboard::Key)hostKey;        
    }
    else if(hostKey >= 0x41 && hostKey <= 0x5A)
    {
        return (Keyboard::Key)hostKey;        
    }
    switch(hostKey)
    {
        case VK_BACK:
            value = Backspace;
            break;
        case VK_TAB:
            value = Tab;
            break;
        case VK_CLEAR:
            value = Clear;
            break;
        case VK_RETURN:
            value = Return;
            break;
        case VK_CAPITAL:
            value = ShiftLock;
            break;
        case VK_PAUSE:
            value = Pause;
            break;
        case VK_ESCAPE:
            value = Escape;
            break;
        case VK_SPACE:
            value = Space;
            break;
        case VK_PRIOR:
            value = PageUp;
            break;
        case VK_NEXT:
            value = PageDown;
            break;
        case VK_END:
            value = End;
            break;
        case VK_HOME:
            value = Home;
            break;
        case VK_LEFT:
            value = LeftArrow;
            break;
        case VK_UP:
            value = UpArrow;
            break;
        case VK_RIGHT:
            value = RightArrow;
            break;
        case VK_DOWN:
            value = DownArrow;
            break;
        case VK_SELECT:
            value =  Select;
            break;
        case VK_SNAPSHOT:
            value = Print;
            break;
        case VK_EXECUTE:
            value =  Execute;
            break;
        case VK_INSERT:
            value =  Insert;
            break;
        case VK_DELETE:
            value =  Delete;
            break;
        case VK_HELP:
            value =  Help;
            break;
        case VK_NUMPAD0:
            value =  Numpad0;
            break;
        case VK_NUMPAD1:
            value =  Numpad1;
            break;
        case VK_NUMPAD2:
            value =  Numpad2;
            break;
        case VK_NUMPAD3:
            value =  Numpad3;
            break;
        case VK_NUMPAD4:
            value =  Numpad4;
            break;
        case VK_NUMPAD5:
            value =  Numpad5;
            break;
        case VK_NUMPAD6:
            value =  Numpad6;
            break;
        case VK_NUMPAD7:
            value =  Numpad7;
            break;
        case VK_NUMPAD8:
            value =  Numpad8;
            break;
        case VK_NUMPAD9:
            value =  Numpad9;
            break;
        case VK_MULTIPLY:
            value =  Multiply;
            break;
        case VK_ADD:
            value =  Add;
            break;
        case VK_SEPARATOR:
            value =  Separator;
            break;
        case VK_SUBTRACT:
            value =  Subtract;
            break;
        case VK_DECIMAL:
            value =  Decimal;
            break;
        case VK_DIVIDE:
            value =  Divide;
            break;
        case VK_F1:
            value = F1;
            break;
        case VK_F2:
            value = F2;
            break;
        case VK_F3:
            value = F3;
            break;
        case VK_F4:
            value = F4;
            break;
        case VK_F5:
            value = F5;
            break;
        case VK_F6:
            value = F6;
            break;
        case VK_F7:
            value = F7;
            break;
        case VK_F8:
            value = F8;
            break;
        case VK_F9:
            value = F9;
            break;
        case VK_F10:
            value = F10;
            break;
        case VK_F11:
            value = F11;
            break;
        case VK_F12:
            value = F12;
            break;
        case VK_NUMLOCK:
            value = NumLock;
            break;
        case VK_SCROLL:
            value =  ScrollLock;
            break;
        case VK_LSHIFT:
            value =  LeftShift;
            break;
        case VK_RSHIFT:
            value =  RightShift;
            break;
        case VK_LCONTROL:
            value =  LeftControl;
            break;
        case VK_RCONTROL:
            value =  RightControl;
            break;
        case VK_OEM_COMMA:
            value =  Comma;
            break;
        case VK_OEM_PERIOD:
            value = Period;
            break;
        case VK_MENU:
            value = LeftAlt;
            break;
        default:
            value = Invalid;
            break;
    }
#else
    switch(hostKey)
    {
        case XK_BackSpace:
            value = Backspace;
            break;
        case XK_Tab:
            value = Tab;
            break;
        case XK_Clear:
            value = Clear;
            break;
        case XK_Return:
            value = Return;
            break;
        case XK_Shift_Lock:
            value = ShiftLock;
            break;
        case XK_Pause:
            value = Pause;
            break;
        case XK_Caps_Lock:
            value = CapsLock;
            break;
        case XK_Escape:
            value = Escape;
            break;
        case XK_KP_Space:
            value = Space;
            break;
        case ' ':
            value = Space;
            break;
        case XK_Page_Up:
            value = PageUp;
            break;
        case XK_Page_Down:
            value = PageDown;
            break;
        case XK_End:
            value = End;
            break;
        case XK_Home:
            value = Home;
            break;
        case XK_Left:
            value = LeftArrow;
            break;
        case XK_Up:
            value = UpArrow;
            break;
        case XK_Right:
            value = RightArrow;
            break;
        case XK_Down:
            value = DownArrow;
            break;
        case XK_Select:
            value = Select;
            break;
        case XK_Execute:
            value = Execute;
            break;
        case XK_Print:
            value = PrintScreen;
            break;
        case XK_Insert:
            value = Insert;
            break;
        case XK_Delete:
            value = Delete;
            break;
        case XK_Help:
            value = Help;
            break;
        case XK_0:
            value = Zero;
            break;
        case XK_1:
            value = One;
            break;
        case XK_2:
            value = Two;
            break;
        case XK_3:
            value = Three;
            break;
        case XK_4:
            value = Four;
            break;
        case XK_5:
            value = Five;
            break;
        case XK_6:
            value = Six;
            break;
        case XK_7:
            value = Seven;
            break;
        case XK_8:
            value = Eight;
            break;
        case XK_9:
            value = Nine;
            break;        
        case XK_A:
            value = A;
            break;
        case XK_B:
            value = B;
            break;
        case XK_C:
            value = C;
            break;
        case XK_D:
            value = D;
            break;
        case XK_E:
            value = E;
            break;
        case XK_F:
            value = F;
            break;
        case XK_G:
            value = G;
            break;
        case XK_H:
            value = H;
            break;
        case XK_I:
            value = I;
            break;
        case XK_J:
            value = J;
            break;
        case XK_K:
            value = K;
            break;
        case XK_L:
            value = L;
            break;
        case XK_M:
            value = M;
            break;
        case XK_N:
            value = N;
            break;
        case XK_O:
            value = O;
            break;
        case XK_P:
            value = P;
            break;
        case XK_Q:
            value = Q;
            break;
        case XK_R:
            value = R;
            break;
        case XK_S:
            value = S;
            break;
        case XK_T:
            value = T;
            break;
        case XK_U:
            value = U;
            break;
        case XK_V:
            value = V;
            break;
        case XK_W:
            value = W;
            break;
        case XK_X:
            value = X;
            break;
        case XK_Y:
            value = Y;
            break;
        case XK_Z:
            value = Z;
            break;
        case XK_KP_0:
            value = Numpad0;
            break;
        case XK_KP_1:
            value = Numpad1;
            break;
        case XK_KP_2:
            value = Numpad2;
            break;
        case XK_KP_3:
            value = Numpad3;
            break;
        case XK_KP_4:
            value = Numpad4;
            break;
        case XK_KP_5:
            value = Numpad5;
            break;
        case XK_KP_6:
            value = Numpad6;
            break;
        case XK_KP_7:
            value = Numpad7;
            break;
        case XK_KP_8:
            value = Numpad8;
            break;
        case XK_KP_9:
            value = Numpad9;
            break;
        case XK_KP_Multiply:
            value = Multiply;
            break;
        case XK_KP_Add:
            value = Add;
            break;
        case XK_KP_Separator:
            value = Separator;
            break;
        case XK_KP_Subtract:
            value = Subtract;
            break;
        case XK_KP_Decimal:
            value = Decimal;
            break;
        case XK_KP_Divide:
            value = Divide;
            break;
        case XK_F1:
            value = F1;
            break;
        case XK_F2:
            value = F2;
            break;
        case XK_F3:
            value = F3;
            break;
        case XK_F4:
            value = F4;
            break;
        case XK_F5:
            value = F5;
            break;
        case XK_F6:
            value = F6;
            break;
        case XK_F7:
            value = F7;
            break;
        case XK_F8:
            value = F8;
            break;
        case XK_F9:
            value = F9;
            break;
        case XK_F10:
            value = F10;
            break;
        case XK_F11:
            value = F11;
            break;
        case XK_F12:
            value = F12;
            break;
        case XK_Num_Lock:
            value =  NumLock;
            break;
        case XK_Scroll_Lock:
            value =  ScrollLock;
            break;
        case XK_Shift_L:
            value =  LeftShift;
            break;
        case XK_Shift_R:
            value =  RightShift;
            break;
        case XK_Control_L:
            value =  LeftControl;
            break;
        case XK_Control_R:
            value =  RightControl;
            break;
        case  XK_comma:
            value = Comma;
            break;
        case XK_period:
            value = Period;
            break;
        case XK_Alt_L:
            value = LeftAlt;
            break;
        case XK_Alt_R:
            value = RightAlt;
            break;
        default:
            value = Invalid;
            break;
    }
#endif
    
    return value;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to lookup a string version of a key.
///
///   \param[in] key The key to get string equivalent for.
///
///   \return "Invalid" on failure, otherwise a string representation of the key.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Keyboard::ToString(const Key key)
{
    std::string value;

    switch(key)
    {
        case Backspace:
            value = "Backspace";
            break;
        case Tab:
            value = "Tab";
            break;
        case Clear:
            value = "Clear";
            break;
        case Return:
            value = "Return";
            break;
        case ShiftLock:
            value = "Shift Lock";
            break;
        case Pause:
            value = "Pause";
            break;
        case CapsLock:
            value = "Caps Lock";
            break;
        case Escape:
            value = "Escape";
            break;
        case Space:
            value = "Space Bar";
            break;
        case PageUp:
            value = "Page Up";
            break;
        case PageDown:
            value = "Page Down";
            break;
        case End:
            value = "End";
            break;
        case Home:
            value = "Home";
            break;
        case LeftArrow:
            value = "Left Arrow";
            break;
        case UpArrow:
            value = "Up Arrow";
            break;
        case RightArrow:
            value = "Right Arrow";
            break;
        case DownArrow:
            value = "Down Arrow";
            break;
        case Select:
            value = "Select";
            break;
        case Print:
            value = "Print";
            break;
        case Execute:
            value = "Execute";
            break;
        case PrintScreen:
            value = "Print Screen";
            break;
        case Insert:
            value = "Insert";
            break;
        case Delete:
            value = "Delete";
            break;
        case Help:
            value = "Help";
            break;
        case Zero:
            value = "0";
            break;
        case One:
            value = "1";
            break;
        case Two:
            value = "2";
            break;
        case Three:
            value = "3";
            break;
        case Four:
            value = "4";
            break;
        case Five:
            value = "5";
            break;
        case Six:
            value = "6";
            break;
        case Seven:
            value = "7";
            break;
        case Eight:
            value = "8";
            break;
        case Nine:
            value = "9";
            break;        
        case A:
            value = "A";
            break;
        case B:
            value = "B";
            break;
        case C:
            value = "C";
            break;
        case D:
            value = "D";
            break;
        case E:
            value = "E";
            break;
        case F:
            value = "F";
            break;
        case G:
            value = "G";
            break;
        case H:
            value = "H";
            break;
        case I:
            value = "I";
            break;
        case J:
            value = "J";
            break;
        case K:
            value = "K";
            break;
        case L:
            value = "L";
            break;
        case M:
            value = "M";
            break;
        case N:
            value = "N";
            break;
        case O:
            value = "O";
            break;
        case P:
            value = "P";
            break;
        case Q:
            value = "Q";
            break;
        case R:
            value = "R";
            break;
        case S:
            value = "S";
            break;
        case T:
            value = "T";
            break;
        case U:
            value = "U";
            break;
        case V:
            value = "V";
            break;
        case W:
            value = "W";
            break;
        case X:
            value = "X";
            break;
        case Y:
            value = "Y";
            break;
        case Z:
            value = "Z";
            break;
        case Numpad0:
            value = "Numpad 0";
            break;
        case Numpad1:
            value = "Numpad 1";
            break;
        case Numpad2:
            value = "Numpad 2";
            break;
        case Numpad3:
            value = "Numpad 3";
            break;
        case Numpad4:
            value = "Numpad 4";
            break;
        case Numpad5:
            value = "Numpad 5";
            break;
        case Numpad6:
            value = "Numpad 6";
            break;
        case Numpad7:
            value = "Numpad 7";
            break;
        case Numpad8:
            value = "Numpad 8";
            break;
        case Numpad9:
            value = "Numpad 9";
            break;
        case Multiply:
            value = "Multiply";
            break;
        case Add:
            value = "Add";
            break;
        case Separator:
            value = "Separator";
            break;
        case Subtract:
            value = "Subtract";
            break;
        case Decimal:
            value = "Decimal";
            break;
       case Divide:
            value = "Divide";
            break;
        case F1:
            value = "F1";
            break;
        case F2:
            value = "F2";
            break;
        case F3:
            value = "F3";
            break;
        case F4:
            value = "F4";
            break;
        case F5:
            value = "F5";
            break;
        case F6:
            value = "F6";
            break;
        case F7:
            value = "F7";
            break;
        case F8:
            value = "F8";
            break;
        case F9:
            value = "F9";
            break;
        case F10:
            value = "F10";
            break;
        case F11:
            value = "F11";
            break;
        case F12:
            value = "F12";
            break;
        case NumLock:
            value = "Num Lock";
            break;
        case ScrollLock:
            value = "Scroll Lock";
            break;
        case LeftShift:
            value = "Left Shift";
            break;
        case RightShift:
            value = "Right Shift";
            break;
        case LeftControl:
            value = "Left Control";
            break;
        case RightControl:
            value = "Right Control";
            break;
        case Comma:
            value = "Comma";
            break;
        case Period:
            value = "Period";
            break;
        case LeftAlt:
            value = "Left Alt";
            break;
        case RightAlt:
            value = "Right Alt";
            break;
        default:
            value = "Unknown";
            break;
    }
    return value;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to convert a string to a key value.
///
///   \param[in] strKey String verion of key to convert to enumeration.
///
///   \return "Invalid" on failure, otherwise a enumeration of the key.
///
////////////////////////////////////////////////////////////////////////////////////
Keyboard::Key Keyboard::FromString(const std::string& strKey)
{
    Key value;

    if(strKey == "Backspace") 
    {
        value = Backspace;
    }
    else if(strKey == "Tab") 
    {
        value = Tab;
    }
    else if(strKey == "Clear")
    {
        value = Clear;
    }
    else if(strKey == "Return")
    {
        value = Return;
    }
    else if(strKey == "Shift Lock") 
    {
        value = ShiftLock;
    }
    else if(strKey == "Pause") 
    {
        value = Pause;
    }
    else if(strKey == "Caps Lock") 
    {
        value = CapsLock;
    }
    else if(strKey == "Escape") 
    {
        value = Escape;
    }
    else if(strKey == "Space Bar") 
    {
        value = Space;
    }
    else if(strKey == "Page Up") 
    {
        value = PageUp;
    }
    else if(strKey == "Page Down") 
    {
        value = PageDown;
    }
    else if(strKey == "End") 
    {
        value = End;
    }
    else if(strKey == "Home") 
    {
        value = Home;
    }
    else if(strKey == "Left Arrow") 
    {
        value = LeftArrow;
    }
    else if(strKey == "Up Arrow") 
    {
        value = UpArrow;
    }
    else if(strKey == "Right Arrow") 
    {
        value = RightArrow;
    }
    else if(strKey == "Down Arrow") 
    {
        value = DownArrow;
    }
    else if(strKey == "Select") 
    {
        value = Select;
    }
    else if(strKey == "Print") 
    {
        value = Print;
    }
    else if(strKey == "Execute")
    {
        value = Execute;
    }
    else if(strKey == "Print") 
    {
        value = Print;
    }
    else if(strKey == "Print Screen") 
    {
        value = PrintScreen;
    }
    else if(strKey == "Insert") 
    {
        value = Insert;
    }
    else if(strKey == "Delete") 
    {
        value = Delete;
    }
    else if(strKey == "Help") 
    {
        value = Help;
    }
    else if(strKey == "0") 
    {
        value = Zero;
    }
    else if(strKey == "1") 
    {
        value = One;
    }
    else if(strKey == "2") 
    {
        value = Two;
    }
    else if(strKey == "3") 
    {
        value = Three;
    }
    else if(strKey == "4")
    {
        value = Four;
    }
    else if(strKey == "5") 
    {
        value = Five;
    }
    else if(strKey == "6") 
    {
        value = Six;
    }
    else if(strKey == "7") 
    {
        value = Seven;
    }
    else if(strKey == "8") {
        value = Eight;
    }
    else if(strKey == "9") 
    {
        value = Nine;
    }        
    else if(strKey == "A") 
    {
        value = A;
    }
    else if(strKey == "B") 
    {
        value = B;
    }
    else if(strKey == "C") 
    {
        value = C;
    }
    else if(strKey == "D") 
    {
        value = D;
    }
    else if(strKey == "E") 
    {
        value = E;
    }
    else if(strKey == "F") 
    {
        value = F;
    }
    else if(strKey == "G") 
    {
        value = G;
    }
    else if(strKey == "H") 
    {
        value = H;
    }
    else if(strKey == "I") 
    {
        value = I;
    }
    else if(strKey == "J") 
    {
        value = J;
    }
    else if(strKey == "K") 
    {
        value = K;
    }
    else if(strKey == "L") 
    {
        value = L;
    }
    else if(strKey == "M") 
    {
        value = M;
    }
    else if(strKey == "N") 
    {
        value = N;
    }
    else if(strKey == "O") 
    {
        value = O;
    }
    else if(strKey == "P") 
    {
        value = P;
    }
    else if(strKey == "Q") 
    {
        value = Q;
    }
    else if(strKey == "R") 
    {
        value = R;
    }
    else if(strKey == "S") 
    {
        value = S;
    }
    else if(strKey == "T") 
    {
        value = T;
    }
    else if(strKey == "U") 
    {
        value = U;
    }
    else if(strKey == "V") 
    {
        value = V;
    }
    else if(strKey == "W") 
    {
        value = W;
    }
    else if(strKey == "X") 
    {
        value = X;
    }
    else if(strKey == "Y") 
    {
        value = Y;
    }
    else if(strKey == "Z") 
    {
        value = Z;
    }
    else if(strKey == "Numpad 0") 
    {
        value = Numpad0;
    }
    else if(strKey == "Numpad 1") 
    {
        value = Numpad1;
    }
    else if(strKey == "Numpad 2") 
    {
        value = Numpad2;
    }
    else if(strKey == "Numpad 3") 
    {
        value = Numpad3;
    }
    else if(strKey == "Numpad 4") 
    {
        value = Numpad4;
    }
    else if(strKey == "Numpad 5") 
    {
        value = Numpad5;
    }
    else if(strKey == "Numpad 6") 
    {
        value = Numpad6;
    }
    else if(strKey == "Numpad 7") 
    {
        value = Numpad7;
    }
    else if(strKey == "Numpad 8") 
    {
        value = Numpad8;
    }
    else if(strKey == "Numpad 9") 
    {
        value = Numpad9;
    }
    else if(strKey == "Multiply") 
    {
        value = Multiply;
    }
    else if(strKey == "Add") 
    {
        value = Add;
    }
    else if(strKey == "Separator") 
    {
        value = Separator;
    }
    else if(strKey == "Subtract") 
    {
        value = Subtract;
    }
    else if(strKey == "Decimal") 
    {
        value = Decimal;
    }
    else if(strKey == "Divide") 
    {
        value = Divide;
    }
    else if(strKey == "F1") 
    {
        value = F1;
    }
    else if(strKey == "F2") 
    {
        value = F2;
    }
    else if(strKey == "F3") 
    {
        value = F3;
    }
    else if(strKey == "F4") 
    {
        value = F4;
    }
    else if(strKey == "F5") 
    {
        value = F5;
    }
    else if(strKey == "F6") 
    {
        value = F6;
    }
    else if(strKey == "F7") 
    {
        value = F7;
    }
    else if(strKey == "F8") 
    {
        value = F8;
    }
    else if(strKey == "F9") 
    {
        value = F9;
    }
    else if(strKey == "F10") 
    {
        value = F10;
    }
    else if(strKey == "F11") 
    {
        value = F11;
    }
    else if(strKey == "F12") 
    {
        value = F12;
    }
    else if(strKey == "Num Lock") 
    {
        value = NumLock;
    }
    else if(strKey == "Scroll Lock") 
    {
        value = ScrollLock;
    }
    else if(strKey == "Left Shift") 
    {
        value = LeftShift;
    }
    else if(strKey == "Right Shift") 
    {
        value = RightShift;
    }
    else if(strKey == "Left Control") 
    {
        value = LeftControl;
    }
    else if(strKey == "Right Control") 
    {
        value = RightControl;
    }
    else if(strKey == "Comma") 
    {
        value = Comma;
    }
    else if(strKey == "Period") 
    {
        value = Period;
    }
    else if(strKey == "Left Alt") 
    {
        value = LeftAlt;
    }
    else if(strKey == "Right Alt") 
    {
        value = RightAlt;
    }
    else 
    {
        value = Invalid;
    }

    return value;
}

#ifdef WIN32
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to look up a hardware scan code used by DirectX.  This
///   is needed because in Windows, DirectX doesn't use Virtual Keys like 
///   EVERY OTHER APPLIATION ON THE OS, WHY WHY WHY!
///
///   I got these mappings from:
///   http://www.gamespp.com/directx/directInputKeyboardScanCodes.html
///
///   \param[in] key The key to perform lookup for.
///
///   \return 0 on failure, otherwise the DirectX scan code value match.
///
////////////////////////////////////////////////////////////////////////////////////
int Keyboard::LookupScanCodeDX(const Key key)
{
    unsigned int value = 0;

    switch(key)
    {
        case Backspace:
            value = 0x0E;
            break;
        case Tab:
            value = 0x0F;
            break;
        case Return:
            value = 0x1C;
            break;
        case CapsLock:
            value = 0x3A;
            break;
        case Escape:
            value = 0x01;
            break;
        case Space:
            value = 0x39;
            break;
        case PageUp:
            value = 0xC9;
            break;
        case PageDown:
            value = 0xD1;
            break;
        case End:
            value = 0xCF;
            break;
        case Home:
            value = 0xC7;
            break;
        case LeftArrow:
            value = 0xCB;
            break;
        case UpArrow:
            value = 0xC8;
            break;
        case RightArrow:
            value = 0xCD;
            break;
        case DownArrow:
            value = 0xD0;
            break;
        case Insert:
            value = 0xD2;
            break;
        case Delete:
            value = 0xD3;
            break;
        case Zero:
            value = 0x0B;
            break;
        case One:
            value = 0x02;
            break;
        case Two:
            value = 0x03;
            break;
        case Three:
            value = 0x04;
            break;
        case Four:
            value = 0x05;
            break;
        case Five:
            value = 0x06;
            break;
        case Six:
            value = 0x07;
            break;
        case Seven:
            value = 0x08;
            break;
        case Eight:
            value = 0x09;
            break;
        case Nine:
            value = 0x0A;
            break;        
        case A:
            value = 0x1E;
            break;
        case B:
            value = 0x30;
            break;
        case C:
            value = 0x2E;
            break;
        case D:
            value = 0x20;
            break;
        case E:
            value = 0x12;
            break;
        case F:
            value = 0x21;
            break;
        case G:
            value = 0x22;
            break;
        case H:
            value = 0x23;
            break;
        case I:
            value = 0x17;
            break;
        case J:
            value = 0x24;
            break;
        case K:
            value = 0x25;
            break;
        case L:
            value = 0x26;
            break;
        case M:
            value = 0x32;
            break;
        case N:
            value = 0x31;
            break;
        case O:
            value = 0x18;
            break;
        case P:
            value = 0x19;
            break;
        case Q:
            value = 0x10;
            break;
        case R:
            value = 0x13;
            break;
        case S:
            value = 0x1F;
            break;
        case T:
            value = 0x14;
            break;
        case U:
            value = 0x16;
            break;
        case V:
            value = 0x2F;
            break;
        case W:
            value = 0x11;
            break;
        case X:
            value = 0x2D;
            break;
        case Y:
            value = 0x15;
            break;
        case Z:
            value = 0x2C;
            break;
        case Numpad0:
            value = 0x52;
            break;
        case Numpad1:
            value = 0x4F;
            break;
        case Numpad2:
            value = 0x50;
            break;
        case Numpad3:
            value = 0x51;
            break;
        case Numpad4:
            value = 0x4B;
            break;
        case Numpad5:
            value = 0x4C;
            break;
        case Numpad6:
            value = 0x4D;
            break;
        case Numpad7:
            value = 0x47;
            break;
        case Numpad8:
            value = 0x48;
            break;
        case Numpad9:
            value = 0x49;
            break;
        case Multiply:
            value = 0x37;
            break;
        case Add:
            value = 0x4E;
            break;
        case Subtract:
            value = 0x4A;
            break;
        case Decimal:
            value = 0x53;
            break;
        case Divide:
            value = 0xB5;
            break;
        case F1:
            value = 0x3B;
            break;
        case F2:
            value = 0x3C;
            break;
        case F3:
            value = 0x3D;
            break;
        case F4:
            value = 0x3E;
            break;
        case F5:
            value = 0xEF;
            break;
        case F6:
            value = 0x40;
            break;
        case F7:
            value = 0x41;
            break;
        case F8:
            value = 0x42;
            break;
        case F9:
            value = 0x43;
            break;
        case F10:
            value = 0x44;
            break;
        case F11:
            value = 0x57;
            break;
        case F12:
            value = 0xF8;
            break;
        case NumLock:
            value = 0x45;
            break;
        case ScrollLock:
            value = 0x46;
            break;
        case LeftShift:
            value = 0x2A;
            break;
        case RightShift:
            value = 0x36;
            break;
        case LeftControl:
            value = 0x1D;
            break;
        case RightControl:
            value = 0x9D;
            break;
        case Comma:
            value = 0x33;
            break;
        case Period:
            value = 0x34;
            break;
        case LeftAlt:
            value = 0x38;
            break;
        case RightAlt:
            value = 0xB8;
            break;
        default:
            value = 0;
            break;
    }
    return value;
}

#endif


/*  End of File */
