#include <dynamixel/dynamixel.h>
#include <stdio.h>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Motor;
int main()
{
    Dynamixel test;
    double feed;
    if(!test.Initialize("/dev/ttyUSB0",false,10.0))
    {
        return -1;
    }
    
    
    //do a sine wave
    
    double t=0;
    double val;
    while(1)
    {
        double top=(564-512)/512.0*100.0;
        double bot=(440-512)/512.0*100.0;
        
        
        
        //val=(top-bot)/2.0*sin(t/(2*M_PI))+(top+bot)/2.0;
        //test.SetServo(1,val);
        //feed=test.GetServo(1);
        //printf("%lf %lf\n",val,feed);
        //t+=0.05;
        for(double val=bot;val<=top;val+=1.0)
        {
            printf("%lf\n",val);
            test.SetServo(1,val);
            CxUtils::SleepMs(50);
        }
        for(double val=top;val>=bot;val-=1.0)
        {
            printf("%lf\n",val);
            test.SetServo(1,val);
            CxUtils::SleepMs(50);
        }
        
    }
    
    return 0;
}

