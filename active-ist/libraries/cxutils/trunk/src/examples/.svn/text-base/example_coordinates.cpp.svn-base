////////////////////////////////////////////////////////////////////////////////////
///
///  \file example_coordinates.cpp
///  \brief This is a sample program for testing/demonstrating use of the
///  coordinate transformations.  This program shows how to perform
///  coordinate and orientation transforms from ( latitude, longitude, elevation,
///  heading, pitch, roll) to thosed used in HLA/DIS applications which
///  use GCC ( x, y, z, psi, theta, phi ) position and orientation.  It also
///  demonstrates how to convert back from HLA/DIS coordinate frame.
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
#include <iostream>
#include "cxutils/cxutils.h"

using namespace std;
using namespace CxUtils;

int main(int argc, char **argv) 
{
    Wgs wgs;    //  World geodetic coordinates (lat, long, elevation).
    Gcc gcc;    //  GeoCentric Coordinates ( X, Y, Z).
    Point3D xyz, hpr; //  HLA/DIS orientation ( psi, theta, phi), and heading, pitch, roll.

    //  Set the starting location in the world using
    //  a latitude and longitue.  The object at the location
    //  is at a heading of 135, pitch of 20 degrees, and roll of 30 degrees.
    wgs.mLatitude = -34.9;
    wgs.mLongitude = 138.5;
    wgs.mElevation = 10000;
    //  Heading, Pitch, Roll
    hpr( CX_DEG2RAD(135), CX_DEG2RAD(20), CX_DEG2RAD(30) );

    //  Display initial values to screen.
    cout << "World Geodetic Position (WGS): " << wgs.ToString(false) << endl;
    cout << "HPR orientation values:\n";
    cout << "Degrees: X: " << CX_RAD2DEG(hpr.mX) << ", Y: " << CX_RAD2DEG(hpr.mY) << ", Z: " << CX_RAD2DEG(hpr.mZ) << endl;
    cout << "Radians: X: " << hpr.mX << ", Y: " << hpr.mY << ", Z: " << hpr.mZ << endl;

    //  Convert from [WGS, HPR] to the HLA/DIS coordinates [GCC, XYZ].
    WgsHprToGccXyz( wgs, hpr, gcc, xyz);

    cout << "World GeoCentric Position (GCC): X: " 
         << gcc.mX << ", Y: " 
         << gcc.mY << ", Z: " 
         << gcc.mZ << endl;
    cout << "DIS orientation values:\n";
    cout << "Degrees: X: " << CX_RAD2DEG(xyz.mX) << ", Y: " << CX_RAD2DEG(xyz.mY) << ", Z: " << CX_RAD2DEG(xyz.mZ) << endl;
    cout << "Radians: X: " << xyz.mX << ", Y: " << xyz.mY << ", Z: " << xyz.mZ << endl;

    //  Convert back to [WGS, HPR] from HLA/DIS coordinates [GCC, XYZ].
    GccXyzToWgsHpr( gcc, xyz, wgs, hpr );

    cout << "World Geodetic Position (WGS): Latitude: " 
         << wgs.mLatitude << ", Longitude: " 
         << wgs.mLongitude << ", Elevation: " 
         << wgs.mElevation << endl;
    cout << "HPR orientation values:\n";
    cout << "Degrees: X: " << CX_RAD2DEG(hpr.mX) << ", Y: " << CX_RAD2DEG(hpr.mY) << ", Z: " << CX_RAD2DEG(hpr.mZ) << endl;
    cout << "Radians: X: " << hpr.mX << ", Y: " << hpr.mY << ", Z: " << hpr.mZ << endl;

    Wgs wgs2;
    Utm utm;

    utm << wgs;
    wgs2 << utm;

    printf("Error: Lat: %lf, Long: %lf\n", fabs( wgs.mLatitude - wgs2.mLatitude), fabs( wgs.mLongitude - wgs.mLongitude ) );
    cout << "\n\n";
    wgs(36.12, -86.67, 1000);
    wgs2(33.94, -118.40, 1000);
    cout << "Great Circle Distance:\n";
    cout << "Origin: " << wgs.ToString() << endl;
    cout << "Destination: " << wgs2.ToString() << endl;
    cout << "Great Circle Distance: " << Wgs::GreatCircleDistance(wgs, wgs2)/1000.0 << " km\n";
    
    Utm utm2;
    utm << wgs;
    utm2 << wgs2;
    
    cout << "UTM Distance: " << Utm::Distance(utm, utm2)/1000.0 << " km\n";

    double srcAngle = CxToRadians(-40.0629);
    double destAngle = CxToRadians(140.466);
    double diff = CxToDegrees(Orientation::AngleDiff(srcAngle, destAngle));
    return 0;
}

/*  End of File */
