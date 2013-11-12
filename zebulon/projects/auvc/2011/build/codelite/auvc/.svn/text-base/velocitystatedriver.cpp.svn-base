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
#include "auvc/baseline/velocitystatedriver.h"
#include <jaus/core/component.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/mobility/drivers/setwrencheffort.h>
#include <iomanip>
#include <cxutils/math/cxmath.h>
#include "auvc/baseline/globalposesensor.h"

using namespace Zebulon;
using namespace Underwater;

#define HEADINGHACK

//#define GRAPHAXIAL
//#define GRAPHLATERAL
//#define GRAPHVERTICAL
//#define GRAPHYAW
//#define GRAPHPITCH
//#define GRAPHROLL


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::VelocityStateDriver() : mpVelocityStateSensor(NULL),
                                             mpGlobalPoseSensor(NULL)
{
    
    // Initialize PID controllers
    mYawController.SetBounds(-100.0, 100.0);
    mPitchController.SetBounds(-100.0, 100.0);
    mRollController.SetBounds(-100.0, 100.0);
    mVerticalController.SetBounds(-100.0, 100.0);
    
    mAxialPower = 0;
    mLateralPower = 0;
    mVerticalPower = 0;
    mYawPower = 0;
    mPitchPower = 0;
    mRollPower = 0;
    
    mDesiredYaw = 0;
    
    JAUS::SetVelocityCommand defaultCommand;
    defaultCommand.SetCommandType(JAUS::SetVelocityCommand::SetDefaultCommand);
    defaultCommand.SetVelocityX(0.0);
    defaultCommand.SetYawRate(0.0);

    mYawThreshold = CxUtils::CxToRadians(90);
    
    mGraphFlag = false;
    
    mMaxVelocityX = 2.56937002; // Theoretical max speed of vehicle in m/s.
    mMaxVelocityY = 2.56937002; // Theoretical max speed of vehicle in m/s.
    
    // low pass filter 
    //mpYawFilter = new Utility::LowPassFilter(1/20.0, 5.0);
    //mpYawFilter->Setup(-M_PI, M_PI);
    
    mReceivedCommand = false;
    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
VelocityStateDriver::~VelocityStateDriver()
{
    mDriverThread.StopThread();
    //delete mpYawFilter;
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
    // services.xml
    
    
    if (!mXML.Load(filename, "JAUS"))
    {
        std::cout << "Failed loading XML configuration" << std::endl;
        return false;
    }
    double p = 0, i = 0, d = 0;

    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.VerticalController@p", p) && 
       mXML.GetVar("VelocityStateDriver.VerticalController@i", i) && 
       mXML.GetVar("VelocityStateDriver.VerticalController@d", d) )
    {
        mVerticalController.SetGains(p, i, d);
    }
    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.YawController@p", p) && 
       mXML.GetVar("VelocityStateDriver.YawController@i", i) && 
       mXML.GetVar("VelocityStateDriver.YawController@d", d) )
    {
        mYawController.SetGains(p, i, d);
    }
    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.PitchController@p", p) && 
       mXML.GetVar("VelocityStateDriver.PitchController@i", i) && 
       mXML.GetVar("VelocityStateDriver.PitchController@d", d) )
    {
        mPitchController.SetGains(p, i, d);
    }
    p = i = d = 0.0;
    if(mXML.GetVar("VelocityStateDriver.RollController@p", p) && 
       mXML.GetVar("VelocityStateDriver.RollController@i", i) && 
       mXML.GetVar("VelocityStateDriver.RollController@d", d) )
    {
        mRollController.SetGains(p, i, d);
    }

    int enable = 0;
   
    double thresh = mYawThreshold;
    if(mXML.GetVar("VelocityStateDriver.YawThreshold", thresh))
    {
        if(fabs(thresh) <= 180)
        {
            mYawThreshold = CxUtils::CxToRadians(fabs(thresh));
        }
    }
    
    mXML.GetVar("VelocityStateDriver.MaxVelocityX", mMaxVelocityX);
    mXML.GetVar("VelocityStateDriver.MaxVelocityY", mMaxVelocityY);
    
    return true;
}


/*////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called when command is received by service to implement.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::SetDriveCommand(const JAUS::SetVelocityCommand *command)
{
    JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
    mStandardsModeFlag = true;
    return true;
}*/


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Called when command is received by service to implement.
///
////////////////////////////////////////////////////////////////////////////////////
bool VelocityStateDriver::SetVectorDriveCommand(const JAUS::SetVelocityVector *command)
{
    JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
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
            mReceivedCommand=true;
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
    unsigned int updatePeriodMs = 10;
    JAUS::Time prevVelocityTimestamp;
    unsigned int debounce = 0;
    
    while(mDriverThread.QuitThreadFlag() == false)
    {
        // Set the dt values for our PID controllers
        mYawController.SetRate(updatePeriodMs/1000.0);
        mPitchController.SetRate(updatePeriodMs/1000.0);
        mRollController.SetRate(updatePeriodMs/1000.0);
        mVerticalController.SetRate(updatePeriodMs/1000.0);
        
        // Check for updated values from XML file for PID controllers.
        static JAUS::Time::Stamp prevLoadTimeMs = 0;
        if(JAUS::Time::GetUtcTimeMs() - prevLoadTimeMs >= 500)
        {
            double p = 0, i = 0, d = 0;
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.VerticalController@p", p) && 
               mXML.GetVar("VelocityStateDriver.VerticalController@i", i) && 
               mXML.GetVar("VelocityStateDriver.VerticalController@d", d) )
            {
                mVerticalController.SetGains(p, i, d);
            }
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.YawController@p", p) && 
               mXML.GetVar("VelocityStateDriver.YawController@i", i) && 
               mXML.GetVar("VelocityStateDriver.YawController@d", d) )
            {
                mYawController.SetGains(p, i, d);
            }
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.PitchController@p", p) && 
               mXML.GetVar("VelocityStateDriver.PitchController@i", i) && 
               mXML.GetVar("VelocityStateDriver.PitchController@d", d) )
            {
                mPitchController.SetGains(p, i, d);
            }
            p = i = d = 0.0;
            if(mXML.GetVar("VelocityStateDriver.RollController@p", p) && 
               mXML.GetVar("VelocityStateDriver.RollController@i", i) && 
               mXML.GetVar("VelocityStateDriver.RollController@d", d) )
            {
                mRollController.SetGains(p, i, d);
            }
            
            double thresh = mYawThreshold;
            if(mXML.GetVar("VelocityStateDriver.YawThreshold", thresh))
            {
                if(fabs(thresh) <= 180)
                {
                    mYawThreshold = CxUtils::CxToRadians(fabs(thresh));
                }
            }
            
            prevLoadTimeMs = JAUS::Time::GetUtcTimeMs();
        }
       
        // Get pointer to velocity data sensor.
        /*if(mpVelocityStateSensor == NULL)
        {
            mpVelocityStateSensor = (JAUS::VelocityStateSensor *)this->GetComponent()->GetService(JAUS::VelocityStateSensor::Name);
        }

        if(mpVelocityStateSensor == NULL)
        {
            // Must have sensor data.
            std::cout << "No Valid VelocityStateSensor\n";
            return;
        }*/
        
        // Get pointer to primitive driver.
        if(mpPrimitiveDriver == NULL)
        {
            mpPrimitiveDriver = (JAUS::PrimitiveDriver *)this->GetComponent()->GetService(JAUS::PrimitiveDriver::Name);
        }

        if(mpPrimitiveDriver == NULL)
        {
            // Must have sensor data.
            std::cout << "No Valid Primitive Driver\n";
            return;
        }

        /*// Make sure we have control of the primitive driver service and
        // that it is in a ready state for us to control.
        //bool isReady = false;*/
        bool isReady = false;
        
        // Setup wrench message to send.-35.1
        JAUS::SetWrenchEffort wrench;
        
        // Set defaults.
        wrench.SetPropulsiveLinearEffortX(0.0);         // Forward thrust
        wrench.SetPropulsiveLinearEffortY(0.0);         // Lateral thrust
        wrench.SetPropulsiveLinearEffortZ(0.0);         // Vertical thrust
        
        wrench.SetPropulsiveRotationalEffortX(0.0);     // Rotational thrust
        wrench.SetPropulsiveRotationalEffortY(0.0);     // Rotational thrust
        wrench.SetPropulsiveRotationalEffortZ(0.0);     // Rotational thrust
        
        if(GetStatus() == JAUS::Management::Status::Ready && mReceivedCommand==true)
        {
            //std::cout << "I'm READYYYYYYYYYYYYYYYYYYYYYYYY" << std::endl;
            isReady = true;
            double error = 0.0;
            
            // Do speed control, first get desired speed.
            //double desiredVelocityX = 0.0;
            //double desiredVelocityY = 0.0;
            
            double desiredAxialPower = 0.0;
            double desiredLateralPower = 0.0;
            
            // Custom message used only by this project.
            desiredAxialPower = GetCurrentVelocityVector().GetVelocityX();
            desiredLateralPower = GetCurrentVelocityVector().GetVelocityY();
            // Get value to send.
            
            //desiredAxialPower = 50;
            
            // Stop if close to 0 m/s.
            if(fabs(desiredAxialPower) < 1)
            {
                mAxialPower = 0.0;
            }
            else
            {
                mAxialPower = desiredAxialPower;
                
                if(desiredAxialPower > 100.0)
                {
                    mAxialPower = 100.0;
                }
                if(desiredAxialPower < -100)
                {
                    mAxialPower = -100.0;
                }
            }
            
            // Do Y also for lateral.
            if(fabs(desiredLateralPower) < 1)
            {
                mLateralPower = 0.0;
            }
            else
            {
                mLateralPower = desiredLateralPower;
                
                if(desiredLateralPower > 100.0)
                {
                    mLateralPower = 100.0;
                }
                if(desiredLateralPower < -100)
                {
                    mLateralPower = -100.0;
                }
            }

            if(mGraphFlag)
            {
#ifdef GRAPHAXIAL
                mGrapher.AddValue(0, GetCurrentVelocityVector().GetVelocityX());
                mGrapher.AddValue(1, mAxialPower);
#endif
#ifdef GRAPHLATERAL
                mGrapher.AddValue(0, GetCurrentVelocityVector().GetVelocityY());
                mGrapher.AddValue(1, mLateralPower);
#endif
            }
            
            wrench.SetPropulsiveLinearEffortX(mAxialPower);
            wrench.SetPropulsiveLinearEffortY(mLateralPower);

            // Do rotational and depth control
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
                
                double desiredVerticalPosition = 0.0;
                double desiredYaw = 0.0;
                double desiredPitch = 0.0;
                double desiredRoll = 0.0;
                
                double currentVerticalPosition = 0.0;
                double currentYaw = 0.0;
                double currentPitch = 0.0;
                double currentRoll = 0.0;
                
                desiredVerticalPosition = GetCurrentVelocityVector().GetPositionZ();
                desiredYaw = GetCurrentVelocityVector().GetYaw();
                desiredPitch = GetCurrentVelocityVector().GetPitch();
                desiredRoll = GetCurrentVelocityVector().GetRoll();
                
                currentVerticalPosition = mpGlobalPoseSensor->GetGlobalPose().GetAltitude();
                currentYaw = mpGlobalPoseSensor->GetGlobalPose().GetYaw();
                currentPitch = mpGlobalPoseSensor->GetGlobalPose().GetPitch();
                currentRoll = mpGlobalPoseSensor->GetGlobalPose().GetRoll();
                
                // Vertical Control
                //desiredVerticalPosition = 20/39.37;
                error = desiredVerticalPosition - currentVerticalPosition;
                double verticalControllerOutput = mVerticalController.UpdatePID(error);
                //verticalControllerOutput = 0;
                wrench.SetPropulsiveLinearEffortZ(verticalControllerOutput);
                
                // Pitch Control
                //desiredPitch = 0;
                error = desiredPitch - currentPitch;
                double pitchControllerOutput = mPitchController.UpdatePID(error);
                //pitchControllerOutput = 0;
                wrench.SetPropulsiveRotationalEffortY(pitchControllerOutput);
                
                // Roll Control
                //desiredRoll = 0;
                error = desiredRoll - currentRoll;
                double rollControllerOutput = mRollController.UpdatePID(error);
                //rollControllerOutput = 0;
                wrench.SetPropulsiveRotationalEffortX(rollControllerOutput);
                
                // Yaw Control
                // Add first order rc filter to yaw
                /*mpYawFilter->Add(GetCurrentVelocityVector().GetYaw());
                mpYawFilter->Output(desiredYaw);
                mDesiredYaw = desiredYaw;*/
                
                error = CxUtils::Orientation::AngleDiff(currentYaw, desiredYaw, true);
                double yawControllerOutput = mYawController.UpdatePID(error);
                //yawControllerOutput = 0;
                wrench.SetPropulsiveRotationalEffortZ(yawControllerOutput);

                // If we are outside the allowed error, then
                // stop driving until we are within range.
                /*if(fabs(error) > mYawThreshold)
                {
                    wrench.SetPropulsiveLinearEffortX(0);
                    wrench.SetPropulsiveLinearEffortY(0);
                }*/
                
                if(mGraphFlag)
                {
#ifdef GRAPHVERTICAL
                    mGrapher.AddValue(0, currentVerticalPosition);
                    mGrapher.AddValue(1, desiredVerticalPosition);
                    mGrapher.AddValue(2, wrench.SetPropulsiveLinearEffortZ());
#endif
#ifdef GRAPHYAW
                    mGrapher.AddValue(0, currentYaw);
                    mGrapher.AddValue(1, desiredYaw);
                    mGrapher.AddValue(2, wrench.GetPropulsiveRotationalEffortZ());
#endif
#ifdef GRAPHPITCH
                    mGrapher.AddValue(0, currentPitch);
                    mGrapher.AddValue(1, desiredPitch);
                    mGrapher.AddValue(2, wrench.GetPropulsiveRotationalEffortY());
#endif
#ifdef GRAPHROLL
                    mGrapher.AddValue(0, currentRoll);
                    mGrapher.AddValue(1, desiredRoll);
                    mGrapher.AddValue(2, wrench.GetPropulsiveRotationalEffortX());
#endif
                    mGrapher.Graph();
                }
            }
        }
        
        if(GetStatus() != JAUS::Management::Status::Ready)
        {
            mReceivedCommand = false;
        }
        

        if(isReady)
        {
            // Send wrench effort command.
            //Send(&wrench);
            //Heading hack
#ifdef HEADINGHACK
            Underwater::GlobalPoseSensor *gp=(Underwater::GlobalPoseSensor *)mpGlobalPoseSensor;
            //gp->SetLiveOffset(5.0*M_PI/180.0*mAxialPower/100.0);
            //try to adjust for motor power offset by thrust
            double currentYaw = mpGlobalPoseSensor->GetGlobalPose().GetYaw();
            double mag=10.0/180.0*M_PI;
            double offset=1.6396;
            double fix=mag*mAxialPower/100.0*sin(currentYaw+offset);

            gp->SetLiveOffset(fix);
#endif
            // us
            ((Underwater::PrimitiveDriver*)mpPrimitiveDriver)->ForceVectorReady(true);
            mpPrimitiveDriver->SetWrenchEffort(&wrench);
        }
        else
        {
            ((Underwater::PrimitiveDriver*)mpPrimitiveDriver)->ForceVectorReady(false);
            mpPrimitiveDriver->SetWrenchEffort(&wrench);
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
    
    {
        std::cout << "[" << GetServiceID().ToString() << "] - Current Vector Command:\n";
        if(GetStatus() != JAUS::Management::Status::Ready)
        {
            std::cout << "Standby - Using Default Command.\n";
        }
        else
        {
            JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
            // only shows the command you recieved
            if(mReceivedCommand)
            {
                mVelocityVectorCommand.PrintMessageBody();
            }
            else
            {
                std::cout << "Standby for Primitive Direct Drive\n";
            }
            //std::cout << "Filtered Yaw " << CxUtils::CxToDegrees(mDesiredYaw) << std::endl;
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
    
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Roll)) > 0) { report.SetRoll(mVelocityVectorCommand.GetRoll()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Pitch)) > 0) { report.SetPitch(mVelocityVectorCommand.GetPitch()); }
    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::Yaw)) > 0) { report.SetYaw(mVelocityVectorCommand.GetYaw()); }

    if( (pv2 & (pv1 & JAUS::QueryVelocityVector::PresenceVector::TimeStamp)) > 0) { report.SetTimeStamp(mVelocityVectorCommand.GetTimeStamp()); }
}


/*  End of File */
