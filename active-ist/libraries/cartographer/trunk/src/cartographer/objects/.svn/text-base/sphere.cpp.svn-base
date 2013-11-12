////////////////////////////////////////////////////////////////////////////////////
///
///  \file sphere.cpp
///  \brief Object class for describing a 3D point sphere object.  This object
///  type is useful for descrbing point or basic range detection data from
///  sensors.
///
///  <br>Author(s): Daniel Barber, Michael Scherer
///  <br>Created: 25 December 2008
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
#include "cartographer/objects/sphere.h"
#include <math.h>
#include <iostream>
#include <cstdio>

using namespace Cartographer;
using namespace Objects;

const std::string Sphere::Name = "Sphere";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] sourceID The object source or creator ID.
///
////////////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(const unsigned int sourceID) : Object(Object::Point, "Sphere", sourceID)
{
    mRadius = 0.0;
    mTotalConsumed = 0;
    mSumOfRadiusValues = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] origin The origin of the sphere in 3D space.
///   \param[in] radius The radius of the sphere in meters.
///   \param[in] sourceID The object source or creator ID.
///
////////////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(const Point3D& origin, const double radius, const unsigned int sourceID) : Object(Object::Point, "Sphere", sourceID),
        mOrigin(origin),
        mRadius(radius)
{
    mTotalConsumed = 0;
    mSumOfRadiusValues = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] sphere The sphere to be equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Sphere::Sphere(const Sphere& sphere) : Object(Object::Point, "Sphere", sphere.mSourceID),
        mOrigin(sphere.mOrigin),
        mRadius(sphere.mRadius),
        mSumOfPointValues(sphere.mSumOfPointValues),
        mSumOfRadiusValues(sphere.mSumOfRadiusValues),
        mTotalConsumed(sphere.mTotalConsumed)
{
    CopyObjectData(&sphere);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Sphere::~Sphere()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A bounding sphere which encapsulates the object.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere Sphere::GetBoundingSphere() const
{
    return BoundingSphere(mOrigin, mRadius);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A Bounding Box which encapsulates the object in 3D space.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox Sphere::GetBoundingBox() const
{
    return BoundingBox(mOrigin, mRadius, mRadius, mRadius, mRadius, mRadius, mRadius);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if the object is the same as the argument.
///
///   This method is used to check if two objects are the same so that there
///   metrics can be combined or merged using the ConsumeMatch method.  This
///   is useful when you have some error associated with your sensor data.
///
///   Object Type, Source ID, and Identification must match.
///
///   \param[in] object Object to check for equality with.
///   \param[in] allowedError The acceptable amount of error for a match.
///
///   \return True if the same object, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::IsSameAs(const Object* object,
                      const double allowedError) const
{
    const Sphere* sphere = dynamic_cast<const Sphere*>(object);
    if (sphere && sphere->mSourceID == mSourceID)
    {
        if (fabs(mRadius - sphere->mRadius) < allowedError &&
                mOrigin.Distance(sphere->mOrigin) < allowedError &&
                mObjectIdentification == sphere->mObjectIdentification)
        {
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Consumed the matching object data by merging the values with those
///   of this object.
///
///   This method performs a running average of all consumed points to attempt
///   at discovering the most correct location and parameters of the object over
///   time.
///
///   This method does not check for equality, it only consumes the data.
///
///   \param[in] object Matching object data to consume.  The IsSameAs method
///              is used to check for a match.
///
///   \return True if consumed, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::ConsumeMatch(const Object* object)
{
    const Sphere* sphere = dynamic_cast<const Sphere*>(object);
    if (sphere)
    {
        if (mTotalConsumed == 0)
        {
            mSumOfPointValues = mOrigin;
            mSumOfRadiusValues = mRadius;
            mTotalConsumed = 1;
        }
        // If the sphere being consumed has not
        // consumed anything yet, use its origin
        // and radius, otherwise add to our total sum
        // the sum of values it has consumed.
        if (sphere->mTotalConsumed == 0)
        {
            mSumOfPointValues += sphere->mOrigin;
            mSumOfRadiusValues += sphere->mRadius;
            mTotalConsumed++;
        }
        else
        {
            mSumOfPointValues += sphere->mSumOfPointValues;
            mSumOfRadiusValues += sphere->mSumOfRadiusValues;
            mTotalConsumed += sphere->mTotalConsumed;
        }
        mOrigin = mSumOfPointValues/(double)mTotalConsumed;
        mRadius = mSumOfRadiusValues/(double)mTotalConsumed;
        // Reset the time stamp value so we don't die.
        mTimeStamp = sphere->mTimeStamp;
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if a sphere intersects with a sphere.
///
///   \param[in] point Point to test
///   \param[in] radius Radius around point
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::CollidesWith(const Point3D& point, const double radius) const
{
    bool result = false;
    double dist = mOrigin.Distance(point);
    if(dist < (radius + mRadius))
    {
        result = true;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if the segment intersects with a sphere.
///
///   \param[in] segment Segment to test intersection.
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::CollidesWith(const Segment3D& segment) const
{
    bool result = false;
    if(Segment3D::GetDistanceToPoint(mOrigin, segment) <= mRadius)
    {
        result = true;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if the object intersects with a triangle
///          copied from RealTime Collision Detection p 141,142.
///
///   \param[in] a A point describing a triangle.
///   \param[in] b A point describing a triangle.
///   \param[in] c A point describing a triangle.
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::CollidesWith(const Point3D& a,
                          const Point3D& b,
                          const Point3D& c) const
{
    Point3D ab = b - a;
    Point3D ac = c - a;
    Point3D ap = mOrigin - a;

    double d1 = Point3D::Dot(ab,ap);
    double d2 = Point3D::Dot(ac,ap);
    if(d1 <= 0.0 && d2 <= 0.0)
    {
        if(mOrigin.Distance(a) < mRadius)
        {
            return true;
        }
        return false;
    }

    Point3D bp = mOrigin - b;
    double d3 = Point3D::Dot(ab,bp);
    double d4 = Point3D::Dot(ac,bp);
    if(d3 >= 0.0f && d4 <= d3)
    {
        if(mOrigin.Distance(b) < mRadius)
        {
            return true;
        }
        return false;
    }

    double vc = d1*d4 - d3*d2;
    if(vc <= 0.0 && d1 >= 0.0 && d3 <=0.0)
    {
        double v = d1/(d1-d3);
        if(mOrigin.Distance(a+(ab*v)) < mRadius)
        {
            return true;
        }
        return false;
    }

    Point3D cp = mOrigin - c;
    double d5 = Point3D::Dot(ab,cp);
    double d6 = Point3D::Dot(ac,cp);
    if(d6 >= 0.0 && d5 <= d6)
    {

        if(mOrigin.Distance(c) < mRadius)
        {
            return true;
        }
        return false;
    }

    double vb = d5*d2 - d1*d6;
    if(vb <= 0.0f && d2 >= 0.0f && d6 <= 0.0f)
    {
        double w = d2/(d2 - d6);
        if(mOrigin.Distance(a + (ac*w)) < mRadius)
        {
            return true;
        }
        return false;
    }

    double va = d3*d6 - d5*d4;
    if(va < 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) >= 0.0)
    {
        double w = (d4 - d3) /((d4-d3)+(d5-d6));
        if(mOrigin.Distance(b+((c-b)*w)) < mRadius)
        {
            return true;
        }
        return false;
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if the object intersects within a
///   specific radius of a point within the XY plane.
///
///   The zHigh and zLow values are used to eliminate obstacles above or
///   below a Z axis threshold of the XY planes.
///
///   \param[in] point The point to check for collision with.
///   \param[in] radius The radius around the point to check for collision with.
///   \param[in] zHigh The upper boundary in the zPlane for collision detection.
///   \param[in] zLow The lower boundary in the zPlane for collision detection.
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::IsCollisionInXY(const Point3D& point,
                             const double radius,
                             const double zHigh, const double zLow) const
{
    //return false;
    bool result = false;
    //Translate to the XY plane.
    Point3D pointXY, originXY;
    pointXY(point.mX, point.mY, 0);
    originXY(mOrigin.mX, mOrigin.mY, 0);
    // Filter out values outside of range, then flatten to XY plane
    if(mOrigin.mZ + mRadius >= zLow && mOrigin.mZ - mRadius <= zHigh)
    {
        // My z value is within the boundaries, so now check if we are
        // within the radius of the point.
        if(originXY.Distance(pointXY) <= radius + mRadius)
        {
            result = true;
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if the object intersects with a segment
///   in the XY plane.
///
///   The zHigh and zLow values are used to eliminate obstacles above or
///   below a Z axis threshold of the XY planes.
///
///   \param[in] segment The segment to check for collision with.
///   \param[in] zHigh The upper boundary in the zPlane for collision detection.
///   \param[in] zLow The lower boundary in the zPlane for collision detection.
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::IsCollisionInXY(const Segment3D& segment,
                             const double zHigh, const double zLow) const
{
    bool result = false;
    // Translate to the XY plane.
    Point3D originXY(mOrigin.mX, mOrigin.mY, 0);
    Segment3D segmentXY(Point3D(segment.mPoint1.mX, segment.mPoint1.mY, 0.0),
                        Point3D(segment.mPoint2.mX, segment.mPoint2.mY, 0.0),
                        segment.mWidth);
    // Filter out values outside of range, then flatten to XY plane
    if(mOrigin.mZ + mRadius >= zLow && mOrigin.mZ - mRadius <= zHigh)
    {
        // My z value is within the boundaries, so now check if we are
        // within the distance to the segment.
        if(Segment3D::GetDistanceToPoint(originXY, segmentXY) <= segment.mWidth/2.0 + mRadius)
        {
            result = true;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to check if the object intersects with a triangle
///   in the XY plane.
///
///   The zHigh and zLow values are used to eliminate obstacles above or
///   below a Z axis threshold of the XY planes.
///
///   \param[in] p1 A point describing a triangle.
///   \param[in] p2 A point describing a triangle.
///   \param[in] p3 A point describing a triangle.
///   \param[in] zHigh The upper boundary in the zPlane for collision detection.
///   \param[in] zLow The lower boundary in the zPlane for collision detection.
///
///   \return True if there is a collision, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Sphere::IsCollisionInXY(const Point3D& p1,
                             const Point3D& p2,
                             const Point3D& p3,
                             const double zHigh, const double zLow) const
{
    bool result = false;
    // Translate to the XY plane.
    Point3D point1XY(p1.mX, p1.mY, 0);
    Point3D point2XY(p2.mX, p2.mY, 0);
    Point3D point3XY(p3.mX, p3.mY, 0);
    Point3D originXY(mOrigin.mX, mOrigin.mY, 0);
    // Filter out values outside of range, then flatten to XY plane
    if(mOrigin.mZ + mRadius >= zLow && mOrigin.mZ - mRadius <= zHigh)
    {
        // My z value is within the boundaries, so now check if we are
        // within the triangle.
        if(originXY.IsInside(point1XY, point2XY, point3XY))
        {
            result = true;
        }
        else
        {
            // If the origin is not inside of the triangle, then
            // check for overlap with any of the triangle edges.
            Segment3D edge1(point1XY, point2XY);
            Segment3D edge2(point2XY, point3XY);
            Segment3D edge3(point3XY, point1XY);
            if(Segment3D::GetDistanceToPoint(originXY, edge1) <= mRadius)
            {
                result = true;
            }
            else if(Segment3D::GetDistanceToPoint(originXY, edge2) <= mRadius)
            {
                result = true;
            }
            else if(Segment3D::GetDistanceToPoint(originXY, edge3) <= mRadius)
            {
                result = true;
            }
        }
    }
    return result;
}

/** Get distance from this object to a point */
double Sphere::GetDistance(const Point3D& point) const
{
    double dist = point.Distance(this->mOrigin) - this->mRadius;
    if (dist < 0)
        return 0;
    else
        return dist;
}

//////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all object information.
///
//////////////////////////////////////////////////////////////////////////////////
void Sphere::Clear()
{
    Object::Clear();
    mOrigin(0, 0, 0);
    mRadius = 0.0;
    mSumOfPointValues(0, 0, 0);
    mSumOfRadiusValues = 0.0;
    mTotalConsumed = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the the object from coordinate frame local to a Pose to
///   map coordinate frame.
///
///   \param[in] pose Pose to use for coordinate frame conversion.
///
///   \return A copy of the object data converted to the new coordinate frame. You
///          must delete it.
///
////////////////////////////////////////////////////////////////////////////////////
Object* Sphere::ConvertFromLocalToMapCoordinates(const Pose& pose) const
{
    // Clone to keep support for derived classes.
    Sphere* copy = (Sphere *)this->Clone();
    copy->mOrigin = Pose::ConvertFromLocalToMapCoordinates(pose, mOrigin);
    copy->mSumOfPointValues(0, 0, 0);
    copy->mSumOfRadiusValues = 0;
    copy->mTotalConsumed = 0;
    return (Object *)copy;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the the object from map coordinate frame to a local
///   coordinate frame relative to a Pose.
///
///   \param[in] pose Pose to use for coordinate frame conversion.
///
///   \return A copy of the object data converted to the new coordinate frame. You
///          must delete it.
///
////////////////////////////////////////////////////////////////////////////////////
Object* Sphere::ConvertFromMapToLocalCoordinates(const Pose& pose) const
{
    // Clone to keep support for derived classes.
    Sphere* copy = (Sphere *)this->Clone();
    copy->mOrigin = Pose::ConvertFromMapToLocalCoordinates(pose, mOrigin);
    copy->mSumOfPointValues(0, 0, 0);
    copy->mSumOfRadiusValues = 0;
    copy->mTotalConsumed = 0;
    return (Object *)copy;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the origin of the sphere.
///
///   \param[in] origin The origin of the sphere in 3D space.
///
////////////////////////////////////////////////////////////////////////////////////
void Sphere::SetOrigin(const Point3D& origin)
{
    mOrigin = origin;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the radius of the sphere.
///
///   \param[in] radius The radius of the sphere in meters.
///
////////////////////////////////////////////////////////////////////////////////////
void Sphere::SetRadius(const double radius)
{
    if (radius >= 0)
    {
        mRadius = radius;
    }
    else
    {
        mRadius = fabs(radius);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Sphere& Sphere::operator=(const Sphere& sphere)
{
    CopyObjectData(&sphere);
    mOrigin = sphere.mOrigin;
    mRadius = sphere.mRadius;
    mSumOfPointValues = sphere.mSumOfPointValues;
    mSumOfRadiusValues = sphere.mSumOfRadiusValues;
    mTotalConsumed = sphere.mTotalConsumed;
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
bool Sphere::Draw(IplImage* image, const double pixelsPerMeter, const Point3D& mapBottomCorner, CvScalar color) const
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;
    CvPoint center;
    center = ConvertToImagePoint(image->height, pixelsPerMeter, mOrigin, mapBottomCorner);
    cvCircle(image,center,(int)(pixelsPerMeter * mRadius),color,-1, CV_AA);
    return true;

}


#endif

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a string containing information about the object.
///
///   \return String of information about the object.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Sphere::ToString() const
{
    std::string result;
    char buf[512];
    result.append(Object::ToString());
    sprintf(buf,"\nX: %lf Y: %lf Z: %lf Radius: %lf",mOrigin.mX,mOrigin.mY, mOrigin.mZ,mRadius);
    //printf("%d\n",mSourceID);
    //std::cout << mObjectType << "\t" << mSourceID << "\t" << buf << std::endl;
    result.append(buf);
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Writes information about the object to a packet.
///
///   Overload this method to write your own objects data (see Sphere for example).
///
///   \param[out] packet Packet to serialize data to.
///
///   \return Number of bytes written.
///
////////////////////////////////////////////////////////////////////////////////////
int Sphere::WriteToPacket(Packet& packet) const
{
    Object::WriteToPacket(packet);

    packet.Write(mOrigin.mX);
    packet.Write(mOrigin.mY);
    packet.Write(mOrigin.mZ);
    packet.Write(mRadius);

    return (int)packet.Length();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Reads information about the object from a packet.
///
///   Overload this method to read your own objects data (see Sphere for example).
///
///   \param[out] packet Packet to serialize data from.
///
///   \return Number of bytes read.
///
////////////////////////////////////////////////////////////////////////////////////
int Sphere::ReadFromPacket(const Packet& packet)
{
    int bytesRead = 0;

    bytesRead = Object::ReadFromPacket(packet);
    bytesRead += packet.Read(mOrigin.mX);
    bytesRead += packet.Read(mOrigin.mY);
    bytesRead += packet.Read(mOrigin.mZ);
    bytesRead += packet.Read(mRadius);

    return bytesRead;
}


/*  End of File */
