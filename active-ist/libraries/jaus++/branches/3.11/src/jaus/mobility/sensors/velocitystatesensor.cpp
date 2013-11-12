////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatesensor.h
///  \brief This file contains the implementation of the VelocityStateSensor class,
///         which reports instantaneous platform velocity.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 1 Febuary 2010
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
#include "jaus/mobility/sensors/velocitystatesensor.h"
#include "jaus/core/events/createevent.h"

using namespace JAUS;

const std::string VelocityStateSensor::Name = "urn:jaus:jss:mobility:VelocityStateSensor";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::VelocityStateSensor(const double updateRate) : Events::Child(Service::ID(VelocityStateSensor::Name),
                                                                                  Service::ID(Events::Name))
{
    mMaxUpdateRate = 10;
    SetSensorUpdateRate(updateRate);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateSensor::~VelocityStateSensor()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the velocity state which should be called by the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::SetVelocityState(const VelocityState& state)
{
    Mutex::ScopedLock lock(&mVelocityStateMutex);
    mVelocityState = state;
    SignalEvent(REPORT_VELOCITY_STATE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Maximum update rate of the sensor.
///
///   \return true successful, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateSensor::SetSensorUpdateRate(const double rate)
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
///   \brief Gets the velocity state updated by the platform.
///
///   \return The most recently updated ReportVelocityState.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityState VelocityStateSensor::GetVelocityState() const
{
    Mutex::ScopedLock lock(&mVelocityStateMutex);
    return mVelocityState;
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
bool VelocityStateSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_VELOCITY_STATE)
    {
        const QueryVelocityState* query = dynamic_cast<const QueryVelocityState*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        VelocityState report;
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
bool VelocityStateSensor::IsEventSupported(const Events::Type type,
                                       const double requestedPeriodicRate,
                                       const Message* queryMessage,
                                       double& confirmedPeriodicRate,
                                       std::string& errorMessage) const
{
    // Support any type of event for QUERY_VELOCITY_STATE data.
    if(queryMessage->GetMessageCode() == QUERY_VELOCITY_STATE)
    {
        if(requestedPeriodicRate >= CreateEvent::Limits::MinUpdateRate &&
           requestedPeriodicRate <= mMaxUpdateRate)
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
void VelocityStateSensor::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_VELOCITY_STATE:
        {
            const QueryVelocityState* query = dynamic_cast<const JAUS::QueryVelocityState*>(message);
            if(query)
            {
                VelocityState report;
                //CreateReportFromQuery already locks mVelocityStateMutex
                CreateReportFromQuery(query, report);
                if(Send(&report) == false)
                {
                    std::cout << "ERROR!\n";
                }
            }
        }
        break;
    case REPORT_VELOCITY_STATE:
        {
            const ReportVelocityState* report = dynamic_cast<const JAUS::ReportVelocityState*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                SetVelocityState(*report);
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
Message* VelocityStateSensor::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_VELOCITY_STATE:
        message = new QueryVelocityState();
        break;
    case REPORT_VELOCITY_STATE:
        message = new ReportVelocityState();
        break;
    default:
        message = NULL;
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for velocity state.
///   \param[out] report The configured report based on mVelocityState
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::CreateReportFromQuery(const QueryVelocityState* query, VelocityState& report) const
{
    Mutex::ScopedLock lock(&mVelocityStateMutex);

    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());

   if(query->IsFieldPresent(VelocityState::PresenceVector::VelocityX) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::VelocityX))
    {
        report.SetVelocityX(mVelocityState.GetVelocityX());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::VelocityY) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::VelocityY))
    {
        report.SetVelocityY(mVelocityState.GetVelocityY());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::VelocityZ) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::VelocityZ))
    {
        report.SetVelocityZ(mVelocityState.GetVelocityZ());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::VelocityRMS) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::VelocityRMS))
    {
        report.SetVelocityRMS(mVelocityState.GetVelocityRMS());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::RollRate) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::RollRate))
    {
        report.SetRollRate(mVelocityState.GetRollRate());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::PitchRate) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::PitchRate))
    {
        report.SetPitchRate(mVelocityState.GetPitchRate());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::YawRate) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::YawRate))
    {
        report.SetYawRate(mVelocityState.GetYawRate());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::RateRMS) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::RateRMS))
    {
        report.SetRateRMS(mVelocityState.GetRateRMS());
    }
   if(query->IsFieldPresent(VelocityState::PresenceVector::TimeStamp) &&
        mVelocityState.IsFieldPresent(VelocityState::PresenceVector::TimeStamp))
    {
        report.SetTimeStamp(mVelocityState.GetTimeStamp());
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateSensor::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mVelocityStateMutex);
    if(GetSynchronizeID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Synchronized to [" << GetSynchronizeID().ToString() << "]:\n";
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Velocity State:\n";
    }
    VelocityState velocity;
    mVelocityStateMutex.Lock();
    velocity = mVelocityState;
    mVelocityStateMutex.Unlock();
    velocity.PrintMessageBody();
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
void VelocityStateSensor::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription, and if not create it.
        if(EventsService()->HaveSubscription(REPORT_VELOCITY_STATE, syncID) == false)
        {
            QueryVelocityState query(syncID, GetComponentID());
            query.SetPresenceVector(query.GetPresenceVectorMask());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}


/*  End of File */
