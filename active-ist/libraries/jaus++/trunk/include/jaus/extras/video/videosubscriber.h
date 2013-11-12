////////////////////////////////////////////////////////////////////////////////////
///
///  \file videosubscriber.h
///  \brief Contains the Video Subscriber service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 23 January 2010
///  Copyright (c) 2010
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
#ifndef __JAUS_EXTRAS_VIDEO_VIDEO_SUBSCRIBER__H
#define __JAUS_EXTRAS_VIDEO_VIDEO_SUBSCRIBER__H

#include "jaus/core/management/management.h"
#include "jaus/extras/video/reportimage.h"
#include "jaus/extras/video/queryimage.h"
#include <cxutils/images/image.h>

namespace JAUS
{
    typedef CxUtils::Image Image;

    class SharedImage;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class VideoSubscriber
    ///   \brief This service is used to subscribe to video data from components
    ///          with the Visual Sensor service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL VideoSubscriber : public Management::Child
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Callback
        ///   \brief Callback class used to receive image/video data as it arrives.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class JAUS_EXTRAS_DLL Callback : public AccessControl::Callback
        {
        public:
            typedef std::set<Callback* > Set;
            Callback() {}
            virtual ~Callback() {}
            virtual void ProcessCompressedVideo(const Address& source,
                                                const Byte camera,
                                                const Image::Format format,
                                                const unsigned char* compressedImage,
                                                const unsigned int compressedImageSize,
                                                const unsigned int frameNumber) {}
            virtual void ProcessRawImage(const Address& source,
                                         const Byte camera,
                                         const Image& image,
                                         const unsigned int frameNumber) {}
        };
        const static std::string Name; ///< String name of the Service.
        // Constructor.
        VideoSubscriber();
        // Destructor.
        ~VideoSubscriber();     
        // Shutsdown the service.
        virtual void Shutdown();
        // Create a video subscription.
        bool CreateVideoSubscription(const Address& id, 
                                     const Byte cameraID = 0,
                                     const unsigned int waitTimeMs = Service::DefaultWaitMs);
        bool GetCameraCount(const Address& id, 
                            unsigned int& count,
                            const unsigned int waitTimeMs = Service::DefaultWaitMs*3) const;
        bool GetCameraList(const Address& id, 
                           std::vector<Byte>& list,
                           const unsigned int waitTimeMs = Service::DefaultWaitMs*3) const;
        // Check to see if you have a video subscription.
        bool HaveVideoSubscription(const Address& id, const int cameraID = -1) const;
        // Cancel a video subscription.
        bool CancelVideoSubscription(const Address& id = Address(), const int cameraID = -1);
        // Register to receive updates of subsystems (add or removes callback).
        void RegisterCallback(Callback* callback, const bool rawImage, const bool add = true);  
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const { return false; }
        // VideoSubscriber doesn't need to be discovered.
        virtual bool IsDiscoverable() const { return false; }
        // Processes messages associated with the VideoSubscriber Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the VideoSubscriber Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Method called when transitioning to a ready state.
        virtual bool Resume() { return true; }
        // Method called to transition due to reset.
        virtual bool Reset() { return true; }
        // Method called when transitioning to a standby state.
        virtual bool Standby() { return true; }
        // Method called when transitioning to an emergency state.
        virtual bool SetEmergency() { return true; }
        // Method called when leaving the emergency state.
        virtual bool ClearEmergency() { return true; }
        // Method called when control is released.
        virtual bool ReleaseControl() { return true; }
    private:
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const { return false; }
        static void SharedImageCallback(const Address& source,
                                        const Byte cameraID,
                                        const Image& img,
                                        const unsigned int frameNumber,
                                        void* fargs);
        Mutex mVideoCallbacksMutex;                       ///< Mutex for thread protection of callback data.
        Mutex mSharedImagesMutex;                         ///< Mutex for shared images.
        Callback::Set mRawCallbacks;                      ///< Callbacks for RAW image data.
        Callback::Set mCompressedCallbacks;               ///< Callbacks for compressed image data.
        std::vector<SharedImage*> mSharedImages;          ///< Shared memory subscriptions.
    };
}

#endif
/*  End of File */
