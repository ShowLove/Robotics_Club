////////////////////////////////////////////////////////////////////////////////////
///
///  \file localposesensor.cpp
///  \brief This file contains the definition of the LocalPoseSensor class,
///         used as an interface for reporting Local Pose information.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 4 February 2010
///  <br>Copyright (c) 2010
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bsun@ist.ucf.edu
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
#include "jaus/mobility/sensors/localposesensor.h"
#include "jaus/core/events/createevent.h"
#include "jaus/core/component.h"

#include <cxutils/math/cxmath.h>

using namespace JAUS;

const std::string LocalPoseSensor::Name = "urn:jaus:jss:mobility:LocalPoseSensor";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  
///
////////////////////////////////////////////////////////////////////////////////////
LocalPoseSensor::LocalPoseSensor(const double updateRate)
                                        : AccessControl::Child(Service::ID(LocalPoseSensor::Name), 
                                                               Service::ID(Events::Name))
{
    mMaxUpdateRate = 10.0;
    SetSensorUpdateRate(updateRate);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalPoseSensor::~LocalPoseSensor()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the local pose for the component/platform.
///
///   \param[in] localPose The current local pose of the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalPoseSensor::SetLocalPose(const JAUS::SetLocalPose& localPose)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    mLocalPose.ClearMessage();
    UInt pv = localPose.GetPresenceVector();
    if( (pv & LocalPose::PresenceVector::X) > 0) { mLocalPose.SetX(localPose.GetX()); }
    if( (pv & LocalPose::PresenceVector::Y) > 0) { mLocalPose.SetY(localPose.GetY()); }
    if( (pv & LocalPose::PresenceVector::Z) > 0) { mLocalPose.SetZ(localPose.GetZ()); }
    if( (pv & LocalPose::PresenceVector::PositionRMS) > 0) { mLocalPose.SetPositionRMS(localPose.GetPositionRMS()); }
    if( (pv & LocalPose::PresenceVector::Roll) > 0) { mLocalPose.SetRoll(localPose.GetRoll()); }
    if( (pv & LocalPose::PresenceVector::Pitch) > 0) { mLocalPose.SetPitch(localPose.GetPitch()); }
    if( (pv & LocalPose::PresenceVector::Yaw) > 0) { mLocalPose.SetYaw(localPose.GetYaw()); }
    if( (pv & LocalPose::PresenceVector::AttitudeRMS) > 0) { mLocalPose.SetAttitudeRMS(localPose.GetAttitudeRMS()); }
    if( (pv & LocalPose::PresenceVector::TimeStamp) > 0) { mLocalPose.SetTimeStamp(localPose.GetTimeStamp()); }

    SignalEvent(REPORT_LOCAL_POSE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the local pose for the component/platform.
///
///   \param[in] localPose The current local pose of the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalPoseSensor::SetLocalPose(const JAUS::LocalPose& localPose)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    mLocalPose = localPose;
    SignalEvent(REPORT_LOCAL_POSE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position, orientation, and time of the pose for the
///          component/platform.  This function may fail if any parameters are out of
///          bounds.
///
///   \param[in] position Desired X, Y, Z stored as a Point3D.
///   \param[in] orientation Desired Roll(X), Pitch(Y), Yaw(Z) stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::SetLocalPose(const Point3D& position,
                                   const Point3D& orientation,
                                   const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.SetPose(position, orientation, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method for setting local pose data using global pose data.
///
///   <b>This method only works if a global reference point has been 
///   (SetLocalPoseReference) or there is a GlobalPoseSensor service on your
///   subsystem.  Without this data, pose cannot be calculated. </b>
///
///   \param[in] globalPose Global pose to use for local pose calculation.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::SetLocalPose(const JAUS::GlobalPose& globalPose)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);

    UInt required = ReportGlobalPose::PresenceVector::Latitude |
                    ReportGlobalPose::PresenceVector::Longitude |
                    ReportGlobalPose::PresenceVector::Altitude |
                    ReportGlobalPose::PresenceVector::Roll |
                    ReportGlobalPose::PresenceVector::Pitch |
                    ReportGlobalPose::PresenceVector::Yaw;
    if(mGlobalPoseReference.AreFieldsPresent(required) == false)
    {
        // Try lookup global pose data if it is available.
        GlobalPoseSensor* globalPoseSensor = (GlobalPoseSensor*)GetComponent()->GetService(GlobalPoseSensor::Name);
        if(globalPoseSensor)
        {
            mGlobalPoseReference = globalPoseSensor->GetGlobalPose();
        }
        else
        {
            Address::List id = GetComponent()->DiscoveryService()->GetComponentsWithService(GlobalPoseSensor::Name);
            for(unsigned int i = 0; i < (unsigned int)id.size(); i++)
            {
                if(id[i].mSubsystem == GetComponentID().mSubsystem)
                {
                    QueryGlobalPose query(id.front(), GetComponentID());
                    query.SetPresenceVector(query.GetPresenceVectorMask());
                    ReportGlobalPose report;
                    if(Send(&query, &report))
                    {
                        mGlobalPoseReference = report;
                        break;
                    }
                }
            }
        }
    }
    // If we have the data we need, calculate local pose.
    if(mGlobalPoseReference.AreFieldsPresent(required) && globalPose.AreFieldsPresent(required))
    {
        CxUtils::Utm origin(CxUtils::Wgs(mGlobalPoseReference.GetLatitude(),
                                         mGlobalPoseReference.GetLongitude(),
                                         mGlobalPoseReference.GetAltitude()));
        CxUtils::Utm pos(CxUtils::Wgs(globalPose.GetLatitude(),
                                      globalPose.GetLongitude(),
                                      globalPose.GetAltitude()));
        CxUtils::Point3D localPos(pos.mNorthing - origin.mNorthing,
                                  pos.mEasting - origin.mEasting,
                                  pos.mElevation - origin.mElevation);
        double angleDiff = CxUtils::Orientation::AngleDiff(mGlobalPoseReference.GetYaw(), globalPose.GetYaw());
        localPos = localPos.Rotate(-mGlobalPoseReference.GetYaw(), Point3D::Z);
        CxUtils::Point3D localOr(CxUtils::Orientation::AngleDiff(mGlobalPoseReference.GetRoll(), globalPose.GetRoll()),
                                 CxUtils::Orientation::AngleDiff(mGlobalPoseReference.GetPitch(), globalPose.GetPitch()),
                                 CxUtils::Orientation::AngleDiff(mGlobalPoseReference.GetYaw(), globalPose.GetYaw()));
        mLocalPose.SetPosition(localPos);
        mLocalPose.SetOrientation(localOr);
        mLocalPose.SetAttitudeRMS(globalPose.GetAttitudeRMS());
        mLocalPose.SetPositionRMS(globalPose.GetPositionRMS());
        if(globalPose.IsFieldPresent(ReportGlobalPose::PresenceVector::TimeStamp))
        {
            mLocalPose.SetTimeStamp(globalPose.GetTimeStamp());
        }
        else
        {
            mLocalPose.SetTimeStamp(Time(true));
        }
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the global coordinate reference point (optional
///          value that can be set) that can be used for
///          calculating local pose based on global pose/orientation
///          changes.
///
///   This is optional data, and is not required for use of Local Pose Sensor
///   service.
///
///   \param[in] globalPose The current global pose value you want to use
///              for caculating changes in position/orientation for
///              measurement of local pose.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalPoseSensor::SetLocalPoseReference(const GlobalPose& globalPose)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    mGlobalPoseReference = globalPose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the position and time of the pose for the component/platform.
///          This function may fail if any parameters are out of bounds.
///
///   \param[in] position Desired X, Y, Z stored as a Point3D.
///   \param[in] time Desired Time Stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::SetLocalPosition(const Point3D& position, const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.SetPosition(position, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
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
bool LocalPoseSensor::SetLocalOrientation(const Point3D& orientation, const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.SetOrientation(orientation, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds to the position and orientation of the pose for the
///          component/platform and updates the time stamp.  This function may
///          fail if any parameters are out of bounds.
///
///   \param[in] position Desired delta X, Y, and Z stored as a Point3D.
///   \param[in] orientation Desired delta Roll(X), Pitch(Y), Yaw(Z) stored as a
///              Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::AddToLocalPose(const Point3D& position,
                                     const Point3D& orientation,
                                     const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.AddToPose(position, orientation, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds to the position of the pose for the component/platform and
///          updates the time stamp.  This function may fail if any parameters are
///          out of bounds.
///
///   \param[in] position Desired change in X, Y, and Z stored as a Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::AddToLocalPosition(const Point3D& position, const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.AddToPosition(position, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds to the orientation of the pose for the component/platform and
///          updates the time stamp. This function may fail if any parameters are
///          out of bounds.
///
///   \param[in] orientation Desired change in Roll(X), Pitch(Y), Yaw(Z) stored as a
///              Point3D.
///   \param[in] time Desired time stamp as a JAUS::Time instance, defaulting to
///              the current time in UTC.
///
///   \return true if successful, false otherwise
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::AddtoLocalOrientation(const Point3D& orientation, const Time& time)
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(mLocalPose.AddToOrientation(orientation, time))
    {
        SignalEvent(REPORT_LOCAL_POSE);
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Maximum update rate of the sensor.
///
///   \return true successful, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalPoseSensor::SetSensorUpdateRate(const double rate)
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
///   \brief Gets the local pose of the sensor.
///
///   \return The most recently updated Local Pose.
///
////////////////////////////////////////////////////////////////////////////////////
LocalPose LocalPoseSensor::GetLocalPose() const
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    return mLocalPose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the global coordinate reference point (optional
///          value that can be set) that can be used for
///          calculating local pose based on global pose/orientation
///          changes.
///
///   \return Gets the global pose value last set or queried from
///           a Global Pose Sensor when a Set Local Pose command
///           was received.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPose LocalPoseSensor::GetLocalPoseReference() const
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    return mGlobalPoseReference;
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
bool LocalPoseSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_LOCAL_POSE)
    {
        const QueryLocalPose* query = dynamic_cast<const QueryLocalPose*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        LocalPose report;
        CreateReportFromQuery(query, report);
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
bool LocalPoseSensor::IsEventSupported(const Events::Type type,
                                        const double requestedPeriodicRate,
                                        const Message* queryMessage,
                                        double& confirmedPeriodicRate,
                                        std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_LOCAL_POSE)
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
///   This Service supports PrimitiveDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalPoseSensor::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_LOCAL_POSE:
        {
            const QueryLocalPose* query = dynamic_cast<const QueryLocalPose*>(message);
            if(query)
            {
                ReportLocalPose report(query->GetSourceID(), GetComponentID());
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case SET_LOCAL_POSE:
        {
            const JAUS::SetLocalPose* command = dynamic_cast<const JAUS::SetLocalPose*>(message);
            if(command)
            {
                SetLocalPose(*command);
                // Try lookup global pose data if it is available.
                GlobalPoseSensor* globalPoseSensor = (GlobalPoseSensor*)GetComponent()->GetService(GlobalPoseSensor::Name);
                if(globalPoseSensor)
                {
                    mGlobalPoseReference = globalPoseSensor->GetGlobalPose();
                }
                else
                {
                    Address::List id = GetComponent()->DiscoveryService()->GetComponentsWithService(GlobalPoseSensor::Name);
                    for(unsigned int i = 0; i < (unsigned int)id.size(); i++)
                    {
                        if(id[i].mSubsystem == GetComponentID().mSubsystem)
                        {
                            QueryGlobalPose query(id.front(), GetComponentID());
                            query.SetPresenceVector(query.GetPresenceVectorMask());
                            ReportGlobalPose report;
                            if(Send(&query, &report))
                            {
                                mGlobalPoseReference = report;
                                break;
                            }
                        }
                    }
                }
            }
        }
        break;
    case REPORT_LOCAL_POSE:
        {
            const JAUS::ReportLocalPose* report = dynamic_cast<const JAUS::ReportLocalPose*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                SetLocalPose(*report);
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
Message* LocalPoseSensor::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_LOCAL_POSE:
        message = new QueryLocalPose();
        break;
    case REPORT_LOCAL_POSE:
        message = new ReportLocalPose();
        break;
    case SET_LOCAL_POSE:
        message = new JAUS::SetLocalPose();
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
void LocalPoseSensor::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mLocalPoseMutex);
    if(GetSynchronizeID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Synchronized to [" << GetSynchronizeID().ToString() << "]:\n";
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Local Pose:\n";
    }
    LocalPose pose;
    mLocalPoseMutex.Lock();
    pose = mLocalPose;
    mLocalPoseMutex.Unlock();
    pose.PrintMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Local Pose.
///   \param[out] report The configured report based on Local Pose.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalPoseSensor::CreateReportFromQuery(const QueryLocalPose* query, LocalPose& report) const
{
    Mutex::ScopedLock lock(&mLocalPoseMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mLocalPose.GetPresenceVector();

    if( (pv2 & (pv1 & LocalPose::PresenceVector::X)) > 0) { report.SetX(mLocalPose.GetX()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::Y)) > 0) { report.SetY(mLocalPose.GetY()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::Z)) > 0) { report.SetZ(mLocalPose.GetZ()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::PositionRMS)) > 0) { report.SetPositionRMS(mLocalPose.GetPositionRMS()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::Roll)) > 0) { report.SetRoll(mLocalPose.GetRoll()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::Pitch)) > 0) { report.SetPitch(mLocalPose.GetPitch()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::Yaw)) > 0) { report.SetYaw(mLocalPose.GetYaw()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::AttitudeRMS)) > 0) { report.SetAttitudeRMS(mLocalPose.GetAttitudeRMS()); }
    if( (pv2 & (pv1 & LocalPose::PresenceVector::TimeStamp)) > 0) { report.SetTimeStamp(mLocalPose.GetTimeStamp()); }
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
void LocalPoseSensor::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription, and if not create it.
        if(EventsService()->HaveSubscription(REPORT_LOCAL_POSE, syncID) == false)
        {
            QueryLocalPose query(syncID, GetComponentID());
            query.SetPresenceVector(query.GetPresenceVectorMask());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}


/*  End of File */
