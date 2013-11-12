#include <dynamixel/dynamixel.h>
#include <stdio.h>
#include <iostream>

using namespace Zebulon;
using namespace Motor;

int main()
{
    Dynamixel test;
    double feed;
    if(!test.Initialize("/dev/ttyUSB0",false, 40.0))
    {
        return -1;
    }

    test.SetServoSpeed(1, 13.32);
    test.SetTorqueLimit(1, 1023);
    test.SetServoDegrees(1, 155);


    while(1)
    {
        feed = test.GetServoDegrees(1);
        if(feed >= 150)
        {
            test.SetServoDegrees(1, 120);
        }
        if(feed <= 125)
        {
            test.SetServoDegrees(1, 155);
        }

        CxUtils::SleepMs(1);
    }
    return 0;
}

