////////////////////////////////////////////////////////////////////////////////////
///
///  \file boundingsphere.h
///  \brief A bounding sphere class for describing a objects area in 3D space.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 14 November 2008
///  <br>Copyright (c) 2008
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
#ifndef __LIB_CARTOGRAPHER_BOUNDING_SPHERE__H
#define __LIB_CARTOGRAPHER_BOUNDING_SPHERE__H

#include "cartographer/libcartdll.h"
#include <cxutils/math/point3d.h>
#include <string>


namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class BoundingSphere
    ///   \brief Data structure describing the sphere that encapsulates the entire
    ///          3D object.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL BoundingSphere
    {
    public:
        BoundingSphere();
        BoundingSphere(const Point3D& origin, 
                       const double radius);
        BoundingSphere(const BoundingSphere& boundingSphere);
        ~BoundingSphere();
        bool Intersects(const BoundingSphere& boundingSphere) const;
        double GetDistance(const BoundingSphere& boundingSphere) const;
        BoundingSphere& operator=(const BoundingSphere& boundingSphere);
        static BoundingSphere CreateBoundingSphere(const Point3D& origin, const double radius);
        static BoundingSphere CreateBoundingSphere(const Segment3D& segment);
        static BoundingSphere CreateBoundingSphere(const Point3D& p1, 
                                                   const Point3D& p2,
                                                   const Point3D& p3);
#ifdef USE_OPENCV
        bool Draw(IplImage* image, const double pixelsPerMeter, const CvPoint& offset, CvScalar color = CV_RGB(75, 75, 75));
#endif
        double mRadius;             ///<  Radius of bounding sphere from the origin (meters).
        Point3D mOrigin;            ///<  Origin of the objects bounding sphere in 3D space (meters).
    };
};

#endif
/*  End of File */
