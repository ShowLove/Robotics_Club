////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalwaypointdriver.h
///  \brief This file contains the implementation of the GlobalWaypointDriver
///         class, used as an interface for the driving of platform to a target
///         waypoint.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 10 February 2010
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
#include "jaus/core/component.h"
#include "jaus/mobility/drivers/globalwaypointdriver.h"

using namespace JAUS;

const std::string GlobalWaypointDriver::Name = "urn:jaus:jss:mobility:GlobalWaypointDriver";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypointDriver::GlobalWaypointDriver() : Management::Child(Service::ID(GlobalWaypointDriver::Name), 
                                                                 Service::ID(Management::Name))
{
    mWaypointAchievedFlag = false;
    mpGlobalPoseSensor = NULL;
    mpVelocityStateSensor = NULL;
    mGlobalWaypointTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypointDriver::~GlobalWaypointDriver()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the current desired destination.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::SetGlobalWaypoint(const JAUS::SetGlobalWaypoint *command)
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    mGlobalWaypoint = *command;
    mGlobalWaypoint.SetSourceID(GetComponentID());
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
bool GlobalWaypointDriver::SetDesiredTravelSpeed(const double speed)
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
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
bool GlobalWaypointDriver::SetDriverToControl(const Address& driver)
{
    //Disallow control of drivers on the same component as this one.
    if(driver == GetComponentID())
    {
        return false;
    }

    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    mControlledDriverID = driver;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Global Waypoint command received.
///
///   \return The current Global Waypoint as a SetGlobalWaypoint message.
///
////////////////////////////////////////////////////////////////////////////////////
SetGlobalWaypoint GlobalWaypointDriver::GetGlobalWaypoint() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mGlobalWaypoint;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Global Pose reported by the Global Pose sensor.
///
///   \return The current Global Pose as a ReportGlobalPose message.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalPose GlobalWaypointDriver::GetGlobalPose() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mGlobalPose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Velocity State reported by the Velocity State
///          sensor.
///
///   \return The current Velocity State as a ReportVelocityState message.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityState GlobalWaypointDriver::GetVelocityState() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mVelocityState;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the desired Travel Speed command received.
///
///   \return The desired Travel Speed as a SetTravelSpeed message.
///
////////////////////////////////////////////////////////////////////////////////////
SetTravelSpeed GlobalWaypointDriver::GetDesiredTravelSpeed() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mDesiredTravelSpeed;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a Set Global Waypoint command was
///           received.
///
////////////////////////////////////////////////////////////////////////////////////
Time GlobalWaypointDriver::GetGlobalWaypointTime() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mGlobalWaypointTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a Set Travel Speed command was
///           received.
///
////////////////////////////////////////////////////////////////////////////////////
Time GlobalWaypointDriver::GetTravelSpeedTime() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    return mTravelSpeedTime;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The address of the Driver being controlled.
///
////////////////////////////////////////////////////////////////////////////////////
Address GlobalWaypointDriver::GetControlledDriverID() const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
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
bool GlobalWaypointDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_GLOBAL_WAYPOINT)
    {
        Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
        const QueryGlobalWaypoint* query = dynamic_cast<const QueryGlobalWaypoint*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportGlobalWaypoint report;
        CreateReportFromQuery(query, report);
        SendEvent(info, &report);

        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_TRAVEL_SPEED)
    {
        Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
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
bool GlobalWaypointDriver::IsEventSupported(const Events::Type type,
                                            const double requestedPeriodicRate,
                                            const Message* queryMessage,
                                            double& confirmedPeriodicRate,
                                            std::string& errorMessage) const
{
    // Support any type of event for QUERY_GLOBAL_WAYPOINT data.
    if(queryMessage->GetMessageCode() == QUERY_GLOBAL_WAYPOINT)
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
///   This Service supports GlobalWaypointDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointDriver::Receive(const Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_GLOBAL_WAYPOINT:
        {
            const JAUS::QueryGlobalWaypoint* query = dynamic_cast<const JAUS::QueryGlobalWaypoint*>(message);
            if(query)
            {
                ReportGlobalWaypoint report;
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
    case SET_GLOBAL_WAYPOINT:
        {
            const JAUS::SetGlobalWaypoint* command = dynamic_cast<const JAUS::SetGlobalWaypoint*>(message);
            if(command)
            {
                Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
                if(SetGlobalWaypoint(command))
                {
                    mGlobalWaypoint = *command;
                    mGlobalWaypointTime.SetCurrentTime();
                    SignalEvent(REPORT_GLOBAL_WAYPOINT);
                }
            }
        }
        break;
    case SET_TRAVEL_SPEED:
        {
            const JAUS::SetTravelSpeed* command = dynamic_cast<const JAUS::SetTravelSpeed*>(message);
            if(command)
            {
                Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
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
Message* GlobalWaypointDriver::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_GLOBAL_WAYPOINT:
        message = new JAUS::QueryGlobalWaypoint();
        break;
    case QUERY_TRAVEL_SPEED:
        message = new JAUS::QueryTravelSpeed();
        break;
    case REPORT_GLOBAL_WAYPOINT:
        message = new JAUS::ReportGlobalWaypoint();
        break;
    case REPORT_TRAVEL_SPEED:
        message = new JAUS::ReportTravelSpeed();
        break;
    case SET_GLOBAL_WAYPOINT:
        message = new JAUS::SetGlobalWaypoint();
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
///   \brief Prints the status of the Global Waypoint Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointDriver::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    
    if(mGlobalWaypoint.GetSourceID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Driving to Waypoint:\n";

        mGlobalWaypointDriverMutex.Lock();
        JAUS::SetGlobalWaypoint wp = mGlobalWaypoint;
        JAUS::SetTravelSpeed speed = mDesiredTravelSpeed;
        mGlobalWaypointDriverMutex.Unlock();

        wp.PrintMessageBody();
        if(speed.GetSourceID().IsValid())
        {
            speed.PrintMessageBody();
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
bool GlobalWaypointDriver::Resume()
{
    return (CheckDriver() &&  CheckSensors());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief // Method called to transition due to reset.
///          Release Control of Driver, clear all data.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::Reset()
{
    bool success = ReleaseResources();

    mWaypointAchievedFlag = false;
    mControlledDriverID.Clear();
    mpGlobalPoseSensor = NULL;
    mpVelocityStateSensor = NULL;
    mGlobalWaypointTime.Clear();
    mGlobalWaypoint.ClearMessage();
    mDesiredTravelSpeed.ClearMessage();

    return success;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when transitioning to a standby state.
///          Release Control of the Driver and remove subscriptions if any.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::Standby()
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
bool GlobalWaypointDriver::SetEmergency()
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
bool GlobalWaypointDriver::ClearEmergency()
{
    return Resume();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called when control is released.  Does not stop operatoin of
///          driver.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::ReleaseControl()
{
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Release Control of Driver and remove subscriptions if any, delete old
///          sensor data.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::ReleaseResources()
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);

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
///   This procedure first checks if Driver control and Global Pose subscription
///   data is available.  If unavailabe, an attempt is made to acquire the Driver
///   control and/or Global Pose subscription.  If prerequisites are met, a
///   Drive command is created and sent to the controlled Driver until the waypoint
///   is achieved.  This procedure sends an idling drive command if waypoint is
///   already achieved or cannot be achieved due to lack of sensor data..
///   
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointDriver::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    Byte status = GetComponent()->ManagementService()->GetStatus();
    switch(status)
    {
    case Management::Status::Ready:
        {
            Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
            //If all in order, generate and send a drive command, delete after use.
            if(CheckDriver())
            {
                Message *command = NULL;
                //Check whether sensor Data is valid.
                if(CheckSensors() &&
                   mGlobalWaypoint.GetSourceID().IsValid())
                {
                    //Send Idle command if Waypoint is achieved.
                    if(IsWaypointAchieved(mGlobalPose, mGlobalWaypoint))
                    {
                        if(!mWaypointAchievedFlag)
                        {
                            mWaypointAchievedFlag = true;
                            WaypointAchieved(mGlobalWaypoint);
                            // Flush out the ID so we don't keep driving to
                            // the waypoint.
                            mGlobalWaypoint.ClearMessage();
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
///   \param query The query for Global Waypoint.
///   \param[out] report The configured report based on Global Waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointDriver::CreateReportFromQuery(const QueryGlobalWaypoint* query,
                                                 ReportGlobalWaypoint& report) const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mGlobalWaypoint.GetPresenceVector();

    report.SetLatitude(mGlobalWaypoint.GetLatitude());
    report.SetLongitude(mGlobalWaypoint.GetLongitude());
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Altitude)) > 0) { report.SetAltitude(mGlobalWaypoint.GetAltitude()); }
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(mGlobalWaypoint.GetRoll()); }
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(mGlobalWaypoint.GetPitch()); }
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(mGlobalWaypoint.GetYaw()); }
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(mGlobalWaypoint.GetWaypointTolerance()); }
    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(mGlobalWaypoint.GetPathTolerance()); }    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Travel Speed.
///   \param[out] report The configured report based on Travel Speed.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointDriver::CreateReportFromQuery(const QueryTravelSpeed* query,
                                                 ReportTravelSpeed& report) const
{
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);
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
bool GlobalWaypointDriver::CheckDriver()
{
    bool success = false;
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);

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
///   \brief Checks if Global Pose, Velocity State (optional), and Acceleration
///          State (optional) sensors are available.  Make subscriptions if
///          necessary.  Updates pose and state if data is locally accessible.
///
///   \return True if Global Pose Sensor data is available.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointDriver::CheckSensors()
{
    bool success = true;
    Mutex::ScopedLock lock(&mGlobalWaypointDriverMutex);

    if(mpGlobalPoseSensor == NULL)
    {
        mpGlobalPoseSensor = dynamic_cast<GlobalPoseSensor*>(GetComponent()->GetService(GlobalPoseSensor::Name));
    }

    if(mpGlobalPoseSensor)
    {
        mGlobalPose = mpGlobalPoseSensor->GetGlobalPose();
        if(mGlobalPose.AreFieldsPresent(ReportGlobalPose::PresenceVector::Latitude | 
                                        ReportGlobalPose::PresenceVector::Longitude ) == false)
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
