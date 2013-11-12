/////////////////////////////////////////////////////////////////////////////
///
/// \file mission.h
/// \brief Base class for sub missions.
///
/// Author(s): John Reeder <br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
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

#ifndef __ZEBULON_UNDERWATER_MISSION_H
#define __ZEBULON_UNDERWATER_MISSION_H

#include <statemachine/mission.h>
#include "auvc/underwater/globalinfo.h"
#include "auvc/underwater/globalcommand.h"

namespace Zebulon 
{
    
    namespace Underwater 
    {
        static const std::string FinalMission = "Pinger";
        
        class Mission : public AI::Mission
        {
        public:
            Mission();
            ~Mission();
            virtual void SetGlobalInfo(AI::GlobalInfo* globalInfo);
            virtual void SetGlobalCommand(AI::GlobalCommand* globalCommand);
            
        protected:
            static GlobalInfo* mGlobalInfo; 
            static GlobalCommand* mGlobalCommand;
        };
    }
}

#endif