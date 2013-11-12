////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalposesensor.cpp
///  \brief This file contains the definition of the GlobalPoseSensor class,
///         used as an interface for reporting Global Pose information.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 February 2010
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
#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/core/events/createevent.h"

using namespace JAUS;

const std::string GlobalPoseSensor::Name = "urn:jaus:jss:mobility:GlobalPoseSensor";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPoseSensor::GlobalPoseSensor(const double updateRate) : AccessControl::Child(Service::ID(GlobalPoseSensor::Name), Service::ID(AccessControl::Name))
{
    mMaxUpdateRate = 10.0;
    SetSensorUpdateRate(updateRate);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPoseSensor::~GlobalPoseSensor()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global pose for the component/platform.
///
///   \param[in] globalPose The current global pose of the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::SetGlobalPose(const JAUS::SetGlobalPose& globalPose)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    mGlobalPose.ClearMessage();
    UInt pv = globalPose.GetPresenceVector();
    if( (pv & GlobalPose::PresenceVector::Latitude) > 0) { mGlobalPose.SetLatitude(globalPose.GetLatitude()); }
    if( (pv & GlobalPose::PresenceVector::Longitude) > 0) { mGlobalPose.SetLongitude(globalPose.GetLongitude()); }
    if( (pv & GlobalPose::PresenceVector::Altitude) > 0) { mGlobalPose.SetAltitude(globalPose.GetAltitude()); }
    if( (pv & GlobalPose::PresenceVector::PositionRMS) > 0) { mGlobalPose.SetPositionRMS(globalPose.GetPositionRMS()); }
    if( (pv & GlobalPose::PresenceVector::Roll) > 0) { mGlobalPose.SetRoll(globalPose.GetRoll()); }
    if( (pv & GlobalPose::PresenceVector::Pitch) > 0) { mGlobalPose.SetPitch(globalPose.GetPitch()); }
    if( (pv & GlobalPose::PresenceVector::Yaw) > 0) { mGlobalPose.SetYaw(globalPose.GetYaw()); }
    if( (pv & GlobalPose::PresenceVector::AttitudeRMS) > 0) { mGlobalPose.SetAttitudeRMS(globalPose.GetAttitudeRMS()); }
    if( (pv & GlobalPose::PresenceVector::TimeStamp) > 0) { mGlobalPose.SetTimeStamp(globalPose.GetTimeStamp()); }

    SignalEvent(REPORT_GLOBAL_POSE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global pose for the component/platform.
///
///   \param[in] globalPose The current global pose of the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::SetGlobalPose(const JAUS::GlobalPose& globalPose)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    mGlobalPose = globalPose;
    SignalEvent(REPORT_GLOBAL_POSE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position, orientation, and time of the pose for the
///          component/platform.  This function may fail if any parameters are out
///          of bounds.
///
///   \param[in] position Desired Latitude, Longitude, Altitude.
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalPoseSensor::SetGlobalPose(const Wgs& position,
                                     const Point3D& orientation,
                                     const Time& time)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    if(mGlobalPose.SetPose(position, orientation, time))
    {
        SignalEvent(REPORT_GLOBAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position and time of the pose for the component/platform.
///          This function may fail if any parameters are out of bounds.
///
///   \param[in] position Desired Latitude, Longitude, Altitude.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalPoseSensor::SetGlobalPosition(const Wgs& position, const Time& time)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    if(mGlobalPose.SetPosition(position, time))
    {
        SignalEvent(REPORT_GLOBAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the orientation and time of the pose for the component/platform.
///          This function may fail if any parameters are out of bounds.
///
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalPoseSensor::SetGlobalOrientation(const Point3D& orientation, const Time& time)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    if(mGlobalPose.SetOrientation(orientation, time))
    {
        SignalEvent(REPORT_GLOBAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global pose for the component/platform.
///
///   \param[in] magneticVariation Geomagnetic variation for the pose/compass
///                                [-PI,PI].
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::SetGeomagneticVariation(const double magneticVariation)
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    if(magneticVariation >= SetGeomagneticProperty::Limits::MinMagneticVariation &&
       magneticVariation <= SetGeomagneticProperty::Limits::MaxMagneticVariation)
    {
        mGeomagneticProperty = magneticVariation;
        SignalEvent(REPORT_GEOMAGNETIC_PROPERTY);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Maximum update rate of the sensor.
///
///   \return true successful, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalPoseSensor::SetSensorUpdateRate(const double rate)
{
    if(rate >= CreateEvent::Limits::MinUpdateRate &&
       rate <= CreateEvent::Limits::MaxUpdateRate)
    {
        mMaxUpdateRate = rate;
        return true;
    }
    return false;      
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the global pose of the sensor.
///
///   \return The most recently updated Global Pose.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPose GlobalPoseSensor::GetGlobalPose() const
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    return mGlobalPose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the geomagnetic property for adjusting global pose.
///
///   \return The geomagnetic property to use [-PI, PI].
///
////////////////////////////////////////////////////////////////////////////////////
double GlobalPoseSensor::GetGeomagneticVariation() const
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    return mGeomagneticProperty;
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
bool GlobalPoseSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_GLOBAL_POSE)
    {
        const QueryGlobalPose* query = dynamic_cast<const QueryGlobalPose*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        GlobalPose report;
        CreateReportFromQuery(query, report);
        SendEvent(info, &report);

        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_GEOMAGNETIC_PROPERTY)
    {
        ReportGeomagneticProperty report;
        report.SetMagneticVariation(mGeomagneticProperty);
        SendEvent(info, &report);

        return true;
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
bool GlobalPoseSensor::IsEventSupported(const Events::Type type,
                                        const double requestedPeriodicRate,
                                        const Message* queryMessage,
                                        double& confirmedPeriodicRate,
                                        std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_GLOBAL_POSE)
    {
        if(requestedPeriodicRate <= mMaxUpdateRate)
        {
            confirmedPeriodicRate = requestedPeriodicRate;
        }
        else
        {
            confirmedPeriodicRate = mMaxUpdateRate;
        }
        return true;
    }
    else if(queryMessage->GetMessageCode() == QUERY_GEOMAGNETIC_PROPERTY)
    {
        if(requestedPeriodicRate <= mMaxUpdateRate)
        {
            confirmedPeriodicRate = requestedPeriodicRate;
        }
        else
        {
            confirmedPeriodicRate = mMaxUpdateRate;
        }
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
///   This Service supports GlobalPoseSensor related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_GLOBAL_POSE:
        {
            const QueryGlobalPose* query = dynamic_cast<const QueryGlobalPose*>(message);
            if(query)
            {
                ReportGlobalPose report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case REPORT_GLOBAL_POSE:
        {
            const JAUS::ReportGlobalPose* report = dynamic_cast<const JAUS::ReportGlobalPose*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                SetGlobalPose(*report);
            }
        }
        break;
    case SET_GLOBAL_POSE:
        {
            const JAUS::SetGlobalPose* command = dynamic_cast<const JAUS::SetGlobalPose*>(message);
            if(command)
            {
                SetGlobalPose(*command);
            }
        }
        break;
    case QUERY_GEOMAGNETIC_PROPERTY:
        {

            ReportGeomagneticProperty report(message->GetSourceID(), GetComponentID());
            report.SetMagneticVariation(GetGeomagneticVariation());
            Send(&report);
        }
        break;
    case SET_GEOMAGNETIC_PROPERTY:
        {
            const SetGeomagneticProperty* command = dynamic_cast<const SetGeomagneticProperty*>(message);
            if(command)
            {
                SetGeomagneticVariation(command->GetMagneticVariation());
            }
        }
        break;
    case REPORT_GEOMAGNETIC_PROPERTY:
        {
            const JAUS::ReportGeomagneticProperty* report = dynamic_cast<const JAUS::ReportGeomagneticProperty*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                SetGeomagneticVariation(report->GetMagneticVariation());
            }
        }
        break;
    default:
        break;
    };
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
Message* GlobalPoseSensor::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_GLOBAL_POSE:
        message = new QueryGlobalPose();
        break;
    case QUERY_GEOMAGNETIC_PROPERTY:
        message = new QueryGeomagneticProperty();
        break;
    case REPORT_GLOBAL_POSE:
        message = new ReportGlobalPose();
        break;
    case SET_GLOBAL_POSE:
        message = new JAUS::SetGlobalPose();
        break;
    case REPORT_GEOMAGNETIC_PROPERTY:
        message = new ReportGeomagneticProperty();
        break;
    case SET_GEOMAGNETIC_PROPERTY:
        message = new SetGeomagneticProperty();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mGlobalPoseMutex);
    if(GetSynchronizeID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Synchronized to [" << GetSynchronizeID().ToString() << "]:\n";
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Global Pose:\n";
    }
    GlobalPose pose;
    mGlobalPoseMutex.Lock();
    pose = mGlobalPose;
    mGlobalPoseMutex.Unlock();
    pose.PrintMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Global Pose.
///   \param[out] report The configured report based on Global Pose.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::CreateReportFromQuery(const QueryGlobalPose* query, GlobalPose& report) const
{
    Mutex::ScopedLock lock(&mGlobalPoseMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mGlobalPose.GetPresenceVector();

    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Latitude)) > 0) { report.SetLatitude(mGlobalPose.GetLatitude()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Longitude)) > 0) { report.SetLongitude(mGlobalPose.GetLongitude()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Altitude)) > 0) { report.SetAltitude(mGlobalPose.GetAltitude()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::PositionRMS)) > 0) { report.SetPositionRMS(mGlobalPose.GetPositionRMS()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Roll)) > 0) { report.SetRoll(mGlobalPose.GetRoll()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Pitch)) > 0) { report.SetPitch(mGlobalPose.GetPitch()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::Yaw)) > 0) { report.SetYaw(mGlobalPose.GetYaw()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::AttitudeRMS)) > 0) { report.SetAttitudeRMS(mGlobalPose.GetAttitudeRMS()); }
    if( (pv2 & (pv1 & GlobalPose::PresenceVector::TimeStamp)) > 0) { report.SetTimeStamp(mGlobalPose.GetTimeStamp()); }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief If synchronization is enabled, then this method is used to
///          subscribe to sensor data from the component we are synchronizing
///          with.
///
///   \param[in] timeSinceLastCheckMs Time since last update in ms.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalPoseSensor::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription, and if not create it.
        if(EventsService()->HaveSubscription(REPORT_GLOBAL_POSE, syncID) == false)
        {
            QueryGlobalPose query(syncID, GetComponentID());
            query.SetPresenceVector(query.GetPresenceVectorMask());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
        if(EventsService()->HaveSubscription(REPORT_GEOMAGNETIC_PROPERTY, syncID) == false)
        {
            QueryGeomagneticProperty query(syncID, GetComponentID());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}


/*  End of File */
