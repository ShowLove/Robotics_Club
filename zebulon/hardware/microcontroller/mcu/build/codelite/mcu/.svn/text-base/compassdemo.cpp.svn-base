/////////////////////////////////////////////////////////////////////////////
///
/// \file compassdemo.cpp
/// \brief Example program demonstrating use of the compass Library.
///
/// Author(s): Gary Stein<br>
/// Created: 2/5/2010<br>
/// Last Modified: 2/5/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu <br>
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

#include <iostream>
#include <compass/compassfactory.h>
#include <cxutils/cxutils.h>

using namespace std;
using namespace CxUtils;
using namespace Zebulon;
using namespace Compass;


class MyCallBackPrint : public Compass::Callback
{
public:
    MyCallBackPrint() {}
    ~MyCallBackPrint() {}
    virtual void ProcessCompassData(const double yaw, const double pitch, const double roll)
    {
        cout << "Attitude (deg): Yaw " << CxUtils::CxToDegrees(yaw)<< " Pitch " << CxUtils::CxToDegrees(pitch) << " Roll " << CxUtils::CxToDegrees(roll) << endl;
    }
};

int main (int argc, char* argv[])
{
    Compass::GenericCompass *sensor=NULL;
    MyCallBackPrint callback;
    
    cout << "Attempting to connect to compass" << endl;

    if(argc >1)
    {
        //generic compass
        sensor=CompassFactory::CreateCompass("3DM-GX1");
        if(sensor!=NULL)
        {
            if(sensor->Initialize(argv[1],38400,false))
            {
                sensor->RegisterCallback(&callback);
                std::cout << "Enter <q> to Exit.\n";
                CxUtils::SleepMs(1000);
                std::string enter;
                cin >> enter;
                sensor->Shutdown();
            }
        }
        else
        {
            cout << "Compass did not exist\n";
        }
    }
    else
    {
        cout << "Specify a serial port" << endl;
    }
    return 0;
}

/*  End of File */
