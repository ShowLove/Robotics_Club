////////////////////////////////////////////////////////////////////////////////////
///
///  \file objectmap.cpp
///  \brief Contains the parent class for deriving any map structures used within
///  Cartographer that store Object based structures within for representing
///  data within a map.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 2011
///  <br>Copyright (c) 2011
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
#include "cartographer/objectmap.h"
#include <cstdio>

using namespace Cartographer;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
ObjectMap::ObjectMap()
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
ObjectMap::~ObjectMap()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to add objects to the map.
///
///   This method should take the input object list and add them to the map.  It
///   should perform any object matching if possible to reduce duplication
///   of data (used in Vector based maps).
///
///   \param[in] objects List of objects to add to the map.  Objects must be
///                      in the maps coordinate frame (e.g. global coodinates).
///   \param[in] clear If true, all previous data is cleared out.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::AddObjects(const Object::List& objects, const bool clear)
{
    Object::List::const_iterator obj;
    unsigned int total = 0;

    if(clear)
    {
        this->Clear();
    }

    for(obj = objects.begin();
        obj != objects.end();
        obj++)
    {
        if(AddObject(*obj))
            total++;
    }
    if(total == (unsigned int)objects.size())
        return true;

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to add an object to the map.
///
///   This method should take the input object and add it to the map.  It
///   should perform any object matching if possible to reduce duplication
///   of data (used in Vector based maps).
///
///   \param[in] object  Object to add to the map.  Objects must be
///                      in the local coordinate frame (e.g. global coodinates).
///   \param[in] pose    Pose coordinates of sensor in the global frame.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::AddObject(const Object* object, const Pose& pose)
{
    bool result = false;
    Object * o = object->ConvertFromLocalToMapCoordinates(pose);
    result = AddObject(o);
    delete o;
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to add objects to the map.
///
///   This method should take the input object list and add them to the map.  It
///   should perform any object matching if possible to reduce duplication
///   of data (used in Vector based maps).
///
///   \param[in] objects List of objects to add to the map.  Objects must be
///                      in the local coordinate frame (e.g. global coodinates).
///   \param[in] pose    Pose coordinates of sensor in the global frame.
///   \param[in] clear If true, all previous data is cleared out.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::AddObjects(const Object::List& objects, const Pose& pose, const bool clear)
{
    Object::List::const_iterator obj;
    unsigned int total = 0;

    if(clear)
    {
        this->Clear();
    }

    for(obj = objects.begin();
        obj != objects.end();
        obj++)
    {
        if(AddObject(*obj, pose))
            total++;
    }
    if(total == (unsigned int)objects.size())
        return true;

    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects around the radius
///          of a point in the map relative to local position.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] point The position in the map to perform the scan at.
///   \param[in] radius The distance around the point in meters to look for
///                     objects.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.  This value is relative to
///                    the elevation of the pose passed.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///                   This values is relative to the pose passed to this
///                   function.  For example, if zLow = 1, and the pose
///                   elevation is 10, the method will check for objects in the
///                   global frame above 11 meters.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::ScanLocalRadiusInXY(const Pose& pose,
                                      const Point3D& point,
                                      const double radius,
                                      Object::Collisions* collisions,
                                      const double zHigh,
                                      const double zLow) const
{
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return ScanRadiusInXY(Pose::ConvertFromLocalToMapCoordinates(pose, point),
                          radius,
                          collisions,
                          globalZHigh,
                          globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects within a segment in the
///   map relative to local position.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] segment The segment to search along for objects.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.  This value is relative to
///                    the elevation of the pose passed.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///                   This values is relative to the pose passed to this
///                   function.  For example, if zLow = 1, and the pose
///                   elevation is 10, the method will check for objects in the
///                   global frame above 11 meters.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::ScanLocalSegmentInXY(const Pose& pose,
                                     const Segment3D& segment,
                                     Object::Collisions* collisions,
                                     const double zHigh,
                                     const double zLow) const
{
    Segment3D newsegment = Pose::ConvertFromLocalToMapCoordinates(pose, segment);
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return ScanSegmentInXY(newsegment, collisions, globalZHigh, globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects within a triangle
///   defined by 3 points, in relative local coordinates.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.  This value is relative to
///                    the elevation of the pose passed.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///                   This values is relative to the pose passed to this
///                   function.  For example, if zLow = 1, and the pose
///                   elevation is 10, the method will check for objects in the
///                   global frame above 11 meters.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool ObjectMap::ScanLocalTriangleInXY(const Pose& pose,
                                     const Point3D& p1,
                                     const Point3D& p2,
                                     const Point3D& p3,
                                     Object::Collisions* collisions,
                                     const double zHigh,
                                     const double zLow) const
{
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return ScanTriangleInXY(Pose::ConvertFromLocalToMapCoordinates(pose, p1),
                            Pose::ConvertFromLocalToMapCoordinates(pose, p2),
                            Pose::ConvertFromLocalToMapCoordinates(pose, p3),
                            collisions,
                            globalZHigh,
                            globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Set a threshold value to use when attempting to merge different
///          types of objects using the Object::SameAs and Object::ConsumeMatch
///          methods.
///
///   \param[in] objectIdentification Object identification type (e.g. "Sphere").
///   \param[in] allowedError The threshold value to use for merging the type of
///                           data.
///
////////////////////////////////////////////////////////////////////////////////////
void ObjectMap::SetConsumeThreshold(const std::string& objectIdentification,
                                    const double allowedError)
{
    mConsumeThresholds[objectIdentification] = allowedError;
}


/*  End of File */

