/*
    Acts as the actual interface to hardware
    This part is seperate from JAUS, but pushes up
    the list to the parent
    
    Holds list of contained internal items and own threads

    //for the name of the pin, and where it is
    //This should be done before registering the callback
    Appliance::AddDigitalInput("Button",4);
*/

#ifndef __ZEBULON_APPLIANCE__H
#define __ZEBULON_APPLIANCE__H

#include <mcu/interface.h>
#include <mcu/messagefactory.h>
#include <iostream>
#include <map>

#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include "zmcu/microcontroller.h"

namespace Zebulon
{
    namespace Utility
    {
        class Microcontroller;
        
        class Appliance : public MCU::Interface::Callback
        {
            public:
                Appliance(); 
                virtual ~Appliance();
                
                virtual void Shutdown();
                
                // Initialize called when component is initialized automagically.
                virtual void Initialize(Microcontroller *Parent);
                
                // Sets the state of a device (on = true, false = off) (overload to send command to MCU).
                virtual bool SetDigitalOut(const std::string& name, const bool value);
                // Sets the analog value for a pin/device [-100,100]% of max (overload to send command to MCU).
                //Not supported by current MCU's yet
                virtual bool SetAnalogOut(const std::string& name, const double value);                
                
                //Takes in messages from the MCU and sets the appropriate member variable (digital/analog)
                virtual void ProcessMessage(const MCU::Message* message);
                
                // Prints MCU information
                //virtual void PrintStatus();
                //Passes the pointer to the MCU to the Microcontroller thread
                bool SetMCUInterface(MCU::Interface *MCU);
                
                bool SetADCBits(int ADCBits);
                bool SetMCUDelay(int MCUDelay);
                
                virtual bool AddDigitalOutput(const std::string &name,const int pin,const bool value=false);
                virtual bool AddDigitalInput(const std::string &name,const int pin);
                virtual bool AddAnalogOutput(const std::string &name,const int pin,const double value=0.0);
                virtual bool AddAnalogInput(const std::string &name,const int pin, const double min=0.0, const double max=0.0);
                
                
        protected:
                Microcontroller *mParent;
                int mADCBits;
                int mMCUDelay;
                
                MCU::Interface *mMCU;
                
                void MCUProcessingThread();
                static void MCUThread(void *args);
                CxUtils::Thread mMCUThread;
                CxUtils::Mutex mMCUMutex;
                struct AnalogScale
                {
                    double mMin;
                    double mMax;
                };
                
                // MCU Digital Inputs
                std::map<std::string,MCU::ReportDigitalInputs> mMCUDigitalInputMessages;
                std::map<int,std::string> mMCUDigitalInputPins;
                // MCU Analog Inputs
                std::map<std::string,MCU::ReportAnalogInputs> mMCUAnalogInputMessages;
                std::map<int,std::string> mMCUAnalogInputPins;
                std::map<std::string,AnalogScale> mMCUAnalogInputScale;
                
                // MCU Digital Outputs
                std::map<std::string,MCU::SetDigitalOutputs> mMCUDigitalOutputMessages;
                std::map<int,std::string> mMCUDigitalOutputPins;
                // MCU Analog Outputs
                std::map<std::string,MCU::SetAnalogOutputs> mMCUAnalogOutputMessages;
                std::map<int,std::string> mMCUAnalogOutputPins;
                
                CxUtils::Time::Stamp mLastCommandTimeMs;                            ///<  The last time we sent commands to the FPGA.
        };
    }
}
#endif