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
#ifndef __ZEBULON_GROUND_VISION__H
#define __ZEBULON_GROUND_VISION__H

#include "dvision/visionutils.h"
#include "ransac/RANSAC.h"
#include "ransac/LineParamEstimator.h"
#include "ground/vision/homography.h"
#include "ground/vision/igvcbarrelobject.h"
#include <xmlconfig/xmlconfigurator.h>
#include <string>
#include <vector>

namespace Zebulon
{
    namespace IGVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///  \class GroundVision
        ///  \brief Useful functions for IGVC vision processing. Settings located in XML.
        ///         Settings can be tweaked while running.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GroundVision
        {
        public:
            GroundVision();
            ~GroundVision();
            //std::vector<CxUtils::Segment3D> FindPerspectiveLaneLines(const IplImage* imgIn,
            //                                                         IplImage*& imgOut);
            std::vector<CxUtils::Segment3D> FindLaneLines(const IplImage* imgIn,
                                                                 IplImage*& imgOut, int rows = 3, int cols = 3);

            IplImage * FindLaneEdges(const IplImage* imgIn);
            //std::vector<CxUtils::Segment3D> FindPerspectiveLaneLines2(const IplImage* imgIn, IplImage*& imgOut);
            bool RansacFit(const IplImage* binaryImage, CxUtils::Segment3D& line) const;
            bool FixPerspective(const IplImage* imgIn, IplImage*& imgOut);
            CxUtils::Point3D FromImageToLocal(const CxUtils::Point3D& point, const IplImage* image);
            CxUtils::Segment3D FromImageToLocal(const CxUtils::Segment3D& line, const IplImage* image);
            CxUtils::Point3D FromGroundToImage(const CxUtils::Point3D point, const IplImage* image);
            CxUtils::Segment3D FromGroundToImage(const CxUtils::Segment3D& line, const IplImage* image);
            bool LoadSettings(const std::string& xmlFilename);
            void DrawSegment(const CxUtils::Segment3D& line, IplImage* imgIn, CvScalar color = CV_RGB(255,0,0)) const;

        private:
            bool LoadSettings();
            //static bool FindLaneLine(const IplImage* imgIn, IplImage*& imgOut, CxUtils::Segment3D& segment, CvScalar objColor);
            Vision::Homography mHomo;
            std::vector<Vision::VisionObject*> mObjects;

            Zebulon::Configuration::XmlConfigurator mXML;

            int mMinWhiteThreshold;     ///< Threshold pre edge detection. Minimum value from 0(black) to 255(white) to be considered white
            int mMaxWhiteThreshold;     ///< Threshold pre edge detection. Max value from 0(black) to 255(white) to be considered white
            int mMinCanny;              ///< Canny input parameters, see opencv docs.
            int mMaxCanny;
            double mMinBlobNoiseFilter; ///< Minium size of a "blob" as a percentage of total image size(0-1) to be considered noise.
            double mMaxBlobNoiseFilter; ///< Max size of a "blob" as a percentage of total image size to be considered noise.
            double mMinRansacUsedData;  ///< Not used. A value returned from ransac, represents percentage of total points used to find line.
            double mMaxRansacUsedData;  ///< Not used. It is possible to set up additional filters by validating lines that use x percent of points.
            int mLineMaskWidth;         ///< Not used. Width of line mask when line is found with ransac.  Only use if running ransac more than once on the same image.
            double mMaxAngleDiffDegrees;    ///< Not used. Max angle of second line compared to first before its considered invalid.
            double mOriginOffsetPercentX;   ///< Vehicle offset in the Y direction(vertical) as percentage (0-1)of image. Image origin starts at top left.
            double mOriginOffsetPercentY;
            //RANSACMT<Point2D,double> mRansac;
        };
    }
}
#endif
