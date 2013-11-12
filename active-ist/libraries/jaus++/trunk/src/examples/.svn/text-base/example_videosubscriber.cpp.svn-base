////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_videosubscriber.cpp
///  \brief This file contains a test/example program for the Video Subscrber
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
#include "jaus/extras/video/videosubscriber.h"
#include "jaus/extras/video/visualsensor.h"
#include <cxutils/keyboard.h>
#include <cxutils/fileio.h>
#include <iostream>
#include <iomanip>
#include <algorithm>

#ifdef WIN32
#ifndef WIN64
//#include <vld.h>
#endif
#endif


using namespace JAUS;

class VideoCallback : public VideoSubscriber::Callback
{
public:
    VideoCallback() { mCompressedCount = mFrameCount = 0; }
    ~VideoCallback() {}
    virtual void ProcessCompressedVideo(const Address& source,
                                        const Byte camera,
                                        const Image::Format format,
                                        const unsigned char* compressedImage,
                                        const unsigned int compressedImageSize,
                                        const unsigned int frameNumber) 
    {
        // Process compressed data however you want.
        mCompressedCount++;
        static double startTime = CxUtils::Timer::GetTimeSeconds();
        if(mCompressedCount == 60)
        {
            std::cout << "FPS: " << mCompressedCount/(CxUtils::Timer::GetTimeSeconds() - startTime) << std::endl;
            mCompressedCount = 0;
            startTime = CxUtils::Timer::GetTimeSeconds();
        }
    }
    virtual void ProcessRawImage(const Address& source,
                                 const Byte camera,
                                 const Image& image,
                                 const unsigned int frameNumber)
    {
        // Use raw data in RGB format.
        char fname[512];
        sprintf(fname, "video/input/%03d/", camera);
        sprintf(fname, "video/input/%03d/image_%03d.jpg", camera, frameNumber);
        //CxUtils::FileIO::CreateDir("video/input/");
        //CxUtils::FileIO::CreateDir(fname);
        //image.Save(fname);
        mFrameCount++;

        if(mFrameCount%30 == 30)
        {
            std::cout << std::setw(5) << std::setfill('0') << (int)camera << " Sent Frame: " << std::setw(5) << std::setfill('0') << frameNumber << " from " << source.ToString() << std::endl;
        }
    }
    unsigned int mFrameCount;
    unsigned int mCompressedCount;
};

int main(int argc, char* argv[])
{
    Component component;
    VideoCallback callback;

    // Add the services we want our component to have
    // beyond the core set.

    // In this example, we are testing the Video Subscriber
    // service, so lets add it.
    component.AddService(new VideoSubscriber());

    // Register callback for the decompressed (RGB) images received.
    ((VideoSubscriber*)component.GetService(VideoSubscriber::Name))->RegisterCallback(&callback, false);

    // Try load settings files.
    // These files determine your UDP network 
    // settings, what Services to turn on/off 
    // or any Service specific settings. See the
    // example file for settings file format.
    component.LoadSettings("settings/services.xml");
    // In this example, lets make an OCU subsystem subscribing to video.
    component.DiscoveryService()->SetSubsystemIdentification(Subsystem::OCU,
                                                             "Test/Sim OCU");

    // Initialize component component with component a random ID.
    if(component.InitializeWithUniqueID() == false)
    {
        std::cout << "Failed to Initialize Component.\n";
        return 0;
    }

    // Allow some time to discover components on the network...
    CxUtils::SleepMs(250);
   
    Time::Stamp startTimeMs = Time::GetUtcTimeMs();
    while(CxUtils::GetChar() != 27)
    {      
        if(component.EventsService()->HaveSubscription(REPORT_IMAGE, Address()) == false)
        {
            Discovery::List subsystems = component.DiscoveryService()->GetSubsystemList();
            Discovery::List::iterator s;
            for(s = subsystems.begin();
                s != subsystems.end();
                s++)
            {
                // Try and create an event subscription to the first Visual Sensor
                // Service we can discover.
                Address visualSensorID;
                if(component.DiscoveryService()->GetSubsystem(s->first)->HaveService(VisualSensor::Name, &visualSensorID))
                {
                    VideoSubscriber* video = (VideoSubscriber*)component.GetService(VideoSubscriber::Name);
                    unsigned int count = 0;
                    bool success = false;
                    if(video->GetCameraCount(visualSensorID, count))
                    {                        
                        for(unsigned int i = 0; i < count; i++)
                        {
                            if(video->CreateVideoSubscription(visualSensorID, i))
                            {
                                std::cout << "Created subscription to " << visualSensorID.ToString() << std::endl;
                                success = true;
                            }
                        }
                    }
                    if(success)
                        break;
                }
            }
        }
        CxUtils::SleepMs(1); 
    }

    // Shutdown the components.
    component.Shutdown();

    return 0;
}


/* End of File */
