/////////////////////////////////////////////////////////////////////////////////////
///
/// \file game.h
/// \brief A program to test some opencv algorithms.
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_IEEE_2012_GAME_
#define __ZEBULON_IEEE_2012_GAME_

#include "ieee/cassy/ieeevision.h"

#include "ieee/communication/datasender.h"
#include "ieee/communication/imagesender.h"

#include "ieee/course/ieeetasks.h"

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include "mcu/slipinterface.h"
#include "mcu/messagefactory.h"

#include <stdio.h>
#include <iostream>
#include <fstream>

#ifndef MAX_POINTS
#   define MAX_POINTS 16
#endif

namespace Zebulon
{
    namespace IEEE
    {
        class Game
        {
        public:
            Game();
            ~Game();

            bool Initialize(int index, std::string usbport, std::string server, int dataport, int imageport, bool judgeOverride=false);

            void Shutdown();

            void GetImage(cv::Mat& image)
            {
                //CxUtils::Mutex::ScopedLock lock(&mFrameMutex);
                mFrameMutex.Lock();
                mCurrentFrame.copyTo(image);
                mFrameMutex.Unlock();
            }

            int GetSelectedID()
            {
                return mSelectedID;
            }

            std::string GetWindowName()
            {
                return mWindowName;
            }

        protected:
            CxUtils::Thread mGameThread;
            static void GameThreadCall(void* args);
            void GameThread();

            CxUtils::Thread mServerThread;
            static void ServerThreadCall(void* args);
            void ServerThread();

            CxUtils::Thread mLogThread;
            static void LogThreadCall(void* args);
            void LogThread();

            static void buttonPressed_StartRun(int state, void* user);
            static void buttonPressed_StopRun(int state, void* user);
            static void buttonPressed_NewRun(int state, void* user);
            static void buttonPressed_RandomizeCourse(int state, void* user);
            static void trackChanged_TeamID(int state, void* user);

            cv::Mat mCurrentFrame;
            CxUtils::Mutex mFrameMutex;

            bool mJudgeOverride;
            CourseConfig mJudgeCourseConfig;
            CxUtils::Mutex mJudgeCCMutex;

            //char mWindowName[50];
            std::string mWindowName;

            int mSelectedID;
            volatile int mTeamID;

            int mSelectedRunNumber;
            volatile int mRunNumber;

            // Game Logic Stuff
            int mCameraIndex;
            Tasks mGameLogic;

            float mXList[16];
            float mYList[16];
            int mNumPoints;

            // Network Stuff
            DataSender mSender;
            ImageSender mImageSender;

            CxUtils::Mutex mMutex;

            std::string mServerName;
            int mImagePort;
            int mDataPort;

            // Logger stuff
            std::ofstream mDataFile;
            std::ofstream mRawFile;
            bool Record(int id);

            //MCU stuff
            Zebulon::MCU::SlipInterface mMCU;
        };

    }
}

#endif // __ZEBULON_IEEE_2012_GAME_
/* End of file */
