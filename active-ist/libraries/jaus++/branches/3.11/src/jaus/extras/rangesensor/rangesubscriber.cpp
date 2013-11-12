////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesubscriber.h
///  \brief Contains the Range Sensor Subscriber service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 11 April 2010
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
#include "jaus/extras/rangesensor/rangesubscriber.h"
#include <cxutils/math/cxmath.h>
using namespace JAUS;

const std::string RangeSubscriber::Name = "urn:jaus:jss:jpp:extras:RangeSubscriber";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSubscriber::RangeSubscriber() : Management::Child(Service::ID(RangeSubscriber::Name),
                                                       Service::ID(Management::Name))
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSubscriber::~RangeSubscriber()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called on Shutdown.
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSubscriber::Shutdown()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Create a subscription to range data.
///
///   \param[in] id The component ID to get video data from.
///   \param[in] deviceID The sensor/source on the component.
///   \param[in] waitTimeMs How long to wait in ms before timeout on request.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RangeSubscriber::CreateRangeSubscription(const Address& id, 
                                              const Byte deviceID,
                                              const unsigned int waitTimeMs)
{
    QueryRangeSensorConfiguration queryConfig(id, GetComponentID());
    Send(&queryConfig);

    QueryLocalRangeScan queryEvent;
    queryEvent.SetSensorID(deviceID);
    return EventsService()->RequestEveryChangeEvent(id, &queryEvent, waitTimeMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Query a component with a Range Sensor service for number
///          of devices.
///
///   \param[in] id The component ID to get range data from.
///   \param[out] list Range sensor data.
///   \param[in] waitTimeMs How long to wait in ms before timeout on request.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RangeSubscriber::GetRangeSensorInfo(const Address& id, 
                                         RangeSensorConfig::List& list,
                                         const unsigned int waitTimeMs) const
{
    QueryRangeSensorConfiguration query(id, GetComponentID());
    ReportRangeSensorConfiguration report;
    if(Send(&query, &report, waitTimeMs))
    {
        list = *report.GetConfiguration();
        return list.size() > 0 ? true : false;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if a video subscription is present.
///
///   \param[in] id ID of the source of the subscription.
///   \param[in] deviceID ID of the sensor, set to -1 for any sensor.
///
///   \return True if a range subscription exists, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool RangeSubscriber::HaveRangeSubscription(const Address& id,
                                            const int deviceID) const
{
    bool result = false;
    Events::Subscription::List list = GetComponent()->EventsService()->GetSubscriptions(id, REPORT_LOCAL_RANGE_SCAN);
    Events::Subscription::List::iterator s;
    for(s = list.begin();
        s != list.end();
        s++)
    {
        if(deviceID < 0)
        {
            result = true;
            break;
        }
        else
        {
            QueryLocalRangeScan* query = dynamic_cast<QueryLocalRangeScan *>(s->mpQueryMessage);
            if(query && query->GetSensorID() == (Byte)deviceID)
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
///   \brief Method to check cancel a range subscription.
///
///   \param[in] id ID of the source of the subscription.
///   \param[in] deviceID ID of the sensor, set to -1 for any sensor.
///
///   \return True if a range subscription exists, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool RangeSubscriber::CancelRangeSubscription(const Address& id,
                                              const int deviceID)
{
    bool result = false;

    Events::Subscription::List list = GetComponent()->EventsService()->GetSubscriptions(id, REPORT_LOCAL_RANGE_SCAN);
    Events::Subscription::List::iterator s;
    for(s = list.begin();
        s != list.end();
        s++)
    {
        if(deviceID < 0)
        {
            if(GetComponent()->EventsService()->CancelSubscription(s->mProducer,
                                                                   REPORT_LOCAL_RANGE_SCAN,
                                                                   s->mID,
                                                                   Service::DefaultWaitMs))
            {
                result = true;
            }
        }
        else
        {
            QueryLocalRangeScan* query = dynamic_cast<QueryLocalRangeScan *>(s->mpQueryMessage);
            if(query && query->GetSensorID() == (Byte)deviceID)
            {
                if(GetComponent()->EventsService()->CancelSubscription(s->mProducer,
                                                                       REPORT_LOCAL_RANGE_SCAN,
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
///   \brief Method to register a callback to receive range data as it arrives.
///
///   \param[in] callback Pointer to callback to add/remove.
///   \param[in] add If true, callback is added, if false, it is removed.
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSubscriber::RegisterCallback(Callback* callback, const bool add)
{
    Mutex::ScopedLock lock(&mRangeCallbacksMutex);
    if(add)
    {
        mRangeCallbacks.insert(callback);
    }
    else
    {
        Callback::Set::iterator cb;

        cb = mRangeCallbacks.find(callback);
        if(cb != mRangeCallbacks.end())
        {
            mRangeCallbacks.erase(cb);
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
void RangeSubscriber::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case REPORT_LOCAL_RANGE_SCAN:
        {
            const ReportLocalRangeScan* report = 
                    dynamic_cast<const ReportLocalRangeScan*>(message);
            if(report)
            {
                Point3D::List scan;
                Point3D::List scanPolar;
                mRangeSensorMutex.Lock();
                
                std::map<Address, RangeSensorConfig::Map>::iterator comp;
                comp = mRangeSensors.find(report->GetSourceID());
                if(comp != mRangeSensors.end())
                {
                    RangeSensorConfig::Map::iterator config = comp->second.find(report->GetSensorID());
                    if(config != comp->second.end())
                    {
                        // Convert the data using the configuration info.
                        ReportLocalRangeScan::Scan::const_iterator v;
                        const ReportLocalRangeScan::Scan* ptr = report->GetScan();
                        double angle = config->second.mScanAngle/-2.0;
                        double incr = config->second.mAngleIncrement;
                        double divider = 1000.0;
                        if(config->second.mUnitType == RangeSensorConfig::CM)
                        {
                            divider = 100.0;
                        }

                        for(v = ptr->begin();
                            v != ptr->end();
                            v++)
                        {
                            Point3D point((*v)/divider);  // Convert to meters.
                            Point3D polar(point.mX);
                            
                            // Convert to cartesian coordinates, relative to
                            // the sensor.
                            point = point.Rotate(-angle, Point3D::Z, false);
                            
                            // Now translate and rotate relative to the
                            // platform frame.                                                     
                            point = point.Rotate(report->GetSensorOrientation().mX, Point3D::X, false);
                            point = point.Rotate(report->GetSensorOrientation().mY, Point3D::Y, false);
                            point = point.Rotate(-report->GetSensorOrientation().mZ, Point3D::Z, false);
                            point += report->GetSensorLocation();
                            
                            // Now add to final scan.
                            scan.push_back(point);

                            polar.mY = atan2(point.mZ, point.mX);
                            polar.mZ = atan2(point.mY, point.mX);
                            scanPolar.push_back(polar);

                            angle += incr;
                        }                        
                    }
                }

                mRangeSensorMutex.Unlock();

                ProcessLocalRangeScan(scan,
                                      report->GetSourceID(),
                                      report->GetSensorID(),
                                      report->GetTimeStamp());

                mRangeCallbacksMutex.Lock();

                Callback::Set::iterator cb;
                for(cb = mRangeCallbacks.begin();
                    cb != mRangeCallbacks.end();
                    cb++)
                {
                    (*cb)->ProcessLocalRangeScan(scan,
                                                 report->GetSourceID(),
                                                 report->GetSensorID(),
                                                 report->GetTimeStamp());
                    (*cb)->ProcessLocalRangeScanPolar(scanPolar,
                                                      report->GetSourceID(),
                                                      report->GetSensorID(),
                                                      report->GetTimeStamp());
                }

                mRangeCallbacksMutex.Unlock();
                
            }
        }
        break;
    case REPORT_RANGE_SENSOR_CONFIGURATION:
        {
            const ReportRangeSensorConfiguration* report = 
                dynamic_cast<const ReportRangeSensorConfiguration*>(message);
            if(report)
            {
                Mutex::ScopedLock lock(&mRangeSensorMutex);
                RangeSensorConfig::List::const_iterator c;
                for(c = report->GetConfiguration()->begin();
                    c != report->GetConfiguration()->end();
                    c++)
                {
                    mRangeSensors[report->GetSourceID()][c->mID] = *c;
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
///   \param[in] messageCode Message to create.
///
///   \return Pointer to newly allocated Message data, NULL if message is not
///           supported by the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* RangeSubscriber::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_LOCAL_RANGE_SCAN:
        message = new QueryLocalRangeScan();
        break;
    case QUERY_RANGE_SENSOR_CONFIGURATION:
        message = new QueryRangeSensorConfiguration();
        break;
    case REPORT_RANGE_SENSOR_CONFIGURATION:
        message = new ReportRangeSensorConfiguration();
        break;
    case REPORT_LOCAL_RANGE_SCAN:
        message = new ReportLocalRangeScan();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


/*  End of File */
