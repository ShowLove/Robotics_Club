/////////////////////////////////////////////////////////////////////////////
///
///  \file globalcommand.h
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
#ifndef __ZEBULON_AI_GLOBAL_COMMAND__H
#define __ZEBULON_AI_GLOBAL_COMMAND__H

#include "globalinfo.h"
#include <tinyxml/tinyxml.h>
#include <xmlconfig/xmlconfigurator.h>

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GlobalCommand
        ///  \brief Interface class for controlling a robot.  This interface is used
        ///         by the state machine for closed loop control (e.g. heading control, 
        ///         driving to waypoints).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  GlobalCommand : public JAUS::SubsystemCommand
        {
            public:
                /** Command mode types (drive modes) */
                enum CommandMode
                {
                    Velocity = 0,
                    Power,
                    Position,
                    GlobalPosition,
                    NoControl = -1
                };
                GlobalCommand();
                virtual ~GlobalCommand();
                // Optional method to load settings from an XML file.
                virtual bool LoadSettings(const std::string& xmlFilename="");
                // Gives a pointer to the info class and starts the thread
                virtual bool Initialize(GlobalInfo* globalInfo, unsigned int delayTimeMs = 50);
                // Prints any status to console.
                virtual void PrintStatus() const;
                // Shutsdown control.
                virtual void Shutdown();
                //=======================================
                //   Start of JAUS Related Methods
                //=======================================
                // This method is called periodically, overload to perform periodic checks, events, etc.
                virtual void CheckServiceStatus(const unsigned int timeSinceLastUpdateMs) { /* Do whatever want, when called. */ }
                //=======================================
                //   End of JAUS Related Methods
                //=======================================
            protected:
                GlobalInfo* mGlobalInfo;                    ///<  Pointer to global info data.
                unsigned int mDelayTimeMs;                  ///<  Delay in ms between generating command events.
                virtual void CommandThread() = 0;           ///<  Does actual commanding, inherit off of this
                static void StarterThread(void* args);      ///<  Just starts the command thread for children
                CxUtils::Thread mCommandThread;             ///<  Thread object, runs command thread.
                Configuration::XmlConfigurator mXmlConfig;  ///<  XML Configuration object.
        };
    }
}
#endif
