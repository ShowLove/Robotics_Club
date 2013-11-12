/////////////////////////////////////////////////////////////////////////////////////
///
/// \file laserdisplay.cpp
/// \brief Class for displaying LIDAR/Laser data.
///
/// Author(s): Daniel Barber<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dbarber@ist.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
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
#include "display/laserdisplay.h"
#include <cxutils/math/cxmath.h>

using namespace Zebulon;
using namespace Utility;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
LaserDisplay::LaserDisplay()
{
    mRangePixels = 600;
    mRange = 8;
    mScale = mRange/mRangePixels;
    mpImage = NULL;
    mDrawColor = CV_RGB(0, 255, 0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
LaserDisplay::~LaserDisplay()
{
    if(mpImage)
    {
        cvReleaseImage(&mpImage);
        mpImage = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Setup the display for drawing the laser information.
///
///   \param[in] name Name of laser (name of window in display).
///   \param[in] rangeMeters Max range of laser data to display in meters.
///   \param[in] rangePixels Window size in pixels.
///
////////////////////////////////////////////////////////////////////////////////////
bool LaserDisplay::Setup(const std::string& name,
                         const double rangeMeters,
                         const int rangePixels)
{
    if(rangeMeters > 0 && rangePixels > 2)
    {
        mRangePixels = rangePixels;
        mRange = rangeMeters;
        mName = name;
        mScale = mRangePixels/mRange;
        if(mpImage)
        {
            cvReleaseImage(&mpImage);
            mpImage = NULL;
        }
        mpImage = cvCreateImage(cvSize(mRangePixels,mRangePixels),IPL_DEPTH_8U,3);
        cvZero(mpImage);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds laser data to the display.
///
///   The coordinate frame for a laser is as follows: X positive in front, Y
///   positive to the right, Z positive down.  A positive Z rotation is a
///   rotation to the right, a positive Y rotation is a tilt up.
///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
///
///   \param[in] scan Laser scan data in polar coordinates relative to the
///                   laser.  Values are as follows: X = range/distance in
///                   meters, Y = pitch [-pi/2, pi/2], Z = yaw [-PI,PI].
///                   values are in order from left to right.
///
////////////////////////////////////////////////////////////////////////////////////
void LaserDisplay::AddScan(const CxUtils::Point3D::List& scan)
{
    if(scan.size() > 0 && mpImage)
    {
        CxUtils::Point3D::List::const_iterator point;
        CvPoint cur, prev;
        unsigned int count = 0;
        for(point = scan.begin();
            point != scan.end();
            point++, count++)
        {
            // Convert to cartesian coordinates, then
            // to pixel values.
            CxUtils::Quaternion rotation;
            rotation.SetRotationZ(-point->mZ + CxUtils::CX_PI/2.0);
            CxUtils::Point3D xy(point->mX, 0, 0);
            xy = rotation.Rotate(xy);
            cur.x = (int)(mScale*xy.mX + mRangePixels/2.0);
            cur.y = (int)(mRangePixels - (mRangePixels/2.0 + mScale*xy.mY));
            if(count > 0)
            {
                cvCircle(mpImage, cur, 1, mDrawColor, -1);
            }
            prev = cur;
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds laser data to the display.
///
///   The coordinate frame for a laser is as follows: X positive in front, Y
///   positive to the right, Z positive down.  A positive Z rotation is a
///   rotation to the right, a positive Y rotation is a tilt up.
///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
///
///   \param[in] scan Laser scan data in cartesian coordinates in meters.
///   \param[in] drawRadiusFromZ If true, points being drawn have radius
///                              set from Z component of points.
///
////////////////////////////////////////////////////////////////////////////////////
void LaserDisplay::AddScanCartesian(const CxUtils::Point3D::List& scan,
                                    const bool drawRadiusFromZ)
{
    if(scan.size() > 0 && mpImage)
    {
        CxUtils::Point3D::List::const_iterator point;
        CvPoint cur, prev;
        unsigned int count = 0;
        for(point = scan.begin();
            point != scan.end();
            point++, count++)
        {
            // Rotate so the view is up.
            CxUtils::Quaternion rotation;
            rotation.SetRotationZ(CxUtils::CX_PI/2.0);
            CxUtils::Point3D xy;
            xy = rotation.Rotate(*point);
            cur.x = (int)(mScale*xy.mY + mRangePixels/2.0);
            cur.y = (int)(mRangePixels - (mRangePixels/2.0 + mScale*xy.mX));

            int radius = 1;
            if(drawRadiusFromZ)
            {
                radius = (int)(mScale*point->mZ);
                if(radius < 0)
                    radius = 1;
            }
            cvCircle(mpImage, cur, radius, mDrawColor, -1);

            prev = cur;
        }
    }
}


/* End of File */
