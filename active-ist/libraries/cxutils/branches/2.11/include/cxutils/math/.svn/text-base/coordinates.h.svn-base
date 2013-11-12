////////////////////////////////////////////////////////////////////////////////////
///
///  \file coordinates.h
///  \brief This file contains software for some common coordinate
///  transformations (WGS, UTM, GCC).  It also contains methods for
///  converting HPR orientations to HLA/DIS orientation values.
///
///   Sources Used to Develop This Code:
///   Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to 
///   Department of Defense World Geodetic System
///   1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency
///
///   Koks, Don. "Using Rotations to Build Aerospace Coordinate Systems,"
///   Electronic Warfare and Radar Division Systems Sciences Laboratory, 
///   DSTO-TN-0640. http://dspace.dsto.defence.gov.au/dspace/handle/1947/3499
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 30 April 2007
///  <br>Copyright (c) 2009
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
#ifndef __CXUTILS_MATH_COORDINATES_H
#define __CXUTILS_MATH_COORDINATES_H

#include "point3d.h"
#include "quaternion.h"

namespace CxUtils
{
    class Wgs;  //  Forward declaration for World-Geodetic System structure.
    class Utm;  //  Forward declaration for Universtal-Transmercator position structure.
    class Gcc;  //  Forward declaration for Geo-Centric Coordinates position structure.
    class Orientation;  // Forward declaration of orientation data structure.
   
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Wgs
    ///   \brief Simple World Geodetic System (WGS) latitude and longitude 
    ///   coordinate system data structure.
    ///
    ///   All coordinate conversions are done using WGS84 values.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Wgs
    {
    public:
        typedef std::vector<Wgs> List;
        Wgs();
        Wgs(const Wgs& wgs);
        Wgs(const Utm& utm);
        Wgs(const double latitude, const double longitude);
        Wgs(const double latitude, const double longitude, const double elevation);
        int Set(const double latitude, const double longitude);
        int Set(const double latitude, const double longitude, const double elevation);
        int Set(const double latDeg, const double latMin, const double latSec,
                const double lonDeg, const double lonMin, const double lonSec);
        int Set(const double latDeg, const double latMin, const double latSec,
                const double lonDeg, const double lonMin, const double lonSec,
                const double elevation);
        int Get(double& latitude, double& longitude) const;
        int Get(double& latitude, double& longitude, double &elevation) const;
        int Get(double& latDeg, double& latMin, double& latSec, 
                double& lonDeg, double& lonMin, double& lonSec, 
                double& elevation) const;
        void Print() const;
        std::string ToString(const bool degreesMinutesFlag = false) const;
        static double GreatCircleDistance(const Wgs& p1, const Wgs& p2);
        Wgs& operator()(const double lat, const double lon, const double elev);
        Wgs& operator=(const Wgs& wgs);
        Wgs& operator<<(const Utm& utm);
        Wgs& operator<<(const Gcc& gcc);
        void operator>>(Utm& utm) const;
        bool operator==(const Wgs& wgs) const;
        inline void operator>>(Gcc& gcc) const;
        double mLatitude;       ///<  Latittude in degrees [-90,90].
        double mLongitude;      ///<  Longitude in degrees [-180,180].
        double mElevation;      ///<  Elevation based on mean-sea level [-10,000, 35,000].
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Utm
    ///   \brief Simple Universal Transverse Mercator (UTM) coordinate 
    ///   system data structure.
    ///
    ///   UTM Northing is positive traveling northward, and Easting is positive 
    ///   traveling eastword.  There are no negative values in UTM.  Units are
    ///   measured in meters.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Utm
    {
    public:
        typedef std::vector<Utm> List;
        Utm();
        Utm(const Utm& utm);
        Utm(const Wgs& wgs);
        Utm(const double northing, const double easting,
            const int zoneNumber, const int zoneLetter);
        int Set(const double northing, const double easting,
                const int zoneNumber, const int zoneLetter);
        int Set(const double northing, const double easting,
                const int zoneNumber, const int zoneLetter, 
                const double elevation);
        int Get(double& northing, double& easting,
                int& zoneNumber, int& zoneLetter) const;
        int Get(double& northing, double& easting,
                int& zoneNumber, int& zoneLetter,
                double& elevation) const;
        void Print() const;
        inline Utm& operator()(const double northing, const double easting, 
                               const int zoneNumber, const int zoneLetter,
                               const double elevation = 0)
        {
            Set(northing, easting, zoneNumber, zoneLetter, elevation);
            return *this;
        }   
        static double Distance(const Utm& utm1, const Utm& utm2);
        static double Distance2D(const Utm& utm1, const Utm& utm2);
        static bool WithinRange(const Utm& utm1, const Utm& utm2, const double thresh)
        {
            return Distance(utm1, utm2) <= thresh ? true : false;
        }
        static bool WithinRange2D(const Utm& utm1, const Utm& utm2, const double thresh)
        {
            return Distance2D(utm1, utm2) <= thresh ? true : false;
        }
        Utm& operator=(const Utm& utm);
        Utm& operator<<(const Wgs& wgs);
        Utm& operator<<(const Gcc& gcc);
        void operator>>(Gcc& gcc) const;
        void operator>>(Wgs& wgs) const;
        bool operator==(const Utm& utm) const;
        int mZoneNumber;          ///<  UTM Longitude Zone.
        int mZoneLetter;          ///<  UTM Latitude Zone.
        double mNorthing;         ///<  UTM Northing.
        double mEasting;          ///<  UTM Easting.
        double mElevation;        ///<  Elevation based on mean-sea level [-10,000, 35,000].
    };

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Gcc
    ///   \brief Simple Geo-Centric Coordinate (GCC) system data structure.
    ///
    ///   GCC position is measured from the center of the earth with a 
    ///   positive X-axis passing through the prime meridian at the equator, positive
    ///   Y-axis passing through 90 degree east longitude at the equator and a
    ///   positive Z-axis passing through the north pole.  
    ///
    ///   A distance of one unit measured in GCC coordinates is equal to a distance
    ///   of 1 meter.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Gcc : public Point3D
    {
    public:
        typedef std::vector<Gcc> List;
        Gcc();
        Gcc(const Gcc& gcc);
        Gcc(const double x, const double y, const double z);
        void Print() const;
        double GetMagnitude() const;
        operator Point3D() const { return *((Point3D*)(this)); }
        inline Gcc& operator()(const double x, const double y, const double z)
        {
            mX = x; mY = y; mZ = z;
            return *this;
        }
        Gcc& operator=(const Point3D& p3d);
        Gcc& operator=(const Gcc& gcc);
        bool operator==(const Gcc& gcc) const;
        Gcc& operator<<(const Wgs& wgs);
        Gcc& operator<<(const Utm& utm);
        void operator>>(Wgs& wgs) const;
        void operator>>(Utm& utm) const;
    };


    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \class Orientation
    ///   \brief Simple class for doing angle difference or other needed orientation
    ///          methods.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    class CX_UTILS_DLL Orientation
    {
    public:
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Calculates the difference between two angles.  All angle values are
        ///   `      in the range of (-PI, PI].
        ///
        ///   \param[in] angleSrc The source angle (e.g. your current heading).
        ///   \param[in] angleDest The destination angle (e.g. where you want to face).
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return Difference between the destination angle, the source angle.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static double AngleDiff(const double angleSrc, 
                                const double angleDest,
                                const bool radiansFlag = true);

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Calculates the difference between multiple angles.  All angle values
        ///          are in the range of (-PI, PI].
        ///
        ///   \param[in] angleSrc The source angles (e.g. your current heading).
        ///   \param[in] angleDest The destination angles (e.g. where you want to face).
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return Difference between the destination angles, the source angles.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static Point3D AngleDiff(const Point3D& angleSrc, 
                                 const Point3D& angleDest,
                                 const bool radiansFlag = true);

        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Add two angles, calculate the result between (-PI, PI].
        ///
        ///   \param[in] angleSrc The source angle (e.g. your current heading).
        ///   \param[in] angleDiff The angle you want to add.
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return The result of the addition between (-PI, PI].
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static double AddToAngle(const double angleSrc,
                                 const double angleDiff,
                                 const bool radiansFlag = true);
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Add two angles, calculate the result between (-PI, PI].
        ///
        ///   \param[in] src The source angle (e.g. your current heading).
        ///   \param[in] delta The angle you want to add.
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return The result of the addition between (-PI, PI].
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static Point3D AddAngles(const Point3D& src,
                                 const Point3D& delta,
                                 const bool radiansFlag = true);
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Calculates the compass heading from one point to another.
        ///
        ///   \param[in] srcPos The starting position.
        ///   \param[in] destPos Destination position.
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return The angle to drive at to hit the destination point.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static double GetGlobalAngle(const Wgs& srcPos,
                                     const Wgs& destPos,
                                     const bool radiansFlag = true);
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \brief Calculates the compass heading from one point to another.
        ///
        ///   \param[in] srcPos The starting position.
        ///   \param[in] destPos Destination position.
        ///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
        ///
        ///   \return The angle to drive at to hit the destination point.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        static double GetGlobalAngle(const Utm& srcPos,
                                     const Utm& destPos,
                                     const bool radiansFlag = true);
    };


    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Converts from a GCC position and DIS/XYZ orientation to a 
    ///   WGS and Heading-Pitch-Roll orientation.
    ///
    ///   Use this function to convert Distributed Interactive Simulation (DIS) 
    ///   coordinates to Geodetic coordinates (lat, long, elevation, heading,
    ///   pitch, roll).
    ///
    ///   \param gcc Geocentric Coordinates (x, y, z).
    ///   \param xyz The three Euler rotations about the x, then y, then z
    ///              axis.  This is in the Distributed Interactive Simulation (DIS)
    ///              format. All are [-pi, pi]. Where z is the angle psi which is
    ///              a rotation around the z-axis, theta/y is rotation around y, and
    ///              sigma/x is the rotation around the x-axis.
    ///   \param wgs WGS coordinates (latitude, longitude, elevation).
    ///   \param hpr Heading, pitch, and roll, where 0 heading is facing north
    ///              0 (rotation around z-axis), pitch is level with the surface of the
    ///              earth (rotation around y-axis, with y-axis positive to the right of
    ///              the object), and roll is also level with the ground (rotation
    ///              around x-axis with x-axis positive in front of the object).  
    ///              The z-axis is positive below the object.  In the Point3D object
    ///              the values are x=heading, y=pitch, z=roll in radians [-pi,pi].
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void GccXyzToWgsHpr(const Gcc& gcc, const Point3D& xyz, Wgs& wgs, Point3D& hpr);

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Converts from a WGS position and Heading-Pitch-Roll orientation to a 
    ///   GCC position and DIS/XYZ orientation.
    ///
    ///   Use this function to convert Geodetic position and head/pitch/roll data
    ///   to Distributed Interactive Simulation (DIS) coordinates 
    ///   (X,Y,Z, psi, theta, phi).
    ///
    ///   \param wgs WGS coordinates (latitude, longitude, elevation).
    ///   \param hpr Heading, pitch, and roll, where 0 heading is facing north
    ///              0 (rotation around z-axis), pitch is level with the surface of the
    ///              earth (rotation around y-axis, with y-axis positive to the right of
    ///              the object), and roll is also level with the ground (rotation
    ///              around x-axis with x-axis positive in front of the object).  
    ///              The z-axis is positive below the object.  In the Point3D object
    ///              the values are x=heading, y=pitch, z=roll in radians [-pi,pi].
    ///   \param gcc Geocentric Coordinates (x, y, z).
    ///   \param xyz The three Euler rotations about the x, then y, then z
    ///              axis.  This is in the Distributed Interactive Simulation (DIS)
    ///              format. All are [-pi, pi]. Where z is the angle psi which is
    ///              a rotation around the z-axis, theta/y is rotation around y, and
    ///              sigma/x is the rotation around the x-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void WgsHprToGccXyz(const Wgs& wgs, const Point3D& hpr, Gcc& gcc, Point3D& xyz);

    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Converts Heading-Pitch-Roll orientation to a GCC position 
    ///   XYZ orientation at a geodetic location.
    ///
    ///   Use this function to convert Geodetic head/pitch/roll data
    ///   to Distributed Interactive Simulation (DIS) orientation 
    ///   (x/psi, y/theta, z/phi).
    ///
    ///   \param wgs WGS coordinates (latitude, longitude, elevation).
    ///   \param hpr Heading, pitch, and roll, where 0 heading is facing north
    ///              0 (rotation around z-axis), pitch is level with the surface of the
    ///              earth (rotation around y-axis, with y-axis positive to the right of
    ///              the object), and roll is also level with the ground (rotation
    ///              around x-axis with x-axis positive in front of the object).  
    ///              The z-axis is positive below the object.  In the Point3D object
    ///              the values are x=heading, y=pitch, z=roll in radians [-pi,pi].
    ///   \param xyz The three Euler rotations about the x, then y, then z
    ///              axes.  This is in the Distributed Interactive Simulation (DIS)
    ///              format. All are [-pi, pi]. Where z is the angle psi which is
    ///              a rotation around the z-axis, theta/y is rotation around y, and
    ///              sigma/x is the rotation around the x-axis.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    CX_UTILS_DLL void WgsHprToGccXyz(const Wgs& wgs, const Point3D& hpr, Point3D& xyz);

    inline void WgsToUtm(const Wgs& wgs, Utm& utm) { utm << wgs; }    ///<  Convert WGS position to UTM position.
    inline void WgsToGcc(const Wgs& wgs, Gcc& gcc) { gcc << wgs; }    ///<  Convert WGS position to GCC position.
    inline void UtmToWgs(const Utm& utm, Wgs& wgs) { wgs << utm; }    ///<  Convert UTM position to WGS position.
    inline void UtmToGcc(const Utm& utm, Gcc& gcc) { gcc << utm; }    ///<  Convert UTM position to GCC position.
    inline void GccToWgs(const Gcc& gcc, Wgs& wgs) { wgs << gcc; }    ///<  Convert GCC position to WGS position.
    inline void GccToUtm(const Gcc& gcc, Utm& utm) { utm << gcc; }    ///<  Convert GCC position to UTM position.
}

#endif //  __COORDINATES_H

/*  End of File */
