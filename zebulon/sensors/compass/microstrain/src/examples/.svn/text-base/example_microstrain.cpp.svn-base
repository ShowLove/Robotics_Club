#include <iostream>
/////////////////////////////////////////////////////////////////////////////
///
/// \file microstraindemo.cpp
/// \brief Example program demonstrating use of the MicroStrain Library.
///
/// Author(s): Daniel Barber<br>
/// Created: 2/28/2008<br>
/// Last Modified: 2/29/2008<br>
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
#include "microstrain/gx1.h"
#include "microstrain/gx2.h"

using namespace std;
using namespace Zebulon;
using namespace Compass;
using namespace CxUtils;


class MyCallBackPrint : public MicroStrain::Callback
{
public:
    MyCallBackPrint() {}
    ~MyCallBackPrint() {}
    virtual void ProcessMessage(const MicroStrain::Message* msg)
    {
        if(msg->GetType() == MicroStrain::Message::SendGyroStabilizedEulerAngles)
        {
            msg->Print();
        }
        if(msg->GetType() == MicroStrain::Message::EulerAnglesAndAngularRates2)
        {
            msg->Print();
        }
    }
};


int main (int argc, char* argv[])
{
    MicroStrain::GX1 sensor;
    MicroStrain::GX2 sensor2;
    
    MyCallBackPrint callback;

    cout << "Attempting to connect to compass" << endl;

    if(argc > 2)
    {
        int type=atoi(argv[2]);
        if(type==1)
        {
            if(sensor.Initialize(argv[1],38400,false))
            {
                sensor.RegisterCallback(&callback);
                std::cout << "Enter <q> to Exit.\n";
                CxUtils::SleepMs(1000);
                std::string enter;
                cin >> enter;
                sensor.Shutdown();
            }
        }
        else if(type==2)
        {
            //printf("GX2: %s %d\n",argv[1],type);
            if(sensor2.Initialize(argv[1],115200,false))
            {
                sensor2.RegisterCallback(&callback);
                std::cout << "Enter <q> to Exit.\n";
                CxUtils::SleepMs(1000);
                std::string enter;
                cin >> enter;
                sensor.Shutdown();
            }
        }
        else
        {
            std::cout << "Using unknown GX Type\n";
        }
        
    }
    else if(argc >1)
    {
        if(sensor.Initialize(argv[1],38400,false))
        {
            sensor.RegisterCallback(&callback);
            std::cout << "Enter <q> to Exit.\n";
            CxUtils::SleepMs(1000);
            std::string enter;
            cin >> enter;
            sensor.Shutdown();
        }
    }
    else if(sensor.Initialize("/dev/ttyUSB1", 57600))
    {
        sensor.RegisterCallback(&callback);
        std::cout << "Enter <q> to Exit.\n";
        CxUtils::SleepMs(1000);
        std::string enter;
        cin >> enter;
        sensor.Shutdown();
    }
    else
    {
        cout << "Compass Connection Failure" << endl;
    }

	return 0;
}

/*  End of File */
