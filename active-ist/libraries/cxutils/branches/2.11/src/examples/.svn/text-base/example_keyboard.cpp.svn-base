////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_keyboard.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Keyboard.
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
#include <iostream>
#include <vector>
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

class KeyboardCallback : public Keyboard::Callback
{
    virtual void ProcessKeyEvent(const Keyboard::Key key, const bool down)
    {
        // Simulate DirectX key presses.
        switch(key)
        {
        case Keyboard::UpArrow:
            if(down)
            {
                Keyboard::FakeKeyPress(Keyboard::W, false, true);
            }
            else
            {
                Keyboard::FakeKeyRelease(Keyboard::W, true);
            }
            break;
        case Keyboard::DownArrow:
            if(down)
            {
                Keyboard::FakeKeyPress(Keyboard::S, false, true);
            }
            else
            {
                Keyboard::FakeKeyRelease(Keyboard::S, true);
            }
            break;
        case Keyboard::LeftArrow:
            if(down)
            {
                Keyboard::FakeKeyPress(Keyboard::A, false, true);
            }
            else
            {
                Keyboard::FakeKeyRelease(Keyboard::A, true);
            }
            break;
        case Keyboard::RightArrow:
            if(down)
            {
                Keyboard::FakeKeyPress(Keyboard::D, false, true);
            }
            else
            {
                Keyboard::FakeKeyRelease(Keyboard::D, true);
            }
            break;
        default:
            break;
        };
    }
};

int main(int argc, char **argv)
{
    Keyboard::Key key;
    Keyboard keyboard;
    KeyboardCallback callback;

    keyboard.RegisterCallback(&callback);

    while(true)
    {
        if(keyboard.IsKeyDown(Keyboard::Any))
        {
            key = keyboard.GetCurrentKey();
            cout << "Pressed: " << Keyboard::ToString(key) << endl;
            if(key == Keyboard::Escape)
                break;

            if(key == Keyboard::M)
            {
                // Fake button presss
                Mouse::FakeButtonPress(Mouse::LeftButton, false);
            }
            if(key == Keyboard::U)
            {
                Mouse::FakeButtonRelease(Mouse::LeftButton);
            }
        }
        
        if(Mouse::IsButtonDown())
        {
            double x, y;
            Mouse::GetCursorPosition(x, y);
            cout << "Mouse Clicked [" << x << ", " << y << "] - " << hex << Mouse::GetCurrentButton() << endl;
        }                                                                           
        // Test setting cursor to absolute position.
        //Mouse::SetCursorPosition(0.5, 0.5, true);
        // Test relative movement of cursor in pixels (bottom right movement).
        //Mouse::SetCursorPosition(1, 1, false);
        
        // Test generating key event
        //Keyboard::FakeKeyPress(Keyboard::LeftShift, false);
        //Keyboard::FakeKeyPress(Keyboard::Zero, true);
        //Keyboard::FakeKeyRelease(Keyboard::LeftShift);
        CxUtils::SleepMs(50);
    }
    cout << "Exiting...\n";
    return 0;
}
                                                                                                                                                                                                                          
/*  End of File */
                                                                                                     
