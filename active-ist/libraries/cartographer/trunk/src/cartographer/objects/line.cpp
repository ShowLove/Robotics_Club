////////////////////////////////////////////////////////////////////////////////////
///
///  \file line.cpp
///  \brief Object class for describing a 3D segment object.  This object
///  type is useful for descrbing pipes, or cylinders, etc.
///
///  <br>Author(s): Daniel Barber, Michael Scherer
///  <br>Created: 24 September 2009
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
#include "cartographer/objects/line.h"
#include <iostream>
#include <math.h>
#include <cstdio>
#include <sstream>
#include <iomanip>

using namespace Cartographer;
using namespace Objects;

const std::string Line::Name = "Line";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] sourceID The object source or creator ID.
///
////////////////////////////////////////////////////////////////////////////////////
Line::Line(const unsigned int sourceID) : Object(Object::Line, "Line", sourceID)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] line Segment data describing the line.
///   \param[in] sourceID The object source or creator ID.
///
////////////////////////////////////////////////////////////////////////////////////
Line::Line(const Segment3D& line, const unsigned int sourceID) : Object(Object::Line, "Line", sourceID),
                                                                 mLine(line)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
///   \param[in] line Segment data describing the line.
///
////////////////////////////////////////////////////////////////////////////////////
Line::Line(const Line& line) : Object(Object::Line, "Line", line.mSourceID)
{
    *this = line;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Line::~Line()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A bounding sphere which encapsulates the object.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingSphere Line::GetBoundingSphere() const
{
    return BoundingSphere::CreateBoundingSphere(mLine);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return A Bounding Box which encapsulates the object in 3D space.
///
////////////////////////////////////////////////////////////////////////////////////
BoundingBox Line::GetBoundingBox() const
{
    return BoundingBox::CreateBoundingBox(mLine);
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
bool Line::IsSameAs(const Object* object,
                    const double allowedError) const
{
    const Line* line = dynamic_cast<const Line*>(object);
    if (line && line->mSourceID == mSourceID)
    {
        if (fabs(line->mLine.mWidth - mLine.mWidth) < allowedError &&
                mLine.mPoint1.Distance(line->mLine.mPoint1) < allowedError &&
                mLine.mPoint2.Distance(line->mLine.mPoint2) < allowedError &&
                mObjectIdentification == line->mObjectIdentification)
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
bool Line::ConsumeMatch(const Object* object)
{
    const Line* line = dynamic_cast<const Line*>(object);
    if (line)
    {
        // Copy the data.
        *this = *line;
        // Reset the time stamp value so we don't die.
        mTimeStamp = line->mTimeStamp;
        return true;
    }
    return false;
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
bool Line::IsCollisionInXY(const Point3D& point,
                           const double radius,
                           const double zHigh, const double zLow) const
{
    bool result = false;
    // Translate to the XY plane.
    Point3D originXY(point.mX, point.mY, 0);
    Segment3D segmentXY(Point3D(mLine.mPoint1.mX, mLine.mPoint1.mY, 0.0),
                        Point3D(mLine.mPoint2.mX, mLine.mPoint2.mY, 0.0),
                        mLine.mWidth);

    BoundingBox bbox = BoundingBox::CreateBoundingBox(mLine);
    // Check that our object is within the z boundaries.
    if(bbox.GetTop() >= zLow && bbox.GetBottom() <= zHigh)
    {
        // My z value is within the boundaries, so now check if we are
        // within the distance to the segment.
        if(Segment3D::GetDistanceToPoint(originXY, segmentXY) <= radius)
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
bool Line::IsCollisionInXY(const Segment3D& segment,
                           const double zHigh, const double zLow) const
{
    bool result = false;
    // Translate to XY plane.
    Segment3D segmentXY1(Point3D(mLine.mPoint1.mX, mLine.mPoint1.mY, 0.0),
                         Point3D(mLine.mPoint2.mX, mLine.mPoint2.mY, 0.0),
                         mLine.mWidth);
    Segment3D segmentXY2(Point3D(segment.mPoint1.mX, segment.mPoint1.mY, 0.0),
                         Point3D(segment.mPoint2.mX, segment.mPoint2.mY, 0.0),
                         segment.mWidth);

    BoundingBox bbox = BoundingBox::CreateBoundingBox(mLine);
    // Check that our object is within the z boundaries.
    if(bbox.GetTop() >= zLow && bbox.GetBottom() <= zHigh)
    {
        // My z value is within the boundaries, so now check if we are
        // within the distance to the segment.
        if(Segment3D::GetDistanceToSegment(segmentXY1, segmentXY2) <= 0.0)
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
bool Line::IsCollisionInXY(const Point3D& p1,
                           const Point3D& p2,
                           const Point3D& p3,
                           const double zHigh, const double zLow) const
{
    bool result = false;
    // Translate to the XY plane.
    Point3D point1XY(p1.mX, p1.mY, 0);
    Point3D point2XY(p2.mX, p2.mY, 0);
    Point3D point3XY(p3.mX, p3.mY, 0);
    Segment3D segmentXY(Point3D(mLine.mPoint1.mX, mLine.mPoint1.mY, 0.0),
                        Point3D(mLine.mPoint2.mX, mLine.mPoint2.mY, 0.0),
                        mLine.mWidth);
    BoundingBox bbox = BoundingBox::CreateBoundingBox(mLine);
    // Check that our object is within the z boundaries.
    if(bbox.GetTop() >= zLow && bbox.GetBottom() <= zHigh)
    {
        if(segmentXY.mPoint1.IsInside(point1XY, point2XY, point3XY) ||
           segmentXY.mPoint2.IsInside(point1XY, point2XY, point3XY) )
        {
            result = true;
        }
        else
        {
            Segment3D edge1(point1XY, point2XY);
            Segment3D edge2(point2XY, point3XY);
            Segment3D edge3(point3XY, point1XY);

            // Check for intersections with edges of the triangle.
            if(Segment3D::GetDistanceToSegment(segmentXY, edge1) <= 0.0)
            {
                result = true;
            }
            else if(Segment3D::GetDistanceToSegment(segmentXY, edge2) <= 0.0)
            {
                result = true;
            }
            else if(Segment3D::GetDistanceToSegment(segmentXY, edge3) <= 0.0)
            {
                result = true;
            }
        }
    }

    return result;
}

/** Get distance from this object to a point */
double Line::GetDistance(const Point3D& point) const
{
    return Object::GetDistanceToPoint(point, this->mLine);
}

//////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all object information.
///
//////////////////////////////////////////////////////////////////////////////////
void Line::Clear()
{
    Object::Clear();
    mLine.Clear();
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
Object* Line::ConvertFromLocalToMapCoordinates(const Pose& pose) const
{
    // Clone to keep support for derived classes.
    Line* copy = (Line *)this->Clone();
    copy->mLine = Pose::ConvertFromLocalToMapCoordinates(pose, mLine);
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
Object* Line::ConvertFromMapToLocalCoordinates(const Pose& pose) const
{
    // Clone to keep support for derived classes.
    Line* copy = (Line *)this->Clone();
    copy->mLine = Pose::ConvertFromMapToLocalCoordinates(pose, mLine);
    return (Object *)copy;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Line& Line::operator =(const Line& line)
{
    if(this != &line)
    {
        CopyObjectData(&line);
        mLine = line.mLine;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a string containing information about the object.
///
///   \return String of information about the object.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Line::ToString() const
{
    std::stringstream str;

    str << Object::ToString() << std::endl;
    str << "P1[" << std::setprecision(2) << mLine.mPoint1.mX << "," << mLine.mPoint1.mY << "," << mLine.mPoint1.mZ << "] ";
    str << "P2[" << std::setprecision(2) << mLine.mPoint2.mX << "," << mLine.mPoint2.mY << "," << mLine.mPoint2.mZ << "] ";
    str << "Width : " << std::setprecision(2) << mLine.mWidth;
    return str.str();

    //std::string result;
    //char buffer[512];
    //result.append(Object::ToString());
    //sprintf(buffer,
    //        "\nP1[%.2lf,%.2lf,%.2lf] P2[%.2lf,%.2lf,%.2lf] Width : %.2lf",
    //        mLine.mPoint1.mX,
    //        mLine.mPoint1.mY,
    //        mLine.mPoint1.mZ,
    //        mLine.mPoint2.mX,
    //        mLine.mPoint2.mY,
    //        mLine.mPoint2.mZ,
    //        mLine.mWidth);
    //result.append(buffer);
    //return result;
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
int Line::WriteToPacket(Packet& packet) const
{
    Object::WriteToPacket(packet);
    
    packet.Write(mLine.mPoint1.mX);
    packet.Write(mLine.mPoint1.mY);
    packet.Write(mLine.mPoint1.mZ);
    
    packet.Write(mLine.mPoint2.mX);
    packet.Write(mLine.mPoint2.mY);
    packet.Write(mLine.mPoint2.mZ);
    
    packet.Write(mLine.mWidth);
    
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
int Line::ReadFromPacket(const Packet& packet)
{
    int bytesRead = Object::ReadFromPacket(packet);
    
    bytesRead += packet.Read(mLine.mPoint1.mX);
    bytesRead += packet.Read(mLine.mPoint1.mY);
    bytesRead += packet.Read(mLine.mPoint1.mZ);
    
    bytesRead += packet.Read(mLine.mPoint2.mX);
    bytesRead += packet.Read(mLine.mPoint2.mY);
    bytesRead += packet.Read(mLine.mPoint2.mZ);
    
    bytesRead += packet.Read(mLine.mWidth);
    
    return bytesRead;
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
bool Line::Draw(IplImage* image, const double pixelsPerMeter, const Point3D& mapBottomCorner, CvScalar color) const
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;
    CvPoint p1, p2;
    // Draw the endpoints.
    p1 = ConvertToImagePoint(image->height, pixelsPerMeter, mLine.mPoint1, mapBottomCorner);
    //cvCircle(image, p1,(int)(pixelsPerMeter * mLine.mWidth/2.0),color,-1, CV_AA);
    p2 = ConvertToImagePoint(image->height, pixelsPerMeter, mLine.mPoint2, mapBottomCorner);
    //cvCircle(image, p2,(int)(pixelsPerMeter * mLine.mWidth/2.0),color,-1, CV_AA);
    // Now draw the line.
    // If the end points are on top of each other, just draw the one point.
    if(p1.x == p2.x && p1.y == p2.y)
    {
        cvCircle(image, p1,(int)(pixelsPerMeter * mLine.mWidth/2.0),color,-1, CV_AA);
    }
    else
    {
        cvLine(image, p1, p2, color, (int)(pixelsPerMeter*mLine.mWidth));
    }
    return true;

}

#endif

/*  End of File */
