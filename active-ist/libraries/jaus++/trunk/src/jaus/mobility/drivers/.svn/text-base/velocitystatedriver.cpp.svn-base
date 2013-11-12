////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatedriver.cpp
///  \brief This file contains the implementation of the VelocityStateDriver class.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: March 23 2011
///  <br>Copyright (c) 2011
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
#include "jaus/mobility/drivers/velocitystatedriver.h"
#include "jaus/core/component.h"

using namespace JAUS;

const std::string VelocityStateDriver::Name = "urn:jaus:jss:mobility:VelocityStateDriver";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::VelocityStateDriver() : Management::Child(Service::ID(VelocityStateDriver::Name), 
                                                               Service::ID(Management::Name))
{
    // Set default values.
    mDefaultVelocityCommand.SetVelocityX(0);
    mDefaultVelocityCommand.SetVelocityY(0);
    mDefaultVelocityCommand.SetVelocityZ(0);
    mDefaultVelocityCommand.SetRollRate(0);
    mDefaultVelocityCommand.SetPitchRate(0);
    mDefaultVelocityCommand.SetYawRate(0);
    mVelocityCommand = mDefaultVelocityCommand;

    mMaxVelocityCommand.SetVelocityX(SetVelocityCommand::Limits::MaxVelocity);
    mMaxVelocityCommand.SetVelocityY(SetVelocityCommand::Limits::MaxVelocity);
    mMaxVelocityCommand.SetVelocityZ(SetVelocityCommand::Limits::MaxVelocity);
    mMaxVelocityCommand.SetRollRate(SetVelocityCommand::Limits::MaxRotationalRate);
    mMaxVelocityCommand.SetPitchRate(SetVelocityCommand::Limits::MaxRotationalRate);
    mMaxVelocityCommand.SetYawRate(SetVelocityCommand::Limits::MaxRotationalRate);
    /* No presence vector means no min
    mMinVelocityCommand.SetVelocityX(SetVelocityCommand::Limits::MinVelocity);
    mMinVelocityCommand.SetVelocityY(SetVelocityCommand::Limits::MinVelocity);
    mMinVelocityCommand.SetVelocityZ(SetVelocityCommand::Limits::MinVelocity);
    mMinVelocityCommand.SetRollRate(SetVelocityCommand::Limits::MinRotationalRate);
    mMinVelocityCommand.SetPitchRate(SetVelocityCommand::Limits::MinRotationalRate);
    mMinVelocityCommand.SetYawRate(SetVelocityCommand::Limits::MinRotationalRate);
    */
    mMaxAcceleration.SetAccelerationX(SetAccelerationLimit::Limits::MaxLinearAcceleration);
    mMaxAcceleration.SetAccelerationY(SetAccelerationLimit::Limits::MaxLinearAcceleration);
    mMaxAcceleration.SetAccelerationZ(SetAccelerationLimit::Limits::MaxLinearAcceleration);
    mMaxAcceleration.SetRollAcceleration(SetAccelerationLimit::Limits::MaxRotationalAcceleration);
    mMaxAcceleration.SetPitchAcceleration(SetAccelerationLimit::Limits::MaxRotationalAcceleration);
    mMaxAcceleration.SetYawAcceleration(SetAccelerationLimit::Limits::MaxRotationalAcceleration);
    /* No presence vector means no min
    mMinAcceleration.SetAccelerationX(SetAccelerationLimit::Limits::MinLinearAcceleration);
    mMinAcceleration.SetAccelerationY(SetAccelerationLimit::Limits::MinLinearAcceleration);
    mMinAcceleration.SetAccelerationZ(SetAccelerationLimit::Limits::MinLinearAcceleration);
    mMinAcceleration.SetRollAcceleration(SetAccelerationLimit::Limits::MinRotationalAcceleration);
    mMinAcceleration.SetPitchAcceleration(SetAccelerationLimit::Limits::MinRotationalAcceleration);
    mMinAcceleration.SetYawAcceleration(SetAccelerationLimit::Limits::MinRotationalAcceleration);
    */
    // Set command type information.
    mDefaultVelocityCommand.SetCommandType(SetVelocityCommand::SetDefaultCommand);
    mVelocityCommand.SetCommandType(SetVelocityCommand::SetCurrentCommand);
    mMinVelocityCommand.SetCommandType(SetVelocityCommand::SetMinimumAllowedValues);
    mMaxVelocityCommand.SetCommandType(SetVelocityCommand::SetMaximumAllowedValues);
    mMinAcceleration.SetCommandType(SetAccelerationLimit::SetMinimumAllowedValues);
    mMaxAcceleration.SetCommandType(SetAccelerationLimit::SetMaximumAllowedValues);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::~VelocityStateDriver()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the acceleration limit.
///
///   \param[in] command Command to set acceleration limits.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::SetAccelerationLimit(const JAUS::SetAccelerationLimit* command)
{
    {
        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        if(command->GetCommandType() == JAUS::SetAccelerationLimit::SetMaximumAllowedValues)
        {
            mMaxAcceleration = *command;
        }
        else
        {
            mMinAcceleration = *command;
        }
    }
    // Signal change in data.
    this->SignalEvent(REPORT_ACCELERATION_LIMIT);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the current drive command received.
///
///   If the component is not in ready state, than the value of the default
///   velocity command is returned, otherwise the current command received
///   by controlling component is taken.
///
///   \return The current drive command last received.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand VelocityStateDriver::GetCurrentDriveCommand() const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    if(this->GetStatus() != JAUS::Management::Status::Ready)
    {
        return mDefaultVelocityCommand;
    }
    return mVelocityCommand;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns velocity command limits.
///
///   \param[in] max If true maximum limits are returned.
///
////////////////////////////////////////////////////////////////////////////////////
SetVelocityCommand VelocityStateDriver::GetDriveCommandLimits(const bool max) const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    if(max)
    {
        return mMaxVelocityCommand;
    }

    return mMinVelocityCommand;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param[in] max If true, max values returned, otherwise minimum.
///
///   \return The acceleration limit data.
///
////////////////////////////////////////////////////////////////////////////////////
SetAccelerationLimit VelocityStateDriver::GetAccelerationLimit(const bool max) const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    if(max)
    {
        return mMaxAcceleration;
    }

    return mMinAcceleration;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The time (UTC seconds) that a drive command was received.
///
////////////////////////////////////////////////////////////////////////////////////
Time VelocityStateDriver::GetDriveCommandTime() const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    return mVelocityCommandTime;
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
bool VelocityStateDriver::GenerateEvent(const Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == QUERY_VELOCITY_COMMAND)
    {
        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        const QueryVelocityCommand* query = dynamic_cast<const QueryVelocityCommand*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportVelocityCommand report;
        CreateReportFromQuery(query, report);
        SendEvent(info, &report);

        return true;
    }
    else if(info.mpQueryMessage->GetMessageCode() == QUERY_ACCELERATION_LIMIT)
    {
        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        const QueryAccelerationLimit* query = dynamic_cast<const QueryAccelerationLimit*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        ReportAccelerationLimit report;
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
bool VelocityStateDriver::IsEventSupported(const Events::Type type,
                                           const double requestedPeriodicRate,
                                           const Message* queryMessage,
                                           double& confirmedPeriodicRate,
                                           std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == QUERY_VELOCITY_COMMAND ||
       queryMessage->GetMessageCode() == QUERY_ACCELERATION_LIMIT)
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
///   This Service supports VelocityStateDriver related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::Receive(const Message *message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_VELOCITY_COMMAND:
        {
            const JAUS::QueryVelocityCommand* query = dynamic_cast<const JAUS::QueryVelocityCommand*>(message);
            if(query)
            {
                Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                ReportVelocityCommand report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case QUERY_ACCELERATION_LIMIT:
        {
            const JAUS::QueryAccelerationLimit* query = dynamic_cast<const JAUS::QueryAccelerationLimit*>(message);
            if(query)
            {
                Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                ReportAccelerationLimit report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case SET_VELOCITY_COMMAND:
        {
            const JAUS::SetVelocityCommand* command = dynamic_cast<const JAUS::SetVelocityCommand*>(message);
            if(command)
            {
                switch(command->GetCommandType())
                {
                case SetVelocityCommand::SetCurrentCommand:
                    if(GetComponent()->ManagementService()->GetStatus() == Management::Status::Ready)
                    {
                        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                        mVelocityCommand = *command;
                        mVelocityCommandTime.SetCurrentTime();
                    }
                    break;
                case SetVelocityCommand::SetDefaultCommand:
                    {
                        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                        mDefaultVelocityCommand = *command;
                    }
                    break;
                case SetVelocityCommand::SetMaximumAllowedValues:
                    {
                        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                        mMaxVelocityCommand = *command;
                    }
                    break;
                default:
                    {
                        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                        mMinVelocityCommand = *command;
                    }
                    break;
                }
                SetVelocityCommand(command);
                SignalEvent(REPORT_VELOCITY_COMMAND);
            }
        }
        break;
    case SET_ACCELERATION_LIMIT:
        {
            const JAUS::SetAccelerationLimit* command = dynamic_cast<const JAUS::SetAccelerationLimit*>(message);
            if(command)
            {
                SetAccelerationLimit(command);
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
Message* VelocityStateDriver::CreateMessage(const UShort messageCode) const
{
    Message* message = NULL;
    switch(messageCode)
    {
    case QUERY_VELOCITY_COMMAND:
        message = new JAUS::QueryVelocityCommand();
        break;
    case QUERY_ACCELERATION_LIMIT:
        message = new JAUS::QueryAccelerationLimit();
        break;
    case REPORT_VELOCITY_COMMAND:
        message = new JAUS::ReportVelocityCommand();
        break;
    case REPORT_ACCELERATION_LIMIT:
        message = new JAUS::ReportAccelerationLimit();
        break;
    case SET_VELOCITY_COMMAND:
        message = new JAUS::SetVelocityCommand();
        break;
    case SET_ACCELERATION_LIMIT:
        message = new JAUS::SetAccelerationLimit();
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
void VelocityStateDriver::PrintStatus() const
{
    std::cout << "[" << GetServiceID().ToString() << "] - Current Velocity Command:\n";
    JAUS::SetVelocityCommand command;
    
    if(GetStatus() != Management::Status::Ready)
    {
        std::cout << "Standby - Using Default Command.\n";
        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        command = mDefaultVelocityCommand;
    }
    else
    {
        Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        command = mVelocityCommand;
    }
    command.PrintMessageBody();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::CreateReportFromQuery(const QueryVelocityCommand* query,
                                                ReportVelocityCommand& report) const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    report.SetCommandType(query->GetCommandType());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mVelocityCommand.GetPresenceVector();
    const JAUS::SetVelocityCommand* command;
    switch(query->GetCommandType())
    {
    case QueryVelocityCommand::SetCurrentCommand:
        command = &mVelocityCommand;
        break;
    case QueryVelocityCommand::SetMaximumAllowedValues:
        command = &mMaxVelocityCommand;
        break;
    case QueryVelocityCommand::SetMinimumAllowedValues:
        command = &mMinVelocityCommand;
        break;
    default:
        command = &mDefaultVelocityCommand;
        break;
    }
    // Check bit field requested from pv1, then see if we have data for
    // that field in pv2, if so, set the data to report message.
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::VelocityX)) > 0) { report.SetVelocityX(command->GetVelocityX()); }
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::VelocityY)) > 0) { report.SetVelocityY(command->GetVelocityY()); }
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::VelocityZ)) > 0) { report.SetVelocityZ(command->GetVelocityZ()); }
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::RollRate)) > 0) { report.SetRollRate(command->GetRollRate()); }
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::PitchRate)) > 0) { report.SetPitchRate(command->GetPitchRate()); }
    if( (pv2 & (pv1 & QueryVelocityCommand::PresenceVector::YawRate)) > 0) { report.SetYawRate(command->GetYawRate()); }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::CreateReportFromQuery(const QueryAccelerationLimit* query,
                                                ReportAccelerationLimit& report) const
{
    Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    report.SetCommandType(query->GetCommandType());
    UInt pv1 = query->GetPresenceVector();
    UInt pv2 = mVelocityCommand.GetPresenceVector();
    const JAUS::SetAccelerationLimit* command;
    switch(query->GetCommandType())
    {
    case QueryAccelerationLimit::SetMaximumAllowedValues:
        command = &mMaxAcceleration;
        break;
    default:
        command = &mMinAcceleration;
        break;
    }
    // Check bit field requested from pv1, then see if we have data for
    // that field in pv2, if so, set the data to report message.
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::AccelerationX)) > 0) { report.SetAccelerationX(command->GetAccelerationX()); }
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::AccelerationY)) > 0) { report.SetAccelerationY(command->GetAccelerationY()); }
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::AccelerationZ)) > 0) { report.SetAccelerationZ(command->GetAccelerationZ()); }
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::RollAcceleration)) > 0) { report.SetRollAcceleration(command->GetRollAcceleration()); }
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::PitchAcceleration)) > 0) { report.SetPitchAcceleration(command->GetPitchAcceleration()); }
    if( (pv2 & (pv1 & QueryAccelerationLimit::PresenceVector::YawAcceleration)) > 0) { report.SetYawAcceleration(command->GetYawAcceleration()); }

}


/*  End of File */
