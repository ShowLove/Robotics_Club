////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensor.h
///  \brief Contains the Range Sensor Service implementation.
///
///  <br>Author(s): Brian Valentino
///  <br>Created: 8 May 2010
///  <br>Copyright (c) 2010
///  <br>Robotics Club at UCF
///  <br>University of Central Florida (UCF)
///  <br>All rights reserved.
///  <br>Email: bmvalentino@gmail.com
///  <br>Web:  http://robotics.ucf.edu
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
#ifndef _ZEBULON_GROUND_RANGE_SENSOR_H
#define _ZEBULON_GROUND_RANGE_SENSOR_H


#include <laser/laser.h>
#include <cxutils/cxutils.h>
#include <jaus/extras/rangesensor/rangesensor.h>
#include <jaus/extras/rangesensor/rangesensorconfig.h>

namespace Zebulon
{
    namespace IGVC
    {
        ///////////////////////////////////////////////////////////////////////////////////
        ///
        ///   \class RangeSensor
        ///   \brief This class is an implementation of a Range Sensor service for
        ///          JAUS++ that uses the Laser::Callback to get Range Sensor data
        ///          as it is generated from a device (e.g. SICK LIDAR).
        ///
        ////////////////////////////////////////////////////////////////////////////////////
        class RangeSensor : public Laser::Callback,
                            public JAUS::RangeSensor
        {
        public:
            RangeSensor();
            virtual ~RangeSensor();
            // Loads settings from XML
            virtual bool LoadSettings(const std::string& settingsXML);
            // Processes range sensor data when it arrives from a device.
            virtual void ProcessLaserData(const CxUtils::Point3D::List& scan,
                                          const CxUtils::Time& timestamp,
                                          const unsigned int id);
            // Method to set position and orientation of laser sensor/device.
            void SetSensorPose(const unsigned int deviceID,
                               const CxUtils::Point3D& position,
                               const CxUtils::Point3D& orientation);
            /** Only accept laser data within this range (values are in radians [-PI, PI]. */
            void SetScanLimits(const double left, const double right)
            {
                CxUtils::Mutex::ScopedLock lock(&mPoseMutex);
                mLeftLimitRadians = left;
                mRightLimitRadians = right;
            }
        protected:
            CxUtils::Mutex mPoseMutex;                                      ///<  Mutex for thread protection of data.
            std::map<unsigned int, CxUtils::Point3D> mSensorLocation;       ///<  Current location of sensor on vehicle.
            std::map<unsigned int, CxUtils::Point3D> mSensorOrientation;    ///<  Current orientation of sensor on vehicle.
            double mLeftLimitRadians;                                       ///<  Left scan limit in radians for lasers..
            double mRightLimitRadians;                                      ///<  Right angle limit in radians for lasers.
        };
    }
}

#endif

/*end of file*/
