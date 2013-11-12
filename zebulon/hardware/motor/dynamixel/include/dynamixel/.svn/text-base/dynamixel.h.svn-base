/////////////////////////////////////////////////////////////////////////////////////
///
/// \file dynamixel.h
/// \brief Dynamixel Servo Interface.
///
/// Author(s): Gary Stein<br>
/// Created: 3/10/2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: gstein@mail.ucf.edu<br>
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
#ifndef __ZEBULON_MOTOR_DYNAMIXEL_H
#define __ZEBULON_MOTOR_DYNAMIXEL_H

#include <string>
#include <cxutils/thread.h>
#include <stdio.h>
#include <set>

// Control table address
#define P_GOAL_POSITION_L       30
#define P_GOAL_POSITION_H       31
#define P_PRESENT_POSITION_L    36
#define P_PRESENT_POSITION_H    37
#define P_MOVING                46
#define P_MOVING_SPEED_L        32
#define P_MOVING_SPEED_H        33
#define P_TORQUE_LIMIT_L        34
#define P_TORQUE_LIMIT_H        35

namespace Zebulon
{
    namespace Motor
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Overload this callback and functions to recieve events from servo
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Callback
        {
        public:
            Callback() {}
            virtual ~Callback() {}
            typedef std::set<Callback*> Set;
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Event called when new data is available (only send certain servos?)
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual void ProcessServoData(const double pos,const unsigned int id = 0) = 0;
        };

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class Dynamixel
        ///  \brief Wrapper interface for communication with a Dynamixel Servo
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class Dynamixel
        {
        public:
            Dynamixel();
            virtual ~Dynamixel();
            virtual bool Initialize(const std::string& port,
                                    const bool usb2dynamixel=true,
                                    const double updateRateHz=50.0,
                                    const int baudNum = 34);
            virtual bool Initialize(const int deviceNum,
                                    const bool usb2dynamixel=true,
                                    const double updateRateHz=50.0,
                                    const int baudNum = 34);
            virtual void Shutdown();
            virtual void SetServoSpeed(const int id, const double rpm = 13.32 /* 2 degrees per 40 Hz. */);
            virtual void SetTorqueLimit(const int id, const int val = 1023 /* 0 to 1023, 1023 max */);
            //-100 to 100 scale
            virtual int AddServo(const int id,const double val=0.0);
            virtual int SetServo(const int id,const double val);
            virtual int SetServoDegrees(const int id,const double val);
            virtual double GetServo(const int id);
            virtual double GetServoDegrees(const int id);
            static double ToDegrees(const double value);
            // Method to register callbacks.
            virtual bool RegisterCallback(Callback* cb)
            {
                CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                mCallbacks.insert(cb);
                return true;
            }
            // Method to remove a callback.
            virtual bool RemoveCallback(Callback* cb)
            {
                CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                Callback::Set::iterator c;
                c = mCallbacks.find(cb);
                if(c != mCallbacks.end())
                {
                    mCallbacks.erase(c);
                    return true;
                }
                return false;
            }
            // Method to clear callbacks.
            void ClearCallbacks()
            {
                CxUtils::Mutex::ScopedLock lock(&mCallbacksMutex);
                mCallbacks.clear();
            }
        private:
            static int ServoConvert(const double val);
            static double ServoUnConvert(const int val);
            static double ServoUnConvertDegrees(const int val);
            virtual void ProcessingFunction();

            CxUtils::Thread mThread;                    ///< Thread for recieving data
            static void ThreadFunction(void *args);     ///< Main processing function, must exist per compass
            CxUtils::Mutex mMutex;                      ///< Mutex for data member protection

            int mBaudNum;                               ///<  Current Baud Num.
            std::string mPort;                          ///<  Current Port to laser.
            double mRate;                               ///<  Update rate.
            std::map<int,double> mServoCommand;         ///<  Servo commands.
            std::map<int,double> mServoFeedback;        ///<  Servo feedback values.
            int mDeviceID;                              ///<  Device ID set.

            int mFails;                                 ///<  Number of failures.
            //  Borrowed from Example in SDK.
            static void PrintCommStatus(int CommStatus);
            static void PrintErrorCode();
            void ClearErrorCode(int id);
            CxUtils::Mutex mCallbacksMutex;             ///<  Mutex for thread protection of callbacks
            Callback::Set mCallbacks;                   ///<  Registered callbacks for data.
        };
    }
}


#endif
