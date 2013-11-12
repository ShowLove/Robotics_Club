////////////////////////////////////////////////////////////////////////////////////
///
///  \file segment3d.h
///  \brief Simple 3D line segment data structure.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 17 November 2008
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dbarber@ist.ucf.edu
///  <br>Web:  http://active.ist.ucf.edu
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ACTIVE LAB, IST, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
/// 
///  THIS SOFTWARE IS PROVIDED BY THE ACTIVE LAB''AS IS'' AND ANY
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
#ifndef __CXUTILS_MATH_SEGMENT_3D_H
#define __CXUTILS_MATH_SEGMENT_3D_H

#include "cxutils/math/point3d.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Simple 3D Line Segment structure.  Class contains methods for
    ///          basic geometric relationships between segments and points in 3D space.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Segment3D
    {
    public:
        typedef std::vector<Segment3D> List; ///<  List (std::vector) of 3D segments.
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \enum Axis
        ///   \brief Different Axes values.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        enum Axis
        {
            X = 0,
            Y,
            Z
        };
        Segment3D();
        Segment3D(const Segment3D& segment);
        Segment3D(const Point3D& p1, 
                  const Point3D& p2, 
                  const double width = 0.0);
        Segment3D Flip();
        void Clear();
        bool IsParallel(const Segment3D& segment) const;
        double GetDistanceX() const; // mPoint2.mX - mPoint1.mX
        double GetDistanceY() const; // mPoint2.mY - mPoint1.mY
        double GetDistanceZ() const; // mPoint2.mZ - mPoint1.mZ
        double GetDotProduct() const;
        double GetNorm() const;
        double GetMagnitude() const;
        double GetVolume() const;
        double GetAngleXY(const bool degrees = false) const;
        double GetAngleXZ(const bool degrees = false) const;
        double GetAngleYZ(const bool degrees = false) const;
        double GetDistanceToPoint(const Point3D& point) const;
        double GetDistanceToSegment(const Segment3D& segment) const;
        Point3D GetClosestPointOnSegment(const Point3D& point) const;
        Vector3D GetVector() const;
        Point3D GetMidpoint() const;
        Segment3D GetProjectionXY() const;
        Segment3D GetProjectionXZ() const;
        Segment3D GetProjectionYZ() const;
        Segment3D Rotate(const double angle,
                         const unsigned int axis,
                         const bool angleInDegrees = false) const;
        Segment3D Rotate(const Point3D& origin,
                         const double angle,
                         const unsigned int axis,
                         const bool angleInDegrees = false) const;
        static bool DoesIntersect(const Segment3D& segment1,
                                  const Segment3D& segment2);
        static bool DoesIntersectXY(const Segment3D& segment1,
                                    const Segment3D& segment2);
        static bool DoesIntersect(const Segment3D& segment1,
                                  const Segment3D& segment2,
                                  Point3D& intersectionPoint);
        static bool DoesIntersectXY(const Segment3D& segment1,
                                    const Segment3D& segment2,
                                    Point3D& intersectionPoint);
        static bool DoesIntersect(const Segment3D& segment,
                                  const Point3D& p1,
                                  const Point3D& p2,
                                  const Point3D& p3);
        static bool IsParallel(const Segment3D& segment1,
                               const Segment3D& segment2);
        static double GetDistanceToPoint(const Point3D& point,
                                         const Segment3D& segment);
        static double GetDistanceToSegment(const Segment3D& segment1,
                                           const Segment3D& segment2);
        static Point3D GetClosestPointOnSegment(const Point3D& point, 
                                                const Segment3D& segment);
        Segment3D& operator=(const Segment3D& segment);
        Point3D mPoint1;        ///<  First point in the segment (source).
        Point3D mPoint2;        ///<  Second point in the segment (destination).
        double mWidth;          ///<  Width of the segment.
    };
}

#endif
/*  End of File */
