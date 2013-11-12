////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatedriver.cpp
///  \brief This file contains the definition of the VelocityStateDriver class, 
///   used as an interface for closed-loop platform mobility.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>Email: dbarber@ist.ucf.edu
///  <br>All rights reserved.
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
#include "igvc/baseline/velocitystatedriver.h"
#include <jaus/core/component.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/mobility/drivers/setwrencheffort.h>
#include <iomanip>
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace IGVC;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::VelocityStateDriver() : mpVelocityStateSensor(NULL),
                                             mpGlobalPoseSensor(NULL),
                                             mStandardsModeFlag(true)
{
    #ifdef SPEED_PID
    mSpeedController.SetBounds(-100.0, 100.0);
    #endif
    mRotationController.SetBounds(-100.0, 100.0);
    mHeadingController.SetBounds(-100.0, 100.0);
    mThrustPower = 0;
    mSteerPower = 0;

    JAUS::SetVelocityCommand defaultCommand;
    defaultCommand.SetCommandType(JAUS::SetVelocityCommand::SetDefaultCommand);
    defaultCommand.SetVelocityX(0.0);
    defaultCommand.SetYawRate(0.0);

    mHeadingThreshold = CxUtils::CxToRadians(90);
    
    mGraphFlag = false;
    
    mMaxVelocityX = 2.56937002; // Theoretical max speed of vehicle in m/s.
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::~VelocityStateDriver()
{
    mDriverThread.StopThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Shutsdown the sensor service.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::Shutdown()
{
    mDriverThread.StopThread();
    JAUS::VelocityStateDriver::Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Load settings from settings/services.xml specific to vehicle dimensions
///          and other things necessary for calculating velocity from encoders.
///          The file is necessary for velocity calculations.
///
///   \param[in] filename Name of XML file containing service specific settings.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::LoadSettings(const std::string& filename)
{
    if (!mXML.Load(filename, "JAUS"))
    {
        std::cout << "Failed loading XML configuration" << std::endl;
        return false;
    }
    double p = 0, i = 0, d = 0;
    #ifdef SPEED_PID
    if(mXML.GetVar("VelocityStateDriver.SpeedController@p", p) && 
       mXML.GetVar("VelocityStateDriver.SpeedController@i", i) && 
       mXML.GetVar("VelocityStateDriver.SpeedController@d", d) )
    {
        mSpeedController.SetGains(p, i, d);
    }
    #endif
    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.RotationController@p", p) && 
       mXML.GetVar("VelocityStateDriver.RotationController@i", i) && 
       mXML.GetVar("VelocityStateDriver.RotationController@d", d) )
    {
        mRotationController.SetGains(p, i, d);
    }
    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.HeadingController@p", p) && 
       mXML.GetVar("VelocityStateDriver.HeadingController@i", i) && 
       mXML.GetVar("VelocityStateDriver.HeadingController@d", d) )
    {
        mHeadingController.SetGains(p, i, d);
    }
    int enable = 0;
    if(mXML.GetVar("VelocityStateDriver.TestVector@graph", enable) &&
        enable == 1)
    {
        mGraphFlag = true;
        // Plot velocity, desired velocty, and thrust.
        mGrapher.Setup("Velocity State Driver", 6, 500, 640, 50);
        mGrapher.Range(0, -4, 4);           // Velocity
        mGrapher.Range(1, -4, 4);           // Desired Velocity
        mGrapher.Range(2, -100, 100);       // Thrust
        
        mGrapher.Range(3, -CxUtils::CX_PI, CxUtils::CX_PI); // Yaw
        mGrapher.Range(4, -CxUtils::CX_PI, CxUtils::CX_PI); // Desired Yaw
        mGrapher.Range(5, -100, 100);                       // Steering
    }
    double thresh = mHeadingThreshold;
    if(mXML.GetVar("VelocityStateDriver.HeadingThreshold", thresh))
    {
        if(fabs(thresh) <= 180)
        {
            mHeadingThreshold = CxUtils::CxToRadians(fabs(thresh));
        }
    }
    mXML.GetVar("VelocityStateDriver.MaxVelocityX", mMaxVelocityX);

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called when command is received by service to implement.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::SetDriveCommand(const JAUS::SetVelocityCommand *command)
{
    JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    mStandardsModeFlag = true;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called when command is received by service to implement.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::SetVectorDriveCommand(const JAUS::SetVelocityVector *command)
{
    mStandardsModeFlag = false;
    return true;
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
bool VelocityStateDriver::GenerateEvent(const JAUS::Events::Subscription& info) const
{
    if(info.mpQueryMessage->GetMessageCode() == JAUS::QUERY_VELOCITY_VECTOR)
    {
        JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
        const JAUS::QueryVelocityVector* query = dynamic_cast<const JAUS::QueryVelocityVector*>(info.mpQueryMessage);

        if(query == NULL)
        {
            return false;
        }

        JAUS::ReportVelocityVector report;
        CreateReportFromQuery(query, report);
        SendEvent(info, &report);
        
        return true;
    }
    return JAUS::VelocityStateDriver::GenerateEvent(info);
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
bool VelocityStateDriver::IsEventSupported(const JAUS::Events::Type type,
                                           const double requestedPeriodicRate,
                                           const JAUS::Message* queryMessage,
                                           double& confirmedPeriodicRate,
                                           std::string& errorMessage) const
{
    if(queryMessage->GetMessageCode() == JAUS::QUERY_VELOCITY_VECTOR)
    {
        confirmedPeriodicRate = requestedPeriodicRate;
        return true;
    }
    return JAUS::VelocityStateDriver::IsEventSupported(type,
                                                       requestedPeriodicRate,
                                                       queryMessage,
                                                       confirmedPeriodicRate,
                                                       errorMessage);
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
void VelocityStateDriver::Receive(const JAUS::Message *message)
{
    switch(message->GetMessageCode())
    {
    case JAUS::QUERY_VELOCITY_VECTOR:
        {
            const JAUS::QueryVelocityVector* query = dynamic_cast<const JAUS::QueryVelocityVector*>(message);
            if(query)
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                JAUS::ReportVelocityVector report;
                CreateReportFromQuery(query, report);
                Send(&report);
            }
        }
        break;
    case JAUS::SET_VELOCITY_VECTOR:
        {
            const JAUS::SetVelocityVector* command = dynamic_cast<const JAUS::SetVelocityVector*>(message);
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                mVelocityVectorCommand = *command;
                mVelocityVectorCommandTime.SetCurrentTime();
            }
            SetVectorDriveCommand(command);
            SignalEvent(JAUS::REPORT_VELOCITY_VECTOR);
        }
        break;
    default:
        JAUS::VelocityStateDriver::Receive(message);
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
JAUS::Message* VelocityStateDriver::CreateMessage(const JAUS::UShort messageCode) const
{
    JAUS::Message* message = NULL;
    switch(messageCode)
    {
    case JAUS::QUERY_VELOCITY_VECTOR:
        message = new JAUS::QueryVelocityVector();
        break;
    case JAUS::REPORT_VELOCITY_VECTOR:
        message = new JAUS::ReportVelocityVector();
        break;
    case JAUS::SET_VELOCITY_VECTOR:
        message = new JAUS::SetVelocityVector();
        break;
    default:
        message = JAUS::VelocityStateDriver::CreateMessage(messageCode);
        break;
    };
    return message;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method called periodically by parent component to update
///          state machine for service.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::CheckServiceStatus(const unsigned int timeSinceLastCheckMs)
{
    if(mDriverThread.IsThreadActive() == false)
    {
        mDriverThread.CreateThread(VelocityStateDriver::DriverThread, this);
        mDriverThread.SetThreadName("Velocity State Driver");
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread method used to generate commands to the motors for
///          closed loop operations.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::DriverThread(void* args)
{
    VelocityStateDriver* driver = (VelocityStateDriver*)args;
    driver->DriverThread();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Thread method used to generate commands to the motors for
///          closed loop operations.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::DriverThread()
{
    bool haveTestVector = false;
    double testVelocityX = 0.0, testYaw = 0.0;
    unsigned int updatePeriodMs = 10;
    JAUS::Time prevVelocityTimestamp;
    unsigned int debounce = 0;
    
    while(mDriverThread.QuitThreadFlag() == false)
    {
        // Set the dt values for our PID controllers
        #ifdef SPEED_PID
        mSpeedController.SetRate(updatePeriodMs/1000.0);
        #endif
        mRotationController.SetRate(updatePeriodMs/1000.0);
        mHeadingController.SetRate(updatePeriodMs/1000.0);
        
        // Check for updated values from XML file for PID controllers.
        static JAUS::Time::Stamp prevLoadTimeMs = 0;
        if(JAUS::Time::GetUtcTimeMs() - prevLoadTimeMs >= 500)
        {
            double p = 0, i = 0, d = 0;
            #ifdef SPEED_PID
            if(mXML.GetVar("VelocityStateDriver.SpeedController@p", p) &&
               mXML.GetVar("VelocityStateDriver.SpeedController@i", i) &&
               mXML.GetVar("VelocityStateDriver.SpeedController@d", d) )
            {
                mSpeedController.SetGains(p, i, d);
            }
            #endif
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.RotationController@p", p) &&
               mXML.GetVar("VelocityStateDriver.RotationController@i", i) &&
               mXML.GetVar("VelocityStateDriver.RotationController@d", d) )
            {
                mRotationController.SetGains(p, i, d);
            }
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.HeadingController@p", p) &&
               mXML.GetVar("VelocityStateDriver.HeadingController@i", i) &&
               mXML.GetVar("VelocityStateDriver.HeadingController@d", d) )
            {
                mHeadingController.SetGains(p, i, d);
            }

            // See if a test vector is being supplied, and if so, set a flag so we
            // try to use it.
            int testing = 0;
            mXML.GetVar("VelocityStateDriver.TestVector@test", testing, true);
            if(testing)
            {
                mXML.GetVar("VelocityStateDriver.TestVector@x", testVelocityX);

                if(mXML.GetVar("VelocityStateDriver.TestVector@yaw", testYaw, true) == false)
                {
                    mStandardsModeFlag = false;
                    if(mXML.GetVar("VelocityStateDriver.TestVector@yaw_rate", testYaw, true))
                    {
                        mStandardsModeFlag = true;
                    }
                }
                testYaw = CxUtils::CxToRadians(testYaw);
                haveTestVector = true;
            }
            prevLoadTimeMs = JAUS::Time::GetUtcTimeMs();
        }
       
        // Get point to velocity data sensor.
        if(mpVelocityStateSensor == NULL)
        {
            mpVelocityStateSensor = (JAUS::VelocityStateSensor *)this->GetComponent()->GetService(JAUS::VelocityStateSensor::Name);
        }

        if(mpVelocityStateSensor == NULL)
        {
            // Must have sensor data.
            return;
        }

        // Lookup primitive driver ID.
        if(mPrimitiveDriverID.IsValid() == false)
        {
            JAUS::Address::List drivers = GetComponent()->DiscoveryService()->GetSubsystem(GetComponentID().mSubsystem)->GetComponentsWithService(JAUS::PrimitiveDriver::Name);
            if(drivers.size() > 0)
            {
                mPrimitiveDriverID = *drivers.begin();
            }
        }
        
        if(mPrimitiveDriverID.IsValid() == false)
        {
            // Need a primitive driver to work.
            return;
        }

        if(GetComponent()->ManagementService()->GetStatus() != JAUS::Management::Status::Ready)
        {
            // Revert to default vector. If we
            // have a test vector, than use the value of
            // standards mode set by it.
            if(haveTestVector == false) 
            {
                mStandardsModeFlag = true;
            }
        }

        // Make sure we have control of the primitive driver service and
        // that it is in a ready state for us to control.
        bool isReady = false;
        if(haveTestVector || GetComponent()->ManagementService()->GetStatus() == JAUS::Management::Status::Ready)
        {
            if(GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID) == false)
            {
                // Request control, send resume.
                if(GetComponent()->AccessControlService()->RequestComponentControl(mPrimitiveDriverID, true))
                {
                    isReady = true;
                }
            }
            // Check for Primitive Driver in ready state.
            if(GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID) &&
                GetComponent()->DiscoveryService()->GetSubsystem(mPrimitiveDriverID)->GetComponent(mPrimitiveDriverID)->mStatus != JAUS::Management::Status::Ready)
            {
                if(GetComponent()->ManagementService()->Resume(mPrimitiveDriverID))
                {
                    isReady = true;
                }
            }
        }
        else
        {
            // Make sure we are not in control of the Primitive Driver component when not
            // in a ready state.
            if(GetComponent()->AccessControlService()->HaveControl(mPrimitiveDriverID))
            {
                GetComponent()->AccessControlService()->ReleaseComponentControl(mPrimitiveDriverID);
            }
        }

        if(isReady || haveTestVector)
        {
            // Setup wrench message to send.
            JAUS::SetWrenchEffort wrench;
            wrench.SetSourceID(GetComponentID());
            wrench.SetDestinationID(mPrimitiveDriverID);
            // Set defaults.
            wrench.SetPropulsiveLinearEffortX(0.0);
            wrench.SetPropulsiveRotationalEffortZ(0.0);

            double error = 0.0;
            // Do speed control, first get desired speed.
            double desiredVelocityX = 0.0;
            if(haveTestVector)
            {
                desiredVelocityX = testVelocityX;
            }
            else if(mStandardsModeFlag) // Set Velocity Command
            {
                desiredVelocityX = GetCurrentDriveCommand().GetVelocityX();
            }
            else                        // Custom message used only by this project.
            {
                desiredVelocityX = GetCurrentVelocityVector().GetVelocityX();
            }

            // Get value to send.

            // Stop if close to 0 m/s.
            if(fabs(desiredVelocityX) < 0.01)
            {
                #ifdef SPEED_PID
                // Use speed value of zero.
                mSpeedController.ClearError();
                #endif
                mThrustPower = 0.0;
            }
            else
            {
                #ifdef SPEED_PID
                error = desiredVelocityX - mpVelocityStateSensor->GetVelocityState().GetVelocityX();
                mThrustPower += mSpeedController.UpdatePID(error);
                if(mThrustPower > 100.0)
                {
                    mThrustPower = 100.0;
                    mSpeedController.Reset();
                }
                if(mThrustPower < -100)
                {
                    mThrustPower = -100.0;
                    mSpeedController.Reset();
                }
                wrench.SetPropulsiveLinearEffortX(mThrustPower);
                #else
                mThrustPower = desiredVelocityX*100.0/mMaxVelocityX;
                if(mThrustPower > 100.0)
                {
                    mThrustPower = 100.0;
                }
                if(mThrustPower < -100)
                {
                    mThrustPower = -100.0;
                }
                wrench.SetPropulsiveLinearEffortX(mThrustPower);
                #endif
            }

            if(mGraphFlag)
            {
                mGrapher.AddValue(0, mpVelocityStateSensor->GetVelocityState().GetVelocityX());
                mGrapher.AddValue(1, desiredVelocityX);
                mGrapher.AddValue(2, wrench.GetPropulsiveLinearEffortX());
            }

            // Do rotational or heading control.

            // Do rotation control
            if(mStandardsModeFlag)
            {
                double currentRotation = mpVelocityStateSensor->GetVelocityState().GetYawRate();
                double desiredRotation = 0.0;
                if(haveTestVector)
                {
                    desiredRotation = testYaw;
                }
                else
                {
                    desiredRotation = this->GetCurrentDriveCommand().GetYawRate();
                }
                error = desiredRotation - currentRotation;
                // This was not adding before!
                mSteerPower += mRotationController.UpdatePID(error);
                if(fabs(desiredRotation) <= 0.001)
                {
                    mSteerPower = 0;
                }
                if(mSteerPower > 100.0)
                {
                    mSteerPower = 100.0;
                    //mRotationController.Reset();
                }
                if(mSteerPower < -100)
                {
                    mSteerPower = -100.0;
                    //mRotationController.Reset();
                }
                if(mGraphFlag)
                {
                    mGrapher.AddValue(3, currentRotation);
                    mGrapher.AddValue(4, desiredRotation);
                    mGrapher.AddValue(5, wrench.GetPropulsiveRotationalEffortZ());
                    mGrapher.Graph();
                }
                wrench.SetPropulsiveRotationalEffortZ(mSteerPower);
            }
            // Do heading control
            else
            {
                if(mpGlobalPoseSensor == NULL)
                {
                    mpGlobalPoseSensor = (JAUS::GlobalPoseSensor *)this->GetComponent()->GetService(JAUS::GlobalPoseSensor::Name);
                }
                if(mpGlobalPoseSensor == NULL)
                {
                    // Must have sensor data.
                    std::cout << "VelocityStateDriver::ERROR No Global Pose Sensor (is everything enabled?).\n";
                    continue;
                }
                double currentYaw = mpGlobalPoseSensor->GetGlobalPose().GetYaw();
                double desiredYaw = currentYaw;
                if(haveTestVector)
                {
                    desiredYaw = testYaw;
                }
                else
                {
                    desiredYaw = GetCurrentVelocityVector().GetYaw();
                }

                error = CxUtils::Orientation::AngleDiff(currentYaw, desiredYaw, true);
                double steering = mHeadingController.UpdatePID(error);
                wrench.SetPropulsiveRotationalEffortZ(steering);

                // If we are outside the allowed error, then
                // stop driving until we are within range.
                if(fabs(error) > mHeadingThreshold)
                {
                    wrench.SetPropulsiveLinearEffortX(0);
                }

                if(mGraphFlag)
                {
                    mGrapher.AddValue(3, currentYaw);
                    mGrapher.AddValue(4, desiredYaw);
                    mGrapher.AddValue(5, wrench.GetPropulsiveRotationalEffortZ());
                    mGrapher.Graph();
                }
            }
            // Send the resulting message.
            Send(&wrench);
        }
        else
        {
            mThrustPower = mSteerPower = 0;
#ifdef SPEED_PID
            mSpeedController.ClearError();
#endif
            mHeadingController.ClearError();
            mRotationController.ClearError();
        }
        CxUtils::SleepMs(updatePeriodMs);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints information to console.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::PrintStatus() const
{
    if(mStandardsModeFlag)
    {
        JAUS::VelocityStateDriver::PrintStatus();
    }
    else
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Vector Command:\n";
        JAUS::SetVelocityVector command;
        if(GetStatus() != JAUS::Management::Status::Ready)
        {
            std::cout << "Standby - Using Default Command.\n";
        }
        else
        {
            JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
            command.PrintMessageBody();
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a report message based on the query.
///
////////////////////////////////////////////////////////////////////////////////////
void VelocityStateDriver::CreateReportFromQuery(const JAUS::QueryVelocityVector* query,
                                                JAUS::ReportVelocityVector& report) const
{
    JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    report.ClearMessage();
    report.SetDestinationID(query->GetSourceID());
    report.SetSourceID(GetComponentID());
    JAUS::UInt pv1 = query->GetPresenceVector();
    JAUS::UInt pv2 = mVelocityVectorCommand.GetPresenceVector();
   
    // Check bit field requested from pv1, then see if we have data for
    // that field in pv2, if so, set the data to report message.
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::VelocityX)) > 0) { report.SetVelocityX(mVelocityVectorCommand.GetVelocityX()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::VelocityY)) > 0) { report.SetVelocityY(mVelocityVectorCommand.GetVelocityY()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::VelocityZ)) > 0) { report.SetVelocityZ(mVelocityVectorCommand.GetVelocityZ()); }
    // Apparently this is not supposed to be here
    //if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::VelocityRMS)) > 0) { report.SetVelocityRMS(mVelocityVectorCommand.GetVelocityRMS()); }
    
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Roll)) > 0) { report.SetRoll(mVelocityVectorCommand.GetRoll()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Pitch)) > 0) { report.SetPitch(mVelocityVectorCommand.GetPitch()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Yaw)) > 0) { report.SetYaw(mVelocityVectorCommand.GetYaw()); }

    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::TimeStamp)) > 0) { report.SetTimeStamp(mVelocityVectorCommand.GetTimeStamp()); }
}


/*  End of File */
