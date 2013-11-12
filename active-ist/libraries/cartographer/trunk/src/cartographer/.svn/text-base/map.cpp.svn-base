////////////////////////////////////////////////////////////////////////////////////
///
///  \file map.cpp
///  \brief Contains the parent class for deriving any map structures used within
///  Cartographer.
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
#include "cartographer/map.h"
#include <cstdio>

using namespace Cartographer;

#define EPSILON .00000001

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Map::Map()
{

    SetResourceFile();
    mFileFinder.SetRecursionFlag(false);
    mCellSizeMeters = 1;
    mMapSizeMeters = 1;
    mNumCells = 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
Map::~Map()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to scan an area relative to a position and
///          orientation in the map.  All points are relative to the pose.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] segment The segment to search along for objects.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::IsCollisionInXY(const Pose& pose,
                          const Point3D& point,
                          const double radius,
                          const double zHigh,
                          const double zLow) const
{
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return IsCollisionInXY(Pose::ConvertFromLocalToMapCoordinates(pose, point),
                           radius,
                           globalZHigh,
                           globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to scan an area relative to a position and
///          orientation in the map.  All points are relative to the pose.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] segment The segment to search along for objects.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::IsCollisionInXY(const Pose& pose,
                          const Segment3D& segment,
                          const double zHigh,
                          const double zLow) const
{
    Segment3D newsegment = Pose::ConvertFromLocalToMapCoordinates(pose, segment);
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return IsCollisionInXY(newsegment, globalZHigh, globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to scan an area relative to a position and
///          orientation in the map.  All points are relative to the pose.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::IsCollisionInXY(const Pose& pose,
                          const Point3D& p1,
                          const Point3D& p2,
                          const Point3D& p3,
                          const double zHigh,
                          const double zLow) const
{
    double globalZHigh = zHigh + pose.mPosition.mZ;
    double globalZLow = zLow + pose.mPosition.mZ;
    return IsCollisionInXY(Pose::ConvertFromLocalToMapCoordinates(pose, p1),
                           Pose::ConvertFromLocalToMapCoordinates(pose, p2),
                           Pose::ConvertFromLocalToMapCoordinates(pose, p3),
                           globalZHigh,
                           globalZLow);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets a GPS position to assign to the origin of the map.  This
///          value can be used to convert from world coordinates to map coordinates.
///
///   \param[in] origin World GPS origin that [0,0,0] on the map, maps to.
///
////////////////////////////////////////////////////////////////////////////////////
void Map::SetWorldOrigin(const Wgs& origin)
{
    mMapOrigin = origin;
    mMapOriginUtm << origin;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts WGS coordinates to map coordinates based on mMapOrigin.
///
///   \param position Position to convert to global map coordinates.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Map::ConvertFromGlobalToMapCoordinates(const Wgs& position) const
{
    return ConvertFromGlobalToMapCoordinates(Utm(position));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts UTM coordinates to map coordinates based on mMapOrigin.
///
///   \param position Position to convert to global map coordinates.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Map::ConvertFromGlobalToMapCoordinates(const Utm& position) const
{
    Point3D result;
    Utm utm(mMapOrigin);
    result.mX = position.mEasting - utm.mEasting;
    result.mY = position.mNorthing - utm.mNorthing;
    result.mZ = position.mElevation - utm.mElevation;
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts UTM coordinate relative to a vehicles UTM position and 
///          orientation to a point relative to the vehicle.
///
///   \param vehiclePosition Position of vehicle in world.
///   \param vehicleOrientation Yaw/heading of vehicle in real world.
///   \param toConvert World point to convert to map point.
///
///   \return The converted point.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Map::ConvertFromGlobalToLocalMapCoordinates(const Utm& vehiclePosition, 
                                                    const double vehicleOrientation, 
                                                    const Utm& toConvert)
{
    Point3D result;

    result.mX = toConvert.mEasting - vehiclePosition.mEasting;
    result.mY = toConvert.mNorthing - vehiclePosition.mNorthing;
    result.mZ = toConvert.mElevation - vehiclePosition.mElevation;
    if(fabs(vehicleOrientation) > 0.00001)
    {
        // Rotate the point.
        result = result.Rotate(vehicleOrientation, Point3D::Z);
    }
    
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts map coordinates to Wgs coordinates based on mMapOrigin.
///
///   \param position Position to convert to global coordinates.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs Map::ConvertFromMapToGlobalCoordinates(const Point3D& position) const
{
    Utm convert(mMapOriginUtm);
    
    convert.mNorthing += position.mY;
    convert.mEasting += position.mX;
    convert.mElevation += position.mZ;
    
    return Wgs(convert);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a point position in a map to a world location independent
///          of a map origin using a vehicles position and orientation.
///
///   \param vehicleMapPosition Position of vehicle in map coordinates.
///   \param vehicleWorldPosition Position of vehicle in the real world
///   \param vehicleWorldOrientaion Yaw of vehicle in the real world.
///   \param pointInMapToConvert The point to convert to real world location.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs Map::ConvertFromMapToGlobalCoordinates(const Point3D& vehicleMapPosition, 
                                           const Wgs& vehicleWorldPosition,
                                           const double vehicleWorldOrientation,
                                           const Point3D& pointInMapToConvert)
{
    Utm convert(vehicleWorldPosition);

    return Wgs(ConvertFromMapToGlobalCoordinates(vehicleMapPosition,
                                                 convert,
                                                 vehicleWorldOrientation,
                                                 pointInMapToConvert));
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts a point position in a map to a world location independent
///          of a map origin using a vehicles position and orientation.
///
///   \param vehicleMapPosition Position of vehicle in map coordinates.
///   \param vehicleWorldPosition Position of vehicle in the real world
///   \param vehicleWorldOrientaion Yaw of vehicle in the real world.
///   \param pointInMapToConvert The point to convert to real world location.
///
////////////////////////////////////////////////////////////////////////////////////
Utm Map::ConvertFromMapToGlobalCoordinates(const Point3D& vehicleMapPosition, 
                                           const Utm& vehicleWorldPosition,
                                           const double vehicleWorldOrientation,
                                           const Point3D& pointInMapToConvert)
{
    Utm convert(vehicleWorldPosition);
    Point3D localMapDistance(pointInMapToConvert.mX - vehicleMapPosition.mX,
                             pointInMapToConvert.mY - vehicleMapPosition.mY,
                             pointInMapToConvert.mZ - vehicleMapPosition.mZ);
    // Rotate the local point.
    Point3D rotated = localMapDistance.Rotate(-vehicleWorldOrientation, Point3D::Z);
    // Now translate to world.
    convert.mNorthing += rotated.mY;
    convert.mEasting += rotated.mX;
    convert.mElevation += rotated.mZ;
    
    return convert;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get map size in meters.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetMapSizeMeters() const
{
    return mMapSizeMeters;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get cell size in meters.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetCellSizeMeters() const
{
    return mCellSizeMeters;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get map width specified in cells.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Map::GetMapWidthInCells() const
{
    return mNumCells;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get bottom left corner of map as a point.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Map::GetBottomLeftCorner() const
{
    return Point3D(-mMapSizeMeters/2.0, -mMapSizeMeters/2.0, 0.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Get top right corner of map as a point.
///
////////////////////////////////////////////////////////////////////////////////////
Point3D Map::GetTopRightCorner() const
{
    return Point3D(mMapSizeMeters/2.0, mMapSizeMeters/2.0, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads a file containing directories for resources such as terrains and 
///          models.
///   
///   \param filename location and name of file containing resource directories.
///   
///   \return True if success.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::SetResourceFile(const std::string& filename)
{
    mResourceFile = filename;
    mFileFinder.LoadIniFile(filename);
    return true;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the bottom left corner and top right corner of the cell
///          in the map (in map coordinates).
///
///   \param[in] row Tile row number.
///   \param[in] col Tile column number.
///   \param[out] bottomLeft Bottom left corner in map coordinates.
///   \param[out] topRight Top right corner in map coordinates.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::GetCellCorners(const unsigned int col, const unsigned int row,
                         Point3D& bottomLeft,
                         Point3D& topRight) const
{
    if(mNumCells > 0 && row < mNumCells && col < mNumCells)
    {
        bottomLeft = GetBottomLeftCorner();
        topRight = GetTopRightCorner();

        // Calcualte world coordinates of bottom left and top right
        // tile corner values.
        bottomLeft.mX = bottomLeft.mX + col*mCellSizeMeters; // Left side.
        topRight.mX = bottomLeft.mX + mCellSizeMeters;       // Right side.

        bottomLeft.mY = bottomLeft.mY + row*mCellSizeMeters; // Bottom side.
        topRight.mY = bottomLeft.mY + mCellSizeMeters;       // Top side.  
        return true;
    }
    return false;
}


#ifdef USE_OPENCV
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the cells of the map to an opencv image.
///
///   \param[in] image Opencv image wished to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Position on the map we wish to treat as origin.
///   \param[in] color Color of cell lines.
///   \param[in] displayCellNumber  Draw cell numbers.
///
///   \return true on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Map::DrawCells(IplImage* image, const double pixelsPerMeter, const CvScalar color, const bool displayCellNumber)
{
    if(image == NULL) return false;
    if(pixelsPerMeter <= 0) return false;

    CvFont font;
    char charBuffer[64];
    cvInitFont(&font, CV_FONT_HERSHEY_PLAIN, 1.0, 1.0);

    for(int i=0; i<=(int)(mNumCells*mNumCells); i++)
    {
        cvRectangle(image,
                    ConvertToImagePoint(image->height, pixelsPerMeter, Point3D(GetCellLeft(i), GetCellTop(i), 0.0), GetBottomLeftCorner()),
                    ConvertToImagePoint(image->height, pixelsPerMeter, Point3D(GetCellRight(i), GetCellBottom(i), 0.0), GetBottomLeftCorner()),
                    color);
        if(displayCellNumber)
        {
            sprintf(charBuffer, "%d", i);
            cvPutText(image,
                      charBuffer,
                      ConvertToImagePoint(image->height, pixelsPerMeter, Point3D(GetCellLeft(i), GetCellBottom(i), 0.0), GetBottomLeftCorner()),
                      &font,
                      color);
        }
    }
    return true;
}

#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the X coordinate of the leftmost cell, looking at the XY plane.
///
///   \param[in] cellNumber index of cell top left corner 0, bottom right numcells.
///
///   \return X coordinate of the cell.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetCellLeft(unsigned int cellNumber)
{
    if(cellNumber > mNumCells*mNumCells) return 0.0;

    return (cellNumber%mNumCells)*mCellSizeMeters - mMapSizeMeters/2.0;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the X coordinate of the rightmost cell, looking at the XY plane.
///
///   \param[in] cellNumber index of cell top left corner 0, bottom right numcells.
///
///   \return X coordinate of the cell.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetCellRight(unsigned int cellNumber)
{
    return GetCellLeft(cellNumber) + mCellSizeMeters;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the Y coordinate of the topmost cell, looking at the XY plane.
///
///   \param[in] cellNumber index of cell top left corner 0, bottom right numcells.
///
///   \return Y coordinate of the cell.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetCellTop(unsigned int cellNumber)
{
    if(cellNumber > mNumCells*mNumCells) return 0.0;

    return mMapSizeMeters/2.0 - (cellNumber/mNumCells)*mCellSizeMeters;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the Y coordinate of the bottom cell, looking at the XY plane.
///
///   \param[in] cellNumber index of cell top left corner 0, bottom right numcells.
///
///   \return Y coordinate of the cell.
///
////////////////////////////////////////////////////////////////////////////////////
double Map::GetCellBottom(unsigned int cellNumber)
{
    return GetCellTop(cellNumber) - mCellSizeMeters;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the cell index that contains the point.
///
///   \param[in] Point in xy plane.
///
///   \return index of cell in mCells.
///
////////////////////////////////////////////////////////////////////////////////////
unsigned int Map::GetCell(const Point3D& point)
{
    int col = (int)((point.mX + mMapSizeMeters/2.0) / (mCellSizeMeters + EPSILON ));
    int row = (int)(mNumCells - ((point.mY + mMapSizeMeters/2.0) / (mCellSizeMeters + EPSILON )));
    return row*mNumCells + col;
}


/*  End of File */
