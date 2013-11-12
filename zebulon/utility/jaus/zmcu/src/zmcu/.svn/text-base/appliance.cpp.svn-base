#include "zmcu/appliance.h"
//#include <bitvector/bitvector.h>
#include "jaus/core/bitvector.h"
using namespace Zebulon;
using namespace Utility;


Appliance::Appliance()
{
    mMCU = NULL;
    //assume ten bits
    mADCBits=10;
    mMCUDelay=25;
    
    
    mLastCommandTimeMs = 0;
}

Appliance:: ~Appliance()
{
    Shutdown();
}

void Appliance::Shutdown()
{
    mMCUThread.StopThread();
}

void Appliance::Initialize(Microcontroller *Parent)
{
    mParent=Parent;
}

void Appliance::ProcessMessage(const MCU::Message* message)
{
    switch(message->mMessageCode)
    {
        case MCU::Message::Pong:
        {
            const MCU::Pong* pong = (const MCU::Pong*)message;
            pong->Print();
        }
            break;
        case MCU::Message::ReportDigitalInputs:
        {
            const MCU::ReportDigitalInputs* DI = (const MCU::ReportDigitalInputs*)message;
            //DI->Print();
            
            std::map<int,std::string>::iterator digitalit;
            
            digitalit = mMCUDigitalInputPins.find(DI->mPort);
            if(digitalit != mMCUDigitalInputPins.end())
            {
                mParent->SetDigitalInput(mMCUDigitalInputPins[DI->mPort],DI->mValue);
            }
            
            break;
        }
        case MCU::Message::ReportAnalogInputs:
        {
            const MCU::ReportAnalogInputs* AI = (const MCU::ReportAnalogInputs*)message;
            //AI->Print();
            
            std::map<int,std::string>::iterator analogit;
            
            analogit = mMCUAnalogInputPins.find(AI->mPort);
            if(analogit != mMCUAnalogInputPins.end())
            {
                double mConvVoltageMCU = 0;
                // Figure out a way to do fancy math here, or outside?

                //std::cout << "Default MCU: No Conversion\n";
                std::string tmpName=mMCUAnalogInputPins[AI->mPort];
                AnalogScale tmpConv=mMCUAnalogInputScale[tmpName];
                //no conversion, scale to 0 to 100.0
                if(tmpConv.mMin==0.0 && tmpConv.mMax==0.0)
                {
                    mConvVoltageMCU = (100.0*AI->mValue/(pow(2.0,mADCBits)-1));
                }
                //use conversion factor
                else
                {
                    mConvVoltageMCU = (tmpConv.mMax-tmpConv.mMin)*AI->mValue/(pow(2.0,mADCBits)-1)+tmpConv.mMin;
                }
                
                if(mConvVoltageMCU >= 100)
                {
                    mConvVoltageMCU = 100;
                }
                else if(mConvVoltageMCU <= 0)
                {
                    mConvVoltageMCU = 0;
                }
                
                mParent->SetAnalogInput(tmpName,mConvVoltageMCU);
            }            
            break;
        }
        default:
            break;
    }
}
                
bool Appliance::SetDigitalOut(const std::string& name, const bool value)
{
    bool ret=false;
    mMCUMutex.Lock();
    std::map<std::string,MCU::SetDigitalOutputs>::iterator it;
    it = mMCUDigitalOutputMessages.find(name);
    
    if(it == mMCUDigitalOutputMessages.end())
    {
        //std::cout << name << " : not a valid digital output\n";
        ret=false;
    }
    else
    {
        mMCUDigitalOutputMessages[name].mValue = value;
        ret=true;
    }
    mMCUMutex.Unlock();
    return ret;
}

bool Appliance::SetAnalogOut(const std::string& name, const double value)
{
    bool ret=false;
    mMCUMutex.Lock();
    std::map<std::string,MCU::SetAnalogOutputs>::iterator it;
    it = mMCUAnalogOutputMessages.find(name);
    
    if(it == mMCUAnalogOutputMessages.end())
    {
        //std::cout << name << " : not a valid analog output\n";
        ret=false;
    }
    else
    {
        mMCUAnalogOutputMessages[name].mValue = value;
        ret=true;
    }
    mMCUMutex.Unlock();
    return ret;
}

bool Appliance::SetMCUInterface(MCU::Interface *MCU)
{
    if(MCU == NULL)
    {
        std::cout << "MCU: I was passed a garbage pointer\n";
        return false;
    }
    mMCU = MCU; 
    mMCU->RegisterCallback(MCU::Message::All, this);
    if(mMCUThread.CreateThread(&MCUThread,this)==false)
    {
        std::cout << "MCU: Thread Creation Failed\n";
        return false;
    }
    return true;
}



void Appliance::MCUThread(void *args)
{
    Appliance *microcontroller=(Appliance *)args;
    microcontroller->MCUProcessingThread();
}

void Appliance::MCUProcessingThread()
{
    //CxUtils::SleepMs(1000);
    while(mMCUThread.QuitThreadFlag() == false)
    {

        //std::cout << "Starting Loop\n";
        
        //Ask for all inputs on demand
        
        std::map<std::string,MCU::ReportDigitalInputs>::iterator dinit;
        //Make copy in case
        mMCUMutex.Lock();
        std::map<std::string,MCU::ReportDigitalInputs> tmpMCUDigitalInputMessages=mMCUDigitalInputMessages;
        mMCUMutex.Unlock();
         
        // Send digital input data
        for(dinit = tmpMCUDigitalInputMessages.begin(); dinit != tmpMCUDigitalInputMessages.end(); dinit++)
        {
            //std::cout << "MCU Digital Inputs...\n";
            mMCU->Send(&(dinit->second), mMCUDelay);
        }
        
        std::map<std::string,MCU::ReportAnalogInputs>::iterator ainit;
        mMCUMutex.Lock();
        std::map<std::string,MCU::ReportAnalogInputs> tmpMCUAnalogInputMessages=mMCUAnalogInputMessages;
        mMCUMutex.Unlock();
        
        // Send analog input data
        for(ainit = tmpMCUAnalogInputMessages.begin(); ainit != tmpMCUAnalogInputMessages.end(); ainit++)
        {
            //std::cout << "MCU Analog Inputs..." << ainit->first << std::endl;
            mMCU->Send(&(ainit->second), mMCUDelay);
        }
        
        
        //Set out outputs from memory
        
        std::map<std::string,MCU::SetDigitalOutputs>::iterator doutit;
        mMCUMutex.Lock();
        std::map<std::string,MCU::SetDigitalOutputs> tmpMCUDigitalOutputMessages=mMCUDigitalOutputMessages;
        mMCUMutex.Unlock();
        
        // Send digital output data
        for(doutit = tmpMCUDigitalOutputMessages.begin(); doutit != tmpMCUDigitalOutputMessages.end(); doutit++)
        {
            //std::cout << "MCU Digital Outputs...\n";
            mMCU->Send(&(doutit->second), mMCUDelay);
        }
        
        std::map<std::string,MCU::SetAnalogOutputs>::iterator aoutit;
        mMCUMutex.Lock();
        std::map<std::string,MCU::SetAnalogOutputs> tmpMCUAnalogOutputMessages=mMCUAnalogOutputMessages;
        mMCUMutex.Unlock();
        
        // Send analog output data
        for(aoutit = tmpMCUAnalogOutputMessages.begin(); aoutit != tmpMCUAnalogOutputMessages.end(); aoutit++)
        {
            //std::cout << "MCU Analog Outputs...\n";
            mMCU->Send(&(aoutit->second), mMCUDelay);
        }

        CxUtils::SleepMs(10);
    }
}

bool Appliance::AddDigitalOutput(const std::string &name,const int pin,const bool value)
{
    MCU::SetDigitalOutputs mTempDigtialOutput;
    
    mTempDigtialOutput.mPort = pin;
    mTempDigtialOutput.mValue = value;
    mMCUDigitalOutputMessages[name] = mTempDigtialOutput;
    mMCUDigitalOutputPins[pin] = name;
}
bool Appliance::AddDigitalInput(const std::string &name,const int pin)
{
    MCU::ReportDigitalInputs mTempDigitalInput;
    
    mTempDigitalInput.mPort = pin;
    mMCUDigitalInputMessages[name] = mTempDigitalInput;
    mMCUDigitalInputPins[pin] = name;
    return true;
}
bool Appliance::AddAnalogOutput(const std::string &name,const int pin,const double value)
{
    MCU::SetAnalogOutputs mTempAnalogOutput;
    
    mTempAnalogOutput.mPort = pin;
    mTempAnalogOutput.mValue = value;
    mMCUAnalogOutputMessages[name] = mTempAnalogOutput;
    mMCUAnalogOutputPins[pin] = name;
}
bool Appliance::AddAnalogInput(const std::string &name,const int pin, const double min, const double max)
{
    MCU::ReportAnalogInputs mTempAnalogInput;
    
    mTempAnalogInput.mPort = pin;
    mMCUAnalogInputMessages[name] = mTempAnalogInput;
    mMCUAnalogInputPins[pin] = name;
    
    AnalogScale mTempAnalogScale;
    mTempAnalogScale.mMin=min;
    mTempAnalogScale.mMax=max;
    mMCUAnalogInputScale[name]=mTempAnalogScale;
    return true;
}

bool Appliance::SetADCBits(int ADCBits)
{
    if(ADCBits<0 || ADCBits>32)
    {
        printf("Appliance: A Crazy number of bits: %d\n",ADCBits);
        return false;
    }
    mADCBits=ADCBits;
    
    return true;
}

bool Appliance::SetMCUDelay(int MCUDelay)
{
    if(MCUDelay<0)
    {
        printf("Appliance: A Crazy number of delay: %d\n",MCUDelay);
        return false;
    }
    mMCUDelay=MCUDelay;
    
    return true;
}


/*  End of File */



