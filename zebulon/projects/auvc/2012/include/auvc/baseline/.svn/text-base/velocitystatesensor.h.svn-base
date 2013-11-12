/////////////////////////////////////////////////////////////////////////////////////
///
/// \file velocitystatesensor.h
/// \brief This class is the Velocity State Sensor service for the ASV, and
///          it combines data received from the vehicles compass and GPS. to output
///          an estimation of the current velocity of the vehicle.
///
/// <br>Author(s): Michael Scherer<br>
/// <br>Created: 2/27/2010<br>
/// <br>Copyright (c) 2010<br>
/// <br>Robotics Laboratory and Club<br>
/// <br>University of Central Florida (UCF) <br>
/// <br>Email: michael.scherer@ieee.org <br>
/// <br>Web: http://robotics.ucf.edu <br>
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
#ifndef __ZEBULON_UNDERWATER_VELOCITYSTATESENSOR__H
#define __ZEBULON_UNDERWATER_VELOCITYSTATESENSOR__H

#include <stdio.h>
#include <jaus/mobility/sensors/velocitystatesensor.h>
#include <compass/compass.h>
#include <gx3/gx3.h>
#include <cxutils/math/cxmath.h>
#include <cxutils/mutex.h>

#define USE_IMU_VELOCITY

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class VelocityStateSensor
        ///   \brief This class is the Velocity State Sensor service for the ASV, and
        ///          it combines data received from the vehicles compass and GPS. to output
        ///          an estimation of the current velocity of the vehicle.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class VelocityStateSensor : public JAUS::VelocityStateSensor,
                                    //public Compass::Callback
                                    public INS::MicroStrain::Callback
        {
        public:
            VelocityStateSensor();
            ~VelocityStateSensor();
            virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope,const CxUtils::Point3D acceleration);
            virtual void ProcessVelocityData(const CxUtils::Point3D velocity);
        private:
            JAUS::ReportVelocityState mVelState;
                       
            FILE* mLogFile;

            CxUtils::Point3D mAccel;
            CxUtils::Point3D mVelocity;

            //Play with some filters
            CxUtils::Point3D LPAccel;
            CxUtils::Point3D HPAccel;
            CxUtils::Point3D OldAccel;
            CxUtils::Point3D LPVelocity;
            CxUtils::Point3D HPVelocity;

            int VelocityCount;

            bool mFirstTime;

            
            CxUtils::Mutex mVelocityMutex;
        };
    }
}

#endif // __ZEBULON_AUVC_VELOCITY_STATE_SENSOR__H
/*  End of File */
