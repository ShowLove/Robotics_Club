/////////////////////////////////////////////////////////////////////////////
///
/// \file StartSwitch.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): David Nguyen<br>
///            Cassondra Puklavage<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
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
#ifndef __ZEBULON_UNDERWATER_STARTSWITCH_H
#define __ZEBULON_UNDERWATER_STARTSWITCH_H

#include "auvc/underwater/mission.h"
//#include "gvision/segment.h"
//#include "gvision/threshhold.h"
//#include "gvision/visionutil.h"
//#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
//#include "statemachine/utility/stopwatch.h"
#include <boost/timer.hpp>

namespace Zebulon
{
    namespace Underwater
    { 
        class StartSwitch : public Underwater::Mission
        {
        public:

            static const std::string NextMission;

            enum State
            {
                Wait,
                LeaveMission
            };
            
            StartSwitch();
            ~StartSwitch();
            
            virtual int ErrorCheck(); //This does nothing, for now.

            /*First if statement checks if the Yaw of the sub exists, if not then
              an error message is printed. Same goes for the depth sensor. The last
              if statement checks to see if the button has been pressed, if it has
              then Started is set to true*/
            virtual int FetchProcess();

            /*If this is entered, then the program will loop back to fetchProcess*/
            virtual std::string ExitEarly();

            /*Here, the program will change the mission state, when it can. If the
              current state is Wait, then "started" will be either true or false, if it
              is true, then the current state is set to NextMission, and info is printed.
              If the switch goes into "case Wait", then it will end up returning KeepRunning
              which will loop the program back to fetchProcess, work its way through to the
              switch statement, and will go into "case NextMission", which will send
              sub into the next mision, "VisionValidationGate2"*/
            virtual std::string Execute();

            /*Initializes mButtonDebouce to 3 (if program detects button pressed 3 times
              in a row, then it's valid). ExitEarlyToCounselor is set to 0 in mGlobalInfo.
              The current state that sub is in is "Wait". And Started is false.*/
            virtual int Init();

            std::string GetStateName(State state);
            
        private:
            State mState;
            
            double mDepthThresh;
            
            double mCurrentDepth;
            double mCurrentYaw;
            
            double mDesiredDepth;
            double mDesiredYaw;
            
            double mStartDepth;
            double mStartYaw;
            
            Zebulon::AI::Debounce mButtonDebounce;

        };
    }
}

#endif // __ZEBULON_UNDERWATER_STARTSWITCH_H
/* End of file */
