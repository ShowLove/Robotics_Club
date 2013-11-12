////////////////////////////////////////////////////////////////////////////////////
///
/// \file statemachine.cpp
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
#include "statemachine/statemachine.h"
#include <iostream>

using namespace CxUtils;
using namespace Zebulon;
using namespace AI;
using namespace std;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
StateMachine::StateMachine()
{
    //ensure that al lists are null
    mMissionStack.clear();

    mNumMissions=0;
    mNumStack=0;
    //by default, the default mission is created
    //should not be called, only there till others initialized
    mMissionActive=NULL;
    //AddMission(mMissionActive);

    mMissionList.clear();
    mFirstRun=0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
StateMachine::~StateMachine()
{
    Shutdown();

    mStateMachineThread.StopThread(1000);

    // Clean up memory.
    std::vector<Mission*>::iterator m;
    for(m = mMissionStack.begin();
        m != mMissionStack.end();
        m++)
    {
        delete *m;
    }
    mMissionStack.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes the state machine.
///
///  \param[in] globalInfo Global Information store for re-use by missions.
///  \param[in] globalCommand Global Command interface for re-use by missions.
///  \param[in] xmlName Root filename to master XML file which contains
///                     references to all XML files for missions loaded.
///  \param[in] delay Delay between updates in milliseconds.
///  \param[in] startMission The mission to start with.  If NULL, first is
///                          used.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool StateMachine::Initialize(GlobalInfo* globalInfo,
                              GlobalCommand* globalCommand,
                              std::string xmlName,
                              const unsigned int delay,
                              std::string startMission)
{
    bool result = false;

    Shutdown();
    mDelayTimeMs=delay;

    Mission::XmlLoad(xmlName);

    //Add the child class missions

    if(!AddMissions())
    {
        cout << "Adding Mission Failed\n";
        return false;
    }

    if(mMissionActive == NULL)
    {
        cout << "No missions are added." << endl;
        return false;
    }

    if (startMission != "")
    {
        map<std::string, Mission *>::iterator iter;
        for(iter=mMissionList.begin();iter!=mMissionList.end();iter++)
        {
            Mission *tmpMission=iter->second;
            if(tmpMission->GetName()==startMission)
            {
                break;
            }
        }
        // check if mission exists
        if(iter==mMissionList.end())
        {
            cout << "StateMachine: tried to initialize to non-existing mission " << startMission << ", defaulting to first added mission\n";
        }
        // mission exists, set as the first mission
        else
        {
            mMissionActive=iter->second;
        }
    }

    if(globalInfo)
    {
        mMissionActive->SetGlobalInfo(globalInfo);
    }
    else
    {
        cout << "Passed Invalid GlobalInfo\n";
        return false;
    }

    if(globalCommand)
    {
        globalCommand->LoadSettings(xmlName);
        mMissionActive->SetGlobalCommand(globalCommand);
    }
    else
    {
        cout << "Passed Invalid GlobalCommand\n";
        return false;
    }

    if(mStateMachineThread.CreateThread(&StateMachine::StateMachineThread, this))
    {
        result = true;
    }


    if(result == false)
    {
        Shutdown();
    }

    return result;

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shuts down the state machine.
///
////////////////////////////////////////////////////////////////////////////////////
void StateMachine::Shutdown()
{
    mFirstRun=0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread function which carries out state machine logic.
///
////////////////////////////////////////////////////////////////////////////////////
void StateMachine::StateMachineThread(void* args)
{
    StateMachine* stateMachine = (StateMachine*)args;
    double fire,topFire;
    std::string transition;
    while(stateMachine->mStateMachineThread.QuitThreadFlag() == false)
    {
        CxUtils::Time::Stamp startTimeMs=CxUtils::GetTimeMs();
        //mutex protection needed?
        stateMachine->mMutex.Lock();
        fire=0;
        topFire=0;

        //if not missions added yet
        if(stateMachine->mMissionActive!=NULL)
        {
            if(stateMachine->mFirstRun==0)
            {
                cout << "StateMachine: First Run Active\n";
                stateMachine->mMissionActive->Init();
                stateMachine->mFirstRun=1;
            }
            //cout << "StateMachine: Running : " << stateMachine->mMissionActive->GetName() << "\n";

            Mission *tmpMission=NULL;
            Mission *topMission=NULL;
            map<std::string, Mission *>::iterator iter;
            for(iter=stateMachine->mMissionList.begin();iter!=stateMachine->mMissionList.end();iter++)
            {
                tmpMission=iter->second;
                fire=tmpMission->Sentinal();
                //the one who wants to fire the most is set
                if(fire>topFire)
                {
                    topFire=fire;
                    topMission=tmpMission;
                }
            }
            //if a rule fired, it interrupts others
            //and pushes to the stack the current active
            //unless it is already the active
            if(topFire>0 && topMission!=stateMachine->mMissionActive)
            {
                stateMachine->PushMission(stateMachine->mMissionActive);
                //Assign self active
                stateMachine->mMissionActive=topMission;
                //by design, a switch happened
                stateMachine->mMissionActive->Init();
            }
            //no one fired, check to see if stack, needs
            //to be popped off since it was the only way to build up
            else if(topFire==0 && stateMachine->mNumStack>0)
            {
                //Get last mission, no init since it is a return to normal operation
                Mission *tmpMission=stateMachine->PopMission();
                //should never be NULL
                if(tmpMission!=NULL)
                {
                    //clean up anything needed when leaving control
                    stateMachine->mMissionActive->Exit();
                    //Put old machine back in control
                    stateMachine->mMissionActive=tmpMission;
                }
            }

            //Active mission is executed
            transition=stateMachine->mMissionActive->Run();
            //if the transition exits to pass command over
            if(transition != Mission::KeepRunning)
            {
                //Fetch the mission (NULL if not found)
                map<std::string, Mission *>::iterator iter;
                //find stopped working?
                //iter=stateMachine->mMissionList.find(transition);
                for(iter=stateMachine->mMissionList.begin();iter!=stateMachine->mMissionList.end();iter++)
                {
                    Mission *tmpMission=iter->second;
                    if(tmpMission->GetName()==transition)
                    {
                        break;
                    }
                }
                //check if transition is possible
                if(iter==stateMachine->mMissionList.end())
                {
                    cout << "StateMachine: tried to transition to non-existing mission " << transition << "\n";
                }
                //good transisition, activate
                else
                {
                    Mission *tmpMission=iter->second;
                    //clean up anything needed when leaving control
                    stateMachine->mMissionActive->Exit();
                    //transision control
                    stateMachine->mMissionActive=tmpMission;
                    //since it is a new enter, init control
                    stateMachine->mMissionActive->Init();
                    //will be run on next pass
                }
            }
        }
        else
        {
             cout << "StateMachine: No Missions have been added yet\n";
        }
        std::cout.flush();
        stateMachine->mMutex.Unlock();
        CxUtils::Time::Stamp timeDiffMs=CxUtils::GetTimeMs() - startTimeMs;

        int sleepMs = (int)(stateMachine->mDelayTimeMs - timeDiffMs);
        if(sleepMs < 1)
        {
            sleepMs = 1;
        }
        //std::cout <<  "Sleep: " << sleepMs << " Diff: " << timeDiffMs << std::endl;
        CxUtils::SleepMs((unsigned int)sleepMs);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds a mission to the state machine.
///
///  \param[in] inMission Mission to add to state machine.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool StateMachine::AddMission(Mission *inMission)
{
    bool ret=true;
    mMutex.Lock();
    if(inMission==NULL)
    {
        cout << "StateMachine: passed corrupt mission to add\n";
        mMutex.Unlock();
        return false;
    }
    //no longer a max, make sure you don't do anything stupid

    //first mission, special case, now the starting mission
    if(mNumMissions==0)
    {
        mMissionActive=inMission;
    }

    cout << "StateMachine: Added mission: " << inMission->GetName() << endl;

    //Set the mission that is actually exists
    mMissionList[inMission->GetName()]=inMission;
    mNumMissions++;


    mMutex.Unlock();
    return ret;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Given a string of mission names, does lookup and adds from
///          global list of mission in factory for you.  This is useful when
///          loading a list of missions to run from an XML file.
///
///   \param[in] missions List of missions to add from factory.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool StateMachine::AddMissionsFromFactory(const std::vector<std::string>& missions)
{
    std::vector<std::string>::const_iterator missionName;
    std::map<std::string, Mission*>::iterator mission;
    unsigned int count = 0;
    for(missionName = missions.begin();
        missionName != missions.end();
        missionName++)
    {
        mission = mMissionFactory.find(*missionName);
        if(mission != mMissionFactory.end())
        {
            AddMission(mission->second);
            count++;
        }
    }
    if(count == (unsigned int)missions.size())
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Pushes mission onto the stack for the state machine.
///
///  \param[in] inMission Mission to push onto statck.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool StateMachine::PushMission(Mission *inMission)
{
    if(inMission == NULL)
    {
        cout << "StateMachine::ERROR push passed corrupt mission\n";
        return false;
    }

    //look for mission, if already in there skip
    //switch up to iterators later
    std::vector<Mission*>::iterator m;
    for(m = mMissionStack.begin(); m != mMissionStack.end(); m++)
    {
        if((*m) == inMission)
        {
            // Already in this mission, not an error, but should not enter again
            return true;
        }
    }

    //not in stack, add to the back
    mMissionStack.push_back(inMission);
    mNumStack++;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Removes mission from the stack, returning a pointer.
///
////////////////////////////////////////////////////////////////////////////////////
Mission *StateMachine::PopMission()
{
    if(mNumStack<=0)
    {
        cout << "StateMachine: stack is empty, and a pop was called\n";
        return NULL;
    }
    else
    {
        mNumStack--;
        Mission *retMission=mMissionStack.back();
        mMissionStack.pop_back();
        return retMission;
    }
}


/*  End of File */
