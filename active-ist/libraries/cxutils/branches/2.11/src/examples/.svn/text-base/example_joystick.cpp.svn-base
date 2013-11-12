////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_joystick.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  Joystick class.
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
#include <iostream>
#include <vector>
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;


class MyCallback : public Joystick::Callback
{
    virtual void ProcessJoystickEvent(const Joystick& jstick)
    {
        jstick.PrintJoystickInfo();
    }
    virtual void ProcessButtonEvent(const unsigned int i, const Joystick::Event type)
    {
        if(type == Joystick::ButtonDown)
        {
            cout << "Button " << i << " is Down.\n";
        }
        else if(type == Joystick::ButtonUp)
        {
            cout << "Button " << i << " is Up.\n";
        }
        SleepMs(250);
    }
};

int main(int argc, char **argv)
{
    Joystick j;
    MyCallback callback;
    std::string calibrationFile;
    
    if(argc > 1)
    {
        calibrationFile = argv[1];
    }
    
    if(j.Initialize(Joystick::AnyJoystick, 20, calibrationFile) == 0) 
    {
        cout << "No Joystick Connected.\n";
        return 0;
    }
    
    cout << "Connected to joystick!\n";
    // You can use a callback, or poll for values.
    // This examples uses a Callback (preferred).
    j.RegisterCallback(&callback);
    while(true) 
    {
        //j.PrintJoystickInfo();
        if(GetChar() == 27)
        {
            break;
        }
        SleepMs(100);
    }
    return 0;
}

/*  End of File */
