////////////////////////////////////////////////////////////////////////////////////
///
///  \file rastermap.cpp
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
#include "cartographer/rastermap.h"

using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RasterMap::RasterMap() : mSizeInPixels(0),
                         mPixelsPerMeter(0.5)
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RasterMap::~RasterMap()
{

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
///                    this threshold is ignored.
///                    Value is relative to the pose z position.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///                   Value is relative to the pose z position.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool RasterMap::ScanLocalRadiusInXY(const Pose& pose,
                                    const Point3D& point,
                                    const double radius,
                                    Point3D::List* collisions,
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
///   \brief This method is used to search for any objects/terrain values
///          for a segment relative to a pose.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] segment Segment scan area.
///   \param[out] collisions If not NULL, then a copy of all objects 
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///                    Value is relative to the pose z position.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///                   Value is relative to the pose z position.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool RasterMap::ScanLocalSegmentInXY(const Pose& pose,
                                     const Segment3D& segment,
                                     Point3D::List* collisions,
                                     const double zHigh, 
                                     const double zLow) const
{
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return ScanSegmentInXY(Pose::ConvertFromLocalToMapCoordinates(pose, segment), 
                           collisions, 
                           globalZHigh, 
                           globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects/terrain values
///          for a triangle relative to a pose.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[out] collisions If not NULL, then a copy of all objects 
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///                    Value is relative to the pose z position.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///                   Value is relative to the pose z position.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool RasterMap::ScanLocalTriangleInXY(const Pose& pose,
                                      const Point3D& p1,
                                      const Point3D& p2,
                                      const Point3D& p3,
                                      Point3D::List* collisions,
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
///   \brief Method to create a Terrain Map.
///
///   \param[in] mapSizeMeters Size of one side of the map in meters.  Maps are
///                            always square.
///   \param[in] cellSizeMeters How large the tiles/cells inside the map should
///                             be in meters.  Cells are always square.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool RasterMap::Create(const double mapSizeMeters,
                       const double cellSizeMeters)
{
    if(mapSizeMeters <= 0 || cellSizeMeters <= 0 || cellSizeMeters > mapSizeMeters) 
    {
        return false;
    }

    mMapSizeMeters = mapSizeMeters;
    mCellSizeMeters = cellSizeMeters;
    if(mCellSizeMeters > mMapSizeMeters)
    {
        mCellSizeMeters = mMapSizeMeters;
    }

    mNumCells = (unsigned int)(mapSizeMeters/cellSizeMeters);
    if(mNumCells == 0)
    {
        mNumCells = 1;
    }
    mSizeInPixels = (unsigned int)(mCellSizeMeters*mPixelsPerMeter*mCellSizeMeters*mPixelsPerMeter);
    if(mSizeInPixels == 0)
    {
        return false;
    }

    return true;
}


/*  End of File */
