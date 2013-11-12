////////////////////////////////////////////////////////////////////////////////////
///
///  \file terrainmap.cpp
///  \brief Contains the class implementation for TerrainMap which is used to
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
#include "cartographer/maps/terrainmap.h"
#include "cartographer/objects/line.h"
#include <string.h>
#include <zlib.h>
#include <limits>
#include <cxutils/fileio.h>
#include <cxutils/images/image.h>
#include <tinyxml/tinyxml.h>


using namespace Cartographer;

const double TerrainMap::InvalidTerrainHeight = -99999.0;
const double TerrainMap::Tile::InvalidTerrainHeight = -99999.0;
const double TerrainMap::MaxElevation = 30000;
const double TerrainMap::MinElevation = -10000;

const double THRESH = 0.000001;    // Threshold used for determining if within tile region.
const double CONVERSION = 1000.0;  // Value to use to convert meters to integer.



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::Tile::Tile() :  mpTile(0),
                            mID(0),
                            mRow(0),
                            mCol(0),
                            mPixelsPerMeter(0.5),
                            mPixelHeight(0),
                            mPixelWidth(0),
                            mSizeInPixels(0),
                            mWidth(0.0),
                            mHeight(0.0),
                            mMinElevation(30000.0),
                            mMaxElevation(-10000.0),
                            mMinElevationSetFlag(false),
                            mMaxElevationSetFlag(false)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::Tile::Tile(const TerrainMap::Tile& tile) :  mpTile(0),
                                                        mID(0),
                                                        mRow(0),
                                                        mCol(0),
                                                        mPixelsPerMeter(0.5),
                                                        mPixelHeight(0),
                                                        mPixelWidth(0),
                                                        mSizeInPixels(0),
                                                        mWidth(0.0),
                                                        mHeight(0.0),
                                                        mMinElevation(30000.0),
                                                        mMaxElevation(-10000.0),
                                                        mMinElevationSetFlag(false),
                                                        mMaxElevationSetFlag(false)
{
    *this = tile;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::Tile::~Tile()
{
    if(mpTile)
    {
        delete[] mpTile;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Creates the terrain tile.
///
///  \param[in] southWestCorner South west corner of tile.
///  \param[in] northEastCorner North east corner of tile.
///  \param[in] pixelsPerMeter Resolution in pixels/meter for map data.
///  \param[in] row Row in larger map tiles.
///  \param[in] col Column value of the tile in the map.
///  \param[in] id The tile number/ID.
///
///  \return True if created, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::Create(const Point3D& southWestCorner,
                              const Point3D& northEastCorner,
                              const double pixelsPerMeter,
                              const unsigned int row,
                              const unsigned int col,
                              const unsigned int id)
{
    if( southWestCorner.mX < northEastCorner.mX && 
        southWestCorner.mY < northEastCorner.mY && 
        pixelsPerMeter > 0.0)
    {
        Clear();
        mSouthWestCorner = southWestCorner;
        mNorthEastCorner = northEastCorner;
        mPixelsPerMeter = pixelsPerMeter;
        mWidth = northEastCorner.mX - southWestCorner.mX;
        mHeight = northEastCorner.mY - southWestCorner.mY;
        mPixelWidth = (unsigned int)(mWidth*mPixelsPerMeter);
        mPixelHeight = (unsigned int)(mHeight*mPixelsPerMeter);            
        mSizeInPixels = mPixelWidth*mPixelHeight;
        mpTile = new int[mSizeInPixels];
        memset(mpTile, 0, sizeof(int)*mSizeInPixels);

        mID = id;
        mRow = row;
        mCol = col;
        mLastUseTime.SetCurrentTime();
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves tile information to an XML file.
///
///   \param[in] filename The name of the file to save to.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::SaveToXml(const std::string& filename) const
{
    //saving xml data
    TiXmlDocument doc;
    TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "yes");
    TiXmlElement* element = new TiXmlElement("Tile");
    TiXmlElement* child = NULL;
    TiXmlElement* child2 = NULL;
    char buffer[512];
    std::string fn = filename;
    fn.append(".xml");
    doc.LinkEndChild(decl);
    doc.LinkEndChild(element);

    element->SetAttribute("index", mID);

    child = new TiXmlElement("SouthWestCorner");
    sprintf(buffer, "%.2lf", mSouthWestCorner.mX);
    child->SetAttribute("x",buffer);
    sprintf(buffer, "%.2lf", mSouthWestCorner.mY);
    child->SetAttribute("y",buffer);
    element->LinkEndChild(child);

    child = new TiXmlElement("NorthEastCorner");
    sprintf(buffer, "%.2lf", mNorthEastCorner.mX);
    child->SetAttribute("x",buffer);
    sprintf(buffer, "%.2lf", mNorthEastCorner.mY);
    child->SetAttribute("y",buffer);
    element->LinkEndChild(child);

    child = new TiXmlElement("Row");
    sprintf(buffer, "%d", mRow);
    child->LinkEndChild(new TiXmlText(buffer));
    element->LinkEndChild(child);

    child = new TiXmlElement("Col");
    sprintf(buffer, "%d", mCol);
    child->LinkEndChild(new TiXmlText(buffer));
    element->LinkEndChild(child);

    child = new TiXmlElement("PixelsPerMeter");
    sprintf(buffer, "%.6lf", mPixelsPerMeter);
    child->LinkEndChild(new TiXmlText(buffer));
    element->LinkEndChild(child);

    child = new TiXmlElement("MinElevation");
    sprintf(buffer, "%.6lf", mMinElevation);
    child->LinkEndChild(new TiXmlText(buffer));
    element->LinkEndChild(child);

    child = new TiXmlElement("MaxElevation");
    sprintf(buffer, "%.6lf", mMaxElevation);
    child->LinkEndChild(new TiXmlText(buffer));
    element->LinkEndChild(child);

    doc.SaveFile(fn.c_str());
    return true;
}
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves tile information to a jpg file.
///
///   \param[in] filename The name of the file to save to.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::SaveToImage(const std::string& filename) const
{
    CxUtils::Image image;
    int result = 0;
    result = image.Create((unsigned short)(mWidth*mPixelsPerMeter),(unsigned short)(mHeight*mPixelsPerMeter),3);
    //char drawColor;
    double elevation = 0;
    double ebase = mMaxElevation - mMinElevation + CxUtils::CX_EPSILON;
    std::string fn = filename;
    fn.append(".jpg");

    //saving image data
    for(int col = 0; col < image.mWidth; col++)
    {
        for(int row = 0; row < image.mHeight; row++)
        {
            elevation = GetElevation(mSouthWestCorner.mX + row/mPixelsPerMeter,mNorthEastCorner.mY - col/mPixelsPerMeter);
            if(elevation <= InvalidTerrainHeight)
            {
                elevation = 0.0;
            }
            if(ebase != 0)
            {
                elevation -= mMinElevation;
                elevation /= ebase;
                elevation *= 765;
            }
            if(elevation > 765) elevation = 765;
            if(elevation < 0) elevation = 0;
            char chan = 0;
            image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 0] = 0;
            image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 1] = 0;
            image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 2] = 0;
            while(elevation > 255)
            {
                image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + chan] = 255;
                chan++;
                elevation -= 255;
            }
            image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + chan] = (unsigned char)elevation;
            //if(row == 0 || col == 0)
            //{
            //    printf("e=%lf row=%d col=%d\n",elevation,row,col);
            //}
        }
    }
    result += image.Save(fn);
    return (result > 0) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves tile information to a zlib compressed binary file.
///
///   \param[in] filename The name of the file to save to.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::Save(const std::string& filename) const
{
    bool result = false;
    gzFile file;
    
    if(mpTile == NULL)
        return result;

    file = gzopen(filename.c_str(), "wb");
    if(file)
    {
        char header[500];
        memset(header, 0, sizeof(char)*500);
        int min = (int)(mMinElevation*CONVERSION), max = (int)(mMaxElevation*CONVERSION);
        //printf("min %lf, max %lf\n",min,max);
        sprintf(header, "%.3lf,%d,%d,%d,%d,%d,%d,%d,%.6lf,%.6lf,%.6lf,%.6lf\n", mPixelsPerMeter,
                                                                          mPixelHeight,
                                                                          mPixelWidth,
                                                                          mRow,
                                                                          mCol,
                                                                          mID,
                                                                          min,
                                                                          max,
                                                                          mSouthWestCorner.mX,
                                                                          mSouthWestCorner.mY,
                                                                          mNorthEastCorner.mX,
                                                                          mNorthEastCorner.mY);
        gzwrite(file, header, (unsigned int)strlen(header));
        gzwrite(file, mpTile, sizeof(int)*mSizeInPixels);
         gzclose(file);
        result = true;
    }    

    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads an image file from disk, and converts it to tile
///          information.  Only PNG and JPEG supported.
///
///   \param[in] file The name of image to load.
///   \param[in] pixelsPerMeter Pixels per meter scale.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::LoadFromXml(const std::string& file)
{
    TiXmlDocument xml;
    std::string fn = file;
    fn.append(".xml");
    if(xml.LoadFile(fn.c_str()))
    {
        TiXmlHandle doc(&xml);
        TiXmlElement* root = doc.FirstChild("Tile").ToElement();
        if(root && root->Attribute("index"))
        {
            mID = atoi(root->Attribute("index"));
            TiXmlNode* node = NULL;
            node = doc.FirstChild("Tile").FirstChild("SouthWestCorner").ToNode();
            mSouthWestCorner.mX = atof(node->ToElement()->Attribute("x"));
            mSouthWestCorner.mY = atof(node->ToElement()->Attribute("y"));
            
            node = doc.FirstChild("Tile").FirstChild("NorthEastCorner").ToNode();
            mNorthEastCorner.mX = atof(node->ToElement()->Attribute("x"));
            mNorthEastCorner.mY = atof(node->ToElement()->Attribute("y"));

            node = doc.FirstChild("Tile").FirstChild("Row").ToNode();
            mRow = atoi(node->FirstChild()->Value());

            node = doc.FirstChild("Tile").FirstChild("Col").ToNode();
            mCol = atoi(node->FirstChild()->Value());

            node = doc.FirstChild("Tile").FirstChild("PixelsPerMeter").ToNode();
            mPixelsPerMeter = atoi(node->FirstChild()->Value());
            
            node = doc.FirstChild("Tile").FirstChild("MinElevation").ToNode();
            mMinElevation = atoi(node->FirstChild()->Value());
            
            node = doc.FirstChild("Tile").FirstChild("MaxElevation").ToNode();
            mMaxElevation = atoi(node->FirstChild()->Value());

            mWidth = mNorthEastCorner.mX - mSouthWestCorner.mX;
            mHeight = mNorthEastCorner.mY - mSouthWestCorner.mY;
            mPixelHeight = (unsigned int)(mHeight * mPixelsPerMeter);
            mPixelWidth = (unsigned int)(mWidth * mPixelsPerMeter);
            mSizeInPixels = mPixelHeight * mPixelWidth;
            return true;
        }
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads an image file from disk, and converts it to tile
///          information.  Only PNG and JPEG supported. Should only called after
///          tile has been loaded from XML file.
///
///   \param[in] file The name of image to load.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::LoadFromImage(const std::string& file)
{
    CxUtils::Image image;
    std::string fn = file;
    fn.append(".jpg");
    if(mpTile == NULL)
    {
        mpTile = new int[mSizeInPixels];
    }
    if(mPixelsPerMeter > 0.0 && image.Load(fn))
    {
        if(image.mWidth != image.mHeight)
            return false;
        double x, y, e;
        for(int col = 0; col < image.mWidth; col++)
        {
            for(int row = 0; row < image.mHeight; row++)
            {
                //if(mPixelsPerMeter >= 1.0)
                //{
                //    y = mNorthEastCorner.mY - col*mPixelsPerMeter;
                //    x = mSouthWestCorner.mX + row*mPixelsPerMeter;
                //}
                //else
                //{
                y = mNorthEastCorner.mY - col/mPixelsPerMeter;
                x = mSouthWestCorner.mX + row/mPixelsPerMeter;
                //}
                double color;
                double maxcolor = 255;
                if(image.mChannels == 3)
                {
                    color = (image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 0] + 
                             image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 1] +
                             image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels + 2]); 
                    maxcolor *= 3;
                }
                else
                {
                    color = image.mpImage[col*image.mWidth*image.mChannels + row*image.mChannels];
                    maxcolor *= image.mChannels;
                }
                
                e = mMinElevation + color*(mMaxElevation - mMinElevation)/maxcolor;
                SetElevation(Point3D(x, y, e));
            }
        }
        return true;
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///  \brief Loads a saved tile.
///
///  \param filename File name of the tile to load.
///
///  \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::Load(const std::string& filename, const bool fromImage)
{
   // Delete previous values.
    if(mpTile)
    {
        delete[] mpTile;
    }

    if(fromImage)
    {
        return (LoadFromXml(filename) && LoadFromImage(filename));
    }
    bool result = false;
    unsigned int size = 0;
    gzFile file;   

 
    mpTile = NULL;
    mPixelsPerMeter = 1;
    mPixelHeight = mPixelWidth = mSizeInPixels = 0;
    
    file = gzopen(filename.c_str(), "rb");
    if(file)
    {
        std::string header;
        char ch = 0;
        while( ch != '\n' && !gzeof(file) )
        {
            ch = (char )gzgetc(file);
            header += ch;
        }
        int max = 0, min = 0;
        if( ch == '\n')
        {
            sscanf(header.c_str(), "%lf,%d,%d,%d,%d,%d,%d,%d,%lf,%lf,%lf,%lf,\n", &mPixelsPerMeter,
                                                                            &mPixelHeight,
                                                                            &mPixelWidth,
                                                                            &mRow,
                                                                            &mCol,
                                                                            &mID,
                                                                            &min,
                                                                            &max,
                                                                            &mSouthWestCorner.mX,
                                                                            &mSouthWestCorner.mY,
                                                                            &mNorthEastCorner.mX,
                                                                            &mNorthEastCorner.mY);
            mSizeInPixels = mPixelWidth*mPixelHeight;
            mWidth = mNorthEastCorner.mX - mSouthWestCorner.mX;
            mHeight = mNorthEastCorner.mY - mSouthWestCorner.mY;
            mMinElevation = min/CONVERSION;
            mMaxElevation = max/CONVERSION;
            if(mSizeInPixels > 0 && mPixelsPerMeter > 0.0)
            {
                mpTile = new int[mSizeInPixels];
                if( gzread(file, mpTile, mSizeInPixels*sizeof(int)) == mSizeInPixels*sizeof(int) )
                {
                    result = true;
                    /*std::vector<std::string> tokens;
                    tokens = CxUtils::FileIO::Tokenize(filename, ".");
                    if(tokens.size() >= 5)
                    {
                        mMinElevation = atoi(tokens[2].c_str())/CONVERSION;
                        mMaxElevation = atoi(tokens[3].c_str())/CONVERSION;
                    }
                    else
                    {
                        mMaxElevation = -10000*CONVERSION;
                        mMinElevation = 30000*CONVERSION;
                        int* ptr = mpTile;
                        for(unsigned int i = 0; i < mSizeInPixels; i++)
                        {
                            if(*ptr > mMaxElevation)
                            {
                                mMaxElevation = *ptr;
                            }
                            if(*ptr < mMinElevation)
                            {
                                mMinElevation = *ptr;
                            }
                            ptr++;
                        }
                        mMaxElevation /= CONVERSION;
                        mMinElevation /= CONVERSION;
                    }*/
                    mLastUseTime.SetCurrentTime();
                }
            }
        }

        gzclose(file);
    }    
    if(result == false)
        Clear();

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all data.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Tile::Clear()
{
    if(mpTile)
    {
        delete[] mpTile;
    }
    mpTile = 0;
    mID = 0;
    mRow = mCol = 0;
    mPixelsPerMeter = 0;
    mPixelHeight = mPixelWidth = mSizeInPixels = 0;
    mWidth = mHeight = 0;
    mSouthWestCorner(0, 0, 0);
    mNorthEastCorner(0, 0, 0);    
    mMinElevation = mMaxElevation = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the elevation at a point in the world.
///
///   \param[in] point A position and elevation value to save.  Values are in
///                    meters.
///
///   \return True on success, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::SetElevation(const Point3D& point)
{
    if(mpTile &&
       point.mX + THRESH >= mSouthWestCorner.mX &&
       point.mX <= mNorthEastCorner.mX + THRESH &&
       point.mY + THRESH >= mSouthWestCorner.mY &&
       point.mY <= mNorthEastCorner.mY + THRESH)
    {
        int row = 0, col = 0;
        ConvertToPixels(point.mX, point.mY, col, row);        

        if(row >= 0 && row < (int)mPixelHeight && col >= 0 && col < (int)mPixelWidth)
        {
            unsigned int pos = row*mPixelWidth + col;
            mpTile[pos] = (int)(point.mZ*CONVERSION); // Convert to integer.
            if(point.mZ > mMaxElevation || !mMaxElevationSetFlag)
            {
                mMaxElevation = point.mZ;
                mMaxElevationSetFlag = true;
            }
            if(point.mZ < mMinElevation || !mMinElevationSetFlag)
            {
                mMinElevation = point.mZ;
                mMinElevationSetFlag = true;
            }
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the elevation at a point in the world.
///
///   \param[out] point A point containing X,Y data to get elevation at. Elevation
///                     is saved to point.
///
///   \return True on success, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::GetElevation(Point3D& point) const
{
    if(mpTile //&&
       //point.mX + THRESH >= mSouthWestCorner.mX &&
       //point.mX <= mNorthEastCorner.mX + THRESH &&
       //point.mY + THRESH >= mSouthWestCorner.mY &&
       //point.mY <= mNorthEastCorner.mY + THRESH &&
       //point.mX >= mSouthWestCorner.mX &&
       //point.mX <= mNorthEastCorner.mX &&
       //point.mY >= mSouthWestCorner.mY &&
       //point.mY <= mNorthEastCorner.mY
       )
    {
        int row = 0, col = 0;
        ConvertToPixels(point.mX, point.mY, col, row, true);

        //if(row > 1 && row < ((int)mPixelHeight) - 2 && col > 1 && col < ((int)mPixelWidth) - 2)
        if(row >= 0 && row < ((int)mPixelHeight) && col >= 0 && col < ((int)mPixelWidth))
        {
            unsigned int pos = row*mPixelWidth + col;
            point.mZ = mpTile[pos]/CONVERSION; // Convert from integer.
            return true;
        }
        else
        {
            return false;
        }
    }
    //std::cout << "Point in tile not found" << std::endl;
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the elevation at a point in the world.
///
///   \param[in] x X position in map to get height at.
///   \param[in] y Y position in map to get height at.
///
///   \return Terrain elevation at position if within tile, -999
///
////////////////////////////////////////////////////////////////////////////////////
double TerrainMap::Tile::GetElevation(const double x, const double y) const
{
    Point3D p(x, y, 0);
    if(GetElevation(p))
    {
        return p.mZ;
    }
    return InvalidTerrainHeight;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Compares tile data to see if it is equal.
///
///   \return True if equal, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::operator==(const TerrainMap::Tile& tile) const
{
    if( (unsigned int)(mPixelsPerMeter*1000) == (unsigned int)(tile.mPixelsPerMeter*1000) &&
        mPixelHeight == tile.mPixelHeight &&
        mPixelWidth == tile.mPixelWidth &&
        Point3D::Distance(mSouthWestCorner, tile.mSouthWestCorner) <= 1 &&
        Point3D::Distance(mNorthEastCorner, tile.mNorthEastCorner) <= 1 &&
        mID == tile.mID &&
        mRow == tile.mRow &&
        mCol == tile.mCol)
    {
        if( mpTile && tile.mpTile)
        {
            if(memcmp(mpTile, tile.mpTile, mSizeInPixels*sizeof(int)) == 0)
            {
                return true;
            }
        }
        else if(!mpTile && !tile.mpTile)
        {
            return true;
        }
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::Tile& TerrainMap::Tile::operator =(const TerrainMap::Tile& tile)
{
    if(this != &tile)
    {
        if(tile.mpTile)
        {
            if(mpTile && mSizeInPixels != tile.mSizeInPixels)
            {
                delete[] mpTile;
                mpTile = NULL;
            }
            if(!mpTile)
            {
                mpTile = new int[tile.mSizeInPixels];
            }
            memcpy(mpTile, tile.mpTile, tile.mSizeInPixels*sizeof(int));
        }
        else
        {
            if(mpTile)
            {
                delete[] mpTile;
            }
            mpTile = NULL;
        }
        mID = tile.mID;
        mRow = tile.mRow;
        mCol = tile.mCol;
        mPixelsPerMeter = tile.mPixelsPerMeter;
        mPixelHeight = tile.mPixelHeight;
        mPixelWidth = tile.mPixelWidth;
        mSizeInPixels = tile.mSizeInPixels;
        mWidth = tile.mWidth;
        mHeight = tile.mHeight;
        mSouthWestCorner = tile.mSouthWestCorner;
        mNorthEastCorner = tile.mNorthEastCorner;
        mMinElevation = tile.mMinElevation;
        mMaxElevation = tile.mMaxElevation;
    }
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the maximum and minimum elevation values within the
///          terrain tile.
///
///   \param[out] minElevation Minimum elevation found.
///   \param[out] maxElevation Maximum elevation found.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Tile::GetMaxAndMin(double& minElevation, double& maxElevation) const
{
    minElevation = mMinElevation;
    maxElevation = mMaxElevation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Scans the radius in the tile, and if any elevation values are
///          within the zLow, zHigh boundaries, a collision is detected.
///
///   \param[in] point The location in the map to scan.
///   \param[in] radius How far around the point in meters to scan.
///   \param[in] collisions Collision values.
///   \param[in] zHigh The elevation upper limit to check for.
///   \param[in] zLow The elevation lower limit to check within.
///
///   \return True if any values of elevation are within the zHigh, zLow 
///           boundaries around the point, otherwise false (clear).
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::ScanRadiusInXY(const Point3D& point,
                                      const double radius,
                                      Point3D::List* collisions,
                                      const double zHigh,
                                      const double zLow) const
{
    bool result = false;
    
    if(mpTile == NULL)
        return result;

    double minX, maxX;
    double minY, maxY;

    if(collisions)
    {
        minX = point.mX - radius;
        maxX = point.mX + radius;
        minY = point.mY - radius;
        maxY = point.mY + radius;
    }

    // First convert from world coordinates to
    // cell col/row coordinates (pixels).
    int row = 0, col = 0;
    int pradius = 1;

    ConvertToPixels(point.mX, point.mY, col, row, false);
    if(mPixelsPerMeter >= 1.0)
    {
        pradius = (int)(radius*mPixelsPerMeter);
    }
    else
    {
        pradius = (int)(radius/mPixelsPerMeter);
    }

    if(pradius <= 0)
        pradius = 1;

    // This does a radial scan the same way you would
    // draw a circle into an image.
    Point3D px;
    double rr = pradius*pradius;

    for(int c = -pradius; c <= pradius && (result == false || collisions); c++)
    {
        //if(col + c > 0 && col + c < (int)(mPixelWidth - 1)) // Don't go to extreme edges.
        if(col + c >= 0 && col + c < (int)(mPixelWidth))
        {
            int height = (int)sqrt(rr - c*c);
            for(int r = -height; r < height && (result == false || collisions); r++)
            {
                //if(row + r > 0 && row + r < (int)(mPixelHeight - 1)) // Don't go to extreme edges.
                if(row + r >= 0 && row + r < (int)(mPixelHeight))
                {
                    px.mZ = mpTile[(row + r)*mPixelWidth + (col + c)]/CONVERSION;
                    if(px.mZ >= zLow)
                    { 
                        result = true; 
                        if(collisions)
                        {                            
                            ConvertFromPixels(col + c, row + r, px.mX, px.mY);
                            if(px.mX > maxX) {px.mX = maxX; }
                            if(px.mX < minX) {px.mX = minX; }
                            if(px.mY > maxY) {px.mY = maxY; }
                            if(px.mY < minY) {px.mY = minY; }
                            collisions->push_back(px);
                        }
                        else
                        {
                            break; 
                        }
                    }
                }
            }
        }
    }   
   
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Scans the segment area for terrain elevation values within the
///          boundaries provided.
///
///   \param[in] segment Area in map to scan.
///   \param[in] collisions Collision values.
///   \param[in] zHigh The elevation upper limit to check for.
///   \param[in] zLow The elevation lower limit to check within.
///
///   \return True if any values of elevation are within the zHigh, zLow 
///           boundaries around the point, otherwise false (clear).
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::ScanSegmentInXY(const Segment3D& segment,
                                       Point3D::List* collisions,
                                       const double zHigh,
                                       const double zLow) const
{
    bool result = false;
    int dr;
    int dc;
    int stepC, stepR;
    int fraction;
    int i;

    double rad;

    // Make sure we have tile data.
    if(mpTile == NULL)
    {
        return result;
    }

    int height = (int)mPixelHeight;
    int width = (int)mPixelWidth;

    int pwidth;
    if(mPixelsPerMeter >= 1.0)
    {
        pwidth = (int)(segment.mWidth*mPixelsPerMeter);
    }
    else
    {
        pwidth = (int)(segment.mWidth/mPixelsPerMeter);
    }

    if(pwidth <= 0)
        pwidth = 1;

    int r0, c0, r1, c1; // Pixel coordinates of the segment endpoints.

    ConvertToPixels(segment.mPoint1.mX, segment.mPoint1.mY, c0, r0, false);
    ConvertToPixels(segment.mPoint2.mX, segment.mPoint2.mY, c1, r1, false);

    dr = r1 - r0;
    dc = c1 - c0;

    if (dr < 0)
    {
        dr = -dr;
        stepR = -1;
    } 
    else
    { 
        stepR = 1;
    }

    if (dc < 0)
    {
        dc = -dc; 
        stepC = -1;
    }
    else
    {
        stepC = 1;
    }

    dr <<= 1;
    dc <<= 1;

    rad = width/2.0;

    // Check area around endpoints of the segment.  This is skipped because the Terrain Map
    // can do it, saving time.
    /*result |= ScanRadiusInXY(segment.mPoint1, segment.mWidth/2.0, collisions, zHigh, zLow);
    if(result && collisions == NULL)
    {
        return result;
    }
    result |= ScanRadiusInXY(segment.mPoint2, segment.mWidth/2.0, collisions, zHigh, zLow);
    if(result && collisions == NULL)
    {
        return result;
    }*/

    Point3D px;

    if (dc > dr)
    {
        pwidth = pwidth + ((int)(pwidth/cos(atan(fabs(((double)r1 - r0) / ((double)c1 - c0))))) - pwidth) / 2;
        //if (c0 > 0 && r0 > 0 && c0 < width - 1 && r0 < height - 1)
        if (c0 >= 0 && r0 >= 0 && c0 < width && r0 < height)
        {
            px.mZ = mpTile[r0*width + c0]/CONVERSION;
            if(px.mZ >= zLow)
            { 
                result = true; 
                if(collisions)
                {                            
                    ConvertFromPixels(c0, r0, px.mX, px.mY);
                    collisions->push_back(px);
                }
                else
                {
                    return result;
                }
            }
        }

        for (i = -pwidth/2; i < pwidth/2; i++)
        {
            //if (c0 > 0 && r0 + i > 0 && c0 < width - 1 && r0 + i < height - 1)
            if (c0 >= 0 && r0 + i >= 0 && c0 < width && r0 + i < height)
            {
                px.mZ = mpTile[(r0 + i)*width + c0]/CONVERSION;
                if(px.mZ >= zLow)
                { 
                    result = true; 
                    if(collisions)
                    {                            
                        ConvertFromPixels(c0, r0 + i, px.mX, px.mY);
                        collisions->push_back(px);
                    }
                    else
                    {
                        return result;
                    }
                }
            }
        }

        fraction = dr - (dc >> 1);
        while (c0 != c1)
        {
            if (fraction >= 0)
            {
                r0 += stepR;
                fraction -= dc;
            }
            c0 += stepC;
            fraction += dr;

            //if (c0 > 0 && r0 > 0 && c0 < width - 1 && r0 < height - 1)
            if (c0 >= 0 && r0 >= 0 && c0 < width && r0 < height)
            {
                px.mZ = mpTile[r0*width + c0]/CONVERSION;
                if(px.mZ >= zLow)
                { 
                    result = true; 
                    if(collisions)
                    {                            
                        ConvertFromPixels(c0, r0, px.mX, px.mY);
                        collisions->push_back(px);
                    }
                    else
                    {
                        return result;
                    }
                }
            }

            for (i = -pwidth/2; i < pwidth/2; i++)
            {
                //if (c0 > 0 && r0 + i > 0 && c0 < width - 1 && r0 + i < height - 1)
                if (c0 >= 0 && r0 + i >= 0 && c0 < width && r0 + i < height)
                {
                    px.mZ = mpTile[(r0 + i)*width + c0]/CONVERSION;
                    if(px.mZ >= zLow)
                    { 
                        result = true; 
                        if(collisions)
                        {                            
                            ConvertFromPixels(c0, r0 + i, px.mX, px.mY);
                            collisions->push_back(px);
                        }
                        else
                        {
                            return result;
                        }
                    }
                }
            }
        }
    } 
    else
    {
        pwidth = pwidth + ((int)(pwidth/sin(atan(fabs(((double)r1 - r0) / ((double)c1 - c0))))) - pwidth) / 2;
        //if (c0 > 0 && r0 > 0 && c0 < width - 1 && r0 < height - 1)
        if (c0 >= 0 && r0 >= 0 && c0 < width && r0 < height)
        {
            px.mZ = mpTile[r0*width + c0]/CONVERSION;
            if(px.mZ >= zLow)
            { 
                result = true; 
                if(collisions)
                {                            
                    ConvertFromPixels(c0, r0, px.mX, px.mY);
                    collisions->push_back(px);
                }
                else
                {
                    return result;
                }
            }
        }

        for (i = -pwidth/2; i < pwidth/2; i++)
        {    
            //if (c0 + i > 0 && r0 > 0 && c0 + i < width - 1 && r0 < height - 1)
            if (c0 + i >= 0 && r0 >= 0 && c0 + i < width && r0 < height)
            {
                px.mZ = mpTile[r0*width + c0 + i]/CONVERSION;
                if(px.mZ >= zLow)
                { 
                    result = true; 
                    if(collisions)
                    {                            
                        ConvertFromPixels(c0 + i, r0, px.mX, px.mY);
                        collisions->push_back(px);
                    }
                    else
                    {
                        return result;
                    }
                }
            }
        }
        fraction = dc - (dr >> 1);
        while (r0 != r1)
        {
            if (fraction >= 0)
            {
                c0 += stepC;
                fraction -= dr;
            }
            r0 += stepR;
            fraction += dc;
            
            //if (c0 > 0 && r0 > 0 && c0 < width - 1 && r0 < height - 1)
            if (c0 >= 0 && r0 >= 0 && c0 < width && r0 < height)
            {
                px.mZ = mpTile[r0*width + c0]/CONVERSION;
                if(px.mZ >= zLow)
                { 
                    result = true; 
                    if(collisions)
                    {                            
                        ConvertFromPixels(c0, r0, px.mX, px.mY);
                        collisions->push_back(px);
                    }
                    else
                    {
                        return result;
                    }
                }
            }

            for (i = -pwidth/2; i < pwidth/2; i++)
            {    
                //if (c0 + i > 0 && r0 > 0 && c0 + i < width - 1 && r0 < height - 1)
                if (c0 + i >= 0 && r0 >= 0 && c0 + i < width && r0 < height)
                {
                    px.mZ = mpTile[r0*width + c0 + i]/CONVERSION;
                    if(px.mZ >= zLow)
                    { 
                        result = true; 
                        if(collisions)
                        {                            
                            ConvertFromPixels(c0 + i, r0, px.mX, px.mY);
                            collisions->push_back(px);
                        }
                        else
                        {
                            return result;
                        }
                    }
                }
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Scans the area for terrain elevation values within the
///          boundaries provided.
///
///   \param[in] p1 Point making up a triangle.
///   \param[in] p2 Point making up a triangle.
///   \param[in] p3 Point making up a triangle.
///   \param[in] collisions Collision values.
///   \param[in] zHigh The elevation upper limit to check for.
///   \param[in] zLow The elevation lower limit to check within.
///
///   \return True if any values of elevation are within the zHigh, zLow 
///           boundaries around the point, otherwise false (clear).
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::ScanTriangleInXY(const Point3D& p1,
                                        const Point3D& p2,
                                        const Point3D& p3,
                                        Point3D::List* collisions,
                                        const double zHigh,
                                        const double zLow) const
{
    bool result = false;
    
    if(mpTile == NULL)
    {
        return result;
    }

    Point3D a, b, c;
    
    int col, row;
    ConvertToPixels(p1.mX, p1.mY, col, row, false);
    a.mX = col; a.mY = row;
    ConvertToPixels(p2.mX, p2.mY, col, row, false);
    b.mX = col; b.mY = row;
    ConvertToPixels(p3.mX, p3.mY, col, row, false);
    c.mX = col; c.mY = row;

    double dx1, dx2, dx3;
    if(b.mY - a.mY > 0) { dx1 = (b.mX - a.mX)/(b.mY - a.mY); } else { dx1 = b.mX - a.mX; }
    if(c.mY - a.mY > 0) { dx2 = (c.mX - a.mX)/(c.mY - a.mY); } else { dx2 = 0; }
    if(c.mY - b.mY > 0) { dx3 = (c.mX - b.mX)/(c.mY - b.mY); } else { dx3 = 0; }

    Point3D s(a), e(a);

    if(dx1 > dx2)
    {
        for(; s.mY <= b.mY; s.mY++, e.mY++, s.mX += dx2, e.mX += dx1)
        {
            result |= CheckLine((int)s.mX, (int)e.mX, (int)s.mY, collisions, zHigh, zLow);
            if(collisions == NULL && result) { return result; }
        }
        e = b;
        for(; s.mY <= c.mY; s.mY++, e.mY++, s.mX += dx2, e.mX += dx3)
        {
            result |= CheckLine((int)s.mX, (int)e.mX, (int)s.mY, collisions, zHigh, zLow);
            if(collisions == NULL && result) { return result; }
        }
    }
    else
    {
        for(; s.mY <= b.mY; s.mY++, e.mY++, s.mX += dx1, e.mX += dx2)
        {
            result |= CheckLine((int)s.mX, (int)e.mX, (int)s.mY, collisions, zHigh, zLow);
            if(collisions == NULL && result) { return result; }
        }
        s = b;
        for(; s.mY <= c.mY; s.mY++, e.mY++, s.mX += dx3, e.mX += dx2)
        {
            result |= CheckLine((int)s.mX, (int)e.mX, (int)s.mY, collisions, zHigh, zLow);
            if(collisions == NULL && result) { return result; }
        }
    }

    return result;
}


#ifdef USE_OPENCV

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the tile data into the map.
///
///   \param[out] image Image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter for conversion to image coords.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] minElev Minimum elevation of terrain map to scale from elevation
///                      to color channel percentage.
///   \param[in] maxElev Maximum elevation of terrain map to scale from elevation
///                      to color channel percentage.
///   \param[in] foregroundColor Color to draw foreground with.
///   \param[in] backgroundColor Color to draw background with.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::Draw(IplImage* image,
                            const double pixelsPerMeter,
                            const Point3D& mapBottomCorner,
                            const double minElev,
                            const double maxElev,
                            CvScalar foregroundColor,
                            CvScalar backgroundColor) const
{
    bool result = false;
    CvScalar drawColor;
    double ebase = maxElev - minElev + CxUtils::CX_EPSILON;
    double dist = 1.0/(pixelsPerMeter + CxUtils::CX_EPSILON);

    for(double x = mSouthWestCorner.mX; x <= mNorthEastCorner.mX; x += dist)
    {
        for(double y = mSouthWestCorner.mY; y <= mNorthEastCorner.mY; y += dist)
        {
            double elevation = GetElevation(x, y);
            if(elevation <= InvalidTerrainHeight)
            {
                elevation = 0.0;
            }
            CvPoint px = ConvertToImagePoint(image->height,
                                             pixelsPerMeter,
                                             CxUtils::Point3D(x, y, elevation),
                                             mapBottomCorner,
                                             true);
            elevation -= minElev;
            drawColor.val[0] = backgroundColor.val[0] + foregroundColor.val[0]*elevation/ebase;
            drawColor.val[1] = backgroundColor.val[1] + foregroundColor.val[1]*elevation/ebase;
            drawColor.val[2] = backgroundColor.val[2] + foregroundColor.val[2]*elevation/ebase;
            drawColor.val[3] = 0;

            if(drawColor.val[0] > 255)
                drawColor.val[0] = 255;
            if(drawColor.val[1] > 255)
                drawColor.val[1] = 255;
            if(drawColor.val[2] > 255)
                drawColor.val[2] = 255;

            if(px.x >= 0 && px.x < image->width && px.y >= 0 && px.y < image->height)
            {
                cvSet2D(image, px.y, px.x, drawColor);
                result = true;
            }
        }
    }

    return result;
}

#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts from world coordinates (map) to tile pixel coordinates.
///
///   \param[in] x X position in map (meters).
///   \param[in] y Y position in map (meters).
///   \param[out] col The pixel column in the tile.
///   \param[out] row The pixel row in the tile.
///   \param[in] checkLimits If true, than the row/col value produced
///                          will not go out of bounds of tile (clipped).
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Tile::ConvertToPixels(const double x, const double y, int& col, int& row, const bool checkLimits) const
{
    row = col = 0;
    if(mPixelsPerMeter >= 1.0)
    {
        row = (int)( (y - mSouthWestCorner.mY)*mPixelsPerMeter );
        col = (int)( (x - mSouthWestCorner.mX)*mPixelsPerMeter );
    }
    else
    {
        row = (int)( (y - mSouthWestCorner.mY)/mPixelsPerMeter );
        col = (int)( (x - mSouthWestCorner.mX)/mPixelsPerMeter );
    }
    if(checkLimits)
    {
        if(row < 0) { row = 0; }
        if(row >= (int)mPixelHeight) { row = (int)(mPixelHeight - 1); }
        if(col < 0) { col = 0; }
        if(col >= (int)mPixelWidth) { col = (int)(mPixelWidth - 1); }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts from tile pixel coordiantes to world coordinates (map).
///
///   \param[in] col The pixel column in the tile.
///   \param[in] row The pixel row in the tile.
///   \param[out] x X position in map (meters).
///   \param[out] y Y position in map (meters).
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Tile::ConvertFromPixels(const int col, const int row, double& x, double& y) const
{
    x = y = 0;
    if(mPixelsPerMeter >= 1.0)
    {
        y = (row/mPixelsPerMeter) + mSouthWestCorner.mY;
        x = (col/mPixelsPerMeter) + mSouthWestCorner.mX;
    }
    else
    {
        y = (row*mPixelsPerMeter) + mSouthWestCorner.mY;
        x = (col*mPixelsPerMeter) + mSouthWestCorner.mX;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates the tile access time stamp value.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Tile::Touch() const
{
    CxUtils::Time* useTime = ( (CxUtils::Time *)(&mLastUseTime) );
    useTime->SetCurrentTime();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Scans a horizontal line for elevation hits.  This is a helper function
///          for scanning in a triangle.
///
///   \param[in] x0 Starting column.
///   \param[in] x1 Ending column.
///   \param[in] y Row to san in.
///   \param[in] collisions Collision values.
///   \param[in] zHigh The elevation upper limit to check for.
///   \param[in] zLow The elevation lower limit to check within.
///
///   \return True if any values of elevation are within the zHigh, zLow 
///           boundaries around the point, otherwise false (clear).
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Tile::CheckLine(const int x0, 
                                 const int x1, 
                                 const int y, 
                                 Point3D::List* collisions,
                                 const double zHigh,
                                 const double zLow) const
{
    bool result = false;
    Point3D point;
    //for(int x = x0; x <= x1 && x < ((int)mPixelWidth)  - 1 && (result == false || collisions); x++)
    for(int x = x0; x <= x1 && x < ((int)mPixelWidth) && (result == false || collisions); x++)
    {
        if(x >= 0 && y >= 0 && y < ((int)mPixelHeight))
        //if(x > 0 && y > 0 && y < ((int)mPixelHeight)  - 1)
        {
            point.mZ = mpTile[y*mPixelWidth + x]/CONVERSION;
            if(point.mZ >= zLow && point.mZ <= zHigh) 
            { 
                result = true; 
                if(collisions)
                {                            
                    ConvertFromPixels(x, y, point.mX, point.mY);
                    collisions->push_back(point);
                }
                else
                {
                    return result;
                }
            }
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::TerrainMap() : mWriteFlag(false),
                           mpCurrentTile(0),
                           mRow(0),
                           mCol(0),
                           mMinElevation(MaxElevation),
                           mMaxElevation(MinElevation),
                           mImgFlag(false),
                           mMinElevationSetFlag(false),
                           mMaxElevationSetFlag(false)
{
#ifdef USE_OPENCV
        mDrawColor = CV_RGB(0, 255, 0);
#endif
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::~TerrainMap()
{
    Destroy();
    ClearObjectList(mCollisions);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads saved map data from disk.
///
///   \param[in] file The name of the file to load.
///   \param[in] writeFlag If true, any changes to the map will be saved, if
///                        false, any new changes will not be saved unless
///                        a new save file is set using SetSaveFile.
///   \param[in] imgFlag   If true, map will try to load from jpgs rather than 
///                        binary zip.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Load(const std::string& file, const bool writeFlag, const bool imgFlag)
{
    TiXmlDocument xml;
    mImgFlag = imgFlag;
    mWorkingDirectory = mFileFinder.GetFilePath(file);
    if(xml.LoadFile(mWorkingDirectory.c_str()))
    {
        int lastSlash = 0;
        if((lastSlash = mWorkingDirectory.rfind('\\')) != std::string::npos)
        {
            mWorkingDirectory = mWorkingDirectory.substr(0,lastSlash);
        }
        else if((lastSlash = mWorkingDirectory.rfind('/')) != std::string::npos)
        {    
            mWorkingDirectory = mWorkingDirectory.substr(0,lastSlash);
        }
        
        
        TiXmlHandle doc(&xml);
        TiXmlElement* root = doc.FirstChild("TerrainMap").ToElement();
        if(root && root->Attribute("version"))
        {
            int version = atoi(root->Attribute("version"));
            if(version == 1)
            {
                TiXmlNode* node = NULL;
                node = doc.FirstChild("TerrainMap").FirstChild("MapSizeMeters").ToNode();
                double mapSizeMeters = atof(node->FirstChild()->Value());
                
                node = doc.FirstChild("TerrainMap").FirstChild("CellSizeMeters").ToNode();
                double cellSizeMeters = atof(node->FirstChild()->Value());

                node = doc.FirstChild("TerrainMap").FirstChild("DefaultPixelsPerMeter").ToNode();
                double pixelsPerMeter = atof(node->FirstChild()->Value());

                node = doc.FirstChild("TerrainMap").FirstChild("MinElevation").ToNode();
                double minElevation = atof(node->FirstChild()->Value());

                node = doc.FirstChild("TerrainMap").FirstChild("MaxElevation").ToNode();
                double maxElevation = atof(node->FirstChild()->Value());

                node = doc.FirstChild("TerrainMap").FirstChild("Origin").FirstChild("Latitude").ToNode();
                double latitude = atof(node->FirstChild()->Value());

                node = doc.FirstChild("TerrainMap").FirstChild("Origin").FirstChild("Longitude").ToNode();
                double longitude = atof(node->FirstChild()->Value());

                std::map<unsigned int, double> pix;
                node = doc.FirstChild("TerrainMap").FirstChild("CellPixelsPerMeter").ToNode();
                if(node && node->FirstChildElement("CellPixelsPerMeter"))
                {
                    TiXmlElement* element = node->FirstChild("CellPixelsPerMeter")->ToElement();
                    while(element)
                    {
                        pix[(unsigned int)atoi(element->Attribute("id"))] = atof(element->Attribute("value"));
                        element = element->NextSiblingElement();
                    }
                }

                if(TerrainMap::Create(mapSizeMeters, cellSizeMeters, pixelsPerMeter))
                {                    
                    char* filename = new char[file.size() + 256];
                    // Generate file names for the different tiles.
                    for(unsigned int i = 0; i < mNumCells*mNumCells; i++)
                    {
                        sprintf(filename, "%s/terrain.xml.tile_%03d.tile", mWorkingDirectory.c_str(), i);
                        mTileNames[i] = filename;
                    }
                    
                    //delete[] filename;
                    if(writeFlag)
                    {
                        mTerrainFileName = file;
                    }
                    mPixelsPerMeterCells = pix;
                    mMinElevation = minElevation;
                    mMaxElevation = maxElevation;
                    mWriteFlag = writeFlag;
                    mMapOrigin.mLatitude = latitude;
                    mMapOrigin.mLongitude = longitude;

                    return true;
                }
            }
        }
    }

    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves terrain map as a jpg file to the "save file name .jpg".
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::SaveToImage(const std::string& filename) const
{
    CxUtils::Image image;
    int result = 0;
    result = image.Create((unsigned short)(mMapSizeMeters*mPixelsPerMeter),(unsigned short)(mMapSizeMeters*mPixelsPerMeter),1);
    double elevation = 0;
    double ebase = mMaxElevation - mMinElevation + CxUtils::CX_EPSILON;
    double x = GetBottomLeftCorner().mX;
    double y = GetTopRightCorner().mY - 1;
    for(int col = 0; col < image.mWidth; col++)
    {
        for(int row = 0; row < image.mHeight; row++)
        {
            elevation = GetElevation(row/mPixelsPerMeter + x,y - col/mPixelsPerMeter );
            if(elevation <= InvalidTerrainHeight)
            {
                elevation = 0.0;
            }
            //elevation = GetElevation(row*mPixelsPerMeter - mMapSizeMeters/2.,col*mPixelsPerMeter - mMapSizeMeters/2.);
            if(mMaxElevation != 0)
            {
                elevation -= mMinElevation;
                elevation /= ebase;
                elevation *= 255;
            }
            image.mpImage[col*image.mWidth*image.mChannels + row] = (unsigned char)elevation;
            
        }
    }
    result += image.Save(filename);
    return (result > 0) ? true : false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Loads an image file from disk, and converts it to terrain
///          height information.  Only PNG and JPEG supported.
///
///   \param[in] file The name of image to load.
///   \param[in] pixelsPerMeter Pixels per meter scale.
///   \param[in] cellSize How large cells should be in meters (max size).
///   \param[in] minElev Minimum elevation value to use.
///   \param[in] maxElev Maximum elevation value to use.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::LoadFromImage(const std::string& file, 
                               const double pixelsPerMeter, 
                               const double cellSize,
                               const double minElev, 
                               const double maxElev)
{
    CxUtils::Image image;

    Destroy();

    if(pixelsPerMeter > 0.0 && image.Load(mFileFinder.GetFilePath(file)))
    {
        if(image.mWidth != image.mHeight)
            return false;

        double mapSizeMeters;
        double cellSizeMeters;

        //if(pixelsPerMeter >= 1.0)
        //{
        //    mapSizeMeters = image.mWidth*pixelsPerMeter;
        //}
        //else
        //{
        //    mapSizeMeters = image.mWidth/pixelsPerMeter;
        //}
        mapSizeMeters = image.mWidth/pixelsPerMeter;
        cellSizeMeters = cellSize;
        if(cellSizeMeters > mapSizeMeters)
        {
            cellSizeMeters = mapSizeMeters;
        }
        
        if(Create(mapSizeMeters, cellSizeMeters, pixelsPerMeter))
        {
            double x, y, e;
            for(int col = 0; col < image.mWidth; col++)
            {
                for(int row = 0; row < image.mHeight; row++)
                {
                    if(mPixelsPerMeter >= 1.0)
                    {
                        y = (GetTopRightCorner().mY - mPixelsPerMeter) - row*mPixelsPerMeter;
                        x = GetBottomLeftCorner().mX + col*mPixelsPerMeter;
                    }
                    else
                    {
                        y = (GetTopRightCorner().mY - mPixelsPerMeter) - row/mPixelsPerMeter;
                        x = GetBottomLeftCorner().mX + col/mPixelsPerMeter;
                    }
                    double color;
                    if(image.mChannels == 3)
                    {
                        color = (image.mpImage[row*image.mWidth*image.mChannels + col*image.mChannels + 0] + 
                                 image.mpImage[row*image.mWidth*image.mChannels + col*image.mChannels + 1] +
                                 image.mpImage[row*image.mWidth*image.mChannels + col*image.mChannels + 2])/3.0; 
                    }
                    else
                    {
                        color = image.mpImage[row*image.mWidth*image.mChannels + col];
                    }
                    
                    e = minElev + color*(maxElev - minElev)/255.0;
                    SetElevation(Point3D(x, y, e));
                }
            }
            return true;
        }
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Saves the current state of the map and it's terrain tiles/cells.
///   
///   \param[in] saveImage flag to specify if tiles should be saved to images also.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Save(const bool saveImage)
{
    if(mNumCells > 0 && mTerrainFileName.empty() == false && mWriteFlag)
    {        
        Tile::Map::const_iterator tile;
        for(tile = mTiles.begin();
            tile != mTiles.end();
            tile++)
        {
            Tile::Names::const_iterator name = mTileNames.find(tile->first);
            if(name != mTileNames.end())
            {
                tile->second->Save(name->second);
                if(saveImage)
                {
                    tile->second->SaveToImage(name->second);
                    tile->second->SaveToXml(name->second);
                }
            }
        }
        SetSaveFile(mTerrainFileName.c_str());
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the master filename to use (or loaded) for the terrain map.
///
///   Once this filename is set, it is possible to save the terrain data.  This
///   method will generate unique filenames for all the tile data so it can
///   load/save tiles.
///
///   \param[in] file The name of the file to use for saving map data to disk.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::SetSaveFile(const std::string& file)
{
    if(mNumCells > 0)
    {
        mTerrainFileName = file;
        mTileNames.clear();

        char* filename = new char[file.size() + 256];
        // Generate file names for the different tiles.
        for(unsigned int i = 0; i < mNumCells*mNumCells; i++)
        {                       
            sprintf(filename, "%s/terrain.xml.tile_%03d.tile", mWorkingDirectory.c_str(), i);
            mTileNames[i] = filename;
        }
        
        delete[] filename;
        
        mWriteFlag = true;
        TiXmlDocument doc;
        FILE* fp = CxUtils::FileIO::Open(mTerrainFileName);
        if(fp == NULL)
        {
            return false;
        }
        fclose(fp);
        TiXmlDeclaration* decl = new TiXmlDeclaration("1.0", "", "yes");
        TiXmlElement* element = new TiXmlElement("TerrainMap");
        TiXmlElement* child = NULL;
        TiXmlElement* child2 = NULL;
        char buffer[512];

        doc.LinkEndChild(decl);
        doc.LinkEndChild(element);

        element->SetAttribute("version", "1");

        child = new TiXmlElement("MapSizeMeters");
        sprintf(buffer, "%.2lf", mMapSizeMeters);
        child->LinkEndChild(new TiXmlText(buffer));
        element->LinkEndChild(child);

        child = new TiXmlElement("CellSizeMeters");
        sprintf(buffer, "%.2lf", mCellSizeMeters);
        child->LinkEndChild(new TiXmlText(buffer));
        element->LinkEndChild(child);

        child = new TiXmlElement("DefaultPixelsPerMeter");
        sprintf(buffer, "%.2lf", mPixelsPerMeter);
        child->LinkEndChild(new TiXmlText(buffer));
        element->LinkEndChild(child);
        
        child = new TiXmlElement("MinElevation");
        sprintf(buffer, "%.2lf", mMinElevation);
        child->LinkEndChild(new TiXmlText(buffer));
        element->LinkEndChild(child);

        child = new TiXmlElement("MaxElevation");
        sprintf(buffer, "%.2lf", mMaxElevation);
        child->LinkEndChild(new TiXmlText(buffer));
        element->LinkEndChild(child);

        child = new TiXmlElement("Origin");
        
        child2 = new TiXmlElement("Latitude");
        sprintf(buffer, "%.6lf", mMapOrigin.mLatitude);
        child2->LinkEndChild(new TiXmlText(buffer));
        child->LinkEndChild(child2);

        child2 = new TiXmlElement("Longitude");
        sprintf(buffer, "%.6lf", mMapOrigin.mLongitude);
        child2->LinkEndChild(new TiXmlText(buffer));
        child->LinkEndChild(child2);
        
        element->LinkEndChild(child);
        
        child = NULL;
        
        std::map<unsigned int, double>::iterator px;
        for(px = mPixelsPerMeterCells.begin();
            px != mPixelsPerMeterCells.end();
            px++)
        {
            if(child == NULL)
            {
                child = new TiXmlElement("CellPixelsPerMeter");
            }

            child2 = new TiXmlElement("Cell");
            sprintf(buffer, "%d", px->first);
            child2->SetAttribute("id", buffer);
            sprintf(buffer, "%.3lf", px->second);
            child2->SetAttribute("value", buffer);

            child->LinkEndChild(child2);
        }

        doc.SaveFile(mTerrainFileName.c_str());
        
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to create a Terrain Map.
///
///   \param[in] mapSizeMeters Size of one side of the map in meters.  Maps are
///                            always square.
///   \param[in] cellSizeMeters How large the tiles/cells inside the map should
///                             be in meters.  Cells are always square.
///   \param[in] pixelsPerMeter How many pixels are used to represent a meter.
///                             The larger this number, the more data points you
///                             have per meter (higher resolution map).                            
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Create(const double mapSizeMeters,
                        const double cellSizeMeters,
                        const double pixelsPerMeter)
{
    Destroy();
    if(pixelsPerMeter > 0)
    {
        mPixelsPerMeter = pixelsPerMeter;
        return RasterMap::Create(mapSizeMeters, cellSizeMeters);
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all allocated memory, and resets to default values.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Destroy()
{
    Save();
    Tile::Map::iterator tile;
    for(tile = mTiles.begin(); tile != mTiles.end(); tile++)
    {
        if(tile->second)
        {
            delete tile->second;
        }
    }
    mTiles.clear();
    mTileNames.clear();
    mpCurrentTile = NULL;
    mTerrainFileName.clear();
    mWriteFlag = false;
    mMapSizeMeters = 0;
    mCellSizeMeters = 0;
    mNumCells = 0;
    mPixelsPerMeter = 0.5;
    mRow = mCol = 0;
    mMinElevation = MaxElevation;
    mMaxElevation = MinElevation;
    mPixelsPerMeterCells.clear();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears the contents of any loaded tile data, but does not modify
///          the data saved to disk.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::Clear()
{
    Tile::Map::iterator tile;
    for(tile = mTiles.begin();
        tile != mTiles.end();
        tile++)
    {
        tile->second->Clear();
    }
    mMinElevation = MaxElevation;
    mMaxElevation = MinElevation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets terrain elevation at a specific point in the map.
///
///   \param[in] point Position and elevation data to save in the map. All
///                    data points are in map coordinates. [-10,000, 30000].
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::SetElevation(const Point3D& point)
{
    bool result = false;

    if(point.mZ < -10000 || point.mZ > 30000)
    {
        return result;
    }

    // Lookup the necessary tile.
    int row, col;
    row = (int)( (point.mY - GetBottomLeftCorner().mY)/mCellSizeMeters );
    col = (int)( (point.mX - GetBottomLeftCorner().mX)/mCellSizeMeters );
    
    Tile* tile = mpCurrentTile;
    if(tile == NULL || tile->mRow != row || tile->mCol != col)
    {
        mpCurrentTile = tile = GetTile(col, row);
    }
    // If tile found, set data.
    if(tile)
    {
        result = tile->SetElevation(point);
    }

    if(result)
    {
        if(point.mZ > mMaxElevation || !mMaxElevationSetFlag) 
        { 
            mMaxElevationSetFlag = true;
            mMaxElevation = point.mZ; 
        }
        if(point.mZ < mMinElevation || !mMinElevationSetFlag) 
        { 
            mMinElevationSetFlag = true;
            mMinElevation = point.mZ; 
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets terrain elevation at a specific point in the map.
///
///   \param[in] x Position and elevation data to save in the map. All
///                    data points are in map coordinates.
///   \param[in] y Position and elevation data to save in the map. All
///                    data points are in map coordinates.
///
///   \return Elevation value in meters.  A value of InvalidTerrainHeight is 
///           returned if data not available.
///
////////////////////////////////////////////////////////////////////////////////////
double TerrainMap::GetElevation(const double x, const double y) const
{
    double result = 0;

    int row, col;
    row = (int)( (y - GetBottomLeftCorner().mY)/mCellSizeMeters );
    col = (int)( (x - GetBottomLeftCorner().mX)/mCellSizeMeters );
    
    Tile* tile = mpCurrentTile;
    if(tile == NULL || tile->mRow != row || tile->mCol != col)
    {
        *((Tile **)(&mpCurrentTile)) = tile = GetTile(col, row);
    }

    if(tile)
    {
        result = tile->GetElevation(x, y);
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets terrain elevation at a specific point in the map.
///
///   \param[in] x Position and elevation data to save in the map. All
///                    data points are in map coordinates.
///   \param[in] y Position and elevation data to save in the map. All
///                    data points are in map coordinates.
///   \param[out' e Elevation data.
///
///   \return True if elevation data was retrieved.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::GetElevation(const double x, const double y, double& e) const
{
    e = 0;

    int row, col;
    row = (int)( (y - GetBottomLeftCorner().mY)/mCellSizeMeters );
    col = (int)( (x - GetBottomLeftCorner().mX)/mCellSizeMeters );
    
    Tile* tile = mpCurrentTile;
    if(tile == NULL || tile->mRow != row || tile->mCol != col)
    {
        *((Tile **)(&mpCurrentTile)) = tile = GetTile(col, row);
    }

    if(tile)
    {
        e = tile->GetElevation(x, y);
    }
    if(e > InvalidTerrainHeight)
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets average terrain elevation at a specific point in the map.
///
///   \param[in] x X position in meters.
///   \param[in] y Y position in meters.
///
///   \return Elevation value in meters.
///
////////////////////////////////////////////////////////////////////////////////////
double TerrainMap::GetFilteredElevation(const double x, const double y) const
{
    double result  = 0;
    double offset = 1/mPixelsPerMeter;
    double div = 0;
    double add = 0;

    add = GetElevation(x,y);

    if(add > InvalidTerrainHeight)
    {
        div++;
        result += add;
    }
    

    add = GetElevation(x+offset*2,y);
    if(add > InvalidTerrainHeight)
    {
        div++;
        result += add;
    }

    add = GetElevation(x-offset*2,y);
    if(add > InvalidTerrainHeight)
    {
        div++;
        result += add;
    }

    add = GetElevation(x,y+offset*2);
    if(add > InvalidTerrainHeight)
    {
        div++;
        result += add;
    }

    add = GetElevation(x,y-offset*2);
    if(add > InvalidTerrainHeight)
    {
        div++;
        result += add;
    }

    if(div != 0)
    {
        result /= div;
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns the average orientation of the ground at a point and yaw. 
///          Imagine if you put a level on the ground to find pitch and roll.
///
///   \param[in] pos Position to find ground angle.
///   \param[in] yaw Orientation you are facing(determines pitch and roll 
///              calculations).
///   \param[out] roll of 'level'.
///   \param[out] yaw of 'level'.
///   \param[in] samples Number of elevation values to average around point.
///              Defaults to 6, which means 12 total samples for pitch, 12 for roll.
///
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::GetGroundOrientation(const Point3D pos,
                                      const double yaw,
                                      double& roll, 
                                      double& pitch, 
                                      unsigned int samples) const
{
    Point3D::List rollPoints, pitchPoints;
    double resolution = 1.0/mPixelsPerMeter;
    int numSamples = (int)samples;
    CxUtils::Quaternion rotation;

    rotation.SetRotationZ(yaw);

    for(int i = -numSamples; i < numSamples; i++)
    {
        Point3D sidePoint;
        sidePoint.mX = i*resolution;
        sidePoint = rotation.Rotate(sidePoint);
        sidePoint += pos;
        double elevation =  GetElevation(sidePoint.mX, sidePoint.mY);
        if(elevation > InvalidTerrainHeight)
        {
            rollPoints.push_back(Point3D(i*resolution, elevation, 0.0));
        }
        Point3D frontPoint;
        frontPoint.mY = i*resolution;
        frontPoint = rotation.Rotate(frontPoint);
        frontPoint += pos;
        elevation =  GetElevation(frontPoint.mX, frontPoint.mY);
        if(elevation > InvalidTerrainHeight)
        {
            pitchPoints.push_back(Point3D(i*resolution, elevation, 0.0));
        }
    }
    pitch = Point3D::GetLinearRegressionAngle(pitchPoints);
    roll = -Point3D::GetLinearRegressionAngle(rollPoints);

    //std::cout << "Pitch: " << pitch << " Roll: " << roll << std::endl;
}


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
bool TerrainMap::Draw(IplImage* image, 
                      const double pixelsPerMeter, 
                      const Point3D& mapBottomCorner, 
                      const bool setBackgroundColor, 
                      CvScalar backgroundColor) const
{
    if(image == NULL) return false;
    if(setBackgroundColor)
    {
        cvSet(image, backgroundColor);
    }
    if(mSizeInPixels == 0)
    {
        return false;
    }
    int row, col;
    int width;
    row = (int)( (mapBottomCorner.mY - GetBottomLeftCorner().mY)/mCellSizeMeters );
    col = (int)( (mapBottomCorner.mX - GetBottomLeftCorner().mX)/mCellSizeMeters );
    if(pixelsPerMeter >= 1.0)
    {
        width = (int)(image->width*pixelsPerMeter/mCellSizeMeters);
    }
    else
    {
        width = (int)(image->width/pixelsPerMeter/mCellSizeMeters);
    }
    if(width == 0)
    {
        width = 1;
    }

    for(int r = row; r < row + width; r++)
    {
        for(int c = col; c < col + width; c++)
        {
            if(c >= 0 && c < (int)mNumCells && r >= 0 && r < (int)mNumCells)
            {
                unsigned int id = (unsigned int)(r*mNumCells + c);
                Tile::Map::const_iterator tile = mTiles.find(id);
                if(tile != mTiles.end())
                {
                    tile->second->Draw(image, pixelsPerMeter, mapBottomCorner, mMinElevation, mMaxElevation, mDrawColor, backgroundColor);
                }
            }
        }
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the object to the image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] points Points to draw in map.
///   \param[in] drawColor Color to draw with.
///
///   \return false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Draw(IplImage* image, 
                      const double pixelsPerMeter, 
                      const Point3D& mapBottomCorner, 
                      const Point3D::List& points, 
                      CvScalar drawColor) const
{
    if(image == NULL) { return false; }

    bool result = false;
    CvPoint px;

    for(unsigned int i = 0; i < (unsigned int)points.size(); i++)
    {
        px = ConvertToImagePoint(image->height, 
                                 pixelsPerMeter,
                                 points[i],
                                 mapBottomCorner,
                                 true);
        if(px.x >= 0 && px.x < image->width && px.y >= 0 && px.y < image->height)
        {
            CvScalar color;
            color.val[0] = drawColor.val[0]*points[i].mZ/(mMaxElevation - mMinElevation);
            color.val[1] = drawColor.val[1]*points[i].mZ/(mMaxElevation - mMinElevation);
            color.val[2] = drawColor.val[2]*points[i].mZ/(mMaxElevation - mMinElevation);
            color.val[3] = 255;

            if(color.val[0] > 255)
                color.val[0] = 255;
            if(color.val[1] > 255)
                color.val[1] = 255;
            if(color.val[2] > 255)
                color.val[2] = 255;

            cvSet2D(image, px.y, px.x, color);
            result = true;
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the object to the image.
///
///   \param[in] image Opencv image to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Bottom left corner of the map to draw from.
///   \param[in] collisions Collisions/points with hits from the map.
///   \param[in] drawColor Color to draw with.
///
///   \return false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::Draw(IplImage* image, 
                      const double pixelsPerMeter, 
                      const Point3D& mapBottomCorner, 
                      const Object::Collisions& collisions, 
                      CvScalar drawColor) const
{
    if(image == NULL) { return false; }

    bool result = false;

    for(unsigned int i = 0; i < (unsigned int)collisions.size(); i++)
    {
        CvScalar color;
        color.val[0] = drawColor.val[0]*collisions[i]->GetBoundingBox().GetTop()/(mMaxElevation - mMinElevation);
        color.val[1] = drawColor.val[1]*collisions[i]->GetBoundingBox().GetTop()/(mMaxElevation - mMinElevation);
        color.val[2] = drawColor.val[2]*collisions[i]->GetBoundingBox().GetTop()/(mMaxElevation - mMinElevation);
        color.val[3] = 255;

        if(color.val[0] > 255)
            color.val[0] = 255;
        if(color.val[1] > 255)
            color.val[1] = 255;
        if(color.val[2] > 255)
            color.val[2] = 255;

        result |= collisions[i]->Draw(image, pixelsPerMeter, mapBottomCorner, color);
    }
    return result;
}


#endif


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to copy the current map object into the map object
///          provided to the method.
///
///   \param[in] map the map object to fill.
///
///   \return True if copy was successful, false otherwise.
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::CopyTo(Map* map) const
{
    TerrainMap* tMap = dynamic_cast<TerrainMap*>(map);
    if(tMap == NULL)
    {
        return false;
    }

    // See if the sizes match and re-create as needed.
    if(fabs(tMap->GetMapSizeMeters() - GetMapSizeMeters()) > 0.001 ||
       fabs(tMap->GetCellSizeMeters() - GetCellSizeMeters()))
    {
        tMap->Destroy();
        tMap->RasterMap::Create(GetMapSizeMeters(), GetCellSizeMeters());
    }

    // Clear any data
    tMap->Clear();

    // Make sure the world origins match
    tMap->SetWorldOrigin(GetWorldOrigin());
    
    // Copy other member variables
    tMap->mWriteFlag        = this->mWriteFlag;
    tMap->mpCurrentTile     = NULL; // TODO: this will take a few lines to set (if needed)
    tMap->mRow              = this->mRow;
    tMap->mCol              = this->mCol;
    tMap->mTerrainFileName  = this->mTerrainFileName;
    tMap->mPixelsPerMeter   = this->mPixelsPerMeter;
    tMap->mMinElevation     = this->mMinElevation;
    tMap->mMaxElevation     = this->mMaxElevation;
    tMap->mImgFlag          = this->mImgFlag;

#ifdef USE_OPENCV
    tMap->mDrawColor        = this->mDrawColor;
#endif

    // Add tiles
    if(mTiles.size() > 0)
    {
        TerrainMap::Tile::Map::const_iterator tileIt;
        for(tileIt = this->mTiles.begin(); tileIt != this->mTiles.end(); tileIt++)
        {
            tMap->mTiles[(*tileIt).first] = (*tileIt).second->Clone();
        }
    }

    // Add tile names
    if(mTileNames.size() > 0)
    {
        TerrainMap::Tile::Names::const_iterator tileNamesIt;
        for(tileNamesIt = this->mTileNames.begin(); tileNamesIt != this->mTileNames.end(); tileNamesIt++)
        {
            tMap->mTileNames[(*tileNamesIt).first] = (*tileNamesIt).second;
        }
    }
    

    // Add pixel per meter cell map
    if(mPixelsPerMeterCells.size() > 0)
    {
        std::map<unsigned int, double>::const_iterator pixelIt;
        for(pixelIt = this->mPixelsPerMeterCells.begin(); pixelIt != this->mPixelsPerMeterCells.end(); pixelIt++)
        {
            tMap->mPixelsPerMeterCells[(*pixelIt).first] = (*pixelIt).second;
        }
    }

    // Copy collision object data
    if(mCollisions.size() > 0)
    {
        Object::List::const_iterator curObj;
    for(curObj = mCollisions.begin(); curObj != mCollisions.end(); curObj++)
    {
        tMap->mCollisions.push_back((*curObj)->Clone());
    }
    }

    return true;
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any elevation around the radius
///          of a point in the map.
///
///   \param[in] point The position in the map to perform the scan at.
///   \param[in] radius The distance around the point in radians to look for
///                     objects.
///   \param[in] collisions Terrain values where collisions occured.
///   \param[in] zHigh The upper z boundary for elevation.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding elevation.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if there is any elevation value within zHigh, zLow and
///           around the point, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::ScanRadiusInXY(const Point3D& point,
                                const double radius,
                                Point3D::List* collisions,
                                const double zHigh, 
                                const double zLow) const

{
    bool result = false;

    if(collisions) { collisions->clear(); }

    int minRow, maxRow;
    int minCol, maxCol;

    minRow = (int)( (point.mY - radius - GetBottomLeftCorner().mY)/mCellSizeMeters );
    maxRow = (int)( (point.mY + radius - GetBottomLeftCorner().mY)/mCellSizeMeters );

    if(minRow > maxRow)
    {
        int temp;
        temp = maxRow;
        maxRow = minRow;
        minRow = temp;
    }
    

    minCol = (int)( (point.mX - radius - GetBottomLeftCorner().mX)/mCellSizeMeters );
    maxCol = (int)( (point.mX + radius - GetBottomLeftCorner().mX)/mCellSizeMeters );

    if(minCol > maxCol)
    {
        int temp;
        temp = maxCol;
        maxCol = minCol;
        minCol = temp;
    }

    Tile* tile = NULL;

    for(int r = minRow; r <= maxRow && (result == false || collisions); r++)
    {
        for(int c = minCol; c <= maxCol && (result == false || collisions); c++)
        {
            tile = GetTile(c, r);

            if(tile)
            {
                result |= tile->ScanRadiusInXY(point, radius, collisions, zHigh, zLow);
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any elevation around the radius
///          of a point in the map.
///
///   \param[in] segment Segment to check for collisions with.
///   \param[in] collisions Point values with elevation that are part of the
///                         collision.
///   \param[in] zHigh The upper z boundary for elevation.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding elevation.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if there is any elevation value within zHigh, zLow and
///           around the point, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::ScanSegmentInXY(const Segment3D& segment,
                                 Point3D::List* collisions,
                                 const double zHigh, 
                                 const double zLow) const
{
    bool result = false;

    if(collisions) 
    { 
        collisions->clear(); 
        Point3D::List hits;
        ScanRadiusInXY(segment.mPoint1, segment.mWidth/2.0, &hits, zHigh, zLow);
        if(hits.size() > 0)
        {
            collisions->insert(collisions->begin(), hits.begin(), hits.end());
        }
        ScanRadiusInXY(segment.mPoint2, segment.mWidth/2.0, &hits, zHigh, zLow);
        if(hits.size() > 0)
        {
            collisions->insert(collisions->begin(), hits.begin(), hits.end());
        }
    }
    else
    {
        if(ScanRadiusInXY(segment.mPoint1, segment.mWidth/2.0, (Point3D::List*)(NULL), zHigh, zLow)) { return true; }
        if(ScanRadiusInXY(segment.mPoint2, segment.mWidth/2.0, (Point3D::List*)(NULL), zHigh, zLow)) { return true; }
    }

    double radius = segment.mWidth/2.0;
    
    int minRow, maxRow;
    int minCol, maxCol;
    BoundingBox scanBox = BoundingBox::CreateBoundingBox(segment);


    minRow = (int)( (scanBox.GetBack() - GetBottomLeftCorner().mY)/mCellSizeMeters );
    maxRow = (int)( (scanBox.GetFront() - GetBottomLeftCorner().mY)/mCellSizeMeters );

    if(minRow > maxRow)
    {
        int temp;
        temp = maxRow;
        maxRow = minRow;
        minRow = temp;
    }
    

    minCol = (int)( (scanBox.GetLeft() - GetBottomLeftCorner().mX)/mCellSizeMeters );
    maxCol = (int)( (scanBox.GetRight() - GetBottomLeftCorner().mX)/mCellSizeMeters );

    if(minCol > maxCol)
    {
        int temp;
        temp = maxCol;
        maxCol = minCol;
        minCol = temp;
    }

    Tile* tile = NULL;

    for(int r = minRow; r <= maxRow && (result == false || collisions); r++)
    {
        for(int c = minCol; c <= maxCol && (result == false || collisions); c++)
        {
            tile = GetTile(c, r);

            if(tile)
            {
                result |= tile->ScanSegmentInXY(segment, collisions, zHigh, zLow);
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any elevations within a triangle 
///   defined by 3 points.
///
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[in] collisions If not NULL, then a copy of all objects 
///              found within the search area is made to the list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.  
///                   Anything below this boundary is ignored in the search.
///
///   \return True if elevation hits found in the area, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool TerrainMap::ScanTriangleInXY(const Point3D& p1,
                                  const Point3D& p2,
                                  const Point3D& p3,
                                  Point3D::List* collisions,
                                  const double zHigh, 
                                  const double zLow) const
{
    bool result = false;

    if(collisions) 
    { 
        collisions->clear(); 
    }
    
    int minRow, maxRow;
    int minCol, maxCol;
    BoundingBox scanBox = BoundingBox::CreateBoundingBox(p1, p2, p3);


    minRow = (int)( (scanBox.GetBack() - GetBottomLeftCorner().mY)/mCellSizeMeters );
    maxRow = (int)( (scanBox.GetFront() - GetBottomLeftCorner().mY)/mCellSizeMeters );

    if(minRow > maxRow)
    {
        int temp;
        temp = maxRow;
        maxRow = minRow;
        minRow = temp;
    }    

    minCol = (int)( (scanBox.GetLeft() - GetBottomLeftCorner().mX)/mCellSizeMeters );
    maxCol = (int)( (scanBox.GetRight() - GetBottomLeftCorner().mX)/mCellSizeMeters );

    if(minCol > maxCol)
    {
        int temp;
        temp = maxCol;
        maxCol = minCol;
        minCol = temp;
    }

    Point3D tri[3];

    tri[0] = p1;
    tri[1] = p2;
    tri[2] = p3;
    bool done = false;
    while(!done)
    {
        done = true;
        for(int i = 0; i < 2; i++)
        {
            if(tri[i + 1].mY < tri[i].mY)
            {
                Point3D tmp;
                tmp = tri[i];
                tri[i] = tri[i + 1];
                tri[i + 1] = tmp;
                done = false;
            }
        }
    }

    Tile* tile = NULL;

    for(int r = minRow; r <= maxRow && (result == false || collisions); r++)
    {
        for(int c = minCol; c <= maxCol && (result == false || collisions); c++)
        {
            tile = GetTile(c, r);

            if(tile)
            {
                result |= tile->ScanTriangleInXY(tri[0], tri[1], tri[2], collisions, zHigh, zLow);
            }
        }
    }

    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the pixels per meter value being used for a specific cell.
///          This is used when some cells in the terrain map have different
///          resolutions (but same physical size).
///
///   \param[in] row Tile row number.
///   \param[in] col Tile column number.
///
///   \return Pixels Per Meter value to be used for the cell, if not set, then
///           the default value is returned.
///
////////////////////////////////////////////////////////////////////////////////////
double TerrainMap::GetPixelsPerMeter(const unsigned int col, const unsigned int row) const
{
    if(row >= 0 && row < mNumCells && col >= 0 && col < mNumCells)
    {
        unsigned int id = row*mNumCells + col;
        std::map<unsigned int, double>::const_iterator px;
        px = mPixelsPerMeterCells.find(id);
        if(px != mPixelsPerMeterCells.end())
        {
            return px->second;
        }
    }
    return mPixelsPerMeter;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pixels per meter value to be used for a specific cell.
///          This is used when some cells in the terrain map have different
///          resolutions (but same physical size).
///
///   \param[in] cellNumber Cell ID Number, [row*NumCells + col].
///   \param[in] pixelsPerMeter Pixels per meter value to use for a cell when
///                             the cell is created.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::SetPixelsPerMeter(const unsigned int cellNumber, const double pixelsPerMeter)
{
    if(cellNumber < mNumCells*mNumCells && pixelsPerMeter > 0.0)
    {
        mPixelsPerMeterCells[cellNumber] = pixelsPerMeter;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the pixels per meter value to be used for a specific cell.
///          This is used when some cells in the terrain map have different
///          resolutions (but same physical size).
///
///   \param[in] row Tile row number.
///   \param[in] col Tile column number.
///   \param[in] pixelsPerMeter Pixels per meter value to use for a cell when
///                             the cell is created.
///
////////////////////////////////////////////////////////////////////////////////////
void TerrainMap::SetPixelsPerMeter(const unsigned int col, const unsigned int row, const double pixelsPerMeter)
{
    if(row >= 0 && row < mNumCells && col >= 0 && col < mNumCells && pixelsPerMeter > 0.0)
    {
        unsigned int id = row*mNumCells + col;
        mPixelsPerMeterCells[id] = pixelsPerMeter;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a tile, creating a new one from scratch if needed.
///
///   \param[in] row Tile row number.
///   \param[in] col Tile column number.
///
///   \return Pointer to tile at [row,col], NULL if tile not found or could not
///           be created.
///
////////////////////////////////////////////////////////////////////////////////////
TerrainMap::Tile* TerrainMap::GetTile(const unsigned int col,
                                      const unsigned int row) const
{
    Tile* tile = NULL;
    
    static CxUtils::Time::Stamp checkTime = 0;

    if(row >= 0 && row < mNumCells && col >= 0 && col < mNumCells)
    {
        unsigned int id = row*mNumCells + col;
        // First check and see if we already loaded this tile.
        Tile::Map::const_iterator t = mTiles.find(id);
        if(t != mTiles.end())
        {
            tile = t->second;
        }
        else
        {
            // First, see if one exists on disk.
            if(mTileNames.size() > 0)
            {
                Tile::Names::const_iterator file;
                file = mTileNames.find(id);
                if(file != mTileNames.end())
                {
                    tile = new Tile();
                    if(tile->Load(file->second,mImgFlag))
                    {
                        if(fabs(tile->mWidth - mCellSizeMeters) <= .1)
                        {
                            double px = GetPixelsPerMeter(col, row);
                            if(fabs(tile->mPixelsPerMeter - px) < 0.0001)
                            { 
                                (*((Tile::Map *)(&mTiles)))[id] = tile;
                            }
                            else
                            {
                                //std::cout << "Tile Cleared: PixelsPerMeter doesnt match" << std::endl;
                                delete tile;
                                tile = NULL;
                            }                            
                        }
                        else
                        {
                            //std::cout << "Tile Cleared: Cell size doesnt match" << std::endl;
                            delete tile;
                            tile = NULL;
                        }
                    }
                    else
                    {
                        //std::cout << "Tile Cleared: Unable to load " << file->second <<std::endl;
                        delete tile;
                        tile = NULL;
                    }
                }
            }
            // If we couldn't load from disk, than create a new tile.
            if(tile == NULL)
            {
                //std::cout << "Creating blank tile" << std::endl;
                tile = new Tile();
                Point3D sw, ne; // South West and North East corners.
                sw = GetBottomLeftCorner();
                ne = GetTopRightCorner();

                // Calcualte world coordinates of bottom left and top right
                // tile corner values.
                sw.mX = sw.mX + col*mCellSizeMeters; // Left side.
                ne.mX = sw.mX + mCellSizeMeters;     // Right side.

                sw.mY = sw.mY + row*mCellSizeMeters; // Bottom side.
                ne.mY = sw.mY + mCellSizeMeters;     // Top side.                
                
                double px = GetPixelsPerMeter(col, row);

                tile->Create(sw, ne, px, row, col, id);
                (*((Tile::Map *)(&mTiles)))[id] = tile;
            }
        }
    }

    if(tile)
    {
        //std::cout << "Tile found" << std::endl;
        tile->Touch();
        double min, max;
        tile->GetMaxAndMin(min, max);
        if(min < mMinElevation)
        {
            *( (double *)(&mMinElevation) ) = min;
        }
        if(max > mMaxElevation)
        {
            *( (double *)(&mMaxElevation) ) = max;
        }
    }

    CxUtils::Time::Stamp checkDelay = 5000;
    if(mWriteFlag && CxUtils::GetTimeMs() - checkTime > checkDelay)
    {
        Tile::Map::iterator t;
        Tile::Map * tiles = ( (Tile::Map *)(&mTiles) );
        t = tiles->begin();
        while(t != tiles->end())
        {
            if(CxUtils::GetTimeMs() - t->second->mLastUseTime.ToMs() >= checkDelay && t->second != tile)
            {
                // Save data
                Tile::Names::const_iterator file;
                file = mTileNames.find(t->first);
                if(file != mTileNames.end())
                {
                    t->second->Save(file->second);
                }
                if(mpCurrentTile == t->second)
                {
                    *((Tile **)(&mpCurrentTile)) = NULL;
                }
                delete t->second;
                tiles->erase(t);
                t = tiles->begin();
            }
            else
            {
                t++;
            }
        }

        checkDelay = CxUtils::GetTimeMs();
    }   
    return tile;
}


/*  End of File */
