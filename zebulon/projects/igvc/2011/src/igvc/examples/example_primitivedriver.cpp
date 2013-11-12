/////////////////////////////////////////////////////////////////////////////////////
///
/// \file example_primitivedriver.h
/// \brief Tests primitive driver by creating a component and connecting to 
///        motor controllers. Must connect with
///        another program, like jaus console joystick.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
///  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
///  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
///  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
///  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
///  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
///  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
///  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
///  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
///  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
///
////////////////////////////////////////////////////////////////////////////////////
#include "igvc/baseline/primitivedriver.h"
#include <cxutils/cxutils.h>
#include <jaus/core/component.h>
#include <roboteq/ax3500.h>
using namespace Zebulon;
using namespace IGVC;

JAUS::UShort gSubsystemID = 55;
int main(int argc, char** argv)
{
    Ground::PrimitiveDriver *driver;
    Motor::Roboteq::AX3500 controller;
    // JAUS Components and Services.
    JAUS::Component component;
    char channel;
    
    if (argc < 2)
    {
        printf("Error: Not enough parameters\n example_motors <port> [<channel>]\n");
        return 0;
    }
        
    std::string port(argv[1]);
    int initialized = controller.Initialize(port);
    printf("Initialize returned %d for port %s\n", initialized, argv[0]);
    if (!initialized)
    {
        initialized = controller.Initialize("/dev/ttyUSB0");
        if(!initialized)
        {
            printf("Unable to initialize connection with Roboteq\n");
            return 0;            
        }
    }
    printf("Initialize returned %d for port %s\n", initialized, argv[0]);
    
    if (argc > 3)
        channel = atoi(argv[2]);
    else
        channel = 0;
    driver = new Ground::PrimitiveDriver::PrimitiveDriver();
    component.AddService(driver);
    // You must add all JAUS services to your component
    // before you load settings or initialize it.  This should
    // be done in the Initialize function above.
    if(component.LoadSettings("settings/services.xml") == false)
    {
        component.DiscoveryService()->SetSubsystemIdentification(JAUS::Subsystem::Vehicle,
                                                                          "Ground Test");
    }
    component.DiscoveryService()->SetComponentIdentification("Primitive Driver Test");
    component.Initialize(JAUS::Address(gSubsystemID, 1, 1), 25);
    int printTimeMs;
    double startTime=CxUtils::GetTimeSeconds();
    while(CxUtils::GetChar() != 27)
    {
        double leftThrust = 0, rightThrust = 0;

        leftThrust = driver->GetLeftThrust();
        rightThrust = driver->GetRightThrust();

        
        // Send motor thrust values.
        controller.SendDualMotorCommand(leftThrust, rightThrust, false);
        controller.SendDualMotorCommand(leftThrust, rightThrust, false);
        
        if(JAUS::Time::GetUtcTimeMs() - printTimeMs >= 100)
        {
            // Print status of services.
            std::cout << "\n======================================================\n";
            component.AccessControlService()->PrintStatus(); std::cout << std::endl;
            if(driver)
            {
                driver->PrintStatus(); std::cout << std::endl;
            }
            printTimeMs = JAUS::Time::GetUtcTimeMs();
        }
        std::cout << "Running Time: " << (CxUtils::GetTimeSeconds()-startTime) << std::endl;
        CxUtils::SleepMs(100);
    }
    return 0;
}