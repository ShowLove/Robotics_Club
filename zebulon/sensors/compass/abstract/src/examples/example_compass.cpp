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
#include <cxutils/math/cxmath.h>

using namespace std;
using namespace CxUtils;
using namespace Zebulon;
using namespace Compass;

#define COMPCOUNT 10
double gStartTime=-1.0;
int gCount=0;

class MyCallBackPrint : public Compass::Callback
{
public:
    MyCallBackPrint() {}
    ~MyCallBackPrint() {}
    virtual void ProcessCompassData(const double yaw, const double pitch, const double roll)
    {
        gCount++;
        if(gCount>=COMPCOUNT)
        {
            cout << "Attitude (deg): Yaw " << CxUtils::CxToDegrees(yaw)<< " Pitch " << CxUtils::CxToDegrees(pitch) << " Roll " << CxUtils::CxToDegrees(roll) << endl;
            if(gStartTime>0)
            {
                cout << "Measurements/Sec:" << (double)(COMPCOUNT/(CxUtils::GetTimeSeconds()-gStartTime)) << std::endl;
            }
            gStartTime=CxUtils::GetTimeSeconds();
            gCount=0;
        }
    }
    
    virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration)
    {
        gCount++;
        if(gCount>=COMPCOUNT)
        {
            CxUtils::Point3D deg_attitude=(CX_RAD2DEG(attitude));
            cout << "Attitude (deg): ";
            deg_attitude.Print();
            
            CxUtils::Point3D deg_gyroscope=(CX_RAD2DEG(gyroscope));
            cout << "Gyroscope (deg/sec): ";
            deg_gyroscope.Print();
                        
            cout << "Accelerometer (m/sec^2): ";
            acceleration.Print();
            
            if(gStartTime>0)
            {
                cout << "Measurements/Sec:" << (double)(COMPCOUNT/(CxUtils::GetTimeSeconds()-gStartTime)) << std::endl;
            }
            gStartTime=CxUtils::GetTimeSeconds();
            gCount=0;
        }
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
        //sensor=Compass::Factory::CreateCompass(Compass::Factory::GX1);
        sensor = Compass::Factory::CreateCompass(Compass::Factory::CoralAHRS);
        //sensor=Compass::Factory::CreateCompass(Compass::Factory::GX2);
        //sensor=Compass::Factory::CreateCompass(Compass::Factory::Inclinometer);
        if(sensor!=NULL)
        {
            //GX1
            //if(sensor->Initialize(argv[1],38400,false,1000))
            //GX2
            //if(sensor->Initialize(argv[1],115200,false,1000))
            //Inclinometer
            ///if(sensor->Initialize(argv[1], 38400, false, 3000))
            if(sensor->Initialize(argv[1], 57600, false, 3000))
            {
                if(sensor->IsImuSupported())
                {
                    sensor->RegisterCallback(&callback,Compass::Callback::IMUCallback);
                }
                else
                {
                    sensor->RegisterCallback(&callback,Compass::Callback::DefaultCallback);
                }
                
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
