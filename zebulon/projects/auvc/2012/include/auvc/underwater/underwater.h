////////////////////////////////////////////////////////////////////////////////////
///
/// \file underwater.h
/// \brief Class containing all data for running the Ground Vehicle 
///        program.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef __ZEBULON_UNDERWATER_GROUND_CLASS__H
#define __ZEBULON_UNDERWATER_GROUND_CLASS__H

#include "auvc/underwater/globalinfo.h"
#include "auvc/underwater/globalcommand.h"
#include <jaus/core/component.h>
#include <xmlconfig/xmlconfigurator.h>
#include <statemachine/globalinfologreader.h>
//#include "mcu/microcontroller.h"
#include "mcu/interface.h"

#include "auvc/underwater/statemachine.h"
#include "auvc/underwater/missions/test/acousticpingerlocator.h"

#include "cxutils/cxutils.h"

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Main program for IGVC 2011 class.  Loads all
        ///          settings and modules.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class UnderwaterVehicle
        {
        public:
            UnderwaterVehicle();
            virtual ~UnderwaterVehicle();
            /** Loads settings from XML */
            virtual bool LoadSettings(const std::string& xmlFilename);
            /*  Create JAUS services and components. */
            virtual bool SetupJAUS();
            /*  Initialize JAUS components.*/
            virtual bool InitializeJAUS();

            virtual bool InitializeHardware();

            /**  Starts the program. */
            virtual bool Start();
            /** Prints data to console window if desired. */
            virtual void PrintStatus() const 
            {
            }
            AI::GlobalInfoLogReader* GetLogReader() { return &mLogReader; }
            AI::GlobalCommand* GetGlobalCommand(){ return mpGlobalCommand; }
            AI::GlobalInfo* GetGlobalInfo(){ return mpGlobalInfo; }
            /** Shutdown JAUS */
            virtual void ShutdownJAUS();
        protected:
            void SetupRawCamera(std::string name,fw1394Camera *camera,Configuration::XmlConfigurator &CameraXmlConfig);
            // XML Configuration
            Configuration::XmlConfigurator mXmlConfig;  ///<  XML config data.
            // Logging and Simulation Flags/Variables
            bool mLoggingEnabledFlag;                   ///<  Enable logging of data.
            CxUtils::Time::Stamp mLogDataDelayMs;       ///<  Delay time between logging events.
            CxUtils::Time::Stamp mImageLogDelayMs;      ///<  Delay time between logging images.
            CxUtils::Time::Stamp mLaserLogDelayMs;      ///<  Delay time between logging laser data.
            bool mSimulateFromLogFlag;                  ///<  Enable simulation of data from log files.
            std::string mSimulationLogName;             ///<  Log folder to simulate from.
            CxUtils::Time mSimulationLogStartTime;      ///<  Time point within simulation log to begin playback from.
            AI::GlobalInfoLogReader mLogReader;         ///<  Reads log data for simulation.
            // JAUS Data Members
            JAUS::Address mUnderwaterComponentID;               ///<  Component ID.
            JAUS::Address mGlobalPoseSensorComponentID;     ///<  Component ID of Global Pose Sensor.
            JAUS::Address mVelocityStateSensorComponentID;  ///<  Component ID of Velocity State Sensor.
            JAUS::Address mVelocityStateDriverComponentID;  ///<  Component ID for Velocity State Driver.
            JAUS::Address mPrimitiveDriverComponentID;      ///<  Component ID for Primitive Driver.
            JAUS::Address mMicrocontrollerComponentID;      ///<  Component ID for Microcontroller service.
            JAUS::Component mUnderwaterComponent;     ///<  Main JAUS component.
            GlobalInfo* mpGlobalInfo;             ///<  Global information.
            GlobalCommand* mpGlobalCommand;       ///<  Global command interface.
            // State Machine
            //Zebulon::IGVC::StateMachine* mpStateMachine;    ///< State Machine
            
            //APL Interface
            //AcousticPingerLocator* mpAPL;
            bool mEnableAPL;
            Video::CvImageCapture mForwardCamera;
            Video::CvImageCapture mDownwardCamera;
            fw1394Camera *mpForwardCameraRaw;
            fw1394Camera *mpDownwardCameraRaw;

            CxUtils::Thread mCameraThread;
            static void CameraThread(void *arg);
            void CaptureThread();
            std::string mXMLFilename;

        };
    }
}


#endif

/*  End of File */
