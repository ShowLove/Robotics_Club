/////////////////////////////////////////////////////////////////////////////
///
///  \file globalinfo.h
///  \brief Class for getting information about the global state and
///         sensors of a robot.
///
///  Author(s): Gary Stein, Daniel Barber<br>
///  Created: 2008<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu, dbarber@ist.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#ifndef _ZEBULON_AI_GLOBAL_INFO__H
#define _ZEBULON_AI_GLOBAL_INFO__H

#include "statemachine/names.h"
#include <jaus/mobility/sensors/globalposesensor.h>
#include <jaus/mobility/sensors/localposesensor.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/extras/command/subsystemcommand.h>
#include <jaus/extras/video/videosubscriber.h>
#include <jaus/extras/rangesensor/rangesubscriber.h>
#include <cxutils/cxutils.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <filter/filter.h>

#include "statemachine/libstatemachinedll.h"
#include "display/display.h"


namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalInfo
        ///  \brief Interface class for storing all system state information used by
        ///         the State Machine or Global Command interface for making decisions
        ///         and controlling the robot.
        ///
        ///  This data structures stores all information needed by the State Machine
        ///  for making decisions.  Examples of data to store here includes Compass,
        ///  GPS, Speed, Images, Button States, etc.
        ///
        ///  GlobalInfo is a JAUS Service as well, so it can make subscriptions to
        ///  other types of data needed by robot AI.
        ///
        ///  The final feature of GlobalInfo is data logging capabilities.  Data
        ///  logging may be needed for playback and debugging.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  GlobalInfo : public JAUS::SubsystemCommand,
                                                      public JAUS::VideoSubscriber::Callback,
                                                      public JAUS::RangeSubscriber::Callback
        {
        public:
                /** Callback interface to get data as it is added to
                    the GlobalInfo interface. */
                class ZEB_STATEMACHINE_UTIL_DLL Callback
                {
                public:
                    typedef std::set<Callback*> Set;                    ///<  Typedef for quick reference.
                    typedef std::map<const std::string, Callback*> Map; ///<  Typedef for quick reference.
                    Callback() {}
                    virtual ~Callback() {}
                    virtual void ProcessData(const std::string& name,
                                             const double value) { }
                    virtual void ProcessData(const std::string& name,
                                             const int value) { }
                    virtual void ProcessData(const std::string& name,
                                             const bool value) { }
                    virtual void ProcessData(const std::string& name,
                                             const CxUtils::Point3D::List& value) {}
                    virtual void ProcessData(const std::string& name,
                                             const IplImage *value) {}
                };

                GlobalInfo();
                virtual ~GlobalInfo();
                // Optional method to register to receive data as it is set within GlobalInfo interface
                void RegisterCallback(Callback* cb);
                // Optional method to register to receive data as it is set within GlobalInfo interface
                void RegisterCallback(const std::string& name, Callback* cb);
                // Optional method to register point data as it arrives
                void RegisterPointCallback(Callback *cb);
                // Optional method to add data members to log on change (if logging is on). Only four primitive types.
                void LogVariableOnChange(const std::string& name) { mOnChangeLogData.insert(name); }
                // Optional method to load settings from an XML file.
                virtual bool LoadSettings(const std::string& xmlFilename="");
                // Prints current status and select information to console for display.
                virtual void PrintStatus() const;
                // Method to display image data.
                void DisplayImages(const unsigned int sleepDelayMs = 10) const;
                // Method to enable data logging.
                void EnableLogging(const bool enable = true,
                                   const CxUtils::Time::Stamp dataIntervalMs = 250,
                                   const CxUtils::Time::Stamp imageIntervalMs = 250,
                                   const CxUtils::Time::Stamp laserIntervalMs = 250);
                // Method to disable logging of specific images added to GlobalInfo
                void AddImageNotToLog(const std::string& imageName);
                // Sets the logging delay interval (sets logging frequency).
                bool SetLoggingInterval(const unsigned int logDelayMs = 250);
                // Gets the logging interval time.
                CxUtils::Time::Stamp GetDataLoggingIntervalTime() const { return mLogDataDelayMs; }
                // Gets the logging interval time.
                CxUtils::Time::Stamp GetLaserLoggingIntervalTime() const { return mLaserLogDelayMs; }
                // Overload to pass names of additional data to logger.
                std::vector<std::string> GetAdditionalLogDataNames() const { return std::vector<std::string>(); }
                // Overload to pass additional data to logger.
                std::vector<std::string> GetAdditionalLogData() const { return std::vector<std::string>(); }
                // Method to check logging status.
                bool IsLogging() const;
                // Method to overload to check if everything is OK.
                virtual bool VerifyInitialization() { return true; }
                // Gets specific information by element name.
                virtual bool GetInfo(const std::string& name, double &value) const;
                // Sets specific information by element name.
                virtual void SetInfo(const std::string& name, const double value);
                // Gets integer data values by element name.
                virtual bool GetInfo(const std::string&  name, int &value) const;
                // Sets integer data values by element name.
                virtual void SetInfo(const std::string&  name, const int value);
                // Gets flag/boolean values by element name.
                virtual bool GetInfo(const std::string&  name, bool &value) const;
                // Sets flag/boolean values by element name.
                virtual void SetInfo(const std::string& name, const bool value);
                // Given a name identifier for a waypoint, saves all values to internal store.
                virtual void SetInfo(const std::string& name, const CxUtils::Utm& position);
                // Gets image data by name.
                virtual bool GetInfo(const std::string& name, IplImage *&value) const;
                // Sets image data by name.
                virtual void SetInfo(const std::string& name, const IplImage *value);
                // Get any laser scan or point array data.
                virtual bool GetInfo(const std::string& name, CxUtils::Point3D::List& value) const;
                // Gets UTM data with the given base name.
                virtual bool GetInfo(const std::string& name, CxUtils::Utm& value) const;
                // Gets UTM data with the given base name.
                virtual bool GetInfo(const std::string& name, CxUtils::Wgs& value) const;
                // Set any laser scan or point array data.
                virtual void SetInfo(const std::string& name, const CxUtils::Point3D::List& value);
                // Gets the timestamp of the image data.
                bool GetInfoUpdateTimeMs(const std::string& name, CxUtils::Time::Stamp& timeMs) const;
                // Check to see if a type of data exists.
                bool IsValueSet(const std::string& name) const;
                //=======================================
                //   Start of JAUS Related Methods
                //=======================================
                // This method must be overloaded to add support for new message types you are subscribing to!
                virtual JAUS::Message* CreateMessage(const JAUS::UShort jausMessageCode) = 0;
                // This method must be overloaded to add support for processing new messages you want.
                virtual void Receive(const JAUS::Message* jausMessage) = 0;
                // Overload this method, and insert code to create subscriptions for the data you need.
                virtual bool CreateSubscriptions() = 0;
                // Method try and create video subscriptions to any available cameras on this subsystem.
                bool CreateVideoSubscriptions(const JAUS::Address& id = JAUS::Address());
                // Method to map the name of a camera to a string name for automatic data setting.
                void SetCameraName(const unsigned int cameraID, const std::string& name);
                // Callback method when JAUS video data is received (don't need to overload, but can).
                virtual void ProcessRawImage(const JAUS::Address& source,
                                             const JAUS::Byte cameraID,
                                             const JAUS::Image& image,
                                             const unsigned int frameNumber);
                bool CreateRangeSensorSubscriptions(const JAUS::Address& id = JAUS::Address(),
                                                    const double updateRateHz = -1);
                // Method to map the name of a range sensor to a specific ID number.
                void SetRangeSensorName(const unsigned int deviceID, const std::string& name);
                // Callback method when JAUS range scan data is received (don't need to overload, but can).
                virtual void ProcessLocalRangeScan(const CxUtils::Point3D::List& scan,
                                                   const JAUS::Address& sourceID,
                                                   const JAUS::Byte deviceID,
                                                   const JAUS::Time& timestamp);
                // This method is called periodically, overload to perform periodic checks, events, etc.
                virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs) { /* Do whatever want, when called. */ }
                //=======================================
                //   End of JAUS Related Methods
                //=======================================
        protected:
                //=======================================
                //   Start of JAUS Related Helper Methods
                //=======================================
                void ProcessGlobalPose(const JAUS::ReportGlobalPose* message);
                void ProcessVelocityState(const JAUS::ReportVelocityState* message);
                //=======================================
                //   End of JAUS Related Helper Methods
                //=======================================
        private:
                // Logging data members.
                CxUtils::Mutex mRunningMutex;               ///<  For thread protection of log stuff.
                volatile bool mLoggingEnabledFlag;          ///<  If true, data logging is enabled (default).
                volatile bool mIsLoggingFlag;               ///<  True if data logging.
                std::set<std::string> mOnChangeLogData;     ///<  Data to log on change immediately.
                // Mutex for log files
                CxUtils::Mutex mLidarLogMutex;              ///<  Mutex for thread protection of log file.
                CxUtils::Mutex mDataLogMutex;               ///<  Mutex for thread protection of log file.
                void LogData();
                static void DataThread(void* args);         ///<  Thread function for data logging.
                static void PointDataThread(void* args);    ///<  Thread function for data logging of point data.
                CxUtils::Thread mDataThread;                ///<  Thread  object for logging and verifying subscriptions.
                CxUtils::Thread mPointDataThread;           ///<  Thread object for point data thread logging.
                CxUtils::Mutex mDataThreadMutex;            ///<  For thread protection of
                std::string mLogDirectory;                  ///<  Directory name where data is logged to.
                FILE* mLogStream;                           ///<  FILE pointer for data logging.
                FILE* mRangeSensorStream;                   ///<  Data file for range sensor information.
                CxUtils::Time::Stamp mLogDataDelayMs;       ///<  Delay time between logging events.
                CxUtils::Time::Stamp mImageLogDelayMs;      ///<  Delay time between logging images.
                CxUtils::Time::Stamp mLaserLogDelayMs;      ///<  Delay time between logging laser data.
                // Global Info Data.
                CxUtils::Mutex mLogStreamMutex;                                 ///<  For thread protection of log file.
                CxUtils::Mutex mDoubleMutex;                                    ///<  Mutex for thread protection of data.
                CxUtils::Mutex mIntMutex;                                       ///<  Mutex for thread protection of data.
                CxUtils::Mutex mBoolMutex;                                      ///<  Mutex for thread protection of data.
                CxUtils::Mutex mImageMutex;                                     ///<  Mutex for thread protection of data.
                CxUtils::Mutex mPointMutex;                                     ///<  Mutex for thread protection of point cloud data.
                CxUtils::Mutex mDataUpdateTimeMutex;                            ///<  Mutex for thread protection of data.
                CxUtils::Mutex mLastFrameNumberMutex;                           ///<  Mutex for thread protection of last frame number logged.
                std::map<std::string, double> mDoubleInfo;                      ///<  STL Map of data by string name.
                std::map<std::string, int> mIntInfo;                            ///<  STL Map of data by string name.
                std::map<std::string, bool> mBoolInfo;                          ///<  STL Map of data by string name.
                std::map<std::string, IplImage *> mImageInfo;                   ///<  STL Map of data by string name.
                std::map<std::string, int> mImageNumbers;                       ///<  STL Map of image frame numbers by name.
                std::map<std::string, int> mImageNumbersLogged;                 ///<  STL Map of image frame number logged to disk.
                std::set<std::string> mImagesNotToLog;                          ///<  Image data not to log.
                std::map<unsigned int, std::string> mCameraNames;               ///<  Names of cameras.
                std::map<unsigned int, std::string> mRangeSensorNames;          ///<  Names of range sensors.
                std::map<std::string, CxUtils::Point3D::List> mPointInfo;       ///<  STL Map of point cloud data.
                std::map<std::string, CxUtils::Time::Stamp> mDataUpdateTime;    ///<  STL Map of data update times.
                Callback::Set mGlobalInfoCallbacks;                             ///<  Global information callbacks.
                Callback::Set mGlobalInfoPointCallbacks;                        ///<  Callbacks for point cloud data only
                Callback::Map mKeyGlobalInfoCallbacks;                          ///<  Map of callbacks for specific data.
                CxUtils::Mutex mGlobalInfoCallbacksMutex;                       ///<  For thread protection of data.
        };
    }
}

#endif
/* End of File */
