////////////////////////////////////////////////////////////////////////////////////
///
///  \file subsystemcommand.cpp
///  \brief Contains the Subsystem service implementation.
///
///  <br>Author(s): Daniel Barber
///  Created: 10 February 2010
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
#include "jaus/extras/command/subsystemcommand.h"
#include "jaus/mobility/sensors/globalposesensor.h"
#include "jaus/mobility/sensors/velocitystatesensor.h"
#include <cmath>
#include <tinyxml/tinyxml.h>

using namespace JAUS;

const std::string SubsystemCommand::Name = "urn:jaus:jss:jpp:extras:SubsystemCommand";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
SubsystemCommand::SubsystemCommand(const Service::ID& id) : Management::Child(id,
                                                                              Service::ID(Management::Name))
{
    mOnChangeEventsFlag = false;
    mDesiredPeriodicRateHz = 25;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
SubsystemCommand::~SubsystemCommand()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Registers a callback to get system state information from 
///          the discovery service.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::Initialize()
{
    //DiscoveryService()->RegisterCallback(this, true);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the type of global pose subscriptions to make.  Choices are
///   every change or periodic. 
///
///   This method must be called before the Component/Service is initialized.
///
///   \param[in] everyChange If true, every change events are requested.
///   \param[in] desiredPeriodicRate If periodic events, this is the desired rate.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::SetGlobalPoseSubscriptionType(const bool everyChange,
                                                     const double desiredPeriodicRate)
{
    if(GetComponent() == NULL || (GetComponent() && GetComponent()->IsInitialized() == false))
    {
        mOnChangeEventsFlag = everyChange;
        mDesiredPeriodicRateHz = desiredPeriodicRate;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the service.
///
////////////////////////////////////////////////////////////////////////////////////
const Discovery* SubsystemCommand::DiscoveryService() const
{
    return GetComponent()->DiscoveryService();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the service.
///
////////////////////////////////////////////////////////////////////////////////////
Discovery* SubsystemCommand::DiscoveryService()
{
    return GetComponent()->DiscoveryService();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the service.
///
////////////////////////////////////////////////////////////////////////////////////
const AccessControl* SubsystemCommand::AccessControlService() const
{
    return GetComponent()->AccessControlService();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a pointer to the service.
///
////////////////////////////////////////////////////////////////////////////////////
AccessControl* SubsystemCommand::AccessControlService()
{
    return GetComponent()->AccessControlService();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sends a command, and if control is not present, it is automatically
///          requested.
///
///   \param[in] commandMessage Command message to send.
///   \param[in] sendResumeFlag If control is requested to send the command, and
///                             this is true, a Resume command is sent first.
///   \param[in] waitTimeMs If control is requested, this is how long to wait
///                         for a response.
///
///   \return True if message is sent successfully.
///
////////////////////////////////////////////////////////////////////////////////////
bool SubsystemCommand::SendCommand(const Message* commandMessage,
                                   const bool sendResumeFlag,
                                   const unsigned int waitTimeMs)
{
    Address dest = commandMessage->GetDestinationID();
    // Make sure we have control.
    RequestComponentControl(dest, sendResumeFlag, true, waitTimeMs);
    return Send(commandMessage);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes messages received by the component.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case REPORT_GLOBAL_POSE:
        {
            const ReportGlobalPose* report = dynamic_cast<const ReportGlobalPose*>(message);
            if(report)
            {
                Wgs wgs;
                Attitude att;
                Time timestamp;

                wgs = DiscoveryService()->GetVehicle(report->GetSourceID().mSubsystem)->mPosition;
                att = DiscoveryService()->GetVehicle(report->GetSourceID().mSubsystem)->mAttitude;

                // Build on top of the current values.

                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Latitude))
                {
                    wgs.mLatitude = report->GetLatitude();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Longitude))
                {
                    wgs.mLongitude = report->GetLongitude();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Altitude))
                {
                    wgs.mElevation = report->GetAltitude();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Roll))
                {
                    att.mX = report->GetRoll();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Pitch))
                {
                    att.mY = report->GetPitch();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::Yaw))
                {
                    att.mZ = report->GetYaw();
                }
                if(report->IsFieldPresent(ReportGlobalPose::PresenceVector::TimeStamp))
                {
                    timestamp = report->GetTimeStamp();
                }
                else
                {
                    timestamp.SetCurrentTime();
                }

                // Save the results.
                DiscoveryService()->SetVehiclePose(report->GetSourceID().mSubsystem, wgs, att, timestamp);

                // Trigger callbacks to signal change in data.
                DiscoveryService()->TriggerDiscoveryCallbacks();
            }
        }
        break;
    case REPORT_VELOCITY_STATE:
        {
            const ReportVelocityState* report = dynamic_cast<const ReportVelocityState*>(message);
            if(report)
            {
                Point3D linearVelocity;
                linearVelocity.Set(report->GetVelocityX(),
                                   report->GetVelocityY(),
                                   report->GetVelocityZ());
                // Save the results.
                DiscoveryService()->SetVehicleLinearVelocity(report->GetSourceID().mSubsystem, linearVelocity, Time(true));
                // Trigger callbacks to signal change in data.
                DiscoveryService()->TriggerDiscoveryCallbacks();
            }
        }
        break;
    default:
        break;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates messages supported by the service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* SubsystemCommand::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case REPORT_GLOBAL_POSE:
        message = new ReportGlobalPose();
        break;
    case REPORT_VELOCITY_STATE:
        message = new ReportVelocityState();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks if the service has global pose subscriptions from newly
///   discovered vehicles.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    Subsystem::Map system;
    static Time::Stamp checkTime = 0;
    if(Time::GetUtcTimeMs() - checkTime > 1000)
    {
        if(GetComponent())
        {
            GetComponent()->DiscoveryService()->GetSubsystems(system);
            this->ProcessSystemState(system);
            Subsystem::DeleteSubsystemMap(system);
        }
        checkTime = Time::GetUtcTimeMs();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints service status to console.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::PrintStatus() const
{
    Vehicle::Map vehicles;

    GetComponent()->DiscoveryService()->GetVehicles(vehicles);

    Vehicle::Map::const_iterator subsystem;
    for(subsystem = vehicles.begin();
        subsystem != vehicles.end();
        subsystem++)
    {
        std::cout << "========================================================================\n";
        std::cout << "Vehicle [" << subsystem->first << "] - " << subsystem->second->mIdentification << std::endl;
        Subsystem::Configuration::const_iterator node;
        for(node = subsystem->second->mConfiguration.begin();
            node != subsystem->second->mConfiguration.end();
            node++)
        {
            Subsystem::Component::Set::const_iterator component;
            for(component = node->second.begin();
                component != node->second.end();
                component++)
            {
                if(GetComponent()->AccessControlService()->HaveControl(component->mID))
                {
                    std::cout << "    Controlling: " << component->mIdentification << " - " << component->mID.ToString() << std::endl;
                }
                else
                {
                    std::cout << "    " << component->mIdentification << " - " << component->mID.ToString() << std::endl;
                }                
            }
        }
        std::cout << "    ";
        subsystem->second->mPosition.Print();
        double x, y, z;
        x = fabs(subsystem->second->mAttitude.mX) < .001 ? 0.0 : subsystem->second->mAttitude.mX;
        y = fabs(subsystem->second->mAttitude.mY) < .001 ? 0.0 : subsystem->second->mAttitude.mY;
        z = fabs(subsystem->second->mAttitude.mZ) < .001 ? 0.0 : subsystem->second->mAttitude.mZ;
        std::cout << "    A <" << x << ", " << y << ", " << z << ">\n    ";
        x = fabs(subsystem->second->mLinearVelocity.mX) < .001 ? 0.0 : subsystem->second->mLinearVelocity.mX;
        y = fabs(subsystem->second->mLinearVelocity.mY) < .001 ? 0.0 : subsystem->second->mLinearVelocity.mY;
        z = fabs(subsystem->second->mLinearVelocity.mZ) < .001 ? 0.0 : subsystem->second->mLinearVelocity.mZ;
        std::cout << "    V <" << x << ", " << y << ", " << z << ">\n    ";
        std::cout << subsystem->second->mUpdateTime.ToString() << std::endl;
    }  

    Vehicle::DeleteVehicleMap(vehicles);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called when the system state updates.  This method checks
///          to see if it has global pose subscriptions for each vehicle.
///
////////////////////////////////////////////////////////////////////////////////////
void SubsystemCommand::ProcessSystemState(const Subsystem::Map& system)
{
    // See if we need to make any requests for Global Pose data.
    Subsystem::Map::const_iterator s;
    for(s = system.begin();
        s != system.end();
        s++)
    {
        Address id;
        if(s->second->HaveService(GlobalPoseSensor::Name, &id))
        {
            if(EventsService()->HaveSubscription(REPORT_GLOBAL_POSE, id) == false)
            {
                QueryGlobalPose query;
                query.SetPresenceVector(query.GetPresenceVectorMask()); // Try all data.
                if(mOnChangeEventsFlag)
                {
                    EventsService()->RequestEveryChangeEvent(id, &query);
                }
                else
                {
                    EventsService()->RequestPeriodicEvent(id, &query, mDesiredPeriodicRateHz);
                }
            }
        }
        if(s->second->HaveService(VelocityStateSensor::Name, &id))
        {
            if(EventsService()->HaveSubscription(REPORT_VELOCITY_STATE, id) == false)
            {
                QueryVelocityState query;
                query.SetPresenceVector(query.GetPresenceVectorMask()); // Try all data.
                if(mOnChangeEventsFlag)
                {
                    EventsService()->RequestEveryChangeEvent(id, &query);
                }
                else
                {
                    EventsService()->RequestPeriodicEvent(id, &query, mDesiredPeriodicRateHz);
                }
            }
        }
    }
}


/*  End of File */
