////////////////////////////////////////////////////////////////////////////////////
///
///  \file object.h
///  \brief Base class for defining any object stored in a map used within this
///         library.  All objects must be derived from this super class.
///
///  <br>Author(s): Daniel Barber, Michael Scherer
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
#ifndef __LIB_CARTOGRAPHER_OBJECT__H
#define __LIB_CARTOGRAPHER_OBJECT__H

#include "cartographer/boundingbox.h"
#include "cartographer/pose.h"
#include <cxutils/math/cxmath.h>
#include <string>
#include <vector>
#include <set>
#include <map>

////////////////////////////////////////////////////////////////////////////////////
///
///   \namespace Cartographer
///   \brief Cartographer is the namespace for all interfaces and structures
///          within the Cartographer Robot Navigation Library.
///
////////////////////////////////////////////////////////////////////////////////////
namespace Cartographer
{
    const double OBJECT_Z_HIGH_DEFAULT = 30000.0;  // Default Z High threshold for filtering out data points within XY plane
    const double OBJECT_Z_LOW_DEFAULT = -30000.0;  // Default Z Low threshold for filtering out data points within XY plane

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Object
    ///   \brief Base class for creating objects represented in maps.
    ///
    ///   This base class contains functions that must be
    ///   implemented so that map analysis can be done easily and
    ///   expanded upon in the future with little change.
    ///
    ///   All objects are built from a basic type (Point, Line, or Polygon).
    ///   The Object class does not limit how you can describe your object, keeping
    ///   it generic, but having the operations needed to check for collisions and
    ///   intersections.
    ///
    ///   Objects are tagged with a time stamp in case a Map decides to assign
    ///   a Time to Live (TTL).  They also have the ID of the source of the
    ///   data in the event you want to tag and Object by it's "creator/sensor".
    ///
    ///   All methods used for comparison assume that the Objects and Points are
    ///   within the same coordinate frame (local versus global).
    ///
    ///   For an example of how to derive Objects using this class, see the
    ///   Point class.
    ///
    ///   All position values are in meters, angles in radians.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL Object
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Extras
        ///   \brief Extras is a data structure for additing additional data to an
        ///          Object that may be specific to a map.
        ///
        ///   Extra data is added using the SetMapInfo method.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Extras
        {
        public:
            Extras() {}
            virtual ~Extras() {}
            virtual Extras* Clone() const = 0;
        };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Factory
        ///   \brief Object factory data structure for creation of objects based on
        ///          string names.
        ///
        ///   By default, all object classes that are part of this library are already
        ///   part of the Factory.  Add your own custom ones using AddObject method.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Factory
        {
        public:
            Factory();
            virtual ~Factory();
            // Add object to factory for creation of data. Duplicates are deleted.
            bool AddObject(const std::string& name, Object* objectPtr);
            // Method to create an object based on the name.
            Object* CreateObject(const std::string& name) const;
        protected:
            Mutex mFactoryMutex;                      ///<  Mutex for thread protection of data.
            std::map<std::string, Object*> mObjects;  ///<  Objects the factory can create.
        };
        static const unsigned int Infinite = 0;
        typedef std::vector<Object*> List;              ///<  List (std::vector) of Objects.
        typedef std::vector<const Object*> Collisions;  ///<  List of objects that collided with scans, etc.
        typedef std::set<Object*> Set;                  ///<  List (std::set) of Objects.
        // The type object (base type).
        enum Type
        {
            Point = 0,
            Line,
            Polygon
        };
        // Types of orientations on a 2D map objects can have to each other
        enum Orientation
        {
            Unknown = 0,
            Overlap,
            North,
            NorthEast,
            East,
            SouthEast,
            South,
            SouthWest,
            West,
            NorthWest
        };
        // Constructor.
        Object(const Type type,
               const std::string& identification,
               const unsigned int sourceID = 0);
        // Destructor.
        virtual ~Object();
        // Set a unique ID associated with the type of object (Optional).
        void SetUniqueID(const unsigned int uniqueID);
        // Set the ID of the creator of the object (defaults to 0).
        void SetSourceID(const unsigned int sourceID);
        // Set the time that the data was created or detected.
        void SetTimeStamp(const Time& timeStamp);
        // Set the time that the object will live in map.
        void SetTimeToLive(const unsigned int timtolivems);
        // Gets the basic type the object derives from.
        Type GetType() const;
        // Gets the source/creator that detected the object.
        unsigned int GetSourceID() const;
        // Gets the time when the data was created or detected.
        Time GetTimeStamp() const;
        // Get the amount of time an object can live in maps.
        unsigned int GetTimeToLive() const;
        // IsDead determines if the time to live has expired based on creation time.
        bool IsDead() const;
        // Gets identification string associated with object (used to determine type of object).
        std::string GetIdentification() const;
        // Gets the unquie identifier associated with the object (optional).
        unsigned int GetUniqueID() const;
        // Gets the origin of object in 3D space.
        virtual Point3D GetOrigin() const = 0;
        // Gets a bounding sphere which encapsulates the object data.
        virtual BoundingSphere GetBoundingSphere() const = 0;
        // Gets a 3D bounding box which encapsulates the object data.
        virtual BoundingBox GetBoundingBox() const = 0;
        // Check to see if an object is the same object (used to merge data from multiple sensors).
        virtual bool IsSameAs(const Object* object,
                              const double allowedError = 0.01) const = 0;
        // If the objects are the same, merge the contents by consuming the data and updating data members.
        // However, this method does not check to see if they are the same (use IsSameAs method).
        virtual bool ConsumeMatch(const Object* object) = 0;
        // Checks if there is a collision within radius of point in the XY Plane (but within z boundaries).
        virtual bool IsCollisionInXY(const Point3D& point,
                                     const double radius,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const = 0;
        // Checks for collision with segment in XY plane (but within z boundaries).
        virtual bool IsCollisionInXY(const Segment3D& segment,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const = 0;
        // Checks for collision within triangle in XY plane (but within z boundaries).
        virtual bool IsCollisionInXY(const Point3D& p1,
                                     const Point3D& p2,
                                     const Point3D& p3,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const = 0;
        // Get distance from this object to a point
        virtual double GetDistance(const Point3D& point) const = 0;
        // Create an exact duplicate of an Object.
        virtual Object* Clone() const = 0;
        // Converts the objects coordinates from Local to Map.
        virtual Object* ConvertFromLocalToMapCoordinates(const Pose& pose) const = 0;
        // Converts the objects coordinates from Map to Local coordinates.
        virtual Object* ConvertFromMapToLocalCoordinates(const Pose& pose) const = 0;
        // Clears object data.
        virtual void Clear();
        // Copies data specific to the object.
        void CopyObjectData(const Object* object);
        // Get the future state/position/etc. of the Object after a specified amount of time has elapsed.
        virtual Object* GetPredictedFutureState(const double elapsedTimeSeconds = 0.0) const;
        // Return data in object as a string for printing or debugging.
        virtual std::string ToString() const;
        // Get distance to a point.
        static double GetDistanceToPoint(const Point3D& point,
                                         const Segment3D& segment);
        // Writes object data to a packet so it can be logged.
        virtual int WriteToPacket(Packet& packet) const;
        // Reads object data from a packet so it can be logged.
        virtual int ReadFromPacket(const Packet& packet);
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
        virtual bool Draw(IplImage* image,
                          const double pixelsPerMeter,
                          const Point3D& mapBottomCorner,
                          CvScalar color = CV_RGB(75, 75, 75)) const = 0;

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the color of the object.
        ///
        ///   \return Color object should be in an image.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual CvScalar GetDrawColor() const = 0;

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Gets the color of the object as it fades with time.
        ///
        ///   \return Color faded as object gets closer to expiring
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual CvScalar CalculateDrawColor(CvScalar color) const;
#endif
        void SetMapData(Extras* data)
        {
            if(mpMapExtras)
            {
                delete mpMapExtras;
                mpMapExtras = NULL;
            }
            if(data)
            {
                mpMapExtras = data;
            }
        }
        Extras* GetMapData() const { return mpMapExtras; }
        Extras* GetMapData() { return mpMapExtras; }
    protected:
        const Type mObjectType;     ///<  Object primitive type.
        unsigned int mSourceID;     ///<  Creator of the object (sensor/data source).
        Time mTimeStamp;            ///<  Time of object detection/creation.
        unsigned int mTimeToLiveMs; ///<  Time to live for objects. 0 = INFINITE.
        std::string mObjectIdentification;  ///<  Optional identification string to associate with object.
        unsigned int mUniqueID;             ///<  Unique Identification Number for this type of object.
    private:
        Extras* mpMapExtras;                  ///<  Pointer to additional data that may be needed by maps, but not by Object.
    };

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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
                               const double pixelsPerMeter,
                               const Point3D& mapBottomCorner,
                               const Point3D& point, const double radius,
                               const CvScalar color = CV_RGB(75, 75, 75));
                  
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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
                               const double pixelsPerMeter,
                               const Point3D& mapBottomCorner,
                               const Pose& pose,
                               const Point3D& point, const double radius,
                               const CvScalar color = CV_RGB(75, 75, 75));

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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
                               const double pixelsPerMeter,
                               const Point3D& mapBottomCorner,
                               const Segment3D& segment,
                               const CvScalar color = CV_RGB(75, 75, 75));

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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
                               const double pixelsPerMeter,
                               const Point3D& mapBottomCorner,
                               const Pose& pose,
                               const Segment3D& segment,
                               const CvScalar color = CV_RGB(75, 75, 75));

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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
                               const double pixelsPerMeter,
                               const Point3D& mapBottomCorner,
                               const Point3D& p1,
                               const Point3D& p2,
                               const Point3D& p3,
                               const CvScalar color = CV_RGB(75, 75, 75));
    
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
    CARTOGRAPHER_DLL bool Draw(IplImage* image,
              const double pixelsPerMeter,
              const Point3D& mapBottomCorner,
              const Pose& pose,
              const Point3D& p1,
              const Point3D& p2,
              const Point3D& p3,
              const CvScalar color = CV_RGB(75, 75, 75));

    CARTOGRAPHER_DLL bool Draw(IplImage* image,
              const double pixelsPerMeter,
              const Point3D& mapBottomCorner,
              const BoundingBox& box,
              const CvScalar color = CV_RGB(75, 75, 75));

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
    CARTOGRAPHER_DLL CvPoint ConvertToImagePoint(const int imageHeight,
                                const double pixelsPerLength,
                                const Point3D& point,
                                const Point3D& offset,
                                const bool flipYAxis = true);
#endif
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Deletes object data within the list.
    ///
    ///   \param[in] objects Object data to delete.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    inline void ClearObjectList(Object::List& objects)
    {
        Object::List::iterator obj;
        for(obj = objects.begin();
            obj != objects.end();
            obj++)
        {
            if(*obj)
            {
                delete *obj;
            }
        }
        objects.clear();
    }

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Deletes object data within the set.
    ///
    ///   \param[in] objects Object data to delete.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    inline void ClearObjectSet(Object::Set& objects)
    {
        Object::Set::iterator obj;
        for(obj = objects.begin();
            obj != objects.end();
            obj++)
        {
            if(*obj)
            {
                delete *obj;
            }
        }
        objects.clear();
    }
};

#endif
/*  End of File */
