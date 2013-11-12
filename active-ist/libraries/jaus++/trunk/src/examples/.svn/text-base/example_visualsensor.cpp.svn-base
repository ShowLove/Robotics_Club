////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_visualsensor.cpp
///  \brief This file contains a test/example program for the Visual Sensor
///         service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 January 2010
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
#include "jaus/extras/video/visualsensor.h"
#include <cxutils/keyboard.h>
#include <iostream>
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

    // In this example, we are testing the Visual Sensor
    // service, so lets add it.
    component.AddService(new VisualSensor(false));

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    if(component.LoadSettings("settings/services.xml") == false)
    {
        // Working directory probably not set (or not running from output directory), so
        // use default values.
        component.DiscoveryService()->SetSubsystemIdentification(Subsystem::Vehicle,
                                                                 "Simulation");
    }
    
    // Initialize component component with component given ID.
    if(component.Initialize(Address(500, 1, 10)) == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    VisualSensor* sensor = ((VisualSensor *)component.GetService(VisualSensor::Name));

    // Set mgsImage data to simulate a video feed.
    Image mgsImage;
    Image auvImage;
    int mgsFrameNumber = 0;
    int auvFrameNumber = 1;
    char fname[512];
    // Make sure we have image data to simulate video.
    if(mgsImage.Load("video/mgs_frames/mgs_image_000.jpg") == 0)
    {
        std::cout << "Failed to load image data for streaming.\n";
        return 0;
    }
    // MGS and AUV Images simulated.
    sensor->SetCameraCount(2); 
    
    Time::Stamp mgsUpdateTimeMs = 0;
    Time::Stamp auvUpdateTimeMs = 0;
    
    double startTime = CxUtils::Timer::GetTimeSeconds();
    unsigned int counter = 0;

    std::cout << "Running...\n";
    while(CxUtils::GetChar() != 27)
    {      
        // Load the next frame.
        //if(Time::GetUtcTimeMs() - mgsUpdateTimeMs > 5)
        {
            sprintf(fname, "video/mgs_frames/mgs_image_%03d.jpg", mgsFrameNumber++);
            if(mgsImage.Load(fname) == FAILURE)
            {
                // Loop forever (start over at mgsImage 0).
                mgsFrameNumber = 0;
                sprintf(fname, "video/mgs_frames/mgs_image_%03d.jpg", mgsFrameNumber++);
                mgsImage.Load(fname);
            }        

            // Camera 1
            sensor->SetCurrentFrame(mgsImage.mpImage,
                                    mgsImage.mWidth,
                                    mgsImage.mHeight,
                                    mgsImage.mChannels,
                                    0,
                                    mgsFrameNumber - 1,
                                    30);
            
            

            mgsUpdateTimeMs = Time::GetUtcTimeMs();
        }

        //if(Time::GetUtcTimeMs() - auvUpdateTimeMs > 5)
        {
            sprintf(fname, "video/auvc/image%03d.jpg", auvFrameNumber++);
            if(auvImage.Load(fname) == FAILURE)
            {
                auvFrameNumber = 1;
                sprintf(fname, "video/auvc/image%03d.jpg", auvFrameNumber++);
                auvImage.Load(fname);
            }
            // Camera 2
            sensor->SetCurrentFrame(auvImage.mpImage,
                                    auvImage.mWidth,
                                    auvImage.mHeight,
                                    auvImage.mChannels,
                                    1,
                                    auvFrameNumber - 1,
                                    30);
            
            

            auvUpdateTimeMs = Time::GetUtcTimeMs();
        }
        static bool first = true;
        if(component.EventsService()->HaveSubscribers(REPORT_IMAGE))
        {
            if(first)
            {
                first = false;
                startTime = CxUtils::Timer::GetTimeSeconds();
            }
            counter += 2;
        }
        else
        {
            counter = 0;
            startTime = CxUtils::Timer::GetTimeSeconds();
            std::cout << "No Subscribers.\n";
            CxUtils::SleepMs(500);
        }
        if(counter == 30)
        {
            std::cout << "FPS: " << counter/(CxUtils::Timer::GetTimeSeconds() - startTime + 0.0000001) << std::endl;
            counter = 0;
            startTime = CxUtils::Timer::GetTimeSeconds();
            std::cout << "MGS Frame Number: " << mgsFrameNumber << std::endl;
            std::cout << "AUV Frame Number: " << auvFrameNumber << std::endl;
        }

        if(CxUtils::GetChar() == 27)
        {
            break;
        }
        CxUtils::SleepMs(1);
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */
