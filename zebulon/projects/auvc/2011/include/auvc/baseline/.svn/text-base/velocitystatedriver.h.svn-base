////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatedriver.h
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
#ifndef __ZEBULON_UNDERWATER_VELOCITY_VECTOR_DRIVER_H
#define __ZEBULON_UNDERWATER_VELOCITY_VECTOR_DRIVER_H

#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/drivers/velocitystatedriver.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/extras/mobility/queryvelocityvector.h>
#include <jaus/extras/mobility/reportvelocityvector.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include <xmlconfig/xmlconfigurator.h>
#include <cxutils/math/pidcontroller.h>
#include <cxutils/thread.h>
#include <filter/rcfilter.h>
#include "auvc/baseline/primitivedriver.h"

#include <grapher/grapher.h>

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VelocityStateDriver
        ///   \brief This service provides closed loop control for velocity, rotation, 
        ///          and as an extra feature heading control which is not part of the
        ///          JAUS standard.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VelocityStateDriver: public JAUS::VelocityStateDriver
        {
        public:
            VelocityStateDriver();
            ~VelocityStateDriver();
            // Called on shutdown.
            virtual void Shutdown();
            // Called at initialization to load settings.
            virtual bool LoadSettings(const std::string& filename);
            // Called when a velocity command is received.
            //virtual bool SetDriveCommand(const JAUS::SetVelocityCommand* command);
            // Called when a velocity command is received.
            virtual bool SetVectorDriveCommand(const JAUS::SetVelocityVector* command);
            // Generates service specific events.
            virtual bool GenerateEvent(const JAUS::Events::Subscription& info) const;
            // Adds support for service specific events.
            virtual bool IsEventSupported(const JAUS::Events::Type type,
                                          const double requestedPeriodicRate,
                                          const JAUS::Message* queryMessage,
                                          double& confirmedPeriodicRate,
                                          std::string& errorMessage) const;
            // Method called whenver a message is received.
            virtual void Receive(const JAUS::Message* message);
            // Creates messages associated with this service.
            virtual JAUS::Message* CreateMessage(const JAUS::UShort messageCode) const;  
            // Called whenever component updates states of all services.
            virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
            // Prints information about the service.
            virtual void PrintStatus() const;
            void SetVelocityStateSensor(JAUS::VelocityStateSensor* sensor)
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                mpVelocityStateSensor = sensor;
            }
            void SetGlobalPoseSensor(JAUS::GlobalPoseSensor* sensor)
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                mpGlobalPoseSensor = sensor;
            }
            JAUS::SetVelocityVector GetCurrentVelocityVector() const 
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                return mVelocityVectorCommand;
            }
            void SetPrimitiveDriver(JAUS::PrimitiveDriver* driver)
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                mpPrimitiveDriver = driver;
            }
            JAUS::Time GetCurrentVelocityVectorTime() const 
            {
                JAUS::Mutex::ScopedLock lock(&mVelocityStateDriverMutex);
                return mVelocityVectorCommandTime;
            }
        protected:
            static void DriverThread(void* args);
            void DriverThread();
            void CreateReportFromQuery(const JAUS::QueryVelocityVector* query, 
                                       JAUS::ReportVelocityVector& report) const;
            CxUtils::Thread mDriverThread;                     ///< Thread for closed loop control calculations.
            Configuration::XmlConfigurator mXML;               ///< XML reader.
            JAUS::VelocityStateSensor* mpVelocityStateSensor;  ///<  Pointer to a velocity state sensor for data.
            JAUS::GlobalPoseSensor* mpGlobalPoseSensor;        ///<  Pointer to a global pose sensor for data.
            JAUS::PrimitiveDriver *mpPrimitiveDriver;
            JAUS::Time mVelocityVectorCommandTime;             ///<  Time when velocity vector command was received.
            JAUS::SetVelocityVector mVelocityVectorCommand;    ///<  Velocity vector drive command.
            // will have 6
            double mAxialPower;                               ///<  Thrust power for PID velocity control.
            double mLateralPower;                                ///<  Steering power for PID rotational control.
            double mVerticalPower;                              ///<  Lateral power/thrust.
            double mYawPower;
            double mPitchPower;
            double mRollPower;
            bool mReceivedCommand;
            
            double mYawThreshold;                          ///<  Heading threshold for flooring speed (radians).
            double mMaxVelocityX;                              ///<  Maximum possible velocity of vehicle in m/s.
            double mMaxVelocityY;                              ///<  Maximum possible velocity (lateral) of vehicle in m/s.
            CxUtils::PIDController mYawController;              ///<  Speed controller values.
            CxUtils::PIDController mPitchController;              ///<  Speed controller values.
            CxUtils::PIDController mRollController;              ///<  Speed controller values.
            CxUtils::PIDController mVerticalController;              ///<  Speed controller values.
            Utility::Grapher mGrapher;                         ///<  Grapher for visualizing data points.
            bool mGraphFlag;                                   ///<  If true, graph data.
            Utility::LowPassFilter *mpYawFilter;               ///<  First order RC Yaw filter.
            double mDesiredYaw;                                ///<  Filtered yaw value in radians.
        };
    }
}

#endif
