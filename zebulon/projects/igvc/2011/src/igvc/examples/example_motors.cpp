#include <roboteq/ax3500.h>
#include <cxutils/cxutils.h>

using namespace Zebulon;
using namespace Motor;

char gChannel;
char gValue;

float t = 0;
float dt = .01;

int main(int argc, char** argv)
{
    argc--;
    
    if (argc < 1)
    {
        printf("Error: Not enough parameters\n example_motors <port> [<channel>]\n");
        return 0;
    }
    
    argv++;
    
    Roboteq::AX3500 controller;
    
    std::string port(argv[0]);
    int initialized = controller.Initialize(port);
    printf("Initialize returned %d for port %s\n", initialized, argv[0]);
    if (!initialized)
    {
        initialized = controller.Initialize("/dev/ttyUSB0");
    }
    printf("Initialize returned %d for port %s\n", initialized, argv[0]);
    
    if (argc > 1)
        gChannel = atoi(argv[1]);
    else
        gChannel = 0;
    /*
    printf("Sending CR 10 times\n");
    const char enterCommand[] = { 0x0D, '\0' };
    Roboteq::AX3500::Message* msg;
    for (int i=0; i<10; i++)
    {
        controller.TransmitMessage(enterCommand, &msg);
        //printf("%s\n", msg->mString.c_str());
        printf(".");
        CxUtils::SleepMs(50);
    }
    printf("\n");
    */
    //controller.EnterWatchdogMode();
    
    while (true)
    {
        gValue = 99*sin(t=(t+dt));
        controller.SendMotorCommand(0, (const char) gValue, false);
        //controller.SendDualMotorCommand(gValue/100.0,gValue/100.0,false);
        
        printf("Sending %d to channel %d\n", gValue, gChannel);
        
        CxUtils::SleepMs(250);
    }
    
    return 0;
}