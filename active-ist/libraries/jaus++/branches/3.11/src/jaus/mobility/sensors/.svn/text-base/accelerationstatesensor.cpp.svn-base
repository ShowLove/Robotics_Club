////////////////////////////////////////////////////////////////////////////////////
///
///  \file accelerationstatesensor.h
///  \brief This file contains the implementation of the AccelerationStateSensor
///         class, which reports platform acceleration.
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
#include "jaus/mobility/sensors/accelerationstatesensor.h"
#include "jaus/core/events/createevent.h"

using namespace JAUS;

const std::string AccelerationStateSensor::Name = "urn:jaus:jss:mobility:AccelerationStateSensor";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccelerationStateSensor::AccelerationStateSensor(const double updateRate)
        : Events::Child(Service::ID(AccelerationStateSensor::Name), Service::ID(Events::Name))
{
    mMaxUpdateRate = 10;
    SetSensorUpdateRate(updateRate);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
AccelerationStateSensor::~AccelerationStateSensor()
{
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration state which should be called by the platform.
///
////////////////////////////////////////////////////////////////////////////////////
void AccelerationStateSensor::SetAccelerationState(const ReportAccelerationState& state)
{
    Mutex::ScopedLock lock(&mAccelerationStateMutex);
    mAccelerationState = state;
    SignalEvent(REPORT_ACCELERATION_STATE);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the Maximum update rate of the sensor.
///
///   \return true successful, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool AccelerationStateSensor::SetSensorUpdateRate(const double rate)
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
///   \brief Gets the acceleration state updated by the platform.
///
///   \return The most recently updated ReportAccelerationState.
///
////////////////////////////////////////////////////////////////////////////////////
ReportAccelerationState AccelerationStateSensor::GetAccelerationState() const
{
    Mutex::ScopedLock lock(&mAccelerationStateMutex);
    return mAccelerationState;
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
bool AccelerationStateSensor::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_ACCELERATION_STATE)
    {
        const QueryAccelerationState* query = dynamic_cast<const QueryAccelerationState*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportAccelerationState report;
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
bool AccelerationStateSensor::IsEventSupported(const Events::Type type,
                                       const double requestedPeriodicRate,
                                       const Message* queryMessage,
                                       double& confirmedPeriodicRate,
                                       std::string& errorMessage) const
{
    // Support any type of event for QUERY_ACCELERATION_STATE data.
    if(queryMessage->GetMessageCode() == QUERY_ACCELERATION_STATE)
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
void AccelerationStateSensor::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_ACCELERATION_STATE:
        {
            const QueryAccelerationState* query = dynamic_cast<const JAUS::QueryAccelerationState*>(message);
            if(query)
            {
                ReportAccelerationState report;
                //CreateReportFromQuery already locks mAccelerationStateMutex
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case REPORT_ACCELERATION_STATE:
        {
            const JAUS::ReportAccelerationState* report = dynamic_cast<const JAUS::ReportAccelerationState*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                SetAccelerationState(*report);
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
Message* AccelerationStateSensor::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_ACCELERATION_STATE:
        message = new QueryAccelerationState();
        break;
    case REPORT_ACCELERATION_STATE:
        message = new ReportAccelerationState();
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
///   \param query The query for acceleration state.
///   \param[out] report The configured report based on mAccelerationState
///
////////////////////////////////////////////////////////////////////////////////////
void AccelerationStateSensor::CreateReportFromQuery(const QueryAccelerationState* query, ReportAccelerationState& report) const
{
    Mutex::ScopedLock lock(&mAccelerationStateMutex);
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());

   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationX) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationX))
    {
        report.SetAccelerationX(mAccelerationState.GetAccelerationX());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationY) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationY))
    {
        report.SetAccelerationY(mAccelerationState.GetAccelerationY());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationZ) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationZ))
    {
        report.SetAccelerationZ(mAccelerationState.GetAccelerationZ());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationRMS) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::AccelerationRMS))
    {
        report.SetAccelerationRMS(mAccelerationState.GetAccelerationRMS());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::RollAcceleration) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::RollAcceleration))
    {
        report.SetRollAcceleration(mAccelerationState.GetRollAcceleration());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::PitchAcceleration) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::PitchAcceleration))
    {
        report.SetPitchAcceleration(mAccelerationState.GetPitchAcceleration());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::YawAcceleration) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::YawAcceleration))
    {
        report.SetYawAcceleration(mAccelerationState.GetYawAcceleration());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::RotationalAccelerationRMS) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::RotationalAccelerationRMS))
    {
        report.SetRotationalAccelerationRMS(mAccelerationState.GetRotationalAccelerationRMS());
    }
   if(query->IsFieldPresent(ReportAccelerationState::PresenceVector::TimeStamp) &&
        mAccelerationState.IsFieldPresent(ReportAccelerationState::PresenceVector::TimeStamp))
    {
        report.SetTimeStamp(mAccelerationState.GetTimeStamp());
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints the status of the Primitive Driver.
///
////////////////////////////////////////////////////////////////////////////////////
void AccelerationStateSensor::PrintStatus() const
{
    //Mutex::ScopedLock lock(&mAccelerationStateMutex);
    mAccelerationStateMutex.Lock();
    JAUS::AccelerationState acc = mAccelerationState;
    mAccelerationStateMutex.Unlock();

    if(GetSynchronizeID().IsValid())
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Synchronized to [" << GetSynchronizeID().ToString() << "]:\n";
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Acceleration State:\n";
    }

    acc.PrintMessageBody();
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
void AccelerationStateSensor::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription, and if not create it.
        if(EventsService()->HaveSubscription(REPORT_ACCELERATION_STATE, syncID) == false)
        {
            QueryAccelerationState query(syncID, GetComponentID());
            query.SetPresenceVector(query.GetPresenceVectorMask());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}


/*  End of File */
