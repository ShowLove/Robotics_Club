////////////////////////////////////////////////////////////////////////////////////
///
/// \file surface.h
/// \brief Class containing all data for running the Surface Vehicle
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
#ifndef __ZEBULON_SURFACE_GROUND_CLASS__H
#define __ZEBULON_SURFACE_GROUND_CLASS__H

#include "asvc/surface/globalinfo.h"
#include "asvc/surface/globalcommand.h"
#include "asvc/surface/statemachine.h"
#include "asvc/baseline/microcontroller.h"
#include <jaus/core/component.h>
#include <xmlconfig/xmlconfigurator.h>
#include <statemachine/globalinfologreader.h>
#include <display/laserdisplay.h>
#include <cvcapture/cvimagecapture.h>
#include <cvcapture/imagecapturepylon.h>
#include "asvc/surface/depthmap.h"

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Main program for Roboboat 2011 class.  Loads all
        ///          settings and modules.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Surface
        {
        public:
            Surface();
            virtual ~Surface();
            /** Loads settings from XML */
            virtual bool LoadSettings(const std::string& xmlFilename);
            /*  Create JAUS services and components. */
            virtual bool SetupJAUS();
            /*  Initialize JAUS components.*/
            virtual bool InitializeJAUS();
            /*  Initialize additional hardware components no setup in baseline. */
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
            DepthMap* GetDepthMap(){ return mpDepthMap; }
            /** Shutdown JAUS */
            virtual void ShutdownJAUS();
            bool IsSimulation() const { return mSimulateFromLogFlag; }
        protected:
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
            JAUS::Address mSurfaceComponentID;              ///<  Component ID.
            JAUS::Address mGlobalPoseSensorComponentID;     ///<  Component ID of Global Pose sensor.
            JAUS::Address mVelocityStateSensorComponentID;  ///<  Component ID of Velocity State sensor.
            JAUS::Address mVelocityStateDriverComponentID;  ///<  Component ID for Velocity State driver.
            JAUS::Address mPrimitiveDriverComponentID;      ///<  Component ID for Primitive Driver.
            JAUS::Address mMicrocontrollerComponentID;      ///<  Component ID for Microcontroller service.
            JAUS::Component mSurfaceComponent;              ///<  Main JAUS component.
            GlobalInfo* mpGlobalInfo;                       ///<  Global information.
            GlobalCommand* mpGlobalCommand;                 ///<  Global command interface.
            double mMaxVelocity;                            ///<  Maximum velocity.
            // State Machine
            Zebulon::ASVC::StateMachine* mpStateMachine;    ///<  State Machine
            Zebulon::Utility::LaserDisplay mLaserDisplay;   ///<  Window for displaying range sensor data
            // Additional hardware
            bool mEnablePylonCameraFlag;                        ///<  Enable pylon camera.
            int mCaptureWidth;                                  ///<  Camera capture width.
            int mCaptureHeight;                                 ///<  Camera capture height.
            std::string mCameraSettingsFile;                    ///<  Camera settings filename.
            Zebulon::Video::CvImageCapture mBaslerCamera;       ///<  Basler camera.
            Zebulon::Video::ImageCapturePylon* mpPylonCamera;   ///<  Pylon Camera Interface (for Basler Direct).
            DepthMap* mpDepthMap;                               ///<  Point to depth map, if exists.
            bool mDepthMapEnableFlag;                           ///<  Is depth map enabled?
            std::vector<std::string> mOnChangeVariables;        ///<  Variables to log on change.
        };
    }
}


#endif

/*  End of File */
