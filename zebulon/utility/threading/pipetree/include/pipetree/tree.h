////////////////////////////////////////////////////////////////////////////////////
///
/// \file tree.h
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

#ifndef ___TREEPROCESS_TREE_H_
#define ___TREEPROCESS_TREE_H_

#include <pthread.h>
#include <vector>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <opencv/cv.h>

#include "pipetree/lockfree/queue.hpp"

#include "pipetree/thread.h"
#include "pipetree/branch.hpp"
#include "pipetree/terminal.h"

namespace PipeTree
{
    class Tree
    {
    public:
        Tree()
        {
        }

        virtual ~Tree()
        {
            for (unsigned int i=0; i<mThreads.size(); i++)
            {
                mThreads[i]->Kill();
            }
            mThreads.clear();
        }

        void Clear()
        {
            mChildLock.lock();
            std::set<Branch*> branches;

            std::set<Pair<Branch, std::string> >::const_iterator iter;
            for (iter=mChildren.begin(); iter!=mChildren.end(); iter++)
            {
                branches.insert(iter->first);
                std::set<Branch*> grandChildren = iter->first->CollectChildren();
                branches.insert(grandChildren.begin(), grandChildren.end());
            }
            mChildLock.unlock();
        }

        bool Initialize(unsigned int numThreads, int timeout=0)
        {
            mTimeout = timeout;
            mJobCounter = 0;

            for (unsigned int i=0; i<numThreads; i++)
            {
                Thread* t = new Thread();
                t->Initialize(&mBranchQueue, &mJobCounter, i);
                mThreads.push_back(t);
            }

            return true;
        }

        void SetJob(const cv::Mat& job)
        {
            /*int jobid = mJobCounter;
            while (!__sync_bool_compare_and_swap(&mJobCounter, jobid, jobid+1))
                jobid = mJobCounter;//*/

            mJobCounter++;
            __sync_synchronize();

            mBranchQueue.clear();

            mChildLock.lock_shared();
            std::set<Pair<Branch, std::string> >::const_iterator iter;
            for (iter=mChildren.begin(); iter!=mChildren.end(); iter++)
            {
                iter->first->Set(iter->second, job);
                mBranchQueue.push_back(iter->first);
            }
            mChildLock.unlock_shared();

            mStartTime = boost::posix_time::microsec_clock::local_time();
            Terminal::GetInstance().SetTimeout(mTimeout, mStartTime);

            std::vector<Thread*>::iterator t;
            for (t=mThreads.begin(); t!=mThreads.end(); t++)
            {
                (*t)->Run();
            }
        }

        void Stop()
        {
            for (unsigned int i=0; i<mThreads.size(); i++)
            {
                mThreads[i]->Stop();
            }
        }

        void AddChild(Branch* child, std::string argName)
        {
            Pair<Branch, std::string> p;
            p.first = child;
            p.second = argName;

            mChildLock.lock();
            mChildren.insert(p);
            mChildLock.unlock();
        }

    protected:
        Lockfree::Queue<Branch*> mBranchQueue;
        volatile int mJobCounter;

        std::set<Pair<Branch, std::string> > mChildren;
        boost::shared_mutex mChildLock;

        std::vector<Thread*> mThreads;

        int mTimeout;
        boost::posix_time::ptime mStartTime;
    };
}

#endif // ___TREEPROCESS_TREE_H_
/* End of file */
