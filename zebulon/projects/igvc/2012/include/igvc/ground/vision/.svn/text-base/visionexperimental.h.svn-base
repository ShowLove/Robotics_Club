////////////////////////////////////////////////////////////////////////////////////
///
/// \file groundvision.h
/// \brief Functions for doing vision processing for the ground vehicle.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_GROUND_VISION_EXPERIMENTAL__H
#define __ZEBULON_GROUND_VISION_EXPERIMENTAL__H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/math/vector3d.h>
#include <cxutils/math/segment3d.h>
#include <xmlconfig/xmlconfigurator.h>
#include <string>
#include <vector>

#include <boost/thread.hpp>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GroundVisionExperimental
        ///  \brief Experimental functions for IGVC vision processing.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GroundVisionExperimental
        {
        public:

            class GVESubscriber
            {
            public:
                virtual void GVECallback(std::vector<CxUtils::Segment3D> lines)=0;
            };

            GroundVisionExperimental();
            ~GroundVisionExperimental();

            void LoadSettings();

            std::vector<CxUtils::Segment3D> FindLaneLinesMeanShift(const cv::Mat imgIn,
                                                                 cv::Mat& imgOut, int rows = 3, int cols = 3);

            void Subscribe(GVESubscriber* subscriber)
            {
                mMutex.lock();
                mSubscribers.insert(subscriber);
                mMutex.unlock();
            }

            void Unsubscribe(GVESubscriber* subscriber)
            {
                mMutex.lock();
                mSubscribers.erase(subscriber);
                mMutex.unlock();
            }

        private:
            cv::MatND mHist;
            cv::Mat* mLineBackProject;
            std::set<GVESubscriber*> mSubscribers;

            boost::shared_mutex mMutex;
        };
    }
}

#endif // __ZEBULON_GROUND_VISION_EXPERIMENTAL__H
/* End of file */
