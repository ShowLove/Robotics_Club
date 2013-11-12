////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalwaypointlistdriver.h
///  \brief This file contains the implementation of the Global Waypoint List
///         Driver service.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 23 April 2010
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
#include "jaus/mobility/drivers/globalwaypointlistdriver.h"
#include "jaus/core/component.h"
#include <iostream>
#include <iomanip>

using namespace JAUS;

const std::string GlobalWaypointListDriver::Name = "urn:jaus:jss:mobility:GlobalWaypointListDriver";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] updateRateHz How often to generate drive commands in Hz.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypointListDriver::GlobalWaypointListDriver(const double updateRateHz) : ListDriver(Service::ID(GlobalWaypointListDriver::Name),
                                                                                           Service::ID(ListManager::Name))
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalWaypointListDriver::~GlobalWaypointListDriver()
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
void GlobalWaypointListDriver::ExecuteList(const double speedMetersPerSecond)
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
bool GlobalWaypointListDriver::IsElementSupported(const JAUS::Message* message) const
{
    const JAUS::SetGlobalWaypoint* command = dynamic_cast<const JAUS::SetGlobalWaypoint*>(message);
    return command != NULL ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports GlobalWaypointListDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointListDriver::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_GLOBAL_WAYPOINT:
        {
            Mutex::ScopedLock lock(&mListDriverMutex);
            const QueryGlobalWaypoint* query = dynamic_cast<const QueryGlobalWaypoint*>(message);

            if(query == NULL)
            {
                return;
            }

            ReportGlobalWaypoint report(message->GetSourceID(), GetComponentID());
            Element current = GetActiveListElement();
            if(current.mpElement)
            {
                SetGlobalWaypoint* wp = dynamic_cast<SetGlobalWaypoint*>(current.mpElement);
                if(wp)
                {
                    UInt pv1 = query->GetPresenceVector();
                    UInt pv2 = wp->GetPresenceVector();

                    report.SetLatitude(wp->GetLatitude());
                    report.SetLongitude(wp->GetLongitude());
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Altitude)) > 0) { report.SetAltitude(wp->GetAltitude()); }
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(wp->GetRoll()); }
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(wp->GetPitch()); }
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(wp->GetYaw()); }
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(wp->GetWaypointTolerance()); }
                    if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(wp->GetPathTolerance()); }    
                }
            }
            Send(&report);
        }
        break;
    case REPORT_GLOBAL_WAYPOINT:
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
Message* GlobalWaypointListDriver::CreateMessage(const UShort messageCode) const
{
    switch(messageCode)
    {
    case SET_GLOBAL_WAYPOINT:
        return new SetGlobalWaypoint();
        break;
    case QUERY_GLOBAL_WAYPOINT:
        return new QueryGlobalWaypoint();
        break;
    case REPORT_GLOBAL_WAYPOINT:
        return new ReportGlobalWaypoint();
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
bool GlobalWaypointListDriver::IsEventSupported(const Events::Type type,
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
///   \brief Generates an event for the given information.
///
///   \param[in] info The event information (ID, Sequence #, etc.) for generation.
///
///   \return True if event generated, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalWaypointListDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_GLOBAL_WAYPOINT)
    {
        const QueryGlobalWaypoint* query = dynamic_cast<const QueryGlobalWaypoint*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportGlobalWaypoint report;
        Element current = GetActiveListElement();
        if(current.mpElement)
        {
            SetGlobalWaypoint* wp = dynamic_cast<SetGlobalWaypoint*>(current.mpElement);
            if(wp)
            {
                UInt pv1 = query->GetPresenceVector();
                UInt pv2 = wp->GetPresenceVector();

                report.SetLatitude(wp->GetLatitude());
                report.SetLongitude(wp->GetLongitude());
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Altitude)) > 0) { report.SetAltitude(wp->GetAltitude()); }
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Roll)) > 0) { report.SetRoll(wp->GetRoll()); }
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Pitch)) > 0) { report.SetPitch(wp->GetPitch()); }
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::Yaw)) > 0) { report.SetYaw(wp->GetYaw()); }
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::WaypointTolerance)) > 0) { report.SetWaypointTolerance(wp->GetWaypointTolerance()); }
                if( (pv2 & (pv1 & ReportGlobalWaypoint::PresenceVector::PathTolerance)) > 0) { report.SetPathTolerance(wp->GetPathTolerance()); }    
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
JAUS::SetGlobalWaypoint GlobalWaypointListDriver::GetCurrentWaypoint() const
{
    Element current = GetActiveListElement();
    if(current.mpElement)
    {
        JAUS::SetGlobalWaypoint* cmd = dynamic_cast<JAUS::SetGlobalWaypoint*>(current.mpElement);
        if(cmd)
        {
            return *cmd;
        }
    }
    return JAUS::SetGlobalWaypoint();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Current active waypoint.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<JAUS::SetGlobalWaypoint> GlobalWaypointListDriver::GetWaypointList() const
{
    // Get local waypoint list.
    JAUS::Element::Map elementList = GetElementList();
    // Convert to SetGlobalWaypointCommands
    JAUS::Element::Map::iterator listElement;
    std::vector<JAUS::SetGlobalWaypoint> commandList;
    for(listElement = elementList.begin();
        listElement != elementList.end();
        listElement++)
    {
        if(listElement->second.mpElement->GetMessageCode() == JAUS::SET_GLOBAL_WAYPOINT)
        {
            commandList.push_back(*( (JAUS::SetGlobalWaypoint *)(listElement->second.mpElement)) );
        }
    }
    return commandList;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Global Waypoint Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalWaypointListDriver::PrintStatus() const
{
    std::vector<JAUS::SetGlobalWaypoint> waypoints = GetWaypointList();
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
