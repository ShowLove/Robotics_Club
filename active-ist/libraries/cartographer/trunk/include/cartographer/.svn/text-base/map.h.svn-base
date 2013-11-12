////////////////////////////////////////////////////////////////////////////////////
///
///  \file map.h
///  \brief Contains the parent class for deriving any map structures used within
///  Cartographer.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 12 January 2009
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
#ifndef __LIB_CARTOGRAPHER_MAP__H
#define __LIB_CARTOGRAPHER_MAP__H

#include "cartographer/object.h"
#include <map>
#include <cxutils/filefinder.h>
namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Map 
    ///   \brief Super class for deriving Map structures within the Cartographer
    ///   Library.
    ///
    ///   All maps have common features within the libraries, so they must all
    ///   be based off of this structure.  It includes basic methods for searches
    ///   used for obstacle detection.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL Map
    {
    public:
        typedef std::map<std::string,Map*> Table;
        Map();
        virtual ~Map();
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Method to create a map.
        ///
        ///   All maps are squares and measured in meters.  
        ///
        ///   \param[in] mapSizeMeters How long the side of the square map should be in
        ///              meters.
        ///   \param[in] cellSizeMeters How big the cell size should be fore cells within
        ///              the map in meters.  Cells are used for culling extraneous objects
        ///              when searching and are squares measured in meters.
        ///
        ///   \return True on success, false on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool Create(const double mapSizeMeters, 
                            const double cellSizeMeters = 1.0) { return false; }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Destroys the map, deleting all data.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual void Destroy() {}
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Clears all data in map, does not delete map dimenions, etc.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual void Clear() {}
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Makes a copy of the map. 
        ///
        ///   \param[in] map The map to copy to.  Method should check if it can copy 
        ///   to the map.
        ///
        ///   \return True on success, false on failure.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool CopyTo(Map* map) const  { return false; };
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief This method is used to search for any objects around the radius
        ///          of a point in the map.
        ///
        ///   \param[in] point The position in the map to perform the scan at.
        ///   \param[in] radius The distance around the point in radians to look for
        ///                     objects.
        ///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
        ///                    this threshold is ignored.
        ///   \param[in] zLow The lower boundary in the z plan for finding objects.  
        ///                   Anything below this boundary is ignored in the search.
        ///
        ///   \return True if objects found in the area, false otherwise.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool IsCollisionInXY(const Point3D& point,
                                     const double radius,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const  { return false; }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief This method is used to search for any objects within a segment in the
        ///   map.
        ///
        ///   \param[in] segment The segment to search along for objects.
        ///   \param[in] collisions If not NULL, then a copy of all objects 
        ///              found within the search area is made to the list passed.
        ///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
        ///                    this threshold is ignored.
        ///   \param[in] zLow The lower boundary in the z plan for finding objects.  
        ///                   Anything below this boundary is ignored in the search.
        ///
        ///   \return True if objects found in the area, false otherwise.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool IsCollisionInXY(const Segment3D& segment,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief This method is used to search for any objects within a triangle 
        ///   defined by 3 points.
        ///
        ///   \param[in] p1 First point describing a triangle search area.
        ///   \param[in] p2 First point describing a triangle search area.
        ///   \param[in] p3 First point describing a triangle search area.
        ///              found within the search area is made to the list passed.
        ///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
        ///                    this threshold is ignored.
        ///   \param[in] zLow The lower boundary in the z plan for finding objects.  
        ///                   Anything below this boundary is ignored in the search.
        ///
        ///   \return True if objects found in the area, false otherwise.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool IsCollisionInXY(const Point3D& p1,
                                     const Point3D& p2,
                                     const Point3D& p3,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const { return false; }
        //  Scan relative to a pose.
        virtual bool IsCollisionInXY(const Pose& pose,
                                     const Point3D& point,
                                     const double radius,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        //  Scan relative to a pose.
        virtual bool IsCollisionInXY(const Pose& pose,
                                     const Segment3D& segment,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        //  Scan relative to a pose.
        virtual bool IsCollisionInXY(const Pose& pose,
                                     const Point3D& p1,
                                     const Point3D& p2,
                                     const Point3D& p3,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        // Is the point within the map?
        bool InBounds(const Point3D& p1)
        {
            return (p1.mX <= GetTopRightCorner().mX && p1.mX >= GetBottomLeftCorner().mX &&
                    p1.mY <= GetTopRightCorner().mY && p1.mY >= GetBottomLeftCorner().mY);
        }
        // Sets a mapping for conversion between WGS/UTM coordinates and map coordianat (maps to 0.0.0).
        void SetWorldOrigin(const Wgs& origin);
        Pose CreatePose(const Utm& position, const Point3D& rotation) const
        {
            Pose pose;
            pose.mTimeStamp.SetCurrentTime();
            pose.mPosition = ConvertFromGlobalToMapCoordinates(position);
            pose.mRotation = rotation;
            return pose;
        }
        Point3D ConvertFromGlobalToMapCoordinates(const Wgs& position) const;
        Point3D ConvertFromGlobalToMapCoordinates(const Utm& position) const;
        static Point3D ConvertFromGlobalToLocalMapCoordinates(const Utm& vehiclePosition, 
                                                              const double vehicleOrientation, 
                                                              const Utm& toConvert);
        Wgs ConvertFromMapToGlobalCoordinates(const Point3D& position) const;
        static Wgs ConvertFromMapToGlobalCoordinates(const Point3D& vehicleMapPosition, 
                                                     const Wgs& vehicleWorldPosition,
                                                     const double vehicleWorldOrientation,
                                                     const Point3D& pointInMapToConvert);
        static Utm ConvertFromMapToGlobalCoordinates(const Point3D& vehicleMapPosition, 
                                                     const Utm& vehicleWorldPosition,
                                                     const double vehicleWorldOrientation,
                                                     const Point3D& pointInMapToConvert);
        double GetMapSizeMeters() const;
        double GetCellSizeMeters() const;
        unsigned int GetMapWidthInCells() const;
        Point3D GetBottomLeftCorner() const;
        Point3D GetTopRightCorner() const;
        Wgs GetWorldOrigin() const { return mMapOrigin; }
        bool GetCellCorners(const unsigned int col, const unsigned int row, Point3D& bottomLeft, Point3D& topRight) const;
        bool SetResourceFile(const std::string& filename="settings/resource_paths.ini");
#ifdef USE_OPENCV
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Draws the object to the image.
        ///
        ///   \param[in] image Opencv image to draw to.
        ///   \param[in] pixelsPerMeter Pixels per meter conversion value.
        ///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
        ///   \param[in] setBackgroundColor Set a backgroundcolor.
        ///   \param[in] backgroundColor Color to set background to.
        ///
        ///   \return false.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual bool Draw(IplImage* image, 
                          const double pixelsPerMeter, 
                          const Point3D& mapBottomCorner, 
                          const bool setBackgroundColor = true, 
                          CvScalar backgroundColor = CV_RGB(0, 0, 0)) const { return false; }
        bool DrawCells(IplImage* image, const double pixelsPerMeter, const CvScalar color = CV_RGB(200, 200, 200), const bool displayCellNumber = true);
#endif
    protected:
        // Methods used to get cell information. (usually used in drawing methods).
        double GetCellLeft(unsigned int cellNumber);
        double GetCellRight(unsigned int cellNumber);
        double GetCellTop(unsigned int cellNumber);
        double GetCellBottom(unsigned int cellNumber);
        unsigned int GetCell(const Point3D& point);

        double mMapSizeMeters;              ///<  Size of map dimension (length of side, all maps are square) in meters.
        double mCellSizeMeters;             ///<  Size of cells inside the map (used for culling of objects in searches, etc).
        unsigned int mNumCells;             ///<  Size of one side of the grid (number of cells mMapSizeMeters/mCellSizeMeters).
        Wgs mMapOrigin;                     ///<  GPS Position of Map Origin.
        Utm mMapOriginUtm;                  ///<  Map origin in UTM format.
        CxUtils::FileFinder mFileFinder;    ///< Locates terrain files
        std::string mResourceFile;          ///< Location of file specifing resource directories
    };
}

#endif
/*  End of File */
