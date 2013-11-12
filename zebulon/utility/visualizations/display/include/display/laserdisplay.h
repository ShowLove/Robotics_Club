/////////////////////////////////////////////////////////////////////////////////////
///
/// \file laserdisplay.h
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
#ifndef _ZEBULON_VISUALIZATIONS_LASER_DISPLAY__H
#define _ZEBULON_VISUALIZATIONS_LASER_DISPLAY__H

#include "display/display.h"
#include <cxutils/math/point3d.h>


namespace Zebulon
{
    namespace Utility
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class LaserDisplay
        ///   \brief Tool to display LIDAR/Laser data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_VISUALIZE_DISPLAY_DLL LaserDisplay
        {
            public:
                LaserDisplay();
                virtual ~LaserDisplay();
                bool Setup(const std::string& name,
                           const double rangeMeters,
                           const int rangePixels);
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
                void AddScan(const CxUtils::Point3D::List& scan);
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
                ///   \param[in] drawRadiusFromZ If true, draws point radius using z component.
                ///
                ////////////////////////////////////////////////////////////////////////////////////
                void AddScanCartesian(const CxUtils::Point3D::List& scan,
                                      const bool drawRadiusFromZ = false);
                void Clear()
                {
                    if(mpImage)
                    {
                        cvZero(mpImage);
                    }
                }
                void Display(const int doCvWaitKey = 10)
                {
                    if(mpImage && mName.empty() == false)
                    {
                        Display::DisplayImage(mName, mpImage, doCvWaitKey);
                    }
                }
                void SetDrawColor(const CvScalar& color) { mDrawColor = color; }
            private:
                int mRangePixels;       ///<  Width of image in pixels.
                double mRange;          ///<  Max range of scan in meters.
                double mScale;          ///<  Pixels/Meter scale.
                std::string mName;      ///<  Laser name.
                IplImage* mpImage;      ///<  Image data.
                CvScalar mDrawColor;    ///<  Draw color.
        };
    }
}
#endif
