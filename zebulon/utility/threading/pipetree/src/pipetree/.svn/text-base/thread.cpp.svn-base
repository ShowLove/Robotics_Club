////////////////////////////////////////////////////////////////////////////////////
///
/// \file thread.cpp
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

#include "pipetree/thread.h"

using namespace PipeTree;

#define YIELD_THRESH 2

void Thread::ProcessThread()
{
    Branch* job=NULL;
    int yield=0;

    while (!mTerminate)
    {
        while (mRun)
        {
            if (mLocalJobCounter != GetRemoteJobCount())
            {
                mLocalJobCounter = GetRemoteJobCount();
            }

            yield=0;
            while (!mTerminate && mRun && !mBranchQueue->pop_front(job))
            {
                if (Terminal::GetInstance().IsReady())
                {
                    if (Terminal::GetInstance().Combine())
                    {
                        Terminal::GetInstance().Reset();

                        /*int count = *mRemoteJobCounter;
                        do
                        {
                            count = *mRemoteJobCounter;
                        } while(!__sync_bool_compare_and_swap(mRemoteJobCounter, count, count+1));//*/
                    }
                    continue;
                }

                if (yield++ > YIELD_THRESH)
                {
                    pthread_yield();
                    yield=0;
                }
            }

            if (Terminal::GetInstance().IsReady())
            {
                if (Terminal::GetInstance().Combine())
                {
                    Terminal::GetInstance().Reset();

                    /*int count = *mRemoteJobCounter;
                    do
                    {
                        count = *mRemoteJobCounter;
                    } while(!__sync_bool_compare_and_swap(mRemoteJobCounter, count, count+1));
                    mBranchQueue->clear();//*/
                }
                continue;
            }

            /*if (GetRemoteJobCount() != mLocalJobCounter)
            {
                mLocalJobCounter = GetRemoteJobCount();
            }
            else//*/
            if (job != NULL)
            {
                const cv::Mat mat = job->Process();

                job->Reset();

                std::vector<Branch*> ready = job->WriteToChildren(mat);

                std::vector<Branch*>::iterator iready;
                for (iready=ready.begin(); iready!=ready.end(); iready++)
                    mBranchQueue->push_back(*iready);
            }
        }
    }
}
