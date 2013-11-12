/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ieeevision.h
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

#ifndef __ZEBULON_IEEE_2012_TASKS_
#define __ZEBULON_IEEE_2012_TASKS_

#include "opencv/cv.h"
#include "opencv/highgui.h"

#include <string>

#include "cxutils/cxutils.h"
#include "xmlconfig/xmlconfigurator.h"

#include "gvision/segment.h"

//#include "zebulon/statemachine/utility/debounce.h"
#include "statemachine/utility/debounce.h"

namespace Zebulon
{
    namespace IEEE
    {
        enum Task
        {
            TaskNULL = -1,

            Task1Left  = 0,
            Task1Right = 1,

            Task2Left,
            Task2Right,

            Task3Left,
            Task3Right,

            Task4Left,
            Task4Right
        };

        class CourseConfig
        {
        public:
            CourseConfig(int lap=0)
            {
                for (int i=0; i<4; i++)
                {
                    CorrectTask[i] = (Task)(i*2+(rand()%2));
                    TraversedTask[i] = TaskNULL;
                }

                mLap=lap;
            }

            CourseConfig(const CourseConfig& other)
            {
                *this = other;
            }

            CourseConfig& operator=(const CourseConfig& other)
            {
                for (int i=0; i<4; i++)
                {
                    CorrectTask[i] = other.CorrectTask[i];
                    TraversedTask[i] = other.TraversedTask[i];
                }

                mLap = other.mLap;

                return *this;
            }

            Task CorrectTask[4];
            Task TraversedTask[4];
            int mLap;
        };

        class Tasks
        {
        public:
            struct Stats
            {
                Stats()
                {
                    mScore = 0;
                    mCorrect = 0;
                    mIncorrect = 0;
                    mTimeMs = 0;
                    mLaps = 0;
                }

                Stats(const Stats& other)
                {
                    *this = other;
                }

                Stats operator=(const Stats& other)
                {
                    mScore = other.mScore;
                    mCorrect = other.mCorrect;
                    mIncorrect = other.mIncorrect;
                    mTimeMs = other.mTimeMs;
                    mLaps = other.mLaps;

                    return *this;
                }

                std::string toString()
                {
                    std::stringstream ss;
                    ss << "Stats: Score " << mScore
                       << " Correct " << mCorrect
                       << " Incorrect " << mIncorrect
                       << " Time " << mTimeMs
                       << " Laps " << mLaps;
                    return ss.str();
                }

                int mScore;
                int mCorrect;
                int mIncorrect;
                int mTimeMs;
                int mLaps;
            };

            Tasks();
            ~Tasks();

            bool Initialize(Configuration::XmlConfigurator* configurator);

            void Update(std::vector<Vision::Return> trackedObjects, bool simulate=false);

            void NewRun();
            bool IsRunning()
            {
                return mRun;
            }

            void Start();
            void Stop();

            Stats GetStats()
            {
                Stats s;

                //CxUtils::Mutex::ScopedLock lock(&mMutex);
                mMutex.Lock();
                s.mScore = mScore;
                s.mCorrect = mCorrect;
                s.mIncorrect = mIncorrect;
                s.mTimeMs = mTimeMs;
                s.mLaps = mLap;
                mMutex.Unlock();

                return s;
            }

            int GetScore() { return mScore; }

            std::vector<CourseConfig> GetCourseHistory()
            {
                std::vector<CourseConfig> history;

                mMutex.Lock();
                history = mCourseHistory;
                mMutex.Unlock();

                return history;
            }

            void GetCurrentLap(CourseConfig& lap)
            {
                CxUtils::Mutex::ScopedLock lock(&mMutex);
                lap = mCurrentLap;
            }

            int GetLaps() { return mLap; }

            void DrawDebug(cv::Mat& image);

        protected:
            bool BoxesOverlap(const cv::Rect& rect, const Vision::Return& ret);

            void ResetScore() { mScore = 0; }
            void ResetLaps() { mLap = 0; }

            bool mRun;

            int mScore;
            int mCorrect;
            int mIncorrect;
            int mLap;

            bool mSimulate; // reduces verbosity when doing manual judge mode

            long long int mStartTimeMs;
            int mTimeMs;

            bool mTaskCompleted[4];
            Zebulon::AI::Debounce mTaskDebounce[8];
            cv::Rect mBoundingBoxes[8];
            bool mOverlap[8];
            Zebulon::AI::Debounce mDoneDebounce;

            cv::Rect mStartingArea;

            std::vector<CourseConfig> mCourseHistory;
            CourseConfig mCurrentLap;

            CxUtils::Mutex mMutex;

            bool mMoved;
            bool VisitingTask[4];
        };
    }
}


#endif // __ZEBULON_IEEE_2012_TASKS_
/* End of file */
