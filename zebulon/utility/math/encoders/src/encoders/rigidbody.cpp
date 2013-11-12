////////////////////////////////////////////////////////////////////////////////////
///
/// \file rigidbody.cpp
/// \brief Data structure for calculating linear and rotational velocities
///        of a rigid body from encoder data.
///
/// Author(s): Daniel Barber<br>
/// Created: 2010<br>
/// Copyright (c) 2010<br>
/// Robotics Laboratory and Club<br>
/// University of Central Florida (UCF)<br>
/// Email: dbarber@ist.ucf.edu<br>
/// Web: http://robotics.ucf.edu <br>
/// All Rights Reserved <br>
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
#include "encoders/rigidbody.h"

using namespace Zebulon;
using namespace Utility;
using namespace Encoders;


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RigidBody::RigidBody()
{
    mDirtyFlag = false;
    mWidth = 1.0;
    mUpdateCounter = 0;
    mLeftSideDist = mRightSideDist = 0.0;
    mLeftSideVelocity = mRightSideVelocity = 0.0;
    mSamplePeriodSeconds = 1.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Copy constructor.
///
////////////////////////////////////////////////////////////////////////////////////
RigidBody::RigidBody(const RigidBody& body)
{
    *this = body;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Destructor.
///
////////////////////////////////////////////////////////////////////////////////////
RigidBody::~RigidBody()
{
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Update encoder data for a side of the rigid body.
///
///   \param[in] encoderID Encoder ID number.
///   \param[in] encoderData Encoder values.
///
////////////////////////////////////////////////////////////////////////////////////
void RigidBody::UpdateLeftEncoder(const unsigned int encoderID,
                                  const Encoder& encoderData)
{
    if(mLeftSide.find(encoderID) == mLeftSide.end())
    {
        mLeftSide[encoderID] = encoderData;
    }
    else
    {
        mLeftSide[encoderID] += encoderData;
    }
    mDirtyFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Update encoder data for a side of the rigid body.
///
///   \param[in] encoderID Encoder ID number.
///   \param[in] encoderData Encoder values.
///
////////////////////////////////////////////////////////////////////////////////////
void RigidBody::UpdateRightEncoder(const unsigned int encoderID,
                                   const Encoder& encoderData)
{
    if(mRightSide.find(encoderID) == mRightSide.end())
    {
        mRightSide[encoderID] = encoderData;
    }
    else
    {
        mRightSide[encoderID] += encoderData;
    }
    mDirtyFlag = true;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Resets all values but width.
///
////////////////////////////////////////////////////////////////////////////////////
void RigidBody::Reset()
{
    mDirtyFlag = false;
    mLeftSide.clear();
    mRightSide.clear();
    mUpdateCounter = 0;
    mLeftSideDist = mRightSideDist = 0.0;
    mLeftSideVelocity = mRightSideVelocity = 0.0;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Updates internal values using current encoder data values.
///
///   \return True on success (enough values) false otherwise.
///
////////////////////////////////////////////////////////////////////////////////////
bool RigidBody::Update()
{
    CxUtils::Time currentTime(true);
    mLeftSideDist = mRightSideDist = 0.0;
    mLeftSideVelocity = mRightSideVelocity = 0.0;
    int leftValues = 0, rightValues = 0;
    Encoder::Map::iterator e;
    double longestPeriod = 0.0;

    for(e = mLeftSide.begin(); e != mLeftSide.end(); e++)
    {
        if(e->second.mSamplePeriodMs > longestPeriod)
        {
            longestPeriod = (double)e->second.mSamplePeriodMs;
        }
        Encoder copy = e->second;
        mLeftSideDist += e->second.ToMeters();
        mLeftSideVelocity += mLeftSideDist/(e->second.mSamplePeriodMs/1000.0);
        leftValues++;
    }

    for(e = mRightSide.begin(); e != mRightSide.end(); e++)
    {
        if(e->second.mSamplePeriodMs > longestPeriod)
        {
            longestPeriod = (double)e->second.mSamplePeriodMs;
        }
        Encoder copy = e->second;
        mRightSideDist += e->second.ToMeters();
        mRightSideVelocity += mRightSideDist/(e->second.mSamplePeriodMs/1000.0);
        rightValues++;
    }

    if(leftValues > 0 && rightValues > 0)
    {
        mLeftSideDist /= leftValues;
        mRightSideDist /= rightValues;
        mLeftSideVelocity /= leftValues;
        mRightSideVelocity /= rightValues;
        mLeftSide.clear();
        mRightSide.clear();
        mUpdateCounter++;
        // If this is the first time called...
        if(mUpdateTime.ToMs() == 0)
        {
            mSamplePeriodSeconds = longestPeriod/1000.0;
        }
        else
        {
            mSamplePeriodSeconds = currentTime - mUpdateTime;
        }
        mUpdateTime = currentTime;
        return true;
    }

    return false;
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the linear velocity data calculated using the Update
///          method.
///
////////////////////////////////////////////////////////////////////////////////////
double RigidBody::GetLinearVelocity() const
{
    if(mUpdateCounter > 0)
    {
        return (mLeftSideDist + mRightSideDist)/(mSamplePeriodSeconds*2.0);
        //return (mLeftSideVelocity + mRightSideVelocity)/2.0;
    }
    else
    {
        return 0.0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Gets the rotational velocity data calculated using the Update
///          method.
///
////////////////////////////////////////////////////////////////////////////////////
double RigidBody::GetRotationalVelocity() const
{
    if(mUpdateCounter > 0)
    {
        return (mLeftSideVelocity - mRightSideVelocity)/mWidth;
        /*
        double v = (mLeftSideDist - mRightSideDist)/mWidth;
        if(v < -1 || v > 1.0)
        {
            // ERROR
            return 0.0;
        }
        return asin(v)/(mSamplePeriodSeconds);
        */
    }
    else
    {
        return 0.0;
    }
}


////////////////////////////////////////////////////////////////////////////////////
///
///   \brief Sets equal to.
///
////////////////////////////////////////////////////////////////////////////////////
RigidBody& RigidBody::operator =(const RigidBody& body)
{
    if(this != &body)
    {
        mDirtyFlag = body.mDirtyFlag;
        mLeftSide = body.mLeftSide;
        mRightSide = body.mRightSide;
        mWidth = body.mWidth;
        mUpdateCounter = body.mUpdateCounter;
        mLeftSideDist = body.mLeftSideDist;
        mRightSideDist = body.mRightSideDist;
        mLeftSideVelocity = body.mLeftSideVelocity;
        mRightSideVelocity = body.mRightSideVelocity;
        mSamplePeriodSeconds = body.mSamplePeriodSeconds;
    }
    return *this;
}

/* End of File */
