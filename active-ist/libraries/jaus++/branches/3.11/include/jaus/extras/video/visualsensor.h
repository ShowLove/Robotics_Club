////////////////////////////////////////////////////////////////////////////////////
///
///  \file visualsensor.h
///  \brief Contains the Visual Sensor Service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 15 January 2010
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
#ifndef __JAUS_EXTRAS_VIDEO_VISUAL_SENSOR__H
#define __JAUS_EXTRAS_VIDEO_VISUAL_SENSOR__H

#include "jaus/core/events/events.h"
#include "jaus/extras/video/reportimage.h"
#include "jaus/extras/video/queryimage.h"
#include <cxutils/images/image.h>

namespace JAUS
{
    typedef CxUtils::Image Image;
    class SharedImage;

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class VisualSensor
    ///   \brief This service provides a means to share image or video data
    ///          captured from a camera or video device.
    ///
    ///   The Visual Sensor service is independent of the type of device the
    ///   data comes from.  It is up to the user to implement their own video
    ///   capture software to get image data from a camera.  This service only
    ///   packages the data in a JAUS Message and transmits to subscribers.
    ///
    ///   This service currently only supports JPEG compression.
    ///
    ///   An example subscriber is the Video Subscriber service.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class JAUS_EXTRAS_DLL VisualSensor : public Events::Child
    {
    public:
        const static std::string Name; ///< String name of the Service.
        // Constructor.
        VisualSensor(const bool sharedImage = true);
        // Destructor.
        ~VisualSensor();
        // Sets the number of cameras provided by the service.
        void SetCameraCount(const Byte cameraCount);
        // Set an image from a camera to share with subscribers.
        bool SetCurrentFrame(const unsigned char* rawImage,
                             const unsigned int width,
                             const unsigned int height,
                             const unsigned char channels,
                             const Byte cameraID,
                             const unsigned int frameNumber,
                             const double frameRateHz);
        // Set an image from a camera to share with subscribers.
        bool SetCurrentFrameCompressed(const unsigned char* compImage,
                                       const unsigned int compImageSize,
                                       const unsigned int width,
                                       const unsigned int height,
                                       const unsigned char channels,
                                       const Byte cameraID,
                                       const unsigned int frameNumber,
                                       const double frameRateHz,
                                       const Image::Format format);
        // Set an image from a camera to share with subscribers.
        bool SetCurrentFrame(const unsigned char* rawImage,
                             const unsigned int width,
                             const unsigned int height,
                             const unsigned char channels,
                             const Byte cameraID,
                             const double frameRateHz);
        // Method called when an Event has been signaled, generates an Event message.
        virtual bool GenerateEvent(const Events::Subscription& info) const;
        // Method called to determine if an Event is supported by the service.
        virtual bool IsEventSupported(const Events::Type type,
                                      const double requestedPeriodicRate,
                                      const Message* queryMessage,
                                      double& confirmedPeriodicRate,
                                      std::string& errorMessage) const;
        // VisualSensor is always discoverable.
        virtual bool IsDiscoverable() const { return true; }
        // Processes messages associated with the VisualSensor Services.
        virtual void Receive(const Message* message);
        // Creates messages associated with the VisualSensor Service.
        virtual Message* CreateMessage(const UShort messageCode) const;
        // Sets JPEG compression quality.
        bool SetCompressionQualityJPEG(const int quality);
        // Prints status
        virtual void PrintStatus() const;
    private:
        static void SharedImageCallback(const Address& source,
                                        const Byte cameraID,
                                        const Image& img,
                                        const unsigned int frameNumber,
                                        void* fargs);
        SharedMutex mVisualSensorMutex;                ///< Mutex for thread protection of data.
        SharedMutex mFrameRatesMutex;                  ///< Different mutex for this information because compression is slow.
        Byte mCameraCount;                             ///< Number of cameras supported by service.
        bool mSharedMemoryImageFlag;                   ///< If true, copy to shared memory buffers.
        std::map<Byte, double> mFrameRates;            ///< Camera frame rates.
        std::map<Byte, ReportImage> mCompressedData;   ///< Compressed image data.
        std::map<Byte, SharedImage*> mSharedImages;    ///< Shared memory images.
        std::map<UShort, QueryImage> mPendingQueryMap; ///< Pending image queries that want image data.
        int mQualityJPEG;                              ///< JPEG compresion quality.
        CxUtils::JPEG::Compressor mJPEG;               ///< JPEG Compression object.
    };
}

#endif
/*  End of File */
