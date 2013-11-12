/////////////////////////////////////////////////////////////////////////////
///
/// \file example_ax3500.cpp
/// \brief Example application showing how to use the AX3500 interface
/// class.
///
/// Author(s): Daniel Barber<br>
/// Created: 5/3/2008<br>
/// Last Modified: 5/3/2008<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
/////////////////////////////////////////////////////////////////////////////
#include "roboteq/ax3500.h"
#include <iostream>
#include <stdio.h>
#include <cxutils/keyboard.h>

using namespace Zebulon;
using namespace Motor;
using namespace Roboteq;
using namespace std;

class MyCallback : public AX3500::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    void ProcessMessage(const AX3500::Message* message, const unsigned int boardID)
    {
        std::cout << message->mString << std::endl;
    }
};

int main(int argc, char **argv)
{
    AX3500 motors;
    MyCallback myCallback;

    motors.RegisterCallback(&myCallback);

    double channel1 = 0;
    double channel2 = 0;
    bool direction1 = true, direction2 = true;
    cout << "Connecting to AX3500...";
    if(motors.Initialize("/dev/ttyACM0") == 0)
    {
        cout << "Failed!\n";
        return 0;
    }
    else
    {
        cout << "Success!\n";
    }
    cout << "Press <escape> to Exit...\n";
    CxUtils::SleepMs(1000);

    bool quitFlag = false;
    while(!quitFlag)
    {
        int key = CxUtils::GetChar();
        // Check for escape key.
        switch(key)
        {
        case 'w':
            motors.EnterWatchdogMode();
            break;
        case 'W':
            motors.EnterWatchdogMode();
            break;
        case 'r':
            motors.ResetController();
            break;
        case 'R':
            motors.ResetController();
            break;
        case 'q':
            {
                double v1, v2;
                if(motors.QueryMainBatteryVoltage(v1, v2))
                {
                    cout << "Main Battery Voltage: " << v1 << endl;
                    cout << "Internal Battery Voltage: " << v2 << endl;
                }
            }
            break;
        case 'e':
            {
                int e1, e2;
                e1 = e2 = 0;
                if(motors.QueryEncoders(e1, e2, true))
                {
                    cout << "Encoders: " << e1 << " " << e2 << endl;
                }
            }
            break;
        case 27:
            quitFlag = true;
            break;
        default:
            break;
        }
        char message[50];
        motors.SendDualMotorCommand(channel2, -1*channel1);
        if(direction1)
        {
            channel1 += 2.0;
        }
        else
        {
            channel1 -= 2.0;
        }
        if(channel1 > 100.0)
        {
            channel1 = 100.0;
            direction1 = false;
        }
        else if(channel1 < -100.0)
        {
            channel1 = -100.0;
            direction1 = true;
        }
        
        CxUtils::SleepMs(100);
    }

    return 0;
}


/*  End of File */
