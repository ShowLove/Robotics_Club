/////////////////////////////////////////////////////////////////////////////
///
/// \file globalcommand.h
/// \brief Class for sending commands to the PrimitiveDriver
///
/// Author(s): John Reeder<br>
/// Created: 2010<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
///
/// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
/// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
/// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
/// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
/// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
/// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
/// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
/// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
/// ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
/// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF
/// THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
/// OF SUCH DAMAGE.
///
/////////////////////////////////////////////////////////////////////////////
#ifndef __ZEBULON_UNDERWATER_GLOBALCOMMAND_H
#define __ZEBULON_UNDERWATER_GLOBALCOMMAND_H

/*#include <statemachine/globalcommand.h>
#include <statemachine/globalinfo.h>
//#include <jaus/mobility/drivers/setwrencheffort.h>
#include <jaus/extras/mcu/setmicrocontrollerstate.h>
//#include <cxutils/math/pidcontroller.h>
#include <jaus/extras/mobility/setvelocityvector.h>*/

#include <statemachine/globalcommand.h>
#include <jaus/mobility/drivers/primitivedriver.h>
#include <jaus/mobility/drivers/velocitystatedriver.h>
#include <jaus/extras/mobility/setvelocityvector.h>
#include <cartographer/planning/beamsearch.h>
#include "auvc/underwater/globalinfo.h"

namespace Zebulon
{
    namespace Underwater
    {
        class GlobalCommand : public AI::GlobalCommand
        {
            public:
                GlobalCommand();
                virtual ~GlobalCommand();
                void SetDrivers(const JAUS::Address& primitiveDriver,
                            const JAUS::Address& velocityDriver)
                            {
                                CxUtils::Mutex::ScopedLock lock(&mCommandMutex);
                                mPrimitiveDriverComponentID = primitiveDriver;
                                mVelocityStateDriverComponentID = velocityDriver;
                            }
                // Gives a pointer to the info class and starts the thread
                bool Initialize ( AI::GlobalInfo* globalInfo, unsigned int delayTimeMs = 50 );

                //Set the contolled angle and depth values
                //these are in the closed loop control
                int SetDesiredYaw ( double yaw );
                int SetDesiredPitch ( double pitch );
                int SetDesiredRoll ( double roll );
                int SetDesiredDepth( double depth );
                
                int Surface();
                
                //These values are relatively open loop
                int SetDesiredLateralVel ( double velocity );
                int SetDesiredAxialVel ( double velocity );
                
                double GetDesiredPitch() { return mDesiredPitch; }
                double GetDesiredYaw() { return mDesiredYaw; }
                double GetDesiredRoll() { return mDesiredRoll; }
                double GetDesiredDepth() { return mDesiredDepth; }
                
                int SetTorpedo1(bool value);
                int SetTorpedo2(bool value);
                int SetDropper1(bool value);
                int SetDropper2(bool value);
                int SetGrabber1(bool value);
                int SetGrabber2(bool value);
                
                /*int SetDispUpperLeft(double red, double green, double blue);
                int SetDispForwardArrow(double red, double green, double blue);
                int SetDispUpperRight(double red, double green, double blue);
                int SetDispLeftArrow(double red, double green, double blue);
                int SetDispCenter(double red, double green, double blue);
                int SetDispRightArrow(double red, double green, double blue);
                int SetDispLowerLeft(double red, double green, double blue);
                int SetDispBackArrow(double red, double green, double blue);
                int SetDispLowerRight(double red, double green, double blue);*/

                // Gets the current drive mode command is operating in.
                //CommandMode GetDriveMode() const;
                virtual void PrintStatus() const;

            protected:
            
                double ConvertInchesToMeters(double inches);
                double ConvertFeetToMeters(double feet);
                /** Overload to have run in a thread commands. */
                virtual void CommandThread();
                AI::GlobalCommand::CommandMode mCommandMode;        ///<  Command mode.
                CxUtils::Mutex mCommandMutex;                       ///<  For thread protection of data.
                JAUS::Address mPrimitiveDriverComponentID;          ///<  ID of Primitive Driver component.
                JAUS::Address mVelocityStateDriverComponentID;      ///<  ID of Velocity State Driver component.
                //JAUS::SetWrenchEffort mWrenchEffort;                ///<  JAUS wrench effort command.
                //JAUS::SetVelocityCommand mVelocityCommand;          ///<  Velocity command.
                //JAUS::SetVelocityVector mVelocityVector;            ///<  Velocity vector command (heading control).
                
                double mDesiredYaw;                   ///< The heading we want to go to.
                double mDesiredPitch;                 ///< Desired Pitch orientation
                double mDesiredRoll;                  ///< Desired Roll orientation
                double mDesiredDepth;                 ///< Desired Depth
                double mDesiredAxialVel;              ///< Forward Velocity
                double mDesiredLateralVel;            ///< Strafing Velocity
                bool mControlReady;                   ///< Holds off control until first ask
                
                bool mTorpedo1;                       ///< Torpedo 1 control
                bool mTorpedo2;                       ///< Torpedo 2 control
                bool mDropper1;                       ///< Dropper 1 control
                bool mDropper2;                       ///< Dropper 2 control
                bool mGrabber1;                        ///< Grabber 1 control
                bool mGrabber2;                        ///< Grabber 2 control
                
                double mUpperLeftRed;
                double mUpperLeftGreen;
                double mUpperLeftBlue;
                double mForwardArrowRed;
                double mForwardArrowGreen;
                double mForwardArrowBlue;
                double mUpperRightRed;
                double mUpperRightGreen;
                double mUpperRightBlue;
                double mLeftArrowRed;
                double mLeftArrowGreen;
                double mLeftArrowBlue;
                double mCenterRed;
                double mCenterGreen;
                double mCenterBlue;
                double mRightArrowRed;
                double mRightArrowGreen;
                double mRightArrowBlue;
                double mLowerLeftRed;
                double mLowerLeftGreen;
                double mLowerLeftBlue;
                double mBackArrowRed;
                double mBackArrowGreen;
                double mBackArrowBlue;
                double mLowerRightRed;
                double mLowerRightGreen;
                double mLowerRightBlue;
                
                //Digital Control Outs, forced to short duration
                const static int mFireTicks=5;       ///< Number of Ticks for Solenoids to be enabled (at 20 Hz) 
                int mTorpedo1Ticks;                  ///< Torpedo 1 Tick Count
                int mTorpedo2Ticks;                  ///< Torpedo 2 Tick Count
                int mDropper1Ticks;                  ///< Dropper 1 Tick Count
                int mDropper2Ticks;                  ///< Dropper 2 Tick Count
                int mGrabber1Ticks;                   ///< Grabber Tick Count
                int mGrabber2Ticks;              ///< Extra solenoid Tick Count
                static bool FireUpdate(bool &mDevice,int &mTicks);
                
                double mCeilingDepth;                 ///< Minimum Depth allowed to be commanded
                double mFloorDepth;                   ///< Maximum Depth allowed to be commanded
                
                double mSurfaceDepth;
                
                bool mSurfaceFlag;                    ///> Once surfaced, ignore
        };
    }
}


#endif // __ZEBULON_UNDERWATER_GLOBALCOMMAND_H
