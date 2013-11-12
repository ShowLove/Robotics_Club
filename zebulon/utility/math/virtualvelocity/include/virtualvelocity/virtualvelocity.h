/////////////////////////////////////////////////////////////////////////////
///
/// \file virtualvelocity.h
/// \brief Creates a virtual velocity sensor from the IMU
///
/// Author(s): Michael Scherer<br>
/// Created: 4/11/2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: michael.scherer@ieee.org <br>
/// Web: http://robotics.ucf.edu <br>
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
/////////////////////////////////////////////////////////////////////////////////////

#include <cxutils/cxutils.h>
#include <compass/compass.h>
#include <cxutils/math/point3d.h>
#include <filter/filter.h>
#include <filter/rcfilter.h>
#include "imudata.h"

#ifndef _ZEBULON_UTILITY_MATH_VIRTUALVELOCITY__H
#define _ZEBULON_UTILITY_MATH_VIRTUALVELOCITY__H

namespace Zebulon
{
    namespace Utility
    {
        class VirtualVelocity : public Zebulon::Compass::Callback
        {
            public:
                VirtualVelocity();
                virtual ~VirtualVelocity();
                
                bool Initialize(bool Gravity,double DT = 0.01);
                
                /*
                bool SetOrientation(CxUtils::Point3D Orientation);
                bool SetAccelerometers(CxUtils::Point3D Acceleration);
                bool SetGyros(CxUtils::Point3d RotationalSpeed);
                */
                
                // Compass data should be set in one fell swoop,
                //  because all data is needed for calculations, and
                //  integration needs to be done as data comes in,
                //  not on the fly from the Get command, thus accel
                //  stuff has to be updated per integration, dependencies
                //  dependencies, dependencies, dependencies
                bool SetImuData(ImuData *Data);
                void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration);
                
                bool GetVelocity(CxUtils::Point3D &Velocity);
                bool GetAcceleration(CxUtils::Point3D &Acceleration);
                bool GetAngularVelocity(CxUtils::Point3D &AngularVelocity);
                
                bool ZeroVelocity();
                
                static CxUtils::Point3D RotationMatrix(CxUtils::Point3D Vector, CxUtils::Point3D Orientation);
                static CxUtils::Point3D InvRotationMatrix(CxUtils::Point3D Vector, CxUtils::Point3D Orientation);
            private:
                bool mGravityEnable;
                static const double gravity = 9.81;
                
                CxUtils::Point3D mVelocity;
                CxUtils::Point3D mAcceleration;
                CxUtils::Point3D mAngularVelocity;
                
                Filter* filterX;
                Filter* filterY;
                Filter* filterZ;
                
                Filter* filterWX;
                Filter* filterWY;
                Filter* filterWZ;
                
                Filter* filterX2;
                Filter* filterY2;
                Filter* filterZ2;
                
                Filter* filterWX2;
                Filter* filterWY2;
                Filter* filterWZ2;
                
                // Keeps track of integration time
                long long int time;
                bool initialized;
                
                //Temp Ramp filter
                static const double RAMP = 0.001;
                
                void DCFilter(double Value,double &Offset);
                
                CxUtils::Point3D mOffset;
                
                int filterFlag;
                double mDT;
                double mTimeStep;
        };
    }
}
#endif
/* End of file */
