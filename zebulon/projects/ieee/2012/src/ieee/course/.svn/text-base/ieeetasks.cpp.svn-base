/////////////////////////////////////////////////////////////////////////////////////
///
/// \file ieeevision.cpp
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

#include "ieee/course/ieeetasks.h"

using namespace Zebulon;
using namespace IEEE;

#define TASKS_DEBUG

Tasks::Tasks()
{
    mRun = false;
}

Tasks::~Tasks()
{

}

bool Tasks::Initialize(Configuration::XmlConfigurator* configurator)
{
    int _debounceLimit;
    configurator->GetVar("IEEEVision.Tasks.Debounce@Limit", _debounceLimit, false);

    bool _debounceReset;
    configurator->GetVar("IEEEVision.Tasks.Debounce@ResetOnMiss", _debounceReset, false);

    for (int i=0; i<8; i++)
    {
        mTaskDebounce[i].Initialize(_debounceLimit, _debounceReset);
    }

    configurator->GetVar("IEEEVision.Tasks.LapCompleteDebounce@Limit", _debounceLimit, false);
    configurator->GetVar("IEEEVision.Tasks.LapCompleteDebounce@ResetOnMiss", _debounceReset, false);
    mDoneDebounce.Initialize(_debounceLimit, _debounceReset);

    // 1

    configurator->GetVar("IEEEVision.Course.Task1Left@x", mBoundingBoxes[Task1Left].x, false);
    std::cout << " -- " << mBoundingBoxes[Task1Left].x << std::endl;
    configurator->GetVar("IEEEVision.Course.Task1Left@y", mBoundingBoxes[Task1Left].y, false);
    configurator->GetVar("IEEEVision.Course.Task1Left@width", mBoundingBoxes[Task1Left].width, false);
    configurator->GetVar("IEEEVision.Course.Task1Left@height", mBoundingBoxes[Task1Left].height, false);

    configurator->GetVar("IEEEVision.Course.Task1Right@x", mBoundingBoxes[Task1Right].x, false);
    configurator->GetVar("IEEEVision.Course.Task1Right@y", mBoundingBoxes[Task1Right].y, false);
    configurator->GetVar("IEEEVision.Course.Task1Right@width", mBoundingBoxes[Task1Right].width, false);
    configurator->GetVar("IEEEVision.Course.Task1Right@height", mBoundingBoxes[Task1Right].height, false);

    // 2

    configurator->GetVar("IEEEVision.Course.Task2Left@x", mBoundingBoxes[Task2Left].x, false);
    configurator->GetVar("IEEEVision.Course.Task2Left@y", mBoundingBoxes[Task2Left].y, false);
    configurator->GetVar("IEEEVision.Course.Task2Left@width", mBoundingBoxes[Task2Left].width, false);
    configurator->GetVar("IEEEVision.Course.Task2Left@height", mBoundingBoxes[Task2Left].height, false);

    configurator->GetVar("IEEEVision.Course.Task2Right@x", mBoundingBoxes[Task2Right].x, false);
    configurator->GetVar("IEEEVision.Course.Task2Right@y", mBoundingBoxes[Task2Right].y, false);
    configurator->GetVar("IEEEVision.Course.Task2Right@width", mBoundingBoxes[Task2Right].width, false);
    configurator->GetVar("IEEEVision.Course.Task2Right@height", mBoundingBoxes[Task2Right].height, false);

    // 3

    configurator->GetVar("IEEEVision.Course.Task3Left@x", mBoundingBoxes[Task3Left].x, false);
    configurator->GetVar("IEEEVision.Course.Task3Left@y", mBoundingBoxes[Task3Left].y, false);
    configurator->GetVar("IEEEVision.Course.Task3Left@width", mBoundingBoxes[Task3Left].width, false);
    configurator->GetVar("IEEEVision.Course.Task3Left@height", mBoundingBoxes[Task3Left].height, false);

    configurator->GetVar("IEEEVision.Course.Task3Right@x", mBoundingBoxes[Task3Right].x, false);
    configurator->GetVar("IEEEVision.Course.Task3Right@y", mBoundingBoxes[Task3Right].y, false);
    configurator->GetVar("IEEEVision.Course.Task3Right@width", mBoundingBoxes[Task3Right].width, false);
    configurator->GetVar("IEEEVision.Course.Task3Right@height", mBoundingBoxes[Task3Right].height, false);

    // 4

    configurator->GetVar("IEEEVision.Course.Task4Left@x", mBoundingBoxes[Task4Left].x, false);
    configurator->GetVar("IEEEVision.Course.Task4Left@y", mBoundingBoxes[Task4Left].y, false);
    configurator->GetVar("IEEEVision.Course.Task4Left@width", mBoundingBoxes[Task4Left].width, false);
    configurator->GetVar("IEEEVision.Course.Task4Left@height", mBoundingBoxes[Task4Left].height, false);

    configurator->GetVar("IEEEVision.Course.Task4Right@x", mBoundingBoxes[Task4Right].x, false);
    configurator->GetVar("IEEEVision.Course.Task4Right@y", mBoundingBoxes[Task4Right].y, false);
    configurator->GetVar("IEEEVision.Course.Task4Right@width", mBoundingBoxes[Task4Right].width, false);
    configurator->GetVar("IEEEVision.Course.Task4Right@height", mBoundingBoxes[Task4Right].height, false);

    // Starting Area
    configurator->GetVar("IEEEVision.Course.Start@x", mStartingArea.x, false);
    configurator->GetVar("IEEEVision.Course.Start@y", mStartingArea.y, false);
    configurator->GetVar("IEEEVision.Course.Start@width", mStartingArea.width, false);
    configurator->GetVar("IEEEVision.Course.Start@height", mStartingArea.height, false);

    NewRun();

    return true;
}

void Tasks::NewRun()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    if (mRun == false)
    {
        mScore = 0;
        mLap = 0;

        mMoved = false;
        mRun = false;

        mCurrentLap = CourseConfig();

        for (int i=0; i<4; i++)
        {
            mTaskCompleted[i] = false;
        }

        for (int i=0; i<8; i++)
            mTaskDebounce[i].ResetDebounce();

        mDoneDebounce.ResetDebounce();

        mCorrect = mIncorrect = 0;
    }
}

void Tasks::Start()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    if (mRun == false)
    {
        mScore = 0;
        mLap = 0;

        mMoved = false;
        mRun = false;

        mCurrentLap = CourseConfig();

        std::cout << std::endl << "Next Lap" << std::endl;
        std::cout << "Voltage     (Yellow) " << ((mCurrentLap.CorrectTask[0]%2)? "Right" : "Left") << std::endl
                  << "Capacitance (Blue)   " << ((mCurrentLap.CorrectTask[1]%2)? "Right" : "Left") << std::endl
                  << "Temperature (Green)  " << ((mCurrentLap.CorrectTask[2]%2)? "Right" : "Left") << std::endl
                  << "Waveform    (Red)    " << ((mCurrentLap.CorrectTask[3]%2)? "Right" : "Left") << std::endl;//*/

        for (int i=0; i<4; i++)
        {
            mTaskCompleted[i] = false;
        }

        for (int i=0; i<8; i++)
            mTaskDebounce[i].ResetDebounce();

        mDoneDebounce.ResetDebounce();

        mCorrect = mIncorrect = 0;

        mRun = true;

        mStartTimeMs = CxUtils::GetTimeMs();
        mTimeMs = 0;

        /*if (!mSimulate)
            std::cout << "Voltage     (Yellow) " << ((mCurrentLap.CorrectTask[0]%2)? "Left" : "Right") << std::endl
                      << "Capacitance (Blue)   " << ((mCurrentLap.CorrectTask[1]%2)? "Left" : "Right") << std::endl
                      << "Temperature (Green)  " << ((mCurrentLap.CorrectTask[1]%2)? "Left" : "Right") << std::endl
                      << "Waveform    (Red)    " << ((mCurrentLap.CorrectTask[1]%2)? "Left" : "Right") << std::endl;//*/

        /*for (int i=0; i<4; i++)
        {
            mTaskCompleted[i] = false;
        }

        for (int i=0; i<8; i++)
            mTaskDebounce[i].ResetDebounce();

        mDoneDebounce.ResetDebounce();

        mCorrect = mIncorrect = 0;

        mStartTimeMs = CxUtils::GetTimeMs();
        mTimeMs = 0;//*/
    }
}

void Tasks::Stop()
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    mRun = false;
    mTimeMs = (int)(240000 - (CxUtils::GetTimeMs() - mStartTimeMs));
}

void Tasks::Update(std::vector<Vision::Return> trackedObjects, bool simulate)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);

    mSimulate = simulate;

    if (mRun == false)
        return;

    mTimeMs = (int)(240000 - (CxUtils::GetTimeMs() - mStartTimeMs));
    if (mTimeMs < 0)
    {
        std::cout << "Time is up!" << std::endl;

        mRun = false;
        mTimeMs = 0;
        return;
    }

    for (int i=0; i<4; i++)
        VisitingTask[i] = false;

    for (int i=0; i<8; i++)
    {
        bool overlap=false;
        std::vector<Vision::Return>::iterator iter;
        for (iter=trackedObjects.begin(); iter!=trackedObjects.end(); iter++)
        {
            if (!mMoved)
            {
                if (!BoxesOverlap(mStartingArea, *iter))
                {
                    mMoved = true;
                    mScore += 100;
#ifdef TASKS_DEBUG
                    if (!simulate)
                        std::cout << "Left start, Score: " << mScore << std::endl;
#endif
                }
            }

            overlap = overlap || BoxesOverlap(mBoundingBoxes[i], *iter);
        }

        if (overlap)
        {
            VisitingTask[(int) i/2] = true;
            mOverlap[i] = true;
        }
        else
        {
            mOverlap[i] = false;
        }


        if (!mTaskCompleted[(int) i/2])
        //if (mCurrentLap.TraversedTask[(int) i/2] != -1)
        {
            if(mTaskCompleted[(int) i/2] = mTaskDebounce[i].Bounce(overlap))
            {
                // Task has just been completed, check score and stuff based on things
                mCurrentLap.TraversedTask[(int) i/2] = (Task)i;

                if (mCurrentLap.TraversedTask[(int) i/2] == mCurrentLap.CorrectTask[(int) i/2])
                {
                    mScore += 10;
                    mCorrect++;
#ifdef TASKS_DEBUG
                    if (!simulate)
                        std::cout << "Completed task " << (i/2) << " Correctly (did " << mCurrentLap.CorrectTask[(int) i/2] << ") Score: " << mScore << std::endl;
#endif
                }
                else
                {
                    mScore -= 10;
                    mIncorrect++;
#ifdef TASKS_DEBUG
                    if (!simulate)
                        std::cout << "Completed task " << (i/2) << " Incorrectly (did " << mCurrentLap.TraversedTask[(int) i/2] << " act " << mCurrentLap.CorrectTask[(int) i/2] << ") Score: " << mScore << std::endl;
#endif
                }
            }
        }
    }

    if (mDoneDebounce.Bounce(mTaskCompleted[0] && mTaskCompleted[1] &&
                             mTaskCompleted[2] && mTaskCompleted[3] &&
                                !( VisitingTask[0] || VisitingTask[1]
                                || VisitingTask[2] || VisitingTask[3] )))
    {
#ifdef TASKS_DEBUG
        if (!simulate)
        {
            std::cout << "Completed Lap " << mLap << std::endl
                      << mCurrentLap.CorrectTask[0] << ' ' << mCurrentLap.TraversedTask[0] << std::endl
                      << mCurrentLap.CorrectTask[1] << ' ' << mCurrentLap.TraversedTask[1] << std::endl
                      << mCurrentLap.CorrectTask[2] << ' ' << mCurrentLap.TraversedTask[2] << std::endl
                      << mCurrentLap.CorrectTask[3] << ' ' << mCurrentLap.TraversedTask[3] << std::endl;


        }
#endif
        mCourseHistory.push_back(mCurrentLap);
        mCurrentLap = CourseConfig();

        std::cout << std::endl << "Next Lap" << std::endl;
        std::cout << "Voltage     (Yellow) " << ((mCurrentLap.CorrectTask[0]%2)? "Right" : "Left") << std::endl
                  << "Capacitance (Blue)   " << ((mCurrentLap.CorrectTask[1]%2)? "Right" : "Left") << std::endl
                  << "Temperature (Green)  " << ((mCurrentLap.CorrectTask[2]%2)? "Right" : "Left") << std::endl
                  << "Waveform    (Red)    " << ((mCurrentLap.CorrectTask[3]%2)? "Right" : "Left") << std::endl;//*/

        /*std::cout << "Voltage     (Yellow) " << ((mCurrentLap.CorrectTask[0]%2)? "Left" : "Right") << std::endl
                  << "Capacitance (Blue)   " << ((mCurrentLap.CorrectTask[1]%2)? "Left" : "Right") << std::endl
                  << "Temperature (Green)  " << ((mCurrentLap.CorrectTask[2]%2)? "Left" : "Right") << std::endl
                  << "Waveform    (Red)    " << ((mCurrentLap.CorrectTask[3]%2)? "Left" : "Right") << std::endl;//*/

        for (int i=0; i<4; i++)
        {
            mTaskCompleted[i] = false;
        }

        for (int i=0; i<8; i++)
            mTaskDebounce[i].ResetDebounce();

        mDoneDebounce.ResetDebounce();
        mLap++;
    }
}

bool Tasks::BoxesOverlap(const cv::Rect& rect, const Vision::Return& ret)
{
    return rect.x < ret.mMaxI && rect.x+rect.width > ret.mMinI &&
           rect.y < ret.mMaxJ && rect.y+rect.height > ret.mMaxJ;
}

void Tasks::DrawDebug(cv::Mat& image)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex);
    for (int i=0; i<8; i++)
    {
        if (mOverlap[i])
            cv::rectangle(image, mBoundingBoxes[i], cv::Scalar(255,255,0));
        else
            cv::rectangle(image, mBoundingBoxes[i], cv::Scalar(0,255,255));
    }
}

/* End of file */
