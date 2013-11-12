////////////////////////////////////////////////////////////////////////////////////
///
///  \file coordinates.cpp
///  \brief This file contains software for some common coordinate
///  transformations (WGS, UTM, GCC).  It also contains methods for
///  converting HPR orientations to HLA/DIS orientation values.
///
///  Sources Used to Develop This Code:
///  Defense Mapping Agency. 1987b. DMA Technical Report: Supplement to
///  Department of Defense World Geodetic System
///  1984 Technical Report. Part I and II. Washington, DC: Defense Mapping Agency
///
///  Koks, Don. "Using Rotations to Build Aerospace Coordinate Systems,"
///  Electronic Warfare and Radar Division Systems Sciences Laboratory,
///  DSTO-TN-0640. http://dspace.dsto.defence.gov.au/dspace/handle/1947/3499
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
#include "cxutils/math/coordinates.h"
#include "cxutils/math/cxmath.h"
#include <iostream>
#include <iomanip>
#include <math.h>
#include <cstdio>
#include "cxutils/math/cxmath.h"

const double WGS_84_EQUATORIAL_RADIUS       = 6378137;
const double WGS_84_ECCENTRICITY            = 0.081819190842622;
const double WGS_84_ECCENTRICITY_SQ         = 0.00669437999014;
const double WGS_84_ECCENTRICITY_PRIME_SQ   = 0.00673949674227510014704353279764;                   ///< (WGS_84_ECCENTRICITY_SQ/(1-WGS_84_ECCENTRICITY_SQ).
const double WGS_84_SEMI_MAJOR_AXIS         = 6378137.0;                                            ///< a = Semi-Major Axis = Equitorial Radius, meters
const double WGS_84_SEMI_MINOR_AXIS         = 6356752.314245;                                       ///< b = Semi-Minor Axis = Polar Radius, meters
const double WGS_84_FLATTENING              = 1.0/298.257223563;                                    ///< f = (a-b)/a ; Flattening parameter
const double WGS_84_MAJOR_SQUARE            = WGS_84_SEMI_MAJOR_AXIS * WGS_84_SEMI_MAJOR_AXIS;      ///< a^2
const double WGS_84_MINOR_SQUARE            = WGS_84_SEMI_MINOR_AXIS * WGS_84_SEMI_MINOR_AXIS;      ///< b^2
const double WGS_84_ECCPRIME_SQUARE         = (WGS_84_MAJOR_SQUARE - WGS_84_MINOR_SQUARE)/(WGS_84_MINOR_SQUARE); ///< (a^2 - b^2)/b^2
const double EARTH_AVG_RADIUS               = 6371010;  ///<  Average radius of earth.

void CxUtils::GccXyzToWgsHpr(const CxUtils::Gcc& gcc,
                             const CxUtils::Point3D& xyz,
                             CxUtils::Wgs& wgs,
                             CxUtils::Point3D& hpr)
{
    Vector3D n0, e0, d0;
    Vector3D n1, e1, d1;
    Vector3D n, e, d;
    Vector3D x0, y0, z0;
    Vector3D x1, y1, z1;
    Vector3D x2, y2, z2;
    Vector3D x3, y3, z3;
    double w, a;
    Quaternion q;

    //  Step 1.
    //  Convert from WGS coordinates to GCC
    wgs << gcc;
    //  Step 2.
    //  Construct the North, East, Down (NED) Axes for the
    //  Local Geographic Coordinate Frame.
    /// First create these initial
    //  base vectors are equal to the unit vectors if we were
    //  located at 0 lat, 0 lon.
    n0(0, 0, 1);  //  In front of the platform is +1 on the GCC z-axis.
    e0(0, 1, 0);  //  To the right of the platform is +1 on the GCC y-axis.
    d0(-1, 0, 0); //  Down on the platform will be -1 on the GCC x-axis

    //  Now rotate them using our current latitude
    //  and longitude to have the loca NED axis at our
    //  current position.
    w = CX_DEG2RAD(wgs.mLongitude);
    a = CX_DEG2RAD(wgs.mLatitude);

    //  Create Qn0(w) and rotate to
    //  get n1, e1, d1
    q.CreateRotation( n0, w );
    n1 = n0;
    e1 = q.Rotate(e0);
    d1 = q.Rotate(d0);

    //  Create Q-e1(a) and rotate to
    //  get n, e, d
    q.CreateRotation( e1*-1, a );
    n = q.Rotate( n1 );
    e = e1;
    d = q.Rotate( d1 );

    // Step 3.
    // Set x0, y0, z0 to the X, Y, Z, ECEF's basis vectors.
    x0( 1, 0, 0 );
    y0( 0, 1, 0 );
    z0( 0, 0, 1 );

    //  Rotate around the z0 vector based
    //  on the psi/x. This will
    //  result in x1, y1, z1
    q.CreateRotation( z0, xyz.mX );
    x1 = q.Rotate( x0 ); // Results in x1
    y1 = q.Rotate( y0 ); // Results in y1
    z1 = q.Rotate( z0 ); // Results in z1
    //  Rotate about y1 using the theta/y value
    //  on each vector, resulting in
    //  vectors x2, y2, z2
    q.CreateRotation(y1, xyz.mY );
    x2 = q.Rotate( x1 ); // Results in x2
    y2 = q.Rotate( y1 ); // Results in y2
    z2 = q.Rotate( z1 ); // Results in z2
    //  Finally rotate about the phi/z along x2
    //  vector to produce final x, y, z vectors.
    q.CreateRotation(x2, xyz.mZ );
    x3 = q.Rotate( x2 ); // Results in final x
    y3 = q.Rotate( y2 ); // Results in final y
    z3 = q.Rotate( z2 ); // Results in final z

    //  Step 4.
    //  Now we can calculate the resulting
    //  Euler angles using the local NED frame and the
    //  x, y, z vectors calculated.
    x0 = n;
    y0 = e;
    z0 = d;

    hpr.mX = atan2( x3.Dot(y0), x3.Dot( x0 ) );
    hpr.mY = atan2( -1*(x3.Dot(z0)),
                    sqrt(  ( (x3.Dot(x0)) * (x3.Dot(x0)) ) +
                           ( (x3.Dot(y0)) * (x3.Dot(y0)) )
                           )
                  );
    q.CreateRotation(z0, hpr.mX);
    y2 = y1 = q.Rotate(y0);
    q.CreateRotation(y1, hpr.mY);
    z2 = q.Rotate(z0);
    hpr.mZ = atan2( y3.Dot(z2), y3.Dot(y2) );
}


void CxUtils::WgsHprToGccXyz(const CxUtils::Wgs& wgs,
                             const CxUtils::Point3D& hpr,
                             CxUtils::Gcc& gcc,
                             CxUtils::Point3D& xyz)
{
    Vector3D n0, e0, d0;
    Vector3D n1, e1, d1;
    Vector3D n, e, d;
    Vector3D x0, y0, z0;
    Vector3D x1, y1, z1;
    Vector3D x2, y2, z2;
    Vector3D x3, y3, z3;
    Quaternion q;
    double w, a;

    //  Step 1.
    //  Convert from WGS coordinates to GCC
    wgs >> gcc;
    //  Step 2.
    //  Construct the North, East, Down (NED) Axes fror the
    //  Local Geographic Coordinate Frame.
    n0(0, 0, 1);
    e0(0, 1, 0);
    d0(-1, 0, 0);

    w = CX_DEG2RAD(wgs.mLongitude);
    a = CX_DEG2RAD(wgs.mLatitude);

    //  Create Qn0(w) and rotate to
    //  get n1, e1, d1
    q.CreateRotation( n0, w );
    n1 = n0;
    e1 = q.Rotate(e0);
    d1 = q.Rotate(d0);

    //  Create Q-e1(a) and rotate to
    //  get n, e, d
    q.CreateRotation( e1*-1, a );
    n = q.Rotate( n1 );
    e = e1;
    d = q.Rotate( d1 );

    //  Now begin at the local NED
    //  coordinate frame, and then rotate
    //  based on the HPR values.
    x0 = n;
    y0 = e;
    z0 = d;
    //  Step 3.
    //  Rotate around the z0 vector based
    //  on the heading. This will
    //  result in x1, y1, z1
    q.CreateRotation( z0, hpr.mX );
    x1 = q.Rotate( x0 ); // Results in x1
    y1 = q.Rotate( y0 ); // Results in y1
    z1 = q.Rotate( z0 ); // Results in z1
    //  Rotate about y1 using the pitch value
    //  on each vector, resulting in
    //  vectors x2, y2, z2
    q.CreateRotation(y1, hpr.mY );
    x2 = q.Rotate( x1 ); // Results in x2
    y2 = q.Rotate( y1 ); // Results in y2
    z2 = q.Rotate( z1 ); // Results in z2
    //  Finally rotate about the roll along x2
    //  vector to produce final x, y, z vectors.
    q.CreateRotation(x2, hpr.mZ );
    x3 = q.Rotate( x2 ); // Results in final x
    y3 = q.Rotate( y2 ); // Results in final y
    z3 = q.Rotate( z2 ); // Results in final z

    //  Step 4.
    //  Now we can calculate the resulting
    //  Euler angles.
    x0( 1, 0, 0 );
    y0( 0, 1, 0 );
    z0( 0, 0, 1 );

    xyz.mX = atan2( x3.Dot(y0), x3.Dot( x0 ) );
    xyz.mY = atan2( -1*(x3.Dot(z0)),
                    sqrt(  ( (x3.Dot(x0)) * (x3.Dot(x0)) ) +
                           ( (x3.Dot(y0)) * (x3.Dot(y0)) )
                           )
                  );
    q.CreateRotation(z0, xyz.mX);
    y2 = y1 = q.Rotate(y0);
    q.CreateRotation(y1, xyz.mY);
    z2 = q.Rotate(z0);
    xyz.mZ = atan2( y3.Dot(z2), y3.Dot(y2) );
}


void CxUtils::WgsHprToGccXyz(const CxUtils::Wgs& wgs,
                             const CxUtils::Point3D& hpr,
                             CxUtils::Point3D& xyz)
{
    Vector3D n0, e0, d0;
    Vector3D n1, e1, d1;
    Vector3D n, e, d;
    Vector3D x0, y0, z0;
    Vector3D x1, y1, z1;
    Vector3D x2, y2, z2;
    Vector3D x3, y3, z3;
    Quaternion q;
    double w, a;

    //  Step 1.
    //  Convert from WGS coordinates to GCC (Not needed here).

    //  Step 2.
    //  Construct the North, East, Down (NED) Axes fror the
    //  Local Geographic Coordinate Frame.
    n0(0, 0, 1);
    e0(0, 1, 0);
    d0(-1, 0, 0);

    w = CX_DEG2RAD(wgs.mLongitude);
    a = CX_DEG2RAD(wgs.mLatitude);

    //  Create Qn0(w) and rotate to
    //  get n1, e1, d1
    q.CreateRotation( n0, w );
    n1 = n0;
    e1 = q.Rotate(e0);
    d1 = q.Rotate(d0);

    //  Create Q-e1(a) and rotate to
    //  get n, e, d
    q.CreateRotation( e1*-1, a );
    n = q.Rotate( n1 );
    e = e1;
    d = q.Rotate( d1 );

    //  Now begin at the local NED
    //  coordinate frame, and then rotate
    //  based on the HPR values.
    x0 = n;
    y0 = e;
    z0 = d;
    //  Step 3.
    //  Rotate around the z0 vector based
    //  on the heading. This will
    //  result in x1, y1, z1
    q.CreateRotation( z0, hpr.mX );
    x1 = q.Rotate( x0 ); // Results in x1
    y1 = q.Rotate( y0 ); // Results in y1
    z1 = q.Rotate( z0 ); // Results in z1
    //  Rotate about y1 using the pitch value
    //  on each vector, resulting in
    //  vectors x2, y2, z2
    q.CreateRotation(y1, hpr.mY );
    x2 = q.Rotate( x1 ); // Results in x2
    y2 = q.Rotate( y1 ); // Results in y2
    z2 = q.Rotate( z1 ); // Results in z2
    //  Finally rotate about the roll along x2
    //  vector to produce final x, y, z vectors.
    q.CreateRotation(x2, hpr.mZ );
    x3 = q.Rotate( x2 ); // Results in final x
    y3 = q.Rotate( y2 ); // Results in final y
    z3 = q.Rotate( z2 ); // Results in final z

    //  Step 4.
    //  Now we can calculate the resulting
    //  Euler angles.
    x0( 1, 0, 0 );
    y0( 0, 1, 0 );
    z0( 0, 0, 1 );

    xyz.mX = atan2( x3.Dot(y0), x3.Dot( x0 ) );
    xyz.mY = atan2( -1*(x3.Dot(z0)),
                    sqrt(  ( (x3.Dot(x0)) * (x3.Dot(x0)) ) +
                           ( (x3.Dot(y0)) * (x3.Dot(y0)) )
                           )
                  );
    q.CreateRotation(z0, xyz.mX);
    y2 = y1 = q.Rotate(y0);
    q.CreateRotation(y1, xyz.mY);
    z2 = q.Rotate(z0);
    xyz.mZ = atan2( y3.Dot(z2), y3.Dot(y2) );
}


using namespace CxUtils;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs::Wgs()
{
    mLatitude = mLongitude = mElevation = 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs::Wgs(const Wgs& wgs)
{
    Set(wgs.mLatitude, wgs.mLongitude, wgs.mElevation);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes from UTM coordinate.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs::Wgs(const Utm& utm)
{
    mLatitude = mLongitude = mElevation = 0;
    *this << utm;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///
////////////////////////////////////////////////////////////////////////////////////
Wgs::Wgs(const double latitude, const double longitude)
{
    Set(latitude, longitude, 0.0);
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///   \param elevation Elevation as ASL format [-10,000, 30,000] meters.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs::Wgs(const double latitude, const double longitude, const double elevation)
{
    Set(latitude, longitude, elevation);
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Set(const double latitude, const double longitude)
{
    if( latitude >= -90.0 && latitude <= 90.0 &&
        longitude >= -180.0 && longitude <= 180.0 )
    {
            mLatitude = latitude;
            mLongitude = longitude;
            return 1;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Set(const double latitude, const double longitude, const double elevation)
{
    if( latitude >= -90.0 && latitude <= 90.0 &&
        longitude >= -180.0 && longitude <= 180.0 &&
        elevation >= -10000 && elevation <= 35000)
    {
            mLatitude = latitude;
            mLongitude = longitude;
            mElevation = elevation;
            return 1;
    }

    return 0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.  Converts from degrees minutes seconds to
///   degrees.
///
///   \param latDeg Degrees portion of latitude.
///   \param latMin Minutes portion of latitude.
///   \param latSec Seconds portion of latitude.
///   \param lonDeg Degrees porition of longitude.
///   \param lonMin Minute portion of longitude.
///   \param lonSec Secons portion of longitude.
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Set(const double latDeg, const double latMin, const double latSec,
             const double lonDeg, const double lonMin, const double lonSec)
{
    double lat;
    double lon;
    if(latDeg < 0)
    {
        lat = latDeg - latMin/60.0 - latSec/3600.0;
    }
    else
    {
        lat = latDeg + latMin/60.0 + latSec/3600.0;
    }
    if(lonDeg < 0)
    {
        lon = lonDeg - lonMin/60.0 - lonSec/3600.0;
    }
    else
    {
        lon = lonDeg + lonMin/60.0 + lonSec/3600.0;
    }

    return Set(lat, lon);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.  Converts from degrees minutes seconds to
///   degrees.
///
///   \param latDeg Degrees portion of latitude.
///   \param latMin Minutes portion of latitude.
///   \param latSec Seconds portion of latitude.
///   \param lonDeg Degrees porition of longitude.
///   \param lonMin Minute portion of longitude.
///   \param lonSec Seconds portion of longitude.
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Set(const double latDeg, const double latMin, const double latSec,
             const double lonDeg, const double lonMin, const double lonSec,
             const double elevation)
{
    double lat;
    double lon;
    if(latDeg < 0)
    {
        lat = latDeg - latMin/60.0 - latSec/3600.0;
    }
    else
    {
        lat = latDeg + latMin/60.0 + latSec/3600.0;
    }
    if(lonDeg < 0)
    {
        lon = lonDeg - lonMin/60.0 - lonSec/3600.0;
    }
    else
    {
        lon = lonDeg + lonMin/60.0 + lonSec/3600.0;
    }

    return Set(lat, lon, elevation);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///
///   \return 1 if values retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Get(double& latitude, double& longitude) const
{
    latitude = mLatitude;
    longitude = mLongitude;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values.
///
///   \param latitude Latitude [-90,90].
///   \param longitude Longitude [-180, 180].
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Get(double& latitude, double& longitude, double &elevation) const
{
    latitude = mLatitude;
    longitude = mLongitude;
    elevation = mElevation;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values in degrees.minutes.seconds format.
///
///   \param latDeg Degrees portion of latitude.
///   \param latMin Minutes portion of latitude.
///   \param latSec Seconds portion of latitude.
///   \param lonDeg Degrees porition of longitude.
///   \param lonMin Minute portion of longitude.
///   \param lonSec Seconds portion of longitude.
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Wgs::Get(double& latDeg, double& latMin, double& latSec,
             double& lonDeg, double& lonMin, double& lonSec,
             double &elevation) const
{
    latDeg = (double)((int)(mLatitude));
    latMin = 60.0*(mLatitude - latDeg);
    if(latMin < 0)
        latMin *= -1.0;
    latSec = 60.0*(latMin - ( (int)(latMin) ));
    latMin = (double)((int)(latMin));

    lonDeg = (double)((int)(mLongitude));
    lonMin = 60.0*(mLongitude - lonDeg);
    if(lonMin < 0)
        lonMin *= -1.0;
    lonSec = 60.0*(lonMin - ( (int)(lonMin) ));
    lonMin = (double)((int)(lonMin));

    elevation = mElevation;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints out data to console.
///
////////////////////////////////////////////////////////////////////////////////////
void Wgs::Print() const
{
    std::cout << "WGS - Latitude: " << std::setprecision(8) << mLatitude << " Longitude: " << std::setprecision(8) << mLongitude << " Elevation: " << std::setprecision(8) << mElevation << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the WGS data to a string.
///
///   \param degreesMinutesFlag If true, data is displayed in degrees.minutes
///          format.
///
///   \return WGS data as a string.
///
////////////////////////////////////////////////////////////////////////////////////
std::string Wgs::ToString(const bool degreesMinutesFlag) const
{
    char buffer[256];
    if(degreesMinutesFlag)
    {
        double latDeg, latMin, latSec;
        double lonDeg, lonMin, lonSec;
        double elevation;
        Get(latDeg, latMin, latSec, lonDeg, lonMin, lonSec, elevation);
        sprintf(buffer, "Latitude: %d.%d.%d, Longitude: %d.%d.%d, Elevation: %lf", (int)latDeg, (int)latMin, (int)latSec, (int)lonDeg, (int)lonMin, (int)lonSec, elevation);
    }
    else
    {
        sprintf(buffer, "Latitude: %lf, Longitude: %lf, Elevation: %lf", mLatitude, mLongitude, mElevation);
    }
    return std::string(buffer);
}



////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Computes the great circle distance between two points.
///
///   \param p1 The first point.
///   \param p2 The second point.
///
///   \return Great circle distance between to GPS waypoints in meters.
///
////////////////////////////////////////////////////////////////////////////////////
double Wgs::GreatCircleDistance(const Wgs& p1, const Wgs& p2)
{
    Wgs rad1(CX_DEG2RAD(p1.mLatitude), CX_DEG2RAD(p1.mLongitude), p1.mElevation);
    Wgs rad2(CX_DEG2RAD(p2.mLatitude), CX_DEG2RAD(p2.mLongitude), p2.mElevation);
    double deltaSigma = 0.0;
    // Vincenty formual for Great Circle Distance
    double lonDelta = rad1.mLongitude - rad2.mLongitude;
    deltaSigma = atan2( sqrt( pow( cos(rad2.mLatitude) * sin(lonDelta), 2.0) +
                              pow( cos(rad1.mLatitude) * sin(rad2.mLatitude) - sin(rad1.mLatitude) * cos(rad2.mLatitude) * cos(lonDelta), 2.0) ),
                         sin( rad1.mLatitude) * sin(rad2.mLatitude) + cos(rad1.mLatitude) * cos(rad2.mLatitude) * cos(lonDelta));
    return EARTH_AVG_RADIUS*deltaSigma;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param lat Latitude.
///   \param lon Longitude.
///   \param elev Elevation..
///
////////////////////////////////////////////////////////////////////////////////////
Wgs& Wgs::operator()(const double lat, const double lon, const double elev)
{
    Set(lat, lon, elev);
    return *this;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs& Wgs::operator=(const Wgs& wgs)
{
    mLatitude = wgs.mLatitude;
    mLongitude = wgs.mLongitude;
    mElevation = wgs.mElevation;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the UTM data to WGS, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs& Wgs::operator<<(const Utm& utm)
{
    double k0 = 0.9996;
    double a = WGS_84_EQUATORIAL_RADIUS;
    double e1 = (1 - sqrt(1.0 - WGS_84_ECCENTRICITY_SQ))/(1 + sqrt(1 - WGS_84_ECCENTRICITY_SQ));
    double N1, T1, C1, R1, D, M;
    double longOrigin;
    double mu, phi1, phi1Rad;
    double x, y;
    int northernHemisphere;      //  1 for northern, 0 for sourthern.

    x = utm.mEasting - 500000.0; //  Remove 500,000 meter offset for longitude.
    y = utm.mNorthing;

    if(utm.mZoneLetter - 'N' >= 0)
        northernHemisphere = 1;  //  Point is in northern hemisphere.
    else {
        northernHemisphere = 0;  //  Point is in southern hemisphere.
        y -= 10000000.0;         //  Remove 10,000,000 meter offset used for southern hemisphere.
    }

    longOrigin = (utm.mZoneNumber - 1)*6 - 180.0 + 3; // +3 puts origin in middle of zone.

    M = y / k0;
    mu = M/(a*(1-WGS_84_ECCENTRICITY_SQ/4-3*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/64-5*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/256));

    phi1Rad = mu    + (3*e1/2-27*e1*e1*e1/32)*sin(2*mu)
                    + (21*e1*e1/16-55*e1*e1*e1*e1/32)*sin(4*mu)
                    +(151*e1*e1*e1/96)*sin(6*mu);
    phi1 = CX_RAD2DEG(phi1Rad);

    N1 = a/sqrt(1-WGS_84_ECCENTRICITY_SQ*sin(phi1Rad)*sin(phi1Rad));
    T1 = tan(phi1Rad)*tan(phi1Rad);
    C1 = WGS_84_ECCENTRICITY_PRIME_SQ*cos(phi1Rad)*cos(phi1Rad);
    R1 = a*(1-WGS_84_ECCENTRICITY_SQ)/pow(1-WGS_84_ECCENTRICITY_SQ*sin(phi1Rad)*sin(phi1Rad), 1.5);
    D = x/(N1*k0);

    this->mLatitude = phi1Rad - (N1*tan(phi1Rad)/R1)*(D*D/2-(5+3*T1+10*C1-4*C1*C1-9*WGS_84_ECCENTRICITY_PRIME_SQ)*D*D*D*D/24
                            +(61+90*T1+298*C1+45*T1*T1-252*WGS_84_ECCENTRICITY_PRIME_SQ-3*C1*C1)*D*D*D*D*D*D/720);
    this->mLatitude = CX_RAD2DEG(this->mLatitude);

    this->mLongitude = (D-(1+2*T1+C1)*D*D*D/6+(5-2*C1+28*T1-3*C1*C1+8*WGS_84_ECCENTRICITY_PRIME_SQ+24*T1*T1)
                     *D*D*D*D*D/120)/cos(phi1Rad);
    this->mLongitude = longOrigin + CX_RAD2DEG(this->mLongitude);
    this->mElevation = utm.mElevation;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the GCC data to WGS, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Wgs& Wgs::operator<<(const Gcc& gcc)
{
    double p = sqrt(gcc.mX * gcc.mX + gcc.mY * gcc.mY);
    double n1 = gcc.mZ*WGS_84_SEMI_MAJOR_AXIS;
    double d1 = p*WGS_84_SEMI_MINOR_AXIS;

    double m = sqrt(n1*n1 + d1*d1);
    n1 /= m;
    d1 /= m;

    double n2 = gcc.mZ + + WGS_84_ECCPRIME_SQUARE*WGS_84_SEMI_MINOR_AXIS * n1 * n1 * n1;
    double d2 = p - WGS_84_ECCENTRICITY_SQ*WGS_84_SEMI_MAJOR_AXIS * d1 * d1 * d1;

    m = sqrt(n2 * n2 + d2 * d2);
    n2 /= m;
    d2 /= m;

    mLatitude = atan2(n2, d2);
    mLongitude = atan2(gcc.mY, gcc.mX);
    mElevation = p / d2 - WGS_84_SEMI_MAJOR_AXIS / sqrt(1.0 - WGS_84_ECCENTRICITY_SQ * n2 * n2);

    mLatitude = CX_RAD2DEG(mLatitude);
    mLongitude = CX_RAD2DEG(mLongitude);

    if(CX_ISNAN(mElevation))
        mElevation = 0;

    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to UTM position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Wgs::operator>>(Utm& utm) const
{
    double a = WGS_84_EQUATORIAL_RADIUS;
    double k0 = 0.9996;
    double longOrigin;
    double N, T, C, A, M;
    double latRad = CX_DEG2RAD(this->mLatitude);
    double longRad = CX_DEG2RAD(this->mLongitude);
    double longOriginRad = 0.0;

    utm.mZoneNumber = (int)( (this->mLongitude + 180.0)/6.0 + 1 );
    if( this->mLatitude >= 56.0 && this->mLatitude < 64.0 && this->mLongitude >= 3.0 && this->mLongitude < 12.0 )
        utm.mZoneNumber = 32;

    longOrigin = (utm.mZoneNumber - 1)*6 - 180.0 + 3; // +3 Puts origin in middle of zone.
    longOriginRad = CX_DEG2RAD(longOrigin);

    if((84 >= this->mLatitude) && (this->mLatitude >= 72)) utm.mZoneLetter = 'X';
    else if((72 > this->mLatitude) && (this->mLatitude >= 64)) utm.mZoneLetter = 'W';
    else if((64 > this->mLatitude) && (this->mLatitude >= 56)) utm.mZoneLetter = 'V';
    else if((56 > this->mLatitude) && (this->mLatitude >= 48)) utm.mZoneLetter = 'U';
    else if((48 > this->mLatitude) && (this->mLatitude >= 40)) utm.mZoneLetter = 'T';
    else if((40 > this->mLatitude) && (this->mLatitude >= 32)) utm.mZoneLetter = 'S';
    else if((32 > this->mLatitude) && (this->mLatitude >= 24)) utm.mZoneLetter = 'R';
    else if((24 > this->mLatitude) && (this->mLatitude >= 16)) utm.mZoneLetter = 'Q';
    else if((16 > this->mLatitude) && (this->mLatitude >= 8)) utm.mZoneLetter = 'P';
    else if(( 8 > this->mLatitude) && (this->mLatitude >= 0)) utm.mZoneLetter = 'N';
    else if(( 0 > this->mLatitude) && (this->mLatitude >= -8)) utm.mZoneLetter = 'M';
    else if((-8> this->mLatitude) && (this->mLatitude >= -16)) utm.mZoneLetter = 'L';
    else if((-16 > this->mLatitude) && (this->mLatitude >= -24)) utm.mZoneLetter = 'K';
    else if((-24 > this->mLatitude) && (this->mLatitude >= -32)) utm.mZoneLetter = 'J';
    else if((-32 > this->mLatitude) && (this->mLatitude >= -40)) utm.mZoneLetter = 'H';
    else if((-40 > this->mLatitude) && (this->mLatitude >= -48)) utm.mZoneLetter = 'G';
    else if((-48 > this->mLatitude) && (this->mLatitude >= -56)) utm.mZoneLetter = 'F';
    else if((-56 > this->mLatitude) && (this->mLatitude >= -64)) utm.mZoneLetter = 'E';
    else if((-64 > this->mLatitude) && (this->mLatitude >= -72)) utm.mZoneLetter = 'D';
    else if((-72 > this->mLatitude) && (this->mLatitude >= -80)) utm.mZoneLetter = 'C';
    else utm.mZoneLetter = 'Z'; //This is here as an error flag to show that the Latitude is outside the UTM limits

    N = a/sqrt(1-WGS_84_ECCENTRICITY_SQ*sin(latRad)*sin(latRad));
    T = tan(latRad)*tan(latRad);
    C = WGS_84_ECCENTRICITY_PRIME_SQ*cos(latRad)*cos(latRad);
    A = cos(latRad)*(longRad-longOriginRad);

    M = a*((1   - WGS_84_ECCENTRICITY_SQ/4       - 3*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/64    - 5*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/256)*latRad
                - (3*WGS_84_ECCENTRICITY_SQ/8    + 3*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/32    + 45*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/1024)*sin(2*latRad)
                                     + (15*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/256 + 45*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/1024)*sin(4*latRad)
                                     - (35*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ*WGS_84_ECCENTRICITY_SQ/3072)*sin(6*latRad));

    utm.mEasting = (double)(k0*N*(A+(1-T+C)*A*A*A/6
                    + (5-18*T+T*T+72*C-58*WGS_84_ECCENTRICITY_PRIME_SQ)*A*A*A*A*A/120)
                    + 500000.0);

    utm.mNorthing = (double)(k0*(M+N*tan(latRad)*(A*A/2+(5-T+9*C+4*C*C)*A*A*A*A/24
                    + (61-58*T+T*T+600*C-330*WGS_84_ECCENTRICITY_PRIME_SQ)*A*A*A*A*A*A/720)));
    if(this->mLatitude < 0)
        utm.mNorthing += 10000000.0; //10000000 meter offset for southern hemisphere

    utm.mElevation = this->mElevation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to GCC position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Wgs::operator>>(Gcc& gcc) const
{
    double latRad, lonRad;
    double sinTheta, cosTheta;
    latRad = CX_DEG2RAD(this->mLatitude);
    lonRad = CX_DEG2RAD(this->mLongitude);
    sinTheta = sin(latRad);
    cosTheta = cos(latRad);
    double n = WGS_84_SEMI_MAJOR_AXIS/sqrt(1.0 - WGS_84_ECCENTRICITY_SQ*sinTheta*sinTheta);

    gcc.mX = gcc.mY = (n + mElevation)*cosTheta;
    gcc.mX *= cos(lonRad);
    gcc.mY *= sin(lonRad);
    gcc.mZ = (n * (1.0 - WGS_84_ECCENTRICITY_SQ) + mElevation) * sinTheta;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the values are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Wgs::operator==(const Wgs& wgs) const
{
    if( fabs( mLatitude - wgs.mLatitude ) < .000001 &&
        fabs( mLongitude - wgs.mLongitude) < .000001 &&
        fabs( mElevation - wgs.mElevation) < .000001 )
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Utm::Utm()
{
    mZoneNumber = mZoneLetter = 0;
    mNorthing = mEasting = mElevation = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Utm::Utm(const Utm& utm)
{
    mNorthing = utm.mNorthing;
    mEasting = utm.mEasting;
    mZoneNumber = utm.mZoneNumber;
    mZoneLetter = utm.mZoneLetter;
    mElevation = utm.mElevation;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Initializes from WGS coordinate.
///
////////////////////////////////////////////////////////////////////////////////////
Utm::Utm(const Wgs& wgs)
{
    mZoneNumber = mZoneLetter = 0;
    mNorthing = mEasting = mElevation = 0.0;
    *this << wgs;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor, sets values.
///
///   \param northing UTM northing.
///   \param easting UTM easting.
///   \param zoneNumber The number of the UTM zone.
///   \param zoneLetter The letter of the UTM zone.
///
////////////////////////////////////////////////////////////////////////////////////
Utm::Utm(const double northing, const double easting,
         const int zoneNumber, const int zoneLetter)
{
    mNorthing = northing;
    mEasting = easting;
    mZoneNumber = zoneNumber;
    mZoneLetter = zoneLetter;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param northing UTM northing.
///   \param easting UTM easting.
///   \param zoneNumber The number of the UTM zone.
///   \param zoneLetter The letter of the UTM zone.
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Utm::Set(const double northing, const double easting,
             const int zoneNumber, const int zoneLetter)
{
    mNorthing = northing;
    mEasting = easting;
    mZoneNumber = zoneNumber;
    mZoneLetter = zoneLetter;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets the values.
///
///   \param northing UTM northing.
///   \param easting UTM easting.
///   \param zoneNumber The number of the UTM zone.
///   \param zoneLetter The letter of the UTM zone.
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values set, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Utm::Set(const double northing, const double easting,
             const int zoneNumber, const int zoneLetter,
             const double elevation)
{
    mNorthing = northing;
    mEasting = easting;
    mZoneNumber = zoneNumber;
    mZoneLetter = zoneLetter;
    mElevation = elevation;

    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values.
///
///   \param northing UTM northing.
///   \param easting UTM easting.
///   \param zoneNumber The number of the UTM zone.
///   \param zoneLetter The letter of the UTM zone.
///
///   \return 1 if values retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Utm::Get(double& northing, double& easting,
            int& zoneNumber, int& zoneLetter) const
{
    northing = mNorthing;
    easting = mEasting;
    zoneNumber = mZoneNumber;
    zoneLetter = mZoneLetter;
    return 1;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the values.
///
///   \param northing UTM northing.
///   \param easting UTM easting.
///   \param zoneNumber The number of the UTM zone.
///   \param zoneLetter The letter of the UTM zone.
///   \param elevation Elevation around mean-sea level [-10,000,35,000].
///
///   \return 1 if values retrieved, otherwise 0.
///
////////////////////////////////////////////////////////////////////////////////////
int Utm::Get(double& northing, double& easting,
            int& zoneNumber, int& zoneLetter,
            double &elevation) const
{
    northing = mNorthing;
    easting = mEasting;
    zoneNumber = mZoneNumber;
    zoneLetter = mZoneLetter;
    elevation = mElevation;
    return 1;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints out data to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void Utm::Print() const
{
    std::cout << "UTM - Northing: " << mNorthing << " Easting: " << mEasting << " " << mZoneNumber << (char)mZoneLetter << " Elevation: " << mElevation << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Distance in meters between two points.
///
////////////////////////////////////////////////////////////////////////////////////
double Utm::Distance(const Utm& utm1, const Utm& utm2)
{
    if(utm1.mZoneNumber != utm2.mZoneNumber ||
       utm1.mZoneLetter != utm2.mZoneLetter)
    {
        // We are in different zones/letters, than the
        // coordinates are in a different frame, therfore
        // we must convert to another global system to calculate
        // distance.
        Wgs wgs1;
        Wgs wgs2;
        wgs1 << utm1;
        wgs2 << utm2;
        return Wgs::GreatCircleDistance(wgs1, wgs2);
    }
    else
    {
        return sqrt( pow(utm1.mEasting - utm2.mEasting, 2.0) +
                     pow(utm1.mNorthing - utm2.mNorthing, 2.0) +
                     pow(utm1.mElevation - utm2.mElevation, 2.0) );
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return Distance in meters between two points.
///
////////////////////////////////////////////////////////////////////////////////////
double Utm::Distance2D(const Utm& utm1, const Utm& utm2)
{
    if(utm1.mZoneNumber != utm2.mZoneNumber ||
       utm1.mZoneLetter != utm2.mZoneLetter)
    {
        // We are in different zones/letters, than the
        // coordinates are in a different frame, therfore
        // we must convert to another global system to calculate
        // distance.
        Wgs wgs1;
        Wgs wgs2;
        wgs1 << utm1;
        wgs2 << utm2;
        wgs1.mElevation = wgs2.mElevation;
        return Wgs::GreatCircleDistance(wgs1, wgs2);
    }
    else
    {
        return sqrt( pow(utm1.mEasting - utm2.mEasting, 2.0) +
                     pow(utm1.mNorthing - utm2.mNorthing, 2.0) );
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Utm& Utm::operator=(const Utm& utm)
{
    mNorthing = utm.mNorthing;
    mEasting = utm.mEasting;
    mZoneNumber = utm.mZoneNumber;
    mZoneLetter = utm.mZoneLetter;
    mElevation = utm.mElevation;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the WGS data to UTM, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Utm& Utm::operator<<(const Wgs& wgs)
{
    wgs >> *this;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the GCC data to UTM, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Utm& Utm::operator<<(const Gcc& gcc)
{
    Wgs wgs;
    wgs << gcc;
    wgs >> *this;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to Wgs position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Utm::operator>>(Wgs& wgs) const
{
    wgs << *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to GCC position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Utm::operator>>(Gcc& gcc) const
{
    Wgs wgs;
    wgs << *this;
    wgs >> gcc;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the values are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Utm::operator==(const Utm& utm) const
{
    if( mZoneLetter == utm.mZoneLetter &&
        mZoneNumber == utm.mZoneNumber &&
        fabs( mNorthing - utm.mNorthing ) < .000001 &&
        fabs( mEasting - utm.mEasting) < .000001 &&
        fabs( mElevation - utm.mElevation) < .000001 )
    {
        return true;
    }
    return false;
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc::Gcc()
{
    mX = mY = mZ = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc::Gcc(const Gcc& gcc) : Point3D(gcc.mX, gcc.mY, gcc.mZ)
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
///   \param x Distance along X axis.
///   \param y Distance along Y axis.
///   \param z Distance along Z axis.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc::Gcc(const double x, const double y, const double z)
{
    mX = x; mY = y; mZ = z;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console window.
///
////////////////////////////////////////////////////////////////////////////////////
void Gcc::Print() const
{
    std::cout << "GCC - X: " << mX << " Y: " << mY << " Z: " << mZ << std::endl;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the distance from center of Earth to GCC location.
///
///   \return Distance from center of Earth to GCC location.
///
////////////////////////////////////////////////////////////////////////////////////
double Gcc::GetMagnitude() const
{
    return sqrt(mX*mX + mY*mY + mZ*mZ);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc& Gcc::operator=(const Point3D& p3d)
{
    mX = p3d.mX;
    mY = p3d.mY;
    mZ = p3d.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc& Gcc::operator=(const Gcc& gcc)
{
    mX = gcc.mX;
    mY = gcc.mY;
    mZ = gcc.mZ;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the WGS to GCC, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc& Gcc::operator<<(const Wgs& wgs)
{
    wgs >> *this;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts the UTM to GCC, and saves it to internal data
///   members.
///
////////////////////////////////////////////////////////////////////////////////////
Gcc& Gcc::operator<<(const Utm& utm)
{
    Wgs wgs;
    wgs << utm;
    wgs >> *this;
    return *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to Wgs position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Gcc::operator>>(Wgs& wgs) const
{
    wgs << *this;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Converts to UTM position data.
///
////////////////////////////////////////////////////////////////////////////////////
void Gcc::operator>>(Utm& utm) const
{
    Wgs wgs;
    wgs << *this;
    wgs >> utm;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \return True if the values are equal, otherwise false.
///
////////////////////////////////////////////////////////////////////////////////////
bool Gcc::operator==(const Gcc& gcc) const
{
    if( fabs( mX - gcc.mX ) < .000001 &&
        fabs( mY - gcc.mY ) < .000001 &&
        fabs( mZ - gcc.mZ ) < .000001 )
    {
        return true;
    }
    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Calculates the difference between two angles.  All angle values are
///   in the range of (-PI, PI].
///
///   \param[in] angleSrc The source angle (e.g. your current heading).
///   \param[in] angleDest The destination angle (e.g. where you want to face).
///   \param[in] radiansFlag If true, values are in radians, otherwise degrees.
///
///   \return Difference between the destination angle, the source angle.
///
////////////////////////////////////////////////////////////////////////////////////
double Orientation::AngleDiff(const double angleSrc,
                              const double angleDest,
                              const bool radiansFlag)
{
    double diff = angleDest - angleSrc;

    if(radiansFlag)
    {
        while(diff > CX_PI)
        {
            diff -= CX_TWO_PI;
        }
        while(diff <= -CX_PI)
        {
            diff += CX_TWO_PI;
        }
    }
    else
    {
        while(diff > 180.0)
        {
            diff -= 360.0;
        }
        while(diff <= -180.0)
        {
            diff += 360.0;
        }
    }

    return diff;
}


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
Point3D Orientation::AngleDiff(const Point3D& angleSrc, 
                               const Point3D& angleDest,
                               const bool radiansFlag)
{
    return Point3D(AngleDiff(angleSrc.mX, angleDest.mX, radiansFlag), 
                   AngleDiff(angleSrc.mY, angleDest.mY, radiansFlag),
                   AngleDiff(angleSrc.mZ, angleDest.mZ, radiansFlag));
}

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
double Orientation::AddToAngle(const double angleSrc,
                               const double angleDiff,
                               const bool radiansFlag)
{
    double result = angleSrc + angleDiff;
    if(fabs(result) <= .00001) return 0;
    if(radiansFlag)
    {
        result = fmod(result,CxUtils::CX_TWO_PI);
        if(result > CxUtils::CX_PI)
        {
            result -= CxUtils::CX_TWO_PI;
        }
        else if (result <= -CxUtils::CX_PI)
        {
            result += CxUtils::CX_TWO_PI;
        }
    }
    else
    {
        result = fmod(result,360.);
        if(result > 180)
        {
            result -= 360;
        }
        else if (result <= -180)
        {
            result += 360;
        }
    }
    return result;

}


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
Point3D Orientation::AddAngles(const Point3D& src,
                               const Point3D& delta,
                               const bool radiansFlag)
{
    Point3D result(AddToAngle(src.mX, delta.mX, radiansFlag),
                   AddToAngle(src.mY, delta.mY, radiansFlag),
                   AddToAngle(src.mZ, delta.mZ, radiansFlag));
    return result;
}


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
double Orientation::GetGlobalAngle(const Wgs& srcPos,
                                   const Wgs& destPos,
                                   const bool radiansFlag)
{
    Utm a(srcPos);
    Utm b(destPos);
    return GetGlobalAngle(a, b, radiansFlag);
}


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
double Orientation::GetGlobalAngle(const Utm& srcPos,
                                   const Utm& destPos,
                                   const bool radiansFlag)
{
    double result = atan2(destPos.mEasting - srcPos.mEasting, destPos.mNorthing - srcPos.mNorthing);
    if(radiansFlag)
    {
        return result;
    }
    else
    {
        return CX_RAD2DEG(result);
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Prints data to console window.
///
///   \param[in] degrees If true values are printed in degrees, otherwise
///                      radians.
///
////////////////////////////////////////////////////////////////////////////////////
//void Orientation::Print(const bool degrees) const
//{
//    if(degrees)
//    {
//        Point3D temp;
//        temp(CX_RAD2DEG(mX),
//             CX_RAD2DEG(mY),
//             CX_RAD2DEG(mZ));
//        temp.Print();
//    }
//    else
//    {
//        Point3D::Print();
//    }
//}


/*  End of File */
