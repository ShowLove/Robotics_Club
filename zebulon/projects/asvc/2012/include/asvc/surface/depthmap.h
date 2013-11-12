////////////////////////////////////////////////////////////////////////////////////
///
/// \file depthmap.h
/// \brief Class for creating a depth map image from lidar data.
///
/// Author(s): Jonathan Mohlenhoff, Tuyen Do<br>
/// Created: 2012<br>
/// Copyright (c) 2012<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: jmohlenh@ist.ucf.edu<br>
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
#ifndef __ZEBULON_SURFACE_DEPTHMAP__H
#define __ZEBULON_SURFACE_DEPTHMAP__H

#include <statemachine/globalinfo.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>
#include <opencv2/core/core_c.h>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui_c.h>
//#include <opencv2/opencv.hpp>
#include <opencv2/legacy/legacy.hpp>

#include "asvc/surface/globalinfo.h"
#include "asvc/surface/globalcommand.h"

namespace Zebulon
{
    namespace ASVC
    {
	    class DepthMap : public AI::GlobalInfo::Callback
	    {
	    public:
            DepthMap();
            ~DepthMap();
            
            void Initialize(const string intrFilename,
                            const string distFilename,
                            int width, int height,
                            GlobalCommand* pGlobalCommand,
                            GlobalInfo* mpGlobalInfo);
            void ProcessData(const std::string& name, const CxUtils::Point3D::List& value);
            void ProcessData(const std::string& name, const double value);
            CxUtils::Point3D ConvertPixelToCoordinates(double row, double col, double depth);
            void ToGrayImage(IplImage * in, IplImage*& outimg);
            struct LaserData
            {
                CxUtils::Point3D::List points;
                double angle;

                LaserData() { }
                LaserData(CxUtils::Point3D::List list, double radians)
                {
                    points = list;
                    angle = radians;
                } // constructor
            };
                             
        private:
            double mCurrentAngle; //Current angle of Hokuyo in radians, parallel to the earth is 0 deg, tilting up is +
            bool mLaserDataFlag;
            int mHeight;
            int mWidth;
            GlobalInfo* mpGlobalInfo;
            GlobalCommand* mpGlobalCommand;

            double mMaxDynamixelDegrees;                                       ///<  Maximum value that can be sent to servo.
            double mMinDynamixelDegrees;                                       ///<  Minimum value that can be sent to servo.
            double mCommandedAngle;
            double mTick;
            double mDynamixelActualDegrees;
            LaserData mLaserDataBuffer;
            CxUtils::Point3D::List mPoints;
            CxUtils::Mutex mMutex1;
            CxUtils::Thread mThread_ProcessLaserData;

            IplImage* mpDepthData;
            IplImage* mpDepthImage;
            CvMat* mImagePoints;
            CvMat* mIntrinsicMatrix;
            CvMat* mObjectPoints;
            CvMat* mDistortionCoeffs;
            CvMat* mRmat;
            CvMat* mTvec;
            CvMat* mRvec;

            double mMax;
            double mMin;
            static void ProcessLaserDataThread(void* ptr);
	    };
	}
}

#endif ///__ZEBULON_SURFACE_DEPTHMAP__H
