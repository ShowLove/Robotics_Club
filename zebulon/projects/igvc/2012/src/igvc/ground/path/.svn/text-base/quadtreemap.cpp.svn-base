#include "ground/path/quadtreemap.h"

using namespace Zebulon;
using namespace IGVC;

using namespace Cartographer;

QuadTreeMap::QuadTreeMap()
{
    mRoot = new Node();
    mMaxDepth = 0;
}

QuadTreeMap::~QuadTreeMap()
{
    this->Destroy();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Method to create a map, will destroy any previous map occupied
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
bool QuadTreeMap::Create(const double mapSizeMeters,
                    const double cellSizeMeters)
{
    Destroy();

    mMutex.Lock();
    if(mapSizeMeters <= 0 || cellSizeMeters <= 0 || cellSizeMeters > mapSizeMeters)
    {
        mMutex.Unlock();
        return false;
    }

    mMapSizeMeters = mapSizeMeters;
    mCellSizeMeters = cellSizeMeters;

    mRoot->mWidth = mapSizeMeters;
    mRoot->mOrigin = CxUtils::Point3D(0.0, 0.0, 0.0);
    mMutex.Unlock();
    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destroys the map, deleting all data.
///
////////////////////////////////////////////////////////////////////////////////////
void QuadTreeMap::Destroy()
{
    mMutex.Lock();
    delete mRoot;
    mRoot = new Node();

    mRoot->mWidth = mMapSizeMeters;
    mRoot->mOrigin = CxUtils::Point3D(0.0, 0.0, 0.0);

    mMaxDepth = 0;
    mMutex.Unlock();
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Clears all data in map, does not delete map dimenions, etc.
///
////////////////////////////////////////////////////////////////////////////////////
void QuadTreeMap::Clear()
{
    mMutex.Lock();

    mRoot->mDirty = false;
    if (mRoot->mObject != NULL)
    {
        delete mRoot->mObject;
        mRoot->mObject = NULL;
    }

    // Recurse through all node quadrants, clear dirty bits
    //  and delete objects at the node. Leave the node
    //  itself in tact because it doesn't matter really,
    //  such space will be ignored because that is the point
    //  of a quad tree.

    RecursiveClear(mRoot);

    mMutex.Unlock();
}

void QuadTreeMap::RecursiveClear(Node* node)
{
    if (node->mNE != NULL)
        RecursiveClear(node->mNE);

    if (node->mNW != NULL)
        RecursiveClear(node->mNW);

    if (node->mSE != NULL)
        RecursiveClear(node->mSE);

    if (node->mSW != NULL)
        RecursiveClear(node->mSW);

    node->mDirty = false;
    if (node->mObject != NULL)
    {
        delete node->mObject;
        node->mObject = NULL;
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Removes dead map items, removes unused branches from memory
///
////////////////////////////////////////////////////////////////////////////////////
void QuadTreeMap::Update()
{
    mMutex.Lock();
    UpdateRecursive(mRoot);
    mMutex.Unlock();
}

bool QuadTreeMap::UpdateRecursive(Node* node)
{
    // Better pruning could be added, could rearrange null nodes, replace dead
    //  parents with children which are still living

    bool dirty = false;
    if (node != NULL)
    {
        if (!UpdateRecursive(node->mNE))
        {
            delete node->mNE;
            node->mNE = NULL;
        }
        else
        {
            dirty = true;
        }

        if (!UpdateRecursive(node->mNW))
        {
            delete node->mNW;
            node->mNW = NULL;
        }
        else
        {
            dirty = true;
        }

        if (!UpdateRecursive(node->mSE))
        {
            delete node->mSE;
            node->mSE = NULL;
        }
        else
        {
            dirty = true;
        }

        if (!UpdateRecursive(node->mSW))
        {
            delete node->mSW;
            node->mSW = NULL;
        }
        else
        {
            dirty = true;
        }

        if (node->mObject != NULL && node->mObject->IsDead())
        {
            dirty = node->mDirty = false;
            delete node->mObject;
            node->mObject = NULL;
        }
        else
        {
            dirty = true;
        }
    }

    return dirty;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to add an object to the map.
///
///   This method should take the input object and add it to the map.  It
///   should perform any object matching if possible to reduce duplication
///   of data (used in Vector based maps).
///
///   \param[in] object  Object to add to the map.  Objects must be
///                      in the maps coordinate frame (e.g. global coodinates).
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool QuadTreeMap::AddObject(const Object* object)
{
    if (!InBounds(object->GetOrigin()))
        return false;

    bool ret;
    mMutex.Lock();
    ret = AddObjectRecursive(object->Clone(), mRoot);
    mMutex.Unlock();
    return ret;
}

bool QuadTreeMap::AddObjectRecursive(Object* object, Node* node, int depth)
{
    // This shouldn't happen, if it does, return an error. Only will occur if the
    //  initial call is null (i.e. the root node is null)
    if (node == NULL)
        return false;

    // If this node is occupied, then we've got to do some magic
    if (node->mObject != NULL)
    {
        double allowedError = .01;
        if(mConsumeThresholds.size() > 0)
        {
            std::map<std::string, double>::iterator e;
            e = mConsumeThresholds.find(object->GetIdentification());
            if(e != mConsumeThresholds.end())
            {
                allowedError = e->second;
            }
        }

        // check if objects are the same, if they are then ignore it.
        if (node->mObject->IsSameAs(object, allowedError))
        {
            // Stupid const correctness... not letting me do simple things
            //  such as UPDATING THE TIME TO LIVE OF AN OBJECT which is the same
            //  as another one. Instead I will have to do a hack where I replace
            //  the old object with the new one on in the same node and pretend like
            //  nobody will notice
            //node->mObject->SetTimeToLive(object->GetTimeToLive());

            //delete node->mObject;
            //node->mObject = object;
            node->mObject->ConsumeMatch(object);
            delete object;
        }
        // objects aren't the same, and there's an object occupying this
        //  cell, so we need to do a split.
        else
        {
            // move down the existing node first
            CxUtils::Point3D oldOrigin = node->mObject->GetOrigin();
            if (oldOrigin.mX < node->mOrigin.mX) // West
            {
                if (oldOrigin.mY > node->mOrigin.mY) // North West
                {
                    if (node->mNW == NULL)
                    {
                        node->mNW = new Node();
                        node->mNW->mWidth = node->mWidth/2.0;
                        node->mNW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(node->mObject, node->mNW, depth+1);
                }
                else // South West
                {
                    if (node->mSW == NULL)
                    {
                        node->mSW = new Node();
                        node->mSW->mWidth = node->mWidth/2.0;
                        node->mSW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(node->mObject, node->mSW, depth+1);
                }
            }
            else // East
            {
                if (oldOrigin.mY > node->mOrigin.mY) // North East
                {
                    if (node->mNE == NULL)
                    {
                        node->mNE = new Node();
                        node->mNE->mWidth = node->mWidth/2.0;
                        node->mNE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(node->mObject, node->mNE, depth+1);
                }
                else // South East
                {
                    if (node->mSE == NULL)
                    {
                        node->mSE = new Node();
                        node->mSE->mWidth = node->mWidth/2.0;
                        node->mSE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(node->mObject, node->mSE, depth+1);
                }
            }

            // Node is officially no longer a leaf, pointer to the object
            //  has moved down the tree
            node->mObject = NULL;

            // move add in the new node next
            CxUtils::Point3D objOrigin = object->GetOrigin();

            if (objOrigin.mX < node->mOrigin.mX) // West
            {
                if (objOrigin.mY > node->mOrigin.mY) // North West
                {
                    if (node->mNW == NULL)
                    {
                        node->mNW = new Node();
                        node->mNW->mWidth = node->mWidth/2.0;
                        node->mNW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(object, node->mNW, depth+1);
                }
                else // South West
                {
                    if (node->mSW == NULL)
                    {
                        node->mSW = new Node();
                        node->mSW->mWidth = node->mWidth/2.0;
                        node->mSW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(object, node->mSW, depth+1);
                }
            }
            else // East
            {
                if (objOrigin.mY > node->mOrigin.mY) // North East
                {
                    if (node->mNE == NULL)
                    {
                        node->mNE = new Node();
                        node->mNE->mWidth = node->mWidth/2.0;
                        node->mNE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(object, node->mNE, depth+1);
                }
                else // South East
                {
                    if (node->mSE == NULL)
                    {
                        node->mSE = new Node();
                        node->mSE->mWidth = node->mWidth/2.0;
                        node->mSE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                    }
                    AddObjectRecursive(object, node->mSE, depth+1);
                }
            }
        }
    }
    // Nothing occupies this cell or any beneath it, so we are free to add a node to it
    else if (!node->mDirty)
    {
        node->mDirty = true;
        node->mObject = object;
        node->mDepth = depth;

        if (depth > mMaxDepth)
            mMaxDepth = depth;
    }
    else
    {
        CxUtils::Point3D objOrigin = object->GetOrigin();

        if (objOrigin.mX < node->mOrigin.mX) // West
        {
            if (objOrigin.mY > node->mOrigin.mY) // North West
            {
                if (node->mNW == NULL)
                {
                    node->mNW = new Node();
                    node->mNW->mWidth = node->mWidth/2.0;
                    node->mNW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                }
                AddObjectRecursive(object, node->mNW, depth+1);
            }
            else // South West
            {
                if (node->mSW == NULL)
                {
                    node->mSW = new Node();
                    node->mSW->mWidth = node->mWidth/2.0;
                    node->mSW->mOrigin = CxUtils::Point3D(node->mOrigin.mX-node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                }
                AddObjectRecursive(object, node->mSW, depth+1);
            }
        }
        else // East
        {
            if (objOrigin.mY > node->mOrigin.mY) // North East
            {
                if (node->mNE == NULL)
                {
                    node->mNE = new Node();
                    node->mNE->mWidth = node->mWidth/2.0;
                    node->mNE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY+node->mWidth/4.0, 0.0);
                }
                AddObjectRecursive(object, node->mNE, depth+1);
            }
            else // South East
            {
                if (node->mSE == NULL)
                {
                    node->mSE = new Node();
                    node->mSE->mWidth = node->mWidth/2.0;
                    node->mSE->mOrigin = CxUtils::Point3D(node->mOrigin.mX+node->mWidth/4.0, node->mOrigin.mY-node->mWidth/4.0, 0.0);
                }
                AddObjectRecursive(object, node->mSE, depth+1);
            }
        }
    }

    return true;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to remove objects from the map individually.
///
///   \param[in] object to be removed
///
///   \return True on success, false on failure.
///
////////////////////////////////////////////////////////////////////////////////////
bool QuadTreeMap::RemoveObject(Object* object)
{
    bool ret;
    mMutex.Lock();
    ret = RemoveObjectRecursive(object, mRoot);
    mMutex.Unlock();
    return ret;
}

bool QuadTreeMap::RemoveObjectRecursive(Object* object, Node* node)
{
    if (node->mObject == object)
    {
        node->mDirty = false;
        node->mObject = NULL;

        delete node->mObject;

        return true;
    }
    else
    {
        // fix this later
        CxUtils::Point3D objOrigin = object->GetOrigin();

        if (objOrigin.mX <= node->mOrigin.mX) // West
        {
            if (objOrigin.mY >= node->mOrigin.mY) // North West
            {
                if (node->mNW != NULL && RemoveObjectRecursive(object, node->mNW))
                {
                    node->mDirty = node->mObject != NULL
                                    || node->mNE != NULL
                                    || node->mNW != NULL
                                    || node->mSE != NULL
                                    || node->mSW != NULL;
                    return true;
                }
                return false;
            }
            else // South West
            {
                if (node->mSW != NULL && RemoveObjectRecursive(object, node->mSW))
                {
                    node->mDirty = node->mObject != NULL
                                    || node->mNE != NULL
                                    || node->mNW != NULL
                                    || node->mSE != NULL
                                    || node->mSW != NULL;
                    return true;
                }
                return false;
            }
        }
        else // East
        {
            if (objOrigin.mY >= node->mOrigin.mY) // North East
            {
                if (node->mNE != NULL && RemoveObjectRecursive(object, node->mNE))
                {
                    node->mDirty = node->mObject != NULL
                                    || node->mNE != NULL
                                    || node->mNW != NULL
                                    || node->mSE != NULL
                                    || node->mSW != NULL;
                    return true;
                }
                return false;
            }
            else // South East
            {
                if (node->mSE != NULL && RemoveObjectRecursive(object, node->mSE))
                {
                    node->mDirty = node->mObject != NULL
                                    || node->mNE != NULL
                                    || node->mNW != NULL
                                    || node->mSE != NULL
                                    || node->mSW != NULL;
                    return true;
                }
                return false;
            }
        }
    }
}

/**
 * Not implemented yet
 **/
bool QuadTreeMap::CopyTo(Map* map) const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects around the radius
///          of a point in the map.
///
///   \param[in] point The position in the map to perform the scan at.
///   \param[in] radius The distance around the point in radians to look for
///                     objects.
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
bool QuadTreeMap::ScanRadiusInXY(const CxUtils::Point3D& point,
                            const double radius,
                            Object::Collisions* collisions,
                            const double zHigh,
                            const double zLow) const
{
    bool ret;
    mMutex.Lock();
    if(collisions)
    {
        collisions->clear();
    }

    ret = RecursiveScanRadiusInXY(point,
                           radius,
                           collisions,
                           zHigh,
                           zLow,
                           GetRootNode());
    //ret = ret_;
    //printf("%d\n", ret);
    mMutex.Unlock();
    return ret;
}

int QuadTreeMap::RecursiveScanRadiusInXY(const CxUtils::Point3D& point,
                                          const double radius,
                                          Object::Collisions* collisions,
                                          const double zHigh,
                                          const double zLow,
                                          const Node* node) const
{
    int ret = 0;
    if (node == NULL)
        return 0;

    // If the square of the distance between the points is less than the square of
    //  the radius we're testing. Checking with the square of the radius because it's
    //  faster to square the radius than it is to get the sqrt of the distance
    // ** Important: this only checks in X-Y, not in Z.
    //if ((node->mOrigin.mX - point.mX)*(node->mOrigin.mX - point.mX)
    //    + (node->mOrigin.mY - point.mY)*(node->mOrigin.mY - point.mY) < radius * radius
    //            && node->mOrigin.mZ <= zHigh && node->mOrigin.mZ >= zLow)
    //{
    //    collisions->push_back(node->mObject);
    //}
    if (node->mObject != NULL && node->mObject->IsCollisionInXY(point, radius, zHigh, zLow))
    {
        if (collisions)
            collisions->push_back(node->mObject);
        ret = 1;
    }

    // Do the recursive search, only check a quad who's within the radius of the point
    //  this one is special because all quads must be checked
    if (point.mX - radius < node->mOrigin.mX) // West
    {
        if (point.mY + radius > node->mOrigin.mY) // North West
        {
            if (node->mNW != NULL)
                ret = ret + RecursiveScanRadiusInXY(point,
                                    radius,
                                    collisions,
                                    zHigh,
                                    zLow,
                                    node->mNW);
        }

        if (point.mY - radius < node->mOrigin.mY) // South West
        {
            if (node->mSW != NULL)
                ret = ret + RecursiveScanRadiusInXY(point,
                                    radius,
                                    collisions,
                                    zHigh,
                                    zLow,
                                    node->mSW);
        }
    }

    if (point.mX + radius > node->mOrigin.mX) // East
    {
        if (point.mY + radius > node->mOrigin.mY) // North East
        {
            if (node->mNE != NULL)
                ret = ret + RecursiveScanRadiusInXY(point,
                                    radius,
                                    collisions,
                                    zHigh,
                                    zLow,
                                    node->mNE);
        }
        if (point.mY - radius < node->mOrigin.mY) // South East
        {
            if (node->mSE != NULL)
                ret = ret + RecursiveScanRadiusInXY(point,
                                    radius,
                                    collisions,
                                    zHigh,
                                    zLow,
                                    node->mSE);
        }
    }

    return ret;
}

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
bool QuadTreeMap::ScanSegmentInXY(const CxUtils::Segment3D& segment,
                             Object::Collisions* collisions,
                             const double zHigh,
                             const double zLow) const
{
    bool ret=false;
    mMutex.Lock();

    CxUtils::Segment3D a;
    a.mPoint1 = segment.GetMidpoint();
    a.mPoint2 = segment.mPoint2;

    CxUtils::Segment3D b;
    b.mPoint1 = segment.mPoint1;
    b.mPoint2 = segment.GetMidpoint();

    Cartographer::Object::Collisions collisionsA;
    Cartographer::Object::Collisions collisionsB;
    mMutex.Unlock();

    ret = ret || ScanRadiusInXY(a.GetMidpoint(), (const double)(a.GetMagnitude() + 2*segment.mWidth), &collisionsA, zHigh, zLow)
          || ScanRadiusInXY(b.GetMidpoint(), (const double)(b.GetMagnitude() + 2*segment.mWidth), &collisionsB, zHigh, zLow);

    mMutex.Lock();
    if (collisions != NULL)
    {
        collisions->clear();

        Object::Collisions::iterator iter;
        for (iter = collisionsA.begin(); iter != collisionsA.end(); iter++)
        {
            if ((*iter)->IsCollisionInXY(segment, zHigh, zLow))
            {
                collisions->push_back(*iter);
                ret = true;
            }
        }

        for (iter = collisionsB.begin(); iter != collisionsB.end(); iter++)
        {
            if ((*iter)->IsCollisionInXY(segment, zHigh, zLow))
            {
                collisions->push_back(*iter);
                ret = true;
            }
        }
    }
    else
    {
        Object::Collisions::iterator iter;
        for (iter = collisionsA.begin(); iter != collisionsA.end() && ret == false; iter++)
        {
            if ((*iter)->IsCollisionInXY(segment, zHigh, zLow))
            {
                ret = true;
            }
        }

        for (iter = collisionsB.begin(); iter != collisionsB.end() && ret == false; iter++)
        {
            if ((*iter)->IsCollisionInXY(segment, zHigh, zLow))
            {
                ret = true;
            }
        }
    }

    mMutex.Unlock();
    return ret;
}

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
/*bool QuadTreeMap::ScanSegmentInXY(const CxUtils::Segment3D& segment,
                             Object::Collisions* collisions,
                             const double zHigh,
                             const double zLow) const
{
    mMutex.Lock();
    double m = (segment.mPoint2.mY - segment.mPoint1.mY) / (segment.mPoint2.mX - segment.mPoint1.mX);

    CxUtils::Segment3D segA;
    CxUtils::Segment3D segB;
    double width = (segment.mWidth/2.0);

    if (m > 0)
    {
        segA = CxUtils::Segment3D(CxUtils::Point3D(segment.mPoint1.mX - width/m,
                                                   segment.mPoint1.mY + width*m, 0),
                                  CxUtils::Point3D(segment.mPoint2.mX - width/m,
                                                   segment.mPoint2.mY + width*m, 0));

        segB = CxUtils::Segment3D(CxUtils::Point3D(segment.mPoint1.mX + width/m,
                                                   segment.mPoint1.mY - width*m, 0),
                                  CxUtils::Point3D(segment.mPoint2.mX + width/m,
                                                   segment.mPoint2.mY - width*m, 0));
    }
    else
    {
        segA = CxUtils::Segment3D(CxUtils::Point3D(segment.mPoint1.mX + width/m,
                                                   segment.mPoint1.mY + width*m, 0),
                                  CxUtils::Point3D(segment.mPoint2.mX + width/m,
                                                   segment.mPoint2.mY + width*m, 0));

        segB = CxUtils::Segment3D(CxUtils::Point3D(segment.mPoint1.mX - width/m,
                                                   segment.mPoint1.mY - width*m, 0),
                                  CxUtils::Point3D(segment.mPoint2.mX - width/m,
                                                   segment.mPoint2.mY - width*m, 0));
    }

    bool ret = RecursiveScanSegmentInXY(segment,
                                 collisions,
                                 zHigh,
                                 zLow,
                                 GetRootNode(),
                                 segA,
                                 segB);
    mMutex.Unlock();
    return ret;
}*/

bool QuadTreeMap::RecursiveScanSegmentInXY(const CxUtils::Segment3D& segment,
                             Cartographer::Object::Collisions* collisions,
                             const double zHigh,
                             const double zLow,
                             const Node* node,
                             const CxUtils::Segment3D& segmentA,
                             const CxUtils::Segment3D& segmentB) const
{
    bool ret = false;
    if (node == NULL)
        return false;

    if (node->mObject != NULL && node->mObject->IsCollisionInXY(segment, zHigh, zLow))
    {
        if (collisions)
            collisions->push_back(node->mObject);
        ret = true;
    }

    // (y-y1) = m*(x-x1)
    // WARNING:: DOES NOT CURRENTLY CHECK FOR SINGULARITIES (vertical or horizontile lines)
    //  THIS CHECK MUST BE IMPLAMENTED LATER

    // No GetSlopeXY function in Segment3D? add later...
    // Slope of both A & B should be the same
    double m = (segmentA.mPoint2.mY - segmentA.mPoint1.mY) / (segmentA.mPoint2.mX - segmentA.mPoint1.mX);

    double yA = m*(node->mOrigin.mX-segmentA.mPoint1.mX) + segmentA.mPoint1.mY;
    double xA = (node->mOrigin.mY-segmentA.mPoint1.mY)/m + segmentA.mPoint1.mX;

    double yB = m*(node->mOrigin.mX-segmentB.mPoint1.mX) + segmentB.mPoint1.mY;
    double xB = (node->mOrigin.mY-segmentB.mPoint1.mY)/m + segmentB.mPoint1.mX;

    if (yA > node->mOrigin.mY || yB > node->mOrigin.mY) // North
    {
        if (xA > node->mOrigin.mX || xB > node->mOrigin.mX) // East
        {
            // NE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNE,
                                                  segmentA,
                                                  segmentB);
            // NW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNW,
                                                  segmentA,
                                                  segmentB);

            // SE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSE,
                                                  segmentA,
                                                  segmentB);
        }

        if (xA < node->mOrigin.mX || xB < node->mOrigin.mX) // West
        {
            // NW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNW,
                                                  segmentA,
                                                  segmentB);
            // NE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNE,
                                                  segmentA,
                                                  segmentB);

            // SW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSW,
                                                  segmentA,
                                                  segmentB);
        }
    }

    if (yA < node->mOrigin.mY || yB < node->mOrigin.mY) // South
    {
        if (xA > node->mOrigin.mX || xB > node->mOrigin.mX) // East
        {
            // SE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSE,
                                                  segmentA,
                                                  segmentB);
            // SW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSW,
                                                  segmentA,
                                                  segmentB);

            // NE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNE,
                                                  segmentA,
                                                  segmentB);
        }

        if (xA < node->mOrigin.mX || xB < node->mOrigin.mX) // West
        {
            // SW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSW,
                                                  segmentA,
                                                  segmentB);
            // SE
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mSE,
                                                  segmentA,
                                                  segmentB);

            // NW
            ret = ret || RecursiveScanSegmentInXY(segment,
                                                  collisions,
                                                  zHigh,
                                                  zLow,
                                                  node->mNW,
                                                  segmentA,
                                                  segmentB);
        }
    }

    return ret;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief This method is used to search for any objects within a triangle
///   defined by 3 points.
///
///     ** WARNING:: Function not yet implemented! **
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
///   \return True if objects found in the area, false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool QuadTreeMap::ScanTriangleInXY(const Point3D& p1,
                              const Point3D& p2,
                              const Point3D& p3,
                              Object::Collisions* collisions,
                              const double zHigh,
                              const double zLow) const
{
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get a pointer to objects in the map.
///
///     ** WARNING:: Function not yet implemented! **
///
///   \return Pointer to objects contained in the map, NULL if the map cannot
///           represent object data this way.
///
////////////////////////////////////////////////////////////////////////////////////
const Object::Set* QuadTreeMap::GetObjects() const
{
    Object::Set* set = new Object::Set();
    GetObjectsRecursive(set, mRoot);
    return static_cast<const Object::Set*>(set);
}

void QuadTreeMap::GetObjectsRecursive(const Object::Set* set, const Node* node) const
{/*
    if (node->mObject != NULL)
    {
        set->insert(node->mObject);
    }
    else
    {
        if (node->mNE != NULL)
            GetObjectsRecursive(set, node->mNE);
        if (node->mNW != NULL)
            GetObjectsRecursive(set, node->mNW);
        if (node->mSE != NULL)
            GetObjectsRecursive(set, node->mSE);
        if (node->mSW != NULL)
            GetObjectsRecursive(set, node->mSW);
    }*/
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Use this method to get a pointer to objects in the map.
///
///     ** WARNING:: Function not yet implemented! **
///
///   \return Pointer to objects contained in the map, NULL if the map cannot
///           represent object data this way.
///
////////////////////////////////////////////////////////////////////////////////////
Object::Set* QuadTreeMap::GetObjects()
{
    Object::Set* set = new Object::Set();
    GetObjectsRecursive(set, mRoot);
    return set;
}

void QuadTreeMap::GetObjectsRecursive(Object::Set* set, Node* node)
{
    if (node->mObject != NULL)
    {
        set->insert(node->mObject->Clone());
    }
    else
    {
        if (node->mNE != NULL)
            GetObjectsRecursive(set, node->mNE);
        if (node->mNW != NULL)
            GetObjectsRecursive(set, node->mNW);
        if (node->mSE != NULL)
            GetObjectsRecursive(set, node->mSE);
        if (node->mSW != NULL)
            GetObjectsRecursive(set, node->mSW);
    }
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a string describing the map
///
///   \return string describing the map (its type, and its depth)
///
////////////////////////////////////////////////////////////////////////////////////
std::string QuadTreeMap::ToString() const
{
    mMutex.Lock();
    char buffer[512];
    sprintf(buffer, "QuadTreeMap Depth:%d", mMaxDepth);
    mMutex.Unlock();
    return buffer;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Returns a const-correct pointer to the root node
///
///   \return const-correct pointer to the root node
///
////////////////////////////////////////////////////////////////////////////////////
const QuadTreeMap::Node* QuadTreeMap::GetRootNode() const
{
    return static_cast<const QuadTreeMap::Node*>(mRoot);
}

#ifdef USE_OPENCV
bool QuadTreeMap::Draw(IplImage* image,
                  const double pixelsPerMeter,
                  const Point3D& mapBottomCorner,
                  const bool setBackgroundColor,
                  CvScalar backgroundColor) const
{
    if (image == NULL)
        return false;

    if(setBackgroundColor)
    {
        cvSet(image, backgroundColor);
    }

    DrawRecursive(image, pixelsPerMeter, mapBottomCorner, GetRootNode());

    return true;
}

void QuadTreeMap::DrawRecursive(IplImage* image, const double pixelsPerMeter, const Point3D& mapBottomCorner, const Node* node) const
{
    if (node == NULL)
        return;

    if (node->mObject != NULL)
    {
        CvPoint bottomLeft = ConvertToImagePoint(image->height, pixelsPerMeter,
                                                 CxUtils::Point3D(node->mOrigin.mX-node->mWidth/2.0, node->mOrigin.mY-node->mWidth/2.0, 0),
                                                 mapBottomCorner);
        CvPoint topRight = ConvertToImagePoint(image->height, pixelsPerMeter,
                                               CxUtils::Point3D(node->mOrigin.mX+node->mWidth/2.0, node->mOrigin.mY+node->mWidth/2.0, 0),
                                               mapBottomCorner);
        cvRectangle(image, bottomLeft, topRight, cvScalar(255,0,0), 1);

        //for (int i=0; i<node->mDepth; i++)
        //    printf(" ");
        //printf("%s\n", node->mObject->ToString().c_str());
        node->mObject->Draw(image, pixelsPerMeter, mapBottomCorner, node->mObject->CalculateDrawColor(node->mObject->GetDrawColor()));

        /*
            (const int imageHeight,
            const double pixelsPerLength,
            const Cartographer::Point3D& point,
            const Cartographer::Point3D& offset,
            const bool flipYAxis)
        */
    }
    else
    {
        DrawRecursive(image, pixelsPerMeter, mapBottomCorner, node->mNW);
        DrawRecursive(image, pixelsPerMeter, mapBottomCorner, node->mNE);
        DrawRecursive(image, pixelsPerMeter, mapBottomCorner, node->mSW);
        DrawRecursive(image, pixelsPerMeter, mapBottomCorner, node->mSE);
    }
}
#endif
