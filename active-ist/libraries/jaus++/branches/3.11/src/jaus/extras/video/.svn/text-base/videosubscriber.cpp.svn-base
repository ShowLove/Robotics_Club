////////////////////////////////////////////////////////////////////////////////////
///
///  \file videosubscriber.cpp
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
#include "jaus/extras/video/videosubscriber.h"
#include "jaus/extras/video/querycameracount.h"
#include "jaus/extras/video/reportcameracount.h"
#include "jaus/extras/video/sharedimage.h"

using namespace JAUS;

const std::string VideoSubscriber::Name = "urn:jaus:jss:jpp:extras:VideoSubscriber";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VideoSubscriber::VideoSubscriber() : Management::Child(Service::ID(VideoSubscriber::Name),
                                                       Service::ID(Management::Name))
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VideoSubscriber::~VideoSubscriber()
{
    Mutex::ScopedLock smLock(&mSharedImagesMutex);
    std::vector<SharedImage*>::iterator sm;
    for(sm = mSharedImages.begin();
        sm != mSharedImages.end();
        sm++)
    {
        (*sm)->CloseSharedImage();
        delete (*sm);
    }
    mSharedImages.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called on Shutdown.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoSubscriber::Shutdown()
{
    Mutex::ScopedLock smLock(&mSharedImagesMutex);
    std::vector<SharedImage*>::iterator sm;
    for(sm = mSharedImages.begin();
        sm != mSharedImages.end();
        sm++)
    {
        (*sm)->CloseSharedImage();
        delete (*sm);
    }
    mSharedImages.clear();
    Management::Child::Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Create a subscription to video data.
///
///   \param[in] id The component ID to get video data from.
///   \param[in] cameraID The camera/source on the component.
///   \param[in] waitTimeMs How long to wait in ms before timeout on request.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoSubscriber::CreateVideoSubscription(const Address& id, 
                                              const Byte cameraID,
                                              const unsigned int waitTimeMs)
{
    std::vector<SharedImage*>::iterator sm;
    Mutex::ScopedLock smLock(&mSharedImagesMutex);
    for(sm = mSharedImages.begin();
        sm != mSharedImages.end();
        sm++)
    {
        if((*sm)->GetSourceID() == id && (*sm)->GetCameraID() == cameraID)
        {
            return true;
        }
    }
    
    // Try create a shared image connection first.
    SharedImage* simage = new SharedImage();
    if(simage->OpenSharedImage(id, cameraID))
    {
        simage->RegisterCallback(SharedImageCallback, this);
        mSharedImages.push_back(simage);
        return true;
    }
    delete simage;
    
    QueryImage query;
    query.SetFormat(Image::JPEG);
    query.SetCameraID(cameraID);
    return EventsService()->RequestEveryChangeEvent(id, &query, waitTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Query a component with a Visual Sensor service for number
///          of cameras.
///
///   \param[in] id The component ID to get video data from.
///   \param[out] count Number of cameras on component.
///   \param[in] waitTimeMs How long to wait in ms before timeout on request.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoSubscriber::GetCameraCount(const Address& id, 
                                     unsigned int& count,
                                     const unsigned int waitTimeMs) const
{
    QueryCameraCount query(id, GetComponentID());
    ReportCameraCount report;
    count = 0;
    if(Send(&query, &report, waitTimeMs))
    {
        count = (unsigned int)report.GetCameraList()->size();
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Query a component with a Visual Sensor service for number
///          of cameras.
///
///   \param[in] id The component ID to get video data from.
///   \param[out] list List of component IDs of cameras.
///   \param[in] waitTimeMs How long to wait in ms before timeout on request.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoSubscriber::GetCameraList(const Address& id, 
                                    std::vector<Byte>& list,
                                    const unsigned int waitTimeMs) const
{
    QueryCameraCount query(id, GetComponentID());
    ReportCameraCount report;
    list.clear();
    if(Send(&query, &report, waitTimeMs))
    {
        list = *report.GetCameraList();
        return list.size() > 0 ? true : false;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if a video subscription is present.
///
///   \param[in] id ID of the source of the subscription.
///   \param[in] cameraID ID of the camera, set to -1 for any camera.
///
///   \return True if a video subscription exists, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoSubscriber::HaveVideoSubscription(const Address& id,
                                            const int cameraID) const
{
    bool result = false;

    std::vector<SharedImage*>::const_iterator sm;
    Mutex::ScopedLock smLock(&mSharedImagesMutex);
    for(sm = mSharedImages.begin();
        sm != mSharedImages.end();
        sm++)
    {
        if((*sm)->GetSourceID() == id && (*sm)->GetCameraID() == cameraID)
        {
            return true;
        }
    }

    Events::Subscription::List list = GetComponent()->EventsService()->GetSubscriptions(id, REPORT_IMAGE);
    Events::Subscription::List::iterator s;
    for(s = list.begin();
        s != list.end();
        s++)
    {
        if(cameraID < 0)
        {
            result = true;
            break;
        }
        else
        {
            QueryImage* query = dynamic_cast<QueryImage *>(s->mpQueryMessage);
            if(query && query->GetCameraID() == (Byte)cameraID)
            {
                result = true;
                break;
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check cancel a video subscription.
///
///   \param[in] id ID of the source of the subscription.
///   \param[in] cameraID ID of the camera, set to -1 for any camera.
///
///   \return True if a video subscription exists, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool VideoSubscriber::CancelVideoSubscription(const Address& id,
                                              const int cameraID)
{
    bool result = false;

    std::vector<SharedImage*>::iterator sm;
    Mutex::ScopedLock smLock(&mSharedImagesMutex);
    sm = mSharedImages.begin();
    while(sm != mSharedImages.end())
    {
        if(id.IsValid() == false ||
           ((*sm)->GetSourceID() == id && (*sm)->GetCameraID() == cameraID) ||
           ((*sm)->GetSourceID() == id && cameraID < 0))
        {
            (*sm)->CloseSharedImage();
            delete (*sm);
            mSharedImages.erase(sm);
            sm = mSharedImages.begin();
            result = true;
        }
        else
        {
            sm++;
        }
    }

    Events::Subscription::List list = GetComponent()->EventsService()->GetSubscriptions(id, REPORT_IMAGE);
    Events::Subscription::List::iterator s;
    for(s = list.begin();
        s != list.end();
        s++)
    {
        if(cameraID < 0)
        {
            if(GetComponent()->EventsService()->CancelSubscription(s->mProducer,
                                                                   REPORT_IMAGE,
                                                                   s->mID,
                                                                   Service::DefaultWaitMs))
            {
                result = true;
            }
        }
        else
        {
            QueryImage* query = dynamic_cast<QueryImage *>(s->mpQueryMessage);
            if(query && query->GetCameraID() == (Byte)cameraID)
            {
                if(GetComponent()->EventsService()->CancelSubscription(s->mProducer,
                                                                       REPORT_IMAGE,
                                                                       s->mID,
                                                                       Service::DefaultWaitMs))
                {
                    result = true;
                }
                break;
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to register a callback to receive Video data as it arrives.
///
///   \param[in] callback Pointer to callback to add/remove.
///   \param[in] rawImage If true, register for decompressed RGB images.  If false
///                       callback is to receive the compressed video source data.
///   \param[in] add If true, callback is added, if false, it is removed.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoSubscriber::RegisterCallback(Callback* callback, const bool rawImage, const bool add)
{
    Mutex::ScopedLock lock(&mVideoCallbacksMutex);
    if(add)
    {
        if(rawImage)
        {
            mRawCallbacks.insert(callback);
        }
        else
        {
            mCompressedCallbacks.insert(callback);
        }
    }
    else
    {
        Callback::Set::iterator cb;
        if(rawImage)
        {
            cb = mRawCallbacks.find(callback);
            if(cb != mRawCallbacks.end())
            {
                mRawCallbacks.erase(cb);
            }
        }
        else
        {
            cb = mCompressedCallbacks.find(callback);
            if(cb != mCompressedCallbacks.end())
            {
                mCompressedCallbacks.erase(cb);
            }
        }

    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports the following message: Report Image
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoSubscriber::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case REPORT_IMAGE:
        {
            const ReportImage* report = dynamic_cast<const ReportImage*>(message);
            if(report)
            {
                Mutex::ScopedLock lock(&mVideoCallbacksMutex);

                Callback::Set::iterator cb;
                for(cb = mCompressedCallbacks.begin();
                    cb != mCompressedCallbacks.end();
                    cb++)
                {
                    (*cb)->ProcessCompressedVideo(report->GetSourceID(),
                                                  report->GetCameraID(),
                                                  report->GetFormat(),
                                                  report->GetImage()->Ptr(),
                                                  report->GetImage()->Length(),
                                                  report->GetFrameNumber());
                }
                // Don't decompress the image data if there are
                // no callbacks to use it.
                if(mRawCallbacks.size() > 0)
                {
                    Image raw;
                    // Decompress and trigger callbacks.
                    if(raw.Decompress(report->GetImage()->Ptr(),
                                      report->GetImage()->Length(),
                                      report->GetFormat()))
                    {
                        for(cb = mRawCallbacks.begin();
                            cb != mRawCallbacks.end();
                            cb++)
                        {
                            (*cb)->ProcessRawImage(report->GetSourceID(),
                                                   report->GetCameraID(),
                                                   raw,
                                                   report->GetFrameNumber());
                        }
                    }
                }
            }
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
Message* VideoSubscriber::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_IMAGE:
        message = new QueryImage();
        break;
    case QUERY_CAMERA_COUNT:
        message = new JAUS::QueryCameraCount();
        break;
    case REPORT_IMAGE:
        message = new ReportImage();
        break;
    case REPORT_CAMERA_COUNT:
        message = new JAUS::ReportCameraCount();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Callback from shared images.
///
////////////////////////////////////////////////////////////////////////////////////
void VideoSubscriber::SharedImageCallback(const Address& source,
                                          const Byte cameraID,
                                          const Image& img,
                                          const unsigned int frameNumber,
                                          void* fargs)
{
    VideoSubscriber* subscriber = (VideoSubscriber*)fargs;
    Mutex::ScopedLock lock(&subscriber->mVideoCallbacksMutex);

    Callback::Set::iterator cb;
    for(cb = subscriber->mCompressedCallbacks.begin();
        cb != subscriber->mCompressedCallbacks.end();
        cb++)
    {
        (*cb)->ProcessCompressedVideo(source,
                                      cameraID,
                                      Image::RAW,
                                      img.mpImage,
                                      img.mDataSize,
                                      frameNumber);
    }
    // Don't decompress the image data if there are
    // no callbacks to use it.
    if(subscriber->mRawCallbacks.size() > 0)
    {
        for(cb = subscriber->mRawCallbacks.begin();
            cb != subscriber->mRawCallbacks.end();
            cb++)
        {
            (*cb)->ProcessRawImage(source,
                                   cameraID,
                                   img,
                                   frameNumber);
        }
    }
}


/* End of File */
