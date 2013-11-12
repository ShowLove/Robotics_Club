////////////////////////////////////////////////////////////////////////////////////
///
///  \file path.h
///  \brief Array of points that make up a path.
///
///  <br>Author(s): David Adams, Jonathan Mohlenhoff
///  <br>Created: 2009
///  <br>Copyright (c) 2009
///  <br>Applied Cognition and Training in Immersive Virtual Environments
///  <br>(ACTIVE) Laboratory
///  <br>Institute for Simulation and Training (IST)
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: dadams@ist.ucf.edu, jmohlenh@ist.ucf.edu
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
#ifndef _LIB__CARTOGRAPHER_PATH__H
#define _LIB__CARTOGRAPHER_PATH__H

#include "cartographer/pose.h"
#include <list>
#include <queue>
#include <stack>

namespace Cartographer
{
    class Navigator;  // Forward declaration.

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Path
    ///   \brief Structure for creating a list of Pose points representing the path 
    ///   a vehicle has taken, or should take.  This class acts in a similar 
    ///   manner to a stack where new path points are popped onto the top, but 
    ///   functions for accessing individual points exist.
    ///
    ///   Because the path class is composed of pose data structures the path is
    ///   stored in map global coordinates. For more information on the pose
    ///   data structure see pose.h.
    ///
    ///   The Path class controls how points are added or removed because it
    ///   calculates metrics about a class that can be used later for scoring the
    ///   fitness of a solution using a Fitness Function.  Metrics include:
    ///   1) Total distance path covers in meters
    ///   2) Direction path is traveling (compass heading in world coords using
    ///      linear regression)
    ///   3) A list of segments that the map is composed of.
    ///
    ////////////////////////////////////////////////////////////////////////////////////    
    class CARTOGRAPHER_DLL Path
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class FitnessFunction
        ///   \brief Class for creating fitness functions to score Path solutions.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class CARTOGRAPHER_DLL FitnessFunction
        {
        public:
            /// Calculates fitness of path, returning score [0, 1] value, 1 being good fitness.
            virtual double CalculateFitness(const Path& path, const Navigator* navinfo = NULL) = 0;
        };
        typedef std::vector<Path> List;
        typedef std::stack<Path> Stack;
        typedef std::priority_queue<Path> PriorityQueue;
        typedef Pose::List::iterator iterator;
        typedef Pose::List::const_iterator const_iterator;
        Path();
        Path(const Pose& pose);
        Path(const Path& path) { *this = path; }
        ~Path();
        void Push(const Pose& point);                                       // Adds a point to the end of the path, and updates internal metrics.
        bool Pop(Pose* point = NULL);                                       // Pops a point from the end of the path, and updates internal metrics.
        bool PopFront(Pose* pose = NULL);                                   // Pops a point from the font of the path, and updates internal metrics.
        bool GetDirection(double& dir) const;                               // Gets the direction (compass heading) of path using linear regression.
        double GetDirection() const;                                        // Gets the direction (compass heading) of path using linear regression.
        inline double GetDistance() const { return mDistance; }             // Gets total distance along path in meters.
        double GetDistanceStartToEnd() const;                               // Gets the distance between the first and last point (not path length).
        inline size_t GetNumPoints() const { return mPath.size(); }         // Number of points making up the path.
        inline Pose& First() { return mPath.front(); }                      // First point on the path.
        inline const Pose& First() const { return mPath.front(); }          // First point on the path.
        inline Pose& Last() { return mPath.back(); }                        // Last point on the path.
        inline const Pose& Last() const { return mPath.back(); }            // Last point on the path.
        inline iterator Begin() { return mPath.begin(); }                   // STL Style method for iterating.
        inline const_iterator Begin() const { return mPath.begin(); }       // STL Style method for iterating.
        inline iterator End() { return mPath.end(); }                       // STL Style method for iterating.
        inline const_iterator End() const { return mPath.end(); }           // STL Style method for iterating.
        bool Get(const unsigned int index, Pose& pose);
        bool Get(const unsigned int index, Segment3D& segment);
        iterator Get(const unsigned int index) { return mPath.begin() + index; } // STL Style method for iterating (gets point at index).
        const_iterator Get(const unsigned int index) const {return mPath.begin() + index; } // STL Style method for iterating (gets point at index).
        const Segment3D::List& GetSegments() const { return mSegments; }         // Returns a list of segments that make up the path.
        bool SetPathWidth(const double width);                                   // Sets the width of the path (applied to all segments).
        double GetPathWidth() const { return mPathWidth; }                       // Returns width of path in meters.
        bool SetFitness(const double fitness){mFitness = fitness; return true;}  // Sets the fitness score of the path [0,1].
        double GetFitness() const { return mFitness;   }                         // Returns the fitness score of the path [0,1].
        static Pose CalculateNextPoint(const Pose pose, const double distance, const double angleOffset = 0.); // Returns a point [distance] meters from the end of the path (follows last points heading).
        void Clear();
        unsigned int Size() const;
        bool Intersects(const Pose& point, const double threshold = 0.1) const;
        bool operator<(const Path& path) const { return (mFitness < path.mFitness); }  
        Path ConvertToLocalCoordinates(const Pose& pose) const;     ///< Convert to coordinates relative to pose.
        Path ConvertToMapCoordinates(const Pose& pose) const;       ///< Convert from relative to pose to map coordinates.
        Path& operator=(const Path& path)                      
        {
            if(this != &path)
            {
                mPath = path.mPath;
                mDistance = path.mDistance;
                mSegments = path.mSegments;
                mPathWidth = path.mPathWidth;
                mFitness = path.mFitness;
                mSumPoints = path.mSumPoints;
                mSumPoints2 = path.mSumPoints2;
                mSumPointsDiff = path.mSumPointsDiff;
            }
            return *this;
        }
#ifdef USE_OPENCV
        bool Draw(IplImage* image,
                  const double pixelsPerMeter,
                  const Point3D& mapBottomCorner,
                  CvScalar color = CV_RGB(255, 255, 0),bool drawText = false, bool drawNodes = false) const;
#endif
    private:
        bool UpdateLinearRegressionAdd(const Pose& point);    //Function to update linear regression variables when point is to be added
        bool UpdateLinearRegressionRemove(const Pose& point); //Function to update linear regression variables when point is to be removed
        Pose::List mPath;           ///<  Points that compose the path.
        Segment3D::List mSegments;  ///<  Segments between points that make up the path.
        double mDistance;           ///<  Length of the path in meters.
        double mPathWidth;          ///<  Width of path in meters.
        double mFitness;            ///<  Fitness value of the path [0,1].
        Point3D mSumPoints;         ///<  Summations of x,y,z values of path used in linear regression calculations.
        Point3D mSumPoints2;        ///<  Summations of x^2, y^2, z^2 values of path used in linear regression calculations.
        double mSumXY;              ///<  Summations of x*y values of path used in linear regression calculations. 
        Point3D mSumPointsDiff;     ///<  Summations of the differences in x, y, z values used in linear regression calculations.
    };
#endif
}

/*End of File*/
