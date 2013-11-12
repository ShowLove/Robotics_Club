//////////////////////////////////////////////////////////////////////////////////////
///
/// \file example.cpp
/// \brief Example/Test Program for MCU Interface
///
/// Author(s): David Adams<br>
/// Created: 1/17/2009<br>
/// Copyright (c) 2009<br>
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
///      * Neither the name of the Robotics Laboratory, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREJAUS global pose sensorMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
//////////////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <cxutils/math/cxmath.h>
#include <cxutils/cxutils.h>
#include "mcu/slipinterface.h"
#include "mcu/messagefactory.h"

using namespace std;
using namespace Zebulon;

class MyCallback : public MCU::SlipInterface::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    virtual void ProcessMessage(const MCU::Message* message)
    {
        //cout << "Recieving Message: \n";
        message->Print();
    }
};

int main(int argc, char * argv[])
{
    MCU::SlipInterface mcu;
    
	MCU::Ping ping;
	ping.mUseStaticFormatFlag = false;

	MCU::SetAnalogOutputs l;
	l.mUseStaticFormatFlag = false;
    
	MCU::SetAnalogOutputs r;
	r.mUseStaticFormatFlag = false;
    
	l.mPort = 0;
	l.mValue = 0 + 265;

	r.mPort = 1;
	r.mValue = 0 + 265;

    MyCallback callback;
    
    bool running = false;

    cout << "Connecting to MCU...\n";
    if(argc > 1)
    {
        running = mcu.Initialize(argv[1], 57600);
    }
    else
    {
        cout << "Default Baud\n";
        running = mcu.Initialize("/dev/asv-mcu", 57600, false);
    }
    mcu.RegisterCallback(&callback);
    
    if(running)
    {
        cout << "Success!\n";
        char key = 0;
        cout << "Press <escape key> To Exit!\n";
        
        while(key != 27)
        {

            //ping.Print();
            mcu.Send(&ping);
            //CxUtils::SleepMs(100);
            //mcu.Send(&l);
            //CxUtils::SleepMs(100);
            //mcu.Send(&r);
            //key = CxUtils::GetChar();

//            MCU::SetMotorOutput * msg = new MCU::SetMotorOutput();
//            msg->mUseStaticFormatFlag = false;
//            msg->mMotors[0] = 100;
//            msg->mMotors[1] = 0;
//            msg->mMotors[2] = 0;
//            msg->mMotors[3] = 0;
//            mcu.Send(msg);
//            delete(msg);

            CxUtils::SleepMs(1000);
        }
    }
    return 0;
}
