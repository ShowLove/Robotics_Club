//////////////////////////////////////////////////////////////////////////////////////
///
/// \file example.cpp
/// \brief Example/Test Program for Coral AHRS Interface.
///
/// Author(s): Daniel Barber<br>
/// Created: 1/17/2009<br>
/// Copyright (c) 2009<br>
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
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
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
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cxutils/math/cxmath.h>
#include <cxutils/keyboard.h>
#include "coralahrs/interface.h"

using namespace std;
using namespace Zebulon;
using namespace Compass;

class MyCallback : public CoralAHRS::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    virtual void ProcessMessage(const CoralAHRS::Message* message)
    {
        message->Print();
    }
};

int main(int argc, char* argv[])
{
    MyCallback callback;
    CoralAHRS::Ping ping;
    CoralAHRS::RequestConfiguration requestConf;
    CoralAHRS::Interface coral;
    CoralAHRS::SetOutputMode outputMode;
    int running = 0;

    cout << "Connecting to Coral AHRS Digital Compass...\n";
    if(argc > 1)
    {
        running = coral.Initialize(argv[1], 57600, false, 5000);
    }
    else
    {
        running = coral.Initialize("/dev/ttyUSB1", 57600);
    }

    if(running)
    {
        cout << "Success!\n";
        int key = 0;
        cout << "Press <escape key> To Exit!\n";
        outputMode.mMode = (unsigned char) (CoralAHRS::SetOutputMode::DataEuler | CoralAHRS::SetOutputMode::Calibrated);
        coral.Send(&outputMode);
        
        coral.RegisterCallback(&callback);

        while(key != 27)
        {
            cout << "Ping!\n";
            coral.Send(&ping);
            cout << "Requesting\n";
            coral.Send(&requestConf);
            key = CxUtils::GetChar();
            CxUtils::SleepMs(500);
        }
    }
    else
    {
        cout << "Failure!\n";
    }    

    return 0;
}

/*  End of File */
