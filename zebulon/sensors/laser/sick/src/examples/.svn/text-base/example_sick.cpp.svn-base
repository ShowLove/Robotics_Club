#include <stdio.h>
#include <iostream>
#include <math.h>

#include <sick/sick.h>
#include <sick/messages/settingsmode.h>
#include <sick/messages/continuousoutputresponse.h>
#include <cxutils/cxutils.h>

using namespace std;
using namespace Zebulon;
using namespace Laser;
using namespace SICK;
using namespace CxUtils;

//#include <vld.h>

// Example implementation of a Sick::Callback
class MessageCallback : public Sick::Callback
{
public:
    MessageCallback()
    {
    }
    ~MessageCallback()
    {
    }
    virtual void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                  const CxUtils::Time& timestamp,
                                  const unsigned int id)
    {
        // Range data in polar coordinates!
    }
    virtual void ProcessMessage(const Zebulon::Laser::SICK::Message* msg)
    {
        static unsigned long long count = 0;
        static unsigned long long startTimeStamp = CxUtils::GetTimeMs();

        if ( Message::ContinuousOutput == msg->GetMessageType() )
        {
            if ( count == 0 )
            {
                startTimeStamp = CxUtils::GetTimeMs();
            }

            count++;

            unsigned long long currentTimeStamp = CxUtils::GetTimeMs();

            if ( count%5 == 0 )
            {
                cout << "\nAvg message/sec = " << count/((currentTimeStamp - startTimeStamp)/1000.0);
                cout << "\nRecieved = " << ((ContinuousOutputResponse*)msg)->mPoints.size();
            }

        }
    }
};


int main(int argc, char **argv)
{
     SICK::Sick laser;
     MessageCallback cb;

     // Register a callback to process the received
     // messages.
     laser.RegisterCallback(&cb);

     cout << "Connecting to SICK...";
     if(laser.Initialize("/dev/ttyUSB0", 38400) == 0)
     {
         cout << "Failure!\n";
         cout << "Unable to connect to SICK LIDAR.\n";
         return 0;
     }
     cout << "Success!\n";
     cout << "Setting new baud rate to 38400...";
     if(laser.ChangeBaudRate(38400) == 0)
     {
         cout << "Failure!\n";
         return 0;
     }
     cout << "Sucess!\n";
    
     cout << "Setting distance mode to mm mode...";
     if(laser.SetDistanceMode(Sick::MM) == 0)
     {
         cout << "Failure!\n";
         return 0;
     }
     cout << "Sucess!\n";

     cout << "Setting scanning angle to 180 degrees\nand resolution to half a degree...";
     if(laser.SetScanningMode(Sick::OneEighty, Sick::Half) == 0)
     {
         cout << "Failure!\n";
         return 0;
     }
     cout << "Success!\n";
    
     cout << "Requesting continous output...";
     if(laser.RequestContinuousOutput() == 0)
     {
         cout << "Failure!\n";
         return 0;
     }
     cout << "Success!\n";

     while ( true )
     {
         if (CxUtils::GetChar() == 27)
         {
             break;
         }
         CxUtils::SleepMs(1);
     }

     // Make sure to shutdown the laser.
     laser.Shutdown();

     return 0;
}

/*  End of File */
