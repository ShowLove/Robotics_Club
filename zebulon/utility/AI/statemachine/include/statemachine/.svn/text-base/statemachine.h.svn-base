////////////////////////////////////////////////////////////////////////////////////
///
/// \file statemachine.h
/// \brief State machine interface for library.
///
/// Author(s): Gary Stein<br>
/// Created: 2009<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#ifndef __ZEBULON_AI_STATE_MACHINE__H
#define __ZEBULON_AI_STATE_MACHINE__H

#include <cxutils/thread.h>
#include <cxutils/mutex.h>
#include <cxutils/timer.h>
#include <string.h>
#include <map>
#include "statemachine/libstatemachinedll.h"
#include "statemachine/mission.h"

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class StateMachine
        ///
        ///   State machine interface class.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  StateMachine
        {
        public:
            StateMachine();
            virtual ~StateMachine();
            /** Initilize State Machine with globla information, command interface, and mission file name. */
            bool Initialize(GlobalInfo* globalInfo,
                            GlobalCommand* globalCommand,
                            std::string xmlName="",
                            const unsigned int delay = 100,
                            std::string startMission="");
            void Shutdown();
            /** Adds a mission to be executed put at the end of the list
                order of adding is precidence in sentinal rules
                aka add the interrupt missions first. */
            bool AddMission(Mission *inMission);
            bool AddMissionsFromFactory(const std::vector<std::string>& missions);
            /** Must implement for your version to add missions only for your project  */
            virtual bool AddMissions() = 0;
            //For the stack operation of the mission interrupts
            /** Pushes the current mission on the stack, if it already exist
                in the stack, it is moved to the end. */
            bool PushMission(Mission *inMission);
            /** Pops the top mission off the top, if no active, then returns NULL */
            Mission *PopMission();
            //bool LoadXML(const std::string filename);
            void AddMissionToFactory(const std::string& name, Mission* mission)
            {
                if(mMissionFactory.find(name) == mMissionFactory.end())
                {
                    mMissionFactory[name] = mission;
                }
            }
        private:
            static void StateMachineThread(void* args);
            CxUtils::Thread mStateMachineThread;    ///<  Thread object for state machine.
            CxUtils::Mutex mMutex;                  ///<  Mutex for thread protection.
            Mission *mMissionActive;                ///<  Points to the current active mission running.
            std::map<std::string, Mission *> mMissionList; ///<  Stores pointers to all missions that can execute.  Sorted by name.
            unsigned int mNumMissions;                     ///<  Number of missions that can run.
            std::vector<Mission *> mMissionStack;          ///<  Current mission stack (active missions).
            unsigned int mNumStack;                        ///<  Count of number of items in stack.
            CxUtils::Time::Stamp mDelayTimeMs;             ///<  Delay time between state machine updates.
            unsigned int mFirstRun;                        ///<  Marks beginning of run.
            std::map<std::string, Mission*> mMissionFactory;    ///<  Stores all missions possible for loading.
        };
    }
}

#endif
/* End of File */
