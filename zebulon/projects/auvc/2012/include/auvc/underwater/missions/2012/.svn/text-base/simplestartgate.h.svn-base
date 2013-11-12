/////////////////////////////////////////////////////////////////////////////
///
/// \file simplestartgate.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): David Nguyen<br>
///            Cassondra Puklavage<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: darkwinter8@gmail.com<br>
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
#ifndef __ZEBULON_UNDERWATER_SIMPLESTARTGATE_H
#define __ZEBULON_UNDERWATER_SIMPLESTARTGATE_H

#include "auvc/underwater/mission.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
#include "statemachine/utility/stopwatch.h"
#include "auvc/underwater/missions/path.h"
#include "auvc/underwater/controls.h"
#include <boost/timer.hpp>

namespace Zebulon
{
    namespace Underwater
    { 
        class SimpleStartGate : public Underwater::Mission
        {
        public:

            static const std::string NextMission;

            enum State
            {
                GoToDepth,
                ThroughGate,
                LeaveMission
            };
            
            SimpleStartGate();
            ~SimpleStartGate();
            
            /*"watchtime" is set to the value that was hardcoded in mXmlConf. The
              mStopWatch is set to watchtime. The start yaw is also set to the member
              variable for starting yaw. mDebounce is set to 5. The current state is
              then set to GoToDepth.*/
            virtual int Init();

            virtual int ErrorCheck(); //This does nothing, for now.

            /*At the beginning of this, every line that has GetVar, just means that variables
              with the respective names get values from the mXmlConf. Next, the compass, depth
              sensor, and downward camera are checked. A frame is created and if mProcessedImage
              is nonexistant.*/
            virtual int FetchProcess();

            //If the program enters this, then it will return to FetchProcess()
            virtual std::string ExitEarly();

            /*In GoToDepth, the if statement checks if the sub is at the correct depth and angle. If
              so, then set the current state to "ThroughGate", which speaks for itself. The
              timer also starts, and this timer determines how long the sub will move, and
              it's supposedly, the time it will take for the sub to get through the gate.
              In ThroughGate, if the timer has run out or we're taking too long, then go
              on to the next mission, which is "Buoys"*/
            virtual std::string Execute();

            std::string GetStateName(State state);

        private:
            State mState;
            
            double mDepthThresh;
            double mYawThresh;

            double mCurrentDepth;
            double mCurrentYaw;
            double mCurrentPitch;
            double mCurrentRoll;

            double mDesiredAxialThrust;
            double mDesiredLateralThrust;
            double mDesiredDepth;
            double mDesiredYaw;
            double mDesiredPitch;
            double mDesiredRoll;

            double mStartDepth;
            double mStartYaw;
            double mGateSpeed;
            
            Zebulon::AI::StopWatch mStopWatch;
        };
    }
}

#endif // __ZEBULON_UNDERWATER_SIMPLESTARTGATE_H
/* End of file */
