/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.h
/// \brief This is the main file for running the Baseline program for the
///        ground vehicle, which does basic sensor and motor control integration.
///
/// Author(s): David Adams<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_GROUND_BASELINE__H
#define __ZEBULON_GROUND_BASELINE__H

#include <jaus/core/component.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/extras/rangesensor/rangesensorconfig.h>
#include <jaus/mobility/sensors/localposesensor.h>

#include <mcu/interface.h>
#include <compass/compassfactory.h>
#include <gps/gpsfactory.h>
#include <laser/laserfactory.h>

#include <cvcapture/cvimagecapture.h>

#include <roboteq/ax3500.h>

#include <baseline/baseline.h>
#include "igvc/baseline/primitivedriver.h"
#include "igvc/baseline/globalposesensor.h"
#include "igvc/baseline/rangesensor.h"
#include "igvc/baseline/visualsensor.h"
#include "igvc/baseline/velocitystatesensor.h"
#include "igvc/baseline/velocitystatedriver.h"
#include "igvc/baseline/microcontroller.h"
#include "igvc/baseline/localwaypointlistdriver.h"

#include <string>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Baseline class for IGVC 2011.
        ///
        ///   JAUS services are assigned to the following components:
        ///   Baseline Component [3]:
        ///     Primitive Driver
        ///     Global Pose Sensor
        ///     Visual Sensor (custom)
        ///     Range Sensor (custom)
        ///   Vector Driver Component [2]:
        ///     Velocity State Driver
        ///   Local Waypoint List Driver [1] (JAUS Challenge Support):
        ///     Local Pose Sensor
        ///     Velocity State Sensor
        ///     List Manager
        ///     Local Waypoint List Driver
        ///     Microcontroller (custom)
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Baseline : public Zebulon::Configuration::Baseline
        {
        public:
            Baseline();
            ~Baseline();
            /** Load program/device settings from XML file. */
            virtual bool LoadSettings(const std::string& xmlFilename);
            /** Create JAUS services and components and configure. */
            virtual bool SetupJAUS();
            /** Connect to sensors and other hardware (e.g. motors). */
            virtual bool InitializeHardware(std::string& errorMessage);
            /** Initialize JAUS. */
            virtual bool InitializeJAUS(std::string& errorMessage);
            /** Shutdown JAUS. */
            virtual void ShutdownJAUS();
            /** Shutdown sensors and other hardware (e.g. motors) */
            virtual void ShutdownHardware();
            /** Method called when main loop updates, add whatever you want. */
            virtual void Update();
            /** Called in main loop, use to print information to consle. */
            virtual void PrintStatus();
            JAUS::Component* GetBaselineComponent() { return &mBaselineComponent; }
            JAUS::Component* GetVectorDriverComponent() { return &mVectorDriverComponent; }
            JAUS::Component* GetLocalWaypointListDriverComponent() { return &mLocalWaypointListDriverComponent; }
        protected:
            CxUtils::Thread mRoboteqThread;                 ///< Thread object for Roboteq communication.
            static void RoboteqThread(void *args);          ///< Thread function for Roboteq communication.
            // CONFIGURATION DATA
            Configuration::XmlConfigurator mXmlConf;        ///< XML reader.
            // JAUS
            std::string mJAUSSettingsXML;                   ///<  Filename for JAUS XML settings.
            JAUS::Address mBaselineComponentID;                         ///<  ID for JAUS Baseline component.
            JAUS::Address mVectorDriverComponentID;                     ///<  ID for JAUS Vector driver component.
            JAUS::Address mLocalWaypointListDriverComponentID;          ///<  ID for JAUS Challenge component.
            JAUS::Component mBaselineComponent;                         ///<  Baseline JAUS component (primary) (ID = 2).
            JAUS::Component mVectorDriverComponent;                     ///<  Component containing vector driving capabilities (ID = 3).
            JAUS::Component mLocalWaypointListDriverComponent;          ///<  Component with JAUS challenge services (ID = 1)
            JAUS::LocalPoseSensor* mpLocalPoseSensor;                   ///<  JAUS Local Pose sensor service.
            Microcontroller* mpMicrocontroller;                         ///<  Microcontroller service.
            VelocityStateSensor* mpVelocityStateSensor;                 ///<  JAUS Velocity State sensor service.
            PrimitiveDriver* mpPrimitiveDriver;                         ///<  JAUS Primitive Driver service.
            GlobalPoseSensor* mpGlobalPoseSensor;                       ///<  JAUS Global Pose sensor service.
            VisualSensor* mpVisualSensor;                               ///<  JAUS Visual Sensor service.
            RangeSensor* mpRangeSensor;                                 ///<  JAUS Range Sensor service.
            VelocityStateDriver* mpVelocityStateDriver;                 ///<  JAUS Velocity State Driver service.
            LocalWaypointListDriver* mpLocalWaypointListDriver;         ///<  JAUS Local Waypoint List Driver service.
            // HARDWARE
            Zebulon::Configuration::Baseline::SerialInfo mARMmiteInfo;              ///<  Information for connecting to MCU.
            Zebulon::Configuration::Baseline::SerialInfo mFrontMotorInfo;           ///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mRearMotorInfo;            ///<  Microcontroller information.
            int mFrontMotorID;                                                      ///<  Front motor ID number.
            int mRearMotorID;                                                       ///<  Rear motor ID number.
            Zebulon::Configuration::Baseline::CameraInfo mCameraInfo;               ///<  Camera information.
            Zebulon::Configuration::Baseline::LaserInfo mLaserInfo;                 ///<  Laser range finder information.
            Zebulon::Configuration::Baseline::CompassInfo mCompassInfo;             ///<  Compass information.
            Zebulon::Configuration::Baseline::SerialInfo mGpsInfo;                  ///<  GPS information.
            // DEVICE/SENSOR INTERFACES
            Zebulon::MCU::Interface mARMmite;                           ///<  Pointer to ARMmite interface.
            Compass::GenericCompass* mpCompass;                         ///<  Pointer to compass device.
            GPS::GenericGPS* mpGPS;                                     ///<  Pointer to GPS device.
            Laser::GenericLaser* mpLaser;                               ///<  Pointer to laser device.
            Motor::Roboteq::AX3500* mpFrontMotor;                       ///<  Pointer to front Roboteq controller interface.
            Motor::Roboteq::AX3500* mpRearMotor;                        ///<  Pointer to rear Roboteq controller interface.
            Video::CvImageCapture mCamera;                              ///<  Camera interface.
        };
    }
}
#endif
