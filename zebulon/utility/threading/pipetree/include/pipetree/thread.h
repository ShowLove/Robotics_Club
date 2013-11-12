////////////////////////////////////////////////////////////////////////////////////
///
/// \file thread.h
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

#include "pipetree/pipetree.h"

#ifndef ___TREEPROCESS_THREAD_H_
#define ___TREEPROCESS_THREAD_H_

#include <pthread.h>
#include <set>

#include "pipetree/lockfree/queue.hpp"
#include "pipetree/branch.hpp"
#include "pipetree/terminal.h"

namespace PipeTree
{
    class Terminal;

    class Thread
    {
    public:
        Thread()
        {
            mTerminate = false;
        }

        ~Thread()
        {
            Kill();
        }

        void Initialize(Lockfree::Queue<Branch*>* queue, volatile int* jobCounter, int id)
        {
            mID = id;

            mBranchQueue = queue;

            mRemoteJobCounter = jobCounter;
            mLocalJobCounter = 0;

            pthread_create(&mThread, NULL, RunThread, this);
        }

        void Run()
        {
            mRun = true;
        }

        void Stop()
        {
            mTerminate = true;
            mRun = false;
            //mCurrentBranch->Interrupt();
        }

        bool IsStopped()
        {
            return mStopped;
        }

        void Kill()
        {
            Stop();
            pthread_cancel(mThread);
        }

        int GetID()
        {
            return mID;
        }

    protected:
        static void* RunThread(void* t)
        {
            Thread* thread = static_cast<Thread*>(t);
            if (thread)
            {
                thread->ProcessThread();
            }

            return NULL;
        }

        void ProcessThread();

        bool GetTerminateFlag() const
        {
            return mTerminate;
        }

        bool GetRunFlag() const
        {
            return mRun;
        }

        int GetRemoteJobCount() const
        {
            //int val = *mRemoteJobCounter;
            //__sync_synchronize();
            return *mRemoteJobCounter;
        }

        volatile Branch* mCurrentBranch;
        Lockfree::Queue<Branch*>* mBranchQueue;

        //std::atomic<bool> mTerminate;
        volatile bool mTerminate;
        volatile bool mRun;
        volatile bool mStopped;
        int mID;

        volatile int mLocalJobCounter;
        volatile int *mRemoteJobCounter;

        pthread_t mThread;
    };
}

#endif // ___TREEPROCESS_THREAD_H_
/* End of file */
