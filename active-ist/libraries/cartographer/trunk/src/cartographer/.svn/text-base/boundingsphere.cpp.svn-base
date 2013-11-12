////////////////////////////////////////////////////////////////////////////////////
///
///  \file boundingsphere.cpp
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
#include "cartographer/boundingsphere.h"
#include <cxutils/math/cxmath.h>
#include <math.h>
#include <algorithm>

using namespace std;
using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere::BoundingSphere() : mRadius(0)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets values.
///
///   \param[in] origin Origin of the bounding sphere in 3D space.
///   \param[in] radius Radius out from the origin that the sphere covers. All
///                     measurements are in meters.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere::BoundingSphere(const Point3D& origin,
                               const double radius)
{
    mRadius = fabs(radius);
    mOrigin = origin;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
///   \param[in] boundingSphere Bounding sphere to copy from.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere::BoundingSphere(const BoundingSphere& boundingSphere)
{
    mRadius = boundingSphere.mRadius;
    mOrigin = boundingSphere.mOrigin;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere::~BoundingSphere(){}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method checks to see if the bounding spheres are touching or 
///          intersecting with each other.
///
///   \param[in] boundingSphere Bounding sphere to check for intersection with.
///
///   \return True if the two spheres intersect, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool BoundingSphere::Intersects(const BoundingSphere& boundingSphere) const
{
    if(GetDistance(boundingSphere) <= CxUtils::CX_EPSILON)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Measures the distance between the bounding spheres. 
///
///   \param[in] boundingSphere Bounding sphere get distance to.
///
///   \return Distance in meters.  A value of 0.0 means they touch or intersect
///           with each other.
///
////////////////////////////////////////////////////////////////////////////////////
double BoundingSphere::GetDistance(const BoundingSphere& boundingSphere) const
{
    double distance = 0.0;
    distance = mOrigin.Distance(boundingSphere.mOrigin) - mRadius - boundingSphere.mRadius;
    // If spheres intersect, then distance is less than 0.0.
    if(distance < 0.0)
    {
        distance = 0.0;
    }
    return distance;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a bounding sphere.
///
///   \param[in] origin Origin of the bounding sphere in 3D space.
///   \param[in] radius Radius out from the origin that the sphere covers. All
///                     measurements are in meters.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere BoundingSphere::CreateBoundingSphere(const Point3D& origin, const double radius)
{
    return BoundingSphere(origin, radius);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a bounding sphere.
///
///   \param[in] segment Segment to create bounding sphere from.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere BoundingSphere::CreateBoundingSphere(const Segment3D& segment)
{
    return BoundingSphere(segment.GetMidpoint(), segment.GetMagnitude() + segment.mWidth/2.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Creates a bounding sphere.
///
///   \param[in] p1 First point in polygon.
///   \param[in] p2 Second point in polygon.
///   \param[in] p3 Last point in polygon.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere BoundingSphere::CreateBoundingSphere(const Point3D& p1,
                                                    const Point3D& p2,
                                                    const Point3D& p3)
{
    double maxX = max(p1.mX,max(p2.mX,p3.mX));
    double minX = min(p1.mX,min(p2.mX,p3.mX));
    double maxY = max(p1.mY,max(p2.mY,p3.mY));
    double minY = min(p1.mY,min(p2.mY,p3.mY));
    double maxZ = max(p1.mZ,max(p2.mZ,p3.mZ));
    double minZ = min(p1.mZ,min(p2.mZ,p3.mZ));

    Point3D origin(p1.mX + p2.mX + p3.mX, 
                   p1.mY + p2.mY + p3.mY, 
                   p1.mZ + p2.mZ + p3.mZ);
    origin /= 3.0;
    double width = max(maxX - minX, max(maxY - minY, maxZ - minZ));
    return BoundingSphere(origin, width/2.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to the data.
///
///   \param[in] boundingSphere Bounding sphere to copy from.
///
///   \return The modified bounding sphere.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere& BoundingSphere::operator=(const BoundingSphere& boundingSphere)
{
    mRadius = boundingSphere.mRadius;
    mOrigin = boundingSphere.mOrigin;
    return *this;
}

#ifdef USE_OPENCV

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the object to the image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] color Color to set.
///
///   \return If object was able to be drawn.
///
////////////////////////////////////////////////////////////////////////////////////
bool BoundingSphere::Draw(IplImage* image, const double pixelsPerMeter, const CvPoint& offset, CvScalar color)
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;
    CvPoint center;
    center.x = (int)(pixelsPerMeter * (mOrigin.mX + offset.x));
    center.y = (int)(pixelsPerMeter * (mOrigin.mY + offset.y));
    cvCircle(image,center,(int)(pixelsPerMeter * mRadius),color,-1);
    return true;

}
#endif
/* End of File */
