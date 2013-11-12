////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensor.cpp
///  \brief Contains the Range Sensor Service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 10 March 2010
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
#include "jaus/extras/rangesensor/rangesensor.h"


using namespace JAUS;


const std::string RangeSensor::Name = "urn:jaus:jss:jpp:extras:RangeSensor";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::RangeSensor() : Events::Child(Service::ID(RangeSensor::Name), Service::ID(Events::Name))
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::~RangeSensor()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Add configuration information and support for a range sensing device.
///
///   This step must be taken before call SetLocalRangeScan, otherwise the
///   data will be ignored.  It also must be used to let subscribers know
///   about what range data is available.
///
///   \param[in] configuration Range sensor configuration data.
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::AddRangeDevice(const RangeSensorConfig& configuration)
{
    Mutex::ScopedLock lock(&mRangeSensorMutex);
    mRangeSensors[configuration.mID] = configuration;
    mRangeScans[configuration.mID] = ReportLocalRangeScan();
    mRangeScans[configuration.mID].SetSourceID(GetComponentID());
    if(GetComponent())
    {
        SignalEvent(REPORT_RANGE_SENSOR_CONFIGURATION);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set the current scan collected/received from the device.
///
///   \param[in] deviceID Device ID number.
///   \param[in] deviceLocation Location on the paltform relative to
///                             it's origin when the scan was performed (meters).
///                             Values are in the platform coordinate frame (e.g.
///                             positive x is in front of vehicle).
///   \param[in] deviceOrientation Orientation of the device (x = roll, y = pitch,
///                                z = yaw), when the scan was performed.
///   \param[in] scan The scan/range data collected.  The units will either be in
///                   MM or CM depending on the device and must match the
///                   unit type set in the RangeSensorConfig set using the
///                   AddRangeDevice method.  Values start from the left, and go
///                   to the right (e.g. first value at -90, last at 90 for 180
///                   degree scan).
///   \param[in] timestamp Time when the data was captured (UTC).
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::SetLocalRangeScan(const Byte deviceID, 
                                    const Point3D &deviceLocation, 
                                    const Point3D &deviceOrientation, 
                                    const std::vector<UShort>& scan,
                                    const Time& timestamp)
{
    Mutex::ScopedLock lock(&mRangeSensorMutex);

    std::map<Byte, ReportLocalRangeScan>::iterator s;
    s = mRangeScans.find(deviceID);
    if(s != mRangeScans.end())
    {
        s->second.SetSensorID(deviceID);
        s->second.SetSensorLocation(deviceLocation);
        s->second.SetSensorOrientation(deviceOrientation);
        s->second.SetTimeStamp(timestamp);
        RangeSensorConfig::Map::iterator config;
        config = mRangeSensors.find(deviceID);
        ReportLocalRangeScan::Scan* ptr = s->second.GetScan();
        ptr->clear(false);
        ptr->reserve((unsigned int)scan.size());

        std::vector<UShort>::const_iterator r;
        for(r = scan.begin();
            r != scan.end();
            r++)
        {
            ptr->push_back((UShort)(*r));
        }

        SignalEvent(REPORT_LOCAL_RANGE_SCAN);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set the current scan collected/received from the device.
///
///   \param[in] deviceID Device ID number.
///   \param[in] deviceLocation Location on the paltform relative to
///                             it's origin when the scan was performed (meters).
///                             Values are in the platform coordinate frame (e.g.
///                             positive x is in front of vehicle).
///   \param[in] deviceOrientation Orientation of the device (x = roll, y = pitch,
///                                z = yaw), when the scan was performed.
///   \param[in] scan The scan/range data collected.  The units are in meters.
///                   Values start from the left, and go
///                   to the right (e.g. first value at -90, last at 90 for 180
///                   degree scan).
///   \param[in] timestamp Time when the data was captured (UTC).
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::SetLocalRangeScan(const Byte deviceID, 
                                    const Point3D &deviceLocation, 
                                    const Point3D &deviceOrientation, 
                                    const std::vector<double>& scan,
                                    const Time& timestamp)
{
    Mutex::ScopedLock lock(&mRangeSensorMutex);

    std::map<Byte, ReportLocalRangeScan>::iterator s;
    s = mRangeScans.find(deviceID);
    if(s != mRangeScans.end())
    {
        s->second.SetSensorID(deviceID);
        s->second.SetSensorLocation(deviceLocation);
        s->second.SetSensorOrientation(deviceOrientation);
        s->second.SetTimeStamp(timestamp);
        RangeSensorConfig::Map::iterator config;
        config = mRangeSensors.find(deviceID);
        ReportLocalRangeScan::Scan* ptr = s->second.GetScan();
        ptr->clear(false);
        ptr->reserve((unsigned int)scan.size());

        std::vector<double>::const_iterator r;
        double multiplier = 1000.0;
        if(config->second.mUnitType == RangeSensorConfig::CM)
        {
            multiplier = 100.0;
        }
        for(r = scan.begin();
            r != scan.end();
            r++)
        {
            ptr->push_back((UShort)(multiplier*(*r)));
        }

        SignalEvent(REPORT_LOCAL_RANGE_SCAN);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set the current scan collected/received from the device.
///
///   \param[in] deviceID Device ID number.
///   \param[in] deviceLocation Location on the paltform relative to
///                             it's origin when the scan was performed (meters).
///                             Values are in the platform coordinate frame (e.g.
///                             positive x is in front of vehicle).
///   \param[in] deviceOrientation Orientation of the device (x = roll, y = pitch,
///                                z = yaw), when the scan was performed.
///   \param[in] scan The scan/range data collected.  The units are in meters.
///                   Values start from the left, and go
///                   to the right (e.g. first value at -90, last at 90 for 180
///                   degree scan).  X values in points are the range data.
///   \param[in] timestamp Time when the data was captured (UTC).
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::SetLocalRangeScan(const Byte deviceID, 
                                    const Point3D &deviceLocation, 
                                    const Point3D &deviceOrientation, 
                                    const Point3D::List& scan,
                                    const Time& timestamp)
{
    Mutex::ScopedLock lock(&mRangeSensorMutex);

    std::map<Byte, ReportLocalRangeScan>::iterator s;
    s = mRangeScans.find(deviceID);
    if(s != mRangeScans.end())
    {
        s->second.SetSensorID(deviceID);
        s->second.SetSensorLocation(deviceLocation);
        s->second.SetSensorOrientation(deviceOrientation);
        RangeSensorConfig::Map::iterator config;
        config = mRangeSensors.find(deviceID);
        ReportLocalRangeScan::Scan* ptr = s->second.GetScan();
        ptr->clear(false);
        ptr->reserve((unsigned int)scan.size());

        Point3D::List::const_iterator r;
        double multiplier = 1000.0;
        if(config->second.mUnitType == RangeSensorConfig::CM)
        {
            multiplier = 100.0;
        }
        for(r = scan.begin();
            r != scan.end();
            r++)
        {
            ptr->push_back((UShort)(multiplier*(r->mX)));
        }

        SignalEvent(REPORT_LOCAL_RANGE_SCAN);
    }
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
bool RangeSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_RANGE_SENSOR_CONFIGURATION)
    {
        ReportRangeSensorConfiguration report;
        RangeSensorConfig::Map::const_iterator config;

        {
            Mutex::ScopedLock lock(&mRangeSensorMutex);
            for(config = mRangeSensors.begin();
                config != mRangeSensors.end();
                config++)
            {
                report.GetConfiguration()->push_back(config->second);
            }
        }
        SendEvent(info, &report);
        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_LOCAL_RANGE_SCAN)
    {
        std::map<Byte, ReportLocalRangeScan>::const_iterator scan;
        ReportLocalRangeScan report;
        bool haveScan = false;

        {
            Mutex::ScopedLock lock(&mRangeSensorMutex);
            scan = mRangeScans.find( ((QueryLocalRangeScan *)info.mpQueryMessage)->GetSensorID() );
            if(scan != mRangeScans.end())
            {
                report = scan->second;
                haveScan = true;
            }
        }

        if(haveScan)
        {
            SendEvent(info, &report);
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
bool RangeSensor::IsEventSupported(const Events::Type type,
                                   const double requestedPeriodicRate,
                                   const Message* queryMessage,
                                   double& confirmedPeriodicRate,
                                   std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_RANGE_SENSOR_CONFIGURATION ||
       queryMessage->GetMessageCode() == QUERY_LOCAL_RANGE_SCAN )
    {
        confirmedPeriodicRate = requestedPeriodicRate;
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports PrimitiveDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_LOCAL_RANGE_SCAN:

        break;
    case QUERY_RANGE_SENSOR_CONFIGURATION:
        {
            ReportRangeSensorConfiguration report(message->GetSourceID(), GetComponentID());
            Mutex::ScopedLock lock(&mRangeSensorMutex);
            RangeSensorConfig::Map::iterator config;
            for(config = mRangeSensors.begin();
                config != mRangeSensors.end();
                config++)
            {
                report.GetConfiguration()->push_back(config->second);
            }
            Send(&report);
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
Message* RangeSensor::CreateMessage(const UShort messageCode) const
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
