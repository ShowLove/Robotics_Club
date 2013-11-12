/////////////////////////////////////////////////////////////////////////////////////
///
/// \file mapbuilder.h
/// \brief Map Builder is a tool to merge laser data into cartographer.
///
/// <br>Author(s): Daniel Barber
/// <br>Created: 05/28/2011<br>
/// <br>Copyright (c) 2011<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: jmohlenh@ist.ucf.edu <br>
/// <br>Web: http://robotics.ucf.edu <br>
///
///  Redistribution and use in source and binary forms, with or without
///  modification, are permitted provided that the following conditions are met:
///      * Redistributions of source code must retain the above copyright
///        notice, this list of conditions and the following disclaimer.
///      * Redistributions in binary form must reproduce the above copyright
///        notice, this list of conditions and the following disclaimer in the
///        documentation and/or other materials provided with the distribution.
///      * Neither the name of the ROBOTICS CLUB AT UCF, UCF, nor the
///        names of its contributors may be used to endorse or promote products
///        derived from this software without specific prior written permission.
///
///  THIS SOFTWARE IS PROVIDED BY THE ROBOTICS CLUB AT UCF ''AS IS'' AND ANY
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
#ifndef __ZEBULON_ASVC_TOOLS_MAP_BUILDER__H
#define __ZEBULON_ASVC_TOOLS_MAP_BUILDER__H


#include <statemachine/utility/objectscan.h>
#include <xmlconfig/xmlconfigurator.h>
#include <cartographer/planning/beamsearch.h>
#include <cartographer/maps/worldmap.h>
#include <cartographer/objects/sphere.h>
#include <cartographer/object.h>
#include <cartographer/pose.h>
#include "asvc/surface/globalinfo.h"
#include "asvc/surface/buoy.h"
#include "asvc/surface/missions/mission.h"

namespace Zebulon
{
    namespace ASVC
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class MapBuilder
        ///
        ///   Base class which does common map setup and creation accross missions.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class MapBuilder : public Mission
        {
        public:
            const static std::string GlobalMap; ///<  Map containing raw data points.
            const static std::string ObjectMap; ///<  Name of map only containing results of object detection.
            MapBuilder();
            ~MapBuilder();
            // Initializes the builder
            bool InitializeMaps(ASVC::GlobalInfo* globalInfo,                               // Pointer to global info
                                const double mapSizeMeters,                                 // Map size in meters (usually 1000)
                                const double mapCellSizeMeters,                             // Grid cell size in meters (usually 1)
                                const bool createObjectMap,                                 // If true, tries to identify objects and adds to own map
                                const double minDistanceForObjects,                         // Minimum distance for object classification
                                const double maxDistanceForObjects,                         // Maximum distance for object classification
                                const bool giveRadius = true,                               // Generates sphere radius value using Z for objects
                                const double distanceThresholdBetweenObjects = 0.5,         // For generation of objects, sets merge distance between objects formed
                                const double distanceThresholdBetweenPointClouds = 1.0);    // For generation of objects, sets merge distance between raw points
            bool InitializeMaps(ASVC::GlobalInfo* globalInfo,                               // Pointer to global info
                                Configuration::XmlConfigurator& xml,
                                const std::string& missionName);
            // Updates the maps from GlobalInfo data
            virtual void UpdateMaps(const bool clearMaps = true,
                                    const bool addLIDAR = true);
            // Draws maps and displays them to screen (if display is on)
            virtual void DrawMaps();
            virtual void DrawLocalObject(const bool objectMap,
                                         const Cartographer::Object* object,
                                         const CvScalar color = CV_RGB(255, 0, 0));
            virtual void DisplayMaps(const std::string& name = "");
            // True if initialized
            bool IsInitialized() const { return mInitializedFlag; }
            // Gets the closest N Number of objects found (if object mapping turned on)
            std::vector<Cartographer::Objects::Sphere> GetClosestObjects(const int maxNumberOfObjects);
            // Gets the closest N Number of objects found (if object mapping turned on)
            Buoy::List GetClosestBuoys(const int maxNumberOfObjects);
            void SetDrawSettings(const double drawResolutionPixelsPerMeter,
                                 const double drawDistanceMeters)
            {
                mDrawResolution = drawResolutionPixelsPerMeter;
                mDrawSizeMeters = drawDistanceMeters;
            }
            /*  Sets a different radius for sphere objects generated
                by raw laser data. Value is in meters. */
            void SetSphereRadius(const double radius)
            {
                mSphereRadius = radius;
            }
            double GetSphereRadius() const { return mSphereRadius; }
            double GetDrawResolutionPixelsPerMeter() const { return mDrawResolution; }
            double GetDrawDistanceMeters() const { return mDrawSizeMeters; }
            Cartographer::BeamSearch* GetNavigator() { return &mNavigator; }
            const Cartographer::BeamSearch* GetNavigator() const { return &mNavigator; }
            Cartographer::WorldMap* GetWorldMap() { return mpGlobalMap; }
            const Cartographer::WorldMap* GetWorldMap() const { return mpGlobalMap; }
            Cartographer::WorldMap* GetObjectMap() { return mpObjectMap; }
            const Cartographer::WorldMap* GetObjectMap() const { return mpObjectMap; }
            Cartographer::Pose GetCurrentPose() const { return mMapPose; }
            /**  Gets the current zone number (if zones are defined) */
            int GetZoneNumber() const { return mCurrentZone; }
            /** Adds zone defintion to zone list. Points make up a polygon. */
            void AddZone(const unsigned int id, Cartographer::Point3D::List& points)
            {
                mZones[id] = points;
            }
        protected:
            // Simple navigator and map store
            Cartographer::BeamSearch mNavigator;    ///< Contains all maps, can be used for planning too.
            Cartographer::WorldMap* mpGlobalMap;    ///< Contains raw data points.
            Cartographer::WorldMap* mpObjectMap;    ///< Contains only data from object detection.
            Configuration::XmlConfigurator* mpXmlConfig; ///<  Saved pointer to XML configuratin data.
            std::string mMissionName;               ///<  Name of mission pulling data.
            AI::ObjectScan mObjectScanner;          ///< Merges raw point cloud data to group objects.
            std::vector<Cartographer::Objects::Sphere> mClosestObjects; ///<  Closest objects (if using object detection).
            bool mGiveRadiusFlag;                   ///< Flag for getting radius values for objects.
            double mMapSizeMeters;                  ///< Size of maps in meters.
            double mMapCellSizeMeters;              ///< Size of map cells in meters.
            double mSphereRadius;                   ///< Radius value in meters for raw data points.
            double mScanAngleDegrees;               ///< LIDAR scan angle in degrees.
            double mScanLeftLimitDegrees;           ///< LIDAR scan left limit in degrees.
            double mScanRightLimitDegrees;          ///< LIDAR scan right limit in degrees.
            double mMinScanDistance;                ///< LIDAR data must be beyond this point to be used.
            Cartographer::Pose mMapPose;            ///< Current position.
            bool mCreateObjectMapFlag;              ///< Creates object maps.
            double mDrawResolution;                 ///<  Resolution for drawing maps (pixels per meter).
            double mDrawSizeMeters;                 ///<  Size around vehicle to draw in meters when drawing maps.
            bool mInitializedFlag;                  ///<  Set to true if we are initialized.
            std::map<unsigned int, std::vector<Cartographer::Wgs> > mZonesWgs;  ///<  Zone defintions (WGS).
            std::map<unsigned int, Cartographer::Point3D::List > mZones;        ///<  Zones defined in map coordinates.
            int mCurrentZone;                                                   ///<  Current zone the robot is in.
            double mMaxObjectSize;                                              ///<  Maximum object size.
        };
    }
}

#endif //__ZEBULON_ASVC_TOOLS_MAP_BUILDER__H
