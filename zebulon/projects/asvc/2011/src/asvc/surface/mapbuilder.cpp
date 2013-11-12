/////////////////////////////////////////////////////////////////////////////////////
///
/// \file mapbuilder.cpp
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
#include "asvc/surface/mapbuilder.h"
#include <statemachine/mission.h>
#include <cartographer/object.h>
#include <cartographer/objects/line.h>
#include <tinyxml/tinyxml.h>

using namespace Zebulon;
using namespace ASVC;

const std::string MapBuilder::GlobalMap = "Raw World Map";
const std::string MapBuilder::ObjectMap = "Object World Map";

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
MapBuilder::MapBuilder()
{
    mpGlobalMap = new Cartographer::WorldMap();
    mpObjectMap = new Cartographer::WorldMap();
    // Navigator will delete thise when done.
    mNavigator.AddMap(mpGlobalMap, GlobalMap);
    mNavigator.AddMap(mpObjectMap, ObjectMap);
    //mpObjectMap->SetDrawColor(Cartographer::Objects::Sphere::Name, CV_RGB(255, 255, 255));
    mpGlobalInfo = NULL;
    mMapSizeMeters = 1000;
    mMapCellSizeMeters = 1;
    mCreateObjectMapFlag = false;
    mDrawResolution = 50;
    mDrawSizeMeters = 10;
    mInitializedFlag = false;
    mSphereRadius = .1;
    mMinScanDistance = 0.1;

    mScanAngleDegrees = 270;
    mScanLeftLimitDegrees = -135;
    mScanRightLimitDegrees = 135;

    mpXmlConfig = NULL;

    mCurrentZone = -1;

    mMaxObjectSize = 1.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
MapBuilder::~MapBuilder()
{

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the map builder settings.
///
///   \param[in] globalInfo Pointer to global information store
///   \param[in] mapSizeMeters Map size in meters (usually 1000)
///   \param[in] mapCellSizeMeters Cell size for grid in meters (usually 1)
///   \param[in] createObjectMap If true, runs ObjectScanner code and adds
///                              results to second map.
///   \param[in] minDistanceForObjects If generating objects, this is the
///                                    minimum distance parameter.
///   \param[in] maxDistanceForObjects If generating objects,this is the
///                                    maximum distance parameter.
///   \param[in] giveRadius True to get radius for objects detected to
///                         figure out sizes of merged points.
///   \param[in] distanceThresholdBetweenObjects When generating objects this
///                     is a threshold for merging objects close to each other.
///   \param[in] distanceThresholdBetweenPointClouds When generating objects this
///                 is the threshold for raw points distance.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MapBuilder::InitializeMaps(ASVC::GlobalInfo* globalInfo,
                                const double mapSizeMeters,
                                const double mapCellSizeMeters,
                                const bool createObjectMap,
                                const double minDistanceForObjects,
                                const double maxDistanceForObjects,
                                const bool giveRadius,
                                const double distanceThresholdBetweenObjects,
                                const double distanceThresholdBetweenPointClouds)
{
    bool result = false;
    if(globalInfo)
    {
        mpGlobalInfo = globalInfo;
        mMapSizeMeters = mapSizeMeters;
        mMapCellSizeMeters = mapCellSizeMeters;
        mCreateObjectMapFlag = createObjectMap;
        mGiveRadiusFlag = giveRadius;
        if(mCreateObjectMapFlag)
        {
            mObjectScanner.Setup(distanceThresholdBetweenObjects,
                                 distanceThresholdBetweenPointClouds,
                                 maxDistanceForObjects,
                                 minDistanceForObjects);
        }
        if(mpGlobalMap->Create(mMapSizeMeters, mMapCellSizeMeters) &&
           mpObjectMap->Create(mMapSizeMeters, mMapCellSizeMeters))
        {
            result = true;

            mNavigator.SetDrawResolution(GlobalMap, mDrawResolution);
            mNavigator.SetDrawResolution(ObjectMap, mDrawResolution);
            mNavigator.SetMapImageSizeMeters(mDrawSizeMeters);

            mMapPose = mpGlobalInfo->GetPose(mpGlobalMap);
            mpGlobalMap->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
            mpObjectMap->SetWorldOrigin(mpGlobalInfo->GetPositionWgs());
            mInitializedFlag = true;
            // Convert zones to map coordinates.
            if(mZonesWgs.size() > 0)
            {
                std::map<unsigned int, std::vector<Cartographer::Wgs> >::iterator wgsZone;
                for(wgsZone = mZonesWgs.begin();
                    wgsZone != mZonesWgs.end();
                    wgsZone++)
                {
                    std::vector<Cartographer::Wgs>::iterator wgs;
                    mZones[wgsZone->first].clear();
                    for(wgs = wgsZone->second.begin();
                        wgs != wgsZone->second.end();
                        wgs++)
                    {
                        mZones[wgsZone->first].push_back(mpGlobalMap->ConvertFromGlobalToMapCoordinates(*wgs));
                    }
                }
            }
            /*
            // Do a test.
            // Zone 1
            Cartographer::Point3D test = mpGlobalMap->ConvertFromGlobalToMapCoordinates(Cartographer::Wgs(36.801818, -76.192163, 0));
            // Zone 2
            //Cartographer::Point3D test = mpGlobalMap->ConvertFromGlobalToMapCoordinates(Cartographer::Wgs(36.802427, -76.191598, 0));
            // Outside
            //Cartographer::Point3D test = mpGlobalMap->ConvertFromGlobalToMapCoordinates(Cartographer::Wgs(36.803056, -76.192939, 0));
            // Find out what zone we are in.
            std::map<unsigned int, Cartographer::Point3D::List >::iterator zone;
            int zoneNumber = -1;
            for(zone = mZones.begin(); zone != mZones.end(); zone++)
            {
                if(test.IsInside2D(zone->second))
                {
                    zoneNumber = (int)zone->first;
                    //break;
                }
            }
            mCurrentZone = zoneNumber;
            */
        }
    }
    return result;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes the map builder settings.
///
///   \param[in] globalInfo Pointer to global information store
///   \param[in] xml XML source to load settings from.
///   \param[in] missionName Name of mission the settings are being
///              loaded from.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool MapBuilder::InitializeMaps(ASVC::GlobalInfo* globalInfo,
                                Configuration::XmlConfigurator& xml,
                                const std::string& missionName)
{
    if(globalInfo == NULL)
        return false;

    double mapSizeMeters = 0;
    double mapCellSizeMeters = 0;
    int createObjectMap = 0;
    double minDistanceForObjects;
    double maxDistanceForObjects;
    int giveRadius;
    double distanceThresholdBetweenObjects;
    double distanceThresholdBetweenPointClouds;
    bool result = true;
    double pixelsPerMeter, drawRange;

    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.MapSizeMeters", mapSizeMeters);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.MapCellSizeMeters", mapCellSizeMeters);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@enable", createObjectMap);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@min_distance", minDistanceForObjects);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@max_distance", maxDistanceForObjects);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@give_radius", giveRadius);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@distance_between_objects_thresh", distanceThresholdBetweenObjects);
    result &= xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@distance_between_points_thresh", distanceThresholdBetweenPointClouds);
    xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@max_size", this->mMaxObjectSize, true);

    xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.MinScanRange", this->mMinScanDistance);
    xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@scan_angle_range", mScanAngleDegrees);
    xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@scan_left_limit", mScanLeftLimitDegrees);
    xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.ObjectMap@scan_right_limit", mScanRightLimitDegrees);

    std::string filename;
    Configuration::XmlConfigurator temp;
    temp.Load(xml.GetPath(), "Surface");

    temp.GetVar(missionName + "@xml", filename);

    TiXmlDocument doc;
    if(doc.LoadFile(filename.c_str()))
    {
        TiXmlHandle handle(&doc);

        TiXmlElement* zone = handle.FirstChild("xml").FirstChild("MapBuilder").FirstChild("Zones").FirstChild("Zone").ToElement();
        while(zone)
        {
            // Load zone files.
            unsigned int count = 0;
            while(zone)
            {
                mZonesWgs[count++] = AI::Mission::LoadWgsWaypointsFromFile(zone->FirstChild()->Value());
                zone = zone->NextSiblingElement("Zone");
            }
        }
    }
    if(result)
    {
        if(xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.Display@pixels_per_meter", pixelsPerMeter) &&
           xml.GetVar(std::string("Surface.") + missionName + ".MapBuilder.Display@draw_range_meters", drawRange) )
        {
            this->SetDrawSettings(pixelsPerMeter, drawRange);
        }
        mpXmlConfig = &xml;
        mMissionName = missionName;
        return InitializeMaps(globalInfo,
                              mapSizeMeters,
                              mapCellSizeMeters,
                              createObjectMap > 0 ? true : false,
                              minDistanceForObjects,
                              maxDistanceForObjects,
                              giveRadius > 0 ? true : false,
                              distanceThresholdBetweenObjects,
                              distanceThresholdBetweenPointClouds);
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates maps based off of global info and initialization settings.
///
///   \param[in] clearMaps If true, old object data is cleared before adding new
///                        data.
///   \param[in] addLIDAR If true LIDAR data is added, false to not add.
///
////////////////////////////////////////////////////////////////////////////////////
void MapBuilder::UpdateMaps(const bool clearMaps,
                            const bool addLIDAR)
{
    if(mpGlobalInfo == NULL || mInitializedFlag == false)
    {
        return;
    }

    if(mpXmlConfig && mMissionName.empty() == false)
    {
        double minDistanceForObjects;
        double maxDistanceForObjects;
        double distanceThresholdBetweenObjects;
        double distanceThresholdBetweenPointClouds;
        bool result = true;
        mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.MinScanRange", this->mMinScanDistance);
        result &= mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.ObjectMap@min_distance", minDistanceForObjects);
        result &= mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.ObjectMap@max_distance", maxDistanceForObjects);
        result &= mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.ObjectMap@distance_between_objects_thresh", distanceThresholdBetweenObjects);
        result &= mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.ObjectMap@distance_between_points_thresh", distanceThresholdBetweenPointClouds);
        mpXmlConfig->GetVar(std::string("Surface.") + mMissionName + ".MapBuilder.ObjectMap@max_size", this->mMaxObjectSize, true);
        mObjectScanner.Setup(distanceThresholdBetweenObjects, distanceThresholdBetweenPointClouds, maxDistanceForObjects, minDistanceForObjects);
    }

    if(clearMaps)
    {
        mpGlobalMap->Clear();
        mpObjectMap->Clear();
    }

    mMapPose = mpGlobalInfo->GetPose(mpGlobalMap);

    // Find out what zone we are in.
    std::map<unsigned int, Cartographer::Point3D::List >::iterator zone;
    int zoneNumber = -1;
    for(zone = mZones.begin(); zone != mZones.end() && zoneNumber < 0; zone++)
    {
        if(mMapPose.mPosition.IsInside2D(zone->second))
        {
            zoneNumber = (int)zone->first;
            break;
        }
    }
    mCurrentZone = zoneNumber;

    static bool first = true;
    if(mCurrentZone >= 0 && (clearMaps || first))
    {
        first = false;
        // Draw current zone.
        zone = mZones.find(mCurrentZone);
        if(zone != mZones.end())
        {
            for(unsigned int i = 0; i < (unsigned int)zone->second.size() - 1; i++)
            {
                Cartographer::Objects::Line line;
                line.GetLine().mPoint1 = zone->second[i];
                line.GetLine().mPoint2 = zone->second[i + 1];
                line.GetLine().mWidth = 0.1;
                mpGlobalMap->AddObject(&line);
                mpObjectMap->AddObject(&line);
            }
            if(zone->second.size() >= 2)
            {
                // Connect endpoints
                Cartographer::Objects::Line line;
                line.GetLine().mPoint1 = zone->second.front();
                line.GetLine().mPoint2 = zone->second.back();
                line.GetLine().mWidth = 0.1;
                mpGlobalMap->AddObject(&line);
                mpObjectMap->AddObject(&line);
            }
        }
    }
    CxUtils::Point3D::List scanData;
    mpGlobalInfo->GetInfo(GlobalInfo::Laser, scanData);
    // Add raw scan data to map.
    CxUtils::Point3D::List::iterator scanPoint;

    if(addLIDAR)
    {
        for(scanPoint = scanData.begin();
            scanPoint != scanData.end();
            scanPoint++)
        {
            if(scanPoint->Distance() >= this->mMinScanDistance)
            {
                Cartographer::Objects::Sphere sphere;
                sphere.SetOrigin(*scanPoint);
                sphere.SetRadius(mSphereRadius);
                sphere.SetSourceID(GlobalInfo::LaserMapID);
                sphere.SetTimeStamp(mMapPose.mTimeStamp);
                mpGlobalMap->AddObject(&sphere, mMapPose);
            }
        }
    }

    if(mCreateObjectMapFlag)
    {
        mObjectScanner.CalculateCartesian(scanData,
                                          mGiveRadiusFlag,
                                          mScanAngleDegrees,
                                          mScanLeftLimitDegrees,
                                          mScanRightLimitDegrees);
        scanData.clear();
        bool isValid = false;
        scanData = mObjectScanner.GetObjects(isValid, AI::ObjectScan::CLOSEST);
        mClosestObjects.clear();
        if(isValid || scanData.size())
        {
            for(scanPoint = scanData.begin();
                scanPoint != scanData.end();
                scanPoint++)
            {
                Buoy sphere;
                CxUtils::Point3D origin = *scanPoint;
                double radius = mSphereRadius;
                // If we are using the radius flag,then the
                // Z component of the point is the radius, so
                // reset the origin z position, and save
                // the new radius value for the object.
                if(mGiveRadiusFlag)
                {
                    origin.mZ = 0;
                    radius = scanPoint->mZ;
                }

                sphere.SetOrigin(origin);
                sphere.SetRadius(radius);
                sphere.SetSourceID(GlobalInfo::LaserMapID);
                sphere.SetTimeStamp(mMapPose.mTimeStamp);

                bool add = false;
                if(mCurrentZone >= 0)
                {
                    zone = mZones.find((int)mCurrentZone);
                    if(zone != mZones.end())
                    {
                        Cartographer::Point3D worldPoint;
                        worldPoint = mMapPose.ConvertFromLocalToMapCoordinates(mMapPose, sphere.GetOrigin());
                        if(worldPoint.IsInside2D(zone->second))
                        {
                            add = true;
                        }
                    }
                    else
                    {
                       add = true;
                    }
                }
                else
                {
                    add = true;
                }

                if(add && sphere.GetRadius() <= mMaxObjectSize)
                {
                    mpObjectMap->AddObject(&sphere, mMapPose);
                    mClosestObjects.push_back(sphere);
                }
            }
        }
    }

}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the maps to the screen.
///
////////////////////////////////////////////////////////////////////////////////////
void MapBuilder::DrawMaps()
{
    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    double vehicleWidth = 1;
    double vehicleLength = 1;
    mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vehicleLength);
    mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vehicleWidth);
    mNavigator.SetDrawPoseSize(vehicleLength, vehicleWidth);
    mNavigator.DrawLocalMap(GlobalMap,
                            mDrawSizeMeters,
                            NULL,
                            NULL,
                            &mMapPose,
                            NULL);
    //Utility::Display::DisplayImage(GlobalMap, mNavigator.GetLocalMapImage(GlobalMap));
    if(mCreateObjectMapFlag)
    {
        mNavigator.DrawLocalMap(ObjectMap,
                                mDrawSizeMeters,
                                NULL,
                                NULL,
                                &mMapPose,
                                NULL);
        //Utility::Display::DisplayImage(ObjectMap, mNavigator.GetLocalMapImage(ObjectMap));
    }

    double vWidth, vLength;
    mpGlobalInfo->GetInfo(GlobalInfo::VehicleLength, vLength);
    mpGlobalInfo->GetInfo(GlobalInfo::VehicleWidth, vWidth);
    CxUtils::Point3D bottomLeftCorner(mMapPose.mPosition.mX - this->mDrawSizeMeters/2.0,
                                      mMapPose.mPosition.mY - this->mDrawSizeMeters/2.0, 0);


    Utm::List trail = this->mpGlobalInfo->GetPositionTrail();
    Utm::List::iterator crumb;
    for(crumb = trail.begin();
        crumb != trail.end();
        crumb++)
    {
        CxUtils::Point3D point = mpGlobalMap->ConvertFromGlobalToMapCoordinates(*crumb);

        Cartographer::Draw(this->mNavigator.GetLocalMapImage(GlobalMap),
                           this->mDrawResolution,
                           bottomLeftCorner,
                           point,
                           vWidth,
                           CV_RGB(255, 255, 0));

        if(mCreateObjectMapFlag)
        {
            Cartographer::Draw(this->mNavigator.GetLocalMapImage(ObjectMap),
                               this->mDrawResolution,
                               bottomLeftCorner,
                               point,
                               vWidth,
                               CV_RGB(255, 255, 0));
        }
    }

    // Draw vehicle on top of everything.
    mMapPose.Draw(mNavigator.GetLocalMapImage(GlobalMap),
                  this->mDrawResolution,
                  bottomLeftCorner,
                  vLength,
                  vWidth);
    if(mCreateObjectMapFlag)
    {
        // Draw vehicle on top of everything.
        mMapPose.Draw(mNavigator.GetLocalMapImage(ObjectMap),
                      this->mDrawResolution,
                      bottomLeftCorner,
                      vLength,
                      vWidth);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the object to either the raw or object map.
///
////////////////////////////////////////////////////////////////////////////////////
void MapBuilder::DrawLocalObject(const bool objectMap,
                                 const Cartographer::Object* object,
                                 const CvScalar color)
{
    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }

    CxUtils::Point3D bottomLeftCorner;

    bottomLeftCorner.Set(mMapPose.mPosition.mX - fabs(mDrawSizeMeters/2.0),
                         mMapPose.mPosition.mY - fabs(mDrawSizeMeters/2.0),
                         mMapPose.mPosition.mZ);

    // Convert object to global coordinates for drawing.
    Cartographer::Object* global = object->ConvertFromLocalToMapCoordinates(mMapPose);
    IplImage* mapImage;
    if(objectMap)
    {
        mapImage = mNavigator.GetLocalMapImage(ObjectMap);
    }
    else
    {
        mapImage = mNavigator.GetLocalMapImage(GlobalMap);
    }

    if(mapImage)
    {
        global->Draw(mapImage,
                     mDrawResolution,
                     bottomLeftCorner,
                     color);
    }

    delete global;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Displays the maps to the screen.
///
///   \param[in] name Name of map to display, empty means all.
///
////////////////////////////////////////////////////////////////////////////////////
void MapBuilder::DisplayMaps(const std::string& name)
{
    if(Utility::Display::GetDisplayFlag() == false || mInitializedFlag == false)
    {
        return;
    }
    if(name.empty() || name == GlobalMap)
    {
        Utility::Display::DisplayImage(GlobalMap, mNavigator.GetLocalMapImage(GlobalMap));
    }
    if((name.empty() || name == ObjectMap) && mCreateObjectMapFlag)
    {
        Utility::Display::DisplayImage(ObjectMap, mNavigator.GetLocalMapImage(ObjectMap));
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets us the N closest objects detected from LIDAR data.
///
///   \param maxNumberOfObjects Number of objects MAX to return in order
///                             of closest to furthest.  If 0, gets all.
///
///   \return Object list (0 if none detected).
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<Cartographer::Objects::Sphere> MapBuilder::GetClosestObjects(const int maxNumberOfObjects)
{
    std::vector<Cartographer::Objects::Sphere> results;
    std::vector<Cartographer::Objects::Sphere>::const_iterator obj;
    for(obj = mClosestObjects.begin();
        obj != mClosestObjects.end();
        obj++)
    {
        results.push_back(*((Cartographer::Objects::Sphere*)&(*obj)));
        if(maxNumberOfObjects != 0 && results.size() == maxNumberOfObjects)
        {
            break;
        }
    }
    return results;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets us the N closest objects detected from LIDAR data. Populates
///   a vector of buoys.
///
///   \param maxNumberOfObjects Number of objects MAX to return in order
///                             of closest to furthest.  If 0, gets all.
///
///   \return Object list (0 if none detected).
///
////////////////////////////////////////////////////////////////////////////////////
Buoy::List MapBuilder::GetClosestBuoys(const int maxNumberOfObjects)
{
    Buoy::List results;
    std::vector<Cartographer::Objects::Sphere>::const_iterator obj;
    for(obj = mClosestObjects.begin();
        obj != mClosestObjects.end();
        obj++)
    {
        Buoy buoy;
        *((Cartographer::Objects::Sphere*)&buoy) = *obj;
        results.push_back(buoy);
        if(maxNumberOfObjects != 0 && results.size() == maxNumberOfObjects)
        {
            break;
        }
    }
    return results;
}


/*  End of File */
