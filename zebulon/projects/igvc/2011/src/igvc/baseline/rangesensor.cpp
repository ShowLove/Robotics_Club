////////////////////////////////////////////////////////////////////////////////////
///
///  \file rangesensor.cpp
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
#include "igvc/baseline/rangesensor.h"
#include <xmlconfig/xmlconfigurator.h>

using namespace Zebulon;
using namespace IGVC;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::RangeSensor()
{
    mLeftLimitRadians = -CxUtils::CxToRadians(270/2.0);
    mRightLimitRadians = CxUtils::CxToRadians(270/2.0);
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::~RangeSensor()
{
    
}


/** Loads XML settings from FILE. */
bool RangeSensor::LoadSettings(const std::string& settings)
{
    Configuration::XmlConfigurator xmlConf;
    if(xmlConf.Load(settings, "JAUS"))
    {
        if(xmlConf.GetVar("RangeSensor.ScanRange@left_limit", mLeftLimitRadians, true))
        {
            mLeftLimitRadians = CxUtils::CxToRadians(mLeftLimitRadians);
        }
        if(xmlConf.GetVar("RangeSensor.ScanRange@right_limit", mRightLimitRadians, true))
        {
            mRightLimitRadians = CxUtils::CxToRadians(mRightLimitRadians);
        }
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new data is available, stored in a vector in 
///          polar coordinates.
///
///   The coordinate frame for a laser is as follows: X positive in front, Y
///   positive to the right, Z positive down.  A positive Z rotation is a
///   rotation to the right, a positive Y rotation is a tilt up.
///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
///
///   Data is saved to JAUS service when it arrives.
///
///   \param[in] scan Laser scan data in polar coordinates relative to the
///                   laser.  Values are as follows: X = range/distance in
///                   meters, Y = pitch [-pi/2, pi/2], Z = yaw [-PI,PI].
///                   values are in order from left to right.
///   \param[in] timestamp Time when the data was captured (UTC).
///   \param[in] id Laser ID.
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::ProcessLaserData(const CxUtils::Point3D::List& scan,
                                   const CxUtils::Time& timestamp,
                                   const unsigned int id)
{
    CxUtils::Point3D::List::const_iterator s;
    CxUtils::Point3D::List cropped;
    for(s = scan.begin(); s != scan.end(); s++)
    {
        if(s->mZ >= mLeftLimitRadians && s->mZ <= mRightLimitRadians)
        {
            cropped.push_back(*s);
        }
    }
    mPoseMutex.Lock();
    SetLocalRangeScan(id, mSensorLocation[id], mSensorOrientation[id], cropped, timestamp);
    mPoseMutex.Unlock();
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Event called when new data is available, stored in a vector in 
///          polar coordinates.
///
///   The coordinate frame for a laser is as follows: X positive in front, Y
///   positive to the right, Z positive down.  A positive Z rotation is a
///   rotation to the right, a positive Y rotation is a tilt up.
///   A Z value of 0.0 is the front of the laser, 90 degrees the right, etc.
///
///   Data is saved to JAUS service when it arrives.
///
///   \param[in] deviceID ID of the laser.
///   \param[in] position Position of the laser on the robot, in
///                       local coordinate frame (x+ front, y+ right, z+ down).
///   \param[in] orientation Orientation of laser at that point (r, p, y).
///
////////////////////////////////////////////////////////////////////////////////////
void RangeSensor::SetSensorPose(const unsigned int deviceID,
                                const CxUtils::Point3D& position,
                                const CxUtils::Point3D& orientation)
{

    mPoseMutex.Lock();
    mSensorLocation[deviceID] = position;
    mSensorOrientation[deviceID] = orientation;
    mPoseMutex.Unlock();
}


/* End of File */
