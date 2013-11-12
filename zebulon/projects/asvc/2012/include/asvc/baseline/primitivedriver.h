////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
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
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF''AS IS'' AND ANY
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
#ifndef __ZEBULON_SURFACE_PRIMITIVEDRIVER__H
#define __ZEBULON_SURFACE_PRIMITIVEDRIVER__H

#include <jaus/mobility/drivers/primitivedriver.h>
#include <cxutils/cxutils.h>
#include <mcu/interface.h>
#include <mcu/messagefactory.h>
#include <mcu/messages/setservooutputs.h>
#include <mixer/mixer.h>
#include <filter/rcfilter.h>

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class PrimitiveDriver
        ///   \brief This class is the primitive driver class that inheritis from the
        ///          JAUS primitive driver, but overloads the SetWrenchEffort function
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class PrimitiveDriver: public JAUS::PrimitiveDriver
        {
        public:
            enum Motor
            {
                FrontLeft = 0,
                FrontRight,
                RearLeft,
                RearRight
            };
            class MicroMotorMessage
            {

            };

            PrimitiveDriver();
            ~PrimitiveDriver();
            virtual void Shutdown();
            // Sets the wrench effort from a JAUS message.
            virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command);
            // Called when the component transitions from Standby to Ready.
            virtual bool Resume();
            // Called when the component transitions to a Standby state.
            virtual bool Standby();
            // Called when the component is set into the Ready state.
            virtual bool Reset();
            // Calculated thrust for side of robot.
            double GetMotorThrust(Motor motor) const;
            // Prints information about the service.
            virtual void PrintStatus() const;
        protected:
            CxUtils::Mutex mThrustMutex;          ///<  For thread protection of data.
            double mFrontLeftThrust;              ///<  Left side thrust [-100,100]%.
            double mFrontRightThrust;             ///<  Right side thrust [-100,100]%.
            double mRearLeftThrust;               ///<  Lateral thrust [-100,100]%.
            double mRearRightThrust;              ///<  Lateral thrust [-100,100]%.
            Zebulon::Utility::Mixer mMixer;       ///<  Utility to perform mixing.
            double mLowerLimit;                   ///<  Lower limit for inputs.
            double mUpperLimit;                   ///<  Upper limit for inputs.
            double mMotorAngleRad[4];             ///<  Angle of each motor relative to forward, positive clockwise
//            Zebulon::Utility::LowPassFilter *mpThrustXFilter;   ///<  Left motor thrust filter.
//            Zebulon::Utility::LowPassFilter *mpThrustYFilter;   ///<  Lateral motor thrust filter.
//            Zebulon::Utility::LowPassFilter *mpThrustZFilter;   ///<  Right motor thrust filter.
        };
    }
}
#endif
