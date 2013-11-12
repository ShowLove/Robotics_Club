/////////////////////////////////////////////////////////////////////////////
///
/// \file StartButton.h
/// \brief DESCRIPTION OF PROGRAM
///
/// Author(s): Michael Scherer<br>
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
#ifndef __ZEBULON_UNDERWATER_StartButton_H
#define __ZEBULON_UNDERWATER_StartButton_H

#include "auvc/underwater/mission.h"
//#include "gvision/segment.h"
//#include "gvision/threshhold.h"
//#include "gvision/visionutil.h"
//#include "gvision/threshratio.h"
#include "statemachine/utility/utility.h"
#include "statemachine/utility/debounce.h"
//#include "statemachine/utility/stopwatch.h"

namespace Zebulon
{
    namespace Underwater
    { 
        class StartButton : public Underwater::Mission
        {
        public:
            enum State
            {
                Wait,
                NextMission
            };
            
            StartButton();
            ~StartButton();
            
            virtual int ErrorCheck();
            virtual int FetchProcess();
            virtual std::string ExitEarly();
            virtual std::string Execute();
            virtual int Init();
            
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

#endif // __ZEBULON_UNDERWATER_StartButton_H
/* End of file */
