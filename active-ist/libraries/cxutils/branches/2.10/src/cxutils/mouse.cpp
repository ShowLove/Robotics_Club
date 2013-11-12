////////////////////////////////////////////////////////////////////////////////////
///
///  \file mouse.cpp
///  \brief Main include file which contains methods for capturing and generating
///  mouse events.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 10 March 2009
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
#include "cxutils/mouse.h"
#include <limits.h>
#ifdef WIN32
#define _WIN32_WINNT 0x0501
#include <windows.h>
#else
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XTest.h>
#endif

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if button on the mouse is being pressed/clicked.
///
///   \param[in] button The mouse button to check if it is clicked.
///
///   \return True if the button is currently down.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::IsButtonDown(const Mouse::Button button)
{
    bool result = false;
#ifdef WIN32
    SHORT _button = 0;
    if(button == Any)
    {
        // Only check for mouse button clicks.
        for(int b = 0x01; b <= 0x06; b++)
        {
            if(b != 0x03)
            {
                if(IsButtonDown((Button)b))
                {
                    result = true;
                }
            }
        }
    }    
    else
    {
        _button  = ::GetAsyncKeyState(button);
        if( (_button & 0x8000) != 0 )
        {
            result = true;
        }
    }
#else
    Window root, child;
    int dummyx, dummyy;
    int _x, _y;
    unsigned int mask = 0;
    Display* display = XOpenDisplay(NULL);
    if(XQueryPointer(display,
                     DefaultRootWindow(display),
                     &root,
                     &child,
                     &_x,
                     &_y,
                     &dummyx,
                     &dummyy,
                     &mask) != 0)
     {
         unsigned int bmask = Button1Mask | Button2Mask | Button3Mask;
         if( (mask & bmask) > 0)
         {
            result = true;
         }
     }
     XCloseDisplay(display);
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current button being pressed.
///
///   \return The value of the current button that is down.
///
////////////////////////////////////////////////////////////////////////////////////
Mouse::Button Mouse::GetCurrentButton()
{
    Mouse::Button pressed = Invalid;
#ifdef WIN32
    // Only check for mouse button clicks.
    for(int b = 0x01; b <= 0x06; b++)
    {
        if(b != 0x03)
        {
            if(IsButtonDown((Button)b))
            {
                pressed = (Button)b;
                break;
            }
        }
    }
#else
    Window root, child;
    int dummyx, dummyy;
    int _x, _y;
    unsigned int mask = 0;
    Display* display = XOpenDisplay(NULL);
    if(XQueryPointer(display,
                     DefaultRootWindow(display),
                     &root,
                     &child,
                     &_x,
                     &_y,
                     &dummyx,
                     &dummyy,
                     &mask) != 0)
    {
        if( (mask & Button1Mask) > 0)
        {
            pressed = LeftButton;
        }
        else if( (mask & Button2Mask) > 0)
        {
            pressed = MiddleButton;
        }
        else if( (mask & Button3Mask) > 0)
        {
            pressed = RightButton;
        }
    }
    XCloseDisplay(display);
#endif

    return pressed;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to fake a mouse button press.
///
///   \param[in] button The mouse button to simulate a click for.
///   \param[in] releaseFlag If true, the method will also generate a 
///                          key up command after the key down event.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::FakeButtonPress(const Mouse::Button button,
                            const bool releaseFlag)
{
    bool result = false;
    if(button != Invalid && button != Any)
    {
#ifdef WIN32
        ::INPUT m;
        m.type = INPUT_MOUSE;
        m.mi.dx = 0;
        m.mi.dy = 0;
        m.mi.mouseData = 0;
        m.mi.time = 0;
        switch(button)
        {
        case Mouse::RightButton:
            m.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
            break;
        case Mouse::MiddleButton:
            m.mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
            break;
        default:
            m.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
            break;
        };
        m.mi.dwExtraInfo = 0;
        if(::SendInput(1, &m, sizeof(::INPUT)) > 0)
        {
            result = true;
            if(releaseFlag)
            {
                FakeButtonRelease(button);
            }
        }
#else
        Display* display = XOpenDisplay(NULL);
        if(display)
        {
            unsigned int _button = 1;
            switch(button)
            {
                case LeftButton:
                    _button = 1;
                    break;
                case RightButton:
                    _button = 3;
                    break;
                case MiddleButton:
                    _button = 2;
                    break;
                default:
                    _button = 1;
                    break;
            };
            if(XTestFakeButtonEvent(display, _button, true, CurrentTime) != 0)
            {
                result = true;
                if(releaseFlag)
                {
                    XTestFakeButtonEvent(display, _button, false, CurrentTime);
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
///   \brief Method used to fake a mouse button release.
///
///   \param[in] button The mouse button to simulate a release for.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::FakeButtonRelease(const Mouse::Button button)
{
    bool result = false;
    if(button != Invalid && button != Any)
    {
#ifdef WIN32
        ::INPUT m;
        m.type = INPUT_MOUSE;
        m.mi.dx = 0;
        m.mi.dy = 0;
        m.mi.mouseData = 0;
        m.mi.time = 0;
        switch(button)
        {
        case Mouse::RightButton:
            m.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
            break;
        case Mouse::MiddleButton:
            m.mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
            break;
        default:
            m.mi.dwFlags = MOUSEEVENTF_LEFTUP;
            break;
        };
        m.mi.dwExtraInfo = 0;
        if(::SendInput(1, &m, sizeof(::INPUT)) > 0)
        {
            result = true;
        }
#else
        Display* display = XOpenDisplay(NULL);
        if(display)
        {
            unsigned int _button = 1;
            switch(button)
            {
                case LeftButton:
                    _button = 1;
                    break;
                case RightButton:
                    _button = 3;
                    break;
                case MiddleButton:
                    _button = 2;
                    break;
                default:
                    _button = 1;
                    break;
            };
            if(XTestFakeButtonEvent(display, _button, false, CurrentTime) != 0)
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
///   \brief Sets the mouse cursor to an absolute position or moves it by
///          a number of pixels relative to current position.
///
///   \param[in] dx The x position of the cursor normalized to [0,1] or the
///                 number of pixels the mouse has moved to the left or right.
///                 Positive relative movements are to the right, negative to
///                 the left.
///   \param[in] dy The y position of the cursor normalized to [0,1] or the
///                 number of pixels the mouse has moved up or down.  A positive
///                 value is a movement down.
///   \param[in] absoluteFlag If true, dx and dy are absolute positions on the
///                           screen to move the mouse.  If false, then dx an dy
///                           are relative movements in pixels.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::SetCursorPosition(const double dx,
                              const double dy,
                              const bool absoluteFlag)
{
    bool result = false;
#ifdef WIN32
    ::INPUT in;
    in.type = INPUT_MOUSE;
    in.mi.dx = (LONG)dx;
    in.mi.dy = (LONG)dy;
    in.mi.mouseData = 0;
    in.mi.dwFlags = MOUSEEVENTF_MOVE;
    if(absoluteFlag)
    {
        in.mi.dx = (USHORT)(dx*USHRT_MAX);
        in.mi.dy = (USHORT)(dy*USHRT_MAX);
        in.mi.dwFlags |= MOUSEEVENTF_ABSOLUTE;
    }
    in.mi.time = 0;
    in.mi.dwExtraInfo = 0;
    if(::SendInput(1, &in, sizeof(::INPUT)) > 0)
    {
        result = true;
    }
#else
        Display* display = XOpenDisplay(NULL);
        if(display)
        {
            if(absoluteFlag)
            {
                int _x, _y;
                unsigned int w, h;
                GetScreenResolution(w, h);
                _x = (int)(dx*w);
                _y = (int)(dy*h);
                if(XTestFakeMotionEvent(display, DefaultScreen(display), _x, _y, CurrentTime) > 0)
                {
                    result = true;
                }
            }
            else
            {
                if(XTestFakeRelativeMotionEvent(display, dx, dy, CurrentTime) > 0)
                {
                    result = true;
                }
            }
            
            XCloseDisplay(display);
        }
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current position of the mouse cursor.
///
///   \param[out] x The x position of the cursor.  0 is the left side increasing
///                 positive to the right.
///   \param[out] y The y position of the cursor. 0 is the top of the screen
///                 increasing as you go down.
///   \param[in] normalized If true, then the values of x and y are normalized to
///                         [0,1].  If false, the values are in screen pixels.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::GetCursorPosition(double& x,
                              double& y,
                              const bool normalized)
{
    bool result = false;
    x = y = 0;
#ifdef WIN32
    ::POINT pos;
    if(::GetCursorPos(&pos))
    {
        x = pos.x;
        y = pos.y;
        if(normalized)
        {
            unsigned int w = 1, h = 1;
            GetScreenResolution(w, h);
            x = x/w;
            y = y/h;
        }
        result = true;
    }
#else
    Window root, child;
    int dummyx, dummyy;
    int _x, _y;
    unsigned int mask = 0;
    Display* display = XOpenDisplay(NULL);
    if(XQueryPointer(display,
                     DefaultRootWindow(display),
                     &root,
                     &child,
                     &_x,
                     &_y,
                     &dummyx,
                     &dummyy,
                     &mask) != 0)
     {
         x = _x;
         y = _y;
         if(normalized)
         {
            unsigned int w = 1, h = 1;
            GetScreenResolution(w, h);
            x = x/w;
            y = y/h;
         }
         result = true;
     }
     XCloseDisplay(display);
#endif
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the current screen resolution.
///
///   \param[out] width Screen width in pixels.
///   \param[out] height Screen height in pixels.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Mouse::GetScreenResolution(unsigned int& width, unsigned int& height)
{
#ifdef WIN32
    RECT desktop;  
    // Get a handle to the desktop window  
    const HWND hDesktop = GetDesktopWindow();  
    // Get the size of screen to the variable desktop  
    GetWindowRect(hDesktop, &desktop);  
    // The top left corner will have coordinates (0,0)  
    // and the bottom right corner will have coordinates  
    // (horizontal, vertical)  
    width = (unsigned int)desktop.right;  
    height = (unsigned int)desktop.bottom; 
    return true;
#else
    Display* display;
    display = XOpenDisplay(NULL);
    width = (unsigned int)XDisplayWidth(display, 0);
    height = (unsigned int)XDisplayHeight(display, 0);
    XCloseDisplay(display);
    return false;
#endif
}

/* End of File */
