////////////////////////////////////////////////////////////////////////////////////
///
/// \file mission.cpp
/// \brief Abstract mission interface for developing missions for state machine.
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
#include "statemachine/mission.h"
#include <tinyxml/tinyxml.h>

using namespace Zebulon;
using namespace AI;
using namespace std;

const std::string Mission::KeepRunning = "KeepRunning";

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
Mission::Mission()
{
    ID="Default";
    mTicks=0;
    mState=0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Mission::~Mission()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Name of the mission (User Defined).
///
////////////////////////////////////////////////////////////////////////////////////
std::string Mission::GetName() const
{
    return ID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Runs all the steps like ErrorCheck, FetchProcess, etc. and displays
///         run status to console.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Mission::Run()
{
    mTicks++;

    // Error happened, error messing should happen in inherited class
    if(ErrorCheck())
    {
        cout << "Mission (" << ID << "): Major Error Occured\n";
        return KeepRunning;
    }
    if(FetchProcess())
    {
        cout << "Mission (" << ID << "): Fetch or Process Error Occured\n";
        return KeepRunning;
    }
    std::string tmp=ExitEarly();
    if(tmp != KeepRunning)
    {
        cout << "Mission (" << ID << "): Early Exit Occured\n";
        return tmp;
    }
    // by this time, all normal operations have happened
    // trust whatever Execute happens
    return Execute();

}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initializes timing information for Mission.
///
////////////////////////////////////////////////////////////////////////////////////
int Mission::Init()
{
    mTicks=0;
    mStartMissionTime=CxUtils::Timer::GetTimeSeconds();

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Called on exit, overload for different functionality.
///
////////////////////////////////////////////////////////////////////////////////////
int Mission::Exit()
{
    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sentinal Rule method, overload to signal that your mission needs
///         to take over.  Values are user defined for each mission.  If any
///         mission returns over 0, than it will take control.  If there are
///         multiple mission returning > 0, the highest ranked will win.
///
///  \return A value > 0 to activate mission due to some user defined rules.
///
////////////////////////////////////////////////////////////////////////////////////
double Mission::Sentinal()
{
    return 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return Amount of time in seconds since mission started.
///
////////////////////////////////////////////////////////////////////////////////////
double Mission::GetMissionTime()
{
   return (CxUtils::Timer::GetTimeSeconds()-mStartMissionTime);
}


/*
int Mission::SetGlobalCommand(GlobalCommand *globalCommand)
{
    if(globalCommand==NULL)
    {
        return 0;
    }
    mGlobalCommand=globalCommand;
    return 1;
}
int Mission::SetGlobalInfo(GlobalInfo *globalInfo)
{
    if(globalInfo==NULL)
    {
        return 0;
    }
    mGlobalInfo=globalInfo;
    return 1;
}
*/

Configuration::XmlConfigurator Mission::mXmlConf;

/** Loads your mission XML file from the master file to
    the mXmlConf data member. */
bool Mission::XmlLoad(std::string mainFilename)
{
    return mXmlConf.Load(mainFilename,"Mission");
}


/*  End of File. */

