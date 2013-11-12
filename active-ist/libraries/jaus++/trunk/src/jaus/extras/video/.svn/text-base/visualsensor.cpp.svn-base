////////////////////////////////////////////////////////////////////////////////////
///
///  \file visualsensor.cpp
///  \brief Contains the Visual Sensor Service implementation.
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
#include "jaus/extras/video/visualsensor.h"
#include "jaus/extras/video/querycameracount.h"
#include "jaus/extras/video/reportcameracount.h"
#include "jaus/extras/video/sharedimage.h"

using namespace JAUS;

const std::string VisualSensor::Name = "urn:jaus:jss:jpp:extras:VisualSensor";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] sharedImage If true, images are shaved to a shared memory
///                          buffer, for sharing image data on a single host
///                          computer (e.g. VideoSubsriber, SharedImage).
///
////////////////////////////////////////////////////////////////////////////////////
VisualSensor::VisualSensor(const bool sharedImage) : Events::Child(Service::ID(VisualSensor::Name),
                                                                   Service::ID(Events::Name))
{
    mSharedMemoryImageFlag = false;
    mSharedMemoryImageFlag = sharedImage;
    mCameraCount = 1;
    mQualityJPEG = -1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VisualSensor::~VisualSensor()
{
    std::map<Byte, SharedImage*>::iterator simg;
    WriteLock wLock(mVisualSensorMutex);
    for(simg = mSharedImages.begin();
        simg != mSharedImages.end();
        simg++)
    {
        delete simg->second;
    }
    mSharedImages.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets how many cameras are supported by this serivice so that
///          subscribers can query video data.
///
///   \param[in] cameraCount Number of cameras connected to Visual Sensor service.
///
////////////////////////////////////////////////////////////////////////////////////
void VisualSensor::SetCameraCount(const Byte cameraCount)
{
    mCameraCount = cameraCount;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current frame from a camera.
///
///   \param[in] rawImage Raw image data in BGR format with 0,0 being the top
///                       left corner of the image.
///   \param[in] width Width of image in pixels.
///   \param[in] height Height of the image in pixels.
///   \param[in] channels Number of channels in the image.
///   \param[in] cameraID The camera ID number.
///   \param[in] frameRateHz The update frequency of the camera.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::SetCurrentFrame(const unsigned char* rawImage,
                                   const unsigned int width,
                                   const unsigned int height,
                                   const unsigned char channels,
                                   const Byte cameraID,
                                   const double frameRateHz)
{
    std::map<UShort, ReportImage>* report;
    {
        WriteLock wLock(mVisualSensorMutex);
        report = (std::map<UShort, ReportImage>*)(&mCompressedData);
        if(mCameraCount < (Byte)mCompressedData.size())
        {
            mCameraCount = (Byte)mCompressedData.size();
        }
    }
    return SetCurrentFrame(rawImage,
                           width,
                           height,
                           channels,
                           cameraID,
                           (*report)[cameraID].GetFrameNumber() + 1,
                           frameRateHz);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current frame from a camera.
///
///   \param[in] rawImage Raw image data in BGR format with 0,0 being the top
///                       left corner of the image.
///   \param[in] width Width of image in pixels.
///   \param[in] height Height of the image in pixels.
///   \param[in] channels Number of channels in the image.
///   \param[in] cameraID The camera ID number.
///   \param[in] frameNumber The frame sequence number.
///   \param[in] frameRateHz The update frequency of the camera.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::SetCurrentFrame(const unsigned char* rawImage,
                                   const unsigned int width,
                                   const unsigned int height,
                                   const unsigned char channels,
                                   const Byte cameraID,
                                   const unsigned int frameNumber,
                                   const double frameRateHz)
{
    {
        WriteLock wfLock(mFrameRatesMutex);
        // Initialize frame rates.
        mFrameRates[cameraID] = frameRateHz;
    }


    std::map<Byte, SharedImage*>::iterator simg;
    simg = mSharedImages.find(cameraID);
    if(simg == mSharedImages.end())
    {
        SharedImage* sm = new SharedImage();
        if(sm->CreateSharedImage(GetComponentID(), cameraID, width*height*channels*2))
        {
            mSharedImages[cameraID] = sm;
            simg = mSharedImages.find(cameraID);
            //simg->second->RegisterCallback(VisualSensor::SharedImageCallback, this);
        }
    }
    if(simg != mSharedImages.end())
    {
        //simg->second->RegisterCallback(VisualSensor::SharedImageCallback, this);
        simg->second->SetFrame(rawImage,
                                width,
                                height,
                                channels,
                                frameNumber);
    }

    

    WriteLock wLock(mVisualSensorMutex);
   
    std::map<UShort, ReportImage>* report = (std::map<UShort, ReportImage>*)(&mCompressedData);
    (*report)[cameraID].SetCameraID(cameraID);
    (*report)[cameraID].SetFrameNumber( frameNumber );

    if(mPendingQueryMap.size() > 0 || EventsService()->HaveSubscribers(REPORT_IMAGE))
    {

        // Compress image.
        (*report)[cameraID].SetImageFormat(Image::JPEG);
        Packet* payload = (Packet *)(*report)[cameraID].GetImage();
        payload->Clear();

        // Reserve enough memory to store image + payload header.
        payload->Reserve(width*height*channels);

        unsigned int jpegSize = 0;
        CxUtils::JPEG::Compressor* jpeg = (CxUtils::JPEG::Compressor*)&mJPEG;
        jpeg->CompressImageNoResize(width,
                                    height,
                                    channels,
                                    rawImage,
                                    payload->Ptr(),
                                    payload->Reserved(),
                                    &jpegSize,
                                    mQualityJPEG);
        payload->SetLength(jpegSize);
        payload->SetWritePos(payload->Length());

        std::map<UShort, QueryImage>::iterator query;
        for(query = mPendingQueryMap.begin();
            query != mPendingQueryMap.end();
            query++)
        {
            (*report)[cameraID].SetSourceID(GetComponentID());
            (*report)[cameraID].SetDestinationID(query->second.GetSourceID());
            Send(&(*report)[cameraID]);
        }
        mPendingQueryMap.clear();

        // Normally we could use the generic SignalEvent method, however
        // we only want to signal events for a change to this specific
        // camera.  That way we don't generate an event for all cameras when
        // only 1 has updated.
        Events::Subscription::List myEvents = GetComponent()->EventsService()->GetProducedEvents(REPORT_IMAGE);
        Events::Subscription::List::iterator e;
        for(e = myEvents.begin();
            e != myEvents.end();
            e++)
        {
            const QueryImage* query = dynamic_cast<const QueryImage*>(e->mpQueryMessage);
            if(query && query->GetCameraID() == cameraID)
            {
                SignalEvent((*e));
            }
        }
    }
    
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current frame from a camera using an already
///          compressed format.
///
///   \param[in] compImage Compressed image data.
///   \param[in] compImageSize Compressed image size in bytes.
///   \param[in] width Width of image in pixels.
///   \param[in] height Height of the image in pixels.
///   \param[in] channels Number of channels in the image.
///   \param[in] cameraID The camera ID number.
///   \param[in] frameNumber The frame sequence number.
///   \param[in] frameRateHz The update frequency of the camera.
///   \param[in] format Compressed image format.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::SetCurrentFrameCompressed(const unsigned char* compImage,
                                             const unsigned int compImageSize,
                                             const unsigned int width,
                                             const unsigned int height,
                                             const unsigned char channels,
                                             const Byte cameraID,
                                             const unsigned int frameNumber,
                                             const double frameRateHz,
                                             const Image::Format format)
{
    if(format != Image::MJPEG && format != Image::JPEG)
    {
        std::cout << "JAUS::VisualSensor::SetCurrentFrameCompressed: Only MJPEG and JPEG supported.\n";
        return false;
    }

    {
        WriteLock wfLock(mFrameRatesMutex);
        // Initialize frame rates.
        mFrameRates[cameraID] = frameRateHz;
    }

    WriteLock wLock(mVisualSensorMutex);

    std::map<UShort, ReportImage>* report = (std::map<UShort, ReportImage>*)(&mCompressedData);
    (*report)[cameraID].SetCameraID(cameraID);
    (*report)[cameraID].SetFrameNumber( frameNumber );

    if(mSharedMemoryImageFlag)
    {
        std::map<Byte, SharedImage*>::iterator simg;
        simg = mSharedImages.find(cameraID);
        if(simg != mSharedImages.end())
        {
            // Shared image only works with RAW data,
            // and we may not know how to decompress.
            simg->second->CloseSharedImage();
            delete simg->second;
        }
    }

    if(mPendingQueryMap.size() > 0 || EventsService()->HaveSubscribers(REPORT_IMAGE))
    {
        // Compress image.
        (*report)[cameraID].SetImageFormat(Image::JPEG);
        Packet* payload = (Packet *)(*report)[cameraID].GetImage();
        payload->Clear();

        // Reserve enough memory to store image + payload header.
        unsigned int payloadSize = width*height*channels + 256;
        payload->Reserve(payloadSize);
        int finalCompImageSize = compImageSize;
        unsigned char* ptr = payload->Ptr();
        if(format == Image::MJPEG)
        {
            finalCompImageSize = CxUtils::JPEG::ConvertMJPEG2JPEG(compImage, compImageSize, &ptr, &payloadSize);
        }
        else
        {
            payload->Write(compImage, compImageSize);
        }
        payload->SetLength(finalCompImageSize);
        payload->SetWritePos(payload->Length());

        std::map<UShort, QueryImage>::iterator query;
        for(query = mPendingQueryMap.begin();
            query != mPendingQueryMap.end();
            query++)
        {
            (*report)[cameraID].SetSourceID(GetComponentID());
            (*report)[cameraID].SetDestinationID(query->second.GetSourceID());
            Send(&(*report)[cameraID]);
        }
        mPendingQueryMap.clear();

        // Normally we could use the generic SignalEvent method, however
        // we only want to signal events for a change to this specific
        // camera.  That way we don't generate an event for all cameras when
        // only 1 has updated.
        Events::Subscription::List myEvents = GetComponent()->EventsService()->GetProducedEvents(REPORT_IMAGE);
        Events::Subscription::List::iterator e;
        for(e = myEvents.begin();
            e != myEvents.end();
            e++)
        {
            const QueryImage* query = dynamic_cast<const QueryImage*>(e->mpQueryMessage);
            if(query && query->GetCameraID() == cameraID)
            {
                SignalEvent((*e));
            }
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Generates an event for the given information.
///
///   \param[in] info The event information (ID, Sequence #, etc.) for generation.
///
///   \return True if event generated, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_IMAGE)
    {
        const QueryImage* query = dynamic_cast<const QueryImage*>(info.mpQueryMessage);

        std::map<UShort, ReportImage>* report = (std::map<UShort, ReportImage>*)(&mCompressedData);
        (*report)[query->GetCameraID()].SetCameraID(query->GetCameraID());
        (*report)[query->GetCameraID()].SetImageFormat(Image::JPEG);

        Packet* payload = (Packet *)(*report)[query->GetCameraID()].GetImage();

        // If we have a compressed version of the image data...
        if(payload->Length() > 0)
        {
            SendEvent(info, &(*report)[query->GetCameraID()]);
            if(mCameraCount > 1)
            {
                //CxUtils::SleepMs(1);
            }
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the event is supported by the Service.
///
///   \param[in] type The event type (Periodic/EveryChange).
///   \param[in] requestedPeriodicRate If type == Periodic, then this is the
///                                    desired update rate.
///   \param[in] queryMessage The query message associated with the event.
///   \param[out] confirmedPeriodicRate This is the confirmed periodic rate
///                                     supported by the Service.
///   \param[out] errorMessage If not supported, this is an optional error message.
///
///   \return True if event supported, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::IsEventSupported(const Events::Type type,
                                    const double requestedPeriodicRate,
                                    const Message* queryMessage,
                                    double& confirmedPeriodicRate,
                                    std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_IMAGE)
    {
        const QueryImage* query = dynamic_cast<const QueryImage*>(queryMessage);

        ReadLock rfLock(*( (SharedMutex*)&mFrameRatesMutex));
        // Currently only JPEG is supported.
        if(query->GetFormat() != Image::JPEG)
        {
            return false;
        }
        std::map<Byte, double>::const_iterator rate = mFrameRates.find(query->GetCameraID());
        double hz = 30.0;
        if(rate != mFrameRates.end())
        {
            hz = rate->second;
        }
        else
        {
            // Unknown camera ID.
            return false;
        }

        if(hz < requestedPeriodicRate)
        {
            confirmedPeriodicRate = hz;
        }
        else
        {
            confirmedPeriodicRate = requestedPeriodicRate;
        }

        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports the following message: Query Image, Report Image
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void VisualSensor::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_IMAGE:
        {
            const QueryImage* query = dynamic_cast<const QueryImage*>(message);
            if(query)
            {
                WriteLock wLock(mVisualSensorMutex);
                mPendingQueryMap[query->GetSourceID().mSubsystem] = *query;
            }
        }
        break;
    case QUERY_CAMERA_COUNT:
        {
            ReportCameraCount report(message->GetSourceID(), GetComponentID());
            {
                ReadLock rLock(mFrameRatesMutex);
                std::map<Byte, double>::const_iterator f;
                for(f = mFrameRates.begin();
                    f != mFrameRates.end();
                    f++)
                {
                    report.GetCameraList()->push_back(f->first);
                }
            }
            Send(&report);
        }
        break;
    case REPORT_IMAGE:
        {

        }
        break;
    default:
        break;
    }
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Attempts to create the message desired.  Only message supported
///          by this Service can be created by this Service.
///
///   This Service supports the following message: Query Image, Report Image
///
///   \param[in] messageCode Message to create.
///
///   \return Pointer to newly allocated Message data, NULL if message is not
///           supported by the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* VisualSensor::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_IMAGE:
        message = new QueryImage();
        break;
    case QUERY_CAMERA_COUNT:
        message = new QueryCameraCount();
        break;
    case REPORT_IMAGE:
        message = new ReportImage();
        break;
    case REPORT_CAMERA_COUNT:
        message = new ReportCameraCount();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets quality for JPEG compression.
///
///   By default, quality is -1 or best.
///
///   \param[in] quality The compression quality.  Set to -1 for default, 0 worst
///                      and 100 best (but larger size).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VisualSensor::SetCompressionQualityJPEG(const int quality)
{
    if(quality >= -1 && quality <= 100)
    {
        WriteLock wLock(mVisualSensorMutex);
        mQualityJPEG = quality;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the visual sensor.
///
////////////////////////////////////////////////////////////////////////////////////
void VisualSensor::PrintStatus() const
{
    std::map<Byte, UInt> frameCounts;
    std::map<Byte, ReportImage>::const_iterator i;
    {
        ReadLock rLock(* ((SharedMutex*)&mVisualSensorMutex));
        for(i = mCompressedData.begin();
            i != mCompressedData.end();
            i++)
            {
                frameCounts[i->first] = i->second.GetFrameNumber();
            }
    }

    std::cout << "[" << GetServiceID().ToString() << "] - " << GetComponentID().ToString() << "\n";

    std::map<Byte, UInt>::iterator fc;
    for(fc = frameCounts.begin();
        fc != frameCounts.end();
        fc++)
    {
        std::cout << "Camera [" << (int)fc->first << "] - Frame Count: " << fc->second << std::endl;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback from shared images.
///
////////////////////////////////////////////////////////////////////////////////////
void VisualSensor::SharedImageCallback(const Address& source,
                                       const Byte cameraID,
                                       const Image& img,
                                       const unsigned int frameNumber,
                                       void* fargs)
{
    VisualSensor* video = (VisualSensor*)fargs;

	std::cout << "CALLBACK!";
    WriteLock wLock(video->mVisualSensorMutex);
   
    std::map<UShort, ReportImage>* report = (std::map<UShort, ReportImage>*)(&video->mCompressedData);
    (*report)[cameraID].SetCameraID(cameraID);
    (*report)[cameraID].SetFrameNumber( frameNumber );

    if(video->mPendingQueryMap.size() > 0 || video->EventsService()->HaveSubscribers(REPORT_IMAGE))
    {
        // Compress image.
        (*report)[cameraID].SetImageFormat(Image::JPEG);
        Packet* payload = (Packet *)(*report)[cameraID].GetImage();
        payload->Clear();

        // Reserve enough memory to store image + payload header.
        payload->Reserve(img.mWidth*img.mHeight*img.mChannels);

        unsigned int jpegSize = 0;
        CxUtils::JPEG::Compressor* jpeg = (CxUtils::JPEG::Compressor*)&video->mJPEG;
        jpeg->CompressImageNoResize(img.mWidth,
                                    img.mHeight,
                                    img.mChannels,
                                    img.mpImage,
                                    payload->Ptr(),
                                    payload->Reserved(),
                                    &jpegSize,
                                    video->mQualityJPEG);
        payload->SetLength(jpegSize);
        payload->SetWritePos(payload->Length());

        std::map<UShort, QueryImage>::iterator query;
        for(query = video->mPendingQueryMap.begin();
            query != video->mPendingQueryMap.end();
            query++)
        {
            (*report)[cameraID].SetSourceID(video->GetComponentID());
            (*report)[cameraID].SetDestinationID(query->second.GetSourceID());
            video->Send(&(*report)[cameraID]);
        }
        video->mPendingQueryMap.clear();

        // Normally we could use the generic SignalEvent method, however
        // we only want to signal events for a change to this specific
        // camera.  That way we don't generate an event for all cameras when
        // only 1 has updated.
        Events::Subscription::List myEvents = video->GetComponent()->EventsService()->GetProducedEvents(REPORT_IMAGE);
        Events::Subscription::List::iterator e;
        for(e = myEvents.begin();
            e != myEvents.end();
            e++)
        {
            const QueryImage* query = dynamic_cast<const QueryImage*>(e->mpQueryMessage);
            if(query && query->GetCameraID() == cameraID)
            {
				std::cout << "EVENT!\n";
                video->SignalEvent((*e));
            }
        }
    }
}


/*  End of File */
