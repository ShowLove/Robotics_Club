////////////////////////////////////////////////////////////////////////////////////
///
///  \file navigator.cpp
///  \brief Iterface that defines how path finding classes should work.
///
///  <br>Author(s): David Adams, Chris Constable
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu, cconstab@ist.ucf.edu
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
#include "cartographer/planning/navigator.h"

using namespace Cartographer;

const std::string LOCAL_POSTFIX = " - Local";

Navigator::Navigator()
{
    mFitnessFunction = NULL;
#ifdef USE_OPENCV
    mMapDrawPoseLength = 2.0;
    mMapDrawPoseWidth = 2.0;
    mMapDrawResolutions["Default"] = 20.0;
    mImageSizeMeters = -1;
    mLocalMapImageFlag = false;
#endif
}


Navigator::~Navigator()
{
#ifdef USE_OPENCV
    std::map<std::string, IplImage*>::iterator img;
    for(img = mMapImages.begin();
        img != mMapImages.end();
        img++)
    {
        if(img->second)
        {
            cvReleaseImage(&img->second);
        }
    }
#endif
    Map::Table::iterator mp;
    for(mp = mMapTable.begin();
        mp != mMapTable.end();
        mp++)
    {
        delete mp->second;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds a map to the navigator with an optional name.
///
///   Navigator will take ownership of the map pointer and delete it on exit.
///
///   \param[in] map Map to add.
///   \param[in] name Optional name, defaults to "Default".
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool Navigator::AddMap(Map* map, std::string name )
{
    mMapTable[name] = map;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a map to the navigator with an optional name.
///
///   \param[in] name Optional name, defaults to "Default".
///
///   \return Map if one, Null if none.
///
////////////////////////////////////////////////////////////////////////////////////
Map* Navigator::GetMap(const std::string& name)
{
    Map::Table::iterator it = mMapTable.find(name.c_str());
    if(it != mMapTable.end())
    {
        return it->second;
    }
    if(name == "Default" && mMapTable.size() > 0)
    {
        return mMapTable.begin()->second;
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets a const map to the navigator with an optional name.
///
///   \param[in] name Optional name, defaults to "Default".
///
///   \return Map if one, Null if none.
///
////////////////////////////////////////////////////////////////////////////////////
const Map* Navigator::GetMap(const std::string& name) const
{
    Map::Table::const_iterator it = mMapTable.find(name.c_str());
    if(it != mMapTable.end())
    {
        return it->second;
    }
    if(name == "Default" || mMapTable.size() > 0)
    {
        it = mMapTable.begin();
        return it->second;
    }
    return NULL;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Function to find a path.
///
///   All paths in map coordinates.
///
///   \param[in] startPosition Starting position in the world that your
///                            vehicle is at.
///   \param[in] startOrientation Orientation at the start position.
///   \param[in] endPosition The desired end position to find
///                  a path to.
///   \param[out] path The path solution found which attempts to go from
///                    start to end.
///   \param[out] allPaths Optional parameter.  If used, then method
///                        returns all potential solution paths.
///   \param[in] mapName Optional name defaults to "Default".  This is the
///                      map to perform search on.
///
///   \return -1 if no map exists, 0 if no path exists, 1 if path found.
///
////////////////////////////////////////////////////////////////////////////////////
int Navigator::GetPathToWorldCoordinate(const Wgs& startPosition,
                                        const Orientation startOrientation,
                                        const Wgs& endPosition,
                                        Path& pathFound,
                                        Path::List * allPaths,
                                        const std::string& mapName)
{
    // Get the requested map. Return if map doesn't exist.
    const Map* map = GetMap(mapName);
    if(!map)
    {
        return -1;
    }

    // Create pose for start and end locations.
    Pose startPose(Pose::ConvertFromGlobalToMapCoordinates(startPosition, map->GetWorldOrigin()),
                   startOrientation);
    Pose endPose(Pose::ConvertFromGlobalToMapCoordinates(endPosition, map->GetWorldOrigin()), Orientation());

    return GetPath(startPose, endPose, pathFound, allPaths, mapName);
}

#ifdef USE_OPENCV


void Navigator::SetDrawResolution(const std::string& mapName,
                                  const double pixelsPerMeter)
{
    if(pixelsPerMeter > 0)
    {
        mMapDrawResolutions[mapName] = pixelsPerMeter;
        std::map<std::string, IplImage*>::iterator img;
        img = mMapImages.find(mapName);
        if(img != mMapImages.end())
        {
            if(img->second)
            {
                cvReleaseImage(&img->second);
            }
            mMapImages.erase(img);
        }
        img = mMapImages.find(mapName + LOCAL_POSTFIX);
        if(img != mMapImages.end())
        {
            if(img->second)
            {
                cvReleaseImage(&img->second);
            }
            mMapImages.erase(img);
        }
    }
}

void Navigator::SetDrawPoseSize(const double platformLength,
                                const double platformWidth)
{
    if(platformLength > 0 && platformWidth > 0)
    {
        mMapDrawPoseLength = platformLength;
        mMapDrawPoseWidth = platformWidth;
    }
}

void Navigator::SetMapImageSizeMeters(const double meters)
{
    mImageSizeMeters = fabs(meters);
}


void Navigator::DrawBlankMap(const std::string& mapName,
                             CvScalar backgroundColor,
                             const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exist
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                double width = mImageSizeMeters > 0 ? mImageSizeMeters : mp->second->GetMapSizeMeters();
                double height = mImageSizeMeters > 0 ? mImageSizeMeters : mp->second->GetMapSizeMeters();

                mMapImages[mapName] = cvCreateImage(cvSize((int)(px->second*width),
                                                    (int)(px->second*height)),
                                                    IPL_DEPTH_8U, 3);
            }
            cvSet(mMapImages[mapName], backgroundColor);
        }
    }
}


IplImage* Navigator::GetMapImage(const std::string& mapName)
{
    std::map<std::string, IplImage*>::iterator it;
    it = mMapImages.find(mapName);
    if(it != mMapImages.end())
    {
        return it->second;
    }
    if(mapName == "Default" || mMapImages.size() > 0)
    {
        return mMapImages.begin()->second;
    }
    return NULL;
}

IplImage* Navigator::GetLocalMapImage(const std::string& mapName)
{
    std::map<std::string, IplImage*>::iterator it;
    it = mMapImages.find(mapName + LOCAL_POSTFIX);
    if(it != mMapImages.end())
    {
        return it->second;
    }
    return NULL;
}


void Navigator::DrawMapObjects(const std::string& mapName,
                               const bool setBackgroundColor,
                               CvScalar backgroundColor,
                               const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exists
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                double width = mImageSizeMeters > 0 ? mImageSizeMeters : mp->second->GetMapSizeMeters();
                double height = mImageSizeMeters > 0 ? mImageSizeMeters : mp->second->GetMapSizeMeters();
                mMapImages[mLocalMapImageFlag ? mapName + LOCAL_POSTFIX : mapName] = cvCreateImage(cvSize((int)(px->second*width),
                                                                                 (int)(px->second*height)),
                                                                                  IPL_DEPTH_8U, 3);
                img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            }
            mp->second->Draw(img->second,
                             px->second,
                             bottomLeftCorner ? *bottomLeftCorner : mp->second->GetBottomLeftCorner(),
                             setBackgroundColor,
                             backgroundColor);
        }
    }
}


void Navigator::DrawMap(const std::string& mapName,
                        const Path::List* pathSolutions,
                        const Path* bestPath,
                        const Pose* currentPose,
                        const Path* waypoints,
                        const Point3D* bottomLeftCornerPtr)
{
    Map* map = GetMap(mapName);
    if(map == NULL)
    {
        return;
    }
    Point3D bottomLeftCorner = bottomLeftCornerPtr ? *bottomLeftCornerPtr : map->GetBottomLeftCorner();
    DrawMapObjects(mapName,
                   true,
                   CV_RGB(0, 0, 0),
                   &bottomLeftCorner);

    if(waypoints)
    {
        DrawWaypoints(mapName, *waypoints, cvScalar(0, 255, 255, 0), &bottomLeftCorner);
    }

    if(pathSolutions)
    {
        DrawPath(mapName,
                 *pathSolutions,
                 cvScalar(255, 150, 0, 0),
                 &bottomLeftCorner);
    }
    if(bestPath)
    {
        DrawPath(mapName,
                 *bestPath,
                 cvScalar(255, 255, 255, 0),
                 &bottomLeftCorner);
    }
    if(currentPose)
    {
        DrawPose(mapName,
                 *currentPose,
                 cvScalar(0, 0, 255, 0),
                 &bottomLeftCorner);
    }

}


void Navigator::DrawLocalMap(const std::string& mapName,
                             const double boxSizeMeters,
                             const Path::List* pathSolutions,
                             const Path* bestPath,
                             const Pose* currentPose,
                             const Path* waypoints)
{
    double temp = mImageSizeMeters;

    mImageSizeMeters = fabs(boxSizeMeters);
    /*
    if(rotate && currentPose)
    {
        mImageSizeMeters *= 1.25;
    }
    */
    Point3D bottomLeftCorner;

    bottomLeftCorner.Set(currentPose ? currentPose->mPosition.mX - fabs(boxSizeMeters/2.0): -fabs(boxSizeMeters),
                         currentPose ? currentPose->mPosition.mY - fabs(boxSizeMeters/2.0) : - fabs(boxSizeMeters),
                         currentPose ? currentPose->mPosition.mZ : 0);

    mLocalMapImageFlag = true;

    DrawMapObjects(mapName,
                   true,
                   CV_RGB(0, 0, 0),
                   &bottomLeftCorner);

    if(waypoints)
    {
        DrawWaypoints(mapName, *waypoints, cvScalar(0, 255, 255, 0), &bottomLeftCorner);
    }

    if(pathSolutions)
    {
        DrawPath(mapName,
                 *pathSolutions,
                 cvScalar(255, 150, 0, 0),
                 &bottomLeftCorner);
    }
    if(bestPath)
    {
        DrawPath(mapName,
                 *bestPath,
                 cvScalar(255, 255, 255, 0),
                 &bottomLeftCorner);
    }
    if(currentPose)
    {
        DrawPose(mapName,
                 *currentPose,
                 cvScalar(0, 0, 255, 0),
                 &bottomLeftCorner);
    }
    // Reset settings.
    mImageSizeMeters = temp;
    mLocalMapImageFlag = false;
    /*
    if(rotate && currentPose)
    {
        std::map<std::string, IplImage*>::iterator img;
        img = mMapImages.find(mapName + LOCAL_POSTFIX);
        if(img != mMapImages.end())
        {
            CvPoint2D32f center = cvPoint2D32f( img->second->width/2, img->second->height/2);
            CvMat* rot_mat = cvCreateMat(2,3,CV_32FC1);
            cv2DRotationMatrix( center, 45, 1, rot_mat );
            IplImage* dst = cvCloneImage(img->second);
            cvWarpAffine(img->second, dst, rot_mat);
        }
    }
    */
}


void Navigator::DrawPath(const std::string& mapName,
                         const Path::List& paths,
                         CvScalar color,
                         const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exists
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                return;
            }
            Path::List::const_iterator p;
            for(p = paths.begin();
                p != paths.end();
                p++)
            {
                p->Draw(img->second,
                        px->second,
                        bottomLeftCorner ? *bottomLeftCorner : mp->second->GetBottomLeftCorner(),
                        color);
            }
        }
    }
}


void Navigator::DrawPath(const std::string& mapName,
                         const Path& path,
                         CvScalar color,
                         const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exists
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                return;
            }
            path.Draw(img->second,
                      px->second,
                      bottomLeftCorner ? *bottomLeftCorner : mp->second->GetBottomLeftCorner(),
                      color);
        }
    }
}


void Navigator::DrawPose(const std::string& mapName,
                         const Pose& pose,
                         CvScalar color,
                         const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exists
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                return;
            }
            pose.Draw(img->second,
                      px->second,
                      bottomLeftCorner ? *bottomLeftCorner : mp->second->GetBottomLeftCorner(),
                      mMapDrawPoseLength,
                      mMapDrawPoseWidth,
                      color);
        }
    }
}


void Navigator::DrawWaypoints(const std::string& mapName,
                              const Path& path,
                              CvScalar color,
                              const Point3D* bottomLeftCorner)
{
    Map::Table::iterator mp;
    mp = mMapTable.find(mapName);
    // Does the map exists
    if(mp != mMapTable.end())
    {
        // Has a resolution been set.
        std::map<std::string, double>::iterator px = mMapDrawResolutions.find(mapName);
        if(px != mMapDrawResolutions.end())
        {
            // Did we create an image yet.
            std::map<std::string, IplImage*>::iterator img;
            img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
            if(img == mMapImages.end())
            {
                return;
            }
            Path::const_iterator wp;
            for(wp = path.Begin(); wp != path.End(); wp++)
            {
                Cartographer::Draw(img->second,
                                   px->second,
                                   bottomLeftCorner ? *bottomLeftCorner : mp->second->GetBottomLeftCorner(),
                                   wp->mPosition,
                                   1.0,
                                   color);
            }
        }
    }
}


void Navigator::DisplayMap(const std::string& mapName)
{
    // Did we create an image yet.
    std::map<std::string, IplImage*>::iterator img;
    img = mLocalMapImageFlag ? mMapImages.find(mapName + LOCAL_POSTFIX) : mMapImages.find(mapName);
    if(img != mMapImages.end())
    {
        /*
        CvPoint2D32f center = cvPoint2D32f( img->second->width/2, img->second->height/2);
        CvMat* rot_mat = cvCreateMat(2,3,CV_32FC1);
        cv2DRotationMatrix( center, 45, 1, rot_mat );
        IplImage* dst = cvCloneImage(img->second);
        cvWarpAffine(img->second, dst, rot_mat);
        */
        cvNamedWindow(mapName.c_str());
        cvShowImage(mapName.c_str(), img->second);
    }
}

void Navigator::DisplayLocalMap(const std::string& mapName)
{
    // Did we create an image yet.
    std::map<std::string, IplImage*>::iterator img;
    img = mMapImages.find(mapName + LOCAL_POSTFIX);
    if(img != mMapImages.end())
    {
        std::string wName = mapName + LOCAL_POSTFIX;
        cvNamedWindow(wName.c_str());
        cvShowImage(wName.c_str(), img->second);
    }
}

#endif


/*  End of File */
