////////////////////////////////////////////////////////////////////////////////////
///
/// \file terminal.cpp
///
/// Author(s): Michael Scherer<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Email: michael.scherer@ieee.org<br>
/// Web: http://zebulon.svn.sourceforge.net/viewvc/zebulon/v4/software/utility/threading/pipetree/<br>
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

#include "pipetree/terminal.h"

using namespace PipeTree;

Terminal Terminal::mInstance;

cv::Mat Terminal::Process() const
{
    //Combine();
    return cv::Mat();
}

bool Terminal::Combine()
{
    // Only execute if nobody else has this round
    if (!__sync_bool_compare_and_swap(&mClaimed, false, true))
        return false;

    int emptyDebounce=0;

    //std::cout << "Num arguments: " << mArguments.size() << std::endl;
    //std::cout << "Num parents: " << mNumParents << std::endl;

    cv::Mat average;
    cv::Mat mat;
    std::vector<cv::Mat> mats;
    while (emptyDebounce<5)
    {
        if (mArguments.pop_front(mat))
        {
            mats.push_back(mat);
            emptyDebounce=0;
        }
        else
        {
            emptyDebounce++;
        }
    }

    if (mats.size() > 0)
    {
        std::vector<cv::Mat>::iterator i;
        i = mats.begin();
        average = *i * (1.0/mats.size());
        i++;
        for (; i!=mats.end(); i++)
        {
            average = average + *i * (1.0/mats.size());
        }
    }

    //return average;

    this->Reset();

    this->Publish(average);

    return true;
}

void Terminal::Set(std::string argName, const cv::Mat& value)
{
    mArguments.push_back(value);
}

void Terminal::Reset()
{
    mArguments.clear();
    mClaimed = false;
}

///
/// \brief This function tells if the particular branch is ready
///         or not. Uses a read lock, so it will block if new argument
///         names are added.
bool Terminal::IsReady()
{
    if (mTimeout == 0)
    {
        unsigned int _size = mArguments.size();
        if (mNumParents != 0 && mNumParents == _size)
            return true;
        else
            return false;
    }

    boost::posix_time::ptime mst1 = boost::posix_time::microsec_clock::local_time();
    boost::posix_time::time_duration msdiff = mst1 - mStartTime;
    return msdiff.total_milliseconds() > mTimeout;
}

/* End of file */
