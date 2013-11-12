////////////////////////////////////////////////////////////////////////////////////
///
///  \file segment3d.cpp
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
#include "cxutils/math/cxmath.h"
#include <math.h>

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Initializes data members.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D::Segment3D()
{
    mWidth = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
///   \param[in] segment The segment to be equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D::Segment3D(const Segment3D& segment)
{
    mPoint1 = segment.mPoint1;
    mPoint2 = segment.mPoint2;
    mWidth = segment.mWidth;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] p1 The source/first point in the segment.
///   \param[in] p2 The destination/second point in the segment.
///   \param[in] width The width of the segment Only used for distance
///                    calculations and volume.  Set to 0 for infinitely
///                    thin line segment.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D::Segment3D(const Point3D& p1,
                     const Point3D& p2,
                     const double width)
{
    mPoint1 = p1;
    mPoint2 = p2;
    mWidth = width;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Flips the source and destination values.
///
///   \return A copy of the current segment with the source and destination points
///           switched.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::Flip()
{
    Point3D temp;
    temp = mPoint1;
    mPoint1 = mPoint2;
    mPoint2 = temp;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets to defaults.
///
////////////////////////////////////////////////////////////////////////////////////
void Segment3D::Clear()
{
    mPoint1.Clear();
    mPoint2.Clear();
    mWidth = 0.0;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if the two segments are paralell.
///
///   \param[in] segment The segment to compare with for paralellism.
///
///   \return True if in parallel, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::IsParallel(const Segment3D& segment) const
{
    return IsParallel(*this, segment);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Distance from source to destination along axis.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceX() const
{
    return mPoint2.mX - mPoint1.mX;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Distance from source to destination along axis.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceY() const
{
    return mPoint2.mY - mPoint1.mY;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Distance from source to destination along axis.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceZ() const
{
    return mPoint2.mZ - mPoint1.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Dot product of the line segment.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDotProduct() const
{
    double dx, dy, dz;
    dx = GetDistanceX();
    dy = GetDistanceY();
    dz = GetDistanceZ();

    return dx*dx + dy*dy + dz*dz;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Normal of the segment (segment length).
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetNorm() const
{
    double dx, dy, dz;
    dx = GetDistanceX();
    dy = GetDistanceY();
    dz = GetDistanceZ();

    return sqrt(dx*dx + dy*dy + dz*dz);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Normal of the segment (segment length).
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetMagnitude() const
{
    double dx, dy, dz;
    dx = GetDistanceX();
    dy = GetDistanceY();
    dz = GetDistanceZ();

    return sqrt(dx*dx + dy*dy + dz*dz);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Volumn of the 3D segment (based on width and magnitude).
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetVolume() const
{
    double rad = mWidth/2.0;
    return GetMagnitude()*CX_PI*rad*rad + 4.1887902*rad*rad*rad;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the angle of the segment within the desired plane.
///
///   \param[in] degrees If true, result is returned in degrees, otherwise
///                      in radians.
///
///   \return Angle within the desired plane.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetAngleXY(const bool degrees) const
{
    return (degrees) ? 
           atan2(mPoint2.mY - mPoint1.mY, mPoint2.mX - mPoint1.mX)*57.2957795 : 
           atan2(mPoint2.mY - mPoint1.mY, mPoint2.mX - mPoint1.mX);
  
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the angle of the segment within the desired plane.
///
///   \param[in] degrees If true, result is returned in degrees, otherwise
///                      in radians.
///
///   \return Angle within the desired plane.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetAngleXZ(const bool degrees) const
{
    return (degrees) ? 
           atan2(mPoint2.mZ - mPoint1.mZ, mPoint2.mX - mPoint1.mX)*57.2957795 : 
           atan2(mPoint2.mZ - mPoint1.mZ, mPoint2.mX  - mPoint1.mX);
  
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the angle of the segment within the desired plane.
///
///   \param[in] degrees If true, result is returned in degrees, otherwise
///                      in radians.
///
///   \return Angle within the desired plane.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetAngleYZ(const bool degrees) const
{
    return (degrees) ? 
           atan2(mPoint2.mZ - mPoint1.mZ, mPoint2.mY - mPoint1.mY)*57.2957795 : 
           atan2(mPoint2.mZ - mPoint1.mZ, mPoint2.mY  - mPoint1.mY);
  
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the shortest distance from the point to the line segment.
///
///   \param[in] point The point to get distance to segment from.
///
///   \return The distance from the point to the segment.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceToPoint(const Point3D& point) const
{
    return GetDistanceToPoint(point, *this);
  
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the distance between the two segments.
///
///   If the segment has a width associated with it, this value is used
///   in the distance measurement.
///
///   \param[in] segment The segment to get distance to.
///
///   \return Shortest between two segments.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceToSegment(const Segment3D& segment) const
{
    return GetDistanceToSegment(*this, segment);
  
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the point on segment closest to point. Ignores segment width.
///
///   \param[in] point Point to find closest point to on segment.
///
///   \return Point on segment closest to point.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Segment3D::GetClosestPointOnSegment(const Point3D& point) const
{
    return GetClosestPointOnSegment(point,*this);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a vector representing the line segment composed of
///   the dx, dy, and dz values of segment.
///
///   \return Vector composed of (GetDistanceX(), GetDistanceY(), GetDistanceZ()).
///
////////////////////////////////////////////////////////////////////////////////////
Vector3D Segment3D::GetVector() const
{
    return Vector3D(GetDistanceX(), GetDistanceY(), GetDistanceZ());
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Midpoint of the segment.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Segment3D::GetMidpoint() const
{
    return Point3D( (mPoint1.mX + mPoint2.mX)/2.0,
                    (mPoint1.mY + mPoint2.mY)/2.0,
                    (mPoint1.mZ + mPoint2.mZ)/2.0 );
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Projection of the segment within the 2D plane.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::GetProjectionXY() const
{
    Segment3D segment(*this);
    segment.mPoint1.mZ = 0;
    segment.mPoint2.mZ = 0;
    return segment;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Projection of the segment within the 2D plane.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::GetProjectionXZ() const
{
    Segment3D segment(*this);
    segment.mPoint1.mY = 0;
    segment.mPoint2.mY = 0;
    return segment;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Projection of the segment within the 2D plane.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::GetProjectionYZ() const
{
    Segment3D segment(*this);
    segment.mPoint1.mX = 0;
    segment.mPoint2.mX = 0;
    return segment;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the segment around the origin by the angle given and
///          axis of rotation.
///
///   \param[in] angle The amount to rotate.
///   \param[in] axis The axis to rotate around, see Segment3D::Axis enumerations.
///   \param[in] angleInDegrees If true the value of angle is in degrees, if false
///                             it is in radians (default).
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::Rotate(const double angle,
                            const unsigned int axis,
                            const bool angleInDegrees) const
{
    Segment3D segment;

    Quaternion rotation;

    switch(axis)
    {
    case X:
        rotation.SetRotationX(angle, angleInDegrees);
        break;
    case Y:
        rotation.SetRotationY(angle, angleInDegrees);
        break;
    default: // Z
        rotation.SetRotationZ(angle, angleInDegrees);
        break;
    }

    segment.mPoint1 = rotation.Rotate(mPoint1);
    segment.mPoint2 = rotation.Rotate(mPoint2);
    segment.mWidth = mWidth;
    return segment;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the segment around the origin by the angle given and
///          axis of rotation.
///
///   \param[in] pivot The 3D point to rotate around instead of origin.
///   \param[in] angle The amount to rotate.
///   \param[in] axis The axis to rotate around, see Segment3D::Axis enumerations.
///   \param[in] angleInDegrees If true the value of angle is in degrees, if false
///                             it is in radians (default).
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Segment3D::Rotate(const Point3D& pivot,
                            const double angle,
                            const unsigned int axis,
                            const bool angleInDegrees) const
{
    Point3D p1(mPoint1.mX - pivot.mX,
               mPoint1.mY - pivot.mY,
               mPoint1.mZ - pivot.mZ);
    Point3D p2(mPoint2.mX - pivot.mX,
               mPoint2.mY - pivot.mY,
               mPoint2.mZ - pivot.mZ);
    Quaternion rotation;

    switch(axis)
    {
    case X:
        rotation.SetRotationX(angle, angleInDegrees);
        break;
    case Y:
        rotation.SetRotationY(angle, angleInDegrees);
        break;
    default: // Z
        rotation.SetRotationZ(angle, angleInDegrees);
        break;
    }
    
    p1 = rotation.Rotate(p1);
    p2 = rotation.Rotate(p1);
    
    return Segment3D(p1 + pivot, p2 + pivot, mWidth);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if two segments intersect.
///
///   \param[in] segment1 The first segment to compare with.
///   \param[in] segment2 The second segment to compare with..
///
///   \return True if the two segments intersect with each other, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::DoesIntersect(const Segment3D& segment1,
                              const Segment3D& segment2)
{
    Point3D result;
    return DoesIntersect(segment1, segment2, result);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if two segments intersect.
///
///   \param[in] segment1 The first segment to compare with.
///   \param[in] segment2 The second segment to compare with..
///   \param[out] intersectionPoint The point where the segments intersect.
///
///   \return True if the two segments intersect with each other, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::DoesIntersect(const Segment3D& segment1,
                              const Segment3D& segment2,
                              Point3D& intersectionPoint)
{
    if(Point3D::IsCoplanar(segment1.mPoint1, 
                           segment1.mPoint2, 
                           segment2.mPoint1, 
                           segment2.mPoint2))
    {
        // Find intersection in XY plane, followed by Z axis.
        if(DoesIntersectXY(segment1, segment2, intersectionPoint))
        {            
            //  Now find out the distance in the z
            //  plane until the point hits the intersection
            //  on the line
            Point3D v = segment2.mPoint2 - segment2.mPoint1;
            Point3D w = intersectionPoint - segment2.mPoint1;
            double c1 = w.Dot(v);
            if(c1 <= 0)
            {
                intersectionPoint.mZ = intersectionPoint.Distance(segment2.mPoint1);
                //  If the intersection point is below
                //  0, than we need to direct down in the z plane
                //  to reach it
                if(segment2.mPoint1.mZ < 0.0)
                {
                    intersectionPoint.mZ *= -1.0;
                }
            }
            else
            {
                double c2 = v.Dot(v);
                if(c2 <= c1)
                {
                    intersectionPoint.mZ = intersectionPoint.Distance(segment2.mPoint2);
                    //  If the intersection point is below
                    //  0, than we need to direct down in the z plane
                    //  to reach it
                    if(segment2.mPoint2.mZ < 0.0)
                    {
                        intersectionPoint.mZ *= -1.0;
                    }
                }
                else
                {
                    double b = c1 / (c2 + CX_EPSILON);
                    //  Get the intersection point on the
                    //  segment
                    Point3D pb = segment2.mPoint1 + v*b;
                    //  Calculate distance
                    intersectionPoint.mZ = intersectionPoint.Distance(pb);
                    //  If the intersection point is below
                    //  0, than we need to direct down in the z plane
                    //  to reach it
                    if(pb.mZ < 0.0)
                    {
                        intersectionPoint.mZ *= -1.0;
                    }
                }
            }

            return true;
        }
    }
    
    intersectionPoint.mX = intersectionPoint.mY = intersectionPoint.mZ = 0.0;
    
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if two segments intersect in the XY plane.
///
///   \param[in] segment1 The first segment to compare with in the XY plane.
///   \param[in] segment2 The second segment to compare with in the XY plane.
///
///   \return True if the two segments intersect with each in the XY plane, 
///           otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::DoesIntersectXY(const Segment3D& segment1,
                                const Segment3D& segment2)
{
    double upperX, upperY, lowerX, lowerY;
    double aX, bX, cX, aY, bY, cY, d, f, e;
    bool result = false;

    aX = segment1.mPoint2.mX - segment1.mPoint1.mX;
    bX = segment2.mPoint1.mX - segment2.mPoint2.mX;

    if (aX < 0.0)
    {
        lowerX = segment1.mPoint2.mX;
        upperX = segment1.mPoint1.mX;
    }
    else
    {
        upperX = segment1.mPoint2.mX;
        lowerX = segment1.mPoint1.mX;
    }

    if (bX > 0.0)
    {
        if ((upperX < segment2.mPoint2.mX) || (segment2.mPoint1.mX < lowerX))
        {
            return result;
        }

    }
    else if( (upperX < segment2.mPoint1.mX) || (segment2.mPoint2.mX < lowerX))
    {
        return result;
    }

    aY = segment1.mPoint2.mY - segment1.mPoint1.mY;
    bY = segment2.mPoint1.mY - segment2.mPoint2.mY;

    if(aY < 0.0)
    {
        lowerY = segment1.mPoint2.mY;
        upperY = segment1.mPoint1.mY;
    }
    else
    {
        upperY = segment1.mPoint2.mY;
        lowerY = segment1.mPoint1.mY;
    }

    if(bY > 0.0)
    {
        if((upperY < segment2.mPoint2.mY) || (segment2.mPoint1.mY < lowerY))
        {
            return result;
        }
    }
    else if( (upperY < segment2.mPoint1.mY) || (segment2.mPoint2.mY < lowerY))
    {
        return result;
    }

    cX = segment1.mPoint1.mX - segment2.mPoint1.mX;
    cY = segment1.mPoint1.mY - segment2.mPoint1.mY;
    d  = (bY * cX) - (bX * cY);
    f  = (aY * bX) - (aX * bY);

    if(f > 0.0)
    {
        if(d < 0.0 || d > f)
        {
            return result;
        }
    }
    else if(d > 0.0 || d < f)
    {
        return result;
    }

    e = (aX * cY) - (aY * cX);

    if(f > 0.0)
    {
        if(e < 0.0 || e > f)
        {
            return result;
        }
    }
    else if(e > 0.0 || e < f)
    {
        return result;
    }

    result = true;

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if two segments intersect in the XY plane.
///
///   \param[in] segment1 The first segment to compare with in the XY plane.
///   \param[in] segment2 The second segment to compare with in the XY plane.
///   \param[out] intersectionPoint The point where the segments intersect
///                                 in the XY plane.
///
///   \return True if the two segments intersect with each in the XY plane, 
///           otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::DoesIntersectXY(const Segment3D& segment1,
                                const Segment3D& segment2,
                                Point3D& intersectionPoint)
{
    intersectionPoint(0, 0, 0);

    if(DoesIntersectXY(segment1, segment2))
    {
        double r, dx1, dx2, dx3, dy1, dy2, dy3;

        dx1 = segment1.mPoint2.mX - segment1.mPoint1.mX;
        dx2 = segment2.mPoint2.mX - segment2.mPoint1.mX;
        dx3 = segment1.mPoint1.mX - segment2.mPoint1.mX;
        dy1 = segment1.mPoint2.mY - segment1.mPoint1.mY;
        dy2 = segment1.mPoint1.mY - segment2.mPoint1.mY;
        dy3 = segment2.mPoint2.mY - segment2.mPoint1.mY;

        r = dx1 * dy3 - dy1 * dx2;

        if(fabs(r) > CX_EPSILON) // If 0.
        {
            r = (dy2 * (segment2.mPoint2.mX - segment2.mPoint1.mX) - dx3 * dy3) / (r + CX_EPSILON);
            intersectionPoint.mX = segment1.mPoint1.mX + r * dx1;
            intersectionPoint.mY = segment1.mPoint1.mY + r * dy1;
        }
        else
        {
            if( fabs((segment1.mPoint2.mX - segment1.mPoint1.mX) * (segment2.mPoint1.mY - segment1.mPoint1.mY) - (segment2.mPoint1.mX - segment1.mPoint1.mX) * (segment1.mPoint2.mY - segment1.mPoint1.mY)) < CX_EPSILON )
            {
                intersectionPoint.mX = segment2.mPoint1.mX;
                intersectionPoint.mY = segment2.mPoint1.mY;
            }
            else
            {
                intersectionPoint.mX = segment2.mPoint2.mX;
                intersectionPoint.mY = segment2.mPoint2.mY;
            }
        }

        return true;
    }
    
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if a segment intersect with a triangle.
///
///   \param[in] segment The segment to check for intersection with triangle.
///   \param[in] p1 Point defining a triangle.
///   \param[in] p2 Point defining a triangle.
///   \param[in] p3 Point defining a triangle.
///
///   \return True if the segment intersects with the triangle.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::DoesIntersect(const Segment3D& segment,
                              const Point3D& p1,
                              const Point3D& p2,
                              const Point3D& p3)
{
    Point3D u, v, n;    //  Triangle vectors
    Point3D dir, w0, w; //  Segment vectors
    Point3D i;          //  Intersection point
    double r, a, b;     

    //  Get triangle edge vectors and plane normal
    u = p2 - p1;
    v = p3 - p1;
    n = u*v;
    if(fabs(n.mX) < CX_EPSILON &&
       fabs(n.mY) < CX_EPSILON &&
       fabs(n.mZ) < CX_EPSILON)
    {
       return false;
    }

    dir = segment.mPoint2 - segment.mPoint1;
    w0 = p1 - segment.mPoint1;
    a = -Point3D::Dot(n, w0);
    b = Point3D::Dot(n, dir);
    if(fabs(b) < CX_EPSILON)
    {
        if(fabs(a) < CX_EPSILON)
        {
            //  Same plane (do more work here)
            return true;
        }
        else
        {
            return false; //  Disjoint
        }

    }
    r = a/b;
    if(fabs(r) < CX_EPSILON || r > 1.0)
    {
        return false;    //  No intersect
    }
    
    i = p1 + dir*r;      //  Intersection point
    
    //  Now check if point is within triangle
    double ss, tt;
    double uu, uv, vv, wu, wv, d;
    uu = Point3D::Dot(u, u);
    uv = Point3D::Dot(u, v);
    vv = Point3D::Dot(v, v);
    w = i - p1;
    wu = Point3D::Dot(w, u);
    wv = Point3D::Dot(w, v);
    d = uv*uv - uu*vv;
    ss = (uv*wv - vv*wu)/(d + CX_EPSILON);
    if(ss < 0.0 || ss > 1.0)
    {
        return false;
    }
    tt = (uv*wu - uu*wv)/(d + CX_EPSILON);
    if(tt < 0.0 || (ss + tt) > 1.0)
    {
        return false;
    }
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares to segments to find out if they are parallel.
///
///   \param[in] segment1 The first segment.
///   \param[in] segment2 The second segment.
///
///   \return True if the two segments are in parallel, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Segment3D::IsParallel(const Segment3D& segment1,
                           const Segment3D& segment2)
{
    double dx1, dx2;
    double dy1, dy2;
    double dz1, dz2;

    dx1 = segment1.mPoint1.mX - segment1.mPoint2.mX;
    dx2 = segment2.mPoint1.mX - segment2.mPoint2.mX;

    dy1 = segment1.mPoint1.mY - segment1.mPoint2.mY;
    dy2 = segment2.mPoint1.mY - segment2.mPoint2.mY;

    dz1 = segment1.mPoint1.mZ - segment1.mPoint2.mZ;
    dz2 = segment2.mPoint1.mZ - segment2.mPoint2.mZ;
    
    if(fabs(dy1/(dx1 + CX_EPSILON) - dy2/(dx2 + CX_EPSILON)) > CX_EPSILON)
    {
        return false;
    }
    if(fabs(dz1/(dy1 + CX_EPSILON) - dz2/(dy2 + CX_EPSILON)) > CX_EPSILON)
    {
        return false;
    }
    if(fabs(dx1/(dz1 + CX_EPSILON) - dx2/(dz2 + CX_EPSILON)) > CX_EPSILON)
    {
        return false;
    }
    //  If they are all equal, return true.
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the shortest distance from the point to the line segment.
///
///   \param[in] point The point to get distance to segment from.
///   \param[in] segment The segment to get distance to point from.
///
///   \return The distance from the point to the segment.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceToPoint(const Point3D& point,
                                     const Segment3D& segment)
{
    Point3D v = segment.mPoint2 - segment.mPoint1;
    Point3D w = point - segment.mPoint1;
    double c1 = w.Dot(v);
    double magnitude = 0.0;

    if(c1 <= 0)
    {
        magnitude = point.Distance(segment.mPoint1) - segment.mWidth/2.0;
        if(magnitude < 0.0) // If less than 0 then the point is on the segment.
        {
            magnitude = 0.0;
        }
        return magnitude;
    }

    double c2 = v.Dot(v);
    if(c2 <= c1)
    {
        magnitude = point.Distance(segment.mPoint2) - segment.mWidth/2.0;
        if(magnitude < 0.0) // If less than 0 then the point is on the segment.
        {
            magnitude = 0.0;
        }
        return magnitude;
    }

    double b = c1/(c2 + CX_EPSILON);
    Point3D pb = segment.mPoint1 + v*b;
    magnitude = point.Distance(pb) - segment.mWidth/2.0;
    if(magnitude < 0.0) // If less than 0 then the point is on the segment.
    {
        magnitude = 0.0;
    }

    return magnitude;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the distance between the two segments.
///
///   If the segment has a width associated with it, this value is used
///   in the distance measurement.
///
///   \param[in] segment1 The first segment.
///   \param[in] segment2 The second segment.
///
///   \return Shortest between two segments.
///
////////////////////////////////////////////////////////////////////////////////////
double Segment3D::GetDistanceToSegment(const Segment3D& segment1,
                                       const Segment3D& segment2)
{
    
    Point3D   u = segment1.mPoint2 - segment1.mPoint1;
    Point3D   v = segment2.mPoint2 - segment2.mPoint1;
    Point3D   w = segment1.mPoint1 - segment2.mPoint1;
    double    a = u.Dot(u);        // always >= 0
    double    b = u.Dot(v);
    double    c = v.Dot(v);        // always >= 0
    double    d = u.Dot(w);
    double    e = v.Dot(w);
    double    D = a*c - b*b;       // always >= 0
    double    sc, sN, sD = D;      // sc = sN / sD, default sD = D >= 0
    double    tc, tN, tD = D;      // tc = tN / tD, default tD = D >= 0
   
    // compute the line parameters of the two closest points
    if (D < CX_EPSILON) 
    { 
        // the lines are almost parallel
        sN = 0.0;         // force using point P0 on segment S1
        sD = 1.0;         // to prevent possible division by 0.0 later
        tN = e;
        tD = c;
    }
    else 
    {   
        // get the closest points on the infinite lines
        sN = (b*e - c*d);
        tN = (a*e - b*d);
        if (sN < 0.0) // sc < 0 => the s=0 edge is visible
        {   
            sN = 0.0;
            tN = e;
            tD = c;
        }
        else if (sN > sD) 
        {  
            // sc > 1 => the s=1 edge is visible
            sN = sD;
            tN = e + b;
            tD = c;
        }
    }

    if (tN < 0.0) 
    {           
        // tc < 0 => the t=0 edge is visible
        tN = 0.0;
        // recompute sc for this edge
        if (-d < 0.0)
            sN = 0.0;
        else if (-d > a)
            sN = sD;
        else 
        {
            sN = -d;
            sD = a;
        }
    }
    else if (tN > tD) 
    {   
        // tc > 1 => the t=1 edge is visible
        tN = tD;
        // recompute sc for this edge
        if ((-d + b) < 0.0)
            sN = 0;
        else if ((-d + b) > a)
            sN = sD;
        else 
        {
            sN = (-d + b);
            sD = a;
        }
    }
    // finally do the division to get sc and tc
    sc = (fabs(sN) < CX_EPSILON ? 0.0 : sN / sD);
    tc = (fabs(tN) < CX_EPSILON ? 0.0 : tN / tD);
    
    // get the difference of the two closest points
    Point3D   dP = w + (u*sc) - (v*tc);  // = S1(sc) - S2(tc)
    
    //  Calculate the shortest distance and then subtract the
    //  widths of the segments as well which make up shortest distance
    double magnitude = dP.Distance() - segment1.mWidth/2.0 - segment2.mWidth/2.0;
    if(magnitude < 0.0)
    {
        magnitude = 0.0;
    }
    return magnitude;   // return the closest distance
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a point on the segment closest to a point. Ignores segment width.
///
///   \param[in] point The point to get closest point on segment from.
///   \param[in] segment The segment to get closest point.
///
///   \return The closest point on segment.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Segment3D::GetClosestPointOnSegment(const Point3D& point, const Segment3D& segment)
{
    Point3D v = segment.mPoint2 - segment.mPoint1;
    Point3D w = point - segment.mPoint1;
    double c1 = w.Dot(v);
    double magnitude = 0.0;

    if(c1 <= 0)
    {
        return segment.mPoint1;
    }

    double c2 = v.Dot(v);
    if(c2 <= c1)
    {
        return segment.mPoint2;
    }

    double b = c1/(c2 + CX_EPSILON);
    return segment.mPoint1 + v*b;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
///   \param[in] segment Segment to be equal to.
///
///   \return The modified segment that is a copy.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D& Segment3D::operator=(const Segment3D& segment)
{
    mPoint1 = segment.mPoint1;
    mPoint2 = segment.mPoint2;
    mWidth = segment.mWidth;
    return *this;
}


/*  End of File */
