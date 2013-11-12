////////////////////////////////////////////////////////////////////////////////////
///
///  \file point3d.cpp
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
#include "cxutils/math/point3d.h"
#include "cxutils/math/cxmath.h"
#include <iostream>
#include <sstream>
#include <iomanip>

using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
///   \param p Point to be equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D::Point3D(const Point3D& p)
{
    mX = p.mX;
    mY = p.mY;
    mZ = p.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Sets values.
///
///   \param x Distance along X axis.
///   \param y Distance along Y axis.
///   \param z Distance along Z axis.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D::Point3D(const double x, const double y, const double z) : mX(x), mY(y), mZ(z)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param x Distance along X axis.
///   \param y Distance along Y axis.
///   \param z Distance along Z axis.
///
///   \return 1 if value set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Point3D::Set(const double x, const double y, const double z)
{
    mX = x; mY = y; mZ = z;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values.
///
///   \param x Distance along X axis.
///   \param y Distance along Y axis.
///   \param z Distance along Z axis.
///
///   \return 1 if value retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Point3D::Get(double& x, double& y, double &z) const
{
    x = mX;  y = mY;  z = mZ;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets point to origin.
///
////////////////////////////////////////////////////////////////////////////////////
void Point3D::Clear()
{
    mX = mY = mZ = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets any value to 0 if the absolute value of the component is
///          less than thresh.
///
///   \param[in] thresh The threshold value to be less than.
///
////////////////////////////////////////////////////////////////////////////////////
void Point3D::Floor(const double thresh)
{
    if( fabs(mX) <= thresh )
    {
        mX = 0.0;
    }
    if( fabs(mY) <= thresh )
    {
        mY = 0.0;
    }
    if( fabs(mZ) <= thresh )
    {
        mZ = 0.0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to a string.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Point3D::ToString(const int precision) const
{
    std::stringstream str;
    double x, y, z;
    x = fabs(mX) < .0000001 ? 0.0 : mX;
    y = fabs(mY) < .0000001 ? 0.0 : mY;
    z = fabs(mZ) < .0000001 ? 0.0 : mZ;

    str << std::fixed << std::setprecision(precision) << "<" << x << ", " << y << ", " << z << ">";
    return str.str();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints out point data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void Point3D::Print() const
{
    double x, y, z;
    x = fabs(mX) < .0000001 ? 0.0 : mX;
    y = fabs(mY) < .0000001 ? 0.0 : mY;
    z = fabs(mZ) < .0000001 ? 0.0 : mZ;
    std::cout << ToString() << "\n";
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if the vectors the poitns represent are parallel.
///
///   \param p The vector to see if we are parallel with.
///
///   \return True if the vectors the points represent
///           are parallel, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsParallel(const Point3D& p) const
{
    double dx1, dx2;
    double dy1, dy2;
    double dz1, dz2;

    dx1 = -mX;
    dx2 = -p.mX;

    dy1 = -mY;
    dy2 = -p.mY;

    dz1 = -mZ;
    dz2 = -p.mZ;

    if(fabs(dy1/(dx1 + CX_EPSILON) - dy2/(dx2 + CX_EPSILON)) > .000000001)
        return false;
    if(fabs(dz1/(dy1 + CX_EPSILON) - dz2/(dy2 + CX_EPSILON)) > .000000001)
        return false;
    if(fabs(dx1/(dz1 + CX_EPSILON) - dx2/(dz2 + CX_EPSILON)) > .000000001)
        return false;
    //  If they are all equal, return true.
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Determines if all four points are within the same plane.
///
///   \param p1 Point on plane.
///   \param p2 Point on plane.
///   \param p3 Point on plane.
///
///   \return True if all points are within the same plane, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsCoplanar(const Point3D& p1, const Point3D& p2, const Point3D& p3) const
{
    Vector3D ab(p1.mX - mX, p1.mY - mY, p1.mZ - mZ);
    Vector3D ac(p2.mX - mX, p2.mY - mY, p2.mZ - mZ);
    Vector3D ad(p3.mX - mX, p3.mY - mY, p3.mZ - mZ);

    if(fabs(ab.Dot(ac*ad)) < CX_EPSILON)
        return true;

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the points are in the same line, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsCollinear(const Point3D &p1,
                          const Point3D &p2) const
{
    double dx1, dx2;
    double dy1, dy2;
    double dz1, dz2;
    double cx, cy, cz;
    dx1 = p1.mX - mX;
    dy1 = p1.mY - mY;
    dz1 = p1.mZ - mZ;
    dx2 = p2.mX - mX;
    dy2 = p2.mY - mY;
    dz2 = p2.mZ - mZ;

    //  Perform 3D cross product
    cx = dy1*dz1 - dy2*dz1;
    cy = dx2*dz1 - dx1*dz2;
    cz = dx1*dy2 - dx2*dy1;

    //  Check for 0 degree angle.
    if(fabs(cx*cx + cy*cy + cz*cz) < CX_EPSILON)
        return true;
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the points are within the triangle defined
///          by the three points, but does not check if the point is
///          in the same plane as the triangle.
///
///  \param p1 First point of triangle.
///  \param p2 Second point of triangle.
///  \param p3 Final point of triangle.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsInside(const Point3D &p1, const Point3D &p2, const Point3D &p3) const
{
    Point3D u, v, w;
    u = p2 - p1;
    v = p3 - p1;
    double s, t;
    double uu, uv, vv, wu, wv, d;
    uu = Dot(u, u);
    uv = Dot(u, v);
    vv = Dot(v, v);
    w = *this - p1;
    wu = Dot(w, u);
    wv = Dot(w, v);
    d = uv*uv - uu*vv;
    s = (uv*wv - vv*wu)/(d + CX_EPSILON);
    if(s < 0.0 || s > 1.0)
        return false;
    t = (uv*wu - uu*wv)/(d + CX_EPSILON);
    if(t < 0.0 || (s + t) > 1.0)
        return false;
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \return True if the points are within the 2D polygon.
///
///   Code is based from the following source:
///
///   http://cboard.cprogramming.com/c-programming/95481-point-polygon-test-spherical-coords.html
///
///  \param poly Points representing the polygon.
///
///  \return True if inside polygon.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsInside2D(const Point3D::List& poly) const
{
    int oddNodes;
    size_t i, j;
    double dy, dxi, dxj;
    int n = (int)poly.size();
    oddNodes=0;
    j=n-1;
    for(i=0; i < (size_t)n; i++)
    {
        if((poly[i].mY < this->mY && poly[j].mY >= this->mY) ||
            (poly[j].mY < this->mY && poly[i].mY >= this->mY))
        {
            dxi = this->mX - poly[i].mX;
            dxj = this->mX - poly[j].mX;
            if(0 < dxi && 0 < dxj)
                oddNodes=1-oddNodes;
            else if(!(0 > dxi && 0 > dxj))
            {
                dy = poly[j].mY - poly[i].mY;
                if(dy > 0)
                {
                    if ((this->mY-poly[i].mY)*(poly[j].mX-poly[i].mX) < dxi*dy)
                        oddNodes=1-oddNodes;
                }
                else if((this->mY-poly[i].mY)*(poly[j].mX-poly[i].mX)> dxi*dy)
                    oddNodes=1-oddNodes;
            }
        }
        j=i;
    }
    return oddNodes > 0 ? true : false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the distance to the point.
///
///  \return Distance to point.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::Distance() const
{
    return sqrt(mX*mX  + mY*mY + mZ*mZ);
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the distance to the point.
///
///  \return Distance to point.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::Magnitude() const
{
    return sqrt(mX*mX  + mY*mY + mZ*mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the sum of the squares of the components.
///
///   \return Sum of the squares of the components.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::SumOfSquares() const
{
    return mX*mX + mY*mY + mZ*mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the distance to the point.
///
///  \param p The point to find the distance to.
///
///  \return Distance to point.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::Distance(const Point3D& p) const
{
    return sqrt((mX - p.mX)*(mX - p.mX) + (mY - p.mY)*(mY - p.mY)  + (mZ - p.mZ)*(mZ - p.mZ));
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief  Treats the 3D points as a vector for calculating the dot
///          product.
///
///  \param[in] p the piont to get the dot product with.
///
///  \return The dot product of current point vector with the other point.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::Dot(const Point3D& p) const
{
    return mX*p.mX + mY*p.mY + mZ*p.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Normalizes the vector the point represents.
///
///   \return The normalized vector the point represents.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::Normalize()
{
    double mag = sqrt(mX*mX + mY*mY + mZ*mZ);
    //  Check for divide by zero.
    if(mag < CX_EPSILON)
        mag = CX_EPSILON;
    mX /= mag;
    mY /= mag;
    mZ /= mag;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a copy of the normalized vector.  (Same vector but magnitude
///   is 0).  This is useful if you are treating points as vectors.
///
///   \return Copy of the normalized vector that the point represented.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::GetUnitVector() const
{
    double mag = sqrt(mX*mX + mY*mY + mZ*mZ);
    //  Check for divide by zero.
    if(mag < CX_EPSILON)
        mag = CX_EPSILON;
    return Point3D(mX/mag, mY/mag, mZ/mag);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the point around the origin by the angle given and
///          axis of rotation.
///
///   \param[in] angle The amount to rotate.
///   \param[in] axis The axis to rotate around, see Point3D::Axis enumerations.
///   \param[in] angleInDegrees If true the value of angle is in degrees, if false
///                             it is in radians (default).
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::Rotate(const double angle,
                        const unsigned int axis,
                        const bool angleInDegrees) const
{
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

    return rotation.Rotate(*this);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the point around another point.
///
///   \param[in] pivot The 3D point to rotate around instead of origin.
///   \param[in] angle The amount to rotate.
///   \param[in] axis The axis to rotate around, see Point3D::Axis enumerations.
///   \param[in] angleInDegrees If true the value of angle is in degrees, if false
///                             it is in radians (default).
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::Rotate(const Point3D& pivot,
                        const double angle,
                        const unsigned int axis,
                        const bool angleInDegrees) const
{
    Point3D point(mX - pivot.mX,
                  mY - pivot.mY,
                  mZ - pivot.mZ);

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

    return pivot + rotation.Rotate(point);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the midpoint between points.
///
///  \param p The point to find midpoint to.
///
///  \return Midpoint data.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::Midpoint(const Point3D& p) const
{
    return Point3D( (p.mX + mX)/2, (p.mY + mY)/2, (p.mZ + mZ)/2);
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Gets the midpoint between points.
///
///  \param p1 First point in segment.
///  \param p2 Second point in segment.
///
///  \return Midpoint data.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::Midpoint(const Point3D& p1, const Point3D& p2)
{
    return Point3D( (p1.mX + p2.mX)/2, (p1.mY + p2.mY)/2, (p1.mZ + p2.mZ)/2);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Checks to see if four points are coplanar.
///
///  \param[in] p1 Point to check for within a plane.
///  \param[in] p2 Point to check for within a plane.
///  \param[in] p3 Point to check for within a plane.
///  \param[in] p4 Point to check for within a plane.
///
///  \return True if the points are all in the same plane.
///
////////////////////////////////////////////////////////////////////////////////////
bool Point3D::IsCoplanar(const Point3D& p1,
                         const Point3D& p2,
                         const Point3D& p3,
                         const Point3D& p4)
{
    return p1.IsCoplanar(p2, p3, p4);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets the values of x, y, and z.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator()(const double x, const double y, const double z)
{
    mX = x;
    mY = y;
    mZ = z;
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator=(const Point3D& p)
{
    mX = p.mX;
    mY = p.mY;
    mZ = p.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds the point.
///
///  \param p The point to add.
///
///  \return The result of addition.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator+=(const Point3D& p)
{
    mX += p.mX;
    mY += p.mY;
    mZ += p.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Subtracts the point.
///
///  \param p The point to subtract.
///
///  \return The result of subtraction.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator-=(const Point3D& p)
{
    mX -= p.mX;
    mY -= p.mY;
    mZ -= p.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Multiplies the point by a scalar value.
///
///  \param val The value to multiply with.
///
///  \return The result of multiplication.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator*=(const double val)
{
    mX *= val;
    mY *= val;
    mZ *= val;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Divides the point by a scalar value.
///
///  \param val The value to divide with.
///
///  \return The result of division by scaler value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D& Point3D::operator/=(const double val)
{
    mX /= val;
    mY /= val;
    mZ /= val;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Adds the point.
///
///  \param p The point to add.
///
///  \return The result of addition.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::operator+(const Point3D& p) const
{
    return Point3D(mX + p.mX, mY + p.mY, mZ + p.mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Subtracts the point.
///
///  \param p The point to subtract.
///
///  \return The result of subtraction.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::operator-(const Point3D& p) const
{
    return Point3D(mX - p.mX, mY - p.mY, mZ - p.mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates the cross product.
///
///  \param[in] p The point to get cross product with.
///
///  \return The cross product of thet wo points.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::operator*(const Point3D& p) const
{
    Point3D r;

    r.mX = mY*p.mZ - mZ*p.mY;
    r.mY = -1.0*(mX*p.mZ - mZ*p.mX);
    r.mZ = mX*p.mY - mY*p.mX;

    return r;
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Multiples the point by the scalar value.
///
///  \param[in] scalar The value to multiply the point values by.
///
///  \return A copy of the point with the values multiplied by the scalar value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::operator*(const double scalar) const
{
    return Point3D(mX*scalar, mY*scalar, mZ*scalar);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Divides the point by the scalar value.
///
///  \param[in] scalar The value to divide the point values by.
///
///  \return A copy of the point with the values divided by the scalar value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Point3D::operator/(const double scalar) const
{
    return Point3D(mX/scalar, mY/scalar, mZ/scalar);
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates the distance between points.
///
///  \param p1 The first point.
///  \param p2 The second point.
///
///  \return Distance between points.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::Distance(const Point3D& p1, const Point3D& p2)
{
    return sqrt( (p1.mX - p2.mX)*(p1.mX - p2.mX) + (p1.mY - p2.mY)*(p1.mY - p2.mY) + (p1.mZ - p2.mZ)*(p1.mZ - p2.mZ));
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Calculates a linear regression of a list of points and returns slope
///         of the line in 2D XY plane.
///
///  \param[in] points Points to regress.
///
///  \return Slope of line. Y/X.
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::LinearRegressionSlope(const Point3D::List& points)
{
    double slopeXY = 0;
    double numerator = 0, denominator = 0, sumXY = 0;
    CxUtils::Point3D sumPoints, sumPoints2;
    if(points.size() < 2) return 0;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        sumPoints += points[i];
        sumPoints2.mX += points[i].mX * points[i].mX;
        sumPoints2.mY += points[i].mY * points[i].mY;
        sumXY += points[i].mX * points[i].mY;
    }
    numerator = ((double)points.size()) * sumXY - sumPoints.mX * sumPoints.mY;
    denominator = ((double)points.size()) * sumPoints2.mX - sumPoints.mX * sumPoints.mX;

    if(denominator < CxUtils::CX_EPSILON)
    {
        slopeXY = 1.0/CxUtils::CX_EPSILON;
    }
    else
    {
        slopeXY = numerator/denominator;
    }

    return slopeXY;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Converts the slope of the line of the XY plane points to an angle
///         [-PI, PI].
///
///  \param[in] points Points to regress.
///
///  \return Angle of line from [-PI, PI].
///
////////////////////////////////////////////////////////////////////////////////////
double Point3D::GetLinearRegressionAngle(const Point3D::List& points)
{
    double slopeXY = 0;
    double numerator = 0, denominator = 0, sumXY = 0;
    CxUtils::Point3D sumPoints, sumPoints2;
    if(points.size() < 2) return 0;

    for(unsigned int i = 0; i < points.size(); i++)
    {
        sumPoints += points[i];
        sumPoints2.mX += points[i].mX * points[i].mX;
        sumPoints2.mY += points[i].mY * points[i].mY;
        sumXY += points[i].mX * points[i].mY;
    }
    numerator = ((double)points.size()) * sumXY - sumPoints.mX * sumPoints.mY;
    denominator = ((double)points.size()) * sumPoints2.mX - sumPoints.mX * sumPoints.mX;

    //if(denominator < CxUtils::CX_EPSILON)
    //{
    //    slopeXY = 1.0/CxUtils::CX_EPSILON;
    //}
    //else
    //{
    //    slopeXY = numerator/denominator;
    //}
    return atan2(numerator,denominator);
}
/*  End of File */
