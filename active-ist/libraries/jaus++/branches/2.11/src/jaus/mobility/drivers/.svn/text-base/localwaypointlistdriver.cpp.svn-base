////////////////////////////////////////////////////////////////////////////////////
///
///  \file localwaypointlistdriver.cpp
///  \brief This file contains the implementation of the Local Waypoint List
///         Driver service.
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
#include "jaus/mobility/drivers/localwaypointlistdriver.h"
#include "jaus/core/component.h"
#include <iostream>
#include <iomanip>

using namespace JAUS;

const std::string LocalWaypointListDriver::Name = "urn:jaus:jss:mobility:LocalWaypointListDriver";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointListDriver::LocalWaypointListDriver() : ListDriver(Service::ID(LocalWaypointListDriver::Name),
                                                                Service::ID(ListManager::Name))
{
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LocalWaypointListDriver::~LocalWaypointListDriver()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is called when an Execute List command is received.
///
///   Overload this method to be notified when list execution should begin and
///   at what speed to execute at.
///
///   \param[in] speedMetersPerSecond The speed to execute at in m/s.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::ExecuteList(const double speedMetersPerSecond)
{
    Mutex::ScopedLock lock(&mListDriverMutex);
    if(speedMetersPerSecond > 0.0)
    {
        mSpeedMetersPerSecond = speedMetersPerSecond;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used by the List Manager service to verify if the
///          element (or command message) is supported by this service.
///
///   \param[in] message The message to check for support of.
///
///   \return True if message is supported, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointListDriver::IsElementSupported(const JAUS::Message* message) const
{
    const JAUS::SetLocalWaypoint* command = dynamic_cast<const JAUS::SetLocalWaypoint*>(message);
    return command != NULL ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports LocalWaypointListDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_LOCAL_WAYPOINT:
        {
            Mutex::ScopedLock lock(&mListDriverMutex);
            const QueryLocalWaypoint* query = dynamic_cast<const QueryLocalWaypoint*>(message);

            if(query == NULL)
            {
                return;
            }

            ReportLocalWaypoint report(message->GetSourceID(), GetComponentID());
            Element current = GetActiveListElement();
            if(current.mpElement)
            {
                SetLocalWaypoint* wp = dynamic_cast<SetLocalWaypoint*>(current.mpElement);
                if(wp)
                {
                    UInt pv1 = query->GetPresenceVector();
                    UInt pv2 = wp->GetPresenceVector();

                    report.SetX(wp->GetX());
                    report.SetY(wp->GetY());
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Z)) > 0) { report.SetZ(wp->GetZ()); }
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(wp->GetRoll()); }
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(wp->GetPitch()); }
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(wp->GetYaw()); }
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(wp->GetWaypointTolerance()); }
                    if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(wp->GetPathTolerance()); }    
                }
            }
            Send(&report);
        }
        break;
    default:
        ListDriver::Receive(message);
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
Message* LocalWaypointListDriver::CreateMessage(const UShort messageCode) const
{
    switch(messageCode)
    {
    case SET_LOCAL_WAYPOINT:
        return new JAUS::SetLocalWaypoint();
        break;
    case QUERY_LOCAL_WAYPOINT:
        return new QueryLocalWaypoint();
        break;
    case REPORT_LOCAL_WAYPOINT:
        return new ReportLocalWaypoint();
        break;
    default:
        return ListDriver::CreateMessage(messageCode);
        break;
    }
    return NULL;
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
bool LocalWaypointListDriver::IsEventSupported(const Events::Type type,
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
///   \brief Generates an event for the given information.
///
///   \param[in] info The event information (ID, Sequence #, etc.) for generation.
///
///   \return True if event generated, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool LocalWaypointListDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_LOCAL_WAYPOINT)
    {
        const QueryLocalWaypoint* query = dynamic_cast<const QueryLocalWaypoint*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportLocalWaypoint report;
        Element current = GetActiveListElement();
        if(current.mpElement)
        {
            SetLocalWaypoint* wp = dynamic_cast<SetLocalWaypoint*>(current.mpElement);
            if(wp)
            {
                UInt pv1 = query->GetPresenceVector();
                UInt pv2 = wp->GetPresenceVector();

                report.SetX(wp->GetX());
                report.SetY(wp->GetY());
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Z)) > 0) { report.SetZ(wp->GetZ()); }
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(wp->GetRoll()); }
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(wp->GetPitch()); }
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(wp->GetYaw()); }
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(wp->GetWaypointTolerance()); }
                if( (pv2 & (pv1 & LocalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(wp->GetPathTolerance()); }    
            }
        }
        SendEvent(info, &report);
        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_TRAVEL_SPEED)
    {
        Mutex::ScopedLock lock(&mListDriverMutex);
        const QueryTravelSpeed* query = dynamic_cast<const QueryTravelSpeed*>(info.mpQueryMessage);
        if(query == NULL)
        {
            return false;
        }
        ReportTravelSpeed report;
        report.SetSpeed(mSpeedMetersPerSecond);
        SendEvent(info, &report);
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current active waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
JAUS::SetLocalWaypoint LocalWaypointListDriver::GetCurrentWaypoint() const
{
    Element current = GetActiveListElement();
    if(current.mpElement)
    {
        JAUS::SetLocalWaypoint* cmd = dynamic_cast<JAUS::SetLocalWaypoint*>(current.mpElement);
        if(cmd)
        {
            return *cmd;
        }
    }
    return JAUS::SetLocalWaypoint();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current active waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<JAUS::SetLocalWaypoint> LocalWaypointListDriver::GetWaypointList() const
{
    // Get local waypoint list.
    JAUS::Element::Map elementList = GetElementList();
    // Convert to SetLocalWaypointCommands
    JAUS::Element::Map::iterator listElement;
    std::vector<JAUS::SetLocalWaypoint> commandList;
    for(listElement = elementList.begin();
        listElement != elementList.end();
        listElement++)
    {
        if(listElement->second.mpElement->GetMessageCode() == JAUS::SET_LOCAL_WAYPOINT)
        {
            commandList.push_back(*( (JAUS::SetLocalWaypoint *)(listElement->second.mpElement)) );
        }
    }
    return commandList;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Local Waypoint Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void LocalWaypointListDriver::PrintStatus() const
{
    std::vector<JAUS::SetLocalWaypoint> waypoints = GetWaypointList();
    if(IsExecuting() == true)
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Driving to Waypoint [" << GetActiveListElementID() << "]:\n";
        std::cout << "There are " << waypoints.size() << " Waypoints in the List.\n";
        std::cout << "Execution Speed (m/s): " << std::fixed << std::setprecision(2) << mSpeedMetersPerSecond << std::endl;
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Idle\n";
        std::cout << "There are " << waypoints.size() << " Waypoints in the List.\n";
    }
}


/*  End of File */
