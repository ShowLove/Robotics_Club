/////////////////////////////////////////////////////////////////////////////
///
/// \file simplestartgate.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Cassondra Puklavage<br>
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
#ifndef __ZEBULON_UNDERWATER_STARTGATE_H
#define __ZEBULON_UNDERWATER_STARTGATE_H

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
        class StartGate : public Underwater::Mission
        {
        public:

            static const std::string NextMission;

            enum State
            {
                GoToDepth,
                InitTravel,

                PathTravel,

                LeaveMission
            };

            StartGate();
            ~StartGate();


            virtual int Init();
            virtual int ErrorCheck(); //This does nothing, for now.
            virtual int FetchProcess();
            //If the program enters this, then it will return to FetchProcess()
            virtual std::string ExitEarly();
            virtual std::string Execute();

            std::string GetStateName(State state);

        private:
            State mState;

            //PathFinder mPathFinder;
            //Zebulon::AI::Debounce mPathDebounce;

            Zebulon::AI::StopWatch mInitTravel;
            Zebulon::AI::StopWatch mPathTravel;

            //bool PathsCount();

            double mDepthThresh;
            double mYawThresh;

            double mInitDepth;
            double mStartYaw;
            double mInitTravelSpeed;
            double mPathTravelSpeed;

            IplImage* mDWFrame;
            IplImage* mDWProcFrame;
            bool mInitCameras;
            bool mLeaveMission;
            bool mVTGary;
            bool mVTDavid;
            bool mVTNew;

            // In Every Mission
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

        };
    }
}

#endif // __ZEBULON_UNDERWATER_STARTGATE_H
/* End of file */
