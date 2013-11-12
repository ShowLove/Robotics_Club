////////////////////////////////////////////////////////////////////////////////////
///
///  \file pose.cpp
///  \brief Pose is a structure for storing the current state of an object in
///  3D space.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 December 2008
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
#include "cartographer/pose.h"
#include "cartographer/object.h"
#include <iostream>

using namespace std;
using namespace Cartographer;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initilizes the size of the history array based on parameters. Will 
///          never intialize history to less than 1 element.
///
///   \param[in] ms Desired amount of time to keep history, in milliseconds.
///   \param[in] hz Amount of pose updates expected per second.
///
////////////////////////////////////////////////////////////////////////////////////
void Pose::History::Initialize(const double ms, const double hz)
{
    CxUtils::Mutex::ScopedLock lock(&mMutex); 
    unsigned int num = (unsigned int)((ms/1000.) * hz);
    if(num < 1) num = 1;
    mHistory.reserve(num);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a historical Pose that correlates most closely with a specific
///          Time. Function makes the assumption that Poses are pushed in order 
///          of increasing time. It searches from the start of history(oldest poses)
///          and will return the pose closest to the time parameter which is also the 
///          oldest possible.
///
///    \param[in] time The time that you wish to find a related pose.
///
///    \return Pose that minimizes time differences between its timestamp and 'time'.
///            If multiple poses satisfy this criteria, returns the pose that was 
///            first added.
///
////////////////////////////////////////////////////////////////////////////////////
bool Pose::History::GetPose(const CxUtils::Time& time, Pose& pose) const
{
    CxUtils::Mutex::ScopedLock lock(&mMutex); 

    if(mHistory.size() < 1) 
    {
        return false;
    }

    bool result = false;
    unsigned int prevDiff = -1;
    unsigned int nextDiff = -1;
    CircularArray::const_iterator it;

    it = mHistory.begin();
    Pose closestPose = *it;
    bool foundBetter = false;
    while(it != mHistory.end())
    {
        if(prevDiff == -1)
        {
            closestPose = *it;
            prevDiff = abs((int)(time.ToMs() - closestPose.mTimeStamp.ToMs()));
            it++;
            result = true;
            continue;
        }
        nextDiff = abs((int)(time.ToMs() - (*it).mTimeStamp.ToMs()));
        if(nextDiff < prevDiff)
        {
            foundBetter = true;
            closestPose = *it;
            prevDiff = nextDiff;
            it++;
            result = true;
            continue;
        }
        it++;
    }

    pose = closestPose;
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Checks to see if there is any Pose in history which is near pose by
///          a factor of radiusM.
///
///    \param[in] pose The pose we want to know if we have historically 
///               visited.
///    \param[in] radiusM Radius around pose in meters in which we would consider
///               a historical Pose to be visited.
///
///    \return True if we have visited pose according to history, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool Pose::History::Visited(const Pose& pose, double radiusM) const
{
    CircularArray::const_iterator it;
    bool result = false;
    CxUtils::Mutex::ScopedLock lock(&mMutex); 
    if(mHistory.size() > 0)
    {
        it = mHistory.begin();
        Pose prev = *it;
        while(it != mHistory.end())
        {
            Segment3D segment(prev.mPosition, (*it).mPosition);
            if(Segment3D::GetDistanceToPoint(pose.mPosition, segment) <= radiusM)
            {
                result = true;
                break;
            }
            /*
            if(pose.mPosition.Distance((*it).mPosition) <= radiusM)
            {
                result = true;
                break;
            }
            */
            it++;
        }
    }
    return result;

}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Pose::Pose() : mID(0),
               mpPoseRotation(NULL)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param[in] position Position in 3D space relative to arbitrary origin [0,0,0].
///   \param[in] orientation Roll (X), Pitch (Y), Yaw (Z) of object.
///   \param[in] velocity Velocity of object along local axes [m/s].
///   \param[in] rotation Rotation rates of object along local axes [rad/s].
///   \param[in] timeStamp Time (UTC) when values were captured.
///   \param[in] id Unique Identifier to associate with Pose data.
///
////////////////////////////////////////////////////////////////////////////////////
Pose::Pose(const Point3D& position,
           const Orientation& orientation,
           const Point3D& velocity,
           const Point3D& rotation,
           const Time& timeStamp,
           const unsigned int id) : mPosition(position),
                                    mRotation(orientation),
                                    mLinearVelocity(velocity),
                                    mRotationalVelocity(rotation),
                                    mTimeStamp(timeStamp),
                                    mID(id),
                                    mpPoseRotation(NULL)
{   
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Pose::Pose(const Pose& pose) : mID(0),
                               mpPoseRotation(NULL)
{
    *this = pose;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Pose::~Pose()
{
    ClearPoseRotation();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method used to create/update the rotation quatnernion based off of
///          Pose orientation values in mOrientation.
///
///   You should use the UpdatePoseRotation value to pre-calculate a rotation
///   quanternion that can be re-used multiple times.  For example, if you need
///   to rotate multiple objects or data points relative to a Pose, then this
///   method can be used to prevent the re-calculation of a rotation quatnernion.
///
///   <b>All of the coordinate conversion methods of this class will use the
///   rotation quatnernion (mpPoseRotation) by default if it exists.  Therfore
///   you must update it or clear it or you will be using old rotation values!</b>
///
////////////////////////////////////////////////////////////////////////////////////
void Pose::UpdatePoseRotation()
{
    if(mpPoseRotation == NULL)
    {
        mpPoseRotation = new CxUtils::Quaternion();
    }
    mpPoseRotation->CreateFromEuler(mRotation.mX, 
                                    mRotation.mY,
                                    mRotation.mZ,
                                    false);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes the current rotation quatnerion associated with the pose.
///
////////////////////////////////////////////////////////////////////////////////////
void Pose::ClearPoseRotation()
{
    if(mpPoseRotation)
    {
        delete mpPoseRotation;
    }
    mpPoseRotation = NULL;  
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if within distance to position.
///
////////////////////////////////////////////////////////////////////////////////////
bool Pose::Intersects(const Pose &endPose, const double distance)
{
    if(mPosition.Distance(endPose.mPosition) <= distance)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if within distance to position.
///
////////////////////////////////////////////////////////////////////////////////////
bool Pose::Intersects(const Point3D &endPoint, const double distance)
{
    if(mPosition.Distance(endPoint) <= distance)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Pose& Pose::operator =(const Pose& pose)
{
    mPosition = pose.mPosition;
    mRotation = pose.mRotation;
    mLinearVelocity = pose.mLinearVelocity;
    mRotationalVelocity = pose.mRotationalVelocity;
    mTimeStamp = pose.mTimeStamp;
    mID = pose.mID;
    if(mpPoseRotation)
    {
        delete mpPoseRotation;
    }
    mpPoseRotation = NULL;
    if(pose.mpPoseRotation)
    {
        mpPoseRotation = new CxUtils::Quaternion(*pose.mpPoseRotation);
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method converts a position in a Global Coordinate system (WGS,
///   UTM, etc.) to coordinates within a Cartographer Map using the origin
///   assigned to the maps origin [0,0,0].
///
///   \param[in] position Position data in a global coordinate system to
///             convert to map coordinates.
///   \param[in] origin An origin assigned to the map or terrain you are using
///              so that position can be converted to a Point3D value
///              which stores position in meters on a flat surface.
///
///   \return A 3D point within the coordinate frame of a map with the
///           mapped origin value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Pose::ConvertFromGlobalToMapCoordinates(const Wgs& position, const Wgs& origin)
{
    Utm utmPosition;
    Utm utmOrigin;
    utmPosition << position;
    utmOrigin << origin;
    return ConvertFromGlobalToMapCoordinates(utmPosition, utmOrigin);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method converts a position in a Global Coordinate system (WGS,
///   UTM, etc.) to coordinates within a Cartographer Map using the origin
///   assigned to the maps origin [0,0,0].
///
///   \param[in] position Position data in a global coordinate system to
///             convert to map coordinates.
///   \param[in] origin An origin assigned to the map or terrain you are using
///              so that position can be converted to a Point3D value
///              which stores position in meters on a flat surface.
///
///   \return A 3D point within the coordinate frame of a map with the
///           mapped origin value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Pose::ConvertFromGlobalToMapCoordinates(const Wgs& position, const Utm& origin)
{
    Utm utmPosition;
    utmPosition << position;
    return ConvertFromGlobalToMapCoordinates(utmPosition, origin);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method converts a position in a Global Coordinate system (WGS,
///   UTM, etc.) to coordinates within a Cartographer Map using the origin
///   assigned to the maps origin [0,0,0].
///
///   \param[in] position Position data in a global coordinate system to
///             convert to map coordinates.
///   \param[in] origin An origin assigned to the map or terrain you are using
///              so that position can be converted to a Point3D value
///              which stores position in meters on a flat surface.
///
///   \return A 3D point within the coordinate frame of a map with the
///           mapped origin value.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Pose::ConvertFromGlobalToMapCoordinates(const Utm& position, const Utm& origin)
{
    Point3D mapPosition;
    mapPosition.mX = position.mEasting - origin.mEasting;
    mapPosition.mY = position.mNorthing - origin.mNorthing;
    mapPosition.mZ = position.mElevation;
    return mapPosition;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts from Cartographer coordinates on a map to WGS 
///   coordinates based on the origin assigned to the map.
///
///   \param[in] position A position value in Cartographer map coordinates to
///              convert.
///   \param[in] origin An origin assigned to the map or terrain you are using
///              so that currentPos can be converted to a Point3D value
///              which stores position in meters on a flat surface.
///
///   \return A WGS position value calculated using the position and the
///           origin of the map.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs Pose::ConvertFromMapToGlobalCoordinates(const Point3D& position, const Wgs& origin)
{
    Utm utmOrigin;
    Wgs wgsCurrentPos;
    
    utmOrigin << origin;
    wgsCurrentPos << ConvertFromMapToGlobalCoordinates(position, utmOrigin);
    return wgsCurrentPos;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts from Cartographer coordinates on a map to UTM 
///   coordinates based on the origin assigned to the map.
///
///   \param[in] position A position value in Cartographer map coordinates to
///              convert.
///   \param[in] origin An origin assigned to the map or terrain you are using
///              so that currentPos can be converted to a Point3D value
///              which stores position in meters on a flat surface.
///
///   \return A UTM position value calculated using the position and the
///           origin of the map.
///
////////////////////////////////////////////////////////////////////////////////////
Utm Pose::ConvertFromMapToGlobalCoordinates(const Point3D& position, const Utm& origin)
{
    Utm utmCurrentPos(origin);
    utmCurrentPos.mEasting += position.mX;
    utmCurrentPos.mNorthing += position.mY;
    utmCurrentPos.mElevation = position.mZ;
    return utmCurrentPos;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a point position value from local coordinates relative
///   to a specific pose to map coordinates (relative to map origin of [0,0,0].
///
///   Map coordinate frame is defined as follows: X axis is positive going East
///   and negative West, Y axis is positive going North, negative going South. The
///   Z axis is positive with increased elevation, negative going down (into 
///   terrain).  
///
///   The local coordinate frame is defined as follows:  X axis is positive in
///   front of the platform/object pose, negative behind.  Y axis is positive to the
///   right of the object, negative to the left.  Z axis is positive below the
///   platform, and negative above.  For example, if an object is 1 meter
///   in front of your vehicle, 2 meters to the right, and .5 meters above, the
///   local position value would be < x:1, y:2, z:-.5>.
///
///   For orientation values, a 0 Yaw value is facing north, with positive
///   Yaw to the right (90 degrees is East).  Yaw is a rotation around the Z
///   axis.  Pitch is a rotation around the local Y axis with a positive
///   pitch has the front pointing towards the sky, negative towards the
///   ground.  Roll is a rotation around the local X axis with positive
///   values tilting to the right side.
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] point A position value relative to the current pose.  In other
///                    words, a position relative to your platform or objects
///                    coordinate frame
///
///   \return The position value in 3D space within map coordinate frame.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Pose::ConvertFromLocalToMapCoordinates(const Pose& pose, const Point3D& point)
{
    CxUtils::Quaternion rotation;
    if(pose.mpPoseRotation)
    {
        rotation = *pose.mpPoseRotation;
    }
    else
    {
        rotation.CreateFromEuler(pose.mRotation.mY, 
                                 pose.mRotation.mX, 
                                 -pose.mRotation.mZ, 
                                 false);    
    }
    
    // Rotate the point around the local axis, then translate
    // to the global space.
    return pose.mPosition + rotation.Rotate(Point3D(point.mY, point.mX, -point.mZ));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a point position value from local coordinates relative
///   to a specific pose to map coordinates (relative to map origin of [0,0,0].
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] points A list of position values to convert from local to
///                     map coordinates.
///
///   \return A list of converted point values.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D::List ConvertFromLocalToMapCoordinates(const Pose& pose, const Point3D::List& points)
{
    Point3D::List mapCoordinates;
    Point3D::List::const_iterator localCoordinate;
    CxUtils::Quaternion rotation;
    if(pose.mpPoseRotation)
    {
        rotation = *pose.mpPoseRotation;
    }
    else
    {
        rotation.CreateFromEuler(pose.mRotation.mY, 
                                 pose.mRotation.mX, 
                                 -pose.mRotation.mZ, false);    
    }
    mapCoordinates.reserve(points.size());
    for(localCoordinate = points.begin();
        localCoordinate != points.end();
        localCoordinate++)
    {
       // Rotate the point around the local axis, then translate
       // to the global space.
       mapCoordinates.push_back(pose.mPosition + rotation.Rotate(Point3D(localCoordinate->mY, localCoordinate->mX, localCoordinate->mZ))); 
    }
    return mapCoordinates;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a position values from local coordinates relative
///   to a specific pose to map coordinates (relative to map origin of [0,0,0].
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] segment 3D segment to convert points from local to map coordinate
///              frame.
///
///   \return Segment converted to map coordinate frame using Pose data.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Pose::ConvertFromLocalToMapCoordinates(const Pose& pose, const Segment3D& segment)
{
    CxUtils::Quaternion rotation;
    Segment3D mapCoordinates(segment);
    if(pose.mpPoseRotation)
    {
        rotation = *pose.mpPoseRotation;
    }
    else
    {
        rotation.CreateFromEuler(pose.mRotation.mY, 
                                 pose.mRotation.mX, 
                                 -pose.mRotation.mZ, false);    
    }
    
    // Rotate the point around the local axis, then translate
    // to the global space.
    mapCoordinates.mPoint1 = pose.mPosition + rotation.Rotate(Point3D(segment.mPoint1.mY, segment.mPoint1.mX, segment.mPoint1.mZ));
    mapCoordinates.mPoint2 = pose.mPosition + rotation.Rotate(Point3D(segment.mPoint2.mY, segment.mPoint2.mX, segment.mPoint2.mZ));
    return mapCoordinates;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a position values from local coordinates relative
///   to a specific pose to map coordinates (relative to map origin of [0,0,0].
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] segments 3D segments to convert points from local to map coordinate
///              frame.
///
///   \return Segments converted to map coordinate frame using Pose data.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D::List Pose::ConvertFromLocalToMapCoordinates(const Pose& pose, const Segment3D::List& segments)
{
    Segment3D::List::const_iterator localCoordinate;
    Segment3D::List mapCoordinates;
    CxUtils::Quaternion rotation;
    if(pose.mpPoseRotation)
    {
        rotation = *pose.mpPoseRotation;
    }
    else
    {
        rotation.CreateFromEuler(pose.mRotation.mY, 
                                 pose.mRotation.mX, 
                                 -pose.mRotation.mZ, false);    
    }
    mapCoordinates.reserve(segments.size());
    for(localCoordinate = segments.begin();
        localCoordinate != segments.end();
        localCoordinate++)
    {
        Segment3D converted(*localCoordinate);
        // Rotate the point around the local axis, then translate
        // to the global space.
        converted.mPoint1 = pose.mPosition + rotation.Rotate(Point3D(localCoordinate->mPoint1.mY, localCoordinate->mPoint1.mX, localCoordinate->mPoint1.mZ));
        converted.mPoint2 = pose.mPosition + rotation.Rotate(Point3D(localCoordinate->mPoint2.mY, localCoordinate->mPoint2.mX, localCoordinate->mPoint2.mZ));
        mapCoordinates.push_back(converted); 
    }
    return mapCoordinates;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a point position value from map coordinates (relative
///   to the maps origin [0,0,0], to values relative to a pose.  This is
///   the inverse of converting to Map Coordinates.
///
///   Map coordinate frame is defined as follows: X axis is positive going East
///   and negative West, Y axis is positive going North, negative going South. The
///   Z axis is positive with increased elevation, negative going down (into 
///   terrain).  
///
///   The local coordinate frame is defined as follows:  X axis is positive in
///   front of the platform/object, negative behind.  Y axis is positive to the
///   right of the object, negative to the left.  Z axis is positive below the
///   platform, and negative above.  For example, if an object is 1 meter
///   in front of your vehicle, 2 meters to the right, and .5 meters above, the
///   local position value would be < x:1, y:2, z:-.5>.
///
///   For orientation values, a 0 Yaw value is facing north, with positive
///   Yaw to the right (90 degrees is East).  Yaw is a rotation around the Z
///   axis.  Pitch is a rotation around the local Y axis with a positive
///   pitch has the front pointing towards the sky, negative towards the
///   ground.  Roll is a rotation around the local X axis with positive
///   values tilting to the right side.
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] point A position value within a maps coordinate frame (global).
///
///   \return The position value in 3D space relative to the pose (local 
///           coordinates).
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Pose::ConvertFromMapToLocalCoordinates(const Pose& pose, const Point3D& point)
{
    Point3D result;
    CxUtils::Quaternion rotation;
    if(pose.mpPoseRotation)
    {
        rotation = *pose.mpPoseRotation;
    }
    else
    {
        rotation.CreateFromEuler(pose.mRotation.mY, 
                                 pose.mRotation.mX, 
                                 -pose.mRotation.mZ, false);    
    }
    // Translate point to origin, then apply inverse rotation to get
    // the local value relative to the Pose.
    result = rotation.Invert().Rotate(point - pose.mPosition);
    // Flip X and Y to match local coordinate frame.
    result(result.mY, result.mX, -result.mZ);
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts point position values from map coordinates (relative
///   to the maps origin [0,0,0], to values relative to a pose.  This is
///   the inverse of converting to Map Coordinates.
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] points Position values within a maps coordinate frame (global).
///
///   \return The position values in 3D space relative to the pose (local 
///           coordinates).
///
////////////////////////////////////////////////////////////////////////////////////
Point3D::List Pose::ConvertFromMapToLocalCoordinates(const Pose& pose, const Point3D::List& points)
{
    CxUtils::Quaternion inverseRotation;
    if(pose.mpPoseRotation)
    {
        inverseRotation = *pose.mpPoseRotation;
    }
    else
    {
        inverseRotation.CreateFromEuler(pose.mRotation.mY, 
                                        pose.mRotation.mX, 
                                        -pose.mRotation.mZ, false);    
    }
    inverseRotation = inverseRotation.Invert();
    
    Point3D::List::const_iterator mapCoordinate;
    Point3D::List localCoordinates;
    Point3D temp;
    // Translate point to origin, then apply inverse rotation to get
    // the local value relative to the Pose.
    for(mapCoordinate = points.begin();
        mapCoordinate != points.end();
        mapCoordinate++)
    {
        temp = inverseRotation.Rotate(*mapCoordinate - pose.mPosition);
        // Flip X and Y to match local coordinate frame.
        localCoordinates.push_back(Point3D(temp.mY, temp.mX, temp.mZ));
    }
    
    return localCoordinates;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts position values from map coordinates (relative
///   to the maps origin [0,0,0], to values relative to a pose.  This is
///   the inverse of converting to Map Coordinates.
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] segment 3D segment to convert from map to local coordinate frame.
///
///   \return The segment converted to local coordinates using Pose data.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D Pose::ConvertFromMapToLocalCoordinates(const Pose& pose, const Segment3D& segment)
{
    CxUtils::Quaternion inverseRotation;
    Segment3D mapCoordinates(segment);
    if(pose.mpPoseRotation)
    {
        inverseRotation = *pose.mpPoseRotation;
    }
    else
    {
        inverseRotation.CreateFromEuler(pose.mRotation.mY, 
                                        pose.mRotation.mX, 
                                        -pose.mRotation.mZ, false);    
    }
    inverseRotation = inverseRotation.Invert();
    // Rotate the point around the local axis, then translate
    // to the global space.
    mapCoordinates.mPoint1 = inverseRotation.Rotate(segment.mPoint1 - pose.mPosition);
    mapCoordinates.mPoint2 = inverseRotation.Rotate(segment.mPoint2 - pose.mPosition);
    // Flip X and Y to match local coordinate frame.
    mapCoordinates.mPoint1(mapCoordinates.mPoint1.mY, mapCoordinates.mPoint1.mX, mapCoordinates.mPoint1.mZ);
    mapCoordinates.mPoint2(mapCoordinates.mPoint2.mY, mapCoordinates.mPoint2.mX, mapCoordinates.mPoint2.mZ);
    return mapCoordinates;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts position values from map coordinates (relative
///   to the maps origin [0,0,0], to values relative to a pose.  This is
///   the inverse of converting to Map Coordinates.
///
///   \param[in] pose A position and orientation in 3D space that is in
///                   map coordinates.
///   \param[in] segments 3D segments to convert from map to local coordinate frame.
///
///   \return The segments converted to local coordinates using Pose data.
///
////////////////////////////////////////////////////////////////////////////////////
Segment3D::List Pose::ConvertFromMapToLocalCoordinates(const Pose& pose, const Segment3D::List& segments)
{
    Segment3D::List::const_iterator localCoordinate;
    Segment3D::List mapCoordinates;
    CxUtils::Quaternion inverseRotation;
    if(pose.mpPoseRotation)
    {
        inverseRotation = *pose.mpPoseRotation;
    }
    else
    {
        inverseRotation.CreateFromEuler(pose.mRotation.mY, 
                                        pose.mRotation.mX, 
                                        -pose.mRotation.mZ, false);    
    }
    inverseRotation = inverseRotation.Invert();
    mapCoordinates.reserve(segments.size());
    for(localCoordinate = segments.begin();
        localCoordinate != segments.end();
        localCoordinate++)
    {
        Segment3D converted(*localCoordinate);
        // Rotate the point around the local axis, then translate
        // to the global space.

        converted.mPoint1 = inverseRotation.Rotate(localCoordinate->mPoint1 - pose.mPosition);
        converted.mPoint2 = inverseRotation.Rotate(localCoordinate->mPoint2 - pose.mPosition);
        // Flip X and Y to match local coordinate frame.
        converted.mPoint1(converted.mPoint1.mY, converted.mPoint1.mX, converted.mPoint1.mZ);
        converted.mPoint2(converted.mPoint2.mY, converted.mPoint2.mX, converted.mPoint2.mZ);
        mapCoordinates.push_back(converted); 
    }
    return mapCoordinates;
}


#ifdef USE_OPENCV

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws a triangle representing the Pose into the map image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] platformLength Lenght of your platform.
///   \param[in] platformWidth Width of your platform.
///   \param[in] color color to set.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Pose::Draw(IplImage* image, 
                const double pixelsPerMeter, 
                const Point3D& mapBottomCorner,
                const double platformLength,
                const double platformWidth,
                const CvScalar color) const
{
    Point3D front(platformLength/2.0, 0, 0);
    Point3D left(-platformLength/2.0, -platformWidth/2.0, 0);
    Point3D right(-platformLength/2.0, platformWidth/2.0, 0);

    front = ConvertFromLocalToMapCoordinates(*this, front);
    left = ConvertFromLocalToMapCoordinates(*this, left);
    right = ConvertFromLocalToMapCoordinates(*this, right);
    return Cartographer::Draw(image, pixelsPerMeter, mapBottomCorner, left, front, right, color);
}


#endif

/*  End of File */
