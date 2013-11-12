////////////////////////////////////////////////////////////////////////////////////
///
///  \file sphere.h
///  \brief Object class for describing a 3D point sphere object.  This object
///  type is useful for descrbing point or basic range detection data from
///  sensors.
///
///  <br>Author(s): Daniel Barber, Michael Scherer
///  <br>Created: 20 December 2008
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
#ifndef _LIB__CARTOGRAPHER_OBJECTS_SPHERE__H
#define _LIB__CARTOGRAPHER_OBJECTS_SPHERE__H

#include "cartographer/object.h"

namespace Cartographer
{
    namespace Objects
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Sphere
        ///   \brief Simple 3D sphere object class.  Used to describe the most primitive
        ///   of obstacle data within a 3D environment.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Sphere : public Object
        {
        public:
            const static std::string Name;  ///<  Name of object.
            Sphere(const unsigned int sourceID = 0);
            Sphere(const Point3D& origin,
                   const double radius,
                   const unsigned int sourceID = 0);
            Sphere(const Sphere& sphere);
            virtual ~Sphere();
            // Gets the origin of object in 3D space.
            virtual Point3D GetOrigin() const { return mOrigin; }
            // Gets a bounding sphere which encapsulates the object data.
            virtual BoundingSphere GetBoundingSphere() const;
            // Gets a 3D bounding box which encapsulates the object data.
            virtual BoundingBox GetBoundingBox() const;
            // Check to see if an object is the same object (used to merge data from multiple sensors).
            virtual bool IsSameAs(const Object* object,
                                  const double allowedError = 0.01) const;
            // If the objects are the same, merge the contents by consuming the data and updating data members.
            virtual bool ConsumeMatch(const Object* object);
            // Checks if there is a collision within radius of point in the XY Plane (but within z boundaries).
            virtual bool IsCollisionInXY(const Point3D& point,
                                         const double radius,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            // Checks for collision with segment in XY plane (but within z boundaries).
            virtual bool IsCollisionInXY(const Segment3D& segment,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            // Checks for collision within triangle in XY plane (but within z boundaries).
            virtual bool IsCollisionInXY(const Point3D& p1,
                                         const Point3D& p2,
                                         const Point3D& p3,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            // Get distance from this object to a point
            virtual double GetDistance(const Point3D& point) const;

            bool CollidesWith(const Point3D& point, const double radius) const;
            bool CollidesWith(const Segment3D& segment) const;
            bool CollidesWith(const Point3D& a,
                              const Point3D& b,
                              const Point3D& c) const;
            // Clears object data.
            virtual void Clear();
            // Create an exact duplicate of an Object.
            virtual Object* Clone() const { return (Object *)new Sphere(*this); }
            // Converts the objects coordinates from Local to Map.
            virtual Object* ConvertFromLocalToMapCoordinates(const Pose& pose) const;
            // Converts the objects coordinates from Map to Local coordinates.
            virtual Object* ConvertFromMapToLocalCoordinates(const Pose& pose) const;
            void SetOrigin(const Point3D& origin);
            void SetRadius(const double radius);
            inline double GetRadius() const { return mRadius; }
            // Sets equal to.
            Sphere& operator=(const Sphere& sphere);
            // Converts to a string.
            virtual std::string ToString() const;
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
            virtual bool Draw(IplImage* image, const double pixelsPerMeter, const Point3D& mapBottomCorner, CvScalar color = CV_RGB(0, 255, 0)) const;

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Gets the color of the object.
            ///
            ///   \return Color object should be in an image.
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual CvScalar GetDrawColor() const { return CV_RGB(255, 165, 0); }
#endif
        protected:
            Point3D mOrigin;    ///<  Origin of the sphere in 3D space.
            double mRadius;     ///<  Radius of the sphere.
            Point3D mSumOfPointValues;   ///<  Summation of origin values used for running average in ConsumeMatch.
            double mSumOfRadiusValues;   ///<  Summation of radius values used for running average in ConsumeMatch.
            unsigned int mTotalConsumed; ///<  Total number of consumed Sphere's used in running average of Consume Match.
        };
    }
}


#endif
/*  End of File */
