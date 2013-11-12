////////////////////////////////////////////////////////////////////////////////////
///
///  \file quaternion.cpp
///  \brief Simple quaternion data structure.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 May 2007
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
#include "cxutils/math/quaternion.h"
#include "cxutils/math/cxmath.h"
#include <math.h>
#include <iostream>
#include <iomanip>
#include <assert.h>

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Conjugate of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
CxUtils::Quaternion CxUtils::operator~(const CxUtils::Quaternion& q)
{
    return CxUtils::Quaternion(q.mW, -q.mX, -q.mY, -q.mZ);
}

using namespace CxUtils;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Creates a Quaternion with 0 rotatation (1, 0, 0, 0).
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion()
{
    mW = 1;
    mX = mY = mZ = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion(const Quaternion& q)
{
    mW = q.mW;
    mX = q.mX;
    mY = q.mY;
    mZ = q.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Initializes a rotation quatnerion given an axis and
///   angle.
///
///   \param axis The axis for rotation.
///   \param angle The angle of rotation.
///   \param degrees If true angle is in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion(const Point3D& axis,
                       const double angle,
                       const bool degrees)
{
    Point3D unit;
    mW = (degrees ? cos(CX_DEG2RAD(angle)/2.0) : cos(angle/2.0));
    unit = axis;
    unit.Normalize();
    unit *= (degrees ? sin(CX_DEG2RAD(angle)/2.0) : sin(angle/2.0));
    mX = unit.mX;
    mY = unit.mY;
    mZ = unit.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion(const double w,
                       const double x,
                       const double y,
                       const double z)
{
    mW = w;
    mX = x;
    mY = y;
    mZ = z;
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates the quaternion using Euler angles.
///
///  \param x Rotation around the X-axis in radians (roll [-PI, PI]).
///  \param y Rotation around the Y-axis in radians (pitch [-PI/2, PI/2]).
///  \param z Rotation around the Z-axis in radians (yaw [-PI, PI]).
///           Positive rotation is clockwise.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion(const double x,
                       const double y,
                       const double z)
{
    CreateFromEuler(x, y, z, false);
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates the quaternion using Euler angles.
///
///  \param p Euler angles.
///  \param degrees If true angles are in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion::Quaternion(const Point3D& p, const bool degrees)
{
    CreateFromEuler(p.mX, p.mY, p.mZ, degrees);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a rotation quaternion given an axis and
///   angle.
///
///   \param axis The axis for rotation.
///   \param angle The angle of rotation.
///   \param degrees If true angle is in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::CreateRotation(const Point3D& axis,
                                const double angle,
                                const bool degrees)
{
    Point3D unit;
    mW = (degrees ? cos(CX_DEG2RAD(angle)/2.0) : cos(angle/2.0));
    unit = axis;
    unit.Normalize();
    unit *= (degrees ? sin(CX_DEG2RAD(angle)/2.0) : sin(angle/2.0));
    mX = unit.mX;
    mY = unit.mY;
    mZ = unit.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates the quaternion using Euler angles.
///
///  \param x Rotation around the X-axis (roll [-PI, PI]).
///  \param y Rotation around the Y-axis (pitch [-PI/2, PI/2]).
///  \param z Rotation around the Z-axis (yaw [-PI, PI]). (positive is clockwise).
///  \param degrees If true angles are in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::CreateFromEuler(const double x,
                                 const double y,
                                 const double z,
                                 bool degrees)
{
 
    double cosHalfTheta, sinHalfTheta;
    double cosHalfPsi, sinHalfPsi;
    double cosHalfPhi, sinHalfPhi;
    if(degrees) 
    {
        cosHalfTheta = cos(CX_DEG2RAD(y)/2.0); sinHalfTheta = sin(CX_DEG2RAD(y)/2.0);
        cosHalfPsi =   cos(CX_DEG2RAD(z)/2.0);     sinHalfPsi = sin(CX_DEG2RAD(z)/2.0);
        cosHalfPhi =   cos(CX_DEG2RAD(x)/2.0);    sinHalfPhi = sin(CX_DEG2RAD(x)/2.0);
    }
    else 
    {
        cosHalfTheta = cos(y/2.0); sinHalfTheta = sin(y/2.0);
        cosHalfPsi = cos(z/2.0);     sinHalfPsi = sin(z/2.0);
        cosHalfPhi = cos(x/2.0);    sinHalfPhi = sin(x/2.0);
    }

    //  Convet from Euler to quaternion.
    mW = cosHalfPhi*cosHalfTheta*cosHalfPsi + sinHalfPhi*sinHalfTheta*sinHalfPsi;
    mX = sinHalfPhi*cosHalfTheta*cosHalfPsi - cosHalfPhi*sinHalfTheta*sinHalfPsi;
    mY = cosHalfPhi*sinHalfTheta*cosHalfPsi + sinHalfPhi*cosHalfTheta*sinHalfPsi;
    mZ = cosHalfPhi*cosHalfTheta*sinHalfPsi - sinHalfPhi*sinHalfTheta*cosHalfPsi;
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates the quaternion using Euler angles.
///
///  \param p Euler angles.
///  \param degrees If true angles are in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::CreateFromEuler(const Point3D& p,
                                 bool degrees)
{
    CreateFromEuler(p.mX, p.mY, p.mZ, degrees);
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates a rotation quaternion around the axis.
///
///  \param z Rotation around Z-axis (yaw [-PI, PI]).
///  \param degrees If true the angle is in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::SetRotationZ(const double z, const bool degrees)
{
    mX = 0;
    mY = 0;
    mZ = (degrees ? sin(CX_DEG2RAD(z/2.0)) : sin(z/2.0));
    mW = (degrees ? cos(CX_DEG2RAD(z/2.0)) : cos(z/2.0));
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates a rotation quaternion around the axis.
///
///  \param y Rotation around Y-axis (pitch [-PI/2, PI/2]).
///  \param degrees If true the angle is in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::SetRotationY(const double y, const bool degrees)
{
    mZ= 0;
    mX = 0;
    mY = (degrees ? sin(CX_DEG2RAD(y/2.0)) : sin(y/2.0));
    mW = (degrees ? cos(CX_DEG2RAD(y/2.0)) : cos(y/2.0));
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Creates a rotation quaternion around the axis.
///
///  \param x Rotation around X-axis (roll [-PI, PI]).
///  \param degrees If true the angle is in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::SetRotationX(const double x, const bool degrees )
{
    mY = 0;
    mZ = 0;
    mX = (degrees ? sin(CX_DEG2RAD(x/2.0)) : sin(x/2.0));
    mW = (degrees ? cos(CX_DEG2RAD(x/2.0)) : cos(x/2.0));
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Converts the quaternion to Euler angles.
///
///  \param x Rotation around the X-axis (roll [-PI, PI]).
///  \param y Rotation around the Y-axis (pitch [-PI/2, PI/2]).
///  \param z Rotation around the Z-axis (yaw [-PI, PI]).
///  \param degrees If true, angles are provided in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::ConvertToEuler(double& x,
                                double& y,
                                double& z,
                                bool degrees) const
{
    x = atan2( (2.0*(mW*mX + mY*mZ)) , (1.0 - 2.0*(mX*mX + mY*mY)) );
    double t = 2.0*(mW*mY - mZ*mX);
    if(t < -1.0)
        t = -1.0;
    if(t > 1.0)
        t = 1.0;
    y = asin(t);
    z = atan2( (2.0*(mW*mZ + mX*mY)) , (1.0 - 2.0*(mY*mY + mZ*mZ)) );

    if(degrees)
    {
        x = CX_RAD2DEG(x);
        y = CX_RAD2DEG(y);
        z = CX_RAD2DEG(z);
    }
}


////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Converts the quaternion to Euler angles.
///
///  \param p Euler angles.
///  \param degrees If true, angles are provided in degrees, otherwise radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::ConvertToEuler(Point3D& p,
                                bool degrees) const
{
    ConvertToEuler(p.mX, p.mY, p.mZ, degrees);
}

////////////////////////////////////////////////////////////////////////////////////
/// 
///  \brief Converts the quaternion to Euler angles.
///
///  \param degrees If true, angles are provided in degrees, otherwise radians.
///
///  \return Euler angles as a Point3D(x, y, z);
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Quaternion::ConvertToEuler(bool degrees) const
{
    double
        x, y, z;
    ConvertToEuler(x, y, z, degrees);
    return Point3D(x, y, z);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets all values to 0.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::Clear()
{
    mW = mX = mY = mZ = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints quaternion information to console.
///
///   \param euler If true than quaternion is converted to Euler angles
///                for display.
///   \param degrees If Euler angles are beingn displayed, set to true for
///                  degrees instead of radians.
///
////////////////////////////////////////////////////////////////////////////////////
void Quaternion::Print(const bool euler, const bool degrees) const
{
    if(euler) 
    {
        Point3D e;
        ConvertToEuler(e, degrees);
        e.Print();
    }
    else
        std::cout << "<" << mW << ", " << mX << ", " << mY << ", " << mZ << ">\n";    
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Norm of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
double Quaternion::Norm() const
{
    return mW*mW + mX*mX + mY*mY + mZ*mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param q The quaternion to calculate norm for.
///
///   \return Norm of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
double Quaternion::Norm(const Quaternion& q)
{
    return q.mW*q.mW + q.mX*q.mX + q.mY*q.mY + q.mZ*q.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates dot product of quaternions.
///
///   \param q The quaternion to calculate dot product with.
///   
///   \return Dot product.
///
////////////////////////////////////////////////////////////////////////////////////
double Quaternion::Dot(const Quaternion& q) const
{
    return mW*q.mW + mX*q.mX + mY*q.mY + mZ*q.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates dot product of quaternions.
///
///   \param a Quaternion 1.
///   \param b Quaternion 2.
///   
///   \return Dot product of 2 quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
double Quaternion::Dot(const Quaternion& a, const Quaternion& b)
{
    return a.mW*b.mW + a.mX*b.mX + a.mY*b.mY + a.mZ*b.mZ;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return The Axis of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Quaternion::GetAxis() const
{
    return Point3D(mX, mY, mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Normalizes the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Normalize() const
{
    double n = sqrt(mW*mW + mX*mX + mY*mY + mZ*mZ);
    //  Prevent division by zero.
    if(n < CX_EPSILON)
        n = CX_EPSILON;
    return Quaternion(mW/n, mX/n, mY/n, mZ/n);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates normalized quaternion.
///
///   \param q The quaternion to get the normalization of.
///
///   \return The normalized quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Normalize(const Quaternion& q) 
{
    double n = sqrt(q.mW*q.mW + q.mX*q.mX + q.mY*q.mY + q.mZ*q.mZ);
    Quaternion r;
    //  Prevent division by zero.
    if(n < CX_EPSILON)
        n = CX_EPSILON;
    r.mW = q.mW/n;
    r.mX = q.mX/n;
    r.mY = q.mY/n;
    r.mZ = q.mZ/n;
    return r;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the quaternion to its' conjugate.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Conjugate() const
{
    return Quaternion(mW, -mX, -mY, -mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param q The quaternion to get the conjugate of.
///
///   \return The conjugate of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Conjugate(const Quaternion& q)
{
    Quaternion r;
    r.mW = q.mW;
    r.mX = -q.mX;
    r.mY = -q.mY;
    r.mZ = -q.mZ;
    return r;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Inverts the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Invert() const
{
    double base = mW*mW + mX*mX + mY*mY + mZ*mZ;
    //  Prevent divide by zero.
    if(base < CX_EPSILON)
        base = CX_EPSILON;
    return Quaternion(mW/base, -mX/base, -mY/base, -mZ/base);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \param q The quaternion to get the inverse of.
///
///   \return The inverse of the quaternion.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Invert(const Quaternion& q)
{
    Quaternion r;
    double base = q.mW*q.mW + q.mX*q.mX + q.mY*q.mY + q.mZ*q.mZ;
    //  Prevent divide by zero.
    if(base < CX_EPSILON)
        base = CX_EPSILON;

    r.mW = q.mW/base;
    r.mX = -q.mX/base;
    r.mY = -q.mY/base;
    r.mZ = -q.mZ/base;

    return r;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the point.
///
///   \param p The point to rotate.
///
///   \return The result of the rotation.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Quaternion::Rotate(const Point3D& p) const
{
    return (((*this)*p)*(~(*this))).GetAxis();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the point using the inverse of the quaternion.
///
///   \param p The p to rotate with.
///
///   \return The result of the rotation.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Quaternion::RotateInverse(const Point3D& p) const
{
    Quaternion q(mW, -mX, -mY, -mZ);
    return ((q*p)*(*this)).GetAxis();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Rotates the second quaternion over the first, and returns the
///   resulting quaternion.
///
///   \param q1 The quaternion to rotate around.
///   \param q2 The quaternion rotating.
///
///   \return The result of the rotation.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::Rotate(const Quaternion& q1, const Quaternion& q2)
{
    return q1*q2*(~q1);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs addition of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator+=(const Quaternion& q)
{
    mW += q.mW;
    mX += q.mX;
    mY += q.mY;
    mZ += q.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs addition of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::operator+(const Quaternion& q) const
{
    Quaternion result;
    result.mW = mW + q.mW;
    result.mX = mX + q.mX;
    result.mY = mY + q.mY;
    result.mZ = mZ + q.mZ;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs subtraction of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator-=(const Quaternion& q)
{
    mW -= q.mW;
    mX -= q.mX;
    mY -= q.mY;
    mZ -= q.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs subtraction of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::operator-(const Quaternion& q) const
{
    Quaternion result;
    result.mW = mW - q.mW;
    result.mX = mX - q.mX;
    result.mY = mY - q.mY;
    result.mZ = mZ - q.mZ;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs mulitiplication of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion &Quaternion::operator*=(const Quaternion& q)
{
    Quaternion t;
    t.mW = (mW*q.mW - mX*q.mX - mY*q.mY - mZ*q.mZ);
    t.mX = (mW*q.mX + mX*q.mW + mY*q.mZ - mZ*q.mY);
    t.mY = (mW*q.mY - mX*q.mZ + mY*q.mW + mZ*q.mX);
    t.mZ = (mW*q.mZ + mX*q.mY - mY*q.mX + mZ*q.mW);
    mW = t.mW;
    mX = t.mX;
    mY = t.mY;
    mZ = t.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs mulitiplication of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::operator*(const Quaternion& q) const
{
    Quaternion t;
    t.mW = (mW*q.mW - mX*q.mX - mY*q.mY - mZ*q.mZ);
    t.mX = (mW*q.mX + mX*q.mW + mY*q.mZ - mZ*q.mY);
    t.mY = (mW*q.mY - mX*q.mZ + mY*q.mW + mZ*q.mX);
    t.mZ = (mW*q.mZ + mX*q.mY - mY*q.mX + mZ*q.mW);
    return t;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs mulitiplication with point.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator*=(const Point3D& p)
{
    Quaternion q(0, p.mX, p.mY, p.mZ);
    *this *= q;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs mulitiplication with point.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::operator*(const Point3D& p) const
{
    Quaternion q(0, p.mX, p.mY, p.mZ);
    return (*this)*q;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs division of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator/=(const Quaternion& q)
{
    Quaternion t;
    double base = q.mW*q.mW + q.mX*q.mX + q.mY*q.mY + q.mZ*q.mZ;
    //  Prevent divide by zero.
    if(base < CX_EPSILON)
        base = CX_EPSILON;
    t.mW = (q.mW*mW + q.mX*mX + q.mY*mY + q.mZ*mZ)/base;
    t.mX = (q.mW*mX - q.mX*mW - q.mY*mZ + q.mZ*mY)/base;
    t.mY = (q.mW*mY + q.mX*mZ - q.mY*mW - q.mZ*mX)/base;
    t.mZ = (q.mW*mZ - q.mX*mY + q.mY*mX - q.mZ*mW)/base;
    mW = t.mW;
    mX = t.mX;
    mY = t.mY;
    mZ = t.mZ;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Performs division of the quaternions.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion Quaternion::operator/(const Quaternion& q) const
{
    Quaternion t;
    double base = q.mW*q.mW + q.mX*q.mX + q.mY*q.mY + q.mZ*q.mZ;
    //  Prevent divide by zero.
    if(base < CX_EPSILON)
        base = CX_EPSILON;
    t.mW = (q.mW*mW + q.mX*mX + q.mY*mY + q.mZ*mZ)/base;
    t.mX = (q.mW*mX - q.mX*mW - q.mY*mZ + q.mZ*mY)/base;
    t.mY = (q.mW*mY + q.mX*mZ - q.mY*mW - q.mZ*mX)/base;
    t.mZ = (q.mW*mZ - q.mX*mY + q.mY*mX - q.mZ*mW)/base;

    return t;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator=(const Quaternion& q)
{
    mW = q.mW;
    mX = q.mX;
    mY = q.mY;
    mZ = q.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Quaternion& Quaternion::operator=(const Point3D& p)
{
    mW = 0;
    mX = p.mX;
    mY = p.mY;
    mZ = p.mZ;

    return *this;
}


/*  End of File */
