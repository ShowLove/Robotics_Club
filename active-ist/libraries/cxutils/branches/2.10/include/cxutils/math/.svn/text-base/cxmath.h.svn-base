////////////////////////////////////////////////////////////////////////////////////
///
///  \file cxmath.h
///  \brief Includes header files and defines values for some math operations.
///   
///  <br>Author(s): Daniel Barber
///  <br>Created: 1 May 2007
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
#ifndef __CXUTILS_MATH_UTILITIES_H
#define __CXUTILS_MATH_UTILITIES_H

#include <cmath>

#include "vector3d.h"
#include "point3d.h"
#include "segment3d.h"
#include "matrix.h"
#include "quaternion.h"
#include "coordinates.h"

#define CX_DEG2RAD(deg) ((deg)*CxUtils::CX_PI_OVER_180)    ///<  Convert degrees to radians.
#define CX_RAD2DEG(rad) ((rad)*CxUtils::CX_180_OVER_PI)    ///<  Convert radians to degrees.
#define CX_ISNAN(x) ((x) != (x))                ///<  Checks if the value is a number.
#define CX_FLOAT_EQUAL(left, right, thresh)( fabs( left - right ) < thresh )    ///<  Compare floating point numbers.


namespace CxUtils 
{
    const double CX_PI                = 3.14159265358979323846;     ///<  Value of PI.
    const double CX_HALF_PI           = 1.57079632679489661923;     ///<  PI divided by 2.0
    const double CX_FOURTH_PI         = 0.785398163397448309615;    ///<  PI diviced by 4.0   
    const double CX_PI_OVER_180       = 0.0174532925199432957692222;///<  PI divided by 180.0
    const double CX_180_OVER_PI       = 57.295779513082320876846364;///<  180 dividied by PI.
    const double CX_TWO_PI            = 6.28318530717958647692;     ///<  PI * 2.
    const double CX_EPSILON           = .00000000000000000000001;   ///<  Small number to prevent divide by 0.
    
#ifdef WIN32
    #if _MSC_VER > 1400
    const double PI                   = CX_PI;                      ///<  Value of PI.
    const double Epsilon              = CX_EPSILON;                 ///<  Small number to prevent divide by 0.
    #endif
#else
    const double PI                   = CX_PI;                      ///<  Value of PI.
    const double Epsilon              = CX_EPSILON;                 ///<  Small number to prevent divide by 0.
#endif
    
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Converts from degrees to radians.
    ///
    ///   \param deg Angle in degrees.
    ///
    ///   \return Degrees value converted to radians.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    inline double CxToRadians(const double deg) 
    {
        return deg*CX_PI_OVER_180;
    }
    ////////////////////////////////////////////////////////////////////////////////////
    ///
    ///   \brief Converts from radians to degrees.
    ///
    ///   \param rad Angle in radians.
    ///
    ///   \return Radians angle value converted to degrees.
    ///
    ////////////////////////////////////////////////////////////////////////////////////
    inline double CxToDegrees(const double rad)
    {
        return rad*CX_180_OVER_PI;
    }
}



#endif
/*  End of File */
