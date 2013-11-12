/////////////////////////////////////////////////////////////////////////////
///
///  \file globalcommand.cpp
///  \brief Class for controlling all physical aspects of the robot.
///
///  Author(s): Gary Stein, Daniel Barber<br>
///  Created: 2008<br>
///  Copyright (c) 2010<br>
///  Robotics Laboratory and Club<br>
///  University of Central Florida (UCF) <br>
///  Email: gstein@mail.ucf.edu, dbarber@ist.ucf.edu<br>
///  Web: http://robotics.ucf.edu <br>
///  All Rights Reserved <br>
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
#include "statemachine/globalcommand.h"

#include <iostream>

using namespace Zebulon;
using namespace AI;

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Constructor, initializes default values.
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::GlobalCommand() : JAUS::SubsystemCommand(JAUS::Service::ID("GlobalCommand"))
{
    mGlobalInfo = NULL;
    mDelayTimeMs = 50;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
GlobalCommand::~GlobalCommand()
{
    Shutdown();
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Initialize, must be called to initialize class and passed global info.
///
///  \param[in] globalInfo global info class that we will be getting feedback from
///  \param[in] delayTimeMs frequency between sending all commands.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool GlobalCommand::Initialize(GlobalInfo* globalInfo, unsigned int delayTimeMs)
{
    mDelayTimeMs = delayTimeMs;

    if (globalInfo == NULL)
    {
        std::cout << "Global command failed, passed invalid GlobalInfo pointer" << std::endl;
        return false;
    }

    mGlobalInfo = globalInfo;
    return mCommandThread.CreateThread(&GlobalCommand::StarterThread, this) > 0 ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread responsible for sending out commands (i.e. JAUS messages).
///
///	 \param[in] args This global command class.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::StarterThread(void* args)
{
    GlobalCommand* globalCommand = (GlobalCommand*)args;
    globalCommand->CommandThread();
}

/*  EXAMPLE 
////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Thread responsible for sending out Jaus commands.
///
///	 \param[in] args This global command class.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::CommandThread()
{
    //This is an skeleton example, copy and past this, fill in the while loop
    while (mCommandThread.QuitThreadFlag() == false)
    {
        // Generate commands!
        CxUtils::SleepMs(mDelayTimeMs);
    }
    Shutdown();
}

*/


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Prints status of important variables.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::PrintStatus() const
{
    std::cout << "\nGlobal Command ==============================================================\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Shutsdown threads and releases control.
///
////////////////////////////////////////////////////////////////////////////////////
void GlobalCommand::Shutdown()
{
    mCommandThread.StopThread(500);
    mCommandThread.KillThread();
    
    JAUS::SubsystemCommand::Shutdown();

}

/** Optional method to load settings from XML. */
bool GlobalCommand::LoadSettings(const std::string& xmlFilename)
{
    return mXmlConfig.Load(xmlFilename,"GlobalCommand");
}

/*end of file*/
