////////////////////////////////////////////////////////////////////////////////////
///
///  \file listdriver.cpp
///  \brief This file contains the implementation of the List Driver class for
///         creating services that execute lists.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 August 2010
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
#include "jaus/mobility/drivers/listdriver.h"
#include "jaus/core/component.h"

using namespace JAUS;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] serviceIdentifier The name of your service.
///   \param[in] parentServiceIdentifier The parent service (ListDriver).
///
////////////////////////////////////////////////////////////////////////////////////
ListDriver::ListDriver(const Service::ID& serviceIdentifier,
                       const Service::ID& parentServiceIdentifier) : ListManager::Child(serviceIdentifier,
                                                                                        parentServiceIdentifier)
{
    mSpeedMetersPerSecond = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ListDriver::~ListDriver()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the desired travel speed.
///
///   \param[in] speedMetersPerSecond Desired travel speed.
///
////////////////////////////////////////////////////////////////////////////////////
void ListDriver::SetExecutionSpeed(const double speedMetersPerSecond)
{
    Mutex::ScopedLock lock(&mListDriverMutex);
    mSpeedMetersPerSecond = speedMetersPerSecond;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to check if an execute command has been received, and the
///          driver should be driving to waypoints.
///
///   \return True if Execute command received and we need to keep driving, false
///           otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ListDriver::IsExecuting() const
{
    if(GetComponent()->AccessControlService()->IsControlled())
    {
        if(GetComponent()->ManagementService()->GetStatus() == Management::Status::Ready)
        {
            return GetActiveListElementID() != 0 ? true : false;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Processes message received by the Service.  If not supported, then
///          message is passed to inheriting services depending on what
///          type of control has been established for the component.
///
///   This Service supports Management related messages only.
///
///   \param[in] message Message data to process.
///
////////////////////////////////////////////////////////////////////////////////////
void ListDriver::Receive(const Message* message)
{
    switch(message->GetMessageCode())
    {
    case QUERY_TRAVEL_SPEED:
        {
            Mutex::ScopedLock lock(&mListDriverMutex);
            ReportTravelSpeed report(message->GetSourceID(), GetComponentID());;
            report.SetSpeed(mSpeedMetersPerSecond);
            Send(&report);
        }
        break;
    case EXECUTE_LIST:
        {
            const JAUS::ExecuteList* command = dynamic_cast<const JAUS::ExecuteList*>(message);
            if(command)
            {
                if(command->IsFieldPresent(JAUS::ExecuteList::PresenceVector::ElementUID))
                {
                    SetActiveListElement(command->GetElementUID());
                    ExecuteList(command->GetSpeed());
                }
                else
                {
                    SetExecutionSpeed(command->GetSpeed());
                }
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
Message* ListDriver::CreateMessage(const UShort messageCode) const
{
    switch(messageCode)
    {
    case SET_TRAVEL_SPEED:
        return new JAUS::SetTravelSpeed();
        break;
    case QUERY_TRAVEL_SPEED:
        return new QueryTravelSpeed();
        break;
    case REPORT_TRAVEL_SPEED:
        return new ReportTravelSpeed();
        break;
    case EXECUTE_LIST:
        return new JAUS::ExecuteList();
        break;
    default:
        return NULL;
        break;
    }
    return NULL;
}


/*  End of File */
