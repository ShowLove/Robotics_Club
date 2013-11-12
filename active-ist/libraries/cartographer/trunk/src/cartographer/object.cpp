////////////////////////////////////////////////////////////////////////////////////
///
///  \file object.cpp
///  \brief Base class for defining any object stored in a map used within this
///         library.  All objects must be derived from this super class.
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
#include "cartographer/object.h"
#include <cxutils/math/cxmath.h>
#include <math.h>
#include <iostream>
#include <cstdio>
#include <sstream>

#include "cartographer/objects/sphere.h"
#include "cartographer/objects/line.h"

#ifdef USE_OPENCV
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a circle to the image using global coordinates.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] point location of the sphere.
///   \param[in] radius of the sphere.
///   \param[in] color to set background to.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
                        const double pixelsPerMeter,
                        const Cartographer::Point3D& mapBottomCorner,
                        const Cartographer::Point3D& point,
                        const double radius,
                        const CvScalar color)
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;
    CvPoint center = ConvertToImagePoint(image->height, pixelsPerMeter, point, mapBottomCorner);
    double useradius = radius;
    if(useradius <= 0) useradius = .01;
    cvCircle(image, center, (int)(pixelsPerMeter * useradius), color, -1, CV_AA);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a circle to the image referenced to global pose and
///          using local coordinates from that pose.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] pose Global pose the points are locally referenced to.
///   \param[in] point location of the sphere.
///   \param[in] radius of the sphere.
///   \param[in] color to set background to.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
                        const double pixelsPerMeter,
                        const Point3D& mapBottomCorner,
                        const Pose& pose,
                        const Point3D& point, const double radius,
                        const CvScalar color)
{
    return Draw(image,
                pixelsPerMeter,
                mapBottomCorner,
                Pose::ConvertFromLocalToMapCoordinates(pose, point),
                radius,
                color);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a segment to the image using global coordinates.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] segment to draw to the image
///   \param[in] color so set the segment to.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
                        const double pixelsPerMeter,
                        const Cartographer::Point3D& mapBottomCorner,
                        const Cartographer::Segment3D& segment,
                        const CvScalar color)
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;

    int width = (int)(segment.mWidth*pixelsPerMeter);
    width = width > 0 ? width : 1;
    CvPoint pt1, pt2;
    pt1 = ConvertToImagePoint(image->height, pixelsPerMeter, segment.GetProjectionXY().mPoint1, mapBottomCorner);
    pt2 = ConvertToImagePoint(image->height, pixelsPerMeter, segment.GetProjectionXY().mPoint2, mapBottomCorner);

    cvLine(image, pt1, pt2, color, width, CV_AA);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a segment to the image referenced to global pose and
///          using local coordinates from that pose.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] pose Global pose the points are locally referenced to.
///   \param[in] segment to draw to the image
///   \param[in] color so set the segment to.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
                        const double pixelsPerMeter,
                        const Point3D& mapBottomCorner,
                        const Pose& pose,
                        const Segment3D& segment,
                        const CvScalar color)
{
    return Draw(image,
                pixelsPerMeter,
                mapBottomCorner,
                Pose::ConvertFromLocalToMapCoordinates(pose, segment),
                color);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a triangle to the image using global coordinates.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] p1 first point in the triangle
///   \param[in] p2 second point in the triangle
///   \param[in] p3 third point in the triangle
///   \param[in] color Color to set.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
                        const double pixelsPerMeter,
                        const Cartographer::Point3D& mapBottomCorner,
                        const Cartographer::Point3D& p1,
                        const Cartographer::Point3D& p2,
                        const Cartographer::Point3D& p3,
                        const CvScalar color)
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;

    CvPoint pts[3];
    pts[0] = ConvertToImagePoint(image->height, pixelsPerMeter, p1, mapBottomCorner);
    pts[1] = ConvertToImagePoint(image->height, pixelsPerMeter, p2, mapBottomCorner);
    pts[2] = ConvertToImagePoint(image->height, pixelsPerMeter, p3, mapBottomCorner);

    cvFillConvexPoly(image, pts, 3, color, CV_AA);
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a triangle to the image referenced to global pose and
///          using local coordinates from that pose.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] pose Global pose the points are locally referenced to.
///   \param[in] p1 first point in the triangle
///   \param[in] p2 second point in the triangle
///   \param[in] p3 third point in the triangle
///   \param[in] color Color to set.
///
///   \return If the object was able to be added or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
          const double pixelsPerMeter,
          const Point3D& mapBottomCorner,
          const Pose& pose,
          const Point3D& p1,
          const Point3D& p2,
          const Point3D& p3,
          const CvScalar color)
{
    return Draw(image, 
                pixelsPerMeter, 
                mapBottomCorner, 
                Pose::ConvertFromLocalToMapCoordinates(pose, p1),
                Pose::ConvertFromLocalToMapCoordinates(pose, p2),
                Pose::ConvertFromLocalToMapCoordinates(pose, p3)); 
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a BoundingBox to the image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] box Bounding box to draw.
///   \param[in] color to set.
///
///   \return If the object was able to be drawn or not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Cartographer::Draw(IplImage* image,
          const double pixelsPerMeter,
          const Cartographer::Point3D& mapBottomCorner,
          const Cartographer::BoundingBox& box,
          const CvScalar color)
{
    if(image == NULL) return false;
    if((box.mDistanceToFront && box.mDistanceToBack &&
        box.mDistanceToLeft && box.mDistanceToRight == 0)) return false;

    CvPoint cvTopLeft;
    CvPoint cvBottomRight;

    Point3D topLeft = box.GetPosition(BoundingBox::BackLeftBottom);
    Point3D bottomRight = box.GetPosition(BoundingBox::FrontRightBottom);

    cvTopLeft = ConvertToImagePoint(image->height, pixelsPerMeter, topLeft, mapBottomCorner);
    cvBottomRight = ConvertToImagePoint(image->height, pixelsPerMeter, bottomRight, mapBottomCorner);

    cvRectangle(image,cvTopLeft,cvBottomRight,color,3);
    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a map coordinate into image coordinates.
///
///   \param[in] imageHeight Height of image in pixels.
///   \param[in] pixelsPerLength Pixels per meter conversion value.
///   \param[in] point Point in map.
///   \param[in] offset Point in map we are treating as origin.
///   \param[in] flipYAxis Flip the Y axis, since opencv starts origin at top left
///              corner.
///
///   \return Pixel location map coordinate would map to.
///
////////////////////////////////////////////////////////////////////////////////////
CvPoint Cartographer::ConvertToImagePoint(const int imageHeight,
                                          const double pixelsPerLength,
                                          const Cartographer::Point3D& point,
                                          const Cartographer::Point3D& offset,
                                          const bool flipYAxis)
{
    CvPoint pt;

    pt.x = (int)(pixelsPerLength * (point.mX - offset.mX));
    if(flipYAxis)
    {
        pt.y = (imageHeight - (int)(pixelsPerLength * (point.mY - offset.mY)));
    }
    else
    {
        pt.y = (int)(pixelsPerLength * (point.mY - offset.mY));
    }

    return pt;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the color of the object as it fades with time.
///
///   \return Color faded as object gets closer to expiring
///
////////////////////////////////////////////////////////////////////////////////////
CvScalar Cartographer::Object::CalculateDrawColor(CvScalar color) const
{
    if(mTimeToLiveMs == Infinite)
    {
        return color;
    }
    else
    {
        double timePassed = (double)(CxUtils::GetTimeMs() - mTimeStamp.ToMs());
        if(timePassed >= mTimeToLiveMs) return CV_RGB(0,0,0);
        double percent = (mTimeToLiveMs - timePassed)/mTimeToLiveMs;
        color.val[0] = color.val[0] * percent;
        color.val[1] = color.val[1] * percent;
        color.val[2] = color.val[2] * percent;
        return color;
    }
}

#endif

using namespace Cartographer;

Object::Factory::Factory()
{
    mObjects[Objects::Sphere::Name] = new Objects::Sphere();
    mObjects[Objects::Line::Name] = new Objects::Line();
}

Object::Factory::~Factory()
{
    std::map<std::string, Object*>::iterator obj;
    for(obj = mObjects.begin(); obj != mObjects.end(); obj++)
    {
        delete obj->second;
    }
    mObjects.clear();
}


bool Object::Factory::AddObject(const std::string& name, Object* object)
{
    Mutex::ScopedLock lock(&mFactoryMutex);
    if(mObjects.find(name) != mObjects.end())
    {
        return false;
    }
    mObjects[name] = object;
    return true;
}


Object* Object::Factory::CreateObject(const std::string& name) const
{
    Mutex::ScopedLock lock(&mFactoryMutex);
    std::map<std::string, Object*>::const_iterator obj;
    obj = mObjects.find(name);
    if(obj != mObjects.end())
    {
        return obj->second->Clone();
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor for Objects.
///
///   \param[in] type The type of object (point, line, polygon).
///   \param[in] identification Object identification string data.
///   \param[in] sourceID The sensor or creator of the object being placed in
///                       a map.
///
////////////////////////////////////////////////////////////////////////////////////
Object::Object(const Object::Type type,
               const std::string& identification,
               const unsigned int sourceID) : mObjectType(type),
                                              mSourceID(sourceID),
                                              mTimeStamp(true),
                                              mTimeToLiveMs(Object::Infinite),
                                              mObjectIdentification(identification),
                                              mUniqueID(0),
                                              mpMapExtras(NULL)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Object::~Object()
{
    if(mpMapExtras)
    {
        delete mpMapExtras;
        mpMapExtras = NULL;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Type provides a high level type associated with an object, the
///          Identification gives a more detailed description.  Unique ID is used
///          to identify to different types/identifications of an object.
///
///   \param[in] uniqueID Unique identifier to assign to object.  This value can
///                       be used to distinguish between objects of the same
///                       type and identification.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::SetUniqueID(const unsigned int uniqueID)
{
    mUniqueID = uniqueID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the source ID of the object.  This defaults to 0, but can
///          be useful for tracking what sensor detected the item in case
///          you want to ignore the data some circumstances.
///
///   \param[in] sourceID The sensor or creator of the object being placed in
///                       a map.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::SetSourceID(const unsigned int sourceID)
{
    mSourceID = sourceID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the timestamp for an object.
///
///   \param[in] timeStamp Time the object was created or detected.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::SetTimeStamp(const Time& timeStamp)
{
    mTimeStamp = timeStamp;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets time time to live.
///
///   \param[in] timetolivems for object to live in map.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::SetTimeToLive(const unsigned int timetolivems)
{
    mTimeToLiveMs = timetolivems;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \return Object base type.
///
////////////////////////////////////////////////////////////////////////////////////
Object::Type Object::GetType() const
{
    return mObjectType;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Object source or creator ID.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Object::GetSourceID() const
{
    return mSourceID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Time of object creation or detection.
///
////////////////////////////////////////////////////////////////////////////////////
Time Object::GetTimeStamp() const
{
    return mTimeStamp;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Number of milliseconds and object is allowed to exist in maps.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Object::GetTimeToLive() const
{
    return mTimeToLiveMs;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the time to live has expired and the object should be
///           removed from any maps, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Object::IsDead() const
{
    if(mTimeToLiveMs == Infinite)
    {
        return false;
    }
    else if(CxUtils::GetTimeMs() - mTimeStamp.ToMs() >= mTimeToLiveMs)
    {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return Optional object identification string data.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Object::GetIdentification() const
{
    return mObjectIdentification;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the unique identification number assigned to this type of
///          or instance of an object.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Object::GetUniqueID() const
{
    return mUniqueID;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all object information.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::Clear()
{
    mSourceID = 0;
    mTimeStamp = Time();
    mObjectIdentification.clear();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copies object data.
///
////////////////////////////////////////////////////////////////////////////////////
void Object::CopyObjectData(const Object* object)
{
    mSourceID = object->mSourceID;
    mObjectIdentification = object->mObjectIdentification;
    mTimeStamp = object->mTimeStamp;
    mUniqueID = object->mUniqueID;
    mTimeToLiveMs = object->mTimeToLiveMs;
    if(mpMapExtras)
    {
        delete mpMapExtras;
        mpMapExtras = NULL;
    }
    if(object->mpMapExtras)
    {
        mpMapExtras = object->mpMapExtras->Clone();
    }
    //mpMapExtras = object->mpMapExtras;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to predict where an object will be and possibly
///          what the state is after a specified amount of time has elapsed.
///
///   This method is used when trying to predict where moving objects are going
///   to be at a future point in time.  By default, this method treats objects
///   as static and returns a copy.  Overload this method to add behaviors to
///   detected dynamic objects.
///
///   \param[in] elapsedTimeSeconds The amount of time in the future to predict
///                                 future state for.
///
///   \return A copy of the object with its state being elapsedTimeSeconds in the
///           future.
///
////////////////////////////////////////////////////////////////////////////////////
Object* Object::GetPredictedFutureState(const double elapsedTimeSeconds) const
{
    return Clone();
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a string containing information about the object.
///
///   \return String of information about the object.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Object::ToString() const
{
    std::stringstream str;

    str << "ID: " << mUniqueID << " Type: " << (int)mObjectType << " Source: " << mSourceID;
    str << " Name: " << mObjectIdentification;
    return str.str();
    //std::string result;
    //char buf[512];
    //sprintf(buf,"ID: %u Type: %d Source: %d ",mUniqueID,mObjectType,mSourceID);
    ////printf("%d\n",mSourceID);
    ////std::cout << mObjectType << "\t" << mSourceID << "\t" << buf << std::endl;
    //result.append(buf);
    //result.append("Name: ");
    //result.append(mObjectIdentification);
    //return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the smallest possible distance from point to line segment
///
///   \param[in] point to check
///   \param[in] segment to check
///
///   \return the smallest possible distance from point to line segment.
///
////////////////////////////////////////////////////////////////////////////////////
double Object::GetDistanceToPoint(const Point3D& point,
                                  const Segment3D& segment)
{
    Point3D ab = segment.mPoint2 - segment.mPoint1;
    Point3D ac = point - segment.mPoint1;
    Point3D bc = point - segment.mPoint2;
    double e = ac.Dot(ab);
    if(e <= 0.0f) return ac.Dot(ac);
    double f = ab.Dot(ab);
    if(e >= f) return bc.Dot(bc);
    return ac.Dot(ac)- e * e / f;
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
int Object::WriteToPacket(Packet& packet) const
{
    packet.Clear();
    
    packet.Write((unsigned int)mObjectIdentification.size());
    packet.Write(mObjectIdentification);
    packet.Write(mSourceID);
    packet.Write(mUniqueID);
    unsigned int tstamp = 0;
    tstamp |= (unsigned int)(mTimeStamp.mDay)    << 27;
    tstamp |= (unsigned int)(mTimeStamp.mHour)   << 22;
    tstamp |= (unsigned int)(mTimeStamp.mMinute) << 16;
    tstamp |= (unsigned int)(mTimeStamp.mSecond) << 10;
    tstamp |= mTimeStamp.mMilliseconds;
    packet.Write(tstamp);
    packet.Write(mTimeToLiveMs);
    
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
int Object::ReadFromPacket(const Packet& packet)
{
    int bytesRead = 0;
    unsigned int identLen = 0, tstamp = 0;
    bytesRead += packet.Read(identLen);
    bytesRead += packet.Read(mObjectIdentification, identLen);
    bytesRead += packet.Read(mSourceID);
    bytesRead += packet.Read(mUniqueID);
    bytesRead += packet.Read(tstamp);
    mTimeStamp.mMilliseconds = (unsigned int)(tstamp & 0x3FF);
    mTimeStamp.mSecond = (unsigned int)((tstamp >> 10) & 0x3F);
    mTimeStamp.mMinute = (unsigned int)((tstamp >> 16) & 0x3F);
    mTimeStamp.mHour = (unsigned int)((tstamp >> 22) & 0x1F);
    mTimeStamp.mDay = (unsigned int)((tstamp >> 27) & 0x1F);
    bytesRead += packet.Read(mTimeToLiveMs);
    
    return bytesRead;
}


/*  End of File */
