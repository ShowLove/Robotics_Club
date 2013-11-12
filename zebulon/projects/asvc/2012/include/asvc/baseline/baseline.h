/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.h
/// \brief This is the main file for running the Baseline program for the
///        surface vehicle, which does basic sensor and motor control integration.
///
///  <br>Author(s): Matt Znoj
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
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
#ifndef __ZEBULON_SURFACE_BASELINE__H
#define __ZEBULON_SURFACE_BASELINE__H

#include <jaus/core/component.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <jaus/mobility/drivers/localwaypointlistdriver.h>

#include <compass/compassfactory.h>
#include <gps/gpsfactory.h>
#include <laser/laserfactory.h>
#include <gx3/gx3.h>
#include <cvcapture/cvimagecapture.h>
#include <baseline/baseline.h>
#include <xmlconfig/xmlconfigurator.h>
#include <mcu/slipinterface.h>
#include <roboteq/sdc2100.h>
#include <raytek/raytek.h>
#include <dynamixel/dynamixel.h>

#include "asvc/baseline/primitivedriver.h"
#include "asvc/baseline/globalposesensor.h"
#include "asvc/baseline/rangesensor.h"
#include "asvc/baseline/visualsensor.h"
#include "asvc/baseline/velocitystatesensor.h"
#include "asvc/baseline/velocitystatedriver.h"
#include "asvc/baseline/microcontroller.h"

#include <iostream>
#include <string>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Baseline
        ///
        ///   JAUS services are assigned to the following components:
        ///   Baseline Component [3]:
        ///     Primitive Driver
        ///     Global Pose Sensor
        ///     Visual Sensor (custom)
        ///     Range Sensor (custom)
        ///     Microcontroller (custom)
        ///   Vector Driver Component [2]:
        ///     Velocity State Driver
        ///   Local Waypoint List Driver [1] (JAUS Challenge Support):
        ///     Local Pose Sensor
        ///     Velocity State Sensor
        ///     List Manager
        ///     Local Waypoint List Driver
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Baseline : public Zebulon::Configuration::Baseline,
                                Zebulon::Motor::Callback
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
            /** Event callback for Dynamixel. */
            virtual void ProcessServoData(const double pos,const unsigned int id = 0);
        protected:
            CxUtils::Thread mBaselineThread;                            ///<  Main thread for communication to MCU and Heat sensor.
            CxUtils::Thread mDynamixelThread;                           ///<  Dedicated thread for dynamixel for smooth movement.
            static void BaselineThread(void *args);
            static void DynamixelThread(void* args);
            // CONFIGURATION DATA
            Configuration::XmlConfigurator mXmlConf;                    ///<  Xml reader
            // JAUS
            std::string mJAUSSettingsXML;                               ///<  Filename for JAUS XML settings.
            JAUS::Address mBaselineComponentID;                         ///<  ID for JAUS Baseline component.
            JAUS::Address mVectorDriverComponentID;                     ///<  ID for JAUS Vector driver component.
            JAUS::Address mLocalWaypointListDriverComponentID;          ///<  ID for JAUS Challenge component.
            JAUS::Component mBaselineComponent;                         ///<  Baseline JAUS component (primary) (ID = 2).
            JAUS::Component mVectorDriverComponent;                     ///<  Component containing vector driving capabilities (ID = 3).
            JAUS::Component mLocalWaypointListDriverComponent;          ///<  Component with JAUS challenge services (ID = 1)
            JAUS::LocalPoseSensor* mpLocalPoseSensor;                   ///<  JAUS Local Pose sensor service.
            VelocityStateSensor* mpVelocityStateSensor;                 ///<  JAUS Velocity State sensor service.
            PrimitiveDriver* mpPrimitiveDriver;                         ///<  JAUS Primitive Driver service.
            GlobalPoseSensor* mpGlobalPoseSensor;                       ///<  JAUS Global Pose sensor service.
            VisualSensor* mpVisualSensor;                               ///<  JAUS Visual Sensor service.
            RangeSensor* mpRangeSensor;                                 ///<  JAUS Range Sensor service.
            VelocityStateDriver* mpVelocityStateDriver;                 ///<  JAUS Velocity State Driver service.
            Microcontroller* mpJAUSMicro;                               ///<  Microcontroller services.
            Zebulon::MCU::SlipInterface* mpMicroInterface;              ///<  Microcontroller interface.
            // HARDWARE
            double mMaxDynamixel;                                       ///<  Maximum value that can be sent to servo.
            double mMinDynamixel;                                       ///<  Minimum value that can be sent to servo.
            Zebulon::Configuration::Baseline::SerialInfo mMicrocontrollerInfo;     ///<  Microcontrol information.
            Zebulon::Configuration::Baseline::CameraInfo mCameraInfo;       ///<  Camera information.
            Zebulon::Configuration::Baseline::CameraInfo mBackupCameraInfo; ///<  Backup Camera information.
            Zebulon::Configuration::Baseline::LaserInfo mLaserInfo;         ///<  Laser range finder information.
            Zebulon::Configuration::Baseline::CompassInfo mCompassInfo;     ///<  Compass information.
            Zebulon::Configuration::Baseline::SerialInfo mGpsInfo;          ///<  GPS information.
            Zebulon::Configuration::Baseline::SerialInfo mGX3Info;                  ///<  GX3 information.
            Zebulon::Configuration::Baseline::SerialInfo mMotorControllerAInfo;     ///<  Info for motorcontrollerA (if used).
            Zebulon::Configuration::Baseline::SerialInfo mMotorControllerBInfo;     ///<  Info for motorcontrollerB (if used).
            Zebulon::Configuration::Baseline::SerialInfo mRaytekInfo;       ///<  Raytek information.
            Zebulon::Configuration::Baseline::SerialInfo mDynamixelInfo;    ///<  Dynamixel information.

            // DEVICE/SENSOR INTERFACES
            Zebulon::INS::MicroStrain::GX3 * mpGX3;                         ///<  Pointer to GX3 compass/gps/accelerometer
            Compass::GenericCompass* mpCompass;                             ///<  Pointer to compass device.
            GPS::GenericGPS* mpGPS;                                         ///<  Pointer to GPS device.
            Laser::GenericLaser* mpLaser;                                   ///<  Pointer to laser device.
            Zebulon::Raytek::Interface* mpRaytekInterface;                  ///<  Pointer to Raytek device.
            Zebulon::Motor::Dynamixel* mpDynamixelInterface;                ///<  Pointer to Dynamixel device.
            Video::CvImageCapture mCamera;                                  ///<  Camera interface.
            Video::CvImageCapture mBackupCamera;                            ///<  Backup camera interface (secondary camera).
            Zebulon::Motor::Roboteq::SDC2100 mMotorControlA;                ///<  Connection to motorcontroller A.
            Zebulon::Motor::Roboteq::SDC2100 mMotorControlB;                ///<  Connection to motorcontroller B.

            bool mLogFlag;                                                  ///<  Used for logging gps data for sensor comparison.
            FILE* mPoseFileStream;
            FILE* mVelocityFileStream;
        };
    }
}
#endif
