////////////////////////////////////////////////////////////////////////////////////
///
///  \file boundingbox.cpp
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
#include "cartographer/boundingbox.h"
#include <cxutils/math/cxmath.h>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Sets default values.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox() :
mDistanceToFront(0.0),
mDistanceToBack(0.0),
mDistanceToLeft(0.0),
mDistanceToRight(0.0),
mDistanceToTop(0.0),
mDistanceToBottom(0.0)
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Creates bounding box from 2 points.
///   
///   \param[in] p1 first point box should contain.
///   \param[in] p2 second point box should contain.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox(const Point3D& p1, const Point3D& p2)
{
    double maxX = max(p1.mX, p2.mX);
    double minX = min(p1.mX, p2.mX);
    double maxY = max(p1.mY, p2.mY);
    double minY = min(p1.mY, p2.mY);
    double maxZ = max(p1.mZ, p2.mZ);
    double minZ = min(p1.mZ, p2.mZ);
    (*this) = CreateBoundingBox(minX,maxX,maxY,minY,maxZ,minZ);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox(const BoundingBox& boundingBox) :
mDistanceToFront(boundingBox.mDistanceToFront),
mDistanceToBack(boundingBox.mDistanceToBack),
mDistanceToLeft(boundingBox.mDistanceToLeft),
mDistanceToRight(boundingBox.mDistanceToRight),
mDistanceToTop(boundingBox.mDistanceToTop),
mDistanceToBottom(boundingBox.mDistanceToBottom),
mOrigin(boundingBox.mOrigin)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Sets attributes based on parameters passed.
///
///   \param[in] origin The origin of the bounding box.
///   \param[in] distanceToLeft Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToRight Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToFront Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToBack Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToTop Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToBottom Distance in meters to the side of the box 
///                             from origin.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox(const Point3D& origin,
                         const double distanceToLeft,
                         const double distanceToRight,
                         const double distanceToFront,
                         const double distanceToBack,
                         const double distanceToTop ,
                         const double distanceToBottom) :
mDistanceToFront(distanceToFront),
mDistanceToBack(distanceToBack),
mDistanceToLeft(distanceToLeft),
mDistanceToRight(distanceToRight),
mDistanceToTop(distanceToTop),
mDistanceToBottom(distanceToBottom),
mOrigin(origin)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.  Sets attributes based on parameters passed.
///
///   \param[in] distanceToLeft Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToRight Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToFront Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToBack Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToTop Distance in meters to the side of the box 
///                             from origin.
///   \param[in] distanceToBottom Distance in meters to the side of the box 
///                             from origin.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::BoundingBox (const double left,
                          const double right,
                          const double front,
                          const double back,
                          const double top,
                          const double bottom)
{
    (*this) = BoundingBox(Point3D(), left,right,front,back,top,bottom);
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox::~BoundingBox()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if bounding boxes intersect/collide with
///          each other.
///
///   \param[in] boundingBox The box to check for intersection with.
///   \param[in] allowedError Allowed error for intersection. In this
///                           case, if distance is less than this value
///							  method will return true.
///
///   \return True if boxes intersect, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool BoundingBox::Intersects(const BoundingBox& boundingBox, const double allowedError) const
{
    if(GetDistance(boundingBox) <= allowedError)
    {
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the distance between this bounding box and another.
///
///   \param[in] boundingBox The box to get the distance to.
///
///   \return Distance to bounding box.  A value of 0 means they intersect.
///
////////////////////////////////////////////////////////////////////////////////////
double BoundingBox::GetDistance(const BoundingBox& boundingBox) const
{
    double dx = 0, dy = 0, dz = 0;
       
    if(GetRight() <= boundingBox.GetLeft())  // I'm to the left of boundingBox
    {
        dx = GetRight() - boundingBox.GetLeft();
    }
    if(boundingBox.GetRight() <= GetLeft()) // I'm to the right of boundingBox
    {
        dx = boundingBox.GetRight() - GetLeft();
    }
    
    if(GetFront() <= boundingBox.GetBack()) // I'm behind boundingBox
    {
        dy = GetFront() - boundingBox.GetBack();
    }
    if(boundingBox.GetFront() <= GetBack()) // I'm in front of boundingBox
    {
        dy = boundingBox.GetFront() - GetBack();
    }

    if(GetTop() <= boundingBox.GetBottom()) // I'm below boundingBox
    {
        dz = GetTop() - boundingBox.GetBottom();
    }
    if(boundingBox.GetTop() <= GetBottom()) // I'm on top of boundingBox
    {
        dz = boundingBox.GetTop() - GetBottom();
    }

    return sqrt(dx*dx + dy*dy + dz*dz);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the 3D point based on the edge on the box desired.
///   
///   Us this method to get the coordinate of the top left corners, sides, etc. of
///   the box in 3D space.
///
///   \param[in] edge The coordinate you want to get.
///
///   \return 3D point of desired coordinate.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D BoundingBox::GetPosition(const Edge edge) const
{
    Point3D point;

    switch(edge)
    {
    case FrontLeftTop:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY + mDistanceToFront, mOrigin.mZ + mDistanceToTop);
        break;
    case FrontLeftBottom:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY + mDistanceToFront, mOrigin.mZ - mDistanceToBottom);
        break;
    case Front:
        point(mOrigin.mX, mOrigin.mY + mDistanceToFront, mOrigin.mZ);
        break;
    case FrontTop:
        point(mOrigin.mX, mOrigin.mY + mDistanceToFront, mOrigin.mZ + mDistanceToTop);
        break;
    case FrontBottom:
        point(mOrigin.mX, mOrigin.mY + mDistanceToFront, mOrigin.mZ - mDistanceToBottom);
        break;
    case FrontRight:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY + mDistanceToFront, mOrigin.mZ + mDistanceToTop);
        break;
    case FrontRightTop:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY + mDistanceToFront, mOrigin.mZ);
        break;
    case FrontRightBottom:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY + mDistanceToFront, mOrigin.mZ - mDistanceToBottom);
        break;
    case BackLeft:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY - mDistanceToBack, mOrigin.mZ);
        break;
    case BackLeftBottom:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY - mDistanceToBack, mOrigin.mZ - mDistanceToBottom);
        break;
    case BackLeftTop:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY - mDistanceToBack, mOrigin.mZ + mDistanceToTop);
        break;
    case Back:
        point(mOrigin.mX, mOrigin.mY - mDistanceToBack, mOrigin.mZ);
        break;
    case BackTop:
        point(mOrigin.mX, mOrigin.mY - mDistanceToBack, mOrigin.mZ + mDistanceToTop);
        break;
    case BackBottom:
        point(mOrigin.mX, mOrigin.mY - mDistanceToBack, mOrigin.mZ - mDistanceToBottom);
        break;
    case BackRight:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY - mDistanceToBack, mOrigin.mZ);
        break;
    case BackRightTop:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY - mDistanceToBack, mOrigin.mZ + mDistanceToTop);
        break;
    case BackRightBottom:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY - mDistanceToBack, mOrigin.mZ - mDistanceToBottom);
        break;
    case Left:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY, mOrigin.mZ);
        break;
    case LeftTop:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY, mOrigin.mZ + mDistanceToTop);
        break;
    case LeftBottom:
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY, mOrigin.mZ - mDistanceToBottom);
        break;
    case Right:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY, mOrigin.mZ);
        break;
    case RightTop:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY, mOrigin.mZ + mDistanceToTop);
        break;
    case RightBottom:
        point(mOrigin.mX + mDistanceToRight, mOrigin.mY, mOrigin.mZ - mDistanceToBottom);
        break;
    default: // FrontLeft
        point(mOrigin.mX - mDistanceToLeft, mOrigin.mY + mDistanceToFront, mOrigin.mZ);
        break;
    }
    return point;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a value to the bounding box to grow/shrink it's size.
///
///   \param[in] distance Amount to increase/decrease size by.
///
///   \return Modified bounding box.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox& BoundingBox::operator+=(const double distance)
{
    mDistanceToFront += distance;
    mDistanceToBack += distance;
    mDistanceToLeft += distance;
    mDistanceToRight += distance;
    mDistanceToTop += distance;
    mDistanceToBottom += distance;

    if(distance < 0)
    {
        if(mDistanceToFront < 0)
        {
            mDistanceToFront = 0.0;
        }
        if(mDistanceToBack < 0)
        {
            mDistanceToBack = 0.0;
        }
        if(mDistanceToLeft < 0)
        {
            mDistanceToLeft = 0.0;
        }
        if(mDistanceToRight < 0)
        {
            mDistanceToRight = 0.0;
        }
        if(mDistanceToTop < 0)
        {
            mDistanceToTop = 0.0;
        }
        if(mDistanceToBottom < 0)
        {
            mDistanceToBottom = 0.0;
        }
    }

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox& BoundingBox::operator=(const BoundingBox& boundingBox)
{
    if(this != &boundingBox)
    {
        mDistanceToFront = boundingBox.mDistanceToFront;
        mDistanceToBack = boundingBox.mDistanceToBack;
        mDistanceToLeft = boundingBox.mDistanceToLeft;
        mDistanceToRight = boundingBox.mDistanceToRight;
        mDistanceToTop = boundingBox.mDistanceToTop;
        mDistanceToBottom = boundingBox.mDistanceToBottom;
        mOrigin = boundingBox.mOrigin;
    }

    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Bounds a point and radius(circle).
///
///   \param[in] origin point of the circle to bound.
///   \param[in] radius of circle to bound.
///
///   \return Bounding box that bounds circle.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox BoundingBox::CreateBoundingBox(const Point3D& origin, const double radius)
{
    return BoundingBox(origin, radius, radius, radius, radius, radius, radius);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Bounds line segment that includes width.
///
///   \param[in] segment to bound.
///
///   \return Bounding box that bounds the segment including width.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox BoundingBox::CreateBoundingBox(const Segment3D& segment)
{
    Point3D origin = segment.GetMidpoint();
    double distanceToLeft = fabs(origin.mX - (min(segment.mPoint1.mX, segment.mPoint2.mX) - segment.mWidth/2.)); 
    double distanceToRight = fabs(origin.mX - (segment.mWidth/2. + max(segment.mPoint1.mX, segment.mPoint2.mX))); 
    double distanceToFront = fabs(origin.mY - (segment.mWidth/2. + max(segment.mPoint1.mY, segment.mPoint2.mY))); 
    double distanceToBack = fabs(origin.mY - (min(segment.mPoint1.mY, segment.mPoint2.mY) - segment.mWidth/2.));
    double distanceToTop = fabs(origin.mZ - (segment.mWidth/2. + max(segment.mPoint1.mZ, segment.mPoint2.mZ))); 
    double distanceToBottom = fabs(origin.mZ - (min(segment.mPoint1.mZ, segment.mPoint2.mZ) - segment.mWidth/2.));
    return BoundingBox(origin,distanceToLeft,distanceToRight,distanceToFront,distanceToBack,distanceToTop,distanceToBottom);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Bounds three points(triangle);
///
///   \param[in] p1 first point.
///   \param[in] p2 second point.
///   \param[in] p3 third point.
///
///   \return Bounding box that bounds the triangle.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox BoundingBox::CreateBoundingBox(const Point3D& p1, 
                                           const Point3D& p2,
                                           const Point3D& p3)
{
    double maxX = max(p1.mX,max(p2.mX,p3.mX));
    double minX = min(p1.mX,min(p2.mX,p3.mX));
    double maxY = max(p1.mY,max(p2.mY,p3.mY));
    double minY = min(p1.mY,min(p2.mY,p3.mY));
    double maxZ = max(p1.mZ,max(p2.mZ,p3.mZ));
    double minZ = min(p1.mZ,min(p2.mZ,p3.mZ));


    return CreateBoundingBox(minX,maxX,maxY,minY,maxZ,minZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates bounding box based on absolute coordinates of its faces.
///          coordinates are in the global frame, not relative to an object.
///
///   \param[in] left side of box in xz plane as a value on the x axis.
///   \param[in] right side of box in xz plane as a value on the x axis.
///   \param[in] front of box in xz plane as a value on the y axis.
///   \param[in] back of box in xz plane as a value on the y axis.
///   \param[in] top of box in xz plane as a value on the z axis.
///   \param[in] bottom of box in xz plane as a value on the z axis.
///
///   \return Bounding box that bounds these values.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox BoundingBox::CreateBoundingBox(const double left,
                                           const double right,
                                           const double front,
                                           const double back,
                                           const double top,
                                           const double bottom)
{


    Point3D origin = Point3D((left+right)/2.,(front+back)/2.,(top+bottom)/2.);
    double distanceToFront = fabs(origin.mY - front);
    double distanceToBack = fabs(origin.mY - back);
    double distanceToLeft = fabs(origin.mX - left); 
    double distanceToRight = fabs(origin.mX - right); 
    double distanceToTop = fabs(origin.mZ - top); 
    double distanceToBottom = fabs(origin.mZ - bottom); 
    return BoundingBox(origin,distanceToLeft,distanceToRight,distanceToFront,distanceToBack,distanceToTop,distanceToBottom);

}


#ifdef USE_OPENCV

#endif
/*  End of File */
