////////////////////////////////////////////////////////////////////////////////////
///
///  \file rastermap.h
///  \brief Class for defining maps that store data in pixel formats like for
///         terrain or collision probability or history of where the robot
///         has traveled.
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
#ifndef __LIB_CARTOGRAPHER_RASTER_MAP__H
#define __LIB_CARTOGRAPHER_RASTER_MAP__H

#include <string>
#include "cartographer/map.h"

#ifdef USE_OPENCV
#include "cartographer/object.h"
#endif

namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class RasterMap
    ///   \brief Map class for defining maps that store data as pixels within an array
    ///          instead of objects (image based maps).
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL RasterMap : public Map
    {
    public:
        RasterMap();
        ~RasterMap();
        virtual bool Create(const double mapSizeMeters, 
                            const double cellSizeMeters,
                            const double pixelsPerMeter) { return false; }
        virtual bool ScanRadiusInXY(const Point3D& point,
                                    const double radius,
                                    Point3D::List* collisions = NULL,
                                    const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                    const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        virtual bool ScanSegmentInXY(const Segment3D& segment,
                                     Point3D::List* collisions = NULL,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        virtual bool ScanTriangleInXY(const Point3D& p1,
                                      const Point3D& p2,
                                      const Point3D& p3,
                                      Point3D::List* collisions = NULL,
                                      const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                      const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        virtual bool ScanLocalRadiusInXY(const Pose& pose,
                                         const Point3D& point,
                                         const double radius,
                                         Point3D::List* collisions = NULL,
                                         const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                         const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanLocalSegmentInXY(const Pose& pose,
                                          const Segment3D& segment,
                                          Point3D::List* collisions = NULL,
                                          const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                          const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanLocalTriangleInXY(const Pose& pose,
                                           const Point3D& p1,
                                           const Point3D& p2,
                                           const Point3D& p3,
                                           Point3D::List* collisions = NULL,
                                           const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                           const double zLow = OBJECT_Z_LOW_DEFAULT) const;
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
        virtual double GetPixelsPerMeter() const { return mPixelsPerMeter; }
    protected:
        virtual bool Create(const double mapSizeMeters, 
                            const double cellSizeMeters);
        
        unsigned int mSizeInPixels;     ///<  Size of tiles in pixels.
        double mPixelsPerMeter;         ///<  Pixels per meter default value.
    };
}

#endif
/*  End of File */
