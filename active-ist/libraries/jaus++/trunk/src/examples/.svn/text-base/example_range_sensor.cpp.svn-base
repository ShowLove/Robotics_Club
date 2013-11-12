////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_rangesensor.cpp
///  \brief This file contains a test/example program for the RangeSensor services.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 10 April 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#include "jaus/core/component.h"
#include "jaus/extras/rangesensor/rangesensor.h"
#include "jaus/extras/rangesensor/rangesubscriber.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <cxutils/math/cxmath.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


using namespace JAUS;

int main(int argc, char* argv[])
{
    Component component;
    // Add the services we want our component to have
    // beyond the core set.

    RangeSensor* rangeSensor = new RangeSensor();
    // Setup the configuration of a range sensing device.
    RangeSensorConfig rangeSensorConfig;
    rangeSensorConfig.mID = 0;
    rangeSensorConfig.mName = "LIDAR";
    rangeSensorConfig.mMinRange = 0.0;
    rangeSensorConfig.mMaxRange = 9.0;                           // 9 meter range.
    rangeSensorConfig.mUnitType = RangeSensorConfig::MM;         // Range values are in mm.
    rangeSensorConfig.mScanAngle = CxUtils::CxToRadians(90);     // [-45, 45] degrees.
    rangeSensorConfig.mAngleIncrement = CxUtils::CxToRadians(1); // 1 degree increment.
    rangeSensor->AddRangeDevice(rangeSensorConfig);
    // Add to component.
    component.AddService(rangeSensor);

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    component.LoadSettings("settings/services.xml");

    // In this example, we are simulating a component on part of a simulated
    // robot.
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
                                                             "Simulation");

    // Initialize component component with component given ID.
    if(component.InitializeWithUniqueID() == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    std::cout << "Component Initialized!\n";

    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    double t = 0.0;
    while(CxUtils::GetChar() != 27)
    {      
        // Simulate fake range sensor data.
        std::vector<double> rangeScan;
        for(double angle = -rangeSensorConfig.mScanAngle/2.0;
            angle <= rangeSensorConfig.mScanAngle/2.0;
            angle += rangeSensorConfig.mAngleIncrement)
        {
            //rangeScan.push_back(fabs(sin(t++)*rangeSensorConfig.mMaxRange*1000.0));
            if(angle < 0.0)
            {
                rangeScan.push_back(3.0); // Meters.
            }
            else
            {
                rangeScan.push_back(6.0); // Meters.
            }
        }
        rangeSensor->SetLocalRangeScan(rangeSensorConfig.mID,
                                       Point3D(0, 0, 0),   // At origin of vehicle.
                                       Point3D(0, 0, 0),   // Facing front with no roll or pitch change.
                                       rangeScan);
        
        CxUtils::SleepMs(10); 
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */
