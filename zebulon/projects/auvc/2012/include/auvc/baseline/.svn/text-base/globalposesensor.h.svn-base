////////////////////////////////////////////////////////////////////////////////////
///
///  \file globalposesensor.h
///  \brief This file contains software for creating a JAUS Global Pose
///  Sensor component (ID 38) using microstrain gx1 compass.
///
/// Author(s): David Adams<br>
/// Created: 2011<br>
/// Copyright (c) 2011<br>
/// Modified By: John Reeder<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF) <br>
/// Email: dadams@ist.ucf.edu <br>
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
////////////////////////////////////////////////////////////////////////////////////

#ifndef __ZEBULON_UNDERWATER_GLOBALPOSESENSOR__H
#define __ZEBULON_UNDERWATER_GLOBALPOSESENSOR__H

#include <jaus/mobility/sensors/globalposesensor.h>
#include <compass/compass.h>
#include <gx3/gx3.h>
#include <keller/keller.h>

namespace Zebulon
{
    namespace Underwater
    {
        ////////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class GlobalPoseSensor
        ///   \brief Jaus component that collects data from the Generic compass.
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class GlobalPoseSensor : public JAUS::GlobalPoseSensor,
                                //public Zebulon::Compass::Callback,
                                 public Zebulon::INS::MicroStrain::Callback,
                                 public Zebulon::Depth::Callback
        {
        public:
            GlobalPoseSensor();
            ~GlobalPoseSensor();
            virtual void ProcessDepthData(const double depth);
            virtual void ProcessIMUData(const CxUtils::Point3D attitude, const CxUtils::Point3D gyroscope, const CxUtils::Point3D acceleration);
            void PrintStatus() const;

            void SetCompassMountOffset(double yaw,double pitch,double roll);
            void SetDepthSensorOffset(double offset);

            void SetLiveOffset(double Offset);
        private:
            double mOffset;
            double mDepthOffset;
            double mYawOffset,mPitchOffset,mRollOffset;
            CxUtils::Mutex mMutex;
        };
    }
}
#endif

/*  End of File */
