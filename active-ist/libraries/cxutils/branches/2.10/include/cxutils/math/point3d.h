////////////////////////////////////////////////////////////////////////////////////
///
///  \file point3d.h
///  \brief Simple 3D point data structure.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 4 May 2007
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
#ifndef __CXUTILS_MATH_POINT_3D_H
#define __CXUTILS_MATH_POINT_3D_H

#include "cxutils/cxbase.h"
#include <vector>
#include <string>

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///    \class Point3D
    ///    \brief Simple 3D point structure.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Point3D
    {
    public:
        typedef std::vector<Point3D> List; ///<  A List (std::vector) of 3D points.
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
        Point3D(const Point3D& p);
        Point3D(const double x = 0,
                const double y = 0,
                const double z = 0);
        int Set(const double x, const double y, const double z);
        int Get(double& x, double& y, double &z) const;
        void Clear();
        void Floor(const double thresh);
        void Print() const;
        bool IsParallel(const Point3D& p) const;
        bool IsCoplanar(const Point3D& p1, const Point3D& p2, const Point3D& p3) const;
        bool IsCollinear(const Point3D& p1, const Point3D& p2) const;
        bool IsInside(const Point3D& p1, const Point3D& p2, const Point3D& p3) const;
        bool IsInside2D(const Point3D::List& polygon) const;
        double Distance() const;  // Same as magnitude.
        double Magnitude() const; // Same as distance.
        double SumOfSquares() const;
        double Distance(const Point3D& p) const;
        double Dot(const Point3D& p) const;
        Point3D& Normalize();
        Point3D GetUnitVector() const;
        Point3D Rotate(const double angle,
                       const unsigned int axis,
                       const bool angleInDegrees = false) const;
        Point3D Rotate(const Point3D& origin,
                         const double angle,
                         const unsigned int axis,
                         const bool angleInDegrees = false) const;
        Point3D Midpoint(const Point3D& p) const;
        Point3D& operator()(const double x, const double y, const double z);
        Point3D& operator=(const Point3D& p);
        Point3D& operator+=(const Point3D& p);
        Point3D& operator-=(const Point3D& p);
        Point3D& operator*=(const double val);
        Point3D& operator/=(const double val);
        Point3D& operator/=(const Point3D& p) { mX/=p.mX; mY/=p.mY; mZ/=p.mZ; return *this; }
        Point3D operator+(const Point3D& p) const;
        Point3D operator-(const Point3D& p) const;
        Point3D operator*(const double scaler) const;
        Point3D operator/(const double scaler) const;
        Point3D operator*(const Point3D& p) const;
        Point3D operator/(const Point3D& p) const { return Point3D(mX/p.mX, mY/p.mY, mZ/p.mZ); }
        static double Distance(const Point3D& p1, const Point3D& p2);
        static Point3D Midpoint(const Point3D& p1, const Point3D& p2);
        static double Dot(const Point3D& p1, const Point3D& p2) { return p1.Dot(p2); }
        static bool IsCoplanar(const Point3D& p1,
                               const Point3D& p2,
                               const Point3D& p3,
                               const Point3D& p4);
        // Gets the slope of the 2D line in the XY plane.
        static double LinearRegressionSlope(const Point3D::List& points);
        // Converts thes lope of the line of the XY plane points to an angle [-PI, PI].
        static double GetLinearRegressionAngle(const Point3D::List& points);
        double mX;  ///<  Distance along X-axis.
        double mY;  ///<  Distance along Y-axis.
        double mZ;  ///<  Distance along Z-axis.
    };
}

#endif
/*  End of File */
