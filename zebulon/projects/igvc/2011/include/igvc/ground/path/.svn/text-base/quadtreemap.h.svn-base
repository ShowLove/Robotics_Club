////////////////////////////////////////////////////////////////////////////////////
///
/// \file quadtreemap.h
/// \brief Class extends Cartographer::Map, implementation of a Quadtree, useful
///         for things which are generally arranged in a mostly 2D environment.
///         for a more 3D application, look into an Octree.
///
/// Author(s): Michael Scherer<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@knights.ucf.edu <br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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

#ifndef __ZEBULON_GROUND_QUADTREEMAP_H__
#define __ZEBULON_GROUND_QUADTREEMAP_H__

#include <cartographer/map.h>
#include <cartographer/objectmap.h>
#include <cxutils/mutex.h>
#include <stdio.h>

namespace Zebulon
{
    namespace IGVC
    {
        class QuadTreeMap : public Cartographer::ObjectMap
        {
        public:
            struct Node
            {
                Node()
                {
                    mNW = NULL;
                    mNE = NULL;
                    mSW = NULL;
                    mSE = NULL;

                    mObject = NULL;

                    mDirty = false;
                }

                ~Node()
                {
                    delete mObject;

                    if (mNW != NULL)
                        delete mNW;
                    if (mNE != NULL)
                        delete mNE;
                    if (mSW != NULL)
                        delete mSW;
                    if (mSE != NULL)
                        delete mSE;
                }

                Node* mNW;
                Node* mNE;
                Node* mSW;
                Node* mSE;

                Cartographer::Object* mObject;
                CxUtils::Point3D mOrigin;
                double mWidth;

                int mDepth;

                bool mDirty;
            };

            QuadTreeMap();
            virtual ~QuadTreeMap();
            virtual bool Create(const double mapSizeMeters,
                                const double cellSizeMeters = 1.0);
            virtual void Destroy();
            virtual void Clear();
            virtual void Update();
            virtual bool AddObject(const Cartographer::Object* object, const Cartographer::Pose& pose)
            {
                Cartographer::Object * o = object->ConvertFromLocalToMapCoordinates(pose);
                return AddObject(o);
            }
            virtual bool AddObject(const Cartographer::Object* object);

            virtual bool RemoveObject(Cartographer::Object* object);
            virtual bool CopyTo(Cartographer::Map* map) const;

            virtual bool ScanRadiusInXY(const CxUtils::Point3D& point,
                                        const double radius,
                                        Cartographer::Object::Collisions* collisions = NULL,
                                        const double zHigh = Cartographer::OBJECT_Z_HIGH_DEFAULT,
                                        const double zLow = Cartographer::OBJECT_Z_LOW_DEFAULT) const;
            virtual bool ScanSegmentInXY(const CxUtils::Segment3D& segment,
                                         Cartographer::Object::Collisions* collisions = NULL,
                                         const double zHigh = Cartographer::OBJECT_Z_HIGH_DEFAULT,
                                         const double zLow = Cartographer::OBJECT_Z_LOW_DEFAULT) const;
            virtual bool ScanTriangleInXY(const CxUtils::Point3D& p1,
                                          const CxUtils::Point3D& p2,
                                          const CxUtils::Point3D& p3,
                                          Cartographer::Object::Collisions* collisions = NULL,
                                          const double zHigh = Cartographer::OBJECT_Z_HIGH_DEFAULT,
                                          const double zLow = Cartographer::OBJECT_Z_LOW_DEFAULT) const;
            virtual const Cartographer::Object::Set* GetObjects() const;
            virtual Cartographer::Object::Set* GetObjects();
            virtual std::string ToString() const;

#ifdef USE_OPENCV
            virtual bool Draw(IplImage* image,
                              const double pixelsPerMeter,
                              const CxUtils::Point3D& mapBottomCorner,
                              const bool setBackgroundColor = true,
                              CvScalar backgroundColor = CV_RGB(0, 0, 0)) const;
#endif

        protected:
            Node* mRoot;
            const Node* GetRootNode() const;

            double mSameThreshold;

            CxUtils::Mutex mMutex;

            void RecursiveClear(Node* node);
            bool UpdateRecursive(Node* node);
            bool AddObjectRecursive(Cartographer::Object* object, Node* node, int depth = 0);
            bool RemoveObjectRecursive(Cartographer::Object* object, Node* node);
            int RecursiveScanRadiusInXY(const CxUtils::Point3D& point,
                                         const double radius,
                                         Cartographer::Object::Collisions* collisions,
                                         const double zHigh,
                                         const double zLow,
                                         const Node* node) const;
            bool RecursiveScanSegmentInXY(const CxUtils::Segment3D& segment,
                                         Cartographer::Object::Collisions* collisions,
                                         const double zHigh,
                                         const double zLow,
                                         const Node* node,
                                         const CxUtils::Segment3D& segmentA,
                                         const CxUtils::Segment3D& segmentB) const;
            void GetObjectsRecursive(const Cartographer::Object::Set* set, const Node* node) const;
            void GetObjectsRecursive(Cartographer::Object::Set* set, Node* node);

#ifdef USE_OPENCV
            void DrawRecursive(IplImage* image, const double pixelsPerMeter, const CxUtils::Point3D& mapBottomCorner, const Node* node) const;
#endif

            int mMaxDepth;
        };
    }
}

#endif // __ZEBULON_GROUND_QUADTREEMAP_H__
