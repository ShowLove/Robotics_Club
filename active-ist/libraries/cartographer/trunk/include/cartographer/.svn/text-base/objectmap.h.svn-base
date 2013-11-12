////////////////////////////////////////////////////////////////////////////////////
///
///  \file objectmap.h
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
#ifndef __LIB_CARTOGRAPHER_OBJECT_MAP__H
#define __LIB_CARTOGRAPHER_OBJECT_MAP__H

#include "cartographer/map.h"

namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class ObjectMap
    ///   \brief Object maps store data represented as geometric shapes derived from
    ///          the Object class type.  The opposite of this map is the RasterMap
    ///          which represents terrain data as pixel values with different
    ///          intensities like in an image.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL ObjectMap : public Map
    {
    public:
        ObjectMap();
        virtual ~ObjectMap();
        // Update all the objects in the map, removing those which have expired
        virtual void Update() {};
        // Adds object in map coordinates to map.
        virtual bool AddObject(const Object* object) { return false; }
        // Add objects using map coordinates.
        virtual bool AddObjects(const Object::List& objects, const bool clear = false);
        // Add objects tot he map relative to a pose.
        virtual bool AddObject(const Object* object, const Pose& pose);
        // Add objects tot he map relative to a pose.
        virtual bool AddObjects(const Object::List& objects, const Pose& pose, const bool clear = false);
        virtual void Clear() = 0;
        virtual int ClearObjectsBySource(const unsigned int sourceID) { return 0; }
        virtual int ClearObjectsByIdentification(const std::string& identification) { return 0; }
        virtual int Clear(const Point3D& point,
                          const double radius,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        virtual int Clear(const Segment3D& segment,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        virtual int Clear(const Point3D& p1,
                          const Point3D& p2,
                          const Point3D& p3,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        virtual int ClearLocal(const Pose& pose,
                          const Point3D& point,
                          const double radius,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        virtual int ClearLocal(const Pose& pose,
                          const Segment3D& segment,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        virtual int ClearLocal(const Pose& pose,
                          const Point3D& p1,
                          const Point3D& p2,
                          const Point3D& p3,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT) { return 0; }
        // Scan map, saves collisions detected to Object::Collisions.
        virtual bool ScanRadiusInXY(const Point3D& point,
                                    const double radius,
                                    Object::Collisions* collisions = NULL,
                                    const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                    const double zLow = OBJECT_Z_LOW_DEFAULT) const  { return false; }
        // Scan map, saves collisions detected to Object::Collisions.
        virtual bool ScanSegmentInXY(const Segment3D& segment,
                                     Object::Collisions* collisions = NULL,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        // Scan map, saves collisions detected to Object::Collisions.
        virtual bool ScanTriangleInXY(const Point3D& p1,
                                      const Point3D& p2,
                                      const Point3D& p3,
                                      Object::Collisions* collisions = NULL,
                                      const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                      const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        virtual bool IsCollisionInXY(const Point3D& point,
                                     const double radius,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const   { return ScanRadiusInXY(point, radius, NULL, zHigh, zLow); }
        virtual bool IsCollisionInXY(const Segment3D& segment,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return ScanSegmentInXY(segment, NULL, zHigh, zLow); }
        virtual bool IsCollisionInXY(const Point3D& p1,
                                     const Point3D& p2,
                                     const Point3D& p3,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return ScanTriangleInXY(p1, p2, p3, NULL, zHigh, zLow); }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to get a pointer to objects in the map.
        ///
        ///   \return Pointer to objects contained in the map, NULL if the map cannot
        ///           represent object data this way.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual Object::Set* GetObjects() { return NULL; };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Use this method to get a pointer to objects in the map.
        ///
        ///   \return Pointer to objects contained in the map, NULL if the map cannot
        ///           represent object data this way.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual const Object::Set* GetObjects() const { return NULL; };
        virtual bool ScanLocalRadiusInXY(const Pose& pose,
                                         const Point3D& point,
                                         const double radius,
                                         Object::Collisions* collisions = NULL,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                         const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanLocalSegmentInXY(const Pose& pose,
                                          const Segment3D& segment,
                                          Object::Collisions* collisions = NULL,
                                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                          const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanLocalTriangleInXY(const Pose& pose,
                                           const Point3D& p1,
                                           const Point3D& p2,
                                           const Point3D& p3,
                                           Object::Collisions* collisions = NULL,
                                           const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                           const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        void SetConsumeThreshold(const std::string& objectIdentification,
                                 const double allowedError);
        const Object::Factory* GetObjectFactory() const { return &mObjectFactory; }
        Object::Factory* GetObjectFactory() { return &mObjectFactory; }
    protected:
        Object::Factory mObjectFactory;         ///<  Object factory.
        std::map<std::string, double> mConsumeThresholds;   ///<  Map of thresholds to use when merging data.
    };
}

#endif
/*  End of File */
