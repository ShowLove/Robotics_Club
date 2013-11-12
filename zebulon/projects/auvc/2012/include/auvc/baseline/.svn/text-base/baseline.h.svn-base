/////////////////////////////////////////////////////////////////////////////////////
///
/// \file baseline.h
/// \brief This is the main file for running the Baseline program for the
///        underwater vehicle, which does basic sensor and motor control integration.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Modified by: John Reeder <br>
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
#ifndef __ZEBULON_UNDERWATER_BASELINE__H
#define __ZEBULON_UNDERWATER_BASELINE__H

#include <jaus/core/component.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/extras/rangesensor/rangesensorconfig.h>
#include <jaus/mobility/sensors/localposesensor.h>

#include <compass/compassfactory.h>
#include <laser/laserfactory.h>
#include <gx3/gx3.h>

#include <cvcapture/cvimagecapture.h>

#include "zmcu/appliance.h"
#include "zmcu/microcontroller.h"

#include <baseline/baseline.h>
#include "auvc/baseline/primitivedriver.h"
#include "auvc/baseline/globalposesensor.h"
#include "auvc/baseline/rangesensor.h"
#include "auvc/baseline/visualsensor.h"
#include "auvc/baseline/velocitystatesensor.h"
#include "auvc/baseline/velocitystatedriver.h"

#include <cxutils/cxutils.h>
#include <xmlconfig/xmlconfigurator.h>

#include <string>

#include <fw/fw1394Camera.h>

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \Class Baseline class for AUVC 2011
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
            
        protected:
            CxUtils::Thread mBaselineThread;
            //static void BaselineThread(void *args);
            //CxUtils::Mutex mBaselineMutex;
            // Configuration Data
            Configuration::XmlConfigurator mXmlConf;        ///< Xml reader
            // JAUS
            std::string mJAUSSettingsXML;
            JAUS::Address mBaselineComponentID;                         ///<  ID for JAUS Baseline component.
            JAUS::Address mVectorDriverComponentID;                     ///<  ID for JAUS Vector driver component.
            JAUS::Address mChallengeComponentID;
            JAUS::Component mBaselineComponent;                         ///<  Baseline JAUS component (primary) (ID = 2).
            JAUS::Component mVectorDriverComponent;                     ///<  Component containing vector driving capabilities (ID = 3).
            JAUS::Component mChallengeComponent;
            JAUS::LocalPoseSensor* mpLocalPoseSensor;                   ///<  JAUS Local Pose sensor service.
            PrimitiveDriver* mpPrimitiveDriver;                         ///<  JAUS Primitive Driver service.
            GlobalPoseSensor* mpGlobalPoseSensor;                       ///<  JAUS Global Pose sensor service.
            VelocityStateSensor* mpVelocityStateSensor;                 ///<  JAUS Velocity State sensor service.
            VisualSensor* mpVisualSensor;                               ///<  JAUS Visual Sensor service.
            RangeSensor* mpRangeSensor; 
            Utility::Microcontroller *mpZMCU;
            VelocityStateDriver* mpVelocityStateDriver; 
            
            //std::string mIdentification;                    ///< System Identification
            
            // Device/Sensor interfaces
#define USE_GX3
#ifdef USE_GX3
            INS::MicroStrain::GX3 *mpCompass;
#else
            Compass::GenericCompass* mpCompass;
#endif
            Depth::Keller *mpDepthSensor;
            Video::CvImageCapture mForwardCamera;
            fw1394Camera *mpForwardCameraRaw;
            
            Video::CvImageCapture mDownwardCamera;
            fw1394Camera *mpDownwardCameraRaw;
            
            void SetupRawCamera(std::string name,fw1394Camera *camera);
            
            //Video::CvImageCapture* mpStereoCamera;
            Laser::GenericLaser *mpLaser;
            Utility::Appliance* mpAVRPower;
            //Utility::Appliance* mAVRMotorControl1;
            //Utility::Appliance* mAVRMotorControl2;
            Utility::Appliance* mpAVRSensor;
            Utility::Appliance* mpAVRPeripheral;
            Utility::Appliance* mpAVRDisplay;
            MCU::Interface *mpPower;
            MCU::Interface *mpMotorOne;
            MCU::Interface *mpMotorTwo;
            MCU::Interface *mpSensor;
            MCU::Interface *mpPeripheral;
            MCU::Interface *mpDisplay;
            
            ///< Data about device connectivity, loaded from XML
            Zebulon::Configuration::Baseline::CompassInfo mCompassInfo;				///<  Compass information.
            Zebulon::Configuration::Baseline::LaserInfo mLaserInfo;					///<  Laser range finder information.
            Zebulon::Configuration::Baseline::CameraInfo mForwardCameraInfo;		///<  Camera information.
            Zebulon::Configuration::Baseline::CameraInfo mDownwardCameraInfo;		///<  Camera information.
            Zebulon::Configuration::Baseline::CameraInfo mStereoCameraInfo;			///<  Camera information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRPowerInfo;    			///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRMotorControl1Info;     ///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRMotorControl2Info;     ///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRSensorInfo;     		///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRPeripheralInfo;     	///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mAVRDisplayInfo;     		///<  Microcontroller information.
            Zebulon::Configuration::Baseline::SerialInfo mDepthSensorInfo;     		///<  Microcontroller information.
            
            JAUS::RangeSensorConfig mLaserConfig;
            
            enum AVRPowerPins
            {
                // Analog Inputs
                ELEC_CURRENT		= 0xF0,
                ELEC_VOLTAGE		= 0xF2,
                MOTOR_CURRENT		= 0xC0,
                MOTOR_VOLTAGE		= 0xC2,
                // Digital Inputs
                /*START 				= 14,	// PB0 - 14
                ESTOP				=  5,   // PD3 - 5
                SHR_M_PWR			= 24,   // PC1 - 24
                BATT_M_PWR			= 23,   // PC0 - 23
                SHR_E_PWR			= 15,   // PB1 - 15
                BATT_E_PWR			= 16,   // PB2 - 16*/
                START 				= 0x0E,	  // PB0 - 14
                ESTOP				= 0x05,   // PD3 - 5
                SHR_M_PWR			= 0x18,   // PC1 - 24
                BATT_M_PWR			= 0x17,   // PC0 - 23
                SHR_E_PWR			= 0x0F,   // PB1 - 15
                BATT_E_PWR			= 0x10,   // PB2 - 16
                // Digital Outputs
                POWERSUPPLYELEC		= 17,   // PB3 - 17
                POWERSUPPLYMOTOR	= 26    // PC3 - 26
            };
            
            enum AVRSensorPins
            {
                // Analog Inputs
                TEMPERATURE 		= 0x00,	  // PB0 - 14
                HUMIDITY			= 0x01,   // PD3 - 5
                DEPTH				= 0x02,   // PC1 - 24
                AD1					= 0x03,   // PC0 - 23
                AD2					= 0x04,   // PB1 - 15
                AD0					= 0x05,   // PB2 - 16
            };
            
            enum AVRPeripheralPins
            {
                // Digital Outputs
                TORPEDO1			= 14,		// PB0 - 14
                TORPEDO2			= 15,		// PB1 - 15
                DROPPER1			= 18,		// PB4 - 18
                DROPPER2			= 19,		// PB5 - 19
                GRABBERARM1			= 17,		// PB3 - 17
                GRABBERARM2			= 16		// PB2 - 16
            };
            
            enum AVRDisplayPins
            {
                // Analog Outputs
                               
                UPPER_LEFT_R    = 0x0B,
                UPPER_LEFT_G    = 0x0D,
                UPPER_LEFT_B    = 0x0C,
                
                FORWARD_ARROW_R    = 0x8E,
                FORWARD_ARROW_G    = 0x90,
                FORWARD_ARROW_B    = 0x8F,
                
                UPPER_RIGHT_R    = 0x08,
                UPPER_RIGHT_G    = 0x0A,
                UPPER_RIGHT_B    = 0x09,
                
                LEFT_ARROW_R    = 0x8B,
                LEFT_ARROW_G    = 0x8D,
                LEFT_ARROW_B    = 0x8C,
                
                CENTER_R         = 0x82,
                CENTER_G         = 0x84,
                CENTER_B         = 0x83,
                
                RIGHT_ARROW_R    = 0x88,
                RIGHT_ARROW_G    = 0x8A,
                RIGHT_ARROW_B    = 0x89,
                
                LOWER_LEFT_R    = 0x05,
                LOWER_LEFT_G    = 0x07,
                LOWER_LEFT_B    = 0x06,
                
                BACK_ARROW_R    = 0x85,
                BACK_ARROW_G    = 0x87,
                BACK_ARROW_B    = 0x86,
                
                LOWER_RIGHT_R    = 0x02,
                LOWER_RIGHT_G    = 0x04,
                LOWER_RIGHT_B    = 0x03,
                
                
            };
        };
    }
}
#endif
