////////////////////////////////////////////////////////////////////////////////////
///
///  \file worldmap.cpp
///  \brief Uses the map class to create a map of objects at absolute coordinates
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
#include "cartographer/maps/worldmap.h"
#include <cstdio>
#include <algorithm>

#define EPSILON .00000001

using namespace Cartographer;

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor
///
////////////////////////////////////////////////////////////////////////////////////
WorldMap::WorldMap()
{
    mCreatedFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor
///
////////////////////////////////////////////////////////////////////////////////////
WorldMap::~WorldMap()
{
    Destroy();
}

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
bool WorldMap::Create(const double mapSizeMeters, const double cellSizeMeters)
{
    if(mapSizeMeters <= 0 || cellSizeMeters <= 0 || cellSizeMeters > mapSizeMeters) return false;
    mMapSizeMeters = mapSizeMeters;
    mCellSizeMeters = cellSizeMeters;
    mNumCells = (unsigned int)(mapSizeMeters/cellSizeMeters);
    mCreatedFlag = true;
    Object::List objects;
    for(int i=0; i<=(int)(mNumCells*mNumCells); i++)
    {
        mCells.push_back(objects);
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destroys the map, deleting all data.
///
////////////////////////////////////////////////////////////////////////////////////
void WorldMap::Destroy()
{
    ClearObjectSet(mObjects);
    mCells.clear();
    mCreatedFlag = false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all map data.
///
////////////////////////////////////////////////////////////////////////////////////
void WorldMap::Clear()
{
    ClearObjectSet(mObjects);
    std::vector<Object::List>::iterator cell;
    for(cell = mCells.begin();
        cell != mCells.end();
        cell++)
    {
        cell->clear();
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Update objects in map, removes dead objects, etc.
///
////////////////////////////////////////////////////////////////////////////////////
void WorldMap::Update()
{
    Object::Set::iterator it = mObjects.begin();
    Object::Set::iterator temp;
    while(it != mObjects.end())
    {
        if((*it)->IsDead())
        {   temp = it;
            it++;
            RemoveObject(*temp);

        }
        else
        {
            it++;
        }
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Removes an object, deletes its memory.
///
///   \param[in] object to remove.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::RemoveObject(Object* object)
{
    if(object == NULL) return false;
    Object::Set::iterator setobj;
    Object::List matches;
    Object::List::iterator obj;
    std::vector<unsigned int> objectCells;
    std::vector<unsigned int>::iterator cell;
    if(mCreatedFlag == false || mObjects.size() == 0) return false;
    objectCells = GetCellMapping(object);
    for(cell = objectCells.begin();
        cell != objectCells.end();
        cell++)
    {
        Object::List* list = &mCells[*cell];
        Object::List::iterator o = list->begin();
        while(o != list->end())
        {
            if(*o == object)
            {
                list->erase(o);
                break;
            }
            else
            {
                o++;
            }
        }
        //int i = 0;
        //for(obj = mCells[*cell].begin();
        //    obj != mCells[*cell].end();
        //    obj++, i++)
        //{
        //    if(*obj == object)
        //    {
        //        mCells[*cell].erase(obj);
        //        break;
        //    }
        //}
    }
    mObjects.erase(object);
    delete object;
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Adds an object to the map list.
///
///   \param[in] object to add to the map.
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::AddObject(const Object* object)
{
    if(mCreatedFlag ==  false)
    {
        return false;
    }

    std::vector<unsigned int> objectCells;
    std::vector<unsigned int>::iterator cell;
    Object::List::iterator temp;
    Object::List matches;
    Object::List merges;
    Object::List::iterator obj;

    double allowedError = CxUtils::CX_EPSILON;
    if(mConsumeThresholds.size() > 0)
    {
        std::map<std::string, double>::iterator e;
        e = mConsumeThresholds.find(object->GetIdentification());
        if(e != mConsumeThresholds.end())
        {
            allowedError = e->second;
        }
    }

    BoundingBox box1 = object->GetBoundingBox();
    box1 += allowedError;
    objectCells = GetCellMapping(&box1);
    // Iterates through all the cells the object is associated
    // with and checks possible matches with current objects in
    // the cells by checking type and source ID.
    for(cell = objectCells.begin();
        cell != objectCells.end();
        cell++)
    {
        for(obj = mCells[*cell].begin();
            obj != mCells[*cell].end();
            obj++)
        {
            if((*obj)->GetType() == object->GetType() &&
                (*obj)->GetSourceID() == object->GetSourceID() &&
                ((MatchInfo*)( (*obj)->GetMapData() ))->mFlag == false)
            {
                ((MatchInfo*)( (*obj)->GetMapData() ))->mFlag = true;
                matches.push_back(*obj);
                BoundingBox box2 = (*obj)->GetBoundingBox();

                if(box1.Intersects(box2, allowedError))
                {
                    bool merge = false;
                    merge = object->IsSameAs(*obj, allowedError);

                    // Perform further analysis.
                    if(merge)
                    {
                        merges.push_back(*obj);
                    }
                }
            }
        }
    }
    SetObjectUsedFlag(matches, false);

    // If we get here, we couldn't match with any
    // data existing inside of the map, therefore
    // add it.

    // Make a copy of the data since we are not
    // allowed to take ownership of the object pointer
    // passed to this method.
    Object* clone = object->Clone();
    Object* toremove;
    Object::List::iterator o = merges.begin();
    while(o != merges.end())
    {
        toremove = *o;
        clone->ConsumeMatch(toremove);
        RemoveObject(toremove);
        o++;
    }

    objectCells = GetCellMapping(clone);
    clone->SetMapData(new MatchInfo(false));
    for(cell = objectCells.begin();
        cell != objectCells.end();
        cell++)
    {
        mCells[*cell].push_back(clone);
    }
    mObjects.insert(clone);

    return true;
}


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
bool WorldMap::CopyTo(Map* map) const
{
    if(mCreatedFlag == false)
    {
        map->Destroy();
        return true;
    }
    WorldMap* wmap = dynamic_cast<WorldMap*>(map);
    if(wmap == NULL)
    {
        return false;
    }
    // See if the sizes match and re-create as needed.
    if(fabs(wmap->GetMapSizeMeters() - GetMapSizeMeters()) > 0.001 ||
       fabs(wmap->GetCellSizeMeters() - GetCellSizeMeters()))
    {
        wmap->Destroy();
        wmap->Create(GetMapSizeMeters(), GetCellSizeMeters());
    }
    // Clear any data.
    wmap->Clear();
    // Make sure the world origins match.
    wmap->SetWorldOrigin(GetWorldOrigin());
    // Copy any consume threshold
    wmap->mConsumeThresholds = mConsumeThresholds;
    // Copy object data.
    Object::Set::const_iterator myObj;
    for(myObj = mObjects.begin(); myObj != mObjects.end(); myObj++)
    {
        // This is faster than searching for objects in cells.
        wmap->AddObject((*myObj));
        /*
        Object* clone = (*myObj)->Clone();
        // Assign to cells, then add to the wmap copy.
        std::vector<Object::List>::const_iterator cell;
        unsigned int cellNumber = 0;
        for(cell = mCells.begin();
            cell != mCells.end();
            cell++, cellNumber++)
        {
            if(std::find(cell->begin(), cell->end(), *myObj) != cell->end())
            {
                wmap->mCells[cellNumber].push_back(clone);
            }
        }
        wmap->mObjects.insert(clone);
        */
    }

    return true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all objects with matching source ID.
///
///   \param[in] sourceID ID type of source for object to remove.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::ClearObjectsBySource(const unsigned int sourceID)
{
    Object::Set matched;
    Object::Set::iterator obj;
    for(obj = mObjects.begin();
        obj != mObjects.end();
        obj++)
    {
        if( (*obj)->GetSourceID() == sourceID)
        {
            matched.insert((*obj));
        }
    }
    for(obj = matched.begin();
        obj != matched.end();
        obj++)
    {
        RemoveObject( (Object *)*obj);
    }
    return (int)matched.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Deletes all objects with matching identifications string.
///
///   \param[in] identification Identification string to match.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::ClearObjectsByIdentification(const std::string& identification)
{
    Object::Set matched;
    Object::Set::iterator obj;
    for(obj = mObjects.begin();
        obj != mObjects.end();
        obj++)
    {
        if( (*obj)->GetIdentification() == identification)
        {
            matched.insert((*obj));
        }
    }
    for(obj = matched.begin();
        obj != matched.end();
        obj++)
    {
        RemoveObject( (Object *)*obj);
    }
    return (int)matched.size();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects in the specified area,
///          in absolute coordinates.
///
///   \param[in] point The position in the map to perform clear at.
///   \param[in] radius The distance around the point in meters to look for
///                     and clear all objects.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::Clear(const Point3D& point,
                    const double radius,
                    const double zHigh,
                    const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanRadiusInXY(point, radius, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects within a segment in the
///   map with absolute coordinates.
///
///   \param[in] segment The segment to remove objects in.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::Clear(const Segment3D& segment,
                    const double zHigh,
                    const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanSegmentInXY(segment, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects within a triangle
///   defined by 3 points in absolute coordinates.
///
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::Clear(const Point3D& p1,
                    const Point3D& p2,
                    const Point3D& p3,
                    const double zHigh,
                    const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanTriangleInXY(p1, p2, p3, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects in the specified area,
///          in relative coordinates.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] point The position in the map to perform clear at.
///   \param[in] radius The distance around the point in meters to look for
///                     and clear all objects.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::ClearLocal(const Pose& pose,
                    const Point3D& point,
                    const double radius,
                    const double zHigh,
                    const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanLocalRadiusInXY(pose, point, radius, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects within a segment in the
///   map with relative local coordinates.
///
///   \param[in] pose The local position which will act as map origin.
///   \param[in] segment The segment to remove objects in.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::ClearLocal(const Pose& pose,
          const Segment3D& segment,
          const double zHigh,
          const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanLocalSegmentInXY(pose, segment, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to clear all objects within a triangle
///   defined by 3 points in relative local coordinates.
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
///   \return Number of objects cleared in the area.
///
////////////////////////////////////////////////////////////////////////////////////
int WorldMap::ClearLocal(const Pose& pose,
          const Point3D& p1,
          const Point3D& p2,
          const Point3D& p3,
          const double zHigh,
          const double zLow)
{
    Object::Collisions objectsToClear;
    Object::Collisions::iterator obj;
    int i = 0;

    ScanLocalTriangleInXY(pose, p1, p2, p3, &objectsToClear, zHigh, zLow);
    for(obj = objectsToClear.begin(); obj != objectsToClear.end(); obj++)
    {
        RemoveObject((Object *)*obj);
        i++;
    }
    return i;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects around the radius
///          of a point in the map relative to local position.
///
///   \param[in] point The position in the map to perform the scan at.
///   \param[in] radius The distance around the point in meters to look for
///                     objects.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::ScanRadiusInXY(const Point3D& point,
                              const double radius,
                              Object::Collisions* collisions,
                              const double zHigh,
                              const double zLow) const
{
    bool found = false;

    if(!mCreatedFlag || mObjects.size() == 0)
    {
        return found;
    }
    // Delete any other discovered objects.
    if(collisions)
    {
        collisions->clear();
    }
    BoundingBox scanBox = BoundingBox::CreateBoundingBox(point, radius);

    std::vector<unsigned int> cellList;
    std::vector<unsigned int>::const_iterator cell;
    cellList = GetCellMapping(&scanBox);
    Object::List objectsOfCells;
    Object::List::const_iterator obj;
    for(cell = cellList.begin();
        cell != cellList.end();
        cell++)
    {
        for(obj = mCells[*cell].begin();
            obj != mCells[*cell].end();
            obj++)
        {
            if(((MatchInfo*)( (*obj)->GetMapData() ))->mFlag == false)
            {
                ((MatchInfo*)( (*obj)->GetMapData() ))->mFlag = true;
                objectsOfCells.push_back(*obj);
                BoundingBox bBox = (*obj)->GetBoundingBox();
                if(scanBox.Intersects(bBox) &&
                    (*obj)->IsCollisionInXY(point,radius,zHigh,zLow))
                {
                    found = true;
                    if(collisions)
                    {
                        collisions->push_back(*obj);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    // Reset flags to false.
    SetObjectUsedFlag(objectsOfCells,false);


    /*Object::Set::const_iterator obj;
    for(obj = mObjects.begin();
    obj != mObjects.end();
    obj++)
    {
        BoundingBox bBox = (*obj)->GetBoundingBox();
        if(scanBox.Intersects(bBox) &&
           (*obj)->IsCollisionInXY(point,radius,zHigh,zLow))
        {
            found = true;
            if(collisions)
            {
                collisions->push_back(*obj);
            }
            else
            {
                break;
            }
        }
    }*/
    return found;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects within a segment in the
///   map with absolute coordinates.
///
///   \param[in] segment The segment to search along for objects.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return True if objects found in area, false if not.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::ScanSegmentInXY(const Segment3D& segment,
                               Object::Collisions* collisions,
                               const double zHigh,
                               const double zLow) const
{
    bool found = false;

    if(!mCreatedFlag || mObjects.size() == 0)
    {
        return found;
    }
    // Delete any other discovered objects.
    if(collisions)
    {
        collisions->clear();
    }
    BoundingBox scanBox = BoundingBox::CreateBoundingBox(segment);

    std::vector<unsigned int> cellList;
    std::vector<unsigned int>::const_iterator cell;
    cellList = GetCellMapping(&scanBox);
    Object::List objectsOfCells;
    Object::List::const_iterator obj;
    for(cell = cellList.begin();
        cell != cellList.end();
        cell++)
    {
        for(obj = mCells[*cell].begin();
            obj != mCells[*cell].end();
            obj++)
        {
            if(((MatchInfo*)( (*obj)->GetMapData() ))->mFlag == false)
            {
                ((MatchInfo*)( (*obj)->GetMapData() ))->mFlag = true;
                objectsOfCells.push_back(*obj);
                BoundingBox bBox = (*obj)->GetBoundingBox();
                if(scanBox.Intersects(bBox) &&
                    (*obj)->IsCollisionInXY(segment,zHigh,zLow))
                {
                    found = true;
                    if(collisions)
                    {
                        collisions->push_back(*obj);
                    }
                    else
                    {
                        // Reset flags to false.
                        SetObjectUsedFlag(objectsOfCells,false);
                        return found;
                    }
                }
            }
        }
    }
    // Reset flags to false.
    SetObjectUsedFlag(objectsOfCells,false);
    return found;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects within a triangle
///   defined by 3 points in absolute coordinates.
///
///   \param[in] p1 First point describing a triangle search area.
///   \param[in] p2 First point describing a triangle search area.
///   \param[in] p3 First point describing a triangle search area.
///   \param[out] collisions If not NULL, then a copy of all objects
///                          found within the search area is made to the
///                          list passed.
///   \param[in] zHigh The upper z boundary for finding objects.  Anything above
///                    this threshold is ignored.
///   \param[in] zLow The lower boundary in the z plan for finding objects.
///                   Anything below this boundary is ignored in the search.
///
///   \return True if collision detected, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::ScanTriangleInXY(const Point3D& p1,
                                const Point3D& p2,
                                const Point3D& p3,
                                Object::Collisions* collisions,
                                const double zHigh,
                                const double zLow) const
{
    bool found = false;

    if(!mCreatedFlag || mObjects.size() == 0)
    {
        return found;
    }
    // Delete any other discovered objects.
    if(collisions)
    {
        collisions->clear();
    }
    BoundingBox scanBox = BoundingBox::CreateBoundingBox(p1, p2, p3);

    std::vector<unsigned int> cellList;
    std::vector<unsigned int>::const_iterator cell;
    cellList = GetCellMapping(&scanBox);
    Object::List objectsOfCells;
    Object::List::const_iterator obj;
    for(cell = cellList.begin();
        cell != cellList.end();
        cell++)
    {
        for(obj = mCells[*cell].begin();
            obj != mCells[*cell].end();
            obj++)
        {
            if(((MatchInfo*)( (*obj)->GetMapData() ))->mFlag == false)
            {
                ((MatchInfo*)( (*obj)->GetMapData() ))->mFlag = true;
                objectsOfCells.push_back(*obj);
                BoundingBox bBox = (*obj)->GetBoundingBox();
                if(scanBox.Intersects(bBox) &&
                    (*obj)->IsCollisionInXY(p1, p2, p3 ,zHigh,zLow))
                {
                    found = true;
                    if(collisions)
                    {
                        collisions->push_back(*obj);
                    }
                    else
                    {
                        break;
                    }
                }
            }
        }
    }
    // Reset flags to false.
    SetObjectUsedFlag(objectsOfCells,false);
    return found;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Returns a string describing the map.
///
////////////////////////////////////////////////////////////////////////////////////
std::string WorldMap::ToString() const
{
    std::string result = "";
    char buf[512];
    sprintf(buf,"Mapsize: %lf Cellsize: %lf Gridsize: %u\n",mMapSizeMeters,mCellSizeMeters,mNumCells);
    result.append(buf);
    result.append("Objects in Map:");
    Object::Set::const_iterator obj;
    for(obj = mObjects.begin(); obj != mObjects.end(); obj++)
    {
        result.append("\n");
        result.append((*obj)->ToString());
    }
    return result;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Identifies cells that the object's bounding box overlaps in the
///          world map.
///
///   \param[in] object Object to get cells for.
///
///   \return List of overlapping cells.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned int> WorldMap::GetCellMapping(const Object* object) const
{
    std::vector<unsigned int> cells;
    if(object == NULL) return cells;
    BoundingBox box = object->GetBoundingBox();
    int minCol = 0, maxCol = 0;
    int minRow = 0, maxRow = 0;
    double halfMap = mMapSizeMeters/2.0;

    minCol = (int)((box.GetLeft() + halfMap) / (mCellSizeMeters + EPSILON ));
    maxCol = (int)((box.GetRight() + halfMap) / (mCellSizeMeters + EPSILON ));
    minRow = (int)(mNumCells -  ((box.GetFront() + halfMap) / (mCellSizeMeters + EPSILON )));
    maxRow = ( int)(mNumCells -  ((box.GetBack() + halfMap) / (mCellSizeMeters + EPSILON )));

    // Check for out of bounds in cells.
    if(minCol < 0)
    {
        minCol = 0;
    }
    if(maxCol >= (int)mNumCells)
    {
        maxCol = mNumCells - 1;
    }
    if(minRow < 0)
    {
        minRow = 0;
    }
    if(maxRow >= (int)mNumCells)
    {
        maxRow = mNumCells - 1;
    }
    // Compute cell numbers.
    for(int r = minRow; r <= maxRow; r++)
    {
        for(int c = minCol; c <= maxCol; c++)
        {
            cells.push_back(r*mNumCells + c);
        }
    }
    return cells;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Identifies cells that the bounding box overlaps in the world map.
///
///   \param[in] box Bounding box to get cells for.
///
///   \return List of overlapping cells.
///
////////////////////////////////////////////////////////////////////////////////////
std::vector<unsigned int> WorldMap::GetCellMapping(const BoundingBox* box) const
{
    std::vector<unsigned int> cells;
    int minCol = 0, maxCol = 0;
    int minRow = 0, maxRow = 0;
    double halfMap = mMapSizeMeters/2.0;

    minCol = (int)((box->GetLeft() + halfMap) / (mCellSizeMeters + EPSILON ));
    maxCol = (int)((box->GetRight() + halfMap) / (mCellSizeMeters + EPSILON ));
    minRow = (int)(mNumCells -  ((box->GetFront() + halfMap) / (mCellSizeMeters + EPSILON )));
    maxRow = ( int)(mNumCells -  ((box->GetBack() + halfMap) / (mCellSizeMeters + EPSILON )));

    // Check for out of bounds in cells.
    if(minCol < 0)
    {
        minCol = 0;
    }
    if(maxCol >= (int)mNumCells)
    {
        maxCol = mNumCells - 1;
    }
    if(minRow < 0)
    {
        minRow = 0;
    }
    if(maxRow >= (int)mNumCells)
    {
        maxRow = mNumCells - 1;
    }
    // Compute cell numbers.
    for(int r = minRow; r <= maxRow; r++)
    {
        for(int c = minCol; c <= maxCol; c++)
        {
            cells.push_back(r*mNumCells + c);
        }
    }
    return cells;
}

#ifdef USE_OPENCV
////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Draws the world map to an opencv image.
///
///   \param[in] image Opencv image wished to draw to.
///   \param[in] pixelsPerMeter Pixels per meter conversion value.
///   \param[in] mapBottomCorner Position on the map we wish to treat as origin.
///   \param[in] setBackgroundColor Flag to specify if we should set a non default
///              background color.
///   \param[in] backgroundColor Color to set background to.
///
///   \return true on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool WorldMap::Draw(IplImage* image,
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
    Object::Set::const_iterator obj;
    for(obj = mObjects.begin(); obj != mObjects.end(); obj++)
    {
        (*obj)->Draw(image, pixelsPerMeter, mapBottomCorner, (*obj)->CalculateDrawColor((*obj)->GetDrawColor()));
    }
    return true;
}


#endif

/*  End of File */
