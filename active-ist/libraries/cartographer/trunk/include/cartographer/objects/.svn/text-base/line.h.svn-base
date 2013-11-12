////////////////////////////////////////////////////////////////////////////////////
///
///  \file line.h
///  \brief Object class for describing a 3D line object.  This object
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
#ifndef _LIB__CARTOGRAPHER_OBJECTS_LINE__H
#define _LIB__CARTOGRAPHER_OBJECTS_LINE__H

#include "cartographer/object.h"

namespace Cartographer
{

    namespace Objects
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Line
        ///   \brief Simple 3D line object class.  Used to describe the most primitive
        ///   of obstacle data within a 3D environment. 
        ///
        ///   One use of this object is to define pipes or event terrain height information.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Line : public Object
        {
        public:
            const static std::string Name;  ///<  Name of object.
            Line(const unsigned int sourceID = 0);
            Line(const Segment3D& line, const unsigned int sourceID = 0);
            Line(const Line& line);
            virtual ~Line();
            virtual Point3D GetOrigin() const { return mLine.GetMidpoint(); }
            virtual BoundingSphere GetBoundingSphere() const;
            virtual BoundingBox GetBoundingBox() const;
            virtual bool IsSameAs(const Object* object, 
                                  const double allowedError = 0.01) const;
            virtual bool ConsumeMatch(const Object* object);
            virtual bool IsCollisionInXY(const Point3D& point, 
                                         const double radius, 
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            virtual bool IsCollisionInXY(const Segment3D& segment,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            virtual bool IsCollisionInXY(const Point3D& p1, 
                                         const Point3D& p2,
                                         const Point3D& p3,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            // Get distance from this object to a point
            virtual double GetDistance(const Point3D& point) const;

            Segment3D& GetLine() { return mLine; }
            const Segment3D& GetLine() const { return mLine; }
            virtual void Clear();
            virtual Object* Clone() const { return (Object *)new Line(*this); }
            virtual Object* ConvertFromLocalToMapCoordinates(const Pose& pose) const;
            virtual Object* ConvertFromMapToLocalCoordinates(const Pose& pose) const;
            Line& operator=(const Line& line);
            virtual std::string ToString() const;
            // Writes object data to a packet so it can be logged.
            virtual int WriteToPacket(Packet& packet) const;
            // Reads object data from a packet so it can be logged.
            virtual int ReadFromPacket(const Packet& packet);
#ifdef USE_OPENCV
            virtual bool Draw(IplImage* image, const double pixelsPerMeter, const Point3D& mapBottomCorner, CvScalar color = CV_RGB(255, 148, 0)) const;
            virtual CvScalar GetDrawColor() const { return CV_RGB(255, 148, 0); }
#endif
        protected:
            Line(const Object::Type type, const std::string& identification, const unsigned int sourceID) :
                Object(type, identification, sourceID) {}

            Segment3D mLine;     ///<  Line data.
        };
    };
};

#endif
/*  End of File */
