////////////////////////////////////////////////////////////////////////////////////
///
///  \file navigator.h
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
#ifndef _LIB__CARTOGRAPHER_NAVIGATOR__H
#define _LIB__CARTOGRAPHER_NAVIGATOR__H

#include "cartographer/objectmap.h"
#include "cartographer/rastermap.h"
#include "cartographer/path.h"

namespace Cartographer
{
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Navigator
    ///   \brief The Navigator class is the main interface for collecting maps
    ///          and using them to find paths between different locations within them.
    ///
    ///   All path solutions found are in map coordinates (i.e. not relative to the
    ///   starting pose).  To convert to local, use the methods in the Path class.
    ///
    ///   By building off of this interface, it is possible to create different
    ///   search algorithms like the Beam Search algorithm in this library.
    ///
    ///   Path Finding algorithms should inherit from this class and overload GetPath.
    ///   A Cartographer::Map must be passed using SetMap, it is stored in an std::map.
    ///   It can be accessed by GetMap, if more than one map is stored, access it by
    ///   a name string, else ignore the name parameter.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL Navigator: protected Path::FitnessFunction
    {
    public:

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class Settings
        ///   \brief Settings encapsulates all configuration variables for a particular
        ///          navigator. It standardizes the interface for storing and retrieving
        ///          parameters as well.
        ///
        ///    This small class only contains two members: a name, for identification, and
        ///    a vector of parameters. A navigator would create an instance of this class
        ///    and use it to house all configurable variables. A standard interface is
        ///    defined below for setting and retreiving the parameters.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL Settings
        {
        public:
            Settings(unsigned int numOfParameters , std::string configName = "")
            {
                mName = configName;
                //mParameters.resize(numOfParameters);
            }
            //using map instead for safer data access
            Settings(std::string configName = "")
            {
                mName = configName;
                //mParameters.resize(numOfParameters);
            }

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Sets a parameter
            ///
            ///   \param[in] parameterID ID of parameter
            ///   \param[in] parameterValue value of parameter
            ///   \param[in] positiveValuesOnlyFlag if set, ensures this value is positive
            ///
            ///   \return True if success, false otherwise
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            virtual bool Set(unsigned int parameterID, double parameterValue, bool positiveValuesOnlyFlag = false)
            {
                if(positiveValuesOnlyFlag && (parameterValue < 0)) return false;
                mParameters[parameterID] = parameterValue;
                return true;
            }

            ////////////////////////////////////////////////////////////////////////////////////
            ///
            ///   \brief Gets a parameter
            ///
            ///   \param[in] parameterID ID of parameter
            ///
            ///   \return double the value of the parameter
            ///
            ////////////////////////////////////////////////////////////////////////////////////
            inline virtual double Get(unsigned int parameterID)
            {
                if(mParameters.find(parameterID) == mParameters.end()) return 0;
                return mParameters[parameterID];
            }
            std::string mName;                  ///< Name of this configuration (e.g. "Shallow Beam Search").

        protected:
            std::map<unsigned int,double> mParameters;    ///< Internal list of parameters. Accessed using Get().
        };

        // Constructor.
        Navigator();
        // Destructor.
        virtual ~Navigator();

        bool AddMap(Map* map, std::string name = "Default");
        Map* GetMap(const std::string& name = "Default");
        const Map* GetMap(const std::string& name = "Default") const;
        ObjectMap* GetObjectMap(const std::string& name = "Default") { return dynamic_cast<ObjectMap*>(GetMap(name)); }
        const ObjectMap* GetObjectMap(const std::string& name = "Default") const { return dynamic_cast<const ObjectMap*>(GetMap(name)); }
        Pose CreatePose(const std::string& mapName, const Utm& position, const Point3D& rotation) const
        {
            const Map* map = GetMap(mapName);
            if(map)
            {
                return map->CreatePose(position, rotation);
            }
            return Pose();
        }
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Function to overload when creating a path finding algorithm.
        ///
        ///   \param[in] start Starting pose in the map to plan from.
        ///   \param[in] end The desired end position (and potentially pose) to find
        ///                  a path to.
        ///   \param[out] path The path solution found which attempts to go from
        ///                    start to end.  Solution is in map coordinates.
        ///   \param[out] allPaths Optional parameter.  If used, then method
        ///                        returns all potential solution paths.
        ///   \param[in] mapName Optional name defaults to "Default".  This is the
        ///                      map to perform search on.
        ///
        ///   \return -1 if no map exists, 0 if no path exists, 1 if path found.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        virtual int GetPath(const Pose& start,
                            const Pose& end,
                            Path& pathFound,
                            Path::List * allPaths = NULL,
                            const std::string& mapName = "Default") = 0;

        int GetPathToWorldCoordinate(const Wgs& startPosition,
                                     const Orientation startOrientation,
                                     const Wgs& endPosition,
                                     Path& pathFound,
                                     Path::List * allPaths = NULL,
                                     const std::string& mapName = "Default");

        // Method to set the fitness function to use for evaluation of paths.
        virtual void SetFitnessFunction(Path::FitnessFunction* ff) { mFitnessFunction = ff;}

        void SetSettings(Cartographer::Navigator::Settings* settings) { mpSettings = settings; }
        Cartographer::Navigator::Settings* GetSettings() { return mpSettings; }

#ifdef USE_OPENCV
        virtual void SetDrawResolution(const std::string& mapName,
                                       const double pixelsPerMeter);
        virtual void SetDrawPoseSize(const double platformLength,
                                     const double platformWidth);
        // How big of an area of the map to draw in meters.
        virtual void SetMapImageSizeMeters(const double meters);
        virtual void DrawBlankMap(const std::string& mapName,
                                  CvScalar backgroundColor = CV_RGB(0, 0, 0),
                                  const Point3D* bottomLeftCorner = NULL);
        virtual void DrawMapObjects(const std::string& mapName = "Default",
                                    const bool setBackgroundColor = false,
                                    CvScalar backgroundColor = CV_RGB(0, 0,0),
                                    const Point3D* bottomLeftCorner = NULL);
        IplImage* GetMapImage(const std::string& mapName = "Default");
        IplImage* GetLocalMapImage(const std::string& mapName = "Default");
        virtual void DrawMap(const std::string& mapName,
                             const Path::List* pathSolutions,
                             const Path* bestPath,
                             const Pose* currentPose,
                             const Path* waypoints = NULL,
                             const Point3D* bottomLeftCorner = NULL);
        virtual void DrawLocalMap(const std::string& mapName,
                                  const double boxSizeMeters,
                                  const Path::List* pathSolutions,
                                  const Path* bestPath,
                                  const Pose* currentPose,
                                  const Path* waypoints = NULL);
        virtual void DrawPath(const std::string& mapName,
                              const Path::List& paths,
                              const CvScalar color = CV_RGB(0, 150, 255),
                              const Point3D* bottomLeftCorner = NULL);
        virtual void DrawPath(const std::string& mapName,
                              const Path& path,
                              const CvScalar color = CV_RGB(255, 255, 255),
                              const Point3D* bottomLeftCorner = NULL);
        virtual void DrawPose(const std::string& mapName,
                              const Pose& pose,
                              const CvScalar color = CV_RGB(255, 0, 0),
                              const Point3D* bottomLeftCorner = NULL);
        virtual void DrawWaypoints(const std::string& mapName,
                                   const Path& path,
                                   const CvScalar color = CV_RGB(255, 255, 0),
                                   const Point3D* bottomLeftCorner = NULL);
        virtual void DisplayMap(const std::string& mapName);
        virtual void DisplayLocalMap(const std::string& mapName);
#endif

    protected:
        Path::FitnessFunction* mFitnessFunction;  ///< Optional user defined fitness function to use to evaluate paths.
        Navigator::Settings* mpSettings;          ///< Pointer to active settings object for this navigator.
    private:
#ifdef USE_OPENCV
        double mMapDrawPoseLength;                         ///<  Length for drawing pose.
        double mMapDrawPoseWidth;                          ///<  Width for drawing pose.
        std::map<std::string, double> mMapDrawResolutions; ///<  Map draw resolutions.
        std::map<std::string, IplImage*> mMapImages;       ///<  Map images drawn.
        double mImageSizeMeters;                           ///<  Map image size in meters.
        bool mLocalMapImageFlag;                           ///<  If true, drawing local map image.
#endif

        double mZUpperBound;    ///< Upper Z-value to scan to for collisions. Relative to current position.
        double mZLowerBound;    ///< Lower Z-value to scan to for collisions. Relative to current position.
        Map::Table mMapTable;   ///< Table of all maps available to search through.
    };
}

#endif //_LIB__CARTOGRAPHER_NAVIGATOR__H

/* End of File */
