////////////////////////////////////////////////////////////////////////////////////
///
///  \file velocitystatesensor.h
///  \brief Contains the Velocity State Sensor Service implementation.
///
///  <br>Author(s): David Adams, Daniel Barber
///  <br>Created: 2011
///  <br>Copyright (c) 2011
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@gmail.com, dbarber@ist.ucf.edu
///  <br>Web:  http://robotics.ucf.edu
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
#ifndef __ZEBULON_GROUND_VELOCITY_STATE_SENSOR__H
#define __ZEBULON_GROUND_VELOCITY_STATE_SENSOR__H

#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <jaus/mobility/sensors/globalposesensor.h>
#include <roboteq/ax3500.h>
#include <encoders/rigidbody.h>
#include <xmlconfig/xmlconfigurator.h>
#include <filter/rcfilter.h>
#include <cxutils/thread.h>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VelocityStateSensor
        ///   \brief Velocity State Sensor service implementation.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VelocityStateSensor : public JAUS::VelocityStateSensor,
                                    public Motor::Roboteq::AX3500::Callback
        {
        public:
            ///< Parameters loaded from XML, check settings/services.xml for more information.
            enum Parameter
            {
                WheelBase = 0,
                WheelRadius,
                MaxEncoderTicks,
                FrontBoardID,
                RearBoardID,
                LeftEncoderID,
                RightEncoderID,
                SamplePeriodMs,
                NumEncoders
            };
        
            VelocityStateSensor();
            ~VelocityStateSensor();
            virtual void Shutdown();
            virtual bool LoadSettings(const std::string& filename);
            virtual void ProcessMessage(const Motor::Roboteq::AX3500::Message* message, 
                                        const unsigned int boardID);
            void SetParameter(const Parameter param, const double value)
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);
                mParameterMap[param] = value;
            }
            double GetParameter(const Parameter param) const
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);
                std::map<Parameter, double>::const_iterator p;
                p = mParameterMap.find(param);
                if(p != mParameterMap.end())
                {
                    return p->second;
                }
                return 0.0;
            }
            virtual void CheckServiceStatus(const unsigned int timeSinceLastCheckMs);
            /** Set pointer to source for global pose data */
            void SetGlobalPoseSensor(JAUS::GlobalPoseSensor* globalPoseSensor)
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);
                mpGlobalPoseSensor = globalPoseSensor;
            }
        protected:
            static void VelocityThread(void* args);
            CxUtils::Thread mVelocityThread;            ///< Thread for calculations of objects.
            CxUtils::Mutex mMutex;                      ///< Synchronize encoder data with callback and other needs.
            Configuration::XmlConfigurator mXML;        ///< XML reader.
            std::map<Parameter,double> mParameterMap;   ///< Parameters specific to our vehicle.
            Utility::Encoders::RigidBody mRigidBody;    ///< Rigid body math for velocity calculations.
            JAUS::GlobalPoseSensor* mpGlobalPoseSensor; ///< Pointer to global pose sensor (used for yaw rate).
            JAUS::GlobalPose mPrevGlobalPose;           ///< Previous global pose.
            Utility::Filter* mpLowPassFilter;           ///< Low pass filter.
        };
    }
}
#endif //__ZEBULON_GROUND_VELOCITYSTATESENSOR__H

/*End Of File*/
