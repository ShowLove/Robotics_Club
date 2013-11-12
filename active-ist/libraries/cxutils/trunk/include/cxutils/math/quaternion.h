////////////////////////////////////////////////////////////////////////////////////
///
///  \file quaternion.h
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
#ifndef __CXUTILS_MATH_QUATERNION_H
#define __CXUTILS_MATH_QUATERNION_H

#include "cxutils/cxbase.h"
#include "cxutils/math/point3d.h"

namespace CxUtils
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Quaternion
    ///   \brief Simple quaternion representation.
    ///
    ///   This quaternion data structure contains methods for most common
    ///   operations involving quaternions including multiplication, division, 
    ///   rotation, etc.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Quaternion
    {
    public:
        Quaternion();
        Quaternion(const Quaternion& q);
        Quaternion(const Point3D& p, const bool degrees = false);
        Quaternion(const double x, 
                   const double y, 
                   const double z);
        Quaternion(const Point3D &axis, 
                   const double angle,
                   bool degrees = false);
        Quaternion(const double w,
                   const double x,
                   const double y,
                   const double z);
        void CreateRotation(const Point3D &axis, 
                            const double angle,
                            bool degrees = false);
        void CreateFromEuler(const double x, 
                             const double y, 
                             const double z, 
                             bool degrees = false);
        void ConvertToEuler(double& x,
                            double& y,
                            double& z,
                            bool degrees = false) const;
        void CreateFromEuler(const Point3D& p, 
                             bool degrees = false);
        void ConvertToEuler(Point3D& p,
                            bool degrees = false) const;
        Point3D ConvertToEuler(bool degrees = false) const;
        void SetRotationX(const double x, const bool degrees = false);
        void SetRotationY(const double y, const bool degrees = false);
        void SetRotationZ(const double z, const bool degrees = false);
        void Clear();
        void Print(const bool euler = false, const bool degrees = true) const;
        double Norm() const;
        double Dot(const Quaternion&q) const;
        Point3D GetAxis() const;
        Quaternion Normalize() const;
        Quaternion Conjugate() const;
        Quaternion Invert() const;
        Point3D Rotate(const Point3D& p) const;
        Point3D RotateInverse(const Point3D& p) const;      
        static double Norm(const Quaternion& q);
        static double Dot(const Quaternion& a, const Quaternion& b);
        static Quaternion Normalize(const Quaternion& q);
        static Quaternion Conjugate(const Quaternion& q);
        static Quaternion Invert(const Quaternion& q);    
        static Quaternion Rotate(const Quaternion& q1, const Quaternion& q2);
        Quaternion& operator+=(const Quaternion& q);
        Quaternion operator+(const Quaternion& q) const;
        Quaternion& operator-=(const Quaternion& q);
        Quaternion operator-(const Quaternion& q) const;
        Quaternion& operator*=(const Quaternion& q);
        Quaternion operator*(const Quaternion& q) const;
        Quaternion& operator*=(const Point3D& p);
        Quaternion operator*(const Point3D& p) const;
        Quaternion& operator/=(const Quaternion& q);
        Quaternion operator/(const Quaternion& q) const;        
        Quaternion& operator=(const Quaternion& q);
        Quaternion& operator=(const Point3D& p);
        double mW;  ///<  Real number scalar of quaternion.
        double mX;  ///<  Part of vector (i).
        double mY;  ///<  Part of vector (j).
        double mZ;  ///<  Part of vector (k).
    };

    CX_UTILS_DLL Quaternion operator~(const Quaternion& q);
}

#endif
/*  End of File */
