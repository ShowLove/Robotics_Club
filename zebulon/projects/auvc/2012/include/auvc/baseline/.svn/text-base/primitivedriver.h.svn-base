////////////////////////////////////////////////////////////////////////////////////
///
///  \file primitivedriver.h
///  \brief This file contains the definition of the PrimitiveDriver class, used
///         as an interface for basic platform mobility.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Modified By: John Reeder<br>
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
#ifndef __ZEBULON_UNDERWATER_PRIMITIVEDRIVER__H
#define __ZEBULON_UNDERWATER_PRIMITIVEDRIVER__H

#include <jaus/mobility/drivers/primitivedriver.h>
#include <cxutils/cxutils.h>
#include <mcu/interface.h>
#include <mcu/messagefactory.h>

//#define ONE_LAT_MOTOR
#define TWO_LAT_MOTOR
#define LAT_ROLL_OFFSET 0.4
#define LAT_OFFSET 0.9

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Primitive Driver
        ///   \brief This class is the primitive driver class that inheritis from the 
        ///          JAUS primitive driver, but overloads the SetWrenchEffort function
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class PrimitiveDriver: public JAUS::PrimitiveDriver
        {
        public:
            PrimitiveDriver();
            ~PrimitiveDriver();
            virtual void Shutdown();
            //Sets the wrench effort from a JAUS message.
            virtual bool SetWrenchEffort(const JAUS::SetWrenchEffort* command);
            //Sets the pointer to the mcu interface.
            //Called when the component transitions from Standby to Ready.
            virtual bool Resume();
            //Called when the component transitions to a Standby state.
            virtual bool Standby();
            //Called when the component is set into the Ready state.
            virtual bool Reset();
            //Prints the motor values.
            virtual void PrintStatus() const;   
            //call to send update to controllers
            //bool Update();
            bool SetControllerOne(MCU::Interface* MCU1);
            bool SetControllerTwo(MCU::Interface* MCU2);
            unsigned char SeabotixConvert(double motorValue);
            void SetMotorZero();
            
            void ForceVectorReady(bool ready);
            
            const static unsigned char MOTOR_STOP = 0x80;
            
            enum MotorAddresses
            {
                AXIALRIGHT =                0x50,
                AXIALLEFT =                 0x52,
                //LATERALTOP =                0x60,
                LATERALTOP =                0x62,
                LATERALBOTTOM =             0x60,

                VERTICALFRONTRIGHT =        0x70,
                VERTICALFRONTLEFT =         0x72,
                VERTICALBACKRIGHT =         0x74,
                VERTICALBACKLEFT =          0x76
            };
            
            
        protected:
            static void DriverThreadOne(void *args);
            static void DriverThreadTwo(void *args);
            CxUtils::Mutex mDriverOneMutex;
            CxUtils::Mutex mDriverTwoMutex;
            CxUtils::Thread mDriverThreadOne;
            CxUtils::Thread mDriverThreadTwo;
            CxUtils::Mutex mMutex;
            JAUS::SetWrenchEffort mWrenchEffort;
            
            bool mIsReady;
            
            MCU::Interface *mMCUInterface1;
            MCU::Interface *mMCUInterface2;
            
            MCU::SetServoOutputs mServoAxialRight;
            MCU::SetServoOutputs mServoAxialLeft;
            MCU::SetServoOutputs mServoLateralTop;
            MCU::SetServoOutputs mServoLateralBottom;
            MCU::SetServoOutputs mServoVertFrontRight;
            MCU::SetServoOutputs mServoVertFrontLeft;
            MCU::SetServoOutputs mServoVertBackRight;
            MCU::SetServoOutputs mServoVertBackLeft;
            
            double mAxialMotion;
            double mLateralMotion;
            double mVerticalMotion;
            double mYawMotion;
            double mPitchMotion;
            double mRollMotion;
            
            double mMotor1;
            double mMotor2;
            double mMotor3;
            double mMotor4;
            
            //mAxial
        };
    }
}
#endif
