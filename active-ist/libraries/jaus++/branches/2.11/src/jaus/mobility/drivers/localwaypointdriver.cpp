////////////////////////////////////////////////////////////////////////////////////
///
///  \file localwaypointdriver.cpp
///  \brief This file contains the definition of the LocalWaypointDriver class,
///         used as an interface for the driving of platform to a target waypoint.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2 June 2010
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
#include "jaus/mobility/drivers/localwaypointdriver.h"

using namespace JAUS;

const std::string LocalWaypointDriver::Name = "urn:jaus:jss:mobility:LocalWaypointDriver";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointDriver::LocalWaypointDriver() : Management::Child(Service::ID(LocalWaypointDriver::Name), 
                                                               Service::ID(Management::Name))
{
    mWaypointAchievedFlag = false;
    mpLocalPoseSensor = NULL;
    mpVelocityStateSensor = NULL;
    mLocalWaypointTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointDriver::~LocalWaypointDriver()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current desired destination.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::SetLocalWaypoint(const JAUS::SetLocalWaypoint *command)
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    mLocalWaypoint = *command;
    mLocalWaypoint.SetSourceID(GetComponentID());
    mWaypointAchievedFlag = false;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current desired destination.
///
///   \param[in] speed The desired speed of the Platform.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::SetDesiredTravelSpeed(const double speed)
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    mTravelSpeedTime.SetCurrentTime();
    mDesiredTravelSpeed.SetSourceID(GetComponentID());
    mDesiredTravelSpeed.SetSpeed(speed);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Address of the Driver to control.
///
///   \return False if a problem was found with the change in driver.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::SetDriverToControl(const Address& driver)
{
    //Disallow control of drivers on the same component as this one.
    if(driver == GetComponentID())
    {
        return false;
    }

    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    mControlledDriverID = driver;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Local Waypoint command received.
///
///   \return The current Local Waypoint as a SetLocalWaypoint message.
///
////////////////////////////////////////////////////////////////////////////////////
SetLocalWaypoint LocalWaypointDriver::GetLocalWaypoint() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mLocalWaypoint;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Local Pose reported by the Local Pose sensor.
///
///   \return The current Local Pose as a ReportLocalPose message.
///
////////////////////////////////////////////////////////////////////////////////////
LocalPose LocalWaypointDriver::GetLocalPose() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mLocalPose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Velocity State reported by the Velocity State
///          sensor.
///
///   \return The current Velocity State as a ReportVelocityState message.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityState LocalWaypointDriver::GetVelocityState() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mVelocityState;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the desired Travel Speed command received.
///
///   \return The desired Travel Speed as a SetTravelSpeed message.
///
////////////////////////////////////////////////////////////////////////////////////
SetTravelSpeed LocalWaypointDriver::GetDesiredTravelSpeed() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mDesiredTravelSpeed;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a Set Local Waypoint command was
///           received.
///
////////////////////////////////////////////////////////////////////////////////////
Time LocalWaypointDriver::GetLocalWaypointTime() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mLocalWaypointTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a Set Travel Speed command was
///           received.
///
////////////////////////////////////////////////////////////////////////////////////
Time LocalWaypointDriver::GetTravelSpeedTime() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mTravelSpeedTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The address of the Driver being controlled.
///
////////////////////////////////////////////////////////////////////////////////////
Address LocalWaypointDriver::GetControlledDriverID() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    return mControlledDriverID;
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
bool LocalWaypointDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_LOCAL_WAYPOINT)
    {
        Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
        const QueryLocalWaypoint* query = dynamic_cast<const QueryLocalWaypoint*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportLocalWaypoint report;
        CreateReportFromQuery(query, report);
        SendEvent(info, &report);

        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_TRAVEL_SPEED)
    {
        Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
        const QueryTravelSpeed* query = dynamic_cast<const QueryTravelSpeed*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportTravelSpeed report;
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
bool LocalWaypointDriver::IsEventSupported(const Events::Type type,
                                            const double requestedPeriodicRate,
                                            const Message* queryMessage,
                                            double& confirmedPeriodicRate,
                                            std::string& errorMessage) const
{
    // Support any type of event for QUERY_LOCAL_WAYPOINT data.
    if(queryMessage->GetMessageCode() == QUERY_LOCAL_WAYPOINT)
    {
        confirmedPeriodicRate = requestedPeriodicRate;
        return true;
    }
    // Support any type of event for QUERY_TRAVEL_SPEED data.
    else if(queryMessage->GetMessageCode() == QUERY_TRAVEL_SPEED)
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
///   This Service supports LocalWaypointDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointDriver::Receive(const Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_LOCAL_WAYPOINT:
        {
            const JAUS::QueryLocalWaypoint* query = dynamic_cast<const JAUS::QueryLocalWaypoint*>(message);
            if(query)
            {
                ReportLocalWaypoint report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case QUERY_TRAVEL_SPEED:
        {
            const JAUS::QueryTravelSpeed* query = dynamic_cast<const JAUS::QueryTravelSpeed*>(message);
            if(query)
            {
                ReportTravelSpeed report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case SET_LOCAL_WAYPOINT:
        {
            const JAUS::SetLocalWaypoint* command = dynamic_cast<const JAUS::SetLocalWaypoint*>(message);
            if(command)
            {
                Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
                if(SetLocalWaypoint(command))
                {
                    mLocalWaypoint = *command;
                    mLocalWaypointTime.SetCurrentTime();
                    SignalEvent(REPORT_LOCAL_WAYPOINT);
                }
            }
        }
        break;
    case SET_TRAVEL_SPEED:
        {
            const JAUS::SetTravelSpeed* command = dynamic_cast<const JAUS::SetTravelSpeed*>(message);
            if(command)
            {
                Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
                if(SetDesiredTravelSpeed(command))
                {
                    mDesiredTravelSpeed = *command;
                    mTravelSpeedTime.SetCurrentTime();
                    SignalEvent(REPORT_TRAVEL_SPEED);
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
Message* LocalWaypointDriver::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_LOCAL_WAYPOINT:
        message = new JAUS::QueryLocalWaypoint();
        break;
    case QUERY_TRAVEL_SPEED:
        message = new JAUS::QueryTravelSpeed();
        break;
    case REPORT_LOCAL_WAYPOINT:
        message = new JAUS::ReportLocalWaypoint();
        break;
    case REPORT_TRAVEL_SPEED:
        message = new JAUS::ReportTravelSpeed();
        break;
    case SET_LOCAL_WAYPOINT:
        message = new JAUS::SetLocalWaypoint();
        break;
    case SET_TRAVEL_SPEED:
        message = new JAUS::SetTravelSpeed();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Local Waypoint Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointDriver::PrintStatus() const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    if(mLocalWaypoint.GetSourceID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Driving to Waypoint:\n";
        mLocalWaypoint.PrintMessageBody();
        if(mDesiredTravelSpeed.GetSourceID().IsValid())
        {
            mDesiredTravelSpeed.PrintMessageBody();
        }
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Idle\n";
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when transitioning to a resume state.
///          Confirms subscription to Sensors, and control of Driver.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::Resume()
{
    return (CheckDriver() &&  CheckSensors());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief // Method called to transition due to reset.
///          Release Control of Driver, clear all data.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::Reset()
{
    bool success = ReleaseResources();

    mWaypointAchievedFlag = false;
    mControlledDriverID.Clear();
    mpLocalPoseSensor = NULL;
    mpVelocityStateSensor = NULL;
    mLocalWaypointTime.Clear();
    mLocalWaypoint.ClearMessage();
    mDesiredTravelSpeed.ClearMessage();

    return success;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when transitioning to a standby state.
///          Release Control of the Driver and remove subscriptions if any.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::Standby()
{
    return ReleaseResources();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when transitioning to an emergency state.  Try to send
///          a Idle command to the controlled Driver.  Afterwards, Release control
///          of the Driver and remove subscriptions if any.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::SetEmergency()
{
    bool success = true;
    Message* command = NULL;
    command = GenerateIdleDriveCommand(GetStatus());
    if(command)
    {
        success &= Send(command);
    }
    success &= ReleaseResources();

    return success;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when leaving the emergency state, try to resume the
///          the Waypoint if any.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::ClearEmergency()
{
    return Resume();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when control is released.  Does not stop operatoin of
///          driver.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::ReleaseControl()
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Release Control of Driver and remove subscriptions if any, delete old
///          sensor data.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::ReleaseResources()
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);

    if(GetComponent()->AccessControlService()->HaveControl(mControlledDriverID))
    {
        GetComponent()->AccessControlService()->ReleaseComponentControl(mControlledDriverID);
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Procedure to generate Drive commands until the waypoint is acheived.
///
///   This procedure first checks if Driver control and Local Pose subscription
///   data is available.  If unavailabe, an attempt is made to acquire the Driver
///   control and/or Local Pose subscription.  If prerequisites are met, a
///   Drive command is created and sent to the controlled Driver until the waypoint
///   is achieved.  This procedure sends an idling drive command if waypoint is
///   already achieved or cannot be achieved due to lack of sensor data..
///   
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointDriver::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    Byte status = GetComponent()->ManagementService()->GetStatus();
    switch(status)
    {
    case Management::Status::Ready:
        {
            Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
            //If all in order, generate and send a drive command, delete after use.
            if(CheckDriver())
            {
                Message *command = NULL;
                //Check whether sensor Data is valid.
                if(CheckSensors() &&
                   mLocalWaypoint.GetSourceID().IsValid())
                {
                    //Send Idle command if Waypoint is achieved.
                    if(IsWaypointAchieved(mLocalPose, mLocalWaypoint))
                    {
                        if(!mWaypointAchievedFlag)
                        {
                            mWaypointAchievedFlag = true;
                            WaypointAchieved(mLocalWaypoint);
                            // Flush out the ID so we don't keep driving to
                            // the waypoint.
                            mLocalWaypoint.ClearMessage();
                        }

                        command = GenerateIdleDriveCommand(status);
                    }
                    else
                    {
                        command = GenerateDriveCommand(status);
                    }
                }
                else
                {
                    command = GenerateIdleDriveCommand(status);
                }

                if(command)
                {
                    command->SetDestinationID(mControlledDriverID);
                    command->SetSourceID(this->GetComponentID());
                    Send(command);
                    delete command;
                }
            }
        }
        break;
    case Management::Status::Standby:
        // Switching to standby mode automatically releases control and halts subscription(s).
        break;
    default:
        break;
    };

    WaypointDriverUpdateEvent(timeSinceLastCheckMs);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Local Waypoint.
///   \param[out] report The configured report based on Local Waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointDriver::CreateReportFromQuery(const QueryLocalWaypoint* query,
                                                 ReportLocalWaypoint& report) const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mLocalWaypoint.GetPresenceVector();

    report.SetX(mLocalWaypoint.GetX());
    report.SetY(mLocalWaypoint.GetY());
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Z)) > 0) { report.SetZ(mLocalWaypoint.GetZ()); }
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(mLocalWaypoint.GetRoll()); }
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(mLocalWaypoint.GetPitch()); }
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(mLocalWaypoint.GetYaw()); }
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(mLocalWaypoint.GetWaypointTolerance()); }
    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(mLocalWaypoint.GetPathTolerance()); }    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Travel Speed.
///   \param[out] report The configured report based on Travel Speed.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointDriver::CreateReportFromQuery(const QueryTravelSpeed* query,
                                                 ReportTravelSpeed& report) const
{
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    report.SetSpeed(mDesiredTravelSpeed.GetSpeed());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if Driver is controlled, acquire control if necessary.
///
///   \return True if Driver is controlled by the end of the method.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::CheckDriver()
{
    bool success = false;
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);

    //Check if control of the driver is available, else try to acquire it
    if(GetComponent()->AccessControlService()->HaveControl(mControlledDriverID))
    {
        success = true;
    }
    else
    {
        if(GetComponent()->AccessControlService()->RequestComponentControl(mControlledDriverID, false))
        {
            success = GetComponent()->ManagementService()->Resume(mControlledDriverID);
        }
    }

    return success;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if Local Pose, Velocity State (optional), and Acceleration
///          State (optional) sensors are available.  Make subscriptions if
///          necessary.  Updates pose and state if data is locally accessible.
///
///   \return True if Local Pose Sensor data is available.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointDriver::CheckSensors()
{
    bool success = true;
    Mutex::ScopedLock lock(&mLocalWaypointDriverMutex);

    if(mpLocalPoseSensor == NULL)
    {
        mpLocalPoseSensor = dynamic_cast<LocalPoseSensor*>(GetComponent()->GetService(LocalPoseSensor::Name));
    }

    if(mpLocalPoseSensor)
    {
        mLocalPose = mpLocalPoseSensor->GetLocalPose();
        if(mLocalPose.AreFieldsPresent(ReportLocalPose::PresenceVector::X | 
                                       ReportLocalPose::PresenceVector::Y ) == false)
        {
            success = false;
        }
    }
    else
    {
        success = false;
    }

    if(mpVelocityStateSensor == NULL)
    {
        mpVelocityStateSensor = dynamic_cast<VelocityStateSensor*>(GetComponent()->GetService(VelocityStateSensor::Name));
    }

    if(mpVelocityStateSensor)
    {
        mVelocityState = mpVelocityStateSensor->GetVelocityState();
    }
    else
    {
        success = false;
    }

    return success;
}


/*  End of File */
