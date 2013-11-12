////////////////////////////////////////////////////////////////////////////////////
///
/// \file objectscan.h
/// \brief Builds large objects from collections of points within LIDAR data.
///
/// Author(s): Gary Stein<br>
/// Created: 2010<br>
/// Copyright (c) 2011<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org<br>
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
#ifndef ZEBULON_AI_STATE_MACHINE_OBJECTSCAN_H
#define ZEBULON_AI_STATE_MACHINE_OBJECTSCAN_H

#include <cxutils/cxutils.h>
#include "statemachine/libstatemachinedll.h"
#include "statemachine/utility/utility.h"

namespace Zebulon
{
    namespace AI
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class ObjectScan
        ///
        ///   Given a set of range scan points, it will generate large objects based
        ///   points near each other.  Useful to try and find objects like a buoy.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class ZEB_STATEMACHINE_UTIL_DLL  ObjectScan
        {
        public:
            enum GetType
            {
                LEFTTORIGHT, // All data
                CLOSEST,     // Closest objects
                BIGGEST      // Bigest
            };
            ObjectScan();
            ~ObjectScan();
            void CalculatePolar(const CxUtils::Point3D::List& rangeScan);
            void CalculateCartesian(const CxUtils::Point3D::List& rangeScan,
                                    bool giveRadius = false,    // Give predicated radius of object based on number of points connected.
                                    double angleRange=270,      // Total scan angle (degrees)
                                    double startAngle = -135,   // Starting angle of scan on platform (where to start) (degrees)
                                    double endAngle = 135);     // Ending angle of scan on platform (where to stop) (degrees)
            void Setup(double distanceThreshold,
                       double derivativeThreshold,
                       double maxDistance,
                       double minDistance = 0.075);
            CxUtils::Point3D::List GetObjects(bool &valid, const GetType type=LEFTTORIGHT);
            
            void SimpleSetup(double maxDistance,double minDistance, double angleRange, double startAngle, double endAngle);
            double SimpleDistance(const CxUtils::Point3D::List& rangeScan, bool &valid);
            
        protected:
            CxUtils::Point3D::List mRangeScan;  ///<  Range scan data.
            CxUtils::Point3D::List mObjects;    ///<  Objects found.
            CxUtils::Point3D::List Sort(GetType type);
            double mDistanceThreshold;      ///<  Distance threshold between points.
            double mDerivativeThreshold;    ///<  Threshold for taking the derivative.
            double mMaxDistance;            ///<  Maximum distance away an object can be.
            double mMinDistance;            ///<  Minimum distance an object can be.
            
            double mStartAngle;
            double mEndAngle;
            double mAngleRange;
        };

    }

}

#endif
/*  End of File */
