////////////////////////////////////////////////////////////////////////////////////
///
///  \file terrainmap.h
///  \brief Contains the class definition for TerrainMap which is used to
///  create maps describing terrain height or traversability information.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 6 September 2009
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
#ifndef __LIB_CARTOGRAPHER_TERRAIN_MAP__H
#define __LIB_CARTOGRAPHER_TERRAIN_MAP__H

#include <string>
#include "cartographer/rastermap.h"
#include "cartographer/object.h"

namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class TerrainMap
    ///   \brief Map class for storing Terrain height information.  Terrain height
    ///   data can be used to describe its traverseability for different types of
    ///   vehicles.  It includes the heights elevation point at any location in
    ///   the world.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL TerrainMap : public RasterMap
    {
    public:
        const static double MaxElevation;                           ///<  Maximum possible elevation value (meters).
        const static double MinElevation;                           ///<  Minimum possible elevation value (meters).
        const static double InvalidTerrainHeight;                   ///<  Constant for invalid terrain height data.
        class CARTOGRAPHER_DLL Tile
        {
        public:
            friend class TerrainMap;
            static const double InvalidTerrainHeight;               ///<  Constant for invalid terrain height data.
            typedef std::map<unsigned int, Tile*> Map;
            typedef std::map<unsigned int, std::string> Names;
            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \class Tile
            ///   \brief Structure for creating tiles to page in/out terrain height information
            ///          for extremely large maps used in Terrain databases.
            ///
            ///   This class is what actually stores terrain height data used by the
            ///   TerrainMap class.  It has methods to save/load data to/from files so that
            ///   they can be stored on disk and not consume large amounts of system
            ///   memory when using databases of large areas of the earth (like the 
            ///   29palms database used by DVTE).
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            Tile();
            Tile(const Tile& tile);
            ~Tile();
            bool Create(const Point3D& southWestCorner,
                        const Point3D& northEastCorner,
                        const double pixelsPerMeter,
                        const unsigned int row = 0,
                        const unsigned int col = 0,
                        const unsigned int id = 0);
            bool Save(const std::string& filename) const;
            bool SaveToImage(const std::string& filename) const;
            bool SaveToXml(const std::string& filename) const;
            bool Load(const std::string& filename, const bool fromImage = false);
            bool LoadFromImage(const std::string& filename);
            bool LoadFromXml(const std::string& filename);
            bool SetElevation(const Point3D& point);
            bool GetElevation(Point3D& point) const;
            double GetElevation(const double x, const double y) const;
            void Clear();
            inline unsigned int GetID() const { return mID; }
            bool operator==(const Tile& tile) const;
            Tile& operator=(const Tile& tile);
            void GetMaxAndMin(double& minElevation, double& maxElevation) const;
            Tile* Clone() const { return new Tile(*this); }
            bool ScanRadiusInXY(const Point3D& point,
                                const double radius,
                                Point3D::List* collisions = NULL,
                                const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            bool ScanSegmentInXY(const Segment3D& segment,
                                 Point3D::List* collisions = NULL,
                                 const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                 const double zLow = OBJECT_Z_LOW_DEFAULT) const;
            bool ScanTriangleInXY(const Point3D& p1,
                                  const Point3D& p2,
                                  const Point3D& p3,
                                  Point3D::List* collisions = NULL,
                                  const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                  const double zLow = OBJECT_Z_LOW_DEFAULT) const;
#ifdef USE_OPENCV 
            virtual bool Draw(IplImage* image,
                              const double pixelsPerMeter,
                              const Point3D& mapBottomCorner,
                              const double minElev,
                              const double maxElev,
                              CvScalar foregroundColor = CV_RGB(0, 255, 0),
                              CvScalar backgroundColor = CV_RGB(0, 127, 15)) const;
#endif
            inline void ConvertToPixels(const double x, const double y, int& col, int& row, const bool checkLimits = true) const;
            inline void ConvertFromPixels(const int col, const int row, double& x, double& y) const;
            inline void Touch() const;
            int* mpTile;                ///<  Terrain tile data (in millimeters).
            unsigned int mID;           ///<  Tile number/ID.
            unsigned int mRow;          ///<  Row location of tile in terrain map.
            unsigned int mCol;          ///<  Column location of tile in terrain map.
            double mPixelsPerMeter;     ///<  Pixels/meter scale for tile data.
            unsigned int mPixelHeight;  ///<  Pixel height (Y) of tile.
            unsigned int mPixelWidth;   ///<  Pixel width (X) of tile.
            unsigned int mSizeInPixels; ///<  Total size of tile in pixels.
            double mWidth;              ///<  Tile width (X) in meters.
            double mHeight;             ///<  Tile height (Y) in meters.
            Point3D mSouthWestCorner;   ///<  Location of southwest corner in map coordinates.
            Point3D mNorthEastCorner;   ///<  Location of northwest corner in map coordinates.
            double mMinElevation;       ///<  Minimum elevation value for the tile.
            double mMaxElevation;       ///<  Maximum elevation value for the tile.
            bool mMinElevationSetFlag;    ///< Set to true when a min or max elevation is set for first time.
            bool mMaxElevationSetFlag;     
        private:
            inline bool CheckLine(const int x0, 
                                  const int x1, 
                                  const int y, 
                                  Point3D::List* collisions,
                                  const double zHigh,
                                  const double zLow) const;
            CxUtils::Time mLastUseTime; ///<  The last time the tile was accessed.
        };
        TerrainMap();
        ~TerrainMap();
        bool Load(const std::string& file, const bool writeFlag = false, const bool imgFlag = false);
        bool LoadFromImage(const std::string& file, 
                           const double pixelsPerMeter,
                           const double cellSize = 100,
                           const double minElev = -10000, 
                           const double maxElev = 30000);
        bool SetSaveFile(const std::string& file);
        bool Save(const bool saveImage = false);
        bool SaveToImage(const std::string& filename) const;
        virtual bool Create(const double mapSizeMeters, 
                            const double cellSizeMeters,
                            const double pixelsPerMeter);
        virtual void Destroy();
        virtual void Clear();
        bool SetElevation(const Point3D& point);
        double GetElevation(const double x, const double y) const;
        bool GetElevation(const double x, const double y, double& e) const;
        double GetFilteredElevation(const double x, const double y) const;
        void GetGroundOrientation(const Point3D pos, 
                                  const double yaw,
                                  double& roll, 
                                  double& pitch, 
                                  unsigned int samples = 6) const;
        virtual bool AddObject(const Object* object) { return false; }
        virtual bool CopyTo(Map* map) const;
        virtual bool ScanRadiusInXY(const Point3D& point,
                                    const double radius,
                                    Point3D::List* collisions = NULL,
                                    const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                    const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanSegmentInXY(const Segment3D& segment,
                                     Point3D::List* collisions = NULL,
                                     const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                     const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual bool ScanTriangleInXY(const Point3D& p1,
                                      const Point3D& p2,
                                      const Point3D& p3,
                                      Point3D::List* collisions = NULL,
                                      const double zHigh = OBJECT_Z_HIGH_DEFAULT, 
                                      const double zLow = OBJECT_Z_LOW_DEFAULT) const;
        virtual double GetPixelsPerMeter() { return RasterMap::GetPixelsPerMeter(); }
        virtual double GetPixelsPerMeter(const unsigned int col, const unsigned int row) const;
        void SetPixelsPerMeter(const unsigned int cellNumber, const double pixelsPerMeter); 
        void SetPixelsPerMeter(const unsigned int col, const unsigned int row, const double pixelsPerMeter); 
        double GetMaxElevation() const { return mMaxElevation; }
        double GetMinElevation() const { return mMinElevation; }
        inline bool GetTileColRow(const Point3D& point, unsigned int& col, unsigned int& row) const
        {
            int c, r;
            r = (int)( (point.mY - GetBottomLeftCorner().mY)/mCellSizeMeters );
            c = (int)( (point.mX - GetBottomLeftCorner().mX)/mCellSizeMeters );
            if(r >= 0 && r < (int)mNumCells && c >= 0 && c < (int)mNumCells)
            {
                col = (unsigned int)c;
                row = (unsigned int)r;
                return true;
            }
            return false;
        }
#ifdef USE_OPENCV
        virtual bool Draw(IplImage* image, 
                          const double pixelsPerMeter, 
                          const Point3D& mapBottomCorner, 
                          const bool setBackgroundColor = true, 
                          CvScalar backgroundColor = CV_RGB(0, 76, 15)) const;
        virtual bool Draw(IplImage* image, 
                          const double pixelsPerMeter, 
                          const Point3D& mapBottomCorner, 
                          const Point3D::List& points,
                          CvScalar drawColor = CV_RGB(255, 0, 0)) const;
        virtual bool Draw(IplImage* image, 
                          const double pixelsPerMeter, 
                          const Point3D& mapBottomCorner, 
                          const Object::Collisions& collisions,
                          CvScalar drawColor = CV_RGB(255, 0, 0)) const;
        void SetTerrainDrawColor(CvScalar color) { mDrawColor = color; }
#endif
    private:
        Tile* GetTile(const unsigned int col,
                      const unsigned int row) const;
        /// Checks if the row/col is next to the current tile.
        inline bool IsNeighborTile(const unsigned int col, 
                                   const unsigned int row) const
        {
            if( mCol < mNumCells - 1 && row == mRow && col == mCol + 1)                             // To the right
                return true; 
            if( mCol > 0 && row == mRow && col == mCol - 1)                                         // To the left
                return true;
            if( mRow > 0 && row == mRow + 1 && col == mCol )                                        // Above
                return true;
            if( mRow < mNumCells - 1 && row == mRow - 1 && col == mCol )                            // Below
                return true;
            if( mRow > 0 && mCol < mNumCells - 1 && row == mRow - 1 && col == mCol + 1)             // Top right
                return true;
            if( mRow < mNumCells - 1 && mCol < mNumCells - 1 && row == mRow + 1 && col == mCol + 1) // Bottom right
                return true;
            if( mCol > 0 && mRow > 0 && row == mRow - 1 && col == mCol - 1)                         // Top left
                return true;
            if( mCol > 0 && mRow < mNumCells && row == mRow + 1 && col == mCol - 1)                 // Bottom left
                return true;

            return false;
        }
        bool mWriteFlag;                ///<  If true, map is saved to disk when needed.
        Tile* mpCurrentTile;            ///<  The current tile that checks are being done in.
        unsigned int mRow;              ///<  Current row number for tile.
        unsigned int mCol;              ///<  Current column for tile.
        Tile::Map mTiles;               ///<  Current tiles loaded into memory for terrain.
        Tile::Names mTileNames;         ///<  Name of all tiles.
        std::string mTerrainFileName;   ///< Master file name for terrain data.
        std::map<unsigned int, double> mPixelsPerMeterCells;    ///<  Alternative pixels/meter values for specific cells.
        double mMinElevation;           ///<  Minimum elevation in the terrain map.
        double mMaxElevation;           ///<  Maximum elevation in the terrain map.
        bool mImgFlag;                  ///< Load tile data from images/xml rather than compressed binary data.
#ifdef USE_OPENCV
        CvScalar mDrawColor;            ///<  Color to use for drawing terrain.
#endif
        Object::List mCollisions;       ///<  List of object collisions.
        bool mMinElevationSetFlag;      ///< Set to true when a min or max elevation is set for first time.
        bool mMaxElevationSetFlag;      ///< Set to true when a min or max elevation is set for first time.
        std::string mWorkingDirectory;   ///< Our working directory for saving and loading tiles
    };
}

#endif
/*  End of File */
