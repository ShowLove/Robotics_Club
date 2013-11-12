////////////////////////////////////////////////////////////////////////////////////
///
///  \file worldmap.h
///  \brief Uses the map class to create a map of objects at absolute coordinates
///
///  <br>Author(s): David Adams
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu
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
#ifndef __LIB_CARTOGRAPHER_WORLDMAP__H
#define __LIB_CARTOGRAPHER_WORLDMAP__H

#include "cartographer/objectmap.h"

namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class WorldMap
    ///   \brief Absolute map of world within the Cartographer
    ///   Library.
    ///
    ///   All maps have common features within the libraries, so they must all
    ///   be based off of this structure.  It includes basic methods for searches
    ///   used for obstacle detection.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL WorldMap : public ObjectMap
    {
    public:
        WorldMap();
        virtual ~WorldMap();
        virtual bool Create(const double mapSizeMeters,
                            const double cellSizeMeters = 1.0);
        virtual void Destroy();
        virtual void Clear();
        virtual void Update();
        virtual bool AddObject(const Object* object, const Pose& pose){return ObjectMap::AddObject(object, pose);}
        virtual bool AddObject(const Object* object);
        virtual bool RemoveObject(Object* object);
        virtual bool CopyTo(Map* map) const;
        virtual int ClearObjectsBySource(const unsigned int sourceID);
        virtual int ClearObjectsByIdentification(const std::string& identification);
        virtual int Clear(const Point3D& point,
                          const double radius,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual int Clear(const Segment3D& segment,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual int Clear(const Point3D& p1,
                          const Point3D& p2,
                          const Point3D& p3,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual int ClearLocal(const Pose& pose,
                          const Point3D& point,
                          const double radius,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual int ClearLocal(const Pose& pose,
                          const Segment3D& segment,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual int ClearLocal(const Pose& pose,
                          const Point3D& p1,
                          const Point3D& p2,
                          const Point3D& p3,
                          const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                          const double zLow = OBJECT_Z_LOW_DEFAULT);
        virtual bool ScanRadiusInXY(const Point3D& point,
                                    const double radius,
                                    Object::Collisions* collisions = NULL,
                                    const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                    const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanSegmentInXY(const Segment3D& segment,
                                     Object::Collisions* collisions = NULL,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanTriangleInXY(const Point3D& p1,
                                      const Point3D& p2,
                                      const Point3D& p3,
                                      Object::Collisions* collisions = NULL,
                                      const double zHigh = OBJECT_Z_HIGH_DEFAULT,
                                      const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual const Object::Set* GetObjects() const { return &mObjects; }
        virtual Object::Set* GetObjects() { return &mObjects; }
        virtual std::string ToString() const;
#ifdef USE_OPENCV
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Draws the map to the image.
        ///
        ///   \param[in] image Opencv image to draw to.
        ///   \param[in] pixelsPerMeter Pixels per meter conversion value.
        ///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
        ///   \param[in] setBackgroundColor Set a backgroundcolor.
        ///   \param[in] backgroundColor Color to set background to.
        ///
        ///   \return If map was able to be drawn.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool Draw(IplImage* image,
                          const double pixelsPerMeter,
                          const Point3D& mapBottomCorner,
                          const bool setBackgroundColor = true,
                          CvScalar backgroundColor = CV_RGB(0, 0, 0)) const;
#endif
    protected:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class MatchInfo
        ///   \brief Extra info to know if an object has been checked/access, already.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL MatchInfo : public Object::Extras
        {
        public:
            MatchInfo(const bool flag) : mFlag(flag) {}
            ~MatchInfo() {}
            virtual Object::Extras* Clone() const { return new MatchInfo(mFlag); }
            bool mFlag; ///<  Match flag data.
        };
        Object::Set  mObjects;              ///< List of objects in map
        std::vector<Object::List> mCells;   ///< Cells that divide the
        bool mCreatedFlag;                  ///< Flag if map has been created yet
        inline void SetObjectUsedFlag(Object::List& objects, const bool flag) const
        {
            Object::List::iterator obj;
            for(obj = objects.begin();
                obj != objects.end();
                obj++)
            {
               MatchInfo *info = dynamic_cast<MatchInfo*>((*obj)->GetMapData());
               if(info)
               {
                   info->mFlag = flag;
               }
            }
        }
        std::vector<unsigned int> GetCellMapping(const BoundingBox* box) const;
        std::vector<unsigned int> GetCellMapping(const Object* object) const;

    };
}

#endif
/*  End of File */
