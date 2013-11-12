////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the implementation of the PrimitiveDriver class,
///         used serving as an interface for basic platform mobility.
///
///  <br>Author(s): Bo Sun
///  <br>Created: 12 January 2010
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
#include "jaus/mobility/drivers/primitivedriver.h"
#include "jaus/core/component.h"

using namespace JAUS;

const std::string PrimitiveDriver::Name = "urn:jaus:jss:mobility:PrimitiveDriver";


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::PrimitiveDriver() : Management::Child(Service::ID(PrimitiveDriver::Name), 
                                                       Service::ID(Management::Name))
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
PrimitiveDriver::~PrimitiveDriver()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current Wrench Effort command received.
///
///   \return The current Wrench Effort as a SetWrenchEffort message.
///
////////////////////////////////////////////////////////////////////////////////////
SetWrenchEffort PrimitiveDriver::GetCurrentWrenchEffort() const
{
    Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
    return mCurrentWrenchEffort;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a Set Wrench Effort command was received.
///
////////////////////////////////////////////////////////////////////////////////////
Time PrimitiveDriver::GetWrenchEffortTime() const
{
    Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
    return mWrenchEffortTime;
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
bool PrimitiveDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_WRENCH_EFFORT)
    {
        Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
        const QueryWrenchEffort* query = dynamic_cast<const QueryWrenchEffort*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportWrenchEffort report;
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
bool PrimitiveDriver::IsEventSupported(const Events::Type type,
                                       const double requestedPeriodicRate,
                                       const Message* queryMessage,
                                       double& confirmedPeriodicRate,
                                       std::string& errorMessage) const
{
    // Support any type of event for QUERY_WRENCH_EFFORT data.
    if(queryMessage->GetMessageCode() == QUERY_WRENCH_EFFORT)
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
///   This Service supports PrimitiveDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::Receive(const Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_WRENCH_EFFORT:
        {
            const JAUS::QueryWrenchEffort* query = dynamic_cast<const JAUS::QueryWrenchEffort*>(message);
            if(query)
            {
                Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
                ReportWrenchEffort report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case SET_WRENCH_EFFORT:
        {
            const JAUS::SetWrenchEffort* command = dynamic_cast<const JAUS::SetWrenchEffort*>(message);
            if(command)
            {
                if(GetComponent()->ManagementService()->GetStatus() == Management::Status::Ready)
                {
                    {
                        Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
                        mCurrentWrenchEffort = *command;
                        mWrenchEffortTime.SetCurrentTime();
                    }
                    SetWrenchEffort(command);
                    SignalEvent(REPORT_WRENCH_EFFORT);
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
Message* PrimitiveDriver::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_WRENCH_EFFORT:
        message = new JAUS::QueryWrenchEffort();
        break;
    case REPORT_WRENCH_EFFORT:
        message = new JAUS::ReportWrenchEffort();
        break;
    case SET_WRENCH_EFFORT:
        message = new JAUS::SetWrenchEffort();
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
void PrimitiveDriver::PrintStatus() const
{
    std::cout << "[" << GetServiceID().ToString() << "] - Current Wrench Effort:\n";
    if(GetStatus() != Management::Status::Ready)
    {
        std::cout << "Standby - No Valid Wrench Effort.\n";
    }
    else
    {
        JAUS::SetWrenchEffort wrench;
        mPrimitiveDriverMutex.Lock();
        wrench = mCurrentWrenchEffort;
        mPrimitiveDriverMutex.Unlock();
        wrench.PrintMessageBody();
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
///   \param query The query for Wrench Effort.
///   \param[out] report The configured report based on Wrench Effort.
///
////////////////////////////////////////////////////////////////////////////////////
void PrimitiveDriver::CreateReportFromQuery(const QueryWrenchEffort* query,
                                            ReportWrenchEffort& report) const
{
    Mutex::ScopedLock lock(&mPrimitiveDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mCurrentWrenchEffort.GetPresenceVector();
    // Check bit field requested from pv1, then see if we have data for
    // that field in pv2, if so, set the data to report message.
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveLinearEffortX)) > 0) { report.SetPropulsiveLinearEffortX(mCurrentWrenchEffort.GetPropulsiveLinearEffortX()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveLinearEffortY)) > 0) { report.SetPropulsiveLinearEffortY(mCurrentWrenchEffort.GetPropulsiveLinearEffortY()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveLinearEffortZ)) > 0) { report.SetPropulsiveLinearEffortZ(mCurrentWrenchEffort.GetPropulsiveLinearEffortZ()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveRotationalEffortX)) > 0) { report.SetPropulsiveRotationalEffortX(mCurrentWrenchEffort.GetPropulsiveRotationalEffortX()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveRotationalEffortY)) > 0) { report.SetPropulsiveRotationalEffortY(mCurrentWrenchEffort.GetPropulsiveRotationalEffortY()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::PropulsiveRotationalEffortZ)) > 0) { report.SetPropulsiveRotationalEffortZ(mCurrentWrenchEffort.GetPropulsiveRotationalEffortZ()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveLinearEffortX)) > 0) { report.SetResistiveLinearEffortX(mCurrentWrenchEffort.GetResistiveLinearEffortX()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveLinearEffortY)) > 0) { report.SetResistiveLinearEffortY(mCurrentWrenchEffort.GetResistiveLinearEffortY()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveLinearEffortZ)) > 0) { report.SetResistiveLinearEffortZ(mCurrentWrenchEffort.GetResistiveLinearEffortZ()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveRotationalEffortX)) > 0) { report.SetResistiveRotationalEffortX(mCurrentWrenchEffort.GetResistiveRotationalEffortX()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveRotationalEffortY)) > 0) { report.SetResistiveRotationalEffortY(mCurrentWrenchEffort.GetResistiveRotationalEffortY()); }
    if( (pv2 & (pv1 & WrenchEffort::PresenceVector::ResistiveRotationalEffortZ)) > 0) { report.SetResistiveRotationalEffortZ(mCurrentWrenchEffort.GetResistiveRotationalEffortZ()); }
}


/*  End of File */
