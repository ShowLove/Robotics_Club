/*
    The concept behind this class to to replace the way MCU is done
    for various projects to make it easier to implement with JAUS
    
    It acts as an interface layer that should not need to be inherited from.
    
    Potentially entirely configurable through XML?
    
    ZMCU acts as the interface, it can have multiple real MCUs in a list
*/


#ifndef __ZEBULON_MICROCONTROLLER__H
#define __ZEBULON_MICROCONTROLLER__H

#include <jaus/extras/mcu/microcontroller.h>
#include <iostream>
#include <map>

#include <cxutils/thread.h>
#include <cxutils/mutex.h>

#include <zmcu/appliance.h>

namespace Zebulon
{
    namespace Utility
    {
        class Appliance;
        
        class Microcontroller : public JAUS::Microcontroller
        {
            public:
                
                Microcontroller(); 
                virtual ~Microcontroller();
                
                virtual void Shutdown();
                
                // Initialize called when component is initialized automagically.
                virtual void Initialize();
                //virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
                
                // Sets the state of a device (on = true, false = off) (overload to send command to MCU).
                virtual void SetDigitalOut(const std::string& name, const bool value);
                // Sets the analog value for a pin/device [-100,100]% of max (overload to send command to MCU).
                //Not supported by current MCU's yet
                virtual void SetAnalogOut(const std::string& name, const double value);
                // Sets the speed of the motor
                
                //virtual void SetServoOutputs(const std::string& name, const double value);
                //Convets a motor value(-100/100) and converts it to a servo value (0/255 w/ 127 as 0)
                //unsigned char ServoConvert(double motorValue) const;
                
                //Places the MCU in the Ready state
                virtual bool Resume() {return true;};
                //Places the MCU in the Standby state
                virtual bool Standby() {return true;};
                //Resets the MCU
                virtual bool Reset() {return true;};
                
                // Prints MCU information
                virtual void PrintStatus();
                
                //Passes the pointer to the MCU to the Microcontroller thread
                bool AddMCU(Appliance *MCU);

                
            protected:
            
                std::vector< Appliance * > mMCUList;
                
                CxUtils::Time::Stamp mLastCommandTimeMs;                            ///<  The last time we sent commands to the FPGA.

                
        };
    }
}
#endif