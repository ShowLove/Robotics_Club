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
#include "auvc/baseline/rangesensor.h"

using namespace Zebulon;
using namespace Underwater;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::RangeSensor()
{
}

////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RangeSensor::~RangeSensor()
{
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
    mPoseMutex.Lock();
    SetLocalRangeScan(id, mSensorLocation[id], mSensorOrientation[id], scan, timestamp);
    mMyScan = scan;
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
///   \param[in] scan Laser scan data in polar coordinates relative to the
///                   laser.  Values are as follows: X = range/distance in
///                   meters, Y = pitch [-pi/2, pi/2], Z = yaw [-PI,PI].
///                   values are in order from left to right.
///   \param[in] timestamp Time when the data was captured (UTC).
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

void RangeSensor::PrintStatus() const
{
    //std::cout << "I'm a laser" << std::endl;
    mPoseMutex.Lock();
    int count = 0;
    for(int i = 0; i < mMyScan.size(); i++)
    {
        //std::cout << "scan: " << mMyScan[i].mX << std::endl;
        count++;
    }
    mPoseMutex.Unlock();
    
    std::cout << "Laser Scan Count: " << count << std::endl;
}


/* End of File */
