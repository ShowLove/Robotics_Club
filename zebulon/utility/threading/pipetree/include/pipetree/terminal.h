////////////////////////////////////////////////////////////////////////////////////
///
/// \file terminal.h
/// \brief Class for getting all sensor data required for use
///        by GlobalCommand interface.
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

#ifndef ___TREEPROCESS_TERMINAL_H_
#define ___TREEPROCESS_TERMINAL_H_

#include "pipetree/branch.hpp"
#include "pipetree/lockfree/queue.hpp"

namespace PipeTree
{
    class Terminal : public Branch
    {
        friend class Tree;

    private:
        Terminal()
        {
            mClaimed = false;
        }

        static Terminal mInstance;

    public:
        class TreeCallback
        {
        public:
            virtual void ProcessComplete(const cv::Mat& result, void* user)=0;
        };

        static Terminal& GetInstance()
        {
            return Terminal::mInstance;
        }

        cv::Mat Process() const;

        bool Combine();

        void Set(std::string argName, const cv::Mat& value);

        void Reset();

        ///
        /// \brief This function tells if the particular branch is ready
        ///         or not. Uses a read lock, so it will block if new argument
        ///         names are added.
        bool IsReady();

        void AddSubscriber(TreeCallback* subscriber, void* user)
        {
            Pair<TreeCallback, void*> p;
            p.first = subscriber;
            p.second = user;

            mSubscriberLock.lock();
            mSubscribers.insert(p);
            mSubscriberLock.unlock();
        }

        bool RemoveSubscriber(TreeCallback* subscriber)
        {
            bool ret=false;

            /*mSubscriberLock.lock();
            typename std::set<Pair<TreeCallback, void*> >::iterator i = mSubscribers.find(subscriber);
            if (i!=mSubscribers.end())
            {
                mSubscribers.erase(i);
                ret=true;
            }
            mSubscriberLock.unlock();//*/

            return ret;
        }

        void Publish(const cv::Mat& result)
        {
            mSubscriberLock.lock_shared();
            std::set<Pair<TreeCallback, void*> >::iterator i = mSubscribers.begin();
            for (; i!=mSubscribers.end(); i++)
            {
                i->first->ProcessComplete(result, i->second);
            }
            mSubscriberLock.unlock_shared();
        }

    protected:
        Lockfree::Queue<cv::Mat> mArguments;

        unsigned int mTimeout;
        boost::posix_time::ptime mStartTime;

        void SetTimeout(unsigned int time, boost::posix_time::ptime start)
        {
            mTimeout = time;
            mStartTime = start;
        }

        virtual void NotifyChild()
        {
            unsigned int _size;
            do
            {
                _size = mNumParents;
            } while(!__sync_bool_compare_and_swap(&mNumParents,_size,_size+1));
        }

        volatile bool mClaimed;

        unsigned int mNumParents;

        std::set<Pair<TreeCallback, void*> > mSubscribers;
        boost::shared_mutex mSubscriberLock;
    };
}

#endif // ___TREEPROCESS_TERMINAL_H_
/* End of File */
