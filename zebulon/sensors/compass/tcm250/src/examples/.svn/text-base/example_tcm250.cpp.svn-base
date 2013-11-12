////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_tcm250.cpp
///  \brief Program that shows how to initialize the tcm 250 and create callback.
///
///  <br>Author(s): David Adams
///  <br>Created: 24 October 2008
///  <br>Last Modified: 24 October 2008
///  <br>Copyright (c) 2008
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>Email: dadasm@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Please see the file license.txt for information on usage and
///  redistribution of this file, and for a DISCLAIMER OF ALL WARRANTIES.
///
///  \include license.txt
///
////////////////////////////////////////////////////////////////////////////////////
#include "tcm250/tcm250.h"
#include <iostream>
#include <cxutils/math/cxmath.h>
#include <cxutils/keyboard.h>


using namespace std;
using namespace Zebulon;
using namespace Compass;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Overloaded callback and functions to recieve events from tcm
///
////////////////////////////////////////////////////////////////////////////////////
class MyCallback : public TCM::Callback
{
public:
    MyCallback() {}
    ~MyCallback() {}
    virtual void ProcessCompassData(const double yaw, const double pitch, const double roll)
    {
        cout << "Y: " << CxUtils::CxToDegrees(yaw) << ", P: " << CxUtils::CxToDegrees(pitch) << ", R: " << CxUtils::CxToDegrees(roll) << endl;
    }
    virtual void ProcessMagnetometerData(const double bx, const double by, const double bz)
    {
        //cout << "mX: " <<  bx << " mY: " << by << " mZ: " << bz << endl;
    }
    virtual void ProcessTemperatureData(const double temp)
    {
        //cout <<"Temp: " << temp << endl;
    }
};
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Main function
///
////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    TCM::TCM250 tcm;
    MyCallback cb;

    if(argc > 1)
    {
        if(tcm.Initialize(argv[1]) == 0)
        {
            cout << "Failed to connect on port " << argv[1] << endl;
            return 0;
        }
    }
    else
    {
        if(tcm.Initialize("COM3") == 0)
        {
            cout << "Failed to connect on port COM4" << endl;
            return 0;
        }
    }
    
    unsigned int time = 10;
    tcm.RegisterCallback(&cb);
    tcm.EnableFastSampling(true);
    tcm.SetClockRate(40);
    tcm.SetTimeConstant(1);  
    tcm.SetQuerySkip();
    tcm.SetSamplingPeriodDivisor();
    tcm.EnableTemperatureData(true);
    tcm.EnableMagneticDistortionAlarm(false);
    tcm.EnableMagnetometerData(true);
    tcm.EnableDigitalDamping(true);
    tcm.StartContinuousOutput();

    while(true)
    {
        if(CxUtils::GetChar() == 27)
        {
            break;
        }
    }

    return 0;
}