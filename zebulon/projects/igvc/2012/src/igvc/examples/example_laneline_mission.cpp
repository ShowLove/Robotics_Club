///////////////////////////////////////////////////////////////////////////////////////
/////
///// \file example_opencv.h
///// \brief A program to test some opencv algorithms.
/////
///// Author(s): David Adams<br>
///// Created: 2011<br>
///// Copyright (c) 2010<br>
///// Robotics Laboratory and Club<br>
///// University of Central Florida (UCF) <br>
///// Email: dadams@ist.ucf.edu <br>
///// Web: http://robotics.ucf.edu <br>
/////
/////  Redistribution and use in source and binary forms, with or without
/////  modification, are permitted provided that the following conditions are met:
/////      * Redistributions of source code must retain the above copyright
/////        notice, this list of conditions and the following disclaimer.
/////      * Redistributions in binary form must reproduce the above copyright
/////        notice, this list of conditions and the following disclaimer in the
/////        documentation and/or other materials provided with the distribution.
/////      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
/////        names of its contributors may be used to endorse or promote products
/////        derived from this software without specific prior written permission.
/////
/////  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
/////  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
/////  WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
/////  DISCLAIMED. IN NO EVENT SHALL UCF BE LIABLE FOR ANY
/////  DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
/////  (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
/////  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
/////  ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
/////  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
/////  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
/////
//////////////////////////////////////////////////////////////////////////////////////
#include "igvc/ground/ground.h"
#include "igvc/ground/vision/groundvision.h"
using namespace Zebulon;
using namespace IGVC;

Zebulon::AI::StateMachine *gStateMachine;
namespace Zebulon
{
    namespace IGVC
    {
        class TestLineMission : public IGVC::Mission
        {
        public:
            /*enum State
            {
                GoToWaypoint,
                NextWaypoint,
                Finish
            };*/

            TestLineMission(){}
            ~TestLineMission(){}

            virtual int ErrorCheck(){ return 0;}
            virtual int FetchProcess(){return 0;}
            virtual std::string ExitEarly()
            {
                return "KeepRunning";
            }
            virtual std::string Execute()
            {
                IplImage * img = NULL;
                IplImage * imgOut = NULL;
                mpGlobalInfo->GetInfo("Forward",img);
                gVision.FindLaneLines(img,imgOut);

                cvShowImage("in", img);
                cvShowImage("out", imgOut);
                cvWaitKey(10);
                cvReleaseImage(&imgOut);
                cvReleaseImage(&img);
                return "KeepRunning";
            }
            virtual int Init(){return 0;}

        private:
            GroundVision gVision;
        };

        class StateMachineTest : public AI::StateMachine
        {
            public:
                StateMachineTest(){}
                virtual ~StateMachineTest(){}
                virtual bool AddMissions()
                {
                    bool missionsResult = true;

                    missionsResult &= AddMission(new TestLineMission());

                    return missionsResult;
                }
        };
    }
}

std::string gSettingsFile = "settings/ground_replay.xml";
int main(int argc, char** argv)
{
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
        gStateMachine = new StateMachine();
        gStateMachine->Initialize(ground.GetGlobalInfo(), ground.GetGlobalCommand(), "settings/ground_replay.xml");
        while(true)
        {
            ground.PrintStatus();

            CxUtils::SleepMs(500);
        }
    }

    ground.ShutdownJAUS();
}
