////////////////////////////////////////////////////////////////////////////////////
///
/// \file branch.hpp
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

//#include <stdatomic.h>

#include "pipetree/pipetree.h"

#ifndef ___TREEPROCESS_BRANCH_H_
#define ___TREEPROCESS_BRANCH_H_

#include <set>
#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/shared_mutex.hpp>

#include <opencv/cv.h>

#include "pipetree/lockfree/stringmap.hpp"
#include "pipetree/lockfree/queue.hpp"

namespace PipeTree
{
    //class Lockfree::StringMap<const cv::Mat>;

    class Branch
    {
    public:
        virtual cv::Mat Process() const = 0;

        std::vector<Branch*> WriteToChildren(const cv::Mat& value)
        {
            std::vector<Branch*> readyChildren;

            mChildLock.lock_shared();
            std::set<Pair<Branch, std::string> >::const_iterator iter;
            for (iter=mChildren.begin(); iter!=mChildren.end(); iter++)
            {
                iter->first->Set(iter->second, value);
                if (iter->first->IsReady())
                    readyChildren.push_back(iter->first);
            }
            mChildLock.unlock_shared();

            return readyChildren;
        }

        std::set<Branch*> CollectChildren()
        {
            std::set<Branch*> branches;

            mChildLock.lock_shared();
            std::set<Pair<Branch, std::string> >::const_iterator iter;
            for (iter=mChildren.begin(); iter!=mChildren.end(); iter++)
            {
                branches.insert(iter->first);
                std::set<Branch*> grandChildren = iter->first->CollectChildren();
                branches.insert(grandChildren.begin(), grandChildren.end());
            }
            mChildLock.unlock_shared();

            return branches;
        }

        void AddChild(Branch* child, const std::string& argName)
        {
            Pair<Branch, std::string> p;
            p.first = child;
            p.second = argName;

            child->NotifyChild(); // Let the child know it's been added
                                  //  useful to know how many parents you have

            mChildLock.lock();
            mChildren.insert(p);
            mChildLock.unlock();
        }

        virtual void Set(std::string argName, const cv::Mat& value)
        {
            mArguments.put(argName, value);
        }

        virtual void Reset()
        {
            mArguments.clear();
        }

        /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
        /// \brief This function tells if the particular branch is ready
        ///         or not. Uses a read lock, so it will block if new argument
        ///         names are added.
        /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// /// ///
        virtual bool IsReady()
        {
            bool ret=true;
            mNameLock.lock_shared();
            std::set<std::string>::const_iterator iter;
            for (iter=mNames.begin(); iter!=mNames.end(); iter++)
            {
                std::string name = *iter;
                cv::Mat mat;
                ret = ret && !Get(*iter).empty();
                //ret = ret && mArguments.get(name, mat);
            }
            mNameLock.unlock_shared();

            return ret;
        }


    protected:
        const cv::Mat Get(const std::string& argName) const
        {
            /*const cv::Mat value;
            while ((value = mArguments.get(argName)) == NULL)
            {
                pthread_yield();
            }//*/

            cv::Mat value;

            if (mNames.find(argName) != mNames.end())
                mArguments.get(argName, value);

            return value;
        }

        void AddArgument(const std::string& argName)
        {
            mNameLock.lock();
            mNames.insert(argName);
            mNameLock.unlock();
        }

        virtual void NotifyChild()
        {
        }

    private:
        Lockfree::StringMap<cv::Mat> mArguments;

        std::set<std::string> mNames;
        boost::shared_mutex mNameLock;

        std::set<Pair<Branch, std::string> > mChildren;
        boost::shared_mutex mChildLock;

        int mJobCounter;
    };
}

#endif // ___TREEPROCESS_BRANCH_H_
/* End of file */
