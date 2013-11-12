/////////////////////////////////////////////////////////////////////////////
///
/// \file testmission.h
/// \brief test mission for turning PID controllers.
///
/// Author(s): Cassondra Puklavage<br>
/// Created: 05/30/2010<br>
/// Copyright (c) 2008<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email:bmvalentino@gmail.com<br>
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

#ifndef __UNDERWATER_TESTMISSION_H
#define __UNDERWATER_TESTMISSION_H

#include "auvc/underwater/mission.h"
#include <statemachine/utility/utility.h>
#include <statemachine/utility/stopwatch.h>

namespace Zebulon
{
    namespace Underwater
    { 
        class TestMission : public Mission
        {
        public:
            TestMission();
            virtual ~TestMission();
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
        private:
            // Yaw
            double mCurrentHeading;
            double mDesiredHeading;
            // Pitch
            double mCurrentPitch;
            double mDesiredPitch;
            // Roll
            double mCurrentRoll;
            double mDesiredRoll;
            // Depth
            double mCurrentDepth;
            double mDesiredDepth;
            // Axial Movement
            double mForwardSpeed;
            // Lateral Movement
            double mLateralSpeed;
            // Vertical Movement
            double mVerticalSpeed;
            // Turning Movement
            double mYawSpeed;
            double mPitchSpeed;
            double mRollSpeed;
            
            bool mLightValue;
            
            //For Display Test
            int mGroup;
            double mRed;
            double mBlue;
            double mGreen;
            
            AI::StopWatch mSquareWatch;
            
        };
    }
}


#endif 
/* End of file*/