////////////////////////////////////////////////////////////////////////////////////
///
///  \file boundingbox.h
///  \brief A bounding box class for describing a objects area in 3D space.
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
#ifndef __LIB_CARTOGRAPHER_BOUNDING_BOX__H
#define __LIB_CARTOGRAPHER_BOUNDING_BOX__H

#include "cartographer/libcartdll.h"
#include "cartographer/boundingsphere.h"
#include <cxutils/math/point3d.h>
#include <string>

////////////////////////////////////////////////////////////////////////////////////
///
///   \namespace Cartographer
///   \brief Cartographer is the namespace for all interfaces and structures 
///          within the Cartographer Robot Navigation Library.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class BoundingBox
    ///   \brief Data structure describing the box that encapsulates the entire
    ///          3D object.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL BoundingBox
    {
    public:
        // The different edges on the bounding box to get the 3D position of.
        enum Edge
        {
            FrontLeft = 0,
            FrontLeftTop,
            FrontLeftBottom,
            Front,
            FrontTop,
            FrontBottom,
            FrontRight,
            FrontRightTop,
            FrontRightBottom,
            BackLeft,
            BackLeftBottom,
            BackLeftTop,
            Back,
            BackTop,
            BackBottom,
            BackRight,
            BackRightTop,
            BackRightBottom,
            Left,
            LeftTop,
            LeftBottom,
            Right,
            RightTop,
            RightBottom
        };
        BoundingBox();
        BoundingBox(const BoundingBox& boundingBox);
        BoundingBox(const Point3D& origin,
                    const double distanceToLeft,
                    const double distanceToRight,
                    const double distanceToFront,
                    const double distanceToBack,
                    const double distanceToTop = 0.0,
                    const double distanceToBottom = 0.0);
        BoundingBox (const double left,
                     const double right,
                     const double front,
                     const double back,
                     const double top,
                     const double bottom);
        BoundingBox(const Point3D& p1, const Point3D& p2);
        ~BoundingBox();
        inline double GetLeft() const { return mOrigin.mX - mDistanceToLeft; }      // Map X axis
        inline double GetRight() const { return mOrigin.mX + mDistanceToRight; }    // Map X axis
        inline double GetTop() const { return mOrigin.mZ + mDistanceToTop; }        // Map Z axis
        inline double GetBottom() const { return mOrigin.mZ - mDistanceToBottom; }  // Map Z axis
        inline double GetFront() const { return mOrigin.mY + mDistanceToFront; }    // Map Y axis
        inline double GetBack() const { return mOrigin.mY - mDistanceToBack; }      // Map Y axis
        inline double Width() const { return (mDistanceToLeft + mDistanceToRight); }
        inline double Length() const { return (mDistanceToFront + mDistanceToBack); }
        inline double Height() const { return (mDistanceToTop + mDistanceToBottom); }
        bool Intersects(const BoundingBox& boundingBox, const double allowedError = CxUtils::CX_EPSILON) const;
        double GetDistance(const BoundingBox& boundingBox) const;
        // Get the 3D coordinate of an edge on the box.
        Point3D GetPosition(const Edge edge) const;
        BoundingBox& operator+=(const double distance);
        BoundingBox& operator=(const BoundingBox& boundingBox);
        static BoundingBox CreateBoundingBox(const Point3D& origin, const double radius);
        static BoundingBox CreateBoundingBox(const Segment3D& segment);
        static BoundingBox CreateBoundingBox(const Point3D& p1, 
                                             const Point3D& p2,
                                             const Point3D& p3);
        static BoundingBox CreateBoundingBox(const double left,
                                             const double right,
                                             const double front,
                                             const double back,
                                             const double top,
                                             const double bottom);
#ifdef USE_OPENCV
        
#endif
        double mDistanceToFront;    ///<  Distance to front side of object relative to origin (map coordinates).
        double mDistanceToBack;     ///<  Distance back side of object relative to origin (map coordinates).
        double mDistanceToLeft;     ///<  Distance to left side of object relative to origin (map coordinates).
        double mDistanceToRight;    ///<  Distance to right side of object relative to origin (map coordinates).
        double mDistanceToTop;      ///<  Distance to top side of object relative to origin (map coordinates).
        double mDistanceToBottom;   ///<  Distance to bottom side of object relative to origin (map coordinates).
        Point3D mOrigin;            ///<  Origin of the objects bounding box in 3D space (map coordinates).
    };
};

#endif
/*  End of File */
