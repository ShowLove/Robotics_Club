#include "zmcu/microcontroller.h"
#include "jaus/core/bitvector.h"

using namespace Zebulon;
using namespace Utility;

Microcontroller::Microcontroller()
{
    mLastCommandTimeMs = 0;
}

Microcontroller:: ~Microcontroller()
{
    Shutdown();
}

void Microcontroller::Shutdown()
{
    JAUS::Service::Shutdown();
    
    //Stop sub processes
}

void Microcontroller::Initialize()
{
    JAUS::Microcontroller::Initialize();
}
                
void Microcontroller::SetDigitalOut(const std::string& name, const bool value)
{
    //See who wants it, then sends it down
    std::vector< Appliance * >::iterator it;
    for(it=mMCUList.begin();it!=mMCUList.end();it++)
    {
        //pass to everyone, if found, quick out
        bool found=(*it)->SetDigitalOut(name,value);
        if(found)
        {
            return;
        }
    }
    std::cout << name << " : not a valid digital output\n";
}

void Microcontroller::SetAnalogOut(const std::string& name, const double value)
{
    //See who wants it, then sends it down
    std::vector< Appliance * >::iterator it;
    for(it=mMCUList.begin();it!=mMCUList.end();it++)
    {
        //pass to everyone, if found, quick out
        bool found=(*it)->SetAnalogOut(name,value);
        if(found)
        {
            return;
        }
    }
    std::cout << name << " : not a valid analog output\n";
}

bool Microcontroller::AddMCU(Appliance *MCU)
{
    if(MCU==NULL)
    {
        printf("ZMCU: Passed a bad pointer for appliance\n");
        return false;
    }
    //Pass in as parent
    MCU->Initialize(this);
    mMCUList.push_back(MCU);
    return true;
}

void Microcontroller::PrintStatus()
{
    DigitalStates digitalStates = mDigitalStates;
    AnalogStates analogStates = mAnalogStates;
    mMcuMutex.Unlock();

    DigitalStates::const_iterator digital;
    for(digital = digitalStates.begin();
        digital != digitalStates.end();
        digital++)
    {
        std::cout << "Digital - " << digital->first << " - " << (int)digital->second << std::endl;
    }
    AnalogStates::const_iterator analog;
    for(analog = analogStates.begin();
        analog != analogStates.end();
        analog++)
    {
        //hack to remove extra displays
        if(strstr(analog->first.c_str(),"Disp")==NULL)
        {
            std::cout << "Analog - " << analog->first << " - " << analog->second << std::endl;
        }
    
    }
}

/*  End of File */



