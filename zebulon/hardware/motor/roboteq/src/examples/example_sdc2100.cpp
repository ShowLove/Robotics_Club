/////////////////////////////////////////////////////////////////////////////
///
/// \file example_ax3500.cpp
/// \brief Example application showing how to use the AX3500 interface
/// class.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Last Modified: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
#include "roboteq/sdc2100.h"
#include <iostream>
#include <stdio.h>
#include <cxutils/keyboard.h>

using namespace Zebulon;
using namespace Motor;
using namespace Roboteq;
using namespace std;

class MyCallback : public SDC2100::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    void ProcessMessage(const SDC2100::Message* message, const unsigned int boardID)
    {
        std::cout << message->mString << std::endl;
    }
};

int main(int argc, char **argv)
{
    SDC2100 motors;
    MyCallback myCallback;

    motors.RegisterCallback(&myCallback);

    double channel1 = 0;
    double channel2 = 0;
    double counter = 0;
    bool direction1 = true, direction2 = true;
    cout << "Connecting to SDC2100...";
    if(motors.Initialize("/dev/ttyACM1") == 0)
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
        //int key = CxUtils::GetChar();
        // Check for escape key.
//        switch(key)
//        {
//        case 'w':
//            break;
//        case 'W':
//            break;
//        case 'r':
//            break;
//        case 'R':
//            break;
//        case 'q':
//            break;
//        case 'e':
//            break;
//        case 27:
//            quitFlag = true;
//            break;
//        default:
//            break;
//        }
        //char message[50];
        //motors.SendDualMotorCommand(channel2, channel1);
        if(motors.SendMotorCommand(1,channel1) != 1)
        {
            cout << "Motor Command Failed" << endl;
            break;
        }
        //motors.SendDualMotorCommand(-100, -100);
        channel1 = sin(counter)*100;
        channel2 = -sin(counter)*100;
        counter+=.01;
        CxUtils::SleepMs(10);
    }

    return 0;
}


/*  End of File */
