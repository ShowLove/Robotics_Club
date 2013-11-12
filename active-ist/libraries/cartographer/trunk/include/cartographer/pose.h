////////////////////////////////////////////////////////////////////////////////////
///
///  \file pose.h
///  \brief Pose is a structure for storing the current state of an object in
///  3D space.
///
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 December 2008
///  <br>Copyright (c) 2008
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
#ifndef __LIB_CARTOGRAPHER_POSE__H
#define __LIB_CARTOGRAPHER_POSE__H

#include "cartographer/libcartdll.h"
#include <cxutils/math/cxmath.h>
#include <cxutils/circulararray.h>
#include <cxutils/mutex.h>
#include <string>
#include <set>

namespace Cartographer
{
    typedef CxUtils::Wgs Wgs;
    typedef CxUtils::Utm Utm;
    
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Pose
    ///   \brief Data structure for representing the position, orientation, and
    ///          velocity of an object (or platform) in 3D space at a specific point
    ///          in time.
    ///   
    ///   Map coordinate frame is defined as follows: X axis is positive going East
    ///   and negative West, Y axis is positive going North, negative going South. The
    ///   Z axis is positive with increased elevation, negative going down (into 
    ///   terrain).  All values are in meters.
    ///
    ///   The local coordinate frame is defined as follows:  X axis is positive in
    ///   front of the platform/object, negative behind.  Y axis is positive to the
    ///   right of the object, negative to the left.  Z axis is positive below the
    ///   platform, and negative above.  For example, if an object is 1 meter
    ///   in front of your vehicle, 2 meters to the right, and .5 meters above, the
    ///   local position value would be < x:1, y:2, z:-.5>.  All values are in meters.
    ///
    ///   For orientation values, a 0 Yaw value is facing north, with positive
    ///   Yaw to the right (90 degrees is East).  Yaw is a rotation around the Z
    ///   axis.  Pitch is a rotation around the local Y axis with a positive
    ///   pitch has the front pointing towards the sky, negative towards the
    ///   ground.  Roll is a rotation around the local X axis with positive
    ///   values tilting to the right side.  All orientation values are in
    ///   radians [-pi,pi]..
    ///
    ///   The Velocity and Rotation Rate values represent the motion of the
    ///   object within its local coordinate system.  Velocity values are in 
    ///   m/s.  Rotation values are in radians/s along the local axis similar to 
    ///   the Orientation axes.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CARTOGRAPHER_DLL Pose
    {
    public:
        typedef CxUtils::CircularArray<Pose> CircularArray;   
        typedef std::vector<Pose> List;
        typedef std::set<Pose> Set;

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class History
        ///   \brief Class for maintaining and searching through a history of poses. Useful
        ///          for synchronizing data among various sensors updating at different rates
        ///          with regards to pose.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL History
        {
        public:
            History(){ Initialize(); }
            ~History(){}
            void Initialize(const double ms = 1000, const double hz = 100);
            bool GetPose(const CxUtils::Time& time, Pose& pose) const;
            bool Visited(const Pose& pose, double radiusM = 1.) const;
            void Push(const Pose& pose) { CxUtils::Mutex::ScopedLock lock(&mMutex); mHistory.push_back(pose); }
        private:
            CircularArray mHistory; ///<  Pose history circular array.
            CxUtils::Mutex mMutex;  ///<  Mutex for thread protection.
        };
        Pose();
        Pose(const Point3D& position,
             const Orientation& orientation = Orientation(),
             const Point3D& velocity = Point3D(),
             const Point3D& rotation = Point3D(),
             const Time& timeStamp = Time(),
             const unsigned int id = 0);
        Pose(const Pose& pose);
        ~Pose();
        void UpdatePoseRotation();
        void ClearPoseRotation();
        bool Intersects(const Pose& endPose, const double distance);
        bool Intersects(const Point3D& endPoint, const double distance);
        inline double& X() { return mPosition.mX; }
        inline double X() const { return mPosition.mX; }
        inline double& Y() { return mPosition.mY; }
        inline double Y() const { return mPosition.mY; }
        inline double& Z() { return mRotation.mZ; }
        inline double Z() const { return mPosition.mZ; }
        inline double& Yaw() { return mRotation.mZ; }
        inline double Yaw() const { return mRotation.mZ; }
        inline double& Pitch() { return mRotation.mY; }
        inline double Pitch() const { return mRotation.mY; }
        inline double& Roll() { return mRotation.mX; }
        inline double Roll() const { return mRotation.mX; }
        Pose& operator=(const Pose& pose);
        // Routines for converting to/from map coorindates from global coordinates.
        static Point3D ConvertFromGlobalToMapCoordinates(const Wgs& position, const Wgs& origin);
        static Point3D ConvertFromGlobalToMapCoordinates(const Wgs& position, const Utm& origin);
        static Point3D ConvertFromGlobalToMapCoordinates(const Utm& position, const Utm& origin);
        static Wgs ConvertFromMapToGlobalCoordinates(const Point3D& position, const Wgs& origin);
        static Utm ConvertFromMapToGlobalCoordinates(const Point3D& position, const Utm& origin);
        // Routines to convert from coordinates local to apose to map coordinates.
        static Point3D ConvertFromLocalToMapCoordinates(const Pose& pose, const Point3D& point);
        static Point3D::List ConvertFromLocalToMapCoordinates(const Pose& pose, const Point3D::List& points);
        static Segment3D ConvertFromLocalToMapCoordinates(const Pose& pose, const Segment3D& segment);
        static Segment3D::List ConvertFromLocalToMapCoordinates(const Pose& pose, const Segment3D::List& segments);
        // Routines to convert from map coordinates to coordinates local to a pose.
        static Point3D ConvertFromMapToLocalCoordinates(const Pose& pose, const Point3D& point);
        static Point3D::List ConvertFromMapToLocalCoordinates(const Pose& pose, const Point3D::List& points);
        static Segment3D ConvertFromMapToLocalCoordinates(const Pose& pose, const Segment3D& segment);
        static Segment3D::List ConvertFromMapToLocalCoordinates(const Pose& pose, const Segment3D::List& segments);

#ifdef USE_OPENCV
        bool Draw(IplImage* image, 
                  const double pixelsPerMeter, 
                  const Point3D& mapBottomCorner,
                  const double platformLength,
                  const double platformWidth,
                  const CvScalar color = CV_RGB(255, 0, 0)) const;
#endif
        Point3D mPosition;              ///<  Position values in 3D space. (+x = East, +y = North, +z is Up).
        Point3D mLinearVelocity;        ///<  Current velocity in meters per second (x in front of vehicle, y to the right, z positive down).
        Point3D mRotationalVelocity;    ///<  Current rotation rates in radians per second.
        Orientation mRotation;          ///<  Roll, pitch, yaw values. Yaw is rotation around Z, pitch around Y, roll around X (radians).
        Time mTimeStamp;                ///<  Time Stamp associated with this Pose.
        unsigned int mID;               ///<  Optional ID number to associate with Pose data.
        CxUtils::Quaternion* mpPoseRotation; ///<  Rotation calculation from mOrientation (used to speed up coordinate conversions).
    };
}

#endif
/*  End of File */
