////////////////////////////////////////////////////////////////////////////////////
///
///  \file timeservice.h
///  \brief Contains the Time Service implementation.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 29 December 2009
///  <br>Copyright (c) 2009
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
#include "jaus/core/time/timeservice.h"
#include "jaus/core/events/event.h"
#include "jaus/core/time/querytime.h"
#include "jaus/core/time/reporttime.h"
#include "jaus/core/time/settime.h"
#include <iostream>
#include <ctime>

using namespace JAUS;

const std::string TimeService::Name = "urn:jaus:jss:core:Time";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TimeService::TimeService() : AccessControl::Child(Service::ID(TimeService::Name), Service::ID(AccessControl::Name))
{
    mpChangeTimeMethod = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TimeService::~TimeService()
{
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
bool TimeService::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_TIME)
    {
        ReportTime report;
        
        const QueryTime* query = dynamic_cast<const QueryTime *>(info.mpQueryMessage);

        if(query == NULL || query->IsFieldPresent(SetTime::PresenceVector::Time))
        {
            Time t;
            t.SetCurrentTime();
            report.SetTimeStamp(t);
        }
        if(query == NULL || query->IsFieldPresent(SetTime::PresenceVector::Date))
        {
            Date d;                
            // Get the current values.
            time_t rawtime;
            tm* ptm;
            time(&rawtime);
            ptm = gmtime(&rawtime);

            d.mYear = ptm->tm_year + 1900;
            d.mDay = ptm->tm_mday;
            d.mMonth = ptm->tm_mon;
            report.SetDateStamp(d);
        }

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
bool TimeService::IsEventSupported(const Events::Type type,
                                   const double requestedPeriodicRate,
                                   const Message* queryMessage,
                                   double& confirmedPeriodicRate,
                                   std::string& errorMessage) const
{
    // Support any type of event for REPORT_TIME data.
    if(queryMessage->GetMessageCode() == QUERY_TIME)
    {
        confirmedPeriodicRate = requestedPeriodicRate;
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services.
///
///   This Service supports the following message: Query Heartbeat Pulse and
///   Report Heartbeat Pulse.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void TimeService::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_TIME:
        {           
            const QueryTime* query = dynamic_cast<const QueryTime*>(message);
            if(query)
            {
                ReportTime report(message->GetSourceID(), GetComponentID());

                if(query->IsFieldPresent(SetTime::PresenceVector::Time))
                {
                    Time t;
                    t.SetCurrentTime();
                    report.SetTimeStamp(t);
                }
                if(query->IsFieldPresent(SetTime::PresenceVector::Date))
                {
                    Date d;                
                    // Get the current values.
                    time_t rawtime;
                    tm* ptm;
                    time(&rawtime);
                    ptm = gmtime(&rawtime);

                    d.mYear = ptm->tm_year + 1900;
                    d.mDay = ptm->tm_mday;
                    d.mMonth = ptm->tm_mon + 1; // tm_mon is 0-11, we need 1-12
                    report.SetDateStamp(d);
                }

                Send(&report);
            }
        }
        break;
    case REPORT_TIME:
        {
            // Support needed if we are synchronizaing
            const ReportTime* report = dynamic_cast<const ReportTime*>(message);
            if(report && report->GetSourceID() == GetSynchronizeID())
            {
                Time t;
                Date d;
                
                // Get the current values.
                time_t rawtime;
                tm* ptm;
                time(&rawtime);
                ptm = gmtime(&rawtime);

                d.mYear = ptm->tm_year + 1900;
                d.mDay = ptm->tm_mday;
                d.mMonth = ptm->tm_mon;

                t.SetCurrentTime();

                if(report->IsFieldPresent(ReportTime::PresenceVector::Time))
                {
                    t = report->GetTimeStamp();
                }
                if(report->IsFieldPresent(ReportTime::PresenceVector::Date))
                {
                    d = report->GetDateStamp();
                }

                if(mpChangeTimeMethod)
                {
                    mpChangeTimeMethod(t, d);
                }
                else
                {
                    CxUtils::SetSystemTime(d.mYear,
                                           d.mMonth,
                                           t.mDay,
                                           t.mHour,
                                           t.mMinute,
                                           t.mSecond,
                                           t.mMilliseconds);
                }
                // Signal change in time.
                SignalEvent(REPORT_TIME);
            }
        }
        break;
    case SET_TIME:
        {
            const SetTime* command = dynamic_cast<const SetTime*>(message);
            if(command)
            {
                Time t;
                Date d;
                
                // Get the current values.
                time_t rawtime;
                tm* ptm;
                time(&rawtime);
                ptm = gmtime(&rawtime);

                d.mYear = ptm->tm_year + 1900;
                d.mDay = ptm->tm_mday;
                d.mMonth = ptm->tm_mon;

                t.SetCurrentTime();

                if(command->IsFieldPresent(SetTime::PresenceVector::Time))
                {
                    t = command->GetTimeStamp();
                }
                if(command->IsFieldPresent(SetTime::PresenceVector::Date))
                {
                    d = command->GetDateStamp();
                }

                if(mpChangeTimeMethod)
                {
                    mpChangeTimeMethod(t, d);
                }
                else
                {
                    CxUtils::SetSystemTime(d.mYear,
                                           d.mMonth,
                                           t.mDay,
                                           t.mHour,
                                           t.mMinute,
                                           t.mSecond,
                                           t.mMilliseconds);
                }
                // Signal change in time.
                SignalEvent(REPORT_TIME);
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
///   This Service supports messages for the Time Service.
///
///   \param[in] messageCode Message to create.
///
///   \return Pointer to newly allocated Message data, NULL if message is not
///           supported by the Service.
///
////////////////////////////////////////////////////////////////////////////////////
Message* TimeService::CreateMessage(const UShort messageCode) const
{
    Message* message;
    switch(messageCode)
    {
    case QUERY_TIME:
        message = new QueryTime();
        break;
    case REPORT_TIME:
        message = new ReportTime();
        break;
    case SET_TIME:
        message = new SetTime();
        break;
    default:
        message = NULL;
        break;
    }
    return message;
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
void TimeService::CheckServiceSynchronization(const unsigned int timeSinceLastCheckMs)
{
    Address syncID = GetSynchronizeID();
    if(syncID.IsValid())
    {
        // See if we have an active subscription
        if(EventsService()->HaveSubscription(REPORT_TIME, syncID) == false)
        {
            QueryTime query(syncID, GetComponentID());
            query.SetPresenceVector(query.GetPresenceVectorMask());
            EventsService()->RequestEveryChangeEvent(syncID, &query);
        }
    }
}

/*  End of File */
