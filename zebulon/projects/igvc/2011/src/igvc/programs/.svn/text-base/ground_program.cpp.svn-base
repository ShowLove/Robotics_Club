////////////////////////////////////////////////////////////////////////////////////
///
/// \file ground_program.h
/// \brief Runs the main IGVC program for 2011.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu<br>
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
#include "igvc/ground/ground.h"

using namespace Zebulon;

std::string gSettingsFile = "settings/ground.xml";
Zebulon::IGVC::StateMachine* gpStateMachine;    ///< State Machine

int main(int argc, char* argv[])
{
    Cartographer::Path path;

    path.Push(Cartographer::Pose());

    path.Push(Cartographer::Pose(CxUtils::Point3D(.75, 1, 0)));
    double angle;
    angle = CxUtils::CxToDegrees(path.GetDirection());
    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(1, 1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(1, 0, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(1, -1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(0, -1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(-1, -1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(-1, 0, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(-1, 1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    path.Pop();
    path.Push(Cartographer::Pose(CxUtils::Point3D(-.1, 1, 0)));
    angle = CxUtils::CxToDegrees(path.GetDirection());

    IGVC::Ground ground;

    if(argc > 1)
    {
        gSettingsFile = argv[1];
    }

    std::cout << "Loading XML File : " << gSettingsFile << std::endl;
    if(ground.LoadSettings(gSettingsFile) == false)
    {
        std::cout << "Unable to load settings.\n";
        return 0;
    }

    std::cout << "Setting up JAUS...\n";
    ground.SetupJAUS();
    std::cout << "Success!\n";
    std::cout << "Initializing JAUS...\n";
    if(ground.InitializeJAUS() == false)
    {
        std::cout << "Failed!\n";
        return 0;
    }
    std::cout << "Success!\n";
    // Wait a moment for data subscriptions to be
    // established.
    CxUtils::SleepMs(500);
    if(ground.Start())
    {
        gpStateMachine = new IGVC::StateMachine();
        gpStateMachine->Initialize(ground.GetGlobalInfo(), ground.GetGlobalCommand(), gSettingsFile);

        while(true)
        {
            ground.PrintStatus();
            if(ground.IsSimulation())
            {
                // Get key presses.
                int key = CxUtils::GetChar();
                if(key > 0)
                {
                    switch(key)
                    {
                    // P
                    case 112:
                        if(ground.GetLogReader()->IsPaused())
                        {
                            ground.GetLogReader()->Pause(false);
                        }
                        else
                        {
                            ground.GetLogReader()->Pause(true);
                        }
                        break;
                    default:
                        std::cout << "Pressed key: " << key << std::endl;
                        break;
                    }
                }
            }
            CxUtils::SleepMs(500);
        }
    }

    ground.ShutdownJAUS();

    return 0;
}

