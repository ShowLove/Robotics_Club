#include <lcd/lcdinterface.h>

using namespace Zebulon;
using namespace Hardware;

bool LCDInterface::Initialize(LCD* screen, double period)
{
    printf("Initializing LCD Screen...");
    this->mScreen = screen;
    this->mShutdown = false;
    this->mPeriod = period;
    if (!mSerialThread.CreateThread(&SerialThreadFunction, this))
    {
        printf("Failure initializing thread\n");
        return false;
    }
    printf("Success!\n");
}

void LCDInterface::SerialThreadFunction(void *args)
{
    LCDInterface* interface = ((LCDInterface*)args);
    interface->ProcessingFunction();
}