////////////////////////////////////////////////////////////////////////////////////
///
/// \file homography.h
/// \brief Primitive homography calibration and adjustment class. For adjusting
///        groundplane in regards to perspective.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu<br>
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
#ifndef __ZEBULON_VISION_HOMOGRAPHY_H
#define __ZEBULON_VISION_HOMOGRAPHY_H

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <cxutils/math/point3d.h>
#include <cxutils/math/vector3d.h>
#include <cxutils/math/segment3d.h>

#ifndef ZEB_VISION_HOMOGRAPHY_DLL

#ifdef WIN32
#define ZEB_VISION_HOMOGRAPHY_DLL __declspec(dllimport)
#else
#define ZEB_VISION_HOMOGRAPHY_DLL
#endif

#endif


namespace Zebulon
{
    namespace Vision
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Homography
        ///   \brief Interface class for transforming between image coordinates and
        ///   ground plane coordiantes, and vice-versa.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_VISION_HOMOGRAPHY_DLL Homography
        {
            public:
                enum WarpProperty
                {
                    Horizontal = 0,
                    Vertical,
                    Rotate,
                    Slant,
                    Zoom
                };
                enum WarpType
                {
                    Original = 0,
                    Modified,
                    Inverse
                };

                Homography();
                ~Homography();
                bool LoadSettings(const std::string& xmlFileName = "settings/vision/homo.xml");
                void SaveSettings(const std::string& xmlFileName = "settings/vision/homo.xml");
                void SetProperty(const WarpProperty type, const double value);
                double GetProperty(const WarpProperty type) const;
                bool CheckerBoardCalibrate(const IplImage *imgIn, double widthCM = 76.8, double heightCM = 48, int hBlocks = 8, int vBlocks = 5);
                bool DistortionCalibrate(const IplImage *imgIn, int hBlocks = 8, int vBlocks = 5, int numFrames = 50);
                IplImage * Warp(const IplImage * imgIn, WarpType type = Modified) const;
                CxUtils::Point3D Warp(const int x, const int y, WarpType type = Modified) const;
                CxUtils::Point3D Warp(CxUtils::Point3D point, WarpType type = Modified) const;
                CxUtils::Segment3D Warp(CxUtils::Segment3D line, WarpType type = Modified) const;
                CvPoint Warp(const CvPoint cvpoint, WarpType type = Modified) const;

                CxUtils::Point3D FromImageToGround(const CxUtils::Point3D& point, const IplImage* originalImage, double offsetPercentX = .5, double offsetPercentY = 1, const bool invertY = true);
                CxUtils::Segment3D FromImageToGround(const CxUtils::Segment3D& line, const IplImage* originalImage,double originPercentX = .5, double originPercentY = 1, const bool invertY = true);
                ///*
                CxUtils::Point3D FromGroundToImage(const CxUtils::Point3D& point, const IplImage* originalImage,double originPercentX = .5, double originPercentY = 1, const bool invertY = true);
                CxUtils::Segment3D FromGroundToImage(const CxUtils::Segment3D& line, const IplImage* originalImage,double originPercentX = .5, double originPercentY = 1, const bool invertY = true);
                //*/
        protected:
                IplImage * Warp(const IplImage * imgIn, CvMat * matrix) const;
                CxUtils::Point3D Warp(const int x, const int y, CvMat * matrix) const;
                CvMat * mOriginalMat;
                CvMat * mInverseMat;
                CvMat * mModifiedMat;
                std::map<WarpProperty,double> mOffsets;
        };
    }
}

#endif//__ZEBULON_VISION_HOMOGRAPHY_H
